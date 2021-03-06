/*
 *  程序名：demo30.cpp，此程序演示freecplus框架中采用MKDIR函数根据绝对路径的文件名或目录名逐级的创建目录。
 *  作者：C语言技术网(www.freecplus.net) 日期：20190525
 */
#include "freecplus.h"

int main() {
  freecplus::MakeDir("/tmp/aaa/bbb/ccc/ddd", false); // 创建"/tmp/aaa/bbb/ccc/ddd"目录。

  freecplus::MakeDir("/tmp/111/222/333/444/data.xml", true); // 创建"/tmp/111/222/333/444"目录。
}
