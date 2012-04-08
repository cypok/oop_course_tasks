#pragma once

#ifdef _MSC_VER
#pragma warning (disable : 4996)
#endif

#include "lib/mds.h"
#include "lib/console.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <new>
#include <time.h>

#include "errors.h"

using namespace MDS;

#define TO_STR_HELPER(x) #x
#define TO_STR(x) TO_STR_HELPER(x)

#define GHOST_NUMBER        4

#define MAP_PACMAN          'C'
#define MAP_GHOST           '&'
#define MAP_POINT           '.'
#define MAP_ENERGIZER       'O'
#define MAP_EMPTY           ' '
#define MAP_WALL            '#'

enum ObjType
{
    PACMAN = 1, // cause it's number of color_pair which can't be null
    GHOST,
    POINT,
    ENERGIZER,
    EMPTY,
    WALL,
};

#define EMPTY_FG            CON_COLOR_BLACK
#define EMPTY_BG            CON_COLOR_BLACK

#define WALL_FG             CON_COLOR_BLUE
#define WALL_BG             CON_COLOR_BLUE

#define POINT_FG            CON_COLOR_WHITE
#define POINT_BG            EMPTY_BG

#define ENERGIZER_FG        CON_COLOR_GREEN
#define ENERGIZER_BG        EMPTY_BG

#define PACMAN_FG           CON_COLOR_YELLOW
#define PACMAN_BG           EMPTY_BG

#define GHOST_FG            CON_COLOR_CYAN
#define GHOST_BG            EMPTY_BG

#define LABEL_COLOR         32
#define LABEL_INV_COLOR     33

#define LABEL_FG            CON_COLOR_WHITE
#define LABEL_BG            CON_COLOR_BLUE

#define TICKS_PER_SEC       100

// move delays
#define MD_PACMAN           20
#define MD_PACMAN_ENERGIZED 14
#define MD_GHOST            20

// energizer
#define ENERGIZE_DURATION   (100*MD_PACMAN_ENERGIZED)
#define DD_ENERGIZER        10                          // draw delay

// score points
#define SCORE_POINT         1
#define SCORE_ENERGIZER     10
#define SCORE_GHOST         30

// screen positions
#define SCREEN_MAP_X        15
#define SCREEN_MAP_Y        0

#define SCREEN_LABELS_X     0
#define SCREEN_LABELS_Y     0
#define SCREEN_LABELS_WIDTH 13

// hero draw flags
#define HERO_UP             0x01
#define HERO_RIGHT          0x02
#define HERO_DOWN           0x04
#define HERO_LEFT           0x08
#define HERO_AGGRESSIVE     0x10
#define HERO_ALIVE          0x20

enum
{
    STOP_POINTS_EATEN,
    STOP_PACMAN_DIED,
    STOP_PAUSED
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
    MSG_TICK,
    MSG_DRAW,
    MSG_STOP_GAME,
    MSG_START_GAME,
    MSG_ENERGIZER,
    MSG_SCORE_UPD,
};

