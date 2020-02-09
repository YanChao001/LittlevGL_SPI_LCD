/**
 * @file ST7789.h
 *
 */

#ifndef ST7789_H
#define ST7789_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
*      INCLUDES
*********************/
#ifndef LV_DRV_NO_CONF
#ifdef LV_CONF_INCLUDE_SIMPLE
#include "lv_drv_conf.h"
#else
#include "../../lv_drv_conf.h"
#endif
#endif

#if USE_ST7789

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

/*********************
*      DEFINES
*********************/

/**********************
*      TYPEDEFS
**********************/

/**********************
* GLOBAL PROTOTYPES
**********************/
void st7789_init(void);
void st7789_flush(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, const lv_color_t * color_p);
void st7789_fill(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, lv_color_t color);
void st7789_addr(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void st7789_command(uint8_t cmd);
void st7789_data_8bit(uint8_t data);
void st7789_data_16bit(uint16_t data);
/**********************
*      MACROS
**********************/

#endif /* USE_ST7789 */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* ST7789_H */
