#include "command_str.h"

#include <cstring>

#include "util/string_util.h"

namespace freecplus {

CCmdStr::CCmdStr() { m_vCmdStr.clear(); }

// 把字符串拆分到m_vCmdStr容器中。
// buffer：待拆分的字符串。
// sepstr：buffer字符串中字段内容的分隔符，注意，分隔符是字符串，如","、" "、"|"、"~!~"。
// bdelspace：是否删除拆分后的字段内容前后的空格，true-删除；false-不删除，缺省删除。
void CCmdStr::SplitToCmd(const string buffer, const char *sepstr, const bool bdelspace) {
  // 清除所有的旧数据
  m_vCmdStr.clear();

  int iPOS = 0;
  string srcstr, substr;

  srcstr = buffer;

  char str[2048];

  while ((iPOS = srcstr.find(sepstr)) >= 0) {
    substr = srcstr.substr(0, iPOS);

    if (bdelspace == true) {
      memset(str, 0, sizeof(str));

      strncpy(str, substr.c_str(), 2000);

      DeleteLRChar(str, ' ');

      substr = str;
    }

    m_vCmdStr.push_back(substr);

    iPOS = iPOS + strlen(sepstr);

    srcstr = srcstr.substr(iPOS, srcstr.size() - iPOS);
  }

  substr = srcstr;

  if (bdelspace == true) {
    memset(str, 0, sizeof(str));

    strncpy(str, substr.c_str(), 2000);

    DeleteLRChar(str, ' ');

    substr = str;
  }

  m_vCmdStr.push_back(substr);

  return;
}

int CCmdStr::CmdCount() { return m_vCmdStr.size(); }

bool CCmdStr::GetValue(const int inum, char *value, const int ilen) {
  if ((inum >= (int)m_vCmdStr.size()) || (value == 0))
    return false;

  if (ilen > 0)
    memset(value, 0, ilen + 1); // 调用者必须保证value的空间足够，否则这里会内存溢出。

  if ((m_vCmdStr[inum].length() <= (unsigned int)ilen) || (ilen == 0)) {
    strcpy(value, m_vCmdStr[inum].c_str());
  } else {
    strncpy(value, m_vCmdStr[inum].c_str(), ilen);
    value[ilen] = 0;
  }

  return true;
}

bool CCmdStr::GetValue(const int inum, int *value) {
  if ((inum >= (int)m_vCmdStr.size()) || (value == 0))
    return false;

  (*value) = 0;

  if (inum >= (int)m_vCmdStr.size())
    return false;

  (*value) = atoi(m_vCmdStr[inum].c_str());

  return true;
}

bool CCmdStr::GetValue(const int inum, unsigned int *value) {
  if ((inum >= (int)m_vCmdStr.size()) || (value == 0))
    return false;

  (*value) = 0;

  if (inum >= (int)m_vCmdStr.size())
    return false;

  (*value) = atoi(m_vCmdStr[inum].c_str());

  return true;
}

bool CCmdStr::GetValue(const int inum, long *value) {
  if ((inum >= (int)m_vCmdStr.size()) || (value == 0))
    return false;

  (*value) = 0;

  if (inum >= (int)m_vCmdStr.size())
    return false;

  (*value) = atol(m_vCmdStr[inum].c_str());

  return true;
}

bool CCmdStr::GetValue(const int inum, unsigned long *value) {
  if ((inum >= (int)m_vCmdStr.size()) || (value == 0))
    return false;

  (*value) = 0;

  if (inum >= (int)m_vCmdStr.size())
    return false;

  (*value) = atol(m_vCmdStr[inum].c_str());

  return true;
}

bool CCmdStr::GetValue(const int inum, double *value) {
  if ((inum >= (int)m_vCmdStr.size()) || (value == 0))
    return false;

  (*value) = 0;

  if (inum >= (int)m_vCmdStr.size())
    return false;

  (*value) = (double)atof(m_vCmdStr[inum].c_str());

  return true;
}

bool CCmdStr::GetValue(const int inum, bool *value) {
  if ((inum >= (int)m_vCmdStr.size()) || (value == 0))
    return false;

  (*value) = 0;

  if (inum >= (int)m_vCmdStr.size())
    return false;

  char strTemp[11];
  memset(strTemp, 0, sizeof(strTemp));
  strncpy(strTemp, m_vCmdStr[inum].c_str(), 10);

  ToUpper(strTemp); // 转换为大写来判断。
  if (strcmp(strTemp, "TRUE") == 0)
    (*value) = true;

  return true;
}

CCmdStr::~CCmdStr() { m_vCmdStr.clear(); }

} // namespace freecplus