#include "../../pocketmeteo/src/domain/PressureHistory.h"

#include <cmath>
#include <stdexcept>

namespace {

void require(bool condition, const char *message) {
  if (!condition) {
    throw std::runtime_error(message);
  }
}

} // namespace

void runPressureHistoryTests() {
  pm::domain::PressureHistory history(3);
  require(history.empty(), "history must start empty");

  history.append(10, 1000.0f);
  history.append(20, 1001.0f);
  history.append(30, 1002.0f);
  history.append(40, 1003.0f);

  require(history.size() == 3, "ring buffer must keep fixed capacity");
  const auto oldest = history.oldest();
  const auto latest = history.latest();
  require(oldest.has_value(), "oldest should exist");
  require(latest.has_value(), "latest should exist");
  require(oldest->timestamp_s == 20, "oldest timestamp mismatch");
  require(std::fabs(oldest->pressure_hpa - 1001.0f) < 0.001f,
          "oldest pressure mismatch");
  require(latest->timestamp_s == 40, "latest timestamp mismatch");
  require(std::fabs(latest->pressure_hpa - 1003.0f) < 0.001f,
          "latest pressure mismatch");

  history.clear();
  require(history.empty(), "clear must reset state");
}
