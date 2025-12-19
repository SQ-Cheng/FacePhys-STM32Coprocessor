/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lcd.h"
#include "stm32g0xx_hal.h"
#include "stm32g0xx_hal_gpio.h"
#include "stm32g0xx_hal_i2c.h"
#include "stm32g0xx_hal_uart.h"
#include <stdint.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define PACKET_SIZE 8
#define RING_BUFFER_CAPACITY (PACKET_SIZE * 10)
#define HEADER_DATA 0xFF

#define BATTERY_FULL_VOLTAGE 4200
#define BATTERY_EMPTY_VOLTAGE 3500
#define ADC_VREF 3220
#define VOLTAGE_DIVIDER_RATIO 43/33

#define DATA_SOURCE_USART 0x00
#define DATA_SOURCE_I2C 0x01

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

I2C_HandleTypeDef hi2c2;

SPI_HandleTypeDef hspi1;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
uint8_t ring_buffer[RING_BUFFER_CAPACITY];
uint16_t ring_buffer_head = 0;
uint16_t ring_buffer_tail = 0;
uint16_t ring_buffer_available = 0;
uint8_t rx_data[PACKET_SIZE];

uint8_t update_display = 0;

uint8_t hr = 0;
uint8_t hrv = 0;
uint8_t pressure = 0;
uint8_t emotion = 0;
uint8_t eyemotion = 0;
uint8_t reliability = 0;
uint8_t battery_level = 0;

uint8_t bootstr[] = "Boot Successful!\n";

uint8_t liney[] = {5, 26, 47, 68, 88, 109};
uint8_t columnx[] = {10, 50, 80, 80};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_ADC1_Init(void);
static void MX_I2C2_Init(void);
/* USER CODE BEGIN PFP */
void Test_Perips(void);
void System_Init(void);
void Draw_UI(void);
void HandleRXData(uint8_t *rxdata, uint8_t data_source);
void Update_Display(void);
uint8_t Read_Battery_Level(void);
void Enter_Sleep_Mode(void);
void Exit_Sleep_Mode(void);

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
  MX_SPI1_Init();
  MX_USART1_UART_Init();
  MX_ADC1_Init();
  MX_I2C2_Init();
  /* USER CODE BEGIN 2 */
  Test_Perips();
  System_Init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    if (update_display) {
      HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
      Update_Display();
      HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
      update_display = 0;
    }
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

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
  RCC_OscInitStruct.PLL.PLLN = 8;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc1.Init.LowPowerAutoWait = DISABLE;
  hadc1.Init.LowPowerAutoPowerOff = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc1.Init.SamplingTimeCommon1 = ADC_SAMPLETIME_160CYCLES_5;
  hadc1.Init.SamplingTimeCommon2 = ADC_SAMPLETIME_160CYCLES_5;
  hadc1.Init.OversamplingMode = DISABLE;
  hadc1.Init.TriggerFrequencyMode = ADC_TRIGGER_FREQ_HIGH;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLINGTIME_COMMON_1;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.Timing = 0x10B17DB5;
  hi2c2.Init.OwnAddress1 = 132;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_1LINE;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_HARD_OUTPUT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

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
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart1, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart1, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart1) != HAL_OK)
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
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LCD_BL_Pin|LCD_DC_Pin|LCD_RST_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(FAN_GPIO_Port, FAN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : LED_Pin */
  GPIO_InitStruct.Pin = LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : SWITCH_Pin */
  GPIO_InitStruct.Pin = SWITCH_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(SWITCH_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LCD_BL_Pin LCD_DC_Pin LCD_RST_Pin */
  GPIO_InitStruct.Pin = LCD_BL_Pin|LCD_DC_Pin|LCD_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : FAN_Pin */
  GPIO_InitStruct.Pin = FAN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(FAN_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void System_Init(void) {
  LCD_Init();
  LCD_Fill(0, 0, 160, 128, LCD_COLOR_BLACK);
  LCD_BL_SET();

  Draw_UI();

  HAL_UART_Receive_IT(&huart1, rx_data, 1);
  HAL_UART_Transmit_IT(&huart1, bootstr, sizeof(bootstr));

  HAL_I2C_Slave_Receive_IT(&hi2c2, rx_data, PACKET_SIZE);
}

/**
  * @brief  Draw the main UI elements
  * @retval None
  */
void Draw_UI(void) {
  uint8_t hr_header[] = "心率";
  uint8_t hrv_header[] = "HRV";
  uint8_t pressure_header[] = "压力";
  uint8_t emotion_header[] = "情绪";
  uint8_t eyemotion_header[] = "眼动";
  uint8_t reliability_header[] = "置信度";
  
  uint8_t bpm_str[] = "BPM";
  uint8_t ms_str[] = "ms";


  LCD_ShowChinese(columnx[0], liney[0], hr_header, LCD_COLOR_WHITE, LCD_COLOR_BLACK, 16, 1);
  LCD_ShowString(columnx[0], liney[1], hrv_header, LCD_COLOR_WHITE, LCD_COLOR_BLACK, 16, 1);
  LCD_ShowChinese(columnx[0], liney[2], pressure_header, LCD_COLOR_WHITE, LCD_COLOR_BLACK, 16, 1);
  LCD_ShowChinese(columnx[0], liney[3], emotion_header, LCD_COLOR_WHITE, LCD_COLOR_BLACK, 16, 1);
  //LCD_ShowChinese(columnx[0], liney[4], eyemotion_header, LCD_COLOR_WHITE, LCD_COLOR_BLACK, 16, 1);
  LCD_ShowChinese(columnx[0], liney[4], reliability_header, LCD_COLOR_WHITE, LCD_COLOR_BLACK, 16, 1);

  LCD_ShowString(columnx[3], 5, bpm_str, LCD_COLOR_WHITE, LCD_COLOR_BLACK, 16, 1);
  LCD_ShowString(columnx[3], 26, ms_str, LCD_COLOR_WHITE, LCD_COLOR_BLACK, 16, 1);
}

void Ring_Buffer_Push(uint8_t *buffer, uint16_t *head, uint8_t data) {
  buffer[*head] = data;
  *head = (*head + 1) % RING_BUFFER_CAPACITY;
  ring_buffer_available++;
}

uint8_t Ring_Buffer_Pop(uint8_t *buffer, uint16_t *tail, uint8_t *data) {
  *data = buffer[*tail];
  *tail = (*tail + 1) % RING_BUFFER_CAPACITY;
  ring_buffer_available--;
  return *data;
}

void HandleRXData(uint8_t *packet, uint8_t data_source) {
  if(data_source == DATA_SOURCE_USART) {
    Ring_Buffer_Push(ring_buffer, &ring_buffer_head, packet[0]);
  }
  else if(data_source == DATA_SOURCE_I2C) {
    for (uint8_t i = 0; i < PACKET_SIZE; i++) {
      Ring_Buffer_Push(ring_buffer, &ring_buffer_head, packet[i]);
    }
  }

  while (ring_buffer_available >= PACKET_SIZE) {
    uint8_t temp_packet[PACKET_SIZE];
    Ring_Buffer_Pop(ring_buffer, &ring_buffer_tail, &temp_packet[0]);

    if (temp_packet[0] == HEADER_DATA) {
      Ring_Buffer_Pop(ring_buffer, &ring_buffer_tail, &temp_packet[1]);
      if (temp_packet[1] == HEADER_DATA) {
        for (uint8_t i = 2; i < PACKET_SIZE; i++) {
          Ring_Buffer_Pop(ring_buffer, &ring_buffer_tail, &temp_packet[i]);
        }
        hr = temp_packet[2];
        hrv = temp_packet[3];
        pressure = temp_packet[4];
        emotion = temp_packet[5];
        eyemotion = temp_packet[6];
        reliability = temp_packet[7];
        update_display = 1;
        battery_level = Read_Battery_Level();
        HAL_UART_Transmit(&huart1, &battery_level, 1, 100);
      }
    } 
  }
}

void Update_Display(void) {
  uint8_t *pressure_strs[4] = {"低", "中", "高", "无"};
  uint8_t *emotion_strs[9] = {"愤怒", "蔑视", "厌恶", "恐惧", "快乐", "中性", "悲伤", "惊讶", "未知"};
  uint8_t *eyemotion_strs[9] = {"←", "↖", "↑", "↗", "→", "↘", "↓", "↙", "无"};
  uint8_t square_char[] = "■";


  LCD_ShowIntNum(columnx[1], liney[0], hr, 3, LCD_COLOR_WHITE, LCD_COLOR_BLACK, 16);
  LCD_ShowIntNum(columnx[1], liney[1], hrv, 3, LCD_COLOR_WHITE, LCD_COLOR_BLACK, 16);
  if  (pressure > 3) pressure = 3;
  LCD_ShowChinese(columnx[2], liney[2], pressure_strs[pressure], LCD_COLOR_WHITE,
                  LCD_COLOR_BLACK, 16, 0);
  if (emotion > 8) emotion = 8;
  LCD_ShowChinese(columnx[2], liney[3], emotion_strs[emotion], LCD_COLOR_WHITE,
                  LCD_COLOR_BLACK, 16, 0);
  //if (eyemotion > 8) eyemotion = 8;
  //LCD_ShowChinese(columnx[2], liney[4], eyemotion_strs[eyemotion], LCD_COLOR_WHITE, LCD_COLOR_BLACK, 16, 0);

  switch (reliability) {
    case 0:
      LCD_ShowChinese(columnx[2], liney[4], square_char, LCD_COLOR_GREEN, LCD_COLOR_BLACK, 16, 0);
      break;
    case 1:
      LCD_ShowChinese(columnx[2], liney[4], square_char, LCD_COLOR_YELLOW, LCD_COLOR_BLACK, 16, 0);
      break;
    case 2:
      LCD_ShowChinese(columnx[2], liney[4], square_char, LCD_COLOR_RED, LCD_COLOR_BLACK, 16, 0);
      break;
    default:
      LCD_ShowChinese(columnx[2], liney[4], square_char, LCD_COLOR_WHITE, LCD_COLOR_BLACK, 16, 0);
      break;
  }
}

uint8_t Read_Battery_Level(void) {
  uint32_t adc_value = 0;
  uint32_t battery_voltage;
  uint8_t battery_percentage;
  HAL_ADC_Start(&hadc1);
  if (HAL_ADC_PollForConversion(&hadc1, 100) == HAL_OK) {
    adc_value = HAL_ADC_GetValue(&hadc1);
  }
  HAL_ADC_Stop(&hadc1);

  battery_voltage = (adc_value * ADC_VREF * 43) / (4095 * 33);

  if (battery_voltage >= BATTERY_FULL_VOLTAGE) {
    battery_percentage = 100;
  } else if (battery_voltage <= BATTERY_EMPTY_VOLTAGE) {
    battery_percentage = 0;
  } else {
    battery_percentage = (uint8_t)(((battery_voltage - BATTERY_EMPTY_VOLTAGE) * 100) / 
                                   (BATTERY_FULL_VOLTAGE - BATTERY_EMPTY_VOLTAGE));
  }

  return battery_percentage;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
  if (huart->Instance == USART1) {
    HandleRXData(rx_data, DATA_SOURCE_USART);
    HAL_UART_Receive_IT(&huart1, rx_data, 1);
  }
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart) {
  if (huart->Instance == USART1) {
    __HAL_UART_CLEAR_OREFLAG(huart);
    HAL_UART_AbortReceive_IT(huart);
    HAL_UART_Receive_IT(huart, rx_data, 1);
    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
  }
}

void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c) {
  if (hi2c->Instance == I2C2) {
    HandleRXData(rx_data, DATA_SOURCE_I2C);
    HAL_I2C_Slave_Receive_IT(hi2c, rx_data, PACKET_SIZE);
  }
}

void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c) {
  if (hi2c->Instance == I2C2) {
    __HAL_I2C_CLEAR_FLAG(hi2c, I2C_FLAG_BERR);
    HAL_I2C_Slave_Receive_IT(hi2c, rx_data, PACKET_SIZE);
    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
  }
}

void Test_Perips(void) {
  for(int i = 0; i < 5; i++) {
    HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
    HAL_Delay(200);
  }
  HAL_GPIO_WritePin(FAN_GPIO_Port, FAN_Pin, GPIO_PIN_RESET);
  HAL_Delay(2000);
  HAL_GPIO_WritePin(FAN_GPIO_Port, FAN_Pin, GPIO_PIN_SET);
}

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
