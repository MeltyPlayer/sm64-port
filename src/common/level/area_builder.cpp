#include "area_builder.hpp"

#include "level_commands.h"

AreaBuilder::AreaBuilder(u8 area_index, const GeoLayout* geo_layout) {
  (*area_wrapper).add_level_scripts({AREA(area_index, geo_layout)})
                 .insert_builder(internal_builder)
                 .add_level_script(END_AREA());
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
