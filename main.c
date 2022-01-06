//Designed by BU7QKU (KUNDI)
//2021/2/6
//1027630917@qq.com
//CRYSTAL CLOCK;10MHZ
//includes:
#include <STC8G.H>
#include "sys.h"
#include "intrins.h"
//defines:
#define on 0
#define off 1
//define IO pin
sbit STRIP=P5^4;
sbit sda=P5^5;
sbit button=P3^3;
//public value
unsigned int i;
unsigned char j;
unsigned int ms=0;
uchar tmp;
//函数名：send-sda
//功能：实现发送一个字节数据给WS2812灯串模组(for 10MHZ)
//入口：dat(一个颜色亮度数据)，出口：无
void send_sda(uchar dat)        
{
    uchar count1;               //用于发送比特数计数
    for (count1=8;count1>0;count1--)
      {   
        if(dat & 0x80)         //dat & 0x80
          {
           sda=1;
           _nop_();
           _nop_();
           _nop_();
           _nop_();
           _nop_();
           _nop_();	
	         _nop_();
           sda=0;
           _nop_();
           _nop_();
           _nop_();
           _nop_();
           _nop_();
           _nop_();
	         _nop_();
           _nop_();	
	         _nop_();
           _nop_();	
          }
         else
         {
					sda=1;
          _nop_();
          _nop_();
          _nop_();
          sda=0;
          _nop_();
          _nop_();
          _nop_();
          _nop_();
          _nop_();
          _nop_();
          _nop_();
          _nop_();		
          _nop_();
          _nop_();			
          _nop_();
          _nop_();						 
				}
            dat<<=1;               //dat<<=1
      }
}
//send a LED data
void senddata(uchar r,uchar g,uchar b)
{
	send_sda(r);//>r > g> b
	send_sda(g);
	send_sda(b);
}
//set all LEDs off
void black(void)
{
	for(j=0;j<=15;j++)
	 senddata(0,0,0);
	sda=0;
  delay_us(50);
}
//set all LEDs light is  pink
void pink(void)
{
	for(j=0;j<=15;j++)
	 senddata(255,0,127);
	sda=0;
  delay_us(50);
}
//set all LEDs light is nuan
void nuan(void)
{
	for(j=0;j<=15;j++)
	 senddata(255,0x5f,0x1f);
	sda=0;
  delay_us(50);
}
//set all LEDs light is blue
void blue(void)
{
	for(j=0;j<=15;j++)
	 senddata(0,0,255);
	sda=0;
  delay_us(50);
}
//set 4*pink+yellow +green+blue
void caise(void)
{
	for(i=0;i<=3;i++)
	{
	 senddata(255,0,127);//pink
	 senddata(255,0x5f,0x1f);//yellow
	 senddata(0,255,0);//green
	 senddata(0,0,255);//blue
	}
		sda=0;
  delay_us(50);
}
//read p30,p31,p32 to selest LED mode
uchar read_LED_mode(void)
{
	if(P30==0&&P31==0&&P32==0)
		return 7;
	else if(P30==1&&P31==0&&P32==0)
		return 6;
	else if(P30==0&&P31==1&&P32==0)
		return 5;
	else if(P30==1&&P31==1&&P32==0)
		return 4;
	else if(P30==0&&P31==0&&P32==1)
		return 3;
	else if(P30==1&&P31==0&&P32==1)
		return 2;
	else if(P30==0&&P31==1&&P32==1)
		return 1;
	else 
		return 0;
}
//--------------------------------------------------------------------
//函数名：Init
//功能：上电初始化GPIO和需要用到的中断
//入口：无，出口：无
void Init (void)    //初始化定时器和中断
{	
	P3M1 = 0x0F; 
	P3M0 = 0x00; //set p30,p31,p32,p33 to Hi-z input mode
	P5M0=0X30;
	P5M1=0X00;//set P54 P55 to push-pull mode
	P_SW2|=0x80;
	P3PU=0X0F;//set P30 P31 P32 P33 are pull-up resistors
	P_SW2&=0x7F;
	
  AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0xF0;		//设置定时初值
	TH0 = 0xD8;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时	

	
	EA=1;//开总中断
	EX1=1;//使能 INT0中断
	IT1=1;

	ms=0;
}
void wakeup() interrupt 2
{ 
	while(!button);//
	P3M1 = 0x0F; 
	P3M0 = 0x00; 
	P_SW2|=0x80;
	P3PU|=0X0F;
	P_SW2&=0x7F;
	STRIP=on;
	EX1=0;//disABLE INT1
	delay_ms(50);
}

void main()
{
 unsigned char pre=0;//count for pressed time
 Init();
	while(1)
	{
		   if(!button)		
			 {
				 delay_ms(1);
				 pre++;
			 }	 
       else
       {	
         if(pre>=10&&button)//press 10ms+ to power-down
				 {				 
		        STRIP=off;
					  pre=0;
				    P_SW2|=0x80;
				    P3PU=0x08;
				    P_SW2&=0X7F;
		        EX1=1;//使能中断 INT1以
		        PCON=0X02;
		        _nop_();
		        _nop_();
				 }
				  pre=0;
			 }

			 if(TF0)
			 {
				 TL0 = 0xF0;		//设置定时初值
	       TH0 = 0xD8;		//设置定时初值
	       TF0 = 0;		//清除TF0标志
	       ms++;
			 }
			tmp=read_LED_mode();		
			 if(tmp==0)
			 {
				 pink();
			 }
			 else if(tmp==1)//flash for pink 
 			 {
				 if(ms<500)
					pink();
				 else if(ms>=500&&ms<1000)
				 {
					black();
				 }
				 else if(ms>=1000)
					ms=0;
			 }
      else if(tmp==2)
      {
				nuan();
			}	
			else if(tmp==3)//flash for nuan
			{
				if(ms<500)
					nuan();
				 else if(ms>=500&&ms<1000)
				 {
					black();
				 }
				 else if(ms>=1000)
					ms=0;
			 }			
      else if(tmp==4)
			{
				blue();
			}
			else if(tmp==5)//blue flash
			{
				if(ms<500)
					blue();
				 else if(ms>=500&&ms<1000)
				 {
					black();
				 }
				 else if(ms>=1000)
					ms=0;
			}
			else if(tmp==6)
			{
				caise();
			}
			else if(tmp==7)	
			{
				if(ms<500)
					caise();
				 else if(ms>=500&&ms<1000)
				 {
					black();
				 }
				 else if(ms>=1000)
					ms=0;
			}
			//delay_ms(1);
		}
	  
}
