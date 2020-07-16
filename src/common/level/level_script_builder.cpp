#include "level_script_builder.hpp"

#include "level_commands.h"
#include "util/unused.hpp"

LevelScriptBuilder& LevelScriptBuilder::
add_level_script(LevelScript in_script) {
  auto part = new LevelScriptPart();
  part->type = LevelScriptPartType::LEVEL_SCRIPT;
  part->script = in_script;

  parts.push_back(std::unique_ptr<LevelScriptPart>(part));

  return *this;
}

LevelScriptBuilder& LevelScriptBuilder::add_level_scripts(
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


LevelScriptBuilder& LevelScriptBuilder::add_builder(
    std::shared_ptr<LevelScriptBuilder> builder) {
  auto part = new LevelScriptPart();
  part->type = LevelScriptPartType::BUILDER;
  part->builder = std::move(builder);

  parts.push_back(std::unique_ptr<LevelScriptPart>(part));

  return *this;
}


LevelScriptBuilder& LevelScriptBuilder::add_jump_to_inner_start() {
  auto part = new LevelScriptPart();
  part->type = LevelScriptPartType::JUMP_TO_INNER_START;

  parts.push_back(std::unique_ptr<LevelScriptPart>(part));

  return *this;
}

LevelScriptBuilder& LevelScriptBuilder::add_jump_to_outer_start() {
  auto part = new LevelScriptPart();
  part->type = LevelScriptPartType::JUMP_TO_OUTER_START;

  parts.push_back(std::unique_ptr<LevelScriptPart>(part));

  return *this;
}


const int JUMP_INSTRUCTION_COUNT = 2;

int LevelScriptBuilder::get_script_count_in_part(const LevelScriptPart& part) const {
  switch (part.type) {
    case LevelScriptPartType::LEVEL_SCRIPT:
      return 1;

    case LevelScriptPartType::LEVEL_SCRIPTS:
      return part.scripts.size();

    case LevelScriptPartType::BUILDER:
      return part.builder->get_script_count();

    case LevelScriptPartType::JUMP_TO_INNER_START:
    case LevelScriptPartType::JUMP_TO_OUTER_START:
      return JUMP_INSTRUCTION_COUNT;

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


void LevelScriptBuilder::build_impl(LevelScript* outer_scripts,
                                    LevelScript* inner_scripts,
                                    int& out_count) {
  const LevelScript inner_jump_scripts[] = {JUMP(inner_scripts)};
  const LevelScript outer_jump_scripts[] = {JUMP(outer_scripts)};

  auto pos = 0;

  const auto part_count = parts.size();
  for (auto p_i = 0; p_i < part_count; ++p_i) {
    const auto part = *parts[p_i];

    switch (part.type) {
      case LevelScriptPartType::LEVEL_SCRIPT:
        inner_scripts[pos] = part.script;
        ++pos;
        break;

      case LevelScriptPartType::LEVEL_SCRIPTS:
        for (auto i = 0; i < part.scripts.size(); ++i) {
          inner_scripts[pos + i] = part.scripts[i];
        }
        pos += part.scripts.size();
        break;

      case LevelScriptPartType::BUILDER:
        int inner_count;
        part.builder->build_impl(outer_scripts, inner_scripts + pos,
                                 inner_count);
        pos += inner_count;
        break;

      case LevelScriptPartType::JUMP_TO_INNER_START:
        for (auto i = 0; i < JUMP_INSTRUCTION_COUNT; ++i) {
          inner_scripts[pos + i] = inner_jump_scripts[i];
        }
        pos += JUMP_INSTRUCTION_COUNT;
        break;

      case LevelScriptPartType::JUMP_TO_OUTER_START:
        for (auto i = 0; i < JUMP_INSTRUCTION_COUNT; ++i) {
          inner_scripts[pos + i] = outer_jump_scripts[i];
        }
        pos += JUMP_INSTRUCTION_COUNT;
        break;
    }
  }

  out_count = pos;
}


const LevelScript* LevelScriptBuilder::build(int& out_count = unused_int) {
  const auto script_count = get_script_count();

  const auto out_scripts = new LevelScript[script_count];
  build_impl(out_scripts, out_scripts, unused_int);

  out_count = script_count;
  return out_scripts;
}
