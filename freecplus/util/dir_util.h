#pragma once

namespace freecplus {

///////////////////////////////////// /////////////////////////////////////
// 目录操作相关的类

// 根据绝对路径的文件名或目录名逐级的创建目录。
// path_or_filename：绝对路径的文件名或目录名。
// is_filename：说明pathorfilename的类型，true-pathorfilename是文件名，否则是目录名，缺省值为true。
// 返回值：true-成功，false-失败，如果返回失败，原因有大概有三种情况：1）权限不足；2）pathorfilename参数不是合法的文件名或目录名；3）磁盘空间不足。
bool MKDIR(const char *path_or_filename, bool is_filename = true);

}  // namespace freecplus