#pragma once

#include <memory>
#include "include/types.h"
#include "util/unused.hpp"

class ILevelScriptBuilder {
public:
  ILevelScriptBuilder() {}
  ILevelScriptBuilder(const ILevelScriptBuilder& other) = delete;

  const LevelScript* build(int& out_count = unused_int,
                           LevelScript* outer_scripts = nullptr);
  virtual void append_builder(int& out_count, LevelScript* outer_scripts,
                              LevelScript* inner_scripts) = 0;

  virtual int get_script_count() const = 0;

 protected:
  virtual const LevelScript* build_impl(int& out_count,
                                   LevelScript* outer_scripts) = 0;
};
