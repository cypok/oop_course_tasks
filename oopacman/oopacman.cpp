#include "oopacman.h"
#include "game.h"

ERROR(IORead, "Cannot read from file \"%s\"")

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: oopacman map_name\n");
        return 1;
    }

    try
    {
        FILE *file = fopen(argv[1], "r");
        if (file == NULL)
            throw IOReadError(argv[1]);

        Game game(file, 1000 / TICKS_PER_SEC);
        game.start();

        return 0;
    }
    catch (Error &e)
    {
        fprintf(stderr, "Error occured! ");
        e.print(stderr);
        return 2;
    }
}

