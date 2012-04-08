#pragma once

#include  "settings.h"

namespace Label
{

class Label
{
protected:
    unsigned x;
    unsigned y;
    unsigned w;
    unsigned h;
    char * caption;

    void init_colors();
    void clear();

public:
    Label(unsigned x, unsigned y, unsigned w, unsigned h, const char *caption);
    virtual ~Label();

    virtual void draw(bool normal = true); // if false -> erase
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

    virtual void draw(bool normal = true);
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

    virtual void draw(bool normal = true);
    void set_percentage(unsigned percent); // 0..100
};

class MenuItem : public Label
{
private:
    bool chosen;

public:
    MenuItem(unsigned x, unsigned y, unsigned w, const char *caption);

    virtual void draw(bool normal = true);
    void toggle();
};

};

