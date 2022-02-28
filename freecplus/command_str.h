#pragma once

#include <string>
#include <vector>

namespace freecplus {

using std::string;
using std::vector;

// CCmdStr类用于拆分有分隔符的字符串。
// 字符串的格式为：字段内容1+分隔符+字段内容2+分隔符+字段内容3+分隔符+...+字段内容n。
// 例如："messi,10,striker,30,1.72,68.5,Barcelona"，这是足球运动员梅西的资料，包括姓名、
// 球衣号码、场上位置、年龄、身高、体重和效力的俱乐部，字段之间用半角的逗号分隔。
class CommandStr {
public:
  vector<string> m_vCmdStr; // 存放拆分后的字段内容。

  CommandStr(); // 构造函数。

  // 把字符串拆分到m_vCmdStr容器中。
  // buffer：待拆分的字符串。
  // sepstr：buffer中采用的分隔符，注意，sepstr参数的数据类型不是字符，是字符串，如","、" "、"|"、"~!~"。
  // bdelspace：拆分后是否删除字段内容前后的空格，true-删除；false-不删除，缺省删除。
  void SplitToCmd(string buffer, const char *sepstr, bool bdelspace = true);

  // 获取拆分后字段的个数，即m_vCmdStr容器的大小。
  [[nodiscard]] int CmdCount() const;

  // 从m_vCmdStr容器获取字段内容。
  // inum：字段的顺序号，类似数组的下标，从0开始。
  // value：传入变量的地址，用于存放字段内容。
  // 返回值：true-成功；如果inum的取值超出了m_vCmdStr容器的大小，返回失败。
  bool GetValue(int inum, char *value, int ilen = 0); // 字符串，ilen缺省值为0。
  bool GetValue(int inum, int *value);                      // int整数。
  bool GetValue(int inum, unsigned int *value);             // unsigned int整数。
  bool GetValue(int inum, long *value);                     // long整数。
  bool GetValue(int inum, unsigned long *value);            // unsigned long整数。
  bool GetValue(int inum, double *value);                   // 双精度double。
  bool GetValue(int inum, bool *value);                     // bool型。

  ~CommandStr(); // 析构函数。
};
///////////////////////////////////// /////////////////////////////////////

} // namespace freecplus
