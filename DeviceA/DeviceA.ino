#include "pinDebouncer.h"
#include "timeUtils.h"
#include "ledBlinker.h"
#include "serialLedController.h"
#include "config.h"


typedef struct {
  int PAState;
  int PBState;
  int PCState;
  int PDState;
} PushButtonsState;

static int pinsToDebounce[] = {PA, PB, PC, PD}; // Pins to debounce
static PushButtonsState buttons;

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
    noInterrupts();
    serialLedParseChar((char) Serial.read());
    interrupts();
  }
}

void onPushButtonA() {
  ledBlinkerIntensityCycle();
}

void onPushButtonB() {
    Serial.print("ELEL329-PRINT:Milliseconds elapsed on device A:");
    Serial.println(millis());
}

void onPushButtonC() {
  Serial.println("ELEL329-REQLEDS");
}

void onPushButtonD() {
  // Not implemented yet...
}

void onExternalInterrupt() {
  noInterrupts();
  pinDebouncerUpdate();
  if ((buttons.PAState == HIGH) && (pinDebouncerGetPinState(PA) == LOW)) {
    onPushButtonA();
  }
  if ((buttons.PBState == HIGH) && (pinDebouncerGetPinState(PB) == LOW)) {
    onPushButtonB();
  }
  if ((buttons.PCState == HIGH) && (pinDebouncerGetPinState(PC) == LOW)) {
    onPushButtonC();
  }
  if ((buttons.PDState == HIGH) && (pinDebouncerGetPinState(PD) == LOW)) {
    onPushButtonD();
  }
  buttons.PAState = pinDebouncerGetPinState(PA);
  buttons.PBState = pinDebouncerGetPinState(PB);
  buttons.PCState = pinDebouncerGetPinState(PC);
  buttons.PDState = pinDebouncerGetPinState(PD);
  interrupts();
}