#ifndef _INTERNALS_H_
#define _INTERNALS_H_

#include "../mds.h"
#include <string.h>
#include <time.h>
#include <stdarg.h>

#define MDS_MAGIC 0x110011FF

#pragma pack(push, 1)
typedef struct _MDS_HEADER
{
    int magic; // MDS_MAGIC
    MDS_ObjectType *obj_type;
} MDS_HEADER;

typedef struct MDS_TYPE_DESCRIPTOR
{
    struct MDS_TYPE_DESCRIPTOR    *next;
    MDS_ObjectType            *type;
} MDS_TYPE_DESCRIPTOR;

typedef struct _MDS_QUEUE_ITEM
{
    struct _MDS_QUEUE_ITEM *next;
    clock_t            time;
    MDS_MSG_INFO    info;
} MDS_QUEUE_ITEM;
#pragma pack(pop)

#endif
