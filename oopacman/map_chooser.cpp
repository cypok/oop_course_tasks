#include  "map_chooser.h"

ERROR(NoMapsDir, "Cannot open maps directory (%s)")
ERROR(NoMaps, "No maps in maps directory (%s/)")

MapChooser::MapChooser(GameStats last)
{
    files = NULL;
    chosen = -1;
    counter = 0;

    // open directory with maps
    DIR *dir = opendir(MAPS_PATH);
    if (dir == NULL)
        throw NoMapsDirError(MAPS_PATH);

    // count files
    struct dirent *dp;
    while ((dp = readdir(dir)) != NULL)
        ++counter;

    // alloc memory for filenames
    files = new char *[counter];

    // copy all filenames of maps to array
    counter = 0;
    rewinddir(dir);
    while ((dp = readdir(dir)) != NULL)
    {
        size_t len = strlen(dp->d_name);
        // minimum length is length of MAPS_EXTENSION plus at least one char
        if ( len < (sizeof(MAPS_EXTENSION)-1) + 1 ||
                strcmp(dp->d_name + len - 4, MAPS_EXTENSION) != 0)
            continue;
        files[counter] = strdup(dp->d_name);
        counter++;
    }

    // finish
    closedir(dir);

    if (counter == 0)
        throw NoMapsError(MAPS_PATH);

    int max_x, max_y;
    con_getMaxXY(&max_x, &max_y);

    const char *text;
    switch ( last.result )
    {
        case GR_NONE :      text = MENU_H_WELCOME; break;
        case GR_WINNER :    text = MENU_H_WINNER;  break;
        case GR_LOSER :     text = MENU_H_LOSER;   break;
    }
    h_main = new Label::Label(
            0,
            SCREEN_WELCOME_Y,
            max_x,
            1,
            text
    );

    char score_buf[20];
    if (last.result != GR_NONE)
        sprintf(score_buf, "Score: %u", last.score);
    else
        score_buf[0] = '\0';
    h_score = new Label::Label(
            0,
            SCREEN_WELCOME_Y + 1,
            max_x,
            1,
            score_buf
    );

    h_level = new Label::Label(
            0,
            SCREEN_WELCOME_Y + 2,
            max_x,
            1,
            MENU_H_LEVEL
    );

    items = new Label::MenuItem * [counter];
    for( int i = 0 ; i < counter ; ++i )
    {
        char * name = strdup(files[i]);
        if (name == NULL)
            throw std::bad_alloc();
        name[ strlen(name) - (sizeof(MAPS_EXTENSION)-1) ] = '\0';

        items[i] = new Label::MenuItem(
                (max_x - SCREEN_MENU_WIDTH)/2,
                SCREEN_WELCOME_Y + 4 + i,
                SCREEN_MENU_WIDTH,
                name
        );

        free(name);
    }

}

MapChooser::~MapChooser()
{
    for( int i = 0; i < counter ; ++i )
    {
        delete items[i];
        free(files[i]);
    }

    delete h_main;
    delete h_score;
    delete h_level;
    delete[] items;
    delete[] files;
}

char * MapChooser::get()
{
    run();

    draw_self(false);

    if (chosen == -1)
        return NULL;

    assert(chosen >= 0);

    char * result = new char[sizeof(MAPS_PATH) + 1 + strlen(files[chosen])];
    sprintf(result, "%s/%s", MAPS_PATH, files[chosen]);
    return result;
}

void MapChooser::draw_self(bool normal)
{
    h_main->draw(normal);
    h_score->draw(normal);
    h_level->draw(normal);

    for( int i = 0 ; i < counter ; ++i )
        items[i]->draw(normal);
}

void MapChooser::beforeRun()
{
    draw_self(true);

    chosen = 0;
    items[0]->toggle();
}

bool MapChooser::processMessage(int message, Data extra_data)
{
    return false;
}

bool MapChooser::processKey(int ch)
{
    switch (ch)
    {
        case CON_KEY_ESCAPE:
            chosen = -1;
            return true;

        case CON_KEY_UP:
            items[chosen]->toggle();
            chosen = (chosen - 1 + counter) % counter;
            items[chosen]->toggle();
            break;

        case CON_KEY_DOWN:
            items[chosen]->toggle();
            chosen = (chosen + 1) % counter;
            items[chosen]->toggle();
            break;

        case '\r':
        case '\n':
            return true;
    }
    return false;
}

bool MapChooser::processIdle()
{
    return false;
}

int MapChooser::getKey()
{
    return con_getKey();
}

int MapChooser::keyPressed()
{
    return con_keyPressed();
}

