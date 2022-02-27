/*
 *  程序名：demo5.cpp，此程序演示freecplus框架中STRNCAT函数的使用。
 *  作者：C语言技术网(www.freecplus.net) 日期：20190525
 */
#include "freecplus.h"

int main() {
  char str[11]; // 字符串str的大小是11字节。

  freecplus::STRCPY(str, sizeof(str), "www");
  freecplus::STRNCAT(str, sizeof(str), ".free", 10); // str原有的内容加上待追加的内容没有超过str可以存放的大小。
  printf("str=%s=\n", str);                          // 出输结果是str=www.free=

  freecplus::STRCPY(str, sizeof(str), "www");
  freecplus::STRNCAT(str, sizeof(str), ".freecplus.net", 6); // str原有的内容加上待追加的内容没有超过str可以存放的大小。
  printf("str=%s=\n", str);                                  // 出输结果是str=www.freec=

  freecplus::STRCPY(str, sizeof(str), "www");
  freecplus::STRNCAT(str, sizeof(str), ".freecplus.net", 10); // str原有的内容加上待追加的内容超过了str可以存放的大小。
  printf("str=%s=\n", str);                                   // 出输结果是str=www.freecp=
}
