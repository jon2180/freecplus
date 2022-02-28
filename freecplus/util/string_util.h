#pragma once

#include <cstddef>
#include <string>

namespace freecplus {

using std::string;

// 以下是字符串操作相关的函数和类

// 安全的strcpy函数。
// dest：目标字符串，不需要初始化，在STRCPY函数中会对它进行初始化。
// destlen：目标字符串dest占用内存的大小。
// src：原字符串。
// 返回值：目标字符串dest的地址。
char *STRCPY(char *dest, size_t destlen, const char *src);

// 安全的strncpy函数。
// dest：目标字符串，不需要初始化，在STRNCPY函数中会对它进行初始化。
// destlen：目标字符串dest占用内存的大小。
// src：原字符串。
// n：待复制的字节数。
// 返回值：目标字符串dest的地址。
char *STRNCPY(char *dest, size_t destlen, const char *src, size_t n);

// 安全的strcat函数。
// dest：目标字符串。
// destlen：目标字符串dest占用内存的大小。
// src：待追加的字符串。
// 返回值：目标字符串dest的地址。
char *STRCAT(char *dest, size_t destlen, const char *src);

// 安全的strncat函数。
// dest：目标字符串。
// destlen：目标字符串dest占用内存的大小。
// src：待追加的字符串。
// n：待追加的字节数。
// 返回值：目标字符串dest的地址。
char *STRNCAT(char *dest, size_t destlen, const char *src, size_t n);

// 安全的sprintf函数。
// 将可变参数(...)按照fmt描述的格式输出到dest字符串中。
// dest：输出字符串，不需要初始化，在SPRINTF函数中会对它进行初始化。
// destlen：输出字符串dest占用内存的大小，如果格式化后的字符串长度大于destlen-1，后面的内容将丢弃。
// fmt：格式控制描述。
// ...：填充到格式控制描述fmt中的参数。
// 返回值：格式化后的字符串的长度，程序员一般不关心返回值。
int SPRINTF(char *dest, size_t destlen, const char *fmt, ...);

// 安全的snprintf函数。
// 将可变参数(...)按照fmt描述的格式输出到dest字符串中。
// dest：输出字符串，不需要初始化，在SNPRINTF函数中会对它进行初始化。
// destlen：输出字符串dest占用内存的大小，如果格式化后的字符串长度大于destlen-1，后面的内容将丢弃。
// n：把格式化后的字符串截取n-1存放到dest中，如果n>destlen-1，则取destlen-1。
// fmt：格式控制描述。
// ...：填充到格式控制描述fmt中的参数。
// 返回值：格式化后的字符串的长度，程序员一般不关心返回值。
// 注意：windows和linux平台下的snprintf函数的第三个参数n的用法略有不同。假设格式化后的字符串的长度
// 超过10,第三个参数n取值是10，那么，在windows平台下dest的长度将是10，linux平台下dest的长度却是9。
size_t SNPRINTF(char *dest, size_t destlen, size_t n, const char *fmt, ...);

// 删除字符串左边指定的字符。
// str：待处理的字符串。
// chr：需要删除的字符。
void DeleteLChar(char *str, char chr);

// 删除字符串右边指定的字符。
// str：待处理的字符串。
// chr：需要删除的字符。
void DeleteRChar(char *str, char chr);

// 删除字符串左右两边指定的字符。
// str：待处理的字符串。
// chr：需要删除的字符。
void DeleteLRChar(char *str, char chr);

// 把字符串中的小写字母转换成大写，忽略不是字母的字符。
// str：待转换的字符串，支持char[]和string两种类型。
void ToUpper(char *str);
void ToUpper(string &str);

// 把字符串中的大写字母转换成小写，忽略不是字母的字符。
// str：待转换的字符串，支持char[]和string两种类型。
void ToLower(char *str);
void ToLower(string &str);

// 字符串替换函数。
// 在字符串str中，如果存在字符串str1，就替换为字符串str2。
// str：待处理的字符串。
// str1：旧的内容。
// str2：新的内容。
// bloop：是否循环执行替换。
// 注意：
// 1、如果str2比str1要长，替换后str会变长，所以必须保证str有足够的空间，否则内存会溢出。
// 2、如果str2中包含了str1的内容，且bloop为true，这种做法存在逻辑错误，UpdateStr将什么也不做。
void UpdateStr(char *str, const char *str1, const char *str2, bool bloop = true);

// 从一个字符串中提取出数字、符号和小数点，存放到另一个字符串中。
// src：原字符串。
// dest：目标字符串。
// bsigned：是否包括符号（+和-），true-包括；false-不包括。
// bdot：是否包括小数点的圆点符号，true-包括；false-不包括。
void PickNumber(const char *src, char *dest, bool bsigned, bool bdot);

// 正则表达式，判断一个字符串是否匹配另一个字符串。
// str：需要判断的字符串，是精确表示的，如文件名"freecplus.cpp"。
// rules：匹配规则的表达式，用星号"*"代表任意字符串，多个表达式之间用半角的逗号分隔，如"*.h,*.cpp"。
// 注意：1）str参数不支持"*"，rules参数支持"*"；2）函数在判断str是否匹配rules的时候，会忽略字母的大小写。
bool MatchStr(string str, string rules);

// 正则表达式，判断一个字符串是否匹配另一个字符串。
// 保留MatchFileName函数是为了兼容旧的版本。
bool MatchFileName(string in_FileName, string in_MatchStr);

// 统计字符串的字数，全角的汉字和全角的标点符号算一个字，半角的汉字和半角的标点符号也算一个字。
// str：待统计的字符串。
// 返回值：字符串str的字数。
int Words(const char *str);
///////////////////////////////////// /////////////////////////////////////

} // namespace freecplus