/*
 * API_max7219_led_display.c
 *
 *  Created on: Apr 1, 2024
 *      Author: lpmeoli
 */


/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx_hal.h"
#include "stm32f4xx_nucleo_144.h" 	/* <- BSP include */

#include "API_max7219.h"

/* Private typedef -----------------------------------------------------------*/
typedef enum {
    REG_NO_OP           = 0x00,
    REG_DIGIT_0         = 0x01,
    REG_DIGIT_1         = 0x02,
    REG_DIGIT_2         = 0x03,
    REG_DIGIT_3         = 0x04,
    REG_DIGIT_4         = 0x05,
    REG_DIGIT_5         = 0x06,
    REG_DIGIT_6         = 0x07,
    REG_DIGIT_7         = 0x08,
    REG_DECODE_MODE     = 0x09,
    REG_INTENSITY       = 0x0A,
    REG_SCAN_LIMIT      = 0x0B,
    REG_SHUTDOWN        = 0x0C,
    REG_DISPLAY_TEST    = 0x0F,
} MAX7219_REGISTERS;

/* Private define ------------------------------------------------------------*/
#define MAX_DISPLAYS	4

#define SPI_TIMEOUT		1000
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static SPI_HandleTypeDef hspi1;
static uint8_t active_displays = MAX_DISPLAYS; // Set max as default

/* Private function prototypes -----------------------------------------------*/
static void max7219_send_data(uint8_t reg, uint8_t data, uint8_t display_num);

/* Public functions ---------------------------------------------------------*/
bool_t max7219_SPI_init() {
	bool_t return_value = false;
	HAL_SPI_StateTypeDef spi_state = HAL_SPI_GetState(&hspi1);

	if(spi_state == HAL_SPI_STATE_RESET) {
		/* SPI configuration*/
		hspi1.Instance 					= SPI1;
		hspi1.Init.Mode 				= SPI_MODE_MASTER;
		hspi1.Init.Direction			= SPI_DIRECTION_1LINE;
		hspi1.Init.DataSize				= SPI_DATASIZE_16BIT;
		hspi1.Init.CLKPolarity			= SPI_POLARITY_LOW;
		hspi1.Init.CLKPhase				= SPI_PHASE_1EDGE;
		hspi1.Init.NSS 					= SPI_NSS_SOFT;
		hspi1.Init.BaudRatePrescaler 	= SPI_BAUDRATEPRESCALER_128;
		hspi1.Init.FirstBit				= SPI_FIRSTBIT_MSB;
		hspi1.Init.TIMode				= SPI_TIMODE_DISABLE;
		hspi1.Init.CRCCalculation 		= SPI_CRCCALCULATION_DISABLE;

		HAL_SPI_MspInit(&hspi1);
		if (HAL_SPI_Init(&hspi1) == HAL_OK) {
			return_value = true;
		}

		// Release Chip Select pin
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
	} else if (spi_state != HAL_SPI_STATE_ERROR) {
		// Not an STATE_ERROR or STATE_RESET,so initialization was already done
		return_value = true;
	}
	return return_value;
}


void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi) {
	GPIO_InitTypeDef  GPIO_InitStruct;

  	/*** Configure the GPIOs ***/
  	/* Enable GPIO clock */
	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOA_CLK_ENABLE();

	/* Configure SPI SCK */
	GPIO_InitStruct.Pin 		= GPIO_PIN_5;
	GPIO_InitStruct.Mode 		= GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull  		= GPIO_PULLDOWN;
	GPIO_InitStruct.Speed 		= GPIO_SPEED_HIGH;
	GPIO_InitStruct.Alternate 	= GPIO_AF5_SPI1;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/* Configure SPI MOSI */
	GPIO_InitStruct.Pin 		= GPIO_PIN_7;
	GPIO_InitStruct.Mode 		= GPIO_MODE_AF_PP;
	GPIO_InitStruct.Speed 		= GPIO_SPEED_HIGH;
	GPIO_InitStruct.Alternate 	= GPIO_AF5_SPI1;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure SPI CS */
	GPIO_InitStruct.Pin 		= GPIO_PIN_4;
	GPIO_InitStruct.Mode 		= GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull 		= GPIO_PULLUP;
	GPIO_InitStruct.Speed 		= GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*** Configure the SPI peripheral ***/
	/* Enable SPI clock */
	/* Peripheral clock enable */
	__HAL_RCC_SPI1_CLK_ENABLE();
}


void max7219_clean_display(uint8_t display_num) {
	if (display_num >= active_displays) {
		return;
	}

	max7219_send_data(REG_DIGIT_0, 0x00, display_num);
	max7219_send_data(REG_DIGIT_1, 0x00, display_num);
	max7219_send_data(REG_DIGIT_2, 0x00, display_num);
	max7219_send_data(REG_DIGIT_3, 0x00, display_num);
	max7219_send_data(REG_DIGIT_4, 0x00, display_num);
	max7219_send_data(REG_DIGIT_5, 0x00, display_num);
	max7219_send_data(REG_DIGIT_6, 0x00, display_num);
	max7219_send_data(REG_DIGIT_7, 0x00, display_num);
}


void max7219_clean_all_displays() {
	for (uint8_t i = 0; i < MAX_DISPLAYS; i++) {
		max7219_send_data(REG_DIGIT_0, 0x00, i);
		max7219_send_data(REG_DIGIT_1, 0x00, i);
		max7219_send_data(REG_DIGIT_2, 0x00, i);
		max7219_send_data(REG_DIGIT_3, 0x00, i);
		max7219_send_data(REG_DIGIT_4, 0x00, i);
		max7219_send_data(REG_DIGIT_5, 0x00, i);
		max7219_send_data(REG_DIGIT_6, 0x00, i);
		max7219_send_data(REG_DIGIT_7, 0x00, i);
	}
}


void max7219_turn_on_LEDs(uint8_t reg, uint8_t data, uint8_t display_num) {
	if ((reg >= REG_DIGIT_0) && (reg <= REG_DIGIT_7)) {
		max7219_send_data(reg, data, display_num);
	}
}


/* ---------- Setters ---------- */
#define MAX7219_SET_REGISTER_FUNC(name,reg) \
void max7219_set_##name(uint8_t data, uint8_t display_num) { \
    max7219_send_data(reg, data, display_num); \
}

MAX7219_SET_REGISTER_FUNC(decode_mode, REG_DECODE_MODE)
MAX7219_SET_REGISTER_FUNC(intensity, REG_INTENSITY)
MAX7219_SET_REGISTER_FUNC(scan_limit, REG_SCAN_LIMIT)
MAX7219_SET_REGISTER_FUNC(shutdown, REG_SHUTDOWN)

void max7219_set_displays(uint8_t total_displays) {
	if (total_displays > MAX_DISPLAYS) {
		active_displays = MAX_DISPLAYS;
	} else {
		active_displays = total_displays;
	}
}


/* ---------- Getters ---------- */
HAL_SPI_StateTypeDef max7219_get_SPI_state() {
	return HAL_SPI_GetState(&hspi1);
}


/* Private functions ---------------------------------------------------------*/
/**
  * @brief Sends data to a specific register of a specific display.
  *
  * @param reg: The register address to write to.
  * 			data: The data to write to the register.
  * 			display_num: The number of the display to write data to. First display corresponds to number 0
  * @retval None
  */
static void max7219_send_data(uint8_t reg, uint8_t data, uint8_t display_num) {
	uint16_t buf = reg<<8 | data;
	uint16_t no_op = 0;

	if (display_num >= active_displays) {
		return;
	}

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);

	/* The no-op register is used for cascading MAX7219 since is acts like a shift register of 16-bit word
	   To address a specific chip, send the desired 16-bit word followed by the appropriate no-op codes.
	   The positioning of the no-op codes depends on the chip you want to communicate with, either
	   preceding or following the desired word.	For example, in a cascade of four MAX7219s, writing to the
	   third chip requires sending first one no-op code, then the desired word followed by two no-op codes.
	   When LOAD/CS transitions high, data is latched in all devices.
	*/
	for (uint8_t i = ((active_displays-1)-display_num); i > 0; i--) {
	    HAL_SPI_Transmit(&hspi1, (uint8_t *)&no_op, sizeof(uint8_t), SPI_TIMEOUT);
	}

	HAL_SPI_Transmit(&hspi1, (uint8_t *)&buf, sizeof(uint8_t), SPI_TIMEOUT);

	for (uint8_t i = 0; i < display_num; i++) {
		HAL_SPI_Transmit(&hspi1, (uint8_t *)&no_op, sizeof(uint8_t), SPI_TIMEOUT);
	}

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
}




