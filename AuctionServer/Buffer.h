#pragma once

#include <vector>
#include <protocol.h>

class CBuffer
{
public:
	CBuffer();
	~CBuffer();

	BYTE GetCmd();
	void SetCmd(BYTE cmd);

	BYTE GetLen();
	void SetLen(BYTE len);

	BYTE* GetData();
	void SetData(BYTE* szData);

	int Receive(CSocket* pSocket);
	int Send(CSocket* pSocket);

    CSocket * GetSocket();
protected:
	BYTE  m_szBuf[1024];
    CSocket * m_pSocket;
};

///////////////////////////////////////////////////////////////////////////////

class CInRegisterClient : public CBuffer
{
public:
	CInRegisterClient();
	~CInRegisterClient();

	CInRegisterClient(const CInRegisterClient& rIn);
	CInRegisterClient& operator = (const CInRegisterClient& rIn);

	CString GetUserID();
	void SetUserID(CString strUserID);

	CString GetUserPassword();
	void SetUserPassword(CString strPassword);
};

///////////////////////////////////////////////////////////////////////////////

class COutRegisterClient : public CBuffer
{
public:
	COutRegisterClient();
	~COutRegisterClient();

	COutRegisterClient(const COutRegisterClient& rOut);
	COutRegisterClient& operator = (const COutRegisterClient& rOut);

    EAuctionState GetState();
    void SetState(EAuctionState eState);

	bool GetValid();
	void SetValid(bool bState);

    bool GetLogin();
    void SetLogin(bool bLogin);

	CString GetUserName();
	void SetUserName(CString strUserName);
};

///////////////////////////////////////////////////////////////////////////////

class CProduct
{
public:
    CProduct(
        DWORD    dwProductID ,
        CString  strName     ,
        DWORD    dwCount     ,
        double   dblPrice
        ) :
        m_dwProductID(dwProductID ),
        m_strName(strName         ),
        m_dwCount(dwCount         ),
        m_dblPrice(dblPrice       )
    {

    }

    ~CProduct()
    {

    }

    CProduct(const CProduct& rObj)
    {
        m_dwProductID = rObj.m_dwProductID ;
        m_strName     = rObj.m_strName     ;
        m_dwCount     = rObj.m_dwCount     ;
        m_dblPrice    = rObj.m_dblPrice    ;
    }

    CProduct& operator = (const CProduct& rObj )
    {
        m_dwProductID = rObj.m_dwProductID;
        m_strName = rObj.m_strName;
        m_dwCount = rObj.m_dwCount;
        m_dblPrice = rObj.m_dblPrice;

        return *this;
    }



    DWORD GetProductID()
    {
        return m_dwProductID;
    }

    CString GetName()
    {
        return m_strName;
    }

    DWORD GetCount()
    {
        return m_dwCount;
    }

    double GetPrice()
    {
        return m_dblPrice;
    }

    void SetProductID(DWORD P_ID)
    {
        m_dwProductID = P_ID;
    }

    void SetName(CString strName)
    {
        m_strName = strName;
    }

    void SetCount(DWORD lCount)
    {
        m_dwCount = lCount;
    }

    void SetPrice(double dblPrice)
    {
        m_dblPrice = dblPrice;
    }

private:
    DWORD    m_dwProductID  ;
    CString  m_strName      ;
    DWORD    m_dwCount      ;
    double   m_dblPrice     ;
};

///////////////////////////////////////////////////////////////////////////////

class CInRetrieveStock : public CBuffer
{
public:
    CInRetrieveStock();
    ~CInRetrieveStock();

    CInRetrieveStock(const CInRetrieveStock& rIn);
    CInRetrieveStock& operator = (const CInRetrieveStock& rIn);

    //EAuctionState GetState();
    //void SetState(EAuctionState eState);

    CString GetUserID();
    void SetUserID(CString strUserID);
};

///////////////////////////////////////////////////////////////////////////////

class COutRetrieveStock : public CBuffer
{
public:
    COutRetrieveStock();
    ~COutRetrieveStock();

    COutRetrieveStock(const COutRetrieveStock& rOut);
    COutRetrieveStock& operator = (const COutRetrieveStock& rOut);

    EAuctionState GetState();
    void SetState(EAuctionState eState);

    std::vector<CProduct> GetListProduct();
    void SetListProduct(std::vector<CProduct> listProduct);
};


///////////////////////////////////////////////////////////////////////////////

class CInAdvertising : public CBuffer
{
public:
    CInAdvertising();
    ~CInAdvertising();

    CInAdvertising(const CInAdvertising& rIn);
    CInAdvertising& operator = (const CInAdvertising& rIn);

	void SetProductID(DWORD lProductID);
	DWORD GetProductID();
	void SetProductCount( DWORD lProductCount);
    DWORD GetProductCount();
	void SetProductPrice( double dblProductPrice);
	double GetProductPrice();
	void SetProductName( CString strProductName);
	CString GetProductName();

};

///////////////////////////////////////////////////////////////////////////////

class COutAdvertising : public CBuffer
{
public:
    COutAdvertising();
    ~COutAdvertising();

    COutAdvertising(const COutAdvertising& rOut);
    COutAdvertising& operator = (const COutAdvertising& rOut);

    EAuctionState GetState();
    void SetState(EAuctionState eState);
};

///////////////////////////////////////////////////////////////////////////////

class CInAuction : public CBuffer
{
public:
    CInAuction();
    ~CInAuction();

    CInAuction(const CInAuction& rIn);
    CInAuction& operator = (const CInAuction& rIn);

	void SetProductID(DWORD lProductID);
	DWORD GetProductID();
	void SetProductCount( DWORD lProductCount);
    DWORD GetProductCount();
	void SetProductPrice( double dblProductPrice);
	double GetProductPrice();
	void SetProductName( CString strProductName);
	CString GetProductName();

};


///////////////////////////////////////////////////////////////////////////////

class COutAuction  : public CBuffer
{
public:
    COutAuction();
    ~COutAuction();

    COutAuction(const COutAuction& rOut);
    COutAuction& operator = (const COutAuction& rOut);

    EAuctionState GetState();
    void SetState(EAuctionState eState);

	void SetMaxBidPrice( double dblMaxBidPrice);
	double GetMaxBidPrice();
};

///////////////////////////////////////////////////////////////////////////////

class CBroadcastPrice : public CBuffer
{
public:
    CBroadcastPrice();
    ~CBroadcastPrice();

    CBroadcastPrice(const CBroadcastPrice& rObj);
    CBroadcastPrice& operator = (const CBroadcastPrice& rObj);

    void SetProductID(DWORD lProductID);
    DWORD GetProductID();
    void SetProductCount(DWORD lProductCount);
    DWORD GetProductCount();
    void SetProductPrice(double dblProductPrice);
    double GetProductPrice();
    void SetProductName(CString strProductName);
    CString GetProductName();

};


///////////////////////////////////////////////////////////////////////////////

class CBroadcastState : public CBuffer
{
public:
    CBroadcastState();
    ~CBroadcastState();

    CBroadcastState(const CBroadcastState& rOut);
    CBroadcastState& operator = (const CBroadcastState& rOut);

    EAuctionState GetState();
    void SetState(EAuctionState eState);
};