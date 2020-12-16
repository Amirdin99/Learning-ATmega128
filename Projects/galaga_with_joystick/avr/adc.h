/*
 * adc.h
 *
 * Created: 2020-12-16 오전 9:12:10
 *  Author: user
 */ 


#ifndef ADC_H_
#define ADC_H_
#include <avr/io.h>

void adc_init() // ADC 설정
{
	ADCSRA = (1 << ADEN); // ADC enable
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // 16Mhz/128 = 125Khz
}

uint16_t adc_read(uint8_t channel) // channel에 해당하는 ADC값 반환
{
	ADMUX = (1 << REFS0); // ADC0, 5V에 0~1023 설정
	ADMUX |= channel; // ADC핀 설정
	
	ADCSRA |= (1 << ADSC); // ADC 변환 시작
	while(ADCSRA & (1 << ADSC)); // ADC 변환 완료
	
	return ADCW; // Return converted value
}

#endif /* ADC._H_ */