#include "file.h"

#include <cstring>
#include <cstdarg>

#include "util/file_util.h"
#include "util/string_util.h"

namespace freecplus {

File::File() // 类的构造函数
{
  m_fp = nullptr;
  m_bEnBuffer = true;
  memset(m_filename, 0, sizeof(m_filename));
  memset(m_filenametmp, 0, sizeof(m_filenametmp));
}

// 关闭文件指针
void File::Close() {
  if (m_fp == nullptr)
    return;

  fclose(m_fp); // 关闭文件指针

  m_fp = nullptr;
  memset(m_filename, 0, sizeof(m_filename));

  // 如果存在临时文件，就删除它。
  if (strlen(m_filenametmp) != 0)
    remove(m_filenametmp);

  memset(m_filenametmp, 0, sizeof(m_filenametmp));
}

// 判断文件是否已打开
bool File::IsOpened() {
  if (m_fp == nullptr)
    return false;

  return true;
}

// 关闭文件指针，并删除文件
bool File::CloseAndRemove() {
  if (m_fp == nullptr)
    return true;

  fclose(m_fp); // 关闭文件指针

  m_fp = nullptr;

  if (remove(m_filename) != 0) {
    memset(m_filename, 0, sizeof(m_filename));
    return false;
  }

  memset(m_filename, 0, sizeof(m_filename));
  memset(m_filenametmp, 0, sizeof(m_filenametmp));

  return true;
}

File::~File() // 类的析构函数
{
  Close();
}

// 打开文件，参数与FOPEN相同，打开成功true，失败返回false
bool File::Open(const char *filename, const char *openmode, bool bEnBuffer) {
  Close();

  if ((m_fp = OpenFile(filename, openmode)) == nullptr)
    return false;

  memset(m_filename, 0, sizeof(m_filename));

  strncpy(m_filename, filename, 300);

  m_bEnBuffer = bEnBuffer;

  return true;
}

// 专为改名而打开文件，参数与fopen相同，打开成功true，失败返回false
bool File::OpenForRename(const char *filename, const char *openmode, bool bEnBuffer) {
  Close();

  memset(m_filename, 0, sizeof(m_filename));
  strncpy(m_filename, filename, 300);

  memset(m_filenametmp, 0, sizeof(m_filenametmp));
  SNPRINTF(m_filenametmp, sizeof(m_filenametmp), 300, "%s.tmp", m_filename);

  if ((m_fp = OpenFile(m_filenametmp, openmode)) == nullptr)
    return false;

  m_bEnBuffer = bEnBuffer;

  return true;
}

// 关闭文件并改名
bool File::CloseAndRename() {
  if (m_fp == nullptr)
    return false;

  fclose(m_fp); // 关闭文件指针

  m_fp = nullptr;

  if (rename(m_filenametmp, m_filename) != 0) {
    remove(m_filenametmp);
    memset(m_filename, 0, sizeof(m_filename));
    memset(m_filenametmp, 0, sizeof(m_filenametmp));
    return false;
  }

  memset(m_filename, 0, sizeof(m_filename));
  memset(m_filenametmp, 0, sizeof(m_filenametmp));

  return true;
}

// 调用fprintf向文件写入数据
void File::Fprintf(const char *fmt, ...) {
  if (m_fp == nullptr)
    return;

  va_list arg;
  va_start(arg, fmt);
  vfprintf(m_fp, fmt, arg);
  va_end(arg);

  if (!m_bEnBuffer)
    fflush(m_fp);
}

// 调用fgets从文件中读取一行，bDelCRT=true删除换行符，false不删除，缺省为false
bool File::Fgets(char *buffer, const int readsize, bool bdelcrt) {
  if (m_fp == nullptr)
    return false;

  memset(buffer, 0, readsize + 1); // 调用者必须保证buffer的空间足够，否则这里会内存溢出。

  if (fgets(buffer, readsize, m_fp) == nullptr)
    return false;

  if (bdelcrt) {
    DeleteRChar(buffer, '\n');
    DeleteRChar(buffer, '\r'); // 如果文件是windows格式，还要删除'\r'。
  }

  return true;
}

// 从文件文件中读取一行
// strEndStr是一行数据的结束标志，如果为空，则以换行符"\n"为结束标志。
bool File::FFGETS(char *buffer, const int readsize, const char *endbz) {
  if (m_fp == nullptr)
    return false;

  return GetLineFromFile(m_fp, buffer, readsize, endbz);
}

// 调用fread从文件中读取数据。
size_t File::Fread(void *ptr, size_t size) {
  if (m_fp == nullptr)
    return -1;

  return fread(ptr, 1, size, m_fp);
}

// 调用fwrite向文件中写数据
size_t File::Fwrite(const void *ptr, size_t size) {
  if (m_fp == nullptr)
    return -1;

  size_t tt = fwrite(ptr, 1, size, m_fp);

  if (!m_bEnBuffer)
    fflush(m_fp);

  return tt;
}

} // namespace freecplus