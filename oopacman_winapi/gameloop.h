#pragma once

#include  "oopacman.h"

class Game; // predefined

class GameLoop;

class GameWindow : public Window
{
protected:
    virtual void OnPaint(XPaintDC & dc);

    virtual bool OnCommand(WORD wEvent, WORD wCmdID, HWND hwndControl);

};

class GameLoop
    : public App
{
public:
    GameLoop(int cmdShow, Game *game);
    GameResult get_result();
    Game * get_game() { return game; }

protected:
    Game * game;
    unsigned energizer_ticks_left;
    GameResult game_result;

    virtual void beforeRun();

    virtual int InitInstance();

    virtual int ExitInstance(int exitCode)
    {
        return App::ExitInstance(exitCode);
    }

    // FOR ALL: if true leave run()
    virtual bool processMessage(int message, Data extra_data);
    virtual bool processKey(int ch);
    virtual bool processIdle();
    // functions for working with UI
    //   for backward compatibility
    virtual int getKey()
    {
        return 0;
    }
    virtual int keyPressed()
    {
        return 0;
    }
private:
    GameWindow window;
    int _cmdShow;
};
#include  "game.h"  // GameLoop must be defined earlier then Game

