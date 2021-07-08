#ifndef CONFIG_H
#define CONFIG_H

#define SLEEP_MODE_PIN 12

/* Pushbuttons */
#define EXT_INT_PIN 3
#define PA 4
#define PB 5
#define PC 6
#define PD 7

/* pin debouncer configuration */

#define PIN_DEBOUNCE_TIME_MS 100
#define MAX_N_PINS 12

/* led interface configuration */

#define MAX_N_LEDS 4

/* led blinker configuration */

#define LED_BLINKER_PIN 13   // L1
#define LED_BLINKER_PERIOD_MS 500

/* serial led controller configuration */

#define SERIAL_LED_PIN_1 12  // L2
#define SERIAL_LED_PIN_2 11  // L3
#define SERIAL_LED_PIN_3 10  // L4

#endif
