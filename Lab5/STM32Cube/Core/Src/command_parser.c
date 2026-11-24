/*
 * command_parser.c
 *
 * Command parser FSM.
 *
 *   The UART interrupt only stores the received byte in 'buffer' and sets
 *   'buffer_flag'. The FSM below runs in the main loop and processes the
 *   stored bytes:
 *
 *        '!'                    '#'  (text == "RST" or "OK")
 *   IDLE ----> READING ------------------------------------> IDLE
 *                 |  '!'  : restart the command text            (command_flag = 1,
 *                 |  too long : drop it, back to IDLE            command_data = code)
 */

#include "command_parser.h"
#include <string.h>

extern UART_HandleTypeDef huart2;

/* ---- reception buffer (written by the interrupt) ---- */
uint8_t          temp         = 0;
volatile uint8_t buffer[MAX_BUFFER_SIZE];
volatile uint8_t index_buffer = 0;   /* write index (interrupt) */
volatile uint8_t buffer_flag  = 0;

/* ---- output of the parser ---- */
uint8_t command_flag = 0;
uint8_t command_data = CMD_NONE;

/* ---- private ---- */
typedef enum {
    PARSER_IDLE = 0,
    PARSER_READING
} ParserState;

static ParserState parser_state = PARSER_IDLE;
static uint8_t     index_read   = 0;                 /* read index (main loop) */
static uint8_t     cmd_text[MAX_CMD_LEN + 1];
static uint8_t     cmd_len      = 0;

/* Add the received character into the buffer */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART2)
    {
        buffer[index_buffer++] = temp;
        if (index_buffer == MAX_BUFFER_SIZE) index_buffer = 0;

        buffer_flag = 1;
        HAL_UART_Receive_IT(&huart2, &temp, 1);
    }
}

void command_parser_init(void)
{
    parser_state  = PARSER_IDLE;
    index_read    = 0;
    index_buffer  = 0;
    buffer_flag   = 0;
    command_flag  = 0;
    command_data  = CMD_NONE;
    cmd_len       = 0;

    HAL_UART_Receive_IT(&huart2, &temp, 1);
}

/* Called when '#' is received: decide which command the text represents */
static void command_decode(void)
{
    cmd_text[cmd_len] = '\0';

    if (strcmp((char *)cmd_text, "RST") == 0)
    {
        command_data = CMD_RST;
        command_flag = 1;
    }
    else if (strcmp((char *)cmd_text, "OK") == 0)
    {
        command_data = CMD_OK;
        command_flag = 1;
    }
    /* anything else is an unknown command and is ignored */
}

/* FSM step for one received character */
static void parser_step(uint8_t c)
{
    switch (parser_state)
    {
    case PARSER_IDLE:
        if (c == '!')
        {
            cmd_len = 0;
            parser_state = PARSER_READING;
        }
        break;

    case PARSER_READING:
        if (c == '#')
        {
            command_decode();
            parser_state = PARSER_IDLE;
        }
        else if (c == '!')
        {
            cmd_len = 0;                       /* start of a new command */
        }
        else if (cmd_len < MAX_CMD_LEN)
        {
            cmd_text[cmd_len++] = c;
        }
        else
        {
            parser_state = PARSER_IDLE;        /* too long -> invalid    */
        }
        break;

    default:
        parser_state = PARSER_IDLE;
        break;
    }
}

void command_parser_fsm(void)
{
    if (buffer_flag == 1)
    {
        buffer_flag = 0;

        /* process every byte the interrupt has stored so far */
        while (index_read != index_buffer)
        {
            uint8_t c = buffer[index_read++];
            if (index_read == MAX_BUFFER_SIZE) index_read = 0;

            /* echo to the terminal */
            HAL_UART_Transmit(&huart2, &c, 1, 50);
            if (c == '#')
            {
                HAL_UART_Transmit(&huart2, (uint8_t *)"\r\n", 2, 50);
            }

            parser_step(c);
        }
    }

    /* restart the reception if it was stopped (e.g. after an overrun error) */
    if (huart2.RxState == HAL_UART_STATE_READY)
    {
        HAL_UART_Receive_IT(&huart2, &temp, 1);
    }
}
