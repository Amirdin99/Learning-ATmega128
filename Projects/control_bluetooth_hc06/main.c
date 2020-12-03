#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "uart.h"

int main(void)
{
	unsigned int cnt= 0;
	uart0_init(BAUDRATE(9600));	// Serial connection
	uart1_init(BAUDRATE(9600)); // Bluetooth connection
	while (1)
	{
		uart1_write((cnt % 10)+'0');
		uart0_write((cnt % 10)+'0');
		cnt++;
		_delay_ms(1000);
	}
}

ISR(USART0_RX_vect)
{
	unsigned char buf = UDR0;
	//uart0_write(buf - 'a' + 'A');
}

ISR(USART1_RX_vect)
{
	unsigned char buf = UDR1;
	uart0_write(buf);
}