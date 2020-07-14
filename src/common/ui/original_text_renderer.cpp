#include "original_text_renderer.hpp"

#include <cstdlib>


#include "game/game_init.h"
#include "game/ingame_menu.h"
#include "game/memory.h"
#include "game/segment2.h"
#include "levels/menu/header.h"

void render_string_impl(s8 hudLUT, s16 x, s16 y, const u8* str);

/**
 * Public
 */
void OriginalTextRenderer::set_lut(s8 lut) { this->lut = lut; }

void OriginalTextRenderer::render_string(s16 x, s16 y, const u8* str) {
  render_string_impl(lut, x, y, str);
}

/**
 * Implementation
 */
void render_char_tile(void** lut, s16 x, s16 y, u8 chr) {
  gDPSetTextureImage(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1,
                     lut[chr]);
  gSPDisplayList(gDisplayListHead++, dl_rgba16_load_tex_block);
  gSPTextureRectangle(gDisplayListHead++, x << 2, y << 2, (x + 16) << 2,
                      (y + 16) << 2, G_TX_RENDERTILE,
                      0, 0, 1 << 10, 1 << 10);
}

#ifdef VERSION_EU
void print_hud_char_umlaut(s16 x, s16 y, u8 chr) {
  void **fontLUT = segmented_to_virtual(main_hud_lut);

  gDPPipeSync(gDisplayListHead++);
  render_char_tile(fontLUT, x, y, chr);
  render_char_tile(fontLUT, x, y - 4, GLYPH_UMLAUT);
}
#endif

void render_string_impl(s8 hudLUT, s16 x, s16 y, const u8 *str) {
  s32 strPos = 0;
  void** hudLUT1 = (void**)segmented_to_virtual(menu_hud_lut);
  // Japanese Menu HUD Color font
  void** hudLUT2 = (void**)segmented_to_virtual(main_hud_lut);
  // 0-9 A-Z HUD Color Font
  u32 curX = x;
  u32 curY = y;

  u32 xStride; // X separation

  if (hudLUT == HUD_LUT_JPMENU) { xStride = 16; }
  else {
    // HUD_LUT_GLOBAL
#if defined(VERSION_JP)
    xStride = 14;
#else
    xStride = 12; //? Shindou uses this.
#endif
  }

  while (str[strPos] != GLOBAR_CHAR_TERMINATOR) {
#ifdef VERSION_EU
    switch (str[strPos]) {
      case GLOBAL_CHAR_SPACE:
        curX += xStride / 2;
        break;
      case HUD_CHAR_A_UMLAUT:
        print_hud_char_umlaut(curX, curY, ASCII_TO_DIALOG('A'));
        curX += xStride;
        break;
      case HUD_CHAR_O_UMLAUT:
        print_hud_char_umlaut(curX, curY, ASCII_TO_DIALOG('O'));
        curX += xStride;
        break;
      case HUD_CHAR_U_UMLAUT:
        print_hud_char_umlaut(curX, curY, ASCII_TO_DIALOG('U'));
        curX += xStride;
        break;
      default:
#endif
#if defined(VERSION_US) || defined(VERSION_SH)
        if (str[strPos] == GLOBAL_CHAR_SPACE) {
          curX += 8;
        } else {
#endif

    void** lut;
    switch (hudLUT) {
      case HUD_LUT_JPMENU:
        lut = hudLUT1;
        break;
      case HUD_LUT_GLOBAL:
      default:
        lut = hudLUT2;
    }
    const auto chr = str[strPos];

    gDPPipeSync(gDisplayListHead++);
    render_char_tile(lut, curX, curY, chr);

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
