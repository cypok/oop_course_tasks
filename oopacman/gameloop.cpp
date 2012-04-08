#include "gameloop.h"

GameLoop::GameLoop(Game *game): MessageLoop(), game(game)
{
    energizer_ticks_left = 0;
    game_result = GR_NONE;
}

void GameLoop::beforeRun()
{
    game->draw_map();
    game->draw_heroes();
    postMessage(game->get_timer(), MSG_TICK, Data());

}

// FOR ALL: if true leave run()
bool GameLoop::processMessage(int message, Data extra_data)
{
    switch(message)
    {
        case MSG_STOP_GAME:
            switch ( extra_data.integer() )
            {
                case STOP_POINTS_EATEN:
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
                game->get_energizer_label()->draw();
            }
            else
                game->get_energizer_label()->draw(false);

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
    if (ch == CON_KEY_ESCAPE)
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

int GameLoop::getKey()
{
    return con_getKey();
}

int GameLoop::keyPressed()
{
    return con_keyPressed();
}

GameResult GameLoop::get_result()
{
    return game_result;
}

