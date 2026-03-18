/******************************************************************************
 * @file    boot_button.h
 * @brief   Bootloader user button interface
 *
 * @details
 * This module provides the interface for reading and processing the user
 * button used to control bootloader entry behavior.
 *
 * It abstracts the hardware access and provides debounced button state
 * information to the boot state machine.
 *
 * @author  can.yildirim
 * @date    2026-03-18
 *
 * @version 1.0.0
 *
 * @note
 * - Button is used to request staying in bootloader mode
 * - Hardware abstraction allows portability
 *
 * @warning
 * - Button debounce must be handled properly to avoid false triggers
 *
 *****************************************************************************/

#ifndef INC_BOOT_BUTTON_H_
#define INC_BOOT_BUTTON_H_

/******************************************************************************
 * INCLUDES
 *****************************************************************************/
#include "stm32g4xx_hal.h"

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
uint8_t bb_BootButtonReadRaw(void);
uint8_t bb_BootButtonIsPressedStable(void);
void bb_BootButtonInit(void);

#endif /* INC_BOOT_BUTTON_H_ */
