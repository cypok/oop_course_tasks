#pragma once

#include "stdafx.h"
#include "XWinLib\XWinApp.h"
#include "XWinLib\XString.h"
#include "XWinLib\XWnd.h"
#include "XWinLib\XDC.h"

#define WM_MDS  WM_APP+1

// TODO: fix error message
// #if ( sizeof(LPARAM) < sizeof(MDS::Data) )
// #   error "Cannot compile because MDS::Data cannot be stored in LPARAM"
// #endif

namespace MDS
{

class Window : public XWnd
{
protected:
    //virtual void PostNcDestroy();

    virtual LRESULT WndProc(UINT msg, WPARAM wp, LPARAM lp);
};

class App
    : public XWinApp,
      public MessageLoop
{
    friend Window;
public:
    App() {}
    virtual ~App() {}

    virtual void postMessage (Object *obj, int message, Data extra_data, unsigned delay = 0);

protected:
    virtual int InitInstance();
    virtual int ExitInstance(int exitCode);

private:
    Window window;
};

}
