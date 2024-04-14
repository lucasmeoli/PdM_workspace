/*
 * API_uart.c
 *
 *  Created on: Mar 30, 2024
 *      Author: lpmeoli
 *
 * @details This header file contains functions to configure and manage UART communication.
 *          It provides functions to initialize UART, send and receive strings in blocking mode.
 */

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx_hal.h"
#include "stm32f4xx_nucleo_144.h"

#include "API_uart.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Definition for USARTx clock resources */
#define USARTx          USART3
#define BAUDRATE		115200
#define WORDLENGTH		UART_WORDLENGTH_8B
#define STOP_BITS		UART_STOPBITS_1
#define PARITY			UART_PARITY_NONE
#define MSG_UART_INIT	"UART initiated. Configuration: 115200 - 8O1 \n\r"

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static UART_HandleTypeDef UartHandle;
static uint8_t msg_uart_init[] = MSG_UART_INIT;

/* Public functions ---------------------------------------------------------*/
bool_t uart_init() {
	bool_t return_value = false;
	HAL_UART_StateTypeDef uart_state = HAL_UART_GetState(&UartHandle);

	if (uart_state == HAL_UART_STATE_RESET) {
		// Init UART only if it was not initialized before
		UartHandle.Instance        		= USARTx;
		UartHandle.Init.BaudRate   		= BAUDRATE;
		UartHandle.Init.WordLength		= WORDLENGTH;
		UartHandle.Init.StopBits   		= STOP_BITS;
		UartHandle.Init.Parity     		= PARITY;
		UartHandle.Init.HwFlowCtl  		= UART_HWCONTROL_NONE;
		UartHandle.Init.Mode       		= UART_MODE_TX_RX;
		UartHandle.Init.OverSampling 	= UART_OVERSAMPLING_16;

		if (HAL_UART_Init(&UartHandle) == HAL_OK) {
			return_value = true;
			uart_send_string(msg_uart_init);
		}
	} else if(uart_state != HAL_UART_STATE_ERROR) {
		// Not an STATE_ERROR or STATE_RESET,so initialization was already done
		return_value = true;
	}

	return return_value;
}

HAL_UART_StateTypeDef uart_get_state() {
	return HAL_UART_GetState(&UartHandle);
}


void uart_send_string(uint8_t * pstring) {
	uint32_t size = 0;

	if(pstring == NULL) {
		return;
	}

	for(; pstring[size] != '\0'; size++ );
	HAL_UART_Transmit(&UartHandle, pstring, size, 1000);
}


void uart_send_string_size(uint8_t * pstring, uint16_t size) {

	if ((pstring == NULL) || (size == 0)) {
		return;
	}

	HAL_UART_Transmit(&UartHandle, pstring, size, 1000);
}


void uart_receive_string_size(uint8_t * pstring, uint16_t size) {

	if ((pstring == NULL) || (size == 0)) {
		return;
	}

	HAL_UART_Receive(&UartHandle, pstring, size, 10000);
}
