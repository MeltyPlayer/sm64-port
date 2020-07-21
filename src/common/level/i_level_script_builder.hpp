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
  virtual void build_into(int& out_count, LevelScript* outer_scripts,
                              LevelScript* inner_scripts) = 0;

  virtual int size() const = 0;

protected:
  const LevelScript* build_impl(int& out_count,
                                LevelScript* outer_scripts);
};
