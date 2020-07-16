#include "level_script_builder.hpp"

#include "level_commands.h"

LevelScriptBuilder &LevelScriptBuilder::add_level_script(LevelScript in_script) {
  scripts.push_back(in_script);
  return *this;
}

LevelScriptBuilder &LevelScriptBuilder::add_level_scripts(const LevelScript *in_scripts, int script_count) {
  for (auto i = 0; i < script_count; ++i) {
    scripts.push_back(in_scripts[i]);
  }
  return *this;
}

const LevelScript *LevelScriptBuilder::build() {
  auto script_count = scripts.size();

  auto out_scripts = new LevelScript[script_count];
  for (auto i = 0; i < script_count; ++i) {
    out_scripts[i] = scripts[i];
  }

  return out_scripts;
}

const LevelScript *LevelScriptBuilder::build_with_jump_back_to_start() {
  auto script_count = scripts.size();
  auto jump_instruction_count = 2;

  auto out_scripts = new LevelScript[script_count + jump_instruction_count];
  for (auto i = 0; i < script_count; ++i) {
    out_scripts[i] = scripts[i];
  }

  const LevelScript jump_instructions[] = { JUMP(out_scripts) };
  for (auto i = 0; i < jump_instruction_count; ++i) {
    out_scripts[script_count + i] = scripts[i];
  }

  return out_scripts;
}