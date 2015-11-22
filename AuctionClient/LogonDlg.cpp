// LogonDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AuctionClient.h"
#include "LogonDlg.h"
#include "afxdialogex.h"
#include "AuctionClientDoc.h"

// CLogonDlg dialog

IMPLEMENT_DYNAMIC(CLogonDlg, CDialog)

CLogonDlg::CLogonDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLogonDlg::IDD, pParent)
{

}

CLogonDlg::~CLogonDlg()
{
}

void CLogonDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_PASSWORD, m_edtPassword);
	DDX_Control(pDX, IDC_EDIT_USER_ID, m_edtUserID);
}


BEGIN_MESSAGE_MAP(CLogonDlg, CDialog)
	//ON_BN_CLICKED(IDOK, &CLogonDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDOK, &CLogonDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CLogonDlg message handlers


//void CLogonDlg::OnBnClickedOk()
//{
//	// TODO: Add your control notification handler code here
//	CDialog::OnOK();
//}


void CLogonDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CString strUserID;
	CString strUserPassword;

	m_edtUserID.GetWindowText(strUserID);
	m_edtPassword.GetWindowText(strUserPassword);

	CAuctionClientDoc * pDoc = GetCurrentDoc();
	if (pDoc == NULL)
	{
		return;
	}

	pDoc->m_strUserID = strUserID;
	pDoc->m_strUserPassword = strUserPassword;

	CInRegisterClient inBuf;
	inBuf.SetUserID(strUserID);
	inBuf.SetUserPassword(strUserPassword);

	//COutRegisterClient outBuf;
	//pDoc->SendRequest(inBuf, outBuf);

	//bool bValid = outBuf.GetState();
	//if (bValid)
	//{
	//	pDoc->m_strUserName = outBuf.GetUserName();
	//}



	CDialog::OnOK();
}
