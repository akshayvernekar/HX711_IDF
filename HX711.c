#include "HX711.h"

HX711_init(char dout, char pd_sck, char gain)
{

	GPIO_PD_SCK = pd_sck;
	GPIO_DOUT = dout;

	gpio_config_t io_conf;
    io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = (1UL<<GPIO_PD_SCK);
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en = 0;
    gpio_config(&io_conf);

    io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
    io_conf.pin_bit_mask = (1UL<<GPIO_DOUT);
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pull_up_en = 1;
	gpio_config(&io_conf);
	
}

bool HX711_isReady()
{
	return gpio_get_level(GPIO_DOUT);
}

void set_gain(char gain)
{

}

long read()
{
	while (!is_ready()) 
	{
		// Will do nothing on Arduino but prevent resets of ESP8266 (Watchdog Issue)
		vTaskDelay(20 / portTICK_PERIOD_MS);
	}
	unsigned long value = 0;
	uint8_t data[3] = { 0 };
	uint8_t filler = 0x00;


}

long read_average(char times)
{

}

double get_value(char times = 1)
{

}

void power_down()
{
 	gpio_set_level(GPIO_PD_SCK, 1);
}

void power_up()
{
 	gpio_set_level(GPIO_PD_SCK, 0);
}