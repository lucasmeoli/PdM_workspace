/*
 * API_debounce.h
 *
 *  Created on: Mar 22, 2024
 *      Author: lpmeoli
 */

#ifndef API_INC_API_DEBOUNCE_H_
#define API_INC_API_DEBOUNCE_H_

/* Function prototypes -----------------------------------------------*/
void debounceFSM_init(uint32_t debounceTime);
void debounceFSM_update();
bool_t readKey();
#endif /* API_INC_API_DEBOUNCE_H_ */
