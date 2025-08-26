#include "Headers/pacman.h"
#include "Headers/game_map.h"

void time_out(pm_data &pacman, ghosts &i_ghost)
{
	int time_passed = timer_ticks(ENERGIZER_TIMER);

	if(time_passed >= ENERGIZER_TIME) //enregizer time out
	{
		pacman.mode = NORMAL;

		for(int i = 0; i < 4; i++)
		{
			int home_x = i_ghost.ghost[i].home[0] * CELL_SIZE;
			int home_y = i_ghost.ghost[i].home[1] * CELL_SIZE;
			if(i_ghost.ghost[i].use_door == 0 && i_ghost.ghost[i].x == home_x && i_ghost.ghost[i].y == home_y)
			{
				i_ghost.ghost[i].use_door = 1;
			}
		}

		stop_timer(ENERGIZER_TIMER);
		reset_timer(ENERGIZER_TIMER);
		resume_timer(GAME_TIMER);
	}


}

void update_pacman(pm_data &pacman, ghosts &i_ghost, std::array<std::array<cell_data, CELL_NUM_HEIGHT>, CELL_NUM_WIDTH> &map, game_data &game)
{
	bool walls[4]; //0:right 1:up 2:left 3:down
	walls[0] = map_collision(0, 0, 0, pacman.x + PM_SPEED, pacman.y, map);
	walls[1] = map_collision(0, 0, 0, pacman.x, pacman.y - PM_SPEED, map);
	walls[2] = map_collision(0, 0, 0, pacman.x - PM_SPEED, pacman.y, map);
	walls[3] = map_collision(0, 0, 0, pacman.x , pacman.y + PM_SPEED, map);

	if(key_down(RIGHT_KEY) || key_down(D_KEY))
	{
		if(walls[0] == 0) //check if there is a wall in the way, if not;
			pacman.direction = 1;
		//if yes then pacman keeps moving the same direction, doesn't stop until hit wall
	}

	if(key_down(UP_KEY) || key_down(W_KEY))
	{
		if(walls[1] == 0)
			pacman.direction = 2;
	}
	
	if(key_down(LEFT_KEY) || key_down(A_KEY))
	{	
		if(walls[2] == 0)
			pacman.direction = 3;
	}
	
	if(key_down(DOWN_KEY) || key_down(S_KEY))
	{
		if(walls[3] == 0)
			pacman.direction = 4;
	}


	if(walls[pacman.direction - 1] == 0)
	{
		switch (pacman.direction)
		{
			case 1:
			{
				pacman.x += PM_SPEED;
				break;
			}
			case 2:
			{
				pacman.y -= PM_SPEED;
				break;
			}
			case 3:
			{
				pacman.x -= PM_SPEED;
				break;
			}
			case 4:
			{
				pacman.y += PM_SPEED;
				break;
			}
			default:
                break;
		}
	}

	if(pacman.x <= -CELL_SIZE)//tunnelinggg
	{
		pacman.x = CELL_NUM_WIDTH * CELL_SIZE;
	}
	else if(pacman.x >= CELL_NUM_WIDTH * CELL_SIZE)
	{
		pacman.x = -CELL_SIZE;
	}
	

	//check if pacman is energized and to only eat pellets in front of him
	if(map_collision(1,0,0,pacman.x,pacman.y,map) == 1)
	{
		play_sound_effect(WAKA);
		game.points += PELLET_PTS;
	}
    else if(map_collision(0,1,0,pacman.x,pacman.y,map) == 1)
    {
        pacman.mode = ENERGIZED;
        game.points += ENERGIZER_PTS;

		start_timer(ENERGIZER_TIMER);
		pause_timer(GAME_TIMER);
    }

	time_out(pacman, i_ghost);
}