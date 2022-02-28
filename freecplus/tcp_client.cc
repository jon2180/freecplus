#include "tcp_client.h"

#include <arpa/inet.h>
#include <cstring>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>

#include "util/socket_util.h"

namespace freecplus {

CTcpClient::CTcpClient() {
  m_sockfd = -1;
  memset(m_ip, 0, sizeof(m_ip));
  m_port = 0;
  m_btimeout = false;
}

bool CTcpClient::ConnectToServer(const char *ip, const int port) {
  if (m_sockfd != -1) {
    close(m_sockfd);
    m_sockfd = -1;
  }

  strcpy(m_ip, ip);
  m_port = port;

  struct hostent *h;
  struct sockaddr_in servaddr;

  if ((m_sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    return false;

  if (!(h = gethostbyname(m_ip))) {
    close(m_sockfd);
    m_sockfd = -1;
    return false;
  }

  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(m_port); // 指定服务端的通讯端口
  memcpy(&servaddr.sin_addr, h->h_addr, h->h_length);

  if (connect(m_sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0) {
    close(m_sockfd);
    m_sockfd = -1;
    return false;
  }

  return true;
}

bool CTcpClient::Read(char *buffer, const int itimeout) {
  if (m_sockfd == -1)
    return false;

  if (itimeout > 0) {
    fd_set tmpfd;

    FD_ZERO(&tmpfd);
    FD_SET(m_sockfd, &tmpfd);

    struct timeval timeout;
    timeout.tv_sec = itimeout;
    timeout.tv_usec = 0;

    m_btimeout = false;

    int i;
    if ((i = select(m_sockfd + 1, &tmpfd, 0, 0, &timeout)) <= 0) {
      if (i == 0)
        m_btimeout = true;
      return false;
    }
  }

  m_buflen = 0;
  return (TcpRead(m_sockfd, buffer, &m_buflen));
}

bool CTcpClient::Write(const char *buffer, const int ibuflen) {
  if (m_sockfd == -1)
    return false;

  fd_set tmpfd;

  FD_ZERO(&tmpfd);
  FD_SET(m_sockfd, &tmpfd);

  struct timeval timeout;
  timeout.tv_sec = 5;
  timeout.tv_usec = 0;

  m_btimeout = false;

  int i;
  if ((i = select(m_sockfd + 1, 0, &tmpfd, 0, &timeout)) <= 0) {
    if (i == 0)
      m_btimeout = true;
    return false;
  }

  int ilen = ibuflen;

  if (ibuflen == 0)
    ilen = strlen(buffer);

  return (TcpWrite(m_sockfd, buffer, ilen));
}

void CTcpClient::Close() {
  if (m_sockfd > 0)
    close(m_sockfd);

  m_sockfd = -1;
  memset(m_ip, 0, sizeof(m_ip));
  m_port = 0;
  m_btimeout = false;
}

CTcpClient::~CTcpClient() { Close(); }

} // namespace freecplus