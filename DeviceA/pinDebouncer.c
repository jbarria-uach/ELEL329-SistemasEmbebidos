/**
 * @file pinDebouncer.c
 * @author Jorge Barría (jebarria@alumnos.uach.cl)
 * @brief Input pin debouncer module header.
 * @version 0.1
 * @date 2021-07-07
 * 
 * @copyright Copyright (c) 2021 by Universidad Austral de Chile
 * 
 * This module exists to face the push button "bounce effect" without using
 * the delay() function.
 * 
 */
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

// Function description in header.
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

// Function description in header.
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

// Function description in header.
int pinDebouncerGetPinState(int pinNumber){
	debouncedPin* pin = getDebouncedPin(pinNumber);
	return pin != NULL ? pin->state : -1;
}

/**
 * @brief Gets the pin debouncedPin structure.
 * 
 * @param pinNumber the pin number.
 * @return a pointer to the corresponding debouncedPin structure instance.
 */
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