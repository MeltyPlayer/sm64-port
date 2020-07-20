#include "area_builder.hpp"

#include "level_commands.h"

AreaBuilder::AreaBuilder(u8 area_index, const GeoLayout* geo_layout)
  : area_wrapper(std::make_unique<MacroLevelScriptBuilder>()),
    internal_builder(std::make_shared<MacroLevelScriptBuilder>()) {
  (*area_wrapper).add_level_scripts({AREA(area_index, geo_layout)})
                 .insert_builder(internal_builder)
                 .add_level_script(END_AREA());
}

AreaBuilder& AreaBuilder::add_object(
    std::shared_ptr<IObjectBuilder> object_builder,
    const std::function<void(ObjectBuilderParams&)>& params_callback) {
  auto params = ObjectBuilderParams();
  params_callback(params);

  const auto model_index = object_builder->get_model_index();
  const auto x = params.x;
  const auto y = params.y;
  const auto z = params.z;
  const auto x_angle = params.x_angle;
  const auto y_angle = params.y_angle;
  const auto z_angle = params.z_angle;
  const auto bhv_param = params.beh_param;
  const auto bhv_scripts = object_builder->get_behavior_scripts();

  internal_builder->add_level_scripts({
      OBJECT(model_index,
             x, y, z,
             x_angle, y_angle, z_angle,
             bhv_param,
             bhv_scripts)
  });

  return *this;
}

void AreaBuilder::append_builder(int& out_count, LevelScript* outer_scripts,
                                 LevelScript* inner_scripts) {
  area_wrapper->append_builder(out_count, outer_scripts, inner_scripts);
}

int AreaBuilder::get_script_count() const {
  return area_wrapper->get_script_count();
}

MacroLevelScriptBuilder& AreaBuilder::get_internal_builder() {
  return *internal_builder;
}