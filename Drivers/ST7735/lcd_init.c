/**
  ******************************************************************************
  * @file    lcd_init.c
  * @author  Songqin Cheng
  * @brief   ST7735 LCD initialization module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of the ST7735 LCD controller:
  *           + GPIO initialization for LCD interface
  *           + LCD initialization and configuration
  *           + Low level communication functions
  *
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
  @verbatim
  ==============================================================================
                    ##### ST7735 LCD Driver features #####
  ==============================================================================
  [..]
    (+) The ST7735 is a single-chip controller/driver for 262K-color, graphic
        type TFT-LCD with in-built display data RAM of 132x162x18 bits.

    (+) The ST7735 supports parallel 8-/9-/16-/18-bit data bus MCU interface,
        6-/9-/16-/18-bit data bus RGB interface and 3-/4-line serial peripheral
        interface (SPI).

    (+) Key features include:
        (++) 132 x 162 resolution and 262K color
        (++) On-chip display data RAM (132x162x18 bits)
        (++) System interface: 8-/9-/16-/18-bit parallel, Serial peripheral interface
        (++) Sleep mode for precise power control

                     ##### How to use this driver #####
  ==============================================================================
  [..]
    (#) Initialize the LCD GPIO pins using LCD_GPIO_Init().

    (#) Initialize the LCD controller using LCD_Init().
        (++) This function configures the ST7735 registers for proper operation
        (++) Sets up frame rate, power sequence, gamma correction
        (++) Configures display orientation based on USE_HORIZONTAL setting

    (#) Use the low-level communication functions for custom operations:
        (++) LCD_WR_REG() to write commands
        (++) LCD_WR_DATA8() to write single byte data
        (++) LCD_WR_DATA() to write word data
        (++) LCD_Address_Set() to set drawing area coordinates

  @endverbatim
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "lcd_init.h"
#include "stm32g0xx_hal.h"
#include "stm32g0xx_hal_spi.h"

/** @addtogroup ST7735_Driver
  * @{
  */

/** @defgroup LCD_INIT LCD Initialization
  * @brief ST7735 LCD initialization module driver
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup LCD_INIT_Exported_Functions LCD Init Exported Functions
  * @{
  */

/** @defgroup LCD_INIT_Exported_Functions_Group1 Initialization functions
  * @brief    Initialization functions
  * @{
  */

/**
  * @brief  Initialize GPIO pins for LCD interface
  * @note   This function configures GPIO pins as output push-pull for LCD control
  * @retval None
  */
void LCD_GPIO_Init(void)
{
  LCD_BL_CLR();
  LCD_DC_CLR();
  LCD_RST_CLR();
  HAL_Delay(100);
  LCD_RST_SET();
  LCD_CS_SET();
  HAL_Delay(100);
}

/**
  * @brief  Initialize ST7735 LCD controller
  * @note   This function performs complete initialization sequence for ST7735
  * @retval None
  */
void LCD_Init(void)
{
  /* Initialize GPIO pins */
  LCD_GPIO_Init();
  
  /* Hardware reset sequence */
  LCD_RST_CLR();
  HAL_Delay(100);
  LCD_RST_SET();
  HAL_Delay(100);
  
  /* Enable backlight */
  LCD_BL_SET();
  HAL_Delay(100);
  
  /* Start Initial Sequence */
  LCD_WR_REG(0x11); /* Sleep out */
  HAL_Delay(120);    /* Delay 120ms */
  
  /* ST7735S Frame Rate configuration */
  LCD_WR_REG(0xB1); 
  LCD_WR_DATA8(0x05); 
  LCD_WR_DATA8(0x3C); 
  LCD_WR_DATA8(0x3C); 
  LCD_WR_REG(0xB2); 
  LCD_WR_DATA8(0x05);
  LCD_WR_DATA8(0x3C); 
  LCD_WR_DATA8(0x3C); 
  LCD_WR_REG(0xB3); 
  LCD_WR_DATA8(0x05); 
  LCD_WR_DATA8(0x3C); 
  LCD_WR_DATA8(0x3C); 
  LCD_WR_DATA8(0x05); 
  LCD_WR_DATA8(0x3C); 
  LCD_WR_DATA8(0x3C); 
  
  /* Dot inversion */
  LCD_WR_REG(0xB4);
  LCD_WR_DATA8(0x03); 
  
  /* ST7735S Power Sequence */
  LCD_WR_REG(0xC0); 
  LCD_WR_DATA8(0x28); 
  LCD_WR_DATA8(0x08); 
  LCD_WR_DATA8(0x04); 
  LCD_WR_REG(0xC1); 
  LCD_WR_DATA8(0XC0); 
  LCD_WR_REG(0xC2); 
  LCD_WR_DATA8(0x0D); 
  LCD_WR_DATA8(0x00); 
  LCD_WR_REG(0xC3); 
  LCD_WR_DATA8(0x8D); 
  LCD_WR_DATA8(0x2A); 
  LCD_WR_REG(0xC4); 
  LCD_WR_DATA8(0x8D); 
  LCD_WR_DATA8(0xEE); 
  
  /* VCOM configuration */
  LCD_WR_REG(0xC5);
  LCD_WR_DATA8(0x1A); 
  
  /* Memory access control - orientation setting */
  LCD_WR_REG(0x36);
  if(USE_HORIZONTAL == LCD_ORIENTATION_PORTRAIT_1)
    LCD_WR_DATA8(0x00);
  else if(USE_HORIZONTAL == LCD_ORIENTATION_PORTRAIT_2)
    LCD_WR_DATA8(0xC0);
  else if(USE_HORIZONTAL == LCD_ORIENTATION_LANDSCAPE_1)
    LCD_WR_DATA8(0x70);
  else 
    LCD_WR_DATA8(0xA0); 
  
  /* ST7735S Gamma Sequence */
  LCD_WR_REG(0xE0); 
  LCD_WR_DATA8(0x04); 
  LCD_WR_DATA8(0x22); 
  LCD_WR_DATA8(0x07); 
  LCD_WR_DATA8(0x0A); 
  LCD_WR_DATA8(0x2E); 
  LCD_WR_DATA8(0x30); 
  LCD_WR_DATA8(0x25); 
  LCD_WR_DATA8(0x2A); 
  LCD_WR_DATA8(0x28); 
  LCD_WR_DATA8(0x26); 
  LCD_WR_DATA8(0x2E); 
  LCD_WR_DATA8(0x3A); 
  LCD_WR_DATA8(0x00); 
  LCD_WR_DATA8(0x01); 
  LCD_WR_DATA8(0x03); 
  LCD_WR_DATA8(0x13); 
  LCD_WR_REG(0xE1); 
  LCD_WR_DATA8(0x04); 
  LCD_WR_DATA8(0x16); 
  LCD_WR_DATA8(0x06); 
  LCD_WR_DATA8(0x0D); 
  LCD_WR_DATA8(0x2D); 
  LCD_WR_DATA8(0x26); 
  LCD_WR_DATA8(0x23); 
  LCD_WR_DATA8(0x27); 
  LCD_WR_DATA8(0x27); 
  LCD_WR_DATA8(0x25); 
  LCD_WR_DATA8(0x2D); 
  LCD_WR_DATA8(0x3B); 
  LCD_WR_DATA8(0x00); 
  LCD_WR_DATA8(0x01); 
  LCD_WR_DATA8(0x04); 
  LCD_WR_DATA8(0x13); 
  
  /* Color mode - 65K mode */
  LCD_WR_REG(0x3A);
  LCD_WR_DATA8(0x05); 
  
  /* Display on */
  LCD_WR_REG(0x29);
}

/**
  * @}
  */

/** @defgroup LCD_INIT_Exported_Functions_Group2 Low level functions
  * @brief    Low level communication functions
  * @{
  */

/**
  * @brief  Write single byte data to LCD
  * @param  dat: Data byte to write
  * @retval None
  */
void LCD_WR_DATA8(uint8_t dat)
{
  LCD_CS_CLR();
  HAL_SPI_Transmit_IT(&hspi1, &dat, 1);
  LCD_CS_SET();	
}

/**
  * @brief  Write 16-bit data to LCD
  * @param  dat: 16-bit data to write
  * @retval None
  */
void LCD_WR_DATA(uint16_t dat)
{
  uint8_t data[2];
  data[0] = (uint8_t)(dat >> 8);    /* High byte first (MSB) */
  data[1] = (uint8_t)(dat & 0xFF);  /* Low byte second (LSB) */
  
  LCD_CS_CLR();
  HAL_SPI_Transmit_IT(&hspi1, data, 2);
  LCD_CS_SET();
}

/**
  * @brief  Write 32-bit data to LCD
  * @param  dat: 32-bit data to write
  * @retval None
  */
void LCD_WR_DATA32(uint32_t dat)
{
  uint8_t data[4];
  data[0] = (uint8_t)(dat >> 24);        /* Byte 3 (MSB) */
  data[1] = (uint8_t)((dat >> 16) & 0xFF); /* Byte 2 */
  data[2] = (uint8_t)((dat >> 8) & 0xFF);  /* Byte 1 */
  data[3] = (uint8_t)(dat & 0xFF);        /* Byte 0 (LSB) */
  
  LCD_CS_CLR();
  HAL_SPI_Transmit_IT(&hspi1, data, 4);
  LCD_CS_SET();
}

/**
  * @brief  Write command/register to LCD
  * @param  dat: Command byte to write
  * @retval None
  */
void LCD_WR_REG(uint8_t dat)
{
  LCD_DC_CLR();  /* Command mode */
  LCD_CS_CLR();
  HAL_SPI_Transmit_IT(&hspi1, &dat, 1);
  LCD_CS_SET();	
  LCD_DC_SET();  /* Data mode */
}

/**
  * @brief  Set drawing area coordinates
  * @param  x1: Start X coordinate
  * @param  y1: Start Y coordinate
  * @param  x2: End X coordinate
  * @param  y2: End Y coordinate
  * @retval None
  */
void LCD_Address_Set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
  LCD_WR_REG(0x2A); /* Column address set */
  LCD_WR_DATA32((x1+1)<<16|(x2+1)); // For 32-bit data write
  LCD_WR_REG(0x2B); /* Row address set */
  LCD_WR_DATA32((y1+3)<<16|(y2+3)); // For 32-bit data write
  LCD_WR_REG(0x2C); /* Memory write */
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
