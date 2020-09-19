/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2020 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H__
#define __MAIN_H__

/* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define led_Pin GPIO_PIN_13
#define led_GPIO_Port GPIOC
#define LOW_SENSOR_Pin GPIO_PIN_0
#define LOW_SENSOR_GPIO_Port GPIOA
#define LOW_SENSOR_EXTI_IRQn EXTI0_IRQn
#define HIGH_SENSOR_Pin GPIO_PIN_1
#define HIGH_SENSOR_GPIO_Port GPIOA
#define HIGH_SENSOR_EXTI_IRQn EXTI1_IRQn
#define BUBBLE_SENSOR_Pin GPIO_PIN_2
#define BUBBLE_SENSOR_GPIO_Port GPIOA
#define BUBBLE_SENSOR_EXTI_IRQn EXTI2_IRQn
#define SPI1_CS_Pin GPIO_PIN_4
#define SPI1_CS_GPIO_Port GPIOA
#define Pump1_Pin GPIO_PIN_0
#define Pump1_GPIO_Port GPIOB
#define Pump2_Pin GPIO_PIN_1
#define Pump2_GPIO_Port GPIOB
#define FLOW_Pin GPIO_PIN_15
#define FLOW_GPIO_Port GPIOA
#define Root_Pin GPIO_PIN_3
#define Root_GPIO_Port GPIOB
#define Vac_Pin GPIO_PIN_4
#define Vac_GPIO_Port GPIOB
#define Cooler_Pin GPIO_PIN_5
#define Cooler_GPIO_Port GPIOB
#define VE_Pin GPIO_PIN_6
#define VE_GPIO_Port GPIOB
#define Heater1_Pin GPIO_PIN_7
#define Heater1_GPIO_Port GPIOB
#define Heater2_Pin GPIO_PIN_8
#define Heater2_GPIO_Port GPIOB

/* ########################## Assert Selection ############################## */
/**
  * @brief Uncomment the line below to expanse the "assert_param" macro in the 
  *        HAL drivers code
  */
/* #define USE_FULL_ASSERT    1U */

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
 extern "C" {
#endif
void _Error_Handler(char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)
#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/