#pragma once

#include "../drivers/Bmp390Driver.h"

namespace pm {

class App {
public:
  void begin();
  void loop();

private:
  void logSensorReading();

  drivers::Bmp390Driver sensor_{};
  bool sensor_ready_{false};
};

} // namespace pm

