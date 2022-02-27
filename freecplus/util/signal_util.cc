#include "signal_util.h"

#include <signal.h>
#include <unistd.h>

// 关闭全部的信号和输入输出
void CloseIOAndSignal() {
  int ii = 0;

  for (ii = 0; ii < 100; ii++) {
	signal(ii, SIG_IGN);
	close(ii);
  }
}