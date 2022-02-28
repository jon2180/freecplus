#include "file_util.h"

#include <cstring> // strncpy strlen
#include <fcntl.h> // open
#include <sys/stat.h>
#include <sys/types.h> // mkdir
#include <unistd.h>    // access
#include <utime.h>     // utimbuf

#include "dir_util.h"
#include "time_util.h"

namespace freecplus {

// 删除目录中的文件，类似Linux系统的rm命令。
// filename：待删除的文件名，建议采用绝对路径的文件名，例如/tmp/root/data.xml。
// times：执行删除文件的次数，缺省是1，建议不要超过3，从实际应用的经验看来，如果删除文件第1次不成功，再尝试
// 2次是可以的，更多就意义不大了。还有，如果执行删除失败，usleep(100000)后再重试。
// 返回值：true-删除成功；false-删除失败，失败的主要原因是权限不足。
// 在应用开发中，可以用REMOVE函数代替remove库函数。
bool REMOVE(const char *filename, const int times) {
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

// 把文件重命名，类似Linux系统的mv命令。
// srcfilename：原文件名，建议采用绝对路径的文件名。
// destfilename：目标文件名，建议采用绝对路径的文件名。
// times：执行重命名文件的次数，缺省是1，建议不要超过3，从实际应用的经验看来，如果重命名文件第1次不成功，再尝
// 试2次是可以的，更多次就意义不大了。还有，如果执行重命名失败，usleep(100000)后再重试。
// 返回值：true-重命名成功；false-重命名失败，失败的主要原因是权限不足或磁盘空间不够，如果原文件和目标文件不
// 在同一个磁盘分区，重命名也可能失败。
// 注意，在重命名文件之前，会自动创建destfilename参数中的目录名。
// 在应用开发中，可以用RENAME函数代替rename库函数。
bool RENAME(const char *srcfilename, const char *dstfilename, const int times) {
  // 如果文件不存在，直接返回失败
  if (access(srcfilename, R_OK) != 0)
    return false;

  if (MKDIR(dstfilename) == false)
    return false;

  for (int ii = 0; ii < times; ii++) {
    if (rename(srcfilename, dstfilename) == 0)
      return true;

    usleep(100000);
  }

  return false;
}

// 复制文件，类似Linux系统的cp命令。
// srcfilename：原文件名，建议采用绝对路径的文件名。
// destfilename：目标文件名，建议采用绝对路径的文件名。
// 返回值：true-复制成功；false-复制失败，失败的主要原因是权限不足或磁盘空间不够。
// 注意：
// 1）在复制名文件之前，会自动创建destfilename参数中的目录名。
// 2）复制文件的过程中，采用临时文件命名的方法，复制完成后再改名为destfilename，避免中间状态的文件被读取。
// 3）复制后的文件的时间与原文件相同，这一点与Linux系统cp命令不同。
bool COPY(const char *srcfilename, const char *dstfilename) {
  if (MKDIR(dstfilename) == false)
    return false;

  char strdstfilenametmp[301];
  memset(strdstfilenametmp, 0, sizeof(strdstfilenametmp));
  snprintf(strdstfilenametmp, 300, "%s.tmp", dstfilename);

  int srcfd, dstfd;

  srcfd = dstfd = -1;

  int iFileSize = FileSize(srcfilename);

  int bytes = 0;
  int total_bytes = 0;
  int onread = 0;
  char buffer[5000];

  if ((srcfd = open(srcfilename, O_RDONLY)) < 0)
    return false;

  if ((dstfd = open(strdstfilenametmp, O_WRONLY | O_CREAT | O_TRUNC, S_IWUSR | S_IRUSR | S_IXUSR)) < 0) {
    close(srcfd);
    return false;
  }

  while (true) {
    memset(buffer, 0, sizeof(buffer));

    if ((iFileSize - total_bytes) > 5000)
      onread = 5000;
    else
      onread = iFileSize - total_bytes;

    bytes = read(srcfd, buffer, onread);

    if (bytes > 0)
      write(dstfd, buffer, bytes);

    total_bytes = total_bytes + bytes;

    if (total_bytes == iFileSize)
      break;
  }

  close(srcfd);

  close(dstfd);

  // 更改文件的修改时间属性
  char strmtime[21];
  memset(strmtime, 0, sizeof(strmtime));
  FileMTime(srcfilename, strmtime);
  UTime(strdstfilenametmp, strmtime);

  if (RENAME(strdstfilenametmp, dstfilename) == false) {
    REMOVE(strdstfilenametmp);
    return false;
  }

  return true;
}

// 获取文件的大小。
// filename：待获取的文件名，建议采用绝对路径的文件名。
// 返回值：如果文件不存在或没有访问权限，返回-1，成功返回文件的大小，单位是字节。
int FileSize(const char *filename) {
  struct stat st_filestat;

  if (stat(filename, &st_filestat) < 0)
    return -1;

  return st_filestat.st_size;
}

// 获取文件的时间。
// filename：待获取的文件名，建议采用绝对路径的文件名。
// mtime：用于存放文件的时间，即stat结构体的st_mtime。
// fmt：设置时间的输出格式，与LocalTime函数相同，但缺省是"yyyymmddhh24miss"。
// 返回值：如果文件不存在或没有访问权限，返回false，成功返回true。
bool FileMTime(const char *filename, char *mtime, const char *fmt) {
  // 判断文件是否存在。
  struct stat st_filestat;

  if (stat(filename, &st_filestat) < 0)
    return false;

  char strfmt[25];
  memset(strfmt, 0, sizeof(strfmt));
  if (fmt == 0)
    strcpy(strfmt, "yyyymmddhh24miss");
  else
    strcpy(strfmt, fmt);

  timetostr(st_filestat.st_mtime, mtime, strfmt);

  return true;
}

// 重置文件的修改时间属性。
// filename：待重置的文件名，建议采用绝对路径的文件名。
// stime：字符串表示的时间，格式不限，但一定要包括yyyymmddhh24miss，一个都不能少。
// 返回值：true-成功；false-失败，失败的原因保存在errno中。
bool UTime(const char *filename, const char *mtime) {
  struct utimbuf stutimbuf;

  stutimbuf.actime = stutimbuf.modtime = strtotime(mtime);

  if (utime(filename, &stutimbuf) != 0)
    return false;

  return true;
}

// 打开文件。
// FOPEN函数调用fopen库函数打开文件，如果文件名中包含的目录不存在，就创建目录。
// FOPEN函数的参数和返回值与fopen函数完全相同。
// 在应用开发中，用FOPEN函数代替fopen库函数。
FILE *FOPEN(const char *filename, const char *mode) {
  if (MKDIR(filename) == false)
    return 0;

  return fopen(filename, mode);
}

// 从文本文件中读取一行。
// fp：已打开的文件指针。
// buffer：用于存放读取的内容。
// readsize：本次打算读取的字节数，如果已经读取到了结束标志，函数返回。
// endbz：行内容结束的标志，缺省为空，表示行内容以"\n"为结束标志。
// 返回值：true-成功；false-失败，一般情况下，失败可以认为是文件已结束。
bool FGETS(const FILE *fp, char *buffer, const int readsize, const char *endbz) {
  if (fp == 0)
    return false;

  memset(buffer, 0, readsize + 1); // 调用者必须保证buffer的空间足够，否则这里会内存溢出。

  char strline[readsize + 1];

  while (true) {
    memset(strline, 0, sizeof(strline));

    if (fgets(strline, readsize, (FILE *)fp) == 0)
      break;

    // 防止buffer溢出
    if ((strlen(buffer) + strlen(strline)) >= (unsigned int)readsize)
      break;

    strcat(buffer, strline);

    if (endbz == 0)
      return true;
    else if (strstr(strline, endbz) != 0)
      return true;
  }

  return false;
}

} // namespace freecplus
