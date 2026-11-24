/*
 * command_parser.h
 *
 * Extracts commands of the form !RST# and !OK# from the bytes received on
 * USART2 (interrupt driven, one byte at a time).
 */

#ifndef INC_COMMAND_PARSER_H_
#define INC_COMMAND_PARSER_H_

#include "main.h"

#define MAX_BUFFER_SIZE   30
#define MAX_CMD_LEN       8      /* longest text allowed between '!' and '#' */

/* Values placed in command_data */
#define CMD_NONE          0
#define CMD_RST           1      /* !RST# : request sensor data  */
#define CMD_OK            2      /* !OK#  : end of communication */

/* Reception buffer (filled by the UART interrupt) */
extern uint8_t           temp;
extern volatile uint8_t  buffer[MAX_BUFFER_SIZE];
extern volatile uint8_t  index_buffer;
extern volatile uint8_t  buffer_flag;

/* Output of the command parser */
extern uint8_t           command_flag;   /* 1 = a new command is available */
extern uint8_t           command_data;   /* CMD_RST or CMD_OK              */

void command_parser_init(void);
void command_parser_fsm(void);

#endif /* INC_COMMAND_PARSER_H_ */
