#pragma once

#include  "oopacman.h"

class Game; // predefined

class GameLoop : public MessageLoop
{
protected:
    Game * game;
    unsigned energizer_ticks_left;
    GameResult game_result;

    virtual void beforeRun();

    // FOR ALL: if true leave run()
    virtual bool processMessage(int message, Data extra_data);
    virtual bool processKey(int ch);
    virtual bool processIdle();

    // functions for working with UI
    virtual int getKey();
    virtual int keyPressed();
public:
    GameLoop(Game *game);
    GameResult get_result();
};

#include  "game.h"  // GameLoop must be defined earlier then Game

