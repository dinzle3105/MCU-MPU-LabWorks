#include "matrix.h"

#define MATRIX_ENM_ACTIVE   GPIO_PIN_RESET
#define MATRIX_ENM_IDLE     GPIO_PIN_SET
#define MATRIX_DATA_ON      GPIO_PIN_RESET
#define MATRIX_DATA_OFF     GPIO_PIN_SET

static GPIO_TypeDef* enmPorts[8] = { ENM0_GPIO_Port, ENM1_GPIO_Port, ENM2_GPIO_Port, ENM3_GPIO_Port,
                                     ENM4_GPIO_Port, ENM5_GPIO_Port, ENM6_GPIO_Port, ENM7_GPIO_Port };
static uint16_t      enmPins[8]  = { ENM0_Pin, ENM1_Pin, ENM2_Pin, ENM3_Pin,
                                     ENM4_Pin, ENM5_Pin, ENM6_Pin, ENM7_Pin };

static GPIO_TypeDef* rowPorts[8] = { ROW0_GPIO_Port, ROW1_GPIO_Port, ROW2_GPIO_Port, ROW3_GPIO_Port,
                                     ROW4_GPIO_Port, ROW5_GPIO_Port, ROW6_GPIO_Port, ROW7_GPIO_Port };
static uint16_t      rowPins[8]  = { ROW0_Pin, ROW1_Pin, ROW2_Pin, ROW3_Pin,
                                     ROW4_Pin, ROW5_Pin, ROW6_Pin, ROW7_Pin };

static int last_active_col = 0;

void MATRIX_DisableAllColumns(void) {
    for (int i = 0; i < 8; i++) {
        HAL_GPIO_WritePin(enmPorts[i], enmPins[i], MATRIX_ENM_IDLE);
    }
}

void MATRIX_UpdateColumn(int col_index, uint8_t *buffer) {
    HAL_GPIO_WritePin(enmPorts[last_active_col], enmPins[last_active_col], MATRIX_ENM_IDLE);

    for (int bit = 0; bit < 8; bit++) {
        HAL_GPIO_WritePin(rowPorts[bit], rowPins[bit],
                          ((buffer[col_index] >> bit) & 0x01) ? MATRIX_DATA_ON : MATRIX_DATA_OFF);
    }

    HAL_GPIO_WritePin(enmPorts[col_index], enmPins[col_index], MATRIX_ENM_ACTIVE);
    last_active_col = col_index;
}
