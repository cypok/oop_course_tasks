// oopacman.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "gameloop.h"
#include "MainFrm.h"

#include "game.h"
#include "oopacmanView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// GameLoop

BEGIN_MESSAGE_MAP(GameLoop, CMFCMDApp)
    ON_COMMAND(ID_APP_ABOUT, &GameLoop::OnAppAbout)
END_MESSAGE_MAP()


// The one and only GameLoop object

GameLoop theApp;

// GameLoop construction

GameLoop::GameLoop()
{
    // TODO: add construction code here,
    // Place all significant initialization in InitInstance
}

// GameLoop initialization

BOOL GameLoop::InitInstance()
{
    CMFCMDApp::InitInstance();

    // Standard initialization
    // If you are not using these features and wish to reduce the size
    // of your final executable, you should remove from the following
    // the specific initialization routines you do not need
    // Change the registry key under which our settings are stored
    // TODO: You should modify this string to be something appropriate
    // such as the name of your company or organization
    SetRegistryKey(_T("Local AppWizard-Generated Applications"));
    LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)
    // Register the application's document templates.  Document templates
    //  serve as the connection between documents, frame windows and views
    CSingleDocTemplate* pDocTemplate;
    pDocTemplate = new CSingleDocTemplate(
        IDR_MAINFRAME,
        RUNTIME_CLASS(Game),
        RUNTIME_CLASS(CMainFrame),       // main SDI frame window
        RUNTIME_CLASS(COOPacManView));
    if (!pDocTemplate)
        return FALSE;
    AddDocTemplate(pDocTemplate);



    // Parse command line for standard shell commands, DDE, file open
    CCommandLineInfo cmdInfo;
    ParseCommandLine(cmdInfo);


    // Dispatch commands specified on the command line.  Will return FALSE if
    // app was launched with /RegServer, /Register, /Unregserver or /Unregister.
    if (!ProcessShellCommand(cmdInfo))
        return FALSE;

    // The one and only window has been initialized, so show and update it
    m_pMainWnd->ShowWindow(SW_SHOW);
    m_pMainWnd->UpdateWindow();
    // call DragAcceptFiles only if there's a suffix
    //  In an SDI app, this should occur after ProcessShellCommand
    return TRUE;
}



// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
    CAboutDlg();

// Dialog Data
    enum { IDD = IDD_ABOUTBOX };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// App command to run the dialog
void GameLoop::OnAppAbout()
{
    CAboutDlg aboutDlg;
    aboutDlg.DoModal();
}



void GameLoop::afterRun()
{
}

Game * GameLoop::get_game()
{
    //#error Get Game from the document
    return Game::GetGame();
}

// GameLoop message handlers

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
            postMessage(get_game()->get_pacman(), MSG_START_GAME, Data());
            for( unsigned i = 0 ; i < GHOST_NUMBER ; ++i )
                postMessage(get_game()->get_ghost(i), MSG_START_GAME, Data());
            break;
        case MSG_ENERGIZER:
            if (extra_data.integer() == ENERGIZER_ON)
            {
                energizer_ticks_left = ENERGIZE_DURATION;
                get_game()->get_energizer_label()->set_percentage(100);
                get_game()->get_energizer_label()->draw();
            }
            else
                get_game()->get_energizer_label()->draw(false);

            postMessage(get_game()->get_pacman(), MSG_ENERGIZER, extra_data);
            for( unsigned i = 0 ; i < GHOST_NUMBER ; ++i )
                postMessage(get_game()->get_ghost(i), MSG_ENERGIZER, extra_data);
            break;
        case MSG_TICK:
            assert(energizer_ticks_left >= 0);

            if (energizer_ticks_left > 0)
            {
                --energizer_ticks_left;
                get_game()->get_energizer_label()->set_percentage(100 * energizer_ticks_left/ENERGIZE_DURATION);
                if ((energizer_ticks_left) == 0)
                    postMessage(NULL, MSG_ENERGIZER, Data(ENERGIZER_OFF));
            }
            break;
        case MSG_SCORE_UPD:
            get_game()->get_score_label()->set_value(extra_data.integer());
            break;
    }
    return false;
}

bool GameLoop::processKey(int ch)
{
    static bool timer_started = false;
    if (!timer_started)
    {
        postMessage(get_game()->get_timer(), MSG_TICK, Data());
        timer_started = true;
    }
    if (ch == VK_ESCAPE)
    {
        game_result = GR_NONE;
        return true;
    }
    get_game()->get_pacman()->process_key(ch);
    return false;
}

bool GameLoop::processIdle()
{
    return false;
}
