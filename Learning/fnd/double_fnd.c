#define  F_CPU  16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

int mode = 0;

unsigned char seg[] =
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
	uint16_t cnt = 0;
	int16_t num = 10;
	DDRA = 0xFF;
	DDRC = 0x03;
	PORTA = seg[0];
	
	EIMSK = 0x03;
	EICRA = (1 << ISC01) | (1 << ISC00) | (1 << ISC11) | (1 << ISC10);
	sei();
	while (1)
	{
		PORTC = 0x01;
		PORTA = seg[num % 10];
		PORTC = 0x02;
		PORTA = seg[(num / 10) % 10];
		if (cnt++ > 12500 && mode)
		{
			num += mode;
			if (num < 0) num = 99;
			if (num > 99) num = 0;
			cnt = 0;
		}
	}
	return 0;
}

ISR(INT0_vect)
{
	mode = 1;
}

ISR(INT1_vect)
{
	mode = -1;
}