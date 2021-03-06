#include "stdafx.h"
#include "hero.h"

void Hero::tick()
{
    //assert(tick_counter < move_delay);
    //++tick_counter;
    //if (tick_counter == move_delay)
        //tick_counter = 0;
}

Hero::Hero(int flag_who)
{
    this->flag_who = flag_who;
    position = NULL;
    is_moving = false;
    alive = true;

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

Direction Hero::get_direction()
{
    return dir;
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
                    case UP :       state |= FLAG_UP;    break;
                    case RIGHT :    state |= FLAG_RIGHT; break;
                    case DOWN :     state |= FLAG_DOWN;  break;
                    case LEFT :     state |= FLAG_LEFT;  break;
                }
                if (aggressive)
                    state |= FLAG_AGGRESSIVE;
                if (alive)
                    state |= FLAG_ALIVE;
                state |= flag_who;


                position->redraw();
            }
            break;
        case MSG_TICK :
            if (!alive)
                break;

            tick();
            tick_action();
            if( can_move(next_dir) )
                dir = next_dir;

            if( is_moving && can_move(dir))
            {
                Cell *prev = position;
                // move
                position = position->neighbor(dir);
                // draw
                prev->redraw();
                position->redraw();
            }

            move_action();
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

PacMan::PacMan() : Hero(FLAG_PACMAN)
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

unsigned PacMan::get_score()
{
    return score;
}

void PacMan::move_action()
{
    switch ( position->pop_content() )
    {
        case FOOD :
            inc_score(SCORE_FOOD);
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
    //move_delay = aggressive ? MD_PACMAN_ENERGIZED : MD_PACMAN;
}

void PacMan::process_key(int ch)
{
    Direction prev = next_dir;
    switch ( ch )
    {
        case VK_UP:
            next_dir = UP;
            break;
        case VK_RIGHT:
            next_dir = RIGHT;
            break;
        case VK_DOWN:
            next_dir = DOWN;
            break;
        case VK_LEFT:
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

Ghost::Ghost() : Hero(FLAG_GHOST)
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
        {
            position->set_dead();
            alive = false;
        }
    }
}
#undef min
#undef max
inline unsigned min(unsigned a, unsigned b) { return (a < b) ? a : b; }
inline unsigned max(unsigned a, unsigned b) { return ((a+1) > (b+1)) ? a : b; } // "+1" - to make max(2, INF) = 2
inline unsigned best(unsigned a, unsigned b, bool best_is_min)
{
    return (best_is_min) ? min(a,b) : max(a, b);
}


void Ghost::tick_action()
{
    unsigned best_depth = aggressive ? unsigned(-1) : 0;
    Direction best_dirs[4];
    unsigned best_dirs_num = 0;

    Cell *neighbor;
    for( unsigned dir = 0 ; dir < 4 ; ++dir )
    {
        neighbor = position->neighbor(Direction(dir));
        if (can_move(Direction(dir)))
            best_depth = best(best_depth, neighbor->wave.depth, aggressive);
    }
    for( unsigned dir = 0 ; dir < 4 ; ++dir )
    {
        neighbor = position->neighbor(Direction(dir));
        if (neighbor->wave.depth == best_depth &&
            can_move(Direction(dir))
           )
            best_dirs[best_dirs_num++] = Direction(dir);
    }

    // if he surrounded by walls or ghosts
    if(best_dirs_num == 0)
    {
        return;
    }
    next_dir = best_dirs[rand() % best_dirs_num];
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

