#include "stdafx.h"
#include  "game.h"

#define init_img_list(type)\
    images[type] = ImageList_LoadBitmap(App::GetInstance(), MAKEINTRESOURCE(IDB_##type),32, 0, RGB(255, 0, 255))

Game::Game(FILE *file, unsigned tick_ms, int cmdShow) :
    map(file, SCREEN_MAP_X, SCREEN_MAP_Y, this),
    timer(tick_ms),
    loop(cmdShow, this),
    score_label(SCREEN_LABELS_X, SCREEN_LABELS_Y, SCREEN_LABELS_WIDTH, "Score"),
    energizer_label(SCREEN_LABELS_X, SCREEN_LABELS_Y + 3, SCREEN_LABELS_WIDTH, "Energizer")
{
    srand( (unsigned)time( NULL ) );

    init_img_list(PACMAN);
    init_img_list(GHOST);
    init_img_list(FOOD);
    init_img_list(ENERGIZER);
    init_img_list(EMPTY);
    init_img_list(WALL);
    init_img_list(DEAD);

    map.setLoop( &loop );
    map.set_cells_loop();

    pacman.setLoop( &loop );
    pacman.set_position( map.init_pos_pacman() );

    timer.setLoop( &loop );
    timer.add_object( &map, 1 );
    timer.add_object( &pacman, MD_PACMAN );
    for( unsigned i = 0 ; i < GHOST_NUMBER ; ++i )
    {
        ghosts[i].setLoop( &loop );
        ghosts[i].set_position( map.init_pos_ghost(i) );
        timer.add_object( &ghosts[i], MD_GHOST );
    }

    pacman.get_position()->start_wave();
}

HIMAGELIST Game::get_image_of(ObjType ot)
{
    return images[ot];
}

Game::~Game()
{
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

void Game::draw_map(XPaintDC &dc)
{
    map.draw(dc);
}

void Game::draw_heroes()
{
    loop.postMessage(&pacman, MSG_DRAW, Data());
    for( unsigned i = 0 ; i < GHOST_NUMBER ; ++i )
    {
        loop.postMessage(&ghosts[i], MSG_DRAW, Data());
    }
}

GameStats Game::start()
{
    score_label.draw(true);
    energizer_label.draw(false);

    loop.run();

    //map.clear();
    score_label.draw(false);
    energizer_label.draw(false);

    GameStats stats = { loop.get_result(), pacman.get_score() };
    return stats;
}

Label::Digital * Game::get_score_label()
{
    return &score_label;
}

Label::Bar * Game::get_energizer_label()
{
    return &energizer_label;
}

