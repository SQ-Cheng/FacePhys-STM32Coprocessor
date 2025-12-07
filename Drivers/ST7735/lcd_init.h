/**
  ******************************************************************************
  * @file    lcd_init.h
  * @author  Songqin Cheng
  * @brief   Header file of ST7735 LCD initialization module.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LCD_INIT_H__
#define __LCD_INIT_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_hal.h"
#include "main.h"

/** @addtogroup ST7735_Driver
  * @{
  */

/** @defgroup LCD_INIT LCD Initialization
  * @brief ST7735 LCD initialization module driver
  * @{
  */

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/** @defgroup LCD_INIT_Exported_Constants LCD Init Exported Constants
  * @{
  */

/** @defgroup LCD_SPI_Configuration LCD SPI Configuration
 * @{
 */
#define LCD_SPI_HARDWARE_NSS_DISABLE  0U
#define LCD_SPI_HARDWARE_NSS_ENABLE 1U
/**
  * @}
  */


/** @defgroup LCD_Orientation LCD Display Orientation
  * @{
  */
#define LCD_ORIENTATION_PORTRAIT_1    0U  /*!< Portrait mode 1 */
#define LCD_ORIENTATION_PORTRAIT_2    1U  /*!< Portrait mode 2 */
#define LCD_ORIENTATION_LANDSCAPE_1   2U  /*!< Landscape mode 1 */
#define LCD_ORIENTATION_LANDSCAPE_2   3U  /*!< Landscape mode 2 */
/**
  * @}
  */

/** @defgroup LCD_Configuration LCD Configuration
  * @{
  */
#define USE_HORIZONTAL                LCD_ORIENTATION_PORTRAIT_2  /*!< Set display orientation */

#if USE_HORIZONTAL == LCD_ORIENTATION_PORTRAIT_1 || USE_HORIZONTAL == LCD_ORIENTATION_PORTRAIT_2
#define LCD_WIDTH                     128U  /*!< LCD width in portrait mode */
#define LCD_HEIGHT                    128U  /*!< LCD height in portrait mode */
#else
#define LCD_WIDTH                     128U  /*!< LCD width in landscape mode */
#define LCD_HEIGHT                    128U  /*!< LCD height in landscape mode */
#endif
/**
  * @}
  */

/** @defgroup LCD_GPIO_Pins LCD GPIO Pin Definitions
  * @{
  */
#define LCD_GPIO_Port                GPIOA        /*!< GPIO port for LCD pins */
/**
  * @}
  */

/** @defgroup LCD_SPI_Handler LCD SPI Handler Definitions
  * @{
  */
extern SPI_HandleTypeDef hspi1;       /*!< SPI handler for LCD communication */
/**
  * @}
  */

/** @defgroup LCD_SPI_Feature LCD SPI Feature Definitions
  * @{
  */
 #define LCD_SPI_TIMEOUT                100U   /*!< SPI timeout duration */
/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/

/** @defgroup LCD_INIT_Exported_Macros LCD Init Exported Macros
  * @{
  */

/** @defgroup LCD_Pin_Control LCD Pin Control Macros
  * @{
  */
#define LCD_SPI_USE_HARDWARE_NSS    LCD_SPI_HARDWARE_NSS_DISABLE  /*!< Set to enable/disable hardware NSS management */

#define LCD_RST_CLR()                 HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_RESET)   /*!< Clear Reset pin */
#define LCD_RST_SET()                 HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_SET)     /*!< Set Reset pin */

#define LCD_DC_CLR()                  HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_RESET)    /*!< Clear DC pin */
#define LCD_DC_SET()                  HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET)      /*!< Set DC pin */

#if LCD_SPI_USE_HARDWARE_NSS == LCD_SPI_HARDWARE_NSS_ENABLE
#define LCD_CS_CLR()                  HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET)    /*!< Clear CS pin */
#define LCD_CS_SET()                  HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET)      /*!< Set CS pin */
#else
#define LCD_CS_CLR()                  ((void)0)  /*!< No operation when hardware NSS is used */
#define LCD_CS_SET()                  ((void)0)  /*!< No operation when hardware NSS is used */
#endif

#define LCD_BL_CLR()                  HAL_GPIO_WritePin(LCD_BL_GPIO_Port, LCD_BL_Pin, GPIO_PIN_RESET)   /*!< Clear Backlight pin */
#define LCD_BL_SET()                  HAL_GPIO_WritePin(LCD_BL_GPIO_Port, LCD_BL_Pin, GPIO_PIN_SET)     /*!< Set Backlight pin */
/**
  * @}
  */

/**
  * @}
  */

/* Exported functions prototypes ---------------------------------------------*/

/** @defgroup LCD_INIT_Exported_Functions LCD Init Exported Functions
  * @{
  */

/** @defgroup LCD_INIT_Exported_Functions_Group1 Initialization functions
  * @brief    Initialization functions
  * @{
  */
void LCD_GPIO_Init(void);
void LCD_Init(void);
/**
  * @}
  */

/** @defgroup LCD_INIT_Exported_Functions_Group2 Low level functions
  * @brief    Low level communication functions
  * @{
  */
void LCD_WR_DATA8(uint8_t dat);
void LCD_WR_DATA(uint16_t dat);
void LCD_WR_REG(uint8_t dat);
void LCD_Address_Set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* STM32G0xx_ST7735_LCD_INIT_H */
