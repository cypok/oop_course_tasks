#include "clock.h"

class CharBufferHolder
{
public:
    char *data;

    CharBufferHolder(size_t size)
    {
        data = new char[size];
    }

    ~CharBufferHolder()
    {
        delete[] data;
    }
};

Clock::Clock(Coord pos)
    : ConsoleObject(pos, Coord(sizeof("[00:00:00]")-1, 1))
{
    ticking_started = false;
    flying = false;

    srand( (unsigned)time( NULL ) );
}

void Clock::render(int action)
{
    con_gotoXY(pos.x, pos.y);
    if (action == 0)
    {
        CharBufferHolder buf(size.x + 1);
        time_t curr_time;
        struct tm * curr_time_tm;

        time(&curr_time);
        curr_time_tm = localtime(&curr_time);
        strftime(buf.data, size.x + 1, "%H:%M:%S", curr_time_tm);

        con_setColor(OBJ_COL);
        if (active)
            con_outTxt("<%*s>", size.x-2, buf.data);
        else
            con_outTxt("[%*s]", size.x-2, buf.data);
    }
    else
    {
        con_setColor(BG_COL);
        con_outTxt("%*s", size.x, "");
    }
    rendered();
}

bool Clock::processMessage(int message, Data extra_data)
{
    bool result = ConsoleObject::processMessage(message, extra_data);
    if (message == MESSAGE_TICK)
    {
        getLoop()->postMessage(this, MESSAGE_RENDER, Data(0));
        getLoop()->postMessage(this, MESSAGE_TICK, Data(0), 1000);
        return false;
    }
    else if (flying && message == MESSAGE_FLYTICK)
    {
        render(-1);
        move(DIRECTION(1 + (rand() % 4)));
        render(0);
        getLoop()->postMessage(this, MESSAGE_FLYTICK, Data(0), 200);
        return false;
    }
    else if( !ticking_started && message == MESSAGE_RENDER)
    {
        ticking_started = true;
        getLoop()->postMessage(this, MESSAGE_TICK, Data());
        return false;
    }
    else
        return result;
}

void Clock::toggle_flying()
{
    flying = !flying;
    if (flying)
    {
        getLoop()->postMessage(this, MESSAGE_FLYTICK, Data(0));
    }
}

bool Clock::processKey(int ch)
{
    if (!ConsoleObject::processKey(ch))
    {
        if (ch == 'r')
            toggle_flying();
        else
            return false;
    }
    return true;
}
