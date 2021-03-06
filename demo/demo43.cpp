/*
 *  程序名：demo43.cpp，此程序演示freecplus框架的CLogFile类的日志文件的切换。
 *  作者：C语言技术网(www.freecplus.net) 日期：20190525
 */
#include <cstdio>

#include "freecplus.h"

int main() {
  freecplus::LogFile logfile;

  // 打开日志文件，如果"/tmp/log"不存在，就创建它，但是要确保当前用户具备创建目录的权限。
  if (logfile.Open("/tmp/log/demo43.log") == false) {
    printf("logfile.Open(/tmp/log/demo43.log) failed.\n");
    return -1;
  }

  logfile.Write("demo43程序开始运行。\n");

  // 让程序循环10000000，生成足够大的日志。
  for (int ii = 0; ii < 10000000; ii++) {
    logfile.Write("本程序演示日志文件的切换，这是第%010%d条记录。\n", ii);
  }

  logfile.Write("demo43程序运行结束。\n");
}
