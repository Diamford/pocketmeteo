#pragma once

#include <string>

namespace pm::domain {

enum class Season { Winter, Summer };

struct ZambrettiResult {
  char code{'Z'};
  std::string text_ru;
};

class ZambrettiForecast {
public:
  static ZambrettiResult forecast(float sea_level_pressure_hpa,
                                  float delta3h_hpa,
                                  Season season);
};

} // namespace pm::domain
