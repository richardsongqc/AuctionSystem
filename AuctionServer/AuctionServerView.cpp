
// AuctionServerView.cpp : implementation of the CAuctionServerView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "AuctionServer.h"
#endif
#include "MainFrm.h"
#include "AuctionServerDoc.h"
#include "AuctionServerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAuctionServerView

IMPLEMENT_DYNCREATE(CAuctionServerView, CListView)

BEGIN_MESSAGE_MAP(CAuctionServerView, CListView)
	ON_WM_CREATE()
END_MESSAGE_MAP()

// CAuctionServerView construction/destruction

CAuctionServerView::CAuctionServerView()
{
	// TODO: add construction code here

}

CAuctionServerView::~CAuctionServerView()
{
}

BOOL CAuctionServerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CListView::PreCreateWindow(cs);
}

void CAuctionServerView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();


	// TODO: You may populate your ListView with items by directly accessing
	//  its list control through a call to GetListCtrl().
	CListCtrl* lstCtrl = (CListCtrl*)&(this->GetListCtrl());

	lstCtrl->DeleteAllItems();

	DWORD dwExStyle = lstCtrl->GetExtendedStyle();
	CRect rcClient;
	GetClientRect(&rcClient);

	dwExStyle |= LVS_EX_GRIDLINES;
	//dwExStyle |= LVS_EX_MULTIWORKAREAS;

	lstCtrl->ModifyStyle(0, LVS_REPORT | 
		LVS_REPORT | 
		LVS_SINGLESEL | 
		LVS_SHOWSELALWAYS | 
		LVS_NOLABELWRAP | 
		LVS_AUTOARRANGE | 
		LVS_EDITLABELS | 
		LVS_ALIGNLEFT | 
		LVS_NOSORTHEADER );
	lstCtrl->SetExtendedStyle(dwExStyle );

	lstCtrl->InsertColumn(1, L"ID", LVCF_TEXT | LVCF_IMAGE | LVCFMT_FIXED_WIDTH, (int)(rcClient.Width() * 0.05) );
	lstCtrl->InsertColumn(2, L"Message", LVCF_TEXT | LVCFMT_FIXED_WIDTH, (int)(rcClient.Width() * 4 / 5));
	lstCtrl->InsertColumn(3, L"Debug", LVCF_TEXT | LVCFMT_FIXED_WIDTH, (int)(rcClient.Width() * 1 / 5));


	//int iColumnOrder = 0;
	//LVCOLUMNW columnInfo;
	//columnInfo.mask = LVS_REPORT | LVCF_TEXT | LVCFMT_FIXED_WIDTH;
	//columnInfo.pszText = L"ID";
	//columnInfo.fmt = LVCFMT_LEFT;
	//columnInfo.cchTextMax = 10;
	//columnInfo.cx = columnInfo.cchTextMax * 20;// rcClient.Width() * 0.05;
	//lstCtrl->InsertColumn(iColumnOrder, columnInfo.pszText);
	//iColumnOrder++;

	//columnInfo.pszText = L"Message";
	//columnInfo.cchTextMax = 1024;
	//columnInfo.cx = columnInfo.cchTextMax * 20;// rcClient.Width() * 0.5;
	//lstCtrl->InsertColumn(iColumnOrder, columnInfo.pszText);
	//iColumnOrder++;

	//columnInfo.pszText = L"Debug";
	//columnInfo.cchTextMax = 1024;
	//columnInfo.cx = columnInfo.cchTextMax * 20;// rcClient.Width() * 0.4;
	//lstCtrl->InsertColumn(iColumnOrder, columnInfo.pszText);
	//iColumnOrder++;

}


// CAuctionServerView diagnostics

#ifdef _DEBUG
void CAuctionServerView::AssertValid() const
{
	CListView::AssertValid();
}

void CAuctionServerView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CAuctionServerDoc* CAuctionServerView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAuctionServerDoc)));
	return (CAuctionServerDoc*)m_pDocument;
}
#endif //_DEBUG

CAuctionServerView * CAuctionServerView::GetView()
{
	CMainFrame * pFrame = (CMainFrame *)AfxGetMainWnd();
	if (!pFrame)
		return NULL;

	CView * pView = pFrame->GetActiveView();

	if (!pView)
		return NULL;

	// Fail if view is of wrong kind
	// (this could occur with splitter windows, or additional
	// views on a single document
	if (!pView->IsKindOf(RUNTIME_CLASS(CAuctionServerView)))
		return NULL;

	return (CAuctionServerView *)pView;
}

// CAuctionServerView message handlers


void CAuctionServerView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
	// TODO: Add your specialized code here and/or call the base class
	CAuctionServerDoc * pDoc = GetDocument();
	CMessageQueue<CString>& listMsg = pDoc->GetListMessage();

	CListCtrl* lstCtrl = (CListCtrl*)&(this->GetListCtrl());

	while (listMsg.IsEmpty() == false)
	{
		CString str = listMsg.Pop();
		int iIndex = lstCtrl->GetItemCount();
		CString strID = lstCtrl->GetItemText(iIndex, 0);
		int nID = _ttoi(strID);
		nID++;
		strID.Format(TEXT("%d"), nID);

		lstCtrl->InsertItem(iIndex, strID);
		lstCtrl->SetItemText(iIndex, 1, str);
	}

	while (lstCtrl->GetItemCount() > 300 )
	{
		lstCtrl->DeleteItem(0);
	}
}


int CAuctionServerView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here


	return 0;
}
