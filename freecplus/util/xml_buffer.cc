
#include "xml_buffer.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "util/string_util.h"

namespace freecplus {

bool GetXMLBuffer(const char *xmlbuffer, const char *fieldname, char *value, const int ilen) {
  if (value == 0)
    return false;

  if (ilen > 0)
    memset(value, 0, ilen + 1); // 调用者必须保证value的空间足够，否则这里会内存溢出。

  char *start = 0, *end = 0;
  char m_SFieldName[51], m_EFieldName[51];

  int m_NameLen = strlen(fieldname);
  memset(m_SFieldName, 0, sizeof(m_SFieldName));
  memset(m_EFieldName, 0, sizeof(m_EFieldName));

  snprintf(m_SFieldName, 50, "<%s>", fieldname);
  snprintf(m_EFieldName, 50, "</%s>", fieldname);

  start = 0;
  end = 0;

  start = (char *)strstr(xmlbuffer, m_SFieldName);

  if (start != 0) {
    end = (char *)strstr(start, m_EFieldName);
  }

  if ((start == 0) || (end == 0)) {
    return false;
  }

  int m_ValueLen = end - start - m_NameLen - 2 + 1;

  if (((m_ValueLen - 1) <= ilen) || (ilen == 0)) {
    strncpy(value, start + m_NameLen + 2, m_ValueLen - 1);
    value[m_ValueLen - 1] = 0;
  } else {
    strncpy(value, start + m_NameLen + 2, ilen);
    value[ilen] = 0;
  }

  DeleteLRChar(value, ' ');

  return true;
}

bool GetXMLBuffer(const char *xmlbuffer, const char *fieldname, bool *value) {
  if (value == 0)
    return false;

  (*value) = false;

  char strTemp[51];

  memset(strTemp, 0, sizeof(strTemp));

  if (GetXMLBuffer(xmlbuffer, fieldname, strTemp, 10) == true) {
    ToUpper(strTemp); // 转换为大写来判断。
    if (strcmp(strTemp, "TRUE") == 0) {
      (*value) = true;
      return true;
    }
  }

  return false;
}

bool GetXMLBuffer(const char *xmlbuffer, const char *fieldname, int *value) {
  if (value == 0)
    return false;

  (*value) = 0;

  char strTemp[51];

  memset(strTemp, 0, sizeof(strTemp));

  if (GetXMLBuffer(xmlbuffer, fieldname, strTemp, 50) == true) {
    (*value) = atoi(strTemp);
    return true;
  }

  return false;
}

bool GetXMLBuffer(const char *xmlbuffer, const char *fieldname, unsigned int *value) {
  if (value == 0)
    return false;

  (*value) = 0;

  char strTemp[51];

  memset(strTemp, 0, sizeof(strTemp));

  if (GetXMLBuffer(xmlbuffer, fieldname, strTemp, 50) == true) {
    (*value) = (unsigned int)atoi(strTemp);
    return true;
  }

  return false;
}

bool GetXMLBuffer(const char *xmlbuffer, const char *fieldname, long *value) {
  if (value == 0)
    return false;

  (*value) = 0;

  char strTemp[51];

  memset(strTemp, 0, sizeof(strTemp));

  if (GetXMLBuffer(xmlbuffer, fieldname, strTemp, 50) == true) {
    (*value) = atol(strTemp);
    return true;
  }

  return false;
}

bool GetXMLBuffer(const char *xmlbuffer, const char *fieldname, unsigned long *value) {
  if (value == 0)
    return false;

  (*value) = 0;

  char strTemp[51];

  memset(strTemp, 0, sizeof(strTemp));

  if (GetXMLBuffer(xmlbuffer, fieldname, strTemp, 50) == true) {
    (*value) = (unsigned long)atol(strTemp);
    return true;
  }

  return false;
}

bool GetXMLBuffer(const char *xmlbuffer, const char *fieldname, double *value) {
  if (value == 0)
    return false;

  (*value) = 0;

  char strTemp[51];

  memset(strTemp, 0, sizeof(strTemp));

  if (GetXMLBuffer(xmlbuffer, fieldname, strTemp, 50) == true) {
    (*value) = atof(strTemp);
    return true;
  }

  return false;
}

} // namespace freecplus
