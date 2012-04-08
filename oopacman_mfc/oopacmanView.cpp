// oopacmanView.cpp : implementation of the COOPacManView class
//

#include "stdafx.h"
#include "gameloop.h"

#include "oopacmanView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COOPacManView

IMPLEMENT_DYNCREATE(COOPacManView, CMFCMDView)

BEGIN_MESSAGE_MAP(COOPacManView, CMFCMDView)
END_MESSAGE_MAP()

// COOPacManView construction/destruction

COOPacManView::COOPacManView()
{
    // TODO: add construction code here

}

COOPacManView::~COOPacManView()
{
}

BOOL COOPacManView::PreCreateWindow(CREATESTRUCT& cs)
{
    // TODO: Modify the Window class or styles here by modifying
    //  the CREATESTRUCT cs

    return CMFCMDView::PreCreateWindow(cs);
}

// COOPacManView drawing

void COOPacManView::OnDraw(CDC* pDC)
{
    Game* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;

    static bool window_resized = false;
    if (!window_resized)
    {
        RECT rect = (GameLoop::GetApp())->get_game()->get_map()->get_rect();
        RECT old_rect;
        //RECT old_client_rect;
        (GameLoop::GetApp())->m_pMainWnd->GetWindowRect(&old_rect);
        //(GameLoop::GetApp())->m_pMainWnd->GetClientRect(&old_client_rect);
        //int old_width = old_rect.right - old_rect.left;
        //int old_client_width = old_client_rect.right - old_client_rect.left;
        //int old_height = old_rect.bottom - old_rect.top;
        //int old_client_height = old_client_rect.bottom - old_client_rect.top;

        old_rect.right = old_rect.left + rect.right - rect.left;
        old_rect.bottom = old_rect.top + rect.bottom - rect.top;
        CWnd* wnd = (GameLoop::GetApp())->m_pMainWnd;
        AdjustWindowRectEx(&old_rect, wnd->GetStyle(), TRUE, wnd->GetExStyle());
        wnd->MoveWindow(&old_rect);

        window_resized = true;
    }
    (GameLoop::GetApp())->get_game()->draw_map(*pDC);
    // TODO: add draw code for native data here
}


// COOPacManView diagnostics

#ifdef _DEBUG
void COOPacManView::AssertValid() const
{
    CMFCMDView::AssertValid();
}

void COOPacManView::Dump(CDumpContext& dc) const
{
    CMFCMDView::Dump(dc);
}

Game* COOPacManView::GetDocument() const // non-debug version is inline
{
    ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Game)));
    return (Game*)m_pDocument;
}
#endif //_DEBUG


COOPacManView * COOPacManView::GetView()
{
    CFrameWnd * pFrame = (CFrameWnd *)(AfxGetApp()->m_pMainWnd);

    CView * pView = pFrame->GetActiveView();

    if ( !pView )
        return NULL;

    // Fail if view is of wrong kind
    // (this could occur with splitter windows, or additional
    // views on a single document
    if ( ! pView->IsKindOf( RUNTIME_CLASS(COOPacManView) ) )
        return NULL;

    return (COOPacManView *) pView;
}

// COOPacManView message handlers
