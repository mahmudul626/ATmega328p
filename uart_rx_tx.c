#include <avr/io.h>

/*---------------------UART Learning project---------------------*/
/*led on/off and geting message in serial monitor using RX and TX*/
/*Devloper: Md. Mahmudul Hasan Mabud     date: (30 apr 2026)*/

void uart_init(void) {
	       UBRR0 = 0x67;  // Baud rate 9600
	       UCSR0B = 0x18; // Enable TX and RX
	       UCSR0C = 0x06; // Data pack size 8bit
}

void send(char data) {
	 while(!(UCSR0A & 0x20));
UDR0 = data;
}

void send_str(char *str) {
	      while(*str) {
		   send(*str++);
	      }
}

int main(void) {
	 DDRB = 0x20;
	 uart_init();
	 while(1) {
	       if(UCSR0A & 0x80) {
		  char data = UDR0;
		  if(data == '1') {
		     PORTB = PORTB | 0x20;
		     send(data);
		     send_str(" -> Led on!\r\n");
		  } else if(data == '0') {
		     PORTB = PORTB & ~0x20;
		     send(data);
		     send_str(" -> Led off!\r\n");
		  } else {
			send_str("Invalid command!\r\n");
		  }
 	       }
	}
return 0;
}
