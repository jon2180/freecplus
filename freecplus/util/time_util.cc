#include "time.h"

#include <ctime>
#include <cstring>
#include <cstdio>

#include "string_util.h"

// 把整数表示的时间转换为字符串表示的时间。
// ltime：整数表示的时间。
// stime：字符串表示的时间。
// fmt：输出字符串时间stime的格式，与LocalTime函数的fmt参数相同，如果fmt的格式不正确，stime将为空。
void timetostr(const time_t ltime, char *stime, const char *fmt) {
  if (stime == 0)
	return;

  strcpy(stime, "");

  struct tm sttm = *localtime(&ltime);

  sttm.tm_year = sttm.tm_year + 1900;
  sttm.tm_mon++;

  if (fmt == 0) {
	snprintf(stime, 20, "%04u-%02u-%02u %02u:%02u:%02u", sttm.tm_year,
			 sttm.tm_mon, sttm.tm_mday, sttm.tm_hour,
			 sttm.tm_min, sttm.tm_sec);
	return;
  }

  if (strcmp(fmt, "yyyy-mm-dd hh24:mi:ss") == 0) {
	snprintf(stime, 20, "%04u-%02u-%02u %02u:%02u:%02u", sttm.tm_year,
			 sttm.tm_mon, sttm.tm_mday, sttm.tm_hour,
			 sttm.tm_min, sttm.tm_sec);
	return;
  }

  if (strcmp(fmt, "yyyy-mm-dd hh24:mi") == 0) {
	snprintf(stime, 17, "%04u-%02u-%02u %02u:%02u", sttm.tm_year,
			 sttm.tm_mon, sttm.tm_mday, sttm.tm_hour,
			 sttm.tm_min);
	return;
  }

  if (strcmp(fmt, "yyyy-mm-dd hh24") == 0) {
	snprintf(stime, 14, "%04u-%02u-%02u %02u", sttm.tm_year,
			 sttm.tm_mon, sttm.tm_mday, sttm.tm_hour);
	return;
  }

  if (strcmp(fmt, "yyyy-mm-dd") == 0) {
	snprintf(stime, 11, "%04u-%02u-%02u", sttm.tm_year, sttm.tm_mon, sttm.tm_mday);
	return;
  }

  if (strcmp(fmt, "yyyy-mm") == 0) {
	snprintf(stime, 8, "%04u-%02u", sttm.tm_year, sttm.tm_mon);
	return;
  }

  if (strcmp(fmt, "yyyymmddhh24miss") == 0) {
	snprintf(stime, 15, "%04u%02u%02u%02u%02u%02u", sttm.tm_year,
			 sttm.tm_mon, sttm.tm_mday, sttm.tm_hour,
			 sttm.tm_min, sttm.tm_sec);
	return;
  }

  if (strcmp(fmt, "yyyymmddhh24mi") == 0) {
	snprintf(stime, 13, "%04u%02u%02u%02u%02u", sttm.tm_year,
			 sttm.tm_mon, sttm.tm_mday, sttm.tm_hour,
			 sttm.tm_min);
	return;
  }

  if (strcmp(fmt, "yyyymmddhh24") == 0) {
	snprintf(stime, 11, "%04u%02u%02u%02u", sttm.tm_year,
			 sttm.tm_mon, sttm.tm_mday, sttm.tm_hour);
	return;
  }

  if (strcmp(fmt, "yyyymmdd") == 0) {
	snprintf(stime, 9, "%04u%02u%02u", sttm.tm_year, sttm.tm_mon, sttm.tm_mday);
	return;
  }

  if (strcmp(fmt, "hh24miss") == 0) {
	snprintf(stime, 7, "%02u%02u%02u", sttm.tm_hour, sttm.tm_min, sttm.tm_sec);
	return;
  }

  if (strcmp(fmt, "hh24mi") == 0) {
	snprintf(stime, 5, "%02u%02u", sttm.tm_hour, sttm.tm_min);
	return;
  }

  if (strcmp(fmt, "hh24") == 0) {
	snprintf(stime, 3, "%02u", sttm.tm_hour);
	return;
  }

  if (strcmp(fmt, "mi") == 0) {
	snprintf(stime, 3, "%02u", sttm.tm_min);
	return;
  }
}

/*
  取操作系统的时间，并把整数表示的时间转换为字符串表示的格式。
  stime：用于存放获取到的时间字符串。
  timetvl：时间的偏移量，单位：秒，0是缺省值，表示当前时间，30表示当前时间30秒之后的时间点，-30表示当前时间30秒之前的时间点。
  fmt：输出时间的格式，缺省是"yyyy-mm-dd hh24:mi:ss"，目前支持以下格式：
  "yyyy-mm-dd hh24:mi:ss"，此格式是缺省格式。
  "yyyymmddhh24miss"
  "yyyy-mm-dd"
  "yyyymmdd"
  "hh24:mi:ss"
  "hh24miss"
  "hh24:mi"
  "hh24mi"
  "hh24"
  "mi"
  注意：
    1）小时的表示方法是hh24，不是hh，这么做的目的是为了保持与数据库的时间表示方法一致；
    2）以上列出了常用的时间格式，如果不能满足你应用开发的需求，请修改源代码增加更多的格式支持；
    3）调用函数的时候，如果fmt与上述格式都匹配，stime的内容将为空。
*/
void LocalTime(char *stime, const char *fmt, const int timetvl) {
  if (stime == 0)
	return;

  time_t timer;

  time(&timer);
  timer = timer + timetvl;

  timetostr(timer, stime, fmt);
}



// 把字符串表示的时间转换为整数表示的时间。
// stime：字符串表示的时间，格式不限，但一定要包括yyyymmddhh24miss，一个都不能少。
// 返回值：整数表示的时间，如果stime的格式不正确，返回-1。
time_t strtotime(const char *stime) {
  char strtime[21], yyyy[5], mm[3], dd[3], hh[3], mi[3], ss[3];
  memset(strtime, 0, sizeof(strtime));
  memset(yyyy, 0, sizeof(yyyy));
  memset(mm, 0, sizeof(mm));
  memset(dd, 0, sizeof(dd));
  memset(hh, 0, sizeof(hh));
  memset(mi, 0, sizeof(mi));
  memset(ss, 0, sizeof(ss));

  PickNumber(stime, strtime, false, false);

  if (strlen(strtime) != 14)
	return -1;

  strncpy(yyyy, strtime, 4);
  strncpy(mm, strtime + 4, 2);
  strncpy(dd, strtime + 6, 2);
  strncpy(hh, strtime + 8, 2);
  strncpy(mi, strtime + 10, 2);
  strncpy(ss, strtime + 12, 2);

  struct tm time_str;

  time_str.tm_year = atoi(yyyy) - 1900;
  time_str.tm_mon = atoi(mm) - 1;
  time_str.tm_mday = atoi(dd);
  time_str.tm_hour = atoi(hh);
  time_str.tm_min = atoi(mi);
  time_str.tm_sec = atoi(ss);
  time_str.tm_isdst = 0;

  return mktime(&time_str);
}

// 把字符串表示的时间加上一个偏移的秒数后得到一个新的字符串表示的时间。
// in_stime：输入的字符串格式的时间。
// out_stime：输出的字符串格式的时间。
// timetvl：需要偏移的秒数，正数往后偏移，负数往前偏移。
// fmt：输出字符串时间out_stime的格式，与LocalTime函数的fmt参数相同。
// 注意：in_stime和out_stime参数可以是同一个变量的地址，如果调用失败，out_stime的内容会清空。
// 返回值：true-成功，false-失败，如果返回失败，可以认为是in_stime的格式不正确。
bool AddTime(const char *in_stime, char *out_stime, const int timetvl, const char *fmt) {
  if ((in_stime == 0) || (out_stime == 0))
	return false;

  time_t timer;
  if ((timer = strtotime(in_stime)) == -1) {
	strcpy(out_stime, "");
	return false;
  }

  timer = timer + timetvl;

  strcpy(out_stime, "");

  timetostr(timer, out_stime, fmt);

  return true;
}
