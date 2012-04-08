#pragma once

#include "XDC.h"

class XWnd
{
public:
    XWnd();
    virtual ~XWnd();

    inline operator HWND() {
        return this->_hwnd; }

    inline operator HWND() const {
        return this->_hwnd; }

    void ShowWindow(int nCmdShow) {
        ::ShowWindow(*this, nCmdShow); }

    void UpdateWindow() {
        ::UpdateWindow(*this); }

    void DestroyWindow() {
        ::DestroyWindow(*this); }

    inline
    bool CreateWnd(
        LPCWSTR lpWindowName,
        DWORD dwStyle,
        int X,
        int Y,
        int nWidth,
        int nHeight,
        HWND hWndParent,
        UINT menuResourceID)
    {
        return this->CreateWnd(lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent,
            MAKEINTRESOURCE(menuResourceID));
    }

    inline
    bool CreateWnd(
        LPCWSTR lpWindowName,
        DWORD dwStyle,
        int X,
        int Y,
        int nWidth,
        int nHeight,
        HWND hWndParent,
        LPCTSTR menuResourceName)
    {
        return this->CreateWndEx(0L, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent,
            menuResourceName);
    }

    inline
    bool CreateWndEx(
        DWORD dwExStyle,
        LPCWSTR lpWindowName,
        DWORD dwStyle,
        int X,
        int Y,
        int nWidth,
        int nHeight,
        HWND hWndParent,
        UINT menuResourceID)
    {
        return this->CreateWndEx(dwExStyle, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent,
            MAKEINTRESOURCE(menuResourceID));
    }

    bool CreateWndEx(
        DWORD dwExStyle,
        LPCWSTR lpWindowName,
        DWORD dwStyle,
        int X,
        int Y,
        int nWidth,
        int nHeight,
        HWND hWndParent,
        LPCTSTR menuResourceName);

protected:
    virtual LRESULT WndProc(UINT msg, WPARAM wp, LPARAM lp);
    virtual void PostNcDestroy();

    virtual bool OnCommand(WORD wEvent, WORD wCmdID, HWND hwndControl);
    virtual void OnPaint(XPaintDC & dc);

private:
    bool _dialog;
    mutable HWND _hwnd;

private:
    HMENU _windowMenu;
    static ATOM _class;
    static LRESULT CALLBACK _wndProc(HWND, UINT, WPARAM, LPARAM);

    LRESULT DefMsgProc(UINT Msg, WPARAM wParam, LPARAM lParam);

    friend class XDialog;
};

class XDialog
    : public XWnd
{
protected:
    typedef XWnd    super;

public:
    XDialog(UINT templateResourceID);
    XDialog(LPTSTR templateResourceName);

    virtual INT_PTR DoModal(HWND parentWindow);

    inline BOOL EndDialog(INT_PTR nResult) {
        return ::EndDialog(*this, nResult); }

protected:
    LPCTSTR _templateResourceName;

    virtual LRESULT WndProc(UINT msg, WPARAM wp, LPARAM lp);

    virtual bool OnInitDialog();
    virtual void OnOK();
    virtual void OnCancel();

    virtual bool OnCommand(WORD wEvent, WORD wCmdID, HWND hwndControl);

private:
    static INT_PTR CALLBACK _dlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
};
