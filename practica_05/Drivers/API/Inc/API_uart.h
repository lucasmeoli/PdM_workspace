/*
 * API_uart.h
 *
 *  Created on: Mar 30, 2024
 *      Author: lpmeoli
 */

#ifndef API_INC_API_UART_H_
#define API_INC_API_UART_H_

typedef bool bool_t;

/**
 * @brief  Configure the UART peripheral as 9600 - 7O1
 *         UART configured as follows:
 *         		- Word Length = 8 Bits (7 data bit + 1 parity bit)
 *         		- Stop Bit    = One Stop bit
 *         		- Parity      = ODD parity
 *         		- BaudRate    = 9600 baud
 *         		- Hardware flow control disabled (RTS and CTS signals)
 * @param  None
 * @retval None
 */
bool_t uart_init();


/**
 * @brief Sends a string through UART communication in blocking mode.
 * It calculates the size of the string to be sent.
 *
 * @param  pstring: Pointer to the string to be sent.
 * @retval None
*/
void uart_send_string(uint8_t * pstring);


/**
 * @brief Sends a string with specified size through UART communication
 * in blocking mode
 *

 * @param  pstring: Pointer to the string to be sent.
 * 		   size: Size of the string to be sent.
 * @retval None
*/
void uart_send_string_size(uint8_t * pstring, uint16_t size);



/**
 * @brief  Receives a string with specified size through UART communication
 * in blocking mode
 *
 * @param  pstring: Pointer to the buffer where the received string will be stored.
 * @param  size: Size of the buffer to store the received string.
 * @retval None
*/
void uart_receive_string_size(uint8_t * pstring, uint16_t size);


#endif /* API_INC_API_UART_H_ */
