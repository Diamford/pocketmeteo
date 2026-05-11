# Fontconvert notes for pocketmeteo

This folder documents how to generate a production Cyrillic `GFXfont` for
`Adafruit_GFX` and `GxEPD2`.

## Why this exists

`Adafruit_GFX` demo fonts do not include full Cyrillic. For release builds,
generate and commit a dedicated Russian font with the glyph range
`U+0400..U+045F`.

## Recommended flow

1. Build `fontconvert` from Adafruit_GFX:
   - source: `Adafruit_GFX/fontconvert/`
2. Pick a TTF with a permissive license and Cyrillic support
   (for example, Noto Sans).
3. Generate the header:

```bash
./fontconvert NotoSans-Regular.ttf 16 0x20 0x7E 0x400 0x45F > NotoSansRu16pt7b.h
```

4. Copy the generated file to:
   - `pocketmeteo/src/ui/fonts/NotoSansRu16pt7b.h`
5. Switch `Eink270Driver` to use that font via `display_.setFont(...)`.

## Current repository state

P3 currently uses a safe transliteration fallback (`CyrillicTranslit.h`) so
Russian text is readable during hardware bring-up without square glyphs.
