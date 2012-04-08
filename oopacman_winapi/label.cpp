#include "stdafx.h"
#include "label.h"

namespace Label
{

char * align_center(char *dst, const char *src, size_t w)
{
    dst[w] = '\0';
    memset(dst, ' ', w);

    size_t len = strlen(src);
    if (len > w)
    {
        memcpy(dst, src, w - 3);
        memcpy(dst + w - 3, "...", 3);
    }
    else
        memcpy(dst + (w - len)/2, src, len);

    return dst;
}

Label::Label(unsigned x, unsigned y, unsigned w, unsigned h, const char *caption) : x(x), y(y), w(w), h(h)
{
    if (w < 4)
        w = 4;

    this->caption = new char[w + 1];
    align_center(this->caption, caption, w);
}

Label::~Label()
{
    delete[] caption;
}

void Label::init_colors()
{
    // init on first draw
    //static int init_col =       con_init_pair(LABEL_COLOR,      LABEL_FG, LABEL_BG);
    //static int init_col_inv =   con_init_pair(LABEL_INV_COLOR,  LABEL_BG, LABEL_FG);
    //static int init_col_clear = con_init_pair(BACKGROUND_COLOR,  BACKGROUND_FG, BACKGROUND_BG);
}

void Label::draw(bool normal)
{
    //if (!normal)
    //{
    //    clear();
    //    return;
    //}

    //init_colors();

    //con_set_color(LABEL_COLOR);
    //con_gotoXY(x, y);
    //con_outTxt("%s", caption);
}

void Label::clear()
{
    //init_colors();

    //con_set_color(BACKGROUND_COLOR);
    //for( unsigned i = 0 ; i < h ; ++i )
    //{
    //    con_gotoXY(x, y + i);
    //    con_outTxt("%*s", w, " ");
    //}
}

Digital::Digital(unsigned x, unsigned y, unsigned w, const char *caption)
    : Label(x, y, w, 2, caption)
{
    output_buf = new char[w + 1];

    value = 0;
}

Digital::~Digital()
{
    delete[] output_buf;
}

void Digital::draw(bool normal)
{
    //if (!normal)
    //{
    //    clear();
    //    return;
    //}
    //init_colors();

    //sprintf(format_buf, "%d", value);
    //align_center(output_buf, format_buf, w);

    //con_set_color(LABEL_COLOR);
    //con_gotoXY(x, y);
    //con_outTxt("%s", caption);
    //con_gotoXY(x, y+1);
    //con_outTxt("%s", output_buf);
}

void Digital::set_value(int v)
{
    if (v != value)
    {
        value = v;
        draw();
    }
}

Bar::Bar(unsigned x, unsigned y, unsigned w, const char *caption)
    : Label(x, y, w, 2, caption)
{
    bar_len = 0;
}

inline int round(double d)
{
    int i = int(d);
    return (2*d > double(2*i + 1)) ? (i + 1) : i;
}

void Bar::draw(bool normal)
{
    //if (!normal)
    //{
    //    clear();
    //    return;
    //}

    //init_colors();

    //con_set_color(LABEL_COLOR);
    //con_gotoXY(x, y);
    //con_outTxt("%s", caption);

    //// draw left border
    //con_set_color(LABEL_COLOR);
    //con_gotoXY(x, y + 1);
    //con_outTxt("[");
    //
    //// draw bar's filled part
    //if (bar_len > 0)
    //{
    //    con_set_color(LABEL_INV_COLOR);
    //    con_gotoXY(x + 1, y + 1);
    //    con_outTxt("%*s", bar_len, " ");
    //}

    //// draw bar's empty part
    //if (w-2 - bar_len > 0)
    //{
    //    con_set_color(LABEL_COLOR);
    //    con_gotoXY(x + 1 + bar_len, y + 1);
    //    con_outTxt("%*s", w-2 - bar_len, " ");
    //}

    //// draw right border
    //con_set_color(LABEL_COLOR);
    //con_gotoXY(x + w - 1, y + 1);
    //con_outTxt("]");
}

void Bar::set_percentage(unsigned percent)
{
    int new_len = round((w-2) * percent / 100.0);
    if (new_len == 0 && percent > 0)
        new_len = 1;
    if (new_len > w-2)
        new_len = w-2;

    if (new_len != bar_len)
    {
        bar_len = new_len;
        draw();
    }
}

MenuItem::MenuItem(unsigned x, unsigned y, unsigned w, const char *caption)
    : Label(x, y, w-6, 1, caption)
{
    // Base class constructor gets w-6 instead w for
    // correct align of caption
    // because label will be '-> caption <-'
    this->w += 6;
    chosen = false;
}

void MenuItem::draw(bool normal)
{
    //if (!normal)
    //{
    //    clear();
    //    return;
    //}

    //init_colors();

    //con_set_color(LABEL_COLOR);
    //con_gotoXY(x, y);
    //con_outTxt("%s %s %s", (chosen ? ">>" : "  "), caption, (chosen ? "<<" : "  "));
}

void MenuItem::toggle()
{
    chosen = !chosen;
    draw();
}

};

