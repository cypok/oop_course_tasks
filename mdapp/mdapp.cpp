#include "stdafx.h"
#include "mdapp.h"

#ifdef _DEBUG
#   define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

using namespace MDS;

class MsgInfo
{
public:
    Object *obj;
    int message;
    Data extra_data;
    MsgInfo(Object *obj = NULL, int message = WM_MDS, Data extra_data = 0) :  obj(obj), message(message), extra_data(extra_data) {}
};

void CALLBACK TimerCallback(HWND hWnd, UINT msg, UINT_PTR id, DWORD global_time);

//void Window::PostNcDestroy()
//{
//  delete this; // don't delete this!
//}

LRESULT MDS::Window::WndProc(UINT msg, WPARAM wp, LPARAM lp)
{
    if (msg >= WM_MDS)
    {
        Data extra_data;
        memcpy(&extra_data, &lp, sizeof(Data));

        if (wp == NULL)
            ((App*)App::GetApplication())->processMessage(msg, extra_data);
        else
            ((Object*)wp)->processMessage(msg, extra_data);
        return NULL;
    }
    if (msg == WM_KEYDOWN)
        return ((App*)App::GetApplication())->processKey(wp);
    else
        return XWnd::WndProc(msg, wp, lp);
}

void MDS::App::postMessage (Object *obj, int message, Data extra_data, unsigned delay)
{
    // TODO: check for winapi errors
    if (delay == 0)
    {
        LPARAM p;
        memcpy(&p, &extra_data, sizeof(Data));
        PostMessage(*this->GetMainWindow(), message, (WPARAM)obj, p);
    }
    else
    {
        MsgInfo *info = new MsgInfo(obj, message, extra_data);
        UINT_PTR id = SetTimer(*this->GetMainWindow(), (UINT_PTR)info, delay, TimerCallback);
    }
}

void CALLBACK TimerCallback(HWND hWnd, UINT msg, UINT_PTR id, DWORD global_time)
{
    _ASSERT(KillTimer(hWnd, id) != 0);
    MsgInfo *info = (MsgInfo *)id;
    ((App*)App::GetApplication())->postMessage(info->obj, info->message, info->extra_data);
    delete info;
}

int App::InitInstance()
{
    int code = XWinApp::InitInstance();
    MessageLoop::internalBeforeRun();
    MessageLoop::beforeRun();
    return code;

}
int App::ExitInstance(int exitCode)
{
    MessageLoop::afterRun();
    MessageLoop::internalAfterRun();
    return XWinApp::ExitInstance(exitCode);
}
