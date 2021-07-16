/**
 * @file serialLedController.h
 * @author Jorge Barría (jebarria@alumnos.uach.cl)
 * @brief Serial operated LED controller header.
 * @version 0.1
 * @date 2021-07-08
 * 
 * @copyright Copyright (c) 2021 by Universidad Austral de Chile
 * 
 * Parses ELEL329-LEDS commands to change the state of three LEDs.
 * 
 */
#ifndef SERIAL_LED_CONTROLLER_H
#define SERIAL_LED_CONTROLLER_H

#include "config.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Module initialization function.
 * 
 * Sets the LED pins modes as output and initializes the input buffer.
 */
void serialLedControllerInit(void);

/**
 * @brief Parse a character from the serial port.
 * 
 * It should be called for every character that arrives from the serial port.
 * When a command is succesfully parsed, change the corresponding LEDs state.
 * 
 * @param c a character to parse.
 */
void serialLedParseChar(char c);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* SERIAL_LED_CONTROLLER_H */
