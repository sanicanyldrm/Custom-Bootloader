/*
 * retarget.c
 *
 *  Created on: Mar 17, 2026
 *      Author: can.yildirim
 */

#include <stdio.h>

#include "stm32g4xx.h"
#include "stm32g4xx_hal.h"

extern UART_HandleTypeDef huart2;

int __io_putchar(int ch)
{
	uint8_t c = (uint8_t)ch;

	HAL_UART_Transmit(&huart2, &c, 1, HAL_MAX_DELAY);
	return ch;
}
