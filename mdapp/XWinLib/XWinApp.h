#pragma once

class XWnd;

class XWinApp
{
public:
    XWinApp();
    virtual ~XWinApp();

    static HINSTANCE GetInstance();

    inline static XWinApp * GetApplication() {
        return _instance; }

    static bool LoadStringW(UINT nStringID, LPWSTR lpBuffer, UINT cchBufferMax);
    static bool LoadStringA(UINT nStringID, LPSTR lpBuffer, UINT cchBufferMax);

    inline XWnd * GetMainWindow() const {
        return this->_mainWindow; }

    void SetMainWindow(XWnd * value);

    int Run();
    virtual bool TranslateAccelerator(MSG & msg);

protected:
    virtual int InitInstance();
    virtual int ExitInstance(int exitCode);
    virtual void OnMainWindowChanged(XWnd * oldValue, XWnd * newValue);

private:
    static XWinApp * _instance;
    mutable XWnd * _mainWindow;
};
