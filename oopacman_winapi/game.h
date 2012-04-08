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

    HIMAGELIST images[OBJ_COUNT];

public:
    HIMAGELIST get_image_of(ObjType ot);

    Game(FILE *file, unsigned tick_ms, int cmdShow);
    ~Game();

    PacMan * get_pacman();
    inline const PacMan * get_pacman() const {
        return const_cast<Game *>(this)->get_pacman(); }

    Ghost * get_ghost(unsigned index);
    Timer * get_timer();
    Map * get_map();

    Label::Digital * get_score_label();
    Label::Bar * get_energizer_label();

    void draw_map(XPaintDC &dc);
    void draw_heroes();

    GameStats start();
};

