#ifndef _CONSOLE_H_
#define _CONSOLE_H_

#ifdef WIN32
#include <conio.h>

#define CON_KEY_UP          -72
#define CON_KEY_LEFT        -75
#define CON_KEY_RIGHT       -77
#define CON_KEY_DOWN        -80
#define CON_KEY_DEL         -83
#define CON_KEY_ESCAPE      27
#define CON_KEY_BACKSPACE   '\b'

#define CON_COLOR_BLACK     0
#define CON_COLOR_GREEN     0
#define CON_COLOR_YELLOW    0
#define CON_COLOR_BLUE      0
#define CON_COLOR_RED       0
#define CON_COLOR_MAGENTA   0
#define CON_COLOR_CYAN      0
#define CON_COLOR_WHITE     0

#else
#include <ncurses.h>

#define CON_KEY_UP          KEY_UP
#define CON_KEY_DOWN        KEY_DOWN
#define CON_KEY_LEFT        KEY_LEFT
#define CON_KEY_RIGHT       KEY_RIGHT
#define CON_KEY_ESCAPE      27
#define CON_KEY_DEL         KEY_DC
#define CON_KEY_BACKSPACE   KEY_BACKSPACE

#define CON_COLOR_BLACK     COLOR_BLACK
#define CON_COLOR_GREEN     COLOR_GREEN
#define CON_COLOR_YELLOW    COLOR_YELLOW
#define CON_COLOR_BLUE      COLOR_BLUE
#define CON_COLOR_RED       COLOR_RED
#define CON_COLOR_MAGENTA   COLOR_MAGENTA
#define CON_COLOR_CYAN      COLOR_CYAN
#define CON_COLOR_WHITE     COLOR_WHITE

#endif

extern void     con_init();
extern void     con_deinit();

extern int      con_gotoXY(int x, int y);
extern int      con_getXY(int *px, int *py);
extern int      con_getMaxXY(int *px, int *py);
extern int      con_clearScr();

extern int      con_outTxt(const char *format, ...);

extern int      con_keyPressed();
extern int      con_getKey();

extern int      con_init_pair(short n, short fg, short bg);
extern int      con_set_color(short n);

#endif
