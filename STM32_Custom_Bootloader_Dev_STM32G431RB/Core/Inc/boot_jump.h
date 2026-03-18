/*
 * boot_jump.h
 *
 *  Created on: Mar 17, 2026
 *      Author: can.yildirim
 */

#ifndef INC_BOOT_JUMP_H_
#define INC_BOOT_JUMP_H_

#include "stdbool.h"

#include "stm32g4xx_hal.h"
#include "stm32g4xx.h"

void bj_BootMainDesicion(void);
bool bj_BootIsAppValid(void);
void bj_BootJumpToApp(void);

#endif /* INC_BOOT_JUMP_H_ */
