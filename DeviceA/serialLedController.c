#include "serialLedController.h"
#include "ledInterface.h"

#include <string.h>

#define HEADER_STRING "ELEL329-LEDS:"
#define START_CHAR 'E'
#define END_CHAR   '\n'
#define MAX_SERIAL_BUFFER_SIZE 20
static const int ledPinNumbers[] = {SERIAL_LED_PIN_1, 
							   	    SERIAL_LED_PIN_2,
							   	    SERIAL_LED_PIN_3};

static char serialBuffer[MAX_SERIAL_BUFFER_SIZE];
static char* bufferPtr;
static int receivedChars;

static void processBuffer(void);
static void resetBuffer(void);

void serialLedControllerInit(void){
	ledInit(SERIAL_LED_PIN_1);
	ledInit(SERIAL_LED_PIN_2);
	ledInit(SERIAL_LED_PIN_3);
	resetBuffer();
}

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

static void resetBuffer(void){
	memset((void *)serialBuffer, 0, 
		   sizeof(char) * MAX_SERIAL_BUFFER_SIZE);
	bufferPtr = &serialBuffer[0];
	receivedChars = 0;
}
