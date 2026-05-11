#include "Eink270Driver.h"

#include <Adafruit_GFX.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <GxEPD2_3C.h>

#include "../ui/fonts/CyrillicTranslit.h"

namespace pm::drivers {

namespace {
constexpr std::int16_t kDisplayWidth = 176;
constexpr std::int16_t kDisplayHeight = 264;
}

Eink270Driver::Eink270Driver(const Pins &pins)
    : pins_(pins),
      display_(GxEPD2_270(pins_.cs, pins_.dc, pins_.rst, pins_.busy)) {}

bool Eink270Driver::begin() {
  display_.init(115200, true, 2, false);
  display_.setRotation(1);
  display_.setTextColor(GxEPD_BLACK);
  display_.setFont(&FreeMonoBold9pt7b);
  clear(hal::DisplayUpdateMode::Full);
  return commit(hal::DisplayUpdateMode::Full);
}

void Eink270Driver::clear(hal::DisplayUpdateMode mode) {
  beginFrame(mode);
  display_.fillScreen(GxEPD_WHITE);
}

void Eink270Driver::drawText(std::string_view text, const hal::TextStyle &style) {
  beginFrame(partial_mode_ ? hal::DisplayUpdateMode::Partial
                           : hal::DisplayUpdateMode::Full);

  display_.setTextColor(style.color);
  display_.setCursor(style.x, style.y);

  char ascii_buffer[192];
  std::size_t ascii_len = 0;

  for (std::size_t idx = 0; idx < text.size();) {
    std::uint32_t codepoint = 0;
    if (!decodeUtf8Codepoint(text, idx, codepoint)) {
      ++idx;
      continue;
    }

    appendCodepointAsAscii(codepoint, style.use_cyrillic, ascii_buffer, ascii_len,
                           sizeof(ascii_buffer));
    if (ascii_len >= sizeof(ascii_buffer) - 2) {
      break;
    }
  }

  ascii_buffer[ascii_len] = '\0';
  display_.print(ascii_buffer);
}

bool Eink270Driver::commit(hal::DisplayUpdateMode mode) {
  if (!frame_open_) {
    return true;
  }
  frame_open_ = false;
  if (mode == hal::DisplayUpdateMode::Partial) {
    display_.displayWindow(0, 0, kDisplayWidth, kDisplayHeight);
  } else {
    display_.nextPage();
  }
  return true;
}

void Eink270Driver::beginFrame(hal::DisplayUpdateMode mode) {
  if (frame_open_ && partial_mode_ == (mode == hal::DisplayUpdateMode::Partial)) {
    return;
  }

  if (frame_open_) {
    commit(partial_mode_ ? hal::DisplayUpdateMode::Partial
                         : hal::DisplayUpdateMode::Full);
  }

  partial_mode_ = (mode == hal::DisplayUpdateMode::Partial);
  frame_open_ = true;

  if (partial_mode_) {
    display_.setPartialWindow(0, 0, kDisplayWidth, kDisplayHeight);
    display_.firstPage();
  } else {
    display_.setFullWindow();
    display_.firstPage();
  }
}

bool Eink270Driver::decodeUtf8Codepoint(std::string_view input, std::size_t &idx,
                                        std::uint32_t &out_codepoint) {
  if (idx >= input.size()) {
    return false;
  }

  const std::uint8_t c0 = static_cast<std::uint8_t>(input[idx]);
  if ((c0 & 0x80) == 0) {
    out_codepoint = c0;
    ++idx;
    return true;
  }

  if ((c0 & 0xE0) == 0xC0 && idx + 1 < input.size()) {
    const std::uint8_t c1 = static_cast<std::uint8_t>(input[idx + 1]);
    out_codepoint = ((c0 & 0x1F) << 6) | (c1 & 0x3F);
    idx += 2;
    return true;
  }

  if ((c0 & 0xF0) == 0xE0 && idx + 2 < input.size()) {
    const std::uint8_t c1 = static_cast<std::uint8_t>(input[idx + 1]);
    const std::uint8_t c2 = static_cast<std::uint8_t>(input[idx + 2]);
    out_codepoint = ((c0 & 0x0F) << 12) | ((c1 & 0x3F) << 6) | (c2 & 0x3F);
    idx += 3;
    return true;
  }

  return false;
}

void Eink270Driver::appendCodepointAsAscii(std::uint32_t codepoint,
                                           bool cyrillic_mode, char *buffer,
                                           std::size_t &len,
                                           std::size_t capacity) {
  if (len + 1 >= capacity) {
    return;
  }
  if (codepoint < 0x80) {
    buffer[len++] = static_cast<char>(codepoint);
    return;
  }
  buffer[len++] = cyrillic_mode ? ui::fonts::translitCodepoint(codepoint) : '?';
}

} // namespace pm::drivers
