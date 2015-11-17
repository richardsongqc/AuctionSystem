// ListeningSocket.cpp : implementation file
//

#include "stdafx.h"
#include "AuctionServer.h"
#include "ListeningSocket.h"


// CListeningSocket

CListeningSocket::CListeningSocket()
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
	theApp.ProcessPendingAccept();
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
