#include "freertos/FreeRTOS.h"
#include "esp_system.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "HX711.h"

#define AVG_SAMPLES   10
#define GPIO_DATA   GPIO_NUM_15
#define GPIO_SCLK   GPIO_NUM_16

static const char* TAG = "HX711_TEST";

static void weight_reading_task(void* arg);
static void initialise_weight_sensor(void);

static void weight_reading_task(void* arg)
{
    HX711_init(GPIO_DATA,GPIO_SCLK,eGAIN_128); 
    HX711_tare();

    unsigned long weight =0;
    while(1)
    {
        weight = HX711_get_units(AVG_SAMPLES);
        ESP_LOGI(TAG, "******* weight = %ld *********\n ", weight);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

static void initialise_weight_sensor(void)
{
    ESP_LOGI(TAG, "****************** Initialing weight sensor **********");
    xTaskCreatePinnedToCore(weight_reading_task, "weight_reading_task", 4096, NULL, 1, NULL,0);
}

void app_main(void)
{
    nvs_flash_init();
    initialise_weight_sensor();
}

