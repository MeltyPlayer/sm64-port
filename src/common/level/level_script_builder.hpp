#pragma once

#include <vector>

#include "include/types.h"

class LevelScriptBuilder {
public:
  LevelScriptBuilder& add_level_script(LevelScript in_script);
  LevelScriptBuilder& add_level_scripts(const LevelScript* in_scripts,
                                        int script_count);

  const LevelScript* build();
  const LevelScript* build_with_jump_back_to_start();

private:
  std::vector<LevelScript> scripts;
};
