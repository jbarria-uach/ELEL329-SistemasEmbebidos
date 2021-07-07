#ifndef CONFIG_H
#define CONFIG_H

#define SLEEP_MODE_PIN 12

/* pin debouncer configuration */

#define PIN_DEBOUNCE_TIME_MS 20
#define MAX_N_PINS 12

/* led interface configuration */

#define MAX_N_LEDS 4

/* led blinker configuration */

#define LED_BLINKER_PIN 13
#define LED_BLINKER_PERIOD_MS 1000
#endif
