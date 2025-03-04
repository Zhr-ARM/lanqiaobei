#include "hardware.h"
uint8_t code t_display[]={                       //标准字库
//   0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F
    0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71,
//black  -     H    J    K    L    N    o   P    U     t    G    Q    r   M    y
    0x00,0x40,0x76,0x1E,0x70,0x38,0x37,0x5C,0x73,0x3E,0x78,0x3d,0x67,0x50,0x37,0x6e,
    0xBF,0x86,0xDB,0xCF,0xE6,0xED,0xFD,0x87,0xFF,0xEF,0x46};    //0. 1. 2. 3. 4. 5. 6. 7. 8. 9. -1

uint8_t code T_COM[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};      //位码
uint8_t dig_buf[8]={16,16,16,16,16,16,16,16};

void show_smg(void)
{
	static uint8_t dig_com=0;
	//消影
	P0=0x00;
	P2=P2&0x1F|0xC0;
	P2&=0x1F;
	//段选
	P0=~t_display[dig_buf[dig_com]];
	P2=P2&0x1F|0xE0;
	P2&=0x1F;
	//位选
	P0=T_COM[dig_com];
	P2=P2&0x1F|0xC0;
	P2&=0x1F;
	
	if(++dig_com>=8)
		dig_com=0;
}

void led_output(uint8_t value)
{
	P0=value;
	P2=P2&0x1F|0x80;
	P2&=0x1F;
}

void relay(uint8_t value)
{
	if(value==ON)
	{
		P0=0x10;
		P2=P2&0x1F|0xA0;
		P2&=0x1F;
	}
	if(value==OFF)
	{
		P0=0x00;
		P2=P2&0x1F|0xA0;
		P2&=0x1F;
	}
}
uint8_t key_value=255;
uint8_t key_state=255;
uint8_t R=255;
uint8_t C=255;

uint8_t code key_pad[4][3]=
{
	7, 11, 15, 
	6, 10, 14, 
	5,  9, 13, 
	4,  8, 12, 
};
void key_scan_all(void)
{
	P3=0x0F;
	P4=0x00;
	
	if(~P3&0x0F)
	{
		if(key_state<255)
			key_state++;
		if(P30==0) R=0;
		else if(P31==0) R=1;
		else if(P32==0) R=2;
		else if(P33==0) R=3;
		
		P3=0xF0;
		P4=0xFF;
		
		if(P44==0) C=0;
		else if(P42==0) C=1;
		else if(P35==0) C=2;
		
		key_value=key_pad[R][C];
	}
	else
	{
		key_value=0;
		key_state=0;
		R=255;
		C=255;
	}
	
}