#pragma once

#include "ClientSocket.h"
#include "ListeningSocket.h"
#include "ClassDBConnection.h"
#include "ListMessage.h"
#include "Buffer.h"


extern CMessageQueue<CBuffer> msgRequestQueue;
extern CMessageQueue<CBuffer> msgResponseQueue;
// CAuctionServerApp

class CAuctionServerApp : public CWinApp
{
	DECLARE_DYNCREATE(CAuctionServerApp)

public:
	CAuctionServerApp();           // protected constructor used by dynamic creation
	virtual ~CAuctionServerApp();

	virtual BOOL InitInstance();
	virtual int ExitInstance();

	void ProcessPendingAccept();
	void ProcessPendingRead(CClientSocket* pSocket);

	void ProcessInQueue();

	bool ValidateUser(CString strUserID, CString strPassword, CString& strUserName);

	static UINT ProcessRequestQueueThread(LPVOID pParam);
	static UINT ProcessResponseQueueThread(LPVOID pParam);
protected:
	DECLARE_MESSAGE_MAP()


	CListeningSocket* m_pSocket;
	std::vector<CClientSocket>  m_listClient;

	ClassDBConnection m_dbConn;

	CWinThread	*	m_pThreadProcessRequestQueue;
	CWinThread	*	m_pThreadProcessResponseQueue;
	CMutex *		m_pMutex;

};


