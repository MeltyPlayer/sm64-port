#include "i_level_script_builder.hpp"

const LevelScript* ILevelScriptBuilder::build(
    int& out_count,
    LevelScript*
    outer_scripts) { return build_impl(out_count, outer_scripts); }
