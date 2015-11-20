
// AuctionClientDoc.cpp : implementation of the CAuctionClientDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "AuctionClient.h"
#endif
#include "MainFrm.h"
#include <ListMessage.h>
#include "AuctionClientDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CMessageQueue<CBuffer> msgRequestQueue;
CMessageQueue<CBuffer> msgResponseQueue;

CAuctionClientDoc * GetCurrentDoc()
{
	CMainFrame * pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	if (pFrame == NULL)
	{
		return NULL;
	}

	CAuctionClientDoc *pDoc = (CAuctionClientDoc*)pFrame->GetDocument();
	return pDoc;
}

// CAuctionClientDoc

IMPLEMENT_DYNCREATE(CAuctionClientDoc, CDocument)

BEGIN_MESSAGE_MAP(CAuctionClientDoc, CDocument)
END_MESSAGE_MAP()


// CAuctionClientDoc construction/destruction

CAuctionClientDoc::CAuctionClientDoc()
{
	// TODO: add one-time construction code here

}

CAuctionClientDoc::~CAuctionClientDoc()
{
}

BOOL CAuctionClientDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

    msgRequestQueue.Clean();
    msgResponseQueue.Clean();

    m_pSocket = new CClientSocket(this);

    if (!m_pSocket->Create())
    {
        delete m_pSocket;
        m_pSocket = NULL;

        return FALSE;
    }

	return TRUE;
}




// CAuctionClientDoc serialization

void CAuctionClientDoc::Serialize(CArchive& ar)
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
void CAuctionClientDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
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
void CAuctionClientDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CAuctionClientDoc::SetSearchContent(const CString& value)
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

// CAuctionClientDoc diagnostics

#ifdef _DEBUG
void CAuctionClientDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CAuctionClientDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CAuctionClientDoc commands
CClientSocket* CAuctionClientDoc::GetSocket()
{
	return m_pSocket;
}


int CAuctionClientDoc::SendRequest(CBuffer inBuf, CBuffer outBuf)
{
	inBuf.Send(m_pSocket);
	while (true)
	{
		int nLen = outBuf.Receive(m_pSocket);
		if (nLen > 0)
		{
			break;
		}
	}

	return 0;
}

BOOL CAuctionClientDoc::ConnectSocket(CString strAddress, UINT nPort)
{
	//m_strHandle = strHandle;



	while (!m_pSocket->Connect(strAddress, nPort + 1500)) // 700
	{
		DWORD    dwRet = GetLastError();

		TRACE(TEXT("Fail to connect the port(%d), Error code = 0x%08X\n"), nPort + 1500, dwRet);
		//if (AfxMessageBox(IDS_RETRYCONNECT, MB_YESNO) == IDNO)
		//{
		//	delete m_pSocket;
		//	m_pSocket = NULL;
		//	return FALSE;
		//}
	}

	//m_pFile = new CSocketFile(m_pSocket);
	//m_pArchiveIn = new CArchive(m_pFile, CArchive::load);
	//m_pArchiveOut = new CArchive(m_pFile, CArchive::store);


	//SendMsg(m_strHandle, SENDING_NICKNAME, false);

	//CString strTemp;
	//if (strTemp.LoadString(IDS_CONNECT))
	//	SendMsg(strTemp, NORMAL_MESSAGE, true);

	return TRUE;
}

void CAuctionClientDoc::ProcessPendingRead()
{
	//do
	//{
	//	ReceiveMsg();
	//	if (m_pSocket == NULL)
	//		return;
	//} 
	//while (!m_pArchiveIn->IsBufferEmpty());

    CBuffer     bufOutput;
    int         nReceived = bufOutput.Receive(m_pSocket);

    msgResponseQueue.Push(bufOutput);
}

void CAuctionClientDoc::ReceiveMsg()
{
	//CMsg msg;

	//TRY
	//{
	//	msg.Serialize(*m_pArchiveIn);

	//	if (msg.code == SENDING_CHATTERS_LIST)
	//	{
	//		UpdateChattersList(&msg);
	//	}

	//	while (!msg.m_msgList.IsEmpty())
	//	{
	//		CString temp = msg.m_msgList.RemoveHead();
	//		DisplayMsg(temp);
	//	}

	//}
	//	CATCH(CFileException, e)
	//{
	//	msg.m_bClose = TRUE;
	//	m_pArchiveOut->Abort();

	//	CString strTemp;
	//	if (strTemp.LoadString(IDS_SERVERRESET))
	//		DisplayMsg(strTemp);
	//	if (strTemp.LoadString(IDS_CONNECTIONCLOSED))
	//		DisplayMsg(strTemp);
	//}
	//END_CATCH

	//	if (msg.m_bClose)
	//	{
	//		delete m_pArchiveIn;
	//		m_pArchiveIn = NULL;
	//		delete m_pArchiveOut;
	//		m_pArchiveOut = NULL;
	//		delete m_pFile;
	//		m_pFile = NULL;
	//		delete m_pSocket;
	//		m_pSocket = NULL;
	//	}
}