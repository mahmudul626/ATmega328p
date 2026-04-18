#include <avr/io.h>

int main(void) {
	DDRB = 0x20;
	DDRD &= ~0x04;
	while(1) {
	     if(!(PIND & 0x04)) PORTB |= 0x20;
	     else PORTB &= 0xDF;
	}
return 0;
}
