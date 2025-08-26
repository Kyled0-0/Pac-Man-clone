#include "Headers/game_map.h"
#include "Headers/ghost.h"
#include "Headers/pacman.h"
#include "Headers/ghostmanager.h"


int main()
{
	load_font(FONT_NAME,FONT_FILENAME);

	create_timer(GAME_TIMER);
	create_timer(ENERGIZER_TIMER);

	load_sound_effect(WAKA, WAKA_FILENAME);
	load_sound_effect(DYING, DYING_FILENAME);
	load_sound_effect(EAT_GHOST, EAT_GHOST_FILENAME);

	load_music(INTRO, INTRO_FILENAME);

	game_data game;
	ghosts i_ghost;
	pm_data pacman;

    std::array<string,CELL_NUM_HEIGHT> map_sketch{
		"                   ",
		"                   ",
		"                   ",
		"###################",
		"#........#........#",
		"#o##.###.#.###.##o#",
		"#.................#",
		"#.##.#.#####.#.##.#",
		"#....#...#...#....#",
		"####.### # ###.####",
		"   #.#   1   #.#   ",
		"####.# ##=## #.####",
		"    .  #234#  .    ",
		"####.# ##### #.####",
		"   #.#       #.#   ",
		"####.# ##### #.####",
		"#........#........#",
		"#.##.###.#.###.##.#",
		"#o.#.....P.....#.o#",
		"##.#.#.#####.#.#.##",
		"#....#...#...#....#",
		"#.######.#.######.#",
		"#.................#",
		"###################"
	};

	open_window("Pac-Man",WIN_WIDTH, WIN_HEIGHT);

	std::array<std::array<cell_data, CELL_NUM_HEIGHT>, CELL_NUM_WIDTH> game_map = convert_sketch(map_sketch, i_ghost, pacman);

	play_music(INTRO, 1);

	while(!quit_requested())
	{	
		
		while(game.new_game == 1)
		{	
			if(pacman.mode == DEAD)
			{
				stop_timer(GAME_TIMER);//reset the timer
				reset_entities(pacman, i_ghost);
			}
			else if(game.won == 1)
			{
				clear_screen(COLOR_BLACK);
				stop_timer(GAME_TIMER);//reset the timer
				game_map = convert_sketch(map_sketch, i_ghost, pacman);//generate a new map
				game.won = 0;
			}
				
			draw_game(game_map, pacman, i_ghost, game);
			draw_text("READY!",COLOR_YELLOW,FONT_NAME, FONT_SIZE, (WIN_WIDTH/5)*2 - CELL_SIZE/4,CELL_SIZE * 14 + CELL_SIZE/5);
			refresh_screen();
			delay(5000);
			game.new_game = 0;

			start_timer(GAME_TIMER);
		}

		update_pacman(pacman, i_ghost, game_map, game);
		time_out(pacman, i_ghost); 

		manage_ghost(pacman, i_ghost, game, game_map);

		update_game_data(game, pacman, game_map); //check game status (win/lose)

		draw_game(game_map, pacman, i_ghost, game);
			
		process_events();
		
	}

    return 0;
}