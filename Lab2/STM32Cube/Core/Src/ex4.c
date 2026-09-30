#include "main.h"
#include "seg7.h"

/* 25 ticks * 10ms = 250ms per digit. 4 digits * 250ms = 1000ms total (1Hz frequency) */
static int digit_counter = 25;

/* 100 ticks * 10ms = 1000ms (1 second per dot toggle) */
static int dot_counter = 100;
static int dot_state   = 0;

void ex4_init(void) {
    turnOffAll7SEG();
    index_led = 0;

    update7SEG(index_led);

    HAL_GPIO_WritePin(DOT_GPIO_Port, DOT_Pin, GPIO_PIN_SET);
}

void ex4_loop(void){
}

void ex4_timer_isr(void) {
    digit_counter--;
    if (digit_counter <= 0) {
        digit_counter = 25; // Reset for 250ms

        /* Display current digit and advance to next index */
        update7SEG(index_led);

        index_led++;
        if (index_led >= MAX_LED) {
            index_led = 0;
        }
    }

    dot_counter--;
    if (dot_counter <= 0) {
        dot_counter = 100; // Reset for 1000ms
        dot_state = !dot_state;
        HAL_GPIO_WritePin(DOT_GPIO_Port, DOT_Pin, dot_state ? GPIO_PIN_RESET : GPIO_PIN_SET);
    }
}
