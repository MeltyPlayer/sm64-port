#pragma once

#include <memory>
#include <vector>

#include "include/level_table.h"
#include "include/types.h"
#include "util/unused.hpp"

enum class LevelScriptPartType {
  LEVEL_SCRIPT,
  LEVEL_SCRIPTS,

  INSERT_BUILDER,

  JUMP_TO_TOP_OF_THIS_BUILDER,
  JUMP_TO_TOP_OF_OUTERMOST_BUILDER,
  JUMP_LINK_TO_ADDRESS,
  JUMP_IF_EQUAL_TO_BUILDER,

  EXECUTE_BUILDER,
  EXIT_AND_EXECUTE_BUILDER,
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

  LevelScriptBuilder& insert_builder(std::shared_ptr<LevelScriptBuilder> builder);

  LevelScriptBuilder& add_jump_to_top_of_this_builder(u8 jump_offset = 0);
  LevelScriptBuilder& add_jump_to_top_of_outermost_builder(u8 jump_offset = 0);
  LevelScriptBuilder& add_jump_link(const LevelScript* address);
  LevelScriptBuilder& add_jump_if_equal(
      u32 value, std::shared_ptr<LevelScriptBuilder> builder);

  LevelScriptBuilder& add_execute(
      u8 segment, u8* segment_start, u8* segment_end,
      std::shared_ptr<LevelScriptBuilder> builder);
  LevelScriptBuilder& add_exit_and_execute(
      u8 segment, u8* segment_start, u8* segment_end,
      std::shared_ptr<LevelScriptBuilder> builder);

  const LevelScript* build(int& out_count = unused_int, 
                           LevelScript* outer_scripts = nullptr);
  void append_builder(int& out_count,
                      LevelScript* outer_scripts, 
                      LevelScript* inner_scripts);

 private:

  int get_script_count_in_part(const LevelScriptPart& part) const;
  int get_script_count() const;

  std::vector<std::unique_ptr<LevelScriptPart>> parts;
};
