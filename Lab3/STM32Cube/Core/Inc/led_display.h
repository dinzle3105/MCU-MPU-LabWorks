#ifndef LED_DISPLAY_H
#define LED_DISPLAY_H

#include "main.h"

/*
 * Traffic-light / seven-segment output driver.
 *
 * Call led_display_init() once after MX_GPIO_Init().
 * Call led_display_task(mode, value) continuously from the main loop.
 *
 * mode:
 *   1 = normal mode (this module leaves traffic-light timing to the
 *       application's traffic FSM; it does not invent a normal sequence)
 *   2 = edit red duration
 *   3 = edit amber duration
 *   4 = edit green duration
 *
 * value: selected duration, expected range 1..99.
 */
void led_display_init(void);
void led_display_all_off(void);
void led_display_set_traffic(unsigned char a_red, unsigned char a_amber, unsigned char a_green,
                             unsigned char b_red, unsigned char b_amber, unsigned char b_green);
void led_display_task(unsigned char mode, unsigned char value);

unsigned char get_duration(unsigned char mode);
void inc_duration(unsigned char mode);
void save_duration(unsigned char mode);
void reset_traffic_light(void);
void reset_traffic_light(void);

#endif /* LED_DISPLAY_H */
