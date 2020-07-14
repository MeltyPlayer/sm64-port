#pragma once

#include "text_to_render.hpp"
#include "types.h"

class ITextRenderer {

public:
  virtual void process_rendered_text() = 0;

  /** Renders text immediately. */
  virtual void render_text(const TextToRender &text_to_render) = 0;
  virtual void render_text(const u8 *str, s16 x, s16 y,
                           LutSource lut_source = LutSource::DEFAULT) {
    render_text(TextToRender(str, x, y).set_lut_source(lut_source));
  }
  virtual void render_text(u8 chr, s16 x, s16 y, LutSource lut_source = LutSource::DEFAULT) {
    render_text(TextToRender(chr, x, y).set_lut_source(lut_source));
  }

  /** Schedules text to be rendered on top at the end of the frame. */
  virtual void schedule_render_text(const TextToRender &text_to_render) = 0;
  virtual void schedule_render_text(const u8* str, s16 x, s16 y,
                                    LutSource lut_source = LutSource::DEFAULT) {
    schedule_render_text(TextToRender(str, x, y).set_lut_source(lut_source));
  }
  virtual void schedule_render_text(u8 chr, s16 x, s16 y, LutSource lut_source = LutSource::DEFAULT) {
    schedule_render_text(TextToRender(chr, x, y).set_lut_source(lut_source));
  }
};
