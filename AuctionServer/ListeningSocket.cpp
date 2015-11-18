// ListeningSocket.cpp : implementation file
//

#include "stdafx.h"
#include "AuctionServer.h"
#include "ListeningSocket.h"
#include "AuctionServerDoc.h"


// CListeningSocket
IMPLEMENT_DYNAMIC(CListeningSocket, CSocket)

CListeningSocket::CListeningSocket(CAuctionServerDoc* pDoc) : m_pDoc(pDoc)
{
}

CListeningSocket::~CListeningSocket()
{
}


/////////////////////////////////////////////////////////////////////////////
// CListeningSocket Overridable callbacks

void CListeningSocket::OnAccept(int nErrorCode)
{
	CSocket::OnAccept(nErrorCode);
	m_pDoc->ProcessPendingAccept();
}

/////////////////////////////////////////////////////////////////////////////
// CSocket Implementation

#ifdef _DEBUG
void CListeningSocket::AssertValid() const
{
	CSocket::AssertValid();
}

void CListeningSocket::Dump(CDumpContext& dc) const
{
	CSocket::Dump(dc);
}
#endif //_DEBUG
