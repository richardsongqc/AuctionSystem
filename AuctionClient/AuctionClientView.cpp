
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
    ON_COMMAND(ID_EDIT_LOGIN, &CAuctionClientView::OnEditLogin)
    ON_COMMAND(ID_EDIT_RETRIEVE_STOCK, &CAuctionClientView::OnEditRetrieveStock)
    ON_UPDATE_COMMAND_UI(ID_EDIT_RETRIEVE_STOCK, &CAuctionClientView::OnUpdateEditRetrieveStock)
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
    DDX_Control(pDX, IDC_STATIC_USER_ID, m_lblUserID);
    DDX_Control(pDX, IDC_STATIC_USER_NAME, m_lblUserName);
    DDX_Control(pDX, IDC_LIST_STOCK, m_listStock);
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

    COutRetrieveStock outBuf;
    m_listStock.DeleteAllItems();

    DWORD dwExStyle = m_listStock.GetExtendedStyle();
    CRect rcClient;
    GetClientRect(&rcClient);

    dwExStyle |= LVS_EX_GRIDLINES;
    //dwExStyle |= LVS_EX_MULTIWORKAREAS;

    m_listStock.ModifyStyle(0, LVS_REPORT |
        LVS_REPORT |
        LVS_SINGLESEL |
        LVS_SHOWSELALWAYS |
        LVS_NOLABELWRAP |
        LVS_AUTOARRANGE |
        LVS_NOSCROLL ||
        LVS_ALIGNLEFT |
        LVS_NOSORTHEADER);
    m_listStock.SetExtendedStyle(dwExStyle);

    m_listStock.InsertColumn(1, L"ID", LVCF_TEXT | LVCF_IMAGE | LVCFMT_FIXED_WIDTH, (int)(rcClient.Width() * 0.05));
    m_listStock.InsertColumn(2, L"Name", LVCF_TEXT | LVCFMT_FIXED_WIDTH, (int)(rcClient.Width() * 0.13));
    m_listStock.InsertColumn(3, L"Count", LVCF_TEXT | LVCFMT_FIXED_WIDTH, (int)(rcClient.Width() * 0.08));
    m_listStock.InsertColumn(4, L"Count", LVCF_TEXT | LVCFMT_FIXED_WIDTH, (int)(rcClient.Width() * 0.18));







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


void CAuctionClientView::OnEditLogin()
{
    // TODO: Add your command handler code here

    m_pLogonDlg->ShowWindow(SW_SHOW);
}


void CAuctionClientView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
    // TODO: Add your specialized code here and/or call the base class
    CAuctionClientDoc * pDoc = GetDocument();

    if (m_pLogonDlg == NULL )
    {
        return;
    }

    CString str;
    if (m_pLogonDlg->IsWindowVisible() == TRUE)
    {
        if (pDoc->GetValid())
        {
            if (pDoc->GetLogin() == false)
            {
                m_pLogonDlg->ShowWindow(SW_HIDE);
            }

            //str = L"This user has already be logined. Please input again.";
        }
        else
        {
            str = L"User name and password are incorrect. Please input again.";
            AfxMessageBox(str);
        }

        
    }

    
    
    if (pDoc->GetValid() && pDoc->GetLogin() == false)
    {
        CString strUserID = pDoc->GetUserID();
        CString strUserName = pDoc->GetUserName();
        
        m_lblUserID.SetWindowText(strUserID);
        m_lblUserName.SetWindowText(strUserName);

        m_listStock.DeleteAllItems();

        std::vector<CProduct>& listProduct = pDoc->GetListProduct();

        int i = 0;
        for (CProduct product : listProduct)
        {
            CString str;
            str.Format(L"%d", product.GetProductID());
            m_listStock.InsertItem(i, str);

            str.Format(L"%s", product.GetName());
            m_listStock.SetItemText(i, 1, str);

            str.Format(L"%d", product.GetCount());
            m_listStock.SetItemText(i, 2, str);

            str.Format(L"%f", product.GetPrice());
            m_listStock.SetItemText(i, 3, str);

            i++;
        }
    }
}


void CAuctionClientView::OnEditRetrieveStock()
{
    // TODO: Add your command handler code here
    CAuctionClientDoc * pDoc = GetDocument();

    CInRetrieveStock inBuf;

    inBuf.SetUserID(pDoc->GetUserID());

    pDoc->SendBuffer(inBuf);






}


void CAuctionClientView::OnUpdateEditRetrieveStock(CCmdUI *pCmdUI)
{
    // TODO: Add your command update UI handler code here
}
