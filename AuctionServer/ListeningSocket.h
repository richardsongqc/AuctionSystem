#pragma once

// CListeningSocket command target

class CListeningSocket : public CSocket
{
public:
	CListeningSocket();
	virtual ~CListeningSocket();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	// Overridable callbacks
protected:
	virtual void OnAccept(int nErrorCode);
};


