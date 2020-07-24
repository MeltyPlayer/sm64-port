#pragma once

#include <memory>
#include "common/scripts/dynamic_level_script_builder.hpp"
#include "types.h"
#include "game/moving_texture.h"

// geo
extern const GeoLayout castle_courtyard_geo_000200[];
extern const GeoLayout castle_courtyard_geo_000218[];

// leveldata
extern const Gfx castle_courtyard_seg7_dl_070048B8[];
extern const Gfx castle_courtyard_seg7_dl_07005078[];
extern const Gfx castle_courtyard_seg7_dl_07005698[];
extern const Gfx castle_courtyard_seg7_dl_07005938[];
extern const Collision castle_courtyard_seg7_collision[];
extern const MacroObject castle_courtyard_seg7_macro_objs[];
extern const struct MovtexQuadCollection castle_courtyard_movtex_star_statue_water[];

// script
extern std::shared_ptr<DynamicLevelScriptBuilder> get_level_castle_courtyard_entry();