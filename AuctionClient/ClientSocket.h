#pragma once

// CClientSocket command target

class CAuctionClientDoc;

class CClientSocket : public CSocket
{
	DECLARE_DYNAMIC(CClientSocket);

public:
	CClientSocket(CAuctionClientDoc* pDoc);
	virtual ~CClientSocket();

	// Implementation
protected:
	virtual void OnReceive(int nErrorCode);

	CAuctionClientDoc *m_pDoc;
};


