#pragma once

#include "../mds.h"
#include <string.h>
#include <time.h>
#include <stdexcept>

namespace MDS
{
#pragma pack(push, 1)

struct MsgInfo
{
    Object  *receiver;
    int     message;
    Data    extra_data;
};

class QueueItem : public MsgInfo
{
public:
    QueueItem  *next;
    clock_t     time;

    QueueItem(Object *receiver, int message, Data extra_data, clock_t time, QueueItem *next = NULL);
};

#pragma pack(pop)
}
