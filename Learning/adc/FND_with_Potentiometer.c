#define  F_CPU  16000000UL
#include <avr/io.h>
#include <util/delay.h>


unsigned char table[] =
{
	0b11000000,
	0b11111001,
	0b10100100,
	0b10110000,
	0b10011001,
	0b10010010,
	0b10000011,
	0b11111000,
	0b10000000,
	0b10011000
};

int main(void)
{
	unsigned short adc;
	DDRA = 0xFF;
	PORTA = table[0];
	
	ADMUX = 0x00; // using ADC0 (PF0)
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); 
	// 1000 0111. ADC Enable and Division Factor to 128
	
	while (1)
	{
		ADCSRA |= 1 << ADSC; // Start Conversion
		adc = ADCL | (ADCH << 8);
		// ADCL is 0~7 bits. ADCH is 8~9 bits.
		
		PORTA = table[adc * 9 / 1023];
	}
	return 0;
}