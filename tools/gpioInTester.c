#include <mraa/gpio.h>
#include <stdio.h>

int main() {
  mraa_gpio_context pin;
  pin = mraa_gpio_init(45);
  mraa_gpio_dir(pin, MRAA_GPIO_IN);
  while (1) {
    printf("%i\n", mraa_gpio_read(pin));
  }
}
