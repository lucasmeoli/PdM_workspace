/*
 * API_adlx345_sensitivity.c
 *
 *  Created on: Apr 9, 2024
 *      Author: lpmeoli
 */

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx_hal.h"
#include "stm32f4xx_nucleo_144.h"
#include "API_adxl345.h"
#include "API_uart.h"

#include "API_adxl345_sensitivity.h"

/* Private typedef -----------------------------------------------------------*/
typedef enum{
	SENSITIVITY_2G		= 0x00,
	SENSITIVITY_4G		= 0x01,
	SENSITIVITY_8G		= 0x02,
	SENSITIVITY_16G		= 0x03,
} sensitivityState_t;

/* Private define ------------------------------------------------------------*/
#define	MSG_2G_SENSITIVITY 	"Sensitivity ± 2g \n\r"
#define	MSG_4G_SENSITIVITY 	"Sensitivity ± 4g \n\r"
#define	MSG_8G_SENSITIVITY 	"Sensitivity ± 8g \n\r"
#define	MSG_16G_SENSITIVITY "Sensitivity ± 16g \n\r"

#define SENSITIVITY_UP_TAPS 		1
#define SENSITIVITY_DOWN_TAPS 		2

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static uint8_t msg_2g[]  = MSG_2G_SENSITIVITY;
static uint8_t msg_4g[]  = MSG_4G_SENSITIVITY;
static uint8_t msg_8g[]  = MSG_8G_SENSITIVITY;
static uint8_t msg_16g[] = MSG_16G_SENSITIVITY;

static sensitivityState_t current_state;

/* Private function prototypes -----------------------------------------------*/
/* Public functions ---------------------------------------------------------*/
bool_t sensitivity_FSM_init() {
	current_state = SENSITIVITY_2G;
	HAL_UART_StateTypeDef uart_state = uart_get_state();

	if ((uart_state == HAL_UART_STATE_ERROR) || (uart_state == HAL_UART_STATE_RESET)) {
		if (!uart_init()) {
			return false;
		}
	}
	return true;
}

void sensitivity_FSM_update(uint8_t taps) {
	switch (current_state) {
		case SENSITIVITY_2G:
			if (taps == SENSITIVITY_DOWN_TAPS) {
				adxl345_set_sensitivity(RESOLUTION_4G);
				current_state = SENSITIVITY_4G;
				uart_send_string(msg_4g);
			} else if (taps == SENSITIVITY_UP_TAPS) {
				uart_send_string(msg_2g);
			}
			break;

		case SENSITIVITY_4G:
			if (taps == SENSITIVITY_DOWN_TAPS) {
				adxl345_set_sensitivity(RESOLUTION_8G);
				current_state = SENSITIVITY_8G;
				uart_send_string(msg_8g);
			} else if (taps == SENSITIVITY_UP_TAPS) {
				adxl345_set_sensitivity(RESOLUTION_2G);
				current_state = SENSITIVITY_2G;
				uart_send_string(msg_2g);
			}
			break;

		case SENSITIVITY_8G:
			if (taps == SENSITIVITY_DOWN_TAPS) {
				adxl345_set_sensitivity(RESOLUTION_16G);
				current_state = SENSITIVITY_16G;
				uart_send_string(msg_16g);
			} else if (taps == SENSITIVITY_UP_TAPS) {
				adxl345_set_sensitivity(RESOLUTION_4G);
				current_state = SENSITIVITY_4G;
				uart_send_string(msg_4g);
			}
			break;

		case SENSITIVITY_16G:
			if (taps == SENSITIVITY_DOWN_TAPS) {
				uart_send_string(msg_16g);
			} else if (taps == SENSITIVITY_UP_TAPS) {
				adxl345_set_sensitivity(RESOLUTION_8G);
				current_state = SENSITIVITY_8G;
				uart_send_string(msg_8g);
			}
			break;

		default:
			// Should never reach here. In that case go to initial state
			current_state = SENSITIVITY_2G;
			break;
	}
}

