#include <avr/io.h>

int main(void) {
	DDRB = 0x04;
	DDRD &= ~0x04;
	DDRD &= ~0x40;

	while(1) {
	     if((!(PIND & 0x04)) && (!(PIND & 0x40))) PORTB |= 0x04;
	     else PORTB &= ~0x04;
	}
return 0;
}
