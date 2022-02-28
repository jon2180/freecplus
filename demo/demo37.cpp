/*
 *  程序名：demo37.cpp，此程序演示freecplus框架中FGETS函数的用法。
 *  作者：C语言技术网(www.freecplus.net) 日期：20190525
 */
#include "freecplus.h"

#include <cstring>

int main() {
  FILE *fp = 0;

  if ((fp = freecplus::OpenFile("/tmp/aaa/bbb/ccc/tmp.xml", "r")) == 0) {
    printf("OpenFile(/tmp/aaa/bbb/ccc/tmp.xml) %d:%s\n", errno, strerror(errno));
    return -1;
  }

  char strBuffer[301];

  while (true) {
    memset(strBuffer, 0, sizeof(strBuffer));
    if (freecplus::GetLineFromFile(fp, strBuffer, 300) == false)
      break; // 行内容以"\n"结束。
    // if (GetLineFromFile(fp,strBuffer,300,"<endl/>")==false) break; // 行内容以"<endl/>"结束。

    printf("strBuffer=%s", strBuffer);
  }

  fclose(fp);
}
