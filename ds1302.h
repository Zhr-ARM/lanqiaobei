#ifndef __DS1302_H__
#define __DS1302_H__

#include "main.h"

extern uint8_t ds1302_buf[3];

void write_ds1302(void);
void read_ds1302(void);


#endif