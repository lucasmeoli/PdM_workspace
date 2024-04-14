/*
 * API_uart.h
 *
 *  Created on: Mar 30, 2024
 *      Author: lpmeoli
 *
 * Description:
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef API_INC_API_UART_H_
#define API_INC_API_UART_H_

/* Exported types ------------------------------------------------------------*/
typedef bool bool_t;

/* Exported functions --------------------------------------------------------*/

/**
 * @brief  Configure the UART peripheral as 115200 - 8N1
 *         UART configured as follows:
 *         		- Word Length = 8 Bits
 *         		- Stop Bit    = One Stop bit
 *         		- Parity      = None parity
 *         		- BaudRate    = 115200 baud
 *         		- Hardware flow control disabled (RTS and CTS signals)
 * @param  None
 * @retval Boolean value indicating whether UART was initialized.
 *         If it was previously initialized by another module and it is
 *         operational, it will also return true.
*/
bool_t uart_init();

/**
  * @brief  Get the current state of the UART communication interface.
  *
  * @param  None
  * @retval HAL_UART_StateTypeDef - Current state of the UART communication.
  */
HAL_UART_StateTypeDef uart_get_state();

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
