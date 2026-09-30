#include "main.h"
#include "seg7.h"

static int counter_digit = 50;  // 50 * 10ms = 500ms (0.5s) per digit
static int counter_dot   = 100; // 100 * 10ms = 1000ms (1.0s) per LED blink

static int digit_state = 0; // 0: '1', 1: '2', 2: '3', 3: '0'
static int dot_state   = 0; // 0: OFF, 1: ON

void ex2_init(void) {
    /* Turn off all 4 digits to start clean (SET = OFF for active-low) */
    HAL_GPIO_WritePin(EN0_GPIO_Port, EN0_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(EN3_GPIO_Port, EN3_Pin, GPIO_PIN_SET);

    /* Turn off the DOT LEDs initially */
    HAL_GPIO_WritePin(DOT_GPIO_Port, DOT_Pin, GPIO_PIN_SET);

    /* Initially display '1' on the first digit (EN0) */
    display7SEG(1);
    HAL_GPIO_WritePin(EN0_GPIO_Port, EN0_Pin, GPIO_PIN_RESET); // RESET = ON
    digit_state = 0;
}

void ex2_loop(void) {
    /* Main loop is empty; all timing and switching is in the ISR */
}

void ex2_timer_isr(void) {
    /* --- 1. Seven-Segment Multiplexing (500ms switching time) --- */
    counter_digit--;
    if (counter_digit <= 0) {
        counter_digit = 50; // Reset to 500ms

        /* Turn off all enable pins to avoid display ghosting */
        HAL_GPIO_WritePin(EN0_GPIO_Port, EN0_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(EN3_GPIO_Port, EN3_Pin, GPIO_PIN_SET);

        /* Advance to the next digit state (0 -> 1 -> 2 -> 3 -> 0) */
        digit_state = (digit_state + 1) % 4;

        switch (digit_state) {
            case 0:
                display7SEG(1);
                HAL_GPIO_WritePin(EN0_GPIO_Port, EN0_Pin, GPIO_PIN_RESET); // Digit 1 = '1'
                break;
            case 1:
                display7SEG(2);
                HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, GPIO_PIN_RESET); // Digit 2 = '2'
                break;
            case 2:
                display7SEG(3);
                HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin, GPIO_PIN_RESET); // Digit 3 = '3'
                break;
            case 3:
                display7SEG(0);
                HAL_GPIO_WritePin(EN3_GPIO_Port, EN3_Pin, GPIO_PIN_RESET); // Digit 4 = '0'
                break;
            default:
                break;
        }
    }

    counter_dot--;
    if (counter_dot <= 0) {
        counter_dot = 100;
        dot_state = !dot_state;
        HAL_GPIO_WritePin(DOT_GPIO_Port, DOT_Pin, dot_state ? GPIO_PIN_RESET : GPIO_PIN_SET);
    }
}
