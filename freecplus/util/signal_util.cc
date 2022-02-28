#include "signal_util.h"

#include <csignal>
#include <unistd.h>

namespace freecplus {

void CloseIOAndSignal() {
  int ii = 0;

  for (ii = 0; ii < 100; ii++) {
    signal(ii, SIG_IGN);
    close(ii);
  }
}

} // namespace freecplus
