# dino-car-firmware

Offline ESP32 toy car that teaches English vocabulary to kids via RFID cards — PlatformIO/Arduino firmware for the Edu-Car project.

## Tiến độ

| Buổi | Ngày | Nội dung | Trạng thái |
|---|---|---|---|
| B1 | 14/11/2026 | Cài đặt và "Hello LED": build → flash → monitor, GPIO, INPUT_PULLUP | ✅ Xong |
| B2 | 21/11/2026 | Không dùng delay(): millis(), chống dội phím, buzzer GPIO 19 bằng tone(), FSM nhỏ | ✅ Xong (làm sớm 15/11) |
| B3 | 28/11/2026 | FreeRTOS task + queue, ngắt IRAM_ATTR | ⬜ |
| B4 | 05/12/2026 | Đọc thẻ RFID RC522 qua SPI, đo tầm đọc | ⬜ |
| B5 | 12/12/2026 | DFPlayer Mini qua UART2, module audio không chặn | ⬜ |
| B6 | 19/12/2026 | Luật chơi: game.cpp FSM, cards.json LittleFS, unit test native | ⬜ |
| B7–B9 | 12/2026–01/2027 | Xem docs/plan.md | ⬜ |
| B10 | 16/01/2027 | Remote 4 nút cho bé (ESP-NOW) | ⬜ |
| B11 | 23/01/2027 | Ghép trò chơi lên xe + buổi thử với bé | ⬜ |
| B12 | 30/01/2027 | Sửa theo phản hồi + Video 2 (MVP) | ⬜ |

Nhật ký chi tiết từng buổi: [docs/journal/](docs/journal/)
