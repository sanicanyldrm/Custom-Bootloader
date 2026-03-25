/******************************************************************************
 * @file    boot_state.c
 * @brief   Bootloader state machine management functions
 *
 * @details
 * This source file contains the main boot state handling logic of the
 * custom bootloader. It is responsible for initializing the boot state,
 * processing bootloader timeout behavior, monitoring user button requests,
 * and deciding whether the system should remain in bootloader mode or jump
 * to the user application.
 *
 * The state flow is designed for STM32G431RB based projects and supports
 * a timed boot window implementation without using blocking delay loops.
 *
 * @author  can.yildirim
 * @date    2026-03-18
 *
 * @version 1.0.0
 *
 * @note
 * - Designed for STM32G431RB Nucleo board
 * - Used as part of the custom bootloader project
 * - Bootloader entry decision is based on button status and timeout handling
 *
 * @warning
 * - Jump to application must only be performed after validating the
 *   application start address and vector table
 * - State transitions must remain deterministic to avoid unintended
 *   bootloader lock conditions
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
#include "boot_image.h"

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
 * @brief    Bootloader startup state performs application validity check.
 *
 * @details
 * Check the application validity and according to the result sets bootloader
 * state. If application is valid, gets current tick to wait 5 second button
 * response. Sets bootloader state to wait for request state. Resets the button
 * latched status.
 *
 * @param[in] param Description None
 *
 * @return None
 *
 *****************************************************************************/
static void BootHandleStartupState(void)
{
	printf("Bootloader started\r\n");

	BootContext.wait_start_tick = HAL_GetTick();
	BootContext.button_latched = 0U;
	BootContext.state = BOOT_STATE_WAIT_FOR_REQUEST;
}

/******************************************************************************
 * @brief    Bootloader wait for request state waits from user input.
 *
 * @details
 * Inform the state status from the on board LED. Checks the button is whether
 * pressed. If button is pressed, set state to stay in bootloader state. If time
 * is expired(5 second is elapsed), sets the bootloader state to jump to app state.
 *
 * @param[in] param Description None
 *
 * @return None
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
 * @brief    Bootloader stay in bootloader state waits in bootloader
 *
 * @details
 * This state informs the state status over on board LED. Toggles the LED
 * periodically.
 *
 * @param[in] param Description None
 *
 * @return None
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
 * @brief    Bootloader jump to application state jumps to main appliation
 *
 * @details
 * This state is perform jumping operation and reset the LED. If error occurs
 * while jumping to application, sets the bootloader state to stay in bootloader
 * state.
 *
 * @param[in] param Description None
 *
 * @return None
 *
 *****************************************************************************/
static void BootHandleJumpToAppState(void)
{

	if(bj_BootIsAppValid() == FALSE)
		{
			printf("Application is not valid. Staying in bootloader.\r\n");
			BootContext.state = BOOT_STATE_STAY_IN_BL;
		}
		else
		{
			if(bi_IsMetaDataValid() == TRUE)
			{
				printf("Boot: Application valid\r\n");
				bl_BootLedOff();
				bj_BootJumpToApp();

				//Should never return here
				printf("Boot: ERROR - returned from the application jump\r\n");
				BootContext.state = BOOT_STATE_STAY_IN_BL;
			}
			else
			{
				printf("Boot: Application is not valid. Staying in bootloader. \r\n");
				BootContext.state = BOOT_STATE_STAY_IN_BL;
			}

		}

}

/******************************************************************************
 * @brief    Initializes the bootloader state machine context
 *
 * @details
 * This function initializes the bootloader state machine by setting the
 * initial state and resetting all related context variables.
 *
 * - Sets the initial state to BOOT_STATE_STARTUP
 * - Clears the bootloader timeout reference (wait_start_tick)
 * - Resets button latch status
 *
 * @param[in] param Description None
 *
 * @return None
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
 * @brief    Executes the bootloader state machine
 *
 * @details
 * This function is the main dispatcher of the bootloader state machine.
 * It evaluates the current boot state stored in the BootContext structure
 * and calls the corresponding state handler function.
 *
 * The state machine controls the overall bootloader behavior including:
 * - Startup initialization sequence
 * - Waiting for user input (button press) within a defined timeout
 * - Staying in bootloader mode if requested
 * - Jumping to the user application if no request is received
 *
 * Each state is handled by a dedicated function to ensure modularity and
 * maintainability of the bootloader design.
 *
 * In case of an invalid or undefined state, the system falls back to
 * BOOT_STATE_STAY_IN_BL as a safe state to prevent unintended jumps.
 *
 * This function is expected to be called periodically from the main loop.
 *
 * @param[in] param Description None
 *
 * @return None
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


