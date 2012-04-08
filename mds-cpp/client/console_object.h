#pragma once

#include "../mds/mds.h"
#include "../lib/console.h"

using namespace MDS;

#define MESSAGE_MOVE                1
#define MESSAGE_CHANGE_ACTIVE       2
#define MESSAGE_FOCUS               3
#define MESSAGE_RENDER              4
#define MESSAGE_MOVED               5
#define MESSAGE_RENDERED            6

enum DIRECTION
{
    DIR_UP = 1,
    DIR_RIGHT,
    DIR_DOWN,
    DIR_LEFT
};

#pragma pack(push, 1)
class Coord
{
public:
    int x;
    int y;
    Coord(int x = 0, int y = 0);
};

class Rect
{
public:
    int x1;
    int y1;
    int x2;
    int y2;

    Rect(Coord pos, Coord size);
    bool overlapped_with(Rect rect);
};

class ConsoleObject : public Object
{
protected:
    Coord   pos;
    Coord   size;
    bool    alive;
    bool    active;

    void move(DIRECTION dir);
    void focus(bool is_active);

    virtual void render(int mode) = 0; // 0 - render, else clear
    void rendered(); // must be called by all render functions

public:
    ConsoleObject(Coord pos, Coord size);

    virtual bool processMessage(int message, Data extra_data);

    virtual bool processKey(int ch); // return true if key was processed

    bool is_alive() const;
    Coord get_pos() const;
    Coord get_size() const;
    Rect get_rect() const;
};

#define OBJ_COL 1
#define BG_COL 2

#pragma pack(pop)
