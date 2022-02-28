#include "ini_file.h"

#include <cstring>

#include "file.h"
#include "util/xml_buffer.h"

namespace freecplus {

XmlFileReader::XmlFileReader() = default;

bool XmlFileReader::LoadFile(const char *filename) {
  m_xmlbuffer.clear();

  File File;

  if (!File.Open(filename, "r"))
    return false;

  char strLine[501];

  while (true) {
    memset(strLine, 0, sizeof(strLine));

    if (!File.FFGETS(strLine, 500))
      break;

    m_xmlbuffer = m_xmlbuffer + strLine;
  }

  if (m_xmlbuffer.length() < 10)
    return false;

  return true;
}

bool XmlFileReader::GetValue(const char *fieldname, bool *value) const {
  return GetXMLBuffer(m_xmlbuffer.c_str(), fieldname, value);
}

bool XmlFileReader::GetValue(const char *fieldname, char *value, int ilen) const {
  return GetXMLBuffer(m_xmlbuffer.c_str(), fieldname, value, ilen);
}

bool XmlFileReader::GetValue(const char *fieldname, int *value) const {
  return GetXMLBuffer(m_xmlbuffer.c_str(), fieldname, value);
}

bool XmlFileReader::GetValue(const char *fieldname, unsigned int *value) const {
  return GetXMLBuffer(m_xmlbuffer.c_str(), fieldname, value);
}

bool XmlFileReader::GetValue(const char *fieldname, long *value) const {
  return GetXMLBuffer(m_xmlbuffer.c_str(), fieldname, value);
}

bool XmlFileReader::GetValue(const char *fieldname, unsigned long *value) const {
  return GetXMLBuffer(m_xmlbuffer.c_str(), fieldname, value);
}

bool XmlFileReader::GetValue(const char *fieldname, double *value) const {
  return GetXMLBuffer(m_xmlbuffer.c_str(), fieldname, value);
}

} // namespace freecplus