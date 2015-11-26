
// AuctionClientDoc.h : interface of the CAuctionClientDoc class
//


#pragma once
#include "..\\AuctionServer\\Buffer.h"
#include <ListMessage.h>
#include "ClientSocket.h"

extern CMessageQueue<CBuffer> msgRequestQueue;
extern CMessageQueue<CBuffer> msgResponseQueue;

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



	CClientSocket* GetSocket();
    //friend UINT ProcessRequestQueueThread(LPVOID pParam);
    //friend UINT ProcessResponseQueueThread(LPVOID pParam);

	int SendRequest(CBuffer inBuf, CBuffer& outBuf);
    int SendBuffer(CBuffer inBuf);

    CMessageQueue<CString>& GetListMessage();

    void            SetValid(bool bValid);
    bool            GetValid();

    void            SetLogin(bool bLOgin);
    bool            GetLogin();

    void            SetUserID(CString strUserID);
    CString         GetUserID();

    void            SetUserPassword(CString strPassword);
    CString         GetUserPassword();

    void            SetUserName(CString strUserName);
    CString         GetUserName();

    bool            GetAuctionFlag();

    std::vector<CProduct>   GetListProduct();
    
protected:
	CClientSocket* m_pSocket;

    CMessageQueue<CString>	m_listMessage;

    std::vector<CProduct>    m_listProduct;

    //CWinThread	*	m_pThreadProcessRequestQueue;
    //CWinThread	*	m_pThreadProcessResponseQueue;

    bool            m_bValid;
    bool            m_bLogin;
    HANDLE          m_hEvtReceive;

    CString         m_strUserID;
    CString         m_strUserPassword;
    CString         m_strUserName;
    
    bool            m_bAuctionStart;
// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS

};

extern CAuctionClientDoc * GetCurrentDoc();