#include <avr/io.h>

int main(void) 
{
	unsigned char table[] = 
	{
		   0b11111100, // FND FONT0
		   0b01100000, // FND FONT1
		   0b11011010, // FND FONT2
		   0b11110010, // FND FONT3
		   0b01100110, // FND FONT4
		   0b10110110, // FND FONT5
		   0b10111110, // FND FONT6
		   0b11100000, // FND FONT7
		   0b11111110, // FND FONT8
		   0b11110110  // FND FONT9
	};

	DDRA = 0xFF;
	PORTA = table[1];	
	return 1;
}