#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
int main() 
{
	unsigned int i;
	DDRB |= 1<<PB5 | 1<<PB6;
	TCCR1B |= 1<<WGM13 | 1<<WGM12;
	TCCR1A |= 1<<WGM11 | 1<<WGM10;
	TCCR1B |= 1<<CS12; 
	TCCR1A |= 1<<COM1A1 | 1<<COM1B1; 
	OCR1A = 625-1; 
	for (i = 0; i < 10; i++) 
	{
		OCR1B = i * 0.1 * (625-1); _delay_ms(2000);
	}
	while (1);
	return 0; 
}
