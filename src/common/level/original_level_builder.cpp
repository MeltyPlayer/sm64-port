#include "original_level_builder.h"

OriginalLevelBuilder& OriginalLevelBuilder::add_level_scripts(std::vector<LevelScript> new_scripts) {
  std::copy(&new_scripts.front(), &new_scripts.back(), std::back_inserter(scripts));
  return *this;
}

const LevelScript* OriginalLevelBuilder::build() {
  return &scripts[0];
}
