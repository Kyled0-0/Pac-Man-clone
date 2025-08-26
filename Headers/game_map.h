#pragma once

#include "splashkit.h"
#include <array>
#include <string>
#include <math.h>

#include "global.h"

std::array<std::array<cell_data, CELL_NUM_HEIGHT>, CELL_NUM_WIDTH> convert_sketch(std::array<std::string,CELL_NUM_HEIGHT> map_sketch, ghosts &i_ghost, pm_data &i_pacman);

bool map_collision (bool i_pellet, bool i_energizer, bool use_door, int i_x, int i_y, std::array<std::array<cell_data, CELL_NUM_HEIGHT>, CELL_NUM_WIDTH> &map);

bool check_win(std::array<std::array<cell_data, CELL_NUM_HEIGHT>, CELL_NUM_WIDTH> game_map);

void update_game_data(game_data &game, pm_data i_pacman, std::array<std::array<cell_data, CELL_NUM_HEIGHT>, CELL_NUM_WIDTH> game_map);

void reset_entities(pm_data &i_pacman, ghosts &i_ghost);

void draw_game(std::array<std::array<cell_data, CELL_NUM_HEIGHT>, CELL_NUM_WIDTH> game_map, pm_data &i_pacman, ghosts &i_ghost, game_data &game);