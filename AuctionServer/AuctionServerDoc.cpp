
// AuctionServerDoc.cpp : implementation of the CAuctionServerDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "AuctionServer.h"
#endif
#include "MainFrm.h"
#include "ClientSocket.h"
#include "AuctionServerDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CMessageQueue<CBuffer> msgRequestQueue;
CMessageQueue<CBuffer> msgResponseQueue;

CAuctionServerDoc * GetCurrentDoc()
{
	CMainFrame * pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	if (pFrame == NULL)
	{
		return NULL;
	}

	CAuctionServerDoc *pDoc = (CAuctionServerDoc*)pFrame->GetDocument();
	return pDoc;
}

// CAuctionServerDoc

IMPLEMENT_DYNCREATE(CAuctionServerDoc, CDocument)

BEGIN_MESSAGE_MAP(CAuctionServerDoc, CDocument)
END_MESSAGE_MAP()


// CAuctionServerDoc construction/destruction

CAuctionServerDoc::CAuctionServerDoc()
{
	// TODO: add one-time construction code here
	msgRequestQueue.Clean();
	msgResponseQueue.Clean();

	CString strConn;

	CString strPath = GetCurAppPath();
	strPath += _T("\\Data\\Data.accdb");
	CString strPwd = _T("");
	strConn.Format(
		_T("DRIVER=MICROSOFT ACCESS DRIVER (*.mdb, *.accdb);DSN='';DBQ=%s;Uid=Admin;Pwd=%s;"),
		strPath,
		strPwd);

	m_dbConn.SetConnectionString(strConn);
	if (m_dbConn.Open() == false)
	{
		return;
	}

	m_pThreadProcessRequestQueue = AfxBeginThread(
		ProcessRequestQueueThread,
		this);

	m_pThreadProcessResponseQueue = AfxBeginThread(
		ProcessResponseQueueThread,
		this);
}

CAuctionServerDoc::~CAuctionServerDoc()
{
}

BOOL CAuctionServerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	m_pSocket = new CListeningSocket(this);
	if (m_pSocket->Create( 10000 + 1500)) // 700
	{
		if (m_pSocket->Listen())
			return TRUE;
	}



	return FALSE;
}




// CAuctionServerDoc serialization

void CAuctionServerDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CAuctionServerDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CAuctionServerDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CAuctionServerDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CAuctionServerDoc diagnostics

#ifdef _DEBUG
void CAuctionServerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CAuctionServerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CAuctionServerDoc commands

void CAuctionServerDoc::ProcessPendingAccept()
{
	CClientSocket* pSocket = new CClientSocket(this);

	if (m_pSocket->Accept(*pSocket))
	{
		pSocket->Init();
		m_listClient.push_back(*pSocket);
		CString str(TEXT("A new connection is accepted!"));
		m_listMessage.Push(str);
	}
	else
	{
		delete pSocket;
	}

	UpdateAllViews(NULL);
}

void CAuctionServerDoc::ProcessPendingRead(CClientSocket* pSocket)
{
	CBuffer buffer;
	int nLen = buffer.Receive(pSocket);

	msgRequestQueue.Push(buffer);
}

void CAuctionServerDoc::ProcessInQueue()
{

}

bool CAuctionServerDoc::ValidateUser(
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

CMessageQueue<CString>& CAuctionServerDoc::GetListMessage()
{
	return m_listMessage;
}

///////////////////////////////////////////////////////////////////////////////

UINT ProcessRequestQueueThread(LPVOID pParam)
{
	while (true)
	{
		if (msgRequestQueue.IsEmpty() == true)
		{
			continue;
		}

		CBuffer buffer = msgRequestQueue.Pop();

		CAuctionServerDoc * pDoc = GetCurrentDoc();
		CMessageQueue<CString>& listMessage = pDoc->GetListMessage();
		CString str;
		switch (buffer.GetCmd())
		{
		case CMD_REGISTER_CLIENT:
			{
				CInRegisterClient* inBuf = (CInRegisterClient*)&buffer;
				CString strUserID = inBuf->GetUserID();
				CString strPassword = inBuf->GetUserPassword();

				CString strUserName;
				bool bValidUser = pDoc->ValidateUser(strUserID, strPassword, strUserName);

				COutRegisterClient outBuf;
				outBuf.SetState(bValidUser);
				if (bValidUser)
				{
					outBuf.SetUserName(strUserName);
				}

				msgResponseQueue.Push(outBuf);
				str.Format(TEXT("UserID = %s, UserName = %s"), strUserID, strUserName);
			
				listMessage.Push(str);
			}
			break;
		case CMD_RETRIEVE_STOCK_OF_CLIENT:
			{
			}
			break;
		case CMD_ADVERTISING:
			{
			}
			break;
		case CMD_BID:
			{
			}
			break;
		}

		pDoc->UpdateAllViews(NULL);
	}


	

	return 0;
}

UINT ProcessResponseQueueThread(LPVOID pParam)
{
	while (true)
	{
		if (msgResponseQueue.IsEmpty() == true)
		{
			continue;
		}

		CAuctionServerDoc * pDoc = GetCurrentDoc();

		CBuffer buffer = msgResponseQueue.Pop();
		buffer.Send(GetCurrentDoc()->m_pSocket);

		//pDoc->UpdateAllViews(NULL);
	}

	return 0;
}