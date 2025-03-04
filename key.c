#include "key.h"
uint8_t key_value=255;
uint8_t key_state=255;

void key_scan(void)
{
	P3|=0x0F;
	if(~P3&0x0F)
	{
		if(key_state<255) key_state++;
		if(P30==0)  key_value=7;
		if(P31==0)  key_value=6;
		if(P32==0)  key_value=5;
		if(P33==0)  key_value=4;
	}
	else
	{
		key_state=0;
		key_value=0;
	}
}