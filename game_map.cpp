#include "Headers/game_map.h"

std::array<std::array<cell_data, CELL_NUM_HEIGHT>, CELL_NUM_WIDTH> convert_sketch(std::array<std::string,CELL_NUM_HEIGHT> map_sketch, ghosts &i_ghost, pm_data &i_pacman)
{	
	std::array<std::array<cell_data, CELL_NUM_HEIGHT>, CELL_NUM_WIDTH> game_map{};
    for (unsigned int y = 0; y < CELL_NUM_HEIGHT; y++)
    {
        for (unsigned int x = 0; x < CELL_NUM_WIDTH; x++)
        {

			switch (map_sketch[y][x])
			{
			case '#':
			{
				game_map[x][y].type = WALL;
				game_map[x][y].cell_color = COLOR_BLUE;
				break;
			}
			case '=':
			{
				game_map[x][y].type = DOOR;
				game_map[x][y].cell_color = COLOR_LIGHT_PINK;
				break;
			}
			case '.':
			{
				if(i_pacman.mode != DEAD)
				{
					game_map[x][y].type = PELLET;
					game_map[x][y].cell_color = COLOR_LIGHT_PINK;
					break;
				}
				else
					break;
			}

			case '1': //redghost
			{
				i_ghost.ghost[0].x = CELL_SIZE * x; //get the center of the cell
				i_ghost.ghost[0].y = CELL_SIZE * y;
				i_ghost.ghost[0].color = COLOR_RED;
				i_ghost.ghost[0].id = 1;

				for(int i = 0; i < 4; i++)
				{
					i_ghost.ghost[i].exit[0] = x;//set the position of red as the exit for all ghost
					i_ghost.ghost[i].exit[1] = y;
				}
				break;
			}
			case '2': //blue
			{
				i_ghost.ghost[1].x = CELL_SIZE * x;
				i_ghost.ghost[1].y = CELL_SIZE * y;
				i_ghost.ghost[1].color = COLOR_BLUE;
				i_ghost.ghost[1].home[0] = x;
				i_ghost.ghost[1].home[1] = y;
				i_ghost.ghost[1].id = 2;
				break;
			}
			case '3': //orange
			{
				i_ghost.ghost[2].x = CELL_SIZE * x;
				i_ghost.ghost[2].y = CELL_SIZE * y;
				i_ghost.ghost[2].color = COLOR_ORANGE;
				i_ghost.ghost[2].home[0] = x;
				i_ghost.ghost[2].home[1] = y;
				i_ghost.ghost[2].id = 3;

				//update red ghost home if he dies
				i_ghost.ghost[0].home[0] = x;
				i_ghost.ghost[0].home[1] = y;
				break;
			}
			case '4': //pink
			{
				i_ghost.ghost[3].x = CELL_SIZE * x;
				i_ghost.ghost[3].y = CELL_SIZE * y;
				i_ghost.ghost[3].color = COLOR_PINK;
				i_ghost.ghost[3].home[0] = x;
				i_ghost.ghost[3].home[1] = y;
				i_ghost.ghost[3].id = 4;
				break;
			}
			case 'P': //pacman
			{   
				i_pacman.x = CELL_SIZE * x;
				i_pacman.y = CELL_SIZE * y;

				i_pacman.initial_x_y[0] = i_pacman.x;
				i_pacman.initial_x_y[1] = i_pacman.y;
				i_pacman.color = COLOR_YELLOW;
				break;
			}
            case 'o':
			{
				if(i_pacman.mode != DEAD)
				{
					game_map[x][y].type = ENERGIZER;
					game_map[x][y].cell_color = COLOR_LIGHT_PINK;
					break;
				}
				else
					break;
				
			}
			default:
				break;
			}
        }
    }

	return game_map;
}

bool map_collision (bool i_pellet, bool i_energizer, bool use_door, int i_x, int i_y, std::array<std::array<cell_data, CELL_NUM_HEIGHT>, CELL_NUM_WIDTH> &map)
{
	bool collied = 0;

	//get the exact position
	float cell_x = i_x / (float)CELL_SIZE;
	float cell_y = i_y / (float)CELL_SIZE;

	//at once pm/ghost can only intercept with 4 cells
	for(int i = 0; i < 4; i++)
	{
		int x;
		int y;
		switch (i)
		{
			case 0: //top left cell
			{
				x = floor(cell_x);
				y = floor(cell_y);
				break;
			}
			case 1: //top right cell
			{
				x = ceil(cell_x);
				y = floor(cell_y);
				break;
			}
			case 2: //bottom left cell
			{
				x = floor(cell_x);
				y = ceil(cell_y);
				break;
			}
			case 3: //bottome right cell
			{
				x = ceil(cell_x);
				y = ceil(cell_y);
				break;
			}
		}

		if(x >= 0 && y >= 0 && x < CELL_NUM_WIDTH && y < CELL_NUM_HEIGHT)
		{
			if(i_pellet == 0 && i_energizer == 0)
			{
				if(map[x][y].type == DOOR && use_door == 0)
				{
					collied = 1;
				}
				else if(map[x][y].type == WALL)
				{
					collied = 1;
				}
			}
			else if(i_pellet == 1 && map[x][y].type == PELLET)
			{
				collied = 1;
				map[x][y].type = NOTHING;
				map[x][y].cell_color = COLOR_BLACK;
			}
			else if(i_energizer == 1 && map[x][y].type == ENERGIZER)
			{
				collied = 1;
				map[x][y].type = NOTHING;
				map[x][y].cell_color = COLOR_BLACK;
			}
		}
	}

	return collied;
}

bool check_win(std::array<std::array<cell_data, CELL_NUM_HEIGHT>, CELL_NUM_WIDTH> game_map)
{
	for (int y = 0; y < CELL_NUM_HEIGHT; y++)
    {
        for (int x = 0; x < CELL_NUM_WIDTH; x++)
		{
			if(game_map[x][y].type == PELLET || game_map[x][y].type == ENERGIZER)
			{
				return 0;
			}
		}
	}

	return 1;
}

void update_game_data(game_data &game, pm_data i_pacman, std::array<std::array<cell_data, CELL_NUM_HEIGHT>, CELL_NUM_WIDTH> game_map)
{	
	//update high_score
	if(game.points >= game.high_score)
	{
		game.high_score = game.points;
	}

	//another live
	if(game.points == 10000)
	{
		game.pm_lives ++;
	}

	if(check_win(game_map))
	{
		game.won = 1;
	}
}

void reset_entities(pm_data &i_pacman, ghosts &i_ghost)
{
	
	//reset the special red boy first
	i_ghost.ghost[0].x = i_ghost.ghost[0].exit[0] * CELL_SIZE;
	i_ghost.ghost[0].y = i_ghost.ghost[0].exit[1] * CELL_SIZE;
	i_ghost.ghost[0].use_door = 0;
	//set the other ghost
	for(int i = 1; i < 4; i++)
	{
		i_ghost.ghost[i].x = i_ghost.ghost[i].home[0] * CELL_SIZE;
		i_ghost.ghost[i].y = i_ghost.ghost[i].home[1] * CELL_SIZE;
		i_ghost.ghost[i].use_door = 0;
	}

	//reset pacman
	i_pacman.x = i_pacman.initial_x_y[0];
	i_pacman.y = i_pacman.initial_x_y[1];
	i_pacman.mode = NORMAL;
}

void draw_game(std::array<std::array<cell_data, CELL_NUM_HEIGHT>, CELL_NUM_WIDTH> game_map, pm_data &i_pacman, ghosts &i_ghost, game_data &game)
{   
	clear_screen(COLOR_BLACK);

	//draw the map
    for (unsigned int y = 0; y < CELL_NUM_HEIGHT; y++)
    {
        for (unsigned int x = 0; x < CELL_NUM_WIDTH; x++)
        {
            if(game_map[x][y].type != WALL && game_map[x][y].type != NOTHING)
            {
                switch (game_map[x][y].type)
                {
                case DOOR:
                    fill_rectangle(game_map[x][y].cell_color, x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE/4);
                    break;

                case PELLET:
                    fill_circle(game_map[x][y].cell_color, x * CELL_SIZE + CELL_SIZE/2, y * CELL_SIZE + CELL_SIZE/2, CELL_SIZE/10);
                    break;

                case ENERGIZER:
                    fill_circle(game_map[x][y].cell_color, x * CELL_SIZE + CELL_SIZE/2, y * CELL_SIZE + CELL_SIZE/2, CELL_SIZE/5);
                    break;

                default:
                    break;
                }
            }
			else
			    fill_rectangle(game_map[x][y].cell_color, x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE);
        }
    }

    //draw pac-man
    fill_circle(COLOR_YELLOW, i_pacman.x+ CELL_SIZE/2, i_pacman.y+ CELL_SIZE/2, ENTITIES_RADIUS);

    //draw the ghosts
    for(int i = 0; i < 4; i++)
    {
		if(i_ghost.ghost[i].mode != SCARED)
		{
			fill_rectangle(i_ghost.ghost[i].color, (i_ghost.ghost[i].x + CELL_SIZE/2) - (ENTITIES_RADIUS), i_ghost.ghost[i].y + CELL_SIZE/2, (ENTITIES_RADIUS)*2 + 1, CELL_SIZE/2 - 1);
        	fill_circle(i_ghost.ghost[i].color, i_ghost.ghost[i].x + CELL_SIZE/2, i_ghost.ghost[i].y + CELL_SIZE/2, ENTITIES_RADIUS);
		}
		else
		{
			//when power up start to run out
			if(timer_ticks(ENERGIZER_TIMER) >= ENERGIZER_TIME - 3000)
			{
				if(timer_ticks(ENERGIZER_TIMER) % 10 == 0)
				{
					fill_rectangle(COLOR_WHITE, (i_ghost.ghost[i].x + CELL_SIZE/2) - (ENTITIES_RADIUS), i_ghost.ghost[i].y + CELL_SIZE/2, (ENTITIES_RADIUS)*2 + 1, CELL_SIZE/2 - 1);
					fill_circle(COLOR_WHITE, i_ghost.ghost[i].x + CELL_SIZE/2, i_ghost.ghost[i].y + CELL_SIZE/2, ENTITIES_RADIUS);
				}
				else
				{
					fill_rectangle(COLOR_DARK_BLUE, (i_ghost.ghost[i].x + CELL_SIZE/2) - (ENTITIES_RADIUS), i_ghost.ghost[i].y + CELL_SIZE/2, (ENTITIES_RADIUS)*2 + 1, CELL_SIZE/2 - 1);
					fill_circle(COLOR_DARK_BLUE, i_ghost.ghost[i].x + CELL_SIZE/2, i_ghost.ghost[i].y + CELL_SIZE/2, ENTITIES_RADIUS);
				}
				
			}
			else
			{
				fill_rectangle(COLOR_DARK_BLUE, (i_ghost.ghost[i].x + CELL_SIZE/2) - (ENTITIES_RADIUS), i_ghost.ghost[i].y + CELL_SIZE/2, (ENTITIES_RADIUS)*2 + 1, CELL_SIZE/2 - 1);
				fill_circle(COLOR_DARK_BLUE, i_ghost.ghost[i].x + CELL_SIZE/2, i_ghost.ghost[i].y + CELL_SIZE/2, ENTITIES_RADIUS);
			}
		}

		
    }
	
	//game text
	draw_text("1UP",COLOR_WHITE,FONT_NAME, FONT_SIZE, CELL_SIZE*2 ,10);
	draw_text("HIGH SCORE",COLOR_WHITE,FONT_NAME, FONT_SIZE, (WIN_WIDTH/6)*2,10);
	draw_text(std::to_string(game.points),COLOR_WHITE,FONT_NAME,FONT_SIZE, CELL_SIZE*2,35);
	draw_text(std::to_string(game.high_score),COLOR_WHITE,FONT_NAME,FONT_SIZE, (WIN_WIDTH/5)*2,35);

	//pm_lives
	for(int i = 0; i < game.pm_lives; i++)
	{
		fill_circle(COLOR_YELLOW, CELL_SIZE/2 + CELL_SIZE*i, MAP_HEIGHT+CELL_SIZE/2, ENTITIES_RADIUS);
	}
	
	if(game.won == 1)
	{
		draw_text("YOU WON!",COLOR_YELLOW,FONT_NAME, FONT_SIZE, (WIN_WIDTH/6)*2 + CELL_SIZE/2,CELL_SIZE * 14 + CELL_SIZE/5);
		game.new_game = 1;
		refresh_screen();
		delay(3000);
	}

	if(i_pacman.mode == DEAD && game.pm_lives > 0)
	{
		//clear_screen(COLOR_BLACK);
		play_sound_effect(DYING);
		fill_circle(COLOR_RED, i_pacman.x+ CELL_SIZE/2, i_pacman.y+ CELL_SIZE/2, ENTITIES_RADIUS);
		game.new_game = 1;
		refresh_screen();
		delay(3000);
	}
	
	if(game.pm_lives == 0)//lose
	{
		clear_screen(COLOR_BLACK);
		play_sound_effect(DYING);
		draw_text("GAME OVER!",COLOR_YELLOW,FONT_NAME, FONT_SIZE, (WIN_WIDTH/5)*1.6, CELL_SIZE * 14 + CELL_SIZE/5);
		fill_circle(COLOR_RED, i_pacman.x+ CELL_SIZE/2, i_pacman.y+ CELL_SIZE/2, ENTITIES_RADIUS);

		//change later
		refresh_screen();
		delay(3000);
		exit(0);
	}
	
	refresh_screen(40);
}