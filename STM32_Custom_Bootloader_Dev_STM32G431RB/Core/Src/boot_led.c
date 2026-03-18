/******************************************************************************
 * @file    boot_led.c
 * @brief   Bootloader LED control and pattern processing implementation
 *
 * @details
 * This source file implements the LED control functions used by the
 * bootloader to provide visual status indication.
 *
 * The module supports:
 * - Turning the bootloader status LED on and off
 * - Toggling the LED output state
 * - Processing non-blocking LED blink patterns based on system tick
 *
 * Different blink periods are used to indicate different bootloader states,
 * such as waiting for a user request or staying in bootloader mode.
 *
 * @author  can.yildirim
 * @date    2026-03-18
 *
 * @version 1.0.0
 *
 * @note
 * - Designed for STM32G431RB Nucleo board
 * - LED timing is based on HAL_GetTick()
 * - Blink patterns are implemented in a non-blocking manner
 *
 * @warning
 * - GPIO configuration for the LED pin must be initialized before using
 *   this module
 * - Tick-based timing accuracy depends on the system time base
 *
 *****************************************************************************/

/******************************************************************************
 * INCLUDES
 *****************************************************************************/
#include "boot_led.h"
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
static void BootLedProcessPattern(uint32_t toggle_period_ms);

/******************************************************************************
 * @brief   Processes a non-blocking LED toggle pattern
 *
 * @details
 * This internal function toggles the bootloader status LED periodically
 * based on the given toggle period.
 *
 * It uses the system tick counter to check elapsed time and changes the
 * LED state only when the requested period has expired. This allows LED
 * blinking without blocking the main bootloader execution flow.
 *
 * @param[in]  toggle_period_ms   LED toggle period in milliseconds
 *
 * @return     None
 *
 *****************************************************************************/
static void BootLedProcessPattern(uint32_t toggle_period_ms)
{
	static uint32_t last_toggle_tick = 0U;
	uint32_t current_tick = HAL_GetTick();

	if((current_tick - last_toggle_tick) >= toggle_period_ms)
	{
		last_toggle_tick = current_tick;
		bl_BootLedToggle();
	}
}

/******************************************************************************
 * @brief   Turns the bootloader status LED on
 *
 * @details
 * This function sets the LED output pin to its active state according to
 * the configured active level definition.
 *
 * It abstracts the hardware polarity so the caller does not need to know
 * whether the LED is active-high or active-low.
 *
 * @param[in]  None
 *
 * @return     None
 *
 *****************************************************************************/
void bl_BootLedOn(void)
{

#if (BOOT_LED_ACTIVE_LEVEL == 1)
	HAL_GPIO_WritePin(BOOT_LED_PORT, BOOT_LED_PIN, GPIO_PIN_SET);
#else
	HAL_GPIO_WritePin(BOOT_LED_PORT, BOOT_LED_PIN, GPIO_PIN_RESET);
#endif

}

/******************************************************************************
 * @brief   Turns the bootloader status LED off
 *
 * @details
 * This function sets the LED output pin to its inactive state according to
 * the configured active level definition.
 *
 * It abstracts the hardware polarity so the caller does not need to know
 * whether the LED is active-high or active-low.
 *
 * @param[in]  None
 *
 * @return     None
 *
 *****************************************************************************/
void bl_BootLedOff(void)
{

#if (BOOT_LED_ACTIVE_LEVEL == 1U)
    HAL_GPIO_WritePin(BOOT_LED_PORT, BOOT_LED_PIN, GPIO_PIN_RESET);
#else
    HAL_GPIO_WritePin(BOOT_LED_PORT, BOOT_LED_PIN, GPIO_PIN_SET);
#endif

}

/******************************************************************************
 * @brief   Toggles the bootloader status LED state
 *
 * @details
 * This function inverts the current output state of the configured
 * bootloader LED pin.
 *
 * It is typically used by periodic LED pattern functions to generate
 * visible blink indications for different bootloader states.
 *
 * @param[in]  None
 *
 * @return     None
 *
 *****************************************************************************/
void bl_BootLedToggle(void)
{
	HAL_GPIO_TogglePin(BOOT_LED_PORT, BOOT_LED_PIN);
}

/******************************************************************************
 * @brief   Processes the LED pattern for bootloader wait state
 *
 * @details
 * This function updates the LED blink pattern used while the bootloader is
 * waiting for a user request within the configured boot window.
 *
 * The blink timing is based on BOOT_LED_WAIT_PERIOD_MS and is processed
 * in a non-blocking manner.
 *
 * @param[in]  None
 *
 * @return     None
 *
 *****************************************************************************/
void bl_BootLedProcessWaitPattern(void)
{
	BootLedProcessPattern(BOOT_LED_WAIT_PERIOD_MS);
}

/******************************************************************************
 * @brief   Processes the LED pattern for bootloader stay mode
 *
 * @details
 * This function updates the LED blink pattern used when the system remains
 * in bootloader mode.
 *
 * The blink timing is based on BOOT_LED_STAY_PERIOD_MS and is processed
 * in a non-blocking manner.
 *
 * @param[in]  None
 *
 * @return     None
 *
 *****************************************************************************/
void bl_BootLedProcessStayPattern(void)
{
	BootLedProcessPattern(BOOT_LED_STAY_PERIOD_MS);
}
