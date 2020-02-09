/**
 * @file ST7789.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "ST7789.h"
#if USE_ST7789

#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include LV_DRV_DISP_INCLUDE
#include LV_DRV_DELAY_INCLUDE

/*********************
 *      DEFINES
 *********************/
#define ST7789_BAUD      2000000    /*< 2,5 MHz (400 ns)*/

#define ST7789_CMD_MODE  0
#define ST7789_DATA_MODE 1

#define ST7789_HOR_RES  240
#define ST7789_VER_RES  240

#define ST7789_DIRECTION	0	/*direction*/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/**
 * Initialize the ST7789
 */
void st7789_init(void)
{
	LV_DRV_DISP_SPI_CS(0);
	
	LV_DRV_DISP_RST(0);
	LV_DRV_DELAY_MS(100);
	LV_DRV_DISP_RST(1);
	LV_DRV_DELAY_MS(100);
	
	st7789_command(0x36);
#if ST7789_DIRECTION == 0
	st7789_data_8bit(0x00);
#elif ST7789_DIRECTION == 1
	st7789_data_8bit(0xC0);
#elif ST7789_DIRECTION == 2
	st7789_data_8bit(0x70);
#else
	st7789_data_8bit(0xA0);
#endif      //ST7789_DIRECTION
	st7789_command(0x3A);
	st7789_data_8bit(0x05);
	st7789_command(0xB2);
	st7789_data_8bit(0x0C);
	st7789_data_8bit(0x0C);
	st7789_data_8bit(0x00);
	st7789_data_8bit(0x33);
	st7789_data_8bit(0x33);
	st7789_command(0xB7);
	st7789_data_8bit(0x35);
	st7789_command(0xBB);
	st7789_data_8bit(0x19);
	st7789_command(0xC0);
	st7789_data_8bit(0x2C);
	st7789_command(0xC2);
	st7789_data_8bit(0x01);
	st7789_command(0xC3);
	st7789_data_8bit(0x12);
	st7789_command(0xC4);
	st7789_data_8bit(0x20);
	st7789_command(0xC6);
	st7789_data_8bit(0x0F);
	st7789_command(0xD0);
	st7789_data_8bit(0xA4);
	st7789_data_8bit(0xA1);
	st7789_command(0xE0);
	st7789_data_8bit(0xD0);
	st7789_data_8bit(0x04);
	st7789_data_8bit(0x0D);
	st7789_data_8bit(0x11);
	st7789_data_8bit(0x13);
	st7789_data_8bit(0x2B);
	st7789_data_8bit(0x3F);
	st7789_data_8bit(0x54);
	st7789_data_8bit(0x4C);
	st7789_data_8bit(0x18);
	st7789_data_8bit(0x0D);
	st7789_data_8bit(0x0B);
	st7789_data_8bit(0x1F);
	st7789_data_8bit(0x23);
	st7789_command(0xE1);
	st7789_data_8bit(0xD0);
	st7789_data_8bit(0x04);
	st7789_data_8bit(0x0C);
	st7789_data_8bit(0x11);
	st7789_data_8bit(0x13);
	st7789_data_8bit(0x2C);
	st7789_data_8bit(0x3F);
	st7789_data_8bit(0x44);
	st7789_data_8bit(0x51);
	st7789_data_8bit(0x2F);
	st7789_data_8bit(0x1F);
	st7789_data_8bit(0x1F);
	st7789_data_8bit(0x20);
	st7789_data_8bit(0x23);
	st7789_command(0x21);
	st7789_command(0x11);
	st7789_command(0x29);
	
	//lv_color_t color;
	//color.full = 0xffff;
	//st7789_fill(0, 0, ST7789_HOR_RES, ST7789_VER_RES, color);
}

void st7789_flush(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, const lv_color_t * color_p)
{
	uint16_t x, y = 0;
	
	st7789_addr(x1, y1, x2, y2);
	for (y = y1; y <= y2; y++)
	{
		for (x = x1; x <= x2; x++)
		{
			st7789_data_16bit(color_p->full);
			color_p++;
		}
	}
}

void st7789_fill(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, lv_color_t color)
{
	uint16_t x, y;
	
	st7789_addr(x1, y1, x2, y2);
	for (y = y1; y <= y2; y++)
	{
		for (x = x1; x <= x2; x++)
		{
			st7789_data_16bit(color.full);
		}
	}
}

/**
 * Set address to the ST7789
 * @param x1 the x1
 * @param y1 the y1
 * @param x2 the x2
 * @param y2 the y2
 */
void st7789_addr(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
#if ST7789_DIRECTION == 0
	st7789_command(0x2a);
	st7789_data_16bit(x1);
	st7789_data_16bit(x2);
	st7789_command(0x2b);
	st7789_data_16bit(y1);
	st7789_data_16bit(y2);
	st7789_command(0x2c);
#elif ST7789_DIRECTION == 1
	st7789_command(0x2a);
	st7789_data_16bit(x1);
	st7789_data_16bit(x2);
	st7789_command(0x2b);
	st7789_data_16bit(y1 + 80);
	st7789_data_16bit(y2 + 80);
	st7789_command(0x2c);
#elif ST7789_DIRECTION == 2
	st7789_command(0x2a);
	st7789_data_16bit(x1);
	st7789_data_16bit(x2);
	st7789_command(0x2b);
	st7789_data_16bit(y1);
	st7789_data_16bit(y2);
	st7789_command(0x2c);
#else
	st7789_command(0x2a);
	st7789_data_16bit(x1 + 80);
	st7789_data_16bit(x2 + 80);
	st7789_command(0x2b);
	st7789_data_16bit(y1);
	st7789_data_16bit(y2);
	st7789_command(0x2c);
#endif      //LCD_DIRECTION
}

/**
 * Write a command to the ST7789
 * @param cmd the command
 */
void st7789_command(uint8_t cmd)
{
	LV_DRV_DISP_CMD_DATA(ST7789_CMD_MODE);
	LV_DRV_DISP_SPI_WR_BYTE(cmd);
}

/**
 * Write 8bit data to the ST7789
 * @param data the data
 */
void st7789_data_8bit(uint8_t data)
{
	LV_DRV_DISP_CMD_DATA(ST7789_DATA_MODE);
	LV_DRV_DISP_SPI_WR_BYTE(data);
}

/**
 * Write 16bit data to the ST7789
 * @param data the data
 */
void st7789_data_16bit(uint16_t data)
{
	LV_DRV_DISP_CMD_DATA(ST7789_DATA_MODE);
	LV_DRV_DISP_SPI_WR_BYTE(data >> 8);
	LV_DRV_DISP_SPI_WR_BYTE(data & 0xff);
}

#endif
