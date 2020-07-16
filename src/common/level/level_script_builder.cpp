#include "level_script_builder.hpp"

#include "level_commands.h"
#include "util/unused.hpp"

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

LevelScriptBuilder& LevelScriptBuilder::add_builder(
    std::shared_ptr<LevelScriptBuilder> builder) {
  auto part = new LevelScriptPart();
  part->type = LevelScriptPartType::BUILDER;
  part->builder = std::move(builder);

  parts.push_back(std::unique_ptr<LevelScriptPart>(part));

  return *this;
}


LevelScriptBuilder& LevelScriptBuilder::add_jump_link(const LevelScript* address) {
  auto part = new LevelScriptPart();
  part->type = LevelScriptPartType::JUMP_LINK;
  part->address = address;

  parts.push_back(std::unique_ptr<LevelScriptPart>(part));

  return *this;
}

LevelScriptBuilder& LevelScriptBuilder::add_jump_if_equal_builder(
    u32 value, std::shared_ptr<LevelScriptBuilder> builder) {
  auto part = new LevelScriptPart();
  part->type = LevelScriptPartType::JUMP_IF_EQUAL_BUILDER;
  part->value = value;
  part->builder = std::move(builder);

  parts.push_back(std::unique_ptr<LevelScriptPart>(part));

  return *this;
}

LevelScriptBuilder& LevelScriptBuilder::add_execute_level(
    u8* segment_start, u8* segment_end,
    std::shared_ptr<LevelScriptBuilder> builder) {
  auto part = new LevelScriptPart();
  part->type = LevelScriptPartType::EXECUTE_LEVEL;
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
  part->type = LevelScriptPartType::EXIT_AND_EXECUTE;
  part->segment_start = segment_start;
  part->segment_end = segment_end;
  part->builder = std::move(builder);

  parts.push_back(std::unique_ptr<LevelScriptPart>(part));

  return *this;
}

LevelScriptBuilder& LevelScriptBuilder::add_jump_to_inner_start(
    u8 jump_offset) {
  auto part = new LevelScriptPart();
  part->type = LevelScriptPartType::JUMP_TO_INNER_START;
  part->jump_offset = jump_offset;

  parts.push_back(std::unique_ptr<LevelScriptPart>(part));

  return *this;
}

LevelScriptBuilder& LevelScriptBuilder::add_jump_to_outer_start(
    u8 jump_offset) {
  auto part = new LevelScriptPart();
  part->type = LevelScriptPartType::JUMP_TO_OUTER_START;
  part->jump_offset = jump_offset;

  parts.push_back(std::unique_ptr<LevelScriptPart>(part));

  return *this;
}

const int JUMP_COUNT = 2;
const int JUMP_LINK_COUNT = 2;
const int JUMP_IF_COUNT = 3;
const int EXECUTE_LEVEL_COUNT = 4;
const int EXIT_AND_EXECUTE_COUNT = 4;

int LevelScriptBuilder::get_script_count_in_part(
    const LevelScriptPart& part) const {
  switch (part.type) {
    case LevelScriptPartType::LEVEL_SCRIPT:
      return 1;
    case LevelScriptPartType::LEVEL_SCRIPTS:
      return part.scripts.size();

    case LevelScriptPartType::BUILDER:
      return part.builder->get_script_count();

    case LevelScriptPartType::JUMP_LINK:
      return JUMP_LINK_COUNT;
    case LevelScriptPartType::JUMP_IF_EQUAL_BUILDER:
      return JUMP_IF_COUNT;

    case LevelScriptPartType::EXECUTE_LEVEL:
      return EXECUTE_LEVEL_COUNT;

    case LevelScriptPartType::JUMP_TO_INNER_START:
    case LevelScriptPartType::JUMP_TO_OUTER_START:
      return JUMP_COUNT;

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

void LevelScriptBuilder::build_recursive(LevelScript* outer_scripts,
                                         LevelScript* inner_scripts,
                                         int& out_count) {
  auto pos = 0;

  const auto part_count = parts.size();
  for (auto p_i = 0; p_i < part_count; ++p_i) {
    const auto part = *parts[p_i];

    auto type = part.type;
    if (type == LevelScriptPartType::LEVEL_SCRIPT) {
      inner_scripts[pos] = part.script;
      ++pos;
    } else if (type == LevelScriptPartType::LEVEL_SCRIPTS) {
      for (auto i = 0; i < part.scripts.size(); ++i) {
        inner_scripts[pos + i] = part.scripts[i];
      }
      pos += part.scripts.size();
    } else if (type == LevelScriptPartType::BUILDER) {
      int inner_count;
      part.builder->build_recursive(outer_scripts, inner_scripts + pos,
                                    inner_count);
      pos += inner_count;
    } else if (type == LevelScriptPartType::JUMP_LINK) {
      const LevelScript jump_link_scripts[] = {
          JUMP_LINK(part.address)
      };

      for (auto i = 0; i < JUMP_LINK_COUNT; ++i) {
        inner_scripts[pos + i] = jump_link_scripts[i];
      }

      pos += JUMP_LINK_COUNT;
    } else if (type == LevelScriptPartType::JUMP_IF_EQUAL_BUILDER) {
      const auto inner_inner_scripts =
          part.builder->build_internal(outer_scripts, unused_int);
      const LevelScript jump_if_level_builder_scripts[] = {
          JUMP_IF(OP_EQ, part.value, inner_inner_scripts)
      };

      for (auto i = 0; i < JUMP_IF_COUNT; ++i) {
        inner_scripts[pos + i] = jump_if_level_builder_scripts[i];
      }

      pos += JUMP_IF_COUNT;
    } else if (type == LevelScriptPartType::EXECUTE_LEVEL) {
      const auto inner_inner_scripts =
          part.builder->build_internal(outer_scripts, unused_int);

      const LevelScript execute_level_scripts[]{
          EXECUTE(0x0E, part.segment_start, part.segment_end,
                  inner_inner_scripts),
      };

      for (auto i = 0; i < EXECUTE_LEVEL_COUNT; ++i) {
        inner_scripts[pos + i] = execute_level_scripts[i];
      }

      pos += EXECUTE_LEVEL_COUNT;
    } else if (type == LevelScriptPartType::EXIT_AND_EXECUTE) {
      const auto inner_inner_scripts =
          part.builder->build_internal(outer_scripts, unused_int);

      const LevelScript exit_and_execute_scripts[]{
          EXIT_AND_EXECUTE(part.segment, part.segment_start, part.segment_end,
                           inner_inner_scripts),
      };

      for (auto i = 0; i < EXIT_AND_EXECUTE_COUNT; ++i) {
        inner_scripts[pos + i] = exit_and_execute_scripts[i];
      }

      pos += EXIT_AND_EXECUTE_COUNT;
    } else if (type == LevelScriptPartType::JUMP_TO_INNER_START) {
      const LevelScript inner_jump_scripts[] = {
          JUMP(inner_scripts + part.jump_offset)
      };

      for (auto i = 0; i < JUMP_COUNT; ++i) {
        inner_scripts[pos + i] = inner_jump_scripts[i];
      }

      pos += JUMP_COUNT;
    } else if (type == LevelScriptPartType::JUMP_TO_OUTER_START) {
      const LevelScript outer_jump_scripts[] = {
          JUMP(outer_scripts + part.jump_offset)
      };

      for (auto i = 0; i < JUMP_COUNT; ++i) {
        inner_scripts[pos + i] = outer_jump_scripts[i];
      }

      pos += JUMP_COUNT;
    }
  }

  out_count = pos;
}

const LevelScript* LevelScriptBuilder::build(int& out_count = unused_int) {
  return build_internal(nullptr, out_count);
}

const LevelScript* LevelScriptBuilder::build_internal(
    LevelScript* outer_scripts = nullptr, int& out_count = unused_int) {
  const auto script_count = get_script_count();

  const auto out_scripts = new LevelScript[script_count];
  build_recursive(outer_scripts != nullptr ? outer_scripts : out_scripts,
                  out_scripts, unused_int);

  out_count = script_count;
  return out_scripts;
}
