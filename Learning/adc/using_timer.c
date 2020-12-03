#include <avr/io.h>
#include <avr/interrupt.h>
volatile unsigned int result;
int main()
{
	DDRA |= 1 << PA0;
	TCCR1B |= 1 << CS12;
	TIMSK |= 1 << TOIE1;
	ADCSRA |= 1 << ADEN | 1 << ADFR | 1 << ADIE | 7;
	sei();
	ADCSRA |= 1 << ADSC;
	while (1);
	return 0;
}

ISR(ADC_vect)
{
	result = ADC;
}

ISR(TIMER1_OVF_vect)
{
	if (result >= 768)
		PORTA |= 1 << PA0;
	else PORTA &= ~(1 << PA0);
	TCNT1 = 65536 - 6250;
}