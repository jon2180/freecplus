#include "log_file.h"

#include <cstring>

#include "util/file_util.h"
#include "util/time_util.h"

#include "file.h"
#include "time.h"

CLogFile::CLogFile(const long MaxLogSize) {
  m_tracefp = 0;
  memset(m_filename, 0, sizeof(m_filename));
  memset(m_openmode, 0, sizeof(m_openmode));
  m_bBackup = true;
  m_bEnBuffer = false;
  m_MaxLogSize = MaxLogSize;
  if (m_MaxLogSize < 10)
	m_MaxLogSize = 10;
}

CLogFile::~CLogFile() {
  Close();
}

void CLogFile::Close() {
  if (m_tracefp != 0) {
	fclose(m_tracefp);
	m_tracefp = 0;
  }

  memset(m_filename, 0, sizeof(m_filename));
  memset(m_openmode, 0, sizeof(m_openmode));
  m_bBackup = true;
  m_bEnBuffer = false;
}

// 打开日志文件。
// filename：日志文件名，建议采用绝对路径，如果文件名中的目录不存在，就先创建目录。
// openmode：日志文件的打开方式，与fopen库函数打开文件的方式相同，缺省值是"a+"。
// bBackup：是否自动切换，true-切换，false-不切换，在多进程的服务程序中，如果多个进行共用一个日志文件，bBackup必须为false。
// bEnBuffer：是否启用文件缓冲机制，true-启用，false-不启用，如果启用缓冲区，那么写进日志文件中的内容不会立即写入文件，缺省是不启用。
bool CLogFile::Open(const char *filename, const char *openmode, bool bBackup, bool bEnBuffer) {
  // 如果文件指针是打开的状态，先关闭它。
  Close();

  strcpy(m_filename, filename);
  m_bEnBuffer = bEnBuffer;
  m_bBackup = bBackup;
  if (openmode == 0)
	strcpy(m_openmode, "a+");
  else
	strcpy(m_openmode, openmode);

  if ((m_tracefp = FOPEN(m_filename, m_openmode)) == 0)
	return false;

  return true;
}

// 如果日志文件大于100M，就把当前的日志文件备份成历史日志文件，切换成功后清空当前日志文件的内容。
// 备份后的文件会在日志文件名后加上日期时间。
// 注意，在多进程的程序中，日志文件不可切换，多线的程序中，日志文件可以切换。
bool CLogFile::BackupLogFile() {
  if (m_tracefp == 0)
	return false;

  // 不备份
  if (m_bBackup == false)
	return true;

  fseek(m_tracefp, 0L, 2);

  if (ftell(m_tracefp) > m_MaxLogSize * 1024 * 1024) {
	fclose(m_tracefp);
	m_tracefp = 0;

	char strLocalTime[21];
	memset(strLocalTime, 0, sizeof(strLocalTime));
	LocalTime(strLocalTime, "yyyymmddhh24miss");

	char bak_filename[301];
	memset(bak_filename, 0, sizeof(bak_filename));
	snprintf(bak_filename, 300, "%s.%s", m_filename, strLocalTime);
	rename(m_filename, bak_filename);

	if ((m_tracefp = FOPEN(m_filename, m_openmode)) == 0)
	  return false;
  }

  return true;
}

// 把内容写入日志文件，fmt是可变参数，使用方法与printf库函数相同。
// Write方法会写入当前的时间，WriteEx方法不写时间。
bool CLogFile::Write(const char *fmt, ...) {
  if (m_tracefp == 0)
	return false;

  if (BackupLogFile() == false)
	return false;

  char strtime[20];
  LocalTime(strtime);

  va_list ap;
  va_start(ap, fmt);
  fprintf(m_tracefp, "%s ", strtime);
  vfprintf(m_tracefp, fmt, ap);
  va_end(ap);

  if (m_bEnBuffer == false)
	fflush(m_tracefp);

  return true;
}

// 把内容写入日志文件，fmt是可变参数，使用方法与printf库函数相同。
// Write方法会写入当前的时间，WriteEx方法不写时间。
bool CLogFile::WriteEx(const char *fmt, ...) {
  if (m_tracefp == 0)
	return false;

  va_list ap;
  va_start(ap, fmt);
  vfprintf(m_tracefp, fmt, ap);
  va_end(ap);

  if (m_bEnBuffer == false)
	fflush(m_tracefp);

  return true;
}
