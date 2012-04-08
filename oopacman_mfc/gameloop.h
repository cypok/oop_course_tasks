// gameloop.h : main header file for the oopacman application
//
#pragma once

#ifndef __AFXWIN_H__
    #error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include "settings.h"


// GameLoop:
// See oopacman.cpp for the implementation of this class
//

//enum
//{
//    MSG_DRAW = WM_MDS,
//    MSG_MUVIN_START,
//    MSG_MUVIN_MOVE,
//};

class Game; // predefined

class GameLoop : public CMFCMDApp
{
protected:
    unsigned energizer_ticks_left;
    GameResult game_result;

public:
    GameLoop();

    Game * GameLoop::get_game();
    GameResult get_result() { return game_result; }

// Overrides
public:
    virtual BOOL InitInstance();
    static GameLoop* GetApp() { return (GameLoop*)CMFCMDApp::GetApp();}
protected:
    virtual bool processMessage(int message, Data extra_data);
    virtual bool processIdle();
    virtual bool processKey(int ch);
    virtual void afterRun(); //!! Debug

// Implementation
    afx_msg void OnAppAbout();
    afx_msg void OnPost();
    DECLARE_MESSAGE_MAP()

};

extern GameLoop theApp;
