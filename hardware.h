#ifndef __HARDWARE_H__
#define __HARDWARE_H__

#include "main.h"

extern uint8_t dig_buf[3][8];
extern uint8_t key_state;
extern uint8_t key_value;

void dig_show(void);
void led_output(uint8_t value);
void key_scan(void);

#endif