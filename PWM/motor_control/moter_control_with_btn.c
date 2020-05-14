#include <avr/io.h>
#include <avr/interrupt.h>
volatile int duty = 0;
int main()
{ 
	DDRB |= 1<<PB5 | 1<<PB6;
	TCCR1B |= 1<<WGM13 | 1<<WGM12; 
	TCCR1A |= 1<<WGM11 | 1<<WGM10; 
	TCCR1B |= 1<<CS12; 
	TCCR1A |= 1<<COM1A1 | 1<<COM1B1; 
	OCR1A = 625-1; 
	OCR1B = duty * 0.1 * (625-1); 
	PORTD |= 1<<PD0 | 1<<PD1;
	EIMSK |= 1<<INT0 | 1<<INT1; 
	EICRA |= 2<<ISC10 | 2<<ISC00; 
	sei(); 
	while (1); 
	return 0; 
} 

ISR(INT0_vect) 
{ 
	duty++; 
	if (duty > 9) 
		duty = 9; 
	OCR1B = duty * 0.1 * (625-1); 
} 

ISR(INT1_vect) 
{ 
	duty--; 
	if (duty < 0)
		duty = 0; 
	OCR1B = duty * 0.1 * (625-1); 
}