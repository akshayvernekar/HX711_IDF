#include "HX711.h"
#include "esp_log.h"
#include <rom/ets_sys.h>

#define HIGH 1
#define LOW 0

static const char* DEBUGTAG = "HX711";

void HX711_init(gpio_num_t dout, gpio_num_t pd_sck, char gain)
{

	GPIO_PD_SCK = pd_sck;
	GPIO_DOUT = dout;
	GAIN = gain;

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

	void HX711_powerDown();
	void HX711_powerUp();
	
}

bool HX711_isReady()
{
	return gpio_get_level(GPIO_DOUT);
}

void HX711_setGain(char gain)
{
	GAIN = gain;
}

uint8_t HX711_shiftIn()
{
	uint8_t value = 0;

    for(int i = 0; i < 8; ++i) 
    {
        gpio_set_level(GPIO_PD_SCK, HIGH);
        ets_delay_us(5);
        value |= gpio_get_level(GPIO_DOUT) << (7 - i); //get level returns 
        gpio_set_level(GPIO_PD_SCK, LOW);
        ets_delay_us(5);
    }

    return value;
}

long HX711_readWeight()
{
	while (!HX711_isReady()) 
	{
		vTaskDelay(1/ portTICK_PERIOD_MS);
	}


	unsigned long value = 0;
	uint8_t data[3] = { 0 };
	uint8_t filler = 0x00;

	// pulse the clock pin 24 times to read the data
	data[2] = HX711_shiftIn();
	data[1] = HX711_shiftIn();
	data[0] = HX711_shiftIn();

	// set the channel and the gain factor for the next reading using the clock pin
	for (unsigned int i = 0; i < GAIN; i++) 
	{
		gpio_set_level(GPIO_PD_SCK, HIGH);
		ets_delay_us(5);
		gpio_set_level(GPIO_PD_SCK, LOW);
		ets_delay_us(5);
	}

	// Replicate the most significant bit to pad out a 32-bit signed integer
	if (data[2] & 0x80) {
		filler = 0xFF;
	} else {
		filler = 0x00;
	}

	// Construct a 32-bit signed integer
	value = ( (unsigned long)(filler) << 24
			| (unsigned long)(data[2]) << 16
			| (unsigned long)(data[1]) << 8
			| (unsigned long)(data[0]) );

return (long)(value);

}

long HX711_readAverage(char times)
{
	ESP_LOGI(DEBUGTAG, "===================== READ AVERAGE START ====================");
	long sum = 0;
	long value ;
	for (char i = 0; i < times; i++) 
	{
		value = 0;
		value = HX711_readWeight();
		ESP_LOGI(DEBUGTAG, "Itr %d , value =%ld",i,value);
		sum += value;
		vTaskDelay(20/ portTICK_PERIOD_MS);
	}
	ESP_LOGI(DEBUGTAG, "===================== READ AVERAGE END : %ld ====================",(sum / times));
	return sum / times;
}

double HX711_getValue(char times)
{

	return 0;
}

void HX711_powerDown()
{
 	gpio_set_level(GPIO_PD_SCK, HIGH);
}

void HX711_powerUp()
{
 	gpio_set_level(GPIO_PD_SCK, LOW);
}
