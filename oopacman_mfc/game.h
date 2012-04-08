// game.h : interface of the Game class
//
#pragma once
#include  <time.h>

#include  "settings.h"
#include  "gameloop.h"
#include  "map.h"
#include  "hero.h"
#include  "timer.h"
#include  "label.h"

class Game : public CDocument
{
private:
    Map map;
    PacMan pacman;
    Ghost ghosts[GHOST_NUMBER];
    Timer timer;

    Label::Digital score_label;
    Label::Bar energizer_label;

    HIMAGELIST images[OBJ_COUNT];

public:
    HIMAGELIST get_image_of(ObjType ot);

    PacMan * get_pacman();
    Ghost * get_ghost(unsigned index);
    Timer * get_timer();
    Map * get_map();

    Label::Digital * get_score_label();
    Label::Bar * get_energizer_label();

    void draw_map(CDC &dc);
    void draw_heroes();

protected: // create from serialization only
    Game();
    DECLARE_DYNCREATE(Game)

// Attributes
public:

// Operations
public:

// Overrides
public:
    virtual BOOL OnNewDocument();
    virtual void Serialize(CArchive& ar);

// Implementation
public:
    virtual ~Game();
    static Game * GetGame();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
    DECLARE_MESSAGE_MAP()
};
