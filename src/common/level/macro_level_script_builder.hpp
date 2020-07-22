#pragma once

#include <memory>
#include <vector>

#include "common/script/i_level_script_builder.hpp"
#include "common/script/validation_node.hpp"
#include "include/types.h"

#include "level_script_part.hpp"

/*
  A wrapper around the macros that define levels in Super Mario 64. This class
  guarantees type-safety, simplifies calls, and allows us to incrementally
  convert levels to a more readable format.
*/
// TODO: Rebuilding the macro allocates new memory. Currently this poses a
// memory leak risk.
// TODO: Should cache the build, update if changes are made.
// TODO: Each version of the build should maintain the same pointer location.
class MacroLevelScriptBuilder final : public ILevelScriptBuilder {
public:
  MacroLevelScriptBuilder() = default;
  MacroLevelScriptBuilder(const MacroLevelScriptBuilder& other) = delete;

  MacroLevelScriptBuilder& add_part(
      std::shared_ptr<IScriptPart<LevelScript>> part) override;

  MacroLevelScriptBuilder& add_script(LevelScript in_script) override;
  MacroLevelScriptBuilder& add_scripts(
      std::initializer_list<const LevelScript> in_scripts) override;
  MacroLevelScriptBuilder& add_scripts(const LevelScript* in_scripts,
                                       int script_count) override;

  MacroLevelScriptBuilder& add_builder(
      std::shared_ptr<IScriptBuilder<LevelScript>> builder) override;

  MacroLevelScriptBuilder& add_call(void (*callback)(void));

  template<typename T>
  MacroLevelScriptBuilder& add_call(void (*callback)(T*), T* value) {
    auto part = new LevelScriptPart();
    part->type = LevelScriptPartType::CALL;
    part->callback = (void (*)(void)) callback;
    part->callback_arg = (uintptr_t) value;

    parts_.push_back(std::unique_ptr<LevelScriptPart>(part));

    return *this;
  }

  MacroLevelScriptBuilder& add_jump_to_top_of_this_builder(
      u8 jump_offset = 0);
  MacroLevelScriptBuilder& add_jump_link(const LevelScript* address);
  MacroLevelScriptBuilder& add_jump_link(
      std::shared_ptr<ILevelScriptBuilder> builder);
  MacroLevelScriptBuilder& add_jump_if_equal(u32 value,
                                             const LevelScript* address);
  MacroLevelScriptBuilder& add_jump_if_equal(
      u32 value,
      std::shared_ptr<ILevelScriptBuilder> builder);

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
      u8 segment,
      const u8* segment_start,
      const u8* segment_end,
      std::shared_ptr<ILevelScriptBuilder> builder);

  int size() const override;
  void build_into(LevelScript* dst, int& dst_pos) const override;
  ValidationNode& get_cache_validation_node() override;

 private:
  bool is_cache_valid() const;
  void invalidate_cache();

  std::vector<std::shared_ptr<IScriptPart<LevelScript>>> parts_;

  ValidationNode cache_validation_impl_;
};
