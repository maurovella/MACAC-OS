#include <stdint.h>
#include <speaker.h>

void beep(uint64_t n_frequency) {
  _beep_start(1193180 / n_frequency);
}

void stop_beep() {
  _beep_stop();
}
