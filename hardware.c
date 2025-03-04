#include "hardware.h"

uint8_t code t_display[]={                       //标准字库
//   0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F
    0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71,
//black  -     H    J    K    L    N    o   P    U     t    G    Q    r   M    y
    0x00,0x40,0x76,0x1E,0x70,0x38,0x37,0x5C,0x73,0x3E,0x78,0x3d,0x67,0x50,0x37,0x6e,
    0xBF,0x86,0xDB,0xCF,0xE6,0xED,0xFD,0x87,0xFF,0xEF,0x46};    //0. 1. 2. 3. 4. 5. 6. 7. 8. 9. -1

uint8_t code T_COM[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};      //位码
uint8_t dig_buf[3][8]={16,16,16,16,16,16,16,16,
					   16,16,16,16,16,16,16,16,
					   16,16,16,16,16,16,16,16};

void dig_show(void)
{
	static dig_com=0;
	//消影
	P0=0x00;
	P2=P2&0x1f|0xc0;
	P2&=0x1f;
	//段选
	P0=~ t_display[dig_buf[mode][dig_com]];
	P2=P2&0x1f|0xe0;
	P2&=0x1f;
	//位选
	P0=T_COM[dig_com];
	P2=P2&0x1f|0xc0;
	P2&=0x1f;
	
	if(++dig_com>=8)
		dig_com=0;
}

void led_output(uint8_t value)
{
	P0=value;
	P2=P2&0x1f|0x80;
	P2&=0x1f;
}

uint8_t key_state=255,key_value=255;

void key_scan(void)
{
	P3|=0x0f;
	if((~P3) & 0x0f)
	{
		if(key_state<255) key_state++;
		
		if(P30==0) key_value=7;
		if(P31==0) key_value=6;
		if(P32==0) key_value=5;
		if(P33==0) key_value=4;
		
	}
	else 
	{
		key_value=0;
		key_state=0;
	}
}