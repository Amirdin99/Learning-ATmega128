/*
 * motor.c
 *
 * Created: 2020-09-09 오전 9:13:02
 * Author : user
 */ 
#define F_CPU 16000000UL
#define BAUDRATE(x) ((F_CPU / 16 / x) - 1)

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

char reading = 0;
char isDone = 0;
char data[32] = {0,};

void uart_init(unsigned int baud)
{
	UBRR0H = (unsigned char)(baud >> 8);
	UBRR0L = (unsigned char)baud;
	UCSR0B = (1 << TXEN0) | (1 << RXEN0) | (1 << RXCIE0);
} // 8bit, no parity, 1 stop bit, TX enable, RX ISR enable

void uart_write(unsigned char data)
{
	while (!(UCSR0A & (1 << UDRE0)));
	// wait for sending
	UDR0 = data; // send
}

void uart_string(char* data)
{
	while (*data)
		uart_write(*data++);
}

int main(void)
{
    DDRA = 0xFF;
	PORTA = 0x00;
	DDRB |= (1<<DDB4);
	TCCR0 |= (1<<CS02) | (1<<WGM01) | (1<<WGM00) | (1<<COM01);
	TCNT0 = 0;
	PORTA = 0x01;
	OCR0 = 0;
	uart_init(BAUDRATE(9600));
	sei();
	while (1)
	{
		if (isDone)
		{
			OCR0 = atoi(data);
			isDone = 0;
		}
		_delay_ms(10);
	}
}

ISR(USART0_RX_vect)
{
	unsigned char buf = UDR0;
	if (buf == 0x02)
	{
		reading = 1;
		return;
	}
	if (reading)
	{
		if (buf == 0x03)
		{
			data[reading - 1] = 0;
			reading = 0;
			isDone = 1;
			return;
		}
		data[reading - 1] = buf;
		reading++;
	}
}