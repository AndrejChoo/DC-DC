#include "pwm.h"
#include "STC8xxxx.h"


void pwm_init(void)
{   
    CCON = 0x00;
    CMOD = 0x08;        //PCA clock is the system clock
    CL = 0x00;
    CH = 0x00;
    
    CCAPM0 = 0x42;      //PCA 2 is in PWM mode
    PCA_PWM0 = 0xC0;    //PCA 2 outputs 8-bit PWM
    CCAP0L = 0xFF;      //PWM duty cycle is 87.5%[(100H-20H)/100H]
    CCAP0H = 0xFF;
    
    CCON |= (1 << 6);
}

void set_pwm(unsigned int val)
{  
    unsigned int duty = 1023 - val;
    char xcca = duty >> 8;

    if(val > 1023) return;
    
    PCA_PWM0 &= 0xC3;
    PCA_PWM0 |= (xcca << 4)|(xcca << 2);
    CCAP0H = duty & 0xFF;
    CCAP0L = duty & 0xFF;
}
