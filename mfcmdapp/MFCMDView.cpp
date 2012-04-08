// MFCMDView.cpp : implementation of the CMFCMDView class
//

#include "stdafx.h"
#include "MFCMDApp.h"

#include "MFCMDView.h"

using namespace MDS;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCMDView

IMPLEMENT_DYNAMIC(CMFCMDView, CView)

BEGIN_MESSAGE_MAP(CMFCMDView, CView)
    ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CMFCMDView diagnostics

#ifdef _DEBUG
void CMFCMDView::AssertValid() const
{
    CView::AssertValid();
}

void CMFCMDView::Dump(CDumpContext& dc) const
{
    CView::Dump(dc);
}

#endif //_DEBUG

// CMFCMDView message handlers
void CMFCMDView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{

    ((CMFCMDApp *)AfxGetApp())->processKey(nChar);
    //CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
