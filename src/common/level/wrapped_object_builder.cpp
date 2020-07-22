#include "wrapped_object_builder.hpp"

#include "behavior_macros.h"
#include "util.hpp"

WrappedObjectBuilder::WrappedObjectBuilder(u16 model_index,
                                           ObjectList object_type)
  : model_index(model_index) { add_script(BEGIN(object_type)); }

u16 WrappedObjectBuilder::get_model_index() { return model_index; }

const BehaviorScript* WrappedObjectBuilder::get_entry_pointer(
    int& out_count) const {
  out_count = size();
  if (cached_behavior_scripts != nullptr) { return cached_behavior_scripts; }
  return build_impl(unused_int);
}

WrappedObjectBuilder& WrappedObjectBuilder::
add_part(std::shared_ptr<IScriptPart<BehaviorScript>> part) {
  parts_.push_back(std::move(part));
  return *this;
}

WrappedObjectBuilder& WrappedObjectBuilder::add_script(
    BehaviorScript script) {
  return add_part(
      std::make_shared<SingleScriptPart<BehaviorScript>>(script));
}

WrappedObjectBuilder& WrappedObjectBuilder::add_scripts(
    std::initializer_list<const BehaviorScript> scripts) {
  return add_part(
      std::make_shared<MultipleScriptPart<BehaviorScript>>(scripts));
}

WrappedObjectBuilder& WrappedObjectBuilder::add_scripts(
    const BehaviorScript* scripts, int script_count) {
  return add_part(
      std::make_shared<MultipleScriptPart<BehaviorScript>>(scripts,
                                                           script_count));
}

WrappedObjectBuilder& WrappedObjectBuilder::add_builder(
    std::shared_ptr<IScriptBuilder<BehaviorScript>> builder) {
  return add_part(std::make_shared<BuilderScriptPart<BehaviorScript>>(builder));
}


int WrappedObjectBuilder::size() const {
  auto total_size = 0;
  for (const auto& part : parts_) { total_size += part->size(); }
  return total_size;
}

void WrappedObjectBuilder::
build_into(LevelScript* dst, int& dst_pos) const {
  for (const auto& part : parts_) { part->build_into(dst, dst_pos); }
}

ValidationNode& WrappedObjectBuilder::get_cache_validation_node() {
  return cache_validation_impl_;
}
