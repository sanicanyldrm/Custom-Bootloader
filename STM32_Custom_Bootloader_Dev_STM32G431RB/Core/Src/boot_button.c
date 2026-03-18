/******************************************************************************
 * @file    boot_button.c
 * @brief   Bootloader user button handling implementation
 *
 * @details
 * This source file implements the button handling logic for the bootloader.
 * It reads the hardware pin state and provides a stable button status to
 * the boot state machine.
 *
 * Optional debounce and edge detection mechanisms can be implemented here.
 *
 * @author  can.yildirim
 * @date    2026-03-18
 *
 * @version 1.0.0
 *
 * @note
 * - Designed for STM32G431RB Nucleo board user button
 * - Can be extended for interrupt-based detection
 *
 * @warning
 * - Direct GPIO reads without debounce may cause unstable behavior
 *
 *****************************************************************************/

/******************************************************************************
 * INCLUDES
 *****************************************************************************/
#include <stdint.h>

#include "boot_button.h"
#include "boot_config.h"
#include "std_def.h"

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
static uint8_t ButtonGetPhysicalLevel(void);

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
static uint8_t ButtonGetPhysicalLevel(void)
{
    return bb_BootButtonReadRaw();
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
uint8_t bb_BootButtonReadRaw(void)
{
	GPIO_PinState pin_state = GPIO_PIN_RESET;

	uint8_t button_pressed = 0U;

	pin_state = HAL_GPIO_ReadPin(BOOT_BUTTON_PORT, BOOT_BUTTON_PIN);

#if (BOOT_BUTTON_ACTIVE_LEVEL == 1U)

	if(pin_state == GPIO_PIN_SET)
	{
		button_pressed = 1U;
	}
	else
	{
		button_pressed = 0U;
	}

#else

    if (pin_state == GPIO_PIN_RESET)
    {
        button_pressed = 1U;
    }
    else
    {
        button_pressed = 0U;
    }

#endif

    return button_pressed;
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
uint8_t bb_BootButtonIsPressedStable(void)
{
	static uint32_t pressed_start_tick = 0U;
	static uint8_t 	debounce_in_progress = 0U;

	uint8_t raw_pressed = ButtonGetPhysicalLevel();
	uint8_t stable_pressed = 0U;

	if(raw_pressed != FALSE)
	{

		if(debounce_in_progress == FALSE)
		{

			debounce_in_progress = TRUE;
			pressed_start_tick = HAL_GetTick();

		}
		else
		{

			if((HAL_GetTick() - pressed_start_tick) >= BOOT_BUTTON_DEBOUNCE_MS)
			{
				stable_pressed = TRUE;
			}
			else
			{
				// Debounce time not completed yet
			}

		}


	}
	else
	{

		debounce_in_progress = FALSE;
		pressed_start_tick = 0U;

	}

	return stable_pressed;
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
void bb_BootButtonInit(void)
{
	GPIO_InitTypeDef user_button = {0};

	__HAL_RCC_GPIOC_CLK_ENABLE();

	user_button.Pin = BOOT_BUTTON_PIN;
	user_button.Mode = GPIO_MODE_INPUT;
	user_button.Pull = BOOT_BUTTON_PULL;
	HAL_GPIO_Init(BOOT_BUTTON_PORT, &user_button);
}
