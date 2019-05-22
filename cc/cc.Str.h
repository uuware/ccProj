/* cc.Str.h
	The Java like string class
	Author: Shu.KK
	created:2004-09-25
	modified:2005-05-01,2006-01-01

	can use _UNICODE and _DEBUG_

	type        byte length
	char           1 -128 -> 126
	unsigned char  1 0 -> 255
	short          2 -32,768 -> 32,767
	unsigned short 2 0 -> 65,535
	long           4 -2,147,483,648 -> 2,147,483,648
	unsigned long  4 0 -> 4,294,967,295
	int            4 same to long
	unsigned int   4 same to unsigned long
	float          4 1.2E-38 -> 3.4E381
	double         8 2.2E-308 -> 1.8E3082
	bool           1 true -> false
	long double   10
	DWORD          4 =unsigned long
	WORD           2 =unsigned short
	BYTE           1 =unsigned char

	HWND           4
	HINSTANCE      4

class C1
{
public:
	C1(){i1 = 1;};
	int get1(){return i1;}
	virtual int get2(){return i1;}
	int get3(){return get2();}
private:
	int i1;
};

class C2 : public C1
{
public:
	//constructors
	C2(){i2 = 2;};
	int get1(){return i2;}
	virtual int get2(){return i2;}
	int get3(){return get2();}
private:
	int i2;
};
C2* c2 = new C2();;
cc::Log::fatal(_T("c2.1:%d"), c2->get1(0));
cc::Log::fatal(_T("c2.2:%d"), c2->get2(0));
cc::Log::fatal(_T("c2.3:%d"), c2->get3(0));
C1* c3 = (C1*)c2;
cc::Log::fatal(_T("c3.1:%d"), c3->get1(0));
cc::Log::fatal(_T("c3.2:%d"), c3->get2(0));
cc::Log::fatal(_T("c3.3:%d"), c3->get3(0));

2006/12/26 14:31:09 FATAL:c2.1:4
2006/12/26 14:31:09 FATAL:c2.2:5
2006/12/26 14:31:09 FATAL:c2.3:56
2006/12/26 14:31:09 FATAL:c3.1:1
2006/12/26 14:31:09 FATAL:c3.2:5
2006/12/26 14:31:09 FATAL:c3.3:53

About NULL or "":
	for function ValueOf(..., int fromIndex, ...), IndexOf(..., int fromIndex, ...), LastIndexOf(..., int fromIndex, ...),
if then fromIndex is not valid(<0), then the result is NULL.
But for Left, Right, Mid, even length is < 0, if the orignal str is not IndexOf, then result is "".
*/

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef CC_STR_H_
#define CC_STR_H_
#define _ccStr

/*
  class Str can contral UNICODE&ASCII(or System code)
  this is determind at define, or then use ToWChar||ToMByte
  and code turn is use:mbstowcs&wcstombs
*/
#if defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif
#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#endif

#include "cc.h"
#include <windows.h>
#include <ctype.h>
#include <wchar.h>
#include <tchar.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <typeinfo.h>

#include "cc.Log.h"

namespace cc
{

class Object;
class Str;

//#if !defined(_CPPRTTI)
//	#error ERROR:requires C++ RTTI[/GR (Enable Run-Time Type Information)]
//set for this:
//1,Open the project's Property Pages dialog box.
//2,Click the C/C++ folder.
//3,Click the Language property page.
//4,Modify the Enable Run-Time Type Info property.
//that all right for BorlandC++Compiler5.5.1
//#endif

//about CC_CLASSID
//  1     ~499999 for end user
//  500000~699999 used by ccProj
//  700000~799999 for devlop level1
//  800000~899999 for devlop level2
//  900000~999999(F423F) for devlop level3
//  max is 16777215(0x00FFFFFF)
#define CC_CLASSID             500000
#define CC_CLASSID_END         699999
#define CC_CLASSID1            700000
#define CC_CLASSID1_END        799999
#define CC_CLASSID2            800000
#define CC_CLASSID2_END        899999
#define CC_CLASSID3            900000
#define CC_CLASSID3_END        999999
//start from this ID for end user to use Timer
#define CC_CLASSID_USER        000100
#define CC_CLASSID_USER_END    499999

#define CC_DEFINE_CLASSID(CTL)      _idFlags = CC_CLASSID_##CTL

//flag for _idFlags of object(only 0xFF000000)
#define CC_OBJECT_FLAG_DISPOSED       0x01000000

#define CC_CLASSID_Object 500010
class Object
{
public:
	//constructors
	Object()
	{
		CC_DEFINE_CLASSID(Object);
	};
	//destructor
	virtual ~Object()
	{
		Dispose();
	};

	//class name with namespace
	cc::Str GetClassName() const;
	bool IsClass(const TCHAR* className);
	bool IsClass(const cc::Str& className);
	virtual cc::Str ToString() const;

	virtual bool Equals(Object* o)
	{
		return this == o;
	};

	static bool Equals(Object* a, Object* b)
	{
		if(a == b)
		{
			return true;
		}

		if(a == NULL || b == NULL)
		{
			return false;
		}

		return a->Equals(b);
	};

	virtual void Dispose()
	{
		//_isDisposed = true;
		_idFlags |= CC_OBJECT_FLAG_DISPOSED;
	};
	virtual bool GetIsDispose()
	{
		//return _isDisposed;
		return ((_idFlags & CC_OBJECT_FLAG_DISPOSED) == CC_OBJECT_FLAG_DISPOSED);
	}
	DWORD GetClassID()
	{
		//return _isDisposed;
		return (_idFlags & 0x00FFFFFF);
	}

protected:
	DWORD _idFlags;

	virtual cc::Str DoGetClassName() const;
};

class ObjectConfig : public Object
{
public:
};

//#define SETGET 0
//#define SETONLY 1
//#define GETONLY 2
//template <class T, class C, int Type = SETGET>
//class Property : public Object
//{
//private:
//	typedef T (C::*Get)();
//	typedef void (C::*Set)(T);
//	Get GetFunc_;
//	Set SetFunc_;
//	C* Class_;
//public:
//	Property(Get GetFunc, Set SetFunc, C* Class)
//	: GetFunc_(GetFunc), SetFunc_(SetFunc), Class_(Class) {}
//
//	operator T () const
//	{
//		return (Class_->*GetFunc_)();
//	}
//
//	Property<T, C, Type>& operator=(T val)
//	{
//		(Class_->*SetFunc_)(val);
//		return *this;
//	}
//};
//
//template <class T, class C>
//class Property<T, C, SETONLY> : public Object
//{
//private:
//	typedef void (C::*Set)(T);
//	Set SetFunc_;
//	C* Class_;
//public:
//	Property(Set SetFunc, C* Class)
//	: SetFunc_(SetFunc), Class_(Class) {}
//
//	Property<T, C, SETONLY>& operator=(T val)
//	{
//		(Class_->*SetFunc_)(val);
//		return *this;
//	}
//};
//
//template <class T, class C>
//class Property<T, C, GETONLY> : public Object
//{
//private:
//	typedef T (C::*Get)();
//	Get GetFunc_;
//	C* Class_;
//public:
//	Property(Get GetFunc, C* Class)
//	: GetFunc_(GetFunc), Class_(Class) {}
//
//	operator T () const
//	{
//		return (Class_->*GetFunc_)();
//	}
//};
//
//class PropertyTest
//{
//protected:
//	int FMyVariable;
//	int get_Length()
//	{
//		return FMyVariable;
//	}
//	void set_Length(int Value)
//	{
//		FMyVariable = Value;
//	}
//
//public:
//	Property<int, PropertyTest> Length;
//	Property<int, PropertyTest, SETONLY> Length2;
//	Property<int, PropertyTest, GETONLY> Length3;
//	PropertyTest() : Length(NULL, NULL, NULL)
//		,	Length2(NULL, NULL)
//		,	Length3(NULL, NULL)
//	{
//		//for invoid of compile warning C4355:'this' : used in base member initializer list
//		Length = Property<int, PropertyTest>(get_Length, set_Length, this);
//		Length2 = Property<int, PropertyTest, SETONLY>(set_Length, this);
//		Length3 = Property<int, PropertyTest, GETONLY>(get_Length, this);
//	}
//};
//
///*
//	PropertyTest my;
//	my.Length = 12;
//	int x = my.Length;
//
//	my.Length2 = 123;
//	int y = my.Length3;
//*/

class StrConfig : public ObjectConfig
{
public:
	static float nIncrement;
	//static unsigned int nIncrementMaxLen;
};

#define CC_CLASSID_Str 500020
//for _Flags
#define CC_STR_NULL 0x00000001
class Str : public Object
{
public:
	//constructors
	Str();
	//initialCapacity:
	//  malloc the initialCapacity when create buf
	//  _nCapacity:char or wchar_t size of buf,when NULL then 0.by default,_nCapacity=0
	//incrementMultiPercent(_nIncrement):
	//  when need resize buf,(resizes size) = (now size)* (_nIncrement)
	//  if(_nIncrement <= 1) then no extra size for buf,resize each time of add char
	//  by default,_nIncrement=1.3 in _Init()
	//before treated big buf,you need _nIncrement and _nCapacity,this reduces the amount of incremental reallocation.
	//Str(unsigned int nMinCapacity); //mabye confuse, not use this from 20141016
	Str(unsigned int nMinCapacity, double incPercentMoreThan1);
	//Str(const char str);
	Str(const char *str, LangCode::Type fromLangCode = LangCode::SYSTEM);
	Str(const char *str, int fromIndex, LangCode::Type fromLangCode = LangCode::SYSTEM);
	Str(const char *str, int fromIndex, int copyLen, LangCode::Type fromLangCode = LangCode::SYSTEM);
	//Str(const wchar_t str);
	Str(const wchar_t *str);
	Str(const wchar_t *str, int fromIndex);
	Str(const wchar_t *str, int fromIndex, int copyLen);
	//_savedLang is same to Str
	Str(const Str &str);
	Str(const Str &str, int fromIndex);
	Str(const Str &str, int fromIndex, int copyLen);
	Str(LangCode::Type saveLangCode);
	Str(LangCode::Type saveLangCode, unsigned int nMinCapacity);
	Str(LangCode::Type saveLangCode, unsigned int nMinCapacity, double incPercentMoreThan1);
	Str(LangCode::Type saveLangCode, const char *str, LangCode::Type fromLangCode = LangCode::SYSTEM);
	Str(LangCode::Type saveLangCode, const char *str, int fromIndex, LangCode::Type fromLangCode = LangCode::SYSTEM);
	Str(LangCode::Type saveLangCode, const char *str, int fromIndex, int copyLen, LangCode::Type fromLangCode = LangCode::SYSTEM);
	Str(LangCode::Type saveLangCode, const wchar_t *str);
	Str(LangCode::Type saveLangCode, const wchar_t *str, int fromIndex);
	Str(LangCode::Type saveLangCode, const wchar_t *str, int fromIndex, int copyLen);
	//this time,_savedLang is same to saveLangCode
	Str(LangCode::Type saveLangCode, const Str &str);
	Str(LangCode::Type saveLangCode, const Str &str, int fromIndex);
	Str(LangCode::Type saveLangCode, const Str &str, int fromIndex, int copyLen);
	//destructor
	virtual ~Str();

///////////////////////////////////////////////////////////////////////////////
// out static for others.
	static int Len(const char *str);
	static int Len(const wchar_t *str);
	static int Cmp(const char *str, const char *str2, bool ignoreCase = false);
	static int Cmp(const wchar_t *str, const wchar_t *str2, bool ignoreCase = false);
	static int NCmp(const char *str, const char *str2, int len, bool ignoreCase = false);
	static int NCmp(const wchar_t *str, const wchar_t *str2, int len, bool ignoreCase = false);
	static int Search(const char *buf, int startIndex, int count, const char *str, int strlen, bool ignoreCase = false);
	static int Search(const char *buf, const char *str, int strlen = -1, bool ignoreCase = false);
	static int Search(const wchar_t *buf, int startIndex, int count, const wchar_t *str, int strlen, bool ignoreCase = false);
	static int Search(const wchar_t *buf, const wchar_t *str, int strlen = -1, bool ignoreCase = false);
	static int SearchLast(const char *buf, int startIndex, int count, const char *str, int strlen, bool ignoreCase = false);
	static int SearchLast(const char *buf, const char *str, int strlen = -1, bool ignoreCase = false);
	static int SearchLast(const wchar_t *buf, int startIndex, int count, const wchar_t *str, int strlen, bool ignoreCase = false);
	static int SearchLast(const wchar_t *buf, const wchar_t *str, int strlen = -1, bool ignoreCase = false);
	static char *Move(char *des, const char *src, int len);
	static wchar_t *Move(wchar_t *des, const wchar_t *src, int len);

///////////////////////////////////////////////////////////////////////////////
// C# LIKE
	//return myself
	Str Clone();
	static int Compare(const char *strA, int indexA, const char *strB, int indexB, int length, bool ignoreCase = false);
	static int Compare(const wchar_t *strA, int indexA, const wchar_t *strB, int indexB, int length, bool ignoreCase = false);
	static int Compare(const Str& strA, int indexA, const Str& strB, int indexB, int length, bool ignoreCase = false);
	static int Compare(const char *strA, const char *strB, bool ignoreCase = false);
	static int Compare(const wchar_t *strA, const wchar_t *strB, bool ignoreCase = false);
	static int Compare(const Str& strA, const Str& strB, bool ignoreCase = false);
	//CompareTo can return 1 or 0 or -1
	int CompareTo(const char *str, bool ignoreCase = false, LangCode::Type fromLangCode = LangCode::SYSTEM) const;
	int CompareTo(const wchar_t *str, bool ignoreCase = false) const;
	int CompareTo(const Str &str, bool ignoreCase = false) const;
	//pay attation to that saveLangCode is defined by _UNICODE
	static Str Concat(const char *str0, const char *str1, LangCode::Type fromLangCode = LangCode::SYSTEM);
	static Str Concat(const char *str0, const char *str1, const char *str2, LangCode::Type fromLangCode = LangCode::SYSTEM);
	static Str Concat(const char *str0, const char *str1, const char *str2, const char *str3, LangCode::Type fromLangCode = LangCode::SYSTEM);
	//pay attation to that saveLangCode is defined by _UNICODE
	static Str Concat(const wchar_t *str0, const wchar_t *str1);
	static Str Concat(const wchar_t *str0, const wchar_t *str1, const wchar_t *str2);
	static Str Concat(const wchar_t *str0, const wchar_t *str1, const wchar_t *str2, const wchar_t *str3);
	static Str Concat(const Str &str0, const Str &str1);
	static Str Concat(const Str &str0, const Str &str1, const Str &str2);
	static Str Concat(const Str &str0, const Str &str1, const Str &str2, const Str &str3);
	bool EndsWith(const char *suffix, bool ignoreCase = false, LangCode::Type fromLangCode = LangCode::SYSTEM) const;
	bool EndsWith(const char *suffix, int endLength, bool ignoreCase = false, LangCode::Type fromLangCode = LangCode::SYSTEM) const;
	bool EndsWith(const wchar_t *suffix, bool ignoreCase = false) const;
	bool EndsWith(const wchar_t *suffix, int endLength, bool ignoreCase = false) const;
	bool EndsWith(const Str &suffix, bool ignoreCase = false) const;
	bool EndsWith(const Str &suffix, int endLength, bool ignoreCase = false) const;
	static bool Equals(const char *a, const char *b, bool ignoreCase = false);
	bool Equals(const char *str, bool ignoreCase = false, LangCode::Type fromLangCode = LangCode::SYSTEM) const;
	static bool Equals(const wchar_t *a, const wchar_t *b, bool ignoreCase = false);
	bool Equals(const wchar_t *str, bool ignoreCase = false) const;
	static bool Equals(const Str &a, const Str &b, bool ignoreCase = false);
	bool Equals(const Str &str, bool ignoreCase = false) const;
	Str &Format(const char *strFormat, ...);
	Str &FormatV(const char *strFormat, va_list argList);
	Str &Format(LangCode::Type fromLangCode, const char *strFormat, ...);
	Str &FormatV(LangCode::Type fromLangCode, const char *strFormat, va_list argList);
	Str &Format(const wchar_t *strFormat, ...);
	Str &FormatV(const wchar_t *strFormat, va_list argList);
	int IndexOf(const char *str, bool ignoreCase = false, LangCode::Type fromLangCode = LangCode::SYSTEM) const;
	int IndexOf(const char *str, int startIndex, bool ignoreCase = false, LangCode::Type fromLangCode = LangCode::SYSTEM) const;
	int IndexOf(const char *str, int startIndex, int count, bool ignoreCase = false, LangCode::Type fromLangCode = LangCode::SYSTEM) const;
	int IndexOf(const wchar_t *str, bool ignoreCase = false) const;
	int IndexOf(const wchar_t *str, int startIndex, bool ignoreCase = false) const;
	int IndexOf(const wchar_t *str, int startIndex, int count, bool ignoreCase = false) const;
	int IndexOf(const Str &str, bool ignoreCase = false) const;
	int IndexOf(const Str &str, int startIndex, bool ignoreCase = false) const;
	int IndexOf(const Str &str, int startIndex, int count, bool ignoreCase = false) const;
	Str &Insert(int insertAt, const char *str, LangCode::Type fromLangCode = LangCode::SYSTEM);
	Str &Insert(int insertAt, const wchar_t *str);
	Str &Insert(int insertAt, const Str &str);
	//cannot know the length of array,so need this param
	static Str Join(const Str &separator, const Str str[], int startIndex, int count, bool noLastSeparator = false);
	int LastIndexOf(const char *str, bool ignoreCase = false, LangCode::Type fromLangCode = LangCode::SYSTEM) const;
	int LastIndexOf(const char *str, int startIndex, bool ignoreCase = false, LangCode::Type fromLangCode = LangCode::SYSTEM) const;
	//count:the len of str to seek,startIndex is count from ahead
	int LastIndexOf(const char *str, int startIndex, int count, bool ignoreCase = false, LangCode::Type fromLangCode = LangCode::SYSTEM) const;
	int LastIndexOf(const wchar_t *str, bool ignoreCase = false) const;
	int LastIndexOf(const wchar_t *str, int startIndex, bool ignoreCase = false) const;
	int LastIndexOf(const wchar_t *str, int startIndex, int count, bool ignoreCase = false) const;
	int LastIndexOf(const Str &str, bool ignoreCase = false) const;
	int LastIndexOf(const Str &str, int startIndex, bool ignoreCase = false) const;
	int LastIndexOf(const Str &str, int startIndex, int count, bool ignoreCase = false) const;
	Str &Pad(int totalWidth, const char paddingChar, LangCode::Type fromLangCode = LangCode::SYSTEM);
	Str &Pad(int totalWidth, const wchar_t paddingChar);
	Str &PadLeft(int totalWidth, const char paddingChar, LangCode::Type fromLangCode = LangCode::SYSTEM);
	Str &PadLeft(int totalWidth, const wchar_t paddingChar);
	Str &PadRight(int totalWidth, const char paddingChar, LangCode::Type fromLangCode = LangCode::SYSTEM);
	Str &PadRight(int totalWidth, const wchar_t paddingChar);
	Str &Remove(int beginIndex);
	Str &Remove(int beginIndex, int endIndex);
	Str &Replace(const char *oldStr, const char *newStr, bool ignoreCase = false, LangCode::Type fromLangCode = LangCode::SYSTEM);
	Str &Replace(const char *oldStr, const char *newStr, int fromIndex, bool ignoreCase = false, LangCode::Type fromLangCode = LangCode::SYSTEM);
	Str &Replace(const wchar_t *oldStr, const wchar_t *newStr, bool ignoreCase = false);
	Str &Replace(const wchar_t *oldStr, const wchar_t *newStr, int fromIndex, bool ignoreCase = false);
	Str &Replace(const Str &oldStr, const Str &newStr, bool ignoreCase = false);
	Str &Replace(const Str &oldStr, const Str &newStr, int fromIndex, bool ignoreCase = false);
	bool StartsWith(const char *prefix, bool ignoreCase = false, LangCode::Type fromLangCode = LangCode::SYSTEM) const;
	bool StartsWith(const char *prefix, int fromIndex, bool ignoreCase = false, LangCode::Type fromLangCode = LangCode::SYSTEM) const;
	bool StartsWith(const wchar_t *prefix, bool ignoreCase = false) const;
	bool StartsWith(const wchar_t *prefix, int fromIndex, bool ignoreCase = false) const;
	bool StartsWith(const Str &prefix, bool ignoreCase = false) const;
	bool StartsWith(const Str &prefix, int fromIndex, bool ignoreCase = false) const;
	Str Substring(int beginIndex) const;
	Str Substring(int beginIndex, int endIndex) const;
	Str &ToLower();
	virtual cc::Str ToString() const;
	Str &ToUpper();
	Str &Trim();
	Str &Trim(const char *str, LangCode::Type fromLangCode = LangCode::SYSTEM);
	Str &Trim(const wchar_t *str);
	Str &Trim(const Str &str);
	Str &TrimLeft();
	Str &TrimLeft(const char *str, LangCode::Type fromLangCode = LangCode::SYSTEM);
	Str &TrimLeft(const wchar_t *str);
	Str &TrimLeft(const Str &str);
	Str &TrimRight();
	Str &TrimRight(const char *str, LangCode::Type fromLangCode = LangCode::SYSTEM);
	Str &TrimRight(const wchar_t *str);
	Str &TrimRight(const Str &str);

	void Clear();
	int CharAt(int Index) const;
	int GetLength() const;
// C# LIKE
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// JAVA LIKE
	Str &ValueOf(const char ch);
	Str &ValueOf(const wchar_t ch);
	Str &ValueOf(int n);
	//instead with Format(%d/%f)
	//Str &ValueOf(long n);
	//Str &ValueOf(float n);
	//Str &ValueOf(double n);
	Str &ValueOf(const char *str, LangCode::Type fromLangCode = LangCode::SYSTEM);
	Str &ValueOf(const char *str, int fromIndex, LangCode::Type fromLangCode = LangCode::SYSTEM);
	Str &ValueOf(const char *str, int fromIndex, int copyLen, LangCode::Type fromLangCode = LangCode::SYSTEM);
	Str &ValueOf(const wchar_t *str);
	Str &ValueOf(const wchar_t *str, int fromIndex);
	Str &ValueOf(const wchar_t *str, int fromIndex, int copyLen);
	//also change langcode to str
	Str &ValueOf(const Str &str);
	Str &ValueOf(const Str &str, int fromIndex);
	Str &ValueOf(const Str &str, int fromIndex, int copyLen);
// JAVA LIKE
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// OTHERS
	int Split(const char *Splitstr, Str **str, bool ignoreCase = false, LangCode::Type fromLangCode = LangCode::SYSTEM) const;
	int Split(const char *Splitstr, Str **str, int limit, bool ignoreCase = false, LangCode::Type fromLangCode = LangCode::SYSTEM) const;
	int Split(const char *Splitstr, Str **str, int limit, bool ignoreCase, LangCode::Type fromLangCode, LangCode::Type toLangCode) const;
	int Split(const wchar_t *Splitstr, Str **str, bool ignoreCase = false) const;
	int Split(const wchar_t *Splitstr, Str **str, int limit, bool ignoreCase = false) const;
	int Split(const wchar_t *Splitstr, Str **str, int limit, bool ignoreCase, LangCode::Type toLangCode) const;
	int Split(const Str &Splitstr, Str **str, bool ignoreCase = false) const;
	int Split(const Str &Splitstr, Str **str, int limit, bool ignoreCase = false) const;
	int Split(const Str &Splitstr, Str **str, int limit, bool ignoreCase, LangCode::Type toLangCode) const;
	const void *GetBuf() const;
	unsigned int GetMinCapacity() const;
	unsigned int GetRealCapacity() const;
	float GetIncPercent() const;
	void SetCapacity(unsigned int nMinCapacity, double incPercentMoreThan1);
	//Trims the capacity of this buffer to be the current(real) size.
	void CompactStringBuf();
	int ToInt(int base = 10) const;
	long ToLong(int base = 10) const;
	float ToFloat() const;
	double ToDouble() const;
	bool ToMByte(LangCode::Type toLangCode = LangCode::SYSTEM);
	bool ToWChar();
	bool ToTChar(LangCode::Type toLangCode = LangCode::SYSTEM);
	Str &ToReverse();
	Str Left(int length) const;
	Str Right(int length) const;
	Str Mid(int beginIndex) const;
	Str Mid(int beginIndex, int len) const;
	Str SplitBy(const char *str, bool ignoreCase = false, LangCode::Type fromLangCode = LangCode::SYSTEM);
	Str SplitBy(const wchar_t *str, bool ignoreCase = false);
	Str SplitBy(const Str &str, bool ignoreCase = false);
	Str &Append(const char str);
	Str &Append(const wchar_t str);
	Str &Append(int num);
	//Str &Append(const char *str);
	//Str &Append(const char *str, int fromIndex);
	Str &Append(const char *str, LangCode::Type fromLangCode = LangCode::SYSTEM);
	Str &Append(const char *str, int fromIndex, LangCode::Type fromLangCode = LangCode::SYSTEM);
	Str &Append(const char *str, int fromIndex, int copyLen, LangCode::Type fromLangCode = LangCode::SYSTEM);
	Str &Append(const wchar_t *str);
	Str &Append(const wchar_t *str, int fromIndex);
	Str &Append(const wchar_t *str, int fromIndex, int copyLen);
	Str &Append(const Str &str);
	Str &Append(const Str &str, int fromIndex);
	Str &Append(const Str &str, int fromIndex, int copyLen);
	Str &AppendFormat(const char *strFormat, ...);
	Str &AppendFormatV(const char *strFormat, va_list argList);
	Str &AppendFormat(LangCode::Type fromLangCode, const char *strFormat, ...);
	Str &AppendFormatV(LangCode::Type fromLangCode, const char *strFormat, va_list argList);
	Str &AppendFormat(const wchar_t *strFormat, ...);
	Str &AppendFormatV(const wchar_t *strFormat, va_list argList);
	Str &Repeat(const char *str, int nRepeat, LangCode::Type fromLangCode = LangCode::SYSTEM);
	Str &Repeat(const wchar_t *str, int nRepeat);
	Str &Repeat(const Str &str, int nRepeat);
	int FindCount(const char *str, bool ignoreCase = false) const;
	int FindCount(const char *str, int fromIndex, bool ignoreCase = false, LangCode::Type fromLangCode = LangCode::SYSTEM) const;
	int FindCount(const wchar_t *str, bool ignoreCase = false) const;
	int FindCount(const wchar_t *str, int fromIndex, bool ignoreCase = false) const;
	int FindCount(const Str &str, bool ignoreCase = false) const;
	int FindCount(const Str &str, int fromIndex, bool ignoreCase = false) const;

	//if length=0 then return false
	bool IsNumber(int nIndex) const;
	bool IsAlpha(int nIndex) const;
	bool IsUpper(int nIndex) const;
	bool IsLower(int nIndex) const;
	bool IsNumber() const;
	bool IsAlpha() const;
	bool IsUpper() const;
	bool IsLower() const;
	bool IsBlankWithTrim() const;
	bool IsBlankWithoutTrim() const;
	bool IsNULL() const;
	bool IsUnicode() const;
	bool IsTChar() const;
	LangCode::Type GetLangCode() const;

	//Assignment Operations
	Str &operator=(const char *str);
	Str &operator=(const wchar_t *str);
	Str &operator=(const Str &str);
	//!!!if need NUICODE<->CHAR,then use LangCode::LOC_SYSTEM(if not set LangCode) !!!
	operator char*();
	operator wchar_t*();

	//Friend Functions
	friend Str operator+(const Str& str, const char ch);
	friend Str operator+(const Str& str, int num);
	friend Str operator+(const Str& str, const char* str2);
	friend Str operator+(const Str& str, const wchar_t* str2);
	friend Str operator+(const char* str, const Str& str2);
	friend Str operator+(const wchar_t* str, const Str& str2);
	friend Str operator+(const Str& str, const Str& str2);

	friend Str operator+(const char ch, const Str& str);
	friend Str operator+(int num, const Str& str);
	//instead with AppendFormat(%d/%f)
	//friend Str operator+(long num, const Str& str);
	//friend Str operator+(const Str& str, long num);
	//friend Str operator+(float num, const Str& str);
	//friend Str operator+(const Str& str, float num);
	//friend Str operator+(double num, const Str& str);
	//friend Str operator+(const Str& str, double num);

	//Concatenation
	Str &operator+=(const char str);
	Str &operator+=(const wchar_t str);
	Str &operator+=(const char *str);
	Str &operator+=(const wchar_t *str);
	Str &operator+=(const Str &str);
	Str &operator+=(int num);
	//Str &operator+=(long num);
	//Str &operator+=(float num);
	//Str &operator+=(double num);
	int operator[](int Index) const;

	//Compare operations
	//friend Str operator==(const Str& str, const char* str2);
	//friend Str operator==(const Str& str, const wchar_t* str2);
	//friend Str operator==(const char* str, const Str& str2);
	//friend Str operator==(const wchar_t* str, const Str& str2);
	//friend Str operator==(const Str& str, const Str& str2);
	//friend Str operator!=(const Str& str, const char* str2);
	//friend Str operator!=(const Str& str, const wchar_t* str2);
	//friend Str operator!=(const char* str, const Str& str2);
	//friend Str operator!=(const wchar_t* str, const Str& str2);
	//friend Str operator!=(const Str& str, const Str& str2);

	bool operator==(const char *str) const;
	bool operator==(const wchar_t *str) const;
	bool operator==(const Str &str) const;
	bool operator!=(const char *str) const;
	bool operator!=(const wchar_t *str) const;
	bool operator!=(const Str &str) const;
	bool operator>(const char *str) const;
	bool operator>(const wchar_t *str) const;
	bool operator>(const Str &str) const;
	bool operator>=(const char *str) const;
	bool operator>=(const wchar_t *str) const;
	bool operator>=(const Str &str) const;
	bool operator<(const char *str) const;
	bool operator<(const wchar_t *str) const;
	bool operator<(const Str &str) const;
	bool operator<=(const char *str) const;
	bool operator<=(const wchar_t *str) const;
	bool operator<=(const Str &str) const;

	//========================================
	//SPECIAL
	//this fun will take over void* buf,and relese it in this class also
	Str &TakeoverPointer(char **str, int len = -1, LangCode::Type fromLangCode = LangCode::SYSTEM);
	Str &TakeoverPointer(wchar_t **str, int len = -1);
	Str &TakeoverPointer(Str &str);

	//Str &CopyPointer(char *str, int bufLength, LangCode::Type fromLangCode = LangCode::SYSTEM);
	//Str &CopyPointer(wchar_t *str, int bufLength);
	//void GiveoutPointer(void **str);

	//added for treate buffer(2007/07), MUST used UpdateStringLength() after this.
	void *GetBuffer() const;
	//the real buffer nLength*sizeof(TCHAR)
	void *GetBufferSetLength(int nLength, bool isPreserveBuf = false);
	//added 2014/08/30, recalculate length used of strlen or wcslen
	int UpdateStringLength();
/*
	//sample for GetBuffer&UpdateStringLength&CompactStringBuf
	cc::Str str;
	int nLen = 128;
	int nRes;
	do
	{
		nLen *= 2;
		lvi.pszText = (LPWSTR)str.GetBufferSetLength(nLen);
		nRes  = (int)::SendMessage(m_hWnd, LVM_GETITEMTEXT, (WPARAM)nItem, (LPARAM)&lvi);
	} while (nRes == nLen-1);
	str.UpdateStringLength();

	//others for pass "...\0...\0..."
	//TCHAR *tcBuf = _T("Icon Files (*.EXE,*.DLL,*.ICL,*.BMP)\0*.EXE;*.DLL;*.ICL;*.BMP\0All Files(*.*)\0*.*\0");
	cc::Str str;
	TCHAR* filter = (TCHAR*)str.GetBufferSetLength(_MAX_PATH + 1);
	memcpy(filter, _T("Icon Files (*.EXE,*.DLL,*.ICL,*.BMP)\0*.EXE;*.DLL;*.ICL;*.BMP\0All Files(*.*)\0*.*\0"), _MAX_PATH);
    do sth. with the buffer...
	str.UpdateStringLength(); or str.CompactBuf(); //for has '\0', so after this, the Length is calculate to first '\0'
*/

protected:
	//protected constants
#if _DEBUG_
	//for show contents with mouse on while debug
	#if defined (_UNICODE)
	wchar_t *_pBufW;
	char *_pBufC;
	#else
	char *_pBufC;
	wchar_t *_pBufW;
	#endif
#endif //#if _DEBUG_
	void *_pBuf;
	//_nCapacity:char or wchar_t size of buf,when NULL or "" then 0.init:_nCapacity=0
	unsigned int _nCapacity;
	unsigned int _nMinCapacity;
	//_nRealCapacity:real size of buf.default:_nRealCapacity=real buf size
	unsigned int _nRealCapacity;
	//_nIncrement:when need resize buf,(resizes size) = (now size)* (_nIncrement).default:_nIncrement=1.3
	float _nIncrement;
	LangCode::Type _savedLang;
	DWORD _Flags;
	//BOOL _isNULL;

	void _Init();
	void _Alloc(int len, bool isPreserveBuf = true);
	void _StrCopy(const char *str, LangCode::Type fromLangCode, int strLen, int fromIndex, int copyLen);
	void _StrCopy(const wchar_t *str, int strLen, int fromIndex, int copyLen);
	void _StrIns(const char *str, LangCode::Type fromLangCode, int strLen, int insPos, int fromIndex, int copyLen);
	void _StrIns(const wchar_t *str, int strLen, int insPos, int fromIndex, int copyLen);
	//from 2009/02/05,changed to force free(_pBuf),so isNULL is same to _pBuf==NULL.
	//void _Free(bool isRealFree);
	void _Free();
	virtual cc::Str DoGetClassName() const;
};

} //namespace cc

#endif //#ifndef CC_STR_H_
