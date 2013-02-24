
typedef struct {
	uint8_t hours;
	uint8_t minutes;
	uint8_t seconds;
} elapsed;



void wallclock_inc_sec (void);
void wallclock_dec_sec (void);
void wallclock_inc_min (void);
void wallclock_dec_min (void);
void wallclock_inc_hour (void);
void wallclock_dec_hour (void);

#ifdef AUTO_TIME
	// initialise with compile-time time with an offset to account for build/upload time
	elapsed time = {HOURS,MINUTES,SECONDS};
#else
	elapsed time = {0,0,0};
#endif



// call at 1Hz
void wallclock_inc_sec (void) {
	if (++time.seconds == 60) {
		wallclock_inc_min();
		time.seconds = 0;
	}
}

void wallclock_inc_min (void) {
	if (++time.minutes == 60) {
		wallclock_inc_hour();
		time.minutes = 0;
	}
}
void wallclock_inc_hour (void) {
	if (++time.hours == 24)
		time.hours = 0;
}






void wallclock_dec_sec(void) {
	if (--time.seconds == 255) {
		wallclock_dec_min();
		time.seconds = 59;
	}
}

void wallclock_dec_min (void) {
	if (--time.minutes == 255) {
		wallclock_dec_hour();
		time.minutes = 59;
	}
}

void wallclock_dec_hour (void) {
	if (--time.hours == 255)
		time.hours = 23;
}
