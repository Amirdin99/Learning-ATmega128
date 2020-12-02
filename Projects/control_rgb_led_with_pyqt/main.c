#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "uart.h"

unsigned char rgb[3] = {0};
int cnt = 0;
int done = 0;

int main(void)
{
	DDRB = 0xFF; //LED용
	TCCR0 = (1<<CS02) | (1<<WGM01) | (1<<WGM00) | (1<<COM01);
	TCCR2 = (1<<CS22) | (1<<WGM21) | (1<<WGM20) | (1<<COM21);
	
	TCCR1A = (1<<WGM10)|(1<<COM1A1);
	TCCR1B = (1<<CS11)|(CS10);
	
	TCNT2 = 0;
	TCNT1 = 0;
	TCNT0 = 0;
	
	uart_init(BAUDRATE(9600));
	
	while (1)
	{
		if (done)
		{
			OCR0 = rgb[0];
			OCR1A = rgb[1];
			OCR2 = rgb[2];
			done = 0;
		}
		_delay_ms(20);	
	}
}

ISR(USART0_RX_vect)
{
	rgb[cnt++] = UDR0;
	if (cnt == 3)
	{
		uart_write(rgb[0]);
		uart_write(rgb[1]);
		uart_write(rgb[2]);
		cnt = 0;
		done = 1;
	}
}