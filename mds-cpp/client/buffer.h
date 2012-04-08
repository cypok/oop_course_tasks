#pragma once

#include "console_object.h"
#include "../mds/mds.h"

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#pragma pack(push, 1)
class Buffer : public ConsoleObject
{
protected:
    char    *text;
    size_t  text_len;
    unsigned int cur_ind;

    virtual void render(int mode);

public:
    Buffer(Coord pos, size_t width);
    ~Buffer();

    virtual bool processKey(int ch);

    void add_char(int ch);
    void del_char();
};
#pragma pack(pop)

