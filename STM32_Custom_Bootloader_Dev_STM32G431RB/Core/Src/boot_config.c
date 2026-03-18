/******************************************************************************
 * @file    boot_config.c
 * @brief   Bootloader configuration definitions
 *
 * @details
 * This file contains all configurable parameters and hardware definitions
 * used by the bootloader.
 *
 * It includes:
 * - Application memory layout definitions (flash addresses, RAM validation)
 * - Bootloader timing parameters (boot window, LED timing, debounce)
 * - Hardware configuration for button and LED (GPIO port, pin, active level)
 *
 * Centralizing configuration in this file improves maintainability and
 * portability across different hardware platforms.
 *
 * @author  can.yildirim
 * @date    2026-03-18
 *
 * @version 1.0.0
 *
 * @note
 * - Application start address must match linker configuration
 * - Timing values are defined in milliseconds
 * - GPIO configuration must be aligned with CubeMX settings
 *
 * @warning
 * - Incorrect APP_ADDRESS may cause invalid jump to application
 * - RAM validation mask must be consistent with MCU memory map
 * - FLASH_END_ADDRESS must reflect the actual device flash size
 *
 *****************************************************************************/

/******************************************************************************
 * INCLUDES
 *****************************************************************************/

#include "boot_config.h"

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
