#pragma once

#include "../domain/DomainTypes.h"

#include <optional>
#include <vector>

namespace pm::hal {

class IStorage {
public:
  virtual ~IStorage() = default;

  virtual void appendPressureSample(const pm::domain::PressureSample &sample) = 0;
  virtual std::vector<pm::domain::PressureSample> pressureSamples() const = 0;

  virtual void setLastSeaLevelPressureHpa(float value) = 0;
  virtual std::optional<float> lastSeaLevelPressureHpa() const = 0;
};

} // namespace pm::hal
