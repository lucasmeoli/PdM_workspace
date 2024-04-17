/*
 * API_decode_coordinates.c
 *
 *  Created on: Apr 8, 2024
 *      Author: lpmeoli
 */
/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx_hal.h"  		/* <- HAL include */
#include "stm32f4xx_nucleo_144.h"
#include "API_uart.h"
#include "API_adxl345.h"
#include "API_max7219.h"
#include "API_decode_coordinates.h"

/* Private typedef -----------------------------------------------------------*/
typedef enum {
    CONFIGURE_MODULES,
    READ_COORDINATES,
    DECODE_COORDINATES,
    DISPLAY_COORDINATES,
} coordinatesState_t;

/* Private define ------------------------------------------------------------*/
#define ADXL345_BW_RATE_VALUE 		0x08 // Set normal operation and 25 Hz output data rate
#define ADXL345_DATA_FORMAT_VALUE 	0x08 // Set full resolution and 2g sensitivity
#define ADXL345_POWER_CTL_VALUE 	0x08 // Set measurement mode
#define COORD_MAX_VALUE    			256
#define COORD_MIN_VALUE				(-COORD_MAX_VALUE) // Coordinates value are symmetrical so MAX_VALUE = -MIN_VALUE

#define TOTAL_DISPLAYS       		4
#define DISPLAY_DIVISIONS  			8

#define DISPLAY_X_INTERVAL			(COORD_MAX_VALUE*2) 					 // For an 8x32 matrix, X interval is total for each display
#define RANGE_X_INTERVAL			(DISPLAY_X_INTERVAL / DISPLAY_DIVISIONS) // For each 8 leds in the X coord there is a range of possible values
#define DISPLAY_Y_INTERVAL   		((COORD_MAX_VALUE*2) / TOTAL_DISPLAYS)   // For an 8x32 matrix, Y interval must be divided in the totals displays
#define RANGE_Y_DIVISON      		(DISPLAY_Y_INTERVAL / DISPLAY_DIVISIONS) // For each 8 leds in the Y coord there is a range of possible values

#define MSG_COORDINATE_X			"Coordinate X: "
#define MSG_COORDINATE_Y			" - Coordinate Y: "
#define MSG_NEW_LINE				"\n\r"

#define SHUTDOWN_MODE_VALUE			0x00
#define NORMAL_MODE_VALUE			0x01
#define NO_DECODE_MODE_VALUE		0X00 // No BCD decode value is used for turning on and of specific LEDs
#define SCAN_LIMIT_VALUE			0x07 // All display digits available (Display digits 0 1 2 3 4 5 6 7)
#define	INTENSITY_VALUE				0x03 // Set low-medium intensity (min value 0x00, max value 0x0F)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static int16_t display_limits[TOTAL_DISPLAYS] = {-DISPLAY_Y_INTERVAL, 0, DISPLAY_Y_INTERVAL, COORD_MAX_VALUE};
static uint8_t msg_coordinate_x[] = MSG_COORDINATE_X;
static uint8_t msg_coordinate_y[] = MSG_COORDINATE_Y;
static uint8_t msg_new_line[] = MSG_NEW_LINE;

static uint16_t decoded_x_coordinate;
static uint16_t decoded_y_coordinate;
static uint8_t current_display;
static coordinatesState_t current_state;
static coordinates_t coordinates;

/* Private function prototypes -----------------------------------------------*/
static void configure_adxl345();
static void configure_max7219();
static bool_t decode_x_coordinate();
static bool_t decode_y_coordinate();
static void send_coordinates_uart();

/* Public functions ---------------------------------------------------------*/
bool_t coordinates_FSM_init() {
	HAL_I2C_StateTypeDef adxl345_i2c_state = adxl345_get_I2C_state();
	HAL_SPI_StateTypeDef max7219_spi_state = max7219_get_SPI_state();
	HAL_UART_StateTypeDef uart_state = uart_get_state();


	if ((adxl345_i2c_state == HAL_I2C_STATE_ERROR) || (adxl345_i2c_state == HAL_I2C_STATE_RESET)) {
		if (!adlx345_I2C_init()) {
			return false;
		}
	}

	if ((max7219_spi_state == HAL_SPI_STATE_ERROR) || (max7219_spi_state == HAL_SPI_STATE_RESET)) {
		if (!max7219_SPI_init()) {
			return false;
		}
	}

	if ((uart_state == HAL_UART_STATE_ERROR) || (uart_state == HAL_UART_STATE_RESET)) {
		if (!uart_init()) {
			return false;
		}
	}

	current_state = CONFIGURE_MODULES;
	return true;
}

void coordinates_FSM_update() {
	switch (current_state) {
		case CONFIGURE_MODULES:
			configure_adxl345();
			configure_max7219();

			current_state = READ_COORDINATES;
			break;

		case READ_COORDINATES:
			coordinates = adxl345_read_coordinates();

			if (coordinates.z_coord > 0) {
				current_state = DECODE_COORDINATES;
			}
			break;

		case DECODE_COORDINATES:
			if(decode_x_coordinate() && decode_y_coordinate()) {
				current_state = DISPLAY_COORDINATES;
			} else {
				current_state = READ_COORDINATES;
			}
			break;

		case DISPLAY_COORDINATES:
			max7219_clean_all_displays();
			max7219_turn_on_LEDs(decoded_x_coordinate, decoded_y_coordinate, current_display);
			send_coordinates_uart();

			if (adxl345_is_data_ready()) {
				current_state = READ_COORDINATES;
			}
			break;

		default:
			// Should never reach here. In that case go to initial state
			current_state = CONFIGURE_MODULES;
			break;
	}
}


/* Private functions ---------------------------------------------------------*/
/**
 * @brief  Configures the ADXL345 accelerometer with predefined settings for bandwidth rate,
 * 		   data format, and power control.
 *
 * @param  None
 * @retval None
*/
static void configure_adxl345() {
	adxl345_set_bandwidth_rate(ADXL345_BW_RATE_VALUE);
	adxl345_set_data_format(ADXL345_DATA_FORMAT_VALUE);
	adxl345_set_power_control(ADXL345_POWER_CTL_VALUE);
}


/**
 * @brief Configures the MAX7219 display driver for all connected displays with predefined
 * 		  settings for shutdown mode, decode mode, intensity, and scan limit.
 *
 * @param None
 * @retval None
*/
static void configure_max7219() {
	max7219_set_displays(TOTAL_DISPLAYS);
	for (int8_t display = 0; display < TOTAL_DISPLAYS; display++) {
		max7219_set_shutdown(SHUTDOWN_MODE_VALUE, display);
		max7219_set_decode_mode(NO_DECODE_MODE_VALUE, display);
		max7219_set_intensity(INTENSITY_VALUE, display);
		max7219_set_scan_limit(SCAN_LIMIT_VALUE, display);
		max7219_set_shutdown(NORMAL_MODE_VALUE, display);
	}
}

/**
 * @brief  Sends the current coordinates via UART in the format: "X:<x_coord_value> - Y:<y_coord_value>\n"
 *
 * @param  None
 * @retval None
 */
static void send_coordinates_uart() {
	uint8_t msg_coord[5];

	uart_send_string(msg_coordinate_x);
	sprintf((char*)msg_coord, "%d", coordinates.x_coord);
	uart_send_string(msg_coord);

	uart_send_string(msg_coordinate_y);
	sprintf((char*)msg_coord, "%d", coordinates.y_coord);
	uart_send_string(msg_coord);

	uart_send_string(msg_new_line);
}

/**
 * @brief  Decodes the X coordinate to fit within the range of display segments.
 *
 * @param  None
 * @retval true if the X coordinate is within the valid range, false otherwise.
 */
static bool_t decode_x_coordinate() {
    if (coordinates.x_coord  > COORD_MIN_VALUE && coordinates.x_coord < COORD_MAX_VALUE) {
    	decoded_x_coordinate = ((coordinates.x_coord  + COORD_MAX_VALUE) / RANGE_X_INTERVAL) + 1;
    	return true;
    }
    return false;
}


/**
 * @brief  Decodes the Y coordinate to determine the appropriate display and position within the display.
 *
 * @retval true if the Y coordinate is within the valid range, false otherwise.
 */
static bool_t decode_y_coordinate() {
	uint8_t max_index = TOTAL_DISPLAYS -1;
	uint8_t position;

    if ((coordinates.y_coord > COORD_MIN_VALUE) && (coordinates.y_coord < COORD_MAX_VALUE)) {
    	/* For each display, there is a specific range of possible coordinate values.
    	 * The range from MIN_COORD_VALUE to MAX_COORD_VALUE is divided by TOTAL_DISPLAYS, assigning specific size range to each display.
    	 * The limits for each range are specified by display_limits.
    	 * Therefore, for each range, the display number must be set, and decoding must be performed, as each range
    	 * is divided into 8 subranges corresponding to specific LEDs out of the 8 possible LEDs.
    	 * Each LED corresponds to one bit in an 8-bit word.
    	 */
    	for (int i = 0; i < TOTAL_DISPLAYS; i++) {
    	    if (coordinates.y_coord < display_limits[i]) {
    	        current_display = max_index - i;
    	        position = (coordinates.y_coord + display_limits[max_index-i]) / RANGE_Y_DIVISON;
    	        decoded_y_coordinate = 1 << (7 - position);
    	        break;
    	    }
    	}
    	return true;
    }
    return false;
}
