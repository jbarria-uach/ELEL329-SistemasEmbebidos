#ifndef CONFIG_H
#define CONFIG_H

#define SLEEP_MODE_PIN 12

/* pin debouncer configuration */

#define PIN_DEBOUNCE_TIME_MS 100
#define MAX_N_PINS 12

/* led interface configuration */

#define MAX_N_LEDS 4

/* led blinker configuration */

#define LED_BLINKER_PIN 13
#define LED_BLINKER_PERIOD_MS 500

/* serial led controller configuration */

#define SERIAL_LED_PIN_1 12
#define SERIAL_LED_PIN_2 11
#define SERIAL_LED_PIN_3 10

#endif
