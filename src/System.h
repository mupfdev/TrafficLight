/**
 * @file      System.h
 * @brief     STM32F103C8T6 system initialisation
 * @copyright Copyright (c) 2020 STMicroelectronics.
 *            All rights reserved.
 *            This software component is licensed by ST under Ultimate
 *            Liberty license SLA0044, the "License"; You may not use
 *            this file except in compliance with the License.  You may
 *            obtain a copy of the License at:  www.st.com/SLA0044
 */
#pragma once

#include "stm32f1xx_hal.h"

#define LED_Pin          GPIO_PIN_13 ///< LED pin
#define LED_GPIO_Port    GPIOC       ///< LED GPIO port
#define LightGN_Pin      GPIO_PIN_8  ///< Green light control pin
#define LightRD_Pin      GPIO_PIN_9  ///< Red light control pin
#define Light_GPIO_Port  GPIOB       ///< Light GPIO port
#define SwitchGN_Pin     GPIO_PIN_12 ///< Green switch pin
#define SwitchRD_Pin     GPIO_PIN_13 ///< Red switch pin
#define Switch_GPIO_Port GPIOB       ///< Switch GPIO port

/**
 * @enum  SystemStatus
 * @brief System status code
 */
typedef enum
{
    SYSTEM_OK = 0,        ///< System OK
    SYSTEM_GENERAL_ERROR, ///< General error
    SYSTEM_GPIO_ERROR,    ///< GPIO error
    SYSTEM_ADC_ERROR,     ///< ADC error
    SYSTEM_CRC_ERROR,     ///< CRC error
    SYSTEM_I2C_ERROR,     ///< I²C error
    SYSTEM_SPI_ERROR      ///< SPI error

} SystemStatus;

SystemStatus System_Init(void);
