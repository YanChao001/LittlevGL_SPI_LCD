#include "lvgl_app.h"

void lvgl_app_create(void)
{
	lv_obj_t* scr = lv_obj_create(NULL, NULL);
	lv_scr_load(scr);

	lv_obj_t* btn1 = lv_btn_create(scr, NULL);
	//lv_obj_set_pos(btn1, 120, 120);
	//lv_obj_set_size(btn1, 20, 20);
	lv_btn_set_fit(btn1, true);
}
