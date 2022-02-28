#pragma once

#include <string>

namespace freecplus {

using std::string;
// 参数文件操作类。
// CIniFile类操作的不是传统的ini文件，是xml格式的参数文件，例如：
/*
 * 文件名：hssms.xml
<?xml version="1.0" encoding="gbk" ?>
<root>
    <!-- 程序运行的日志文件名。 -->
    <logpath>/log/hssms</logpath>

    <!-- 数据库连接参数。 -->
    <connstr>hssms/smspwd@hssmszx</connstr>

    <!-- 数据文件存放的根目录。 -->
    <datapath>/data/hssms</datapath>

    <!-- 中心服务器的ip地址。 -->
    <serverip>192.168.1.1</serverip>

    <!-- 中心服务器的通信端口。 -->
    <port>5058</port>

    <!-- 是否采用长连接，true-是；false-否。 -->
    <online>true</online>
</root>
*/

class XmlFileReader {
public:
  string m_xmlbuffer; // 存放参数文件全部的内容，由LoadFile方法载入。

  XmlFileReader();

  // 把参数文件的内容载入到m_xmlbuffer成员变量中。
  bool LoadFile(const char *filename);

  // 获取参数的值。
  // fieldname：字段的标签名。
  // value：传入变量的地址，用于存放字段的值，支持bool、int、insigned int、long、unsigned long、double和char[]。
  // 注意，当value参数的数据类型为char []时，必须保证value的内存足够，否则可能发生内存溢出的问题，
  // 也可以用ilen参数限定获取字段内容的长度，ilen的缺省值为0，表示不限定获取字段内容的长度。
  // 返回值：true-成功；false-失败。
  bool GetValue(const char *fieldname, bool *value) const;
  bool GetValue(const char *fieldname, int *value) const;
  bool GetValue(const char *fieldname, unsigned int *value) const;
  bool GetValue(const char *fieldname, long *value) const;
  bool GetValue(const char *fieldname, unsigned long *value) const;
  bool GetValue(const char *fieldname, double *value) const;
  bool GetValue(const char *fieldname, char *value, int ilen = 0) const;
};

} // namespace freecplus
