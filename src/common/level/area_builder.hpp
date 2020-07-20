#pragma once
#include <functional>

#include "i_level_script_builder.hpp"
#include "macro_level_script_builder.hpp"
#include "i_object_builder.hpp"
#include "object_builder_params.hpp"

class AreaBuilder : public ILevelScriptBuilder {
public:
  AreaBuilder(u8 area_index, const GeoLayout* geo_layout);
  AreaBuilder(const AreaBuilder& other) = delete;

  AreaBuilder& add_object(std::shared_ptr<IObjectBuilder> object_builder,
                          const std::function<void(ObjectBuilderParams&)> &params_callback);

  void append_builder(int& out_count,
                      LevelScript* outer_scripts,
                      LevelScript* inner_scripts) override;

  int get_script_count() const override;

  MacroLevelScriptBuilder& get_internal_builder();

private:
  const std::unique_ptr<MacroLevelScriptBuilder> area_wrapper;
  const std::shared_ptr<MacroLevelScriptBuilder> internal_builder;
};
