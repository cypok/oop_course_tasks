// GAME SETTINGS
#pragma once

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <new>

#include "errors.h"


#define MAPS_PATH           "maps"
#define MAPS_EXTENSION      ".map"

#define MENU_H_WELCOME      "W e l c o m e   t o   O O P a c M a n !"
#define MENU_H_WINNER       "Y o u  a r e  W I N N E R ! ! !"
#define MENU_H_LOSER        "G A M E   O V E R ! ! !"
#define MENU_H_LEVEL        "Choose the level or press Esc to exit"

#define TO_STR_HELPER(x) #x
#define TO_STR(x) TO_STR_HELPER(x)

#define GHOST_NUMBER        4

#define MAP_PACMAN          'C'
#define MAP_GHOST           '&'
#define MAP_FOOD           '.'
#define MAP_ENERGIZER       'O'
#define MAP_EMPTY           ' '
#define MAP_WALL            '#'

enum ObjType
{
    PACMAN = 0,
    GHOST,
    FOOD,
    ENERGIZER,
    EMPTY,
    WALL,
    DEAD,
    OBJ_COUNT // not an object, just that
};

//#define BACKGROUND_FG       CON_COLOR_BLACK
//#define BACKGROUND_BG       CON_COLOR_BLACK
//
//#define EMPTY_FG            BACKGROUND_FG
//#define EMPTY_BG            BACKGROUND_BG
//
//#define WALL_FG             CON_COLOR_BLUE
//#define WALL_BG             CON_COLOR_BLUE
//
//#define FOOD_FG            CON_COLOR_WHITE
//#define FOOD_BG            EMPTY_BG
//
//#define ENERGIZER_FG        CON_COLOR_GREEN
//#define ENERGIZER_BG        EMPTY_BG
//
//#define PACMAN_FG           CON_COLOR_YELLOW
//#define PACMAN_BG           EMPTY_BG
//
//#define GHOST_FG            CON_COLOR_CYAN
//#define GHOST_BG            EMPTY_BG
//
//#define DEAD_FG             CON_COLOR_BLACK
//#define DEAD_BG             EMPTY_BG
//
//#define LABEL_COLOR         32
//#define LABEL_INV_COLOR     33
//
#define BACKGROUND_COLOR    RGB(0,0,0)

#define LABEL_FG            CON_COLOR_WHITE
#define LABEL_BG            CON_COLOR_BLUE

#define TICKS_PER_SEC       100

// move delays
#define MD_PACMAN           20
#define MD_PACMAN_ENERGIZED 14
#define MD_GHOST            20

// energizer
#define ENERGIZE_DURATION   (30*MD_PACMAN_ENERGIZED)
#define DD_ENERGIZER        10                          // draw delay

#define ENERGIZER_STATES    "|/-\\"
#define ENERGIZER_STATES_NUM (sizeof(ENERGIZER_STATES)/sizeof(ENERGIZER_STATES[0]) - 1)

// score points
#define SCORE_FOOD         1
#define SCORE_ENERGIZER     10
#define SCORE_GHOST         30

// screen positions
#define SCREEN_MAP_X        0
#define SCREEN_MAP_Y        0

#define SCREEN_LABELS_X     0
#define SCREEN_LABELS_Y     0
#define SCREEN_LABELS_WIDTH 13

#define SCREEN_MENU_WIDTH   20
#define SCREEN_WELCOME_Y    3

// common draw flags
#define FLAG_ERASE          0x0001
// hero draw flags
#define FLAG_UP             0x0002
#define FLAG_RIGHT          0x0004
#define FLAG_DOWN           0x0008
#define FLAG_LEFT           0x0010
#define FLAG_AGGRESSIVE     0x0020
#define FLAG_ALIVE          0x0040
// 'whom to draw' flags
#define FLAG_PACMAN         0x0080
#define FLAG_GHOST          0x0100

enum
{
    STOP_FOOD_EATEN,
    STOP_PACMAN_DIED,
};

enum GameResult
{
    GR_NONE,
    GR_WINNER,
    GR_LOSER,
};

struct GameStats
{
    GameResult result;
    unsigned score;
};

enum Direction
{
    UP = 0,
    RIGHT,
    DOWN,
    LEFT
};

enum
{
    ENERGIZER_ON,
    ENERGIZER_OFF
};

enum
{
    MSG_TICK = WM_MDS,
    MSG_DRAW,
    MSG_STOP_GAME,
    MSG_START_GAME,
    MSG_ENERGIZER,
    MSG_SCORE_UPD,
    //MSG_RESIZED,
};
