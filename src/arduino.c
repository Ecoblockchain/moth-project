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
    mraa_i2c_address(arduino_context, 0x33);
}

void analog_update() {
    printf("analog");
  mraa_i2c_read(arduino_context, arduino_buffer, 8);
  save_log_value(ANALOG_0, bytesToInt(arduino_buffer[0], arduino_buffer[1]), 1);
  save_log_value(ANALOG_1, bytesToInt(arduino_buffer[2], arduino_buffer[3]), 1);
  save_log_value(ANALOG_2, bytesToInt(arduino_buffer[4], arduino_buffer[5]), 1);
  save_log_value(ANALOG_3, bytesToInt(arduino_buffer[6], arduino_buffer[7]), 1);
    printf("analog-done\n");
}
