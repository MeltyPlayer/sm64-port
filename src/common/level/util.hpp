#pragma once

#include "include/types.h"
#include "macro_level_script_builder.hpp"

extern void append_script(LevelScript* dst, int& dst_pos,
                          LevelScript src);
extern void append_scripts(LevelScript* dst, int& dst_pos,
                           const LevelScript* src,
                           const int src_count);
extern void append_scripts(LevelScript* dst, int& dst_pos,
                           std::initializer_list<const LevelScript> src);

extern void append_jump_to_address(LevelScript* dst, int& dst_pos,
                                   const LevelScript* address);
extern void append_jump_link_to_address(LevelScript* dst, int& dst_pos,
                                        const LevelScript* address);
extern void append_jump_if_equal_to_address(LevelScript* dst, int& dst_pos,
                                            u32 value,
                                            const LevelScript* address);

extern void append_execute(LevelScript* dst, int& dst_pos,
                           u8 segment,
                           const u8* segment_start,
                           const u8* segment_end,
                           const LevelScript* address);
extern void append_exit_and_execute(LevelScript* dst, int& dst_pos,
                                    u8 segment,
                                    const u8* segment_start,
                                    const u8* segment_end,
                                    const LevelScript* address);