#pragma once

#include "settings.h"
#include "cell.h"
#include <stdio.h>

class Game; // predefined

class Map : public Object
{
private:
    unsigned screen_position_x;
    unsigned screen_position_y;
    unsigned width, height;
    Cell *cells;
    struct
    {
        Cell *pacman;
        Cell *ghosts[GHOST_NUMBER];
    } init_positions;
    unsigned food_num;
    unsigned energizers_count;
    unsigned tick_counter;
    Cell **energizers;


    Game *game;

public:
    Map(FILE *file, unsigned screen_position_x, unsigned screen_position_y, Game *game);
    ~Map();

    void set_cells_loop();
    void draw(CDC &dc);
    void clear();
    void reset_wave();
    unsigned get_ticks();

    ObjType who_is_here(Cell *cell);

    void pop_food();

    Cell * init_pos_pacman();
    Cell * init_pos_ghost(unsigned index);

    void dump_and_check(FILE *file);
    void dump_wave(FILE *file);
    RECT get_rect();

    virtual bool processMessage(int message, Data extra_data);
};

#include  "game.h"  // Map must be defined earlier then Game

