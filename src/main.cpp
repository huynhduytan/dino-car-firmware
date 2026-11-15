#include <Arduino.h>
#include "pins.h"

int lastState = HIGH;
unsigned long pressCount = 0;
void setup() {
  Serial.begin(115200);
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_BUTTON, INPUT_PULLUP);   // not pressed = HIGH, pressed = LOW
  Serial.println("B1 ready - press button");
}

void loop() {
  int state = digitalRead(PIN_BUTTON);
  digitalWrite(PIN_LED, state == LOW ? HIGH : LOW);
  if (lastState == HIGH && state == LOW) {   // print only on the falling edge (press)
    pressCount++;
    Serial.printf("pressed #%lu\n", pressCount);
  }
  lastState = state;
}