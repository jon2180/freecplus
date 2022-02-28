#include "timer.h"

#include <cstring>
#include <cstdlib>
#include <sys/time.h>

#include <cstdio>

namespace freecplus {

Timer::Timer() {
  memset(&m_start, 0, sizeof(struct timeval));
  memset(&m_end, 0, sizeof(struct timeval));

  // 开始计时
  Start();
}

// 开始计时
void Timer::Start() { gettimeofday(&m_start, nullptr); }

// 计算已逝去的时间，单位：秒，小数点后面是微秒
double Timer::Elapsed() {

  gettimeofday(&m_end, nullptr);

  double dstart, dend;

  dstart = dend = 0;

  char strtemp[51];
  memset(strtemp, 0, sizeof(strtemp));
  snprintf(strtemp, 30, "%ld.%d", m_start.tv_sec, m_start.tv_usec);
  dstart = atof(strtemp);

  memset(strtemp, 0, sizeof(strtemp));
  snprintf(strtemp, 30, "%ld.%d", m_end.tv_sec, m_end.tv_usec);
  dend = atof(strtemp);

  // 重新开始计时
  Start();

  return dend - dstart;
}

} // namespace freecplus
