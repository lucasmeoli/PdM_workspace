/*
 * API_max7219_led_display.h
 *
 *  Created on: Apr 1, 2024
 *      Author: lpmeoli
 */

#ifndef API_INC_API_MAX7219_LED_DISPLAY_H_
#define API_INC_API_MAX7219_LED_DISPLAY_H_

/* Exported types ------------------------------------------------------------*/
typedef bool bool_t;

/* Exported functions ------------------------------------------------------- */
/**
  * @brief	Initializes the SPI communication interface for MAX7219.
  *
  *         Configures SPI1 as a master device with the following parameters:
  *         - Mode: Master mode.
  *         - Data size: 16 bits.
  *         - Clock polarity: Low.
  *         - Clock phase: First edge.
  *         - Baud rate prescaler: 128.
  *         - Bit order: MSB first.
  *
  *         If SPI1 is in RESET state, it configures the peripheral with the specified parameters.
  *         After initialization, if successful, it releases the Chip Select pin.
  *
  * @param  None
  * @retval bool_t - TRUE if initialization is successful or if it has already been initialized
  * 		successfully; FALSE otherwise.
  */
bool_t max7219_SPI_init();

/**
  * @brief	Turn off all the LEDs of the all the active displays.
  *
  * @param  None
  * @retval None
  */
void max7219_clean_all_displays();

/**
  * @brief	Turn off all the LEDs of the specified display.
  *
  * @param  display_num: The display number to clean (0-indexed).
  *         This should be a value between 0 and the total number of active displays - 1.
  * @retval None
  */
void max7219_clean_display(uint8_t display_num);


/**
  * @brief Turn on specified LEDs of a specific display when display is in decode mode
  *
  * @param reg: Digit register to write. It must be between 0x01 and 0x08
  *        data: The data to be written to the intensity register.
  * 	   display_num: The display number to apply the settings to.
  * @retval None
  */
void max7219_turn_on_LEDs(uint8_t reg, uint8_t data, uint8_t display_num);


/* ---------- Setters ---------- */
/**
  * @brief Sets the number of active displays for MAX7219.
  *
  * @param total_displays: The total number of displays to activate.
  *                        If total_displays is greater than the max displays(4) supported,
  *                        it sets the number of active displays to 4.
  * @retval None
  */
void max7219_set_displays(uint8_t total_displays);

/**
  * @brief  Sets the decode mode register of the specified display.
  *
  * @param  data: The data to be written to the decode mode register.
  * 		display_num: The display number to apply the settings to.
  * @retval None
  */
void max7219_set_decode_mode(uint8_t data, uint8_t display_num);

/**
  * @brief  Sets the intensity register of the specified display.
  *
  * @param  data: The data to be written to the intensity register.
  * 		display_num: The display number to apply the settings to.
  * @retval None
  */
void max7219_set_intensity(uint8_t data, uint8_t display_num);

/**
  * @brief  Sets the scan limit register of the specified display.
  *
  * @param  data: The data to be written to the scan limit register.
  * 		display_num: The display number to apply the settings to.
  * @retval None
  */
void max7219_set_scan_limit(uint8_t data, uint8_t display_num);

/**
  * @brief  Sets the shutdown mode register of the specified display.
  *
  * @param  data: The data to be written to the shutdown register.
  * 		display_num: The display number to apply the settings to.
  * @retval None
  */
void max7219_set_shutdown(uint8_t data, uint8_t display_num);


/* ---------- Getters ---------- */
/**
  * @brief	Get the current state of the SPI communication interface for MAX7219.
  *
  * @param  None
  * @retval HAL_SPI_StateTypeDef - Current state of the SPI communication.
  */
HAL_SPI_StateTypeDef max7219_get_SPI_state();

#endif /* API_INC_API_MAX7219_LED_DISPLAY_H_ */
