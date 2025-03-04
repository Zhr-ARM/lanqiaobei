#ifndef __MAIN_H__
#define __MAIN_H__

#include <STC15F2K60S2.H>
#include "intrins.h"

typedef unsigned char uint8_t;
typedef unsigned int  uint32_t;

#define ON  1
#define OFF 0

#define CONFIG  0
#define COLLECT 1
#define CHECK   2

extern uint8_t mode;

#endif 