#pragma once

#include "splashkit.h"

//game constants
const int FONT_SIZE = 15;
const string FONT_NAME = "main";
const string FONT_FILENAME = "PressStart2P-Regular.ttf";
const string GAME_TIMER = "GameTimer";
const string ENERGIZER_TIMER = "EnergizerTimer";
const int PELLET_PTS = 10;//10pts each
const int ENERGIZER_PTS = 50;//50pts each
const int TOTAL_DOTS = 150;
const int GHOST_PTS = 200;

//load sound effects
const string WAKA = "waka";
const string WAKA_FILENAME = "waka2.ogg";

const string DYING = "dying";
const string DYING_FILENAME = "pm_dying.mp3";

const string EAT_GHOST = "eat_ghost";
const string EAT_GHOST_FILENAME = "pm_eat_ghost.mp3";


//load background music
const string INTRO = "intro";
const string INTRO_FILENAME = "pm_start.ogg";

//map dimensions
const int CELL_SIZE = 20;
const int CELL_NUM_HEIGHT = 24;
const int CELL_NUM_WIDTH = 19;
const int MAP_WIDTH = CELL_SIZE * CELL_NUM_WIDTH;
const int MAP_HEIGHT = CELL_SIZE * CELL_NUM_HEIGHT;
const int WIN_WIDTH = MAP_WIDTH;
const int WIN_HEIGHT = MAP_HEIGHT + CELL_SIZE * 3;

//entities dimensions
const int ENTITIES_RADIUS = CELL_SIZE/2 - CELL_SIZE/10;

//pacman dimensions
const int PM_SPEED = 2;
const int ENERGIZER_TIME = 9000; //9seconds

//ghosts dimensions
const int GHOST_SPEED = 2;
const int GHOST_SCARED_SPEED = 1;

const int GHOST_P_CHASE = 2;//pink ghost target the 2th cell infront of pacman
const int GHOST_O_CHASE = 4;//if distance less than 4 cells, orange switch to scatter
const int GHOST_B_CHASE = 1;//target 1 cell infront of pacman, to calculate blue ghost target

const int LONG_SCATTER_TIME = 7000;
const int SHORT_SCATTER_TIME = 5000;
const int CHASE_TIME = 20000;
const int WAVE_1_END = LONG_SCATTER_TIME + CHASE_TIME;
const int WAVE_2_END = WAVE_1_END * 2;
const int WAVE_3_END = WAVE_2_END + SHORT_SCATTER_TIME + CHASE_TIME;
const int WAVE_4_END = WAVE_3_END + SHORT_SCATTER_TIME;


typedef enum
{
    NOTHING,//" "
    WALL,   //"#"
    PELLET, //"."
    ENERGIZER,  //"o"
    DOOR    //"="
}cell_type;

typedef enum
{
    NORMAL,
    ENERGIZED,
    DEAD
}pm_mode;

typedef enum
{   
    SCATTER,
    CHASE,
    SCARED,
    EATEN
}ghost_mode;

struct cell_data
{
    cell_type type = NOTHING;
    color cell_color = COLOR_BLACK;
};

struct game_data
{
    bool new_game = 1;
    bool won = 0;
    int pm_lives = 2;
    int high_score = 10000;
    int points = 0;
};

struct ghost_data
{
	int id;
    int x = 0;
	int y = 0;
    int direction = 0;//0:right 1:up 2:left 3:down
    bool use_door = 0;//door privilege
	ghost_mode mode = SCATTER;
	color color;
    
    int target[2];
    int exit[2];//{x,y}
    int home[2];
};

struct ghosts
{
    ghost_data ghost[4]; //0:red 1:blue 2:orange 3:pink
};

struct pm_data
{
	int x = 0;
	int y = 0;
	int direction = 0;//0:still 1:right 2:up 3:left 4:down
    int initial_x_y[2];//new game starting position
    pm_mode mode = NORMAL;
	color color;
};