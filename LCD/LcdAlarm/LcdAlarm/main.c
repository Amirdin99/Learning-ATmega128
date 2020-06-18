/*
 * LcdAlarm.c
 *
 * Created: 2020-06-16 오후 10:01:05
 * Author : user
 */ 

#include "lcd.h"
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#define LONGTIME 1000UL
#define MAIN 0
#define SETTIME 1
#define ALARM 2
#define WATCH 3

char time_idx[] = {1,4,6,7,9,10,20,21,23,24,26,27};
char alarm_idx[] = {4,5,7,8};
char time_str[33] = {0};
char buf[20];
time_t time_sec = 0;
time_t alarm_sec = 0;
int mode = 0;
int set_idx = 0;
int alarm_set_idx = 0;
char set_blink = 0;
int isAlarmOn = 0;
int isAlarmSetting= 0;
int al_hour, al_min;

void print_lcd()
{
	LCD_wBCommand(0x80 | 0x00);
	struct tm* time = gmtime(&time_sec);
	sprintf(time_str, " %d-%02d-%02d", time->tm_year + 1900, time->tm_mon + 1, time->tm_mday);
	LCD_wString(time_str);
	LCD_wBCommand(0x80 | 0x40);
	int hour = time->tm_hour > 12 ? time->tm_hour - 12 : time->tm_hour;
	sprintf(time_str + 16, " %s %02d:%02d:%02d    ", (time->tm_hour > 12 ? "PM" : "AM"), hour, time->tm_min, time->tm_sec);
	LCD_wString(time_str + 16);
}

void alarm_set()
{
	char str[17] = {0};
	LCD_wBCommand(0x80 | 0x00);
	LCD_wString(" ALARM SET..    ");
	LCD_wBCommand(0x80 | 0x40);
	int hour = al_hour > 12 ? al_hour - 12 : al_hour;
	sprintf(str, " %s %02d:%02d %s", (al_hour > 12 ? "PM" : "AM"), hour, al_min, (isAlarmOn ? "ON" : "OFF"));
	LCD_wString(str);
}

int checkAlarm()
{
	struct tm* now = gmtime(&time_sec);
	return al_hour == now->tm_hour && al_min == now->tm_min;
}

int main(void)
{
	struct tm init;
	init.tm_year = 2000 - 1900, init.tm_mon=0, init.tm_mday = 1;
	init.tm_hour=0, init.tm_min=0, init.tm_sec = 0;
	init.tm_isdst = -1;
	time_sec = mktime(&init);
	int cnt=0;
	
	DDRA = 0xFF;
	DDRC = 0xFF;
	DDRD = 0x00;
	DDRB = 0x01;
	
	EIMSK = 0x0F; // enable 0~3 interrupts
	EICRA = 0xAA; // falling edge
	sei();
	LCD_Init();

    while (1) 
    {
		if(!(PIND & (1 << PIND4)) && mode == MAIN) 
		{
			mode = WATCH;
		}
		
		if (checkAlarm() && isAlarmOn)
		{
			PORTB = 0x01;
			LCD_Cursor(0, 0);
			LCD_wString("jnj");
		}
		else
		{
			PORTB = 0x00;
		}
	
		if (mode == MAIN)
		{
			time_sec++;
			print_lcd();
		}
		else if (mode == SETTIME)
		{
			LCD_wBCommand(0x80 | 0x00);
			LCD_wString(time_str);
			LCD_wBCommand(0x80 | 0x40);
			LCD_wString(time_str + 16);
			_delay_ms(500);
			for (int i = time_idx[set_idx]; i <= time_idx[set_idx + 1]; i++)
			{	
				LCD_Cursor(i / 16, i % 16);
				LCD_wString(" ");
			}
			_delay_ms(500);
		}
		else if (mode == WATCH) 
		{
			time_t stop = 0;
			struct tm* t = gmtime(&stop);
			LCD_wBCommand(0x80 | 0x00);
			LCD_wString(" STOP Watch.. ");
			LCD_wBCommand(0x80 | 0x40);
			sprintf(buf, " %02d:%02d(sec)  ", t->tm_min, t->tm_sec);
			LCD_wString(buf);
			while (PIND & (1 << PIND2));
			_delay_ms(500);
			while((PIND & (1 << PIND2)))
			{
				struct tm* t = gmtime(&stop);
				LCD_wBCommand(0x80 | 0x00);
				LCD_wString(" STOP Watch.. ");
				LCD_wBCommand(0x80 | 0x40);
				sprintf(buf, " %02d:%02d(sec)  ", t->tm_min, t->tm_sec);
				LCD_wString(buf);
				stop++;
				_delay_ms(100);
			}
			mode = MAIN;
			while (PIND & (1 << PIND2));
			_delay_ms(2000);
		}
		else if (mode == ALARM)
		{
			alarm_set();
			_delay_ms(500);
			for (int i = alarm_idx[alarm_set_idx * 2]; i <= alarm_idx[alarm_set_idx * 2 + 1]; i++)
			{
				LCD_Cursor(1, i);
				LCD_wString(" ");
			}
			_delay_ms(500);
			isAlarmSetting = 1;
		}
		_delay_ms(100);
    }
}

ISR(INT0_vect) // UP
{
	if (mode == SETTIME)
	{
		struct tm* up = gmtime(&time_sec);
		int* arr[] = {&up->tm_year,&up->tm_mon,&up->tm_mday,&up->tm_hour,&up->tm_min,&up->tm_sec};
		(*arr[set_idx / 2])++;
		time_sec = mktime(up);
		print_lcd();
	}
	else if (mode == ALARM)
	{
		int* arr[] = {&al_hour, &al_min};
		(*arr[alarm_set_idx])++;
	}
}

ISR(INT1_vect) // DOWN
{
	if (mode == SETTIME)
	{
		struct tm* up = gmtime(&time_sec);
		int* arr[] = {&up->tm_year,&up->tm_mon,&up->tm_mday,&up->tm_hour,&up->tm_min,&up->tm_sec};
		(*arr[set_idx / 2])--;
		time_sec = mktime(up);
		print_lcd();
	}
	else if (mode == ALARM)
	{
		int* arr[] = {&al_hour, &al_min};
		(*arr[alarm_set_idx])--;
	}
}

ISR(INT2_vect) // Set
{
	volatile uint16_t cnt = 0;
	while (!(PIND & 0x04) && mode != WATCH)
	{
		cnt++;
		_delay_ms(1);
	}
	
	if (cnt > LONGTIME)
	{
		if (!(PIND & (1 << PIND3)) && mode == MAIN)
		{
			mode = ALARM;
		}
	}
	else
	{
		if (mode == SETTIME)
		{
			set_idx = (set_idx + 2) % 12;
			_delay_ms(300);
		}
		else if (mode == ALARM)
		{
			alarm_set_idx = !alarm_set_idx;
			_delay_ms(300);
		}
	}
}

ISR(INT3_vect) // Mode
{
	volatile uint16_t cnt = 0;
	while (!(PIND & 0x08))
	{
		cnt++;
		_delay_ms(1);
	}
	if (cnt > LONGTIME)
	{
		if (mode == MAIN)
		{
			mode = SETTIME;
		}
	}
	else
	{
		if (mode == SETTIME && cnt > 200)
		{
			mode = MAIN;
		}
		
		if (mode == ALARM && isAlarmSetting)
		{
			isAlarmSetting = 0;
			isAlarmOn = 1;
			mode = MAIN;
		}
		
	}
}