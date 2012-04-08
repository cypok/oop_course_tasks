// MainFrm.cpp : implementation of the CMFCMDMainFrame class
//

#include "stdafx.h"
#include "MFCMDApp.h"

#include "MainFrm.h"

using namespace MDS;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCMDMainFrame

IMPLEMENT_DYNCREATE(CMFCMDMainFrame, CFrameWnd)


// CMFCMDMainFrame construction/destruction

CMFCMDMainFrame::CMFCMDMainFrame()
{
    // TODO: add member initialization code here
}

CMFCMDMainFrame::~CMFCMDMainFrame()
{
}

LRESULT CMFCMDMainFrame::WindowProc(UINT msg, WPARAM wp, LPARAM lp)
{
    if (msg >= WM_MDS)
    {
        Data extra_data;
        memcpy(&extra_data, &lp, sizeof(Data));

        bool result;
        if (wp == NULL)
            result = (CMFCMDApp::GetApp())->processMessage(msg, extra_data);
        else
            result = ((Object*)wp)->processMessage(msg, extra_data);
        if (result)
            DestroyWindow();
        return NULL;
    }
    else if (msg == WM_KEYDOWN)
        return (CMFCMDApp::GetApp())->processKey(wp);
    else
        return CFrameWnd::WindowProc(msg, wp, lp);
}
