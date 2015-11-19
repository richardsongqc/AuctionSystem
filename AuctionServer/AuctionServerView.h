
// AuctionServerView.h : interface of the CAuctionServerView class
//

#pragma once


class CAuctionServerView : public CListView
{
protected: // create from serialization only
	CAuctionServerView();
	DECLARE_DYNCREATE(CAuctionServerView)

// Attributes
public:
	CAuctionServerDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // called first time after construct

// Implementation
public:
	virtual ~CAuctionServerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	static CAuctionServerView * GetView();
protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};

#ifndef _DEBUG  // debug version in AuctionServerView.cpp
inline CAuctionServerDoc* CAuctionServerView::GetDocument() const
   { return reinterpret_cast<CAuctionServerDoc*>(m_pDocument); }
#endif

