#include "../../pocketmeteo/src/domain/SeaLevelReducer.h"

#include <cmath>
#include <stdexcept>

namespace {

void require(bool condition, const char *message) {
  if (!condition) {
    throw std::runtime_error(message);
  }
}

} // namespace

void runSeaLevelTests() {
  const auto reduced =
      pm::domain::SeaLevelReducer::toSeaLevelHpa(980.0f, 200.0f, 20.0f);
  require(reduced.has_value(), "sea-level reduction should succeed");
  require(*reduced > 1000.0f, "sea-level pressure should increase");
  require(*reduced < 1006.0f, "sea-level pressure expected range mismatch");

  const auto invalid =
      pm::domain::SeaLevelReducer::toSeaLevelHpa(-1.0f, 100.0f, 20.0f);
  require(!invalid.has_value(), "invalid pressure must be rejected");
}
