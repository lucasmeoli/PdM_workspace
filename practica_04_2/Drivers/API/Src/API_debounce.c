/*
 * API_debounce.c
 *
 *  Created on: Mar 22, 2024
 *      Author: lpmeoli
 */

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "stm32f4xx_hal.h"  		/* <- HAL include */
#include "stm32f4xx_nucleo_144.h"
#include "API_delay.h"
#include "API_debounce.h"
#include <stdint.h>
#include <stdbool.h>

/* Private typedef -----------------------------------------------------------*/
typedef enum{
	BUTTON_UP,
	BUTTON_FALLING,
	BUTTON_DOWN,
	BUTTON_RAISING,
} debounceState_t;


/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static debounceState_t current_state;
static uint32_t debounce_time;
static delay_t debounce_timer;
static bool_t rising_edge;


/* Private function prototypes -----------------------------------------------*/
static void FSM_error_handler(void);

/* Public functions ---------------------------------------------------------*/
void debounce_FSM_init(uint32_t time) {
	current_state = BUTTON_UP;
	debounce_time = time;
	rising_edge = false;

	BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO);
	delay_init(&debounce_timer, debounce_time);
}

void debounce_FSM_update() {
	switch (current_state) {
		case BUTTON_UP:
			if(BSP_PB_GetState(BUTTON_USER)) {
				current_state = BUTTON_FALLING;
			}
			break;

		case BUTTON_FALLING:
			if (delay_read(&debounce_timer)) {
				if(BSP_PB_GetState(BUTTON_USER)) {
					current_state = BUTTON_DOWN;
					rising_edge = true;
				} else {
					current_state = BUTTON_UP;
				}
			}
			break;

		case BUTTON_DOWN:
			if(!BSP_PB_GetState(BUTTON_USER)) {
				current_state = BUTTON_RAISING;
			}
			break;

		case BUTTON_RAISING:
			if (delay_read(&debounce_timer)) {
				if(!BSP_PB_GetState(BUTTON_USER)) {
					current_state = BUTTON_UP;
				} else {
					current_state = BUTTON_DOWN;
				}
			}
			break;

		default:
			FSM_error_handler();
			break;
	}
}

bool_t read_key() {
	bool_t return_value = false;

	if (rising_edge) {
		rising_edge = false;
		return_value = true;
	}

	return return_value;
}


/**
 * @brief  This function is executed in case of error occurrence.
 *
 * @param  None
 * @retval None
 */
static void FSM_error_handler(void) {
	/* Turn LED2 on */
	BSP_LED_Init(LED2);
	BSP_LED_On(LED2);
	while (1)
	{
	}
}

