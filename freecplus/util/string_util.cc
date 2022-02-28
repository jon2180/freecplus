#include "string_util.h"

#include <cstring>
#include <cstdarg>

#include "command_str.h"

namespace freecplus {

/**
 * 安全的strcpy函数
 * @param dest 目标字符串，不需要初始化，在STRCPY函数中有初始化代码。
 * @param destlen 目标字符串dest占用内存的大小。
 * @param src 原字符串。
 * @return 目标字符串dest的地址。
 */
char *STRCPY(char *dest, const size_t destlen, const char *src) {
  if (dest == nullptr)
    return nullptr;
  memset(dest, 0, destlen); // 初始化dest。
  if (src == nullptr)
    return dest;

  if (strlen(src) > destlen - 1)
    strncpy(dest, src, destlen - 1);
  else
    strcpy(dest, src);

  return dest;
}

// 安全的strncpy函数。
// dest：目标字符串，不需要初始化，在STRCPY函数中有初始化代码。
// destlen：目标字符串dest占用内存的大小。
// src：原字符串。
// n：待复制的字节数。
// 返回值：目标字符串dest的地址。
char *STRNCPY(char *dest, const size_t destlen, const char *src, size_t n) {
  if (dest == nullptr)
    return nullptr;
  memset(dest, 0, destlen); // 初始化dest。
  if (src == nullptr)
    return dest;

  if (n > destlen - 1)
    strncpy(dest, src, destlen - 1);
  else
    strncpy(dest, src, n);

  return dest;
}

// 安全的strcat函数。
// dest：目标字符串，注意，如果dest从未使过，那么需要至少一次初始化。
// destlen：目标字符串dest占用内存的大小。
// src：待追加字符串。
// 返回值：目标字符串dest的地址。
char *STRCAT(char *dest, const size_t destlen, const char *src) {
  if (dest == nullptr)
    return nullptr;
  if (src == nullptr)
    return dest;

  unsigned int left = destlen - 1 - strlen(dest);

  if (strlen(src) > left) {
    strncat(dest, src, left);
    dest[destlen - 1] = 0;
  } else
    strcat(dest, src);

  return dest;
}

// 安全的strncat函数。
// dest：目标字符串，注意，如果dest从未使过，那么需要至少一次初始化。
// destlen：目标字符串dest占用内存的大小。
// src：待追加字符串。
// n：待追加的字节数。
// 返回值：目标字符串dest的地址。
char *STRNCAT(char *dest, const size_t destlen, const char *src, size_t n) {
  if (dest == nullptr)
    return nullptr;
  if (src == nullptr)
    return dest;

  size_t left = destlen - 1 - strlen(dest);

  if (n > left) {
    strncat(dest, src, left);
    dest[destlen - 1] = 0;
  } else
    strncat(dest, src, n);

  return dest;
}

// 安全的sprintf函数。
// 将可变参数(...)按照fmt描述的格式输出到dest字符串中。
// dest：输出字符串，不需要初始化，在SPRINTF函数中会对它进行初始化。
// destlen：输出字符串dest占用内存的大小，如果格式化后的字符串内容的长度大于destlen-1，后面的内容将丢弃。
// fmt：格式控制描述。
// ...：填充到格式控制描述fmt中的参数。
// 返回值：格式化后的内容的长度，程序员一般不关心返回值。
int SPRINTF(char *dest, const size_t destlen, const char *fmt, ...) {
  if (dest == nullptr)
    return -1;

  memset(dest, 0, destlen);

  va_list arg;
  va_start(arg, fmt);
  vsnprintf(dest, destlen, fmt, arg);
  va_end(arg);

  return strlen(dest);
}

// 安全的snprintf函数。
// 将可变参数(...)按照fmt描述的格式输出到dest字符串中。
// dest：输出字符串，不需要初始化，在SNPRINTF函数中会对它进行初始化。
// destlen：输出字符串dest占用内存的大小，如果格式化后的字符串内容的长度大于destlen-1，后面的内容将丢弃。
// n：把格式化后的字符串截取n-1存放到dest中，如果n>destlen，则取destlen-1。
// fmt：格式控制描述。
// ...：填充到格式控制描述fmt中的参数。
// 返回值：格式化后的内容的长度，程序员一般不关心返回值。
size_t SNPRINTF(char *dest, const size_t destlen, size_t n, const char *fmt, ...) {
  if (dest == nullptr)
    return -1;

  memset(dest, 0, destlen);

  size_t len = n;
  if (n > destlen)
    len = destlen;

  va_list arg;
  va_start(arg, fmt);
  vsnprintf(dest, len, fmt, arg);
  va_end(arg);

  return strlen(dest);
}

// 删除字符串左边指定的字符。
// str：待处理的字符串。
// chr：需要删除的字符。
void DeleteLChar(char *str, const char chr) {
  if (str == nullptr)
    return;
  if (strlen(str) == 0)
    return;

  char strTemp[strlen(str) + 1];

  int iTemp = 0;
  memset(strTemp, 0, sizeof(strTemp));
  strcpy(strTemp, str);

  while (strTemp[iTemp] == chr)
    iTemp++;

  memset(str, 0, strlen(str) + 1);
  strcpy(str, strTemp + iTemp);
}

// 删除字符串右边指定的字符。
// str：待处理的字符串。
// chr：需要删除的字符。
void DeleteRChar(char *str, const char chr) {
  if (str == nullptr)
    return;
  if (strlen(str) == 0)
    return;

  size_t istrlen = strlen(str);

  while (istrlen > 0) {
    if (str[istrlen - 1] != chr)
      break;

    str[istrlen - 1] = 0;

    istrlen--;
  }
}

// 删除字符串左右两边指定的字符。
// str：待处理的字符串。
// chr：需要删除的字符。
void DeleteLRChar(char *str, const char chr) {
  DeleteLChar(str, chr);
  DeleteRChar(str, chr);
}

// 把字符串中的小写字母转换成大写，忽略不是字母的字符。
// str：待转换的字符串，支持char[]和string两种类型。
void ToUpper(char *str) {
  if (str == nullptr)
    return;

  if (strlen(str) == 0)
    return;

  size_t istrlen = strlen(str);

  for (size_t ii = 0; ii < istrlen; ii++) {
    if ((str[ii] >= 'a') && (str[ii] <= 'z'))
      str[ii] = static_cast<char>(str[ii] - 32);
  }
}

void ToUpper(string &str) {
  if (str.empty())
    return;

  char strtemp[str.size() + 1];

  memset(strtemp, 0, sizeof(strtemp));
  strcpy(strtemp, str.c_str());
  ToUpper(strtemp);
  str = strtemp;
}

// 把字符串中的大写字母转换成小写，忽略不是字母的字符。
// str：待转换的字符串，支持char[]和string两种类型。
void ToLower(char *str) {
  if (str == nullptr)
    return;

  if (strlen(str) == 0)
    return;

  size_t istrlen = strlen(str);

  for (size_t ii = 0; ii < istrlen; ii++) {
    if ((str[ii] >= 'A') && (str[ii] <= 'Z'))
      str[ii] = static_cast<char>(str[ii] + 32);
  }
}

void ToLower(string &str) {
  if (str.empty())
    return;

  char strtemp[str.size() + 1];

  memset(strtemp, 0, sizeof(strtemp));
  strcpy(strtemp, str.c_str());
  ToLower(strtemp);
  str = strtemp;
}

// 字符串替换函数
// 在字符串str中，如果存在字符串str1，就替换为字符串str2。
// str：待处理的字符串。
// str1：旧的内容。
// str2：新的内容。
// bloop：是否循环执行替换。
// 注意：
// 1、如果str2比str1要长，替换后str会变长，所以必须保证str有足够的长度，否则内存会溢出。
// 2、如果str2中包函了str1的内容，且bloop为true，存在逻辑错误，将不执行任何替换。
void UpdateStr(char *str, const char *str1, const char *str2, bool bloop) {
  if (str == nullptr)
    return;
  if (strlen(str) == 0)
    return;
  if ((str1 == nullptr) || (str2 == 0))
    return;

  // 如果bloop为true并且str2中包函了str1的内容，直接返回，因为会进入死循环，最终导致内存溢出。
  if (bloop && (strstr(str2, str1) > (void *)nullptr))
    return;

  // 尽可能分配更多的空间，但仍有可能出现内存溢出的情况，最好优化成string。
  size_t ilen = strlen(str) * 10;
  if (ilen < 1000)
    ilen = 1000;

  char strTemp[ilen];

  char *strStart = str;

  char *strPos = nullptr;

  while (true) {
    if (bloop) {
      strPos = strstr(str, str1);
    } else {
      strPos = strstr(strStart, str1);
    }

    if (strPos == nullptr)
      break;

    memset(strTemp, 0, sizeof(strTemp));
    strncpy(strTemp, str, strPos - str);
    strcat(strTemp, str2);
    strcat(strTemp, strPos + strlen(str1));
    strcpy(str, strTemp);

    strStart = strPos + strlen(str2);
  }
}

// 从一个字符串中提取出数字的内容，存放到另一个字符串中。
// src：源字符串。
// dest：目标字符串。
// bsigned：是否包括符号（+和-），true-包括；false-不包括。
// bdot：是否包括小数点的圆点符号，true-包括；false-不包括。
void PickNumber(const char *src, char *dest, const bool bsigned, const bool bdot) {
  if (dest == nullptr)
    return;
  if (src == nullptr) {
    strcpy(dest, "");
    return;
  }

  char strtemp[strlen(src) + 1];
  memset(strtemp, 0, sizeof(strtemp));
  strcpy(strtemp, src);
  DeleteLRChar(strtemp, ' ');

  int ipossrc, iposdst, ilen;
  ipossrc = iposdst = ilen = 0;

  ilen = strlen(strtemp);

  for (ipossrc = 0; ipossrc < ilen; ipossrc++) {
    if (bsigned && (strtemp[ipossrc] == '+')) {
      dest[iposdst++] = strtemp[ipossrc];
      continue;
    }

    if (bsigned && (strtemp[ipossrc] == '-')) {
      dest[iposdst++] = strtemp[ipossrc];
      continue;
    }

    if (bdot && (strtemp[ipossrc] == '.')) {
      dest[iposdst++] = strtemp[ipossrc];
      continue;
    }

    if (isdigit(strtemp[ipossrc]))
      dest[iposdst++] = strtemp[ipossrc];
  }

  dest[iposdst] = 0;
}

// 正则表达式，判断一个字符串是否匹配另一个字符串。
// str：需要判断的字符串，精确表示的字符串，如文件名"freecplus.cpp"。
// rules：匹配规则表达式，用星号"*"表示任意字符串，多个字符串之间用半角的逗号分隔，如"*.h,*.cpp"。
// 注意，str参数不支持"*"，rules参数支持"*"，函数在判断str是否匹配rules的时候，会忽略字母的大小写。
bool MatchStr(const string str, const string rules) {
  // 如果用于比较的字符是空的，返回false
  if (rules.empty())
    return false;

  // 如果被比较的字符串是"*"，返回true
  if (rules == "*")
    return true;

  // 处理文件名匹配规则中的时间匹配dd-nn.mm
  char strTemp[2049];
  memset(strTemp, 0, sizeof(strTemp));
  strncpy(strTemp, rules.c_str(), 2000);

  int ii, jj;
  int iPOS1, iPOS2;
  CommandStr CmdStr, CmdSubStr;

  string strFileName, strMatchStr;

  strFileName = str;
  strMatchStr = strTemp;

  // 把字符串都转换成大写后再来比较
  ToUpper(strFileName);
  ToUpper(strMatchStr);

  CmdStr.SplitToCmd(strMatchStr, ",");

  for (ii = 0; ii < CmdStr.CmdCount(); ii++) {
    // 如果为空，就一定要跳过，否则就会被配上
    if (CmdStr.m_vCmdStr[ii].empty())
      continue;

    iPOS1 = iPOS2 = 0;
    CmdSubStr.SplitToCmd(CmdStr.m_vCmdStr[ii], "*");

    for (jj = 0; jj < CmdSubStr.CmdCount(); jj++) {
      // 如果是文件名的首部
      if (jj == 0) {
        if (strncmp(strFileName.c_str(), CmdSubStr.m_vCmdStr[jj].c_str(), CmdSubStr.m_vCmdStr[jj].size()) != 0)
          break;
      }

      // 如果是文件名的尾部
      if (jj == CmdSubStr.CmdCount() - 1) {
        if (strcmp(strFileName.c_str() + strFileName.size() - CmdSubStr.m_vCmdStr[jj].size(),
                   CmdSubStr.m_vCmdStr[jj].c_str()) != 0)
          break;
      }

      iPOS2 = strFileName.find(CmdSubStr.m_vCmdStr[jj], iPOS1);

      if (iPOS2 < 0)
        break;

      iPOS1 = iPOS2 + CmdSubStr.m_vCmdStr[jj].size();
    }

    if (jj == CmdSubStr.CmdCount())
      return true;
  }

  return false;
}

// 正则表达式，判断一个字符串是否匹配另一个字符串。
// 保留MatchFileName函数是为了兼容之前的版本。
bool MatchFileName(const string in_FileName, const string in_MatchStr) { return MatchStr(in_FileName, in_MatchStr); }

// 统计字符串的字数，全角的汉字和全角的标点符号算一个字，半角的汉字和半角的标点符号也算一个字。
// str：待统计的字符串。
// 返回值：字符串str的字数。
int Words(const char *str) {
  int wlen = 0;
  bool biswide = false;
  size_t ilen = strlen(str);

  for (size_t ii = 0; ii < ilen; ii++) {
    if ((unsigned int)str[ii] < 128) {
      wlen = wlen + 1;
    } else {
      if (biswide) {
        wlen = wlen + 1;
        biswide = false;
      } else {
        biswide = true;
      }
    }
  }

  return wlen;
}

} // namespace freecplus