// ClientSocket.cpp : implementation file
//

#include "stdafx.h"
#include "AuctionServer.h"
#include "ClientSocket.h"


// CClientSocket

CClientSocket::CClientSocket()
{
}

CClientSocket::CClientSocket(const CClientSocket& client)
{

}

//CClientSocket::~CClientSocket()
//{
//}


// CClientSocket member functions
void CClientSocket::Init()
{
	m_pFile = new CSocketFile(this);
	m_pArchiveIn = new CArchive(m_pFile, CArchive::load);
	m_pArchiveOut = new CArchive(m_pFile, CArchive::store);
}

void CClientSocket::Abort()
{
	if (m_pArchiveOut != NULL)
	{
		m_pArchiveOut->Abort();
		delete m_pArchiveOut;
		m_pArchiveOut = NULL;
	}
}

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

	theApp.ProcessPendingRead(this);
}

/////////////////////////////////////////////////////////////////////////////
// CSocket Implementation

CClientSocket::~CClientSocket()
{
	if (m_pArchiveOut != NULL)
		delete m_pArchiveOut;

	if (m_pArchiveIn != NULL)
		delete m_pArchiveIn;

	if (m_pFile != NULL)
		delete m_pFile;
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
