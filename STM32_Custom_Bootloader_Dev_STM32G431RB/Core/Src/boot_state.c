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
#include <stdio.h>

#include "boot_state.h"
#include "boot_jump.h"
#include "boot_config.h"
#include "std_def.h"
#include "boot_button.h"
#include "boot_led.h"

/******************************************************************************
 * MACROS
 *****************************************************************************/

/******************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************/

typedef struct
{
	bs_BootState_e state;
	uint32_t wait_start_tick;
	uint8_t button_latched;
} BootContext_t;

/******************************************************************************
 * GLOBAL VARIABLES
 *****************************************************************************/

/******************************************************************************
 * STATIC VARIABLES
 *****************************************************************************/

static BootContext_t BootContext;

/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/

static void BootHandleStartupState(void);
static void BootHandleWaitForRequestState(void);
static void BootHandleStayInBootloaderState(void);
static void BootHandleJumpToAppState(void);

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
static void BootHandleStartupState(void)
{
	printf("Bootloader started\r\n");

	if(bj_BootIsAppValid() == FALSE)
	{
		printf("Application is not valid. Staying in bootloader.\r\n");
		BootContext.state = BOOT_STATE_STAY_IN_BL;
	}
	else
	{
		printf("Boot: Application valid\r\n");
		printf("Boot: Waiting %lu ms for button request\r\n", (unsigned long)BOOT_WINDOW_MS);

		BootContext.wait_start_tick = HAL_GetTick();
		BootContext.button_latched = 0U;
		BootContext.state = BOOT_STATE_WAIT_FOR_REQUEST;
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
static void BootHandleWaitForRequestState(void)
{
	bl_BootLedProcessWaitPattern();

	if(bb_BootButtonIsPressedStable() != FALSE)
	{
		BootContext.button_latched = 1U;
	}

	if(BootContext.button_latched != FALSE)
	{
		printf("Boot: Button pressed, staying in bootloader\r\n");
		BootContext.state = BOOT_STATE_STAY_IN_BL;
	}
	else if((HAL_GetTick() - BootContext.wait_start_tick) >= BOOT_WINDOW_MS)
	{
		printf("Boot: Timeout expired, jumping to application\r\n");
		BootContext.state = BOOT_STATE_JUMP_TO_APP;
	}
	else
	{
		// Waiting window is still active
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
static void BootHandleStayInBootloaderState(void)
{
	bl_BootLedProcessStayPattern();

	/* Future phases:
	   - UART update protocol
	   - software request handling
	   - image download
	*/
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
static void BootHandleJumpToAppState(void)
{
	bl_BootLedOff();
	bj_BootJumpToApp();

	//Should never return here
	printf("Boot: ERROR - returned from the application jump\r\n");
	BootContext.state = BOOT_STATE_STAY_IN_BL;
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
void bs_BootStateInit(void)
{
	// Initialize the boot context
	BootContext.state = BOOT_STATE_STARTUP;
	BootContext.wait_start_tick = 0U;
	BootContext.button_latched = 0U;

	// Debug log
	printf("Boot: State machine is initialized\r\n");
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
void bs_BootStateMachine(void)
{

	switch(BootContext.state)
	{

		case BOOT_STATE_STARTUP:
			BootHandleStartupState();
		break;

		case BOOT_STATE_WAIT_FOR_REQUEST:
			BootHandleWaitForRequestState();
		break;

		case BOOT_STATE_STAY_IN_BL:
			BootHandleStayInBootloaderState();
		break;

		case BOOT_STATE_JUMP_TO_APP:
			BootHandleJumpToAppState();
		break;

		default:
			printf("Boot: Invalid State, forcing bootloader mode\r\n");
			BootContext.state = BOOT_STATE_STAY_IN_BL;
		break;

	}

}


