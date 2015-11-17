// ClientSocket.cpp : implementation file
//

#include "stdafx.h"
#include "AuctionClient.h"
#include "ClientSocket.h"
#include "AuctionClientDoc.h"


// CClientSocket

IMPLEMENT_DYNAMIC(CClientSocket, CSocket)

CClientSocket::CClientSocket(CAuctionClientDoc* pDoc)
{
	m_pDoc = pDoc;
}

void CClientSocket::OnReceive(int nErrorCode)
{
	CSocket::OnReceive(nErrorCode);

	m_pDoc->ProcessPendingRead();
}



CClientSocket::~CClientSocket()
{
}


// CClientSocket member functions
