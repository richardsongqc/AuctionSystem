#pragma once


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

class COutRegisterClient : public CBuffer
{
public:
	COutRegisterClient();
	~COutRegisterClient();

	COutRegisterClient(const COutRegisterClient& rOut);
	COutRegisterClient& operator = (const COutRegisterClient& rOut);

	bool GetState();
	void SetState(bool bState);

    bool GetLogin();
    void SetLogin(bool bLogin);

	CString GetUserName();
	void SetUserName(CString strUserName);
};

///////////////////////////////////////////////////////////////////////////////

class CProduct
{
public:
    CProduct() :
        m_dwProductID(0),
        m_strName    (L""),
        m_dwCount    (0),
        m_dblPrice   (0.0)
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

    void SetCount(long lCount)
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

    CString GetUserID();
    void SetUserID(CString strUserID);
};

class COutRetrieveStock : public CBuffer
{
public:
    COutRetrieveStock();
    ~COutRetrieveStock();

    COutRetrieveStock(const COutRetrieveStock& rOut);
    COutRetrieveStock& operator = (const COutRetrieveStock& rOut);

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

    CString GetUserID();
    void SetUserID(CString strUserID);

    CString GetUserPassword();
    void SetUserPassword(CString strPassword);
};

class COutAdvertising : public CBuffer
{
public:
    COutAdvertising();
    ~COutAdvertising();

    COutAdvertising(const COutAdvertising& rOut);
    COutAdvertising& operator = (const COutAdvertising& rOut);

    bool GetState();
    void SetState(bool bState);

    bool GetLogin();
    void SetLogin(bool bLogin);

    CString GetUserName();
    void SetUserName(CString strUserName);
};

///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////

