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

	class CProcessRequestQueueThread : public CWinThread
	{
	public:
		CProcessRequestQueueThread(void* pParent);
		~CProcessRequestQueueThread();

		virtual int Run();

	protected:
		void*	m_pParent;
	};

	class CProcessResponseQueueThread : public CWinThread
	{
	public:
		CProcessResponseQueueThread(void* hParent);
		~CProcessResponseQueueThread();

		virtual int Run();

	protected:
		void*	m_pParent;
	};

	
protected:
	DECLARE_MESSAGE_MAP()


	CListeningSocket* m_pSocket;
	std::vector<CClientSocket>  m_listClient;

	ClassDBConnection m_dbConn;

	CProcessRequestQueueThread *	m_pThreadProducer;
	CProcessResponseQueueThread *	m_pThreadConsumer;
//	CSemaphore*			m_pSemaphoreEmpty;
//	CSemaphore*			m_pSemaphoreFull;
	CMutex *						m_pMutex;

};


