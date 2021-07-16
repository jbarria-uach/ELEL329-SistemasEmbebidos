/**
 * @file pinDebouncer.h
 * @author Jorge Barría (jebarria@alumnos.uach.cl)
 * @brief Input pin debouncer module header.
 * @version 0.1
 * @date 2021-07-07
 * 
 * @copyright Copyright (c) 2021 by Universidad Austral de Chile
 * 
 * This module exists to face the push button "bounce effect" without using
 * the delay() function.
 * 
 */
#ifndef PIN_DEBOUNCER_H
#define PIN_DEBOUNCER_H

#include "config.h"
#include "Arduino.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initializes the module.
 * 
 * This function should be called just once before using the module.
 * 
 * @param nPins number of pins to consider in the context of the module.
 * @param pinNumbers a pointer to an array of pin numbers.
 */
void pinDebouncerInit(int nPins, int *pinNumbers);

/**
 * @brief Update debouncer state.
 * 
 * This function should be called as frequent as possible.
 * It checks and updates the state of the input pins considering the time of
 * the last change to avoid getting the actual state of the pin in a time 
 * window in which is expected that it should be "bouncing".
 */
void pinDebouncerUpdate();

/**
 * @brief Get the pin state, bounce effect safe.
 * 
 * If the pin is expected to be bouncing, it returns the last "valid" state.
 * The current hardware value is returned otherwise.
 * 
 * @param pinNumber the pin number which its state is requested.
 * @return the pin state.
 */
int pinDebouncerGetPinState(int pinNumber);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* PIN_DEBOUNCER_H */
