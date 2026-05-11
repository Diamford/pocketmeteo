#include "PressureHistory.h"

#include <algorithm>
#include <cmath>

namespace pm::domain {

PressureHistory::PressureHistory(std::size_t capacity)
    : buffer_(std::max<std::size_t>(capacity, 1U)),
      capacity_(std::max<std::size_t>(capacity, 1U)) {}

void PressureHistory::append(std::int64_t timestamp_s, float pressure_hpa) {
  if (!std::isfinite(pressure_hpa)) {
    return;
  }

  buffer_[head_] = PressureSample{timestamp_s, pressure_hpa};
  head_ = (head_ + 1) % capacity_;
  if (count_ < capacity_) {
    ++count_;
  }
}

void PressureHistory::clear() {
  count_ = 0;
  head_ = 0;
}

bool PressureHistory::empty() const { return count_ == 0; }

std::size_t PressureHistory::size() const { return count_; }

std::size_t PressureHistory::capacity() const { return capacity_; }

std::optional<PressureSample> PressureHistory::oldest() const {
  if (empty()) {
    return std::nullopt;
  }
  const std::size_t oldest_idx = (head_ + capacity_ - count_) % capacity_;
  return buffer_[oldest_idx];
}

std::optional<PressureSample> PressureHistory::latest() const {
  if (empty()) {
    return std::nullopt;
  }
  const std::size_t latest_idx = (head_ + capacity_ - 1) % capacity_;
  return buffer_[latest_idx];
}

std::vector<PressureSample> PressureHistory::samples() const {
  std::vector<PressureSample> result;
  result.reserve(count_);

  const std::size_t start = (head_ + capacity_ - count_) % capacity_;
  for (std::size_t i = 0; i < count_; ++i) {
    result.push_back(buffer_[(start + i) % capacity_]);
  }
  return result;
}

} // namespace pm::domain
