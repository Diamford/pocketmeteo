#include "App.h"

#include <Arduino.h>

#include "../log/Log.h"

namespace pm {

void App::begin() {
  LOG_I("App::begin");
  sensor_ready_ = sensor_.begin();
  if (sensor_ready_) {
    LOG_I("BMP390 initialized");
  } else {
    LOG_E("BMP390 init failed");
  }

  display_ready_ = display_.begin();
  if (display_ready_) {
    LOG_I("e-ink initialized");
    drawBootScreen();
  } else {
    LOG_E("e-ink init failed");
  }
}

void App::loop() {
  // Minimal "alive" heartbeat without blocking.
  static uint32_t last_ms = 0;
  const uint32_t now_ms = millis();
  if (now_ms - last_ms >= 10000) {
    last_ms = now_ms;
    logSensorReading();
  }
}

void App::logSensorReading() {
  if (!sensor_ready_) {
    LOG_W("sensor not ready");
    return;
  }

  hal::SensorReading reading{};
  if (!sensor_.readAveraged(8, reading)) {
    LOG_W("BMP390 read failed");
    return;
  }

  char buffer[96];
  snprintf(buffer, sizeof(buffer), "sensor t=%.2fC p=%.1fPa", reading.temperature_c,
           reading.pressure_pa);
  LOG_I(buffer);
}

void App::drawBootScreen() {
  if (!display_ready_) {
    return;
  }

  display_.clear(hal::DisplayUpdateMode::Full);
  display_.drawText("POCKETMETEO", hal::TextStyle{8, 40, 0, false});
  display_.drawText("Погода запущена", hal::TextStyle{8, 70, 0, true});
  display_.drawText("Датчик BMP390 активен",
                    hal::TextStyle{8, 96, 0, true});
  display_.commit(hal::DisplayUpdateMode::Full);
}

} // namespace pm

