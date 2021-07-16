/**
 * @file ledBlinker.h
 * @author Jorge Barría (jebarria@alumnos.uach.cl)
 * @brief LED blinker module header.
 * @version 0.1
 * @date 2021-07-07
 * 
 * @copyright Copyright (c) 2021 by Universidad Austral de Chile.
 * 
 * A module responsible of controlling a LED blink without using Arduino 
 * delay() function. It allows duty cycle control cyclyng between four 
 * modes:
 *      - Mode 1: 25% duty cycle.
 *      - Mode 2: 50% duty cycle.
 *      - Mode 3: 75% duty cycle.
 *      - Mode 4: 100% duty cycle.
 * @see https://siveducmd.uach.cl/pluginfile.php/1803649/mod_resource/content/3/Tarea%20ELEL-329%20Programaci%C3%B3n%20de%20Sistemas%20Embebidos.pdf
 * 
 */
#ifndef LED_BLINKER_H
#define LED_BLINKER_H

#include "config.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize LED blinker module.
 * 
 * Initializes the module setting internal variables to initial values and the
 * LED pin mode. 
 */
void ledBlinkerInit(void);

/**
 * @brief Iterate module activity.
 * 
 * This should be called periodically as fast as possible taking into account
 * that the calling period T_call = 1/f_call will be equal to the maximum time
 * error. e.g.:
 *      - f_call = 10 Hz, e_time <= T_call = 100ms
 *      - f_call = 100 Hz, e_time <= T_call = 10ms
 *      - f_call = 500 Hz, e_time <= T_call = 2ms
 *      - f_call = 1 kHz, e_time <= T_call = 1ms
 * 
 * Anyways, given that the human eye perceive 100ms as instantaneous, any 
 * f_call >= 10 Hz is enough.
 */
void ledBlinkerIterate(void);

/**
 * @brief Changes the LED blinking duty cycle.
 * 
 * It cycles between 25%, 50%, 75% and 100% as given by requirements in 3.1
 * @see https://siveducmd.uach.cl/pluginfile.php/1803649/mod_resource/content/3/Tarea%20ELEL-329%20Programaci%C3%B3n%20de%20Sistemas%20Embebidos.pdf
 */
void ledBlinkerIntensityCycle(void);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* LED_BLINKER_H */