/******************************************************************************
 * @file    boot_state.h
 * @brief   Bootloader state machine interface definitions
 *
 * @details
 * This header file contains the public definitions of the bootloader state
 * machine module. It provides the state type declarations, boot context
 * structure definitions, and function prototypes required to initialize and
 * execute the bootloader state machine.
 *
 * The boot state machine is responsible for controlling the boot flow,
 * including startup handling, waiting for a bootloader request, staying in
 * bootloader mode, and jumping to the user application.
 *
 * @author  can.yildirim
 * @date    2026-03-18
 *
 * @version 1.0.0
 *
 * @note
 * - Designed for STM32G431RB Nucleo board
 * - Part of the custom bootloader project
 * - Provides the interface of the boot state management module
 *
 * @warning
 * - State definitions and interfaces in this file must remain consistent
 *   with the implementation in boot_state.c
 * - Any change in boot state flow should be reflected in both the state
 *   machine logic and related application jump conditions
 *
 *****************************************************************************/

#ifndef INC_BOOT_STATE_H_
#define INC_BOOT_STATE_H_

/******************************************************************************
 * INCLUDES
 *****************************************************************************/

/******************************************************************************
 * MACROS
 *****************************************************************************/

/******************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************/

typedef enum
{
	BOOT_STATE_STARTUP = 0,
	BOOT_STATE_WAIT_FOR_REQUEST,
	BOOT_STATE_STAY_IN_BL,
	BOOT_STATE_JUMP_TO_APP
}bs_BootState_e;

/******************************************************************************
 * GLOBAL VARIABLES
 *****************************************************************************/

/******************************************************************************
 * STATIC VARIABLES
 *****************************************************************************/

/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/
void bs_BootStateInit(void);
void bs_BootStateMachine(void);

#endif /* INC_BOOT_STATE_H_ */
