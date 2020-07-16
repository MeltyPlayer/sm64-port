#include "entry.h"

#include "level_commands.h"
#include "segment_symbols.h"
#include "sm64.h"
#include <ultra64.h>

#include "levels/intro/header.h"

#include "common/level/level_script_builder.hpp"
#include "make_const_nonconst.h"
#include "util/unused.hpp"

const LevelScript* get_level_script_entry(int& out_count = unused_int) {
  const LevelScript level_script_entry[] = {
      INIT_LEVEL(),
      SLEEP(/*frames*/ 2),
      BLACKOUT(/*active*/ FALSE),
      SET_REG(/*value*/ 0),
      EXECUTE(
          /*seg*/ 0x14,                  /*script*/
                  _introSegmentRomStart, /*scriptEnd*/
                  _introSegmentRomEnd,
                  /*entry*/ level_intro_entry_1),
  };

  return LevelScriptBuilder().add_level_scripts(level_script_entry, 8)
                             .add_jump_to_inner_start()
                             .build(out_count);
}
