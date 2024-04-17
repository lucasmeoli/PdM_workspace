/*
 * API_adxl345.c
 *
 *  Created on: Mar 31, 2024
 *      Author: lpmeoli
 */

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx_hal.h"
#include "stm32f4xx_nucleo_144.h"

#include "API_adxl345.h"

/* Private typedef -----------------------------------------------------------*/
typedef enum {
	REG_DEVID			= 0x00,
	REG_THRESH_ACT		= 0x24,
	REG_ACT_INACT_CTL	= 0x27,
	REG_BW_RATE			= 0x2C,
	REG_POWER_CTL 		= 0x2D,
	REG_INT_SOURCE		= 0x30,
	REG_DATA_FORMAT 	= 0x31,
	REG_DATAX0 			= 0x32,
	REG_DATAX1 			= 0x33,
	REG_DATAY0 			= 0x34,
	REG_DATAY1 			= 0x35,
	REG_DATAZ0 			= 0x36,
	REG_DATAZ1 			= 0x37,
} adxl345_register_t;

/* Private define ------------------------------------------------------------*/
#define ADXL345_ADDRESS 	0x53
#define CLOCK_SPEED			100000	// This parameter must be set to a value lower than 400kHz
#define REGISTER_DEVID		0xE5
#define MASK_DATA_READY		0x80

#define I2C_TIMEOUT			1000

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static I2C_HandleTypeDef hi2c1;

/* Private function prototypes -----------------------------------------------*/
static void write_register(uint8_t reg, uint8_t value);
static uint8_t read_register(uint8_t reg);


/* Public functions ---------------------------------------------------------*/

bool_t adlx345_I2C_init() {
	bool_t return_value = false;
	HAL_I2C_StateTypeDef i2c_state = HAL_I2C_GetState(&hi2c1);

	if (i2c_state == HAL_I2C_STATE_RESET) {
		/* I2C configuration*/
		hi2c1.Instance 				= I2C1;
		hi2c1.Init.ClockSpeed 		= CLOCK_SPEED;
		hi2c1.Init.DutyCycle 		= I2C_DUTYCYCLE_2;
		hi2c1.Init.AddressingMode 	= I2C_ADDRESSINGMODE_7BIT;
		hi2c1.Init.DualAddressMode 	= I2C_DUALADDRESS_DISABLE;
		hi2c1.Init.GeneralCallMode 	= I2C_GENERALCALL_DISABLE;
		hi2c1.Init.NoStretchMode 	= I2C_NOSTRETCH_DISABLE;

		HAL_I2C_MspInit(&hi2c1);

		// Init I2C and read static device ID to check communication
		if ((HAL_I2C_Init(&hi2c1) == HAL_OK) && (read_register(REG_DEVID) == REGISTER_DEVID)) {
			return_value = true;
		}
	} else if (i2c_state != HAL_I2C_STATE_ERROR) {
		// Not an STATE_ERROR or STATE_RESET,so initialization was already done
		return_value = true;
	}

	return return_value;
}


void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c1) {
	GPIO_InitTypeDef GPIO_InitStruct;

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/* I2C GPIO configuration */
	GPIO_InitStruct.Pin 		= GPIO_PIN_8|GPIO_PIN_9;
	GPIO_InitStruct.Mode 		= GPIO_MODE_AF_OD;
	GPIO_InitStruct.Pull 		= GPIO_NOPULL;
	GPIO_InitStruct.Speed 		= GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate 	= GPIO_AF4_I2C1;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	 /* Peripheral clock enable */
	__HAL_RCC_I2C1_CLK_ENABLE();
}


coordinates_t adxl345_read_coordinates() {
	uint8_t reg_data_coord = REG_DATAX0;
	coordinates_t coord;
	uint16_t coord_size = sizeof(coord)/ sizeof(uint8_t);

	HAL_I2C_Master_Transmit(&hi2c1, ADXL345_ADDRESS<<1, &reg_data_coord, sizeof(reg_data_coord), I2C_TIMEOUT);
	HAL_I2C_Master_Receive(&hi2c1, ADXL345_ADDRESS<<1, (uint8_t *) &coord, coord_size, I2C_TIMEOUT);

	return coord;
}


HAL_I2C_StateTypeDef adxl345_get_I2C_state() {
	return HAL_I2C_GetState(&hi2c1);
}

bool_t adxl345_is_data_ready() {
	uint8_t reg = REG_INT_SOURCE;
	uint8_t value = read_register(reg);

	if (value&MASK_DATA_READY) {
		return true;
	} else {
		return false;
	}
}

void adxl345_set_sensitivity(adxl345_sensitivity_t sensitivity) {
	uint8_t reg_sensitivity = sensitivity;

	if (sensitivity > RESOLUTION_16G) {
		return;
	}

	write_register(REG_DATA_FORMAT, reg_sensitivity);
}


#define DEFINE_ADXL345_REGISTER_FUNCTION(name, reg) \
    void adxl345_set_##name(uint8_t value) { \
        write_register(reg, value); \
    }

DEFINE_ADXL345_REGISTER_FUNCTION(bandwidth_rate, REG_BW_RATE)
DEFINE_ADXL345_REGISTER_FUNCTION(power_control, REG_POWER_CTL)
DEFINE_ADXL345_REGISTER_FUNCTION(data_format, REG_DATA_FORMAT)




/* Private functions ---------------------------------------------------------*/

/**
 * @brief   Reads the value from the specified register of the ADXL345 accelerometer.
 *
 * @param   reg: The register address to read from.
 * @retval  The value read from the specified register.
 */
static uint8_t read_register(uint8_t reg) {
	uint8_t value;

	HAL_I2C_Master_Transmit(&hi2c1, ADXL345_ADDRESS<<1, &reg, sizeof(reg), I2C_TIMEOUT);
	HAL_I2C_Master_Receive(&hi2c1, ADXL345_ADDRESS<<1, &value, sizeof(value), I2C_TIMEOUT);

	return value;
}



/**
 * @brief   Writes a value to the specified register of the ADXL345 accelerometer.
 * @param   reg: The register address to write to.
 * @param   value: The value to write to the register.
 * @retval  None.
 */
static void write_register(uint8_t reg, uint8_t value) {
	 uint8_t buf[] = {reg, value};
	 uint16_t size = sizeof(buf) / sizeof(uint8_t);

	 HAL_I2C_Master_Transmit(&hi2c1, ADXL345_ADDRESS<<1, buf, size, I2C_TIMEOUT);
}


