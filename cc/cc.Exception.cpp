// cc.Exception.h
// cc.Exception class
// Author: Shu.KK
// 2005-01-01
#include "cc.Exception.h"
using namespace cc;

///////////////////////////////////////////////////////////////////////////////
// Exception
// constructors
Exception::Exception()
{
	_Init();
}

Exception::Exception(const TCHAR *errmsgFormat, ...)
{
	_Init();
	va_list argList;
	va_start(argList, errmsgFormat);
	SetMessageV(&_pBuf, errmsgFormat, argList);
	va_end(argList);

	SetMessage(&_pAllMsgBuf, _pBuf);
}

Exception::Exception(const Exception& innerException)
{
	_Init();
	//this time,save innermsg into normal msg
	SetMessage(&_pBuf, innerException._pBuf);
	SetMessage(&_pAllMsgBuf, innerException._pAllMsgBuf);
}

Exception::Exception(const Exception& innerException, const TCHAR *errmsgFormat, ...)
{
	_Init();
	va_list argList;
	va_start(argList, errmsgFormat);
	SetMessageV(&_pBuf, errmsgFormat, argList);
	va_end(argList);

	//this time,save normal msg,and [normal msg + coming allmsg] into allmsg
	SetInnerMessage(innerException);
}

///////////////////// destructor //////////////////////////
Exception::~Exception()
{
	if(_pBuf != NULL)
	{
		free(_pBuf);
		_pBuf = NULL;
	}
	if(_pAllMsgBuf != NULL)
	{
		free(_pAllMsgBuf);
		_pAllMsgBuf = NULL;
	}
}

///////////////////// private/internal used methods //////////////////////////
const TCHAR* Exception::GetMessage() const
{
	return _pBuf;
}

const TCHAR* Exception::GetAllMessage() const
{
	return _pAllMsgBuf;
}

cc::Str Exception::ToString() const
{
	return cc::Str(_pAllMsgBuf);
}

void Exception::_Init()
{
	_pBuf = NULL;
	_pAllMsgBuf = NULL;
}

///////////////////// public used methods //////////////////////////
// Assignment Operations
Exception& Exception::operator=(const Exception& exp)
{
	SetMessage(&_pBuf, exp._pBuf);
	SetMessage(&_pAllMsgBuf, exp._pAllMsgBuf);
	return *this;
}

void Exception::SetMessageV(TCHAR** sDes, const TCHAR *errmsgFormat, va_list argList)
{
	cc::Buf::FormatV(sDes, errmsgFormat, argList);
}

void Exception::SetMessage(TCHAR** sDes, const TCHAR *errmsgFormat, ...)
{
	va_list argList;
	va_start(argList, errmsgFormat);
	SetMessageV(sDes, errmsgFormat, argList);
	va_end(argList);
}

void Exception::SetInnerMessage(const Exception& innerException)
{
	if(innerException._pAllMsgBuf != NULL)
	{
		SetMessage(&_pAllMsgBuf, _T("%s\r\n%s"), this->_pBuf, innerException._pAllMsgBuf);
	}
	else if(_pAllMsgBuf != NULL)
	{
		free(_pAllMsgBuf);
		_pAllMsgBuf = NULL;
	}
}
// Exception
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// UserException
UserException::UserException() : Exception()
{
}

UserException::UserException(const TCHAR *errmsgFormat, ...) : Exception()
{
	va_list argList;
	va_start(argList, errmsgFormat);
	SetMessageV(&_pBuf, errmsgFormat, argList);
	va_end(argList);
}

UserException::UserException(const Exception& innerException) : Exception(innerException)
{
}

UserException::UserException(const Exception& innerException, const TCHAR *errmsgFormat, ...) : Exception()
{
	va_list argList;
	va_start(argList, errmsgFormat);
	SetMessageV(&_pBuf, errmsgFormat, argList);
	va_end(argList);

	//this time,save normal msg,and [normal msg + coming allmsg] into allmsg
	SetInnerMessage(innerException);
}

UserException::~UserException()
{
}
// UserException
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// OutOfMemoryException
OutOfMemoryException::OutOfMemoryException() : Exception()
{
}

OutOfMemoryException::OutOfMemoryException(const TCHAR *errmsgFormat, ...) : Exception()
{
	va_list argList;
	va_start(argList, errmsgFormat);
	SetMessageV(&_pBuf, errmsgFormat, argList);
	va_end(argList);
}

OutOfMemoryException::OutOfMemoryException(const Exception& innerException) : Exception(innerException)
{
}

OutOfMemoryException::OutOfMemoryException(const Exception& innerException, const TCHAR *errmsgFormat, ...) : Exception()
{
	va_list argList;
	va_start(argList, errmsgFormat);
	SetMessageV(&_pBuf, errmsgFormat, argList);
	va_end(argList);

	//this time,save normal msg,and [normal msg + coming allmsg] into allmsg
	SetInnerMessage(innerException);
}

OutOfMemoryException::~OutOfMemoryException()
{
}
// OutOfMemoryException
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// ArgumentException
ArgumentException::ArgumentException() : Exception()
{
}

ArgumentException::ArgumentException(const TCHAR *errmsgFormat, ...) : Exception()
{
	va_list argList;
	va_start(argList, errmsgFormat);
	SetMessageV(&_pBuf, errmsgFormat, argList);
	va_end(argList);
}

ArgumentException::ArgumentException(const Exception& innerException) : Exception(innerException)
{
}

ArgumentException::ArgumentException(const Exception& innerException, const TCHAR *errmsgFormat, ...) : Exception()
{
	va_list argList;
	va_start(argList, errmsgFormat);
	SetMessageV(&_pBuf, errmsgFormat, argList);
	va_end(argList);

	//this time,save normal msg,and [normal msg + coming allmsg] into allmsg
	SetInnerMessage(innerException);
}

ArgumentException::~ArgumentException()
{
}
// ArgumentException
///////////////////////////////////////////////////////////////////////////////
