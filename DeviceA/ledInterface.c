/**
 * @file ledInterface.c
 * @author Jorge Barría (jebarria@alumnos.uach.cl)
 * @brief LED handling interface source code.
 * @version 0.1
 * @date 2021-07-07
 * 
 * @copyright Copyright (c) 2021 by Universidad Austral de Chile.
 *
 * An interface module to wrap GPIO handling to control LEDs. It supports
 * PWM intensity control.
 * @note It may be overkill in the context of this project  because it's used
 *       just to wrap digitalWrite() calls, but it exists because of initial 
 *       misunderstanding of "duty cycle" specified in 3.1.
 * @see https://siveducmd.uach.cl/pluginfile.php/1803649/mod_resource/content/3/Tarea%20ELEL-329%20Programaci%C3%B3n%20de%20Sistemas%20Embebidos.pdf
 * 
 */
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

// Function description in header.
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

// Function description in header.
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

// Function description in header.
void ledTurnOff(int pinNumber){
	ledState* led = getLed(pinNumber);
	if (led != NULL) {
		led->state = LOW;
		digitalWrite(led->pinNumber, LOW);
	}
}

// Function description in header.
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

// Function description in header.
void ledSetIntensity(int pinNumber, int intensity){
	ledState* led = getLed(pinNumber);
	if (led != NULL) {
		led->intensity = intensity;
		if (led->state == HIGH) {
			analogWrite(led->pinNumber, led->intensity);
		}
	}
}

// Function description in header.
int ledGetState(int pinNumber){
	ledState* led = getLed(pinNumber);
	if (led != NULL) {
		return led->state;
	}
	else {
		return LOW;
	}
}

/**
 * @brief Get the LED object related with the pin number.
 * 
 * @param pinNumber the number of the pin to which the LED is connected.
 * @return ledState* a pointer to the corresponding LED ledState struct or 
 * 					 NULL if there's no LED connected to this.
 */
static ledState* getLed(int pinNumber) {
	int i;
	for (i=0; i<nLeds; i++) {
		if (leds[i].pinNumber == pinNumber) {
			return &leds[i];
		}
	}
	return NULL;
}