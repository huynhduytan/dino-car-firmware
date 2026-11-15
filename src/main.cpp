#include <Arduino.h>
#include "pins.h"

int lastState = HIGH;
unsigned long pressCount = 0;
void setup() {
  Serial.begin(115200);
  pinMode(PIN_LED_ONBOARD, OUTPUT);
  pinMode(PIN_BUTTON_BOOT, INPUT_PULLUP);   // not pressed = HIGH, pressed = LOW
  Serial.println("B1 ready - press BOOT button");
}

void loop() {
  int state = digitalRead(PIN_BUTTON_BOOT);
  digitalWrite(PIN_LED_ONBOARD, state == LOW ? HIGH : LOW);
  if (lastState == HIGH && state == LOW) {   // print only on the falling edge (press)
    pressCount++;
    Serial.printf("pressed #%lu\n", pressCount);
  }
  lastState = state;
}