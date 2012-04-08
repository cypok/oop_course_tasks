#pragma once

#include "oopacman.h"

using namespace MDS;

enum Direction
{
    UP = 0,
    RIGHT,
    DOWN,
    LEFT
};

#define MUVIN_INTERVAL 5

class Muvin :
    public Object
{
protected:
    int x, y;
    int w, h;

    void draw(CDC *dc);

public: //protected:
    Direction dir;
    bool started;
public:
    Muvin(int w, int h);
    virtual ~Muvin();

    virtual bool processMessage(int message, Data extra_data);
};
