#pragma once

#include <string>
#include <vector>

using std::vector;
using std::string;

// 获取某目录及其子目录中的文件列表信息。
class CDir {
public:
  char m_DirName[301];        // 目录名，例如：/tmp/root。
  char m_FileName[301];       // 文件名，不包括目录名，例如：data.xml。
  char m_FullFileName[301];   // 文件全名，包括目录名，例如：/tmp/root/data.xml。
  int m_FileSize;            // 文件的大小，单位：字节。
  char m_ModifyTime[21];      // 文件最后一次被修改的时间，即stat结构体的st_mtime成员。
  char m_CreateTime[21];      // 文件生成的时间，即stat结构体的st_ctime成员。
  char m_AccessTime[21];      // 文件最后一次被访问的时间，即stat结构体的st_atime成员。
  char m_DateFMT[25];         // 文件时间显示格式，由SetDateFMT方法设置。

  vector<string> m_vFileName; // 存放OpenDir方法获取到的绝对路径文件名清单。
  int m_pos;                  // 已读取m_vFileName容器的位置，每调用一次ReadDir方法m_pos加1。

  CDir();  // 构造函数。

  void initdata(); // 初始化成员变量。

  // 设置文件时间的格式，支持"yyyy-mm-dd hh24:mi:ss"和"yyyymmddhh24miss"两种，缺省是前者。
  void SetDateFMT(const char *in_DateFMT);

  // 打开目录，获取目录中的文件列表信息，存放于m_vFileName容器中。
  // in_DirName，待打开的目录名，采用绝对路径，如/tmp/root。
  // in_MatchStr，待获取文件名的匹配规则，不匹配的文件被忽略，具体请参见freecplus框架的MatchStr函数。
  // in_MaxCount，获取文件的最大数量，缺省值为10000个。
  // bAndChild，是否打开各级子目录，缺省值为false-不打开子目录。
  // bSort，是否对获取到的文件列表（即m_vFileName容器中的内容）进行排序，缺省值为false-不排序。
  // 返回值：true-成功，false-失败，如果in_DirName参数指定的目录不存在，OpenDir方法会创建该目录，如果创建失败，返回false，如果当前用户对in_DirName目录下的子目录没有读取权限也会返回false。
  bool OpenDir(const char *in_DirName,
			   const char *in_MatchStr,
			   const unsigned int in_MaxCount = 10000,
			   const bool bAndChild = false,
			   bool bSort = false);

  // 这是一个递归函数，被OpenDir()的调用，在CDir类的外部不需要调用它。
  bool _OpenDir(const char *in_DirName, const char *in_MatchStr, const unsigned int in_MaxCount, const bool bAndChild);

  // 从m_vFileName容器中获取一条记录（文件名），同时获取该文件的大小、修改时间等信息。
  // 调用OpenDir方法时，m_vFileName容器被清空，m_pos归零，每调用一次ReadDir方法m_pos加1。
  // 当m_pos小于m_vFileName.size()，返回true，否则返回false。
  bool ReadDir();

  ~CDir();  // 析构函数。
};
