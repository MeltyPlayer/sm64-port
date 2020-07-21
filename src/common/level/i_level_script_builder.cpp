#include "i_level_script_builder.hpp"

const LevelScript* ILevelScriptBuilder::build_impl(int& out_count,
                                                   LevelScript* outer_scripts) {
  const auto script_count = size();

  const auto out_scripts = new LevelScript[script_count];
  build_into(unused_int,
                 outer_scripts != nullptr ? outer_scripts : out_scripts,
                 out_scripts);

  out_count = script_count;
  return out_scripts;
}

const LevelScript* ILevelScriptBuilder::build(
    int& out_count,
    LevelScript* outer_scripts) {
  return build_impl(out_count, outer_scripts);
}
