#include "Arduino.h"
#include "timeUtils.h"
#include "ledBlinker.h"
#include "ledInterface.h"

/* Constants */
#define N_DUTY_CYCLES 4
static const float dutyCycles[] = {0.25, 0.50, 0.75, 1.0};

/* Internal variables */
static unsigned int currentDutyCycleIndex = 0;
static unsigned long lastBlinkTime = 0;
static int tOnMilliseconds[4];
static int tOffMilliseconds[4];

/* Internal functions */
static void calculateTimes(void);
static void ledBlinkerIterateOn(void);
static void ledBlinkerIterateOff(void);

void ledBlinkerInit(void){
	ledInit(LED_BLINKER_PIN);
	calculateTimes();
	lastBlinkTime = 0;
}

unsigned long ledBlinkerIterate(void){
	if (ledGetState(LED_BLINKER_PIN) == HIGH) {
		ledBlinkerIterateOn();
	}
	else {
		ledBlinkerIterateOff();
	}
	return ledGetState(LED_BLINKER_PIN);
}

void ledBlinkerIntensityCycle(void){
	if (++currentDutyCycleIndex >= N_DUTY_CYCLES) { // do circular iteration
		currentDutyCycleIndex = 0;
	}
}

static void calculateTimes(void) {
	int i;
	for (i=0; i<N_DUTY_CYCLES; i++) {
		tOnMilliseconds[i] = (int) (dutyCycles[i] * LED_BLINKER_PERIOD_MS);
		tOffMilliseconds[i] = (int) ((1.0 - dutyCycles[i]) * LED_BLINKER_PERIOD_MS);
	}
}

static void ledBlinkerIterateOn(void) {
	if (millisecondsElapsedSince(lastBlinkTime) 
			>= tOnMilliseconds[currentDutyCycleIndex]) {
		ledTurnOff(LED_BLINKER_PIN);
		lastBlinkTime = getMilliseconds();

	}
}

static void ledBlinkerIterateOff(void) {
	if (millisecondsElapsedSince(lastBlinkTime) 
			>= tOffMilliseconds[currentDutyCycleIndex]) {
		ledTurnOn(LED_BLINKER_PIN);
		lastBlinkTime = getMilliseconds();
	}
}