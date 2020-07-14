#pragma once

#include "game/ingame_menu.h"
#include "include/types.h"
#include "i_text_renderer.hpp"

class OriginalTextRenderer : public ITextRenderer {

public:
  /** Not sure what lut is, some kind of font table?? */
  void set_lut(s8 lut);

  void render_string(s16 x, s16 y, const u8* str);

private:
  s8 lut = HUD_LUT_GLOBAL;
};
