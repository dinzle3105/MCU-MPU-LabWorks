#include "main.h"
#include "matrix.h"

#define MATRIX_SHIFT_TICKS  50

static uint8_t matrix_buffer[8] = { 0x00, 0xFC, 0xFE, 0x13, 0x13, 0xFE, 0xFC, 0x00 }; /* Character 'A' */
static int matrix_anim_tick = 0;
static volatile int matrix_shift_ready = 0;

static void shiftMatrixLeft(void) {
    uint8_t first = matrix_buffer[0];
    for (int c = 0; c < 7; c++) {
        matrix_buffer[c] = matrix_buffer[c + 1];
    }
    matrix_buffer[7] = first;
}
static void rotateMatrix90Clockwise(void) {
    uint8_t temp[8] = {0};

    for (int c = 0; c < 8; c++) {
        for (int r = 0; r < 8; r++) {
            /* Check if the pixel at Row 'r', Column 'c' is lit */
            if ((matrix_buffer[c] >> r) & 0x01) {
                /* Set bit 'c' at new Column (7 - r) */
                temp[7 - r] |= (1 << c);
            }
        }
    }

    /* Copy rotated result back into main buffer */
    for (int i = 0; i < 8; i++) {
        matrix_buffer[i] = temp[i];
    }
}
void ex10_init(void) {
    /* Exercise 10: Display 'A' on LED Matrix and shift it left every 500ms */
    MATRIX_DisableAllColumns();
}

void ex10_loop(void) {
    /* Multiplex the matrix display */
    for (int i = 0; i < 8; i++) {
        MATRIX_UpdateColumn(i, matrix_buffer);
        HAL_Delay(4);
    }

    /* Check if the timer ISR flagged a shift */
    if (matrix_shift_ready) {
        matrix_shift_ready = 0;
        shiftMatrixLeft();
    }
}

void ex10_timer_isr(void) {
    /* Timer raises flag for matrix shift */
    matrix_anim_tick++;
    if (matrix_anim_tick >= MATRIX_SHIFT_TICKS) {
        matrix_anim_tick = 0;
        matrix_shift_ready = 1;
    }
}
