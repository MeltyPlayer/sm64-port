#include "util.hpp"

#include "constants.hpp"
#include "include/types.h"
#include "level_commands.h"
#include "macro_level_script_builder.hpp"
#include "util/unused.hpp"

void append_script(LevelScript* dst, int& dst_pos, LevelScript src) {
  dst[dst_pos++] = src;
}

void append_scripts(LevelScript* dst, int& dst_pos, const LevelScript* src,
                    const int src_count) {
  for (auto i = 0; i < src_count; ++i) {
    append_script(dst, dst_pos, src[i]);
  }
}

void append_scripts(LevelScript* dst, int& dst_pos,
                    std::initializer_list<const LevelScript> src) {
  append_scripts(dst, dst_pos, src.begin(), src.size());
}

void append_jump_to_address(LevelScript* dst, int& dst_pos,
                            const LevelScript* address) {
  append_scripts(dst, dst_pos, {JUMP(address)});
}

void append_jump_link_to_address(LevelScript* dst, int& dst_pos,
                                 const LevelScript* address) {
  append_scripts(dst, dst_pos, {JUMP_LINK(address)});
}

extern void append_jump_if_equal_to_address(LevelScript* dst, int& dst_pos,
                                            u32 value,
                                            const LevelScript* address) {
  append_scripts(dst, dst_pos, {JUMP_IF(OP_EQ, value, address)});
}

void append_execute(LevelScript* dst, int& dst_pos, u8 segment,
                    const u8* segment_start, const u8* segment_end,
                    const LevelScript* address) {
  append_scripts(dst, dst_pos, {
                     EXECUTE(segment, segment_start, segment_end, address),
                 });
}

void append_exit_and_execute(LevelScript* dst, int& dst_pos, u8 segment,
                             const u8* segment_start, const u8* segment_end,
                             const LevelScript* address) {
  append_scripts(dst, dst_pos, {
                     EXIT_AND_EXECUTE(segment, segment_start, segment_end,
                                      address),
                 });
}
