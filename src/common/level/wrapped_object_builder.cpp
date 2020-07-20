#include "wrapped_object_builder.hpp"

#include "behavior_macros.h"
#include "util.hpp"

WrappedObjectBuilder::WrappedObjectBuilder(u16 model_index,
                                           ObjectList object_type)
  : model_index(model_index) { add_script(BEGIN(object_type)); }

u16 WrappedObjectBuilder::get_model_index() { return model_index; }

const BehaviorScript* WrappedObjectBuilder::get_behavior_scripts() {
  if (cached_behavior_scripts != nullptr) { return cached_behavior_scripts; }
  return build();
}

WrappedObjectBuilder& WrappedObjectBuilder::add_script(
    BehaviorScript in_script) {
  auto part = new MacroBehaviorScriptPart();
  part->type = MacroBehaviorScriptPartType::SCRIPT;
  part->script = in_script;

  parts.push_back(std::unique_ptr<MacroBehaviorScriptPart>(part));

  return *this;
}

WrappedObjectBuilder& WrappedObjectBuilder::add_scripts(
    std::initializer_list<const BehaviorScript> in_scripts) {
  auto part = new MacroBehaviorScriptPart();
  part->type = MacroBehaviorScriptPartType::SCRIPTS;
  std::copy(
      in_scripts.begin(),
      in_scripts.end(),
      std::back_inserter(part->scripts));

  parts.push_back(std::unique_ptr<MacroBehaviorScriptPart>(part));

  return *this;
}


const BehaviorScript* WrappedObjectBuilder::build() {
  auto script_count = 0;
  for (auto i = 0; i < parts.size(); ++i) {
    const auto& part = parts[i];

    switch (part->type) {
      case MacroBehaviorScriptPartType::SCRIPT:
        ++script_count;
        break;

      case MacroBehaviorScriptPartType::SCRIPTS:
        script_count += part->scripts.size();
        break;
    }
  }

  auto pos = 0;
  auto out_scripts = new BehaviorScript[script_count];

  for (const auto& part : parts) {
    switch (part->type) {
      case MacroBehaviorScriptPartType::SCRIPT:
        append_script(out_scripts, pos, part->script);
        break;

      case MacroBehaviorScriptPartType::SCRIPTS:
        append_scripts(out_scripts,
                       pos,
                       &part->scripts[0],
                       part->scripts.size());
        break;
    }
  }

  return cached_behavior_scripts = out_scripts;
}
