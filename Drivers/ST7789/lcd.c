/**
  ******************************************************************************
  * @file    lcd.c
  * @author  Songqin Cheng
  * @brief   ST7735 LCD display module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of the ST7735 LCD display:
  *           + Drawing and graphics operations
  *           + Text and character display
  *           + Chinese character display support
  *           + Number and float display
  *           + Image display functionality
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
                    ##### ST7735 LCD Display Driver features #####
  ==============================================================================
  [..]
    (+) The LCD driver provides comprehensive display functionality including:
        (++) Basic drawing operations (points, lines, curves, rectangles,
circles)
        (++) Text display with multiple font sizes
        (++) Chinese character support with multiple sizes
        (++) Number and floating point display
        (++) Image and picture display support

    (+) Drawing features include:
        (++) Fill areas with specified colors
        (++) Draw individual pixels
        (++) Draw lines using Bresenham algorithm
        (++) Draw rectangles and circles
        (++) Support for 16-bit RGB565 color format

    (+) Text display features include:
        (++) Multiple font sizes (12x12, 16x16, 24x24, 32x32)
        (++) Foreground and background color control
        (++) Overlay and normal display modes
        (++) Chinese character support

                     ##### How to use this driver #####
  ==============================================================================
  [..]
    (#) Initialize the LCD using LCD_Init() from lcd_init module.

    (#) Use drawing functions to create graphics:
        (++) LCD_Fill() to fill rectangular areas
        (++) LCD_DrawPoint() to draw individual pixels
        (++) LCD_DrawLine() to draw lines between two points
        (++) LCD_DrawCurve() to draw curves based on data points
        (++) LCD_DrawRectangle() to draw rectangle outlines
        (++) LCD_DrawCircle() to draw circles

    (#) Use text display functions:
        (++) LCD_ShowChar() to display single characters
        (++) LCD_ShowString() to display text strings
        (++) LCD_ShowChinese() to display Chinese characters

    (#) Use number display functions:
        (++) LCD_ShowIntNum() to display integers
        (++) LCD_ShowFloatNum1() to display floating point numbers

    (#) Use LCD_ShowPicture() to display bitmap images.

  @endverbatim
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "lcd.h"
#include "lcd_init.h"
#include "lcdfont.h"
#include <stdint.h>

/** @addtogroup ST7735_Driver
  * @{
  */

/** @defgroup LCD LCD Display
  * @brief ST7735 LCD display module driver
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup LCD_Exported_Functions LCD Exported Functions
 * @{
 */
void LCD_FindMaxMin(uint16_t *data, uint16_t length, uint16_t *max, uint16_t *min)
{
  uint16_t maxval = 0;
  uint16_t minval = 0xFFFF;
  uint16_t i = 0;

  for (i = 0; i < length; i++) {
    if (data[i] > maxval) {
      maxval = data[i];
    }
    if (data[i] < minval) {
      minval = data[i];
    }
  }

  *max = maxval;
  *min = minval;
}
/**
  * @}
  */

/** @defgroup LCD_Exported_Functions_Group1 Drawing functions
  * @brief    Basic drawing functions
  * @{
  */

/**
  * @brief  Fill rectangular area with specified color
  * @param  xsta: Start X coordinate
  * @param  ysta: Start Y coordinate
  * @param  xend: End X coordinate
  * @param  yend: End Y coordinate
  * @param  color: Fill color in RGB565 format
  * @retval None
  */
void LCD_Fill(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend, uint16_t color)
{          
  uint16_t i, j;

  LCD_Address_Set(xsta, ysta, xend - 1, yend - 1);
  for(i = ysta; i < yend; i++)
  {													   	 	
    for(j = xsta; j < xend; j++)
    {
      LCD_WR_DATA(color);
    }
  } 					  	    
}

/**
  * @brief  Draw a point at specified coordinates
  * @param  x: X coordinate
  * @param  y: Y coordinate
  * @param  color: Point color in RGB565 format
  * @retval None
  */
void LCD_DrawPoint(uint16_t x, uint16_t y, uint16_t color)
{
  LCD_Address_Set(x, y, x, y);
  LCD_WR_DATA(color);
} 

/**
  * @brief  Draw a line between two points using Bresenham algorithm
  * @param  x1: Start X coordinate
  * @param  y1: Start Y coordinate
  * @param  x2: End X coordinate
  * @param  y2: End Y coordinate
  * @param  color: Line color in RGB565 format
  * @retval None
  */
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
  uint16_t t; 
  uint16_t xerr = 0, yerr = 0, delta_x, delta_y, distance;
  int16_t incx, incy;
  uint16_t uRow, uCol;
  
  int16_t dx = (int16_t)x2 - (int16_t)x1;
  int16_t dy = (int16_t)y2 - (int16_t)y1;
  
  uRow = x1;
  uCol = y1;
  
  if(dx > 0) {
    incx = 1;
    delta_x = (uint16_t)dx;
  }
  else if (dx == 0) {
    incx = 0;
    delta_x = 0;
  }
  else {
    incx = -1;
    delta_x = (uint16_t)(-dx);
  }

  if(dy > 0) {
    incy = 1;
    delta_y = (uint16_t)dy;
  }
  else if (dy == 0) {
    incy = 0;
    delta_y = 0;
  }
  else {
    incy = -1;
    delta_y = (uint16_t)(-dy);
  }
  
  if(delta_x > delta_y)
    distance = delta_x;
  else 
    distance = delta_y;
    
  for(t = 0; t < distance + 1; t++)
  {
    LCD_DrawPoint(uRow, uCol, color);
    xerr += delta_x;
    yerr += delta_y;
    if(xerr > distance)
    {
      xerr -= distance;
      uRow = (uint16_t)((int16_t)uRow + incx);
    }
    if(yerr > distance)
    {
      yerr -= distance;
      uCol = (uint16_t)((int16_t)uCol + incy);
    }
  }
}

/**
  * @brief  Draw a curve based on provided data points
  * @param  x1: Start X coordinate
  * @param  y1: Start Y coordinate
  * @param  x2: End X coordinate
  * @param  y2: End Y coordinate
  * @param  data: Pointer to array of data points
  * @param  datalength: Number of data points in the array
  * @param  color: Curve color in RGB565 format
  * @retval None
  */
void LCD_DrawStaticCurve(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
                   uint16_t *data, uint16_t datalength, uint16_t color) {
  uint32_t i = 0;
  uint32_t j = 0;
  uint32_t thisx = 0, thisy = 0;
  uint32_t prevx = 0, prevy = 0;
  uint32_t displaylength = 0;
  uint32_t displayheight = 0;
  uint32_t datarange = 0;
  uint32_t datamax = 0, datamin = 0;
  uint16_t columndata[64] = {0};
  uint32_t columndatastart = 0;
  uint32_t columndataend = 0;
  uint32_t columndatamax = 0;
  uint32_t columndatamin = 0;

  displaylength = x2 - x1;
  displayheight = y2 - y1;
  LCD_FindMaxMin(data, datalength, &datamax, &datamin);
  datarange = datamax - datamin;

  if (datalength > displaylength) {
    for(i = 0; i < displaylength; i++) {
      columndatastart = i * datalength / displaylength;
      columndataend = (i + 1) * datalength / displaylength;
      for(j = 0; j < columndataend - columndatastart; j++) {
        columndata[j] = data[columndatastart + j];
      }
      LCD_FindMaxMin(columndata, columndataend - columndatastart, &columndatamax, &columndatamin);
      LCD_DrawLine(
          x1 + i, y2 - (columndatamin - datamin) * displayheight / datarange,
          x1 + i, y2 - (columndatamax - datamin) * displayheight / datarange,
          color);
    }
  } else {
    for (i = 0; i < datalength; i++) {
      thisx = x1 + i * displaylength / datalength;
      thisy = y2 - (data[i] - datamin) * displayheight / datarange;
      if (i == 0) {
        prevx = thisx;
        prevy = thisy;
        LCD_DrawPoint(thisx, thisy, color);
      } else {
        LCD_DrawLine(prevx, prevy, thisx, thisy, color);
        prevx = thisx;
        prevy = thisy;
      }
    }
  }
}

void LCD_DrawRollingCurve(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
                          uint16_t newdata, uint16_t color, RollingCurveState *state) {
  if (!state->initialized) {
    state->thisx = 0;
    state->data_count = 0;
    state->prev_max_data = newdata + (newdata >> 3);
    state->prev_min_data = (newdata > (newdata >> 3)) ? (newdata - (newdata >> 3)) : 0;
    state->max_data = newdata;
    state->min_data = newdata;
    state->first_screen = 1;
    state->initialized = 1;
  }

  uint16_t displayheight = y2 - y1;
  uint16_t displaylength = x2 - x1;

  if (newdata > state->max_data) state->max_data = newdata;
  if (newdata < state->min_data) state->min_data = newdata;

  state->data_count++;

  if (state->thisx < displaylength) {
    uint16_t current_x = x1 + state->thisx;
    uint16_t range, y_pos;
    uint32_t display_min, display_max;
    
    display_max = state->prev_max_data;
    display_min = state->prev_min_data;
    range = (display_max > display_min) ? (display_max - display_min) : 1;
    
    if (newdata >= display_min && newdata <= display_max) {
      y_pos = y2 - ((uint32_t)(newdata - display_min) * displayheight) / range;
    } else if (newdata > display_max) {
      y_pos = y1;
    } else {
      y_pos = y2;
    }
    
    if (y_pos < y1) y_pos = y1;
    if (y_pos > y2) y_pos = y2;

    LCD_Fill(current_x, y1, current_x + 1, y2, LCD_COLOR_BLACK);
    LCD_DrawPoint(current_x, y_pos, color);

    state->thisx++;
  } else {
    uint32_t current_range, margin;
    uint16_t new_max, new_min;
    
    current_range = (state->max_data > state->min_data) ? 
                    (state->max_data - state->min_data) : 1;
    margin = (current_range >> 2) + 1;
    
    new_max = (state->max_data < (0xFFFF - margin)) ? 
              (state->max_data + margin) : 0xFFFF;
    new_min = (state->min_data > margin) ? 
              (state->min_data - margin) : 0;
    
    state->thisx = 0;
    state->prev_max_data = new_max;
    state->prev_min_data = new_min;
    state->max_data = newdata;
    state->min_data = newdata;
    state->data_count = 0;
    state->first_screen = 0;
  }
}

// data range : 0-maxdata
void LCD_DrawRollingBarGraph(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
                             uint16_t newdata_l, uint16_t newdata_h, uint16_t maxdata,
                             uint16_t color, RollingCurveState *state) {
  if (!state->initialized) {
    state->thisx = 0;
    state->data_count = 0;
    state->first_screen = 1;
    state->initialized = 1;
  }

  uint16_t displayheight = y2 - y1;
  uint16_t displaylength = x2 - x1;

  state->data_count++;

  if (state->thisx < displaylength) {
    uint16_t current_x = x1 + state->thisx;
    uint16_t y_pos_h, y_pos_l;

    y_pos_h = y2 - ((uint32_t)(newdata_h) * displayheight) / maxdata;
    y_pos_l = y2 - ((uint32_t)(newdata_l) * displayheight) / maxdata;

    if (y_pos_h < y1) y_pos_h = y1;
    if (y_pos_h > y2) y_pos_h = y2;

    if (y_pos_l < y1) y_pos_l = y1;
    if (y_pos_l > y2) y_pos_l = y2;

    LCD_Fill(current_x, y1, current_x + 1, y2, LCD_COLOR_BLACK);
    LCD_Fill(current_x, y_pos_h, current_x + 1, y_pos_l, color);

    state->thisx++;

  } else {
    state->thisx = 0;
    state->data_count = 0;
    state->first_screen = 0;
  }
}


/**
  * @brief  Draw a rectangle outline
  * @param  x1: Start X coordinate
  * @param  y1: Start Y coordinate
  * @param  x2: End X coordinate
  * @param  y2: End Y coordinate
  * @param  color: Rectangle color in RGB565 format
  * @retval None
  */
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
  LCD_DrawLine(x1, y1, x2, y1, color);
  LCD_DrawLine(x1, y1, x1, y2, color);
  LCD_DrawLine(x1, y2, x2, y2, color);
  LCD_DrawLine(x2, y1, x2, y2, color);
}

/**
  * @brief  Draw a circle using midpoint circle algorithm
  * @param  x0: Center X coordinate
  * @param  y0: Center Y coordinate
  * @param  r: Circle radius
  * @param  color: Circle color in RGB565 format
  * @retval None
  */
void LCD_DrawCircle(uint16_t x0, uint16_t y0, uint8_t r, uint16_t color)
{
  uint16_t a, b;
  a = 0;
  b = r;
  
  while(a <= b)
  {
    LCD_DrawPoint(x0 - b, y0 - a, color);
    LCD_DrawPoint(x0 + b, y0 - a, color);
    LCD_DrawPoint(x0 - a, y0 + b, color);
    LCD_DrawPoint(x0 - a, y0 - b, color);
    LCD_DrawPoint(x0 + b, y0 + a, color);
    LCD_DrawPoint(x0 + a, y0 - b, color);
    LCD_DrawPoint(x0 + a, y0 + b, color);
    LCD_DrawPoint(x0 - b, y0 + a, color);
    a++;
    if((a * a + b * b) > (r * r))
    {
      b--;
    }
  }
}

/**
  * @}
  */

/** @defgroup LCD_Exported_Functions_Group2 Text display functions
  * @brief    Text and character display functions
  * @{
  */

/**
  * @brief  Display Chinese characters with specified font size
  * @param  x: X coordinate
  * @param  y: Y coordinate
  * @param  s: Pointer to Chinese character string
  * @param  fc: Foreground color
  * @param  bc: Background color
  * @param  sizey: Font size (12, 16, 24, or 32)
  * @param  mode: Display mode (0: overlay, 1: normal)
  * @retval None
  */
void LCD_ShowChinese(uint16_t x, uint16_t y, uint8_t *s, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode)
{
  while(*s != 0)
  {
    if(sizey == LCD_FONT_SIZE_12) 
      LCD_ShowChinese12x12(x, y, s, fc, bc, sizey, mode);
    else if(sizey == LCD_FONT_SIZE_16) 
      LCD_ShowChinese16x16(x, y, s, fc, bc, sizey, mode);
    else if(sizey == LCD_FONT_SIZE_24) 
      LCD_ShowChinese24x24(x, y, s, fc, bc, sizey, mode);
    else if(sizey == LCD_FONT_SIZE_32) 
      LCD_ShowChinese32x32(x, y, s, fc, bc, sizey, mode);
    else 
      return;
    s += 2;
    x += sizey;
  }
}

/**
  * @brief  Display Chinese characters with 12x12 font
  * @param  x: X coordinate
  * @param  y: Y coordinate
  * @param  s: Pointer to Chinese character string
  * @param  fc: Foreground color
  * @param  bc: Background color
  * @param  sizey: Font size
  * @param  mode: Display mode (0: overlay, 1: normal)
  * @retval None
  */
void LCD_ShowChinese12x12(uint16_t x, uint16_t y, uint8_t *s, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode)
{
  uint8_t i, j, m = 0;
  uint16_t k;
  uint16_t HZnum;
  uint16_t TypefaceNum;
  uint16_t x0 = x;
  TypefaceNum = (sizey / 8 + ((sizey % 8) ? 1 : 0)) * sizey;
						   
  HZnum = sizeof(tfont12) / sizeof(typFNT_GB12);
  for(k = 0; k < HZnum; k++) 
  {
    if((tfont12[k].Index[0] == *(s)) && (tfont12[k].Index[1] == *(s + 1)))
    { 	
      LCD_Address_Set(x, y, x + sizey - 1, y + sizey - 1);
      for(i = 0; i < TypefaceNum; i++)
      {
        for(j = 0; j < 8; j++)
        {	
          if(!mode)
          {
            if(tfont12[k].Msk[i] & (0x01 << j))
              LCD_WR_DATA(fc);
            else 
              LCD_WR_DATA(bc);
            m++;
            if(m % sizey == 0)
            {
              m = 0;
              break;
            }
          }
          else
          {
            if(tfont12[k].Msk[i] & (0x01 << j))	
              LCD_DrawPoint(x, y, fc);
            x++;
            if((x - x0) == sizey)
            {
              x = x0;
              y++;
              break;
            }
          }
        }
      }
    }				  	
    continue;
  }
} 

/**
  * @brief  Display Chinese characters with 16x16 font
  * @param  x: X coordinate
  * @param  y: Y coordinate
  * @param  s: Pointer to Chinese character string
  * @param  fc: Foreground color
  * @param  bc: Background color
  * @param  sizey: Font size
  * @param  mode: Display mode (0: overlay, 1: normal)
  * @retval None
  */
void LCD_ShowChinese16x16(uint16_t x, uint16_t y, uint8_t *s, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode)
{
  uint8_t i, j, m = 0;
  uint16_t k;
  uint16_t HZnum;
  uint16_t TypefaceNum;
  uint16_t x0 = x;
  TypefaceNum = (sizey / 8 + ((sizey % 8) ? 1 : 0)) * sizey;
  HZnum = sizeof(tfont16) / sizeof(typFNT_GB16);
  for(k = 0; k < HZnum; k++) 
  {
    if((tfont16[k].Index[0] == *(s)) && (tfont16[k].Index[1] == *(s + 1)))
    { 	
      LCD_Address_Set(x, y, x + sizey - 1, y + sizey - 1);
      for(i = 0; i < TypefaceNum; i++)
      {
        for(j = 0; j < 8; j++)
        {	
          if(!mode)
          {
            if(tfont16[k].Msk[i] & (0x01 << j))
              LCD_WR_DATA(fc);
            else 
              LCD_WR_DATA(bc);
            m++;
            if(m % sizey == 0)
            {
              m = 0;
              break;
            }
          }
          else
          {
            if(tfont16[k].Msk[i] & (0x01 << j))	
              LCD_DrawPoint(x, y, fc);
            x++;
            if((x - x0) == sizey)
            {
              x = x0;
              y++;
              break;
            }
          }
        }
      }
    }				  	
    continue;
  }
} 

/**
  * @brief  Display Chinese characters with 24x24 font
  * @param  x: X coordinate
  * @param  y: Y coordinate
  * @param  s: Pointer to Chinese character string
  * @param  fc: Foreground color
  * @param  bc: Background color
  * @param  sizey: Font size
  * @param  mode: Display mode (0: overlay, 1: normal)
  * @retval None
  */
void LCD_ShowChinese24x24(uint16_t x, uint16_t y, uint8_t *s, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode)
{
  uint8_t i, j, m = 0;
  uint16_t k;
  uint16_t HZnum;
  uint16_t TypefaceNum;
  uint16_t x0 = x;
  TypefaceNum = (sizey / 8 + ((sizey % 8) ? 1 : 0)) * sizey;
  HZnum = sizeof(tfont24) / sizeof(typFNT_GB24);
  for(k = 0; k < HZnum; k++) 
  {
    if((tfont24[k].Index[0] == *(s)) && (tfont24[k].Index[1] == *(s + 1)))
    { 	
      LCD_Address_Set(x, y, x + sizey - 1, y + sizey - 1);
      for(i = 0; i < TypefaceNum; i++)
      {
        for(j = 0; j < 8; j++)
        {	
          if(!mode)
          {
            if(tfont24[k].Msk[i] & (0x01 << j))
              LCD_WR_DATA(fc);
            else 
              LCD_WR_DATA(bc);
            m++;
            if(m % sizey == 0)
            {
              m = 0;
              break;
            }
          }
          else
          {
            if(tfont24[k].Msk[i] & (0x01 << j))	
              LCD_DrawPoint(x, y, fc);
            x++;
            if((x - x0) == sizey)
            {
              x = x0;
              y++;
              break;
            }
          }
        }
      }
    }				  	
    continue;
  }
} 

/**
  * @brief  Display Chinese characters with 32x32 font
  * @param  x: X coordinate
  * @param  y: Y coordinate
  * @param  s: Pointer to Chinese character string
  * @param  fc: Foreground color
  * @param  bc: Background color
  * @param  sizey: Font size
  * @param  mode: Display mode (0: overlay, 1: normal)
  * @retval None
  */
void LCD_ShowChinese32x32(uint16_t x, uint16_t y, uint8_t *s, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode)
{
  uint8_t i, j, m = 0;
  uint16_t k;
  uint16_t HZnum;
  uint16_t TypefaceNum;
  uint16_t x0 = x;
  TypefaceNum = (sizey / 8 + ((sizey % 8) ? 1 : 0)) * sizey;
  HZnum = sizeof(tfont32) / sizeof(typFNT_GB32);
  for(k = 0; k < HZnum; k++) 
  {
    if((tfont32[k].Index[0] == *(s)) && (tfont32[k].Index[1] == *(s + 1)))
    { 	
      LCD_Address_Set(x, y, x + sizey - 1, y + sizey - 1);
      for(i = 0; i < TypefaceNum; i++)
      {
        for(j = 0; j < 8; j++)
        {	
          if(!mode)
          {
            if(tfont32[k].Msk[i] & (0x01 << j))
              LCD_WR_DATA(fc);
            else 
              LCD_WR_DATA(bc);
            m++;
            if(m % sizey == 0)
            {
              m = 0;
              break;
            }
          }
          else
          {
            if(tfont32[k].Msk[i] & (0x01 << j))	
              LCD_DrawPoint(x, y, fc);
            x++;
            if((x - x0) == sizey)
            {
              x = x0;
              y++;
              break;
            }
          }
        }
      }
    }				  	
    continue;
  }
}

/**
  * @brief  Display a single character
  * @param  x: X coordinate
  * @param  y: Y coordinate
  * @param  num: Character to display
  * @param  fc: Foreground color
  * @param  bc: Background color
  * @param  sizey: Font size
  * @param  mode: Display mode (0: overlay, 1: normal)
  * @retval None
  */
void LCD_ShowChar(uint16_t x, uint16_t y, uint8_t num, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode)
{
  uint8_t temp, sizex, t, m = 0;
  uint16_t i, TypefaceNum;
  uint16_t x0 = x;
  sizex = sizey / 2;
  TypefaceNum = (sizex / 8 + ((sizex % 8) ? 1 : 0)) * sizey;
  num = num - ' ';
  LCD_Address_Set(x, y, x + sizex - 1, y + sizey - 1);
  for(i = 0; i < TypefaceNum; i++)
  {
    if(sizey == 16)
      temp = ascii_1608[num][i];
    else if(sizey == 24)
      temp = ascii_2412[num][i];
    else if(sizey == 32)
      temp = ascii_3216[num][i];
    else
      return;
      
    for(t = 0; t < 8; t++)
    {
      if(!mode)
      {
        if(temp & (0x01 << t))
          LCD_WR_DATA(fc);
        else 
          LCD_WR_DATA(bc);
        m++;
        if(m % sizex == 0)
        {
          m = 0;
          break;
        }
      }
      else
      {
        if(temp & (0x01 << t))
          LCD_DrawPoint(x, y, fc);
        x++;
        if((x - x0) == sizex)
        {
          x = x0;
          y++;
          break;
        }
      }
    }
  }   	 	  
}

/**
  * @brief  Display a string of characters
  * @param  x: X coordinate
  * @param  y: Y coordinate
  * @param  p: Pointer to string
  * @param  fc: Foreground color
  * @param  bc: Background color
  * @param  sizey: Font size
  * @param  mode: Display mode (0: overlay, 1: normal)
  * @retval None
  */
void LCD_ShowString(uint16_t x, uint16_t y, const uint8_t *p, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode)
{         
  while(*p != '\0')
  {       
    LCD_ShowChar(x, y, *p, fc, bc, sizey, mode);
    x += sizey / 2;
    p++;
  }  
}

/**
  * @}
  */

/** @defgroup LCD_Exported_Functions_Group3 Number display functions
  * @brief    Number and float display functions
  * @{
  */

/**
  * @brief  Display integer number
  * @param  x: X coordinate
  * @param  y: Y coordinate
  * @param  num: Number to display
  * @param  len: Number of digits to display
  * @param  fc: Foreground color
  * @param  bc: Background color
  * @param  sizey: Font size
  * @retval None
  */
void LCD_ShowIntNum(uint16_t x, uint16_t y, uint16_t num, uint8_t len, uint16_t fc, uint16_t bc, uint8_t sizey)
{         	
  uint8_t t, temp;
  uint8_t enshow = 0;
  uint8_t sizex = sizey / 2;
  for(t = 0; t < len; t++)
  {
    temp = (num / LCD_Power(10, len - t - 1)) % 10;
    if(enshow == 0 && t < (len - 1))
    {
      if(temp == 0)
      {
        LCD_ShowChar(x + t * sizex, y, ' ', fc, bc, sizey, 0);
        continue;
      }
      else 
        enshow = 1; 
    }
    LCD_ShowChar(x + t * sizex, y, temp + 48, fc, bc, sizey, 0);
  }
} 

/**
  * @brief  Display floating point number with one decimal place
  * @param  x: X coordinate
  * @param  y: Y coordinate
  * @param  num: Floating point number to display
  * @param  len: Total number of digits to display
  * @param  fc: Foreground color
  * @param  bc: Background color
  * @param  sizey: Font size
  * @retval None
  */
void LCD_ShowFloatNum1(uint16_t x, uint16_t y, float num, uint8_t len, uint16_t fc, uint16_t bc, uint8_t sizey)
{         	
  uint8_t t, temp, sizex;
  uint16_t num1;
  sizex = sizey / 2;
  num1 = num * 100;
  for(t = 0; t < len; t++)
  {
    temp = (num1 / LCD_Power(10, len - t - 1)) % 10;
    if(t == (len - 2))
    {
      LCD_ShowChar(x + (len - 2) * sizex, y, '.', fc, bc, sizey, 0);
      t++;
      len += 1;
    }
    LCD_ShowChar(x + t * sizex, y, temp + 48, fc, bc, sizey, 0);
  }
}

/**
  * @}
  */

/** @defgroup LCD_Exported_Functions_Group4 Image display functions
  * @brief    Image and picture display functions
  * @{
  */

/**
  * @brief  Display bitmap image
  * @param  x: X coordinate
  * @param  y: Y coordinate
  * @param  length: Image width
  * @param  width: Image height
  * @param  pic: Pointer to image data array
  * @retval None
  */
void LCD_ShowPicture(uint16_t x, uint16_t y, uint16_t length, uint16_t width, const uint8_t pic[])
{
  uint16_t i, j;
  uint32_t k = 0;
  LCD_Address_Set(x, y, x + length - 1, y + width - 1);
  for(i = 0; i < length; i++)
  {
    for(j = 0; j < width; j++)
    {
      LCD_WR_DATA8(pic[k * 2]);
      LCD_WR_DATA8(pic[k * 2 + 1]);
      k++;
    }
  }			
}

/**
  * @}
  */

/** @defgroup LCD_Exported_Functions_Group5 Utility functions
  * @brief    Utility and helper functions
  * @{
  */

/**
  * @brief  Calculate power of a number
  * @param  m: Base number
  * @param  n: Exponent
  * @retval Result of m^n
  */
uint32_t LCD_Power(uint8_t m, uint8_t n)
{
  uint32_t result = 1;	 
  while(n--)
    result *= m;
  return result;
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
