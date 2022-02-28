#include "ftp_client.h"

#include <cstring>
#include <cstdio>

#include "base/ftplib.h"
#include "util/dir_util.h"
#include "util/time_util.h"
#include "util/file_util.h"

namespace freecplus {

Cftp::Cftp() {
  m_ftpconn = 0;

  initdata();

  FtpInit();

  m_connectfailed = false;
  m_loginfailed = false;
  m_optionfailed = false;
}

Cftp::~Cftp() { logout(); }

void Cftp::initdata() {
  m_size = 0;

  memset(m_mtime, 0, sizeof(m_mtime));
}

bool Cftp::login(const char *host, const char *username, const char *password, const int imode) {
  if (m_ftpconn != 0) {
    FtpQuit(m_ftpconn);
    m_ftpconn = 0;
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

bool Cftp::logout() {
  if (m_ftpconn == 0)
    return false;

  FtpQuit(m_ftpconn);

  m_ftpconn = 0;

  return true;
}

bool Cftp::get(const char *remotefilename, const char *localfilename, const bool bCheckMTime) {
  if (m_ftpconn == 0)
    return false;

  // 创建本地文件目录
  MKDIR(localfilename);

  char strlocalfilenametmp[301];
  memset(strlocalfilenametmp, 0, sizeof(strlocalfilenametmp));
  snprintf(strlocalfilenametmp, 300, "%s.tmp", localfilename);

  // 获取远程服务器的文件的时间
  if (mtime(remotefilename) == false)
    return false;

  // 取文件
  if (FtpGet(strlocalfilenametmp, remotefilename, FTPLIB_IMAGE, m_ftpconn) == false)
    return false;

  // 判断文件获取前和获取后的时间，如果时间不同，表示文件已改变，返回失败
  if (bCheckMTime == true) {
    char strmtime[21];
    strcpy(strmtime, m_mtime);

    if (mtime(remotefilename) == false)
      return false;

    if (strcmp(m_mtime, strmtime) != 0)
      return false;
  }

  // 重置文件时间
  UTime(strlocalfilenametmp, m_mtime);

  // 改为正式的文件
  if (rename(strlocalfilenametmp, localfilename) != 0)
    return false;

  // 获取文件的大小
  m_size = FileSize(localfilename);

  return true;
}

bool Cftp::mtime(const char *remotefilename) {
  if (m_ftpconn == 0)
    return false;

  memset(m_mtime, 0, sizeof(m_mtime));

  char strmtime[21];
  memset(strmtime, 0, sizeof(strmtime));

  if (FtpModDate(remotefilename, strmtime, 14, m_ftpconn) == false)
    return false;

  AddTime(strmtime, m_mtime, 0 + 8 * 60 * 60, "yyyymmddhh24miss");

  return true;
}

bool Cftp::size(const char *remotefilename) {
  if (m_ftpconn == 0)
    return false;

  m_size = 0;

  if (FtpSize(remotefilename, &m_size, FTPLIB_IMAGE, m_ftpconn) == false)
    return false;

  return true;
}

bool Cftp::site(const char *command) {
  if (m_ftpconn == 0)
    return false;

  if (FtpSite(command, m_ftpconn) == false)
    return false;

  return true;
}

bool Cftp::chdir(const char *remotedir) {
  if (m_ftpconn == 0)
    return false;

  if (FtpChdir(remotedir, m_ftpconn) == false)
    return false;

  return true;
}

bool Cftp::mkdir(const char *remotedir) {
  if (m_ftpconn == 0)
    return false;

  if (FtpMkdir(remotedir, m_ftpconn) == false)
    return false;

  return true;
}

bool Cftp::rmdir(const char *remotedir) {
  if (m_ftpconn == 0)
    return false;

  if (FtpRmdir(remotedir, m_ftpconn) == false)
    return false;

  return true;
}

bool Cftp::dir(const char *remotedir, const char *listfilename) {
  if (m_ftpconn == 0)
    return false;

  if (FtpDir(listfilename, remotedir, m_ftpconn) == false)
    return false;

  return true;
}

bool Cftp::nlist(const char *remotedir, const char *listfilename) {
  if (m_ftpconn == 0)
    return false;

  // 创建本地文件目录
  MKDIR(listfilename);

  if (FtpNlst(listfilename, remotedir, m_ftpconn) == false)
    return false;

  return true;
}

bool Cftp::put(const char *localfilename, const char *remotefilename, const bool bCheckSize) {
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
  if (bCheckSize == true) {
    if (size(remotefilename) == false)
      return false;

    if (m_size != FileSize(localfilename))
      return false;
  }

  return true;
}

bool Cftp::ftpdelete(const char *remotefilename) {
  if (m_ftpconn == 0)
    return false;

  if (FtpDelete(remotefilename, m_ftpconn) == false)
    return false;

  return true;
}

bool Cftp::ftprename(const char *srcremotefilename, const char *dstremotefilename) {
  if (m_ftpconn == 0)
    return false;

  if (FtpRename(srcremotefilename, dstremotefilename, m_ftpconn) == false)
    return false;

  return true;
}

char *Cftp::response() {
  if (m_ftpconn == 0)
    return 0;

  return FtpLastResponse(m_ftpconn);
}

} // namespace freecplus
