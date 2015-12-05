#include "stdafx.h"


#include <string>
#include <protocol.h>

#pragma warning( disable : 4244)

using namespace std;


#include "Buffer.h"

string toBytes(double x) 
{
    std::size_t size = sizeof(x);
    string _return(size, 0);
    char * start = reinterpret_cast<char *>(&x);
    for (std::size_t i = 0; i<size; i++) 
    {
        _return[i] = *(start + i);
    }
    return _return;
}

double getDouble(const string & x) 
{
    char * p = const_cast<char*>(x.c_str());
    double * _return = 0;
    _return = reinterpret_cast<double*>(p);
    return *_return;
}


string toBytes(DWORD x) 
{
    std::size_t size = sizeof(x);
    std::size_t cnt = size;
    string _return(size, 0);
    while (cnt--) 
    {
        unsigned char byte = (unsigned char)((x >> ((size - 1 - cnt) * 8)) & 0xff);
        _return[cnt] = byte;
    }
    return _return;
}


DWORD getDWORD(const string & x)
{
    DWORD _return = 0;
    for (std::size_t i = 0; i < x.size(); ++i) 
    {
        _return += static_cast<unsigned char>(x[x.size() - 1 - i]) *static_cast<unsigned int>(pow(256, i));
    }

    return _return;
}

DWORD getDWORD(CString str)
{
    USES_CONVERSION;
    string x( T2A(str.GetBuffer()) );
    DWORD _return = 0;
    for (std::size_t i = 0; i < x.size(); ++i)
    {
        _return += static_cast<unsigned char>(x[x.size() - 1 - i]) *static_cast<unsigned int>(pow(256, i));
    }

    return _return;
}


float Bytes2Float(BYTE * szBuf)
{
    float f;
    memcpy(&f, szBuf, sizeof(f));
    return f;
}

void Float2Bytes(float f, BYTE * szBuf)
{
    memcpy(szBuf, &f, sizeof(f));
}

DWORD Bytes2DWORD(BYTE * szBuf)
{
    DWORD dw;
    memcpy(&dw, szBuf, sizeof(DWORD));
    return dw;
}

void DWORD2Bytes(DWORD dw, BYTE * szBuf)
{
    memcpy(szBuf, &dw, sizeof(DWORD));
}


///////////////////////////////////////////////////////////////////////////////


CBuffer::CBuffer()
{
	memset(m_szBuf, 0, sizeof(m_szBuf));
}


CBuffer::~CBuffer()
{
}


BYTE CBuffer::GetCmd()
{
	return m_szBuf[0];
}


void CBuffer::SetCmd(BYTE cmd)
{
	m_szBuf[0] = cmd;
}

BYTE CBuffer::GetLen()
{
	return m_szBuf[1];
}

void CBuffer::SetLen(BYTE len)
{
	m_szBuf[1] = len;
}

BYTE* CBuffer::GetData()
{
	return m_szBuf + 2;
}

void CBuffer::SetData(BYTE* szData)
{
	memcpy(m_szBuf + 2, szData, m_szBuf[1]);
}

int CBuffer::Receive(CSocket* pSocket)
{
    m_pSocket = pSocket;
	return pSocket->Receive(m_szBuf, sizeof(m_szBuf));
}

int CBuffer::Send(CSocket* pSocket)
{
	int nRet = pSocket->Send(m_szBuf, sizeof(m_szBuf));

    if (nRet == -1)
    {
        TRACE( L"Error = %d\n", GetLastError());
    }
    return nRet;
}

CSocket * CBuffer::GetSocket()
{
    return m_pSocket;
}

///////////////////////////////////////////////////////////////////////////////

CInRegisterClient::CInRegisterClient()
{
	SetCmd(CMD_REGISTER_CLIENT);
}

CInRegisterClient::~CInRegisterClient()
{
}

CInRegisterClient::CInRegisterClient(const CInRegisterClient& rObj)
{
	memcpy(m_szBuf, rObj.m_szBuf, sizeof(m_szBuf));
}

CInRegisterClient& CInRegisterClient::operator = (const CInRegisterClient& rObj)
{
	memcpy(m_szBuf, rObj.m_szBuf, sizeof(m_szBuf));
	return *this;
}

CString CInRegisterClient::GetUserID()
{
	USES_CONVERSION;
	int nUserIDLen = m_szBuf[2];

	BYTE szBuf[1024] = { 0 };
	memcpy(szBuf, m_szBuf + 3, nUserIDLen);

	CString strUserID = A2T((LPCSTR)szBuf);

	return strUserID;
}

void CInRegisterClient::SetUserID(CString strUserID)
{	
	USES_CONVERSION;

	int nUserIDLen = strUserID.GetLength();
	if (nUserIDLen == m_szBuf[2])
	{
		memcpy(m_szBuf + 3, strUserID.GetBuffer(nUserIDLen), nUserIDLen);
		return;
	}

	m_szBuf[2] = strUserID.GetLength();
	int nPasswordLenPos = 3 + nUserIDLen;
	int nPasswordLen = m_szBuf[nPasswordLenPos];
	
	BYTE szPassword[1024] = { 0 };
	memcpy(szPassword, m_szBuf + nPasswordLenPos + 1, nPasswordLen);
	
	m_szBuf[1] = nPasswordLen + nUserIDLen + 2;
	memcpy(m_szBuf + 3, T2A(strUserID.GetBuffer(nUserIDLen)), nUserIDLen);

	m_szBuf[4 + nUserIDLen] = nPasswordLen;
	memcpy( m_szBuf + nUserIDLen + 5, szPassword, nPasswordLen );
}


CString CInRegisterClient::GetUserPassword()
{
	USES_CONVERSION;

	int nUserIDLen = m_szBuf[2];
	int nPasswordLen = m_szBuf[3 + nUserIDLen];

	BYTE szBuf[1024] = { 0 };
	memcpy(szBuf, m_szBuf + 4 + nUserIDLen, nPasswordLen);

	CString strPassword = A2T((LPCSTR)szBuf);

	return strPassword;
}

void CInRegisterClient::SetUserPassword(CString strPassword)
{
	USES_CONVERSION;

	int nPasswordLen = strPassword.GetLength();
	
	int nUserIDLen = m_szBuf[2];
	m_szBuf[3 + nUserIDLen] = nPasswordLen;

	BYTE szPassword[1024] = { 0 };
	memcpy(m_szBuf + nUserIDLen + 4, T2A(strPassword.GetBuffer(nPasswordLen)), nPasswordLen);
	
	m_szBuf[1] = nUserIDLen + nPasswordLen + 2;
}


///////////////////////////////////////////////////////////////////////////////

COutRegisterClient::COutRegisterClient()
{
	SetCmd(RSP_REGISTER_CLIENT);
}

COutRegisterClient::~COutRegisterClient()
{
}

COutRegisterClient::COutRegisterClient(const COutRegisterClient& rObj)
{
	memcpy(m_szBuf, rObj.m_szBuf, sizeof(m_szBuf));
}

COutRegisterClient& COutRegisterClient::operator = (const COutRegisterClient& rObj)
{
	memcpy(m_szBuf, rObj.m_szBuf, sizeof(m_szBuf));
	return *this;
}

EAuctionState COutRegisterClient::GetState()
{
    return (EAuctionState)(m_szBuf[3]);
}

void COutRegisterClient::SetState(EAuctionState eState)
{
    m_szBuf[3] = eState;
    m_szBuf[2] = 1;
    m_szBuf[1] = 1 + 1 + 1 + m_szBuf[8] + 3;
}

bool COutRegisterClient::GetValid()
{
	return m_szBuf[5] ? true : false;
}

void COutRegisterClient::SetValid(bool bState)
{
	m_szBuf[5] = bState ? 1 : 0;
	m_szBuf[4] = 1;
    m_szBuf[1] = 1 + 1 + 1 + m_szBuf[8] + 3;
}

bool COutRegisterClient::GetLogin()
{
    return m_szBuf[7] ? true : false;
}

void COutRegisterClient::SetLogin(bool bLogin)
{
    m_szBuf[7] = bLogin ? 1 : 0;
    m_szBuf[6] = 1;
    m_szBuf[1] = 1 + 1 + 1 + m_szBuf[8] + 3;
}

CString COutRegisterClient::GetUserName()
{
	USES_CONVERSION;

	int nUserNameLen = m_szBuf[8];
	BYTE szBuf[1024] = { 0 };
	memcpy(szBuf, m_szBuf + 9, nUserNameLen);

	CString strUserName = A2T((LPCSTR)szBuf);

	return strUserName;
}

void COutRegisterClient::SetUserName(CString strUserName)
{
	USES_CONVERSION;

	m_szBuf[8] = strUserName.GetLength();

	memcpy(m_szBuf + 9, T2A(strUserName.GetBuffer(m_szBuf[8])), m_szBuf[8]);

    m_szBuf[1] = 1 + 1 + 1 + m_szBuf[8] + 3;
}


/////////////////////////////////////////////////////////////////////////////// 

CInRetrieveStock::CInRetrieveStock()
{
    SetCmd(CMD_RETRIEVE_STOCK_OF_CLIENT);
}

CInRetrieveStock::~CInRetrieveStock()
{
}

CInRetrieveStock::CInRetrieveStock(const CInRetrieveStock& rObj)
{
    memcpy(m_szBuf, rObj.m_szBuf, sizeof(m_szBuf));
}

CInRetrieveStock& CInRetrieveStock::operator = (const CInRetrieveStock& rObj)
{
    memcpy(m_szBuf, rObj.m_szBuf, sizeof(m_szBuf));

    return *this;
}

CString CInRetrieveStock::GetUserID()
{
	USES_CONVERSION;
	int nUserIDLen = m_szBuf[2];

	BYTE szBuf[1024] = { 0 };
	memcpy(szBuf, m_szBuf + 3, nUserIDLen);

	CString strUserID = A2T((LPCSTR)szBuf);

	return strUserID;
}

void CInRetrieveStock::SetUserID(CString strUserID)
{
	USES_CONVERSION;

	int nUserIDLen = strUserID.GetLength();
	if (nUserIDLen == m_szBuf[2])
	{
		memcpy(m_szBuf + 3, strUserID.GetBuffer(nUserIDLen), nUserIDLen);
		return;
	}

	m_szBuf[2] = nUserIDLen;
	
	m_szBuf[1] = nUserIDLen + 1;
	memcpy(m_szBuf + 3, T2A(strUserID.GetBuffer(nUserIDLen)), nUserIDLen);
}



/////////////////////////////////////////////////////////////////////////////// 
 
COutRetrieveStock::COutRetrieveStock()
{
    SetCmd(RSP_RETRIEVE_STOCK_OF_CLIENT);
}

COutRetrieveStock::~COutRetrieveStock()
{
}

COutRetrieveStock::COutRetrieveStock(const COutRetrieveStock& rObj)
{
    memcpy(m_szBuf, rObj.m_szBuf, sizeof(m_szBuf));
}

COutRetrieveStock& COutRetrieveStock::operator = (const COutRetrieveStock& rObj)
{
    memcpy(m_szBuf, rObj.m_szBuf, sizeof(m_szBuf));

    return *this;
}

std::vector<CProduct> COutRetrieveStock::GetListProduct()
{
    USES_CONVERSION;
    std::vector<CProduct> listProduct;

    listProduct.clear();

    int nLen = GetLen();
    int position = 2;

    CHAR szBuf[sizeof(m_szBuf)] = { 0 };
    while (nLen > 0)
    {
        memset(szBuf, 0, sizeof(szBuf));

        // General Length for Product 
        int nProductLen = m_szBuf[position];
        position++;

        // Product ID Len
        int nProductIDLen = m_szBuf[position];
        position++;
        // Product ID
        DWORD dwProductID = Bytes2DWORD(m_szBuf + position);
        position += nProductIDLen;

        // Product Name Len
        int nProductNameLen = m_szBuf[position];
        position++;

        // Product Name
        memset(szBuf, 0, sizeof(szBuf));
        memcpy(szBuf, m_szBuf + position, nProductNameLen);
        CString strProductName(A2T(szBuf));
        position += nProductNameLen;

        // Product Count Len
        int nCoundLen = m_szBuf[position];
        position++;

        // Product Count
        DWORD dwCount = Bytes2DWORD(m_szBuf + position);
        position += nCoundLen;

        // Product Price Len
        int nPriceLen = m_szBuf[position];
        position++;

        // Product Price
        double dblPrice = Bytes2Float(m_szBuf + position);
        position += nPriceLen;

        CProduct product(dwProductID, strProductName, dwCount, dblPrice);

        listProduct.push_back(product);

        nLen -= nProductLen;
    }

    return listProduct;
}

void COutRetrieveStock::SetListProduct(std::vector<CProduct> listProduct)
{
    USES_CONVERSION;

    int nTotalSize = 0;
    int position = 2;

    int nLenDWORD = sizeof(DWORD);
    int nLenDouble = sizeof(double);
    for (CProduct product : listProduct)
    {
        int nProductSize = sizeof(DWORD)*2 + sizeof(double) + product.GetName().GetLength();
        nProductSize += 4;

        m_szBuf[position] = (byte)nProductSize;
        position++;

        m_szBuf[position] = sizeof(DWORD);
        position++;
        DWORD2Bytes(product.GetProductID(), m_szBuf + position);
        position += sizeof(DWORD);

        int nProductNameLen = product.GetName().GetLength();
        m_szBuf[position]=(byte)nProductNameLen;
        position++;
        CString strName = product.GetName();
        memcpy(m_szBuf + position, T2A(strName), nProductNameLen);
        position += nProductNameLen;

        m_szBuf[position] = sizeof(DWORD);
        position++;
        DWORD2Bytes(product.GetCount(), m_szBuf + position);
        position += sizeof(DWORD);

        m_szBuf[position] = sizeof(double);
        position++;
        Float2Bytes(product.GetPrice(), m_szBuf + position);
        position += sizeof(double);


        nTotalSize += nProductSize;
    }

    SetLen((byte)nTotalSize);
}

/////////////////////////////////////////////////////////////////////////////// 
 
CInAdvertising::CInAdvertising()
{
    SetCmd(CMD_ADVERTISING);
}

CInAdvertising::~CInAdvertising()
{
}

CInAdvertising::CInAdvertising(const CInAdvertising& rObj)
{
    memcpy(m_szBuf, rObj.m_szBuf, sizeof(m_szBuf));
}

CInAdvertising& CInAdvertising::operator = (const CInAdvertising& rObj)
{
    memcpy(m_szBuf, rObj.m_szBuf, sizeof(m_szBuf));

    return *this;
}

void CInAdvertising::SetProductID(DWORD dwProductID)
{
    int position = 2;
    m_szBuf[position] = sizeof(DWORD);
    position++;

    DWORD2Bytes(dwProductID, m_szBuf + position);

    SetLen(sizeof(DWORD) * 2 + sizeof(double) + m_szBuf[5 + sizeof(DWORD) * 2 + sizeof(double)] +4);
}

DWORD CInAdvertising::GetProductID()
{
    int nProductIDLen = m_szBuf[2];
    return Bytes2DWORD(m_szBuf + 3);
}

void CInAdvertising::SetProductCount( DWORD dwProductCount)
{
    int position = 3 + sizeof(DWORD);
    m_szBuf[position] = sizeof(DWORD);
    position++;
    DWORD2Bytes(dwProductCount, m_szBuf + position);

    SetLen(sizeof(DWORD) * 2 + sizeof(double) + m_szBuf[5 + sizeof(DWORD) * 2 + sizeof(double)] + 4);
}

DWORD CInAdvertising::GetProductCount()
{
    int position = 3 + sizeof(DWORD);
    int nProductIDLen = m_szBuf[position++];

    return Bytes2DWORD(m_szBuf + position);
}

void CInAdvertising::SetProductPrice( double dblProductPrice)
{
    int position = 4 + sizeof(DWORD)*2;
    m_szBuf[position++] = sizeof(double);
    Float2Bytes(dblProductPrice, m_szBuf + position);

    SetLen(sizeof(DWORD) * 2 + sizeof(double) + m_szBuf[5 + sizeof(DWORD) * 2 + sizeof(double)] + 4);
}

double CInAdvertising::GetProductPrice()
{
    int position = 4 + sizeof(DWORD) * 2;
    int PriceLen = m_szBuf[position++];
    
    return Bytes2Float(m_szBuf + position);
}

void CInAdvertising::SetProductName( CString strProductName)
{
    USES_CONVERSION;
    int position = 5 + sizeof(DWORD) * 2 + sizeof(double);

    int nProductNameLen = strProductName.GetLength();
    m_szBuf[position++] = nProductNameLen;
    memcpy(m_szBuf + position, T2A(strProductName), nProductNameLen );

    SetLen(sizeof(DWORD) * 2 + sizeof(double) + nProductNameLen + 4);
}

CString CInAdvertising::GetProductName()
{
    USES_CONVERSION;
    int position = 6 + sizeof(DWORD) * 2 + sizeof(double);
    CString str(A2T((char*)m_szBuf + position));
    return str;
}

/////////////////////////////////////////////////////////////////////////////// 

COutAdvertising::COutAdvertising()
{
    SetCmd(RSP_ADVERTISING);
}

COutAdvertising::~COutAdvertising()
{

}

COutAdvertising::COutAdvertising(const COutAdvertising& rObj)
{
    memcpy(m_szBuf, rObj.m_szBuf, sizeof(m_szBuf));
}

COutAdvertising& COutAdvertising::operator = (const COutAdvertising& rObj)
{
    memcpy(m_szBuf, rObj.m_szBuf, sizeof(m_szBuf));
    return *this;
}

EAuctionState COutAdvertising::GetState()
{
    return (EAuctionState)m_szBuf[3] ;
}

void COutAdvertising::SetState(EAuctionState eState)
{
    m_szBuf[3] = eState;
    m_szBuf[2] = 1;
    m_szBuf[1] = 1 + 1 + m_szBuf[6] + 2;
}

/////////////////////////////////////////////////////////////////////////////// 

CInAuction::CInAuction()
{
    SetCmd(CMD_BID);
}

CInAuction::~CInAuction()
{
}


CInAuction::CInAuction(const CInAuction& rObj)
{
    memcpy(m_szBuf, rObj.m_szBuf, sizeof(m_szBuf));
}

CInAuction& CInAuction::operator = (const CInAuction& rObj)
{
    memcpy(m_szBuf, rObj.m_szBuf, sizeof(m_szBuf));

    return *this;
}


void CInAuction::SetProductID(DWORD dwProductID)
{
    int position = 2;
    m_szBuf[position] = sizeof(DWORD);
    position++;
    DWORD2Bytes(dwProductID, m_szBuf + position);

    SetLen(sizeof(DWORD) * 2 + sizeof(double) + m_szBuf[5 + sizeof(DWORD) * 2 + sizeof(double)] + 4);
}

DWORD CInAuction::GetProductID()
{
    int nProductIDLen = m_szBuf[2];
    return Bytes2DWORD(m_szBuf + 3);
}

void CInAuction::SetProductCount( DWORD dwProductCount)
{
    int position = 3 + sizeof(DWORD);
    m_szBuf[position] = sizeof(DWORD);
    position++;
    DWORD2Bytes(dwProductCount, m_szBuf + position);

    SetLen(sizeof(DWORD) * 2 + sizeof(double) + m_szBuf[5 + sizeof(DWORD) * 2 + sizeof(double)] + 4);
}

DWORD CInAuction::GetProductCount()
{
    int position = 3 + sizeof(DWORD);
    int nProductIDLen = m_szBuf[position++];

    return Bytes2DWORD(m_szBuf + position);
}

void CInAuction::SetProductPrice( double dblProductPrice)
{
    int position = 4 + sizeof(DWORD) * 2;
    m_szBuf[position++] = sizeof(double);
    Float2Bytes(dblProductPrice, m_szBuf + position);

    SetLen(sizeof(DWORD) * 2 + sizeof(double) + m_szBuf[5 + sizeof(DWORD) * 2 + sizeof(double)] + 4);
}

double CInAuction::GetProductPrice()
{
    int position = 4 + sizeof(DWORD) * 2;
    int PriceLen = m_szBuf[position++];

    return Bytes2Float(m_szBuf + position);
}

void CInAuction::SetProductName( CString strProductName)
{
    USES_CONVERSION;
    int position = 5 + sizeof(DWORD) * 2 + sizeof(double);

    int nProductNameLen = strProductName.GetLength();
    m_szBuf[position++] = nProductNameLen;
    memcpy(m_szBuf + position, T2A(strProductName), nProductNameLen);

    SetLen(sizeof(DWORD) * 2 + sizeof(double) + nProductNameLen + 4 );
}

CString CInAuction::GetProductName()
{
    USES_CONVERSION;
    int position = 6 + sizeof(DWORD) * 2 + sizeof(double);
    CString str(A2T((char*)m_szBuf + position));
    return str;
}
     
void CInAuction::SetUserID(CString strUserID)
{
    USES_CONVERSION;
    int nProductNameLen = m_szBuf[6 + sizeof(DWORD) * 2 + sizeof(double)];
    int position = 6 + sizeof(DWORD) * 2 + sizeof(double) + nProductNameLen;

    int nUserIDLen = strUserID.GetLength();
    m_szBuf[position++] = nUserIDLen;
    memcpy(m_szBuf + position, T2A(strUserID), nUserIDLen);

    SetLen(sizeof(DWORD) * 2 + sizeof(double) + nProductNameLen + 4);
}

CString CInAuction::GetUserID()
{
    USES_CONVERSION;
    int nProductNameLen = m_szBuf[6 + sizeof(DWORD) * 2 + sizeof(double)];
    int position = 8 + sizeof(DWORD) * 2 + sizeof(double) + nProductNameLen;
    CString str(A2T((char*)m_szBuf + position));
    return str;
}

/////////////////////////////////////////////////////////////////////////////// 

COutAuction::COutAuction()
{
    SetCmd(RSP_BID);
}

COutAuction::~COutAuction()
{
}

COutAuction::COutAuction(const COutAuction& rObj)
{
    memcpy(m_szBuf, rObj.m_szBuf, sizeof(m_szBuf));
}

COutAuction& COutAuction::operator = (const COutAuction& rObj)
{
    memcpy(m_szBuf, rObj.m_szBuf, sizeof(m_szBuf));

    return *this;
}

EAuctionState COutAuction::GetState()
{
    return (EAuctionState)m_szBuf[3] ;
}

void COutAuction::SetState(EAuctionState eState)
{
    m_szBuf[3] = eState;
    m_szBuf[2] = 1;
    m_szBuf[1] = 1 + 1 + m_szBuf[6] + 2;

    SetLen(3 + sizeof(double));
}

void COutAuction::SetMaxBidPrice( double dblMaxBidPrice)
{
    int position = 4 ;
    m_szBuf[position++] = sizeof(double);
    Float2Bytes(dblMaxBidPrice, m_szBuf + position);

    SetLen(3 + sizeof(double));
}

double COutAuction::GetMaxBidPrice()
{
    int position = 4 ;
    int PriceLen = m_szBuf[position++];

    return Bytes2Float(m_szBuf + position);
}
     
/////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////// 

CBroadcastPrice::CBroadcastPrice()
{
    SetCmd(CMD_BROADCAST_PRICE);
}

CBroadcastPrice::~CBroadcastPrice()
{
}


CBroadcastPrice::CBroadcastPrice(const CBroadcastPrice& rObj)
{
    memcpy(m_szBuf, rObj.m_szBuf, sizeof(m_szBuf));
}

CBroadcastPrice& CBroadcastPrice::operator = (const CBroadcastPrice& rObj)
{
    memcpy(m_szBuf, rObj.m_szBuf, sizeof(m_szBuf));

    return *this;
}

void CBroadcastPrice::SetProductID(DWORD dwProductID)
{
    int position = 2;
    m_szBuf[position] = sizeof(DWORD);
    position++;
    DWORD2Bytes(dwProductID, m_szBuf + position);

    SetLen(sizeof(DWORD) * 2 + sizeof(double) + m_szBuf[5 + sizeof(DWORD) * 2 + sizeof(double)] + 4);
}

DWORD CBroadcastPrice::GetProductID()
{
    int nProductIDLen = m_szBuf[2];
    return Bytes2DWORD(m_szBuf + 3);
}

void CBroadcastPrice::SetProductCount(DWORD dwProductCount)
{
    int position = 3 + sizeof(DWORD);
    m_szBuf[position] = sizeof(DWORD);
    position++;
    DWORD2Bytes(dwProductCount, m_szBuf + position);

    SetLen(sizeof(DWORD) * 2 + sizeof(double) + m_szBuf[5 + sizeof(DWORD) * 2 + sizeof(double)] + 4);
}

DWORD CBroadcastPrice::GetProductCount()
{
    int position = 3 + sizeof(DWORD);
    int nProductIDLen = m_szBuf[position++];

    return Bytes2DWORD(m_szBuf + position);
}

void CBroadcastPrice::SetProductPrice(double dblProductPrice)
{
    int position = 4 + sizeof(DWORD) * 2;
    m_szBuf[position++] = sizeof(double);
    Float2Bytes(dblProductPrice, m_szBuf + position);

    SetLen(sizeof(DWORD) * 2 + sizeof(double) + m_szBuf[5 + sizeof(DWORD) * 2 + sizeof(double)] + 4);
}

double CBroadcastPrice::GetProductPrice()
{
    int position = 4 + sizeof(DWORD) * 2;
    int PriceLen = m_szBuf[position++];

    return Bytes2Float(m_szBuf + position);
}

void CBroadcastPrice::SetProductName(CString strProductName)
{
    USES_CONVERSION;
    int position = 5 + sizeof(DWORD) * 2 + sizeof(double);

    int nProductNameLen = strProductName.GetLength();
    m_szBuf[position++] = nProductNameLen;
    memcpy(m_szBuf + position, T2A(strProductName), nProductNameLen);

    SetLen(sizeof(DWORD) * 2 + sizeof(double) + nProductNameLen + 4);
}

CString CBroadcastPrice::GetProductName()
{
    USES_CONVERSION;
    int position = 6 + sizeof(DWORD) * 2 + sizeof(double);
    CString str(A2T((char*)m_szBuf + position));
    return str;
}
/////////////////////////////////////////////////////////////////////////////// 

CBroadcastState::CBroadcastState()
{
    SetCmd(CMD_BROADCAST_AUCTION_END);
}

CBroadcastState::~CBroadcastState()
{

}

CBroadcastState::CBroadcastState(const CBroadcastState& rObj)
{
    memcpy(m_szBuf, rObj.m_szBuf, sizeof(m_szBuf));
}

CBroadcastState& CBroadcastState::operator = (const CBroadcastState& rObj)
{
    memcpy(m_szBuf, rObj.m_szBuf, sizeof(m_szBuf));
    return *this;
}

EAuctionState CBroadcastState::GetState()
{
    return (EAuctionState)m_szBuf[3];
}

void CBroadcastState::SetState(EAuctionState eState)
{
    m_szBuf[3] = eState;
    m_szBuf[2] = 1;
    m_szBuf[1] = 2;
}

/////////////////////////////////////////////////////////////////////////////// 
 
/////////////////////////////////////////////////////////////////////////////// 
 
/////////////////////////////////////////////////////////////////////////////// 
 
/////////////////////////////////////////////////////////////////////////////// 
  
/////////////////////////////////////////////////////////////////////////////// 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
