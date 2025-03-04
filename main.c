#include "main.h"
#include "hardware.h"
#include "i2c.h"
/*****定义区域******/
uint8_t time0_10ms=0;
uint8_t time0_191ms=0;
uint32_t time0_1s=0;
uint8_t buf=0;
uint32_t temp=0;
uint8_t mode=OFF;
/*****定时器0*********/
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
void Timer0Service(void) interrupt 1
{
	smg_show();
	time0_10ms++;
	time0_191ms++;
	if(mode==ON)
	{
	  time0_1s++;
	}
}
void key_handle(void)
{
	if(key_value==7&&key_state==2)  
	{
		temp=0;
		mode=ON;
	}
	if(key_value==6&&key_state==2)
	{
		mode=OFF;
		time0_1s=0;
	}		
}
void mode_select(void)
{
	if(mode==ON)
		{
			relay(ON);
			dig_buf[1]=0+32;
			dig_buf[2]=5;
			dig_buf[3]=0;
			if(temp/1000%10==0)
			{
			dig_buf[4]=temp/1000%10;
			dig_buf[5]=(temp/100%10)+32;
			dig_buf[6]=temp/10%10;
			dig_buf[7]=temp%10;
			}
			
			if(temp/1000%10!=0)
			{
			dig_buf[4]=temp/1000%10;
			dig_buf[5]=(temp/100%10)+32;
			dig_buf[6]=0;
			dig_buf[7]=temp%10;
			}
		}
		else if(mode == OFF)
		{
			uint32_t buf=temp/2;
			
			relay(OFF);
			
			dig_buf[1]=0+32;
			dig_buf[2]=5;
			dig_buf[3]=0;
			if(buf/1000%10==0)
			{
			dig_buf[4]=buf/1000%10;
			dig_buf[5]=(buf/100%10)+32;
			dig_buf[6]=buf/10%10;
			dig_buf[7]=buf%10;
			}
			else if(buf/1000%10!=0)
			{
			dig_buf[4]=buf/1000%10;
			dig_buf[5]=(buf/100%10)+32;
			dig_buf[6]=0;
			dig_buf[7]=buf%10;
			}
		}
}
int main(void)
{
	Timer0Init();
	led_output(0xFF);
	relay(OFF);
	mode=OFF;
	while(1)
	{
		if(time0_10ms==10)
		{
			key_scan();
			key_handle();
			mode_select();
			time0_10ms=0;
		}
		if(time0_191ms==191)
		{
			buf=rd_PCF8591(1);
			time0_191ms=0;
		}
		if(time0_1s==1000)
		{
			if(temp<990)
				temp=temp+10;
			else 
				mode=OFF;
			time0_1s=0;
		}
		
	}
}