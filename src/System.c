/**
 * @file      System.c
 * @brief     STM32F103C8T6 system initialisation
 * @copyright Copyright (c) 2020 STMicroelectronics.
 *            All rights reserved.
 *            This software component is licensed by ST under Ultimate
 *            Liberty license SLA0044, the "License"; You may not use
 *            this file except in compliance with the License.  You may
 *            obtain a copy of the License at:  www.st.com/SLA0044
 */

#include "stm32f1xx_hal.h"
#include "System.h"

static void System_GPIO_Init(void);

/**
 * @brief  Period elapsed callback in non blocking mode
 * @note   This function is called when TIM4 interrupt took place,
 *         inside HAL_TIM_IRQHandler(). It makes a direct call to
 *         HAL_IncTick() to increment a global variable "uwTick" used
 *         as application time base.
 * @param  htim : TIM handle
 * @retval None
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (TIM4 == htim->Instance)
    {
        HAL_IncTick();
    }
}

/**
 * @brief  System Initialisation Function
 * @return System status code
 */
SystemStatus System_Init(void)
{
    SystemStatus             eStatus           = SYSTEM_OK;
    RCC_OscInitTypeDef       RCC_OscInitStruct = { 0 };
    RCC_ClkInitTypeDef       RCC_ClkInitStruct = { 0 };
    RCC_PeriphCLKInitTypeDef PeriphClkInit     = { 0 };

    /* Reset of all peripherals, Initialises the Flash interface and the
     * Systick.
     */
    HAL_Init();

    /** Initialises the CPU, AHB and APB busses clocks
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState       = RCC_HSE_ON;
    RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
    RCC_OscInitStruct.HSIState       = RCC_HSI_ON;
    RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource  = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLMUL     = RCC_PLL_MUL9;

    if (HAL_OK != HAL_RCC_OscConfig(&RCC_OscInitStruct))
    {
        return SYSTEM_GENERAL_ERROR;
    }

    // Initialises the CPU, AHB and APB busses clocks
    RCC_ClkInitStruct.ClockType =
        RCC_CLOCKTYPE_HCLK   |
        RCC_CLOCKTYPE_SYSCLK |
        RCC_CLOCKTYPE_PCLK1  |
        RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_OK != HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2))
    {
        return SYSTEM_GENERAL_ERROR;
    }

    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
    PeriphClkInit.AdcClockSelection    = RCC_ADCPCLK2_DIV6;

    if (HAL_OK != HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit))
    {
        return SYSTEM_GENERAL_ERROR;
    }

    // Initialise peripherals
    System_GPIO_Init();

    return eStatus;
}

/**
 * @brief  GPIO Initialisation Function
 */
static void System_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = { 0 };

    // GPIO Ports Clock Enable
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);

    GPIO_InitStruct.Pin   = LED_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

    HAL_GPIO_Init(LED_GPIO_Port, &GPIO_InitStruct);

    HAL_GPIO_WritePin(Light_GPIO_Port, LightGN_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(Light_GPIO_Port, LightRD_Pin, GPIO_PIN_RESET);

    GPIO_InitStruct.Pin   = LightGN_Pin | LightRD_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

    HAL_GPIO_Init(Light_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin  = SwitchGN_Pin | SwitchRD_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;

    HAL_GPIO_Init(Switch_GPIO_Port, &GPIO_InitStruct);
}
