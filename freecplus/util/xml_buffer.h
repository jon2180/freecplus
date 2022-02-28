#pragma once

namespace freecplus {
///////////////////////////////////// /////////////////////////////////////
// 解析xml格式字符串的函数族。
// xml格式的字符串的内容如下：
// <filename>/tmp/freecplus.h</filename><mtime>2020-01-01 12:20:35</mtime><size>18348</size>
// <filename>/tmp/freecplus.cpp</filename><mtime>2020-01-01 10:10:15</mtime><size>50945</size>
// xmlbuffer：待解析的xml格式字符串。
// fieldname：字段的标签名。
// value：传入变量的地址，用于存放字段内容，支持bool、int、insigned int、long、unsigned long、double和char[]。
// 注意，当value参数的数据类型为char
// []时，必须保证value数组的内存足够，否则可能发生内存溢出的问题，也可以用ilen参数限定获取字段内容的长度，ilen的缺省值为0，表示不限长度。
// 返回值：true-成功；如果fieldname参数指定的标答名不存在，返回失败。
bool GetXMLBuffer(const char *xmlbuffer, const char *fieldname, bool *value);
bool GetXMLBuffer(const char *xmlbuffer, const char *fieldname, int *value);
bool GetXMLBuffer(const char *xmlbuffer, const char *fieldname, unsigned int *value);
bool GetXMLBuffer(const char *xmlbuffer, const char *fieldname, long *value);
bool GetXMLBuffer(const char *xmlbuffer, const char *fieldname, unsigned long *value);
bool GetXMLBuffer(const char *xmlbuffer, const char *fieldname, double *value);
bool GetXMLBuffer(const char *xmlbuffer, const char *fieldname, char *value, int ilen = 0);
///////////////////////////////////// /////////////////////////////////////

} // namespace freecplus