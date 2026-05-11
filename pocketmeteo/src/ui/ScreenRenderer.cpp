#include "ScreenRenderer.h"

#include "Theme.h"

#include <cstdio>
#include <cstring>

namespace pm::ui {

namespace {

/// Greedy word wrap for Russian forecast lines (UTF-8 aware, space-separated).
void wrapForecastUtf8(std::string_view text, int max_chars_per_line,
                      char *lines, std::size_t line_stride, int max_lines,
                      int &out_line_count) {
  out_line_count = 0;
  if (max_lines <= 0 || text.empty()) {
    return;
  }

  std::size_t line_start = 0;
  while (line_start < text.size() && out_line_count < max_lines) {
    std::size_t line_end = text.size();
    std::size_t last_space = std::string_view::npos;
    int visual = 0;
    for (std::size_t i = line_start; i < text.size(); ++i) {
      const unsigned char c = static_cast<unsigned char>(text[i]);
      bool is_space = (c == ' ' || c == '\t');
      if ((c & 0xC0) != 0x80) {
        ++visual;
      }
      if (is_space) {
        last_space = i;
      }
      if (visual > max_chars_per_line) {
        if (last_space != std::string_view::npos && last_space > line_start) {
          line_end = last_space;
        } else {
          line_end = i;
        }
        break;
      }
    }

    char *row = lines + static_cast<std::size_t>(out_line_count) * line_stride;
    const std::size_t copy_len = line_end - line_start;
    const std::size_t cap = line_stride - 1;
    const std::size_t n = copy_len < cap ? copy_len : cap;
    if (n > 0) {
      std::memcpy(row, text.data() + line_start, n);
    }
    row[n] = '\0';

    ++out_line_count;
    while (line_end < text.size() &&
           (text[line_end] == ' ' || text[line_end] == '\t')) {
      ++line_end;
    }
    line_start = line_end;
  }
}

bool regionSet(std::uint8_t mask, ScreenRegion bit) {
  return (mask & static_cast<std::uint8_t>(bit)) != 0;
}

} // namespace

void ScreenRenderer::render(hal::IDisplay &display, const StateSnapshot &state,
                            const RenderPolicy &policy) {
  if (policy.refresh == RenderRefresh::FullScreen) {
    renderFull(display, state);
    return;
  }
  const std::uint8_t dirty =
      policy.dirty_regions == 0 ? kAllScreenRegions : policy.dirty_regions;
  renderPartial(display, state, dirty);
}

void ScreenRenderer::renderFull(hal::IDisplay &display,
                                const StateSnapshot &state) {
  display.clear(hal::DisplayUpdateMode::Full, nullptr);
  paintHeader(display, state, hal::DisplayUpdateMode::Full, nullptr);
  paintMetrics(display, state, hal::DisplayUpdateMode::Full, nullptr);
  paintTrend(display, state, hal::DisplayUpdateMode::Full, nullptr);
  paintForecast(display, state, hal::DisplayUpdateMode::Full, nullptr);
  display.commit(hal::DisplayUpdateMode::Full);
}

void ScreenRenderer::renderPartial(hal::IDisplay &display,
                                   const StateSnapshot &state,
                                   std::uint8_t dirty_regions) {
  if (regionSet(dirty_regions, ScreenRegion::Header)) {
    const hal::PartialRect clip = headerClip();
    display.clear(hal::DisplayUpdateMode::Partial, &clip);
    paintHeader(display, state, hal::DisplayUpdateMode::Partial, &clip);
    display.commit(hal::DisplayUpdateMode::Partial);
  }
  if (regionSet(dirty_regions, ScreenRegion::Metrics)) {
    const hal::PartialRect clip = metricsClip();
    display.clear(hal::DisplayUpdateMode::Partial, &clip);
    paintMetrics(display, state, hal::DisplayUpdateMode::Partial, &clip);
    display.commit(hal::DisplayUpdateMode::Partial);
  }
  if (regionSet(dirty_regions, ScreenRegion::Trend)) {
    const hal::PartialRect clip = trendClip();
    display.clear(hal::DisplayUpdateMode::Partial, &clip);
    paintTrend(display, state, hal::DisplayUpdateMode::Partial, &clip);
    display.commit(hal::DisplayUpdateMode::Partial);
  }
  if (regionSet(dirty_regions, ScreenRegion::Forecast)) {
    const hal::PartialRect clip = forecastClip();
    display.clear(hal::DisplayUpdateMode::Partial, &clip);
    paintForecast(display, state, hal::DisplayUpdateMode::Partial, &clip);
    display.commit(hal::DisplayUpdateMode::Partial);
  }
}

void ScreenRenderer::paintHeader(hal::IDisplay &display,
                                 const StateSnapshot &state,
                                 hal::DisplayUpdateMode mode,
                                 const hal::PartialRect *partial_clip) {
  (void)state;
  (void)mode;
  (void)partial_clip;
  hal::TextStyle style{Theme::kMarginX, Theme::kTitleBaseline, Theme::kInkBlack,
                       true};
  display.drawText(Theme::kAppTitleRu, style);
}

void ScreenRenderer::paintMetrics(hal::IDisplay &display,
                                  const StateSnapshot &state,
                                  hal::DisplayUpdateMode mode,
                                  const hal::PartialRect *partial_clip) {
  (void)mode;
  (void)partial_clip;
  char line[96];
  if (!state.sensor_ok) {
    hal::TextStyle st{Theme::kMarginX, Theme::kMetric1Baseline,
                      Theme::kInkBlack, true};
    display.drawText(Theme::kSensorFault, st);
    return;
  }

  std::snprintf(line, sizeof(line), "%s%.1f C", Theme::kLabelTemp.data(),
                static_cast<double>(state.temp_c));
  display.drawText(line, hal::TextStyle{Theme::kMarginX, Theme::kMetric1Baseline,
                                        Theme::kInkBlack, true});

  std::snprintf(line, sizeof(line), "%s%.1f%s", Theme::kLabelPressure.data(),
                static_cast<double>(state.pressure_slp_hpa),
                Theme::kLabelPressureUnit.data());
  display.drawText(line, hal::TextStyle{Theme::kMarginX, Theme::kMetric2Baseline,
                                        Theme::kInkBlack, true});
}

void ScreenRenderer::paintTrend(hal::IDisplay &display,
                                const StateSnapshot &state,
                                hal::DisplayUpdateMode mode,
                                const hal::PartialRect *partial_clip) {
  (void)mode;
  (void)partial_clip;
  char line[96];
  if (!state.delta_3h_hpa.has_value()) {
    std::snprintf(line, sizeof(line), "%s%s", Theme::kLabelTrend.data(),
                  Theme::kTrendNoData.data());
  } else {
    std::snprintf(line, sizeof(line), "%s%+.2f гПа", Theme::kLabelTrend.data(),
                  static_cast<double>(*state.delta_3h_hpa));
  }
  display.drawText(line, hal::TextStyle{Theme::kMarginX, Theme::kTrendBaseline,
                                        Theme::kInkBlack, true});
}

void ScreenRenderer::paintForecast(hal::IDisplay &display,
                                   const StateSnapshot &state,
                                   hal::DisplayUpdateMode mode,
                                   const hal::PartialRect *partial_clip) {
  (void)mode;
  (void)partial_clip;
  if (state.forecast_ru.empty()) {
    return;
  }

  constexpr int kMaxLines = Theme::kForecastMaxLines;
  constexpr std::size_t kStride = 96;
  char buffer[static_cast<std::size_t>(kMaxLines) * kStride]{};
  int line_count = 0;
  wrapForecastUtf8(state.forecast_ru, Theme::kForecastCharsPerLine, buffer,
                   kStride, kMaxLines, line_count);

  std::int16_t y = Theme::kForecastFirstBaseline;
  for (int i = 0; i < line_count; ++i) {
    const char *row = buffer + static_cast<std::size_t>(i) * kStride;
    display.drawText(row, hal::TextStyle{Theme::kMarginX, y, Theme::kInkBlack,
                                         true});
    y = static_cast<std::int16_t>(y + Theme::kForecastLineStep);
  }
}

} // namespace pm::ui
