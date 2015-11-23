#include "stdafx.h"
#include "Buffer.h"
#include <protocol.h>

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
	return pSocket->Send(m_szBuf, sizeof(m_szBuf));
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

CInRegisterClient::CInRegisterClient(const CInRegisterClient& rIn)
{
	memcpy(m_szBuf, rIn.m_szBuf, sizeof(m_szBuf));
}

CInRegisterClient& CInRegisterClient::operator = (const CInRegisterClient& rIn)
{
	memcpy(m_szBuf, rIn.m_szBuf, sizeof(m_szBuf));
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

COutRegisterClient::COutRegisterClient(const COutRegisterClient& rOut)
{
	memcpy(m_szBuf, rOut.m_szBuf, sizeof(m_szBuf));
}

COutRegisterClient& COutRegisterClient::operator = (const COutRegisterClient& rOut)
{
	memcpy(m_szBuf, rOut.m_szBuf, sizeof(m_szBuf));
	return *this;
}

bool COutRegisterClient::GetState()
{
	return m_szBuf[3] ? true : false;
}

void COutRegisterClient::SetState(bool bState)
{
	m_szBuf[3] = bState ? 1 : 0;
	m_szBuf[2] = 1;
    m_szBuf[1] = 1 + 1 + m_szBuf[6] + 2;
}

bool COutRegisterClient::GetLogin()
{
    return m_szBuf[5] ? true : false;
}

void COutRegisterClient::SetLogin(bool bLogin)
{
    m_szBuf[5] = bLogin ? 1 : 0;
    m_szBuf[4] = 1;
    m_szBuf[1] = 1 + 1 + m_szBuf[6] + 2;
}

CString COutRegisterClient::GetUserName()
{
	USES_CONVERSION;

	int nUserNameLen = m_szBuf[6];
	BYTE szBuf[1024] = { 0 };
	memcpy(szBuf, m_szBuf + 7, nUserNameLen);

	CString strUserName = A2T((LPCSTR)szBuf);

	return strUserName;
}

void COutRegisterClient::SetUserName(CString strUserName)
{
	USES_CONVERSION;

	m_szBuf[6] = strUserName.GetLength();

	memcpy(m_szBuf + 7, T2A(strUserName.GetBuffer(m_szBuf[6])), m_szBuf[6]);

    m_szBuf[1] = 1 + 1 + m_szBuf[6] + 2;
}


///////////////////////////////////////////////////////////////////////////////