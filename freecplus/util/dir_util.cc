#include "dir_util.h"

#include <cstring> // strncpy strlen
#include <unistd.h> // access
#include <sys/stat.h> // mkdir
#include <sys/types.h>  

// 根据绝对路径的文件名或目录名逐级的创建目录。
// pathorfilename：绝对路径的文件名或目录名。
// bisfilename：说明pathorfilename的类型，true-pathorfilename是文件名，否则是目录名，缺省值为true。
// 返回值：true-创建成功，false-创建失败，如果返回失败，原因有大概有三种情况：1）权限不足； 2）pathorfilename参数不是合法的文件名或目录名；3）磁盘空间不足。
bool MKDIR(const char *filename, bool bisfilename) {
  // 检查目录是否存在，如果不存在，逐级创建子目录
  char strPathName[301];

  int ilen = strlen(filename);

  for (int ii = 1; ii < ilen; ii++) {
	if (filename[ii] != '/')
	  continue;

	memset(strPathName, 0, sizeof(strPathName));
	strncpy(strPathName, filename, ii);

	if (access(strPathName, F_OK) == 0)
	  continue;

	if (mkdir(strPathName, 0755) != 0)
	  return false;
  }

  if (bisfilename == false) {
	if (access(filename, F_OK) != 0) {
	  if (mkdir(filename, 0755) != 0)
		return false;
	}
  }

  return true;
}

