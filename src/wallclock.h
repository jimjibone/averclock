
typedef struct {
	char hours;
	char minutes;
	char seconds;
} elapsed;

void inc_time (void);


#ifdef AUTO_TIME
	// initialise with compile-time time with an offset to account for build/upload time
	elapsed time = {HOURS,MINUTES,SECONDS};
#else
	elapsed time = {0,0,0};
#endif



// call at 1Hz
void inc_time (void) {
	if (++time.seconds == 60) {
		time.minutes++;
		time.seconds = 0;
	}

	if (time.minutes == 60) {
		time.hours++;
		time.minutes = 0;
	}

	if (time.hours == 24)
		time.hours = 0;
}

