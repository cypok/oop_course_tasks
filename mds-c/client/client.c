#include <stdio.h>

#include "../mds/mds.h"
#include "../lib/console.h"
#include <time.h>
#include <string.h>
#include <stdarg.h>

#ifndef NDEBUG
#   ifdef WIN32
#       pragma warning(disable : 4996)
#       define _CRTDBG_MAP_ALLOC
#       include <crtdbg.h>
#   elif __GLIBC__
#       include <mcheck.h>
#   endif
#endif

#define MESSAGE_CHANGE_ACTIVE       (MDS_MESSAGE_USER+0)
#define MESSAGE_FOCUS               (MDS_MESSAGE_USER+1)
#define MESSAGE_RENDER              (MDS_MESSAGE_USER+2)
#define MESSAGE_MOVE                (MDS_MESSAGE_USER+3)
#define MESSAGE_CLOCK_KEY_PRESSED   (MDS_MESSAGE_USER+4)
#define MESSAGE_STOPWATCH_INC       (MDS_MESSAGE_USER+5)
#define MESSAGE_STOPWATCH_START     (MDS_MESSAGE_USER+6)

const MDS_DATA NULL_DATA = {0};

#pragma pack(push, 1)
typedef struct _Coord
{
    int x,y;
} Coord;

typedef struct _Object
{
    Coord pos;
    Coord size;
    int alive;
    int active;
} Object;

typedef struct _Clock
{
    Coord pos;
    Coord size;
    int alive;
    int active;
} Clock;

typedef struct _Buffer
{
    Coord pos;
    Coord size;
    int alive;
    int active;

    char *text;
    size_t text_len;
    unsigned int cur_ind;
} Buffer;

typedef struct _Button
{
    Coord pos;
    Coord size;
    int alive;
    int active;

    MDS_MSG_INFO msg;
    char *caption;
} Button;

#define STOPWATCH_INC   50
typedef struct _Stopwatch
{
    Coord pos;
    Coord size;
    int alive;
    int active;

    int running;
    int msec;
    MDS_DATA msg_id; //current everysecond message identifier (to avoid message reproduction and delay decreasing)
} Stopwatch;
#pragma pack(pop)

#define OBJ_NUM   7
Object *objects[OBJ_NUM];
int active_index;

MDS_DATA def_msg_proc(void *obj, int message, MDS_DATA extra_data)
{
    static int render_index = 0;
    MDS_DATA data;
    data.integer = MDS_ERROR_OK;
    switch (message)
    {
    case MDS_MESSAGE_IDLE:
        {
            int prev = render_index;
            do
                render_index = (render_index + 1) % OBJ_NUM;
            while (!objects[render_index]->alive && render_index != prev);

            if (objects[render_index]->alive)
                MDS_sendMessage(objects[render_index], MESSAGE_RENDER, NULL_DATA);
            break;
        }
    case MDS_MESSAGE_KEY_PRESSED:
        switch (extra_data.integer)
        {
        case CON_KEY_ESCAPE:
            data.integer = MDS_ERROR_CANCEL;
            break;

        case '\t':
            MDS_postMessage(NULL, MESSAGE_CHANGE_ACTIVE, NULL_DATA, 0);
            break;

        default:
            if (active_index >= 0)
                MDS_postMessage(objects[active_index], MDS_MESSAGE_KEY_PRESSED, extra_data, 0);
            break;
        }
        break;
    case MESSAGE_CHANGE_ACTIVE:
        {
            MDS_DATA flag;
            int previous = active_index;

            flag.integer = 0;
            MDS_sendMessage(objects[previous], MESSAGE_FOCUS, flag);

            do
                active_index = (active_index + 1) % OBJ_NUM;
            while (!objects[active_index]->alive && previous != active_index);
            if (previous == active_index && !objects[active_index]->alive)
                active_index = -1;
            else
            {
                flag.integer = 1;
                MDS_sendMessage(objects[active_index], MESSAGE_FOCUS, flag);
            }
            break;
        }
    }
    return data;
}

void move_object(Object *obj, int dir)
{
    //          /-
    //          |   1 if moving UP
    //          |   2 if moving RIGHT
    //   dir  = |   3 if moving DOWN
    //          |   4 if moving LEFT
    //          \-
    int x, y;
    con_getMaxXY(&x, &y);
    if (dir == 1 && obj->pos.y > 0)
        --obj->pos.y;
    else if (dir == 2 && (obj->pos.x + obj->size.x) < x)
        ++obj->pos.x;
    else if (dir == 3 && (obj->pos.y + obj->size.y) < y)
        ++obj->pos.y;
    else if (dir == 4 && obj->pos.x > 0)
        --obj->pos.x;
}

void set_activity(Object *obj, int flag)
{
    obj->active = flag;
}

int process_keys(Object *obj, int key)
{
    // return 1 if key processed else 0
    int dir = 0;
    switch (key)
    {
    // TODO
    //case CON_KEY_DEL:
    //    MDS_postMessage(obj, MDS_MESSAGE_FINALIZE_OBJECT, NULL_DATA, 0);
    //    return 1;
    case CON_KEY_LEFT:  // dir = 4
        ++dir;
    case CON_KEY_DOWN:  // dir = 3
        ++dir;
    case CON_KEY_RIGHT: // dir = 2
        ++dir;
    case CON_KEY_UP:    // dir = 1
        ++dir;
        {
            MDS_DATA data;

            data.integer = -1;
            MDS_sendMessage(obj, MESSAGE_RENDER, data);

            data.integer = dir;
            MDS_sendMessage(obj, MESSAGE_MOVE, data);

            MDS_sendMessage(obj, MESSAGE_RENDER, NULL_DATA);
        }
        return 1;
    default:            return 0;
    }
}

void add_char(Buffer *buffer, char ch)
{
    if (buffer->cur_ind == buffer->text_len)
    {
        memset(buffer->text, ' ', buffer->text_len);
        buffer->cur_ind = 0;
    }
    buffer->text[buffer->cur_ind] = ch;
    buffer->cur_ind++;
}

void del_char(Buffer *buffer)
{
    if (buffer->cur_ind > 0)
    {
        --buffer->cur_ind;
        buffer->text[buffer->cur_ind] = ' ';
    }
}

MDS_ERROR ObjectInitProc(void *obj, va_list *args)
// (void *obj, int x, int y)
{
    Object *object = (Object *)obj;
    object->pos.x = va_arg(*args, int);
    object->pos.y = va_arg(*args, int);
    object->size.x = 1;
    object->size.y = 1;
    object->active = 0;
    object->alive = 1;
    return MDS_ERROR_OK;
}

MDS_ERROR ClockInitProc(void *obj, va_list *args)
// (void *obj, int x, int y)
{
    ObjectInitProc(obj, args);
    ((Clock *)obj)->size.x = sizeof("[00:00:00]") - 1;
    ((Clock *)obj)->size.y = 1;
    return MDS_ERROR_OK;
}

MDS_ERROR BufferInitProc(void *obj, va_list *args)
// (void *obj, int x, int y, int w)
{
    Buffer *buffer = (Buffer *)obj;
    ObjectInitProc(obj, args);

    buffer->size.x = va_arg(*args, int);
    buffer->text_len = buffer->size.x - 2;
    buffer->text = (char *)malloc(buffer->text_len + 1);
    if (buffer->text == NULL)
        return MDS_ERROR_MEMORY;
    buffer->text[buffer->text_len] = '\0';
    memset(buffer->text, ' ', buffer->text_len);
    buffer->cur_ind = 0;
    return MDS_ERROR_OK;
}

MDS_ERROR ButtonInitProc(void *obj, va_list *args)
// (void *obj, int x, int y, const char *caption, char void *obj, int message, MDS_DATA extra_data)
{
    Button *button = (Button *)obj;
    ObjectInitProc(obj, args);
    button->caption = strdup(va_arg(*args, char *));
    button->size.x = strlen(button->caption) + 4; // included spaces and brackets
    button->size.y = 3; // 3 lines
    button->msg.receiver = va_arg(*args, void *);
    button->msg.message = va_arg(*args, int);
    button->msg.extra_data = va_arg(*args, MDS_DATA);
    return MDS_ERROR_OK;
}

MDS_ERROR StopwatchInitProc(void *obj, va_list *args)
// (void *obj, int x, int y)
{
    Stopwatch *stopwatch = (Stopwatch *)obj;
    ObjectInitProc(obj, args);
    stopwatch->size.x = sizeof("00:00:00") + 1; // included brackets
    stopwatch->size.y = 1;

    stopwatch->msec = 0;
    stopwatch->running = 0;
    stopwatch->msg_id = NULL_DATA;
    return MDS_ERROR_OK;
}

void ObjectFinalProc(void *obj)
{
    MDS_DATA data;
    // delete from screen
    data.integer = -1;
    MDS_sendMessage(obj, MESSAGE_RENDER, data);
    // set flag
    ((Object *)obj)->alive = 0;
    // change current object
    MDS_postMessage(NULL, MESSAGE_CHANGE_ACTIVE, NULL_DATA, 0);
}

void ClockFinalProc(void *obj)
{
    ObjectFinalProc(obj);
}

void BufferFinalProc(void *obj)
{
    free(((Buffer *)obj)->text);
    ObjectFinalProc(obj);
}

void ButtonFinalProc(void *obj)
{
    free(((Button *)obj)->caption);
    ObjectFinalProc(obj);
}

void StopwatchFinalProc(void *obj)
{
    ObjectFinalProc(obj);
}

MDS_DATA ClockMsgProc(void *obj, int message, MDS_DATA extra_data)
{
    Clock *clock = (Clock *)obj;
    if (!clock->alive)
        return NULL_DATA;
    switch (message)
    {
    case MESSAGE_RENDER:
        // single rendering if extra_data == 0
        // erasing rendering if extra_data == -1

        con_gotoXY(clock->pos.x, clock->pos.y);
        if (extra_data.integer == 0)
        {
            time_t curr_time;
            struct tm * curr_time_tm;
            char *buffer = (char *)malloc(clock->size.x + 1);
            if (buffer == NULL)
                break;

            time(&curr_time);
            curr_time_tm = localtime(&curr_time);
            strftime(buffer, clock->size.x + 1, "%H:%M:%S", curr_time_tm);

            if (clock->active)
                con_outTxt("<%*s>", clock->size.x-2, buffer);
            else
                con_outTxt("[%*s]", clock->size.x-2, buffer);
            free(buffer);
        }
        else
            con_outTxt("%*s", clock->size.x, "");
        break;
    case MESSAGE_MOVE:
        move_object((Object *)clock, extra_data.integer);
        break;
    case MDS_MESSAGE_KEY_PRESSED:
        process_keys((Object *)clock, extra_data.integer);
        break;
    case MESSAGE_FOCUS:
        set_activity((Object *)clock, extra_data.integer);
        break;
    }
    return NULL_DATA;
}

MDS_DATA BufferMsgProc(void *obj, int message, MDS_DATA extra_data)
{
    Buffer *buffer = (Buffer *)obj;
    if (!buffer->alive)
        return NULL_DATA;
    switch (message)
    {
    case MESSAGE_RENDER:
        // single rendering if extra_data == 0
        // erasing rendering if extra_data == -1
        con_gotoXY(buffer->pos.x, buffer->pos.y);
        if (extra_data.integer == -1)
        {
            con_outTxt("%*s", buffer->size.x, "");
        }
        else
        {
            if (buffer->active)
                con_outTxt("<%*s>", buffer->text_len, buffer->text);
            else
                con_outTxt("[%*s]", buffer->text_len, buffer->text);
        }
        break;
    case MESSAGE_MOVE:
        move_object((Object *)buffer, extra_data.integer);
        break;
    case MESSAGE_FOCUS:
        set_activity((Object *)buffer, extra_data.integer);
        break;
    case MDS_MESSAGE_KEY_PRESSED:
        if (!process_keys((Object *)buffer, extra_data.integer))
        {
            if (extra_data.integer >= 32 && extra_data.integer <= 255)
                add_char(buffer, extra_data.integer);
            else if (extra_data.integer == CON_KEY_BACKSPACE)
                del_char(buffer);
        }
        break;
    }
    return NULL_DATA;
}

MDS_DATA ButtonMsgProc(void *obj, int message, MDS_DATA extra_data)
{
    Button *button = (Button *)obj;
    if (!button->alive)
        return NULL_DATA;
    switch (message)
    {
    case MESSAGE_RENDER:
        // single rendering if extra_data == 0
        // erasing rendering if extra_data == -1
        if (extra_data.integer == -1)
        {
            int i;
            for (i = 0; i < button->size.y; ++i)
            {
                con_gotoXY(button->pos.x, button->pos.y + i);
                con_outTxt("%*s", button->size.x, "");
            }
        }
        else
        {
            char borders[5];
            char *buf = (char *)malloc(button->size.x-2 + 1);
            if (buf == NULL)
                return NULL_DATA;

            // border format:
            // 02222221
            // 3 text 3
            // 12222220
            if (button->active)
                strcpy(borders, "  --");
            else
                strcpy(borders, "/\\-|");

            memset(buf, borders[2], button->size.x-2);
            buf[button->size.x-2] = '\0';

            con_gotoXY(button->pos.x, button->pos.y);
            con_outTxt("%c%s%c", borders[0], buf, borders[1]);

            con_gotoXY(button->pos.x, button->pos.y + 1);
            con_outTxt("%c %s %c", borders[3], button->caption, borders[3]);

            con_gotoXY(button->pos.x, button->pos.y + 2);
            con_outTxt("%c%s%c", borders[1], buf, borders[0]);

            free(buf);
        }
        break;
    case MESSAGE_MOVE:
        move_object((Object *)button, extra_data.integer);
        break;
    case MESSAGE_FOCUS:
        set_activity((Object *)button, extra_data.integer);
        break;
    case MDS_MESSAGE_KEY_PRESSED:
        if (!process_keys((Object *)button, extra_data.integer))
            if (extra_data.integer == ' ')
                MDS_postMessage(button->msg.receiver,  button->msg.message, button->msg.extra_data, 0);
        break;
    }
    return NULL_DATA;
}

MDS_DATA StopwatchMsgProc(void *obj, int message, MDS_DATA extra_data)
{
    Stopwatch *stopwatch = (Stopwatch *)obj;
    if (!stopwatch->alive)
        return NULL_DATA;
    switch (message)
    {
    case MESSAGE_RENDER:
        // single rendering if extra_data == 0
        // erasing rendering if extra_data == -1

        con_gotoXY(stopwatch->pos.x, stopwatch->pos.y);
        if (extra_data.integer == 0)
        {
            char *buffer = (char *)malloc(stopwatch->size.x + 1);
            int s = stopwatch->msec / 1000;
            if (buffer == NULL)
                break;


            sprintf(buffer, "%02i:%02i:%02i", s / 3600, (s % 3600) / 60, s % 60);

            if (stopwatch->active)
                con_outTxt("<%*s>", stopwatch->size.x-2, buffer);
            else
                con_outTxt("[%*s]", stopwatch->size.x-2, buffer);
            free(buffer);
        }
        else
            con_outTxt("%*s", stopwatch->size.x, "");
        break;
    case MESSAGE_MOVE:
        move_object((Object *)stopwatch, extra_data.integer);
        break;
    case MDS_MESSAGE_KEY_PRESSED:
        process_keys((Object *)stopwatch, extra_data.integer);
        break;
    case MESSAGE_FOCUS:
        set_activity((Object *)stopwatch, extra_data.integer);
        break;
    case MESSAGE_STOPWATCH_START:
        stopwatch->running = !stopwatch->running;
        if (stopwatch->running)
        {
            ++(stopwatch->msg_id.integer);
            MDS_postMessage(stopwatch, MESSAGE_STOPWATCH_INC, stopwatch->msg_id, STOPWATCH_INC);
        }
        break;
    case MESSAGE_STOPWATCH_INC:
        if (extra_data.integer == stopwatch->msg_id.integer)
        {
            stopwatch->msec += STOPWATCH_INC;
            if (stopwatch->running)
                MDS_postMessage(stopwatch, MESSAGE_STOPWATCH_INC, stopwatch->msg_id, STOPWATCH_INC);
        }
        break;
    }
    return NULL_DATA;
}

int main()
{
    MDS_ObjectType obj_type;
    MDS_DATA data;
    int i = 0;

#if (!NDEBUG && __GLIBC__)
    mtrace();
#endif

    MDS_init();

    INIT_OBJECT_TYPE(obj_type, Clock);
    MDS_registerObjectType(&obj_type);
    INIT_OBJECT_TYPE(obj_type, Buffer);
    MDS_registerObjectType(&obj_type);
    INIT_OBJECT_TYPE(obj_type, Button);
    MDS_registerObjectType(&obj_type);
    INIT_OBJECT_TYPE(obj_type, Stopwatch);
    MDS_registerObjectType(&obj_type);

    active_index = 0;

    // 2 buffers
    for (i = 0; i < 2; ++i)
    {
        MDS_createObject("Buffer", (void **)&objects[i]);
        MDS_initObject(objects[i], 0, i, (i+1)*10);
    }

    // 2 clocks
    for (i = 2; i < 4; ++i)
    {
        MDS_createObject("Clock", (void **)&objects[i]);
        MDS_initObject(objects[i], 0, i);
    }

    // Stopwatch
    MDS_createObject("Stopwatch", (void **)&objects[i]);
    MDS_initObject(objects[i], 0, i);
    ++i;

    MDS_createObject("Button", (void **)&objects[i]);
    MDS_initObject(objects[i], 10, i+3, "stopwatch", objects[i-1], MESSAGE_STOPWATCH_START, NULL_DATA);
    ++i;

    MDS_createObject("Button", (void **)&objects[i]);
    MDS_initObject(objects[i], 40, 0, "EXIT", NULL, MDS_MESSAGE_KEY_PRESSED, CON_KEY_ESCAPE);
    ++i;

    data.integer = 1;
    MDS_sendMessage(objects[0], MESSAGE_FOCUS, data);

    MDS_mainLoop(def_msg_proc);

    for (i = 0; i < OBJ_NUM; ++i)
        MDS_deleteObject(objects[i]);
    MDS_cleanUp();

#if (!NDEBUG && WIN32)
    _CrtDumpMemoryLeaks();
#endif

    return 0;
}
