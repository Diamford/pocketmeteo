#pragma once

#include <cstdint>
#include <string_view>

namespace pm::hal {

enum class DisplayUpdateMode : std::uint8_t {
  Partial = 0,
  Full = 1,
};

/// Sub-rectangle for partial refreshes (device coordinates after rotation).
struct PartialRect {
  std::int16_t x{0};
  std::int16_t y{0};
  std::uint16_t width{0};
  std::uint16_t height{0};
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
  /// When `mode == Partial` and `area == nullptr`, the full panel is used.
  virtual void clear(DisplayUpdateMode mode, const PartialRect *area = nullptr) = 0;
  virtual void drawText(std::string_view text, const TextStyle &style) = 0;
  virtual bool commit(DisplayUpdateMode mode) = 0;
};

} // namespace pm::hal
