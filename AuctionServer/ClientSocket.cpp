// ClientSocket.cpp : implementation file
//

#include "stdafx.h"
#include "AuctionServer.h"
#include "AuctionServerDoc.h"
#include "ClientSocket.h"


// CClientSocket

CClientSocket::CClientSocket(CAuctionServerDoc *pDoc) : 
	m_pDoc(pDoc),
    m_bLogin(false)
{
}


CClientSocket::CClientSocket(const CClientSocket& obj)
{
    m_pDoc        = obj.m_pDoc;
	m_strUserID	  = obj.m_strUserID	  ;
	m_strUserName = obj.m_strUserName ;
	m_bLogin      = obj.m_bLogin      ;
}

CClientSocket& CClientSocket::operator = (const CClientSocket& obj)
{
    m_pDoc = obj.m_pDoc;
    m_strUserID = obj.m_strUserID;
    m_strUserName = obj.m_strUserName;
    m_bLogin = obj.m_bLogin;

    return *this;
}

// CClientSocket member functions

//void CClientSocket::SendMsg(CMsg* pMsg)
//{
//	if (m_pArchiveOut != NULL)
//	{
//		pMsg->Serialize(*m_pArchiveOut);
//		m_pArchiveOut->Flush();
//	}
//}
//
//void CClientSocket::ReceiveMsg(CMsg* pMsg)
//{
//	pMsg->Serialize(*m_pArchiveIn);
//}

/////////////////////////////////////////////////////////////////////////////
// CClientSocket Overridable callbacks

void CClientSocket::OnReceive(int nErrorCode)
{
	CSocket::OnReceive(nErrorCode);

	m_pDoc->ProcessPendingRead(this);
}

/////////////////////////////////////////////////////////////////////////////
// CSocket Implementation

CClientSocket::~CClientSocket()
{
}

#ifdef _DEBUG
void CClientSocket::AssertValid() const
{
	CSocket::AssertValid();
}

void CClientSocket::Dump(CDumpContext& dc) const
{
	CSocket::Dump(dc);
}
#endif //_DEBUG

CString	 CClientSocket::GetUserID()	
{ 
	return m_strUserID; 
}

CString	 CClientSocket::GetUserName()	
{ 
	return m_strUserName; 
}

bool	 CClientSocket::GetLogin()	
{ 
	return m_bLogin; 
}

void     CClientSocket::SetUserID(CString strUserID) 
{ 
	m_strUserID = strUserID; 
}
void     CClientSocket::SetUserName(CString strUserName) 
{
	m_strUserName = strUserName; 
}

void     CClientSocket::SetLogin(bool	  bLogin) 
{ 
	m_bLogin = bLogin; 
}