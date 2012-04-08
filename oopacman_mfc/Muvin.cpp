#include "StdAfx.h"
#include "Muvin.h"

Muvin::Muvin(int w, int h) : x(0), y(0), w(w), h(h), dir(RIGHT), started(false)
{
}

Muvin::~Muvin()
{
}

void Muvin::draw(CDC *dc)
{
    RECT rect = {x, y, x+w, y+h};

    CBrush br(RGB(255, 0, 0));

    dc->FillRect(&rect, &br);
}

bool Muvin::processMessage(int message, Data extra_data)
{
    switch (message)
    {
    case MSG_DRAW:
        draw((CDC *)extra_data.pointer());
        break;
    case MSG_MUVIN_START:
        if (started)
            break;
        started = true;
        // and act as if ...MOVE received: no break
    case MSG_MUVIN_MOVE:
        switch (dir)
        {
        case LEFT:
            x -= MUVIN_INTERVAL;
            break;
        case UP:
            y -= MUVIN_INTERVAL;
            break;
        case RIGHT:
            x += MUVIN_INTERVAL;
            break;
        case DOWN:
            y += MUVIN_INTERVAL;
            break;
        }
        (COOPacManApp::GetApp())->m_pMainWnd->Invalidate(FALSE);
        getLoop()->postMessage(this, MSG_MUVIN_MOVE, 0, 50);
        break;
    }
    return false;
}
