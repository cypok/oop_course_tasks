#pragma once

#include "oopacman.h"
#include "label.h"

#ifdef _MSC_VER
#   include "lib/dirent.h"
#else
#   include <dirent.h>
#endif


class MapChooser : public MessageLoop
{
private:
    char ** files;
    int counter;
    int chosen;

    Label::Label * h_main;
    Label::Label * h_score;
    Label::Label * h_level;
    Label::MenuItem ** items;

protected:
    // FOR ALL: if true leave run()
    virtual bool processMessage(int message, Data extra_data);
    virtual bool processKey(int ch);
    virtual bool processIdle();

    virtual void beforeRun();

    virtual int getKey();
    virtual int keyPressed();

    void draw_self(bool normal = true); // if false -> erase

public:
    MapChooser(GameStats last);
    ~MapChooser();

    // returns string on heap
    char * get();
};

