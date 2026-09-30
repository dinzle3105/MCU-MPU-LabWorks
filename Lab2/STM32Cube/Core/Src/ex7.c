#include "main.h"
#include "seg7.h"

static int hour = 15;
static int minute = 8;
static int second = 50;

// Software timer variables
int timer1_counter = 0;
int timer1_flag = 0;


void setTimer1(int duration) {
    timer1_counter = duration / 10; // Convert duration to ISR ticks
    timer1_flag = 0;
}


void timerRun(void) {
    if (timer1_counter > 0) {
        timer1_counter--;
        if (timer1_counter <= 0) {
            timer1_flag = 1;
        }
    }
}

void ex7_init(void) {
    turnOffAll7SEG();
    index_led = 0;

    update7SEG(index_led);

    // Initial setup for 1-second software timer
    setTimer1(1000);
}

static void updateClockBuffer(void) {
    // Digits 0 & 1: Hours
    led_buffer[0] = hour / 10;
    led_buffer[1] = hour % 10;

    // Digits 2 & 3: Minutes
    led_buffer[2] = minute / 10;
    led_buffer[3] = minute % 10;
}

void ex7_loop(void) {
    // Check if 1 second has passed via software timer
    if (timer1_flag == 1) {
        setTimer1(1000); // Reset timer for another 1 second

        // Toggle or update the DOT pin (PA4) in main loop
        HAL_GPIO_TogglePin(DOT_GPIO_Port, DOT_Pin);

        // Update clock values
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
    }
}

void ex7_timer_isr(void) {
    // Software timer tick processing
    timerRun();

    // 7-Segment display multiplexing
    update7SEG(index_led);
    index_led++;
    if (index_led >= 4) {
        index_led = 0;
    }
}
