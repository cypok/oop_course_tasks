#pragma once

#include "../mds/mds.h"
#include "../lib/console.h"
#include "console_object.h"
#include <memory.h>

using namespace MDS;

#define MAX_OBJECTS_NUM 50

class ConsoleLoop : public MessageLoop
{
private:
    int     active_index;

    ConsoleObject   *objs[MAX_OBJECTS_NUM];
    size_t          objs_num;

protected:
    virtual void beforeRun();

    virtual int getKey();
    virtual int keyPressed();


public:
    ConsoleLoop(ConsoleObject *objs[], size_t n);

    // FOR ALL: if true leave run()
    virtual bool processMessage(int message, Data extra_data);
    virtual bool processKey(int ch);
    virtual bool processIdle();
};
