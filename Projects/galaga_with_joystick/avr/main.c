
#include <avr/io.h>
#include "uart.h"
#include "adc.h"

int main(void)
{
	DDRC = 0x00;
	DDRA = 0xFF;
	DDRG = 0xFF;
    uart_init(BAUDRATE(9600));
	adc_init();
    while (1) 
    {
		uint16_t joy = adc_read(0);
		
		if (joy == 1023)
			uart_write('2');
		else if (joy == 0)
			uart_write('1');
		else
			uart_write('0');

		if (PINC & 0x01)
		{
			uart_write('0');	
		}
		else
		{
			uart_write('1');
			PORTG = 0x01;
			_delay_ms(50);
			PORTG = 0x00;
			continue;
		}
	
		
		_delay_ms(50);
    }
}


ISR(USART0_RX_vect)
{
	unsigned char buf = UDR0;
	switch (buf)
	{
		case 'r':
			PORTA = 0x01;
			break;
		case 'g':
			PORTA = 0x02;
			break;
		case 'b':
			PORTA = 0x04;
			break;
		default:
			break;
	}
}