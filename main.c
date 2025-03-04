#include "main.h"
#include "hardware.h"
#include "ds1302.h"
#include "onewire.h"

uint8_t  time0_10ms=0;
uint32_t time0_191ms=0;
uint32_t time0_1000ms=0;
uint32_t time1_1s=0;

uint32_t tempereture[10];
uint8_t led=0xff;
uint8_t mode=CONFIG;
uint8_t flag[4]={1,5,30,60};

static uint8_t com=0;
static uint8_t buf=0;

uint8_t temp=0;

uint8_t blink_smg=0;

uint8_t blink_led=0;

/*************************/
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

void Timer1Init(void)		//1毫秒@12.000MHz
{
	AUXR |= 0x40;		//定时器时钟1T模式
	TMOD &= 0x0F;		//设置定时器模式
	TL1 = 0x20;		//设置定时初值
	TH1 = 0xD1;		//设置定时初值
	TF1 = 0;		//清除TF1标志
	TR1 = 1;		//定时器1开始计时
	
	ET1=1;
	EA=1;
}


void key_handle(void)
{
	if(mode==CONFIG)
	{
		if(key_value==4&&key_state==2)
		{
			if(com<3)
				com++;
			else 
				com=0;
		}
		if(key_value==5&&key_state==2)
		{
			mode=COLLECT;
			temp=flag[com];
			time0_1000ms=0;
			time1_1s=0;
		}
	}
	else if(mode==COLLECT&&buf>10)
	{
		if(key_value==6&&key_state==2)
		{
			mode=CHECK;
			led=0xff;
			buf=0;
		}
	}
	else if(mode==CHECK)
	{
		dig_buf[mode][6]=tempereture[buf]/1000%10;
		dig_buf[mode][7]=tempereture[buf]/100%10;
		
		if(key_value==6&&key_state==2)
		{
			if(buf<9)
				buf++;
			else 
				buf=0;
		}
		if(key_value==7&&key_state==2)
		{
			mode=CONFIG;
		}
	}
}
void time_detect(void)
{
	if(flag[com]==1)
	{
		if(time1_1s>=1000)
		{
			tempereture[buf]=rd_tempereture();
			buf++;
			time1_1s=0;
		}
		if(buf>10)
		{
			
			if(blink_led==0)
				led&=~(0x01);
			else
				led|=0x01;
		}
	}
}
void config_mode(void)
{
	dig_buf[mode][5]=17;
	dig_buf[mode][6]=flag[com]/10;
	dig_buf[mode][7]=flag[com]%10;
}
void collect_mode(void)
{
	if(blink_smg==0)
	{
		dig_buf[mode][2]=17;
		dig_buf[mode][5]=17;
	}         
	else     
	{         
		dig_buf[mode][2]=16;
		dig_buf[mode][5]=16;
	}
	
	time_detect();
	
	dig_buf[mode][0]=ds1302_buf[0]/0x10;
	dig_buf[mode][1]=ds1302_buf[0]%0x10;
			 
	dig_buf[mode][3]=ds1302_buf[1]/0x10;
	dig_buf[mode][4]=ds1302_buf[1]%0x10;
			
	dig_buf[mode][6]=ds1302_buf[2]/0x10;
	dig_buf[mode][7]=ds1302_buf[2]%0x10;
}
void check_mode(void)
{
	
}

void mode_handle(void)
{
	if(mode==CONFIG)
		config_mode();
	else if(mode==COLLECT)
		collect_mode();
	else if(mode==CHECK)
		check_mode();
}
int main(void)
{
	P2&=0x1F;
	Timer0Init();
	Timer1Init();
	led=0xFF;
	mode=CONFIG;
	write_ds1302();
	rd_tempereture();
	while(1)
	{
		
		if(time0_10ms>=10)
		{
			key_scan();
			mode_handle();
			key_handle();
			led_output(led);
			time0_10ms=0;
		}
		if(time0_191ms>=191)
		{
			read_ds1302();
			time0_191ms=0;
		}
		if(time0_1000ms>=1000)
		{
			blink_smg=~blink_smg;
			blink_led=~blink_led;
			time0_1000ms=0;
		}

	}
}
void Timer0Serveice(void) interrupt 1
{
	dig_show();
	time0_1000ms++;
	time0_191ms++;
	time0_10ms++;
}
void Timer1Serveice(void) interrupt 3
{
	if(mode==COLLECT)
	{
		time1_1s++;
	}
}