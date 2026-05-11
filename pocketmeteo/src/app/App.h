#pragma once

#include "../drivers/Bmp390Driver.h"
#include "../drivers/Eink270Driver.h"

namespace pm {

class App {
public:
  void begin();
  void loop();

private:
  void logSensorReading();
  void drawBootScreen();

  drivers::Bmp390Driver sensor_{};
  drivers::Eink270Driver display_{};
  bool sensor_ready_{false};
  bool display_ready_{false};
};

} // namespace pm

