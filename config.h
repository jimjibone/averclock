// to use this, uncomment and install a 10K LDR potential divider on A5 to ground
#define AUTO_BRIGHTNESS
#define BRIGHTNESS_THRESH_LIGHT 250
#define BRIGHTNESS_THRESH_DARK  300

// 0->255 == bright->dim (MUST BE MULTIPLES OF 10)
#define DISP_DIM    120
#define DISP_BRIGHT 0

// update display every second (good if ESD rapes your display)
#define AGGRESSIVE_MODE

#define LDR_PIN A5

// display ADC reading
//#define DEBUG

// set time from PC?
#define AUTO_TIME

// tick interval in ms, 1-20 recommended, 260 max
#define TICK 20

// 1 second intervals to update clock
#define HEARTBEAT_PERIOD          (1000/TICK)

// 40ms to update brightness (10K LDR high + 15K resistor low)
#define UPDATE_BRIGHTNESS_PERIOD  (100/TICK)

#define DISPLAY_ADC_PERIOD        (400/TICK)

// display SPI slave select pin
#define DISP_SS 10


