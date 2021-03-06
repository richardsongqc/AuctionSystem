
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
#include "AuctionServerView.h"
#include "timer.h"
#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CMessageQueue<CBuffer> msgRequestQueue;
CMessageQueue<CBuffer> msgResponseQueue;
std::vector<CClientSocket*>  CAuctionServerDoc::m_listClient;
ClassDBConnection CAuctionServerDoc::m_dbConn;



const DWORD     WAITINGTIME = 300;

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

DWORD CAuctionServerDoc::m_dwAuctionID = 0;

// CAuctionServerDoc

IMPLEMENT_DYNCREATE(CAuctionServerDoc, CDocument)

BEGIN_MESSAGE_MAP(CAuctionServerDoc, CDocument)
END_MESSAGE_MAP()


// CAuctionServerDoc construction/destruction

CAuctionServerDoc::CAuctionServerDoc() : 
    m_stateAuction(E_NONE)
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

    //m_pThreadProcessRequestQueue = AfxBeginThread(
    //    ProcessRequestQueueThread,
    //    this);

    //m_pThreadProcessResponseQueue = AfxBeginThread(
    //    ProcessResponseQueueThread,
    //    this);

}

CAuctionServerDoc::~CAuctionServerDoc()
{
}

struct DocView
{
	DocView(CAuctionServerDoc* pDoc, CAuctionServerView* pView ) : 
		m_pDoc(pDoc),
		m_pView(pView)
	{
	}

	~DocView()
	{
	}

	DocView(const DocView& obj)
	{ 
		m_pDoc  = obj.m_pDoc ;
		m_pView = obj.m_pView;
	}

	DocView& operator = (const DocView& obj)
	{
		m_pDoc = obj.m_pDoc;
		m_pView = obj.m_pView;

		return *this;
	}

	CAuctionServerDoc*	m_pDoc ;
	CAuctionServerView* m_pView;
};

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

//	DocView docview(this, CAuctionServerView::GetView())


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
void CAuctionServerDoc::BroadcastBuffer( CBuffer buf)
{
    for (CClientSocket* client : m_listClient)
    {
        if (client->GetLogin() == false )
        {
            continue;
        }

        //if (client.GetUserID() == pSocket->GetUserID())
        //{
        //    continue;
        //}

        TRACE(L"Broadcast Return: %d", buf.Send(client) );
    }
}

void CAuctionServerDoc::UpdateClientName(CString strUserID, CString strUserName)
{
    for (CClientSocket* client : m_listClient)
    {
        if (client->GetUserID() == strUserID)
        {
            client->SetUserName(strUserName);
        }
    }
}

void CAuctionServerDoc::UpdateClientLogin(CString strUserID, bool bLogin)
{
    for (CClientSocket* client : m_listClient)
    {
        if (client->GetUserID() == strUserID)
        {
            client->SetLogin(bLogin);
        }
    }
}


void CAuctionServerDoc::UpdateClientUserID(CClientSocket * pSocket, CString strUserID)
{
    for (CClientSocket* client : m_listClient)
    {
        if (client == pSocket)
        {
            client->SetUserID(strUserID);
        }
    }
}

void CAuctionServerDoc::UpdateClientName(CClientSocket * pSocket, CString strUserName)
{
    for (CClientSocket* client : m_listClient)
    {
        if (client == pSocket)
        {
            client->SetUserName(strUserName);
        }
    }
}

void CAuctionServerDoc::UpdateClientLogin(CClientSocket * pSocket, bool bLogin)
{
    for (CClientSocket* client : m_listClient)
    {
        if (client == pSocket)
        {
            client->SetLogin(bLogin);
        }
    }
}

void CAuctionServerDoc::ProcessPendingAccept()
{
	CClientSocket* pSocket = new CClientSocket(this);

    sockaddr_in from;
    memset(&from, 0, sizeof(sockaddr_in));
    int fromlen = sizeof(from);
    CString str;

    if (m_pSocket->Accept(*pSocket, (struct sockaddr*)&from, &fromlen ))
	{
		m_listClient.push_back(pSocket);
        char szTemp[512] = { 0 };
        strcpy_s(szTemp, inet_ntoa(from.sin_addr));
        TCHAR szBuf[1024] = { 0 };
        int nLen = sizeof(szBuf);
        int nRetLen = MultiByteToWideChar(
            CP_ACP,
            0,
            szTemp,
            strlen(szTemp),
            szBuf,
            nLen);

		CString str(TEXT("A new connection is accepted!"));
		m_listMessage.Push(str);

        str.Format(TEXT("\tClient IP:%s"), szBuf);
        m_listMessage.Push(str);


        str.Format(TEXT("\tClient Port:%d"), from.sin_port);
        m_listMessage.Push(str);
	}
	else
	{
		delete pSocket;
	}

	UpdateAllViews(NULL);
}


void WINAPI CAuctionServerDoc::TimerAdvertisingProc(void* p)
{

}

void CALLBACK CAuctionServerDoc::TimerBidProc(void* p)
{

}

void CALLBACK CAuctionServerDoc::TimerAfterBidProc(void* p)
{
    CTime lastTxTime = GetRecentTransactionTime(m_dwAuctionID);
    CTime now = CTime::GetCurrentTime();
    CTimeSpan ts = now - lastTxTime;
    TRACE(L"\nDiff of Tx = %I64d\n", ts.GetTotalSeconds() );

    CBroadcastState state;
    if (ts.GetTotalSeconds() > WAITINGTIME)
    {
        state.SetState(E_NONE);
    }
    else
    {
        state.SetState(E_AUCTION);
    }

    BroadcastBuffer(state);
}

void CALLBACK CAuctionServerDoc::TimerAfterAdvertisingProc(void* p)
{
    m_dwAuctionID = GetAuctionID();
    m_dwAuctionID++;

    CBroadcastState state;
    state.SetState(E_AUCTION);

    BroadcastBuffer(state);
}

void CAuctionServerDoc::ProcessPendingRead(CClientSocket* pSocket)
{
	CBuffer buffer;
	int nLen = buffer.Receive(pSocket);
    CString str;
    switch (buffer.GetCmd())
    {
    case CMD_REGISTER_CLIENT:
        {
            CInRegisterClient* inBuf = (CInRegisterClient*)&buffer;
            CString strUserID = inBuf->GetUserID();
            CString strPassword = inBuf->GetUserPassword();

            CString strUserName;
            bool bValidUser = ValidateUser(strUserID, strPassword, strUserName);

            pSocket->SetUserID(strUserID);
            UpdateClientUserID(pSocket, strUserID);

            COutRegisterClient outBuf;
            outBuf.SetValid(bValidUser);
            if (bValidUser)
            {
                outBuf.SetUserName(strUserName);
                pSocket->SetUserName(strUserName);
                UpdateClientName(pSocket, strUserName);

                outBuf.SetLogin(CheckLogin(strUserID));
                UpdateClientLogin(pSocket, true);
            }

            outBuf.Send(pSocket);

            str.Format(TEXT("CMD_REGISTER_CLIENT") );
            m_listMessage.Push(str);

            str.Format(TEXT("\tUserID = %s, UserName = %s"), strUserID, strUserName);
            m_listMessage.Push(str);

            m_stateAuction = E_NONE;
        }
        break;
    case CMD_RETRIEVE_STOCK_OF_CLIENT:
        {
            CInRetrieveStock* inBuf = (CInRetrieveStock*)&buffer;
            CString strUserID = inBuf->GetUserID();

            std::vector<CProduct> & listProduct = GetListProduct(strUserID);

            COutRetrieveStock outBuf;
            outBuf.SetListProduct(listProduct);

            str.Format(TEXT("CMD_RETRIEVE_STOCK_OF_CLIENT") );
            m_listMessage.Push(str);

            for (CProduct product : listProduct)
            {
                str.Format(TEXT(">>>ProductID = %d, ProductCount = %d,Price = %f, Product Name = %s"),
                    product.GetProductID(),
                    product.GetCount(),
                    product.GetPrice(),
                    product.GetName());
                m_listMessage.Push(str);
            }

            outBuf.Send(pSocket);

            m_stateAuction = E_NONE;
        }
        break;
    case CMD_ADVERTISING:
        {
            str.Format(TEXT("CMD_ADVERTISING"));
            m_listMessage.Push(str);

            m_stateAuction = E_ADVERTISING;

            CInAdvertising* inBuf = (CInAdvertising*)&buffer;
            DWORD    dwProductID  = inBuf->GetProductID();
            CString  strName      = inBuf->GetProductName();
            DWORD    dwCount      = inBuf->GetProductCount();
            double   dblPrice     = inBuf->GetProductPrice();

            CBroadcastPrice buf;
            buf.SetProductID(dwProductID);
            buf.SetProductCount(dwCount);
            buf.SetProductPrice(dblPrice);
            buf.SetProductName(strName);

            str.Format(TEXT("Broadcast Advertising Event. (%s, %d, %f)"), strName, dwCount, dblPrice);
            m_listMessage.Push(str);

            // Broadcast this packet to the other clients
            BroadcastBuffer( buf);

            COutAdvertising outBuf;
            outBuf.SetState(m_stateAuction);

            outBuf.Send(pSocket);



            // *****************************************************************************
            // After 5 minutes, the Auction should be started and the bids will be allowed.

            
            int * p = NULL;

            timer.registerHandler(&TimerAdvertisingProc, p);
            timer.registerHandlerAfter(&TimerAfterAdvertisingProc, p);
            
            timer.setInterval(1);
            timer.SetPeriod(WAITINGTIME);
            timer.Start();
            


            
            //******************************************************************************
        }
        break;
    case CMD_BID:
        {
            str.Format(TEXT("CMD_BID"));
            m_listMessage.Push(str);

            CInAuction* inBuf = (CInAuction*)&buffer;
            double   m_dblPrice = inBuf->GetProductPrice();
            CString strUserID = inBuf->GetUserID();
            // Check the max bid price
            double dblMaxBidPrice = GetMaxBidPrice(m_dwAuctionID);
            //double dblMaxBidPrice = GetDBConn().GetMaxBidPrice(m_lAuctionID);
            
            CProduct product(
                inBuf->GetProductID(),
                inBuf->GetProductName(),
                inBuf->GetProductCount(),
                inBuf->GetProductPrice());

            COutAuction outBuf;

            m_stateAuction = E_AUCTION;
            if (m_dblPrice > dblMaxBidPrice)
            {
                str.Format(TEXT("The new biggest price(%f) come in"), m_dblPrice);
                m_listMessage.Push(str);

                m_mutex.lock();
                SetBidTransaction(
                    m_dwAuctionID,
                    strUserID,
                    product);
                m_mutex.unlock();

                CBroadcastPrice buf;
                buf.SetProductID(inBuf->GetProductID());
                buf.SetProductName(inBuf->GetProductName());
                buf.SetProductCount(inBuf->GetProductCount());
                buf.SetProductPrice(inBuf->GetProductPrice());

                BroadcastBuffer(buf);

                //CBroadcastState state;
                //state.SetState(E_AUCTION);
                //BroadcastBuffer(state);

                //*********************************************************
                //如果这个最高价格保持5分钟，这个拍卖就可以结束了。
                //实现起来，我们还需要一个线程去检查在这5分钟里，有没有新价格高于它。
                //*********************************************************

                int * p = NULL;
                timerBid.registerHandler(&TimerBidProc, p);
                timerBid.registerHandlerAfter(&TimerAfterBidProc, p);

                timerBid.setInterval(1);
                timerBid.SetPeriod(WAITINGTIME);
                timerBid.Start();

                outBuf.SetState(m_stateAuction);
                outBuf.Send(pSocket);
            }







        }
        break;
    }

    UpdateAllViews(NULL);

    //CAuctionServerView::GetView()->UpdateWindow();
}


void CAuctionServerDoc::ProcessInQueue()
{

}

DWORD CAuctionServerDoc::GetAuctionID()
{
    DWORD dwAuctionID = 0;
    try
    {
        CString strSQL;
        strSQL.Format(TEXT("SELECT MAX(AuctionID) AS MaxAuctionID FROM Auction;") );

        m_dbConn.ExecuteSql(strSQL);

        CDBVariant MaxAuctionID;

        if (m_dbConn.GetRecordCount() > 0)
        {
            while (!m_dbConn.m_Recordset->IsEOF())
            {
                m_dbConn.m_Recordset->GetFieldValue(TEXT("MaxAuctionID"), MaxAuctionID);
                dwAuctionID = MaxAuctionID.m_lVal;
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

    return dwAuctionID;
}

double CAuctionServerDoc::GetMaxBidPrice(DWORD dwAuctionID)
{
    double dblMaxPrice = 0;
    try
    {
        CString strSQL;
        strSQL.Format(TEXT("SELECT MAX(Price) AS BidPrice FROM Auction WHERE AuctionID = %d;"), dwAuctionID);

        m_dbConn.ExecuteSql(strSQL);

        CDBVariant BidPrice;

        if (m_dbConn.GetRecordCount() > 0)
        {
            while (!m_dbConn.m_Recordset->IsEOF())
            {
                m_dbConn.m_Recordset->GetFieldValue(TEXT("BidPrice"), BidPrice);
                dblMaxPrice = BidPrice.m_dblVal;
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

    return dblMaxPrice;
}

CTime CAuctionServerDoc::GetRecentTransactionTime(DWORD dwAuctionID)
{
    CTime time;
    try
    {
        CString strSQL;
        strSQL.Format(TEXT("SELECT MAX(TransactionTime) AS recentTime FROM Auction WHERE AuctionID = %d;"), dwAuctionID);

        m_dbConn.ExecuteSql(strSQL);

        CDBVariant recentTime;

        if (m_dbConn.GetRecordCount() > 0)
        {
            while (!m_dbConn.m_Recordset->IsEOF())
            {
                m_dbConn.m_Recordset->GetFieldValue(TEXT("recentTime"), recentTime);
                time = ConvTimeStruct(*recentTime.m_pdate); 
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

    return time;
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
				strUserName = (*Name.m_pstring);
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

std::vector<CProduct> CAuctionServerDoc::GetListProduct(CString strUserID)
{
    std::vector<CProduct> listProduct;

    listProduct.clear();

    try
    {
        CString strSQL;
        strSQL.Format(TEXT("SELECT ProductCategory.CategoryID as CategoryID, ProductCategory.CategoryName as CategoryName, Product.Price as Price, User.Name as UserName FROM (Product INNER JOIN [User] ON Product.UserID = User.UserID) INNER JOIN ProductCategory ON Product.ProductID = ProductCategory.CategoryID  WHERE User.LoginName = '%s'"), strUserID);

        m_dbConn.ExecuteSql(strSQL);

        CDBVariant Name;

        if (m_dbConn.GetRecordCount() > 0)
        {
            CDBVariant CategoryID, CategoryName, Price;
            while (!m_dbConn.m_Recordset->IsEOF())
            {
                m_dbConn.m_Recordset->GetFieldValue(TEXT("CategoryID"), CategoryID);
                m_dbConn.m_Recordset->GetFieldValue(TEXT("CategoryName"), CategoryName);
                m_dbConn.m_Recordset->GetFieldValue(TEXT("Price"), Price);
                
                CProduct product(CategoryID.m_lVal, (*CategoryName.m_pstring), 1, Price.m_dblVal);

                listProduct.push_back(product);

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

    return listProduct;
}


void CAuctionServerDoc::SetBidTransaction(
    DWORD dwAuctionID,
    CString strUserID,
    CProduct product)
{
    CString strTime;

    SYSTEMTIME sys;
    GetLocalTime(&sys);

    strTime.Format(TEXT("%4d-%02d-%02d %02d:%02d:%02d"),
        sys.wYear,
        sys.wMonth,
        sys.wDay,
        sys.wHour,
        sys.wMinute,
        sys.wSecond);

    try
    {
        m_dbConn.BeginTrans();



        CString strSQL;
        strSQL.Format(TEXT("INSERT INTO Auction(AuctionID, UserID, ProductID, Count, Price, TransactionTime ) VALUES( %d, '%s', %d, %d, %f, '%s');"),
            dwAuctionID,
            strUserID, 
            product.GetProductID(),
            product.GetCount(),
            product.GetPrice(),
            strTime);

        TRACE(L"%s\n", strSQL);
        m_dbConn.ExecuteSqlNonQuery(strSQL);

        m_dbConn.CommitTrans();
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

}

CMessageQueue<CString>& CAuctionServerDoc::GetListMessage()
{
	return m_listMessage;
}

void CAuctionServerDoc::UpdateAllView()
{
	this->UpdateAllViews(NULL);
}

CAuctionServerDoc * CAuctionServerDoc::GetDoc()
{
	CMainFrame * pFrame = (CMainFrame *)AfxGetMainWnd();
	if (!pFrame)
		return NULL;
	return (CAuctionServerDoc *)pFrame->GetActiveDocument();
}

bool CAuctionServerDoc::CheckLogin(CString strUserID)
{
    bool bRet = false;
    for (CClientSocket *pSocket : m_listClient )
    {
        if (pSocket->GetUserID() == strUserID && pSocket->GetLogin() == true)
        {
            bRet = true;
            break;
        }
    }

    return bRet;
}

///////////////////////////////////////////////////////////////////////////////

//UINT /*CAuctionServerDoc::*/ProcessRequestQueueThread(LPVOID pParam)
//{
//	CAuctionServerDoc * pDoc = (CAuctionServerDoc*)pParam;
//
//	while (true)
//	{
//		if (msgRequestQueue.IsEmpty() == true)
//		{
//			continue;
//		}
//
//		CBuffer buffer = msgRequestQueue.Pop();
//
//		
//		CMessageQueue<CString>& listMessage = pDoc->GetListMessage();
//		CString str;
//		switch (buffer.GetCmd())
//		{
//		case CMD_REGISTER_CLIENT:
//			{
//				CInRegisterClient* inBuf = (CInRegisterClient*)&buffer;
//				CString strUserID = inBuf->GetUserID();
//				CString strPassword = inBuf->GetUserPassword();
//
//				CString strUserName;
//				bool bValidUser = pDoc->ValidateUser(strUserID, strPassword, strUserName);
//
//                CClientSocket * pSocket = (CClientSocket*)buffer.GetSocket();
//                pSocket->SetUserID(strUserID);
//           
//				COutRegisterClient outBuf;
//				outBuf.SetState(bValidUser);
//				if (bValidUser)
//				{
//					outBuf.SetUserName(strUserName);
//                    pSocket->SetUserName(strUserName);
//                    outBuf.SetLogin(pDoc->CheckLogin(strUserID));
//				}
//
//				msgResponseQueue.Push(outBuf);
//				str.Format(TEXT("UserID = %s, UserName = %s"), strUserID, strUserName);
//			
//				listMessage.Push(str);
//			}
//			break;
//		case CMD_RETRIEVE_STOCK_OF_CLIENT:
//			{
//			}
//			break;
//		case CMD_ADVERTISING:
//			{
//			}
//			break;
//		case CMD_BID:
//			{
//			}
//			break;
//		}
//
//		//pDoc->UpdateAllViews(NULL);
//		
//		//CAuctionServerView::GetView()->UpdateWindow();
//	}
//
//
//	
//
//	return 0;
//}
//
//UINT /*CAuctionServerDoc::*/ProcessResponseQueueThread(LPVOID pParam)
//{
//	CAuctionServerDoc * pDoc = (CAuctionServerDoc*)pParam;
//
//	while (true)
//	{
//		if (msgResponseQueue.IsEmpty() == true)
//		{
//			continue;
//		}
//
//		
//
//		CBuffer buffer = msgResponseQueue.Pop();
//		buffer.Send(pDoc->m_pSocket);
//
//		//pDoc->UpdateAllViews(NULL);
//		//CAuctionServerView::GetView()->UpdateWindow();
//	}
//
//	return 0;
//}