#pragma once

#include "../domain/PressureHistory.h"
#include "../drivers/Bmp390Driver.h"
#include "../drivers/Eink270Driver.h"

#include <cstdint>
#include <optional>
#include <string>

namespace pm {

class App {
public:
  void begin();
  void loop();

private:
  void runMeasurementCycle(bool initial_full_refresh);
  void logSensorReading(const hal::SensorReading &reading, bool ok);

  drivers::Bmp390Driver sensor_{};
  drivers::Eink270Driver display_{};
  domain::PressureHistory history_{};
  bool sensor_ready_{false};
  bool display_ready_{false};
  std::optional<int> last_trend_sign_{};
  std::uint32_t partial_cycle_count_{0};
  std::uint32_t last_measurement_ms_{0};
  std::string forecast_storage_{};

  static constexpr std::uint32_t kMeasurementPeriodMs =
      10UL * 60UL * 1000UL;
};

} // namespace pm
