#pragma once
#include "file_select.hpp"

#include <PR/ultratypes.h>
#include "object_fields.h"

class MenuButton {
public:
  MenuButton(struct Object* impl)
    : impl(impl) {}

  void set_timer(s32 timer) { impl->oMenuButtonTimer = timer; }
  s32& get_timer() const { return impl->oMenuButtonTimer; }

  void grow_from_main_menu() {
    if (change_size_in_main_menu(true)) {
      impl->oMenuButtonState = MENU_BUTTON_STATE_FULLSCREEN;
    }
  }

  void shrink_to_main_menu() {
    if (change_size_in_main_menu(false)) {
      impl->oMenuButtonState = MENU_BUTTON_STATE_DEFAULT;
    }
  }

private:
  /** Returns true once complete. */
  bool change_size_in_main_menu(bool grow) {
    const auto timer = get_timer();
    const auto growthSign = grow ? 1 : -1;

    if (timer < 16) { impl->oFaceAngleYaw += growthSign * 0x800; }
    if (timer < 8) { impl->oFaceAnglePitch += growthSign * 0x800; }
    if (timer >= 8 && timer < 16) {
      impl->oFaceAnglePitch -= growthSign * 0x800;
    }

    impl->oParentRelativePosX -= growthSign * impl->oMenuButtonOrigPosX / 16.0;
    impl->oParentRelativePosY -= growthSign * impl->oMenuButtonOrigPosY / 16.0;

    if ((grow && impl->oPosZ < impl->oMenuButtonOrigPosZ + 17800)
        || (!grow && impl->oPosZ > impl->oMenuButtonOrigPosZ)) {
      impl->oParentRelativePosZ += growthSign * 1112.5;
    }

    set_timer(timer + 1);
    if (get_timer() == 16) {
      impl->oParentRelativePosX = grow ? 0 : impl->oMenuButtonOrigPosX;
      impl->oParentRelativePosY = grow ? 0 : impl->oMenuButtonOrigPosY;

      set_timer(0);

      return true;
    }

    return false;
  }

  struct Object* impl;
};
