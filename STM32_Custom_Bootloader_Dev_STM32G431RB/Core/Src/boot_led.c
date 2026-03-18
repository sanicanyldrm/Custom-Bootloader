/******************************************************************************
 * @file    boot_state.c
 * @brief   TODO: Fill
 *
 * @details
 * TODO: Fill
 *
 * @author  can.yildirim
 * @date    2026-03-18
 *
 * @version 1.0.0
 *
 * @note
 * - TODO:
 *
 * @warning
 * - TODO:
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
 * @brief    TODO:
 *
 * @details
 * TODO:
 *
 * @param[in] param Description TODO:
 *
 * @return TODO:
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
 * @brief    TODO:
 *
 * @details
 * TODO:
 *
 * @param[in] param Description TODO:
 *
 * @return TODO:
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
 * @brief    TODO:
 *
 * @details
 * TODO:
 *
 * @param[in] param Description TODO:
 *
 * @return TODO:
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
 * @brief    TODO:
 *
 * @details
 * TODO:
 *
 * @param[in] param Description TODO:
 *
 * @return TODO:
 *
 *****************************************************************************/
void bl_BootLedToggle(void)
{
	HAL_GPIO_TogglePin(BOOT_LED_PORT, BOOT_LED_PIN);
}

/******************************************************************************
 * @brief    TODO:
 *
 * @details
 * TODO:
 *
 * @param[in] param Description TODO:
 *
 * @return TODO:
 *
 *****************************************************************************/
void bl_BootLedProcessWaitPattern(void)
{
	BootLedProcessPattern(BOOT_LED_WAIT_PERIOD_MS);
}

/******************************************************************************
 * @brief    TODO:
 *
 * @details
 * TODO:
 *
 * @param[in] param Description TODO:
 *
 * @return TODO:
 *
 *****************************************************************************/
void bl_BootLedProcessStayPattern(void)
{
	BootLedProcessPattern(BOOT_LED_STAY_PERIOD_MS);
}
