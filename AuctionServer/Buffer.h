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