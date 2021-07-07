#ifndef LED_INTERFACE_H
#define LED_INTERFACE_H

#include "config.h"

#ifdef __cplusplus
extern "C" {
#endif

void ledInit(int pinNumber);
void ledTurnOn(int pinNumber);
void ledTurnOff(int pinNumber);
void ledToggle(int pinNumber);
void ledSetIntensity(int pinNumber, int intensity);
int ledGetState(int pinNumber);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* LED_INTERFACE_H */
