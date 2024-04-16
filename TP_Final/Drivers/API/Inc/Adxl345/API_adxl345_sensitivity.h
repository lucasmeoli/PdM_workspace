/*
 * API_adxl345_sensitivity.h
 *
 *  Created on: Apr 9, 2024
 *      Author: lpmeoli
 */

#ifndef API_INC_API_ADXL345_SENSITIVITY_H_
#define API_INC_API_ADXL345_SENSITIVITY_H_
/* Function prototypes -----------------------------------------------*/

/*
 * @brief  Initializes the sensitivity Finite State Machine, that modifies ADXL345 sensitivity.
 * 		   It checks the initialization status of UART module.
 *
 * @param  None
 * @retval bool_t - TRUE if UART module is running, false otherwise.
*/
bool_t sensitivity_FSM_init();

/**
 * @brief  Updates the sensitivity state machine based on the number of taps detected.
 *         Adjusts the sensitivity setting of the ADXL345 accelerometer accordingly.
 *         Sends messages via UART to indicate sensitivity changes.
 *
 * @param  taps: The number of taps detected. 2 taps sets lower sensibility. 1 taps set higher sensibility.
 * @retval None
 */
void sensitivity_FSM_update();

#endif /* API_INC_API_ADXL345_SENSITIVITY_H_ */
