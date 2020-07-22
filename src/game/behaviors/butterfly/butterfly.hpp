#pragma once
#include "common/object/i_object_wrapper.hpp"
#include "include/types.h"

enum class ButterflyState {
  RESTING,
  FOLLOW_MARIO,
  RETURN_HOME
};


class Butterfly : public IObjectWrapper {
public:
  Butterfly(struct Object* wrapped_object) : IObjectWrapper(wrapped_object) {}
  Butterfly(const Butterfly& other) = delete;

  void init() override;
  void tick() override;

private:
  void step(s32 speed);

  void tick_rest();
  void tick_follow_mario();
  void tick_return_home();

  ButterflyState state_ = ButterflyState::RESTING;
};
