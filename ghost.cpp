#include "Headers/ghost.h"
#include "Headers/game_map.h"

bool caught(pm_data pacman, ghost_data i_ghost)
{

	if(circles_intersect(i_ghost.x, i_ghost.y, ENTITIES_RADIUS, pacman.x, pacman.y, ENTITIES_RADIUS))
	{
        return 1;
	}
    return 0;

}

void reset_ghost_pos(ghost_data &i_ghost)
{
    i_ghost.x = i_ghost.home[0] * CELL_SIZE;
    i_ghost.y = i_ghost.home[1] * CELL_SIZE;
}

float get_optimal_path(ghost_data i_ghost, int i_direction)
{
    int x = i_ghost.x;
    int y = i_ghost.y;

    switch (i_direction)
    {
    case 0: // right
        x += GHOST_SPEED;
        break;
    case 1: // up
        y -= GHOST_SPEED;
        break;
    case 2: // left
        x -= GHOST_SPEED;
        break;
    case 3: // down
        y += GHOST_SPEED;
        break;
    default:
        break;
    }

    return float(sqrt(pow(x - i_ghost.target[0], 2) + pow(y - i_ghost.target[1], 2)));
}

void update_target(ghost_data &i_ghost, pm_data i_pacman, ghost_data &i_ghost_red)
{
    if(i_ghost.use_door == 1)//have not left the house
    {
        i_ghost.target[0] = i_ghost.exit[0];
        i_ghost.target[1] = i_ghost.exit[1];

        int target_x = i_ghost.target[0] * CELL_SIZE;
        int target_y = i_ghost.target[1] * CELL_SIZE;

        if(i_ghost.x == target_x && i_ghost.y == target_y)
        {
            i_ghost.use_door = 0;
        }   
    }
    else 
    {
        if(i_ghost.mode == SCATTER)
        {
            switch(i_ghost.id)
            {
                case 1://red
                {
                    i_ghost.target[0] = CELL_SIZE * (CELL_NUM_WIDTH - 1);
                    i_ghost.target[1] = 0;
                    break;
                }
                case 2://blue
                {
                    i_ghost.target[0] = CELL_SIZE * (CELL_NUM_WIDTH - 1);
                    i_ghost.target[1] = CELL_SIZE * (CELL_NUM_HEIGHT - 1);
                    break;
                }
                case 3://orange
                {
                    i_ghost.target[0] = 0;
                    i_ghost.target[1] = CELL_SIZE * (CELL_NUM_HEIGHT - 1);
                    break;
                }
                case 4://pink
                {
                    i_ghost.target[0] = 0;
                    i_ghost.target[1] = 0;
                    break;
                }
                default:
                    break;
            }
        }
        else if(i_ghost.mode == CHASE)
        {
            switch (i_ghost.id)
            {
                case 1://red
                {
                    i_ghost.target[0] = i_pacman.x;
                    i_ghost.target[1] = i_pacman.y;
                    break;
                }
                case 2://blue
                {
                    i_ghost.target[0] = i_pacman.x;
                    i_ghost.target[1] = i_pacman.y;

                    switch (i_pacman.direction)
                    {
                        case 1://right
                        {
                            i_ghost.target[0] += GHOST_B_CHASE * CELL_SIZE;
                            break;
                        }
                        case 2://up
                        {
                            i_ghost.target[1] -= GHOST_B_CHASE * CELL_SIZE;
                            break;
                        }
                        case 3://left
                        {
                            i_ghost.target[0] -= GHOST_B_CHASE * CELL_SIZE;
                            break;
                        }
                        case 4://down
                        {
                            i_ghost.target[1] += GHOST_B_CHASE * CELL_SIZE;
                            break;
                        }
                        default:
                            break;
                    }

                    i_ghost.target[0] += i_ghost.target[0] - i_ghost_red.x;
                    i_ghost.target[1] += i_ghost.target[1] - i_ghost_red.y;
                    break;
                }
                case 3://orange
                {   
                    if(CELL_SIZE * GHOST_O_CHASE <= sqrt(pow(i_ghost.x - i_pacman.x, 2) + pow(i_ghost.y - i_pacman.y, 2)))
                    {
                        i_ghost.target[0] = i_pacman.x;
                        i_ghost.target[1] = i_pacman.y;
                    }
                    else
                    {
                        i_ghost.target[0] = 0;
                        i_ghost.target[1] = CELL_SIZE * (CELL_NUM_HEIGHT - 1);
                    }
                    break;
                }
                case 4://pink
                {
                    i_ghost.target[0] = i_pacman.x;
                    i_ghost.target[1] = i_pacman.y;

                    switch (i_pacman.direction)
                    {
                        case 1://right
                            i_ghost.target[0] += GHOST_P_CHASE * CELL_SIZE;
                            break;
                        case 2://up
                            i_ghost.target[1] -= GHOST_P_CHASE * CELL_SIZE;
                            break;
                        case 3://left
                            i_ghost.target[0] -= GHOST_P_CHASE * CELL_SIZE;
                            break;
                        case 4://down
                            i_ghost.target[1] += GHOST_P_CHASE * CELL_SIZE;
                            break;
                    }
                    break;
                }
                default:
                    break;
            }
        }
    }
}

void update_ghost(ghost_data &i_ghost, pm_data &i_pacman , ghost_data i_ghost_red, game_data &game, std::array<std::array<cell_data, CELL_NUM_HEIGHT>, CELL_NUM_WIDTH> &map)
{   
    bool move = 0;
    
    int available_ways = 0;
    int speed = GHOST_SPEED;

    if(i_pacman.mode == ENERGIZED && i_ghost.mode != EATEN)
    {
        i_ghost.mode = SCARED;
        speed = GHOST_SCARED_SPEED;
    }


    update_target(i_ghost, i_pacman, i_ghost_red);

    bool walls[4];//0:right 1:up 2:left 3:down
    walls[0] = map_collision(0, 0, i_ghost.use_door, i_ghost.x + speed, i_ghost.y, map);
    walls[1] = map_collision(0, 0, i_ghost.use_door, i_ghost.x, i_ghost.y - speed, map);
    walls[2] = map_collision(0, 0, i_ghost.use_door, i_ghost.x - speed, i_ghost.y, map);
    walls[3] = map_collision(0, 0, i_ghost.use_door, i_ghost.x, i_ghost.y + speed, map);
    
    if(i_ghost.mode != SCARED && i_ghost.mode != EATEN)
    {   
        int optimal_direction = 4;

        move = 1;
        
        for(int a = 0; a < 4; a++)//get optimal direction
        {
            if(a == (i_ghost.direction + 2) % 4)
            {
                continue; //ghost cant turn back
            }
            else if(walls[a] == 0)
            {
                available_ways++;
                if(optimal_direction == 4)
                {
                    optimal_direction = a;
                }
                
                if(get_optimal_path(i_ghost, a) < get_optimal_path(i_ghost, optimal_direction))
                {
                    optimal_direction = a;
                }
            }
        }
        if(available_ways > 1)//atleast 2 paths
        {
            i_ghost.direction = optimal_direction;
        }
        else
        {
            if (optimal_direction == 4)//ghost can turn back if stuck
            {
                i_ghost.direction = (i_ghost.direction + 2) % 4;
            } 
            else//take the only path remaining
            {
                i_ghost.direction = optimal_direction;
            }
        }
    }
    else if(i_ghost.mode == SCARED)//scared mode
    {
        int random_direction = rand() % 4;

        move = 1;

        for(int a = 0; a < 4; a++)
        {
            if(a == (i_ghost.direction + 2) % 4)
            {
                continue; //ghost cant turn back
            }
            else if(walls[a] == 0)
            {
                available_ways++;
            }
        }

        if(available_ways > 0)//atleast 1 path
        {
            while(walls[random_direction] == 1 || random_direction == (i_ghost.direction + 2) % 4)
            {
                random_direction = rand() % 4;
            }
            i_ghost.direction = random_direction;
        }
        else
        {   
            //turn back if there's no other way
            i_ghost.direction = (i_ghost.direction + 2) % 4;
        }
    }

    
    if(move == 1)
    {
        switch (i_ghost.direction)
        {
            case 0://right
            {
                i_ghost.x += speed;
                break;
            }
            case 1://up
            {
                i_ghost.y -= speed;
                break;
            }
            case 2://left
            {
                i_ghost.x -= speed;
                break;
            }
            case 3://down
            {
                i_ghost.y += speed;
                break;
            }
            default:
                break;
        }

        if (i_ghost.x <= -CELL_SIZE) // ghost using tunnel
        {
            i_ghost.x = CELL_NUM_WIDTH * CELL_SIZE - CELL_SIZE;
        }
        else if (i_ghost.x >= CELL_NUM_WIDTH * CELL_SIZE)
        {
            i_ghost.x = -CELL_SIZE;
        }
    }

    if(caught(i_pacman, i_ghost) == 1)
    {
        if(i_ghost.mode != SCARED)//pacman dies
        {
            i_pacman.mode = DEAD;
            game.pm_lives --;
        }
        else//home we go
        {   
            play_sound_effect(EAT_GHOST);
            draw_text("200", COLOR_WHITE, FONT_NAME, FONT_SIZE - 5, i_ghost.x, i_ghost.y - CELL_SIZE/3);
            refresh_screen();
            delay(700);
            i_ghost.mode = EATEN;
            reset_ghost_pos(i_ghost);
            game.points += GHOST_PTS;
        }
    }
    
}