/*
 * GccApplication1.c
 *
 * Created: 2020-05-27 오전 10:55:52
 * Author : user
 */ 

#define  F_CPU  16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

unsigned char sw = 0;

int main(void)
{
	DDRA = 0xFF;
	PORTA = 0xFF;
	
	EIMSK = (1 << INT0);
	// using INT0 interrupt (PD0)
	
	EICRA = (1 << ISC01) | (1 << ISC00);
	// The rising edge of INT0 generates asynchronously an interrupt request.
	
	sei();

	while (1);
	return 0;
}

ISR(INT0_vect)
{
	PORTA = ~PORTA;
}