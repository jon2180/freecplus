#include "dir.h"

#include <cstdio>
#include <cstring>
#include <dirent.h>
#include <sys/stat.h> // DIR, stat

#include "util/dir_util.h"
#include "util/string_util.h"

namespace freecplus {

Dir::Dir() {
  m_pos = 0;

  memset(m_DateFMT, 0, sizeof(m_DateFMT));
  strcpy(m_DateFMT, "yyyy-mm-dd hh24:mi:ss");

  m_vFileName.clear();

  InitData();
}

void Dir::InitData() {
  memset(m_DirName, 0, sizeof(m_DirName));
  memset(m_FileName, 0, sizeof(m_FileName));
  memset(m_FullFileName, 0, sizeof(m_FullFileName));
  m_FileSize = 0;
  memset(m_CreateTime, 0, sizeof(m_CreateTime));
  memset(m_ModifyTime, 0, sizeof(m_ModifyTime));
  memset(m_AccessTime, 0, sizeof(m_AccessTime));
}

// 设置文件时间的格式，支持"yyyy-mm-dd hh24:mi:ss"和"yyyymmddhh24miss"两种，缺省是前者。
void Dir::SetDateFMT(const char *in_DateFMT) {
  memset(m_DateFMT, 0, sizeof(m_DateFMT));
  strcpy(m_DateFMT, in_DateFMT);
}

// 打开目录，获取目录中的文件列表信息，存放于m_vFileName容器中。
// in_DirName，待打开的目录名。
// in_MatchStr，待获取文件名的匹配规则，不匹配的文件被忽略。
// in_MaxCount，获取文件的最大数量，缺省值为10000个。
// bAndChild，是否打开各级子目录，缺省值为false-不打开子目录。
// bSort，是否对获取到的文件列表（即m_vFileName容器中的内容）进行排序，缺省值为false-不排序。
// 返回值：如果in_DirName参数指定的目录不存在，OpenDir方法会创建该目录，如果创建失败，返回false，还有，如果当前用户对in_DirName目录下的子目录没有读取权限也会返回false，其它正常情况下都会返回true。
bool Dir::OpenDir(const char *in_DirName, const char *in_MatchStr, const unsigned int in_MaxCount,
				  const bool bAndChild, bool bSort) {
  m_pos = 0;
  m_vFileName.clear();

  // 如果目录不存在，就创建该目录
  if (MakeDir(in_DirName, false) == false)
    return false;

  bool bRet = _OpenDir(in_DirName, in_MatchStr, in_MaxCount, bAndChild);

  if (bSort == true) {
    sort(m_vFileName.begin(), m_vFileName.end());
  }

  return bRet;
}

// 这是一个递归函数，用于OpenDir()的调用，在CDir类的外部不需要调用它。
bool Dir::_OpenDir(const char *in_DirName, const char *in_MatchStr, const unsigned int in_MaxCount,
				   const bool bAndChild) {
  DIR *dir;

  if ((dir = opendir(in_DirName)) == 0)
    return false;

  char strTempFileName[1024];

  struct dirent *st_fileinfo;
  struct stat st_filestat;

  while ((st_fileinfo = readdir(dir)) != 0) {
    // 以"."打头的文件不处理
    if (st_fileinfo->d_name[0] == '.')
      continue;

    memset(strTempFileName, 0, sizeof(strTempFileName));

    snprintf(strTempFileName, 300, "%s//%s", in_DirName, st_fileinfo->d_name);

    UpdateStr(strTempFileName, "//", "/");

    stat(strTempFileName, &st_filestat);

    // 判断是否是目录
    if (S_ISDIR(st_filestat.st_mode)) {
      if (bAndChild == true) {
        if (_OpenDir(strTempFileName, in_MatchStr, in_MaxCount, bAndChild) == false) {
          closedir(dir);
          return false;
        }
      }
    } else {
      if (MatchFileName(st_fileinfo->d_name, in_MatchStr) == false)
        continue;

      m_vFileName.push_back(strTempFileName);

      if (m_vFileName.size() >= in_MaxCount)
        break;
    }
  }

  closedir(dir);

  return true;
}

/*
st_gid
  Numeric identifier of group that owns file (UNIX-specific) This field will always be zero on NT systems. A redirected
file is classified as an NT file. st_atime Time of last access of file. st_ctime Time of creation of file. st_dev Drive
number of the disk containing the file (same as st_rdev). st_ino Number of the information node (the inode) for the file
(UNIX-specific). On UNIX file systems, the inode describes the file date and time stamps, permissions, and content. When
files are hard-linked to one another, they share the same inode. The inode, and therefore st_ino, has no meaning in the
FAT, HPFS, or NTFS file systems. st_mode Bit mask for file-mode information. The _S_IFDIR bit is set if path specifies a
directory; the _S_IFREG bit is set if path specifies an ordinary file or a device. User read/write bits are set
according to the file’s permission mode; user execute bits are set according to the filename extension. st_mtime Time of
last modification of file. st_nlink Always 1 on non-NTFS file systems. st_rdev Drive number of the disk containing the
file (same as st_dev). st_size Size of the file in bytes; a 64-bit integer for _stati64 and _wstati64 st_uid Numeric
identifier of user who owns file (UNIX-specific). This field will always be zero on NT systems. A redirected file is
classified as an NT file.
*/

// 从m_vFileName容器中获取一条记录（文件名），同时得到该文件的大小、修改时间等信息。
// 调用OpenDir方法时，m_vFileName容器被清空，m_pos归零，每调用一次ReadDir方法m_pos加1。
// 当m_pos小于m_vFileName.size()，返回true，否则返回false。
bool Dir::ReadDir() {
  InitData();

  int ivsize = m_vFileName.size();

  // 如果已读完，清空容器
  if (m_pos >= ivsize) {
    m_pos = 0;
    m_vFileName.clear();
    return false;
  }

  int pos = 0;

  pos = m_vFileName[m_pos].find_last_of("/");

  // 目录名
  memset(m_DirName, 0, sizeof(m_DirName));
  strcpy(m_DirName, m_vFileName[m_pos].substr(0, pos).c_str());

  // 文件名
  memset(m_FileName, 0, sizeof(m_FileName));
  strcpy(m_FileName, m_vFileName[m_pos].substr(pos + 1, m_vFileName[m_pos].size() - pos - 1).c_str());

  // 文件全名，包括路径
  snprintf(m_FullFileName, 300, "%s", m_vFileName[m_pos].c_str());

  struct stat st_filestat;

  stat(m_FullFileName, &st_filestat);

  m_FileSize = st_filestat.st_size;

  struct tm nowtimer;

  if (strcmp(m_DateFMT, "yyyy-mm-dd hh24:mi:ss") == 0) {
    nowtimer = *localtime(&st_filestat.st_mtime);
    nowtimer.tm_mon++;
    snprintf(m_ModifyTime, 20, "%04u-%02u-%02u %02u:%02u:%02u", nowtimer.tm_year + 1900, nowtimer.tm_mon,
             nowtimer.tm_mday, nowtimer.tm_hour, nowtimer.tm_min, nowtimer.tm_sec);

    nowtimer = *localtime(&st_filestat.st_ctime);
    nowtimer.tm_mon++;
    snprintf(m_CreateTime, 20, "%04u-%02u-%02u %02u:%02u:%02u", nowtimer.tm_year + 1900, nowtimer.tm_mon,
             nowtimer.tm_mday, nowtimer.tm_hour, nowtimer.tm_min, nowtimer.tm_sec);

    nowtimer = *localtime(&st_filestat.st_atime);
    nowtimer.tm_mon++;
    snprintf(m_AccessTime, 20, "%04u-%02u-%02u %02u:%02u:%02u", nowtimer.tm_year + 1900, nowtimer.tm_mon,
             nowtimer.tm_mday, nowtimer.tm_hour, nowtimer.tm_min, nowtimer.tm_sec);
  }

  if (strcmp(m_DateFMT, "yyyymmddhh24miss") == 0) {
    nowtimer = *localtime(&st_filestat.st_mtime);
    nowtimer.tm_mon++;
    snprintf(m_ModifyTime, 20, "%04u%02u%02u%02u%02u%02u", nowtimer.tm_year + 1900, nowtimer.tm_mon, nowtimer.tm_mday,
             nowtimer.tm_hour, nowtimer.tm_min, nowtimer.tm_sec);

    nowtimer = *localtime(&st_filestat.st_ctime);
    nowtimer.tm_mon++;
    snprintf(m_CreateTime, 20, "%04u%02u%02u%02u%02u%02u", nowtimer.tm_year + 1900, nowtimer.tm_mon, nowtimer.tm_mday,
             nowtimer.tm_hour, nowtimer.tm_min, nowtimer.tm_sec);

    nowtimer = *localtime(&st_filestat.st_atime);
    nowtimer.tm_mon++;
    snprintf(m_AccessTime, 20, "%04u%02u%02u%02u%02u%02u", nowtimer.tm_year + 1900, nowtimer.tm_mon, nowtimer.tm_mday,
             nowtimer.tm_hour, nowtimer.tm_min, nowtimer.tm_sec);
  }

  m_pos++;

  return true;
}

Dir::~Dir() {
  m_vFileName.clear();

  // m_vDirName.clear();
}

} // namespace freecplus
