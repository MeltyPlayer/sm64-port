#pragma once

#include <memory>

#include "include/types.h"
#include "util/unused.hpp"

class ILevelScriptBuilder {
public:
  ILevelScriptBuilder() {}
  ILevelScriptBuilder(const ILevelScriptBuilder& other) = delete;

  const LevelScript* get_entry_pointer(int& out_count = unused_int);
  virtual void build_into(LevelScript* dst, int& dst_count) = 0;

  virtual int size() const = 0;

protected:
  const LevelScript* build_impl(int& out_count);
};
