
// AuctionClientView.h : interface of the CAuctionClientView class
//

#pragma once

#include "resource.h"
#include "LogonDlg.h"
#include "afxwin.h"
#include "afxcmn.h"

class CAuctionClientView : public CFormView
{
protected: // create from serialization only
	CAuctionClientView();
	DECLARE_DYNCREATE(CAuctionClientView)

public:
	enum{ IDD = IDD_AUCTIONCLIENT_FORM };

// Attributes
public:
	CAuctionClientDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct

// Implementation
public:
	virtual ~CAuctionClientView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
	CLogonDlg * m_pLogonDlg;
public:
    afx_msg void OnEditLogin();
    virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
    CStatic m_lblUserID;
    CStatic m_lblUserName;
    afx_msg void OnEditRetrieveStock();
    afx_msg void OnUpdateEditRetrieveStock(CCmdUI *pCmdUI);
    CListCtrl m_listStock;
    CButton m_btnAdvertise;
    afx_msg void OnBnClickedButtonAdvertising();
//    afx_msg void OnBnClickedButton2();
    afx_msg void OnBnClickedButtonPlaceBid();
    CEdit m_edtBid;
    CButton m_btnBid;
    void UpdateMode();
    CStatic m_lblCurrentBid;
    CStatic m_edtBidCount;
    CStatic m_edtBidName;
};

#ifndef _DEBUG  // debug version in AuctionClientView.cpp
inline CAuctionClientDoc* CAuctionClientView::GetDocument() const
   { return reinterpret_cast<CAuctionClientDoc*>(m_pDocument); }
#endif

