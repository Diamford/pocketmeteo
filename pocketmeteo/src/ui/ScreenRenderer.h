#pragma once

#include "../hal/IDisplay.h"
#include "Layout.h"

#include <cstdint>
#include <optional>
#include <string_view>

namespace pm::ui {

struct StateSnapshot {
  float temp_c{0};
  float pressure_slp_hpa{0};
  std::optional<float> delta_3h_hpa;
  std::string_view forecast_ru;
  bool sensor_ok{false};
};

enum class RenderRefresh : std::uint8_t {
  PartialRegions = 0,
  FullScreen = 1,
};

struct RenderPolicy {
  RenderRefresh refresh{RenderRefresh::PartialRegions};
  std::uint8_t dirty_regions{kAllScreenRegions};
};

class ScreenRenderer {
public:
  static void render(hal::IDisplay &display, const StateSnapshot &state,
                     const RenderPolicy &policy);

private:
  static void renderFull(hal::IDisplay &display, const StateSnapshot &state);
  static void renderPartial(hal::IDisplay &display, const StateSnapshot &state,
                            std::uint8_t dirty_regions);

  static void paintHeader(hal::IDisplay &display, const StateSnapshot &state,
                          hal::DisplayUpdateMode mode,
                          const hal::PartialRect *partial_clip);
  static void paintMetrics(hal::IDisplay &display, const StateSnapshot &state,
                           hal::DisplayUpdateMode mode,
                           const hal::PartialRect *partial_clip);
  static void paintTrend(hal::IDisplay &display, const StateSnapshot &state,
                         hal::DisplayUpdateMode mode,
                         const hal::PartialRect *partial_clip);
  static void paintForecast(hal::IDisplay &display, const StateSnapshot &state,
                            hal::DisplayUpdateMode mode,
                            const hal::PartialRect *partial_clip);
};

} // namespace pm::ui
