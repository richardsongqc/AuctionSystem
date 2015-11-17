// AuctionServerApp.cpp : implementation file
//

#include "stdafx.h"
#include "ClientSocket.h"
#include "AuctionServer.h"
#include "AuctionServerApp.h"
#include "Buffer.h"

const int g_nPort = 10000;


CMessageQueue<CBuffer> msgRequestQueue;
CMessageQueue<CBuffer> msgResponseQueue;

// CAuctionServerApp

IMPLEMENT_DYNCREATE(CAuctionServerApp, CWinApp)

CAuctionServerApp::CAuctionServerApp()
{
}

CAuctionServerApp::~CAuctionServerApp()
{
	if (m_pSocket == (CListeningSocket*)NULL)
	{
		return;
	}

	delete m_pSocket;
	m_pSocket = NULL;
}

BOOL CAuctionServerApp::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	msgRequestQueue.Clean();
	msgResponseQueue.Clean();

	CString strConn;
	CString strPath = _T("D:\\Richard\\Documents\\Visual Studio 2013\\Projects\\AuctionSystem\\Data\\Data.accdb");
	CString strPwd = _T("");
	strConn.Format(
		_T("DRIVER=MICROSOFT ACCESS DRIVER (*.mdb, *.accdb);DSN='';DBQ=%s;Uid=Admin;Pwd=%s;"), 
		strPath, 
		strPwd );

	m_dbConn.SetConnectionString(strConn);
	if (m_dbConn.Open() == false)
	{
		return FALSE;
	}

	m_pSocket = new CListeningSocket();
	if (m_pSocket->Create(g_nPort + 1500)) // 700
	{
		if (m_pSocket->Listen())
			return TRUE;
	}

	m_pThreadProducer = new CProcessRequestQueueThread(this);
	m_pThreadConsumer = new CProcessResponseQueueThread(this);

	m_pThreadProducer->CreateThread(CREATE_SUSPENDED);
	VERIFY(m_pThreadProducer->SetThreadPriority(THREAD_PRIORITY_IDLE));
	
	m_pThreadConsumer->CreateThread(CREATE_SUSPENDED);
	VERIFY(m_pThreadConsumer->SetThreadPriority(THREAD_PRIORITY_IDLE));
	
	TRACE("m_pThreadProducer In\n");
	m_pThreadProducer->ResumeThread();
	TRACE("m_pThreadConsumer Out\n");
	m_pThreadConsumer->ResumeThread();


	return TRUE;
}

int CAuctionServerApp::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinApp::ExitInstance();
}

BEGIN_MESSAGE_MAP(CAuctionServerApp, CWinApp)
END_MESSAGE_MAP()


void CAuctionServerApp::ProcessInQueue()
{

}

// CAuctionServerApp message handlers


void CAuctionServerApp::ProcessPendingAccept()
{
	CClientSocket* pSocket = new CClientSocket();

	if (m_pSocket->Accept(*pSocket))
	{
		pSocket->Init();
		m_listClient.push_back(*pSocket);
	}
	else
	{
		delete pSocket;
	}
		
}


void CAuctionServerApp::ProcessPendingRead(CClientSocket* pSocket)
{
	CBuffer buffer;
	int nLen = buffer.Receive(pSocket);

	msgRequestQueue.Push(buffer);
}

bool CAuctionServerApp::ValidateUser(
	CString strUserID, 
	CString strPassword, 
	CString& strUserName)
{
	bool bRet = false;

	try
	{
		CString strSQL;
		strSQL.Format(TEXT("SELECT * FROM USER WHERE LoginName = '%s' AND Password = '%s';"), strUserID, strPassword);

		m_dbConn.ExecuteSql(strSQL);

		CDBVariant Name;

		if (m_dbConn.GetRecordCount() > 0)
		{
			bRet = true;
			while (!m_dbConn.m_Recordset->IsEOF())
			{
				m_dbConn.m_Recordset->GetFieldValue(TEXT("Name"), Name);
				strUserName = (LPSTR)Name.m_pstring;
				m_dbConn.m_Recordset->MoveNext();
			}
		}
	}
	catch (CString e)
	{
		AfxMessageBox(e);
	}
	catch (CDBException* e)
	{
		AfxMessageBox(e->m_strError); 
		e->Delete();
	}

	return bRet;
}

///////////////////////////////////////////////////////////////////////////////


CAuctionServerApp::CProcessRequestQueueThread::CProcessRequestQueueThread(void* hParent) : 
	m_pParent(hParent)
{

}

CAuctionServerApp::CProcessRequestQueueThread::~CProcessRequestQueueThread()
{
}

int CAuctionServerApp::CProcessRequestQueueThread::Run()
{
	while (true)
	{
		if (msgRequestQueue.IsEmpty() == true )
		{
			continue;
		}

		CBuffer buffer = msgRequestQueue.Pop();

		CAuctionServerApp * pParent = (CAuctionServerApp*)m_pParent;
		switch (buffer.GetCmd())
		{
		case CMD_REGISTER_CLIENT:
			{
				CInRegisterClient* inBuf = (CInRegisterClient*)&buffer;
				CString strUserID = inBuf->GetUserID();
				CString strPassword = inBuf->GetUserPassword();

				CString strUserName;
				bool bValidUser = pParent->ValidateUser(strUserID, strPassword, strUserName);

				COutRegisterClient outBuf;
				outBuf.SetState(bValidUser);
				if (bValidUser)
				{
					outBuf.SetUserName(strUserName);
				}

				msgResponseQueue.Push(outBuf);
			}
			break; 
		case CMD_RETRIEVE_STOCK_OF_CLIENT : 
			{
			}
			break;
		case CMD_ADVERTISING			  : 
			{
			}
			break;
		case CMD_BID					  : 
			{
			}
			break;
		}
	}




	return CWinThread::Run();
}

CAuctionServerApp::CProcessResponseQueueThread::CProcessResponseQueueThread(void *pParent) : 
	m_pParent(pParent)
{

}

CAuctionServerApp::CProcessResponseQueueThread::~CProcessResponseQueueThread()
{
}

int CAuctionServerApp::CProcessResponseQueueThread::Run()
{
	while (true)
	{
		if (msgResponseQueue.IsEmpty() == true)
		{
			continue;
		}

		CBuffer buffer = msgResponseQueue.Pop();
		CAuctionServerApp * pParent = (CAuctionServerApp*)m_pParent;
		buffer.Send(pParent->m_pSocket);

	}

	return CWinThread::Run();
}