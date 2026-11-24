#include <Arduino.h>
#include "pins.h"

const int happyNotes[] = {523, 659, 784, 1047}; // Đô-Mi-Sol-Đô (quãng 8), giai điệu đi lên = "vui"
const int happyDurations[] = {100, 100, 100, 150};
const int sadNotes[] = {392, 262}; // Sol xuống Đô (quãng 8 thấp), đi xuống chậm = "buồn"
const int sadDurations[] = {250, 350};

enum MelodyType { MELODY_HAPPY, MELODY_SAD };

QueueHandle_t buttonEventQueue;
QueueHandle_t melodyQueue;
QueueHandle_t ledQueue;

void IRAM_ATTR buttonISR() {
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  uint8_t dummy = 0;
  xQueueSendFromISR(buttonEventQueue, &dummy, &xHigherPriorityTaskWoken);
  portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void taskButton(void *param) {
  uint8_t dummy;
  int stableState = HIGH;
  unsigned long pressCount = 0;
  bool nextIsHappy = true;
  for (;;) {
    xQueueReceive(buttonEventQueue, &dummy, portMAX_DELAY);  // ngủ cho tới khi có ngắt
    vTaskDelay(pdMS_TO_TICKS(30));                           // chờ dội phím lắng xuống
    int raw = digitalRead(PIN_BUTTON);
    if (raw != stableState) {
      stableState = raw;
      if (stableState == LOW) {
        pressCount++;
        Serial.printf("[btn] pressed #%lu (core %d, stack left %u words)\n",
                      pressCount, xPortGetCoreID(), uxTaskGetStackHighWaterMark(NULL));
        MelodyType type = nextIsHappy ? MELODY_HAPPY : MELODY_SAD;
        xQueueSend(melodyQueue, &type, 0);   // báo tin cho taskBuzzer, không chờ
        nextIsHappy = !nextIsHappy;
      }
    }
  }
}

void taskLed(void *param) {
  bool ledOn;
  for (;;) {
    xQueueReceive(ledQueue, &ledOn, portMAX_DELAY);   // ngủ cho tới khi taskBuzzer báo bật/tắt
    digitalWrite(PIN_LED, ledOn ? HIGH : LOW);
  }
}

void taskBuzzer(void *param) {
  MelodyType type;
  for (;;) {
    xQueueReceive(melodyQueue, &type, portMAX_DELAY);   // ngủ cho tới khi có lệnh phát nhạc
    const int *notes = (type == MELODY_HAPPY) ? happyNotes : sadNotes;
    const int *durations = (type == MELODY_HAPPY) ? happyDurations : sadDurations;
    int length = (type == MELODY_HAPPY) ? 4 : 2;
    Serial.printf("[buzzer] playing %s (core %d, stack left %u words)\n",
                  type == MELODY_HAPPY ? "happy" : "sad", xPortGetCoreID(),
                  uxTaskGetStackHighWaterMark(NULL));

    bool ledOn = true;
    xQueueSend(ledQueue, &ledOn, 0);
    for (int i = 0; i < length; i++) {
      tone(PIN_BUZZER, notes[i], durations[i]);
      vTaskDelay(pdMS_TO_TICKS(durations[i]));   // chặn CHỈ task này — task khác (taskButton, loop) vẫn chạy bình thường
    }
    bool ledOff = false;
    xQueueSend(ledQueue, &ledOff, 0);
  }
}

void setup()
{
  Serial.begin(115200);
  pinMode(PIN_BUTTON, INPUT_PULLUP);
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_LED, OUTPUT);

  buttonEventQueue = xQueueCreate(10, sizeof(uint8_t));
  melodyQueue = xQueueCreate(4, sizeof(MelodyType));
  ledQueue = xQueueCreate(4, sizeof(bool));
  xTaskCreate(taskButton, "taskButton", 2048, NULL, 1, NULL);
  xTaskCreate(taskBuzzer, "taskBuzzer", 2048, NULL, 1, NULL);
  xTaskCreate(taskLed, "taskLed", 2048, NULL, 1, NULL);
  attachInterrupt(digitalPinToInterrupt(PIN_BUTTON), buttonISR, CHANGE);

  Serial.println("B3 step 3 ready - taskButton + taskBuzzer + taskLed via queue");
}

void loop()
{
  // Để trống — mọi việc đã chuyển sang 3 task riêng (taskButton, taskBuzzer, taskLed).
}