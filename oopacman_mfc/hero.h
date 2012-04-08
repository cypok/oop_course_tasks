#pragma once

#include "settings.h"

class Cell;

class Hero : public Object
{
protected:
    Cell *position;
    bool alive;
    bool aggressive;
    int flag_who;

    unsigned move_delay; // move happens after every `move_delay` timer ticks
    bool is_moving;
    virtual bool can_move(Direction dir);

    Direction dir;
    Direction next_dir;

    unsigned tick_counter;

    void tick();

    virtual void tick_action() = 0; // called after every timer tick
    virtual void move_action() = 0; // called after every move
    virtual void energize_action(int flag) = 0; // called on toggling energizer

public:
    Hero(int flag_who);
    void set_position(Cell *new_position);
    Cell * get_position();
    Direction get_direction();
    void set_move_delay(unsigned ticks);
    bool is_alive();

    virtual bool processMessage(int message, Data extra_data);
};

class PacMan : public Hero
{
protected:
    unsigned score;

    virtual void tick_action();
    virtual void move_action();
    virtual void energize_action(int flag);
public:
    PacMan();
    void inc_score(unsigned i);
    unsigned get_score();

    void process_key(int ch);
};

class Ghost : public Hero
{
protected:

    virtual bool can_move(Direction dir);
    void check_meeting(); // with pacman

    virtual void tick_action();
    virtual void move_action();
    virtual void energize_action(int flag);

public:
    Ghost();
};

#include "cell.h"

