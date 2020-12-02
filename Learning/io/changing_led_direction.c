#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	DDRA = 0xFF;
	DDRD = 0x00;
	PORTA = 0x00;

	int sw = 0;
	unsigned char led = 1;
	while (1)
	{
		if ((PIND & 0x01) == 0x00)
		{
			sw = 1;
		}
		else if ((PIND & 0x02) == 0x00)
		{
			if (!sw)
				led = 0x80;
			sw = 2;
		}
		if (!sw)
			continue;	
		PORTA = led;
		led <<= (sw == 1);
		led >>= (sw != 1);
		if (!led)
			led = (sw == 1) ? 0x01 : 0x80;
		_delay_ms(100);
	}
	return 0;
}
