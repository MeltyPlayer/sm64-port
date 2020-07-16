#pragma once

#include <vector>

#include "include/types.h"

class OriginalLevelBuilder {
public:
  OriginalLevelBuilder &add_level_scripts(std::vector<LevelScript> scripts);

  const LevelScript *build();

private:
  std::vector<LevelScript> scripts;
};
