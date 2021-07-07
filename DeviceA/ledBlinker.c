#include "Arduino.h"
#include "timeUtils.h"
#include "ledBlinker.h"

/* Data structures */

typedef struct {
	unsigned int currentDutyCycleIndex;
	unsigned int currentDutyCycle;
	unsigned int currentState;
	unsigned long lastBlinkTime;
} ledBlinkerData;


/* Constants */
#define N_DUTY_CYCLES 4
static const int dutyCycles[] = {64, 128, 192, 255};

/* Internal variables */
static ledBlinkerData led;

/* Internal function prototypes */
static void toggleLedState(void);
static void turnLedOn(void);
static void turnLedOff(void);


void ledBlinkerInit(void){
	pinMode(LED_BLINKER_PIN, OUTPUT);
	led.currentState = LOW;
	led.currentDutyCycleIndex = 0;
	led.currentDutyCycle = dutyCycles[0];
	turnLedOff();
}

unsigned long ledBlinkerIterate(void){
	if (millisecondsElapsedSince(led.lastBlinkTime) >= LED_BLINKER_PERIOD_MS) {
		led.lastBlinkTime = millis();
		toggleLedState();
	}
	return led.currentState;
}

void ledBlinkerIntensityCycle(void){
	if (++led.currentDutyCycleIndex >= N_DUTY_CYCLES) { // do circular iteration
		led.currentDutyCycleIndex = 0;
	}
	led.currentDutyCycle = dutyCycles[led.currentDutyCycleIndex];
	if (led.currentState == HIGH) {
		turnLedOn(); // Force PWM update
	}
}

static void toggleLedState(void) {
	if (led.currentState == HIGH) {
		led.currentState = LOW;
		turnLedOff();
	}
	else {
		led.currentState = HIGH;
		turnLedOn();
	}
}

static void turnLedOn(void){
	analogWrite(LED_BLINKER_PIN, led.currentDutyCycle);
}

static void turnLedOff(void){
	//analogWrite(LED_BLINKER_PIN, 5);
	digitalWrite(LED_BLINKER_PIN, LOW);
}
