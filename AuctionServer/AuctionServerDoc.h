
// AuctionServerDoc.h : interface of the CAuctionServerDoc class
//


#pragma once

#include "ClientSocket.h"
#include "ListeningSocket.h"
#include "ClassDBConnection.h"
#include <ListMessage.h>
#include "Buffer.h"
#include "timer.h"
#include <boost\\thread\\mutex.hpp>

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
    static void BroadcastBuffer( CBuffer buf);

    ////////////////////////////////////////////////////////////////////////////////
    // Datebase
	static bool ValidateUser(CString strUserID, CString strPassword, CString& strUserName);
    static std::vector<CProduct> GetListProduct(CString strUserID);
    static DWORD GetAuctionID();
    static void SetBidTransaction(DWORD dwAuctionID, CString strUserID, CProduct product);
    static double GetMaxBidPrice(DWORD dwAuctionID);
    static CTime GetRecentTransactionTime(DWORD dwAuctionID);

    /////////////////////////////////////////////////////////////////////////////////////

	//friend UINT ProcessRequestQueueThread(LPVOID pParam);
	//friend UINT ProcessResponseQueueThread(LPVOID pParam);

	CMessageQueue<CString>& GetListMessage();


	static CAuctionServerDoc * GetDoc();
protected:
	CListeningSocket* m_pSocket;
	static std::vector<CClientSocket*>  m_listClient;
    CMessageQueue<CString>	m_listMessage;

	static ClassDBConnection m_dbConn;

	//CWinThread	*	m_pThreadProcessRequestQueue;
	//CWinThread	*	m_pThreadProcessResponseQueue;
	//CMutex *		m_pMutex;

	void UpdateAllView();

    static void CALLBACK TimerAdvertisingProc(void* p);
    static void CALLBACK TimerAfterAdvertisingProc(void* p);
    static void CALLBACK TimerBidProc(void* p);
    static void CALLBACK TimerAfterBidProc(void* p);
    static DWORD    m_dwAuctionID;

    void UpdateClientName(  CString strUserID, CString strUserName);
    void UpdateClientLogin( CString strUserID, bool bLogin );

    void UpdateClientUserID(CClientSocket * pSocket, CString strUserID  );
    void UpdateClientName  (CClientSocket * pSocket, CString strUserName);
    void UpdateClientLogin (CClientSocket * pSocket, bool bLogin        );
// Generated message map functions
protected:
    
    EAuctionState   m_stateAuction;
    boost::mutex m_mutex;
    bool CheckLogin(CString strUserID);
    //void WINAPI TimerProc(void* p);
    Timer timer;
    Timer timerBid;

	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};

extern CAuctionServerDoc * GetCurrentDoc();