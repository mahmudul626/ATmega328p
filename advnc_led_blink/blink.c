#include "blink.h"
#define BUFFER_SIZE 64

volatile uint8_t flag = 0;

int main() {
	uart_init();
	timer0_init();
	sei();
	DDRB |= (1<<PB5);
	char cmd_buffer[BUFFER_SIZE];
	uint8_t i = 0;
	while(1) {
		char c = rx_dequeue();
		if(c == '\0') continue;
		if(c == '\r') {
			cmd_buffer[i] = '\0';
			i = 0;
			if(strcmp(cmd_buffer, "led_blink") == 0) {
				if(flag) {
					flag = 0;
					PORTB ^= (1<<PB5);
					if(PORTB & (1<<PB5)) {
						send_str("LED ON\r\n");
					} else {
						send_str("LED OFF\r\n");
					}
				}
			}
		}else if(i < BUFFER_SIZE-1) {
			cmd_buffer[i++] = c;
			tx_enqueue(c);
		}
	}
}
