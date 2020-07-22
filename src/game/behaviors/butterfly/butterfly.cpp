#include "butterfly.hpp"

#include "engine/behavior_script.h"
#include "engine/math_util.h"
#include "game/obj_behaviors.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "include/object_fields.h"

#define o gCurrentObject

const s32 kStartFollowingMarioDistance = 1000;
const s32 kStartReturningHomeDistance = 1200;

const s32 kFollowMarioSpeed = 7;
const s32 kReturnHomeSpeed = 7;

// TODO: Move this to a common file.
s32 is_point_within_radius_of_mario_(f32 x, f32 y, f32 z, s32 dist) {
  f32 mGfxX = gMarioObject->header.gfx.pos[0];
  f32 mGfxY = gMarioObject->header.gfx.pos[1];
  f32 mGfxZ = gMarioObject->header.gfx.pos[2];

  if ((x - mGfxX) * (x - mGfxX) + (y - mGfxY) * (y - mGfxY) +
      (z - mGfxZ) * (z - mGfxZ) <
      (f32)(dist * dist)) { return TRUE; }

  return FALSE;
}

// TODO: Move this to a common file.
void set_object_visibility_(struct Object* obj, s32 dist) {
  f32 objX = obj->oPosX;
  f32 objY = obj->oPosY;
  f32 objZ = obj->oPosZ;

  if (is_point_within_radius_of_mario_(objX, objY, objZ, dist) == TRUE) {
    obj->header.gfx.node.flags &= ~GRAPH_RENDER_INVISIBLE;
  } else { obj->header.gfx.node.flags |= GRAPH_RENDER_INVISIBLE; }
}

void Butterfly::init() {
  cur_obj_init_animation(1);

  o->oButterflyYPhase = random_float() * 100.0f;
  o->header.gfx.unk38.animFrame = random_float() * 7.0f;
  o->oHomeX = o->oPosX;
  o->oHomeY = o->oPosY;
  o->oHomeZ = o->oPosZ;
}

void Butterfly::tick() {
  switch (state_) {
    case ButterflyState::RESTING:
      tick_rest();
      break;

    case ButterflyState::FOLLOW_MARIO:
      tick_follow_mario();
      break;

    case ButterflyState::RETURN_HOME:
      tick_return_home();
      break;
  }

  set_object_visibility_(o, 3000);
}

// sp28 = speed

void Butterfly::step(s32 speed) {
  struct FloorGeometry* sp24;
  s16 yaw = o->oMoveAngleYaw;
  s16 pitch = o->oMoveAnglePitch;
  s16 yPhase = o->oButterflyYPhase;
  f32 floorY;

  o->oVelX = sins(yaw) * (f32)speed;
  o->oVelY = sins(pitch) * (f32)speed;
  o->oVelZ = coss(yaw) * (f32)speed;

  o->oPosX += o->oVelX;
  o->oPosZ += o->oVelZ;

  if (state_ == ButterflyState::FOLLOW_MARIO) {
    o->oPosY -= o->oVelY + coss((s32)(yPhase * 655.36)) * 20.0f / 4;
  } else { o->oPosY -= o->oVelY; }

  floorY = find_floor_height_and_data(o->oPosX, o->oPosY, o->oPosZ, &sp24);

  if (o->oPosY < floorY + 2.0f)
    o->oPosY = floorY + 2.0f;

  o->oButterflyYPhase++;
  if (o->oButterflyYPhase >= 101) {
    o->oButterflyYPhase = 0;
  }
}

void butterfly_calculate_angle_(void) {
  gMarioObject->oPosX += 5 * o->oButterflyYPhase / 4;
  gMarioObject->oPosZ += 5 * o->oButterflyYPhase / 4;
  obj_turn_toward_object(o, gMarioObject, 16, 0x300);
  gMarioObject->oPosX -= 5 * o->oButterflyYPhase / 4;
  gMarioObject->oPosZ -= 5 * o->oButterflyYPhase / 4;

  gMarioObject->oPosY += (5 * o->oButterflyYPhase + 0x100) / 4;
  obj_turn_toward_object(o, gMarioObject, 15, 0x500);
  gMarioObject->oPosY -= (5 * o->oButterflyYPhase + 0x100) / 4;
}

void Butterfly::tick_rest() {
  if (is_point_within_radius_of_mario_(
      o->oPosX,
      o->oPosY,
      o->oPosZ,
      kStartFollowingMarioDistance)) {
    cur_obj_init_animation(0);

    state_ = ButterflyState::FOLLOW_MARIO;
    o->oMoveAngleYaw = gMarioObject->header.gfx.angle[1];
  }
}

void Butterfly::tick_follow_mario() {
  butterfly_calculate_angle_();

  butterfly_step(kFollowMarioSpeed);

  if (!is_point_within_radius_of_mario_(
      o->oHomeX,
      o->oHomeY,
      o->oHomeZ,
      kStartReturningHomeDistance)) { state_ = ButterflyState::RETURN_HOME; }
}

void Butterfly::tick_return_home() {
  f32 homeDistX = o->oHomeX - o->oPosX;
  f32 homeDistY = o->oHomeY - o->oPosY;
  f32 homeDistZ = o->oHomeZ - o->oPosZ;
  s16 hAngleToHome = atan2s(homeDistZ, homeDistX);
  s16 vAngleToHome =
      atan2s(sqrtf(homeDistX * homeDistX + homeDistZ * homeDistZ), -homeDistY);

  o->oMoveAngleYaw =
      approach_s16_symmetric(o->oMoveAngleYaw, hAngleToHome, 0x800);
  o->oMoveAnglePitch =
      approach_s16_symmetric(o->oMoveAnglePitch, vAngleToHome, 0x50);

  butterfly_step(kReturnHomeSpeed);

  if (homeDistX * homeDistX + homeDistY * homeDistY + homeDistZ * homeDistZ <
      144.0f) {
    cur_obj_init_animation(1);

    state_ = ButterflyState::RESTING;
    o->oPosX = o->oHomeX;
    o->oPosY = o->oHomeY;
    o->oPosZ = o->oHomeZ;
  }
}
