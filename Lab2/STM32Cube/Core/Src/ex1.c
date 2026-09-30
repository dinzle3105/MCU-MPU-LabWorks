#include "main.h"
#include "seg7.h"

static int digit_state = 0; // 0 = first segment active, 1 = second segment active
static int digit_tick  = 0;

void ex1_init(void) {
    HAL_GPIO_WritePin(EN0_GPIO_Port, EN0_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, GPIO_PIN_SET);

    display7SEG(1);
    HAL_GPIO_WritePin(EN0_GPIO_Port, EN0_Pin, GPIO_PIN_RESET); // RESET turns it ON
    digit_state = 0;
}

void ex1_loop(void) {
    /* Idle in main loop - all work is in the timer ISR */
}

void ex1_timer_isr(void) {
    digit_tick++;
    if (digit_tick >= 50) { // 50 ticks = 500ms (half a second)
        digit_tick = 0;

        if (digit_state == 0) {
            HAL_GPIO_WritePin(EN0_GPIO_Port, EN0_Pin, GPIO_PIN_SET);   // Turn OFF first
            display7SEG(2);                                            // Setup segments for '2'
            HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, GPIO_PIN_RESET); // Turn ON second
            digit_state = 1;
        }
        else {
            HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, GPIO_PIN_SET);   // Turn OFF second
            display7SEG(1);                                            // Setup segments for '1'
            HAL_GPIO_WritePin(EN0_GPIO_Port, EN0_Pin, GPIO_PIN_RESET); // Turn ON first
            digit_state = 0;
        }
    }
}
