//PSW Register D0h [RS1:RS0] = [B4:B3] - Number of register page



#include "adc.h"
#include "STC8xxxx.h"


void adc_init(void)
{   
    ADCCFG |= (1 << 5); //Alligned Left
    ADCCFG &= 0xF0;    //SysCLK/2/1
    
    ADC_CONTR |= (1 << 7); //ADC POWER ON
}

unsigned int read_adc(char ch)
{
    if(ch > 15) ch = ch % 15;
    ADC_CONTR &= 0xF0;
    ADC_CONTR |= ch;       //Ch num
    ADC_CONTR |= (1 << 6); //Start
    while(!(ADC_CONTR & (1 << 5))); //While !ADC_FLAG
    return((ADC_RES << 8) | ADC_RESL);
}
