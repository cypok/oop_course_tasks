#include "stdafx.h"
#include  "timer.h"

Timer::Timer(unsigned tick_ms) : tick_ms(tick_ms)
{
    objs_num = 2;
    objs = (Object **)malloc(2 * sizeof(Object *));

    if (objs == NULL)
        throw std::bad_alloc();
    objs[0] = this;
    objs[1] = NULL;
}

void Timer::add_object(Object * object)
{
    assert(object != this);
    ++objs_num;
    objs = (Object **)realloc(objs, objs_num*sizeof(Object *));
    if (objs == NULL)
        throw std::bad_alloc();
    objs[objs_num - 1] = object;
}

Timer::~Timer()
{
    free(objs);
}

bool Timer::processMessage(int message, Data extra_data)
{
    if (message == MSG_TICK)
        for( unsigned i = 0 ; i < objs_num ; ++i )
            getLoop()->postMessage(objs[i], MSG_TICK, extra_data, tick_ms);
    return false;
}

