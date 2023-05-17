#include "gpio.h"
#include "STC8xxxx.h"




void gpio_init(void)
{
    //PxM1.n,PxM0.n 	=00--->Standard,	01--->push-pull		
    //					=10--->pure input,	11--->open drain
    
    //ADC CH3 P33 INPUT
    P3M1 |= (1 << 3); P3M0 &= ~(1 << 3);
    
    //UART1
    P3M1 &= ~(1 << 1); P3M0 |= (1 << 1);
    
    //PWM P32 PUSH_PULL
    P3M1 &= ~(1 << 2); P3M0 |= (1 << 2);
}
