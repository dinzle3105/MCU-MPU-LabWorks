#include "main.h"
#include "seg7.h"

/* Fast refresh tick counter (e.g., 25 ticks * 10ms = 250ms per digit scan) */
//static int scan_counter = 25;

void ex3_init(void) {
    /* Initialize display to all OFF and reset index */
    turnOffAll7SEG();
    index_led = 0;

    /* Display initial digit */
    update7SEG(index_led);
}

void ex3_loop(void) {
}

void ex3_timer_isr(void) {
    update7SEG(index_led);

    index_led++;
    if (index_led >= MAX_LED) {
        index_led = 0;
    }
}
