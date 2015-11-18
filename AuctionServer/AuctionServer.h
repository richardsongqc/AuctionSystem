
// AuctionServer.h : main header file for the AuctionServer application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CAuctionServerApp:
// See AuctionServer.cpp for the implementation of this class
//

class CAuctionServerApp : public CWinApp
{
public:
	CAuctionServerApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CAuctionServerApp theApp;
