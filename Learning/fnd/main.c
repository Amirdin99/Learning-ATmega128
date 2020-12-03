/*
 * GccApplication1.c
 *
 * Created: 2020-05-27 오전 10:55:52
 * Author : user
 */ 

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
	DDRE = 0xFF;
	PORTE = table[1];
	
	while (1)
	{
		for (int i = 0; i < 10; i++)
		{
			PORTE = table[i];
			_delay_ms(400);
		}	
	}
	return 0;
}
