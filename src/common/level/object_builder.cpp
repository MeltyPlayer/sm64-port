#include "object_builder.hpp"


ObjectBuilder::ObjectBuilder(u16 model_index,
                             const BehaviorScript* behavior_scripts)
    : model_index(model_index),
      behavior_scripts(behavior_scripts) {
}
