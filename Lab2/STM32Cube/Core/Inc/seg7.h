#ifndef SEG7_H
#define SEG7_H

#include "main.h"
#define MAX_LED 4

extern int index_led;
extern int led_buffer[MAX_LED];
void display7SEG(int num);
void update7SEG(int index);
void turnOffAll7SEG(void);

#endif
