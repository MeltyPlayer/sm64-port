#include "original_text_renderer.hpp"

#include "config.h"
#include "game/game_init.h"
#include "game/memory.h"
#include "game/print.h"
#include "game/segment2.h"
#include "levels/menu/header.h"

void process_rendered_strings_impl(
  std::vector<std::unique_ptr<TextToRender>>& all_text_to_render);

void render_string_impl(const u8* str, s16 x, s16 y, s16 x_scale, s16 y_scale,
                        s16 dsdx, s16 dsdy,
                        LutSource lut_source);

/**
 * Public
 */
void OriginalTextRenderer::process_rendered_text() {
  process_rendered_strings_impl(all_text_to_render);
}

void OriginalTextRenderer::render_text(const TextToRender& text_to_render) {
  render_string_impl(text_to_render.str, text_to_render.x, text_to_render.y,
                     16, // text_to_render.x_scale,
                     16, //text_to_render.y_scale,
                     1, //text_to_render.dsdx,
                     1, //text_to_render.dsdy,
                     text_to_render.lut_source);
}

void OriginalTextRenderer::schedule_render_text(
  const TextToRender& text_to_render_impl) {
  auto text_to_render = new TextToRender(text_to_render_impl);
  all_text_to_render.push_back(std::unique_ptr<TextToRender>(text_to_render));
}

/**
 * Implementation
 */
/**
 * Converts a char into the proper colorful glyph for the char.
 */
s8 char_to_glyph_index_(char c) {
  if (c >= 'A' && c <= 'Z') { return c - 55; }

  if (c >= 'a' && c <= 'z') { return c - 87; }

  if (c >= '0' && c <= '9') { return c - 48; }

  if (c == ' ') { return GLYPH_SPACE; }

  if (c == '!') {
    return GLYPH_EXCLAMATION_PNT; // !, JP only
  }

  if (c == '#') {
    return GLYPH_TWO_EXCLAMATION; // !!, JP only
  }

  if (c == '?') {
    return GLYPH_QUESTION_MARK; // ?, JP only
  }

  if (c == '&') {
    return GLYPH_AMPERSAND; // &, JP only
  }

  if (c == '%') {
    return GLYPH_PERCENT; // %, JP only
  }

  if (c == '*') {
    return GLYPH_MULTIPLY; // x
  }

  if (c == '+') {
    return GLYPH_COIN; // coin
  }

  if (c == ',') {
    return GLYPH_MARIO_HEAD; // Imagine I drew Mario's head
  }

  if (c == '-') {
    return GLYPH_STAR; // star
  }

  if (c == '.') {
    return GLYPH_PERIOD; // large shaded dot, JP only
  }

  if (c == '/') {
    return GLYPH_BETA_KEY; // beta key, JP only. Reused for Ü in EU.
  }

  return GLYPH_SPACE;
}

void add_glyph_texture(const u8* const * glyphs, s8 glyphIndex) {
  gDPPipeSync(gDisplayListHead++);
  gDPSetTextureImage(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1,
                     glyphs[glyphIndex]);
  gSPDisplayList(gDisplayListHead++, dl_hud_img_load_tex_block);
}

void apply_jitter(s16& x, s16& y) {
  auto randXF = rand() * 1. / RAND_MAX;
  auto randYF = rand() * 1. / RAND_MAX;

  x += 4 * (randXF - .5);
  y += 4 * (randYF - .5);
}

#ifndef WIDESCREEN
/**
 * Clips textrect into the boundaries defined.
 */
void clip_to_bounds(s16& x, s16& y) {
  if (x < TEXRECT_MIN_X) { x = TEXRECT_MIN_X; }

  if (x > TEXRECT_MAX_X) { x = TEXRECT_MAX_X; }

  if (y < TEXRECT_MIN_Y) { y = TEXRECT_MIN_Y; }

  if (y > TEXRECT_MAX_Y) { y = TEXRECT_MAX_Y; }
}
#endif

void render_textrect(s16 x, s16 y, s16 x_scale, s16 y_scale, s16 dsdx,
                     s16 dsdy) {
  apply_jitter(x, y);

#ifndef WIDESCREEN
  // For widescreen we must allow drawing outside the usual area
  clip_to_bounds(x, y);
#endif
  gSPTextureRectangle(gDisplayListHead++, x << 2, y << 2, (x + x_scale-1) << 2,
                      (y + y_scale - 1) << 2, G_TX_RENDERTILE, 0, 0, dsdx << 10,
                      dsdy << 10);
}

void process_rendered_strings_impl(
  std::vector<std::unique_ptr<TextToRender>>& all_text_to_render) {

  auto text_to_render_count = all_text_to_render.size();
  if (text_to_render_count == 0) { return; }

  Mtx* mtx;

  mtx = (Mtx*)alloc_display_list(sizeof(*mtx));
  if (mtx == NULL) {
    all_text_to_render.clear();
    return;
  }

  guOrtho(mtx, 0.0f, SCREEN_WIDTH, 0.0f, SCREEN_HEIGHT, -10.0f, 10.0f, 1.0f);
  gSPPerspNormalize((Gfx *) (gDisplayListHead++), 0xFFFF);
  gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(mtx),
            G_MTX_PROJECTION | G_MTX_LOAD | G_MTX_NOPUSH);
  gSPDisplayList(gDisplayListHead++, dl_hud_img_begin);

  for (auto i = 0; i < text_to_render_count; ++i) {
    auto& text_to_render = *all_text_to_render[i];

    auto str = text_to_render.str;
    auto length = text_to_render.length;

    auto cur_x = text_to_render.x;
    auto cur_y = text_to_render.y;

    auto x_scale = 16; //text_to_render.x_scale;
    auto y_scale = 16;

    auto dsdx = 4;
    auto dsdy = 1;

    auto lut_source = text_to_render.lut_source;

    const u8* const * lut;
    switch (lut_source) {
      case LutSource::JAPANESE:
        lut = menu_hud_lut;
        break;
      case LutSource::DEFAULT:
      default:
        lut = main_hud_lut;
        break;
    }

    u32 x_stride;
    if (lut_source == LutSource::JAPANESE) { x_stride = x_scale; }
    else {
      // HUD_LUT_GLOBAL
#if defined(VERSION_JP)
      x_stride = (u32)(x_scale / 16. * 14);
#else
      x_stride = (u32)(x_scale / 16. * 12); //? Shindou uses this.
#endif
    }

    const u8* const * glyphs = (const u8*const *)segmented_to_virtual(lut);

    for (auto j = 0; j < length; j++) {
      auto chr = str[j];
      // auto glyphIndex = char_to_glyph_index_(chr);
      // Assumes it's already formatted in glyphs.
      auto glyphIndex = chr;

      // if (glyphIndex != GLYPH_SPACE) {
#ifdef VERSION_EU
      // Beta Key was removed by EU, so glyph slot reused.
      // This produces a colorful Ü.
      if (glyphIndex == GLYPH_BETA_KEY) {
        add_glyph_texture(GLYPH_U);
        render_textrect(text_to_render);

        add_glyph_texture(GLYPH_UMLAUT);
        render_textrect(sTextLabels[i]->x, sTextLabels[i]->y + 3, j);
      } else {
        add_glyph_texture(glyphIndex);
        render_textrect(text_to_render);
      }
#else
#if defined(VERSION_US) || defined(VERSION_SH)
      if (glyphIndex == GLOBAL_CHAR_SPACE) {
        cur_x += x_scale / 2;
      } else {
#endif

      add_glyph_texture(glyphs, glyphIndex);
      render_textrect(cur_x, cur_y, x_scale, y_scale,
                      dsdx,
                      dsdy);
#endif

      cur_x += x_stride;
#if defined(VERSION_US) || defined(VERSION_SH)
    }
#endif
      //}
    }
  }

  gSPDisplayList(gDisplayListHead++, dl_hud_img_end);

  all_text_to_render.clear();
}

void render_char_tile(u8 chr, s16 x, s16 y, s16 x_scale, s16 y_scale, s16 dsdx,
                      s16 dsdy, void** lut) {
  apply_jitter(x, y);

  auto glyph_texture = lut[chr];
  gDPSetTextureImage(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1,
                     glyph_texture);
  gSPDisplayList(gDisplayListHead++, dl_rgba16_load_tex_block);
  gSPTextureRectangle(gDisplayListHead++, x << 2, y << 2, (x + x_scale) << 2,
                      (y + y_scale) << 2,
                      G_TX_RENDERTILE, 0, 0, dsdx << 10, dsdy << 10);
}

#ifdef VERSION_EU
void print_hud_char_umlaut(u8 chr, s16 x, s16 y, s16 x_scale, s16 y_scale, s16 dsdx, s16 dsdy) {
  void **fontLUT = segmented_to_virtual(main_hud_lut);

  gDPPipeSync(gDisplayListHead++);
  render_char_tile(chr, x, y, x_scale, y_scale, dsdx, dsdy, fontLUT);
  render_char_tile(chr, x, y - y_scale / 16. * 4, x_scale, y_scale, dsdx, dsdy, fontLUT);
}
#endif

void render_string_impl(const u8* str, s16 x, s16 y, s16 x_scale, s16 y_scale,
                        s16 dsdx, s16 dsdy,
                        LutSource lut_source = LutSource::DEFAULT) {
  s32 strPos = 0;
  // Japanese Menu HUD Color font
  void** hudLUT1 = (void**)segmented_to_virtual(menu_hud_lut);
  // 0-9 A-Z HUD Color Font
  void** hudLUT2 = (void**)segmented_to_virtual(main_hud_lut);
  u32 curX = x;
  u32 curY = y;

  u32 xStride; // X separation

  if (lut_source == LutSource::JAPANESE) { xStride = x_scale; }
  else {
    // HUD_LUT_GLOBAL
#if defined(VERSION_JP)
    xStride = (u32)(x_scale / 16. * 14);
#else
    xStride = (u32)(x_scale / 16. * 12); //? Shindou uses this.
#endif
  }

  while (str[strPos] != GLOBAR_CHAR_TERMINATOR) {
#ifdef VERSION_EU
    switch (str[strPos]) {
      case GLOBAL_CHAR_SPACE:
        curX += xStride / 2;
        break;
      case HUD_CHAR_A_UMLAUT:
        print_hud_char_umlaut(ASCII_TO_DIALOG('A'), curX, curY, x_scale, y_scale, dsdx, dsdy);
        curX += xStride;
        break;
      case HUD_CHAR_O_UMLAUT:
        print_hud_char_umlaut(ASCII_TO_DIALOG('O'), curX, curY, x_scale, y_scale, dsdx, dsdy);
        curX += xStride;
        break;
      case HUD_CHAR_U_UMLAUT:
        print_hud_char_umlaut(ASCII_TO_DIALOG('U'), curX, curY, x_scale, y_scale, dsdx, dsdy);
        curX += xStride;
        break;
      default:
#endif
#if defined(VERSION_US) || defined(VERSION_SH)
        if (str[strPos] == GLOBAL_CHAR_SPACE) {
          curX += x_scale / 2;
        } else {
#endif

    void** lut;
    switch (lut_source) {
      case LutSource::JAPANESE:
        lut = hudLUT1;
        break;
      case LutSource::DEFAULT:
      default:
        lut = hudLUT2;
    }
    const auto chr = str[strPos];

    gDPPipeSync(gDisplayListHead++);
    render_char_tile(chr, curX, curY, x_scale, y_scale, dsdx, dsdy, lut);

    curX += xStride;
#ifdef VERSION_EU
          break;
        }
#endif
#if defined(VERSION_US) || defined(VERSION_SH)
    }
#endif
    strPos++;
  }
}
