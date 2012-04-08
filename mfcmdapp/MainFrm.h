// MainFrm.h : interface of the CMFCMDMainFrame class
//


#pragma once

namespace MDS
{

class CMFCMDMainFrame : public CFrameWnd
{

protected: // create from serialization only
    CMFCMDMainFrame();
    DECLARE_DYNCREATE(CMFCMDMainFrame)

    LRESULT CMFCMDMainFrame::WindowProc(UINT msg, WPARAM wp, LPARAM lp);

public:
    virtual ~CMFCMDMainFrame();
};

}
