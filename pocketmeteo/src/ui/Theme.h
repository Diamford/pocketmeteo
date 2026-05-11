#pragma once

#include <cstdint>
#include <string_view>

namespace pm::ui {

/// Typography and spacing for the 2.7" panel (rotation 1, FreeMonoBold9pt).
struct Theme {
  static constexpr std::int16_t kMarginX = 8;
  static constexpr std::uint16_t kInkBlack = 0;

  static constexpr std::int16_t kTitleBaseline = 38;
  static constexpr std::int16_t kMetric1Baseline = 64;
  static constexpr std::int16_t kMetric2Baseline = 86;
  static constexpr std::int16_t kTrendBaseline = 112;
  static constexpr std::int16_t kForecastFirstBaseline = 142;
  static constexpr int kForecastLineStep = 14;
  static constexpr int kForecastMaxLines = 8;
  static constexpr int kForecastCharsPerLine = 22;

  static constexpr std::string_view kAppTitleRu = "КАРМАН-МЕТЕО";
  static constexpr std::string_view kLabelTemp = "Темп.: ";
  static constexpr std::string_view kLabelPressure = "Давл.: ";
  static constexpr std::string_view kLabelPressureUnit = " гПа (н.м.)";
  static constexpr std::string_view kLabelTrend = "Тренд 3ч: ";
  static constexpr std::string_view kTrendNoData = "нет данных";
  static constexpr std::string_view kSensorFault = "Нет данных датчика";
};

} // namespace pm::ui
