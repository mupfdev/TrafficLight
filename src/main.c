// SPDX-License-Identifier: Beerware
/**
 * @file      main.c
 * @brief     Bobby car raceway traffic light
 * @author    Michael Fitzmayer
 * @copyright "THE BEER-WARE LICENCE" (Revision 42)
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "FreeRTOS.h"
#include "System.h"
#include "cmsis_os.h"
#include "stm32f1xx_hal.h"
#include "task.h"

#define GREEN_PHASE_IN_MS 120000 ///< Green light phase duration in ms
#define RED_PHASE_IN_MS    20000 ///< Red light phase duration in ms

static osThreadId hMainThread;       ///< Main thread
static osThreadId hSwitchThread;     ///< Switch thread
static bool       bAutoMode = true;  ///< Automatic mode state

static void MainThread(void const* pArg);
static void SwitchThread(void const* pArg);

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
    if (SYSTEM_OK != System_Init())
    {
        return EXIT_FAILURE;
    }

    osThreadDef(hMainThread, MainThread, osPriorityNormal, 0, configMINIMAL_STACK_SIZE);
    hMainThread = osThreadCreate(osThread(hMainThread), NULL);

    osThreadDef(hSwitchThread, SwitchThread, osPriorityNormal, 0, configMINIMAL_STACK_SIZE);
    hSwitchThread = osThreadCreate(osThread(hSwitchThread), NULL);

    osKernelStart();
    while(1);

    return EXIT_SUCCESS;
}

/**
 * @brief Main thread handler
 * @param pArg: Unused
 */
static void MainThread(void const* pArg)
{
    while(1)
    {
        if (bAutoMode)
        {
            HAL_GPIO_WritePin(Light_GPIO_Port, LightGN_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(Light_GPIO_Port, LightRD_Pin, GPIO_PIN_RESET);
            osDelay(GREEN_PHASE_IN_MS);
        }
        else
        {
            osDelay(5);
        }

        if (bAutoMode)
        {
            HAL_GPIO_WritePin(Light_GPIO_Port, LightRD_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(Light_GPIO_Port, LightGN_Pin, GPIO_PIN_RESET);
            osDelay(RED_PHASE_IN_MS);
        }
        else
        {
            osDelay(5);
        }
    };
}

/**
 * @brief Switch thread handler
 * @param pArg: Unused
 */
static void SwitchThread(void const* pArg)
{
    bool bSwitchGN = false;
    bool bSwitchRD = false;

    while(1)
    {
        if (HAL_GPIO_ReadPin(Switch_GPIO_Port, SwitchGN_Pin))
        {
            bSwitchGN = true;
        }
        else
        {
            bSwitchGN = false;
        }

        if (HAL_GPIO_ReadPin(Switch_GPIO_Port, SwitchRD_Pin))
        {
            bSwitchRD = true;
        }
        else
        {
            bSwitchRD = false;
        }

        if (bSwitchGN || bSwitchRD)
        {
            bAutoMode = false;

            if (bSwitchGN)
            {
                HAL_GPIO_WritePin(Light_GPIO_Port, LightGN_Pin, GPIO_PIN_SET);
                HAL_GPIO_WritePin(Light_GPIO_Port, LightRD_Pin, GPIO_PIN_RESET);
            }

            if (bSwitchRD)
            {
                HAL_GPIO_WritePin(Light_GPIO_Port, LightRD_Pin, GPIO_PIN_SET);
                HAL_GPIO_WritePin(Light_GPIO_Port, LightGN_Pin, GPIO_PIN_RESET);
            }
        }
        else
        {
            bAutoMode = true;
        }

        osDelay(10);
    }
}
