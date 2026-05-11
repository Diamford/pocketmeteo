#include "TrendAnalyzer.h"

namespace pm::domain {

std::optional<float> TrendAnalyzer::delta3hHpa(const PressureHistory &history) {
  const auto latest = history.latest();
  if (!latest.has_value()) {
    return std::nullopt;
  }

  const auto all = history.samples();
  if (all.size() < 2) {
    return std::nullopt;
  }

  const std::int64_t target = latest->timestamp_s - 3 * 60 * 60;
  const PressureSample *baseline = nullptr;
  for (const auto &sample : all) {
    if (sample.timestamp_s <= target) {
      baseline = &sample;
    } else {
      break;
    }
  }

  if (baseline == nullptr) {
    const auto oldest = history.oldest();
    if (!oldest.has_value()) {
      return std::nullopt;
    }
    const std::int64_t span = latest->timestamp_s - oldest->timestamp_s;
    if (span < 3 * 60 * 60) {
      return std::nullopt;
    }
    baseline = &(*oldest);
  }

  return latest->pressure_hpa - baseline->pressure_hpa;
}

} // namespace pm::domain
