#include "current_time.h"


// tick interval in ms
#define TICK 20

// 1 second intervals to update clock
#define HEARTBEAT_PERIOD (1000/TICK)

int heartbeat_count = 0;


typedef struct {
	char hour    = HOURS;
	char minute  = MINUTES;
	char seconds = SECONDS;
} time;

volatile time time;

void ISR (void);
void inc_time (void);


void setup() {
	// TODO set up timer to call ISR every TICK ms
}

void loop() {
	// TODO sleep
}

// Interrupt service routine, scheduler
void ISR(void) {
	if (heartbeat_count++ == HEARTBEAT_PERIOD) {
		heartbeat_count = 0;
		inc_time();
	}

	// TODO: reset interrupt flag!
}

// call at 1Hz
void inc_time (void) {
	if (time.second++ == 60) {
		time.minute++;
		time.second = 0;
	}

	if (time.minute++ == 60) {
		time.hour++;
		time.minute = 0;
	}

	if (time.hour == 24)
		time.hour = 0;
}
