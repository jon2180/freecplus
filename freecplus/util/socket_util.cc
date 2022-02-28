#include "socket_util.h"

#include <arpa/inet.h>
#include <cstring>
#include <sys/select.h>
#include <unistd.h>

#include "file_util.h"

namespace freecplus {

bool TcpRead(const int sockfd, char *buffer, int *ibuflen, const int itimeout) {
  if (sockfd == -1)
    return false;

  if (itimeout > 0) {
    fd_set tmpfd;

    FD_ZERO(&tmpfd);
    FD_SET(sockfd, &tmpfd);

    struct timeval timeout;
    timeout.tv_sec = itimeout;
    timeout.tv_usec = 0;

    int i;
    if ((i = select(sockfd + 1, &tmpfd, 0, 0, &timeout)) <= 0)
      return false;
  }

  (*ibuflen) = 0;

  if (Readn(sockfd, (char *)ibuflen, 4) == false)
    return false;

  (*ibuflen) = ntohl(*ibuflen); // 把网络字节序转换为主机字节序。

  if (Readn(sockfd, buffer, (*ibuflen)) == false)
    return false;

  return true;
}

bool TcpWrite(const int sockfd, const char *buffer, const size_t ibuflen) {
  if (sockfd == -1)
    return false;

  fd_set tmpfd;

  FD_ZERO(&tmpfd);
  FD_SET(sockfd, &tmpfd);

  struct timeval timeout;
  timeout.tv_sec = 5;
  timeout.tv_usec = 0;

  if (select(sockfd + 1, 0, &tmpfd, 0, &timeout) <= 0)
    return false;

  size_t ilen = 0;

  // 如果长度为0，就采用字符串的长度
  if (ibuflen == 0)
    ilen = strlen(buffer);
  else
    ilen = ibuflen;

  int ilenn = htonl(ilen); // 转换为网络字节序。

  char strTBuffer[ilen + 4];
  memset(strTBuffer, 0, sizeof(strTBuffer));
  memcpy(strTBuffer, &ilenn, 4);
  memcpy(strTBuffer + 4, buffer, ilen);

  if (!Writen(sockfd, strTBuffer, ilen + 4))
    return false;

  return true;
}

bool Readn(const int sockfd, char *buffer, const size_t n) {
  int nLeft, nread, idx;

  nLeft = n;
  idx = 0;

  while (nLeft > 0) {
    if ((nread = recv(sockfd, buffer + idx, nLeft, 0)) <= 0)
      return false;

    idx += nread;
    nLeft -= nread;
  }

  return true;
}

bool Writen(const int sockfd, const char *buffer, const size_t n) {
  int nLeft, idx, nwritten;
  nLeft = n;
  idx = 0;
  while (nLeft > 0) {
    if ((nwritten = send(sockfd, buffer + idx, nLeft, 0)) <= 0)
      return false;

    nLeft -= nwritten;
    idx += nwritten;
  }

  return true;
}

// 把文件通过sockfd发送给对端
bool SendFile(int sockfd, struct st_fileinfo *stfileinfo, LogFile *logfile) {
  char strSendBuffer[301], strRecvBuffer[301];
  memset(strSendBuffer, 0, sizeof(strSendBuffer));

  snprintf(strSendBuffer, 300, "<filename>%s</filename><filesize>%d</filesize><mtime>%s</mtime>", stfileinfo->filename,
           stfileinfo->filesize, stfileinfo->mtime);

  if (!TcpWrite(sockfd, strSendBuffer)) {
    if (logfile != nullptr)
      logfile->Write("SendFile TcpWrite() failed.\n");
    return false;
  }

  int bytes = 0;
  int total_bytes = 0;
  int onread = 0;
  char buffer[1000];

  FILE *fp = nullptr;

  if ((fp = OpenFile(stfileinfo->filename, "rb")) == nullptr) {
    if (logfile != nullptr)
      logfile->Write("SendFile OpenFile(%s) failed.\n", stfileinfo->filename);
    return false;
  }

  while (true) {
    memset(buffer, 0, sizeof(buffer));

    if ((stfileinfo->filesize - total_bytes) > 1000)
      onread = 1000;
    else
      onread = stfileinfo->filesize - total_bytes;

    bytes = fread(buffer, 1, onread, fp);

    if (bytes > 0) {
      if (!Writen(sockfd, buffer, bytes)) {
        if (logfile != nullptr)
          logfile->Write("SendFile Writen() failed.\n");
        fclose(fp);
        fp = nullptr;
        return false;
      }
    }

    total_bytes = total_bytes + bytes;

    if ((int)total_bytes == stfileinfo->filesize)
      break;
  }

  fclose(fp);

  // 接收对端返回的确认报文
  int buflen = 0;
  memset(strRecvBuffer, 0, sizeof(strRecvBuffer));
  if (!TcpRead(sockfd, strRecvBuffer, &buflen)) {
    if (logfile != nullptr)
      logfile->Write("SendFile TcpRead() failed.\n");
    return false;
  }

  if (strcmp(strRecvBuffer, "ok") != 0)
    return false;

  return true;
}

// 接收通过socdfd发送过来的文件
bool RecvFile(int sockfd, struct st_fileinfo *stfileinfo, LogFile *logfile) {
  char strSendBuffer[301];

  char strfilenametmp[301];
  memset(strfilenametmp, 0, sizeof(strfilenametmp));
  sprintf(strfilenametmp, "%s.tmp", stfileinfo->filename);

  FILE *fp = nullptr;

  if ((fp = OpenFile(strfilenametmp, "wb")) == nullptr) // FOPEN可创建目录
  {
    if (logfile != nullptr)
      logfile->Write("RecvFile OpenFile %s failed.\n", strfilenametmp);
    return false;
  }

  int total_bytes = 0;
  int onread;
  char buffer[1000];

  while (true) {
    memset(buffer, 0, sizeof(buffer));

    if ((stfileinfo->filesize - total_bytes) > 1000)
      onread = 1000;
    else
      onread = stfileinfo->filesize - total_bytes;

    if (!Readn(sockfd, buffer, onread)) {
      if (logfile != nullptr)
        logfile->Write("RecvFile Readn() failed.\n");
      fclose(fp);
      fp = nullptr;
      return false;
    }

    fwrite(buffer, 1, onread, fp);

    total_bytes = total_bytes + onread;

    if ((int)total_bytes == stfileinfo->filesize)
      break;
  }

  fclose(fp);

  // 重置文件的时间
  SetFileModifyTime(strfilenametmp, stfileinfo->mtime);

  memset(strSendBuffer, 0, sizeof(strSendBuffer));
  if (RenameFile(strfilenametmp, stfileinfo->filename))
    strcpy(strSendBuffer, "ok");
  else
    strcpy(strSendBuffer, "failed");

  // 向对端返回响应内容
  if (!TcpWrite(sockfd, strSendBuffer)) {
    if (logfile != 0)
      logfile->Write("RecvFile TcpWrite() failed.\n");
    return false;
  }

  if (strcmp(strSendBuffer, "ok") != 0)
    return false;

  return true;
}

} // namespace freecplus
