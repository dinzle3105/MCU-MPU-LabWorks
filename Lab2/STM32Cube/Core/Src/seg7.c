#include "seg7.h"
/* Segment pattern table: segOn[digit][segment], 1 = segment lit */
static const uint8_t segOn[10][7] = {
    {1,1,1,1,1,1,0}, // 0
    {0,1,1,0,0,0,0}, // 1
    {1,1,0,1,1,0,1}, // 2
    {1,1,1,1,0,0,1}, // 3
    {0,1,1,0,0,1,1}, // 4
    {1,0,1,1,0,1,1}, // 5
    {1,0,1,1,1,1,1}, // 6
    {1,1,1,0,0,0,0}, // 7
    {1,1,1,1,1,1,1}, // 8
    {1,1,1,1,0,1,1}  // 9
};
int index_led = 0;
int led_buffer[MAX_LED] = {1, 2, 8, 8};
static GPIO_TypeDef* segPorts[7] = { SEG0_GPIO_Port, SEG1_GPIO_Port, SEG2_GPIO_Port, SEG3_GPIO_Port,
                                     SEG4_GPIO_Port, SEG5_GPIO_Port, SEG6_GPIO_Port };
static uint16_t      segPins[7]  = { SEG0_Pin, SEG1_Pin, SEG2_Pin, SEG3_Pin,
                                     SEG4_Pin, SEG5_Pin, SEG6_Pin };

static GPIO_TypeDef* enPorts[4]  = { EN0_GPIO_Port, EN1_GPIO_Port, EN2_GPIO_Port, EN3_GPIO_Port };
static uint16_t      enPins[4]   = { EN0_Pin, EN1_Pin, EN2_Pin, EN3_Pin };

void display7SEG(int num) {
    for (int i = 0; i < 7; i++) {
        HAL_GPIO_WritePin(segPorts[i], segPins[i], segOn[num][i] ? GPIO_PIN_RESET : GPIO_PIN_SET);
    }
}

void update7SEG(int index)
{
    turnOffAll7SEG();

    switch (index)
    {
        case 0:
            display7SEG(led_buffer[0]);
            HAL_GPIO_WritePin(EN0_GPIO_Port, EN0_Pin, GPIO_PIN_RESET);
            break;

        case 1:
            display7SEG(led_buffer[1]);
            HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, GPIO_PIN_RESET);
            break;

        case 2:
            display7SEG(led_buffer[2]);
            HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin, GPIO_PIN_RESET);
            break;

        case 3:
            display7SEG(led_buffer[3]);
            HAL_GPIO_WritePin(EN3_GPIO_Port, EN3_Pin, GPIO_PIN_RESET);
            break;

        default:
            break;
    }
}

void turnOffAll7SEG(void) {
    for (int i = 0; i < 4; i++) {
        HAL_GPIO_WritePin(enPorts[i], enPins[i], GPIO_PIN_SET);
    }
}
