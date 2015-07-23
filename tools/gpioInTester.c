#include <mraa/gpio.h>
#include <unistd.h>
#include <stdio.h>

int main() {
  mraa_gpio_context pin;
  pin = mraa_gpio_init(45);
  mraa_gpio_dir(pin, MRAA_GPIO_IN);
  int status = mraa_gpio_read(pin);
  int otherStatus;
  while (1) {
    usleep(10000);
    otherStatus = mraa_gpio_read(pin);
    if (status != otherStatus)
      printf("Change detected!\n");
    status = mraa_gpio_read(pin);
  }
}
