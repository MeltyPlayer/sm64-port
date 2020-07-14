#pragma once

#include "types.h"

class ITextRenderer {

public:
  /** Not sure what lut is, some kind of font table?? */
  virtual void set_lut(s8 lut) = 0;

  virtual void render_string(s16 x, s16 y, const u8* str) = 0;
};
