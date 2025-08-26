#include "Headers/ghostmanager.h"
#include "Headers/ghost.h"

int check_current_dots(std::array<std::array<cell_data, CELL_NUM_HEIGHT>, CELL_NUM_WIDTH> game_map)
{
    int current_dots = 0;
    
    for(int x = 0; x < CELL_NUM_WIDTH; x++)
    {
        for(int y = 0; y < CELL_NUM_HEIGHT; y++)
        {
            if(game_map[x][y].type == PELLET || game_map[x][y].type == ENERGIZER)
            {
                current_dots++;
            }
        }
    }

    return current_dots;
}

void manage_ghost(pm_data &i_pacman, ghosts &i_ghost, game_data &game, std::array<std::array<cell_data, CELL_NUM_HEIGHT>, CELL_NUM_WIDTH> game_map)
{
    
    int time_passed = timer_ticks(GAME_TIMER);

    if(i_pacman.mode == NORMAL)
    {
        if(time_passed < WAVE_1_END)
        {
            for(int i = 0; i < 4; i++)
            {
                i_ghost.ghost[i].mode = SCATTER;
            }

            if(time_passed > LONG_SCATTER_TIME)
            {
                for(int i = 0; i < 4; i++)
                {
                    i_ghost.ghost[i].mode = CHASE;
                }
            }
        }
        else if(time_passed > WAVE_1_END && time_passed < WAVE_2_END)
        {
            for(int i = 0; i < 4; i++)
            {
                i_ghost.ghost[i].mode = SCATTER;
            }

            if(time_passed > WAVE_1_END + LONG_SCATTER_TIME)
            {
                for(int i = 0; i < 4; i++)
                {
                    i_ghost.ghost[i].mode = CHASE;
                }
            }
        }
        else if(time_passed > WAVE_2_END && time_passed < WAVE_3_END)
        {
            
            for(int i = 0; i < 4; i++)
            {
                i_ghost.ghost[i].mode = SCATTER;
            }

            if(time_passed > WAVE_2_END + SHORT_SCATTER_TIME)
            {
                for(int i = 0; i < 4; i++)
                {
                    i_ghost.ghost[i].mode = CHASE;
                }
            }
        }
        else if(time_passed > WAVE_3_END && time_passed < WAVE_4_END)
        {
            for(int i = 0; i < 4; i++)
            {
                i_ghost.ghost[i].mode = SCATTER;
            }
        }
        else
        {
            for(int i = 0; i < 4; i++)
            {
                i_ghost.ghost[i].mode = CHASE;
                //chase mode permanently
            }
        }
    }

    if(time_passed > 5000 && time_passed < 5500)
    {
        i_ghost.ghost[3].use_door = 1;//let pink out
    }
    else if(time_passed > 20000 && time_passed < 20500)
    {
        i_ghost.ghost[1].use_door = 1;//let blue out
    }
    else if(time_passed > 25000 && time_passed < 25500)
    {
        i_ghost.ghost[2].use_door = 1;//let orange out
    }

    for (int i = 0; i < 4; i++)
    {
        update_ghost(i_ghost.ghost[i], i_pacman ,  i_ghost.ghost[0], game, game_map);
    }

}