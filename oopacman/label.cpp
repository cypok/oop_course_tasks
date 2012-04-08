#include "label.h"

namespace Label
{

char * align_center(char *dst, const char *src, size_t w)
{
    dst[w] = '\0';
    memset(dst, ' ', w);

    size_t len = strlen(src);
    if (len > w)
        len = w;

    memcpy(dst + (w - len)/2, src, len);

    return dst;
}

Label::Label(unsigned x, unsigned y, unsigned w, const char *capt) : x(x), y(y), w(w)
{
    if (w < 1)
        w = 1;

    caption = new char[w + 1];
    align_center(caption, capt, w);
}

Label::~Label()
{
    delete[] caption;
}

void Label::init_colors()
{
    // init on first draw
    static int init_col =       con_initPair(LABEL_COLOR,      LABEL_FG, LABEL_BG);
    static int init_col_inv =   con_initPair(LABEL_INV_COLOR,  LABEL_BG, LABEL_FG);
}

Digital::Digital(unsigned x, unsigned y, unsigned w, const char *capt)
    : Label(x, y, w, capt)
{
    output_buf = new char[w + 1];

    value = 0;
}

Digital::~Digital()
{
    delete[] output_buf;
}

void Digital::draw()
{
    init_colors();

    sprintf(format_buf, "%d", value);
    align_center(output_buf, format_buf, w);

    con_setColor(LABEL_COLOR);
    con_gotoXY(x, y);
    con_outTxt("%s", caption);
    con_gotoXY(x, y+1);
    con_outTxt("%s", output_buf);
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
    : Label(x, y, w, caption)
{
    bar_len = 0;
}

inline int round(double d)
{
    int i = int(d);
    return (2*d > double(2*i + 1)) ? (i + 1) : i;
}

void Bar::draw()
{
    init_colors();

    con_setColor(LABEL_COLOR);
    con_gotoXY(x, y);
    con_outTxt("%s", caption);

    // draw left border
    con_setColor(LABEL_COLOR);
    con_gotoXY(x, y + 1);
    con_outTxt("[");

    // draw bar's filled part
    if (bar_len > 0)
    {
        con_setColor(LABEL_INV_COLOR);
        con_gotoXY(x + 1, y + 1);
        con_outTxt("%*s", bar_len, " ");
    }

    // draw bar's empty part
    if (w-2 - bar_len > 0)
    {
        con_setColor(LABEL_COLOR);
        con_gotoXY(x + 1 + bar_len, y + 1);
        con_outTxt("%*s", w-2 - bar_len, " ");
    }

    // draw right border
    con_setColor(LABEL_COLOR);
    con_gotoXY(x + w - 1, y + 1);
    con_outTxt("]");
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

};

