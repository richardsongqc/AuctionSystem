#pragma once

// CClientSocket command target
class CAuctionServerDoc;

class CClientSocket : public CSocket
{
public:
	CClientSocket(CAuctionServerDoc *pDoc);
	virtual ~CClientSocket();
	CClientSocket(const CClientSocket& client);
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Attributes
public:
	int m_nMsgCount;
	CSocketFile* m_pFile;
	CArchive* m_pArchiveIn;
	CArchive* m_pArchiveOut;
	BOOL IsAborted() { return m_pArchiveOut == NULL; }


	// Operations
public:
	void Init();
	void Abort();
	//void SendMsg(CMsg* pMsg);
	//void ReceiveMsg(CMsg* pMsg);

	// Overridable callbacks
protected:
	virtual void OnReceive(int nErrorCode);
	CAuctionServerDoc* m_pDoc;
};


