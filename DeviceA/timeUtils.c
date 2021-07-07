#include "Arduino.h"
#include "timeUtils.h"


unsigned long getMilliseconds(void) {
	return millis();
}

unsigned long millisecondsElapsedSince(unsigned long millisecondsSince){
	return getMilliseconds() - millisecondsSince;
}
