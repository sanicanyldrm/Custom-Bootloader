/*
 * boot_jump.c
 *
 *  Created on: Mar 17, 2026
 *      Author: can.yildirim
 */
#include <stdio.h>
#include <stdbool.h>

#include "boot_jump.h"
#include "boot_config.h"
#include "std_def.h"




static void BootStayMode(void);
static void ResetSysTick(void);


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

static void BootStayMode(void)
{
	printf("APP SP   = 0x%08lX\r\n", *(volatile uint32_t*)APP_ADDRESS);
	printf("APP Reset= 0x%08lX\r\n", *(volatile uint32_t*)(APP_ADDRESS + 4U));

	while(TRUE)
	{
		printf("Application is not valid\r\n");
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
		HAL_Delay(100);
	}

}

static void ResetSysTick(void)
{
	SysTick->CTRL = 0U;
	SysTick->LOAD = 0U;
	SysTick->VAL = 0U;
}


void bj_BootMainDesicion(void)
{
	if(bj_BootIsAppValid() == TRUE)
	{
		printf("Preparing jump to application\r\n");
		bj_BootJumpToApp();
	}
	else
	{
		BootStayMode();
	}
}
