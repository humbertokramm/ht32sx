/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    tim.h
  * @brief   This file contains all the function prototypes for
  *          the tim.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TIM_H__
#define __TIM_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#include "gpio.h"
#include "rtc.h"
#include "usart.h" //TIRAR7
#include "st_rf_api.h"
/* USER CODE END Includes */

extern TIM_HandleTypeDef htim2;

/* USER CODE BEGIN Private defines */

/**
 * @brief  Enables or Disables a specific Timer with its IRQ.
 * @param  TIMER: timer to be set.
 *          This parameter can be a pointer to @ref TIM_TypeDef
 * @param  NEWSTATE: specifies if a timer has to be enabled or disabled.
 *          This parameter is a float.
 * @retval None
 */
#define SdkEvalTimersState(TIM_HANDLER_P , NEWSTATE)      {((uint8_t)NEWSTATE)?(HAL_TIM_Base_Start_IT(TIM_HANDLER_P)):(HAL_TIM_Base_Stop_IT(TIM_HANDLER_P));}


/* USER CODE END Private defines */

void MX_TIM2_Init(void);

/* USER CODE BEGIN Prototypes */
extern void SdkDelayMs(volatile uint32_t lTimeMs);
extern void SdkEvalTimersTimConfig(TIM_HandleTypeDef* TIM_TimeBaseStructure, uint16_t nPrescaler, uint16_t nPeriod);
extern void SdkEvalTimersFindFactors(uint32_t lCycles, uint16_t *pnPrescaler, uint16_t *pnCounter);

/* Set flags */
extern void setNotifyEndFlag(uint8_t notifyEnd);
extern void setNIntermediateTimIrqFlag(uint8_t nIntermediateTimeIrq);

/* Get flags */
extern uint8_t getNotifyEndFlag(void);
extern uint32_t getNIntermediateTimIrqFlag(void);

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __TIM_H__ */

