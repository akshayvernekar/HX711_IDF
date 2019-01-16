#ifndef HX711_h
#define HX711_h

#define GAIN_128 128
#define GAIN_64	 64

//clock GPIO
static char GPIO_PD_SCK = GPIO_NUM_10;
static char GPIO_DOUT = GPIO_NUM_11;
static char GAIN = GAIN_128;

HX711_init(char dout, char pd_sck, char gain);

// check if HX711 is ready
// from the datasheet: When output data is not ready for retrieval, digital output pin DOUT is high. Serial clock
// input PD_SCK should be low. When DOUT goes to low, it indicates data is ready for retrieval.
bool HX711_isReady();

// set the gain factor; takes effect only after a call to read()
// channel A can be set for a 128 or 64 gain; channel B has a fixed 32 gain
// depending on the parameter, the channel is also set to either A or B
void set_gain(char gain = 128);

// waits for the chip to be ready and returns a reading
long read();

// returns an average reading; times = how many times to read
long read_average(char times = 10);


// returns (read_average() - OFFSET), that is the current value without the tare weight; times = how many readings to do
double get_value(char times = 1);

// puts the chip into power down mode
void power_down();

// wakes up the chip after power down mode
void power_up();

#endif