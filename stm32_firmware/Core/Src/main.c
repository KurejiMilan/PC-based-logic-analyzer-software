/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdlib.h>
#include <stdint.h>

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define	TRANSMITDATA 240

// for port A
#define ENABLE_LATCH 0							// tested OK
#define WRITE_ENABLE 1							// tested OK
#define RESET_COUNTER 15							// tested NOT OK
#define RESET_TRIGGER 3							// tested OK
#define LATCH_PULSE 4							// tested OK
#define TRIGGER_MUX_SELECT 5					// tested OK, values 0 = for edge 1 for = word
#define PULSEIN_DEMUX_SELECT 6					// tested OK, values 0 1 2 3
#define CLOCK_SELECT_CLOCK_IN 8					// tested OK

#define COMPLETE 11
#define TRIGGER_STATUS 12
#define INC 13

// for port B
#define DATA_FROM_MCU 8							// tested

// for port C
#define MASTER_CONTROL 15


/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */

void latchInPulse(void);
void resetTriggerSignal(void);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */

  // stores received command from PC software
  uint8_t receivedWord = 0;
  // operatingMode defines if it's communicating with logic analyzer software or acquiring data
  // 0  is for communicating with logic analyzer software to configure trigger and sample rates

  uint8_t ConfigTrigger = 0x01, ReadyToRx = 2, AckFromRx = 255;
  uint8_t sampleReady = 245, chunkReceiveAck =242, sampleTransmitComplete = 243, ackFromTx = 244;

  uint8_t READY_TO_TX_SAMPLE_RATE = 0x03;
  uint8_t READY_TO_TX_SAMPLE_SIZE = 0x04;

  volatile uint8_t operatingMode = 0;										// defines if it's communicating or reading samples

  volatile uint8_t triggerType = 0, triggerMask = 0, triggerWord = 0, sampleRateIndex = 2, sampleSizeIndex = 3;
  volatile uint8_t tempData = 0;
  volatile uint16_t blockTransferSize = 32;   	                           	// default number due to default sample size 3
  uint8_t samplePtr[256];

  GPIOC->ODR &=  ~(1<<13);
//  GPIOC->ODR |= 1<<13;

  HAL_Delay(5000);															// LETTING THE FPGA TO START
  HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
  // initializing FPGA

  // making sure that FPGA is not clocked
  GPIOC->ODR &= ~(1<<MASTER_CONTROL);

  // making sure that input latch are disabled
  GPIOA->ODR &= ~1;

  // setting default clock rate
  GPIOB->ODR = 2<<DATA_FROM_MCU;
  GPIOA->ODR |= 1<<CLOCK_SELECT_CLOCK_IN;
  HAL_Delay(1);
  GPIOA->ODR &= ~(1<<CLOCK_SELECT_CLOCK_IN);

  // setting default sample size
  GPIOB->ODR = 0b00011111<<DATA_FROM_MCU;
  GPIOA->ODR &= ~(3<<PULSEIN_DEMUX_SELECT);
  latchInPulse();

  GPIOB->ODR = 0b11111111<<DATA_FROM_MCU;
  GPIOA->ODR &= ~(3<<PULSEIN_DEMUX_SELECT);
  GPIOA->ODR |= 1<<PULSEIN_DEMUX_SELECT;
  latchInPulse();

  // IF TRIGGER STATUS IS SET FOR SOME REASON RESET IT
  resetTriggerSignal();

  //RESET counter
  // resets on rising edge can be set to low when starting sampling process
  GPIOA->ODR |= 1<<RESET_COUNTER;
  HAL_Delay(1);
  GPIOA->ODR &= ~(1<<RESET_COUNTER);


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  // communicate with software mode
	  if(operatingMode == 0){

		  HAL_UART_Receive(&huart1, &receivedWord, 1, HAL_MAX_DELAY);
		  tempData = receivedWord;

		  if(tempData == (uint8_t)TRANSMITDATA) operatingMode = 1;

		  //	if trigger setting is received then
		  else if(tempData == ConfigTrigger){
			  HAL_UART_Transmit(&huart1, &ReadyToRx, 1, HAL_MAX_DELAY);
			  // receive the trigger type
			  HAL_UART_Receive(&huart1, &receivedWord, 1, HAL_MAX_DELAY);
			  triggerType = receivedWord;

			  if(triggerType == 0){
				  // no trigger
				  // the no trigger should be handler in other section
				  HAL_UART_Transmit(&huart1, &AckFromRx, 1, 100);
				  HAL_Delay(10);
				  GPIOC->ODR &=  ~(1<<13);
			  }

			  else if(triggerType == 1){
				  // this is edge trigger
				  HAL_UART_Transmit(&huart1,  &AckFromRx, 1, 100);
				  HAL_Delay(10);
				  HAL_UART_Receive(&huart1, &receivedWord, 1, HAL_MAX_DELAY);
				  triggerMask = receivedWord;
				  HAL_UART_Transmit(&huart1, &AckFromRx, 1, 100);
				  HAL_Delay(5);
				  HAL_UART_Receive(&huart1, &receivedWord, 1, HAL_MAX_DELAY);
				  triggerWord = ~receivedWord;														// this inversion is necessary look at xor truth table
				  HAL_UART_Transmit(&huart1,  &AckFromRx, 1, 100);
				  HAL_Delay(1);

				  GPIOA->ODR &= ~(1<<TRIGGER_MUX_SELECT);
				  if((triggerWord == 0X0f)&&(triggerMask == 0x0f)) GPIOC->ODR &=  ~(1<<13);			// just to validate if correct data is transmitted
			  }

			  else if(triggerType == 2){
				  // this is word trigger
				  HAL_UART_Transmit(&huart1, &AckFromRx, 1, 100);
				  HAL_Delay(10);
				  HAL_UART_Receive(&huart1, &receivedWord, 1, HAL_MAX_DELAY);
				  triggerWord = receivedWord;														// this inversion is necessary look at xor truth table
				  HAL_UART_Transmit(&huart1,  &AckFromRx, 1, 100);
				  HAL_Delay(1);

				  triggerMask = 0XFF;

				  GPIOA->ODR |= (1<<TRIGGER_MUX_SELECT);

				  if(triggerWord == 0X0f) GPIOC->ODR &=  ~(1<<13);			// JUST FOR TEST
			  }

			  // since no trigger is handled in different way
			  // makes use of same code block for both edge and word trigger
			  if(triggerType != 0){
				  // SETTING THE MASK REGISTER IN TRIGGER MODULE
				  GPIOA->ODR &= ~(3<<PULSEIN_DEMUX_SELECT);
				  GPIOA->ODR |= 2<<PULSEIN_DEMUX_SELECT;
				  GPIOB->ODR = triggerMask<<DATA_FROM_MCU;
				  latchInPulse();

				  // SETTING THE WORD REGISTER IN TRIGGER MODULE
				  GPIOA->ODR &= ~(3<<PULSEIN_DEMUX_SELECT);
				  GPIOA->ODR |= 3<<PULSEIN_DEMUX_SELECT;
				  GPIOB->ODR = triggerWord<<DATA_FROM_MCU;
				  latchInPulse();

				  resetTriggerSignal();
			  }
		  }


		  // setting sample rate // tested
		  else if(tempData == READY_TO_TX_SAMPLE_RATE){
			  HAL_UART_Transmit(&huart1, &ReadyToRx, 1, 100);
			  HAL_Delay(10);
			  HAL_UART_Receive(&huart1, &receivedWord, 1, HAL_MAX_DELAY);
			  sampleRateIndex = receivedWord;
			  HAL_UART_Transmit(&huart1, &AckFromRx, 1, 100);
			  HAL_Delay(1);

			  switch (sampleRateIndex){
			  	  case 0:
			  		  GPIOB->ODR = 0<<DATA_FROM_MCU;
			  		  break;
			  	  case 1:
			  		  GPIOB->ODR = 1<<DATA_FROM_MCU;
			  		  break;
			  	  case 2:
			  		  GPIOB->ODR = 2<<DATA_FROM_MCU;
			  		  break;
			  	  case 3:
			  		  GPIOB->ODR = 3<<DATA_FROM_MCU;
			  		  break;
			  	  default:
			  		  GPIOB->ODR = 4<<DATA_FROM_MCU;

			  }

			  GPIOA->ODR |= 1<<CLOCK_SELECT_CLOCK_IN;
			  HAL_Delay(1);
			  GPIOA->ODR &= ~(1<<CLOCK_SELECT_CLOCK_IN);

			  if (sampleRateIndex == 0) GPIOC->ODR &=  ~(1<<13);			// JUST FOR TEST
		  }

		  // setting sample size //tested
		  else if(tempData == READY_TO_TX_SAMPLE_SIZE){
			  HAL_UART_Transmit(&huart1, &ReadyToRx, 1, 100);
			  HAL_Delay(10);
			  HAL_UART_Receive(&huart1, &receivedWord, 1, HAL_MAX_DELAY);
			  sampleSizeIndex = receivedWord;
			  HAL_UART_Transmit(&huart1, &AckFromRx, 1, 100);
			  HAL_Delay(1);

			  // setting compare address msb
			  switch(sampleSizeIndex){
			  	  case 0:
			  		  GPIOB->ODR = 0b00000011<<DATA_FROM_MCU;
			  		  blockTransferSize = 4;
			  		  break;
			  	  case 1:
			  		  GPIOB->ODR = 0b00000111<<DATA_FROM_MCU;
			  		  blockTransferSize = 8;
			  		  break;
			  	  case 2:
			  		  GPIOB->ODR = 0b00001111<<DATA_FROM_MCU;
			  		  blockTransferSize = 16;
			  		  break;
			  	  case 3:
			  		  GPIOB->ODR = 0b00011111<<DATA_FROM_MCU;
			  		  blockTransferSize = 32;
			  		  break;
			  	  case 4:
			  		  GPIOB->ODR = 0b00111111<<DATA_FROM_MCU;
			  		  blockTransferSize = 64;
			  		  break;
			  	  case 5:
			  		  GPIOB->ODR = 0b01111111<<DATA_FROM_MCU;
			  		  blockTransferSize = 128;
			  		  break;
			  	  default:
			  		  GPIOB->ODR = 0b11111111<<DATA_FROM_MCU;
			  		  blockTransferSize = 256;
			  }

			  GPIOA->ODR &= ~(3<<PULSEIN_DEMUX_SELECT);
			  latchInPulse();

			  // does not need this as lsb value does not need to change
			  // setting compare address lsb
//			  GPIOB->ODR = 0b11111111<<DATA_FROM_MCU;
//
//			  GPIOA->ODR &= ~(3<<PULSEIN_DEMUX_SELECT);
//			  GPIOA->ODR |= 1<<PULSEIN_DEMUX_SELECT;
//			  latchInPulse();

			  if (sampleSizeIndex == 1) GPIOC->ODR &=  ~(1<<13);			// JUST FOR TEST
		  }

		  tempData = 0x00;
	  }

	  // read acquire data mode
	  else if(operatingMode == 1){
		  // uncomment this section for final firmware
		  HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);

		  if(triggerType == 0){
			  // set the trigger signal
			  if(! (GPIOA->IDR & (1<<TRIGGER_STATUS)) ){
				GPIOA->ODR |= 1<<RESET_TRIGGER;
				HAL_Delay(1);
				GPIOA->ODR &= ~(1<<RESET_TRIGGER);
			  }
		  }else{
			  // reset the trigger signal
			  resetTriggerSignal();
		  }

		  // reseting counter to 0 to start counting at 0 address
		  GPIOA->ODR |= 1<<RESET_COUNTER;
		  HAL_Delay(1);
		  GPIOA->ODR &= ~(1<<RESET_COUNTER);


		  GPIOA->ODR |= (1<<ENABLE_LATCH) | (1<<WRITE_ENABLE);		// enable latch and enables write operation
		  GPIOC->ODR |= 1<<MASTER_CONTROL;

		  // poll until complete signal enabled
		  while(! (GPIOA->IDR & (1<<COMPLETE)) );
		  HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);

		  GPIOA->ODR &= ~( (1<<WRITE_ENABLE) | (1<<ENABLE_LATCH) ); // putting it into read mode
		  GPIOC->ODR &= ~(1<<MASTER_CONTROL);

		  HAL_UART_Transmit(&huart1, &sampleReady, 1, 100);
		  HAL_UART_Receive(&huart1, &receivedWord, 1, HAL_MAX_DELAY);

		  GPIOA->ODR |= ( 1<<INC );
		  GPIOA->ODR |= ( 1<<RESET_COUNTER );
		  HAL_Delay(1);
		  GPIOA->ODR &= ~(1<<RESET_COUNTER);
		  GPIOA->ODR &= ~(1<<INC);



		  for(int i = 0; i<blockTransferSize; i++){

			  for(int j = 0; j < 256; j++){
				 samplePtr[j] = (GPIOB->IDR) & 0xFF;										// reading values
				 GPIOA->ODR |= (1<<INC);
				 GPIOA->ODR |= (1<<INC);
				 GPIOA->ODR &= ~(1<<INC);
			  }

			  HAL_UART_Transmit(&huart1, samplePtr, 256, HAL_MAX_DELAY);
			  HAL_UART_Receive(&huart1, &receivedWord, 1, HAL_MAX_DELAY);
		  }

		  // test signal section
		  // creating test data
//		  GPIOC->ODR |=  (1<<13);
//		  for(int i=0 ; i < 256; i++){
//			  HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
//			  samplePtr[i] = i;
//		  }
//		 GPIOC->ODR &=  ~(1<<13);
//		 HAL_UART_Transmit(&huart1, &sampleReady, 1, 100);               // had 1 second delay
//		 HAL_UART_Receive(&huart1, &receivedWord, 1, HAL_MAX_DELAY);
//
//		 for(int i = 0; i<blockTransferSize; i++){
//			GPIOC->ODR |=  (1<<13);
//		  	HAL_UART_Transmit(&huart1, samplePtr, 256, HAL_MAX_DELAY);
//		  	HAL_Delay(1);                                     // was 10
//		  	GPIOC->ODR &=  ~(1<<13);
//		  	HAL_UART_Receive(&huart1, &receivedWord, 1, HAL_MAX_DELAY);
//		  	HAL_Delay(1);
//		  }
//		 GPIOC->ODR |=  (1<<13);
//		 HAL_Delay(1);

		  HAL_UART_Transmit(&huart1, &sampleTransmitComplete, 1, 100);
		  resetTriggerSignal();
		  operatingMode = 0;
	  }


  }
  /* USER CODE END 3 */
}

void latchInPulse(){
	 GPIOA->ODR |= 1<<LATCH_PULSE;
	 HAL_Delay(1);
	 GPIOA->ODR &= ~(1<<LATCH_PULSE);
}

void resetTriggerSignal(){
	if(GPIOA->IDR & (1<<TRIGGER_STATUS)){
		 GPIOA->ODR |= 1<<RESET_TRIGGER;
		 HAL_Delay(1);
		 GPIOA->ODR &= ~(1<<RESET_TRIGGER);
	 }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
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
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
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
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13|GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_3|GPIO_PIN_4
                          |GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8
                          |GPIO_PIN_13|GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13
                          |GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_8|GPIO_PIN_9, GPIO_PIN_RESET);

  /*Configure GPIO pins : PC13 PC15 */
  GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PA0 PA1 PA3 PA4
                           PA5 PA6 PA7 PA8
                           PA13 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_3|GPIO_PIN_4
                          |GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8
                          |GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB0 PB1 PB3 PB4
                           PB5 PB6 PB7 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_3|GPIO_PIN_4
                          |GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PB2 */
  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PB10 PB11 PB12 PB13
                           PB14 PB15 PB8 PB9 */
  GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13
                          |GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_8|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PA11 PA12 */
  GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PA15 */
  GPIO_InitStruct.Pin = GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

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
  __disable_irq();
  while (1)
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
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
