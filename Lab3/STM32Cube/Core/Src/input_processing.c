
#include "main.h"
#include "input_reading.h"
#include "input_processing.h"

#define MIN_MODE  1U
#define MAX_MODE  4U

typedef enum
{
    BUTTON_RELEASED,
    BUTTON_PRESSED
} ButtonState;

static ButtonState btn1_state = BUTTON_RELEASED;
static ButtonState btn2_state = BUTTON_RELEASED;
static ButtonState btn3_state = BUTTON_RELEASED;
static unsigned char current_mode = MIN_MODE;

void fsm_for_input_processing(void)
{
	switch (btn1_state)
	    {
	        case BUTTON_RELEASED:
	            if (is_button_pressed(BUTTON_1_INDEX))
	            {
	                current_mode++;
	                if (current_mode > MAX_MODE)
	                {
	                    current_mode = MIN_MODE;
	                }
	                btn1_state = BUTTON_PRESSED;
	            }
	            break;

	        case BUTTON_PRESSED:
	            if (!is_button_pressed(BUTTON_1_INDEX))
	            {
	                btn1_state = BUTTON_RELEASED;
	            }
	            break;
	    }

	    /* Button 2: Increase time duration */
	    switch (btn2_state)
	    {
	    	case BUTTON_RELEASED:
				if (is_button_pressed(BUTTON_2_INDEX))
				{
					if ((current_mode >= 2U) && (current_mode <= 4U))
					{
						inc_duration(current_mode);
					}
					btn2_state = BUTTON_PRESSED;
				}
				break;

	        case BUTTON_PRESSED:
	            if (!is_button_pressed(BUTTON_2_INDEX))
	            {
	                btn2_state = BUTTON_RELEASED;
	            }
	            break;
	    }

	    /* Button 3: Set / Save time duration and reset traffic lights */
	    switch (btn3_state)
	    {
			case BUTTON_RELEASED:
				if (is_button_pressed(BUTTON_3_INDEX))
				{
					if ((current_mode >= 2U) && (current_mode <= 4U))
					{
						save_duration(current_mode);
					}
					btn3_state = BUTTON_PRESSED;
				}
				break;

	        case BUTTON_PRESSED:
	            if (!is_button_pressed(BUTTON_3_INDEX))
	            {
	                btn3_state = BUTTON_RELEASED;
	            }
	            break;
	    }
}

unsigned char get_current_mode(void)
{
    return current_mode;
}

