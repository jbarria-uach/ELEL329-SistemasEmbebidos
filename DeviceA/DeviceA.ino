#include <Arduino.h>
#include <limits.h>
#include <avr/sleep.h>
#include "pinDebouncer.h"
#include "timeUtils.h"
#include "ledBlinker.h"
#include "serialLedController.h"
#include "ledInterface.h"
#include "config.h"

typedef struct {
  volatile int PAState;
  volatile int PBState;
  volatile int PCState;
  volatile int PDState;
  volatile int PAPushed;
  volatile int PBPushed;
  volatile int PCPushed;
  volatile int PDPushed;
} PushButtonsState;

static int pinsToDebounce[] = {PA, PB, PC, PD}; // Pins to debounce
static unsigned long timeToSleep = ULONG_MAX;
static volatile PushButtonsState buttons;
static volatile unsigned long lastInterruptTime = 0;

static void goToSleep();
static void wakeUpRoutine();

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

void onPushButtonA() {
  requestLEDsStateOFF();
  ledBlinkerIntensityCycle();
}

void onPushButtonB() {
  requestLEDsStateOFF();
  Serial.print("ELEL329-PRINT-Milliseconds elapsed on device A:");
  Serial.println(millis());
}

void onPushButtonC() {
  requestLEDsStateON();
}

void onPushButtonD() {
  requestLEDsStateOFF();
  timeToSleep = millis() + PIN_DEBOUNCE_TIME_MS;
}

/* Interrupt Service Routines */

void onExternalInterrupt() {
  if ((buttons.PAState == HIGH) && (pinDebouncerGetPinState(PA) == LOW)) {
    buttons.PAPushed++;
  }
  if ((buttons.PBState == HIGH) && (pinDebouncerGetPinState(PB) == LOW)) {
    buttons.PBPushed++;
  }
  if ((buttons.PCState == HIGH) && (pinDebouncerGetPinState(PC) == LOW)) {
    buttons.PCPushed++;
  }
  if ((buttons.PDState == HIGH) && (pinDebouncerGetPinState(PD) == LOW)) {
    buttons.PDPushed++;
  }
  buttons.PAState = pinDebouncerGetPinState(PA);
  buttons.PBState = pinDebouncerGetPinState(PB);
  buttons.PCState = pinDebouncerGetPinState(PC);
  buttons.PDState = pinDebouncerGetPinState(PD);
}

void wakeUpOnExternalInterrupt() {

}

/* Internal helper functions */

static void goToSleep(){
  // Turn OFF all the LEDs
  ledTurnOff(LED_BLINKER_PIN);
  ledTurnOff(SERIAL_LED_PIN_1);
  ledTurnOff(SERIAL_LED_PIN_2);
  ledTurnOff(SERIAL_LED_PIN_3);
  // Change ISR
  detachInterrupt(digitalPinToInterrupt(EXT_INT_PIN));
  delay(100);
  attachInterrupt(digitalPinToInterrupt(EXT_INT_PIN), 
                  wakeUpOnExternalInterrupt, LOW);
  delay(100);
  // Put the processor to sleep
  sleep_enable();
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_cpu();
}

static void wakeUpRoutine(){
  detachInterrupt(digitalPinToInterrupt(EXT_INT_PIN));
  attachInterrupt(digitalPinToInterrupt(EXT_INT_PIN), 
                  onExternalInterrupt, FALLING);
  timeToSleep = ULONG_MAX;
}

static void requestLEDsStateON(void) {
  Serial.println("ELEL329-REQLEDS-ON");
}

static void requestLEDsStateOFF(void) {
  Serial.println("ELEL329-REQLEDS-OFF");
}