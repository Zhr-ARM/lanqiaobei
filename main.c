#include "main.h"
#include "hardware.h"
#include "onewire.h"

uint8_t time0_10ms=0;
uint8_t time0_191ms=0;
uint8_t time0_200ms=0;
uint32_t time0_400ms=0;
uint32_t time0_800ms=0;

uint8_t blink=0;

uint32_t tempereture=18;
uint8_t mode=DETECT;
uint8_t flag=0;//判断现在输入的是第几位
uint8_t temp;//存储按键值

uint8_t high_10=3;
uint8_t high_1=0;
uint8_t high=30;
uint8_t low_10=2;
uint8_t low_1=0;
uint8_t low=20;

uint8_t led=0xFF;
uint8_t value=OFF;

uint8_t detect_flag=0;
/***************************/
void flag_detect(void)
{
	if(detect_flag==0)
		{
			value=OFF;
			if(time0_200ms>=200)
			{
				time0_200ms=0;
				blink=~blink;
				if(blink==0)
					led&=(~0x01);
				else
					led|=0x01;
			}
		}
		else if(detect_flag==1)
		{
			value=OFF;
			if(time0_400ms>=400)
			{
				time0_400ms=0;
				blink=~blink;
				if(blink==0)
					led&=(~0x01);
				else
					led|=0x01;
			}
		}
		else if(detect_flag==2)
		{
			value=ON;
			if(time0_800ms>=800)
			{
				time0_800ms=0;
				blink=~blink;
				if(blink==0)
					led&=(~0x01);
				else
					led|=0x01;
			}
		}
}

/**********************/
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
void detect_mode(void);
void Timer0Serveice(void) interrupt 1
{
	show_smg();
	time0_10ms++;
	time0_191ms++;
	
	if(detect_flag==0)
	{
		time0_200ms++;
		time0_400ms=0;
		time0_800ms=0;
	}
	else if(detect_flag==1)
	{
		time0_200ms=0;
		time0_400ms++;
		time0_800ms=0;
	}
	else if(detect_flag==2)
	{
		time0_200ms=0;
		time0_400ms=0;
		time0_800ms++;
	}
}

void config_mode(void)
{
	led=0xFF;
	value=OFF;
	switch(flag)
	{
		case 0:
		{
			dig_buf[1]=16;
			dig_buf[2]=16;
			dig_buf[6]=16;
			dig_buf[7]=16;
			break;
		}
		case 1:
		{
			if(temp==16)
			{
				high_10=0;
				dig_buf[1]=16;
			}
			else
			{
				high_10=temp;
				dig_buf[1]=high_10;
			}
			break;
		}
		case 2:
		{
			if(temp==16)
			{
				high_1=0;
				dig_buf[2]=16;
			}
			else
			{
				high_1=temp;
				dig_buf[2]=high_1;
			}
			break;
		}
		case 3:
		{
			if(temp==16)
			{
				low_10=0;
				dig_buf[6]=16;
			}
			else
			{
				low_10=temp;
				dig_buf[6]=low_10;
			}
			break;
		}
		case 4:
		{
			if(temp==16)
			{
				low_1=0;
				dig_buf[7]=16;
			}
			else
			{
				low_1=temp;
				dig_buf[7]=low_1;
			}
			break;
		}	
	}
	if(flag>=4)
	{
		high=high_10*10+high_1;
		low=low_10*10+low_1;
		
		
		if(high<low)
			led&=(~0x02);
		else
			led|=0x02;
	}
	else if(flag<4)
	{
		led=0xff;
	}
}

void detect_mode(void)
{
	led=0xFF;
	dig_buf[0]=17;
	dig_buf[1]=high/10%10;
	dig_buf[2]=high%10;
	dig_buf[5]=17;
	dig_buf[6]=low/10%10;
	dig_buf[7]=low%10;
	if(tempereture/100<low)
	{
		detect_flag=0;
	}
	else if(tempereture/100>=low&&tempereture/100<=high)
	{
		detect_flag=1;
	}
	else if(tempereture/100>high)
	{
		detect_flag=2;
	}
}

void key_handle(void)
{
	if(key_value==7&&key_state==2)
		{
			temp=0;
			if(mode==CONFIG)
			{
				config_mode();
			}
			flag++;
	    }	
	else if(key_value==6&&key_state==2)
		{
			temp=3;
			if(mode==CONFIG)
			{
				config_mode();
			}
			flag++;
	    }
	else if(key_value==5&&key_state==2)
		{
			temp=6;
			if(mode==CONFIG)
			{
				config_mode();
			}
			flag++;
	    }	
	else if(key_value==4&&key_state==2)
		{
			temp=9;
			if(mode==CONFIG)
			{
				config_mode();
			}
			flag++;
	    }	
	else if(key_value==11&&key_state==2)
		{
			temp=1;
			if(mode==CONFIG)
			{
				config_mode();
			}
			flag++;
	    }
	else if(key_value==10&&key_state==2)
		{
			temp=4;
			if(mode==CONFIG)
			{
				config_mode();
			}
			flag++;
	    }
	else if(key_value==9&&key_state==2)
		{
			temp=7;
			if(mode==CONFIG)
			{
				config_mode();
			}
			flag++;
	    }
	else if(key_value==8&&key_state==2)
	{
		if(mode==DETECT&&flag==0)
		{
			mode=CONFIG;
			config_mode();
			flag++;
		}
		else if(mode==CONFIG&&flag>=5)
		{
			mode=DETECT;
			flag=0;
		}
	}
	else if(key_value==15&&key_state==2)
		{
			temp=2;
			if(mode==CONFIG)
			{
				config_mode();
			}
			flag++;
	    }
	else if(key_value==14&&key_state==2)
		{
			temp=5;
			if(mode==CONFIG)
			{
				config_mode();
			}
			flag++;
	    }
	else if(key_value==13&&key_state==2)
		{
			temp=8;
			if(mode==CONFIG)
			{
				config_mode();
			}
			flag++;
	    }
		
	else if(key_value==12&&key_state==2)
	{
		flag--;
		temp=16;
		if(mode==CONFIG)
		{
			config_mode();
		}
	}
}


int main(void)
{
	P2&=0x1F;
	led=0xFF;
	value=OFF;
	Timer0Init();
	
	while(1)
	{
		if(time0_10ms==10)
		{
			key_scan_all();
			key_handle();
			led_output(led);
			relay(value);
			time0_10ms=0;
		}
		if(time0_191ms==191)
		{
			tempereture = rd_tempereture();
			time0_191ms=0;
		}
		if(mode==DETECT)
		{
			detect_mode();
			flag_detect();
		}
	}
}
