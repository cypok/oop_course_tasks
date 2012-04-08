#include  "game.h"

#define INIT_COLOR(what) con_init_pair(int(what), what##_FG, what##_BG)

Game::Game(FILE *file, unsigned tick_ms) :
    map(file, SCREEN_MAP_X, SCREEN_MAP_Y, this),
    timer(tick_ms),
    loop(this),
    score_label(SCREEN_LABELS_X, SCREEN_LABELS_Y, SCREEN_LABELS_WIDTH, "Score"),
    energizer_label(SCREEN_LABELS_X, SCREEN_LABELS_Y + 3, SCREEN_LABELS_WIDTH, "Energizer")
{
    srand( (unsigned)time( NULL ) );

    con_init();
    INIT_COLOR(PACMAN);
    INIT_COLOR(GHOST);
    INIT_COLOR(POINT);
    INIT_COLOR(ENERGIZER);
    INIT_COLOR(EMPTY);
    INIT_COLOR(WALL);

    map.setLoop( &loop );
    map.set_cells_loop();

    pacman.setLoop( &loop );
    pacman.set_move_delay( MD_PACMAN );
    pacman.set_position( map.init_pos_pacman() );

    timer.setLoop( &loop );
    timer.add_object( &map );
    timer.add_object( &pacman );
    for( unsigned i = 0 ; i < GHOST_NUMBER ; ++i )
    {
        ghosts[i].setLoop( &loop );
        ghosts[i].set_move_delay( MD_GHOST );
        ghosts[i].set_position( map.init_pos_ghost(i) );
        timer.add_object( &ghosts[i] );
    }

    pacman.get_position()->start_wave();
}

Game::~Game()
{
    con_deinit();
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

void Game::draw_map()
{
    map.draw();
}

void Game::draw_heroes()
{
    loop.postMessage(&pacman, MSG_DRAW, Data());
    for( unsigned i = 0 ; i < GHOST_NUMBER ; ++i )
    {
        loop.postMessage(&ghosts[i], MSG_DRAW, Data());
    }
}

void Game::start()
{
    // work hard (set positions and etc.)
    loop.run();
}

Label::Digital * Game::get_score_label()
{
    return &score_label;
}

Label::Bar * Game::get_energizer_label()
{
    return &energizer_label;
}

