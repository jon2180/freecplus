#pragma once

#include "log_file.h"

namespace freecplus {

// 文件信息的数据结构
// 全路径文件名，大小，时间的结构体
struct st_fileinfo {
  char filename[301];
  int filesize;
  char mtime[21];
};

// 接收socket的对端发送过来的数据。
// sockfd：可用的socket连接。
// buffer：接收数据缓冲区的地址。
// ibuflen：本次成功接收数据的字节数。
// itimeout：接收等待超时的时间，单位：秒，缺省值是0-无限等待。
// 返回值：true-成功；false-失败，失败有两种情况：1）等待超时；2）socket连接已不可用。
bool TcpRead(int sockfd, char *buffer, int *ibuflen, int itimeout = 0);

// 向socket的对端发送数据。
// sockfd：可用的socket连接。
// buffer：待发送数据缓冲区的地址。
// ibuflen：待发送数据的字节数，如果发送的是ascii字符串，ibuflen取0，如果是二进制流数据，ibuflen为二进制数据块的大小。
// 返回值：true-成功；false-失败，如果失败，表示socket连接已不可用。
bool TcpWrite(int sockfd, const char *buffer, size_t ibuflen = 0);

// 从已经准备好的socket中读取数据。
// sockfd：已经准备好的socket连接。
// buffer：接收数据缓冲区的地址。
// n：本次接收数据的字节数。
// 返回值：成功接收到n字节的数据后返回true，socket连接不可用返回false。
bool Readn(int sockfd, char *buffer, size_t n);

// 向已经准备好的socket中写入数据。
// sockfd：已经准备好的socket连接。
// buffer：待发送数据缓冲区的地址。
// n：待发送数据的字节数。
// 返回值：成功发送完n字节的数据后返回true，socket连接不可用返回false。
bool Writen(int sockfd, const char *buffer, size_t n);

// 以上是socket通信的函数和类

// 向socket的对端发送文件。
// sockfd：可用的socket连接。
// stfileinfo：待发送的文件信息，用struct st_fileinfo表示。
// logfile：用于记录错误的日志文件的指针，如果为0，发生了错误不记录日志。
// 返回值：true-成功；false-发送失败，失败的原因有两种：1）sockfd不可用，2）待发送的文件不存在或权限不足。
bool SendFile(int sockfd, struct st_fileinfo *stfileinfo, LogFile *logfile = 0);

// 接收socket的对端发送过来的文件。
// sockfd：可用的socket连接。
// stfileinfo：待接收的文件信息，用struct st_fileinfo表示。
// logfile：用于记录错误的日志文件的指针，如果为0，发生了错误不记录日志。
// 返回值：true-成功；false-接收失败，失败的原因有两种：1）sockfd不可用，2）待发送的文件不存在或权限不足。
bool RecvFile(int sockfd, struct st_fileinfo *stfileinfo, LogFile *logfile = 0);

} // namespace freecplus