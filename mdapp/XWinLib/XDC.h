#pragma once

class XDC
{
protected:
    XDC();

public:
    XDC(HDC hdc);
    virtual ~XDC();

    inline operator HDC() {
        return this->_hdc; }

    inline operator HDC() const {
        return this->_hdc; }

    inline void FillRect(const RECT & rc, HBRUSH hbr)
    {
        _ASSERT(this->_hdc != NULL);
        ::FillRect(this->_hdc, &rc, hbr);
    }

protected:
    mutable HDC _hdc;
};

class XPaintDC : public XDC
{
public:
    XPaintDC(HWND hwnd);
    virtual ~XPaintDC();

    inline const PAINTSTRUCT & GetPaintStruct() const {
        return this->_ps; }

private:
    HWND _hwnd;
    PAINTSTRUCT _ps;
};

class XWindowDC : public XDC
{
public:
    XWindowDC(HWND hwnd, bool client);
    virtual ~XWindowDC();

private:
    HWND _hwnd;
};
