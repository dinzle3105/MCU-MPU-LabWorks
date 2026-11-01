#include "led_display.h"
#include "main.h"
/*
 * Assumptions that can be changed after testing:
 * - Seven-segment segment lines are active LOW (common-anode display).
 * - Digit-select lines are active LOW.
 * - Traffic-light LED outputs are active HIGH.
 *
 * If a digit is blank, flickers, or lights in reverse, change the relevant
 * polarity macro below.
 */
#define SEGMENT_ON             GPIO_PIN_RESET
#define SEGMENT_OFF            GPIO_PIN_SET
#define DIGIT_SELECT_ON        GPIO_PIN_RESET
#define DIGIT_SELECT_OFF       GPIO_PIN_SET
#define TRAFFIC_LED_ON         GPIO_PIN_RESET
#define TRAFFIC_LED_OFF        GPIO_PIN_SET
#define DISPLAY_REFRESH_TICKS  15U
#define BLINK_HALF_PERIOD_TICKS (BLINK_HALF_PERIOD_MS / TIMER_PERIOD_MS)
/* Timer tick is incremented by the existing TIM2 callback in main.c. */
extern volatile uint32_t timer_tick;

typedef struct
{
    GPIO_TypeDef *port;
    uint16_t pin;
} Pin;

static const Pin segments_a[7] =
{
    {Aa_GPIO_Port, Aa_Pin}, {Ab_GPIO_Port, Ab_Pin}, {Ac_GPIO_Port, Ac_Pin},
    {Ad_GPIO_Port, Ad_Pin}, {Ae_GPIO_Port, Ae_Pin}, {Af_GPIO_Port, Af_Pin}, {Ag_GPIO_Port, Ag_Pin}
};

static const Pin segments_b[7] =
{
    {Ba_GPIO_Port, Ba_Pin}, {Bb_GPIO_Port, Bb_Pin}, {Bc_GPIO_Port, Bc_Pin},
    {Bd_GPIO_Port, Bd_Pin}, {Be_GPIO_Port, Be_Pin}, {Bf_GPIO_Port, Bf_Pin}, {Bg_GPIO_Port, Bg_Pin}
};

static const Pin digit_selects[4] =
{
    {SEG_A1_GPIO_Port, SEG_A1_Pin}, {SEG_A2_GPIO_Port, SEG_A2_Pin},
    {SEG_B1_GPIO_Port, SEG_B1_Pin}, {SEG_B2_GPIO_Port, SEG_B2_Pin}
};

static const Pin traffic_lights[6] =
{
    {A_RED_GPIO_Port, A_RED_Pin}, {A_AMBER_GPIO_Port, A_AMBER_Pin}, {A_GREEN_GPIO_Port, A_GREEN_Pin},
    {B_RED_GPIO_Port, B_RED_Pin}, {B_AMBER_GPIO_Port, B_AMBER_Pin}, {B_GREEN_GPIO_Port, B_GREEN_Pin}
};

static const uint8_t digit_patterns[10] =
{
    0x3FU, 0x06U, 0x5BU, 0x4FU, 0x66U, 0x6DU, 0x7DU, 0x07U, 0x7FU, 0x6FU
};

static uint8_t display_digits[4] = {0U, 0U, 0U, 0U};
static uint8_t display_blank[4] = {1U, 0U, 1U, 0U};
static uint8_t current_digit = 0U;

/* Initial state: Red = 7, Green = 5, Amber = 2 */
static unsigned char red_duration = 7U;
static unsigned char green_duration = 5U;
static unsigned char amber_duration = 2U;
static unsigned char temp_red_duration = 7U;
static unsigned char temp_amber_duration = 2U;
static unsigned char temp_green_duration = 5U;
typedef enum { LIGHT_RED, LIGHT_GREEN, LIGHT_AMBER } LightState;

static LightState state_a, state_b;
static int counter_a, counter_b;
static uint32_t last_second_tick = 0U;

static uint32_t last_blink_tick = 0U;
static uint8_t blink_visible = 1U;
static uint32_t last_display_refresh_ms = 0U;

static void write_pin(const Pin *pin, GPIO_PinState state)
{
    HAL_GPIO_WritePin(pin->port, pin->pin, state);
}

static void disable_all_digits(void)
{
    for (uint8_t i = 0U; i < 4U; i++)
    {
        write_pin(&digit_selects[i], DIGIT_SELECT_OFF);
    }
}

static void write_segments(uint8_t display_b, uint8_t digit, uint8_t blank)
{
    const Pin *pins = display_b ? segments_b : segments_a;
    uint8_t pattern = 0U;

    if ((blank == 0U) && (digit <= 9U))
    {
        pattern = digit_patterns[digit];
    }

    for (uint8_t i = 0U; i < 7U; i++)
    {
        GPIO_PinState state = (pattern & (1U << i)) ? SEGMENT_ON : SEGMENT_OFF;
        write_pin(&pins[i], state);
    }
}

void led_display_init(void)
{
    disable_all_digits();
    for (uint8_t i = 0U; i < 7U; i++)
    {
        write_pin(&segments_a[i], SEGMENT_OFF);
        write_pin(&segments_b[i], SEGMENT_OFF);
    }
    for (uint8_t i = 0U; i < 6U; i++)
    {
        HAL_GPIO_WritePin(traffic_lights[i].port, traffic_lights[i].pin, TRAFFIC_LED_OFF);
    }

    red_duration = 7U;
    green_duration = 5U;
    amber_duration = 2U;
    temp_red_duration = red_duration;
    temp_amber_duration = amber_duration;
    temp_green_duration = green_duration;

    last_blink_tick = timer_tick;
    last_second_tick = timer_tick;
    blink_visible = 1U;
    current_digit = 0U;
}

void led_display_set_traffic(unsigned char a_red, unsigned char a_amber, unsigned char a_green,
                             unsigned char b_red, unsigned char b_amber, unsigned char b_green)
{
    HAL_GPIO_WritePin(A_RED_GPIO_Port, A_RED_Pin, a_red ? TRAFFIC_LED_ON : TRAFFIC_LED_OFF);
    HAL_GPIO_WritePin(A_AMBER_GPIO_Port, A_AMBER_Pin, a_amber ? TRAFFIC_LED_ON : TRAFFIC_LED_OFF);
    HAL_GPIO_WritePin(A_GREEN_GPIO_Port, A_GREEN_Pin, a_green ? TRAFFIC_LED_ON : TRAFFIC_LED_OFF);
    HAL_GPIO_WritePin(B_RED_GPIO_Port, B_RED_Pin, b_red ? TRAFFIC_LED_ON : TRAFFIC_LED_OFF);
    HAL_GPIO_WritePin(B_AMBER_GPIO_Port, B_AMBER_Pin, b_amber ? TRAFFIC_LED_ON : TRAFFIC_LED_OFF);
    HAL_GPIO_WritePin(B_GREEN_GPIO_Port, B_GREEN_Pin, b_green ? TRAFFIC_LED_ON : TRAFFIC_LED_OFF);
}

void reset_traffic_light(void)
{
    state_a   = LIGHT_GREEN;  counter_a = green_duration;
    state_b   = LIGHT_RED;    counter_b = red_duration;
    last_second_tick = timer_tick;
}

//unsigned char get_red_duration(void)
//{
//    return temp_red_duration;
//}
//
//void inc_red_duration(void)
//{
//    temp_red_duration++;
//    if (temp_red_duration > 99U)
//    {
//        temp_red_duration = 1U;
//    }
//}
//
//void save_red_duration(void)
//{
//    red_duration = temp_red_duration;
//}

static unsigned char *temp_ptr(unsigned char mode)
{
    if (mode == 2U) return &temp_red_duration;
    if (mode == 3U) return &temp_amber_duration;
    if (mode == 4U) return &temp_green_duration;
    return 0;
}

unsigned char get_duration(unsigned char mode)
{
    unsigned char *p = temp_ptr(mode);
    return p ? *p : 0U;
}

void inc_duration(unsigned char mode)
{
    unsigned char *p = temp_ptr(mode);
    if (p == 0) return;
    (*p)++;
    if (*p > 99U) *p = 1U;
}

void save_duration(unsigned char mode)
{
    if (mode == 2U)      red_duration   = temp_red_duration;
    else if (mode == 3U) amber_duration = temp_amber_duration;
    else if (mode == 4U) green_duration = temp_green_duration;
}

#define ONE_SECOND_TICKS  (1000U / TIMER_PERIOD_MS)

static unsigned char duration_of(LightState s)
{
    if (s == LIGHT_RED)   return red_duration;
    if (s == LIGHT_GREEN) return green_duration;
    return amber_duration;
}

static LightState next_of(LightState s)
{
    if (s == LIGHT_RED)   return LIGHT_GREEN;
    if (s == LIGHT_GREEN) return LIGHT_AMBER;
    return LIGHT_RED;
}

static void update_normal_traffic(void)
{
    if ((uint32_t)(timer_tick - last_second_tick) >= ONE_SECOND_TICKS)
    {
        last_second_tick += ONE_SECOND_TICKS;
        counter_a--;
        counter_b--;

        if (counter_a <= 0)
        {
            state_a = next_of(state_a);
            counter_a = duration_of(state_a);
        }
        if (counter_b <= 0)
        {
            state_b = next_of(state_b);
            counter_b = duration_of(state_b);
        }
    }

    led_display_set_traffic(state_a == LIGHT_RED, state_a == LIGHT_AMBER, state_a == LIGHT_GREEN,
                            state_b == LIGHT_RED, state_b == LIGHT_AMBER, state_b == LIGHT_GREEN);
}

void led_display_task(unsigned char mode, unsigned char value)
{
	static unsigned char prev_mode = 0xFFU;
	if (mode != prev_mode)
	{
	    prev_mode = mode;
	    temp_red_duration   = red_duration;
	    temp_amber_duration = amber_duration;
	    temp_green_duration = green_duration;
	    if (mode == 1U) reset_traffic_light();
	}
    uint32_t now = timer_tick;

    if ((uint32_t)(now - last_blink_tick) >= BLINK_HALF_PERIOD_TICKS)
    {
        last_blink_tick = now;
        blink_visible = (uint8_t)!blink_visible;
    }

    uint8_t display_val = 0U;
    uint8_t display_val_b = 0U;
    if (mode == 1U)
    {
        update_normal_traffic();
        display_val   = (uint8_t)counter_a;
        display_val_b = (uint8_t)counter_b;
    }

    else if (mode >= 2U && mode <= 4U)
    {
        display_val = get_duration(mode);
        if (blink_visible)
        {
            if (mode == 2U)      led_display_set_traffic(1, 0, 0, 1, 0, 0);
            else if (mode == 3U) led_display_set_traffic(0, 1, 0, 0, 1, 0);
            else                 led_display_set_traffic(0, 0, 1, 0, 0, 1);
        }
        else
        {
            led_display_set_traffic(0, 0, 0, 0, 0, 0);
        }
    }

    display_digits[0] = display_val / 10U;
    display_digits[1] = display_val % 10U;
    display_blank[0] = (display_val < 10U) ? 1U : 0U;
    display_blank[1] = 0U;

    if (mode == 1U)
    {
        display_digits[2] = display_val_b / 10U;
        display_digits[3] = display_val_b % 10U;
        display_blank[2]  = (display_val_b < 10U) ? 1U : 0U;
        display_blank[3]  = 0U;
    }
    else
    {
        display_digits[2] = 0U;
        display_digits[3] = mode;
        display_blank[2]  = 1U;
        display_blank[3]  = 0U;
    }

    static uint32_t last_display_tick = 0U;
	if ((uint32_t)(now - last_display_tick) >= DISPLAY_REFRESH_TICKS)
	{
		last_display_tick = now;

		disable_all_digits();
		write_segments(0U, display_digits[current_digit], display_blank[current_digit]);
		write_segments(1U, display_digits[current_digit + 2U], display_blank[current_digit + 2U]);
		write_pin(&digit_selects[current_digit], DIGIT_SELECT_ON);
		write_pin(&digit_selects[current_digit + 2U], DIGIT_SELECT_ON);

		current_digit = (current_digit + 1U) % 2U;
	}
}
