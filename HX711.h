#ifndef HX711_h
#define HX711_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/event_groups.h"
#include "driver/gpio.h"

#define GAIN_128 1
#define GAIN_64	 2

//clock GPIO
static gpio_num_t GPIO_PD_SCK = GPIO_NUM_10;
static gpio_num_t GPIO_DOUT = GPIO_NUM_11;
static char GAIN = GAIN_128;

void HX711_init(gpio_num_t dout, gpio_num_t pd_sck, char gain);

// check if HX711 is ready
// from the datasheet: When output data is not ready for retrieval, digital output pin DOUT is high. Serial clock
// input PD_SCK should be low. When DOUT goes to low, it indicates data is ready for retrieval.
bool HX711_isReady();

// set the gain factor; takes effect only after a call to read()
// channel A can be set for a 128 or 64 gain; channel B has a fixed 32 gain
// depending on the parameter, the channel is also set to either A or B
void HX711_setGain(char gain );

// waits for the chip to be ready and returns a reading
long HX711_readWeight();

// returns an average reading; times = how many times to read
long HX711_readAverage(char times);


// returns (read_average() - OFFSET), that is the current value without the tare weight; times = how many readings to do
double HX711_getValue(char times );

// puts the chip into power down mode
void HX711_powerDown();

// wakes up the chip after power down mode
void HX711_powerUp();

#endif
