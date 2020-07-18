#pragma once

#include "include/types.h"

class ObjectBuilder {
public:
  ObjectBuilder(u16 model_index, const BehaviorScript* scripts);

  const u16 model_index;
  const BehaviorScript* behavior_scripts;
};
