/*
 * boot_config.h
 *
 *  Created on: Mar 18, 2026
 *      Author: can.yildirim
 */

#ifndef INC_BOOT_CONFIG_H_
#define INC_BOOT_CONFIG_H_

#define APP_ADDRESS				0x08008000U				// Application stack pointer
#define APP_RESET_ADDRESS		(APP_ADDRESS + 4U)		// Application reset handler address

#define WORD_FLASH_EMPTY		(0xFFFFFFFFU)
#define RAM_START_ADDRESS		(0x20000000U)
#define RAM_VALID_MASK			(0x2FFE0000U)
#define FLASH_END_ADDRESS		(0x0801FFFFU)

#define BOOT_WINDOW_MS				(5000U)
#define BOOT_LED_WAIT_PERIOD_MS		(200U)
#define BOOT_LED_STAY_PERIOD_MS		(500U)

#define BOOT_BUTTON_DEBOUNCE_MS       (30U)


#define BOOT_BUTTON_PORT              GPIOC
#define BOOT_BUTTON_PIN               GPIO_PIN_13
#define BOOT_BUTTON_PULL              GPIO_NOPULL

#define BOOT_BUTTON_ACTIVE_LEVEL      (1U)

#define BOOT_LED_PORT              GPIOA
#define BOOT_LED_PIN               GPIO_PIN_5
#define BOOT_LED_ACTIVE_LEVEL      (1U)

#endif /* INC_BOOT_CONFIG_H_ */
