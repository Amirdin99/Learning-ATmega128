/*
 * second.c
 *
 * Created: 2020-06-24 오전 10:07:33
 *  Author: user
 */ 

#include "lcd.h"
#include <avr/interrupt.h>
#include <stdio.h>
unsigned int hour = 0, min = 0, sec = 0;
int isStop = 1;

int main()
{
	char str[17];
	DDRA = 0xFF;
	DDRC = 0xFF;
	
	EIMSK = 0x03;
	EICRA = 0x0F;
	sei();
	LCD_Init();
	LCD_Cursor(0, 3);
	LCD_wString("Stop watch");
	
	
	while (1)
	{
		LCD_Cursor(1, 3);
		sprintf(str, "%02d:%02d:%02d", hour, min, sec);
		LCD_wString(str);
		if (!isStop)
			sec++;
		if (min == 60) hour++, min = 0;
		if (sec == 60) min++, sec = 0;
		_delay_ms(100);
	}
}

ISR(INT0_vect)
{
	isStop = !isStop;
}

ISR(INT1_vect)
{
	hour = min = sec = 0;
}