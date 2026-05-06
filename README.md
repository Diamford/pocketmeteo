# pocketmeteo

Метеостанция на **ESP32‑C3** с датчиком **BMP390** и дисплеем **Waveshare 2.7" e‑Ink (264×176)**. Wi‑Fi постоянно включён: устройство поднимает веб‑дашборд и REST/JSON API, синхронизирует время по NTP, измеряет давление/температуру по расписанию (каждые 10 минут) и отображает прогноз по **Zambretti** (на экране — только русский).

## Железо

- **MCU**: ESP32‑C3 (например, `ESP32C3 Dev Module`, 4MB flash)
- **Датчик**: Bosch **BMP390** по I²C
- **Дисплей**: Waveshare **2.7" e‑Ink** (монохромный) — драйвер `GxEPD2_270`
- **Подключение дисплея (SPI, как в архитектуре)**:
  - `CS=10, DC=4, RST=3, BUSY=2, SCLK=6, MOSI=7`

## ПО и зависимости

- **Arduino IDE**: 2.x
- **ESP32 core**: `esp32 by Espressif Systems` ≥ 2.0.14
- Библиотеки (планируемые, через Library Manager / `arduino-cli lib install`):
  - `Adafruit BMP3XX Library` (BMP390)
  - `GxEPD2` (e‑Ink)
  - `ArduinoJson` ≥ 7.x (REST/JSON)
- Используемые компоненты ESP32 core (обычно без отдельной установки): `WiFi`, `WebServer`, `DNSServer`, `ESPmDNS`, `LittleFS`, `Preferences`, `Update`.

## Документация

- Архитектура: [`docs/ARCHITECTURE.md`](docs/ARCHITECTURE.md)
- План разработки: [`docs/DEVELOPMENT_PLAN.md`](docs/DEVELOPMENT_PLAN.md)

## Быстрый старт (Arduino IDE 2.x)

1. Установите Arduino IDE 2.x.
2. В **Boards Manager** установите **ESP32 by Espressif Systems** (версия ≥ 2.0.14).
3. Выберите плату: **Tools → Board → ESP32 Arduino → ESP32C3 Dev Module**.
4. Рекомендуемые настройки (согласно принятому плану):
   - **Flash Size**: `4MB`
   - **Partition Scheme**: `Default 4MB with spiffs (1.2MB APP / 1.5MB SPIFFS)` (под LittleFS)
   - **CPU Freq**: `160 MHz`
   - **USB CDC On Boot**: `Enabled` (логи)
5. Откройте скетч `pocketmeteo/pocketmeteo.ino` и прошейте устройство.

> На данный момент в репозитории зафиксированы архитектура и план. Реализация скетча и модулей будет добавляться по фазам из `docs/DEVELOPMENT_PLAN.md`.

## Быстрый старт (arduino-cli)

Примерные команды для воспроизводимой сборки (версии и путь к скетчу уточняются по мере реализации):

```bash
arduino-cli core update-index
arduino-cli core install esp32:esp32@<pinned>
arduino-cli lib install "Adafruit BMP3XX Library"
arduino-cli lib install "GxEPD2"
arduino-cli lib install "ArduinoJson"

arduino-cli compile -b esp32:esp32:esp32c3 pocketmeteo/
```

## Лицензия

Будет добавлена при первом релизе (см. фазу P10 в `docs/DEVELOPMENT_PLAN.md`).

