#pragma once

#include  "oopacman.h"

namespace Label
{

class Label
{
protected:
    unsigned x;
    unsigned y;
    unsigned w;
    char * caption;

    void init_colors();

public:
    Label(unsigned x, unsigned y, unsigned w, const char *caption);
    virtual ~Label();

    virtual void draw() = 0;
};

/*
 *   Caption
 *     57
 */
class Digital : public Label
{
private:
    int value;

    char format_buf[13 + 1];
    char * output_buf;

public:
    Digital(unsigned x, unsigned y, unsigned w, const char *caption);
    virtual ~Digital();

    virtual void draw();
    void set_value(int v);
};

/*
 *    Caption
 * XXXXXXXXX____
 */
class Bar : public Label
{
private:
    unsigned bar_len; // 0..(w-2)

public:
    Bar(unsigned x, unsigned y, unsigned w, const char *caption);

    virtual void draw();
    void set_percentage(unsigned percent); // 0..100
};

};

