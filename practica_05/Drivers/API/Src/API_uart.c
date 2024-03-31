/*
 * API_uart.c
 *
 *  Created on: Mar 30, 2024
 *      Author: lpmeoli
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx_hal.h"
#include "stm32f4xx_nucleo_144.h"
#include "API_uart.h"


/* Definition for USARTx clock resources */
#define USARTx          USART3
#define BAUDRATE		9600
#define MSG_UART_INIT	"UART iniciada, configuracion: 9600 - 7O1 \n\r"

static UART_HandleTypeDef UartHandle;
static uint8_t msg_uart_init[] = MSG_UART_INIT;


bool_t uart_init() {

	  UartHandle.Instance        	= USARTx;
	  UartHandle.Init.BaudRate   	= BAUDRATE;
	  UartHandle.Init.WordLength	= UART_WORDLENGTH_8B;
	  UartHandle.Init.StopBits   	= UART_STOPBITS_1;
	  UartHandle.Init.Parity     	= UART_PARITY_ODD;
	  UartHandle.Init.HwFlowCtl  	= UART_HWCONTROL_NONE;
	  UartHandle.Init.Mode       	= UART_MODE_TX_RX;
	  UartHandle.Init.OverSampling 	= UART_OVERSAMPLING_16;


	  if (HAL_UART_Init(&UartHandle) != HAL_OK) {
		  return false;
	  }

	  uart_send_string(msg_uart_init);

	  return true;
}


void uart_send_string(uint8_t * pstring) {
	uint32_t size = 0;

	if(pstring == NULL) {
		return;
	}

	for(; pstring[size] != '\0'; size++ );
	HAL_UART_Transmit(&UartHandle, pstring, size, HAL_MAX_DELAY);
}


void uart_send_string_size(uint8_t * pstring, uint16_t size) {

	if ((pstring == NULL) || (size == 0)) {
		return;
	}

	HAL_UART_Transmit(&UartHandle, pstring, size, HAL_MAX_DELAY);
}


void uart_receive_string_size(uint8_t * pstring, uint16_t size) {

	if ((pstring == NULL) || (size == 0)) {
		return;
	}

	HAL_UART_Receive(&UartHandle, pstring, size, HAL_MAX_DELAY);
}
