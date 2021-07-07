#include "pinDebouncer.h"
#include "timeUtils.h"
#include "ledBlinker.h"

typedef enum {
  BLINKING_LED,
  SERIAL_PRINT,
  SERIAL_LEDS,
  ESCAPE
} deviceAction;


static deviceAction currentAction = BLINKING_LED;
static int pinsToDebounce[] = {4,5,6,7}; // Pins to debounce
static int prevState;

void setup() {
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(13, OUTPUT);
  pinDebouncerInit(4, pinsToDebounce);
  ledBlinkerInit();
  Serial.begin(115200);
  prevState = 0;
  //attachInterrupt(digitalPinToInterrupt(WAKEUP_PIN))
}

void loop() {
  pinDebouncerUpdate();
  ledBlinkerIterate();
  int newState = pinDebouncerGetPinState(4);
  if (newState != prevState) {
    prevState = newState;
    if (newState == LOW) {
      Serial.println("ACTION!");
      ledBlinkerIntensityCycle();
    }
  }
}