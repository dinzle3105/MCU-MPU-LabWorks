#include "main.h"
#include "seg7.h"

int ex8_hour = 15;
int ex8_minute = 8;
int ex8_second = 50;

// Software timers
int ex8_timer1_counter = 0;
int ex8_timer1_flag = 0;
int ex8_timer2_counter = 0;
int ex8_timer2_flag = 0;

void ex8_setTimer1(int duration) {
    ex8_timer1_counter = duration / 10; // Assuming ISR runs every 10ms
    ex8_timer1_flag = 0;
}

void ex8_setTimer2(int duration) {
    ex8_timer2_counter = duration / 10;
    ex8_timer2_flag = 0;
}

void ex8_timerRun(void) {
    if (ex8_timer1_counter > 0) {
        ex8_timer1_counter--;
        if (ex8_timer1_counter <= 0) {
            ex8_timer1_flag = 1;
        }
    }
    if (ex8_timer2_counter > 0) {
        ex8_timer2_counter--;
        if (ex8_timer2_counter <= 0) {
            ex8_timer2_flag = 1;
        }
    }
}

void ex8_updateClockBuffer(void) {
    led_buffer[0] = ex8_hour / 10;
    led_buffer[1] = ex8_hour % 10;
    led_buffer[2] = ex8_minute / 10;
    led_buffer[3] = ex8_minute % 10;
}

void ex8_init(void) {
    turnOffAll7SEG();
    index_led = 0;

    ex8_updateClockBuffer();

    ex8_setTimer1(1000); // 1-second timer for the clock
    ex8_setTimer2(10);   // 10ms timer for display multiplexing
}

void ex8_loop(void) {
    // Clock update and DOT toggle every 1 second
    if (ex8_timer1_flag == 1) {
        ex8_setTimer1(1000);

        HAL_GPIO_TogglePin(DOT_GPIO_Port, DOT_Pin);

        ex8_second++;
        if (ex8_second >= 60) {
            ex8_second = 0;
            ex8_minute++;
        }
        if (ex8_minute >= 60) {
            ex8_minute = 0;
            ex8_hour++;
        }
        if (ex8_hour >= 24) {
            ex8_hour = 0;
        }

        ex8_updateClockBuffer();
    }

    // 7-segment multiplexing every 10ms (Moved from ISR to main loop)
    if (ex8_timer2_flag == 1) {
        ex8_setTimer2(10);

        update7SEG(index_led);
        index_led++;
        if (index_led >= 4) {
            index_led = 0;
        }
    }
}

void ex8_timer_isr(void) {
    // ISR now only handles software timers. Complex logic is removed.
    ex8_timerRun();
}
