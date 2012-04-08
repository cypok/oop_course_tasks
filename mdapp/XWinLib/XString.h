#pragma once

#include <string>

#ifdef UNICODE
#define BaseStlString std::wstring
#else
#define BaseStlString std::string
#endif

class XString : public BaseStlString
{
public:
    typedef BaseStlString super;

    virtual ~XString();

    XString()
    {
    }
    XString(const TCHAR * _Ptr, size_type _Count)
        : super(_Ptr, _Count) {
    }
    XString(const TCHAR * _Ptr)
        : super(_Ptr) {
    }
    XString(size_type _Count, TCHAR _Ch) : super(_Count, _Ch) {
    }
    XString(BaseStlString & _Right, size_type _Roff, size_type _Count = BaseStlString::npos)
        : super(_Right, _Roff, _Count) {
    }

    operator LPCTSTR() const {
        return this->c_str(); }

    inline bool LoadFromResource(UINT nStringID) {
        return this->LoadFromResource(NULL, nStringID); }

    bool LoadFromResource(HINSTANCE hInstance, UINT nStringID) throw(...);
};
