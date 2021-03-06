#pragma once

#include <cstdio>

namespace freecplus {

using std::FILE;

/**
 * 删除目录中的文件，类似Linux系统的rm命令
 * @param filename 待删除的文件名，建议采用绝对路径的文件名，例如/tmp/root/data.xml
 * @param times 执行删除文件的次数，缺省是1，建议不要超过3，从实际应用的经验看来，如果删除文件第1次不成功，再尝试
 * @return 返回值：true-删除成功；false-删除失败，失败的主要原因是权限不足。
 * @note 2次是可以的，更多就意义不大了。还有，如果执行删除失败，usleep(100000)后再重试。在应用开发中，可以用REMOVE函数代替remove库函数
 */
bool RemoveFile(const char *filename, int times = 1);

/**
 * 重命名文件，类似Linux系统的mv命令。
 * @param src_filename 原文件名，建议采用绝对路径的文件名。
 * @param dst_filename 目标文件名，建议采用绝对路径的文件名。
 * @param times 执行重命名文件的次数，缺省是1，建议不要超过3，从实际应用的经验看来，如果重命名文件第1次不成功，再尝试2次是可以的，更多次就意义不大了。还有，如果执行重命名失败，usleep(100000)后再重试。
 * @return true-成功；false-失败，失败的主要原因是权限不足或磁盘空间不够，如果原文件和目标文件不在同一个磁盘分区，重命名也可能失败。
 * @note 注意，在重命名文件之前，会自动创建destfilename参数中包含的目录。在应用开发中，可以用RENAME函数代替rename库函数
 */
bool RenameFile(const char *src_filename, const char *dst_filename, int times = 1);

/**
 *
 * @param src_filename 原文件名，建议采用绝对路径的文件名。
 * @param dst_filename 目标文件名，建议采用绝对路径的文件名。
 * @return true-成功；false-失败，失败的主要原因是权限不足或磁盘空间不够。
 * 1）在复制名文件之前，会自动创建destfilename参数中的目录名。
 * 2）复制文件的过程中，采用临时文件命名的方法，复制完成后再改名为destfilename，避免中间状态的文件被读取。
 * 3）复制后的文件的时间与原文件相同，这一点与Linux系统cp命令不同。
 */
bool CopyFile(const char *src_filename, const char *dst_filename);

/**
 * 获取文件的大小。
 * @param filename 待获取的文件名，建议采用绝对路径的文件名。
 * @return 如果文件不存在或没有访问权限，返回-1，成功返回文件的大小，单位是字节。
 */
size_t GetFileSize(const char *filename);

/**
 *
 * @param filename 待获取的文件名，建议采用绝对路径的文件名。
 * @param mtime 用于存放文件的时间，即stat结构体的st_mtime。
 * @param fmt 设置时间的输出格式，与LocalTime函数相同，但缺省是"yyyymmddhh24miss"。
 * @return 如果文件不存在或没有访问权限，返回false，成功返回true。
 */
bool GetFileModifyTime(const char *filename, char *mtime, const char *fmt = nullptr);

/**
 * 重置文件的修改时间属性。
 * @param filename 待重置的文件名，建议采用绝对路径的文件名。
 * @param mtime 字符串表示的时间，格式不限，但一定要包括yyyymmddhh24miss，一个都不能少，顺序也不能变。
 * @return true-成功；false-失败，失败的原因保存在errno中
 */
bool SetFileModifyTime(const char *filename, const char *mtime);

/**
 * 打开文件。
 * @param filename FOPEN函数调用fopen库函数打开文件，如果文件名中包含的目录不存在，就创建目录。
 * @param mode
 * @return
 * @note FOPEN函数的参数和返回值与fopen函数完全相同。在应用开发中，用FOPEN函数代替fopen库函数。
 */
FILE *OpenFile(const char *filename, const char *mode);

/**
 * 从文本文件中读取一行。
 * @param fp 已打开的文件指针。
 * @param buffer 用于存放读取的内容，buffer必须大于readsize+1，否则可能会造成读到的数据不完整或内存的溢出。
 * @param read_size 本次打算读取的字节数，如果已经读取到了行结束标志，函数返回。
 * @param endbz 行内容结束的标志，缺省为空，表示行内容以"\n"为结束标志。
 * @return true-成功；false-失败，一般情况下，失败可以认为是文件已结束。
 */
bool GetLineFromFile(const FILE *fp, char *buffer, int read_size, const char *endbz = 0);

} // namespace freecplus