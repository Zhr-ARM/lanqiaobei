#include "main.h"
#include "ds1302.h"
#include "onewire.h"
#include "hardware.h"
uint8_t time0_10ms=0;
uint8_t time0_191ms=0;
uint8_t time0_200ms=0;
uint32_t time0_1s=0;
uint8_t led=0xff;
uint8_t day=0;
uint8_t alarm_day=1;
uint32_t temp=18;

uint8_t mode=TIME;

uint8_t h=23;
uint8_t m=59;
uint8_t s=50;

uint8_t alarm_h=0;
uint8_t alarm_m=0;
uint8_t alarm_s=0;

uint8_t alarm_buf[3]={0};

uint8_t flag=1;
uint8_t flag_time=0;
uint8_t flag_alarm=0;
uint8_t blink=0;
uint8_t led_blink=0;
void init_hardware(void)
{
	led=0xff;
	P0=0x00;
	P2=P2&0x1f|0xa0;
	P2&=0x1f;
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
void time_handle(void)
{
	dig_buf[0]=ds1302_buf[0]/0x10;
	dig_buf[1]=ds1302_buf[0]%0x10;
	dig_buf[2]=17;
	dig_buf[3]=ds1302_buf[1]/0x10;
	dig_buf[4]=ds1302_buf[1]%0x10;
	dig_buf[5]=17;
	dig_buf[6]=ds1302_buf[2]/0x10;
	dig_buf[7]=ds1302_buf[2]%0x10;
	if(h>(ds1302_buf[0]/0x10)*10+(ds1302_buf[0]%0x10))
		day++;
	h=(ds1302_buf[0]/0x10)*10+(ds1302_buf[0]%0x10);
	m=(ds1302_buf[1]/0x10)*10+(ds1302_buf[1]%0x10);
	s=(ds1302_buf[2]/0x10)*10+(ds1302_buf[2]%0x10);
	if((flag==1)&&(day>=alarm_day)&&(h>alarm_h||(h==alarm_h&&m>alarm_m)||(h==alarm_h&&m==alarm_m&&s>alarm_s)))
	{
		flag_alarm=1;
	}
	else
		flag_alarm=0;
}
void time_config(void)
{
	dig_buf[2]=17;
	dig_buf[5]=17;
	if(flag_time==1&&blink==1)
	{
		dig_buf[0]=h/10%10;
		dig_buf[1]=h%10;
		dig_buf[3]=m/10%10;
		dig_buf[4]=m%10;
		dig_buf[6]=s/10%10;
		dig_buf[7]=s%10;
	}
	else if(flag_time==1&&blink==0)
	{
 	dig_buf[0]=16;
	dig_buf[1]=16;
	}
	if(flag_time==2&&blink==1)
	{
		dig_buf[0]=h/10%10;
		dig_buf[1]=h%10;
		dig_buf[3]=m/10%10;
		dig_buf[4]=m%10;
		dig_buf[6]=s/10%10;
		dig_buf[7]=s%10;
	}
	else if(flag_time==2&&blink==0)
	{
 	dig_buf[3]=16;
	dig_buf[4]=16;
	}
	if(flag_time==3&&blink==1)
	{
		dig_buf[0]=h/10%10;
		dig_buf[1]=h%10;
		dig_buf[3]=m/10%10;
		dig_buf[4]=m%10;
		dig_buf[6]=s/10%10;
		dig_buf[7]=s%10;
	}
	else if(flag_time==3&&blink==0)
	{
 	dig_buf[6]=16;
	dig_buf[7]=16;
	}
}
void alarm_handle(void)
{
	if(flag_alarm==1)
	{
		if(key_state>=2)
		{
		   flag=0;
		   flag_alarm=0;
		   led|=0x01;
		}
		if(led_blink==1)
			led&=(~0x01);
		else
			led|=0x01;
	}
}
void alarm_config(void)
{
	dig_buf[2]=17;
	dig_buf[5]=17;
	if(flag_time==1&&blink==1)
	{
		dig_buf[0]=alarm_h/10%10;
		dig_buf[1]=alarm_h%10;
		dig_buf[3]=alarm_m/10%10;
		dig_buf[4]=alarm_m%10;
		dig_buf[6]=alarm_s/10%10;
		dig_buf[7]=alarm_s%10;
	}
	else if(flag_time==1&&blink==0)
	{
 	dig_buf[0]=16;
	dig_buf[1]=16;
	}
	if(flag_time==2&&blink==1)
	{
		dig_buf[0]=alarm_h/10%10;
		dig_buf[1]=alarm_h%10;
		dig_buf[3]=alarm_m/10%10;
		dig_buf[4]=alarm_m%10;
		dig_buf[6]=alarm_s/10%10;
		dig_buf[7]=alarm_s%10;
	}
	else if(flag_time==2&&blink==0)
	{
 	dig_buf[3]=16;
	dig_buf[4]=16;
	}
	if(flag_time==3&&blink==1)
	{
		dig_buf[0]=alarm_h/10%10;
		dig_buf[1]=alarm_h%10;
		dig_buf[3]=alarm_m/10%10;
		dig_buf[4]=alarm_m%10;
		dig_buf[6]=alarm_s/10%10;
		dig_buf[7]=alarm_s%10;
	}
	else if(flag_time==3&&blink==0)
	{
 	dig_buf[6]=16;
	dig_buf[7]=16;
	}
}
void temp_show(void)
{
	dig_buf[0]=16;
	dig_buf[1]=16;
	dig_buf[2]=16;
	dig_buf[3]=16;
	dig_buf[4]=16;
	dig_buf[5]=temp/1000%10;
	dig_buf[6]=temp/100%10;
	dig_buf[7]=12;
}
void mode_select(void)
{
	if(mode==TIME)
		time_handle();
	else if(mode==TIME_CONFIG)
		time_config();
	else if(mode==ALARM_CONFIG)
		alarm_config();
	else if(mode==TEMP_SHOW)
		temp_show();
}
void key_handle(void)
{
	if(mode==TIME||mode==TEMP_SHOW)
	{
		if(key_value==4)
		{
			mode=TEMP_SHOW;
		}
		else 
			mode=TIME;
	}
	if(mode==TIME)
	{
		if(key_value==7&&key_state==2)
		{
			mode=TIME_CONFIG;
		}
		if(key_value==6&&key_state==2)
		{
			mode=ALARM_CONFIG;
		}
	}
	if(mode==TIME_CONFIG)
	{
		if(key_value==7&&key_state==2)
			if(flag_time<3)
				flag_time++;
			else
			{
				ds1302_buf[0]=(h/10%10<<4)|(h%10);
				ds1302_buf[1]=(m/10%10<<4)|(m%10);
				ds1302_buf[2]=(s/10%10<<4)|(s%10);
				write_ds1302();
				mode=TIME;
				flag_time=0;
			}
		if(flag_time==1)
		{
			if(key_value==5&&key_state==2)
			{
				if(h<24)
					h++;
				else
				{
					day++;
					h=0;
				}
			}
			else if(key_value==4&&key_state==2)
			{
				if(h>0)
					h--;
				else
				{
					h=24;
					day--;
				}
			}
		}
		if(flag_time==2)
		{
			if(key_value==5&&key_state==2)
			{
				if(m<60)
					m++;
				else
					m=0;
			}
			else if(key_value==4&&key_state==2)
			{
				if(m>0)
					m--;
				else
					m=60;
			}
		}
		if(flag_time==3)
		{
			if(key_value==5&&key_state==2)
			{
				if(s<60)
					s++;
				else
					s=0;
			}
			else if(key_value==4&&key_state==2)
			{
				if(s>0)
					s--;
				else
					s=60;
			}
		}
	}
	if(mode==ALARM_CONFIG)
	{
		if(key_value==6&&key_state==2)
		{
			if(flag_time<3)
				flag_time++;
			else
			{
				flag=1;
				mode=TIME;
				flag_time=0;
			}
		}
		if(flag_time==1)
		{
			if(key_value==5&&key_state==2)
			{
				if(alarm_h<24)
					alarm_h++;
				else
				{
					alarm_day++;
					alarm_h=0;
				}
			}
			else if(key_value==4&&key_state==2)
				if(alarm_h>0)
					alarm_h--;
				else
				{
					alarm_day--;
					alarm_h=24;
				}
		}
		if(flag_time==2)
		{
			if(key_value==5&&key_state==2)
			{
				if(alarm_m<60)
					alarm_m++;
				else
					alarm_m=0;
			}
			else if(key_value==4&&key_state==2)
			{
				if(alarm_m>0)
					alarm_m--;
				else
					alarm_m=60;
			}
		}
		if(flag_time==3)
		{
			if(key_value==5&&key_state==2)
			{
				if(alarm_s<60)
					alarm_s++;
				else
					alarm_s=0;
			}
			else if(key_value==4&&key_state==2)
			{
				if(alarm_m>0)
					alarm_m--;
				else
					alarm_m=60;
			}
		}
	}
}

void main(void)
{
	P2&=0x1f;
	init_hardware();
	write_ds1302();
	flag_time=0;
	Timer0Init();
	while(1)
	{
		if(time0_10ms>=10)
		{
			led_output(led);
			key_scan();
			key_handle();
			mode_select();
			alarm_handle();
			time0_10ms=0;
		}
		if(time0_191ms>=191)
		{
			temp=rd_temperature();
			read_ds1302();
			time0_191ms=0;
		}
		if(time0_200ms>=200)
		{
			if(led_blink==0)
				led_blink=1;
			else
				led_blink=0;
			time0_200ms=0;
		}
		if(time0_1s>=1000)
		{
			if(blink==0)
				blink=1;
			else
				blink=0;
			time0_1s=0;
		}
	}
}
void Timer0Service(void) interrupt 1
{
	show_smg();
	time0_10ms++;
	time0_191ms++;
	time0_200ms++;
	time0_1s++;
}