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

} // namespace pm

