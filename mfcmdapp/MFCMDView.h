// MFCMDView.h : interface of the CMFCMDView class
//


#pragma once

namespace MDS
{

class CMFCMDView : public CView
{
protected: // create from serialization only
    DECLARE_DYNAMIC(CMFCMDView)

public:
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

protected:
    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // debug version in MFCMDView.cpp
inline CDocument* CMFCMDView::GetDocument() const
   { return reinterpret_cast<CDocument*>(m_pDocument); }
#endif

}
