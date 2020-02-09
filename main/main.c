#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "lvgl.h"
#include "demo.h"
#include "lv_test_theme_1.h"
#include "lv_test_theme_2.h"

#include "lcd_interface.h"
#include "touchpad_interface.h"
#include "lvgl_interface.h"
#include "lvgl_app.h"

void app_main()
{
	lcd_init();
	touchpad_init();
	lv_init();
	lv_hal_init();
	lv_test_theme_1(lv_theme_night_init(100, NULL));
	//lv_test_theme_2();
	//demo_create();
	//lvgl_app_create();
	
	xTaskCreate(lv_tick_task, "lv_tick_task", 1024 * 2, NULL, configMAX_PRIORITIES - 1, NULL);
	xTaskCreate(lv_hander, "lv_hander", 1024 * 4, NULL, configMAX_PRIORITIES - 2, NULL);
	xTaskCreate(touchpad_get_data, "touchpad_get_data", 1024 * 2, NULL, configMAX_PRIORITIES - 3, NULL);
}
