#include "buffer.h"

Buffer::Buffer(Coord pos, size_t width)
  : ConsoleObject(pos, Coord(int(width), 1))
{
    text_len = size.x - 2;
    cur_ind = 0;

    text = new char[text_len + 1];
    memset(text, ' ', text_len);
    text[text_len] = '\0';
}

Buffer::~Buffer()
{
    delete[] text;
}

void Buffer::render(int action)
{
    con_gotoXY(pos.x, pos.y);
    if (action == 0)
    {
        // normal rendering
        con_setColor(OBJ_COL);
        if (active)
            con_outTxt("<%*s>", text_len, text);
        else
            con_outTxt("[%*s]", text_len, text);
    }
    else
    {
        // erasing
        con_setColor(BG_COL);
        con_outTxt("%*s", size.x, "");
    }
    rendered();
}

bool Buffer::processKey(int ch)
{
    if (!ConsoleObject::processKey(ch))
    {
        if (ch >= 32 && ch <= 255)
            add_char(ch);
        else if (ch == CON_KEY_BACKSPACE)
            del_char();
        else
            return false;
    }
    return true;
}

void Buffer::add_char(int ch)
{
    if (cur_ind == text_len)
    {
        memset(text, ' ', text_len);
        cur_ind = 0;
    }
    text[cur_ind] = ch;
    cur_ind++;
    getLoop()->postMessage(this, MESSAGE_RENDER, Data(0));
}

void Buffer::del_char()
{
    if (cur_ind > 0)
    {
        --cur_ind;
        text[cur_ind] = ' ';
        getLoop()->postMessage(this, MESSAGE_RENDER, Data(0));

    }
}

