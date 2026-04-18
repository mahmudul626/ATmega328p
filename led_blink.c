#include <avr/io.h>

void wait(int time) {
	 volatile int i,j;
	 for(i = 0; i < time; i++) {
	    for(j = 0; j < 1000; j++) ;
	 }
}

int main(void) {
	DDRB = 0x20;

	while(1) {
	     PORTB |= 0x20;
	     wait(1000);
	     PORTB &= 0xDF;
	     wait(1000);
	}

return 0;
}
