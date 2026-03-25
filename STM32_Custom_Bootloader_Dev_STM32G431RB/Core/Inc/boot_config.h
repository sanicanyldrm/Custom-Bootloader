/******************************************************************************
 * @file    boot_config.h
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

#ifndef INC_BOOT_CONFIG_H_
#define INC_BOOT_CONFIG_H_

/******************************************************************************
 * INCLUDES
 *****************************************************************************/

/******************************************************************************
 * MACROS
 *****************************************************************************/

#define APP_ADDRESS				0x08008000U				// Application base address
#define APP_RESET_ADDRESS		(APP_ADDRESS + 4U)		// Application reset handler address

#define APP_PARTITION_SIZE_BYTES   	(96UL * 1024UL)		// Total application size
#define APP_METADATA_SIZE_BYTES		(256UL)				// Total metadata size
#define APP_MAX_SIZE_BYTES			(APP_PARTITION_SIZE_BYTES - APP_METADATA_SIZE_BYTES)

#define APP_METADATA_ADDRESS	(0x801FF00U)

#define WORD_FLASH_EMPTY		(0xFFFFFFFFU)
#define RAM_START_ADDRESS		(0x20000000U)
#define RAM_VALID_MASK			(0x2FFE0000U)
#define FLASH_END_ADDRESS		(0x0801FFFFU)

#define BOOT_WINDOW_MS				(5000U)
#define BOOT_LED_WAIT_PERIOD_MS		(200U)
#define BOOT_LED_STAY_PERIOD_MS		(500U)

#define BOOT_BUTTON_DEBOUNCE_MS      (30U)


#define BOOT_BUTTON_PORT              GPIOC
#define BOOT_BUTTON_PIN               GPIO_PIN_13
#define BOOT_BUTTON_PULL              GPIO_NOPULL

#define BOOT_BUTTON_ACTIVE_LEVEL      (1U)

#define BOOT_LED_PORT              	  GPIOA
#define BOOT_LED_PIN                  GPIO_PIN_5
#define BOOT_LED_ACTIVE_LEVEL      	  (1U)

#define BOOT_METADATA_MAGIC			  (0x424F4F54UL)
#define BOOT_METADATA_FORMAT_VERSION  (1U)



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

#endif /* INC_BOOT_CONFIG_H_ */
