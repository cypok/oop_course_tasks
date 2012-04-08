#include "oopacman.h"
#include "game.h"
#include "map_chooser.h"

ERROR(IORead, "Cannot read from file \"%s\"")

int main()
{
    con_init();
    try
    {
        GameStats last_stats = {GR_NONE, 0};

        while(true)
        {
            char *filename = NULL;

            MapChooser chooser(last_stats);
            // need to delete filename later
            filename = chooser.get();

            // if choosing canceled -> exit
            if (filename == NULL)
            {
                con_deinit();
                return 0;
            }

            FILE *file = fopen(filename, "r");
            if (file == NULL)
                throw IOReadError(filename);

            delete[] filename;

            Game game(file, 1000 / TICKS_PER_SEC);
            last_stats = game.start();
        }
    }
    catch (Error &e)
    {
        con_deinit();
        fprintf(stderr, "Error occured! ");
        e.print(stderr);
        return 1;
    }
    con_deinit();
}

