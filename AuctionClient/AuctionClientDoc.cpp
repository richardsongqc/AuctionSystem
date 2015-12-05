
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
#include <protocol.h>
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

CAuctionClientDoc::CAuctionClientDoc() : 
    m_eAuctionState(E_NONE),
    m_curProduct(0, L"", 0,0)
{
	// TODO: add one-time construction code here
    //m_pThreadProcessRequestQueue = AfxBeginThread(
    //    ProcessRequestQueueThread,
    //    this);

    //m_pThreadProcessResponseQueue = AfxBeginThread(
    //    ProcessResponseQueueThread,
    //    this);

    m_hEvtReceive = CreateEvent(NULL, FALSE, TRUE, NULL);

    //ResetEvent(m_hEvtReceive);
}

CAuctionClientDoc::~CAuctionClientDoc()
{
    CloseHandle(m_hEvtReceive);
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

int CAuctionClientDoc::SendBuffer(CBuffer inBuf)
{
    return inBuf.Send(m_pSocket);
}

int CAuctionClientDoc::SendRequest(CBuffer inBuf, CBuffer& outBuf)
{
	inBuf.Send(m_pSocket);

    DWORD dwRet = WaitForSingleObject(m_hEvtReceive, INFINITE);// No wait for object
    switch (dwRet)
    {
    case WAIT_OBJECT_0:
        {
            int nLen = outBuf.Receive(m_pSocket);
            if (nLen > 0)
            {

                break;
            }
        }
        break;

    default:
        break;
 
    }


    //ResetEvent(m_hEvtReceive);
	
    return 0;
}

void CAuctionClientDoc::SetValid(bool bValid)
{
    m_bValid = bValid;
}

bool CAuctionClientDoc::GetValid()
{
    return m_bValid;
}

void CAuctionClientDoc::SetLogin(bool bLogin)
{
    m_bLogin = bLogin;
}

bool CAuctionClientDoc::GetLogin()
{
    return m_bLogin;
}

void    CAuctionClientDoc::SetUserID(CString strUserID) 
{ 
    m_strUserID = strUserID; 
}

CString CAuctionClientDoc::GetUserID()                  
{ 
    return m_strUserID; 
}

void    CAuctionClientDoc::SetUserPassword(CString strPassword) 
{ 
    m_strUserPassword = strPassword; 
}

CString CAuctionClientDoc::GetUserPassword()                    
{ 
    return m_strUserPassword; 
}

void    CAuctionClientDoc::SetUserName(CString strUserName)    
{ 
    m_strUserName = strUserName; 
}

CString CAuctionClientDoc::GetUserName()                       
{
    return  m_strUserName; 
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

EAuctionState CAuctionClientDoc::GetAuctionState()
{
    return m_eAuctionState;
}

CProduct CAuctionClientDoc::GetCurrentProduct()
{
    return m_curProduct;
}

void CAuctionClientDoc::SetAuctionState(EAuctionState eAuctionState)
{
    m_eAuctionState = eAuctionState;
}

void CAuctionClientDoc::ProcessPendingRead()
{
    CBuffer     bufOutput;
    int         nReceived = bufOutput.Receive(m_pSocket);

    switch (bufOutput.GetCmd())
    {
    case RSP_REGISTER_CLIENT:
        {
            COutRegisterClient* outBuf = (COutRegisterClient*)&bufOutput;
            SetValid(outBuf->GetValid());
            SetUserName(outBuf->GetUserName());
            SetLogin(outBuf->GetLogin());

            m_eAuctionState = outBuf->GetState();
        }
        break;
    case RSP_RETRIEVE_STOCK_OF_CLIENT : 
        {
            COutRetrieveStock * outBuf = (COutRetrieveStock*)&bufOutput;
            m_listProduct = outBuf->GetListProduct();

            //m_eAuctionState = outBuf->GetState();
        }
        break;
    case RSP_ADVERTISING              : 
        {
            m_eAuctionState = E_ADVERTISING;
            
            COutAdvertising * outBuf = (COutAdvertising*)&bufOutput;
            m_eAuctionState = outBuf->GetState();

        }
        break;
    case RSP_BID                      : 
        {
            COutAdvertising * outBuf = (COutAdvertising*)&bufOutput;
            m_eAuctionState = outBuf->GetState();
        }
        break;
    case CMD_BROADCAST_PRICE          : 
        {
            CBroadcastPrice * outBuf = (CBroadcastPrice*)&bufOutput;
            m_curProduct.SetCount(outBuf->GetProductCount());
            m_curProduct.SetName(outBuf->GetProductName());
            m_curProduct.SetProductID(outBuf->GetProductID());
            m_curProduct.SetPrice(outBuf->GetProductPrice());
        }
        break;
    case CMD_BROADCAST_AUCTION_END    : 
        {
            CBroadcastState * outBuf = (CBroadcastState*)&bufOutput;
            m_eAuctionState = outBuf->GetState();
        }
        break;
    }

    UpdateAllViews(NULL);

    //msgResponseQueue.Push(bufOutput);
}

CMessageQueue<CString>& CAuctionClientDoc::GetListMessage()
{
    return m_listMessage;
}

std::vector<CProduct> CAuctionClientDoc::GetListProduct()
{
    return m_listProduct;
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


//UINT ProcessRequestQueueThread(LPVOID pParam)
//{
//    CAuctionClientDoc * pDoc = (CAuctionClientDoc*)pParam;
//
//    while (true)
//    {
//        if (msgRequestQueue.IsEmpty() == true)
//        {
//            continue;
//        }
//
//
//
//        CBuffer buffer = msgRequestQueue.Pop();
//        buffer.Send(pDoc->m_pSocket);
//
//        //pDoc->UpdateAllViews(NULL);
//        //CAuctionServerView::GetView()->UpdateWindow();
//    }
//
//    return 0;
//}
//
//UINT ProcessResponseQueueThread(LPVOID pParam)
//{
//    CAuctionClientDoc * pDoc = (CAuctionClientDoc*)pParam;
//
//    while (true)
//    {
//        if (msgResponseQueue.IsEmpty() == true)
//        {
//            continue;
//        }
//
//        CBuffer buffer = msgResponseQueue.Pop();
//
//
//        CMessageQueue<CString>& listMessage = pDoc->GetListMessage();
//        CString str;
//        switch (buffer.GetCmd())
//        {
//        case RSP_REGISTER_CLIENT:
//        {
//            COutRegisterClient* outBuf = (COutRegisterClient*)&buffer;
//
//            //CInRegisterClient* inBuf = (CInRegisterClient*)&buffer;
//            //CString strUserID = inBuf->GetUserID();
//            //CString strPassword = inBuf->GetUserPassword();
//            bool bValidUser = outBuf->GetState();
//            pDoc->m_bLogin = outBuf->GetLogin();
//            pDoc->m_strUserName = outBuf->GetUserName();
//
//            //CString strUserName;
//            //bool bValidUser = pDoc->ValidateUser(strUserID, strPassword, strUserName);
//
//            //CClientSocket * pSocket = (CClientSocket*)buffer.GetSocket();
//            //pSocket->SetUserID(strUserID);
//
//            //COutRegisterClient outBuf;
//            //outBuf.SetState(bValidUser);
//            //if (bValidUser)
//            //{
//            //    outBuf.SetUserName(strUserName);
//            //    pSocket->SetUserName(strUserName);
//            //    outBuf.SetLogin(pDoc->CheckLogin(strUserID));
//            //}
//
//            //msgResponseQueue.Push(outBuf);
//            //str.Format(TEXT("UserID = %s, UserName = %s"), strUserID, strUserName);
//
//            listMessage.Push(str);
//        }
//        break;
//        case RSP_RETRIEVE_STOCK_OF_CLIENT:
//        {
//        }
//        break;
//        case RSP_ADVERTISING:
//        {
//        }
//        break;
//        case RSP_BID:
//        {
//        }
//        break;
//        }
//
//        //pDoc->UpdateAllViews(NULL);
//
//        //CAuctionServerView::GetView()->UpdateWindow();
//    }
//
//
//    return 0;
//}