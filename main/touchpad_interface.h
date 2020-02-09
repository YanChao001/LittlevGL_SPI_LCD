#ifndef _TOUCHPAD_INTERFACE_H_
#define _TOUCHPAD_INTERFACE_H_

extern uint8_t press_flag;
extern uint8_t press_x, press_y;

void touchpad_init(void);
void touchpad_get_data(void* param);

#endif // !_TOUCHPAD_INTERFACE_H_
