// TODO general scheduler addTask(pointer,interval,offset)
// and better with a queue and priorities?

#include "current_time.h"

// tick interval in ms, 1-20 recommended, 260 max
#define TICK 20

// 1 second intervals to update clock
#define HEARTBEAT_PERIOD (1000/TICK)

volatile int heartbeat_count = 0;


typedef struct {
	char hours;
	char minutes;
	char seconds;
} elapsed;

// initialise with compile-time time
elapsed time = {HOURS,MINUTES,SECONDS};

// tasks
void inc_time (void);
void update_display (void);


void setup () {
	// enable interrupts (match A, too)
	noInterrupts();
	TCCR1A = 0;
	TCCR1B = 0;
	TCNT1  = 0;

	// set up timer1 @ 16MHz to call ISR every TICK ms
	// will clobber servo lib and pwm 9 and 10

	// TODO: A and B or what????

	// SYS clock @ 16Mhz, Prescaler at 64, 16 bit timer1
	// so 250 per ms
	OCR1A = 250*TICK;

	// CTC mode (clear timer on compare match
	TCCR1B |= (1 << WGM12);

	// 64 prescaler
	TCCR1B |= (1 << CS11);
	TCCR1B |= (1 << CS10);

	// enable timer compare interrupt
	TIMSK1 |= (1 << OCIE1A);

	interrupts();

	update_display();
}

void loop () {
	// standby mode with timer1 active and CPU off
	SMCR |= 0b1101;
}

// Interrupt service routine, scheduler
ISR(TIMER1_COMPA_vect) {
	if (heartbeat_count++ == HEARTBEAT_PERIOD) {
		heartbeat_count = 0;
		inc_time();
	}

	// TODO: reset interrupt flag!
}

// call at 1Hz
void inc_time (void) {
	if (time.seconds++ == 60) {
		time.minutes++;
		time.seconds = 0;
	}

	if (time.minutes++ == 60) {
		time.hours++;
		time.minutes = 0;
	}

	if (time.hours == 24)
		time.hours = 0;
}

void update_display (void) {

}
