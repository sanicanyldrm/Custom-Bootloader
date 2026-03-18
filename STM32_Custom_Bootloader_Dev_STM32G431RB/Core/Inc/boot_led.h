/******************************************************************************
 * @file    boot_led.h
 * @brief   Bootloader LED control interface
 *
 * @details
 * This header file provides the public interface for controlling the
 * bootloader status LED.
 *
 * It includes function declarations for:
 * - Turning the LED on and off
 * - Toggling the LED state
 * - Processing LED blink patterns for different bootloader states
 *
 * The LED is used as a visual indicator to represent bootloader behavior,
 * such as waiting for user input or staying in bootloader mode.
 *
 * @author  can.yildirim
 * @date    2026-03-18
 *
 * @version 1.0.0
 *
 * @note
 * - Designed for STM32G431RB Nucleo board
 * - LED behavior depends on configuration defined in boot_config.h
 * - Timing is handled via non-blocking tick-based mechanisms
 *
 * @warning
 * - GPIO for LED must be properly initialized before use
 * - Active level configuration must match hardware design
 *
 *****************************************************************************/

#ifndef INC_BOOT_LED_H_
#define INC_BOOT_LED_H_

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

void bl_BootLedOn(void);
void bl_BootLedOff(void);
void bl_BootLedToggle(void);
void bl_BootLedProcessWaitPattern(void);
void bl_BootLedProcessStayPattern(void);

#endif /* INC_BOOT_LED_H_ */
