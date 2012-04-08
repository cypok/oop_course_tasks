#pragma once

#include  "oopacman.h"
#include  <new>

struct ObjInfo;

class Timer : public Object
{
private:
    unsigned tick_ms;
    unsigned tick_counter;

    unsigned objs_num;
    ObjInfo *objs;

    unsigned find(Object *obj);

public:
    Timer(unsigned tick_ms);
    ~Timer();

    void add_object(Object * object, unsigned tick_period);
    void change_period(Object * object, unsigned tick_period);
    void remove_object(Object * object);

    virtual bool processMessage(int message, Data extra_data);
};

