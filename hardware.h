#ifndef  __HARDWARE_H__
#define  __HARDWARE_H__

#include "main.h"

extern uint8_t key_value;
extern uint8_t key_state;
extern uint8_t dig_buf[8];

void key_scan(void);
void led_output(uint8_t value);
void dig_show(void);

#endif