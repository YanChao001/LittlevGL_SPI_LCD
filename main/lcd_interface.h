#ifndef _LCD_INTERFACE_H_
#define _LCD_INTERFACE_H_

#include "../../lv_drv_conf.h"
#include "lvgl/lvgl.h"

void lcd_init(void);
void lcd_dc_pin(uint8_t data);
void lcd_rst_pin(uint8_t data);
void lcd_send_byte(uint8_t data);
void lcd_flash(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, const lv_color_t * color_p);

#endif //_LCD_INTERFACE_H_
