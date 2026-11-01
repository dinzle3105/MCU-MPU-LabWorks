
#include "input_reading.h"

/* Final debounced state */
static GPIO_PinState buttonBuffer[N0_OF_BUTTONS];

/* Two buffers used for debouncing */
static GPIO_PinState debounceButtonBuffer1[N0_OF_BUTTONS];
static GPIO_PinState debounceButtonBuffer2[N0_OF_BUTTONS];

/* Button GPIO configuration */
static GPIO_TypeDef * const button_ports[N0_OF_BUTTONS] =
{
    BUTTON_1_GPIO_Port,
    BUTTON_2_GPIO_Port,
    BUTTON_3_GPIO_Port
};

static const uint16_t button_pins[N0_OF_BUTTONS] =
{
    BUTTON_1_Pin,
    BUTTON_2_Pin,
    BUTTON_3_Pin
};

void button_reading_init(void)
{
    for (unsigned char i = 0; i < N0_OF_BUTTONS; i++)
    {
        GPIO_PinState initial =
            HAL_GPIO_ReadPin(button_ports[i], button_pins[i]);

        buttonBuffer[i] = initial;
        debounceButtonBuffer1[i] = initial;
        debounceButtonBuffer2[i] = initial;
    }
}


void button_reading_task(void)
{
    for (unsigned char i = 0; i < N0_OF_BUTTONS; i++)
    {
        /* Move the previous sample into buffer 2 */
        debounceButtonBuffer2[i] = debounceButtonBuffer1[i];

        /* Read the current physical button state */
        debounceButtonBuffer1[i] =
            HAL_GPIO_ReadPin(button_ports[i], button_pins[i]);

        /* Accept the state only if consecutive readings match */
        if (debounceButtonBuffer1[i] == debounceButtonBuffer2[i])
        {
            buttonBuffer[i] = debounceButtonBuffer1[i];
        }
    }
}

unsigned char is_button_pressed(unsigned char index)
{
    if (index >= N0_OF_BUTTONS)
    {
        return 0U;
    }

    return (buttonBuffer[index] == BUTTON_IS_PRESSED) ? 1U : 0U;
}
