/*
 * usart.c
 *
 *  Created on: Mar 17, 2026
 *      Author: can.yildirim
 */
#include "usart.h"

#define BAUD_RATE		115200U

UART_HandleTypeDef huart2;

void USART2_Init(void)
{
	GPIO_InitTypeDef gpio_usart_2 = {0};

	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_USART2_CLK_ENABLE();

	// PA2: USART2_TX
	// PA3: USART2_RX
	gpio_usart_2.Pin = GPIO_PIN_2 | GPIO_PIN_3;
	gpio_usart_2.Mode = GPIO_MODE_AF_PP;
	gpio_usart_2.Pull = GPIO_NOPULL;
	gpio_usart_2.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	gpio_usart_2.Alternate = GPIO_AF7_USART2;

	HAL_GPIO_Init(GPIOA, &gpio_usart_2);	// Initialize the GPIO pin according to the UART config

	huart2.Instance = USART2;
	huart2.Init.BaudRate = BAUD_RATE;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.OverSampling = UART_OVERSAMPLING_16;
	huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	huart2.Init.ClockPrescaler = UART_PRESCALER_DIV1;

	if(HAL_UART_Init(&huart2) != HAL_OK)
	{
		Error_Handler();
	}

	if(HAL_UARTEx_SetTxFifoThreshold(&huart2, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
	{
		Error_Handler();
	}

	if(HAL_UARTEx_SetRxFifoThreshold(&huart2, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
	{
		Error_Handler();
	}

	if(HAL_UARTEx_DisableFifoMode(&huart2) != HAL_OK)
	{
		Error_Handler();
	}

}
