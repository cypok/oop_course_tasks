// game.cpp : implementation of the Game class
//

#include "stdafx.h"

#include "game.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Game

IMPLEMENT_DYNCREATE(Game, CDocument)

BEGIN_MESSAGE_MAP(Game, CDocument)
END_MESSAGE_MAP()


// Game construction/destruction
#define init_img_list(type)\
    images[type] = ImageList_LoadBitmap(GameLoop::GetApp()->m_hInstance, MAKEINTRESOURCE(IDB_##type),32, 0, RGB(255, 0, 255))

Game::Game() :
    map(fopen("maps/classic.map", "r"), SCREEN_MAP_X, SCREEN_MAP_Y, this),
    timer(1000 / TICKS_PER_SEC),
    score_label(SCREEN_LABELS_X, SCREEN_LABELS_Y, SCREEN_LABELS_WIDTH, "Score"),
    energizer_label(SCREEN_LABELS_X, SCREEN_LABELS_Y + 3, SCREEN_LABELS_WIDTH, "Energizer")
{
    MessageLoop * loop = (MessageLoop *)GameLoop::GetApp();
    srand( (unsigned)time( NULL ) );

    init_img_list(PACMAN);
    init_img_list(GHOST);
    init_img_list(FOOD);
    init_img_list(ENERGIZER);
    init_img_list(EMPTY);
    init_img_list(WALL);
    init_img_list(DEAD);

    map.setLoop( loop );
    map.set_cells_loop();

    pacman.setLoop( loop );
    pacman.set_move_delay( MD_PACMAN );
    pacman.set_position( map.init_pos_pacman() );

    timer.setLoop( loop );
    timer.add_object( &map );
    timer.add_object( &pacman );
    for( unsigned i = 0 ; i < GHOST_NUMBER ; ++i )
    {
        ghosts[i].setLoop( loop );
        ghosts[i].set_move_delay( MD_GHOST );
        ghosts[i].set_position( map.init_pos_ghost(i) );
        timer.add_object( &ghosts[i] );
    }

    pacman.get_position()->start_wave();
}

Game::~Game()
{
}

BOOL Game::OnNewDocument()
{
    if (!CDocument::OnNewDocument())
        return FALSE;

    // TODO: add reinitialization code here
    // (SDI documents will reuse this document)

    return TRUE;
}




// Game serialization

void Game::Serialize(CArchive& ar)
{
    if (ar.IsStoring())
    {
        // TODO: add storing code here
    }
    else
    {
        // TODO: add loading code here
    }
}


// Game diagnostics

#ifdef _DEBUG
void Game::AssertValid() const
{
    CDocument::AssertValid();
}

void Game::Dump(CDumpContext& dc) const
{
    CDocument::Dump(dc);
}
#endif //_DEBUG

Game * Game::GetGame()
{
    CFrameWnd * pFrame = (CFrameWnd *)(AfxGetApp()->m_pMainWnd);
    return (Game *) pFrame->GetActiveDocument();
}

HIMAGELIST Game::get_image_of(ObjType ot)
{
    return images[ot];
}

PacMan * Game::get_pacman()
{
    return &pacman;
}

Ghost * Game::get_ghost(unsigned index)
{
    return &ghosts[index];
}

Timer * Game::get_timer()
{
    return &timer;
}

Map * Game::get_map()
{
    return &map;
}

void Game::draw_map(CDC &dc)
{
    map.draw(dc);
}

void Game::draw_heroes()
{
    GameLoop::GetApp()->postMessage(&pacman, MSG_DRAW, Data());
    for( unsigned i = 0 ; i < GHOST_NUMBER ; ++i )
    {
        GameLoop::GetApp()->postMessage(&ghosts[i], MSG_DRAW, Data());
    }
}

Label::Digital * Game::get_score_label()
{
    return &score_label;
}

Label::Bar * Game::get_energizer_label()
{
    return &energizer_label;
}
