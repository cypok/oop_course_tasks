#include <iostream>

#include "../mds/mds.h"
#include "console_loop.h"
#include "buffer.h"
#include "clock.h"

#ifndef NDEBUG
#   ifdef WIN32
#       pragma warning(disable : 4996)
#       define _CRTDBG_MAP_ALLOC
#       include <crtdbg.h>
#   else
#       include <mcheck.h>
#   endif
#endif

using namespace std;

#define OBJ_NUM   size_t(5)


int main()
{
#if (!NDEBUG && !WIN32)
    mtrace();
#endif

    try
    {
        con_init();
        con_init_pair(OBJ_COL, CON_COLOR_GREEN,CON_COLOR_CYAN);
        con_init_pair(BG_COL, CON_COLOR_BLACK,CON_COLOR_BLACK);
        ConsoleObject *objects[OBJ_NUM];
        int i = 0;

        objects[i++] = new Clock(Coord(8, 5));
        objects[i++] = new Clock(Coord(20, 15));
        objects[i++] = new Buffer(Coord(2, 3), 10);
        objects[i++] = new Buffer(Coord(5, 8), 20);
        objects[i++] = new Buffer(Coord(0, 0), 3);

        ConsoleLoop loop(objects, OBJ_NUM);

        loop.run();

        for(i = 0; i < OBJ_NUM; ++i)
            delete objects[i];

        con_deinit();
    }
    catch (exception &e)
    {
        con_deinit();
        cout << "Exception caught: " << e.what() << endl;
    }
    catch (...)
    {
        con_deinit();
        cout << "Exception caught..." << endl;
    }

#if (!NDEBUG && WIN32)
    _CrtDumpMemoryLeaks();
#endif
    return 0;
}
