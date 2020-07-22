#include "macro_level_script_builder.hpp"

#include "include/level_commands.h"

#include "util.hpp"

MacroLevelScriptBuilder& MacroLevelScriptBuilder::add_script(
    LevelScript in_script) {
  auto part = new LevelScriptPart();
  part->type = LevelScriptPartType::LEVEL_SCRIPT;
  part->script = in_script;

  parts.push_back(std::unique_ptr<LevelScriptPart>(part));

  return *this;
}

MacroLevelScriptBuilder& MacroLevelScriptBuilder::add_scripts(
    std::initializer_list<const LevelScript> in_scripts) {
  auto part = new LevelScriptPart();
  part->type = LevelScriptPartType::LEVEL_SCRIPTS;
  std::copy(
      in_scripts.begin(),
      in_scripts.end(),
      std::back_inserter(part->scripts));

  parts.push_back(std::unique_ptr<LevelScriptPart>(part));

  return *this;
}

MacroLevelScriptBuilder& MacroLevelScriptBuilder::add_scripts(
    const LevelScript* in_scripts,
    int script_count) {
  auto part = new LevelScriptPart();
  part->type = LevelScriptPartType::LEVEL_SCRIPTS;
  for (auto i = 0; i < script_count; ++i) {
    part->scripts.push_back(in_scripts[i]);
  }

  parts.push_back(std::unique_ptr<LevelScriptPart>(part));

  return *this;
}

MacroLevelScriptBuilder& MacroLevelScriptBuilder::add_builder(
    std::shared_ptr<IScriptBuilder<LevelScript>> builder) {
  auto part = new LevelScriptPart();
  part->type = LevelScriptPartType::INSERT_BUILDER;
  part->builder = std::move(builder);

  parts.push_back(std::unique_ptr<LevelScriptPart>(part));

  return *this;
}

MacroLevelScriptBuilder& MacroLevelScriptBuilder::add_call(
    void (*callback)(void)) {
  auto part = new LevelScriptPart();
  part->type = LevelScriptPartType::CALL;
  part->callback = callback;

  parts.push_back(std::unique_ptr<LevelScriptPart>(part));

  return *this;
}

MacroLevelScriptBuilder& MacroLevelScriptBuilder::add_jump_link(
    const LevelScript* address) {
  auto part = new LevelScriptPart();
  part->type = LevelScriptPartType::JUMP_LINK_TO_ADDRESS;
  part->address = address;

  parts.push_back(std::unique_ptr<LevelScriptPart>(part));

  return *this;
}

MacroLevelScriptBuilder& MacroLevelScriptBuilder::add_jump_link(
    std::shared_ptr<ILevelScriptBuilder> builder) {
  auto part = new LevelScriptPart();
  part->type = LevelScriptPartType::JUMP_LINK_TO_BUILDER;
  part->builder = std::move(builder);

  parts.push_back(std::unique_ptr<LevelScriptPart>(part));

  return *this;
}

MacroLevelScriptBuilder& MacroLevelScriptBuilder::add_jump_if_equal(
    u32 value,
    const LevelScript* address) {
  auto part = new LevelScriptPart();
  part->type = LevelScriptPartType::JUMP_IF_EQUAL_TO_ADDRESS;
  part->value = value;
  part->address = address;

  parts.push_back(std::unique_ptr<LevelScriptPart>(part));

  return *this;
}

MacroLevelScriptBuilder& MacroLevelScriptBuilder::add_jump_if_equal(
    u32 value,
    std::shared_ptr<ILevelScriptBuilder> builder) {
  auto part = new LevelScriptPart();
  part->type = LevelScriptPartType::JUMP_IF_EQUAL_TO_BUILDER;
  part->value = value;
  part->builder = std::move(builder);

  parts.push_back(std::unique_ptr<LevelScriptPart>(part));

  return *this;
}

MacroLevelScriptBuilder& MacroLevelScriptBuilder::add_execute(
    u8 segment,
    const u8* segment_start,
    const u8* segment_end,
    const LevelScript* address) {
  auto part = new LevelScriptPart();
  part->type = LevelScriptPartType::EXECUTE_ADDRESS;
  part->segment = segment;
  part->segment_start = segment_start;
  part->segment_end = segment_end;
  part->address = address;

  parts.push_back(std::unique_ptr<LevelScriptPart>(part));

  return *this;
}

MacroLevelScriptBuilder& MacroLevelScriptBuilder::add_execute(
    u8 segment,
    const u8* segment_start,
    const u8* segment_end,
    std::shared_ptr<ILevelScriptBuilder> builder) {
  auto part = new LevelScriptPart();
  part->type = LevelScriptPartType::EXECUTE_BUILDER;
  part->segment = segment;
  part->segment_start = segment_start;
  part->segment_end = segment_end;
  part->builder = std::move(builder);

  parts.push_back(std::unique_ptr<LevelScriptPart>(part));

  return *this;
}

MacroLevelScriptBuilder& MacroLevelScriptBuilder::add_exit_and_execute(
    u8 segment,
    const u8* segment_start,
    const u8* segment_end,
    std::shared_ptr<ILevelScriptBuilder> builder) {
  auto part = new LevelScriptPart();
  part->type = LevelScriptPartType::EXIT_AND_EXECUTE_BUILDER;
  part->segment = segment;
  part->segment_start = segment_start;
  part->segment_end = segment_end;
  part->builder = std::move(builder);

  parts.push_back(std::unique_ptr<LevelScriptPart>(part));

  return *this;
}

MacroLevelScriptBuilder&
MacroLevelScriptBuilder::add_jump_to_top_of_this_builder(u8 jump_offset) {
  auto part = new LevelScriptPart();
  part->type = LevelScriptPartType::JUMP_TO_TOP_OF_THIS_BUILDER;
  part->jump_offset = jump_offset;

  parts.push_back(std::unique_ptr<LevelScriptPart>(part));

  return *this;
}

int MacroLevelScriptBuilder::size() const {
  auto script_count = 0;

  for (const auto &part : parts) {
    script_count += part->size();
  }

  return script_count;
}

void MacroLevelScriptBuilder::build_into(LevelScript* dst, int& dst_pos) const {
        append_exit_and_execute(dst,
  for (const auto &part : parts) {
    part->build_into(dst, dst_pos);
  }
}
