#include "Arduino.h"
#include "timeUtils.h"
#include "ledBlinker.h"
#include "ledInterface.h"

/* Constants */
#define N_DUTY_CYCLES 4
static const int dutyCycles[] = {64, 128, 192, 255};

/* Internal variables */
static unsigned int currentDutyCycleIndex = 0;
static unsigned long lastBlinkTime = 0;


void ledBlinkerInit(void){
	ledInit(LED_BLINKER_PIN);
	ledSetIntensity(LED_BLINKER_PIN, dutyCycles[currentDutyCycleIndex]);
	lastBlinkTime = 0;
}

unsigned long ledBlinkerIterate(void){
	if (millisecondsElapsedSince(lastBlinkTime) >= LED_BLINKER_PERIOD_MS) {
		lastBlinkTime = getMilliseconds();
		ledToggle(LED_BLINKER_PIN);
	}
	return ledGetState(LED_BLINKER_PIN);
}

void ledBlinkerIntensityCycle(void){
	if (++currentDutyCycleIndex >= N_DUTY_CYCLES) { // do circular iteration
		currentDutyCycleIndex = 0;
	}
	ledSetIntensity(LED_BLINKER_PIN, dutyCycles[currentDutyCycleIndex]);
}
