/**
 * @file serialLedController.c
 * @author Jorge Barría (jebarria@alumnos.uach.cl)
 * @brief Serial operated LED controller source code.
 * @version 0.1
 * @date 2021-07-08
 * 
 * @copyright Copyright (c) 2021 by Universidad Austral de Chile
 * 
 * Parses ELEL329-LEDS commands to change the state of three LEDs.
 * 
 */
#include "serialLedController.h"
#include "ledInterface.h"
#include <string.h>

/* Constants */
#define HEADER_STRING "ELEL329-LEDS:"
#define START_CHAR 'E'
#define END_CHAR   '\n'
#define MAX_SERIAL_BUFFER_SIZE 20
static const int ledPinNumbers[] = {SERIAL_LED_PIN_1, 
							   	    SERIAL_LED_PIN_2,
							   	    SERIAL_LED_PIN_3};

/* Internal variables */
static char serialBuffer[MAX_SERIAL_BUFFER_SIZE];
static char* bufferPtr;
static int receivedChars;

/* Internal functions prototypes */

static void processBuffer(void);
static void resetBuffer(void);

// Description in header file.
void serialLedControllerInit(void){
	ledInit(SERIAL_LED_PIN_1);
	ledInit(SERIAL_LED_PIN_2);
	ledInit(SERIAL_LED_PIN_3);
	resetBuffer();
}

// Description in header file.
void serialLedParseChar(char c){
	if ((receivedChars > 0) || (c == START_CHAR)){
		*bufferPtr = c;
		bufferPtr++;
		receivedChars++;
		if (c == END_CHAR) {
			processBuffer();
		}
		else if (receivedChars >= MAX_SERIAL_BUFFER_SIZE) {
			resetBuffer();
		}
	}
}

/**
 * @brief Process the buffer.
 * 
 * It should be called when the end character is detected.
 * It verifies if the buffer has a valid command and act on the LEDs if so.
 * It resets the buffer before ending its execution.
 * 
 */
static void processBuffer(void){
	int i;
	char* ptr;
	if (!(strncmp(serialBuffer, HEADER_STRING, 13))) { // if header matches
		ptr = &serialBuffer[13];
		for (i=0; i<3; i++){
			if (*ptr == 'H') {
				ledTurnOn(ledPinNumbers[i]);
			}
			else if (*ptr == 'L') {
				ledTurnOff(ledPinNumbers[i]);
			}
			ptr++;
		}
	}
	resetBuffer();
}

/**
 * @brief Reset the buffer.
 * 
 * It flushes all the data in the buffer.
 * 
 */
static void resetBuffer(void){
	memset((void *)serialBuffer, 0, 
		   sizeof(char) * MAX_SERIAL_BUFFER_SIZE);
	bufferPtr = &serialBuffer[0];
	receivedChars = 0;
}
