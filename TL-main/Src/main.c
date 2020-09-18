
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f1xx_hal.h"

/* USER CODE BEGIN Includes */
//#include "tm_stm32_hd44780.h"
#include "rc522.h"
#include "stdio.h"
#include "stdlib.h"
#include <cstring>
#include <stdbool.h>
#include "math.h"
//#include <Sim80x.h>
#include <string.h>
//include <stdio.h>
//#define relay_off HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_RESET)
// Define input and ouput
//Input
#define LS_State HAL_GPIO_ReadPin(LOW_SENSOR_GPIO_Port,LOW_SENSOR_Pin)
#define HS_State HAL_GPIO_ReadPin(HIGH_SENSOR_GPIO_Port,HIGH_SENSOR_Pin)
#define BS_State HAL_GPIO_ReadPin(BUBBLE_SENSOR_GPIO_Port,BUBBLE_SENSOR_Pin)
//Output
#define Pump1_on HAL_GPIO_WritePin(Pump1_GPIO_Port,Pump1_Pin,GPIO_PIN_SET);
#define Pump1_off HAL_GPIO_WritePin(Pump1_GPIO_Port,Pump1_Pin,GPIO_PIN_RESET);
#define Pump2_on HAL_GPIO_WritePin(Pump2_GPIO_Port,Pump2_Pin,GPIO_PIN_SET);
#define Pump2_off HAL_GPIO_WritePin(Pump2_GPIO_Port,Pump2_Pin,GPIO_PIN_RESET);
#define Root_on HAL_GPIO_WritePin(Root_GPIO_Port,Root_Pin,GPIO_PIN_SET);
#define Root_off HAL_GPIO_WritePin(Root_GPIO_Port,Root_Pin,GPIO_PIN_RESET);
#define Vac_on HAL_GPIO_WritePin(Vac_GPIO_Port,Vac_Pin,GPIO_PIN_SET);
#define Vac_off HAL_GPIO_WritePin(Vac_GPIO_Port,Vac_Pin,GPIO_PIN_RESET);
#define Cooler_on HAL_GPIO_WritePin(Cooler_GPIO_Port,Cooler_Pin,GPIO_PIN_SET);
#define Cooler_off HAL_GPIO_WritePin(Cooler_GPIO_Port,Cooler_Pin,GPIO_PIN_RESET);
#define VE_on HAL_GPIO_WritePin(VE_GPIO_Port,VE_Pin,GPIO_PIN_SET);
#define VE_off HAL_GPIO_WritePin(VE_GPIO_Port,VE_Pin,GPIO_PIN_RESET);
#define Heater1_on HAL_GPIO_WritePin(Heater1_GPIO_Port,Heater1_Pin,GPIO_PIN_SET);
#define Heater1_off HAL_GPIO_WritePin(Heater1_GPIO_Port,Heater1_Pin,GPIO_PIN_RESET);
#define Heater2_on HAL_GPIO_WritePin(Heater2_GPIO_Port,Heater2_Pin,GPIO_PIN_SET);
#define Heater2_off HAL_GPIO_WritePin(Heater2_GPIO_Port,Heater2_Pin,GPIO_PIN_RESET);
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim4;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
bool Error_Sensor=false;
uint8_t stage=0,mode=1;
uint16_t flow =22;
char bfr_wifi1[200],bfr_wifi2[200],bfr_rs485[200];
uint8_t i=0,check=0;
uint8_t id=1;
uint16_t tik = 0;
uint8_t count_Byte;
char current_Address;																	// Dia chi mod bus
char Rx_data;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_TIM4_Init(void);
static void MX_TIM2_Init(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* END CRC Modbus Function */

/* BEGIN Read Holding Register Function */

/* END Read Holding Register Function */

/* BEGIN TransferRTU Enable*/
void TRANSFER_EN(void)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);
}
/* END TransferRTU Enable */

/* BEGIN TransferRTU Enable */
void RECEIVER_EN(void)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);
}
/* END TransferRTU Enable */

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	if(GPIO_Pin == LOW_SENSOR_Pin){
		// Xuong duoi muc thap
		// Duoi muc thap 0 0 1 0 
		if(LS_State&&HS_State){
//			Pump1_on;
//			Pump2_off;
			stage=0;
			Error_Sensor=false; //RESET LOI
		}
		if(LS_State&&!HS_State){
			Error_Sensor=true; //LOI
		}
		//Vuot len muc thap
		//Nam giua muc thap va muc cao(TH on dinh) 1 0 1 1 
		if((!LS_State) &&(HS_State)){
//			Pump1_on;
//			Pump2_on;
			stage = 1;
			Error_Sensor=false; //RESET LOI
		}
		// Vuot qua ca muc thap va muc cao (de phong loi cam bien muc cao)
		// 2 truong hop 1100 va 01xx deu phai bao loi
		if(!LS_State&&!HS_State){
			Error_Sensor=true;;// LOI
		}

	}
	if(GPIO_Pin == HIGH_SENSOR_Pin){
		// Vuot qua muc cao 1101
		if(!HS_State&&!LS_State){
			stage=2;
//			Pump1_off;
//			Pump2_on;
			Error_Sensor=false; //RESET LOI
		}
		if(!HS_State&&LS_State){
			Error_Sensor=true;//LOI
		}
		// Xuong duoi muc cao
		//Giam xuong nam giua muc thap va muc cao (TH on dinh) 1 0 1 1
		if(HS_State&&!LS_State){
//			Pump2_on;
//			Pump1_on;
			stage=1;
			Error_Sensor=false; //RESET LOI
		}
		// 01xx va 0010 la 2 th bao loi
		if(HS_State&&LS_State){
		Error_Sensor=true;// LOI
		}
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim4)
{
	if(htim4 -> Instance == TIM4)
	{
		tik= tik+1;
	}
}

void sendata_wifi(){
//			sprintf(bfr_wifi2,"{\"Id\":\"%d\",\"U1\":\"%0.3f\",\"U2\":\"%0.3f\",\"I1\":\"%0.3f\",\"I2\":\"%0.3f\",\"cosP1\":\"%0.3f\",\"cosP2\":\"%0.3f\"}\n",id,Vhd1,Vhd2,Ihd1,Ihd2,cosP1,cosP2);
//			HAL_UART_Transmit_IT(&huart1,(uint8_t*)bfr_wifi2,strlen(bfr_wifi2));
//			HAL_Delay(100);
//			sprintf(bfr_wifi1,"{\"csS\":\"%0.3f\",\"csP\":\"%0.3f\",\"csQ\":\"%0.3f\",\"cosP\":\"%0.3f\",\"freq\":\"%0.3f\",\"P_peak\":\"%0.3f\",\"Q_peak\":\"%0.3f\"}\n",csS,csP,csQ,cosP,freq,P_peak,Q_peak);
//			HAL_UART_Transmit_IT(&huart1,(uint8_t*)bfr_wifi1,strlen(bfr_wifi1));
//			HAL_Delay(100);

}

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_USART1_UART_Init();
  MX_TIM4_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
	HAL_TIM_Base_Start_IT(&htim4);
	//HAL_TIM_Base_Start_IT(&htim2);


	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		//mode=1 => Auto Mode
		HAL_GPIO_TogglePin(led_GPIO_Port,led_Pin);
		if(mode==1){
			if(stage==0){ // Trang thai muc nuoc duoi muc thap
				// Neu ca 2 cam bien chua tac dong -> duoi muc thap -> thi moi cho bom
				Cooler_on; //Bat che do lam lanh
				Pump1_on; //Chay bom dau vao
				Pump2_off;//ngat bom dau ra
				Root_on; //Chay root pump
				Vac_on; // Chay Vac pump
				VE_off; //Khoa van VE
				if(tik%5==0){
					check=5;
						}
		//HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET);
				}
			if(stage==1){ //Trang thai on dinh
				Cooler_on; //Bat che do lam lanh
				Pump1_on; //Chay bom dau vao
				Pump2_on;//ngat bom dau ra
				Root_on; //Chay root pump
				Vac_on; // Chay Vac pump
				VE_off; //Khoa van VE	
		
			}
		}
		HAL_Delay(1000);


  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* SPI1 init function */
static void MX_SPI1_Init(void)
{

  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* TIM2 init function */
static void MX_TIM2_Init(void)
{

  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;

  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 100;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_ETRMODE2;
  sClockSourceConfig.ClockPolarity = TIM_CLOCKPOLARITY_NONINVERTED;
  sClockSourceConfig.ClockPrescaler = TIM_CLOCKPRESCALER_DIV1;
  sClockSourceConfig.ClockFilter = 0;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* TIM4 init function */
static void MX_TIM4_Init(void)
{

  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;

  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 36000;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 2000;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* USART1 init function */
static void MX_USART1_UART_Init(void)
{

  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(led_GPIO_Port, led_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, Pump1_Pin|Pump2_Pin|Root_Pin|Vac_Pin 
                          |Cooler_Pin|VE_Pin|Heater1_Pin|Heater2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : led_Pin */
  GPIO_InitStruct.Pin = led_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(led_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PC14 PC15 */
  GPIO_InitStruct.Pin = GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : LOW_SENSOR_Pin HIGH_SENSOR_Pin BUBBLE_SENSOR_Pin */
  GPIO_InitStruct.Pin = LOW_SENSOR_Pin|HIGH_SENSOR_Pin|BUBBLE_SENSOR_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : SPI1_CS_Pin */
  GPIO_InitStruct.Pin = SPI1_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(SPI1_CS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : Pump1_Pin Pump2_Pin Root_Pin Vac_Pin 
                           Cooler_Pin VE_Pin Heater1_Pin Heater2_Pin */
  GPIO_InitStruct.Pin = Pump1_Pin|Pump2_Pin|Root_Pin|Vac_Pin 
                          |Cooler_Pin|VE_Pin|Heater1_Pin|Heater2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);

  HAL_NVIC_SetPriority(EXTI2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI2_IRQn);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
