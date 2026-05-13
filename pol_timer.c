#include <avr/io.h>
	//init()
	//set mode :normal,ctc,fast pwm etc.
	//set timer overflow limit
	//then set prescler
	//
	//if OCR0A 's value matched then TIDR0's OCF0A will high
	//and we check this register on main() if high then counter++ if count >= 100 then PB5 high
void timer0_init() {
	TCCR0A = (1<<WGM01);
	OCR0A = 156; //almost 10ms 
	TCCR0B = (1<<CS02) | (1<<CS00);
}

int main() {
	timer0_init();
	DDRB = (1<<DDB5);
	int count = 0;
	while(1) {
		if(TIFR0 & (1<<OCF0A)) {
			TIFR0 |= (1<<OCF0A);
			count++;

			if(count >= 100) {
				PORTB ^= (1<<PORTB5);
				count = 0;
			}
		}
	}
}
