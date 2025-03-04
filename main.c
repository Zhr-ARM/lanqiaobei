#include "main.h"
#include "smg.h"
#include "relay.h"
#include "i2c.h"
#include "onewire.h"
#include "key.h"
/********变量定义区域**********/
uint8_t time0_10ms=0;
uint8_t time0_191ms=0;
uint8_t temp_high=30;
uint8_t temp_low=23;
uint8_t high=30;
uint8_t low=23;
uint8_t mode=OK;
uint8_t flag=0;
uint32_t tempereture=0;
/*****************/
void Delay5ms()		//@12.000MHz
{
	unsigned char i, j;

	i = 59;
	j = 90;
	do
	{
		while (--j);
	} while (--i);
}
/*********按键处理函数********/
void key_handle(void)
{
	if(key_value==4&&key_state==2)
		{
			if(high<99&&high>=0)
			high++;
			write_at24c02(0x00,high);
		}
	if(key_value==5&&key_state==2)
		{
			if(low<99&&low>=0)
			low++;
			write_at24c02(0x01,low);
		}
	if(key_value==6&&key_state==2)
		{
			if(high<=99&&high>0)
			high--;
			write_at24c02(0x00,high);
		}
	if(key_value==7&&key_state==2)	
		{
			if(low<=99&&low>0)
			low--;
			write_at24c02(0x01,low);
		}
}
uint8_t temp_detect(void)
{
	uint8_t temp=(tempereture/1000%10)*10+(tempereture/100%10);
	if(temp<=high&&temp>=low)
		return OK;
	else if(temp>high)
		return HIGH_ERROR;
	else if(temp<low)
		return LOW_ERROR;
	else 
		return ERROR;
}
void temp_handle(void)
{
	
	if(mode==OK)
	{
		relay_handle(OFF);
		flag=0;
	}
	else if(mode==LOW_ERROR)
	{
		relay_handle(ON);
	}
	else if(mode==HIGH_ERROR)
	{
		if(flag==0)
			flag=1;
	}
		
}


/*******定时器0*********/
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
	dig_show();
	mode=temp_detect();
	time0_10ms++;
	time0_191ms++;
}
/*******定时器1*********/
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
void Timer1Service(void) interrupt 3
{
	if(flag==1)
	{
		static uint8_t i=0;
		if(i<=3)
			P34=0;
		else if(i<=10)
			P34=1;
		i++;
		if(i==10)
			i=0;
    }
}
int main(void)
{
	P2&=0x1f;
	relay_handle(OFF);
	write_at24c02(0x00,high);
	Delay5ms();
	write_at24c02(0x01,low);
	Timer0Init();
	Timer1Init();
	while(1)
	{
		if(time0_10ms>=10)
		{
			temp_high=read_at24c02(0x00);
			Delay5ms();
			temp_low=read_at24c02(0x01);
			Delay5ms();
			key_scan();
			key_handle();
			temp_handle();
			time0_10ms=0;
		}
		if(time0_191ms>=191)
		{
			tempereture=rd_tempereture();
			time0_191ms=0;
		}
		
		dig_buf[0]=temp_high/10%10;
		dig_buf[1]=temp_high%10;
		dig_buf[2]=temp_low/10%10;
		dig_buf[3]=temp_low%10;

		dig_buf[6]=tempereture/1000%10;
		dig_buf[7]=tempereture/100%10;
	}
}