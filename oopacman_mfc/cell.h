#pragma once

#include "settings.h"
class Map; // predefined

class Cell;

class WaveInfo
{
public:
    unsigned depth;
    Cell *next;
    Cell *prev;

    void reset();
    WaveInfo();
};

class Cell : public Object
{
private:
    ObjType content;
    Cell * neighbors[4];
    struct {
        int x,y;
    } screen_position;
    Map *map;

    void draw(CDC &dc, int flags);
    void draw_at(CDC &dc, int x, int y, ObjType ot);

public:
    void redraw();

    WaveInfo wave;
    void start_wave(); // starts wave from this cell

    Cell(ObjType content,
            Cell * up, Cell * right, Cell * down, Cell *left, int x, int y, Map *map);

    bool is_wall();
    bool is_here(ObjType ot);
    bool can_move(Direction dir);
    ObjType pop_content();
    void set_dead();
    Cell * neighbor(Direction dir);
    ObjType get_content();

    virtual bool processMessage(int message, Data extra_data);

    void * operator new(unsigned size, void *pointer);
#ifdef _MSC_VER
    void operator delete(void *pointer, void *pointer2);
#endif
};

#include  "map.h" // Cell must be defined earlier than Map

