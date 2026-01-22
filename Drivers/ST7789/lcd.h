/**
  ******************************************************************************
  * @file    lcd.h
  * @author  Songqin Cheng
  * @brief   Header file of ST7735 LCD display module.
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
#ifndef __LCD_H__
#define __LCD_H__

#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_hal.h"
#include "lcd_init.h"

/** @addtogroup ST7735_Driver
  * @{
  */

/** @defgroup LCD LCD Display
  * @brief ST7735 LCD display module driver
  * @{
  */

/* Exported types ------------------------------------------------------------*/
typedef struct {
  uint8_t initialized;
  uint16_t thisx;
  uint16_t data_count;
  uint16_t prev_max_data;
  uint16_t prev_min_data;
  uint16_t max_data;
  uint16_t min_data;
  uint8_t first_screen;
} RollingCurveState;

/* Exported constants --------------------------------------------------------*/

/** @defgroup LCD_Exported_Constants LCD Exported Constants
  * @{
  */

/** @defgroup LCD_Colors LCD Color Definitions
  * @{
  */
#define LCD_COLOR_WHITE               0xFFFFU   /*!< White color */
#define LCD_COLOR_BLACK               0x0000U   /*!< Black color */
#define LCD_COLOR_BLUE                0x001FU   /*!< Blue color */
#define LCD_COLOR_BRED                0xF81FU   /*!< Bright red color */
#define LCD_COLOR_GRED                0xFFE0U   /*!< Green-red color */
#define LCD_COLOR_GBLUE               0x07FFU   /*!< Green-blue color */
#define LCD_COLOR_RED                 0xF800U   /*!< Red color */
#define LCD_COLOR_MAGENTA             0xF81FU   /*!< Magenta color */
#define LCD_COLOR_GREEN               0x07E0U   /*!< Green color */
#define LCD_COLOR_CYAN                0x7FFFU   /*!< Cyan color */
#define LCD_COLOR_YELLOW              0xFFE0U   /*!< Yellow color */
#define LCD_COLOR_BROWN               0xBC40U   /*!< Brown color */
#define LCD_COLOR_BRRED               0xFC07U   /*!< Bright red color */
#define LCD_COLOR_GRAY                0x8430U   /*!< Gray color */
#define LCD_COLOR_DARKBLUE            0x01CFU   /*!< Dark blue color */
#define LCD_COLOR_LIGHTBLUE           0x7D7CU   /*!< Light blue color */
#define LCD_COLOR_GRAYBLUE            0x5458U   /*!< Gray blue color */
#define LCD_COLOR_LIGHTGREEN          0x841FU   /*!< Light green color */
#define LCD_COLOR_LGRAY               0xC618U   /*!< Light gray color */
#define LCD_COLOR_LGRAYBLUE           0xA651U   /*!< Light gray blue color */
#define LCD_COLOR_LBBLUE              0x2B12U   /*!< Light bright blue color */
/**
  * @}
  */

/** @defgroup LCD_Font_Sizes LCD Font Size Definitions
  * @{
  */
#define LCD_FONT_SIZE_12              12U       /*!< Font size 12x12 */
#define LCD_FONT_SIZE_16              16U       /*!< Font size 16x16 */
#define LCD_FONT_SIZE_24              24U       /*!< Font size 24x24 */
#define LCD_FONT_SIZE_32              32U       /*!< Font size 32x32 */
/**
  * @}
  */

/** @defgroup LCD_Display_Modes LCD Display Mode Definitions
  * @{
  */
#define LCD_MODE_OVERLAY              0U        /*!< Overlay mode */
#define LCD_MODE_NORMAL               1U        /*!< Normal mode */
/**
  * @}
  */

/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/

/** @defgroup LCD_Exported_Functions LCD Exported Functions
 * @brief    Basic data processing functions
 * @{
 */
void LCD_FindMaxMin(uint16_t *data, uint16_t length, uint16_t *max,
                    uint16_t *min);
/**
  * @}
  */

/** @defgroup LCD_Exported_Functions_Group1 Drawing functions
  * @brief    Basic drawing functions
  * @{
  */
void LCD_Fill(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend, uint16_t color);
void LCD_DrawPoint(uint16_t x, uint16_t y, uint16_t color);
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void LCD_DrawStaticCurve(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
                         uint16_t *data, uint16_t datalength, uint16_t color);
void LCD_DrawRollingCurve(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
                          uint16_t newdata, uint16_t color, RollingCurveState *state);
void LCD_DrawRollingBarGraph(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
                             uint16_t newdata_l, uint16_t newdata_h, uint16_t maxdata,
                             uint16_t color, RollingCurveState *state);
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void LCD_DrawCircle(uint16_t x0, uint16_t y0, uint8_t r, uint16_t color);
/**
  * @}
  */

/** @defgroup LCD_Exported_Functions_Group2 Text display functions
  * @brief    Text and character display functions
  * @{
  */
void LCD_ShowChinese(uint16_t x, uint16_t y, uint8_t *s, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode);
void LCD_ShowChinese12x12(uint16_t x, uint16_t y, uint8_t *s, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode);
void LCD_ShowChinese16x16(uint16_t x, uint16_t y, uint8_t *s, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode);
void LCD_ShowChinese24x24(uint16_t x, uint16_t y, uint8_t *s, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode);
void LCD_ShowChinese32x32(uint16_t x, uint16_t y, uint8_t *s, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode);
void LCD_ShowChar(uint16_t x, uint16_t y, uint8_t num, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode);
void LCD_ShowString(uint16_t x, uint16_t y, const uint8_t *p, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode);
/**
  * @}
  */

/** @defgroup LCD_Exported_Functions_Group3 Number display functions
  * @brief    Number and float display functions
  * @{
  */
void LCD_ShowIntNum(uint16_t x, uint16_t y, uint16_t num, uint8_t len, uint16_t fc, uint16_t bc, uint8_t sizey);
void LCD_ShowFloatNum1(uint16_t x, uint16_t y, float num, uint8_t len, uint16_t fc, uint16_t bc, uint8_t sizey);
/**
  * @}
  */

/** @defgroup LCD_Exported_Functions_Group4 Image display functions
  * @brief    Image and picture display functions
  * @{
  */
void LCD_ShowPicture(uint16_t x, uint16_t y, uint16_t length, uint16_t width, const uint8_t pic[]);
/**
  * @}
  */

/** @defgroup LCD_Exported_Functions_Group5 Utility functions
  * @brief    Utility and helper functions
  * @{
  */
uint32_t LCD_Power(uint8_t m, uint8_t n);
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

#endif /* STM32G0xx_ST7735_LCD_H */





