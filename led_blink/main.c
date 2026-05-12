#include "led.h"

int main() {
	led_init();

	while(1) {
		led_on();
		delay(1000);
		led_off();
		delay(1000);
	}
}
