/*
 *  程序名：demo12.cpp，此程序演示freecplus框架中字符串大小写转换函数的使用。
 *  作者：C语言技术网(www.freecplus.net) 日期：20190525
 */
#include "freecplus.h"

#include <string>

int main() {
  char str1[31]; // C语言风格的字符串。

  freecplus::STRCPY(str1, sizeof(str1), "12abz45ABz8西施。");
  freecplus::ToUpper(str1);   // 把str1中的小写字母转换为大写。
  printf("str1=%s=\n", str1); // 出输结果是str1=12ABZ45ABZ8西施。=

  freecplus::STRCPY(str1, sizeof(str1), "12abz45ABz8西施。");
  freecplus::ToLower(str1);   // 把str1中的大写字母转换为小写。
  printf("str1=%s=\n", str1); // 出输结果是str1=12abz45abz8西施。=

  std::string str2; // C++语言风格的字符串。

  str2 = "12abz45ABz8西施。";
  freecplus::ToUpper(str2);           // 把str2中的小写字母转换为大写。
  printf("str2=%s=\n", str2.c_str()); // 出输结果是str2=12ABZ45ABZ8西施。=

  str2 = "12abz45ABz8西施。";
  freecplus::ToLower(str2);           // 把str2中的大写字母转换为小写。
  printf("str2=%s=\n", str2.c_str()); // 出输结果是str1=12abz45abz8西施。=
}
