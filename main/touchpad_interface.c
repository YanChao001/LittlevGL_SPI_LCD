#include "driver/uart.h"
#include "touchpad_interface.h"

#define TXD_PIN (GPIO_NUM_4)
#define RXD_PIN (GPIO_NUM_5)
#define RX_BUF_SIZE		100

uint8_t press_flag = 0;
uint8_t press_x, press_y;

void touchpad_init(void) 
{
//	const uart_config_t uart_config = {
//		.baud_rate = 115200,
//		.data_bits = UART_DATA_8_BITS,
//		.parity = UART_PARITY_DISABLE,
//		.stop_bits = UART_STOP_BITS_1,
//		.flow_ctrl = UART_HW_FLOWCTRL_DISABLE
//	};
//	uart_param_config(UART_NUM_1, &uart_config);
//	uart_set_pin(UART_NUM_1, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
	uart_driver_install(UART_NUM_0, RX_BUF_SIZE * 2, 0, 0, NULL, 0);
}

void touchpad_get_data(void* param)
{
	uint8_t* data = (uint8_t*) malloc(RX_BUF_SIZE + 1);
	while (1)
	{
		const int rxBytes = uart_read_bytes(UART_NUM_0, data, RX_BUF_SIZE, 10 / portTICK_RATE_MS);
		if (rxBytes > 0)
		{
			//ESP_LOGE("TEST", "%d, %d, %d", data[0], data[1], data[2]);
			data[rxBytes] = 0;
			if (data[0] == 0xff)
			{
				press_flag = 1;
			}
			else if (data[0] == 0xfe)
			{
				press_flag = 0;
			}
			press_x = data[1];
			press_y = data[2];
		}
	}
	free(data);
}
