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

/* Functions ---------------------------------------------------------*/
void debounceFSM_init(uint32_t time) {
	current_state = BUTTON_UP;
	debounce_time = time;
	rising_edge = false;

	BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO);
	delayInit(&debounce_timer, debounce_time);
}

void debounceFSM_update() {
	switch (current_state) {
		case BUTTON_UP:
			if(BSP_PB_GetState(BUTTON_USER)) {
				current_state = BUTTON_FALLING;
			}
			break;

		case BUTTON_FALLING:
			if (delayRead(&debounce_timer)) {
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
			if (delayRead(&debounce_timer)) {
				if(!BSP_PB_GetState(BUTTON_USER)) {
					current_state = BUTTON_UP;
				} else {
					current_state = BUTTON_DOWN;
				}
			}
			break;

		default:
			break;
	}
}

bool_t readKey() {
	bool_t return_value = false;

	if (rising_edge) {
		rising_edge = false;
		return_value = true;
	}

	return return_value;
}

