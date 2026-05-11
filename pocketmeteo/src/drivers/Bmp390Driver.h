#pragma once

#include <Adafruit_BMP3XX.h>

#include <cstddef>
#include <cstdint>

#include "../hal/ISensor.h"

namespace pm::drivers {

class Bmp390Driver final : public hal::ISensor {
public:
  struct Config {
    std::uint8_t i2c_address{BMP3XX_DEFAULT_ADDRESS};
    std::size_t default_average_samples{8};
    std::uint8_t temperature_oversampling{BMP3_OVERSAMPLING_8X};
    std::uint8_t pressure_oversampling{BMP3_OVERSAMPLING_16X};
    std::uint8_t iir_filter_coeff{BMP3_IIR_FILTER_COEFF_3};
  };

  Bmp390Driver();
  explicit Bmp390Driver(const Config &config);

  bool begin() override;
  bool read(hal::SensorReading &out_reading) override;
  bool readAveraged(std::size_t samples, hal::SensorReading &out_reading) override;

private:
  bool configureSensor();

  Config config_{};
  Adafruit_BMP3XX bmp_{};
};

} // namespace pm::drivers

