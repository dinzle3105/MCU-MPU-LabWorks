
#ifndef INPUT_READING_H
#define INPUT_READING_H

#include "main.h"

#define N0_OF_BUTTONS           3U
#define INPUT_TIMER_PERIOD_MS   10U

#define BUTTON_1_INDEX  0U
#define BUTTON_2_INDEX  1U
#define BUTTON_3_INDEX  2U

#define BUTTON_IS_PRESSED   GPIO_PIN_RESET
#define BUTTON_IS_RELEASED  GPIO_PIN_SET

void button_reading_init(void);
void button_reading_task(void);
unsigned char is_button_pressed(unsigned char index);

#endif /* INPUT_READING_H */
