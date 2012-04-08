#include "../../StdAfx.h"
#include "../XString.h"
#include "../XWinApp.h"

XString::~XString()
{
}

bool XString::LoadFromResource(HINSTANCE hInstance, UINT nStringID)
{
    if (hInstance == NULL)
        hInstance = XWinApp::GetInstance();

    TCHAR buffer[257];

    if (!::LoadString(hInstance, nStringID, buffer, _countof(buffer)))
        return false;

    *this = buffer;

    return true;
}
