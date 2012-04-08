#include "hero.h"

void Hero::tick()
{
    assert(tick_counter < move_delay);
    ++tick_counter;
    if (tick_counter == move_delay)
        tick_counter = 0;
}

Hero::Hero()
{
    position = NULL;
    is_moving = false;
    alive = true;

    move_delay = 0;
    tick_counter = 0;
    dir = UP;
    next_dir = dir;
}

void Hero::set_position(Cell *new_position)
{
    position = new_position;
}

Cell * Hero::get_position()
{
    return position;
}

void Hero::set_move_delay(unsigned ticks)
{
    move_delay = ticks;
}

bool Hero::can_move(Direction dir)
{
    return position->can_move(dir);
}

bool Hero::is_alive()
{
    return alive;
}

bool Hero::processMessage(int message, Data extra_data)
{
    switch ( message )
    {
        case MSG_DRAW :
            {
                int state = 0;
                switch ( next_dir )
                {
                    case UP :       state |= HERO_UP;    break;
                    case RIGHT :    state |= HERO_RIGHT; break;
                    case DOWN :     state |= HERO_DOWN;  break;
                    case LEFT :     state |= HERO_LEFT;  break;
                }
                if (aggressive)
                    state |= HERO_AGGRESSIVE;
                if (alive)
                    state |= HERO_ALIVE;

                getLoop()->postMessage(position, MSG_DRAW, Data(state));
            }
            break;
        case MSG_TICK :
            if (!alive)
                break;

            tick();
            tick_action();
            if (tick_counter == 0)
            {
                if( can_move(next_dir) )
                    dir = next_dir;

                if( is_moving && can_move(dir))
                {
                    // clear previous cell
                    getLoop()->postMessage(position, MSG_DRAW, Data(0));
                    // move
                    position = position->neighbor(dir);
                    // draw
                    getLoop()->sendMessage(this, MSG_DRAW, Data());
                }

                move_action();
            }
            break;
        case MSG_START_GAME :
            alive = true;
            is_moving = true;
            break;
        case MSG_ENERGIZER :
            if (!alive)
                break;

            energize_action(extra_data.integer());
            break;
    }
    return false;
}

PacMan::PacMan()
{
    score = 0;
    aggressive = false;
}

void PacMan::tick_action()
{
}

void PacMan::inc_score(unsigned i)
{
    score += i;
    getLoop()->postMessage(NULL, MSG_SCORE_UPD, Data(score));
}

void PacMan::move_action()
{
    switch ( position->pop_content() )
    {
        case POINT :
            inc_score(SCORE_POINT);
            break;
        case ENERGIZER :
            inc_score(SCORE_ENERGIZER);
            getLoop()->postMessage(NULL, MSG_ENERGIZER, Data(ENERGIZER_ON));
            break;
    }
    position->start_wave();
}

void PacMan::energize_action(int flag)
{
    aggressive = (flag == ENERGIZER_ON);
    move_delay = aggressive ? MD_PACMAN_ENERGIZED : MD_PACMAN;
}

void PacMan::process_key(int ch)
{
    Direction prev = next_dir;
    switch ( ch )
    {
        case CON_KEY_UP:
            next_dir = UP;
            break;
        case CON_KEY_RIGHT:
            next_dir = RIGHT;
            break;
        case CON_KEY_DOWN:
            next_dir = DOWN;
            break;
        case CON_KEY_LEFT:
            next_dir = LEFT;
            break;
        default :
            break;
    }
    if (prev != next_dir)
        getLoop()->sendMessage(this, MSG_DRAW, Data());
    if (!is_moving)
        getLoop()->postMessage(NULL, MSG_START_GAME, Data());
}

Ghost::Ghost()
{
    // init skill, aggr, ...
    aggressive = true;
}

bool Ghost::can_move(Direction dir)
{
    return Hero::can_move(dir) && !position->neighbor(dir)->is_here(GHOST);
}

void Ghost::check_meeting()
{
    if (position->wave.depth == 0)
    {
        if (aggressive)
            getLoop()->postMessage(NULL, MSG_STOP_GAME, Data(STOP_PACMAN_DIED));
        else
            alive = false;
    }
}

inline unsigned min(unsigned a, unsigned b) { return (a < b) ? a : b; }
inline unsigned max(unsigned a, unsigned b) { return ((a+1) > (b+1)) ? a : b; } // "+1" - to make max(2, INF) = 2
inline unsigned best(unsigned a, unsigned b, bool best_is_min)
{
    return (best_is_min) ? min(a,b) : max(a, b);
}


void Ghost::tick_action()
{
    if( tick_counter == move_delay - 1 )
    {
        unsigned best_depth = aggressive ? unsigned(-1) : 0;
        Direction best_dirs[4];
        unsigned best_dirs_num = 0;

        Cell *neighbor;
        for( unsigned dir = 0 ; dir < 4 ; ++dir )
        {
            neighbor = position->neighbor(Direction(dir));
            best_depth = best(best_depth, neighbor->wave.depth, aggressive);
        }
        for( unsigned dir = 0 ; dir < 4 ; ++dir )
        {
            neighbor = position->neighbor(Direction(dir));
            if (neighbor->wave.depth == best_depth &&
                !neighbor->is_wall()
               )
                best_dirs[best_dirs_num++] = Direction(dir);
        }

        // if he surrounded by walls or ghosts
        if(best_dirs_num == 0)
        {
            return;
        }
        next_dir = best_dirs[rand() % best_dirs_num];
    }
    check_meeting();
}

void Ghost::move_action()
{
    check_meeting();
}

void Ghost::energize_action(int flag)
{
    aggressive = (flag == ENERGIZER_OFF);
}

