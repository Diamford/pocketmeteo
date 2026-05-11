#pragma once

#include <cstdint>

namespace pm::domain {

struct PressureSample {
  std::int64_t timestamp_s{0};
  float pressure_hpa{0.0f};
};

} // namespace pm::domain
