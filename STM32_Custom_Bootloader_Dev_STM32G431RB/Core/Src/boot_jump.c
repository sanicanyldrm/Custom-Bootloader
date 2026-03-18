/******************************************************************************
 * @file    boot_jump.c
 * @brief   Bootloader application validation and jump implementation
 *
 * @details
 * This source file implements the logic required to validate the user
 * application image and transfer execution from the bootloader to the
 * application.
 *
 * The module provides:
 * - Application validity checks (stack pointer, reset handler, memory range)
 * - Safe jump mechanism to application space
 * - System deinitialization before handing over control
 *
 * The jump procedure includes:
 * - Disabling interrupts
 * - Resetting SysTick timer
 * - Deinitializing HAL and peripherals
 * - Relocating vector table (VTOR)
 * - Setting Main Stack Pointer (MSP)
 * - Jumping to application reset handler
 *
 * @author  can.yildirim
 * @date    2026-03-17
 *
 * @version 1.0.0
 *
 * @note
 * - Designed for STM32G431RB Nucleo board
 * - Application start address must match linker configuration
 * - Uses function pointer to branch to application reset handler
 *
 * @warning
 * - Jump must only be executed after successful application validation
 * - Incorrect MSP or VTOR configuration may lead to system crash
 * - Interrupts and peripherals must be properly deinitialized before jump
 * - Debug prints (printf) should be disabled in production builds if timing
 *   or performance is critical
 *
 *****************************************************************************/

/******************************************************************************
 * INCLUDES
 *****************************************************************************/

#include <stdio.h>
#include <stdbool.h>

#include "boot_jump.h"
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

static void ResetSysTick(void);

/******************************************************************************
 * @brief   Checks whether the user application image is valid
 *
 * @details
 * This function verifies the integrity and validity of the user application
 * located at the configured application start address.
 *
 * The following checks are performed:
 * - Stack pointer is not empty (not 0xFFFFFFFF)
 * - Stack pointer is within valid RAM address range
 * - Reset handler address is within valid flash memory range
 *
 * These checks ensure that jumping to the application will not result in
 * undefined behavior.
 *
 * @param[in]  None
 *
 * @return     TRUE  - Application is valid
 *             FALSE - Application is invalid
 *
 *****************************************************************************/
bool bj_BootIsAppValid(void)
{
	uint32_t bj_app_stack_pointer = *(volatile uint32_t*)(APP_ADDRESS);
	uint32_t bj_app_reset_handler = *(volatile uint32_t*)(APP_RESET_ADDRESS);

	uint8_t return_code = TRUE;

	if(bj_app_stack_pointer == WORD_FLASH_EMPTY)
	{
		return_code = FALSE;
	}
	else
	{
		// Do nothing
	}

	if((bj_app_stack_pointer & RAM_VALID_MASK) != (RAM_START_ADDRESS))
	{
		return_code = FALSE;
	}
	else
	{
		// Do nothing
	}

	if((bj_app_reset_handler < APP_ADDRESS) || (bj_app_reset_handler > FLASH_END_ADDRESS))
	{
		return_code = FALSE;
	}
	else
	{
		// Do nothing
	}

	return return_code;
}

/******************************************************************************
 * @brief   Transfers execution from bootloader to user application
 *
 * @details
 * This function performs a controlled jump to the user application by
 * reconfiguring the system environment and branching to the application
 * reset handler.
 *
 * The following sequence is executed:
 * - Reads application stack pointer and reset handler address
 * - Disables all interrupts
 * - Resets SysTick timer
 * - Deinitializes HAL and peripherals
 * - Relocates vector table to application base address
 * - Sets Main Stack Pointer (MSP) to application stack
 * - Jumps to application reset handler
 *
 * This function does not return under normal operation.
 *
 * @param[in]  None
 *
 * @return     None
 *
 * * @warning
 * - This function must only be called after successful application validation
 * - Interrupts must remain disabled until application takes control
 *****************************************************************************/
void bj_BootJumpToApp(void)
{
	uint32_t app_stack_pointer = *(volatile uint32_t*)(APP_ADDRESS);
	uint32_t app_reset_handler = *(volatile uint32_t*)(APP_RESET_ADDRESS);

	HAL_Delay(200);

	printf("Jumping to application\r\n");
	printf("APP SP   = 0x%08lX\r\n", *(volatile uint32_t*)APP_ADDRESS);
	printf("APP Reset= 0x%08lX\r\n", *(volatile uint32_t*)(APP_ADDRESS + 4U));

	pFunction jump_to_app = (pFunction)app_reset_handler;

	__disable_irq();

	ResetSysTick();

	HAL_DeInit();

	SCB->VTOR = APP_ADDRESS;
	__set_MSP(app_stack_pointer);

	__enable_irq();

	jump_to_app();

	while (1)
	{
		// Do nothing
	}
}

/******************************************************************************
 * @brief   Resets the SysTick timer configuration
 *
 * @details
 * This function disables and clears the SysTick timer registers to ensure
 * that no residual timing configuration from the bootloader affects the
 * application after the jump.
 *
 * It clears:
 * - SysTick control register
 * - Reload value register
 * - Current value register
 *
 * @param[in]  None
 *
 * @return     None
 *
 *****************************************************************************/
static void ResetSysTick(void)
{
	SysTick->CTRL = 0U;
	SysTick->LOAD = 0U;
	SysTick->VAL = 0U;
}
