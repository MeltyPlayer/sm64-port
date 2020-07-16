#pragma once

#include <memory>
#include <vector>

#include "include/level_table.h"
#include "include/types.h"

enum class LevelScriptPartType {
  LEVEL_SCRIPT,
  LEVEL_SCRIPTS,
  BUILDER,
  JUMP_LINK,
  JUMP_IF_EQUAL_BUILDER,
  EXECUTE_LEVEL,
  EXIT_AND_EXECUTE,
  JUMP_TO_INNER_START,
  JUMP_TO_OUTER_START,
};

class LevelScriptBuilder;

struct LevelScriptPart {
  LevelScriptPartType type;
  LevelScript script;
  std::vector<LevelScript> scripts;
  u32 value;
  u8 segment;
  u8* segment_start;
  u8* segment_end;
  std::shared_ptr<LevelScriptBuilder> builder;
  const LevelScript* address;
  u8 jump_offset;
};

class LevelScriptBuilder {
public:
  LevelScriptBuilder& add_level_script(LevelScript in_script);
  LevelScriptBuilder& add_level_scripts(const LevelScript* in_scripts,
                                        int script_count);

  LevelScriptBuilder& add_builder(std::shared_ptr<LevelScriptBuilder> builder);

  LevelScriptBuilder& add_jump_link(const LevelScript* address);
  LevelScriptBuilder& add_jump_if_equal_builder(
      u32 value, std::shared_ptr<LevelScriptBuilder> builder);

  LevelScriptBuilder& add_execute_level(
      u8* segment_start, u8* segment_end,
      std::shared_ptr<LevelScriptBuilder> builder);
  LevelScriptBuilder& add_exit_and_execute(
      u8 segment, u8* segment_start, u8* segment_end,
      std::shared_ptr<LevelScriptBuilder> builder);

  LevelScriptBuilder& add_jump_to_inner_start(u8 jump_offset = 0);
  LevelScriptBuilder& add_jump_to_outer_start(u8 jump_offset = 0);

  const LevelScript* build(int& out_count);

private:
  const LevelScript* build_internal(LevelScript* outer_scripts, int& out_count);
  void build_recursive(LevelScript* outer_scripts,
                       LevelScript* inner_scripts,
                       int& out_count);

  int get_script_count_in_part(const LevelScriptPart& part) const;
  int get_script_count() const;

  std::vector<std::unique_ptr<LevelScriptPart>> parts;
};
