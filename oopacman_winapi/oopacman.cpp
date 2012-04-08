// oopacman.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "oopacman.h"
#include "game.h"

#define MAX_LOADSTRING 100


// Global Variables:
HINSTANCE hInst;                                // current instance



int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hInstance);
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    {
        int tmpDbgFlag;

        tmpDbgFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
        tmpDbgFlag |= _CRTDBG_LEAK_CHECK_DF;
        _CrtSetDbgFlag(tmpDbgFlag);
    }
    FILE * file = fopen("maps/classic.map", "r");
    Game game(file, 1000 / TICKS_PER_SEC, nCmdShow);

    return game.start().score;

}

