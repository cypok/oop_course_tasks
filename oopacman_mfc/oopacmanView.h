// oopacmanView.h : interface of the COOPacManView class
//
#pragma once
#include "../mfcmdapp/mfcmdview.h"
#include "game.h"

class COOPacManView : public CMFCMDView
{
protected: // create from serialization only
    COOPacManView();
    DECLARE_DYNCREATE(COOPacManView)

// Attributes
public:
    Game* GetDocument() const;

// Operations
public:

// Overrides
public:
    virtual void OnDraw(CDC* pDC);  // overridden to draw this view
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
    virtual ~COOPacManView();
    static COOPacManView * COOPacManView::GetView();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
    DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in oopacmanView.cpp
inline Game* COOPacManView::GetDocument() const
   { return reinterpret_cast<Game*>(m_pDocument); }
#endif

