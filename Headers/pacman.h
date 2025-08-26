#pragma once

#include "splashkit.h"
#include <array>
#include <string>
#include <math.h>

#include "global.h"

void time_out(pm_data &pacman, ghosts &i_ghost);

void update_pacman(pm_data &pacman, ghosts &i_ghost, std::array<std::array<cell_data, CELL_NUM_HEIGHT>, CELL_NUM_WIDTH> &map, game_data &game);