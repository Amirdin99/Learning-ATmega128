/*
 * lcd.c
 *
 * Created: 2020-06-24 오전 8:54:54
 *  Author: user
 */ 

#include "lcd.h"
#include <avr/interrupt.h>
#include <stdio.h>

int main()
{
	int cnt = 900;
	char str[17];
	char* pt;
	DDRA = 0xFF;
	DDRC = 0xFF;
	LCD_Init();
	
	while (1)
	{		
		LCD_Cursor(0,0);
		sprintf(str, "%04d", cnt++);		
		pt = str;
		while (*pt)
			LCD_wData(*pt++);
		
		_delay_ms(100);
	}
}