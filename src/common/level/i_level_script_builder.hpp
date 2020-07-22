#pragma once

#include <memory>

#include "include/types.h"
#include "util/unused.hpp"

class ILevelScriptBuilder {
public:
  ILevelScriptBuilder() = default;
  ILevelScriptBuilder(const ILevelScriptBuilder& other) = delete;

  virtual ILevelScriptBuilder& add_script(LevelScript script) = 0;
  virtual ILevelScriptBuilder& add_scripts(
      std::initializer_list<const LevelScript> scripts) = 0;
  virtual ILevelScriptBuilder& add_scripts(const LevelScript* scripts,
                                               int script_count) = 0;

  virtual ILevelScriptBuilder& add_builder(
      std::shared_ptr<ILevelScriptBuilder> other) = 0;
  
  virtual int size() const = 0;

  virtual void build_into(LevelScript* dst, int& dst_count) = 0;

  const LevelScript* get_entry_pointer(int& out_count = unused_int);

 protected:
  const LevelScript* build_impl(int& out_count);
};
