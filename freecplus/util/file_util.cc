#include "file_util.h"

#include <cstring> // strncpy strlen
#include <fcntl.h> // open
#include <sys/stat.h>
#include <unistd.h>    // access
#include <utime.h>     // utimbuf

#include "dir_util.h"
#include "time_util.h"

namespace freecplus {

bool RemoveFile(const char *filename, const int times) {
  // 如果文件不存在，直接返回失败
  if (access(filename, R_OK) != 0)
    return false;

  for (int ii = 0; ii < times; ii++) {
    if (remove(filename) == 0)
      return true;
    usleep(100000);
  }

  return false;
}

bool RenameFile(const char *src_filename, const char *dst_filename, const int times) {
  // 如果文件不存在，直接返回失败
  if (access(src_filename, R_OK) != 0)
    return false;

  if (!MakeDir(dst_filename))
    return false;

  for (int ii = 0; ii < times; ii++) {
    if (rename(src_filename, dst_filename) == 0)
      return true;
    usleep(100000);
  }

  return false;
}

bool CopyFile(const char *src_filename, const char *dst_filename) {
  if (!MakeDir(dst_filename))
    return false;

  char str_dst_filename_tmp[301];
  memset(str_dst_filename_tmp, 0, sizeof(str_dst_filename_tmp));
  snprintf(str_dst_filename_tmp, 300, "%s.tmp", dst_filename);

  int srcfd, dstfd;

  srcfd = dstfd = -1;

  size_t file_size = GetFileSize(src_filename);

  size_t bytes = 0;
  size_t total_bytes = 0;
  size_t onread = 0;
  char buffer[5000];

  if ((srcfd = open(src_filename, O_RDONLY)) < 0)
    return false;

  if ((dstfd = open(str_dst_filename_tmp, O_WRONLY | O_CREAT | O_TRUNC, S_IWUSR | S_IRUSR | S_IXUSR)) < 0) {
    close(srcfd);
    return false;
  }

  while (true) {
    memset(buffer, 0, sizeof(buffer));

    if ((file_size - total_bytes) > 5000)
      onread = 5000;
    else
      onread = file_size - total_bytes;

    bytes = read(srcfd, buffer, onread);

    if (bytes > 0)
      write(dstfd, buffer, bytes);

    total_bytes = total_bytes + bytes;

    if (total_bytes == file_size)
      break;
  }

  close(srcfd);

  close(dstfd);

  // 更改文件的修改时间属性
  char str_m_time[21];
  memset(str_m_time, 0, sizeof(str_m_time));
  GetFileModifyTime(src_filename, str_m_time);
  SetFileModifyTime(str_dst_filename_tmp, str_m_time);

  if (!RenameFile(str_dst_filename_tmp, dst_filename)) {
	RemoveFile(str_dst_filename_tmp);
    return false;
  }

  return true;
}

size_t GetFileSize(const char *filename) {
  struct stat st_filestat;

  if (stat(filename, &st_filestat) < 0)
    return -1;

  return st_filestat.st_size;
}

bool GetFileModifyTime(const char *filename, char *mtime, const char *fmt) {
  // 判断文件是否存在。
  struct stat st_filestat;

  if (stat(filename, &st_filestat) < 0)
    return false;

  char str_fmt[25];
  memset(str_fmt, 0, sizeof(str_fmt));
  if (fmt == 0)
    strcpy(str_fmt, "yyyymmddhh24miss");
  else
    strcpy(str_fmt, fmt);

  timetostr(st_filestat.st_mtime, mtime, str_fmt);
  return true;
}


bool SetFileModifyTime(const char *filename, const char *mtime) {
  struct utimbuf stutimbuf;

  stutimbuf.actime = stutimbuf.modtime = strtotime(mtime);

  if (utime(filename, &stutimbuf) != 0)
    return false;

  return true;
}

FILE *OpenFile(const char *filename, const char *mode) {
  if (!MakeDir(filename))
    return nullptr;

  return fopen(filename, mode);
}

bool GetLineFromFile(const FILE *fp, char *buffer, const int read_size, const char *endbz) {
  if (fp == nullptr)
    return false;

  memset(buffer, 0, read_size + 1); // 调用者必须保证buffer的空间足够，否则这里会内存溢出。

  char strline[read_size + 1];

  while (true) {
    memset(strline, 0, sizeof(strline));

    if (fgets(strline, read_size, (FILE *)fp) == 0)
      break;

    // 防止buffer溢出
    if ((strlen(buffer) + strlen(strline)) >= (unsigned int)read_size)
      break;

    strcat(buffer, strline);

    if (endbz == nullptr)
      return true;
    else if (strstr(strline, endbz) != nullptr)
      return true;
  }

  return false;
}

} // namespace freecplus
