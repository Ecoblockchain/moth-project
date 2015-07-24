#include "shared.h"

void* i2cRead() {
  initSonar();

  pingSonar(0);
  usleep(40000);
  pingSonar(1);
  usleep(40000);
  while (1) {
    updateSonar(0);
    pingSonar(2);
    usleep(40000);
    updateSonar(1);
    pingSonar(3);
    usleep(40000);
    updateSonar(2);
    pingSonar(0);
    usleep(40000);
    updateSonar(3);
    pingSonar(1);
    usleep(40000);
  }

  return NULL;
}
