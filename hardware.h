#ifndef __HARDWARE_H__
#define __HARDWARE_H__

#include "main.h"

extern uint8_t dig_buf[8];
extern uint8_t key_value;
extern uint8_t key_state;
void smg_show(void);
void led_output(uint8_t led);
void relay(uint8_t value);
void key_scan(void);
#endif