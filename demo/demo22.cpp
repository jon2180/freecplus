/*
 *  程序名：demo22.cpp，此程序演示调用freecplus框架的GetXMLBuffer函数解析xml字符串。
 *  作者：C语言技术网(www.freecplus.net) 日期：20190525
 */
#include "freecplus.h"

#include <cstring>

// 用于存放足球运动员资料的结构体。
struct st_player {
  char name[51]; // 姓名
  char no[6];    // 球衣号码
  bool striker;  // 场上位置是否是前锋，true-是；false-不是。
  int age;       // 年龄
  double weight; // 体重，kg。
  long sal;      // 年薪，欧元。
  char club[51]; // 效力的俱乐部
} stplayer;

int main() {
  memset(&stplayer, 0, sizeof(struct st_player));

  char buffer[301];
  freecplus::STRCPY(buffer, sizeof(buffer),
         "<name>messi</name><no>10</no><striker>true</striker><age>30</age><weight>68.5</weight><sal>21000000</"
         "sal><club>Barcelona</club>");

  freecplus::GetXMLBuffer(buffer, "name", stplayer.name, 50);
  freecplus::GetXMLBuffer(buffer, "no", stplayer.no, 5);
  freecplus::GetXMLBuffer(buffer, "striker", &stplayer.striker);
  freecplus::GetXMLBuffer(buffer, "age", &stplayer.age);
  freecplus::GetXMLBuffer(buffer, "weight", &stplayer.weight);
  freecplus::GetXMLBuffer(buffer, "sal", &stplayer.sal);
  freecplus::GetXMLBuffer(buffer, "club", stplayer.club, 50);

  printf("name=%s,no=%s,striker=%d,age=%d,weight=%.1f,sal=%ld,club=%s\n", stplayer.name, stplayer.no, stplayer.striker,
         stplayer.age, stplayer.weight, stplayer.sal, stplayer.club);
  // 输出结果:name=messi,no=10,striker=1,age=30,weight=68.5,sal=21000000,club=Barcelona
}
