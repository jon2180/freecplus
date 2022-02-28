/*
 *  程序名：demo34.cpp，此程序演示freecplus框架的文件操作函数的用法
 *  作者：C语言技术网(www.freecplus.net) 日期：20190525
 */
#include "freecplus.h"

#include <cstring>

int main() {
  // 删除文件。
  if (freecplus::RemoveFile("/tmp/root/_freecplus.h") == false) {
    printf("RemoveFile(/tmp/root/_freecplus.h) %d:%s\n", errno, strerror(errno));
  }

  // 重命名文件。
  if (freecplus::RenameFile("/tmp/root/freecplus.cpp", "/tmp/root/aaa/bbb/ccc/freecplus.cpp") == false) {
    printf("RenameFile(/tmp/root/freecplus.cpp) %d:%s\n", errno, strerror(errno));
  }

  // 复制文件。
  if (freecplus::CopyFile("/freecplus/_freecplus.h", "/tmp/root/aaa/bbb/ccc/_freecplus.h") == false) {
    printf("CopyFile(/freecplus/_freecplus.h) %d:%s\n", errno, strerror(errno));
  }

  // 获取文件的大小。
  printf("size=%d\n", freecplus::GetFileSize("/freecplus/_freecplus.h"));

  // 重置文件的时间。
  freecplus::SetFileModifyTime("/freecplus/_freecplus.h", "2020-01-05 13:37:29");

  // 获取文件的时间。
  char mtime[21];
  memset(mtime, 0, sizeof(mtime));
  freecplus::GetFileModifyTime("/freecplus/_freecplus.h", mtime, "yyyy-mm-dd hh24:mi:ss");
  printf("mtime=%s\n", mtime); // 输出mtime=2020-01-05 13:37:29
}
