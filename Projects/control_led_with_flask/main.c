#define F_CPU 16000000UL
#define BAUDRATE(x) ((F_CPU / 16 / x) - 1)
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void uart_init(unsigned int baud)
{
	UBRR0H = (unsigned char)(baud >> 8);
	UBRR0L = (unsigned char)baud;
	UCSR0B = (1 << TXEN0) | (1 << RXEN0) | (1<<RXCIE0);
} // 8bit, no parity, 1 stop bit, TX enable, RX ISR enable

void uart_write(unsigned char data)
{
	while (!(UCSR0A & (1 << UDRE0)));
	// wait for sending
	UDR0 = data; // send
}

void uart_string(char* str)
{
	while (*str)
		uart_write(*str++);
}

int main(void)
{
	uart_init(BAUDRATE(9600));
	sei();
	
	DDRA = 0x01;
	PORTA = 0x00;
	while(1)
	{
	}
}

ISR(USART0_RX_vect)
{
	unsigned char buf = UDR0;
	buf -= '0';
	PORTA = buf;
}