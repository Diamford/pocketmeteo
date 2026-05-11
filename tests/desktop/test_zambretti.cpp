#include "../../pocketmeteo/src/domain/ZambrettiForecast.h"

#include <stdexcept>

namespace {

void require(bool condition, const char *message) {
  if (!condition) {
    throw std::runtime_error(message);
  }
}

} // namespace

void runZambrettiTests() {
  const auto rising = pm::domain::ZambrettiForecast::forecast(
      1031.0f, 2.0f, pm::domain::Season::Summer);
  require(rising.code == 'A', "high pressure rising code mismatch");

  const auto steady = pm::domain::ZambrettiForecast::forecast(
      1010.0f, 0.1f, pm::domain::Season::Winter);
  require(steady.code == 'G', "winter steady code mismatch");

  const auto falling = pm::domain::ZambrettiForecast::forecast(
      1001.0f, -2.0f, pm::domain::Season::Summer);
  require(falling.code == 'L', "falling pressure code mismatch");
}
