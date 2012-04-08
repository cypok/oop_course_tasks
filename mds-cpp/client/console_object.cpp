#include "console_object.h"

Coord::Coord(int x, int y) : x(x), y(y) {}

Rect::Rect(Coord pos, Coord size)
{
    x1 = pos.x;
    y1 = pos.y;
    x2 = x1 + size.x - 1;
    y2 = y1 + size.y - 1;
}

bool Rect::overlapped_with(Rect rect)
{
    return !( y2 < rect.y1 ||
              x1 > rect.x2 ||
              y1 > rect.y2 ||
              x2 < rect.x1 );
}

ConsoleObject::ConsoleObject(Coord pos, Coord size) : Object(), pos(pos), size(size)
{
    active = 0;
    alive = 1;
}

bool ConsoleObject::is_alive() const
{
    return alive;
}

void ConsoleObject::move(DIRECTION dir)
{
    int x, y;
    Coord old_pos = pos;
    Coord old_size = size;
    con_getMaxXY(&x, &y);
    if (dir == DIR_UP && pos.y > 0)
        --pos.y;
    else if (dir == DIR_RIGHT && (pos.x + size.x) <= x)
        ++pos.x;
    else if (dir == DIR_DOWN && (pos.y + size.y) <= y)
        ++pos.y;
    else if (dir == DIR_LEFT && pos.x > 0)
        --pos.x;
    else
        return;
    Data data(new Rect(old_pos, old_size));
    getLoop()->postMessage(NULL, MESSAGE_MOVED, data);
}

void ConsoleObject::focus(bool is_active)
{
    if (alive)
        active = is_active;
}

bool ConsoleObject::processKey(int ch)
{
    // return true if key processed
    DIRECTION dir;

    switch (ch)
    {
    case CON_KEY_LEFT:
        dir = DIR_LEFT;
        break;
    case CON_KEY_DOWN:
        dir = DIR_DOWN;
        break;
    case CON_KEY_RIGHT:
        dir = DIR_RIGHT;
        break;
    case CON_KEY_UP:
        dir = DIR_UP;
        break;
    default:
        return false;
    }

    render(-1); // erase
    move(dir);
    render(0); // normal render

    return true;
}

bool ConsoleObject::processMessage(int message, Data extra_data)
{
    if (!alive)
        return false;

    switch (message)
    {
    case MESSAGE_RENDER:
        render(extra_data.integer());
        break;
    case MESSAGE_FOCUS:
        focus(extra_data.integer() != 0);
        render(0);
        break;
    }
    return false;
}

void ConsoleObject::rendered()
{
    con_gotoXY(0, 0);
    Data data(this);
    getLoop()->postMessage(NULL, MESSAGE_RENDERED, data);
}

Rect ConsoleObject::get_rect() const
{
    return Rect(pos, size);
}
