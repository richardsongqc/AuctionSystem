// Class which establish connection between database and program classes

#pragma once



//class CMyDatabase : 
//    public CDatabase
//{
//public:
//	virtual BOOL Open(LPCTSTR lpszDSN, BOOL bExclusive = FALSE,
//		BOOL bReadonly = FALSE, LPCTSTR lpszConnect = _T("ODBC;"),
//		BOOL bUseCursorLib = TRUE);
//};

class ClassDBConnection
{
private: // Variables
	CString			m_ConnectionString;
    long            m_RecordCount;


public: // Variables
	bool			isOpen;         // indicates if connection is open
	CRecordset*		m_Recordset;    // Recordset wich is filled by ExecuteSql() function
	CDatabase		m_Database ;     // Current database object

public: // Functions

	ClassDBConnection(CString connectionString);
	ClassDBConnection();            // by default constructor
	~ClassDBConnection(void);
    //ClassDBConnection( const ClassDBConnection& dbConn);
    //ClassDBConnection& operator = ( const ClassDBConnection& dbConn);
    ClassDBConnection& operator = ( const CString connectionString);

	void SetConnectionString(CString connectionString); // Will be called if we use default constructor

	bool BeginTrans();
	bool CommitTrans();
	bool RollBack();

	bool Open();                    // Open connection
	void Close();                   // Close connection

	bool ExecuteSql			(CString strSql); // Execute by recordset (will have result in m_Recordset)
	bool ExecuteSql			(CString strSql, CRecordset* & recordset); // Execute by recordset (will have result in recordset)	
	bool ExecuteSqlNonQuery	(CString strSql); // Execute by database (without result, for Update, Delete and Insert)
	bool ExecuteQuery		(CString QryName);

    long GetRecordCount();
};

