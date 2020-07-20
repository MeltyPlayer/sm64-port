#pragma once
#include <initializer_list>
#include <memory>
#include <vector>


#include "game/object_list_processor.h"
#include "i_object_builder.hpp"

enum class MacroBehaviorScriptPartType {
  SCRIPT,
  SCRIPTS
};

struct MacroBehaviorScriptPart {
  MacroBehaviorScriptPartType type;
  BehaviorScript script;
  std::vector<BehaviorScript> scripts;
};

// TODO: Memory leaking will be a problem with the behavior scripts.
// TODO: Will not update if changes are made after first build.
// TODO: Each version of the build should maintain the same pointer location.
class WrappedObjectBuilder : public IObjectBuilder {
public:
  WrappedObjectBuilder(u16 model_index, ObjectList object_type);
  WrappedObjectBuilder(const WrappedObjectBuilder& other) = delete;

  u16 get_model_index() override;
  const BehaviorScript* get_behavior_scripts() override;

  WrappedObjectBuilder& add_script(BehaviorScript in_script);
  WrappedObjectBuilder& add_scripts(
      std::initializer_list<const BehaviorScript> in_scripts);

 private:
  const BehaviorScript* build();

  const u16 model_index;
  std::vector<std::unique_ptr<MacroBehaviorScriptPart>> parts;

  const BehaviorScript* cached_behavior_scripts = nullptr;
};
