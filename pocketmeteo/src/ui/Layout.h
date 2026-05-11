#pragma once

#include "../hal/IDisplay.h"

#include <cstdint>

namespace pm::ui {

inline constexpr std::int16_t kPanelW = 176;
inline constexpr std::int16_t kPanelH = 264;

enum class ScreenRegion : std::uint8_t {
  Header = 1 << 0,
  Metrics = 1 << 1,
  Trend = 1 << 2,
  Forecast = 1 << 3,
};

inline constexpr std::uint8_t kAllScreenRegions =
    static_cast<std::uint8_t>(ScreenRegion::Header) |
    static_cast<std::uint8_t>(ScreenRegion::Metrics) |
    static_cast<std::uint8_t>(ScreenRegion::Trend) |
    static_cast<std::uint8_t>(ScreenRegion::Forecast);

inline constexpr hal::PartialRect headerClip() {
  return {0, 4, static_cast<std::uint16_t>(kPanelW), 44};
}

inline constexpr hal::PartialRect metricsClip() {
  return {0, 52, static_cast<std::uint16_t>(kPanelW), 44};
}

inline constexpr hal::PartialRect trendClip() {
  return {0, 96, static_cast<std::uint16_t>(kPanelW), 28};
}

inline constexpr hal::PartialRect forecastClip() {
  return {0, 124, static_cast<std::uint16_t>(kPanelW),
          static_cast<std::uint16_t>(kPanelH - 124)};
}

} // namespace pm::ui
