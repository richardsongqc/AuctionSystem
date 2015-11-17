
// AuctionClientView.cpp : implementation of the CAuctionClientView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "AuctionClient.h"
#endif

#include "AuctionClientDoc.h"
#include "AuctionClientView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAuctionClientView

IMPLEMENT_DYNCREATE(CAuctionClientView, CFormView)

BEGIN_MESSAGE_MAP(CAuctionClientView, CFormView)
END_MESSAGE_MAP()

// CAuctionClientView construction/destruction

CAuctionClientView::CAuctionClientView()
	: CFormView(CAuctionClientView::IDD),
	m_pLogonDlg(NULL)
{
	// TODO: add construction code here

}

CAuctionClientView::~CAuctionClientView()
{
	if (m_pLogonDlg)
	{
		m_pLogonDlg->DestroyWindow();

		delete m_pLogonDlg;
	}
}

void CAuctionClientView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BOOL CAuctionClientView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CAuctionClientView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	CAuctionClientDoc * pDoc = GetDocument();
	pDoc->ConnectSocket(L"127.0.0.1", 10000);

	if (m_pLogonDlg == NULL)
	{
		m_pLogonDlg = new CLogonDlg(this);
		m_pLogonDlg->Create(CLogonDlg::IDD);
	}
	else
	{
		m_pLogonDlg->DestroyWindow();
		delete m_pLogonDlg;

		m_pLogonDlg = new CLogonDlg(this);
		m_pLogonDlg->Create(CLogonDlg::IDD);
	}

	m_pLogonDlg->ShowWindow(SW_SHOW);
}


// CAuctionClientView diagnostics

#ifdef _DEBUG
void CAuctionClientView::AssertValid() const
{
	CFormView::AssertValid();
}

void CAuctionClientView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CAuctionClientDoc* CAuctionClientView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAuctionClientDoc)));
	return (CAuctionClientDoc*)m_pDocument;
}
#endif //_DEBUG


// CAuctionClientView message handlers
