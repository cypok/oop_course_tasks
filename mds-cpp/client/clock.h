#pragma once

#include "console_object.h"
#include "../mds/mds.h"

#include <stdlib.h>
#include <time.h>

using namespace MDS;

#define MESSAGE_TICK        100
#define MESSAGE_FLYTICK     101

#pragma pack(push, 1)
class Clock: public ConsoleObject
{
protected:
    bool ticking_started;

    bool flying;
    void toggle_flying();

    virtual void render(int mode);

public:
    Clock(Coord pos);

    virtual bool processKey(int ch);

    bool processMessage(int message, Data extra_data);
};
#pragma pack(pop)

