#include <LPC21xx.h>
#include "delay.h"
#include "Pin_connect_block.h"
#include "adc_defines.h"
void Init_ADC(void){
    //cfg p0.27 as  (analog input pins 0)
    CfgPortPinFunc(0,27,1);
    //cfg ADCR
    ADCR=((CLKDIV<<CLKDIV_BITS)|(1<<pdn_bit));
}
void Read_ADC(unsigned int chNo,unsigned int* adcVal)//equivivalent analog result
{
    //mask any prev channel set
    ADCR &=0xffffff00;
    //write channel for analog read
    ADCR|=(1<<chNo)|(1<<ADC_CONV_START_BIT);
    delay_ms(3);
    //wait untill conversion complete
    while(((ADDR>>DONE_BIT)&1)==0);
    //stop adc conveert wrt that channel
    ADCR &=~(1<<ADC_CONV_START_BIT);
    //read digital bits
    *adcVal=((ADDR>>DIGITAL_DATA_BITS)&1023);
    //EQYUIVALENT VALUEB/W 0 TO 3.3
   // *eAR=(*adcVal*(3.3/1024));
}
