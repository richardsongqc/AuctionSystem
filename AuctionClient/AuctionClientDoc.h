
// AuctionClientDoc.h : interface of the CAuctionClientDoc class
//


#pragma once

#include "ClientSocket.h"

class CAuctionClientDoc : public CDocument
{
protected: // create from serialization only
	CAuctionClientDoc();
	DECLARE_DYNCREATE(CAuctionClientDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CAuctionClientDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	BOOL ConnectSocket(CString strAddress, UINT nPort);
	void ProcessPendingRead();
	void ReceiveMsg();

	CString m_strUserID;
	CString m_strUserPassword;
	CString m_strUserName;

	CClientSocket* GetSocket();

	int SendRequest(CBuffer inBuf, CBuffer outBuf);
protected:
	CClientSocket* m_pSocket;

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS

};

extern CAuctionClientDoc * GetCurrentDoc();