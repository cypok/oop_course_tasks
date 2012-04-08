#ifndef _MDS_H_
#define _MDS_H_

#include "../console/console.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum _MDS_ERROR
{
    MDS_ERROR_OK = 0,
    MDS_ERROR_CANCEL,
    MDS_ERROR_MEMORY,
    MDS_ERROR_ALREADY_REGISTERED,
    MDS_ERROR_NULL_ARGUMENT,
    MDS_ERROR_UNREGISTERED_TYPE,
    MDS_ERROR_NOT_AN_OBJECT,
} MDS_ERROR;

typedef enum _MDS_MESSAGE
{
    MDS_MESSAGE_USER = 0,

    /* system messages */
    MDS_MESSAGE_IDLE                = -1,
    MDS_MESSAGE_KEY_PRESSED         = -2,
} MDS_MESSAGE;

#pragma pack(push, 1)
typedef union _MDS_DATA
{
    int integer;
    void * ptr;
} MDS_DATA;

typedef struct _MDS_RESULT
{
    MDS_DATA data;
    MDS_ERROR error;
} MDS_RESULT;

typedef struct _MDS_MSG_INFO
{
    void            *receiver;
    MDS_MESSAGE     message;
    MDS_DATA        extra_data;
} MDS_MSG_INFO;
#pragma pack(pop)

typedef MDS_DATA    (* MDS_MSGPROC)(void *obj, int message, MDS_DATA extra_data);
typedef MDS_ERROR   (* MDS_INITPROC)(void *obj, va_list *args);
typedef void        (* MDS_FINALPROC)(void *obj);

#pragma pack(push, 1)
typedef struct _ObjectType
{
    const char *id;
    size_t size;
    MDS_MSGPROC msg_proc;
    MDS_INITPROC init_proc;
    MDS_FINALPROC final_proc;
} MDS_ObjectType;
#pragma pack(pop)


#define INIT_OBJECT_TYPE(obj_type, type) {\
    (obj_type).id = #type;\
    (obj_type).size = sizeof(type);\
    (obj_type).init_proc = type##InitProc;\
    (obj_type).final_proc = type##FinalProc;\
    (obj_type).msg_proc = type##MsgProc; }

extern MDS_ERROR        MDS_init();
extern void             MDS_cleanUp();

extern MDS_ERROR        MDS_registerObjectType      (MDS_ObjectType * object_type);
extern MDS_ObjectType * MDS_getObjectType           (const char *id);

extern MDS_ERROR        MDS_createObject            (char *id, void **obj);
extern MDS_ERROR        MDS_initObject              (void *obj, ...);
extern MDS_ERROR        MDS_deleteObject            (void *obj);

extern MDS_RESULT       MDS_sendMessage             (void *obj, int message, MDS_DATA extra_data);
extern MDS_ERROR        MDS_postMessage             (void *obj, int message, MDS_DATA extra_data, unsigned delay);
extern MDS_ERROR        MDS_mainLoop                (MDS_MSGPROC def_msg_proc);

extern void             MDS_dumpObjectType          (FILE *file);
extern void             MDS_dumpObject              (FILE *file, void *object);
extern void             MDS_dumpQueue                (FILE *file);


#ifdef __cplusplus
}
#endif

#endif
