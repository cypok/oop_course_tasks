#include "../../StdAfx.h"
#include "../XDC.h"

XDC::XDC()
{
    this->_hdc = NULL;
}

XDC::XDC(HDC hdc)
{
    this->_hdc = hdc;
}

XDC::~XDC()
{
}

XPaintDC::XPaintDC(HWND hwnd)
{
    static PAINTSTRUCT empty = {0};
    this->_hwnd = hwnd;
    this->_ps = empty;
    this->_hdc = ::BeginPaint(hwnd, &this->_ps);
    _ASSERT(this->_hdc != NULL);
}

XPaintDC::~XPaintDC()
{
    ::EndPaint(this->_hwnd, &this->_ps);
    this->_hdc = NULL;
}

XWindowDC::XWindowDC(HWND hwnd, bool client)
{
    this->_hwnd = hwnd;

    if (client)
        this->_hdc = ::GetDC(hwnd);
    else
        this->_hdc = ::GetWindowDC(hwnd);

    _ASSERT(this->_hdc != NULL);
}

XWindowDC::~XWindowDC()
{
    ::ReleaseDC(this->_hwnd, this->_hdc);
    this->_hdc = NULL;
}
