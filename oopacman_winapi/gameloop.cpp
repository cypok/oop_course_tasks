#include "stdafx.h"
#include "gameloop.h"

static HIMAGELIST img_pacman;

GameLoop::GameLoop(int cmdShow, Game *game): App(), game(game), _cmdShow(cmdShow)
{
    energizer_ticks_left = 0;
    game_result = GR_NONE;
}

void GameLoop::beforeRun()
{
    //game->draw_map();
    //game->draw_heroes();
    //postMessage(game->get_timer(), MSG_TICK, Data());

}

// FOR ALL: if true leave run()
bool GameLoop::processMessage(int message, Data extra_data)
{
    switch(message)
    {
        case MSG_STOP_GAME:
            switch ( extra_data.integer() )
            {
                case STOP_FOOD_EATEN:
                    game_result = GR_WINNER;
                    break;
                case STOP_PACMAN_DIED :
                    game_result = GR_LOSER;
                    break;
            }
            return true;
        case MSG_START_GAME:
            postMessage(game->get_pacman(), MSG_START_GAME, Data());
            for( unsigned i = 0 ; i < GHOST_NUMBER ; ++i )
                postMessage(game->get_ghost(i), MSG_START_GAME, Data());
            break;
        case MSG_ENERGIZER:
            if (extra_data.integer() == ENERGIZER_ON)
            {
                energizer_ticks_left = ENERGIZE_DURATION;
                game->get_energizer_label()->set_percentage(100);
                SetWindowText(*GetMainWindow(), _T("oopacman | 100"));
                game->get_energizer_label()->draw();
                game->get_timer()->change_period(game->get_pacman(), MD_PACMAN_ENERGIZED);
            }
            else
            {
                game->get_energizer_label()->draw(false);
                SetWindowText(*GetMainWindow(), _T("oopacman"));
                game->get_timer()->change_period(game->get_pacman(), MD_PACMAN);
            }

            postMessage(game->get_pacman(), MSG_ENERGIZER, extra_data);
            for( unsigned i = 0 ; i < GHOST_NUMBER ; ++i )
                postMessage(game->get_ghost(i), MSG_ENERGIZER, extra_data);
            break;
        case MSG_TICK:
            assert(energizer_ticks_left >= 0);

            if (energizer_ticks_left > 0)
            {
                --energizer_ticks_left;
                game->get_energizer_label()->set_percentage(100 * energizer_ticks_left/ENERGIZE_DURATION);

                static wchar_t buffer[15];
                wsprintf(buffer, _T("oopacman | %3i"), 100 * energizer_ticks_left/ENERGIZE_DURATION );
                SetWindowText(*GetMainWindow(), buffer);
                if ((energizer_ticks_left) == 0)
                    postMessage(NULL, MSG_ENERGIZER, Data(ENERGIZER_OFF));
            }
            break;
        case MSG_SCORE_UPD:
            game->get_score_label()->set_value(extra_data.integer());
            break;
    }
    return false;
}

bool GameLoop::processKey(int ch)
{
    static bool timer_started = false;
    if (!timer_started)
    {
        postMessage(game->get_timer(), MSG_TICK, Data());
        timer_started = true;
    }
    if (ch == VK_ESCAPE)
    {
        game_result = GR_NONE;
        return true;
    }
    game->get_pacman()->process_key(ch);
    return false;
}

bool GameLoop::processIdle()
{
    return false;
}


GameResult GameLoop::get_result()
{
    return game_result;
}

int GameLoop::InitInstance()
{
    int code = App::InitInstance();

    InitCommonControls();

    //img_pacman = ImageList_LoadBitmap(App::GetInstance(), MAKEINTRESOURCE(IDB_BITMAP1),32, 0, RGB(255, 0, 255));
    XString title;
    title.LoadFromResource(IDS_APP_TITLE);

    XString wclass;
    wclass.LoadFromResource(IDC_OOPACMAN);

    RECT rect = game->get_map()->get_rect();

    DWORD style = WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME;

    AdjustWindowRect(&rect, style, FALSE);

    if (!window.CreateWnd(title,
        style,
        CW_USEDEFAULT, 0,
        rect.right-rect.left, rect.bottom-rect.top,
        NULL, LPCTSTR(NULL))
       )
        return 1;

    SetMainWindow(&window);

    window.ShowWindow(_cmdShow);
    window.UpdateWindow();
    return code;
}

void GameWindow::OnPaint(XPaintDC & dc)
{
    //static bool x = false;
    //HBRUSH hbr = CreateSolidBrush(x ? RGB(255, 0, 128) : RGB(255, 128, 0));
    //x = !x;

    //dc.FillRect(dc.GetPaintStruct().rcPaint, hbr);

    //DeleteObject(hbr);
    ((GameLoop*)App::GetApplication())->get_game()->draw_map(dc);
}

bool GameWindow::OnCommand(WORD wEvent, WORD wCmdID, HWND hwndControl)
{
    UNREFERENCED_PARAMETER(wEvent);
    UNREFERENCED_PARAMETER(hwndControl);

    switch (wCmdID)
    {
    case IDM_EXIT:
        this->DestroyWindow();
        return true;
    case ID_START:
        ((App*)App::GetApplication())->postMessage(((GameLoop*)App::GetApplication())->get_game()->get_timer(), MSG_TICK, Data());
        return true;
    }
    return false;
}
