#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "lv_conf.h"
#include "lvgl.h"

#include "ST7789.h"
#include "lcd_interface.h"
#include "touchpad_interface.h"
#include "lvgl_interface.h"

void my_lv_task(lv_task_t* param);

void my_disp_flush(lv_disp_drv_t * disp, const lv_area_t * area, lv_color_t * color_p)
{
	lcd_flash(area->x1, area->y1, area->x2, area->y2, color_p);
	lv_disp_flush_ready(disp); /* Indicate you are ready with the flushing*/
}

bool my_touchpad_read(lv_indev_drv_t * indev_driver, lv_indev_data_t * data)
{
	static lv_coord_t last_x = 0;
	static lv_coord_t last_y = 0;

	/*Save the state and save the pressed coordinate*/
	data->state = press_flag ? LV_INDEV_STATE_PR : LV_INDEV_STATE_REL; 
	if(data->state == LV_INDEV_STATE_PR)
	{
		last_x = press_x;
		last_y = press_y;
	}
	/*Set the coordinates (if released use the last pressed coordinates)*/
	data->point.x = last_x;
	data->point.y = last_y;
	return false; /*Return `false` because we are not buffering and no more data to read*/
}

void lv_tick_task(void* param)
{
	while (1) 
	{
		lv_tick_inc(10);
		vTaskDelay(10 / portTICK_PERIOD_MS);
	}
}

void lv_hander(void* param)
{
	while (1)
	{
		lv_task_handler();
		vTaskDelay(30 / portTICK_PERIOD_MS);
	}
}

lv_obj_t * img_cursor;
void lv_hal_init(void)
{
	static lv_disp_buf_t disp_buf;
	static lv_color_t buf[LV_HOR_RES_MAX * LV_VER_RES_MAX / 60];					/*Declare a buffer for 10 lines*/
	lv_disp_buf_init(&disp_buf, buf, NULL, LV_HOR_RES_MAX * LV_VER_RES_MAX / 60);	/*Initialize the display buffer*/
	
	lv_disp_drv_t disp_drv;					/*Descriptor of a display driver*/
	lv_disp_drv_init(&disp_drv);			/*Basic initialization*/
	disp_drv.flush_cb = my_disp_flush;		/*Set your driver function*/
	disp_drv.buffer = &disp_buf;			/*Assign the buffer to the display*/
	lv_disp_drv_register(&disp_drv);		/*Finally register the driver*/
	
	lv_indev_drv_t indev_drv;
	lv_indev_t *my_indev;
//	lv_obj_t * img_cursor;
	lv_indev_drv_init(&indev_drv);			/*Descriptor of a input device driver*/
	indev_drv.type = LV_INDEV_TYPE_POINTER; /*Touch pad is a pointer-like device*/
	indev_drv.read_cb = my_touchpad_read;	/*Set your driver function*/
	my_indev = lv_indev_drv_register(&indev_drv); /*Finally register the driver*/
	
	//LV_IMG_DECLARE(mouse_cursor_icon);
	//img_cursor = lv_img_create(lv_scr_act(), NULL);
	//lv_img_set_src(img_cursor, &mouse_cursor_icon); 
	img_cursor = lv_label_create(lv_disp_get_scr_act(NULL), NULL);
	lv_label_set_recolor(img_cursor, true);
	lv_label_set_text(img_cursor, "#00FF00 Up");
	lv_indev_set_cursor(my_indev, img_cursor);
	
	lv_task_create(my_lv_task, 30, LV_TASK_PRIO_MID, NULL);	/*cursor flash task*/
}

void my_lv_task(lv_task_t* param)
{
	if (press_flag == 0)
	{
		lv_label_set_text(img_cursor, "#00FF00 Up");
	}
	else if (press_flag == 1)
	{
		lv_label_set_text(img_cursor, "#FF0000 Pr");
	}
	lv_obj_set_pos(img_cursor, press_x, press_y);
}


