#include "App.h"

#include <Arduino.h>

#include "../log/Log.h"

namespace pm {

void App::begin() {
  LOG_I("App::begin");
}

void App::loop() {
  // Minimal "alive" heartbeat without blocking.
  static uint32_t last_ms = 0;
  const uint32_t now_ms = millis();
  if (now_ms - last_ms >= 1000) {
    last_ms = now_ms;
    LOG_I("tick");
  }
}

} // namespace pm

