// TODO general scheduler addTask(pointer,interval,offset)
// and better with a queue and priorities?

// TODO feed-forward brightness adjusting based on LDR value. 2 values is fine.
// TODO debounced (async) button for setting time. Logarithmically repeating response, adjusting minutes and hours at the same time
// TODO alarm?
// TODO split into headers (and defines for SPI commands)
// TODO use registers instead of digitalwrite, etc
// TODO use registers instead of SPI library
// TODO support for bare multiplexed display without SPI

// this is automatically generated by deploy.sh
#include "current_time.h"
#include "../config.h"

//#include "main.h"

#include <SPI.h>

volatile unsigned int heartbeat_count         = 0;
volatile unsigned int update_brightness_count = 0;
volatile unsigned int display_adc_count       = 0;



typedef struct {
	char hours;
	char minutes;
	char seconds;
} elapsed;

typedef enum {
	BRIGHT,
	BRIGHTENING,
	FADING,
	DIM
} disp_state;

#ifdef AUTO_TIME
	// initialise with compile-time time with an offset to account for build/upload time
	elapsed time = {HOURS,MINUTES,SECONDS};
#else
	elapsed time = {0,0,0};
#endif

void init_display(void);

// tasks
void inc_time (void);
void update_display (char);
void update_brightness (void);

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

	// account for compile upload time
	inc_time();
	inc_time();
	inc_time();

	pinMode(LDR_PIN,INPUT);
	// 20K pullup
	digitalWrite(LDR_PIN,HIGH);

	init_display();
}

void loop () {
	// standby mode with timer1 active and CPU off
	SMCR |= 0b1101;
}

// Interrupt service routine, scheduler
ISR(TIMER1_COMPA_vect) {

#ifndef DEBUG
	if (++heartbeat_count == HEARTBEAT_PERIOD) {
		heartbeat_count = 0;
		inc_time();
		update_display(0);
		toggle_colon();
	}
#else
	if (++display_adc_count == DISPLAY_ADC_PERIOD) {
		display_adc_count = 0;
		display_adc();
	}
#endif

#ifdef AUTO_BRIGHTNESS
	if (++update_brightness_count == UPDATE_BRIGHTNESS_PERIOD) {
		update_brightness_count = 0;
		update_brightness();
	}
#endif

	// TODO: reset interrupt flag!
}

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

void update_display (char force) {
#ifndef AGGRESSIVE_MODE
	// no seconds, so no point in updating every second. Only update on 0 seconds
	if (time.seconds && !force) return;
#endif

	// select display
	digitalWrite(DISP_SS,0);

	// hours
	SPI.transfer(time.hours/10);
	SPI.transfer(time.hours%10);

	// minutes
	SPI.transfer(time.minutes/10);
	SPI.transfer(time.minutes%10);

	// deselect display
	digitalWrite(DISP_SS,1);
}

void init_display(void) {
	// init SPI

	// slave select pin init
	pinMode (DISP_SS, OUTPUT);
	digitalWrite(DISP_SS,1);

	// wait for it to boot
	delay(300);

	// initialize SPI:
	SPI.begin();
	SPI.setClockDivider(SPI_CLOCK_DIV32);

	// reset, turn on colon
	digitalWrite(DISP_SS,0);
	// reset
	SPI.transfer(0x76);
	// dots
	SPI.transfer(0x77);
	// colon
	SPI.transfer(0x10);
	digitalWrite(DISP_SS,1);

	// fill with initial time (force)
	update_display(1);
}

void toggle_colon(){
	static char colon_state = false;

	colon_state = ! colon_state;

	// reset, turn on colon
	digitalWrite(DISP_SS,0);
	// dots
	SPI.transfer(0x77);
	// colon or no colon
	SPI.transfer(colon_state?1<<4:0);

	digitalWrite(DISP_SS,1);
}

void update_brightness() {
	unsigned int light = 0;

	// display brightness
	static disp_state state = BRIGHT;
	static signed int brightness = DISP_BRIGHTEST;

	light = analogRead(LDR_PIN);

	// state machine
	switch (state) {
		case BRIGHT:
			if ( light > BRIGHTNESS_THRESH_DARK )
				state = FADING;
		return;

		case DIM:
			if (light < BRIGHTNESS_THRESH_LIGHT )
				state = BRIGHTENING;
		return;

		case FADING:
			brightness += DISP_STEP;

			if (brightness >= DISP_DIMMEST) {
				state      = DIM;
				brightness = DISP_DIMMEST;
			}
		break;

		case BRIGHTENING:
			brightness -= DISP_STEP;
			if (brightness <= DISP_BRIGHTEST) {
				state      = BRIGHT;
				brightness = DISP_BRIGHTEST;
			}
		break;
	}

	digitalWrite(DISP_SS,0);
	SPI.transfer(0x7A);
	SPI.transfer(brightness);
	digitalWrite(DISP_SS,1);
}

void display_adc() {
	unsigned int light = 0;

	light = analogRead(LDR_PIN);

	digitalWrite(DISP_SS,0);

	SPI.transfer(light/1000%10);
	SPI.transfer(light/100 %10);
	SPI.transfer(light/10  %10);
	SPI.transfer(light/1   %10);

	// deselect display
	digitalWrite(DISP_SS,1);
}
