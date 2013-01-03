// to use this, uncomment and install an LDR potential divider (high side) on A5
#define AUTO_BRIGHTNESS

// tick interval in ms, 1-20 recommended, 260 max
#define TICK 20

// 1 second intervals to update clock
#define HEARTBEAT_PERIOD          (1000/TICK)

// 40ms to update brightness (10K LDR high + 15K resistor low)
#define UPDATE_BRIGHTNESS_PERIOD  (40/TICK)

// display SPI slave select pin
#define DISP_SS 10


