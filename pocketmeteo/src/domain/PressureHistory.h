#pragma once

#include "DomainTypes.h"

#include <cstddef>
#include <optional>
#include <vector>

namespace pm::domain {

class PressureHistory {
public:
  // 19 points at 10-minute intervals cover a full 3-hour delta.
  explicit PressureHistory(std::size_t capacity = 19);

  void append(std::int64_t timestamp_s, float pressure_hpa);
  void clear();

  [[nodiscard]] bool empty() const;
  [[nodiscard]] std::size_t size() const;
  [[nodiscard]] std::size_t capacity() const;

  [[nodiscard]] std::optional<PressureSample> oldest() const;
  [[nodiscard]] std::optional<PressureSample> latest() const;
  [[nodiscard]] std::vector<PressureSample> samples() const;

private:
  std::vector<PressureSample> buffer_;
  std::size_t capacity_{0};
  std::size_t count_{0};
  std::size_t head_{0};
};

} // namespace pm::domain
