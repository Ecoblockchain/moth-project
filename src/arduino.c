#include <mraa/i2c.h>
#include "shared.h"
#include "log.h"

#define MAX_FIELDS 12

mraa_i2c_context arduino_context;
uint8_t arduino_buffer[8];

void arduino_init() {
    arduino_context = mraa_i2c_init(1);
    if (arduino_context == NULL) {
      printf("ERROR: unable to initialize arduino_context\n");
    }
    if (mraa_i2c_address(arduino_context, 0x75) != MRAA_SUCCESS) {
      printf("ERROR: unable to set arduino_context address\n");
    }
}

void analog_update() {
  if (mraa_i2c_read(arduino_context, arduino_buffer, 8) != 8) {
    printf("ERROR: unable to read bytes from arduino\n");
  }
  int i, val;
  for (i = 0; i < 4; i++) {
    val = (arduino_buffer[i * 2] << 8) | arduino_buffer[(i * 2) + 1];
    if (val < 10000) save_log_value(ANALOG_0 + i, val, 1);
  }
}
