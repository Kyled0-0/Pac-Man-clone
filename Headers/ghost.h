#pragma once

#include "splashkit.h"
#include <array>
#include <string>
#include <math.h>

#include "global.h"

bool caught(pm_data pacman, ghost_data i_ghost);

void reset_ghost_pos(ghost_data &i_ghost);

float get_optimal_path(ghost_data i_ghost, int i_direction);

void update_target(ghost_data &i_ghost, pm_data i_pacman, ghost_data &i_ghost_red);

void update_ghost(ghost_data &i_ghost, pm_data &i_pacman , ghost_data i_ghost_red, game_data &game, std::array<std::array<cell_data, CELL_NUM_HEIGHT>, CELL_NUM_WIDTH> &map);