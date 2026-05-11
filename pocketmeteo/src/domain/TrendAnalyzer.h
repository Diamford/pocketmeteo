#pragma once

#include "PressureHistory.h"

#include <optional>

namespace pm::domain {

class TrendAnalyzer {
public:
  static std::optional<float> delta3hHpa(const PressureHistory &history);
};

} // namespace pm::domain
