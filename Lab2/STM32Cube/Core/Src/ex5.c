#include "main.h"
#include "seg7.h"
int hour = 15;
int minute = 8;
int second = 50;
void ex5_init(void) {
    turnOffAll7SEG();
    index_led = 0;

    update7SEG(index_led);

    HAL_GPIO_WritePin(DOT_GPIO_Port, DOT_Pin, GPIO_PIN_SET);
}
void updateClockBuffer(void) {
    // Digits 0 & 1: Hours (e.g., 15 -> [1, 5])
    led_buffer[0] = hour / 10;
    led_buffer[1] = hour % 10;

    // Digits 2 & 3: Minutes (e.g., 8 -> [0, 8])
    led_buffer[2] = minute / 10;
    led_buffer[3] = minute % 10;
}
void ex5_loop(void){
	second++;

	if (second >= 60) {
		second = 0;
		minute++;
	}
	if (minute >= 60) {
		minute = 0;
		hour++;
	}
	if (hour >= 24) {
		hour = 0;
	}

	updateClockBuffer();

	HAL_Delay(1000);
}

void ex5_timer_isr(void) {
	update7SEG(index_led);
	index_led++;
	if (index_led >= 4)
	{
		index_led = 0;
	}

}
