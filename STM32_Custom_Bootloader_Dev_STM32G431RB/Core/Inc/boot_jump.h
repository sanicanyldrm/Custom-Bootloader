/******************************************************************************
 * @file    boot_jump.h
 * @brief   Bootloader application jump interface
 *
 * @details
 * This header file contains the public interface of the boot jump module.
 * It provides function declarations required to evaluate the boot decision,
 * validate the user application image, and perform the jump from bootloader
 * space to the application space.
 *
 * The module is responsible for:
 * - Determining whether the bootloader should remain active or transfer
 *   control to the user application
 * - Validating the application vector table and memory addresses
 * - Jumping safely to the application reset handler
 *
 * @author  can.yildirim
 * @date    2026-03-17
 *
 * @version 1.0.0
 *
 * @note
 * - Designed for STM32G431RB Nucleo board
 * - Application validity check must be completed before jump operation
 * - Function interfaces in this file are used by the bootloader control flow
 *
 * @warning
 * - Jump operation must only be performed if the application image is valid
 * - Invalid stack pointer or reset handler values may cause undefined behavior
 * - Any interrupt or peripheral state affecting the application startup must
 *   be handled before the jump
 *
 *****************************************************************************/

#ifndef INC_BOOT_JUMP_H_
#define INC_BOOT_JUMP_H_

/******************************************************************************
 * INCLUDES
 *****************************************************************************/

#include "stdbool.h"

#include "stm32g4xx_hal.h"
#include "stm32g4xx.h"

/******************************************************************************
 * MACROS
 *****************************************************************************/

/******************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************/

/******************************************************************************
 * GLOBAL VARIABLES
 *****************************************************************************/

/******************************************************************************
 * STATIC VARIABLES
 *****************************************************************************/

/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/

void bj_BootMainDesicion(void);
bool bj_BootIsAppValid(void);
void bj_BootJumpToApp(void);

#endif /* INC_BOOT_JUMP_H_ */
