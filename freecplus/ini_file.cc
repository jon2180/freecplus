#include "ini_file.h"

#include <cstring>

#include "file.h"
#include "util/xml_buffer.h"

namespace freecplus {

CIniFile::CIniFile() {}

bool CIniFile::LoadFile(const char *filename) {
  m_xmlbuffer.clear();

  CFile File;

  if (File.Open(filename, "r") == false)
    return false;

  char strLine[501];

  while (true) {
    memset(strLine, 0, sizeof(strLine));

    if (File.FFGETS(strLine, 500) == false)
      break;

    m_xmlbuffer = m_xmlbuffer + strLine;
  }

  if (m_xmlbuffer.length() < 10)
    return false;

  return true;
}

bool CIniFile::GetValue(const char *fieldname, bool *value) {
  return GetXMLBuffer(m_xmlbuffer.c_str(), fieldname, value);
}

bool CIniFile::GetValue(const char *fieldname, char *value, int ilen) {
  return GetXMLBuffer(m_xmlbuffer.c_str(), fieldname, value, ilen);
}

bool CIniFile::GetValue(const char *fieldname, int *value) {
  return GetXMLBuffer(m_xmlbuffer.c_str(), fieldname, value);
}

bool CIniFile::GetValue(const char *fieldname, unsigned int *value) {
  return GetXMLBuffer(m_xmlbuffer.c_str(), fieldname, value);
}

bool CIniFile::GetValue(const char *fieldname, long *value) {
  return GetXMLBuffer(m_xmlbuffer.c_str(), fieldname, value);
}

bool CIniFile::GetValue(const char *fieldname, unsigned long *value) {
  return GetXMLBuffer(m_xmlbuffer.c_str(), fieldname, value);
}

bool CIniFile::GetValue(const char *fieldname, double *value) {
  return GetXMLBuffer(m_xmlbuffer.c_str(), fieldname, value);
}

} // namespace freecplus