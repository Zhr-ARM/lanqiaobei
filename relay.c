#include "relay.h"

void relay_handle(uint8_t val)
{
	if(val == ON)
	{
		P0=0x10;
		P2=P2&0x1F|0xA0;
		P2&=0x1F;
	}
	else if(val == OFF)
	{
		P0=0x00;
		P2=P2&0x1F|0xA0;
		P2&=0x1F;
	}
}