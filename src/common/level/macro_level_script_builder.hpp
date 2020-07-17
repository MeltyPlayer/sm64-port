#pragma once

#include <memory>
#include <vector>


#include "include/level_table.h"
#include "include/types.h"
#include "util/unused.hpp"
#include "i_level_script_builder.hpp"

enum class MacroLevelScriptPartType {
  LEVEL_SCRIPT,
  LEVEL_SCRIPTS,

  INSERT_BUILDER,

  JUMP_TO_TOP_OF_THIS_BUILDER,
  JUMP_TO_TOP_OF_OUTERMOST_BUILDER,
  JUMP_LINK_TO_ADDRESS,
  JUMP_LINK_TO_BUILDER,
  JUMP_IF_EQUAL_TO_ADDRESS,
  JUMP_IF_EQUAL_TO_BUILDER,

  EXECUTE_ADDRESS,
  EXECUTE_BUILDER,
  EXIT_AND_EXECUTE_BUILDER,
};

class MacroLevelScriptBuilder;

struct MacroLevelScriptPart {
public:
  MacroLevelScriptPartType type;
  LevelScript script;
  std::vector<LevelScript> scripts;
  u32 value;
  u8 segment;
  const u8* segment_start;
  const u8* segment_end;
  std::shared_ptr<ILevelScriptBuilder> builder;
  const LevelScript* address;
  u8 jump_offset;
};

class MacroLevelScriptBuilder final : public ILevelScriptBuilder {
public:
  MacroLevelScriptBuilder() {}
  MacroLevelScriptBuilder(const MacroLevelScriptBuilder& other) = delete;

  MacroLevelScriptBuilder& add_level_script(LevelScript in_script);
  MacroLevelScriptBuilder& add_level_scripts(const LevelScript* in_scripts,
                         int script_count);

  MacroLevelScriptBuilder& insert_builder(
      std::shared_ptr<ILevelScriptBuilder> builder);

  MacroLevelScriptBuilder& add_jump_to_top_of_this_builder(
      u8 jump_offset = 0);
  MacroLevelScriptBuilder& add_jump_to_top_of_outermost_builder(
      u8 jump_offset = 0);
  MacroLevelScriptBuilder& add_jump_link(const LevelScript* address);
  MacroLevelScriptBuilder& add_jump_link(
      std::shared_ptr<ILevelScriptBuilder> builder);
  MacroLevelScriptBuilder& add_jump_if_equal(u32 value,
                         const LevelScript* address);
  MacroLevelScriptBuilder& add_jump_if_equal(
      u32 value, std::shared_ptr<ILevelScriptBuilder> builder);

  MacroLevelScriptBuilder& add_execute(u8 segment,
                   const u8* segment_start,
                   const u8* segment_end,
                   const LevelScript* address);
  MacroLevelScriptBuilder& add_execute(
      u8 segment,
                   const u8* segment_start,
                   const u8* segment_end,
                   std::shared_ptr<ILevelScriptBuilder> builder);
  MacroLevelScriptBuilder& add_exit_and_execute(
      u8 segment, const u8* segment_start, const u8* segment_end,
      std::shared_ptr<ILevelScriptBuilder> builder);

  void append_builder(int& out_count,
                      LevelScript* outer_scripts,
                      LevelScript* inner_scripts) override;

  int get_script_count() const override;

 protected:
  const LevelScript* build_impl(int& out_count,
                                LevelScript* outer_scripts) override;

private:
  int get_script_count_in_part(const MacroLevelScriptPart& part) const;

  std::vector<std::unique_ptr<MacroLevelScriptPart>> parts;
};
