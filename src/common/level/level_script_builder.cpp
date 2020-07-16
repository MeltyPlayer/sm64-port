#include "level_script_builder.hpp"

#include "level_commands.h"
#include "util/unused.hpp"
#include "constants.hpp"
#include "util.hpp"

LevelScriptBuilder& LevelScriptBuilder::add_level_script(
    LevelScript in_script) {
  auto part = new LevelScriptPart();
  part->type = LevelScriptPartType::LEVEL_SCRIPT;
  part->script = in_script;

  parts.push_back(std::unique_ptr<LevelScriptPart>(part));

  return *this;
}

LevelScriptBuilder& LevelScriptBuilder::add_level_scripts(
    const LevelScript* in_scripts, int script_count) {
  auto part = new LevelScriptPart();
  part->type = LevelScriptPartType::LEVEL_SCRIPTS;
  for (auto i = 0; i < script_count; ++i) {
    part->scripts.push_back(in_scripts[i]);
  }

  parts.push_back(std::unique_ptr<LevelScriptPart>(part));

  return *this;
}

LevelScriptBuilder& LevelScriptBuilder::insert_builder(
    std::shared_ptr<LevelScriptBuilder> builder) {
  auto part = new LevelScriptPart();
  part->type = LevelScriptPartType::INSERT_BUILDER;
  part->builder = std::move(builder);

  parts.push_back(std::unique_ptr<LevelScriptPart>(part));

  return *this;
}


LevelScriptBuilder& LevelScriptBuilder::add_jump_link(
    const LevelScript* address) {
  auto part = new LevelScriptPart();
  part->type = LevelScriptPartType::JUMP_LINK_TO_ADDRESS;
  part->address = address;

  parts.push_back(std::unique_ptr<LevelScriptPart>(part));

  return *this;
}

LevelScriptBuilder& LevelScriptBuilder::add_jump_link(
    std::shared_ptr<LevelScriptBuilder> builder) {
  auto part = new LevelScriptPart();
  part->type = LevelScriptPartType::JUMP_LINK_TO_BUILDER;
  part->builder = std::move(builder);

  parts.push_back(std::unique_ptr<LevelScriptPart>(part));

  return *this;
}

LevelScriptBuilder& LevelScriptBuilder::add_jump_if_equal(
    u32 value, std::shared_ptr<LevelScriptBuilder> builder) {
  auto part = new LevelScriptPart();
  part->type = LevelScriptPartType::JUMP_IF_EQUAL_TO_BUILDER;
  part->value = value;
  part->builder = std::move(builder);

  parts.push_back(std::unique_ptr<LevelScriptPart>(part));

  return *this;
}

LevelScriptBuilder& LevelScriptBuilder::add_execute(
    u8 segment, u8* segment_start,
    u8* segment_end,
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


LevelScriptBuilder& LevelScriptBuilder::add_execute(
    u8 segment, u8* segment_start, u8* segment_end,
    std::shared_ptr<LevelScriptBuilder> builder) {
  auto part = new LevelScriptPart();
  part->type = LevelScriptPartType::EXECUTE_BUILDER;
  part->segment = segment;
  part->segment_start = segment_start;
  part->segment_end = segment_end;
  part->builder = std::move(builder);

  parts.push_back(std::unique_ptr<LevelScriptPart>(part));

  return *this;
}

LevelScriptBuilder& LevelScriptBuilder::add_exit_and_execute(
    u8 segment, u8* segment_start, u8* segment_end,
    std::shared_ptr<LevelScriptBuilder> builder) {
  auto part = new LevelScriptPart();
  part->type = LevelScriptPartType::EXIT_AND_EXECUTE_BUILDER;
  part->segment = segment;
  part->segment_start = segment_start;
  part->segment_end = segment_end;
  part->builder = std::move(builder);

  parts.push_back(std::unique_ptr<LevelScriptPart>(part));

  return *this;
}

LevelScriptBuilder& LevelScriptBuilder::add_jump_to_top_of_this_builder(
    u8 jump_offset) {
  auto part = new LevelScriptPart();
  part->type = LevelScriptPartType::JUMP_TO_TOP_OF_THIS_BUILDER;
  part->jump_offset = jump_offset;

  parts.push_back(std::unique_ptr<LevelScriptPart>(part));

  return *this;
}

LevelScriptBuilder& LevelScriptBuilder::add_jump_to_top_of_outermost_builder(
    u8 jump_offset) {
  auto part = new LevelScriptPart();
  part->type = LevelScriptPartType::JUMP_TO_TOP_OF_OUTERMOST_BUILDER;
  part->jump_offset = jump_offset;

  parts.push_back(std::unique_ptr<LevelScriptPart>(part));

  return *this;
}

int LevelScriptBuilder::get_script_count_in_part(
    const LevelScriptPart& part) const {
  switch (part.type) {
    case LevelScriptPartType::LEVEL_SCRIPT:
      return 1;
    case LevelScriptPartType::LEVEL_SCRIPTS:
      return part.scripts.size();

    case LevelScriptPartType::INSERT_BUILDER:
      return part.builder->get_script_count();

    case LevelScriptPartType::JUMP_TO_TOP_OF_THIS_BUILDER:
    case LevelScriptPartType::JUMP_TO_TOP_OF_OUTERMOST_BUILDER:
      return JUMP_COUNT;
    case LevelScriptPartType::JUMP_LINK_TO_ADDRESS:
    case LevelScriptPartType::JUMP_LINK_TO_BUILDER:
      return JUMP_LINK_COUNT;
    case LevelScriptPartType::JUMP_IF_EQUAL_TO_BUILDER:
      return JUMP_IF_COUNT;

    case LevelScriptPartType::EXECUTE_ADDRESS:
    case LevelScriptPartType::EXECUTE_BUILDER:
      return EXECUTE_COUNT;
    case LevelScriptPartType::EXIT_AND_EXECUTE_BUILDER:
      return EXIT_AND_EXECUTE_COUNT;

    default:
      return 0;
  }
}

int LevelScriptBuilder::get_script_count() const {
  auto script_count = 0;

  const auto part_count = parts.size();
  for (auto i = 0; i < part_count; ++i) {
    const auto part = *parts[i];
    script_count += get_script_count_in_part(part);
  }

  return script_count;
}

void LevelScriptBuilder::append_builder(int& out_count,
                                        LevelScript* outer_scripts,
                                        LevelScript* inner_scripts) {
  auto pos = 0;

  const auto part_count = parts.size();
  for (auto p_i = 0; p_i < part_count; ++p_i) {
    const auto part = *parts[p_i];

    auto type = part.type;
    switch (part.type) {
      case LevelScriptPartType::LEVEL_SCRIPT:
        append_script(inner_scripts, pos,
                      part.script);
        break;
      case LevelScriptPartType::LEVEL_SCRIPTS:
        append_scripts(inner_scripts, pos,
                       &part.scripts[0],
                       part.scripts.size());
        break;

      case LevelScriptPartType::INSERT_BUILDER:
        int inner_count;
        part.builder->append_builder(inner_count,
                                     outer_scripts,
                                     inner_scripts + pos);
        pos += inner_count;
        break;

      case LevelScriptPartType::JUMP_TO_TOP_OF_THIS_BUILDER:
        append_jump_to_address(inner_scripts, pos,
                               inner_scripts + part.jump_offset);
        break;
      case LevelScriptPartType::JUMP_TO_TOP_OF_OUTERMOST_BUILDER:
        append_jump_to_address(inner_scripts, pos,
                               outer_scripts + part.jump_offset);
        break;
      case LevelScriptPartType::JUMP_LINK_TO_ADDRESS:
        append_jump_link_to_address(inner_scripts, pos, part.address);
        break;
      case LevelScriptPartType::JUMP_LINK_TO_BUILDER: {
        const auto inner_inner_scripts =
            part.builder->build(unused_int, outer_scripts);
        append_jump_link_to_address(inner_scripts, pos,
                                    inner_inner_scripts);
        break;
      }
      case LevelScriptPartType::JUMP_IF_EQUAL_TO_BUILDER: {
        const auto inner_inner_scripts =
            part.builder->build(unused_int, outer_scripts);
        append_jump_if_equal_to_address(inner_scripts, pos,
                                        part.value,
                                        inner_inner_scripts);
        break;
      }

      case LevelScriptPartType::EXECUTE_ADDRESS:
        append_execute(inner_scripts, pos,
                       part.segment,
                       part.segment_start,
                       part.segment_end,
                       part.address);
        break;
      case LevelScriptPartType::EXECUTE_BUILDER: {
        const auto inner_inner_scripts =
            part.builder->build(unused_int, outer_scripts);
        append_execute(inner_scripts, pos,
                       part.segment,
                       part.segment_start,
                       part.segment_end,
                       inner_inner_scripts);
        break;
      }
      case LevelScriptPartType::EXIT_AND_EXECUTE_BUILDER: {
        const auto inner_inner_scripts =
            part.builder->build(unused_int, outer_scripts);
        append_exit_and_execute(inner_scripts, pos,
                                part.segment,
                                part.segment_start,
                                part.segment_end,
                                inner_inner_scripts);
        break;
      }
    }
  }

  out_count = pos;
}

const LevelScript* LevelScriptBuilder::build(
    int& out_count,
    LevelScript* outer_scripts) {
  const auto script_count = get_script_count();

  const auto out_scripts = new LevelScript[script_count];
  append_builder(unused_int,
                 outer_scripts != nullptr ? outer_scripts : out_scripts,
                 out_scripts);

  out_count = script_count;
  return out_scripts;
}
