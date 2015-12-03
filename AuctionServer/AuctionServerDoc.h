
// AuctionServerDoc.h : interface of the CAuctionServerDoc class
//


#pragma once

#include "ClientSocket.h"
#include "ListeningSocket.h"
#include "ClassDBConnection.h"
#include <ListMessage.h>
#include "Buffer.h"
#include "timer.h"

extern CMessageQueue<CBuffer> msgRequestQueue;
extern CMessageQueue<CBuffer> msgResponseQueue;




class CAuctionServerDoc : public CDocument
{
protected: // create from serialization only
	CAuctionServerDoc();
	DECLARE_DYNCREATE(CAuctionServerDoc)

// Attributes
public:

// Operations
public:
	void ProcessPendingAccept();
	void ProcessPendingRead(CClientSocket* pSocket);

	void ProcessInQueue();
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
	virtual ~CAuctionServerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
    void BroadcastBuffer(CClientSocket * pSocket, CBuffer buf);


	bool ValidateUser(CString strUserID, CString strPassword, CString& strUserName);
    std::vector<CProduct> GetListProduct(CString strUserID);
    DWORD GetAuctionID();

	//friend UINT ProcessRequestQueueThread(LPVOID pParam);
	//friend UINT ProcessResponseQueueThread(LPVOID pParam);

	CMessageQueue<CString>& GetListMessage();

	static CAuctionServerDoc * GetDoc();
protected:
	CListeningSocket* m_pSocket;
	std::vector<CClientSocket>  m_listClient;
    CMessageQueue<CString>	m_listMessage;

	ClassDBConnection m_dbConn;

	//CWinThread	*	m_pThreadProcessRequestQueue;
	//CWinThread	*	m_pThreadProcessResponseQueue;
	//CMutex *		m_pMutex;

	void UpdateAllView();
// Generated message map functions
protected:
    static DWORD    m_dwAuctionID;
    EAuctionState   m_stateAuction;

    bool CheckLogin(CString strUserID);
    //void WINAPI TimerProc(void* p);

	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};

extern CAuctionServerDoc * GetCurrentDoc();