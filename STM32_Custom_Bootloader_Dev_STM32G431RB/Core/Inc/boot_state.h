/*
 * boot_state.h
 *
 *  Created on: Mar 18, 2026
 *      Author: can.yildirim
 */

#ifndef INC_BOOT_STATE_H_
#define INC_BOOT_STATE_H_

typedef enum
{
	BOOT_STATE_STARTUP = 0,
	BOOT_STATE_WAIT_FOR_REQUEST,
	BOOT_STATE_STAY_IN_BL,
	BOOT_STATE_JUMP_TO_APP
}bs_BootState_e;


void bs_BootStateInit(void);
void bs_BootStateMachine(void);

#endif /* INC_BOOT_STATE_H_ */
