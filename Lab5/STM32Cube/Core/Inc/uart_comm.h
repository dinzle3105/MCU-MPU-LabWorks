/*
 * uart_comm.h
 *
 * Communication protocol FSM:
 *   user  -> !RST#        request sensor data
 *   STM32 -> !ADC=1234#   ADC value (re-sent every 3 s until acknowledged)
 *   user  -> !OK#         end of the communication
 */

#ifndef INC_UART_COMM_H_
#define INC_UART_COMM_H_

#include "main.h"

#define TIMEOUT_WAIT_OK_MS   3000   /* wait for !OK# before re-sending */
#define PACKET_SIZE          24

void uart_comm_init(void);
void uart_communiation_fsm(void);

#endif /* INC_UART_COMM_H_ */
