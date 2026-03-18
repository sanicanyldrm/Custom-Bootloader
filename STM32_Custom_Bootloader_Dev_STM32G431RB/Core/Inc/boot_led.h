/*
 * boot_led.h
 *
 *  Created on: Mar 18, 2026
 *      Author: can.yildirim
 */

#ifndef INC_BOOT_LED_H_
#define INC_BOOT_LED_H_

#include "stm32g4xx_hal.h"

void bl_BootLedOn(void);
void bl_BootLedOff(void);
void bl_BootLedToggle(void);
void bl_BootLedProcessWaitPattern(void);
void bl_BootLedProcessStayPattern(void);

#endif /* INC_BOOT_LED_H_ */
