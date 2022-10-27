/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    rtc.h
  * @brief   This file contains all the function prototypes for
  *          the rtc.c file
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
#ifndef __RTC_H__
#define __RTC_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern RTC_HandleTypeDef hrtc;

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_RTC_Init(void);

/* USER CODE BEGIN Prototypes */
extern RTC_HandleTypeDef * getRtcHandler(void);

extern void Configure_RTC_Clock(void);

/* Set flags */
extern void setRtcInUseFlag(uint8_t rtcInUse);
extern void setRtcIrqFlag(uint8_t rtcIrq);
extern void setRtcInUseForCsFlag(uint8_t rtcInUseForCs);
extern void setNextRtcWakeUpFlag(uint32_t nextRtcWakeUp);
extern void setRtcPrescFlag(uint16_t rtcPresc);


/* Get flags */ 
extern uint32_t getNextRtcWakeUpFlag(void);
extern uint8_t getRtcInUseForCsFlag(void);
extern uint8_t getRtcIrqFlag(void);
extern uint8_t getRtcInUseFlag(void);
extern uint16_t getRtcPrescFlag(void);

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __RTC_H__ */

