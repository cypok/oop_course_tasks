#include "internals.h"

static MDS_TYPE_DESCRIPTOR *types;
static MDS_QUEUE_ITEM *q_out = NULL;

#define MDS_GET_HEADER(x) (((MDS_HEADER *)(x)) - 1)

MDS_ERROR MDS_init()
{
    con_init();
    types = NULL;
    return MDS_ERROR_OK;
}

void MDS_cleanUp()
{
    con_deinit();
    {
        MDS_TYPE_DESCRIPTOR *cur = NULL;
        MDS_TYPE_DESCRIPTOR *next = types;
        while (next != NULL)
        {
            cur = next;
            next = next->next;
            free((void*)cur->type->id);
            free(cur->type);
            free(cur);
        }
    }
    {
        MDS_QUEUE_ITEM *cur = NULL;
        MDS_QUEUE_ITEM *next = q_out;
        while (next != NULL)
        {
            cur = next;
            next = next->next;
            free(cur);
        }
    }
}

MDS_ERROR MDS_registerObjectType(MDS_ObjectType * object_type)
{

    if (!object_type)
        return MDS_ERROR_NULL_ARGUMENT;
    else if (MDS_getObjectType(object_type->id))
        return MDS_ERROR_ALREADY_REGISTERED;
    else
    {
        MDS_TYPE_DESCRIPTOR *td = (MDS_TYPE_DESCRIPTOR*)malloc(sizeof(*types));
        if ((NULL == td) ||
            (NULL == (td->type = (MDS_ObjectType*)malloc(sizeof(*td->type)))) ||
            (NULL == (td->type->id = strdup(object_type->id))))
            return MDS_ERROR_MEMORY;
        td->next = types;
        td->type->msg_proc = object_type->msg_proc;
        td->type->init_proc = object_type->init_proc;
        td->type->final_proc = object_type->final_proc;
        td->type->size = object_type->size;
        types = td;
        return MDS_ERROR_OK;
    }
}

void MDS_dumpObjectType(FILE *file)
{
    MDS_TYPE_DESCRIPTOR *cur;
    MDS_ObjectType *obj_type;
    for(cur = types; cur != NULL; cur = cur->next)
    {
        obj_type = (MDS_ObjectType *)(cur->type);
        fprintf(file, "[ id: \"%s\", size: %u, msg_proc: %p ]\n", obj_type->id, obj_type->size, obj_type->msg_proc);
    }
}

void MDS_dumpObject(FILE *file, void *object)
{
    MDS_HEADER *header;
    if (object == NULL)
    {
        fprintf(file, "< (nil) >");
        return;
    }
    header = MDS_GET_HEADER(object);
    if (header->magic == MDS_MAGIC)
    {
        MDS_ObjectType *obj_type = header->obj_type;
        fprintf(file, "< \"%s\" instance, data: %p >", obj_type->id, (void *)(header + 1));
    }
    else
    {
        fprintf(file, "< not an object >");
    }
}


MDS_ObjectType * MDS_getObjectType(const char *id)
{
    MDS_TYPE_DESCRIPTOR *cur;
    MDS_ObjectType *obj_type;
    for(cur = types; cur != NULL; cur = cur->next)
    {
        obj_type = (MDS_ObjectType *)(cur->type);
        if (strcmp(obj_type->id, id) == 0)
            return obj_type;
    }
    return NULL;
}

MDS_ERROR  MDS_createObject(char *id, void **obj)
{
    MDS_ObjectType *obj_type;
    if (!obj || !id)
        return MDS_ERROR_NULL_ARGUMENT;

    obj_type = MDS_getObjectType(id);
    if (!obj_type)
        return MDS_ERROR_UNREGISTERED_TYPE;
    else
    {
        MDS_HEADER *header;
        header = (MDS_HEADER *)malloc(sizeof(*header) + obj_type->size);
        if (!header)
            return MDS_ERROR_MEMORY;

        header->magic = MDS_MAGIC;
        header->obj_type = obj_type;

        *obj = header + 1;
        return MDS_ERROR_OK;
    }
}

MDS_ERROR MDS_checkObject(void *obj)
{
    if (obj != NULL)
    {
        if (MDS_GET_HEADER(obj)->magic != MDS_MAGIC)
            return MDS_ERROR_NOT_AN_OBJECT;
        else
            return MDS_ERROR_OK;
    }
    else
        return MDS_ERROR_NULL_ARGUMENT;
}

MDS_ERROR MDS_deleteObject(void *obj)
{
    MDS_ERROR error = MDS_checkObject(obj);
    if (error == MDS_ERROR_OK)
    {
        MDS_GET_HEADER(obj)->obj_type->final_proc(obj);
        free(MDS_GET_HEADER(obj));
    }
    return error;
}

MDS_ERROR MDS_initObject(void *obj, ...)
{
    MDS_ERROR error = MDS_checkObject(obj);
    if (error == MDS_ERROR_OK)
    {
        MDS_HEADER *header = MDS_GET_HEADER(obj);
        va_list arglist;
        va_start(arglist, obj);
        error = header->obj_type->init_proc(obj, &arglist);
        va_end(arglist);
    }
    return error;
}

MDS_RESULT MDS_sendMessage(void *obj, int message, MDS_DATA extra_data)
{
    MDS_RESULT result = {{0}, MDS_checkObject(obj)};
    if (result.error == MDS_ERROR_OK)
        result.data = (MDS_GET_HEADER(obj)->obj_type->msg_proc)(obj, message, extra_data);
    return result;
}

MDS_ERROR MDS_postMessage(void *obj, int message, MDS_DATA extra_data, unsigned delay)
{
    MDS_QUEUE_ITEM *item = (MDS_QUEUE_ITEM*)malloc(sizeof(*item));
    if (item == NULL)
        return MDS_ERROR_MEMORY;
    item->time = clock() + (clock_t)delay*CLOCKS_PER_SEC/1000;
    item->info.receiver = obj;
    item->info.message    = message;
    item->info.extra_data = extra_data;
    if (q_out == NULL)
    {
        q_out = item;
        item->next = NULL;
    }
    else
    {
        MDS_QUEUE_ITEM *cur = q_out;
        if (q_out->time > item->time)
        {
            item->next = q_out;
            q_out = item;
        }
        else
        {
            while (cur->next != NULL && cur->next->time <= item->time)
                cur = cur->next;
            item->next = cur->next;
            cur->next = item;
        }
    }
    return MDS_ERROR_OK;
}

void MDS_dumpQueue(FILE *file)
{
    MDS_QUEUE_ITEM *cur;
    MDS_MSG_INFO *info;
    fprintf(file, "Messages queue (current time = %i):\n", (int)clock());
    for(cur = q_out; cur != NULL; cur = cur->next)
    {
        info = &(cur->info);
        fprintf(file, "[ time: %i, receiver: ", cur->time);
        MDS_dumpObject(file, info->receiver);
        fprintf(file, ", message: %i, extra-data: %i(%p) ]\n", info->message, info->extra_data.integer, info->extra_data.ptr);
    }
}

MDS_ERROR MDS_mainLoop(MDS_MSGPROC def_msg_proc)
{
    for (;;)
    {
        MDS_MSG_INFO msg;

        if (con_keyPressed())
        {
            msg.message = MDS_MESSAGE_KEY_PRESSED;
            msg.extra_data.integer = con_getKey();
        }
        else if (q_out == NULL || q_out->time > clock())
        {
            msg.message = MDS_MESSAGE_IDLE;
            msg.extra_data.integer = 0;
        }
        else
        {
            MDS_QUEUE_ITEM * qelem = q_out;
            msg = qelem->info;

            q_out = q_out->next;
            free(qelem);

            if (msg.receiver != NULL)
            {
                MDS_ERROR result = MDS_sendMessage(msg.receiver, msg.message, msg.extra_data).error;
                if (result != MDS_ERROR_OK)
                    return result;
                continue;
            }
        }
        /* call def msg proc */
        {
            MDS_ERROR result = (MDS_ERROR)def_msg_proc(NULL, msg.message, msg.extra_data).integer;
            if (result != MDS_ERROR_OK)
                return result;
        }
    }
}
