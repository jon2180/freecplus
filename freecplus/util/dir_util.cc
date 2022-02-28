#include "dir_util.h"

#include <sys/stat.h>  // mkdir
#include <unistd.h>  // access

#include <cstring>  // strncpy strlen

namespace freecplus {

bool MakeDir(const char *filename, bool is_filename) {
  // 检查目录是否存在，如果不存在，逐级创建子目录
  char str_path_name[301];
  size_t filename_len = strlen(filename);

  for (size_t i = 1; i < filename_len; i++) {
    if (filename[i] != '/') continue;
    memset(str_path_name, 0, sizeof(str_path_name));
    strncpy(str_path_name, filename, i);
    if (access(str_path_name, F_OK) == 0) continue;
    if (mkdir(str_path_name, 0755) != 0) return false;
  }

  if (!is_filename) {
    if (access(filename, F_OK) != 0) {
      if (mkdir(filename, 0755) != 0) return false;
    }
  }

  return true;
}

}  // namespace freecplus
