#include <Arduino.h>
#include "pins.h"

const unsigned long DEBOUNCE_MS = 30;
int lastRawState = HIGH;
int stableState = HIGH;
unsigned long pressCount = 0;
unsigned long lastChangeTime = 0;

const int happyNotes[] = {523, 659, 784, 1047}; // Đô-Mi-Sol-Đô (quãng 8), giai điệu đi lên = "vui"
const int happyDurations[] = {100, 100, 100, 150};
const int sadNotes[] = {392, 262}; // Sol xuống Đô (quãng 8 thấp), đi xuống chậm = "buồn"
const int sadDurations[] = {250, 350};

const int *melodyNotes = nullptr;
const int *melodyDurations = nullptr;
int melodyLength = 0;
int melodyNoteIndex = 0;
unsigned long noteStartTime = 0;
bool melodyPlaying = false;

void startMelody(const int notes[], const int durations[], int length)
{
  melodyNotes = notes;
  melodyDurations = durations;
  melodyLength = length;
  melodyNoteIndex = 0;
  noteStartTime = millis();
  melodyPlaying = true;
  tone(PIN_BUZZER, melodyNotes[0], melodyDurations[0]);
}

void updateMelody()
{
  if (!melodyPlaying)
    return;
  if (millis() - noteStartTime >= (unsigned long)melodyDurations[melodyNoteIndex])
  {
    melodyNoteIndex++;
    if (melodyNoteIndex >= melodyLength)
    {
      melodyPlaying = false;
      return;
    }
    noteStartTime = millis();
    tone(PIN_BUZZER, melodyNotes[melodyNoteIndex], melodyDurations[melodyNoteIndex]);
  }
}

enum State
{
  IDLE,
  PLAYING
};
State state = IDLE;

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
  updateMelody();

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
      static bool nextIsHappy = true;
      if (nextIsHappy)
      {
        startMelody(happyNotes, happyDurations, 4);
      }
      else
      {
        startMelody(sadNotes, sadDurations, 2);
      }
      nextIsHappy = !nextIsHappy;
      digitalWrite(PIN_LED, HIGH);
      state = PLAYING;
    }
  }
  if (state == PLAYING && !melodyPlaying)
  {
    digitalWrite(PIN_LED, LOW);
    state = IDLE;
  }

  lastRawState = raw;
}