#pragma once

#include "splashkit.h"
#include <array>
#include <string>
#include <math.h>

#include "global.h"

int check_current_dots(std::array<std::array<cell_data, CELL_NUM_HEIGHT>, CELL_NUM_WIDTH> game_map);

void manage_ghost(pm_data &i_pacman, ghosts &i_ghost, game_data &game, std::array<std::array<cell_data, CELL_NUM_HEIGHT>, CELL_NUM_WIDTH> game_map);