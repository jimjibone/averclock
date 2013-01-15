// to use this, uncomment and install a 10K LDR potential divider on A5 to ground
#define AUTO_BRIGHTNESS
#define BRIGHTNESS_THRESH_LIGHT 250
#define BRIGHTNESS_THRESH_DARK  300

#define TOGGLE_COLON

// 0->255 == bright->dim
#define DISP_DIMMEST   120
#define DISP_BRIGHTEST 5

#define LDR_PIN A5

#define HEATBEAT_LED

// display ADC reading
//#define DISPLAY_ADC

// set time from PC?
#define AUTO_TIME

// tick interval in ms, 1-20 recommended, 260 max
#define TICK 10


// 1 second intervals to update clock
#define HEARTBEAT_PERIOD          (1000/TICK)

#define UPDATE_BRIGHTNESS_PERIOD  (100/TICK)

#define DISPLAY_ADC_PERIOD        (400/TICK)

// display SPI slave select pin
#define DISP_SS 10

// tick duty cycle on pin 8?
#define DEBUG
