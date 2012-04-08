#include "../../StdAfx.h"
#include "../XWinApp.h"
#include "../XWnd.h"

XWnd::XWnd()
{
    this->_dialog = false;
    this->_windowMenu = NULL;
    this->_hwnd = NULL;
}

XWnd::~XWnd()
{
    _ASSERT(this->_hwnd == NULL);

    if (this->_windowMenu != NULL)
    {
        if (::IsMenu(this->_windowMenu))
            ::DestroyMenu(this->_windowMenu);

        this->_windowMenu = NULL;
    }
}

LRESULT XWnd::WndProc(UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg)
    {
    case WM_COMMAND:
        if (this->OnCommand(HIWORD(wp), LOWORD(wp), (HWND)lp))
            return 0;
        else
            break;

    case WM_PAINT:
        {
            XPaintDC dc(*this);
            this->OnPaint(dc);
        }
        break;

    case WM_DESTROY:
        if (XWinApp * app = XWinApp::GetApplication())
        {
            if (app->GetMainWindow() == this)
                ::PostQuitMessage(0);
        }
        break;
    }
    return this->DefMsgProc(msg, wp, lp);
}

void XWnd::PostNcDestroy()
{
}

void XWnd::OnPaint(XPaintDC & dc)
{
    UNREFERENCED_PARAMETER(dc);
}

bool XWnd::OnCommand(WORD wEvent, WORD wCmdID, HWND hwndControl)
{
    UNREFERENCED_PARAMETER(wEvent);
    UNREFERENCED_PARAMETER(wCmdID);
    UNREFERENCED_PARAMETER(hwndControl);

    return false;
}

LRESULT XWnd::DefMsgProc(UINT Msg, WPARAM wParam, LPARAM lParam)
{
    if (this->_dialog)
        return 0; //::DefDlgProc(this->_hwnd, Msg, wParam, lParam);
    else
        return ::DefWindowProc(this->_hwnd, Msg, wParam, lParam);
}

LRESULT XWnd::_wndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
    XWnd * ptr = NULL;

    if (msg == WM_NCCREATE)
    {
        CREATESTRUCT * cs = reinterpret_cast<CREATESTRUCT *>(lp);
        ptr = reinterpret_cast<XWnd *>(cs->lpCreateParams);
        ptr->_hwnd = hwnd;
#pragma warning(push)
#pragma warning(disable: 4311)
        ::SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG)ptr);
#pragma warning(pop)
    }
    else
    {
        LONG_PTR value = ::GetWindowLongPtr(hwnd, GWLP_USERDATA);
        ptr = reinterpret_cast<XWnd *>(value);
    }
    if (ptr == NULL)
        return ::DefWindowProc(hwnd, msg, wp, lp);

    LRESULT lr = ptr->WndProc(msg, wp, lp);

    if (msg == WM_NCDESTROY)
    {
        ::SetWindowLongPtr(hwnd, GWLP_USERDATA, 0);

        ptr->_hwnd = NULL;
        ptr->PostNcDestroy();
    }
    return lr;
}

ATOM XWnd::_class = 0;

bool XWnd::CreateWndEx(DWORD dwExStyle,
                       LPCWSTR lpWindowName,
                       DWORD dwStyle,
                       int X,
                       int Y,
                       int nWidth,
                       int nHeight,
                       HWND hWndParent,
                       LPCTSTR menuResourceName)
{
    HINSTANCE hInstance = XWinApp::GetInstance();

    if (menuResourceName != NULL)
    {
        this->_windowMenu = ::LoadMenu(hInstance, menuResourceName);
        if (this->_windowMenu == NULL)
        {
            _RPT0(_CRT_WARN, _T("Can't load window menu."));
            return false;
        }
    }
    if (_class == 0)
    {
        WNDCLASSEX wcex = {0};

        wcex.cbSize = sizeof(WNDCLASSEX);

        wcex.style          = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc    = _wndProc;
        wcex.hInstance      = hInstance;
        wcex.hIcon          = ::LoadIcon(NULL, IDI_APPLICATION);
        wcex.hCursor        = ::LoadCursor(NULL, IDC_ARROW);
        wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+0);
        wcex.lpszClassName  = _T("XWnd");

        _class = RegisterClassEx(&wcex);
        if (_class == 0)
            return false;
    }

    if (HWND hwnd = ::CreateWindowEx(
        dwExStyle, _T("XWnd"), lpWindowName, dwStyle,
        X, Y, nWidth, nHeight, hWndParent, this->_windowMenu,
        hInstance, this))
    {
        _ASSERT(this->_hwnd == hwnd);
        return true;
    }
    _RPT0(_CRT_WARN, _T("Can't create window."));
    return false;
}

//--------------------------------------

XDialog::XDialog(UINT templateResourceID)
{
    this->_dialog = true;
    this->_templateResourceName = MAKEINTRESOURCE(templateResourceID);
}

XDialog::XDialog(LPTSTR templateResourceName)
{
    this->_dialog = true;
    this->_templateResourceName = templateResourceName;
}

INT_PTR XDialog::DoModal(HWND parentWindow)
{
    return ::DialogBoxParam(XWinApp::GetInstance(),
        this->_templateResourceName, parentWindow, _dlgProc, (LPARAM)this);
}

LRESULT XDialog::WndProc(UINT msg, WPARAM wp, LPARAM lp)
{
    UNREFERENCED_PARAMETER(wp);
    UNREFERENCED_PARAMETER(lp);

    switch (msg)
    {
    case WM_INITDIALOG:
        return this->OnInitDialog();

    case WM_COMMAND:
    case WM_PAINT:
    case WM_DESTROY:
    default:
        return this->super::WndProc(msg, wp, lp);
    }
}

bool XDialog::OnInitDialog() {
    return true; }

void XDialog::OnOK() {
    this->EndDialog(IDOK); }

void XDialog::OnCancel() {
    this->EndDialog(IDCANCEL); }

bool XDialog::OnCommand(WORD wEvent, WORD wCmdID, HWND hwndControl)
{
    UNREFERENCED_PARAMETER(wEvent);
    UNREFERENCED_PARAMETER(hwndControl);

    if (wCmdID == IDOK)
    {
        this->OnOK();
        return true;
    }
    if (wCmdID == IDCANCEL)
    {
        this->OnOK();
        return true;
    }
    return false;
}

INT_PTR XDialog::_dlgProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
    XWnd * ptr = NULL;

    if (msg == WM_INITDIALOG)
    {
        ptr = reinterpret_cast<XWnd *>(lp);
        ptr->_hwnd = hwnd;
#pragma warning(push)
#pragma warning(disable: 4311)
        ::SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG)ptr);
#pragma warning(pop)
    }
    else
    {
        LONG_PTR value = ::GetWindowLongPtr(hwnd, GWLP_USERDATA);
        ptr = reinterpret_cast<XWnd *>(value);
    }

    if (ptr == NULL)
        return (INT_PTR)false;

    LRESULT lr = ptr->WndProc(msg, wp, lp);

    if (msg == WM_NCDESTROY)
    {
        ::SetWindowLongPtr(hwnd, GWLP_USERDATA, 0);

        ptr->_hwnd = NULL;
        ptr->PostNcDestroy();
    }
    return (INT_PTR)lr;
}
