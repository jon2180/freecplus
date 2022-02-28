#include "ftp_client.h"

#include <cstring>
#include <cstdio>

#include "base/ftplib.h"
#include "util/dir_util.h"
#include "util/time_util.h"
#include "util/file_util.h"

namespace freecplus {

FtpClient::FtpClient() {
  m_ftpconn = nullptr;

  initdata();

  FtpInit();

  m_connectfailed = false;
  m_loginfailed = false;
  m_optionfailed = false;
}

FtpClient::~FtpClient() { logout(); }

void FtpClient::initdata() {
  m_size = 0;

  memset(m_mtime, 0, sizeof(m_mtime));
}

bool FtpClient::login(const char *host, const char *username, const char *password, const int imode) {
  if (m_ftpconn != nullptr) {
    FtpQuit(m_ftpconn);
    m_ftpconn = nullptr;
  }

  m_connectfailed = m_loginfailed = m_optionfailed = false;

  if (FtpConnect(host, &m_ftpconn) == false) {
    m_connectfailed = true;
    return false;
  }

  if (FtpLogin(username, password, m_ftpconn) == false) {
    m_loginfailed = true;
    return false;
  }

  if (FtpOptions(FTPLIB_CONNMODE, (long)imode, m_ftpconn) == false) {
    m_optionfailed = true;
    return false;
  }

  return true;
}

bool FtpClient::logout() {
  if (m_ftpconn == nullptr)
    return false;

  FtpQuit(m_ftpconn);

  m_ftpconn = nullptr;

  return true;
}

bool FtpClient::get(const char *remotefilename, const char *localfilename, const bool bCheckMTime) {
  if (m_ftpconn == nullptr)
    return false;

  // 创建本地文件目录
  MakeDir(localfilename);

  char strlocalfilenametmp[301];
  memset(strlocalfilenametmp, 0, sizeof(strlocalfilenametmp));
  snprintf(strlocalfilenametmp, 300, "%s.tmp", localfilename);

  // 获取远程服务器的文件的时间
  if (!mtime(remotefilename))
    return false;

  // 取文件
  if (FtpGet(strlocalfilenametmp, remotefilename, FTPLIB_IMAGE, m_ftpconn) == false)
    return false;

  // 判断文件获取前和获取后的时间，如果时间不同，表示文件已改变，返回失败
  if (bCheckMTime) {
    char strmtime[21];
    strcpy(strmtime, m_mtime);

    if (!mtime(remotefilename))
      return false;

    if (strcmp(m_mtime, strmtime) != 0)
      return false;
  }

  // 重置文件时间
  SetFileModifyTime(strlocalfilenametmp, m_mtime);

  // 改为正式的文件
  if (rename(strlocalfilenametmp, localfilename) != 0)
    return false;

  // 获取文件的大小
  m_size = GetFileSize(localfilename);

  return true;
}

bool FtpClient::mtime(const char *remotefilename) {
  if (m_ftpconn == nullptr)
    return false;

  memset(m_mtime, 0, sizeof(m_mtime));

  char strmtime[21];
  memset(strmtime, 0, sizeof(strmtime));

  if (FtpModDate(remotefilename, strmtime, 14, m_ftpconn) == false)
    return false;

  AddTime(strmtime, m_mtime, 0 + 8 * 60 * 60, "yyyymmddhh24miss");

  return true;
}

bool FtpClient::size(const char *remotefilename) {
  if (m_ftpconn == nullptr)
    return false;

  m_size = 0;

  if (FtpSize(remotefilename, &m_size, FTPLIB_IMAGE, m_ftpconn) == false)
    return false;

  return true;
}

bool FtpClient::site(const char *command) const {
  if (m_ftpconn == nullptr)
    return false;

  if (FtpSite(command, m_ftpconn) == false)
    return false;

  return true;
}

bool FtpClient::chdir(const char *remotedir) const {
  if (m_ftpconn == nullptr)
    return false;

  if (FtpChdir(remotedir, m_ftpconn) == false)
    return false;

  return true;
}

bool FtpClient::mkdir(const char *remotedir) const {
  if (m_ftpconn == nullptr)
    return false;

  if (FtpMkdir(remotedir, m_ftpconn) == false)
    return false;

  return true;
}

bool FtpClient::rmdir(const char *remotedir) const {
  if (m_ftpconn == nullptr)
    return false;

  if (FtpRmdir(remotedir, m_ftpconn) == false)
    return false;

  return true;
}

bool FtpClient::dir(const char *remotedir, const char *listfilename) const {
  if (m_ftpconn == nullptr)
    return false;

  if (FtpDir(listfilename, remotedir, m_ftpconn) == false)
    return false;

  return true;
}

bool FtpClient::nlist(const char *remotedir, const char *listfilename) const {
  if (m_ftpconn == nullptr)
    return false;

  // 创建本地文件目录
  MakeDir(listfilename);

  if (FtpNlst(listfilename, remotedir, m_ftpconn) == false)
    return false;

  return true;
}

bool FtpClient::put(const char *localfilename, const char *remotefilename, const bool bCheckSize) {
  if (m_ftpconn == 0)
    return false;

  char strremotefilenametmp[301];
  memset(strremotefilenametmp, 0, sizeof(strremotefilenametmp));
  snprintf(strremotefilenametmp, 300, "%s.tmp", remotefilename);

  if (FtpPut(localfilename, strremotefilenametmp, FTPLIB_IMAGE, m_ftpconn) == false)
    return false;

  if (FtpRename(strremotefilenametmp, remotefilename, m_ftpconn) == false)
    return false;

  // 判断已上传的文件的大小与本地文件是否相同，确保上传成功。
  if (bCheckSize) {
    if (!size(remotefilename))
      return false;

    if (m_size != GetFileSize(localfilename))
      return false;
  }

  return true;
}

bool FtpClient::ftpdelete(const char *remotefilename) const {
  if (m_ftpconn == nullptr)
    return false;

  if (FtpDelete(remotefilename, m_ftpconn) == false)
    return false;

  return true;
}

bool FtpClient::ftprename(const char *srcremotefilename, const char *dstremotefilename) const {
  if (m_ftpconn == nullptr)
    return false;

  if (FtpRename(srcremotefilename, dstremotefilename, m_ftpconn) == false)
    return false;

  return true;
}

char *FtpClient::response() const {
  if (m_ftpconn == nullptr)
    return nullptr;

  return FtpLastResponse(m_ftpconn);
}

} // namespace freecplus
