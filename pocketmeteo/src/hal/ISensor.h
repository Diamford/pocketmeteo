#pragma once

#include <cstddef>

namespace pm::hal {

struct SensorReading {
  float temperature_c{0.0f};
  float pressure_pa{0.0f};
};

class ISensor {
public:
  virtual ~ISensor() = default;

  virtual bool begin() = 0;
  virtual bool read(SensorReading &out_reading) = 0;
  virtual bool readAveraged(std::size_t samples, SensorReading &out_reading) = 0;
};

} // namespace pm::hal

