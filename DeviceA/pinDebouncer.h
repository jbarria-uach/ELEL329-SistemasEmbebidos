#ifndef PIN_DEBOUNCER_H
#define PIN_DEBOUNCER_H

#include "config.h"
#include "Arduino.h"

#ifdef __cplusplus
extern "C" {
#endif

void pinDebouncerInit(int nPins, int *pinNumbers);
void pinDebouncerUpdate();
int pinDebouncerGetPinState(int pinNumber);

unsigned long pinDebouncerGetPinLastChangeTime(int pinNumber);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* PIN_DEBOUNCER_H */
