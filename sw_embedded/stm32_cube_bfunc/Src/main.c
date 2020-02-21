
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

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim1;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
enum states {
	IDLE,
	WFM_OUT_SINE,
	WFM_OUT_TRIANGLE,
	WFM_OUT_SQUARE,
	WFM_OUT_SAWTOOTH
};

enum waveform_modes {
	SINE, 
	TRIANGLE,
	SQUARE,
	SAWTOOTH
};

struct ad9837_ctrl_reg {
	int freqreg		: 2; 
	int b28			: 1;
	int hlb			: 1;
	int fsel		: 1;
	int psel		: 1;
	int reserved9   : 1;
	int reset	    : 1;
	int sleep1	    : 1;
	int sleep12	    : 1;
	int opbiten	    : 1;
	int reserved4   : 1;
	int div2	    : 1;
	int reserved2   : 1;
	int mode	    : 1;
	int reserved0   : 1;
};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM1_Init(void);
static void MX_SPI1_Init(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
void InitCtrlAD9837(struct ad9837_ctrl_reg *dds_control);
void StartOutput(void);
void StopOutput(void);
void SetFreq0Value(uint32_t freq);
void SetWaveformMode(enum   waveform_modes  Mode, 
					 struct ad9837_ctrl_reg *dds_control);
enum states NextState(enum states CurrentState);
/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
void delay_us(unsigned int time) 
{
	SysTick->LOAD = time;
	SysTick->VAL  = 0;
	while(!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk));
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  // Note: In 16 bit mode, byte data[1] will be transmitted first
  /*
  In this example structure, a bus transaction will yield "2 1 4 3"
    uint8_t data[8] = { 1, 2, 3, 4, 0, 0, 0, 0 };
  */
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
  MX_TIM1_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */
  // Following line included to enable SWD debug
  AFIO->MAPR = AFIO_MAPR_SWJ_CFG_JTAGDISABLE;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  enum states current_state = IDLE;
  enum waveform_modes current_mode = SINE;
  struct ad9837_ctrl_reg dds_control;

  InitCtrlAD9837(&dds_control);

  // Initializing AD9837 as per ADI App Note AN-1070
  SetFreq0Value(0x00002000);

  while (1)
  {
	GPIOC->BSRR = GPIO_BSRR_BS13;
  
	delay_us(48000000);
	current_state = NextState(current_state);
	GPIOC->BSRR = GPIO_BSRR_BR13;
	delay_us(48000000);
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
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
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
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
  hspi1.Init.DataSize = SPI_DATASIZE_16BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_HIGH;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_HARD_OUTPUT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* TIM1 init function */
static void MX_TIM1_Init(void)
{

  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;

  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 0;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 0;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
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
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GRN_LED_GPIO_Port, GRN_LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : GRN_LED_Pin */
  GPIO_InitStruct.Pin = GRN_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GRN_LED_GPIO_Port, &GPIO_InitStruct);

}


/* USER CODE BEGIN 4 */
void InitCtrlAD9837(struct ad9837_ctrl_reg *dds_control)
{
	dds_control->freqreg	= 0;
	dds_control->b28		= 1;
	dds_control->hlb		= 0;
	dds_control->fsel		= 0;
	dds_control->psel		= 0;
	dds_control->reserved9  = 0;
	dds_control->reset	    = 0;
	dds_control->sleep1	    = 0;
	dds_control->sleep12    = 0;
	dds_control->opbiten    = 0;
	dds_control->reserved4  = 0;
	dds_control->div2	    = 0;
	dds_control->reserved2  = 0;
	dds_control->mode	    = 0;
	dds_control->reserved0  = 0;
}

void SetFreq0Value(uint32_t freq) 
{
	
	uint8_t data[2] = { 0x00, 0x20 };
	// Note: All SPI transactions are len=1, but 
	// require a 2 byte input due to the peripheral being 
	// in 16-bit output mode
	
	// Sets 14 LSBs of Freq0 register
	data[0] = (uint8_t)(freq & 0x000000ff);
	data[1] = (uint8_t)(0x40 | ((freq & 0x00003f00)>>8));
	HAL_SPI_Transmit(&hspi1, data, 1, 10);
	// Sets 14 MSBs of Freq0 register
	data[0] = (uint8_t)((freq & 0x003fc000)>>14);
	data[1] = (uint8_t)(0x40 | ((freq & 0x0fc00000)>>22));
	HAL_SPI_Transmit(&hspi1, data, 1, 10);
     	
}

void StartOutput(void) {
	uint8_t data[2] = { 0x00, 0x20 };
	// Note: All SPI transactions are len=1, but 
	// require a 2 byte input due to the peripheral being 
	// in 16-bit output mode
	HAL_SPI_Transmit(&hspi1, data, 1, 10);
}

void StopOutput(void) 
{
	
	uint8_t data[2] = { 0x00, 0x21 };
	// Note: All SPI transactions are len=1, but 
	// require a 2 byte input due to the peripheral being 
	// in 16-bit output mode
	HAL_SPI_Transmit(&hspi1, data, 1, 10);
    	
}

void SetWaveformMode(enum   waveform_modes  Mode, 
					 struct ad9837_ctrl_reg *dds_control)
{
	uint8_t data[2] = { 0x00, 0x00 };
	switch (Mode) {
		case SINE:
			// Clears OPBITEN and MODE bits
			dds_control->opbiten = 0;
			dds_control->mode    = 0;
			break;
		case TRIANGLE:
			// Sets MODE bit; clears OPBITEN
			dds_control->opbiten = 0;
			dds_control->mode    = 1;
			data[0] = 0x02;
			data[1] = 0x20;
			break;
		case SQUARE:
			// Sets OPBITEN;
			dds_control->opbiten = 1;
			data[0] = 0x20;
			data[1] = 0x20;
			break;
		default:
			break;	
	}
}

enum states NextState(enum states CurrentState)
{
	switch (CurrentState) {
		case IDLE:
			StartOutput();
			return WFM_OUT_SINE;
			break;
		case WFM_OUT_SINE:
			StopOutput();
			return IDLE;
			break;
		default:
			StopOutput();
			return IDLE;
			break;
	}
}
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
