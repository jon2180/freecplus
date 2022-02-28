#pragma once

#include <ctime>
#include <cstdlib>

namespace freecplus {

// 这是一个精确到微秒的计时器。
class Timer {
private:
  struct timeval m_start; // 开始计时的时间。
  struct timeval m_end;   // 计时完成的时间。

  // 开始计时。
  void Start();

public:
  Timer(); // 构造函数中会调用Start方法。

  // 计算已逝去的时间，单位：秒，小数点后面是微秒。
  double Elapsed();
};

} // namespace freecplus