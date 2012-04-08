// MFCMDApp.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "MFCMDApp.h"

using namespace MDS;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

class MsgInfo
{
public:
    Object *obj;
    int message;
    Data extra_data;
    MsgInfo(Object *obj = NULL, int message = WM_MDS, Data extra_data = 0) :  obj(obj), message(message), extra_data(extra_data) {}
};

static void CALLBACK TimerCallback(HWND hWnd, UINT msg, UINT_PTR id, DWORD global_time);

// CMFCMDApp

// CMFCMDApp construction

CMFCMDApp::CMFCMDApp()
{
}

// CMFCMDApp initialization

BOOL CMFCMDApp::InitInstance()
{
    CWinApp::InitInstance();
    MessageLoop::internalBeforeRun();
    beforeRun();
    return TRUE;
}

int CMFCMDApp::ExitInstance()
{
    afterRun();
    MessageLoop::internalAfterRun();
    return CWinApp::ExitInstance();
}

void CMFCMDApp::postMessage (Object *obj, int message, Data extra_data, unsigned delay)
{
    // TODO: check for winapi errors
    if (delay == 0)
    {
        LPARAM p;
        memcpy(&p, &extra_data, sizeof(Data));
        m_pMainWnd->PostMessage(message, (WPARAM)obj, p);
    }
    else
    {
        MsgInfo *info = new MsgInfo(obj, message, extra_data);
        m_pMainWnd->SetTimer((UINT_PTR)info, delay, TimerCallback);
    }
}

void CALLBACK TimerCallback(HWND hWnd, UINT msg, UINT_PTR id, DWORD global_time)
{
    KillTimer(hWnd, id); // right? or get _somehow_ a CWindow by hWnd and call its KillTimer
    MsgInfo *info = (MsgInfo *)id;
    (CMFCMDApp::GetApp())->postMessage(info->obj, info->message, info->extra_data);
    delete info;
}
