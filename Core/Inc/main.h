/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define SPI2_NSS_IMU_Pin GPIO_PIN_13
#define SPI2_NSS_IMU_GPIO_Port GPIOC
#define WIREL_CE_Pin GPIO_PIN_14
#define WIREL_CE_GPIO_Port GPIOC
#define L_LED_Pin GPIO_PIN_15
#define L_LED_GPIO_Port GPIOC
#define AD_BAT_Pin GPIO_PIN_0
#define AD_BAT_GPIO_Port GPIOA
#define AD_XJ1_Pin GPIO_PIN_1
#define AD_XJ1_GPIO_Port GPIOA
#define AD_XJ2_Pin GPIO_PIN_4
#define AD_XJ2_GPIO_Port GPIOA
#define AD_XJ3_Pin GPIO_PIN_5
#define AD_XJ3_GPIO_Port GPIOA
#define AD_XJ4_Pin GPIO_PIN_6
#define AD_XJ4_GPIO_Port GPIOA
#define AD_XJ5_Pin GPIO_PIN_7
#define AD_XJ5_GPIO_Port GPIOA
#define LQ_B_PWM_Pin GPIO_PIN_0
#define LQ_B_PWM_GPIO_Port GPIOB
#define LQ_F_PWM_Pin GPIO_PIN_1
#define LQ_F_PWM_GPIO_Port GPIOB
#define ECHO_Pin GPIO_PIN_10
#define ECHO_GPIO_Port GPIOB
#define IR_PWM_Pin GPIO_PIN_11
#define IR_PWM_GPIO_Port GPIOB
#define SPI2_NSS_WILEL_Pin GPIO_PIN_12
#define SPI2_NSS_WILEL_GPIO_Port GPIOB
#define OLED_W_SCL_Pin GPIO_PIN_8
#define OLED_W_SCL_GPIO_Port GPIOA
#define OLED_W_SDA_Pin GPIO_PIN_9
#define OLED_W_SDA_GPIO_Port GPIOA
#define TRIG_Pin GPIO_PIN_10
#define TRIG_GPIO_Port GPIOA
#define BUZZER_PWM_Pin GPIO_PIN_11
#define BUZZER_PWM_GPIO_Port GPIOA
#define R_LED_Pin GPIO_PIN_12
#define R_LED_GPIO_Port GPIOA
#define WIREL_IRQ_Pin GPIO_PIN_15
#define WIREL_IRQ_GPIO_Port GPIOA
#define KEY_Pin GPIO_PIN_3
#define KEY_GPIO_Port GPIOB
#define RQ_F_PWM_Pin GPIO_PIN_4
#define RQ_F_PWM_GPIO_Port GPIOB
#define RQ_B_PWM_Pin GPIO_PIN_5
#define RQ_B_PWM_GPIO_Port GPIOB
#define LH_F_PWM_Pin GPIO_PIN_6
#define LH_F_PWM_GPIO_Port GPIOB
#define LH_B_PWM_Pin GPIO_PIN_7
#define LH_B_PWM_GPIO_Port GPIOB
#define RH_F_PWM_Pin GPIO_PIN_8
#define RH_F_PWM_GPIO_Port GPIOB
#define RH_B_PWM_Pin GPIO_PIN_9
#define RH_B_PWM_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
