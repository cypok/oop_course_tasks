#pragma once

#include "oopacman.h"
#include <string.h>
#include <stdio.h>

class Error
{
private:
    char * data;

public:
    inline Error(const char * data_string)
    {
        data = strdup(data_string);
    }

    inline ~Error()
    {
        free(data);
    }

    virtual char * get_info() = 0;

    inline void print(FILE * file)
    {
        fprintf(file, get_info(), data);
        fprintf(file, "\n");
    }
};

#define ERROR(Type, error_info) \
class Type##Error : public Error \
{ \
private: \
    char info[sizeof(error_info)]; \
public: \
    Type##Error(const char * data_string) : Error(data_string) \
    { \
        strcpy(info, error_info); \
    } \
    virtual char * get_info() { return info; } \
};

