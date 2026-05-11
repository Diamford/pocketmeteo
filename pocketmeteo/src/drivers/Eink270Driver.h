#pragma once

#include <GxEPD2_BW.h>

#include "../hal/IDisplay.h"

namespace pm::drivers {

class Eink270Driver final : public hal::IDisplay {
public:
  struct Pins {
    std::int8_t cs{10};
    std::int8_t dc{4};
    std::int8_t rst{3};
    std::int8_t busy{2};
  };

  Eink270Driver();
  explicit Eink270Driver(const Pins &pins);

  bool begin() override;
  void clear(hal::DisplayUpdateMode mode) override;
  void drawText(std::string_view text, const hal::TextStyle &style) override;
  bool commit(hal::DisplayUpdateMode mode) override;

private:
  static bool decodeUtf8Codepoint(std::string_view input, std::size_t &idx,
                                  std::uint32_t &out_codepoint);
  static void appendCodepointAsAscii(std::uint32_t codepoint, bool cyrillic_mode,
                                     char *buffer, std::size_t &len,
                                     std::size_t capacity);

  void beginFrame(hal::DisplayUpdateMode mode);

  Pins pins_{};
  bool frame_open_{false};
  bool partial_mode_{false};
  GxEPD2_BW<GxEPD2_270, GxEPD2_270::HEIGHT> display_;
};

} // namespace pm::drivers
