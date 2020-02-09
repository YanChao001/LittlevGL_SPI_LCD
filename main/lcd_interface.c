#include "driver/gpio.h"
#include "driver/spi_master.h"

#include "ST7789.h"
#include "lcd_interface.h"

/*Use IOMUX, speed up to 80MHz*/
#define PIN_NUM_MISO 12
#define PIN_NUM_MOSI 13
#define PIN_NUM_CLK  14
#define PIN_NUM_CS   15

#define PIN_NUM_DC   21
#define PIN_NUM_RST  18
#define PIN_NUM_BCKL 5

spi_device_handle_t lcd_spi;
static void lcd_spi_pre_transfer_callback(spi_transaction_t *t);

void lcd_init(void) 
{
	gpio_set_direction(PIN_NUM_DC, GPIO_MODE_OUTPUT);
	gpio_set_direction(PIN_NUM_RST, GPIO_MODE_OUTPUT);
	gpio_set_level(PIN_NUM_DC, 1);
	gpio_set_level(PIN_NUM_RST, 1);
	
	spi_bus_config_t buscfg =  {
		.miso_io_num = PIN_NUM_MISO,
		.mosi_io_num = PIN_NUM_MOSI,
		.sclk_io_num = PIN_NUM_CLK,
		.quadwp_io_num = -1,
		.quadhd_io_num = -1,
		.max_transfer_sz = 0
	};
	spi_device_interface_config_t devcfg =  {
		.clock_speed_hz = 60 * 1000 * 1000, 
		.mode = 0,
		.spics_io_num = PIN_NUM_CS, 
	    .queue_size = 7, 
		.pre_cb = lcd_spi_pre_transfer_callback, 
	};
	esp_err_t ret;
	ret = spi_bus_initialize(HSPI_HOST, &buscfg, 1);
	ESP_ERROR_CHECK(ret);
	ret = spi_bus_add_device(HSPI_HOST, &devcfg, &lcd_spi);
	ESP_ERROR_CHECK(ret);
	
	st7789_init();
}

void lcd_dc_pin(uint8_t data)
{
	gpio_set_level(PIN_NUM_DC, data);
}

void lcd_rst_pin(uint8_t data)
{
	gpio_set_level(PIN_NUM_RST, data);
}

void lcd_send_byte(uint8_t data)
{
	esp_err_t ret;
	spi_transaction_t t;
	memset(&t, 0, sizeof(t));						//Zero out the transaction
	t.length = 8;									//Command is 8 bits
	t.tx_buffer = &data;							//The data is the cmd itself
	t.user = (void*)0;								//D/C needs to be set to 0
	ret = spi_device_polling_transmit(lcd_spi, &t); 		//Transmit!
	assert(ret == ESP_OK);							//Should have had no issues.
}

void lcd_flash(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, const lv_color_t * color_p)
{
	esp_err_t ret;
	spi_transaction_t t;
	
	st7789_addr(x1, y1, x2, y2);
	gpio_set_level(PIN_NUM_DC, 1);
	memset(&t, 0, sizeof(t));   								//Zero out the transaction
	t.length = 16 * (x2 - x1 + 1) * (y2 - y1 + 1);              //Command is 8 bits
	t.tx_buffer = color_p; 										//The data is the cmd itself
	t.user = (void*)1; 											//D/C needs to be set to 0
	ret = spi_device_polling_transmit(lcd_spi, &t);  				//Transmit!
	assert(ret == ESP_OK); 										//Should have had no issues.
}

static void lcd_spi_pre_transfer_callback(spi_transaction_t *t)
{
	//int dc = (int)t->user;
	//gpio_set_level(PIN_NUM_DC, dc);
}
