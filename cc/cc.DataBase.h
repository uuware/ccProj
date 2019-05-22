// cc.DB.h
// cc.DB class
// Author: Shu.KK
// 2007-01-01
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef CC_DB_H_
#define CC_DB_H_

#include "cc.h"
#include <windows.h>
#include <string.h>
#include <wchar.h>
#include <tchar.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include <comdef.h>
#include <conio.h>

#include "cc.Str.h"
#include "cc.Exception.h"
#include "cc.List.h"
#include "cc.File.h"

#include "cc.Log.h"

//http://www.7880.com/Info/Article-7c8b3d60.html
//http://www.stcore.com/dotnet/2006-09-19/1158601731dotnet105779.html
//MDAC1.5(ADO1.5) with 1,NT Options Pack(IIS 4)/2,IE4/3,Win98
//Windows 2000 is MDAC 2.5, same with WinMe

//#import "D:\temp_c\refsrc\msado15.dll" no_namespace rename("EOF", "EndOfFile")
//#import "c:\Program Files\Common Files\System\ADO\msado15.dll" rename("EOF", "EndOfFile")
//#import "..\msado15.dll" rename("EOF", "EndOfFile")

//typedef ADODB::_RecordsetPtr RecPtr;
//typedef ADODB::_ConnectionPtr CnnPtr; 


//#include <adoint.h>
//#include <icrsint.h>
//#include <comdef.h>
//_COM_SMARTPTR_TYPEDEF(_Connection， __uuidof(_Connection));
//_COM_SMARTPTR_TYPEDEF(_Recordset， __uuidof(_Recordset));
//_COM_SMARTPTR_TYPEDEF(Fields， __uuidof(Fields));
//_COM_SMARTPTR_TYPEDEF(Field， __uuidof(Field));


namespace cc
{

class DB : public Object
{
public:
	DB();
	virtual ~DB();

protected:
};


//#define CATCHERROR(ptr,a)	catch(_com_error &e)\
//							{\
//								ErrorHandler(e,m_ErrStr);\
//								ptr=NULL;\
//								return a;\
//							}
//
//#define CATCHERRGET			catch(_com_error &e)\
//							{\
//								ErrorHandler(e,m_ErrStr);\
//								sprintf(m_ErrStr,"%s\n**For Field Name:%s",m_ErrStr,FieldName);\
//								return 0;\
//							}
//
//class Database;
//class Table;
//
//class Database : public Object
//{
//public:
//	CnnPtr m_Cnn;
//	char m_ErrStr[500];
//	Database();
//	bool Open(char* UserName, char* Pwd,char* CnnStr);
//	bool OpenTbl(int Mode, char* CmdStr, Table& Tbl);
//	bool Execute(char* CmdStr);
//	bool Execute(char* CmdStr, Table& Tbl);
//	void GetErrorErrStr(char* ErrStr);
//};
//
//class Table
//{
//public:
//	RecPtr m_Rec;
//	char m_ErrStr[500];
//	Table();
//	void GetErrorErrStr(char* ErrStr);
//	int ISEOF();
//	HRESULT MoveNext();
//	HRESULT MovePrevious();
//	HRESULT MoveFirst();
//	HRESULT MoveLast();
//	int AddNew();
//	int Update();
//	int Add(char* FieldName, char* FieldValue);
//	int Add(char* FieldName,int FieldValue);
//	int Add(char* FieldName,float FieldValue);
//	int Add(char* FieldName,double FieldValue);
//	int Add(char* FieldName,long FieldValue);
//	bool Get(char* FieldName, char* FieldValue);
//	bool Get(char* FieldName,int& FieldValue);
//	bool Get(char* FieldName,float& FieldValue);
//	bool Get(char* FieldName,double& FieldValue);
//	bool Get(char* FieldName,double& FieldValue,int Scale);
//	bool Get(char* FieldName,long& FieldValue);
//};

} //namespace cc

#endif //#ifndef CC_DB_H_

