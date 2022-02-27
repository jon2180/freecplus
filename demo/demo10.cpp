/*
 *  程序名：demo10.cpp，此程序演示freecplus框架中删除字符串左、右、两边指定字符的使用。
 *  作者：C语言技术网(www.freecplus.net) 日期：20190525
 */
#include "freecplus.h"

int main() {
  char str[11]; // 字符串str的大小是11字节。

  freecplus::STRCPY(str, sizeof(str), "  西施  ");
  freecplus::DeleteLChar(str, ' '); // 删除str左边的空格
  printf("str=%s=\n", str);         // 出输结果是str=西施  =

  freecplus::STRCPY(str, sizeof(str), "  西施  ");
  freecplus::DeleteRChar(str, ' '); // 删除str右边的空格
  printf("str=%s=\n", str);         // 出输结果是str=  西施=

  freecplus::STRCPY(str, sizeof(str), "  西施  ");
  freecplus::DeleteLRChar(str, ' '); // 删除str两边的空格
  printf("str=%s=\n", str);          // 出输结果是str=西施=
}
