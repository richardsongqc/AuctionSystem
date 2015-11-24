#include "stdafx.h"

#include "ClassDBConnection.h"
#pragma warning( disable : 4800)

//// This line of code is borrowed from Dbcore.cpp
//static const TCHAR szODBC[] = _T("ODBC;");
//
//// These two lines are borrowed from Afximpl.h
//// determine number of elements in an array (not bytes)
//#define _countof(array) (sizeof(array)/sizeof(array[0]))
//
//BOOL CMyDatabase::Open(LPCTSTR lpszDSN, BOOL bExclusive,
//    BOOL bReadonly, LPCTSTR lpszConnect, BOOL bUseCursorLib)
//{
//    UCHAR szConnectOutput[MAX_CONNECT_LEN];
//
//#ifdef _68K_
//    if(nClassObject == 0)
//    AfxThrowDBException(AFX_SQL_ERROR_ODBC_LOAD_FAILED, NULL,
//                        SQL_NULL_HSTMT);
//#endif
//
//    ASSERT_VALID(this);
//    ASSERT(lpszDSN == NULL || AfxIsValidString(lpszDSN));
//    ASSERT(lpszConnect == NULL || AfxIsValidString(lpszConnect));
//
//    // Exclusive access not supported.
//    ASSERT(!bExclusive);
//    UNUSED(bExclusive);  // unused in release builds
//
//    m_bUpdatable = !bReadonly;
//
//    TRY
//    {
//        if (lpszConnect != NULL)
//            m_strConnect = lpszConnect;
//
//        // For Visual Basic & Microsoft Access compatibility, use the
//        // "ODBC;" (or "odbc;") prefix to the connect string
//        if (_tcsnicmp(m_strConnect, szODBC, lstrlen(szODBC)) != 0)
//        {
//            TRACE0("Error: Missing 'ODBC' prefix on connect string.\n");
//            return FALSE;
//        }
//
//        // Strip "ODBC;"
//        m_strConnect = m_strConnect.Right(m_strConnect.GetLength()
//                        - lstrlen(szODBC));
//
//        if (lpszDSN != NULL && lstrlen(lpszDSN) != 0)
//        {
//            // Append "DSN=" lpszDSN
//            m_strConnect += ";DSN=";
//            m_strConnect += lpszDSN;
//        }
//
//        AllocConnect();
//
//        RETCODE nRetCode;
//        // Turn on cursor lib support
//        if (bUseCursorLib)
//        {
//            AFX_SQL_SYNC(::SQLSetConnectOption(m_hdbc,
//                SQL_ODBC_CURSORS, SQL_CUR_USE_ODBC));
//            // With cursor library added records immediately in result set
//            m_bIncRecordCountOnAdd = TRUE;
//            }
//
//            HWND hWndTop;
//            CWnd* pWnd = CWnd::GetSafeOwner(NULL, &hWndTop);
//            if (pWnd == NULL)
//            pWnd = CWnd::GetDesktopWindow();
//            ASSERT_VALID(pWnd);
//
//            SWORD nResult;
//#ifndef _MAC
//            USES_CONVERSION;
//
//            // Code before it was changed:
//            //    AFX_SQL_SYNC(::SQLDriverConnect(m_hdbc, pWnd->m_hWnd,
//            //      (UCHAR*)T2A((LPCTSTR)m_strConnect), SQL_NTS,
//            //      szConnectOutput, _countof(szConnectOutput),
//            //      &nResult, SQL_DRIVER_COMPLETE));
//
//            AFX_SQL_SYNC(::SQLDriverConnect(m_hdbc, NULL,
//            (UCHAR*)(const char*)m_strConnect, SQL_NTS,
//            szConnectOutput, _countof(szConnectOutput),
//            &nResult, SQL_DRIVER_NOPROMPT));
//
//#else
//            AFX_SQL_SYNC(::SQLDriverConnect(m_hdbc, GetWrapperWindow
//                (pWnd->m_hWnd), (UCHAR*)(const char*)m_strConnect, SQL_NTS,
//                szConnectOutput, _countof(szConnectOutput),
//                &nResult, SQL_DRIVER_COMPLETE));
//
//#endif
//            if (hWndTop != NULL)
//                ::EnableWindow(hWndTop, TRUE);
//
//            // Code before it was changed:
//            //    if (nRetCode == SQL_NO_DATA_FOUND)
//            // Now need to check for SQL_ERROR too
//            if ((nRetCode == SQL_NO_DATA_FOUND) || (nRetCode == SQL_ERROR))
//            {
//                Free();
//                return FALSE;
//            }
//
//            if (!Check(nRetCode))
//            {
//#ifdef _DEBUG
//                if (pWnd->m_hWnd == NULL)
//                    TRACE0("Error: No default window (AfxGetApp()->m_pMainWnd) for
//                SQLDriverConnect.\n");
//#endif
//                ThrowDBException(nRetCode);
//            }
//
//            // Connect strings must have "ODBC;"
//            m_strConnect = szODBC;
//            // Save connect string returned from ODBC
//            m_strConnect += (char*)szConnectOutput;
//
//            SWORD nAPIConformance;
//            AFX_SQL_SYNC(::SQLGetInfo(m_hdbc, SQL_ODBC_API_CONFORMANCE,
//                &nAPIConformance, sizeof(nAPIConformance), &nResult));
//            if (!Check(nRetCode))
//                ThrowDBException(nRetCode);
//
//            if (nAPIConformance < SQL_OAC_LEVEL1)
//                ThrowDBException(AFX_SQL_ERROR_API_CONFORMANCE);
//
//            SWORD nSQLConformance;
//            AFX_SQL_SYNC(::SQLGetInfo(m_hdbc, SQL_ODBC_SQL_CONFORMANCE,
//                &nSQLConformance, sizeof(nSQLConformance), &nResult));
//            if (!Check(nRetCode))
//                ThrowDBException(nRetCode);
//
//            if (nSQLConformance < SQL_OSC_MINIMUM)
//                ThrowDBException(AFX_SQL_ERROR_SQL_CONFORMANCE);
//
//            AFX_SQL_SYNC(::SQLGetInfo(m_hdbc, SQL_CURSOR_COMMIT_BEHAVIOR,
//                &m_nCursorCommitBehavior, sizeof(m_nCursorCommitBehavior),
//                &nResult));
//            if (!Check(nRetCode))
//                m_nCursorCommitBehavior = SQL_ERROR;
//
//            AFX_SQL_SYNC(::SQLGetInfo(m_hdbc, SQL_CURSOR_ROLLBACK_BEHAVIOR,
//                &m_nCursorRollbackBehavior, sizeof(m_nCursorRollbackBehavior),
//                &nResult));
//            if (!Check(nRetCode))
//            m_nCursorRollbackBehavior = SQL_ERROR;
//
//            UDWORD dwGetDataExtensions;
//            AFX_SQL_SYNC(::SQLGetInfo(m_hdbc, SQL_GETDATA_EXTENSIONS,
//            &dwGetDataExtensions, sizeof(dwGetDataExtensions),
//            &nResult));
//            if (!Check(nRetCode))
//            dwGetDataExtensions = 0;
//            if (dwGetDataExtensions & SQL_GD_BOUND)
//            m_dwUpdateOptions = AFX_SQL_GDBOUND;
//            else
//            m_dwUpdateOptions = 0;
//
//            // Set required transaction support for CRecordset cursors
//            if ((m_nCursorCommitBehavior == SQL_CB_PRESERVE) &&
//            (m_nCursorRollbackBehavior == SQL_CB_PRESERVE))
//            m_bTransactions = TRUE;
//
//            if (m_bUpdatable)
//            {
//            // Make sure data source is Updatable
//            char szReadOnly[10];
//            AFX_SQL_SYNC(::SQLGetInfo(m_hdbc, SQL_DATA_SOURCE_READ_ONLY,
//                szReadOnly, _countof(szReadOnly), &nResult));
//            if (Check(nRetCode) && nResult == 1)
//                m_bUpdatable = !(lstrcmpA(szReadOnly, "Y") == 0);
//            else
//                m_bUpdatable = FALSE;
//#ifdef _DEBUG
//            if (!m_bUpdatable && (afxTraceFlags & traceDatabase))
//            TRACE0("Warning: data source is readonly.\n");
//#endif
//        }
//        else
//        {
//            // Make data source is Updatable
//            AFX_SQL_SYNC(::SQLSetConnectOption(m_hdbc,
//            SQL_ACCESS_MODE, SQL_MODE_READ_ONLY));
//        }
//
//        char szIDQuoteChar[2];
//        AFX_SQL_SYNC(::SQLGetInfo(m_hdbc, SQL_IDENTIFIER_QUOTE_CHAR,
//            szIDQuoteChar, _countof(szIDQuoteChar), &nResult));
//        if (Check(nRetCode) && nResult == 1)
//            m_chIDQuoteChar = szIDQuoteChar[0];
//        else
//            m_chIDQuoteChar = ' ';
//
//#ifdef _DEBUG
//        if (afxTraceFlags & traceDatabase)
//        {
//            char szInfo[64];
//            AFX_SQL_SYNC(::SQLGetInfo(m_hdbc, SQL_DBMS_NAME,
//            szInfo, _countof(szInfo), &nResult));
//            if (Check(nRetCode))
//            {
//                CString strInfo = szInfo;
//                TRACE1("DBMS: %s\n", strInfo);
//                AFX_SQL_SYNC(::SQLGetInfo(m_hdbc, SQL_DBMS_VER,
//                    szInfo, _countof(szInfo), &nResult));
//                if (Check(nRetCode))
//                {
//                    strInfo = szInfo;
//                    TRACE1(", Version: %s\n", strInfo);
//                }
//            }
//        }
//#endif
//    }
//    CATCH_ALL(e)
//    {
//    Free();
//    THROW_LAST();
//    }
//    END_CATCH_ALL
//
//    return TRUE;
//}















///////////////////////////////////////////////////////////////////////////////



using namespace std;

	//CString sDsn = _T("ODBC;DRIVER=MICROSOFT ACCESS DRIVER (*.mdb, *.accdb);DSN='';DBQ=C:\\Users\\Vahram\\Documents\\Visual Studio 2010\\Projects\\Lagrange-Linear-MFC\\Lagrange-Linear-MFC\\dbLagrangeLinear.accdb");
	//CString sDsn = _T("ODBC;DRIVER=MICROSOFT ACCESS DRIVER (*.mdb, *.accdb);DSN='';DBQ=") + mMyPath + _T("\\GUCodeCivile.accdb"); 



ClassDBConnection::ClassDBConnection(CString connectionString)
{
	m_ConnectionString = connectionString;

	m_Recordset = NULL;
    m_RecordCount = -2;
	isOpen = false;
}



ClassDBConnection::ClassDBConnection()
{
	m_ConnectionString = _T("");

	m_Recordset = NULL;
    m_RecordCount = -2;
	isOpen = false;
}



ClassDBConnection::~ClassDBConnection(void)
{
	if(m_Recordset != NULL)
	{
		m_Recordset->Close();
		delete m_Recordset;
		m_Recordset = NULL;
	}
	m_Database.Close();
}



void ClassDBConnection::SetConnectionString(CString connectionString)
{
	m_ConnectionString = connectionString;
}

//ClassDBConnection::ClassDBConnection( const ClassDBConnection& dbConn)
//{
//    m_ConnectionString = dbConn.m_ConnectionString;
//
//    m_Recordset = dbConn.m_Recordset;
//    isOpen = dbConn.isOpen;
//
//}

//ClassDBConnection& ClassDBConnection::operator = ( const ClassDBConnection& dbConn)
//{
//    m_ConnectionString = dbConn.m_ConnectionString;
//
//    m_Recordset = dbConn.m_Recordset;
//    isOpen = dbConn.isOpen;
//
//    return *this;
//}

ClassDBConnection& ClassDBConnection::operator = ( const CString connectionString)
{
	m_ConnectionString = connectionString;

	m_Recordset = NULL;
    m_RecordCount = -2;
	isOpen = false;

    return *this;
}

// Open the connection, return true or false
bool ClassDBConnection::Open()
{	
	bool result;
	Close();

	try
	{
		result = m_Database.OpenEx ( m_ConnectionString , CDatabase::noOdbcDialog );
		isOpen = result;
	}
	catch(CDBException* e)	
	{						
	   result = false;	
	   CString str = e->m_strError;
	   e->Delete();			
	   throw str;	
	}

	return result;
}

// Close the connection
void ClassDBConnection::Close()
{
	if(m_Recordset != NULL)
	{
		m_Recordset->Close();
		delete m_Recordset;
		m_Recordset = NULL;
	}
	m_Database.Close();
	isOpen = false;
}


// Execute sql and put the result in m_Recordset
bool ClassDBConnection::ExecuteSql(CString strSql)
{
	bool result;
	try
	{
		if(m_Recordset != NULL)
		{
			m_Recordset->Close();
		}
		else
		{
			m_Recordset = new CRecordset(&m_Database);
		}

		result = m_Recordset->Open(CRecordset::dynaset, strSql, CRecordset::readOnly);
        m_RecordCount = -2;

	}
	catch(CDBException* e)	
	{						
	   result = false;	
       m_RecordCount = -2;
	   CString str = e->m_strError;
	   e->Delete();			
	   throw str;
	}
	
	return result;
}


// Execute Query and put the result in m_Recordset
bool ClassDBConnection::ExecuteQuery(CString QryName)
{
	bool result;
	try
	{
		if(m_Recordset != NULL)
		{
			m_Recordset->Close();
		}
		else
		{
			m_Recordset = new CRecordset(&m_Database);
		}
		result = m_Recordset->Open(CRecordset::dynaset, _T("{call ") + QryName + _T("}"), CRecordset::readOnly);
        m_RecordCount = -2;
	}
	catch(CDBException* e)	
	{						
	   result = false;
       m_RecordCount = -2;
	   CString str = e->m_strError;
	   e->Delete();			
	   throw str;
	}
	
	return result;
}


// Execute sql and put the result in recordset
bool ClassDBConnection::ExecuteSql(CString strSql, CRecordset* & recordset)
{
	bool result;
	try
	{
		if(recordset != NULL)
		{
			recordset->Close();
		}
		else
		{
			recordset = new CRecordset(&m_Database);           
		}
		result = recordset->Open(CRecordset::dynaset, strSql, CRecordset::readOnly);
        m_RecordCount = -2;
	}
	catch(CDBException* e)	
	{						
	   result = false;	
       m_RecordCount = -2;
	   CString str = e->m_strError;
	   e->Delete();			
	   throw str;	
	}
	
	return result;
}

// Execute insert sql
bool ClassDBConnection::ExecuteSqlNonQuery(CString strSql)
{
	bool result;
	try
	{
		m_Database.ExecuteSQL(strSql);
		result = true;
	}
	catch(CDBException* e)	
	{						
	   result = false;	
	   CString str = e->m_strError;
	   e->Delete();			
	   throw str;
	}

	return result;
}


// Functions for database transaction
bool ClassDBConnection::BeginTrans()
{
	return (m_Database.BeginTrans() == TRUE);
}

bool ClassDBConnection::CommitTrans()
{
	return (m_Database.CommitTrans() == TRUE);
}

bool ClassDBConnection::RollBack()
{
	return (m_Database.Rollback() == TRUE);
}
///////////////////////////////////


long ClassDBConnection::GetRecordCount()
{
    if(this->m_RecordCount == -2)
    {   
        m_RecordCount = -1;
        if( this->m_Recordset == NULL )
        {
            return m_RecordCount;
        }

        try
        {
            m_RecordCount = 0;
            while (!m_Recordset->IsEOF())
            {
                m_RecordCount++;
                m_Recordset->MoveNext();            
            }   

			if (m_RecordCount > 0)
				m_Recordset->MoveFirst();
        }
        catch(CDBException* e)	
	    {							
	       CString str = e->m_strError;
	       e->Delete();			
	       throw str;
           m_RecordCount = -1;
           return m_RecordCount;
	    }
    }

    return m_RecordCount;
}