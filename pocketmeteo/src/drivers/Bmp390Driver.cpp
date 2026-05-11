#include "Bmp390Driver.h"

#include <Arduino.h>
#include <Wire.h>

#include <cmath>

namespace pm::drivers {

Bmp390Driver::Bmp390Driver(Config config) : config_(config) {}

bool Bmp390Driver::begin() {
  Wire.begin();

  if (!bmp_.begin_I2C(config_.i2c_address, &Wire)) {
    return false;
  }

  return configureSensor();
}

bool Bmp390Driver::read(hal::SensorReading &out_reading) {
  if (!bmp_.performReading()) {
    return false;
  }

  out_reading.temperature_c = bmp_.temperature;
  out_reading.pressure_pa = bmp_.pressure;
  return true;
}

bool Bmp390Driver::readAveraged(std::size_t samples,
                                hal::SensorReading &out_reading) {
  const std::size_t sample_count =
      samples > 0 ? samples : config_.default_average_samples;
  if (sample_count == 0) {
    return false;
  }

  float temperature_sum = 0.0f;
  float pressure_sum = 0.0f;

  hal::SensorReading single{};
  for (std::size_t idx = 0; idx < sample_count; ++idx) {
    if (!read(single)) {
      return false;
    }
    temperature_sum += single.temperature_c;
    pressure_sum += single.pressure_pa;
    delay(5);
  }

  const float inv_count = 1.0f / static_cast<float>(sample_count);
  out_reading.temperature_c = temperature_sum * inv_count;
  out_reading.pressure_pa = pressure_sum * inv_count;
  return true;
}

bool Bmp390Driver::configureSensor() {
  bmp_.setTemperatureOversampling(config_.temperature_oversampling);
  bmp_.setPressureOversampling(config_.pressure_oversampling);
  bmp_.setIIRFilterCoeff(config_.iir_filter_coeff);
  bmp_.setOutputDataRate(BMP3_ODR_25_HZ);
  return true;
}

} // namespace pm::drivers

