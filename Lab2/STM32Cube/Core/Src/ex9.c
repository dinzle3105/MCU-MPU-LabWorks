#include "main.h"
#include "matrix.h"

#define MAX_LED_MATRIX 8

static uint8_t matrix_buffer[8] = { 0x00, 0xFC, 0xFE, 0x13, 0x13, 0xFE, 0xFC, 0x00 }; /* 'A' */
static int index_led_matrix = 0;

static void updateLEDMatrix(int index) {
    if (index >= 0 && index < MAX_LED_MATRIX) {
        MATRIX_UpdateColumn(index, matrix_buffer);
    }
}

void ex9_init(void) {
    MATRIX_DisableAllColumns();
}

void ex9_loop(void) {
    updateLEDMatrix(index_led_matrix);
    index_led_matrix++;
    if (index_led_matrix >= MAX_LED_MATRIX) index_led_matrix = 0;
    HAL_Delay(4);   // replace with the software timer version later
}