#ifndef __HARDWARE_H__
#define __HARDWARE_H__

#include "main.h"

extern uint8_t dig_buf[8];
void show_smg(void);
void led_output(uint8_t value);
extern uint8_t key_value;
extern uint8_t key_state;
void key_scan(void);

#endif 