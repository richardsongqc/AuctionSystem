#include "stdafx.h"
#include "Buffer.h"


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
	return pSocket->Receive(m_szBuf, sizeof(m_szBuf));
}

int CBuffer::Send(CSocket* pSocket)
{
	return pSocket->Send(m_szBuf, sizeof(m_szBuf));
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
	
	int nUserIDLen = m_szBuf[2];

	BYTE szBuf[1024] = { 0 };
	memcpy(szBuf, m_szBuf + 3, nUserIDLen);

	CString strUserID((TCHAR*)szBuf);

	return strUserID;
}

void CInRegisterClient::SetUserID(CString strUserID)
{	
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
	memcpy(m_szBuf + 3, strUserID.GetBuffer(nUserIDLen), nUserIDLen);

	m_szBuf[4 + nUserIDLen] = nPasswordLen;
	memcpy( m_szBuf + nUserIDLen + 5, szPassword, nPasswordLen );
}


CString CInRegisterClient::GetUserPassword()
{
	int nUserIDLen = m_szBuf[2];
	int nPasswordLen = m_szBuf[4 + nUserIDLen];

	BYTE szBuf[1024] = { 0 };
	memcpy(szBuf, m_szBuf + 5 + nUserIDLen, nPasswordLen);

	CString strPassword((TCHAR*)szBuf);

	return strPassword;
}

void CInRegisterClient::SetUserPassword(CString strPassword)
{
	int nPasswordLen = strPassword.GetLength();
	
	int nUserIDLen = m_szBuf[2];
	m_szBuf[4 + nUserIDLen] = nPasswordLen;

	BYTE szPassword[1024] = { 0 };
	memcpy(m_szBuf + nUserIDLen + 5, strPassword.GetBuffer(nPasswordLen), nPasswordLen);
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
}


CString COutRegisterClient::GetUserName()
{
	int nUserNameLen = m_szBuf[4];
	BYTE szBuf[1024] = { 0 };
	memcpy(szBuf, m_szBuf + 5, nUserNameLen);

	CString strUserName((TCHAR*)szBuf);

	return strUserName;
}

void COutRegisterClient::SetUserName(CString strUserName)
{
	m_szBuf[4] = strUserName.GetLength();

	memcpy(m_szBuf + 5, strUserName.GetBuffer(m_szBuf[4]), m_szBuf[4]);
}


///////////////////////////////////////////////////////////////////////////////