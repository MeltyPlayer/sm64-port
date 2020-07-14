#pragma once

#include "include/types.h"

#include "game/ingame_menu.h"
#include "i_text_renderer.hpp"

/**
 * This changes the font used when text rendering, since each one has different
 * glyphs.
 */
enum class LutSource {
  UNDEFINED,
  DEFAULT,
  JAPANESE,
  DIFF,
};

class TextToRender {

public:
  TextToRender(const u8 *str, s16 x, s16 y) {
    u8 c;
    s32 strPos = 0;
    do {
      c = str[strPos];
      this->str[strPos] = c;

      strPos++;
    } while (c != GLOBAR_CHAR_TERMINATOR);
    length = strPos - 1;

    this->x = x;
    this->y = y;
  }

  TextToRender(u8 chr, s16 x, s16 y) {
    str[0] = chr;
    str[1] = GLOBAR_CHAR_TERMINATOR;
    length = 1;

    this->x = x;
    this->y = y;
  }

  TextToRender(const TextToRender& other) {
    u8 c;
    s32 strPos = 0;
    do {
      c = other.str[strPos];
      this->str[strPos] = c;

      strPos++;
    } while (c != GLOBAR_CHAR_TERMINATOR);
    length = strPos - 1;

    this->x = other.x;
    this->y = other.y;
  }

  TextToRender &set_lut_source(LutSource lut_source) {
    this->lut_source = lut_source;
    return *this;
  }

  u8 str[50];
  s16 length;

  s16 x;
  s16 y;

  //s16 x_scale = 16;
  // s16 y_scale = 16;

  // s16 dsdx = 4;
  // s16 dsdy = 1;

  LutSource lut_source;
};
