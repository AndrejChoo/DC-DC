
#include "STC8xxxx.h"
#include "adc.h"
#include "gpio.h"
#include "uart.h"
#include "pwm.h"


unsigned int curr_pwm = 0, curr_adc;
volatile unsigned int  vltg = 0, need_adc = 0;
volatile unsigned char exchange[5];
volatile unsigned char rx_counter = 0, adc_del = 1, pwm_vl = 1;

long i;

void uart0 (void) interrupt 4 using 1  
{
    if(RI == 1)
    {
        exchange[rx_counter] =  SBUF;
		rx_counter++;
        RI = 0;
        if(rx_counter == 5)
        {
            rx_counter = 0;
            if(exchange[0] != 0xDC || exchange[4] != 0xCD) return;
            else
            {
                switch(exchange[1])
                {
                    case 0xDC:
                    {
                        vltg = (exchange[2] * 100) + exchange[3];
                        if(vltg > 3200) return;
                        else need_adc = (vltg * 10) / 32;
                        break;
                    }
                    case 0xD0:
                    {
                        if(exchange[2] < 1 || exchange[2] > 100) return;
                        else adc_del = exchange[2];
                        break;
                    }
                    case 0xD1:
                    {
                        if(exchange[2] < 1 || exchange[2] > 5) return;
                        else pwm_vl = exchange[2];
                        break;
                    }
                }

            }
        }

    }
    if(TI == 1)
    {
        TI = 0;
    }
}

int main(void)
{
    gpio_init();
    adc_init();
    uart_init();
    pwm_init();
    
    set_pwm(0);
    
    EA = 1;
    

    
    while(1)
    {
      curr_adc = read_adc(3);
       /*
        usart_sendString("Need ADC: ");
        print_int(need_adc, 1);
        usart_sendString("Current ADC: ");
        print_int(curr_adc, 1);
        usart_sendString("Set voltage: ");
        print_int(vltg, 1);        
      */
 
      if(curr_adc < need_adc && curr_pwm <= (1023 - pwm_vl))
	  {
		  curr_pwm += pwm_vl;
		  set_pwm(curr_pwm);
	  }

	  if(curr_adc > need_adc && curr_pwm > pwm_vl)
	  {
		  curr_pwm -= pwm_vl;
		  set_pwm(curr_pwm);
	  }

      for(i = 0; i < 0x6F; i++);
    }
    
}


