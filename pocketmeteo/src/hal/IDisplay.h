#pragma once

#include <cstdint>
#include <string_view>

namespace pm::hal {

enum class DisplayUpdateMode : std::uint8_t {
  Partial = 0,
  Full = 1,
};

struct TextStyle {
  std::int16_t x{0};
  std::int16_t y{0};
  std::uint16_t color{0};
  bool use_cyrillic{true};
};

class IDisplay {
public:
  virtual ~IDisplay() = default;

  virtual bool begin() = 0;
  virtual void clear(DisplayUpdateMode mode) = 0;
  virtual void drawText(std::string_view text, const TextStyle &style) = 0;
  virtual bool commit(DisplayUpdateMode mode) = 0;
};

} // namespace pm::hal
