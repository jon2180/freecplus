/*
 *  程序名：demo24.cpp，此程序演示freecplus框架中LocalTime时间函数的使用。
 *  作者：C语言技术网(www.freecplus.net) 日期：20190525
 */
#include "freecplus.h"

#include <cstring>
#include <ctime>

int main() {
  char strtime[20];
  memset(strtime, 0, sizeof(strtime));

  freecplus::LocalTime(strtime, "yyyy-mm-dd hh24:mi:ss", -30); // 获取30秒前的时间。
  printf("strtime1=%s\n", strtime);

  freecplus::LocalTime(strtime, "yyyy-mm-dd hh24:mi:ss"); // 获取当前时间。
  printf("strtime2=%s\n", strtime);

  freecplus::LocalTime(strtime, "yyyy-mm-dd hh24:mi:ss", 30); // 获取30秒后的时间。
  printf("strtime3=%s\n", strtime);

  printf("=%ld\n", time(0));
}
