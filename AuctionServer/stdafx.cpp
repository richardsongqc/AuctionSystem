
// stdafx.cpp : source file that includes just the standard includes
// AuctionServer.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"


CString GetCurAppPath()
{
	HMODULE hModule = GetModuleHandleW(NULL);
	TCHAR path[MAX_PATH] = { 0 };
	GetModuleFileNameW(hModule, path, MAX_PATH);

	CString str = path;

	int nPos = str.ReverseFind('\\');

	return str.Left(nPos);
}

CTime ConvTimeString(CString strTime)
{
    //TIMESTAMP_STRUCT
    TCHAR *charTime = (TCHAR*)(LPCTSTR)strTime;
    CString sFormat = _T("%04d-%02d-%02d %02d:%02d:%02d");
    TCHAR *charFormat = (TCHAR*)(LPCTSTR)sFormat;
    int nYear;
    int nMonth;
    int nDate;
    int nHour;
    int nMin;
    int nSec;
    swscanf_s(charTime, charFormat, &nYear, &nMonth, &nDate, &nHour, &nMin, &nSec);
    CTime t(nYear, nMonth, nDate, nHour, nMin, nSec);


    return t;
}

CTime ConvTimeStruct(TIMESTAMP_STRUCT txStamp)
{
    CTime time(txStamp.year, txStamp.month, txStamp.day, txStamp.hour, txStamp.minute, txStamp.second);

    return time;
}