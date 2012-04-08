#pragma once

#include  <time.h>

#include  "oopacman.h"
#include  "map.h"
#include  "hero.h"
#include  "timer.h"
#include  "gameloop.h"
#include  "label.h"

class GameLoop; // predefined

class Game
{
private:
    Map map;
    PacMan pacman;
    Ghost ghosts[GHOST_NUMBER];
    Timer timer;

    GameLoop loop;
    Label::Digital score_label;
    Label::Bar energizer_label;

public:
    Game(FILE *file, unsigned tick_ms);
    ~Game();

    PacMan * get_pacman();
    Ghost * get_ghost(unsigned index);
    Timer * get_timer();

    Label::Digital * get_score_label();
    Label::Bar * get_energizer_label();

    void draw_map();
    void draw_heroes();

    GameStats start();
};

