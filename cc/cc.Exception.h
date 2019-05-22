// cc.Exception.h
// cc.Exception class
// Author: Shu.KK
// 2005-01-01
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef CC_EXCEPTION_H_
#define CC_EXCEPTION_H_

#include "cc.h"
#ifdef __BORLANDC__
#include <windows.h>
#endif /* __BORLANDC__ */

#include <string.h>
#include <wchar.h>
#include <tchar.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "cc.Log.h"
#include "cc.Str.h"

namespace cc
{

//define comment of class
#if defined(_DEBUG_)
#define EXCEPTION_MSG_STR(x) #x
#define EXCEPTION_MSG(msg)	(_T("[File:") _T(__FILE__) \
	_T(", Function:") _T(__FUNCTION__) _T(", Line:") \
	_T(EXCEPTION_MSG_STR(__LINE__)) _T("]") _T(msg))
#else
#define EXCEPTION_MSG(msg)	(_T(msg))
#endif

class Exception : public Object
{
protected:
	//protected constants
	TCHAR *_pBuf;
	TCHAR *_pAllMsgBuf;
	//TCHAR *_pAppName;
	//TCHAR *_pClassFunName;
	//Exception *_innerExp;
	void _Init();
	void SetInnerMessage(const Exception& innerException);
	void SetMessageV(TCHAR** sDes, const TCHAR *errmsgFormat, va_list argList);
	void SetMessage(TCHAR** sDes, const TCHAR *errmsgFormat, ...);

public:
	//constructors
	Exception();
	Exception(const TCHAR *errmsgFormat, ...);
	Exception(const Exception& innerException);
	Exception(const Exception& innerException, const TCHAR *errmsgFormat, ...);
	//destructor
	virtual ~Exception();

	//Assignment Operations
	Exception& operator=(const Exception& exp);

	const TCHAR* GetMessage() const;
	const TCHAR* GetAllMessage() const;
	virtual cc::Str ToString() const;
};

class UserException : public Exception
{
protected:
	//protected constants

public:
	//constructors
	UserException();
	UserException(const TCHAR *errmsgFormat, ...);
	UserException(const Exception& innerException);
	UserException(const Exception& innerException, const TCHAR *errmsgFormat, ...);
	//destructor
	virtual ~UserException();
};

class OutOfMemoryException : public Exception
{
public:
	//constructors
	OutOfMemoryException();
	OutOfMemoryException(const TCHAR *errmsgFormat, ...);
	OutOfMemoryException(const Exception& innerException);
	OutOfMemoryException(const Exception& innerException, const TCHAR *errmsgFormat, ...);
	//destructor
	virtual ~OutOfMemoryException();
};

class ArgumentException : public Exception
{
public:
	//constructors
	ArgumentException();
	ArgumentException(const TCHAR *errmsgFormat, ...);
	ArgumentException(const Exception& innerException);
	ArgumentException(const Exception& innerException, const TCHAR *errmsgFormat, ...);
	//destructor
	virtual ~ArgumentException();
};

} //namespace cc

#endif //#ifndef CC_EXCEPTION_H_

