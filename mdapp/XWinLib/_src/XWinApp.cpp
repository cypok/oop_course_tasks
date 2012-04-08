#include "../../StdAfx.h"
#include "../XWinApp.h"
#include "../XWnd.h"

XWinApp * XWinApp::_instance = NULL;

XWinApp::XWinApp()
{
    _ASSERT(_instance == NULL);
    _instance = this;
}

XWinApp::~XWinApp()
{
    _ASSERT(_instance == this);
    _instance = NULL;
}

HINSTANCE XWinApp::GetInstance()
{
    static HINSTANCE instance = NULL;

    if (instance == NULL)
        instance = (HINSTANCE)::GetModuleHandle(NULL);

    return instance;
}

bool XWinApp::LoadStringW(UINT nStringID, LPWSTR lpBuffer, UINT cchBufferMax) {
    return 0 != ::LoadStringW(GetInstance(), nStringID, lpBuffer, cchBufferMax); }

bool XWinApp::LoadStringA(UINT nStringID, LPSTR lpBuffer, UINT cchBufferMax) {
    return 0 != ::LoadStringA(GetInstance(), nStringID, lpBuffer, cchBufferMax); }

void XWinApp::SetMainWindow(XWnd * value)
{
    XWnd * oldValue = this->_mainWindow;
    this->_mainWindow = value;
    this->OnMainWindowChanged(oldValue, this->_mainWindow);
}

void XWinApp::OnMainWindowChanged(XWnd * oldValue, XWnd * newValue)
{
    UNREFERENCED_PARAMETER(oldValue);
    UNREFERENCED_PARAMETER(newValue);
}

int XWinApp::InitInstance()
{
    return 0;
}

int XWinApp::ExitInstance(int exitCode)
{
    return exitCode;
}

int XWinApp::Run()
{
    int exitCode = this->InitInstance();

    if (exitCode == 0)
    {
        MSG msg;

        while (GetMessage(&msg, NULL, 0, 0))
        {
            if (this->TranslateAccelerator(msg))
                continue;

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        exitCode = (int) msg.wParam;
    }
    return this->ExitInstance(exitCode);
}

bool XWinApp::TranslateAccelerator(MSG & msg)
{
    UNREFERENCED_PARAMETER(msg);
    return false;
}
