#include "main.h"
#include "hardware.h"
#include "onewire.h"

uint8_t led=0xff;
uint8_t time0_10ms=0;
uint8_t time0_191ms=0;
uint32_t time0_1000ms=0;
uint32_t time=0;
uint32_t temperature = 18;

uint8_t mode=1;
uint8_t detect=0;

uint8_t count=0;
uint8_t flag=0;

void hardware_handle(void)
{
	if(time>0)
	{
		if(mode==1)
		{
			flag=2;
			led=0xff;
			led&=(~0x01);
		}
		else if(mode==2)
		{
			flag=3;
			led=0xff;
			led&=(~0x02);
		}
		else if(mode==3)
		{
			flag=7;
			led=0xff;
			led&=(~0x04);
		}
	}
	else 
		led=0xff;
}
void mode1_handle(void)
{
	dig_buf[0]=17;
	dig_buf[1]=1;
	dig_buf[2]=17;
	dig_buf[4]=time/1000%10;
	dig_buf[5]=time/100%10;
	dig_buf[6]=time/10%10;
	dig_buf[7]=time%10;
}
void mode2_handle(void)
{
	dig_buf[0]=17;
	dig_buf[1]=2;
	dig_buf[2]=17;
	dig_buf[4]=time/1000%10;
	dig_buf[5]=time/100%10;
	dig_buf[6]=time/10%10;
	dig_buf[7]=time%10;
}
void mode3_handle(void)
{
	dig_buf[0]=17;
	dig_buf[1]=3;
	dig_buf[2]=17;
	dig_buf[4]=time/1000%10;
	dig_buf[5]=time/100%10;
	dig_buf[6]=time/10%10;
	dig_buf[7]=time%10;
}
void mode4_handle(void)
{
	dig_buf[0]=17;
	dig_buf[1]=4;
	dig_buf[2]=17;
	led=0xff;
	dig_buf[4]=16;
	dig_buf[5]=temperature/1000%10;
	dig_buf[6]=temperature/100%10;
	dig_buf[7]=12;
}
void mode_select(void)
{
	if(mode==1&&detect==0)
		mode1_handle();
	else if(mode==2&&detect==0)
		mode2_handle();
	else if(mode==3&&detect==0)
		mode3_handle();
	else if(detect==1)
		mode4_handle();
}

void Timer0Init(void)		//1毫秒@12.000MHz
{
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0x20;		//设置定时初值
	TH0 = 0xD1;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	
	ET0=1;
	EA=1;
}
void Timer1Init(void)		//100微秒@12.000MHz
{
	AUXR |= 0x40;		//定时器时钟1T模式
	TMOD &= 0x0F;		//设置定时器模式
	TL1 = 0x50;		//设置定时初值
	TH1 = 0xFB;		//设置定时初值
	TF1 = 0;		//清除TF1标志
	TR1 = 1;		//定时器1开始计时
	
	ET1=1;
	EA=1;
}

void key_handle(void)
{
	if(key_value==7&&key_state==2)
	{
		if(detect==1)
			detect=0;
		else
			detect=1;
	}
	if(key_value==5&&key_state==2)
	{
		time+=60;
	}
	if(key_value==4&&key_state==2)
	{
		if(mode<3)
			mode++;
		else
			mode=1;
	}
}
void main(void)
{
	P2&=0x1F;
	Timer0Init();
	Timer1Init();
	led=0xff;
	mode=1;
	detect=0;
	while(1)
	{
		if(time0_10ms>=10)
		{
			key_scan();
			key_handle();
			led_output(led);
			mode_select();
			hardware_handle();
			time0_10ms=0;
		}
		if(time0_191ms>=191)
		{
			temperature=rd_temperature();
			time0_191ms=0;
		}
		if(time0_1000ms>=1000)
		{
			if(time>=1)
				time--;
			time0_1000ms=0;
		}
		  	if(count>=10)
			count=0;
			if(count<flag)
				P34=1;
			else
				P34=0;
	}
}
void Timer0Serveice(void) interrupt 1
{
	time0_10ms++;
	time0_191ms++;
	time0_1000ms++;
	dig_show();
}
void Timer1Serveice(void) interrupt 3
{
	count++;
//	if(count++>=10)
//		count=0;
//	if(count<flag)
//		P34=1;
//	else
//		P34=0;
}