/**
 * @file ledInterface.h
 * @author Jorge Barría (jebarria@alumnos.uach.cl)
 * @brief LED handling interface header.
 * @version 0.1
 * @date 2021-07-07
 * 
 * @copyright Copyright (c) 2021 by Universidad Austral de Chile.
 * 
 * An interface module to wrap GPIO handling to control LEDs. It supports
 * PWM intensity control.
 * @note It may be overkill in the context of this project  because it's used
 *       just to wrap digitalWrite() calls, but it exists because of initial 
 *       misunderstanding of "duty cycle" specified in 3.1.
 * @see https://siveducmd.uach.cl/pluginfile.php/1803649/mod_resource/content/3/Tarea%20ELEL-329%20Programaci%C3%B3n%20de%20Sistemas%20Embebidos.pdf
 * 
 */
#ifndef LED_INTERFACE_H
#define LED_INTERFACE_H

#include "config.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initializes the LED pin.
 * 
 * This function should be called before handling the LED.
 * 
 * @param pinNumber the number of the pin to which the LED is connected.
 */
void ledInit(int pinNumber);

/**
 * @brief Turns the LED ON.
 * 
 * @param pinNumber the number of the pin to which the LED is connected.
 */
void ledTurnOn(int pinNumber);

/**
 * @brief Turns the LED OFF.
 * 
 * @param pinNumber the number of the pin to which the LED is connected.
 */
void ledTurnOff(int pinNumber);

/**
 * @brief Toggles the LED state.
 * 
 * Turns it off if it's on. Turns it on if it's off.
 * 
 * @param pinNumber the number of the pin to which the LED is connected.
 */
void ledToggle(int pinNumber);

/**
 * @brief Sets the LED intensity.
 * 
 * It sets the intensity at which the LED should bright whe it's ON.
 * 
 * @param pinNumber the number of the pin to which the LED is connected.
 * @param intensity the intensity (as PWM duty cycle) at which the LED should
 *                  bright when it's ON.
 */
void ledSetIntensity(int pinNumber, int intensity);

/**
 * @brief Get the current LED's state.
 * 
 * @param pinNumber the number of the pin to which the LED is connected.
 * @return int the current LED state. (it could be tested with Arduino HIGH
 *             and LOW Arduino macros).
 */
int ledGetState(int pinNumber);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* LED_INTERFACE_H */
