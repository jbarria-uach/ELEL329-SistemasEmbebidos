#ifndef LED_BLINKER_H
#define LED_BLINKER_H

#include "config.h"

#ifdef __cplusplus
extern "C" {
#endif

void ledBlinkerInit(void);
unsigned long ledBlinkerIterate(void);
void ledBlinkerIntensityCycle(void);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* LED_BLINKER_H */