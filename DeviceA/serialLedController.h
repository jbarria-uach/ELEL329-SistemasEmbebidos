#ifndef SERIAL_LED_CONTROLLER_H
#define SERIAL_LED_CONTROLLER_H

#include "config.h"

#ifdef __cplusplus
extern "C" {
#endif

void serialLedControllerInit(void);
void serialLedParseChar(char c);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* SERIAL_LED_CONTROLLER_H */
