#include "macro_level_script_builder.hpp"

#include "include/level_commands.h"
#include "util/unused.hpp"

#include "constants.hpp"
#include "util.hpp"

MacroLevelScriptBuilder& MacroLevelScriptBuilder::add_script(
    LevelScript in_script) {
  auto part = new MacroLevelScriptPart();
  part->type = MacroLevelScriptPartType::LEVEL_SCRIPT;
  part->script = in_script;

  parts.push_back(std::unique_ptr<MacroLevelScriptPart>(part));

  return *this;
}

MacroLevelScriptBuilder& MacroLevelScriptBuilder::add_scripts(
    std::initializer_list<const LevelScript> in_scripts) {
  auto part = new MacroLevelScriptPart();
  part->type = MacroLevelScriptPartType::LEVEL_SCRIPTS;
  std::copy(
      in_scripts.begin(),
      in_scripts.end(),
      std::back_inserter(part->scripts));

  parts.push_back(std::unique_ptr<MacroLevelScriptPart>(part));

  return *this;
}

MacroLevelScriptBuilder& MacroLevelScriptBuilder::add_scripts(
    const LevelScript* in_scripts,
    int script_count) {
  auto part = new MacroLevelScriptPart();
  part->type = MacroLevelScriptPartType::LEVEL_SCRIPTS;
  for (auto i = 0; i < script_count; ++i) {
    part->scripts.push_back(in_scripts[i]);
  }

  parts.push_back(std::unique_ptr<MacroLevelScriptPart>(part));

  return *this;
}

MacroLevelScriptBuilder& MacroLevelScriptBuilder::insert_builder(
    std::shared_ptr<ILevelScriptBuilder> builder) {
  auto part = new MacroLevelScriptPart();
  part->type = MacroLevelScriptPartType::INSERT_BUILDER;
  part->builder = std::move(builder);

  parts.push_back(std::unique_ptr<MacroLevelScriptPart>(part));

  return *this;
}

MacroLevelScriptBuilder& MacroLevelScriptBuilder::add_call(
    void (*callback)(void)) {
  auto part = new MacroLevelScriptPart();
  part->type = MacroLevelScriptPartType::CALL;
  part->callback = callback;

  parts.push_back(std::unique_ptr<MacroLevelScriptPart>(part));

  return *this;
}

MacroLevelScriptBuilder& MacroLevelScriptBuilder::add_jump_link(
    const LevelScript* address) {
  auto part = new MacroLevelScriptPart();
  part->type = MacroLevelScriptPartType::JUMP_LINK_TO_ADDRESS;
  part->address = address;

  parts.push_back(std::unique_ptr<MacroLevelScriptPart>(part));

  return *this;
}

MacroLevelScriptBuilder& MacroLevelScriptBuilder::add_jump_link(
    std::shared_ptr<ILevelScriptBuilder> builder) {
  auto part = new MacroLevelScriptPart();
  part->type = MacroLevelScriptPartType::JUMP_LINK_TO_BUILDER;
  part->builder = std::move(builder);

  parts.push_back(std::unique_ptr<MacroLevelScriptPart>(part));

  return *this;
}

MacroLevelScriptBuilder& MacroLevelScriptBuilder::add_jump_if_equal(
    u32 value,
    const LevelScript* address) {
  auto part = new MacroLevelScriptPart();
  part->type = MacroLevelScriptPartType::JUMP_IF_EQUAL_TO_ADDRESS;
  part->value = value;
  part->address = address;

  parts.push_back(std::unique_ptr<MacroLevelScriptPart>(part));

  return *this;
}

MacroLevelScriptBuilder& MacroLevelScriptBuilder::add_jump_if_equal(
    u32 value,
    std::shared_ptr<ILevelScriptBuilder> builder) {
  auto part = new MacroLevelScriptPart();
  part->type = MacroLevelScriptPartType::JUMP_IF_EQUAL_TO_BUILDER;
  part->value = value;
  part->builder = std::move(builder);

  parts.push_back(std::unique_ptr<MacroLevelScriptPart>(part));

  return *this;
}

MacroLevelScriptBuilder& MacroLevelScriptBuilder::add_execute(
    u8 segment,
    const u8* segment_start,
    const u8* segment_end,
    const LevelScript* address) {
  auto part = new MacroLevelScriptPart();
  part->type = MacroLevelScriptPartType::EXECUTE_ADDRESS;
  part->segment = segment;
  part->segment_start = segment_start;
  part->segment_end = segment_end;
  part->address = address;

  parts.push_back(std::unique_ptr<MacroLevelScriptPart>(part));

  return *this;
}

MacroLevelScriptBuilder& MacroLevelScriptBuilder::add_execute(
    u8 segment,
    const u8* segment_start,
    const u8* segment_end,
    std::shared_ptr<ILevelScriptBuilder> builder) {
  auto part = new MacroLevelScriptPart();
  part->type = MacroLevelScriptPartType::EXECUTE_BUILDER;
  part->segment = segment;
  part->segment_start = segment_start;
  part->segment_end = segment_end;
  part->builder = std::move(builder);

  parts.push_back(std::unique_ptr<MacroLevelScriptPart>(part));

  return *this;
}

MacroLevelScriptBuilder& MacroLevelScriptBuilder::add_exit_and_execute(
    u8 segment,
    const u8* segment_start,
    const u8* segment_end,
    std::shared_ptr<ILevelScriptBuilder> builder) {
  auto part = new MacroLevelScriptPart();
  part->type = MacroLevelScriptPartType::EXIT_AND_EXECUTE_BUILDER;
  part->segment = segment;
  part->segment_start = segment_start;
  part->segment_end = segment_end;
  part->builder = std::move(builder);

  parts.push_back(std::unique_ptr<MacroLevelScriptPart>(part));

  return *this;
}

MacroLevelScriptBuilder&
MacroLevelScriptBuilder::add_jump_to_top_of_this_builder(u8 jump_offset) {
  auto part = new MacroLevelScriptPart();
  part->type = MacroLevelScriptPartType::JUMP_TO_TOP_OF_THIS_BUILDER;
  part->jump_offset = jump_offset;

  parts.push_back(std::unique_ptr<MacroLevelScriptPart>(part));

  return *this;
}

int MacroLevelScriptBuilder::get_script_count_in_part(
    const MacroLevelScriptPart& part) const {
  switch (part.type) {
    case MacroLevelScriptPartType::LEVEL_SCRIPT:
      return 1;
    case MacroLevelScriptPartType::LEVEL_SCRIPTS:
      return part.scripts.size();

    case MacroLevelScriptPartType::INSERT_BUILDER:
      return part.builder->size();

    case MacroLevelScriptPartType::CALL:
      return 2;

    case MacroLevelScriptPartType::JUMP_TO_TOP_OF_THIS_BUILDER:
      return JUMP_COUNT;
    case MacroLevelScriptPartType::JUMP_LINK_TO_ADDRESS:
    case MacroLevelScriptPartType::JUMP_LINK_TO_BUILDER:
      return JUMP_LINK_COUNT;
    case MacroLevelScriptPartType::JUMP_IF_EQUAL_TO_ADDRESS:
    case MacroLevelScriptPartType::JUMP_IF_EQUAL_TO_BUILDER:
      return JUMP_IF_COUNT;

    case MacroLevelScriptPartType::EXECUTE_ADDRESS:
    case MacroLevelScriptPartType::EXECUTE_BUILDER:
      return EXECUTE_COUNT;
    case MacroLevelScriptPartType::EXIT_AND_EXECUTE_BUILDER:
      return EXIT_AND_EXECUTE_COUNT;

    default:
      return 0;
  }
}

int MacroLevelScriptBuilder::size() const {
  auto script_count = 0;

  const auto part_count = parts.size();
  for (auto i = 0; i < part_count; ++i) {
    const auto part = *parts[i];
    script_count += get_script_count_in_part(part);
  }

  return script_count;
}

typedef void (*NativeFunc)(void);

void MacroLevelScriptBuilder::build_into(LevelScript* dst, int& dst_pos) {
  const auto part_count = parts.size();
  for (auto p_i = 0; p_i < part_count; ++p_i) {
    const auto part = *parts[p_i];

    auto type = part.type;
    switch (part.type) {
      case MacroLevelScriptPartType::LEVEL_SCRIPT:
        append_script(dst, dst_pos, part.script);
        break;
      case MacroLevelScriptPartType::LEVEL_SCRIPTS:
        append_scripts(
            dst,
            dst_pos,
            &part.scripts[0],
            part.scripts.size());
        break;

      case MacroLevelScriptPartType::INSERT_BUILDER:
        part.builder->build_into(dst, dst_pos);
        break;

      case MacroLevelScriptPartType::CALL:
        append_scripts(
            dst,
            dst_pos,
            {CALL(part.callback_arg, part.callback)});
        break;

      case MacroLevelScriptPartType::JUMP_TO_TOP_OF_THIS_BUILDER:
        append_jump_to_address(
            dst,
            dst_pos,
            dst + part.jump_offset);
        break;
      case MacroLevelScriptPartType::JUMP_LINK_TO_ADDRESS:
        append_jump_link_to_address(dst, dst_pos, part.address);
        break;
      case MacroLevelScriptPartType::JUMP_LINK_TO_BUILDER: {
        const auto inner_inner_scripts =
            part.builder->get_entry_pointer(unused_int);
        append_jump_link_to_address(dst, dst_pos, inner_inner_scripts);
        break;
      }
      case MacroLevelScriptPartType::JUMP_IF_EQUAL_TO_ADDRESS:
        append_jump_if_equal_to_address(
            dst,
            dst_pos,
            part.value,
            part.address);
        break;
      case MacroLevelScriptPartType::JUMP_IF_EQUAL_TO_BUILDER: {
        const auto inner_inner_scripts =
            part.builder->get_entry_pointer(unused_int);
        append_jump_if_equal_to_address(
            dst,
            dst_pos,
            part.value,
            inner_inner_scripts);
        break;
      }

      case MacroLevelScriptPartType::EXECUTE_ADDRESS:
        append_execute(dst,
                       dst_pos,
                       part.segment,
                       part.segment_start,
                       part.segment_end,
                       part.address);
        break;
      case MacroLevelScriptPartType::EXECUTE_BUILDER: {
        const auto inner_inner_scripts =
            part.builder->get_entry_pointer(unused_int);
        append_execute(dst,
                       dst_pos,
                       part.segment,
                       part.segment_start,
                       part.segment_end,
                       inner_inner_scripts);
        break;
      }
      case MacroLevelScriptPartType::EXIT_AND_EXECUTE_BUILDER: {
        const auto inner_inner_scripts =
            part.builder->get_entry_pointer(unused_int);
        append_exit_and_execute(dst,
                                dst_pos,
                                part.segment,
                                part.segment_start,
                                part.segment_end,
                                inner_inner_scripts);
        break;
      }
    }
  }
}
