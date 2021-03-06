/* USER CODE BEGIN Header */
/**scuko
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
  * COPYRIGHT(c) 2018 STMicroelectronics
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
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "crc.h"
#include "dma.h"
#include "iwdg.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "digiTOS-Lib/digiTOS-Core.h"
#include <stdio.h>
#include <string.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_NVIC_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

static void timer1_deadtime(int dt)
{
    // Tdts = Tck_int //
    TIM1->CR1 &= ~TIM_CR1_CKD;
    TIM1->BDTR &= ~TIM_BDTR_DTG;

    //TIM1->BDTR |= TIM_BDTR_DTG_2 | TIM_BDTR_DTG_1 | TIM_BDTR_DTG_0;     // value dead-time

    TIM1->BDTR |= (dt & 0x1f);//~(TIM_BDTR_DTG)
    TIM1->BDTR |= TIM_BDTR_DTG_6;// | TIM_BDTR_DTG_0;
    TIM1->BDTR |= TIM_BDTR_MOE | TIM_BDTR_AOE; // enable generation output
}

void PWM_50Hz_Init (void) {
	//RCC->APB2ENR |= RCC_APB2ENR_TIM16EN; // enable clock for TIM1
	//RCC->APB2ENR |= RCC_APB2ENR_IOPAEN; // enable clock for port A
	//RCC->APB2ENR |= RCC_APB2ENR_IOPBEN; // enable clock for port B
	//RCC->APB2ENR |= RCC_APB2ENR_AFIOEN; // enable clock for alternative gpio

	/****************************** Setting PA10 **************************************/

	//GPIOA->CRH 	&= ~GPIO_CRH_CNF10; // setting out alternative push-pull for PWM
	//GPIOA->CRH 	|= GPIO_CRH_CNF10_1;

	//GPIOA->CRH	&= ~GPIO_CRH_MODE10;
	//GPIOA->CRH	|= GPIO_CRH_MODE10; // gpio speed 50 MHz

	/***************************** Setting PB15 ***************************************/

	//GPIOB->CRH 	&= ~GPIO_CRH_CNF15; // setting complementary for CH3N
	//GPIOB->CRH 	|= GPIO_CRH_CNF15_1;

	//GPIOB->CRH	&= ~GPIO_CRH_MODE15;
	//GPIOB->CRH	|= GPIO_CRH_MODE15; // gpio speed 50 MHz

	/************************** Config PWM channel ************************************/

	TIM1->PSC = 960-1; // div for clock: F = SYSCLK / [PSC + 1]
	TIM1->ARR = 1000; // count to 1000
	//TIM1->CR1 &= ~TIM_CR1_CKD; // div for dead-time: Tdts = 1/Fosc = 41.6 ns
	TIM1->CCR3 = 500; // duty cycle 50%

	TIM1->CCER |= TIM_CCER_CC3E | TIM_CCER_CC3NE; // enable PWM complementary out
	//TIM1->CCER &= ~TIM_CCER_CC3NP; // active high level: 0 - high, 1 - low
	TIM1->CCER |= TIM_CCER_CC3NP; // active high level: 0 - high, 1 - low
	TIM1->CCER |= TIM_CCER_CC3P; // active high level: 0 - high, 1 - low

	TIM1->CR2 |= TIM_CR2_OIS3;
	TIM1->CR2 |= TIM_CR2_OIS3N;


	TIM1->CCMR2 &= ~TIM_CCMR2_OC3M;
	TIM1->CCMR2 |= TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1; // positiv PWM

	//TIM1->BDTR &= ~TIM_BDTR_DTG; // clear register
	//TIM1->BDTR |= TIM_BDTR_DTG_2 | TIM_BDTR_DTG_1 | TIM_BDTR_DTG_0;	 // value dead-time
	//TIM1->BDTR |= TIM_BDTR_MOE | TIM_BDTR_AOE; // enable generation output

	timer1_deadtime(30);

	/*******************************************************************************/

	TIM1->CR1 &= ~TIM_CR1_DIR; // count up: 0 - up, 1 - down
	TIM1->CR1 &= ~TIM_CR1_CMS; // aligned on the front signal
	TIM1->CR1 |= TIM_CR1_CEN; // start count

	}

void PWM_Sinus_Init (void){
//RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; // enable clock for TIM2
//RCC->APB2ENR |= RCC_APB2ENR_IOPAEN; // enable clock for port A
//RCC->APB2ENR |= RCC_APB2ENR_AFIOEN; // enable clock for alternative gpio

/****************************** Setting PA0 ***************************************/

//GPIOA->CRL 	&= ~GPIO_CRL_CNF0; // setting out alternative push-pull for PWM1_CH1
//GPIOA->CRL 	|= GPIO_CRL_CNF0_1;

//GPIOA->CRL	&= ~GPIO_CRL_MODE0;
//GPIOA->CRL	|= GPIO_CRL_MODE0; // gpio speed 50 MHz
/****************************** Setting PA1 ***************************************/

//GPIOA->CRL 	&= ~GPIO_CRL_CNF1; // setting out alternative push-pull for PWM1_CH1
//GPIOA->CRL 	|= GPIO_CRL_CNF1_1;

//GPIOA->CRL	&= ~GPIO_CRL_MODE1;
//GPIOA->CRL	|= GPIO_CRL_MODE1; // gpio speed 50 MHz

/*************************** Config PWM channel ***********************************/

	TIM3->PSC = SinResPSC; // div for clock: F = SYSCLK / [PSC + 1]
	TIM3->ARR = 1000; // count to 1000
	TIM3->CCR1 = 0; // duty cycle 0%
	TIM3->CCR2 = 0; // duty cycle 0%

	TIM3->CCER |= TIM_CCER_CC1E; // enable PWM out to PA8
	//TIM3->CCER &= ~TIM_CCER_CC1P; // active high level: 0 - high, 1 - low
	TIM3->CCER |= TIM_CCER_CC1P;

	TIM3->CCER |= TIM_CCER_CC2E; // enable PWM complementary out to PA9
	//TIM3->CCER &= ~TIM_CCER_CC1P; // active high level: 0 - high, 1 - low
	TIM3->CCER |= TIM_CCER_CC2P;

	TIM3->CCMR1 &= ~(TIM_CCMR1_OC1M | TIM_CCMR1_OC2M);
	TIM3->CCMR1 |= TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1 |
	 TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1; // positiv PWM1_CH1 and PWM1_CH2

	/*******************************************************************************/

	TIM3->CR1 &= ~TIM_CR1_DIR; // count up: 0 - up, 1 - down
	TIM3->CR1 &= ~TIM_CR1_CMS; // aligned on the front signal: 00 - front; 01, 10, 11 - center
	TIM3->CR1 |= TIM_CR1_CEN; // start count

}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

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
  MX_DMA_Init();
  MX_CRC_Init();
  MX_ADC_Init();
  MX_IWDG_Init();
  MX_TIM1_Init();
  MX_TIM14_Init();
  MX_USART1_UART_Init();
  MX_TIM3_Init();
  MX_TIM16_Init();

  /* Initialize interrupts */
  MX_NVIC_Init();
  /* USER CODE BEGIN 2 */

  // init LED1-LED4

  ResetWDG();

  HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin,GPIO_PIN_SET);
  HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin,GPIO_PIN_SET);
  HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin,GPIO_PIN_SET);
  HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin,GPIO_PIN_SET);


  HAL_TIM_Base_Start(&htim16);
  HAL_TIM_Base_Start_IT(&htim16);

  BoardStatus=sBoot;
  TIM3->ARR=sBoot_Delay;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  ResetWDG();

  buttonInit(&DevModeKey, DEV_MODE1_GPIO_Port, DEV_MODE1_Pin, GPIO_PIN_RESET, 30, 2000);
  buttonInit(&DevModeKey2, DEV_MODE2_GPIO_Port, DEV_MODE2_Pin, GPIO_PIN_RESET, 10, 500);
  buttonUpdate(&DevModeKey);
  buttonUpdate(&DevModeKey2);

  HAL_Delay(500);


  Get_Version();
  SerialPrintln(1);


  Get_ChipID();
  SerialPrintln(1);

  Get_FlashSize();
  SerialPrintln(1);


  ClearUART_Buff();



  if(buttonUpdate(&DevModeKey) == isPressed){
	strcpy(uart_buff,"DEV MODE - wait\r\n");
	SerialPrintln(1);
	ResetWDG();
	HAL_Delay(700);
	ResetWDG();
    HAL_Delay(700);
	//if(buttonUpdate(&DevModeKey) == isPressedLong){
	if(buttonUpdate(&DevModeKey) == isPressed){
		//HAL_TIM_Base_Start_IT(&htim16);
		strcpy(uart_buff,"DEV MODE - confirmed\r\n");
	    SerialPrintln(1);
		DevMode1=1;
	}
  }

    strcpy(uart_buff,"Start Loop\r\n");
    SerialPrintln(1);

  	BoardStatus=sAC_City;
  	TIM3->ARR=sDEF_Delay;

    //Start loop
      HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin,GPIO_PIN_RESET);
      HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin,GPIO_PIN_RESET);
      HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin,GPIO_PIN_RESET);
      HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin,GPIO_PIN_RESET);

      // Start PWM Sinus
      //SinWave=swStart;

       SinWave=swStart;

       PWM_50Hz_Init();
       PWM_Sinus_Init();

       HAL_TIM_Base_Start(&htim1);
       HAL_TIM_Base_Start_IT(&htim1);

       HAL_TIM_Base_Start(&htim3);
       HAL_TIM_Base_Start_IT(&htim3);

       //HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_3);

       TIM14->PSC=SinResPSC;

       HAL_TIM_Base_Start(&htim14);
       HAL_TIM_Base_Start_IT(&htim14);

       //HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
       //HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_2);


    while (1)
    {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  	//asm("NOP");
  	__NOP();
    }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI14|RCC_OSCILLATORTYPE_LSI
                              |RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSI14State = RCC_HSI14_ON;
  RCC_OscInitStruct.HSI14CalibrationValue = 16;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief NVIC Configuration.
  * @retval None
  */
static void MX_NVIC_Init(void)
{
  /* ADC1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(ADC1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(ADC1_IRQn);
  /* DMA1_Channel2_3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel2_3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel2_3_IRQn);
  /* RCC_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(RCC_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(RCC_IRQn);
  /* FLASH_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(FLASH_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(FLASH_IRQn);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

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
void assert_failed(char *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
