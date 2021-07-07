#include <limits.h>
#include "Arduino.h"
#include "pinDebouncer.h"
#include "timeUtils.h"

/* Data structures */
typedef struct {
  unsigned long lastTimeChanged;
  unsigned int state;
  int pinNumber;
} debouncedPin;

typedef struct {
  unsigned int nPins;
  debouncedPin* input;
} pinDebouncerState;

/* Internal variables */
static debouncedPin pins[MAX_N_PINS];
static pinDebouncerState state;

/* Internal function prototypes */
static debouncedPin* getDebouncedPin(int pinNumber);

void pinDebouncerInit(int nPins, int *pinNumbers){
	int i;
	state.nPins = nPins;
	state.input = pins;
	debouncedPin* pin;
	for (i=0; i<nPins; i++) {
		pin = &state.input[i];
		pin->lastTimeChanged = 0.0;
		pin->pinNumber = pinNumbers[i];
		pin->state = digitalRead(pinNumbers[i]);
	}
}

void pinDebouncerUpdate(){
	int i, newState;
	debouncedPin* pin;
	for (i=0; i<state.nPins ; i++) {
		pin = &state.input[i];
		if (millisecondsElapsedSince(pin->lastTimeChanged) > PIN_DEBOUNCE_TIME_MS) {
			newState = digitalRead(pin->pinNumber);
			if (newState != pin->state) {
				pin->state = newState;
				pin->lastTimeChanged = getMilliseconds();
			}
		}
	}
}

int pinDebouncerGetPinState(int pinNumber){
	debouncedPin* pin = getDebouncedPin(pinNumber);
	return pin != NULL ? pin->state : -1;
}

unsigned long pinDebouncerGetPinLastChangeTime(int pinNumber){
	debouncedPin* pin = getDebouncedPin(pinNumber);
	return pin != NULL ? pin->lastTimeChanged : ULONG_MAX;
}

static debouncedPin* getDebouncedPin(int pinNumber){
	int pinIndex = 0;
	while (state.input[pinIndex].pinNumber != pinNumber) {
		pinIndex++;
		if (pinIndex >= state.nPins) {
			return NULL;
		}
	}
	return &state.input[pinIndex];
}