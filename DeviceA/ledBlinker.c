/**
 * @file ledBlinker.c
 * @author Jorge Barría (jebarria@alumnos.uach.cl)
 * @brief LED blinker module source code.
 * @version 0.1
 * @date 2021-07-07
 * 
 * @copyright Copyright (c) 2021 by Universidad Austral de Chile.
 * 
 * A module responsible of controlling a LED blink without using Arduino 
 * delay() function. It allows duty cycle control cyclyng between four 
 * modes:
 *      - Mode 1: 25% duty cycle.
 *      - Mode 2: 50% duty cycle.
 *      - Mode 3: 75% duty cycle.
 *      - Mode 4: 100% duty cycle.
 * @see https://siveducmd.uach.cl/pluginfile.php/1803649/mod_resource/content/3/Tarea%20ELEL-329%20Programaci%C3%B3n%20de%20Sistemas%20Embebidos.pdf
 * 
 */
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

// Function documentation in header.
void ledBlinkerInit(void){
	ledInit(LED_BLINKER_PIN);
	calculateTimes();
	lastBlinkTime = 0;
}

// Function documentation in header.
void ledBlinkerIterate(void){
	if (ledGetState(LED_BLINKER_PIN) == HIGH) {
		ledBlinkerIterateOn();
	}
	else {
		ledBlinkerIterateOff();
	}
}

// Function documentation in header.
void ledBlinkerIntensityCycle(void){
	if (++currentDutyCycleIndex >= N_DUTY_CYCLES) { // do circular iteration
		currentDutyCycleIndex = 0;
	}
}

/**
 * @brief Calculate duty cycle ON/OFF times.
 * 
 * This should be called just once at the beginning.
 * It calculates the duty cycle t_ON and t_OFF in milliseconds given the 
 * LED_BLINKER_PERIOD_MS definition in config.h.
 */
static void calculateTimes(void) {
	int i;
	for (i=0; i<N_DUTY_CYCLES; i++) {
		tOnMilliseconds[i] = (int) (dutyCycles[i] * LED_BLINKER_PERIOD_MS);
		tOffMilliseconds[i] = (int) ((1.0 - dutyCycles[i]) * LED_BLINKER_PERIOD_MS);
	}
}

/**
 * @brief Iterate the module activity when the LED is ON.
 */
static void ledBlinkerIterateOn(void) {
	if (millisecondsElapsedSince(lastBlinkTime) 
			>= tOnMilliseconds[currentDutyCycleIndex]) {
		ledTurnOff(LED_BLINKER_PIN);
		lastBlinkTime = getMilliseconds();

	}
}

/**
 * @brief Iterate the module activity when the LED is OFF.
 */
static void ledBlinkerIterateOff(void) {
	if (millisecondsElapsedSince(lastBlinkTime) 
			>= tOffMilliseconds[currentDutyCycleIndex]) {
		ledTurnOn(LED_BLINKER_PIN);
		lastBlinkTime = getMilliseconds();
	}
}