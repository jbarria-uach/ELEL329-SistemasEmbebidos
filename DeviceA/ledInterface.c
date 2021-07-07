#include "Arduino.h"
#include "ledInterface.h"

#define MAX_LED_INTENSITY 255

typedef struct {
	unsigned int state;
	unsigned int intensity;
	unsigned int pinNumber;
} ledState;

static ledState leds[MAX_N_LEDS];
static unsigned int nLeds = 0;

static ledState* getLed(int pinNumber);

void ledInit(int pinNumber){
	if (nLeds < MAX_N_LEDS) {
		leds[nLeds].state = LOW;
		leds[nLeds].intensity = MAX_LED_INTENSITY;
		leds[nLeds].pinNumber = pinNumber;
		pinMode(pinNumber, OUTPUT);
		digitalWrite(pinNumber, LOW);
		nLeds++;
	}
}

void ledTurnOn(int pinNumber){
	ledState* led = getLed(pinNumber);
	if (led != NULL) {
		led->state = HIGH;
		if (led->intensity == MAX_LED_INTENSITY) {
			digitalWrite(led->pinNumber, HIGH);
		}
		else {
			analogWrite(led->pinNumber, led->intensity);
		}
	}
}

void ledTurnOff(int pinNumber){
	ledState* led = getLed(pinNumber);
	if (led != NULL) {
		led->state = LOW;
		digitalWrite(led->pinNumber, LOW);
	}
}

void ledToggle(int pinNumber){
	ledState* led = getLed(pinNumber);
	if (led != NULL) {
		if (led->state == HIGH) {
			ledTurnOff(led->pinNumber);
		}
		else {
			ledTurnOn(led->pinNumber);
		}
	}
}

void ledSetIntensity(int pinNumber, int intensity){
	ledState* led = getLed(pinNumber);
	if (led != NULL) {
		led->intensity = intensity;
		if (led->state == HIGH) {
			analogWrite(led->pinNumber, led->intensity);
		}
	}
}

int ledGetState(int pinNumber){
	ledState* led = getLed(pinNumber);
	if (led != NULL) {
		return led->state;
	}
	else {
		return LOW;
	}
}

static ledState* getLed(int pinNumber) {
	int i;
	for (i=0; i<nLeds; i++) {
		if (leds[i].pinNumber == pinNumber) {
			return &leds[i];
		}
	}
	return NULL;
}