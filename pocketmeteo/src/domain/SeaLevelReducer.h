#pragma once

#include <optional>

namespace pm::domain {

class SeaLevelReducer {
public:
  // Converts station pressure (hPa) to sea-level pressure (hPa).
  static std::optional<float> toSeaLevelHpa(float station_pressure_hpa,
                                            float altitude_m,
                                            float temperature_c);
};

} // namespace pm::domain
