/*
 * API_delay.c
 *
 *  Created on: Mar 14, 2024
 *      Author: lpmeoli
 */

#include "API_delay.h"

/**
 * @brief  Initializes the delay structure with the given duration.
 * @param  delay: Pointer to the structure representing the delay.
 * 		time_ms: The duration of the delay in milliseconds. It must be greater than 0.
 * @retval None
 */
void delayInit(delay_t * delay, tick_t duration)
{
	if ((delay == NULL) || (duration == 0))
		return;

	delay->duration = duration;
	delay->running = false;
	delay->startTime = 0;
}


/**
 * @brief  Checks the state of the 'running' flag. If true, calculates
 * whether the delay time has elapsed or not. When the delay time elapses,
 * sets the 'running' flag to false.
 * @param  delay: Pointer to the structure representing the delay.
 * @retval Boolean value indicating whether the delay time has elapsed.
 */
bool_t delayRead(delay_t * delay)
{
	uint32_t elapsedTime = 0;
	bool_t returnValue = false;

	if (delay == NULL)
		return returnValue;

	if (delay->running == false)
	{
		delay->startTime = HAL_GetTick();
		delay->running = true;

	}
	else
	{
		elapsedTime = HAL_GetTick() - delay->startTime;
		if (elapsedTime >= delay->duration)
		{
			delay->running = false;
			returnValue = true;
		}
	}

	return returnValue;
}


/**
 * @brief  Allows changing the duration time of an existing delay only if the
 * delay it is not running.
 * @param  delay: Pointer to the structure representing the delay.
 *  		duration: The new duration time for the delay in milliseconds. Must be greater than 0.
 * @retval None
 */
void delayWrite(delay_t * delay, tick_t duration)
{
	if ((delay == NULL) || (duration == 0))
		return;

	if (delay->running == false) {
		delay->duration = duration;
	}
}
