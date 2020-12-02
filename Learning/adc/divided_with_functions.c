#include <avr/io.h>  
#include <stdio.h>  
    
    
void adcInit(void)  //ADC Initialize
{  
    ADCSRA |= ((1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)); //16Mhz/128 = 125Khz  
    ADMUX |= (1<<REFS0);       //AVCC(5V)  
    ADCSRA |= (1<<ADEN);      //ADC Enable
}  
    
uint16_t readAdc(uint8_t channel)  // read analog value function
{       
    ADMUX &= 0xF0;  
    ADMUX |= channel;  
        
    ADCSRA |= (1<<ADSC);      // Begin convert
    while(ADCSRA&(1<<ADSC));// Wait for complete 
        
    return ADCW;  // Return converted value
}

int main(void)  
{         
    adcInit();             
        
    while(1)  
    {  
        int value =  readAdc(0);
    }  
}  