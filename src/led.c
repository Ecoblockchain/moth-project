#include <mraa/gpio.h>

void * statusLED() {
	mraa_gpio_context led;

	led = mraa_gpio_init(15);

	mraa_gpio_dir(led, MRAA_GPIO_OUT_HIGH);

	while (1) {
		mraa_gpio_write(led, 1);
	}

	return NULL;
}
