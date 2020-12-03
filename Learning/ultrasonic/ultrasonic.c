#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define TRIG 1
#define ECHO 0
#define SOUND_VELOCITY 340UL

int main(void)
{
	unsigned int distance;
	
	DDRC = (1 << TRIG); // TRIG 는 출력
	DDRC &= ~(1 << ECHO); // ECHO는 입력
	sei();
	
	DDRA = 0x01;
	while(1)
	{
		TCCR1B = 0x03;
		PORTC &= ~(1<<TRIG); // 트리거 끔
		_delay_us(10); // 10us 대기
		PORTC |= (1<<TRIG); // 트리거 활성화
		_delay_us(10); // 10us 대기
		PORTC &= ~(1<<TRIG); // 트리거 끔
		while(!(PINC & (1<<ECHO))); // 에코가 비활성화까지 대기 (이전 파동에 반응하는 에코 제거)
		TCNT1 = 0x0000; // 타이머 시작
		while(PINC & (1<<ECHO)); // 에코가 들어올 때 까지 대기
		TCCR1B = 0x00; // 타이머 끝
		distance = (unsigned int)(SOUND_VELOCITY * (TCNT1 * 4 / 2) / 1000); // mm단위 거리
		PORTA = distance > 100 ? 0x01 : 0x00;	// 10cm이상 벌어지면 부저 작동
		_delay_ms(200);
	}
}