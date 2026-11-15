#include <Arduino.h>
#include "pins.h"

const unsigned long DEBOUNCE_MS = 30;
int lastRawState = HIGH;
int stableState = HIGH;
unsigned long pressCount = 0;
unsigned long lastChangeTime = 0;
enum State
{
  IDLE,
  ACTIVE
};
State state = IDLE;
unsigned long activeStartTime = 0;
const unsigned long ACTIVE_DURATION_MS = 300;
void setup()
{
  Serial.begin(115200);
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_BUTTON, INPUT_PULLUP);
  pinMode(PIN_BUZZER, OUTPUT);
  Serial.println("ready - press button");
}

void loop()
{
  int raw = digitalRead(PIN_BUTTON);

  if (raw != lastRawState)
  {
    lastChangeTime = millis();
  }
  if (millis() - lastChangeTime > DEBOUNCE_MS && raw != stableState)
  {
    stableState = raw;
    digitalWrite(PIN_LED, stableState == LOW ? HIGH : LOW);
    if (stableState == LOW && state == IDLE)
    { // print only on the falling edge (press)
      pressCount++;
      Serial.printf("pressed #%lu\n", pressCount);
      tone(PIN_BUZZER, 1000, 100);
      digitalWrite(PIN_LED, HIGH);
      state = ACTIVE;
      activeStartTime = millis();
    }
  }
  if (state == ACTIVE && millis() - activeStartTime >= ACTIVE_DURATION_MS)
  {
    digitalWrite(PIN_LED, LOW);
    state = IDLE;
  }

  lastRawState = raw;
}