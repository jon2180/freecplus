#pragma once

#include <cstdio>

using std::FILE;

///////////////////////////////////// /////////////////////////////////////
// 文件操作相关的函数和类

// 文件操作类声明
class CFile {
private:
  FILE *m_fp;              // 文件指针
  bool m_bEnBuffer;        // 是否启用缓冲，true-启用；false-不启用，缺省是启用。
  char m_filename[301];    // 文件名，建议采用绝对路径的文件名。
  char m_filenametmp[301]; // 临时文件名，在m_filename后加".tmp"。

public:
  CFile(); // 构造函数。

  bool IsOpened(); // 判断文件是否已打开，返回值：true-已打开；false-未打开。

  // 打开文件。
  // filename：待打开的文件名，建议采用绝对路径的文件名。
  // openmode：打开文件的模式，与fopen库函数的打开模式相同。
  // bEnBuffer：是否启用缓冲，true-启用；false-不启用，缺省是启用。
  // 注意：如果待打开的文件的目录不存在，就会创建目录。
  bool Open(const char *filename, const char *openmode, bool bEnBuffer = true);

  // 关闭文件指针，并删除文件。
  bool CloseAndRemove();

  // 专为重命名而打开文件，参数与Open方法相同。
  // 注意：OpenForRename打开的是filename后加".tmp"的临时文件，所以openmode只能是"a"、"a+"、"w"、"w+"。
  bool OpenForRename(const char *filename, const char *openmode, bool bEnBuffer = true);
  // 关闭文件指针，并把OpenForRename方法打开的临时文件名重命名为filename。
  bool CloseAndRename();

  // 调用fprintf向文件写入数据，参数与fprintf库函数相同，但不需要传入文件指针。
  void Fprintf(const char *fmt, ...);

  // 从文件中读取以换行符"\n"结束的一行。
  // buffer：用于存放读取的内容，buffer必须大于readsize+1，否则可能会造成内存的溢出。
  // readsize：本次打算读取的字节数，如果已经读取到了结束标志"\n"，函数返回。
  // bdelcrt：是否删除行结束标志"\r"和"\n"，true-删除；false-不删除，缺省值是false。
  // 返回值：true-成功；false-失败，一般情况下，失败可以认为是文件已结束。
  bool Fgets(char *buffer, const int readsize, bool bdelcrt = false);

  // 从文件文件中读取一行。
  // buffer：用于存放读取的内容，buffer必须大于readsize+1，否则可能会造成读到的数据不完整或内存的溢出。
  // readsize：本次打算读取的字节数，如果已经读取到了结束标志，函数返回。
  // endbz：行内容结束的标志，缺省为空，表示行内容以"\n"为结束标志。
  // 返回值：true-成功；false-失败，一般情况下，失败可以认为是文件已结束。
  bool FFGETS(char *buffer, const int readsize, const char *endbz = 0);

  // 从文件中读取数据块。
  // ptr：用于存放读取的内容。
  // size：本次打算读取的字节数。
  // 返回值：本次从文件中成功读取的字节数，如果文件未结束，返回值等于size，如果文件已结束，返回值为实际读取的字节数。
  size_t Fread(void *ptr, size_t size);

  // 向文件中写入数据块。
  // ptr：待写入数据块的地址。
  // size：待写入数据块的字节数。
  // 返回值：本次成功写入的字节数，如果磁盘空间足够，返回值等于size。
  size_t Fwrite(const void *ptr, size_t size);

  // 关闭文件指针，如果存在临时文件，就删除它。
  void Close();

  ~CFile(); // 析构函数会调用Close方法。
};
