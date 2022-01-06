#ifndef __SYS_H
#define __SYS_H
#include <STC8G.H>
#include "intrins.h"
#define uint unsigned int
#define uchar unsigned char
#define MAIN_Fosc 10000000UL //10MHZ

void delay_ms(uint count)  // /* X1ms */
{
     uint i;
     do{
          i = MAIN_Fosc / 13000;
          while(--i);
     }while(--count);
}
//---------------------------------------------------------------------------
//函数名：delayus
//功能：us级别的低精度软件延时
//入口：us(延时的us数)，出口：无
void delay_us(uint us)  //10mhz
{        
	
	while (--us) _nop_();  
        
}

#endif
