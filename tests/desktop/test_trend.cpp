#include "../../pocketmeteo/src/domain/PressureHistory.h"
#include "../../pocketmeteo/src/domain/TrendAnalyzer.h"

#include <cmath>
#include <stdexcept>

namespace {

void require(bool condition, const char *message) {
  if (!condition) {
    throw std::runtime_error(message);
  }
}

} // namespace

void runTrendTests() {
  pm::domain::PressureHistory history(19);
  const std::int64_t base = 1'000;

  for (int i = 0; i < 19; ++i) {
    history.append(base + i * 600, 1000.0f + static_cast<float>(i) * 0.2f);
  }

  const auto delta = pm::domain::TrendAnalyzer::delta3hHpa(history);
  require(delta.has_value(), "trend should exist for 3h history");
  require(std::fabs(*delta - 3.6f) < 0.05f, "unexpected trend value");

  pm::domain::PressureHistory short_history(19);
  short_history.append(base, 1000.0f);
  short_history.append(base + 600, 1000.2f);
  const auto short_delta = pm::domain::TrendAnalyzer::delta3hHpa(short_history);
  require(!short_delta.has_value(), "short history must not produce 3h trend");
}
