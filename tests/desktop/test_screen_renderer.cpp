#include <stdexcept>
#include <utility>
#include <vector>

#include "hal/IDisplay.h"
#include "ui/ScreenRenderer.h"

namespace {

class MockDisplay final : public pm::hal::IDisplay {
public:
  bool begin() override { return true; }

  void clear(pm::hal::DisplayUpdateMode mode,
             const pm::hal::PartialRect *area) override {
    clears_.push_back(
        {mode, area ? std::optional<pm::hal::PartialRect>(*area) : std::nullopt});
  }

  void drawText(std::string_view, const pm::hal::TextStyle &) override {}

  bool commit(pm::hal::DisplayUpdateMode mode) override {
    commits_.push_back(mode);
    return true;
  }

  const std::vector<
      std::pair<pm::hal::DisplayUpdateMode, std::optional<pm::hal::PartialRect>>> &
  clears() const {
    return clears_;
  }

  const std::vector<pm::hal::DisplayUpdateMode> &commits() const {
    return commits_;
  }

private:
  std::vector<
      std::pair<pm::hal::DisplayUpdateMode, std::optional<pm::hal::PartialRect>>>
      clears_;
  std::vector<pm::hal::DisplayUpdateMode> commits_;
};

void expect(bool condition, const char *message) {
  if (!condition) {
    throw std::runtime_error(message);
  }
}

} // namespace

void runScreenRendererTests() {
  MockDisplay display;
  pm::ui::StateSnapshot state{};
  state.sensor_ok = true;
  state.temp_c = 20.0f;
  state.pressure_slp_hpa = 1013.2f;
  state.delta_3h_hpa = 0.5f;
  state.forecast_ru = "Тест прогноза";

  pm::ui::RenderPolicy full{};
  full.refresh = pm::ui::RenderRefresh::FullScreen;
  pm::ui::ScreenRenderer::render(display, state, full);

  expect(display.clears().size() == 1, "full refresh should issue one clear");
  expect(display.clears()[0].first == pm::hal::DisplayUpdateMode::Full,
         "full refresh clear mode");
  expect(!display.clears()[0].second.has_value(), "full refresh clears full panel");
  expect(display.commits().size() == 1, "full refresh should issue one commit");
  expect(display.commits()[0] == pm::hal::DisplayUpdateMode::Full,
         "full refresh commit mode");

  MockDisplay partial_display;
  pm::ui::RenderPolicy partial{};
  partial.refresh = pm::ui::RenderRefresh::PartialRegions;
  partial.dirty_regions = pm::ui::kAllScreenRegions;
  pm::ui::ScreenRenderer::render(partial_display, state, partial);

  expect(partial_display.clears().size() == 4,
         "partial refresh should clear each region");
  expect(partial_display.commits().size() == 4,
         "partial refresh should commit each region");
  for (const auto &c : partial_display.clears()) {
    expect(c.first == pm::hal::DisplayUpdateMode::Partial, "partial clear mode");
    expect(c.second.has_value(), "partial clear should specify a rectangle");
  }
}
