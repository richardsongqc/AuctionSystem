// AuctionServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "AuctionServerApp.h"
#include "AuctionServer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CAuctionServerApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	HMODULE hModule = ::GetModuleHandle(NULL);

	if (!AfxSocketInit())
	{
		return FALSE;
	}

	if (hModule != NULL)
	{
		// initialize MFC and print and error on failure
		if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
		{
			// TODO: change error code to suit your needs
			_tprintf(_T("Fatal Error: MFC initialization failed\n"));
			nRetCode = 1;
		}
		else
		{
			// TODO: code your application's behavior here.
			theApp.InitInstance();

			do
			{

			} 
			while (true);
		}
	}
	else
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: GetModuleHandle failed\n"));
		nRetCode = 1;
	}

	return nRetCode;
}
