#include "butterfly.hpp"

#include "game/behavior_actions.h"

void Butterfly::init() { bhv_butterfly_init(); }
void Butterfly::tick() { bhv_butterfly_loop(); }