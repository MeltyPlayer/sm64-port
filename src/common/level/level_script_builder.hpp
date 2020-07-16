#pragma once

#include <memory>
#include <vector>

#include "include/types.h"

enum class LevelScriptPartType {
  LEVEL_SCRIPT,
  LEVEL_SCRIPTS,
  BUILDER,
  JUMP_TO_INNER_START,
  JUMP_TO_OUTER_START,
};

class LevelScriptBuilder;

struct LevelScriptPart {
  LevelScriptPartType type;
  LevelScript script;
  std::vector<LevelScript> scripts;
  std::shared_ptr<LevelScriptBuilder> builder;
};

class LevelScriptBuilder {
public:
  LevelScriptBuilder& add_level_script(LevelScript in_script);
  LevelScriptBuilder& add_level_scripts(const LevelScript* in_scripts,
                                        int script_count);

  LevelScriptBuilder& add_builder(std::shared_ptr<LevelScriptBuilder> builder);

  LevelScriptBuilder& add_jump_to_inner_start();
  LevelScriptBuilder& add_jump_to_outer_start();

  const LevelScript* build(int& out_count);

private:
  void build_impl(LevelScript* outer_scripts,
                  LevelScript* inner_scripts,
                  int& out_count);

  int get_script_count_in_part(const LevelScriptPart& part) const;
  int get_script_count() const;

  std::vector<std::unique_ptr<LevelScriptPart>> parts;
};
