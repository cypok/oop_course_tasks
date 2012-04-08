// MFCMDApp.h : main header file for the MFCMDApp application
//
#pragma once

#ifndef __AFXWIN_H__
    #error "include 'stdafx.h' before including this file for PCH"
#endif

#include "stdafx.h"         // we really need only MDS yet, but let it be
#include "mfcmdview.h"
#include "mainfrm.h"

// CMFCMDApp:
// See MFCMDApp.cpp for the implementation of this class
//

#define WM_MDS  WM_APP+1

namespace MDS
{

class CMFCMDApp :
      public CWinApp,
      public MessageLoop
{
friend CMFCMDView;
friend CMFCMDMainFrame;

public:
    CMFCMDApp();


// Overrides
public:
    virtual void postMessage (Object *obj, int message, Data extra_data, unsigned delay = 0);

    virtual BOOL InitInstance();
    virtual int ExitInstance();

    // functions for working with UI - here they are dummy, and they are never called
    virtual int getKey() { return 0; }
    virtual int keyPressed() { return 0; }

    static CMFCMDApp* GetApp() { return (CMFCMDApp*)AfxGetApp();}

};

}
