#include "App.h"

#include <Arduino.h>
#include <esp_timer.h>
#include <time.h>

#include "../domain/SeaLevelReducer.h"
#include "../domain/TrendAnalyzer.h"
#include "../domain/ZambrettiForecast.h"
#include "../log/Log.h"
#include "../ui/ScreenRenderer.h"

namespace pm {

namespace {

constexpr float kAltitudeMeters = 120.0f;

std::optional<float> stationToSeaLevelHpa(const hal::SensorReading &reading) {
  const float station_hpa = reading.pressure_pa / 100.0f;
  return domain::SeaLevelReducer::toSeaLevelHpa(station_hpa, kAltitudeMeters,
                                               reading.temperature_c);
}

int zambrettiTrendSign(float delta3h_hpa) {
  if (delta3h_hpa > 1.0f) {
    return 1;
  }
  if (delta3h_hpa < -1.0f) {
    return -1;
  }
  return 0;
}

domain::Season inferSeason() {
  const time_t now = time(nullptr);
  if (now < 1700000000) {
    return domain::Season::Summer;
  }
  struct tm *local = localtime(&now);
  if (local == nullptr) {
    return domain::Season::Summer;
  }
  const int month = local->tm_mon + 1;
  if (month == 12 || month <= 2) {
    return domain::Season::Winter;
  }
  if (month >= 6 && month <= 8) {
    return domain::Season::Summer;
  }
  return domain::Season::Summer;
}

} // namespace

void App::begin() {
  LOG_I("App::begin");
  sensor_ready_ = sensor_.begin();
  if (sensor_ready_) {
    LOG_I("BMP390 initialized");
  } else {
    LOG_E("BMP390 init failed");
  }

  display_ready_ = display_.begin();
  if (display_ready_) {
    LOG_I("e-ink initialized");
  } else {
    LOG_E("e-ink init failed");
  }

  last_measurement_ms_ = millis();
  if (display_ready_) {
    runMeasurementCycle(true);
  }
}

void App::loop() {
  const uint32_t now = millis();
  if (now - last_measurement_ms_ >= kMeasurementPeriodMs) {
    last_measurement_ms_ = now;
    runMeasurementCycle(false);
  }
}

void App::runMeasurementCycle(bool initial_full_refresh) {
  hal::SensorReading reading{};
  bool ok = sensor_ready_ && sensor_.readAveraged(8, reading);
  if (ok) {
    logSensorReading(reading, true);
  } else {
    LOG_W("BMP390 read failed");
  }

  std::optional<float> sea_level_hpa;
  if (ok) {
    sea_level_hpa = stationToSeaLevelHpa(reading);
    if (sea_level_hpa.has_value()) {
      const std::int64_t ts_s =
          static_cast<std::int64_t>(esp_timer_get_time() / 1000000LL);
      history_.append(ts_s, *sea_level_hpa);
    } else {
      ok = false;
    }
  }

  const auto delta = domain::TrendAnalyzer::delta3hHpa(history_);
  int sign = 0;
  if (delta.has_value()) {
    sign = zambrettiTrendSign(*delta);
  }
  bool trend_changed = false;
  if (last_trend_sign_.has_value() && sign != *last_trend_sign_) {
    trend_changed = true;
  }
  last_trend_sign_ = sign;

  domain::ZambrettiResult forecast{};
  if (sea_level_hpa.has_value()) {
    forecast = domain::ZambrettiForecast::forecast(*sea_level_hpa,
                                                    delta.value_or(0.0f),
                                                    inferSeason());
    forecast_storage_ = std::move(forecast.text_ru);
  } else {
    forecast_storage_.clear();
  }

  if (!display_ready_) {
    return;
  }

  ++partial_cycle_count_;
  const bool full_every_12 = (partial_cycle_count_ % 12) == 0;
  const bool use_full =
      initial_full_refresh || full_every_12 || trend_changed;

  ui::StateSnapshot snapshot{};
  snapshot.sensor_ok = ok && sea_level_hpa.has_value();
  snapshot.temp_c = reading.temperature_c;
  snapshot.pressure_slp_hpa = sea_level_hpa.value_or(0.0f);
  snapshot.delta_3h_hpa = delta;
  snapshot.forecast_ru = forecast_storage_;

  ui::RenderPolicy policy{};
  policy.refresh = use_full ? ui::RenderRefresh::FullScreen
                            : ui::RenderRefresh::PartialRegions;
  policy.dirty_regions = ui::kAllScreenRegions;

  ui::ScreenRenderer::render(display_, snapshot, policy);
}

void App::logSensorReading(const hal::SensorReading &reading, bool ok) {
  (void)ok;
  char buffer[96];
  snprintf(buffer, sizeof(buffer), "sensor t=%.2fC p=%.1fPa",
           reading.temperature_c, reading.pressure_pa);
  LOG_I(buffer);
}

} // namespace pm
