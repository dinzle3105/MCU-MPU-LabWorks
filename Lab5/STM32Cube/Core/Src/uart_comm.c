/*
 * uart_comm.c
 *
 * Communication FSM:
 *
 *            command == RST                 (packet is built, ADC read once)
 *   IDLE ------------------------> SEND
 *     ^                              |  transmit packet, start 3 s timeout
 *     |                              v
 *     |        command == OK     WAIT_OK
 *     +------------------------------|
 *                                    | timeout (3 s)
 *                                    +--> SEND  (same packet, value is kept)
 */

#include "uart_comm.h"
#include "command_parser.h"
#include <stdio.h>

extern ADC_HandleTypeDef  hadc1;
extern UART_HandleTypeDef huart2;

typedef enum {
    COMM_IDLE = 0,
    COMM_SEND,
    COMM_WAIT_OK
} CommState;

static CommState comm_state  = COMM_IDLE;
static uint32_t  ADC_value   = 0;
static uint8_t   packet[PACKET_SIZE];
static uint16_t  packet_len  = 0;
static uint32_t  timeout_tick = 0;

void uart_comm_init(void)
{
    comm_state = COMM_IDLE;

    /* ADC runs in continuous mode: start it once, then just read the result */
    HAL_ADCEx_Calibration_Start(&hadc1);
    HAL_ADC_Start(&hadc1);
}

void uart_communiation_fsm(void)
{
    switch (comm_state)
    {
    case COMM_IDLE:
        if (command_flag == 1)
        {
            command_flag = 0;

            if (command_data == CMD_RST)
            {
                /* read the sensor and build the packet !ADC=1234# */
                ADC_value  = HAL_ADC_GetValue(&hadc1);
                packet_len = (uint16_t)snprintf((char *)packet, PACKET_SIZE,
                                                "!ADC=%lu#\r\n",
                                                (unsigned long)ADC_value);
                comm_state = COMM_SEND;
            }
            /* !OK# without a pending request is ignored */
        }
        break;

    case COMM_SEND:
        /* (re)send the stored packet: the value is kept as the previous one */
        HAL_UART_Transmit(&huart2, packet, packet_len, 1000);
        timeout_tick = HAL_GetTick();
        comm_state   = COMM_WAIT_OK;
        break;

    case COMM_WAIT_OK:
        if (command_flag == 1)
        {
            command_flag = 0;
            /* --- TEMP DEBUG: show what command_data actually is --- */
//			char dbg[16];
//			int n = snprintf(dbg, sizeof(dbg), "[cmd=%d]\r\n", command_data);
//			HAL_UART_Transmit(&huart2, (uint8_t *)dbg, n, 50);
			/* --- END TEMP DEBUG --- */
            if (command_data == CMD_OK)
            {
                comm_state = COMM_IDLE;      /* communication finished */
                break;
            }
            /* any other command (e.g. a second !RST#) is ignored while waiting */
        }

        if (HAL_GetTick() - timeout_tick >= TIMEOUT_WAIT_OK_MS)
        {
            comm_state = COMM_SEND;          /* timeout: send the packet again */
        }
        break;

    default:
        comm_state = COMM_IDLE;
        break;
    }
}
