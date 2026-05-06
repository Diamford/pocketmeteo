#include <Arduino.h>

#include "src/app/App.h"
#include "src/log/Log.h"

static pm::App g_app;

void setup() {
  // Recommended: Tools → USB CDC On Boot → Enabled
  Serial.begin(115200);
  delay(200);

  pm::log::begin();
  LOG_I("pocketmeteo boot");

  g_app.begin();
}

void loop() {
  g_app.loop();
}

