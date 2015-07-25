#include <mraa/i2c.h>
#include "shared.h"
#include "log.h"

#define MAX_FIELDS 12

mraa_i2c_context arduino_context;
uint8_t arduino_buffer[8];

int bytesToInt(uint8_t high, uint8_t low) {
  return (high << 8) | low;
}

void arduino_init() {
    arduino_context = mraa_i2c_init(1);
    mraa_i2c_address(arduino_context, 0x7f);
}

void analog_update() {
  mraa_i2c_read(arduino_context, arduino_buffer, 8);
  int i, val;
  for (i = 0; i < 4; i++) {
    val = bytesToInt(arduino_buffer[i * 2], arduino_buffer[(i * 2) + 1]);
    if (val < 10000) save_log_value(ANALOG_0 + i, val, 1);
  }
}
