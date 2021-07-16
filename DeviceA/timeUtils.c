/**
 * @file timeUtils.c
 * @author Jorge Barría (jebarria@alumnos.uach.cl)
 * @brief Time related utility functions source code.
 * @version 0.1
 * @date 2021-07-07
 * 
 * @copyright Copyright (c) 2021 by Universidad Austral de Chile
 * 
 */
#include "Arduino.h"
#include "timeUtils.h"

// Function description in header file.
unsigned long getMilliseconds(void) {
	return millis();
}

// Function description in header file.
unsigned long millisecondsElapsedSince(unsigned long millisecondsSince){
	return getMilliseconds() - millisecondsSince;
}
