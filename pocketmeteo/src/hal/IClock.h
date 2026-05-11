#pragma once

#include <cstdint>

namespace pm::hal {

class IClock {
public:
  virtual ~IClock() = default;
  virtual std::int64_t unixNowSeconds() const = 0;
};

} // namespace pm::hal
