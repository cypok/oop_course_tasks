#include "cell.h"

#define INITIAL_DEPTH   unsigned(-1)

class CellQueue
{
private:
    Cell *head, *tail;
public:
    CellQueue() : head(NULL), tail(NULL) {}

    void push(Cell *cell)
    {
        cell->wave.prev = tail;
        cell->wave.next = NULL;
        if(tail != NULL)
            tail->wave.next = cell;
        else
            head = cell;
        tail = cell;
    }

    Cell *pop()
    {
        assert(head != NULL);
        Cell *result = head;
        if(head == tail)
            tail = NULL;
        else
            head->wave.next->wave.prev = NULL;
        head = head->wave.next;
        return result;
    }

    bool empty()
    {
        return head == NULL;
    }
} cell_queue;

void WaveInfo::reset()
{
    depth = INITIAL_DEPTH;
}

WaveInfo::WaveInfo()
{
    reset();
}

void Cell::start_wave()
{
    map->reset_wave();

    this->wave.depth = 0;
    cell_queue.push(this);

    while( !cell_queue.empty() )
    {
        Cell *cur = cell_queue.pop();

        // try to push all neighbors
        for( unsigned dir = 0 ; dir < 4 ; ++dir )
        {
            Cell *neighbor = cur->neighbor(Direction(dir));

            // continue if it's wall or it's already in queue
            if( neighbor->is_wall() || neighbor->wave.depth != INITIAL_DEPTH)
                continue;

            neighbor->wave.depth = cur->wave.depth + 1;
            cell_queue.push(neighbor);
        }
    }
}

Cell::Cell( ObjType content,
            Cell * up, Cell * right, Cell * down, Cell *left,
            int x, int y, Map *map) :
                            content(content), map(map)
{
    neighbors[UP] = up;
    neighbors[RIGHT] = right;
    neighbors[DOWN] = down;
    neighbors[LEFT] = left;
    screen_position.x = x;
    screen_position.y = y;
}

bool Cell::is_wall()
{
    return content == WALL;
}

bool Cell::is_here(ObjType ot)
{
    return map->who_is_here(this) == ot;
}

bool Cell::can_move(Direction dir)
{
    return !neighbors[dir]->is_wall();
}

ObjType Cell::pop_content()
{
    ObjType ot = EMPTY;
    switch ( content )
    {
        case POINT :
            map->pop_point();
        case ENERGIZER :
            ot = content;
            content = EMPTY;
            break;
    }
    return ot;
}

void Cell::set_dead()
{
    content = DEAD;
}

Cell * Cell::neighbor(Direction dir)
{
    return neighbors[dir];
}

ObjType Cell::get_content()
{
    return content;
}

void draw_at(int x, int y, short color, int ch)
{
    con_gotoXY(x, y);
    con_setColor(color);
    con_outTxt("%c", ch);
}

void Cell::draw(int flags)
{
    int x = screen_position.x;
    int y = screen_position.y;

    if (flags & FLAG_ERASE)
    {
        draw_at(x, y, BACKGROUND_COLOR, ' ');
        draw_at(x+1, y, BACKGROUND_COLOR, ' ');
        return;
    }

    ObjType ot = map->who_is_here(this);
    if (ot == EMPTY)
        ot = content;
    int ch;
    switch ( ot )
    {
        case PACMAN :
            if ( !( (flags & FLAG_PACMAN) && (flags & FLAG_ALIVE) ) )
                break;

            assert(flags & (FLAG_UP | FLAG_RIGHT | FLAG_DOWN | FLAG_LEFT));

            if (flags & FLAG_UP)
                ch = '^';
            else if (flags & FLAG_RIGHT)
                ch = '>';
            else if (flags & FLAG_DOWN)
                ch = 'v';
            else if (flags & FLAG_LEFT)
                ch = '<';

            draw_at(x, y, short(EMPTY), ' ');
            draw_at(x + 1, y, short(PACMAN), ch);
            break;

        case GHOST :
            if ( !( (flags & FLAG_GHOST) && (flags & FLAG_ALIVE) ) )
                break;

            ch = '&';

            draw_at(x, y, short(EMPTY), ' ');
            draw_at(x + 1, y, short(GHOST), ch);
            break;

        case POINT :
            ch = '.';

            draw_at(x, y, short(EMPTY), ' ');
            draw_at(x + 1, y, short(POINT), ch);
            break;

        case ENERGIZER :
            ch = ENERGIZER_STATES[ (map->get_ticks() / DD_ENERGIZER) % ENERGIZER_STATES_NUM ];

            draw_at(x, y, short(EMPTY), ' ');
            draw_at(x + 1, y, short(ENERGIZER), ch);
            break;

        case EMPTY :
            ch = ' ';

            draw_at(x, y, short(EMPTY), ch);
            draw_at(x + 1, y, short(EMPTY), ch);
            break;

        case WALL :
            ch = ' ';

            draw_at(x, y, short(WALL), ch);
            draw_at(x + 1, y, short(WALL), ch);
            break;
        case DEAD:
            ch = 'X';

            draw_at(x, y, short(DEAD), ' ');
            draw_at(x + 1, y, short(DEAD), ch);
            break;
    }
}

bool Cell::processMessage(int message, Data extra_data)
{
    if( message == MSG_DRAW )
        draw(extra_data.integer());
    return false;
}

void * Cell::operator new(size_t size, void *pointer)
{
    return pointer;
}

#ifdef _MSC_VER
void Cell::operator delete(void *pointer, void *pointer2)
{
}
#endif

