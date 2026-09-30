#include "main.h"
#include "matrix.h"

static uint8_t matrix_buffer[8] = { 0x00, 0xFC, 0xFE, 0x13, 0x13, 0xFE, 0xFC, 0x00 }; /* Character 'A' */

void ex9_init(void) {
    /* Exercise 9: Display static 'A' pattern on 8x8 LED Matrix */
    MATRIX_DisableAllColumns();
}

void ex9_loop(void) {
    /* Multiplex the matrix in the main loop */
    for (int i = 0; i < 8; i++) {
        MATRIX_UpdateColumn(i, matrix_buffer);
        HAL_Delay(4);
    }
}

void ex9_timer_isr(void) {
    /* No timer interrupt logic needed for static display */
}
