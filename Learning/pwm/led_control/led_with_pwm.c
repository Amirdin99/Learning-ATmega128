#include <avr/io.h>
 
 
#define F_CPU 16000000UL
 
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
 
 
 
int main(void)
{
    DDRB |= (1<<DDB4); //LED용
 
    //프리스케일러로 64를 선택
    TCCR0 |= (1<<CS02);
 
    //FAST PWM 모드
    TCCR0 |= (1<<WGM01);
    TCCR0 |= (1<<WGM00);
 
    //non-inverted mode
    TCCR0 |= (1<<COM01);
 
    //카운터 초기화
    TCNT0 = 0;
 
 
    /* Replace with your application code */
    while (1)
    {
        int i;
 
        for( i=0; i<255; i++)
        {
            OCR0 = i;
            _delay_ms(10);
        }
 
        for( i=255; i>=0; i--)
        {
            OCR0 = i;
            _delay_ms(10);
        }
 
    }
}
 