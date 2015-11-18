#pragma once

// CListeningSocket command target

class CListeningSocket : public CSocket
{
	DECLARE_DYNAMIC(CListeningSocket);
public:
	CListeningSocket();
	virtual ~CListeningSocket();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	// Overridable callbacks

	virtual void OnAccept(int nErrorCode);
protected:
	
};


