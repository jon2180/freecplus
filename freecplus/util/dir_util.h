/**
 * 目录操作相关的类
 */

#pragma once

namespace freecplus {

/**
 * 根据绝对路径的文件名或目录名逐级的创建目录
 * @param path_or_filename 绝对路径的文件名或目录名
 * @param is_filename 说明 path_or_filename 的类型，true - path_or_filename是文件名，否则是目录名，缺省值为true
 * @return 返回值：
 * true-成功
 * false-失败，如果返回失败，原因有大概有三种情况：
 *  1）权限不足；
 *  2）pathorfilename参数不是合法的文件名或目录名；
 *  3）磁盘空间不足。
 */
bool MakeDir(const char *path_or_filename, bool is_filename = true);

}  // namespace freecplus