#pragma once

#include <memory>
#include <vector>

#include "i_text_renderer.hpp"
#include "include/types.h"
#include "text_to_render.hpp"

class OriginalTextRenderer : public ITextRenderer {

public:
  void process_rendered_text();

  void render_text(const TextToRender &text_to_render);
  void schedule_render_text(const TextToRender &text_to_render);

private:
  std::vector<std::unique_ptr<TextToRender>> all_text_to_render;
};
