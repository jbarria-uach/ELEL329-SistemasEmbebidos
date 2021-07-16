/**
 * @file DeviceA.ino
 * @author Jorge Barría (jebarria@alumnos.uach.cl)
 * @brief Sketch for ELEL329 "Embedded Systems" assignment.
 * @version 0.1
 * @date 2021-07-07
 * 
 * @copyright Copyright (c) 2021 by Universidad Austral de Chile
 * 
 * @see https://siveducmd.uach.cl/pluginfile.php/1803649/mod_resource/content/3/Tarea%20ELEL-329%20Programaci%C3%B3n%20de%20Sistemas%20Embebidos.pdf
 */

#include <Arduino.h>
#include <limits.h>
#include <avr/sleep.h>
#include "pinDebouncer.h"
#include "timeUtils.h"
#include "ledBlinker.h"
#include "serialLedController.h"
#include "ledInterface.h"
#include "config.h"

/**
 * @brief push buttons state.
 * 
 * It stores the current state and flags to notify the push events for
 * every button: Pa, Pb, Pc, Pd.
 */
typedef struct {
  volatile int PAState; /**< current state (Hi/Lo) of Pa button */
  volatile int PBState; /**< current state (Hi/Lo) of Pb button */
  volatile int PCState; /**< current state (Hi/Lo) of Pc button */
  volatile int PDState; /**< current state (Hi/Lo) of Pd button */
  volatile int PAPushed; /**< flag to notify that Pa button was pushed */
  volatile int PBPushed; /**< flag to notify that Pb button was pushed */
  volatile int PCPushed; /**< flag to notify that Pc button was pushed */
  volatile int PDPushed; /**< flag to notify that Pd button was pushed */
} PushButtonsState;

static int pinsToDebounce[] = {PA, PB, PC, PD}; // Pins to debounce
static unsigned long timeToSleep = ULONG_MAX;
static volatile PushButtonsState buttons;
static volatile unsigned long lastInterruptTime = 0;

/* Local function prototypes */
static void goToSleep();
static void wakeUpRoutine();

/**
 * @brief Sketch initialization.
 * 
 * Initializes the system.
 * 
 * Currently it:
 *    - Set button inputs pin mode (with internal pull ups enabled as
 *      they're passive inputs).
 *    - Set external interrupt pin as input (with internal pull up
 *      disabled as it is an active input) an attaches the main ISR
 *      function to its falling edge events.
 *    - Initializes the serial port hardware.
 *    - Initializes the project modules.
 */
void setup() {
  // Push Buttons are passive, use internal pullups on these inputs
  pinMode(PA, INPUT_PULLUP);
  pinMode(PB, INPUT_PULLUP);
  pinMode(PC, INPUT_PULLUP);
  pinMode(PD, INPUT_PULLUP);
  
  // External interrupt pin is active
  pinMode(EXT_INT_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(EXT_INT_PIN), 
                  onExternalInterrupt, FALLING);

  // Debouncer setup
  pinDebouncerInit(4, pinsToDebounce);
  buttons.PAState = pinDebouncerGetPinState(PA);
  buttons.PBState = pinDebouncerGetPinState(PB);
  buttons.PCState = pinDebouncerGetPinState(PC);
  buttons.PDState = pinDebouncerGetPinState(PD);
  
  // LEDs Blinker setup
  ledBlinkerInit();

  // Serial port setup
  Serial.begin(115200);
  serialLedControllerInit();
}

/**
 * @brief Main loop.
 * 
 * - Iterate/update on every module.
 * - Captures the serial port data and send it to the serial LEDs controller
 *   to parse.
 * - Processes the button events.
 * - Put the device to sleep when requested.
 */
void loop() {
  // Update modules periodically
  pinDebouncerUpdate();
  ledBlinkerIterate();
  // Check if there's something in serial port
  while (Serial.available() > 0) {
    // Input for the Serial LEDs controller to parse.
    serialLedParseChar((char) Serial.read());
  }
  // Check button events asynchronously.
  if (buttons.PAPushed) {
    buttons.PAPushed = 0;
    onPushButtonA();
  }
  if (buttons.PBPushed) {
    buttons.PBPushed = 0;
    onPushButtonB();
  }
  if (buttons.PCPushed) {
    buttons.PCPushed = 0;
    onPushButtonC();
  }
  if (buttons.PDPushed) {
    buttons.PDPushed = 0;
    onPushButtonD();
  }
  if (millis() >= timeToSleep) { // Sleep asynchronously
    goToSleep();
    // Sleeping...
    wakeUpRoutine();
  }
}

/* Button Callbacks */ 

/**
 * @brief Pa button callback routine
 * 
 * Requests the RPi to stop sending LEDs state (which have no effect if it's
 * not sending anything) and then changes the L1 LED duty cycle as requested
 * in 3.1.
 */
void onPushButtonA() {
  requestLEDsStateOFF();
  ledBlinkerIntensityCycle();
}

/**
 * @brief Pb button callback routine
 * 
 * Requests the RPi to stop sending LEDs state (which have no effect if it's
 * not sending anything) and then sends a "print" command to Raspberry Pi (as
 * requested in 3.2) with the millis() value.
 */
void onPushButtonB() {
  requestLEDsStateOFF();
  Serial.print("ELEL329-PRINT-Milliseconds elapsed on device A:");
  Serial.println(millis());
}


/**
 * @brief Pc button callback routine
 * 
 * Requests the RPi to start sending LEDs state (as requested in 3.3).
 */
void onPushButtonC() {
  requestLEDsStateON();
}

/**
 * @brief Pd button callback routine
 * 
 * Schedule to enter in sleep mode (as requested in 3.4) after a debounce
 * period (to avoid waking up inmediatly).
 */
void onPushButtonD() {
  requestLEDsStateOFF();
  timeToSleep = millis() + PIN_DEBOUNCE_TIME_MS;
}

/* Interrupt Service Routines */

/**
 * @brief Main ISR function for external interrupt.
 * 
 * Basically it responds to push button state changes to notify the main loop
 * in case a button is pushed. It obtains the pin value from the pinDebouncer
 * module so it's relatively "bounce effect safe".
 */
void onExternalInterrupt() {
  if ((buttons.PAState == HIGH) && (pinDebouncerGetPinState(PA) == LOW)) {
    buttons.PAPushed++;
  }
  else if ((buttons.PBState == HIGH) && (pinDebouncerGetPinState(PB) == LOW)) {
    buttons.PBPushed++;
  }
  else if ((buttons.PCState == HIGH) && (pinDebouncerGetPinState(PC) == LOW)) {
    buttons.PCPushed++;
  }
  else if ((buttons.PDState == HIGH) && (pinDebouncerGetPinState(PD) == LOW)) {
    buttons.PDPushed++;
  }
  buttons.PAState = pinDebouncerGetPinState(PA);
  buttons.PBState = pinDebouncerGetPinState(PB);
  buttons.PCState = pinDebouncerGetPinState(PC);
  buttons.PDState = pinDebouncerGetPinState(PD);
}

/**
 * @brief Wake up ISR function for external interrupt.
 * 
 * It's a dummy function used just to wake up from sleep mode.
 */
void wakeUpOnExternalInterrupt() {

}

/* Internal helper functions */

/**
 * @brief Puts the device into sleep mode.
 * 
 * It takes care of turning the LEDs off and changing the external interrupt
 * ISR function before going ino sleep mode.
 */
static void goToSleep(){
  // Turn OFF all the LEDs
  ledTurnOff(LED_BLINKER_PIN);
  ledTurnOff(SERIAL_LED_PIN_1);
  ledTurnOff(SERIAL_LED_PIN_2);
  ledTurnOff(SERIAL_LED_PIN_3);
  // Change ISR
  detachInterrupt(digitalPinToInterrupt(EXT_INT_PIN));
  attachInterrupt(digitalPinToInterrupt(EXT_INT_PIN), 
                  wakeUpOnExternalInterrupt, LOW);
  // Put the processor to sleep
  sleep_enable();
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_cpu();
}

/**
 * @brief Handle the device wake up tasks.
 * 
 * Basically it changes the ISR function to the main one and then sets
 * "timeToSleep" on the infinity.
 */
static void wakeUpRoutine(){
  detachInterrupt(digitalPinToInterrupt(EXT_INT_PIN));
  attachInterrupt(digitalPinToInterrupt(EXT_INT_PIN), 
                  onExternalInterrupt, FALLING);
  timeToSleep = ULONG_MAX;
}

/**
 * @brief Wrapper function for REQLEDS ON command
 * 
 * It requests the Raspberry Pi to start the L2, L3 and L4 random states
 * transmission.
 */
static void requestLEDsStateON(void) {
  Serial.println("ELEL329-REQLEDS-ON");
}

/**
 * @brief Wrapper function for REQLEDS ON command
 * 
 * It requests the Raspberry Pi to stop the L2, L3 and L4 random states
 * transmission.
 */
static void requestLEDsStateOFF(void) {
  Serial.println("ELEL329-REQLEDS-OFF");
}