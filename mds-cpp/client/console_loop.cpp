#include "console_loop.h"

ConsoleLoop::ConsoleLoop(ConsoleObject *objs[], size_t n)
{
    objs_num = (n < MAX_OBJECTS_NUM) ? n : MAX_OBJECTS_NUM;

    if (objs_num != 0)
    {
        for (size_t i = 0; i < objs_num; ++i)
        {
            this->objs[i] = objs[i];
            this->objs[i]->setLoop(this);
        }

        active_index = 0;
    }
    else
    {
        active_index = -1;
    }
}

void ConsoleLoop::beforeRun()
{
    if (active_index >= 0)
    {
        for(unsigned i = 0; i < objs_num; ++i)
            postMessage(objs[i], MESSAGE_RENDER, Data(0));
        postMessage(objs[active_index], MESSAGE_FOCUS, Data(1));
    }
}

int ConsoleLoop::getKey()
{
    return con_getKey();
}

int ConsoleLoop::keyPressed()
{
    return con_keyPressed();
}

bool ConsoleLoop::processMessage(int message, Data extra_data)
{
    switch (message)
    {
    case MESSAGE_MOVED:
        if (active_index >= 0)
        {
            Rect *rect = (Rect *)(extra_data.pointer());
            for(unsigned i = 0; i < objs_num; ++i)
            {
                ConsoleObject* obj = objs[i];
                if (obj->is_alive() &&
                    rect->overlapped_with(obj->get_rect())
                )
                    postMessage(obj, MESSAGE_RENDER, Data(0));
            }
            delete[] rect;
        }
        break;
    case MESSAGE_RENDERED:
        if (active_index >= 0)
        {
            // render active object if it was overlapped by other
            ConsoleObject* rendered = (ConsoleObject *)extra_data.pointer();
            ConsoleObject* active = objs[active_index];
            if (rendered != active &&
                active->get_rect().overlapped_with(rendered->get_rect())
            )
                postMessage(active, MESSAGE_RENDER, Data(0));
        }
        break;
    case MESSAGE_CHANGE_ACTIVE:
        if (active_index >= 0)
        {
            int previous = active_index;

            postMessage(objs[previous], MESSAGE_FOCUS, Data(0));

            do
                active_index = (active_index + 1) % int(objs_num);
            while (!objs[active_index]->is_alive() && previous != active_index);

            if (previous == active_index && !objs[active_index]->is_alive())
                active_index = -1;
            else
                postMessage(objs[active_index], MESSAGE_FOCUS, Data(1));
        }
        break;
    }
    return false; // no loop breaking
}

bool ConsoleLoop::processKey(int ch)
{
    switch (ch)
    {
    case CON_KEY_ESCAPE:
        return true; // stop loop

    case '\t':
        postMessage(NULL, MESSAGE_CHANGE_ACTIVE, Data());
        break;

    default:
        if (active_index >= 0)
            objs[active_index]->processKey(ch);
        break;
    }
    return false; // no loop breaking
}

bool ConsoleLoop::processIdle()
{
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;;  ;;;;;;;;;  ;;;;       ;;;;;;;        ;;;
    ;;  ;;;;;;;;;  ;;;;  ;;;;  ;;;;;  ;;;;;;  ;;
    ;;    ;;;;;    ;;;;  ;;;;;  ;;;;  ;;;;;;;;;;
    ;;  ;  ;;;  ;  ;;;;  ;;;;;  ;;;;;  ;;;;;;;;;
    ;;  ;;  ;  ;;  ;;;;  ;;;;;  ;;;;;;      ;;;;
    ;;  ;;;   ;;;  ;;;;  ;;;;;  ;;;;;;;;;;;   ;;
    ;;  ;;;;;;;;;  ;;;;  ;;;;;  ;;;;;;;;;;;;  ;;
    ;;  ;;;;;;;;;  ;;;;  ;;;;  ;;;;;  ;;;;;  ;;;
    ;;  ;;;;;;;;;  ;;;;       ;;;;;;;       ;;;;
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;;;;;;;;;;;;;; return false ;;;;;;;;;;;;;;;;
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
}
