// stdafx.cpp : source file that includes just the standard includes
// AuctionServer.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file

CString GetCurAppPath()
{
	HMODULE hModule = GetModuleHandleW(NULL);
	TCHAR path[MAX_PATH] = { 0 };
	GetModuleFileNameW(hModule, path, MAX_PATH);

	CString str = path;

	int nPos = str.ReverseFind('\\');

	return str.Left(nPos);
}