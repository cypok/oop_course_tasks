#pragma once

#include  "settings.h"
#include  <new>

class Timer : public Object
{
private:
    unsigned tick_ms;

    unsigned objs_num;
    Object **objs;

public:
    Timer(unsigned tick_ms);
    ~Timer();

    void add_object(Object * object);

    virtual bool processMessage(int message, Data extra_data);
};

