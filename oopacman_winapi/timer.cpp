#include "stdafx.h"
#include  "timer.h"
#include <memory.h>

struct ObjInfo
{
    Object * obj;
    unsigned tick_period;
};

Timer::Timer(unsigned tick_ms)
{
    this->tick_ms = tick_ms;
    tick_counter = 0;
    objs_num = 1;
    objs = (ObjInfo *)malloc(1 * sizeof(objs[0]));
    if (objs == NULL)
        throw std::bad_alloc();

    objs[0].obj = NULL;
    objs[0].tick_period = 1;
}

void Timer::add_object(Object * object, unsigned tick_period)
{
    assert(object != this);
    objs = (ObjInfo *)realloc(objs, (objs_num+1)*sizeof(ObjInfo));
    if (objs == NULL)
        throw std::bad_alloc();

    ObjInfo * e = objs + objs_num++;
    e->obj = object;
    e->tick_period = tick_period;
}

void Timer::change_period(Object * object, unsigned tick_period)
{
    unsigned i = find(object);
    _ASSERT(i != INFINITE);
    objs[i].tick_period = tick_period;
}

void Timer::remove_object(Object * object)
{
    unsigned i = find(object);
    _ASSERT(i != INFINITE);
    if (i != objs_num - 1)
        memmove(objs+i, objs + i + 1, (objs_num - i - 1)*sizeof(objs[0]));
    objs = (ObjInfo *)realloc(objs, (--objs_num)*sizeof(objs[0]));
}

Timer::~Timer()
{
    free(objs);
}

bool Timer::processMessage(int message, Data extra_data)
{
    if (message == MSG_TICK)
    {
        ++tick_counter;
        getLoop()->postMessage(this, MSG_TICK, extra_data, tick_ms);
        for( unsigned i = 0 ; i < objs_num ; ++i )
            if (tick_counter % objs[i].tick_period == 0)
                getLoop()->postMessage(objs[i].obj, MSG_TICK, extra_data);
    }
    return false;
}

unsigned Timer::find(Object *obj)
{
    for(unsigned i = 0; i < objs_num; ++i)
        if ( objs[i].obj == obj )
            return i;
    return INFINITE;
}
