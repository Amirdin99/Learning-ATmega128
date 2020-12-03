/*
 * third.c
 * 공룡게임 만들기
 * Created: 2020-06-24 오전 10:20:26
 *  Author: user
 */ 

#include "lcd.h"
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#define ARRAY 3

typedef struct tagHurdle
{
	char y;
	char x;
} hurdle;

int jump=1;
int jump_cnt=0;
hurdle hur[ARRAY];

int getRow(int idx)
{
	int i, row;
	do
	{
		row = 15 + rand() % 10;
		for (i = 0; i < ARRAY; i++)
		{
			if (i == idx)
				continue;
			if (abs(hur[i].x - row) < 3)
			{
				break;
			}
		}
	} while (i != ARRAY);
	return row;
}

void move_hurdle()
{
	for (int i = 0; i < ARRAY; i++)
	{
		hur[i].x--;
	}
}

void print_hurdle()
{
	for (int i = 0; i < ARRAY; i++)
	{
		if (0 <= hur[i].x && hur[i].x <= 15)
		{
			LCD_Cursor(hur[i].y, hur[i].x);
			LCD_wData(hur[i].y ? 'I' : '*');	
		}
	}
}

void refresh_hurdle()
{
	for (int i = 0; i < ARRAY; i++)
	{
		if (hur[i].x == 0xFF)
		{
			hur[i].y = rand() % 2;
			hur[i].x = getRow(i);
		}
	}
}

void init_hurdle()
{
	for (int i = 0; i < ARRAY; i++)
	{
		hur[i].x = getRow(i);
		hur[i].y = rand() % 2;
	}
}

int main()
{
	int cnt= 0;
	int score=0;
	DDRA = 0xFF;
	DDRC = 0xFF;
	
	EIMSK = 0x01;
	EICRA = 0x03;
	
	memset(hur, 0, sizeof(hurdle) * ARRAY);
	init_hurdle();
	sei();
	LCD_Init();
	
	while (1)
	{	
		LCD_Cursor(0, 0);
		LCD_wString(" ");
		LCD_Cursor(1, 0);
		LCD_wString(" ");
		LCD_Cursor(jump, 0);
		LCD_wString("O");
		if(jump==0) 
		{
			if (jump_cnt++ > 80) 
			{
				jump = 1;
				jump_cnt=0;
			}
		}
		
		if (cnt++ > 50)
		{
			refresh_hurdle();
			move_hurdle();
			print_hurdle();
			cnt = 0;
		}
		else if (cnt++ > 40)
		{
			LCD_Cursor(1, 1);
			LCD_wString("               ");
			LCD_Cursor(0, 1);
			LCD_wString("               ");
			score++;
		}
		
		for(int i = 0; i < ARRAY; i++)
		{
			if(hur[i].x == 0 && hur[i].y == jump)
			{
				// 장애물과 부딫혔는지 확인
				char str[17] = {0};
				LCD_Cursor(0, 0);
				LCD_wString("  GAME OVER     ");
				LCD_Cursor(1, 0);
				sprintf(str, "  score : %5d ", score);
				LCD_wString(str);
				score=0;
				jump = 1;
				jump_cnt = 0;
				cnt = 0;
				init_hurdle();
				_delay_ms(3000);
				print_hurdle();
				break;
			}
		}
		
		if (score > 500)
		{
			LCD_Cursor(0, 0);
			LCD_wString("  GAME CLEAR    ");
			LCD_Cursor(1, 0);
			LCD_wString("                ");
			_delay_ms(3000);
			score=0;
			jump = 1;
			jump_cnt = 0;
			cnt = 0;
			init_hurdle();
			
		}
		_delay_ms(10);
	}
}

ISR(INT0_vect)
{
	 jump=0;
}
