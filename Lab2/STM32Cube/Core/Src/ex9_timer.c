#include "main.h"
#include "matrix.h"

static volatile int matrix_timer_counter = 0;
static volatile int matrix_timer_flag = 0;

static void setMatrixTimer(int duration) {
    matrix_timer_counter = duration / 10;   // TIMER_CYCLE = 10 ms
    matrix_timer_flag = 0;
}

void ex9_init(void) {
    MATRIX_DisableAllColumns();
    setMatrixTimer(10);
}

void ex9_loop(void) {
    if (matrix_timer_flag == 1) {
        setMatrixTimer(10);                      // next column in 10 ms
        updateLEDMatrix(index_led_matrix);
        index_led_matrix++;
        if (index_led_matrix >= MAX_LED_MATRIX) {
            index_led_matrix = 0;
        }
    }
}

void ex9_timer_isr(void) {
    if (matrix_timer_counter > 0) {
        matrix_timer_counter--;
        if (matrix_timer_counter == 0) matrix_timer_flag = 1;
    }
}