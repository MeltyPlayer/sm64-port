#include "util.hpp"

#include "constants.hpp"
#include "include/types.h"
#include "level_commands.h"
#include "level_script_builder.hpp"
#include "util/unused.hpp"

void append_script(LevelScript* dst, int& dst_pos,
                   LevelScript src) {
  dst[dst_pos++] = src;
}

void append_scripts(LevelScript* dst, int& dst_pos,
                    const LevelScript* src,
                    const int src_count) {
  for (auto i = 0; i < src_count; ++i) {
    append_script(dst, dst_pos, src[i]);
  }
}


void append_jump_to_address(LevelScript* dst, int& dst_pos,
                            const LevelScript* address) {
  const LevelScript jump_scripts[] = {JUMP(address)};
  append_scripts(dst, dst_pos, jump_scripts, JUMP_COUNT);
}

void append_jump_link_to_address(LevelScript* dst, int& dst_pos,
                                 const LevelScript* address) {
  const LevelScript jump_link_scripts[] = {JUMP_LINK(address)};
  append_scripts(dst, dst_pos, jump_link_scripts, JUMP_LINK_COUNT);
}

extern void append_jump_if_equal_to_address(LevelScript* dst, int& dst_pos,
                                            u32 value,
                                            const LevelScript* address) {
  const LevelScript jump_if_equal_scripts[] = {JUMP_IF(OP_EQ, value, address)};
  append_scripts(dst, dst_pos, jump_if_equal_scripts, JUMP_IF_COUNT);
}


void append_execute(LevelScript* dst, int& dst_pos,
                    u8 segment,
                    const u8* segment_start,
                    const u8* segment_end,
                    const LevelScript* address) {
  const LevelScript execute_scripts[]{
      EXECUTE(segment, segment_start, segment_end, address),
  };
  append_scripts(dst, dst_pos, execute_scripts,
                 EXECUTE_COUNT);
}

void append_exit_and_execute(LevelScript* dst, int& dst_pos,
                             u8 segment,
                             const u8* segment_start,
                             const u8* segment_end,
                             const LevelScript* address) {
  const LevelScript exit_and_execute_scripts[]{
      EXIT_AND_EXECUTE(segment, 
                       segment_start,
                       segment_end,
                       address),
  };
  append_scripts(dst, dst_pos, exit_and_execute_scripts,
                 EXIT_AND_EXECUTE_COUNT);
}