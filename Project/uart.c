#include "uart.h"
#include "STC8xxxx.h"


void uart_init(void)
{
	SCON = 0x50;		//8 bits and variable baudrate
	AUXR |= 0x40;		//imer clock is 1T mode
	AUXR &= 0xFE;		//UART 1 use Timer1 as baudrate generator
	TMOD &= 0x0F;		//Set timer work mode
	TL1 = 0xF3;     //
	TH1 = 0xFC;		//Initial timer value
	ET1 = 0;		//Disable Timer%d 
	TR1 = 1;		//Timer1 start run
    
    IE |= (1 << 4);
}

void USARTSend_char(char ch)
{
	TI = 0;
	SBUF = ch;
	while(TI == 0);
}


void usart_sendString(char *str)
{
	int i = 0;
	while(str[i] != '\0')
	{
		TI = 0;
		SBUF = str[i];
		while(TI == 0);
		i++;
	}
}

void print_int(unsigned int chs, char endl)
{
    char r5 = 0, r4 = 0, r3 = 0, r2 = 0;
    
    if((chs / 10000) == 0) {r5 = 1;}
    else USARTSend_char((chs / 10000) + 0x30); 
    if(((chs % 10000) / 1000) == 0 && r5 == 1) {r4 = 1;}
    else USARTSend_char(((chs % 10000) / 1000) + 0x30);
    if(((chs % 1000) / 100) == 0 && r4 == 1) {r3 = 1;}
    else USARTSend_char(((chs % 1000) / 100) + 0x30);
    if(((chs % 1000) / 100) == 0 && r3 == 1) {r2 = 1;}
    else USARTSend_char(((chs % 100) / 10) + 0x30);
    USARTSend_char((chs % 10) + 0x30);
    if(endl) {USARTSend_char('\r'); USARTSend_char('\n');}
}
