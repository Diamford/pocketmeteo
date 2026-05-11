#include "SeaLevelReducer.h"

#include <cmath>

namespace pm::domain {

std::optional<float> SeaLevelReducer::toSeaLevelHpa(float station_pressure_hpa,
                                                    float altitude_m,
                                                    float temperature_c) {
  if (!std::isfinite(station_pressure_hpa) || !std::isfinite(altitude_m) ||
      !std::isfinite(temperature_c) || station_pressure_hpa <= 0.0f ||
      altitude_m < 0.0f) {
    return std::nullopt;
  }

  const float kelvin = temperature_c + 273.15f;
  if (kelvin <= 0.0f) {
    return std::nullopt;
  }

  // Barometric reduction with ISA lapse rate approximation.
  const float exponent = 5.257f;
  const float lapse_rate = 0.0065f;
  const float denom = kelvin + lapse_rate * altitude_m;
  const float base = 1.0f - (lapse_rate * altitude_m) / denom;
  if (base <= 0.0f) {
    return std::nullopt;
  }

  const float sea_level_hpa = station_pressure_hpa / std::pow(base, exponent);
  return sea_level_hpa;
}

} // namespace pm::domain
