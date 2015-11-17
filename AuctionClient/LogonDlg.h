#pragma once
#include "afxwin.h"


// CLogonDlg dialog

class CLogonDlg : public CDialog
{
	DECLARE_DYNAMIC(CLogonDlg)

public:
	CLogonDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLogonDlg();

// Dialog Data
	enum { IDD = IDD_LOGONDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_edtPassword;
	CEdit m_edtUserID;
//	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedOk();
};
