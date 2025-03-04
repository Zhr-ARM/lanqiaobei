#ifndef __MAIN_H__
#define __MAIN_H__

#include <STC15F2K60S2.H>
#include "intrins.h"

typedef unsigned char uint8_t;
typedef unsigned int  uint32_t;

#define ON    1
#define OFF   0

#define OK         0
#define HIGH_ERROR 1
#define LOW_ERROR  2
#define ERROR      3
void Delay20ms(void);
void Delay1ms(void);		//@12.000MHz
void Delay5ms(void);		//@12.000MHz



#endif 