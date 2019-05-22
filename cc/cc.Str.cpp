/* cc.Str.cpp
   The Java like string class
   Author: Shu.KK
   created:2004-09-25
   modified:2005-05-01

   use _UNICODE and _DEBUG_
   pay attention to:
       Borland C++:malloc(0) then return NULL
*/
#include "cc.Str.h"
#include "cc.Exception.h"
using namespace cc;

//===========================================================cc::Object
///////////////////////////////////////////////////////////////////////////////
cc::Str Object::GetClassName() const
{
	//need "/GR" while compile to active _CPPRTTI
#if !defined(_CPPRTTI)
	return DoGetClassName();
#else
	return cc::Str(typeid(*this).name());
#endif
//	return _T("cc::Object");
}

cc::Str Object::DoGetClassName() const
{
	return cc::Str(_T("class cc::Str"));
}

bool Object::IsClass(const TCHAR* className)
{
	return (this->GetClassName().Equals(className));
}

bool Object::IsClass(const cc::Str& className)
{
	return (this->GetClassName().Equals(className));
}

cc::Str Object::ToString() const
{
	return this->GetClassName().Append(cc::Str().Format(_T("@%d"), this));
}

float StrConfig::nIncrement = (float)1.3;
//unsigned int StrConfig::nIncrementMaxLen = 1024000;
//===========================================================cc::Str
///////////////////////////////////////////////////////////////////////////////
///////////////////// constructors //////////////////////////
Str::Str()
{
	CC_DEFINE_CLASSID(Str);
	_Init();
}

//Str::Str(unsigned int nMinCapacity)
//{
//	_Init();
//	if((int)nMinCapacity > 0)
//	{
//		_nMinCapacity = nMinCapacity;
//		_Alloc(0, false);
//	}
//}

Str::Str(unsigned int nMinCapacity, double incPercentMoreThan1)
{
	_Init();
	if(incPercentMoreThan1 > 1)
	{
		_nIncrement = (float)incPercentMoreThan1;
	}
	if((int)nMinCapacity > 0)
	{
		_nMinCapacity = nMinCapacity;
		_Alloc(0, false);
	}
}
//
//Str::Str(const char str)
//{
//	_Init();
//	Format("%c", str);
//}

Str::Str(const char *str, LangCode::Type fromLangCode)
{
	_Init();
	_StrCopy(str, fromLangCode, Len(str), 0, -1);
}

Str::Str(const char* str, int fromIndex, LangCode::Type fromLangCode)
{
	_Init();
	_StrCopy(str, fromLangCode, Len(str), fromIndex, -1);
}

//len:<0 for to end
Str::Str(const char* str, int fromIndex, int len, LangCode::Type fromLangCode)
{
	_Init();
	_StrCopy(str, fromLangCode, Len(str), fromIndex, len);
}
//
//Str::Str(const wchar_t str)
//{
//	_Init();
//	Format(L"%c", str);
//}

Str::Str(const wchar_t *str)
{
	_Init();
	_StrCopy(str, Len(str), 0, -1);
}

Str::Str(const wchar_t* str, int fromIndex)
{
	_Init();
	_StrCopy(str, Len(str), fromIndex, -1);
}

//len:<0 for to end
Str::Str(const wchar_t* str, int fromIndex, int len)
{
	_Init();
	_StrCopy(str, Len(str), fromIndex, len);
}

Str::Str(const Str& str)
{
	_Init();
	_savedLang = str.GetLangCode();
	if(str._savedLang == LangCode::UNICODE16)
	{
		_StrCopy((wchar_t*)str.GetBuf(), str.GetLength(), 0, -1);
	}
	else
	{
		_StrCopy((char*)str.GetBuf(), str.GetLangCode(), str.GetLength(), 0, -1);
	}
	_Flags = str._Flags;
}

Str::Str(const Str& str, int fromIndex)
{
	_Init();
	_savedLang = str.GetLangCode();
	if(str._savedLang == LangCode::UNICODE16)
	{
		_StrCopy((wchar_t*)str.GetBuf(), str.GetLength(), fromIndex, -1);
	}
	else
	{
		_StrCopy((char*)str.GetBuf(), str.GetLangCode(), str.GetLength(), fromIndex, -1);
	}
	_Flags = str._Flags;
}

//len:<0 for to end
Str::Str(const Str& str, int fromIndex, int len)
{
	_Init();
	_savedLang = str.GetLangCode();
	if(str._savedLang == LangCode::UNICODE16)
	{
		_StrCopy((wchar_t*)str.GetBuf(), str.GetLength(), fromIndex, len);
	}
	else
	{
		_StrCopy((char*)str.GetBuf(), str.GetLangCode(), str.GetLength(), fromIndex, len);
	}
	_Flags = str._Flags;
}

Str::Str(LangCode::Type saveLangCode)
{
	_Init();
	_savedLang = saveLangCode;
}

Str::Str(LangCode::Type saveLangCode, unsigned int nMinCapacity)
{
	_Init();
	_savedLang = saveLangCode;
	if((int)nMinCapacity > 0)
	{
		_nMinCapacity = nMinCapacity;
		_Alloc(0, false);
	}
}

Str::Str(LangCode::Type saveLangCode, unsigned int nMinCapacity, double incPercentMoreThan1)
{
	_Init();
	_savedLang = saveLangCode;
	if(incPercentMoreThan1 > 1)
	{
		_nIncrement = (float)incPercentMoreThan1;
	}
	if((int)nMinCapacity > 0)
	{
		_nMinCapacity = nMinCapacity;
		_Alloc(0, false);
	}
}

Str::Str(LangCode::Type saveLangCode, const char *str, LangCode::Type fromLangCode)
{
	_Init();
	_savedLang = saveLangCode;
	_StrCopy(str, fromLangCode, Len(str), 0, -1);
}

Str::Str(LangCode::Type saveLangCode, const char* str, int fromIndex, LangCode::Type fromLangCode)
{
	_Init();
	_savedLang = saveLangCode;
	_StrCopy(str, fromLangCode, Len(str), fromIndex, -1);
}

Str::Str(LangCode::Type saveLangCode, const char* str, int fromIndex, int len, LangCode::Type fromLangCode)
{
	_Init();
	_savedLang = saveLangCode;
	_StrCopy(str, fromLangCode, Len(str), fromIndex, len);
}

Str::Str(LangCode::Type saveLangCode, const wchar_t *str)
{
	_Init();
	_savedLang = saveLangCode;
	_StrCopy(str, Len(str), 0, -1);
}

Str::Str(LangCode::Type saveLangCode, const wchar_t* str, int fromIndex)
{
	_Init();
	_savedLang = saveLangCode;
	_StrCopy(str, Len(str), fromIndex, -1);
}

Str::Str(LangCode::Type saveLangCode, const wchar_t* str, int fromIndex, int len)
{
	_Init();
	_savedLang = saveLangCode;
	_StrCopy(str, Len(str), fromIndex, len);
}

Str::Str(LangCode::Type saveLangCode, const Str& str)
{
	_Init();
	_savedLang = saveLangCode;
	if(str._savedLang == LangCode::UNICODE16)
	{
		_StrCopy((wchar_t*)str.GetBuf(), str.GetLength(), 0, -1);
	}
	else
	{
		_StrCopy((char*)str.GetBuf(), str.GetLangCode(), str.GetLength(), 0, -1);
	}
	_Flags = str._Flags;
}

Str::Str(LangCode::Type saveLangCode, const Str& str, int fromIndex)
{
	_Init();
	_savedLang = saveLangCode;
	if(str._savedLang == LangCode::UNICODE16)
	{
		_StrCopy((wchar_t*)str.GetBuf(), str.GetLength(), fromIndex, -1);
	}
	else
	{
		_StrCopy((char*)str.GetBuf(), str.GetLangCode(), str.GetLength(), fromIndex, -1);
	}
	_Flags = str._Flags;
}

Str::Str(LangCode::Type saveLangCode, const Str& str, int fromIndex, int len)
{
	_Init();
	_savedLang = saveLangCode;
	if(str._savedLang == LangCode::UNICODE16)
	{
		_StrCopy((wchar_t*)str.GetBuf(), str.GetLength(), fromIndex, len);
	}
	else
	{
		_StrCopy((char*)str.GetBuf(), str.GetLangCode(), str.GetLength(), fromIndex, len);
	}
	_Flags = str._Flags;
}

///////////////////// destructor //////////////////////////
Str::~Str()
{
	if(_pBuf != NULL)
	{
		free(_pBuf);
		_pBuf = NULL;
	}
}

///////////////////// private/internal used methods //////////////////////////
void Str::_Init()
{
	_pBuf = NULL;
#if _DEBUG_
	//for show contents with mouse on while debug
	_pBufW = (wchar_t*)_pBuf;
	_pBufC = (char*)_pBuf;
#endif //#if _DEBUG_
	//_isNULL = TRUE;
	_Flags = 0;
	CC_BIT_ON(_Flags, CC_STR_NULL);
	#if defined (_UNICODE)
	_savedLang = LangCode::UNICODE16;
	#else
	_savedLang = LangCode::SYSTEM;
	#endif
	_nCapacity = 0;
	_nMinCapacity = 0;
	_nRealCapacity = 0;
	//multiple to increase
	//_nIncrement = 1.3;
	_nIncrement = StrConfig::nIncrement;
}

/*
pay attention to:
  _pBuf may be changed by this process,so you need adjust this.
*/
void Str::_Alloc(int len, bool isPreserveBuf)
{
	//_isNULL = FALSE;
	CC_BIT_OFF(_Flags, CC_STR_NULL);
	unsigned int nOldLen = _nCapacity;
	unsigned int nNeedLen = (len < 0) ? 0 : len;
	unsigned int nNeedBufLen = (nNeedLen < _nMinCapacity) ? _nMinCapacity : nNeedLen;
	int nSizeChar = (_savedLang == LangCode::UNICODE16) ? sizeof(wchar_t) : sizeof(char);
	//first time and (_pBuf == NULL),then creat real needed size
	if(_pBuf == NULL)
	{
		_pBuf = (void*)malloc((nNeedBufLen + 1) * nSizeChar);
#if _DEBUG_
		//for show contents with mouse on while debug
		_pBufW = (wchar_t*)_pBuf;
		_pBufC = (char*)_pBuf;
#endif //#if _DEBUG_
		if(_pBuf == NULL)
		{
			// throw:g++ only,if gcc then error!;
			#ifdef _DEBUG_
			_tprintf(_T("Err:malloc(%d)\n"), (nNeedBufLen + 1) * nSizeChar);
			#endif //#ifdef _DEBUG_
			throw;
		}
	}
	else
	{
		unsigned int nIncLen = (unsigned int)(nNeedBufLen * _nIncrement);
		//if(nIncLen > nNeedBufLen + StrConfig::nIncrementMaxLen)
		//{
		//	nIncLen = nNeedBufLen + StrConfig::nIncrementMaxLen;
		//}
		if(nNeedBufLen <= _nRealCapacity && _nRealCapacity <= nIncLen)
		{
			nNeedBufLen = _nRealCapacity;
		}
		else
		{
			nNeedBufLen = nIncLen;
			//realloc is malloc&copy,but !isPreserveBuf is not need copy
			void* _pBufOld = NULL;
			if(!isPreserveBuf && nNeedBufLen > _nRealCapacity)
			{
				free(_pBuf);
				_pBuf = (void*)malloc((nNeedBufLen + 1) * nSizeChar);
			}
			else
			{
				_pBufOld = _pBuf;
				_pBuf = (void*)realloc(_pBuf, (nNeedBufLen + 1) * nSizeChar);
			}
			if(_pBuf == NULL)
			{
				//free _pBufOld only need while realloc has error, otherwist freed by realloc
				if(_pBufOld != NULL)
				{
					free(_pBufOld);
					_pBufOld = NULL;
				}
				// throw:g++ only,if gcc then error!;
				#ifdef _DEBUG_
				_tprintf(_T("Err:realloc(%d)\n"), (nNeedBufLen + 1) * nSizeChar);
				#endif //#ifdef _DEBUG_
				throw;
			}
		}
	}
	if(_savedLang == LangCode::UNICODE16)
	{
		if(!isPreserveBuf)
		{
			wmemset((wchar_t*)_pBuf, L'\0', (nNeedBufLen + 1));
		}
		else if(nOldLen < nNeedLen)
		{
			wmemset((wchar_t*)_pBuf + nOldLen, L'\0', (nNeedBufLen - nOldLen + 1));
		}
		((wchar_t*)_pBuf)[nNeedLen] = L'\0';
	}
	else
	{
		if(!isPreserveBuf)
		{
			memset((char*)_pBuf, '\0', (nNeedBufLen + 1));
		}
		else if(nOldLen < nNeedLen)
		{
			memset((char*)_pBuf + nOldLen, '\0', (nNeedBufLen - nOldLen + 1));
		}
		((char*)_pBuf)[nNeedLen] = '\0';
	}
	_nRealCapacity = nNeedBufLen;
	_nCapacity = nNeedLen;
}

inline int Str::Len(const char *str)
{
	return (str == NULL) ? 0 : (int)strlen(str);
}

inline int Str::Len(const wchar_t *str)
{
	return (str == NULL) ? 0 : (int)wcslen(str);
}

int Str::Cmp(const char *str, const char *str2, bool ignoreCase)
{
	if(str == NULL && str2 == NULL)
	{
		return 0;
	}
	if(str == NULL)
	{
		return -1;
	}
	if(str2 == NULL)
	{
		return 1;
	}
	if(ignoreCase)
	{
		#ifdef __BORLANDC__
		return stricmp(str, str2);
		#else
		return _stricmp(str, str2);
		#endif /* __BORLANDC__ */
	}
	else
	{
		return strcmp(str, str2);
	}
}

int Str::Cmp(const wchar_t *str, const wchar_t *str2, bool ignoreCase)
{
	if(str == NULL && str2 == NULL)
	{
		return 0;
	}
	if(str == NULL)
	{
		return -1;
	}
	if(str2 == NULL)
	{
		return 1;
	}
	if(ignoreCase)
	{
		return _wcsicmp(str, str2);
	}
	else
	{
		return wcscmp(str, str2);
	}
}

int Str::NCmp(const char *str, const char *str2, int len, bool ignoreCase)
{
	if(str == NULL && str2 == NULL)
	{
		return 0;
	}
	if(str == NULL)
	{
		return -1;
	}
	if(str2 == NULL)
	{
		return 1;
	}
	//treate as len != 0
	if(ignoreCase)
	{
		#ifdef __BORLANDC__
		return strnicmp(str, str2, len);
		#else
		return _strnicmp(str, str2, len);
		#endif /* __BORLANDC__ */
	}
	else
	{
		return strncmp(str, str2, len);
	}
}

int Str::NCmp(const wchar_t *str, const wchar_t *str2, int len, bool ignoreCase)
{
	if(str == NULL && str2 == NULL)
	{
		return 0;
	}
	if(str == NULL)
	{
		return -1;
	}
	if(str2 == NULL)
	{
		return 1;
	}
	//len=0?
	if(ignoreCase)
	{
		return _wcsnicmp(str, str2, len);
	}
	else
	{
		return wcsncmp(str, str2, len);
	}
}

int Str::Search(const char *buf, const char *str, int strlen, bool ignoreCase)
{
	return Search(buf, 0, -1, str, strlen, ignoreCase);
}
//as C#(IndexOf)
//The search begins at startIndex and continues to startIndex + count-1.
//The character at startIndex + count is not included in the search.
int Str::Search(const char *buf, int startIndex, int count, const char *str, int strlen, bool ignoreCase)
{
	if(buf == NULL && str == NULL)
	{
		return 0;
	}
	if(buf == NULL || str == NULL)
	{
		return -1;
	}
	if(strlen < 0)
	{
		strlen = Len(str);
	}
	if(strlen <= 0)
	{
		return 0;
	}

	const char *orgbuf = buf;
	const char *p1;
	const char *p2;
	const char *endbuf = NULL;
	if(count >= 0)
	{
		endbuf = buf + startIndex + count;
	}
	buf += startIndex;
	if(ignoreCase)
	{
		int	toUp = 'a' - 'A';
		while(true)
		{
			while(*buf != '\0' && (endbuf == NULL || buf < endbuf)
				&& (*buf != *str && (*buf > 90  || *buf < 65 || *buf != *str - toUp)
				&& (*buf > 122 || *buf < 97 || *buf != *str + toUp)))
			{
				buf++;
			}
			if(*buf == '\0' || (endbuf != NULL && buf + strlen > endbuf))
			{
				return -1;
			}
			p1 = buf + 1;
			p2 = str + 1;
			while(p2 - str < strlen && (*p1 == *p2
				|| (*p1 <= 90  && *p1 >= 65 && *p1 == *p2 - toUp)
				|| (*p1 <= 122 && *p1 >= 97 && *p1 == *p2 + toUp)))
			{
				p1++;
				p2++;
			}
			if(p2 - str == strlen)
			{
				return (int)(buf - orgbuf);
			}
			buf++;
		}
		return -1;
	}
	else
	{
		//return strstr(buf, str);
		while(true)
		{
			while(*buf != '\0' && (endbuf == NULL || buf < endbuf) && *buf != *str)
			{
				buf++;
			}
			if(*buf == '\0' || (endbuf != NULL && buf + strlen > endbuf))
			{
				return -1;
			}
			p1 = buf + 1;
			p2 = str + 1;
			while(p2 - str < strlen && *p1 == *p2)
			{
				p1++;
				p2++;
			}
			if(p2 - str == strlen)
			{
				return (int)(buf - orgbuf);
			}
			buf++;
		}
		return -1;
	}
}

int Str::Search(const wchar_t *buf, const wchar_t *str, int strlen, bool ignoreCase)
{
	return Search(buf, 0, -1, str, strlen, ignoreCase);
}
//as C#(IndexOf)
//The search begins at startIndex and continues to startIndex + count-1.
//The character at startIndex + count is not included in the search.
int Str::Search(const wchar_t *buf, int startIndex, int count, const wchar_t *str, int strlen, bool ignoreCase)
{
	if(buf == NULL && str == NULL)
	{
		return 0;
	}
	if(buf == NULL || str == NULL)
	{
		return -1;
	}
	if(strlen < 0)
	{
		strlen = Len(str);
	}
	if(strlen <= 0)
	{
		return 0;
	}

	const wchar_t *orgbuf = buf;
	const wchar_t *p1;
	const wchar_t *p2;
	const wchar_t *endbuf = NULL;
	if(count >= 0)
	{
		endbuf = buf + startIndex + count;
	}
	buf += startIndex;
	if(ignoreCase)
	{
		int	toUp = L'a' - L'A';
		while(true)
		{
			while(*buf != L'\0' && (endbuf == NULL || buf < endbuf)
				&& (*buf != *str && (*buf > 90  || *buf < 65 || *buf != *str - toUp)
				&& (*buf > 122 || *buf < 97 || *buf != *str + toUp)))
			{
				buf++;
			}
			if(*buf == L'\0' || (endbuf != NULL && buf + strlen > endbuf))
			{
				return -1;
			}
			p1 = buf + 1;
			p2 = str + 1;
			while(p2 - str < strlen && (*p1 == *p2
				|| (*p1 <= 90  && *p1 >= 65 && *p1 == *p2 - toUp)
				|| (*p1 <= 122 && *p1 >= 97 && *p1 == *p2 + toUp)))
			{
				p1++;
				p2++;
			}
			if(p2 - str == strlen)
			{
				return (int)(buf - orgbuf);
			}
			buf++;
		}
		return -1;
	}
	else
	{
		//return strstr(buf, str);
		while(true)
		{
			while(*buf != L'\0' && (endbuf == NULL || buf < endbuf) && *buf != *str)
			{
				buf++;
			}
			if(*buf == L'\0' || (endbuf != NULL && buf + strlen > endbuf))
			{
				return -1;
			}
			p1 = buf + 1;
			p2 = str + 1;
			while(p2 - str < strlen && *p1 == *p2)
			{
				p1++;
				p2++;
			}
			if(p2 - str == strlen)
			{
				return (int)(buf - orgbuf);
			}
			buf++;
		}
		return -1;
	}
}

//while c#,"a".LastIndexOf("") ==> 0
//The search begins at the startIndex(from begin) character position of this instance
//and proceeds backwards towards the beginning until either value is found
//or count character positions have been examined.
int Str::SearchLast(const char *buf, const char *str, int strlen, bool ignoreCase)
{
	return SearchLast(buf, Len(buf), -1, str, strlen, ignoreCase);
}
//count:the len of str to seek, startIndex is count from ahead
int Str::SearchLast(const char *buf, int startIndex, int count, const char *str, int strlen, bool ignoreCase)
{
	if(buf == NULL && str == NULL)
	{
		return 0;
	}
	if(buf == NULL || str == NULL)
	{
		return -1;
	}
	if(strlen < 0)
	{
		strlen = Len(str);
	}
	int buflen = Len(buf);
	if(strlen <= 0)
	{
		//like c#,set it to GetLength() - 1
		if(buflen > 0)
		{
			return buflen - 1;
		}
		return 0;
	}

	if(startIndex <= 0 || strlen > startIndex || (count >= 0 && count < strlen))
	{
		return -1;
	}
	if(startIndex > buflen)
	{
		startIndex = buflen;
	}
	const char *p1;
	const char *p2;
	const char *p = buf + startIndex - strlen;
	const char *startp = buf;
	if(count >= 0)
	{
		startp = buf + startIndex - count;
		if(startp < buf)
		{
			startp = buf;
		}
	}
	if(ignoreCase)
	{
		int	toUp = 'a' - 'A';

		while(true)
		{
			while(p >= startp && (*p != *str
				&& (*p > 90  || *p < 65 || *p != *str - toUp)
				&& (*p > 122 || *p < 97 || *p != *str + toUp)))
			{
				p--;
			}
			if(p < startp)
			{
				return -1;
			}
			p1 = p + 1;
			p2 = str + 1;
			while(p2 - str < strlen && (*p1 == *p2
				|| (*p1 <= 90  && *p1 >= 65 && *p1 == *p2 - toUp)
				|| (*p1 <= 122 && *p1 >= 97 && *p1 == *p2 + toUp)))
			{
				p1++;
				p2++;
			}
			if(p2 - str == strlen)
			{
				return (int)(p - buf);
			}
			p--;
		}
		return -1;
	}
	else
	{
		while(true)
		{
			while(p >= startp && *p != *str)
			{
				p--;
			}
			if(p < startp)
			{
				return -1;
			}
			p1 = p + 1;
			p2 = str + 1;
			while(p2 - str < strlen && *p1 == *p2)
			{
				p1++;
				p2++;
			}
			if(p2 - str == strlen)
			{
				return (int)(p - buf);
			}
			p--;
		}
		return -1;
	}
}

int Str::SearchLast(const wchar_t *buf, const wchar_t *str, int strlen, bool ignoreCase)
{
	return SearchLast(buf, Len(buf), -1, str, strlen, ignoreCase);
}
//count:the len of str to seek, startIndex is count from ahead
int Str::SearchLast(const wchar_t *buf, int startIndex, int count, const wchar_t *str, int strlen, bool ignoreCase)
{
	if(buf == NULL && str == NULL)
	{
		return 0;
	}
	if(buf == NULL || str == NULL)
	{
		return -1;
	}
	if(strlen < 0)
	{
		strlen = Len(str);
	}
	int buflen = Len(buf);
	if(strlen <= 0)
	{
		//like c#,set it to GetLength() - 1
		if(buflen > 0)
		{
			return buflen - 1;
		}
		return 0;
	}

	if(startIndex <= 0 || strlen > startIndex || (count >= 0 && count < strlen))
	{
		return -1;
	}
	if(startIndex > buflen)
	{
		startIndex = buflen;
	}
	const wchar_t *p1;
	const wchar_t *p2;
	const wchar_t *p = buf + startIndex - strlen;
	const wchar_t *startp = buf;
	if(count >= 0)
	{
		startp = buf + startIndex - count;
		if(startp < buf)
		{
			startp = buf;
		}
	}
	if(ignoreCase)
	{
		int	toUp = L'a' - L'A';

		while(true)
		{
			while(p >= startp && (*p != *str
				&& (*p > 90  || *p < 65 || *p != *str - toUp)
				&& (*p > 122 || *p < 97 || *p != *str + toUp)))
			{
				p--;
			}
			if(p < startp)
			{
				return -1;
			}
			p1 = p + 1;
			p2 = str + 1;
			while(p2 - str < strlen && (*p1 == *p2
				|| (*p1 <= 90  && *p1 >= 65 && *p1 == *p2 - toUp)
				|| (*p1 <= 122 && *p1 >= 97 && *p1 == *p2 + toUp)))
			{
				p1++;
				p2++;
			}
			if(p2 - str == strlen)
			{
				return (int)(p - buf);
			}
			p--;
		}
		return -1;
	}
	else
	{
		while(true)
		{
			while(p >= startp && *p != *str)
			{
				p--;
			}
			if(p < startp)
			{
				return -1;
			}
			p1 = p + 1;
			p2 = str + 1;
			while(p2 - str < strlen && *p1 == *p2)
			{
				p1++;
				p2++;
			}
			if(p2 - str == strlen)
			{
				return (int)(p - buf);
			}
			p--;
		}
		return -1;
	}
}

char *Str::Move(char *des, const char *src, int len)
{
	char *p = (char *)des;
	const char *s = (char *)src;
	if(p >= s)
	{
		while(len-- > 0)
		{
			*(p + len) = *(s + len);
		}
	}
	else
	{
		while(len-- > 0)
		{
			*p++ = *s++;
		}
	}
	return des;
}

wchar_t *Str::Move(wchar_t *des, const wchar_t *src, int len)
{
	wchar_t *p = (wchar_t *)des;
	const wchar_t *s = (wchar_t *)src;
	if(p >= s)
	{
		while(len-- > 0)
		{
			*(p + len) = *(s + len);
		}
	}
	else
	{
		while(len-- > 0)
		{
			*p++ = *s++;
		}
	}
	return des;
}

//copyLen:<0 for to end
//copy from str to buf,if need, change language
//copy only while strByteLen > 0 && copyByteLen > 0
void Str::_StrCopy(const char *str, LangCode::Type fromLangCode, int strLen, int fromIndex, int copyLen)
{
	if(str == NULL || fromIndex < 0)
	{
		_Free();
		return;
	}
	if(fromIndex >= 0 && strLen > fromIndex && fromLangCode != LangCode::UNICODE16)
	{
		if(_pBuf != NULL)
		{
			_Alloc(0, false);
		}
		_StrIns(str, fromLangCode, strLen, 0, fromIndex, copyLen);
	}
	else
	{
		_Alloc(0, false);
	}
}

void Str::_StrCopy(const wchar_t *str, int strLen, int fromIndex, int copyLen)
{
	if(str == NULL || fromIndex < 0)
	{
		_Free();
		return;
	}
	if(fromIndex >= 0 && strLen > fromIndex)
	{
		if(_pBuf != NULL)
		{
			_Alloc(0, false);
		}
		_StrIns(str, strLen, 0, fromIndex, copyLen);
	}
	else
	{
		_Alloc(0, false);
	}
}

//copyLen:<0 for to end
void Str::_StrIns(const char *str, LangCode::Type fromLangCode, int strLen, int insPos, int fromIndex, int copyLen)
{
	CC_ASSERT(fromLangCode != LangCode::UNICODE16);

	if(str == NULL || strLen < 0 || fromIndex < 0 ||  fromIndex > strLen)
	{
		return;
	}
	//Insert "" while NULL
	if(strLen == 0 && CC_BIT_HAS(_Flags, CC_STR_NULL))
	{
		_Alloc(0);
		return;
	}

	unsigned int nOldLen = GetLength();
	if(nOldLen < 0)
	{
		nOldLen = 0;
	}
	if(insPos < 0)
	{
		insPos = 0;
	}
	if((unsigned int)insPos > nOldLen)
	{
		insPos = nOldLen;
	}
	if(copyLen < 0 || copyLen > (strLen - fromIndex))
	{
		copyLen = strLen - fromIndex;
	}
	if(_savedLang == fromLangCode)
	{
		_Alloc(nOldLen + copyLen, true);
		if((unsigned int)insPos < nOldLen)
		{
			if(_savedLang == LangCode::UNICODE16)
			{
				Move((wchar_t*)_pBuf + insPos + copyLen, (wchar_t*)_pBuf + insPos, nOldLen - insPos);
			}
			else
			{
				Move((char*)_pBuf + insPos + copyLen, (char*)_pBuf + insPos, nOldLen - insPos);
			}
		}
//#ifdef __STDC_SECURE_LIB__
//		//strncpy_s((char*)_pBuf + insPos, copyLen + 1, str + fromIndex, copyLen);
//		memmove_s((char*)_pBuf + insPos, copyLen + 1, str + fromIndex, copyLen);
//#else
//		//strncpy((char*)_pBuf + insPos, str + fromIndex, copyLen);
//		memmove((char*)_pBuf + insPos, str + fromIndex, copyLen);
//#endif
		Move((char*)_pBuf + insPos, str + fromIndex, copyLen);
	}
	else if(_savedLang == LangCode::UNICODE16)
	{
		int nbytes = MultiByteToWideChar((int)fromLangCode, 0, str + fromIndex, copyLen, 0, 0);
		if(nbytes > 0)
		{
			_Alloc(nOldLen + nbytes, true);
			if((unsigned int)insPos < nOldLen)
			{
				Move((wchar_t*)_pBuf + insPos + nbytes, (wchar_t*)_pBuf + insPos, nOldLen - insPos);
			}
			MultiByteToWideChar((int)fromLangCode, 0, str + fromIndex, copyLen, (wchar_t*)_pBuf + insPos, nbytes*2);
		}
	}
	else
	{
		int nbytes = MultiByteToWideChar((int)fromLangCode, 0, str + fromIndex, copyLen, 0, 0);
		if(nbytes > 0)
		{
			wchar_t *cDes = (wchar_t*)malloc((nbytes + 1) * sizeof(wchar_t));
			memset(cDes, L'\0', (nbytes + 1) * sizeof(wchar_t));
			//Returns the number of WCHAR values written to the buffer indicated by lpWideCharStr if successful.
			//If the function succeeds and cchWideChar is 0, the return value is the required size for the buffer indicated by lpWideCharStr.
			MultiByteToWideChar((int)fromLangCode, 0, (const char*)str + fromIndex, copyLen, cDes, nbytes);
			//Returns the number of bytes written to the buffer pointed to by lpMultiByteStr if successful.
			//!!The number includes the byte for the terminating null character.!!
			//nbytes = WideCharToMultiByte((int)_savedLang, 0, cDes, -1, 0, 0, 0, 0);
			nbytes = WideCharToMultiByte((int)_savedLang, 0, cDes, nbytes, 0, 0, 0, 0);
			if(nbytes > 0)
			{
				_Alloc(nOldLen + nbytes, true);
				if((unsigned int)insPos < nOldLen)
				{
					Move((char*)_pBuf + insPos + nbytes, (char*)_pBuf + insPos, nOldLen - insPos);
				}
				WideCharToMultiByte((int)_savedLang, 0, cDes, -1, (char*)_pBuf + insPos, nbytes, 0, 0);
			}
			free(cDes);
		}
	}
#if _DEBUG_
	//for show contents with mouse on while debug
	_pBufW = (wchar_t*)_pBuf;
	_pBufC = (char*)_pBuf;
#endif //#if _DEBUG_
}

void Str::_StrIns(const wchar_t *str, int strLen, int insPos, int fromIndex, int copyLen)
{
	if(str == NULL || strLen < 0 || fromIndex < 0 || fromIndex > strLen)
	{
		return;
	}
	//Insert "" while NULL
	if(strLen == 0 && CC_BIT_HAS(_Flags, CC_STR_NULL))
	{
		_Alloc(0);
		return;
	}

	unsigned int nOldLen = GetLength();
	if(nOldLen < 0)
	{
		nOldLen = 0;
	}
	if(insPos < 0)
	{
		insPos = 0;
	}
	if((unsigned int)insPos > nOldLen)
	{
		insPos = nOldLen;
	}
	if(copyLen < 0 || copyLen > (strLen - fromIndex))
	{
		copyLen = strLen - fromIndex;
	}
	if(_savedLang != LangCode::UNICODE16)
	{
		//void *to = NULL;
		int nbytes = WideCharToMultiByte((int)_savedLang, 0, str + fromIndex, copyLen, 0, 0, 0, 0);
		if(nbytes > 0)
		{
			_Alloc(nOldLen + nbytes, true);
			if((unsigned int)insPos < nOldLen)
			{
				Move((char*)_pBuf + insPos + nbytes, (char*)_pBuf + insPos, nOldLen - insPos);
			}
			WideCharToMultiByte((int)_savedLang, 0, str + fromIndex, copyLen, (char*)_pBuf + insPos, nbytes, 0, 0);
		}
	}
	else
	{
		_Alloc(nOldLen + copyLen, true);
		if((unsigned int)insPos < nOldLen)
		{
			Move((wchar_t*)_pBuf + insPos + copyLen, (wchar_t*)_pBuf + insPos, nOldLen - insPos);
		}
//#ifdef __STDC_SECURE_LIB__
//		//wcsncpy_s((wchar_t*)_pBuf + insPos, copyLen + 1, str + fromIndex, copyLen);
//		memmove_s((wchar_t*)_pBuf + insPos, (copyLen + 1) * sizeof(wchar_t), str + fromIndex, copyLen * sizeof(wchar_t));
//#else
//		//wcsncpy((wchar_t*)_pBuf + insPos, str + fromIndex, copyLen);
//		memmove((wchar_t*)_pBuf + insPos, str + fromIndex, copyLen * sizeof(wchar_t));
//#endif
		Move((wchar_t*)_pBuf + insPos, str + fromIndex, copyLen);
	}
#if _DEBUG_
	//for show contents with mouse on while debug
	_pBufW = (wchar_t*)_pBuf;
	_pBufC = (char*)_pBuf;
#endif //#if _DEBUG_
}

// mem free?
// TCHAR *p = (TCHAR*)malloc(100);
// p += 40;
// free(p);
// ====> only relase 60/100!!!
inline void Str::_Free()
{
	//if(_nMinCapacity == 0 || isRealFree)
	//{
		if(_pBuf != NULL)
		{
			free(_pBuf);
			_pBuf = NULL;
		}
		_nRealCapacity = 0;
	//}
	//else
	//{
	//	_Alloc(0, false);
	//}
	//_isNULL = TRUE;
	CC_BIT_ON(_Flags, CC_STR_NULL);
	_nCapacity = 0;
#if _DEBUG_
	//for show contents with mouse on while debug
	_pBufW = (wchar_t*)_pBuf;
	_pBufC = (char*)_pBuf;
#endif //#if _DEBUG_
}

cc::Str Str::DoGetClassName() const
{
	return cc::Str(_T("cc::Str"));
}

///////////////////////////////////////////////////////////////////////////////
// C# LIKE
Str Str::Clone()
{
	return *this;
}

int Str::Compare(const char *strA, int indexA, const char *strB, int indexB, int length, bool ignoreCase)
{
	if(indexA < 0 || indexB < 0 || length < 0
		|| (strA == NULL && indexA > 0) || (strB == NULL && indexB > 0))
	{
#if defined(CC_EXCEPTION_H_)
		throw ArgumentException(_T("Argument Out Of Range Exception"));
#else
		throw _T("Argument Out Of Range Exception");
#endif
	}
	if(strA == NULL && strB == NULL)
	{
		return 0;
	}
	if(strA == NULL)
	{
		return -1;
	}
	if(strB == NULL)
	{
		return 1;
	}
	int lenA = (int)strlen(strA);
	int lenB = (int)strlen(strB);
	if(lenA < indexA || lenB < indexB)
	{
#if defined(CC_EXCEPTION_H_)
		throw ArgumentException(_T("Argument Out Of Range Exception"));
#else
		throw _T("Argument Out Of Range Exception");
#endif
	}
	return NCmp(strA + indexA, strB + indexB, length, ignoreCase);
}

int Str::Compare(const wchar_t *strA, int indexA, const wchar_t *strB, int indexB, int length, bool ignoreCase)
{
	if(indexA < 0 || indexB < 0 || length < 0
		|| (strA == NULL && indexA > 0) || (strB == NULL && indexB > 0))
	{
#if defined(CC_EXCEPTION_H_)
		throw ArgumentException(_T("Argument Out Of Range Exception"));
#else
		throw _T("Argument Out Of Range Exception");
#endif
	}
	if(strA == NULL && strB == NULL)
	{
		return 0;
	}
	if(strA == NULL)
	{
		return -1;
	}
	if(strB == NULL)
	{
		return 1;
	}
	int lenA = (int)wcslen(strA);
	int lenB = (int)wcslen(strB);
	if(lenA < indexA || lenB < indexB)
	{
#if defined(CC_EXCEPTION_H_)
		throw ArgumentException(_T("Argument Out Of Range Exception"));
#else
		throw _T("Argument Out Of Range Exception");
#endif
	}
	return NCmp(strA + indexA, strB + indexB, length, ignoreCase);
}

int Str::Compare(const Str& strA, int indexA, const Str& strB, int indexB, int length, bool ignoreCase)
{
	if(strA.GetLangCode() == strB.GetLangCode())
	{
		if(strA.IsUnicode())
		{
			return Compare((wchar_t*)strA.GetBuf(), indexA, (wchar_t*)strB.GetBuf(), indexB, length, ignoreCase);
		}
		else
		{
			return Compare((char*)strA.GetBuf(), indexA, (char*)strB.GetBuf(), indexB, length, ignoreCase);
		}
	}
	else
	{
		return Compare(strA, indexA, Str(strA.GetLangCode(), strB), indexB, length, ignoreCase);
	}
}

int Str::Compare(const char *strA, const char *strB, bool ignoreCase)
{
	return Cmp(strA, strB, ignoreCase);
}

int Str::Compare(const wchar_t *strA, const wchar_t *strB, bool ignoreCase)
{
	return Cmp(strA, strB, ignoreCase);
}

int Str::Compare(const Str& strA, const Str& strB, bool ignoreCase)
{
	if(strA.GetLangCode() == strB.GetLangCode())
	{
		if(strA.IsUnicode())
		{
			return Compare((wchar_t*)strA.GetBuf(), (wchar_t*)strB.GetBuf(), ignoreCase);
		}
		else
		{
			return Compare((char*)strA.GetBuf(), (char*)strB.GetBuf(), ignoreCase);
		}
	}
	else
	{
		return Compare(strA, Str(strA.GetLangCode(), strB), ignoreCase);
	}
}
// C# LIKE
///////////////////////////////////////////////////////////////////////////////

///////////////////// JAVA LIKE //////////////////////////
void Str::Clear()
{
	_Free();
}

//please forget this,change with compiler.
int Str::CharAt(int Index) const
{
	return operator[](Index);
}

int Str::CompareTo(const char* str, bool ignoreCase, LangCode::Type fromLangCode) const
{
	if(_savedLang != fromLangCode)
	{
		return CompareTo(Str(_savedLang, str, fromLangCode), ignoreCase);
	}
	int nRetu = Cmp((char*)_pBuf, str, ignoreCase);
	if(nRetu > 0)
	{
		return 1;
	}
	else if(nRetu < 0)
	{
		return -1;
	}
	else
	{
		return 0;
	}
}

int Str::CompareTo(const wchar_t* str, bool ignoreCase) const
{
	if(_savedLang != LangCode::UNICODE16)
	{
		return CompareTo(Str(_savedLang, str), ignoreCase);
	}
	int nRetu = Cmp((wchar_t*)_pBuf, str, ignoreCase);
	if(nRetu > 0)
	{
		return 1;
	}
	else if(nRetu < 0)
	{
		return -1;
	}
	else
	{
		return 0;
	}
}

int Str::CompareTo(const Str& str, bool ignoreCase) const
{
	if(str._savedLang == LangCode::UNICODE16)
	{
		return CompareTo((wchar_t*)str.GetBuf(), ignoreCase);
	}
	else
	{
		return CompareTo((char*)str.GetBuf(), ignoreCase, str.GetLangCode());
	}
}

Str Str::Concat(const char* str0, const char *str1, LangCode::Type fromLangCode)
{
	Str s = Str(str0, fromLangCode);
	s.Append(str1, fromLangCode);
	return s;
}

Str Str::Concat(const char* str0, const char *str1, const char *str2, LangCode::Type fromLangCode)
{
	Str s = Str(str0, fromLangCode);
	s.Append(str1, fromLangCode);
	s.Append(str2, fromLangCode);
	return s;
}

Str Str::Concat(const char* str0, const char *str1, const char *str2, const char *str3, LangCode::Type fromLangCode)
{
	Str s = Str(str0, fromLangCode);
	s.Append(str1, fromLangCode);
	s.Append(str2, fromLangCode);
	s.Append(str3, fromLangCode);
	return s;
}

Str Str::Concat(const wchar_t* str0, const wchar_t *str1)
{
	Str s = Str(str0);
	s.Append(str1);
	return s;
}

Str Str::Concat(const wchar_t* str0, const wchar_t *str1, const wchar_t *str2)
{
	Str s = Str(str0);
	s.Append(str1);
	s.Append(str2);
	return s;
}

Str Str::Concat(const wchar_t* str0, const wchar_t *str1, const wchar_t *str2, const wchar_t *str3)
{
	Str s = Str(str0);
	s.Append(str1);
	s.Append(str2);
	s.Append(str3);
	return s;
}

Str Str::Concat(const Str &str0, const Str &str1)
{
	Str s = Str(str0);
	s.Append(str1);
	return s;
}

Str Str::Concat(const Str &str0, const Str &str1, const Str &str2)
{
	Str s = Str(str0);
	s.Append(str1);
	s.Append(str2);
	return s;
}

Str Str::Concat(const Str &str0, const Str &str1, const Str &str2, const Str &str3)
{
	Str s = Str(str0);
	s.Append(str1);
	s.Append(str2);
	s.Append(str3);
	return s;
}

bool Str::EndsWith(const char *suffix, bool ignoreCase, LangCode::Type fromLangCode) const
{
	return EndsWith(suffix, GetLength(), ignoreCase, fromLangCode);
}

// "012345678"
// if endLength = 3,and suffix="23",then true
bool Str::EndsWith(const char *suffix, int endLength, bool ignoreCase, LangCode::Type fromLangCode) const
{
	if(suffix == NULL && CC_BIT_HAS(_Flags, CC_STR_NULL))
	{
		return true;
	}
	if(suffix == NULL || CC_BIT_HAS(_Flags, CC_STR_NULL))
	{
		return false;
	}
	int nlen = Len(suffix);
	int buflen = GetLength();
	if(endLength < 0 || endLength - nlen >= buflen || endLength - nlen < 0)
	{
		return false;
	}
	if(nlen <= 0)
	{
		return true;
	}
	if(_savedLang != fromLangCode)
	{
		return EndsWith(Str(_savedLang, suffix, fromLangCode), endLength, ignoreCase);
	}
	return (NCmp((char*)_pBuf + (endLength - nlen), suffix, nlen, ignoreCase) == 0);
}

bool Str::EndsWith(const wchar_t *suffix, bool ignoreCase) const
{
	return EndsWith(suffix, GetLength(), ignoreCase);
}

bool Str::EndsWith(const wchar_t *suffix, int endLength, bool ignoreCase) const
{
	if(suffix == NULL && CC_BIT_HAS(_Flags, CC_STR_NULL))
	{
		return true;
	}
	if(suffix == NULL || CC_BIT_HAS(_Flags, CC_STR_NULL))
	{
		return false;
	}
	int nlen = Len(suffix);
	int buflen = GetLength();
	if(endLength < 0 || endLength - nlen >= buflen || endLength - nlen < 0)
	{
		return false;
	}
	if(nlen <= 0)
	{
		return true;
	}
	if(_savedLang != LangCode::UNICODE16)
	{
		return EndsWith(Str(_savedLang, suffix), endLength, ignoreCase);
	}
	return (NCmp((wchar_t*)_pBuf + (endLength - nlen), suffix, nlen, ignoreCase) == 0);
}

bool Str::EndsWith(const Str& suffix, bool ignoreCase) const
{
	return EndsWith(suffix, GetLength(), ignoreCase);
}

bool Str::EndsWith(const Str& suffix, int endLength, bool ignoreCase) const
{
	if(suffix.IsUnicode())
	{
		return EndsWith((wchar_t*)suffix.GetBuf(), endLength, ignoreCase);
	}
	else
	{
		return EndsWith((char*)suffix.GetBuf(), endLength, ignoreCase, suffix.GetLangCode());
	}
}

bool Str::Equals(const char *a, const char *b, bool ignoreCase)
{
	return (Cmp(a, b, ignoreCase) == 0);
}

bool Str::Equals(const char* str, bool ignoreCase, LangCode::Type fromLangCode) const
{
	if(str == NULL && CC_BIT_HAS(_Flags, CC_STR_NULL))
	{
		return true;
	}
	if(str == NULL || CC_BIT_HAS(_Flags, CC_STR_NULL))
	{
		return false;
	}
	if(_savedLang != fromLangCode)
	{
		return Equals(Str(_savedLang, str, fromLangCode), ignoreCase);
	}
	return (Cmp((char*)_pBuf, str, ignoreCase) == 0);
}

bool Str::Equals(const wchar_t *a, const wchar_t *b, bool ignoreCase)
{
	return (Cmp(a, b, ignoreCase) == 0);
}

bool Str::Equals(const wchar_t* str, bool ignoreCase) const
{
	if(str == NULL && CC_BIT_HAS(_Flags, CC_STR_NULL))
	{
		return true;
	}
	if(str == NULL || CC_BIT_HAS(_Flags, CC_STR_NULL))
	{
		return false;
	}
	if(_savedLang != LangCode::UNICODE16)
	{
		return Equals(Str(_savedLang, str), ignoreCase);
	}
	return (Cmp((wchar_t*)_pBuf, str, ignoreCase) == 0);
}

bool Str::Equals(const Str &a, const Str &b, bool ignoreCase)
{
	if(a.GetLangCode() == b.GetLangCode())
	{
		if(a.IsUnicode())
		{
			return (Cmp((wchar_t*)a.GetBuf(), (wchar_t*)b.GetBuf(), ignoreCase) == 0);
		}
		else
		{
			return (Cmp((char*)a.GetBuf(), (char*)b.GetBuf(), ignoreCase) == 0);
		}
	}
	else
	{
		return Equals(a, Str(a._savedLang, b, b._savedLang), ignoreCase);
	}
}

bool Str::Equals(const Str& str, bool ignoreCase) const
{
	if(str._savedLang == LangCode::UNICODE16)
	{
		return Equals((wchar_t*)str.GetBuf(), ignoreCase);
	}
	else
	{
		return Equals((char*)str.GetBuf(), ignoreCase, str.GetLangCode());
	}
}

Str &Str::Format(const char *strFormat, ...)
{
	if(strFormat == NULL)
	{
		_Free();
		return *this;
	}
	va_list argList;
	va_start(argList, strFormat);
	FormatV(LangCode::SYSTEM, strFormat, argList);
	va_end(argList);
	return *this;
};

Str &Str::FormatV(const char *strFormat, va_list argList)
{
	FormatV(LangCode::SYSTEM, strFormat, argList);
	return *this;
};

Str &Str::Format(LangCode::Type fromLangCode, const char *strFormat, ...)
{
	if(strFormat == NULL)
	{
		_Free();
		return *this;
	}
	va_list argList;
	va_start(argList, strFormat);
	FormatV(fromLangCode, strFormat, argList);
	va_end(argList);
	return *this;
}

Str &Str::FormatV(LangCode::Type fromLangCode, const char *strFormat, va_list argList)
{
	if(strFormat == NULL)
	{
		_Free();
		return *this;
	}
	LangCode::Type oldLangCode = _savedLang;
	bool isUni = IsUnicode();
	if(_savedLang != fromLangCode)
	{
		_savedLang = fromLangCode;
	}

	int nLen = _vscprintf(strFormat, argList);
	_Alloc(nLen, false);
#ifdef __STDC_SECURE_LIB__
	int len = _vsnprintf_s((char*)_pBuf, nLen + 1, nLen, strFormat, argList);
#else
	int len = _vsnprintf((char*)_pBuf, nLen, strFormat, argList);
#endif
	/*
	int len = -1;
	int nFmt = Len(strFormat) + 1;
	int n = (int)_nRealCapacity < nFmt ? nFmt : _nRealCapacity;
	//no good way to detime the length of result
	while(true) //for out of loop,n<1024M
	{
		_Alloc(n, false);
		len = _vsnprintf((char*)_pBuf, n, strFormat, argList);
		if(len >= 0)
		{
			break;
		}
		n = n * 2;
		if(n >= 1024000000)
		{
			//do nothing
			len = 0;
		}
	}
	if(len < 0)
	{
		len = 0;
	}
	if(len != n)
	{
		_Alloc(len, true);
	}*/
	((char *)_pBuf)[len] = '\0';
	if(oldLangCode != fromLangCode)
	{
		if(isUni)
		{
			ToWChar();
		}
		else
		{
			ToMByte(oldLangCode);
		}
	}
#if _DEBUG_
	//for show contents with mouse on while debug
	_pBufW = (wchar_t*)_pBuf;
	_pBufC = (char*)_pBuf;
#endif //#if _DEBUG_
	return *this;
}

Str &Str::Format(const wchar_t *strFormat, ...)
{
	if(strFormat == NULL)
	{
		_Free();
		return *this;
	}
	va_list argList;
	va_start(argList, strFormat);
	FormatV(strFormat, argList);
	va_end(argList);
	return *this;
}

Str &Str::FormatV(const wchar_t *strFormat, va_list argList)
{
	if(strFormat == NULL)
	{
		_Free();
		return *this;
	}
	LangCode::Type oldLangCode = _savedLang;
	bool isUni = IsUnicode();
	if(_savedLang != LangCode::UNICODE16)
	{
		_savedLang = LangCode::UNICODE16;
	}

	int nLen = _vscwprintf(strFormat, argList);
	_Alloc(nLen, false);
#ifdef __STDC_SECURE_LIB__
	int len = _vsnwprintf_s((wchar_t*)_pBuf, nLen + 1, nLen, strFormat, argList);
#else
	int len = _vsnwprintf((wchar_t*)_pBuf, nLen, strFormat, argList);
#endif
	/*
	int len = -1;
	int nFmt = Len(strFormat) + 1;
	int n = (int)_nRealCapacity < nFmt ? nFmt : _nRealCapacity;
	//no good way to detime the length of result
	while(true) //for out of loop,n<1024M
	{
		_Alloc(n, false);
		len = _vsnwprintf((wchar_t*)_pBuf, n, strFormat, argList);
		if(len >= 0)
		{
			break;
		}
		n = n * 2;
		if(n >= 1024000000)
		{
			//do nothing
			len = 0;
		}
	}
	if(len < 0)
	{
		len = 0;
	}
	if(len != n)
	{
		_Alloc(len, true);
	}
	*/
	((wchar_t *)_pBuf)[len] = L'\0';
	if(!isUni)
	{
		ToMByte(oldLangCode);
	}
#if _DEBUG_
	//for show contents with mouse on while debug
	_pBufW = (wchar_t*)_pBuf;
	_pBufC = (char*)_pBuf;
#endif //#if _DEBUG_
	return *this;
}

int Str::IndexOf(const char* str, bool ignoreCase, LangCode::Type fromLangCode) const
{
	return IndexOf(str, 0, -1, ignoreCase, fromLangCode);
}

int Str::IndexOf(const char* str, int startIndex, bool ignoreCase, LangCode::Type fromLangCode) const
{
	return IndexOf(str, startIndex, -1, ignoreCase, fromLangCode);
}

//as C#(IndexOf)
//The search begins at startIndex and continues to startIndex + count-1.
//The character at startIndex + count is not included in the search.
int Str::IndexOf(const char* str, int startIndex, int count, bool ignoreCase, LangCode::Type fromLangCode) const
{
	if(str == NULL && CC_BIT_HAS(_Flags, CC_STR_NULL))
	{
		return 0;
	}
	if(str == NULL || CC_BIT_HAS(_Flags, CC_STR_NULL))
	{
		return -1;
	}
	int slen = Len(str);
	if(slen <= 0)
	{
		return 0;
	}
	int buflen = GetLength();
	if(startIndex < 0 || startIndex > buflen || slen > buflen || startIndex + slen > buflen)
	{
		return -1;
	}
	if(_savedLang != fromLangCode)
	{
		return IndexOf(Str(_savedLang, str, fromLangCode), startIndex, count, ignoreCase);
	}
	//const char* temp = NSearch((char*)_pBuf + startIndex,
	//	(char*)_pBuf + startIndex + count - slen, str, slen, ignoreCase);
	//return (temp == NULL) ? -1 : (int)(temp - (char*)_pBuf);
	return Search((char*)_pBuf, startIndex, count, str, slen, ignoreCase);
}

int Str::IndexOf(const wchar_t* str, bool ignoreCase) const
{
	return IndexOf(str, 0, -1, ignoreCase);
}

int Str::IndexOf(const wchar_t* str, int startIndex, bool ignoreCase) const
{
	return IndexOf(str, startIndex, -1, ignoreCase);
}

//as C#(IndexOf)
//The search begins at startIndex and continues to startIndex + count-1.
//The character at startIndex + count is not included in the search.
int Str::IndexOf(const wchar_t* str, int startIndex, int count, bool ignoreCase) const
{
	if(str == NULL && CC_BIT_HAS(_Flags, CC_STR_NULL))
	{
		return 0;
	}
	if(str == NULL || CC_BIT_HAS(_Flags, CC_STR_NULL))
	{
		return -1;
	}
	int slen = Len(str);
	if(slen <= 0)
	{
		return 0;
	}
	int buflen = GetLength();
	if(startIndex < 0 || startIndex > buflen || slen > buflen || startIndex + slen > buflen)
	{
		return -1;
	}
	if(_savedLang != LangCode::UNICODE16)
	{
		return IndexOf(Str(_savedLang, str), startIndex, count, ignoreCase);
	}
	//const wchar_t* temp = NSearch((wchar_t*)_pBuf + startIndex,
	//	(wchar_t*)_pBuf + startIndex + count - slen, str, slen, ignoreCase);
	//return (temp == NULL) ? -1 : (int)(temp - (wchar_t*)_pBuf);
	return Search((wchar_t*)_pBuf, startIndex, count, str, slen, ignoreCase);
}

int Str::IndexOf(const Str& str, bool ignoreCase) const
{
	return IndexOf(str, 0, -1, ignoreCase);
}

int Str::IndexOf(const Str& str, int startIndex, bool ignoreCase) const
{
	if(str._savedLang == LangCode::UNICODE16)
	{
		return IndexOf((wchar_t*)str.GetBuf(), startIndex, -1, ignoreCase);
	}
	else
	{
		return IndexOf((char*)str.GetBuf(), startIndex, -1, ignoreCase, str.GetLangCode());
	}
}

int Str::IndexOf(const Str& str, int startIndex, int count, bool ignoreCase) const
{
	if(str._savedLang == LangCode::UNICODE16)
	{
		return IndexOf((wchar_t*)str.GetBuf(), startIndex, count, ignoreCase);
	}
	else
	{
		return IndexOf((char*)str.GetBuf(), startIndex, count, ignoreCase, str.GetLangCode());
	}
}

Str& Str::Insert(int insertAt, const char* str, LangCode::Type fromLangCode)
{
	CC_ASSERT(fromLangCode != LangCode::UNICODE16);

	int strLen = Len(str);
	_StrIns(str, fromLangCode, strLen, insertAt, 0, strLen);
	return *this;
}

Str& Str::Insert(int insertAt, const wchar_t* str)
{
	int strLen = Len(str);
	_StrIns(str, strLen, insertAt, 0, strLen);
	return *this;
}

Str& Str::Insert(int insertAt, const Str& str)
{
	int strLen = str.GetLength();
	if(str._savedLang == LangCode::UNICODE16)
	{
		_StrIns((wchar_t*)str.GetBuf(), strLen, insertAt, 0, strLen);
	}
	else
	{
		_StrIns((char*)str.GetBuf(), str.GetLangCode(), strLen, insertAt, 0, strLen);
	}
	return *this;
}

//this is static and return new string
Str Str::Join(const Str &separator, const Str str[], int startIndex, int count, bool noLastSeparator)
{
	if(startIndex < 0 || count < 0)
	{
#if defined(CC_EXCEPTION_H_)
		throw ArgumentException(_T("error for startIndex < 0 || count < 0."));
#else
		throw _T("error for startIndex < 0 || count < 0.");
#endif

	}
	Str s;
	int nEnd = startIndex + count - 1;
	for(int i = startIndex; i <= nEnd; i++)
	{
		s.Append(str[i]);
		if(!noLastSeparator || i < nEnd)
		{
			s.Append(separator);
		}
	}
	return s;
}

/*
LastIndexOf()
search from hind,sample from c#&java:
if cs = _T("aaaa") then cs.LastIndexOf(_T("aaa")) = 1 but not 0

but while Replace:
String s ="aaa";
s.LastIndexOf("aa") ==> 1
s.Replace("aa", "xyz") ==> xyza
*/
int Str::LastIndexOf(const char* str, bool ignoreCase, LangCode::Type fromLangCode) const
{
	int buflen = GetLength();
	return LastIndexOf(str, buflen, -1, ignoreCase, fromLangCode);
}

int Str::LastIndexOf(const char* str, int startIndex, bool ignoreCase, LangCode::Type fromLangCode) const
{
	//int buflen = GetLength();
	return LastIndexOf(str, startIndex, -1, ignoreCase, fromLangCode);
}

//while c#,"a".LastIndexOf("") ==> 0
//The search begins at the startIndex(from begin) character position of this instance
//and proceeds backwards towards the beginning until either value is found
//or count character positions have been examined.
//count -1 for all string(startIndex to begin)
int Str::LastIndexOf(const char* str, int startIndex, int count, bool ignoreCase, LangCode::Type fromLangCode) const
{
	if(str == NULL && CC_BIT_HAS(_Flags, CC_STR_NULL))
	{
		return 0;
	}
	if(str == NULL || CC_BIT_HAS(_Flags, CC_STR_NULL))
	{
		return -1;
	}
	int strlen = Len(str);
	int buflen = GetLength();
	if(strlen <= 0)
	{
		//like c#,set it to GetLength() - 1?
		if(buflen > 0)
		{
			return buflen - 1;
		}
		return 0;
	}
	if(startIndex <= 0 || strlen > startIndex || (count >= 0 && count < strlen))
	{
		return -1;
	}
	if(_savedLang != fromLangCode)
	{
		return LastIndexOf(Str(_savedLang, str, fromLangCode), startIndex, count, ignoreCase);
	}
	return SearchLast((char*)_pBuf, startIndex, count, str, strlen, ignoreCase);
}

int Str::LastIndexOf(const wchar_t* str, bool ignoreCase) const
{
	int buflen = GetLength();
	return LastIndexOf(str, buflen, -1, ignoreCase);
}

int Str::LastIndexOf(const wchar_t* str, int startIndex, bool ignoreCase) const
{
	//int buflen = GetLength();
	return LastIndexOf(str, startIndex, -1, ignoreCase);
}

//while c#,"a".LastIndexOf("") ==> 0
//The search begins at the startIndex(from begin) character position of this instance
//and proceeds backwards towards the beginning until either value is found
//or count character positions have been examined.
int Str::LastIndexOf(const wchar_t* str, int startIndex, int count, bool ignoreCase) const
{
	if(str == NULL && CC_BIT_HAS(_Flags, CC_STR_NULL))
	{
		return 0;
	}
	if(str == NULL || CC_BIT_HAS(_Flags, CC_STR_NULL))
	{
		return -1;
	}
	int strlen = Len(str);
	int buflen = GetLength();
	if(strlen <= 0)
	{
		if(buflen > 0)
		{
			return buflen - 1;
		}
		return 0;
	}
	if(startIndex <= 0 || strlen > startIndex || (count >= 0 && count < strlen))
	{
		return -1;
	}
	if(_savedLang != LangCode::UNICODE16)
	{
		return LastIndexOf(Str(_savedLang, str), startIndex, count, ignoreCase);
	}
	return SearchLast((wchar_t*)_pBuf, startIndex, count, str, strlen, ignoreCase);
}

int Str::LastIndexOf(const Str& str, bool ignoreCase) const
{
	int buflen = GetLength();
	return LastIndexOf(str, buflen, -1, ignoreCase);
}

int Str::LastIndexOf(const Str& str, int startIndex, bool ignoreCase) const
{
	//int buflen = GetLength();
	if(str._savedLang == LangCode::UNICODE16)
	{
		return LastIndexOf((wchar_t*)str.GetBuf(), startIndex, -1, ignoreCase);
	}
	else
	{
		return LastIndexOf((char*)str.GetBuf(), startIndex, -1, ignoreCase, str.GetLangCode());
	}
}

int Str::LastIndexOf(const Str& str, int startIndex, int count, bool ignoreCase) const
{
	if(str._savedLang == LangCode::UNICODE16)
	{
		return LastIndexOf((wchar_t*)str.GetBuf(), startIndex, count, ignoreCase);
	}
	else
	{
		return LastIndexOf((char*)str.GetBuf(), startIndex, count, ignoreCase, str.GetLangCode());
	}
}

Str &Str::Pad(int totalWidth, const char paddingChar, LangCode::Type fromLangCode)
{
	int nLen = GetLength();
	int nAdd = totalWidth - GetLength();
	if(nAdd > 0)
	{
		int nLeft = (int)(nAdd/2);
		PadLeft(nLeft + nLen, paddingChar, fromLangCode);
		PadRight(totalWidth, paddingChar, fromLangCode);
	}
	return *this;
}

Str &Str::Pad(int totalWidth, const wchar_t paddingChar)
{
	int nLen = GetLength();
	int nAdd = totalWidth - GetLength();
	if(nAdd > 0)
	{
		int nLeft = (int)(nAdd/2);
		PadLeft(nLeft + nLen, paddingChar);
		PadRight(totalWidth, paddingChar);
	}
	return *this;
}

Str &Str::PadLeft(int totalWidth, const char paddingChar, LangCode::Type fromLangCode)
{
	CC_ASSERT(fromLangCode != LangCode::UNICODE16);

	int nLen = GetLength();
	int nAdd = totalWidth - nLen;
	if(nAdd > 0 && paddingChar != '\0')
	{
		char *buf = (char *)malloc((nAdd + 1) * sizeof(char));
		for(int i = 0; i < nAdd; i++)
		{
			buf[i] = paddingChar;
		}
		buf[nAdd] = '\0';
		_StrIns(buf, fromLangCode, nAdd, 0, 0, nAdd);
		free(buf);
	}
	return *this;
}

Str &Str::PadLeft(int totalWidth, const wchar_t paddingChar)
{
	int nLen = GetLength();
	int nAdd = totalWidth - nLen;
	if(nAdd > 0 && paddingChar != '\0')
	{
		wchar_t *buf = (wchar_t *)malloc((nAdd + 1) * sizeof(wchar_t));
		for(int i = 0; i < nAdd; i++)
		{
			buf[i] = paddingChar;
		}
		buf[nAdd] = L'\0';
		_StrIns(buf, nAdd, 0, 0, nAdd);
		free(buf);
	}
	return *this;
}

Str &Str::PadRight(int totalWidth, const char paddingChar, LangCode::Type fromLangCode)
{
	CC_ASSERT(fromLangCode != LangCode::UNICODE16);

	int nLen = GetLength();
	int nAdd = totalWidth - nLen;
	if(nAdd > 0 && paddingChar != '\0')
	{
		char *buf = (char *)malloc((nAdd + 1) * sizeof(char));
		for(int i = 0; i < nAdd; i++)
		{
			buf[i] = paddingChar;
		}
		buf[nAdd] = '\0';
		_StrIns(buf, fromLangCode, nAdd, nLen, 0, nAdd);
		free(buf);
	}
	return *this;
}

Str &Str::PadRight(int totalWidth, const wchar_t paddingChar)
{
	int nLen = GetLength();
	int nAdd = totalWidth - nLen;
	if(nAdd > 0 && paddingChar != '\0')
	{
		wchar_t *buf = (wchar_t *)malloc((nAdd + 1) * sizeof(wchar_t));
		for(int i = 0; i < nAdd; i++)
		{
			buf[i] = paddingChar;
		}
		buf[nAdd] = L'\0';
		_StrIns(buf, nAdd, nLen, 0, nAdd);
		free(buf);
	}
	return *this;
}

Str& Str::Remove(int beginIndex)
{
	int ilenold = GetLength();
	if(ilenold <= 0)
	{
		return *this;
	}
	if(beginIndex >= 0 && beginIndex < ilenold)
	{
		_Alloc(beginIndex, true);
	}
	return *this;
}

Str& Str::Remove(int beginIndex, int endIndex)
{
	int ilenold = GetLength();
	if(ilenold <= 0)
	{
		return *this;
	}
	if(endIndex > ilenold)
	{
		endIndex = ilenold;
	}
	if(beginIndex >= 0 && beginIndex < endIndex)
	{
		if(endIndex < ilenold)
		{
			if(_savedLang == LangCode::UNICODE16)
			{
				Move((wchar_t*)_pBuf + beginIndex, (wchar_t*)_pBuf + endIndex, ilenold - endIndex);
			}
			else
			{
				Move((char*)_pBuf + beginIndex, (char*)_pBuf + endIndex, ilenold - endIndex);
			}
		}
		_Alloc(ilenold - (endIndex - beginIndex), true);
	}
	return *this;
}

Str& Str::Replace(const char *oldStr, const char *newStr, bool ignoreCase, LangCode::Type fromLangCode)
{
	return Replace(oldStr, newStr, 0, ignoreCase, fromLangCode);
}

/*************************************************
  Function: Replace
  Description: replace oldString to newString.
  Input:
    const char *oldStr: old string for replace.
    const char *newStr: new string for replace to.
    int fromIndex: will do replace from index of orignal string.
    bool ignoreCase: if is ignore upper or lower case.
    LangCode::Type fromLangCode: the lang code of oldStr and newStr.
  Output:
    (none)
  Return: cc::Str
  Others:
    if buf = "aaa" while Replace "aa" to "xyz",then
    if search from head result is:xyza, but will be "axyz" if from behind.
    here will search from head.
    for performances,I had rewritten this function for 10times!
*************************************************/
Str& Str::Replace(const char *oldStr, const char *newStr, int fromIndex, bool ignoreCase, LangCode::Type fromLangCode)
{
	//if buf is null,then do nothing.
	// || newStr == NULL 20101118
	if(oldStr == NULL || CC_BIT_HAS(_Flags, CC_STR_NULL))
	{
		return *this;
	}
	if(newStr == NULL)
	{
		newStr = "";
	}
	//must turn to same lang,then do replace.
	if(_savedLang != fromLangCode)
	{
		return Replace(Str(_savedLang, oldStr, fromLangCode), Str(_savedLang, newStr, fromLangCode), fromIndex, ignoreCase);
	}
	int nold = Len(oldStr);
	int nnew = Len(newStr);
	int buflen = GetLength();
	if(fromIndex < 0)
	{
		fromIndex = 0;
	}
	if(nold == 0 || fromIndex > buflen - nold)
	{
		return *this;
	}
	int nPos = 0;
	int ncnt = 0;
	char *p = (char*)_pBuf + fromIndex;
	char *oldp = NULL;
	//if replace big size to small size, it is simple.
	//but while turn small size to big size, will need cancute the result size.
	if(nold >= nnew)
	{
		while((nPos = Search(p, 0, -1, oldStr, nold, ignoreCase)) >= 0)
		{
			p += nPos;
			if(nnew > 0)
			{
//#ifdef __STDC_SECURE_LIB__
//				//strncpy_s(p, nnew + 1, newStr, nnew);
//				memmove_s(p, nnew + 1, newStr, nnew);
//#else
//				//strncpy(p, newStr, nnew);
//				memmove(p, newStr, nnew);
//#endif
				Move(p, newStr, nnew);
			}
			ncnt++;
			//if replace big size to small size,should move next string ahead.
			if(ncnt > 1 && nold > nnew)
			{
				Move(oldp - (nold - nnew) * (ncnt - 1), oldp, (int)(p - oldp + nnew));
			}
			p += nold;
			oldp = p;
		}
		if(ncnt > 0 && nold > nnew)
		{
			//if replace big size to small size,should move next string ahead.
			if(oldp < (char*)_pBuf + buflen)
			{
				Move(oldp - (nold - nnew) * ncnt, oldp, (int)((char*)_pBuf + buflen - oldp));
			}
			_Alloc((int)(buflen - (nold - nnew) * ncnt), true);
		}
	}
	else
	{
		nPos = Search(p, 0, -1, oldStr, nold, ignoreCase);
		if(nPos >= 0)
		{
			//first loop,get Replace count,and set where need Replace
			ncnt++;
			char *startp = (char*)_pBuf + nPos;
			p += nPos;
			*p = '\0';
			p += nold;
			oldp = p;
			while((nPos = Search(p, 0, -1, oldStr, nold, ignoreCase)) >= 0)
			{
				ncnt++;
				p += nPos;
				*p = '\0';
				p += nold;
				oldp = p;
			}

			//when _Alloc then lost point,so need remember the index
			int nend = (int)(oldp - (char*)_pBuf);
			if(buflen + (nnew - nold) * ncnt > (int)_nRealCapacity)
			{
				//when need resize,so create a new buf and second loop to copy and Replace
				char *oldbuf = (char*)_pBuf;
				_pBuf = NULL;
				// when _pBuf = NULL,then only create the give size,so(* _nIncrement)
				_Alloc((int)(buflen + (nnew - nold) * ncnt * _nIncrement), false);
				//then resize buf
				_Alloc(buflen + (nnew - nold) * ncnt, false);

				char *des = (char*)_pBuf;
				p = oldbuf;
				oldp = oldbuf + buflen;
				while(p < oldp)
				{
					if(*p != '\0')
					{
						*des = *p;
						p++;
						des++;
					}
					else
					{
//#ifdef __STDC_SECURE_LIB__
//						//strncpy_s(des, nnew + 1, newStr, nnew);
//						memmove_s(des, nnew + 1, newStr, nnew);
//#else
//						//strncpy(des, newStr, nnew);
//						memmove(des, newStr, nnew);
//#endif
						Move(des, newStr, nnew);
						p += nold;
						des += nnew;
					}
				}
				free(oldbuf);
			}
			else
			{
				//when need not resize,will use current buf,secend loop to move and Replace
				int buflen2 = buflen + (nnew - nold) * ncnt;
				_Alloc(buflen2, true);

				char *des = (char*)_pBuf + buflen2;
				int ncnt2 = ncnt;
				oldp = (char*)_pBuf + buflen - nold;
				p = (char*)_pBuf + nend - nold + 1;
				while(p > startp)
				{
					p--;
					if(*p == '\0')
					{
						ncnt2--;
						if(p < oldp)
						{
							Move(p + (des - oldp), p + nold, (int)(oldp - p));
						}
						des = p + (nnew - nold) * ncnt2;
						oldp = p - nold;
//#ifdef __STDC_SECURE_LIB__
//						//strncpy_s(des, nnew + 1, newStr, nnew);
//						memmove_s(des, nnew + 1, newStr, nnew);
//#else
//						//strncpy(des, newStr, nnew);
//						memmove(des, newStr, nnew);
//#endif
						Move(des, newStr, nnew);
					}
				}
			}
		}
	}
	return *this;
}

Str& Str::Replace(const wchar_t *oldStr, const wchar_t *newStr, bool ignoreCase)
{
	return Replace(oldStr, newStr, 0, ignoreCase);
}

Str& Str::Replace(const wchar_t *oldStr, const wchar_t *newStr, int fromIndex, bool ignoreCase)
{
	// || newStr == NULL 20101118
	if(oldStr == NULL || CC_BIT_HAS(_Flags, CC_STR_NULL))
	{
		return *this;
	}
	if(newStr == NULL)
	{
		newStr = L"";
	}
	if(_savedLang != LangCode::UNICODE16)
	{
		return Replace(Str(_savedLang, oldStr), Str(_savedLang, newStr), fromIndex, ignoreCase, _savedLang);
	}
	int nold = Len(oldStr);
	int nnew = Len(newStr);
	int buflen = GetLength();
	if(fromIndex < 0)
	{
		fromIndex = 0;
	}
	if(nold == 0 || fromIndex > buflen - nold)
	{
		return *this;
	}
	int nPos = 0;
	int ncnt = 0;
	wchar_t *p = (wchar_t*)_pBuf + fromIndex;
	wchar_t *oldp = NULL;
	if(nold >= nnew)
	{
		while((nPos = Search(p, 0, -1, oldStr, nold, ignoreCase)) >= 0)
		{
			p += nPos;
			if(nnew > 0)
			{
//#ifdef __STDC_SECURE_LIB__
//				//wcsncpy_s(p, nnew + 1, newStr, nnew);
//				memmove_s(p, (nnew + 1) * sizeof(wchar_t), newStr, nnew * sizeof(wchar_t));
//#else
//				//wcsncpy(p, newStr, nnew);
//				memmove(p, newStr, nnew * sizeof(wchar_t))
//#endif
				Move(p, newStr, nnew);
			}
			ncnt++;
			if(ncnt > 1 && nold > nnew)
			{
				Move(oldp - (nold - nnew) * (ncnt - 1), oldp, (int)(p - oldp + nnew));
			}
			p += nold;
			oldp = p;
		}
		if(ncnt > 0 && nold > nnew)
		{
			if(oldp < (wchar_t*)_pBuf + buflen)
			{
				Move(oldp - (nold - nnew) * ncnt, oldp, (int)((wchar_t*)_pBuf + buflen - oldp));
			}
			_Alloc(buflen - (nold - nnew) * ncnt, true);
		}
	}
	else
	{
		nPos = Search(p, 0, -1, oldStr, nold, ignoreCase);
		if(nPos >= 0)
		{
			//first loop,get Replace count,and set where need Replace
			ncnt++;
			wchar_t *startp = (wchar_t*)_pBuf + nPos;
			p += nPos;
			*p = L'\0';
			p += nold;
			oldp = p;
			while((nPos = Search(p, 0, -1, oldStr, nold, ignoreCase)) >= 0)
			{
				ncnt++;
				p += nPos;
				*p = L'\0';
				p += nold;
				oldp = p;
			}

			//when _Alloc then lost point,so need remenber the index
			int nend = (int)(oldp - (wchar_t*)_pBuf);
			if(buflen + (nnew - nold) * ncnt > (int)_nRealCapacity)
			{
				//when need resize,secend loop to copy and Replace
				wchar_t *oldbuf = (wchar_t*)_pBuf;
				_pBuf = NULL;
				// when _pBuf = NULL,then only create the give size,so(* _nIncrement)
				_Alloc((int)(buflen + (nnew - nold) * ncnt * _nIncrement), false);
				_Alloc(buflen + (nnew - nold) * ncnt, false);

				wchar_t *des = (wchar_t*)_pBuf;
				p = oldbuf;
				oldp = oldbuf + buflen;
				while(p < oldp)
				{
					if(*p != '\0')
					{
						*des = *p;
						p++;
						des++;
					}
					else
					{
//#ifdef __STDC_SECURE_LIB__
//						//wcsncpy_s(des, nnew + 1, newStr, nnew);
//						memmove_s(des, (nnew + 1) * sizeof(wchar_t), newStr, nnew * sizeof(wchar_t));
//#else
//						//wcsncpy(des, newStr, nnew);
//						memmove(des, newStr, nnew * sizeof(wchar_t))
//#endif
						Move(des, newStr, nnew);
						p += nold;
						des += nnew;
					}
				}
				free(oldbuf);
			}
			else
			{
				//when need not resize,secend loop to move and Replace
				int buflen2 = buflen + (nnew - nold) * ncnt;
				_Alloc(buflen2, true);

				wchar_t *des = (wchar_t*)_pBuf + buflen2;
				int ncnt2 = ncnt;
				oldp = (wchar_t*)_pBuf + buflen - nold;
				p = (wchar_t*)_pBuf + nend - nold + 1;
				while(p > startp)
				{
					p--;
					if(*p == '\0')
					{
						ncnt2--;
						if(p < oldp)
						{
							Move(p + (des - oldp), p + nold, (int)(oldp - p));
						}
						des = p + (nnew - nold) * ncnt2;
						oldp = p - nold;
//#ifdef __STDC_SECURE_LIB__
//						//wcsncpy_s(des, nnew + 1, newStr, nnew);
//						memmove_s(des, (nnew + 1) * sizeof(wchar_t), newStr, nnew * sizeof(wchar_t));
//#else
//						//wcsncpy(des, newStr, nnew);
//						memmove(des, newStr, nnew * sizeof(wchar_t))
//#endif
						Move(des, newStr, nnew);
					}
				}
			}
		}
	}
	return *this;
}

Str& Str::Replace(const Str &oldStr, const Str &newStr, bool ignoreCase)
{
	return Replace(oldStr, newStr, 0, ignoreCase);
}

Str& Str::Replace(const Str &oldStr, const Str &newStr, int fromIndex, bool ignoreCase)
{
	if(_savedLang != oldStr.GetLangCode() || _savedLang != newStr.GetLangCode())
	{
		return Replace(Str(_savedLang, oldStr), Str(_savedLang, newStr), fromIndex, ignoreCase);
	}
	if(_savedLang == LangCode::UNICODE16)
	{
		return Replace((wchar_t*)oldStr.GetBuf(), (wchar_t*)newStr.GetBuf(), fromIndex, ignoreCase);
	}
	else
	{
		return Replace((char*)oldStr.GetBuf(), (char*)newStr.GetBuf(), fromIndex, ignoreCase, oldStr.GetLangCode());
	}
}

int Str::GetLength() const
{
	return _nCapacity;
}

/* java:"ababab".Split("ab") then return 0,
   here:
		NULL.Split(any) or any.Split(NULL) or "".Split(any) then return 0
		"a".Split("ab") then return {""}
		"ab".Split("ab") then return {""}
		"abab".Split("ab") then return {"", ""}
		"ababa".Split("ab") then return {"", "", "a"}
		"ababab".Split("ab") then return {"", "", ""}
		"abababx".Split("ab") then return {"", "", "", "x"}

   use like:
	Str *pcs = NULL;
	cs = _T("xx1@yy22@zz333,,");
	int Splitcnt = cs.Split(_T("@"), &pcs);
	_tprintf(_T("all pcs:\n"));
	for(int i = 0; i < Splitcnt; i++)
	{
		_tprintf(_T("(%2d,%s)"), i, (TCHAR*)(pcs[i].GetBuf()));
	}
	delete []pcs;
	pcs = NULL;
*/
int Str::Split(const char* Splitstr, Str **str, bool ignoreCase, LangCode::Type fromLangCode) const
{
	return Split(Splitstr, str, 0, ignoreCase, fromLangCode);
}

int Str::Split(const char* Splitstr, Str **str, int limit, bool ignoreCase, LangCode::Type fromLangCode) const
{
	return Split(Splitstr, str, limit, ignoreCase, fromLangCode, _savedLang);
}

//if Splitstr = ""?
int Str::Split(const char* Splitstr, Str **str, int limit, bool ignoreCase, LangCode::Type fromLangCode, LangCode::Type toLangCode) const
{
	CC_ASSERT(*str == NULL);
	int buflen = GetLength();
	int slen = Len(Splitstr);
	if(Splitstr == NULL || buflen <= 0 || slen <= 0)
	{
		return 0;
	}
	if(_savedLang != fromLangCode)
	{
		return Split(Str(_savedLang, Splitstr, fromLangCode), str, limit, ignoreCase, toLangCode);
	}

	int nPos = Search((char*)_pBuf, 0, -1, Splitstr, slen, ignoreCase);
	if(nPos >= 0)
	{
		int ncnt = 1;
		char *pstart = (char*)_pBuf + nPos;
		char *oldp = pstart + slen;
		while((limit <= 0 || ncnt < limit) && (nPos = Search(oldp, 0, -1, Splitstr, slen, ignoreCase)) >= 0)
		{
			ncnt++;
			oldp += nPos + slen;
		}

		//if have char after Splitstr,then Split it.
		if(oldp < (char*)_pBuf + buflen)
		{
			ncnt++;
		}
		//also can use like below,but not free mem auto!
		//*str = (Str*)malloc(100 * sizeof(Str));
		//memset(*str, '\0', (100) * sizeof(Str));
		*str = new Str[ncnt];
		if(oldp < (char*)_pBuf + buflen)
		{
			(*str + ncnt - 1)->_savedLang = toLangCode;
			(*str + ncnt - 1)->ValueOf((char*)_pBuf, (int)(oldp - (char*)_pBuf), (int)((char*)_pBuf + buflen - oldp), _savedLang);
		}

		(*str + 0)->_savedLang = toLangCode;
		(*str + 0)->ValueOf((char*)_pBuf, 0, (int)(pstart - (char*)_pBuf), _savedLang);
		int ncnt2 = 1;
		oldp = pstart + slen;
		while((limit <= 0 || ncnt2 < limit) && (nPos = Search(oldp, 0, -1, Splitstr, slen, ignoreCase)) >= 0)
		{
			(*str + ncnt2)->_savedLang = toLangCode;
			(*str + ncnt2)->ValueOf((char*)_pBuf, (int)(oldp - (char*)_pBuf), nPos, _savedLang);
			oldp += nPos + slen;
			ncnt2++;
		}
		return ncnt;
	}
	else
	{
		*str = new Str[1];
		(*str + 0)->_savedLang = toLangCode;
		(*str + 0)->ValueOf((char*)_pBuf, 0, buflen);
		return 1;
	}
}

int Str::Split(const wchar_t* Splitstr, Str **str, bool ignoreCase) const
{
	return Split(Splitstr, str, 0, ignoreCase);
}

int Str::Split(const wchar_t* Splitstr, Str **str, int limit, bool ignoreCase) const
{
	return Split(Splitstr, str, limit, ignoreCase, _savedLang);
}

int Str::Split(const wchar_t* Splitstr, Str **str, int limit, bool ignoreCase, LangCode::Type toLangCode) const
{
	int buflen = GetLength();
	int slen = Len(Splitstr);
	if(Splitstr == NULL || buflen <= 0 || slen <= 0)
	{
		return 0;
	}
	if(_savedLang != LangCode::UNICODE16)
	{
		return Split(Str(_savedLang, Splitstr), str, limit, ignoreCase, toLangCode);
	}

	int nPos = Search((wchar_t*)_pBuf, 0, -1, Splitstr, slen, ignoreCase);
	if(nPos >= 0)
	{
		int ncnt = 1;
		wchar_t *pstart = (wchar_t*)_pBuf + nPos;
		wchar_t *oldp = pstart + slen;
		while((limit <= 0 || ncnt < limit) && (nPos = Search(oldp, 0, -1, Splitstr, slen, ignoreCase)) >= 0)
		{
			ncnt++;
			oldp += nPos + slen;
		}

		//if have char after Splitstr,then Split it.
		if(oldp < (wchar_t*)_pBuf + buflen)
		{
			ncnt++;
		}
		// u can use like below,but not free mem auto!
		//*str = (Str*)malloc(100 * sizeof(Str));
		//memset(*str, '\0', (100) * sizeof(Str));
		*str = new Str[ncnt];
		if(oldp < (wchar_t*)_pBuf + buflen)
		{
			(*str + ncnt - 1)->_savedLang = toLangCode;
			(*str + ncnt - 1)->ValueOf((wchar_t*)_pBuf, (int)(oldp - (wchar_t*)_pBuf), (int)((wchar_t*)_pBuf + buflen - oldp));
		}

		(*str + 0)->_savedLang = toLangCode;
		(*str + 0)->ValueOf((wchar_t*)_pBuf, 0, (int)(pstart - (wchar_t*)_pBuf));
		int ncnt2 = 1;
		oldp = pstart + slen;
		while((limit <= 0 || ncnt2 < limit) && (nPos = Search(oldp, 0, -1, Splitstr, slen, ignoreCase)) >= 0)
		{
			(*str + ncnt2)->_savedLang = toLangCode;
			//(*str + loopi)->ValueOf((wchar_t*)_pBuf, (int)(oldp - (wchar_t*)_pBuf), nPos);
			(*str + ncnt2)->ValueOf(oldp, 0, nPos);
			oldp += nPos + slen;
			ncnt2++;
		}
		return ncnt;
	}
	else
	{
		*str = new Str[1];
		(*str + 0)->_savedLang = _savedLang;
		(*str + 0)->ValueOf((wchar_t*)_pBuf, 0, buflen);
		return 1;
	}
}

int Str::Split(const Str& Splitstr, Str **str, bool ignoreCase) const
{
	return Split(Splitstr, str, 0, ignoreCase);
}

int Str::Split(const Str& Splitstr, Str **str, int limit, bool ignoreCase) const
{
	return Split(Splitstr, str, limit, ignoreCase, _savedLang);
}

int Str::Split(const Str& Splitstr, Str **str, int limit, bool ignoreCase, LangCode::Type toLangCode) const
{
	if(Splitstr.IsUnicode())
	{
		return Split((wchar_t*)Splitstr.GetBuf(), str, limit, ignoreCase, toLangCode);
	}
	else
	{
		return Split((char*)Splitstr.GetBuf(), str, limit, ignoreCase, Splitstr.GetLangCode(), toLangCode);
	}
}

bool Str::StartsWith(const char *prefix, bool ignoreCase, LangCode::Type fromLangCode) const
{
	return StartsWith(prefix, 0, ignoreCase, fromLangCode);
}

bool Str::StartsWith(const char *prefix, int fromIndex, bool ignoreCase, LangCode::Type fromLangCode) const
{
	if(prefix == NULL && CC_BIT_HAS(_Flags, CC_STR_NULL))
	{
		return true;
	}
	if(prefix == NULL || CC_BIT_HAS(_Flags, CC_STR_NULL))
	{
		return false;
	}
	int nlen = Len(prefix);
	int buflen = GetLength();
	if(fromIndex < 0 || fromIndex > (buflen - nlen))
	{
		return false;
	}
	if(nlen <= 0)
	{
		return true;
	}
	if(_savedLang != fromLangCode)
	{
		return StartsWith(Str(_savedLang, prefix, fromLangCode), fromIndex, ignoreCase);
	}
	return (NCmp((char*)_pBuf + fromIndex, prefix, nlen, ignoreCase) == 0);
}

bool Str::StartsWith(const wchar_t *prefix, bool ignoreCase) const
{
	return StartsWith(prefix, 0, ignoreCase);
}

bool Str::StartsWith(const wchar_t *prefix, int fromIndex, bool ignoreCase) const
{
	if(prefix == NULL && CC_BIT_HAS(_Flags, CC_STR_NULL))
	{
		return true;
	}
	if(prefix == NULL || CC_BIT_HAS(_Flags, CC_STR_NULL))
	{
		return false;
	}
	int nlen = Len(prefix);
	int buflen = GetLength();
	if(fromIndex < 0 || fromIndex > (buflen - nlen))
	{
		return false;
	}
	if(nlen <= 0)
	{
		return true;
	}
	if(_savedLang != LangCode::UNICODE16)
	{
		return StartsWith(Str(_savedLang, prefix), fromIndex, ignoreCase);
	}
	return (NCmp((wchar_t*)_pBuf + fromIndex, prefix, nlen, ignoreCase) == 0);
}

bool Str::StartsWith(const Str& prefix, bool ignoreCase) const
{
	if(prefix.IsUnicode())
	{
		return StartsWith((wchar_t*)prefix.GetBuf(), 0, ignoreCase);
	}
	else
	{
		return StartsWith((char*)prefix.GetBuf(), 0, ignoreCase, prefix.GetLangCode());
	}
	//return StartsWith(prefix, 0, ignoreCase);
}

bool Str::StartsWith(const Str& prefix, int fromIndex, bool ignoreCase) const
{
	if(prefix.IsUnicode())
	{
		return StartsWith((wchar_t*)prefix.GetBuf(), fromIndex, ignoreCase);
	}
	else
	{
		return StartsWith((char*)prefix.GetBuf(), fromIndex, ignoreCase, prefix.GetLangCode());
	}
}

Str Str::Substring(int beginIndex) const
{
	return Str(*this, beginIndex, -1);
}

Str Str::Substring(int beginIndex, int endIndex) const
{
	return Str(*this, beginIndex, endIndex - beginIndex);
}

Str& Str::ToLower()
{
	if(!CC_BIT_HAS(_Flags, CC_STR_NULL))
	{
		if(_savedLang == LangCode::UNICODE16)
		{
#ifdef __STDC_SECURE_LIB__
			_wcslwr_s((wchar_t*)_pBuf, GetLength() + 1);
#else
			_pBuf = _wcslwr((wchar_t*)_pBuf);
#endif
		}
		else
		{
			#ifdef __BORLANDC__
			_pBuf = strlwr((char*)_pBuf);
			#else
#ifdef __STDC_SECURE_LIB__
			_strlwr_s((char*)_pBuf, GetLength() + 1);
#else
			_pBuf = _strlwr((char*)_pBuf);
#endif
			#endif /* __BORLANDC__ */
		}
		//mBuf = _tcslwr(mBuf);
	}
	return *this;
}

cc::Str Str::ToString() const
{
	return cc::Str(*this);
}

Str& Str::ToUpper()
{
	if(!CC_BIT_HAS(_Flags, CC_STR_NULL))
	{
		if(_savedLang == LangCode::UNICODE16)
		{
#ifdef __STDC_SECURE_LIB__
			_wcsupr_s((wchar_t*)_pBuf, GetLength() + 1);
#else
			_pBuf = _wcsupr((wchar_t*)_pBuf);
#endif
		}
		else
		{
			#ifdef __BORLANDC__
			_pBuf = strupr((char*)_pBuf);
			#else
#ifdef __STDC_SECURE_LIB__
			_strupr_s((char*)_pBuf, GetLength() + 1);
#else
			_pBuf = _strupr((char*)_pBuf);
#endif
			#endif /* __BORLANDC__ */
		}
		//mBuf = _tcsupr(mBuf);
	}
	return *this;
}

Str& Str::Trim()
{
	return (_savedLang == LangCode::UNICODE16) ? TrimRight(L" ").TrimLeft(L" ") : TrimRight(" ", _savedLang).TrimLeft(" ", _savedLang);
}

Str& Str::Trim(const char* str, LangCode::Type fromLangCode)
{
	if(_savedLang != fromLangCode)
	{
		return Trim(Str(_savedLang, str, fromLangCode));
	}
	return TrimRight(str).TrimLeft(str);
}

Str& Str::Trim(const wchar_t* str)
{
	if(_savedLang != LangCode::UNICODE16)
	{
		return Trim(Str(_savedLang, str));
	}
	return TrimRight(str).TrimLeft(str);
}

Str& Str::Trim(const Str &str)
{
	//if need change lang,only once
	if(_savedLang != GetLangCode())
	{
		return Trim(Str(_savedLang, str));
	}
	if(str._savedLang == LangCode::UNICODE16)
	{
		return TrimRight((wchar_t*)str.GetBuf()).TrimLeft((wchar_t*)str.GetBuf());
	}
	else
	{
		return TrimRight((char*)str.GetBuf(), str.GetLangCode()).TrimLeft((char*)str.GetBuf(), str.GetLangCode());
	}
}

Str& Str::TrimLeft()
{
	return (_savedLang == LangCode::UNICODE16) ? TrimLeft(L" ") : TrimLeft(" ", _savedLang);
}

Str& Str::TrimLeft(const char* str, LangCode::Type fromLangCode)
{
	if(_savedLang != fromLangCode)
	{
		return TrimLeft(Str(_savedLang, str, fromLangCode));
	}
	int ilen = GetLength();
	int slen = Len(str);
	if(ilen > 0 && slen > 0 && ilen >= slen)
	{
		char *p = (char*)_pBuf;
		while(NCmp(p, str, slen) == 0)
		{
			p += slen;
		}
		if(p > (char*)_pBuf)
		{
			if(ilen - (p - (char*)_pBuf) > 0)
			{
				Move((char*)_pBuf, p, (int)(ilen - (p - (char*)_pBuf)));
			}
			_Alloc((int)(ilen - (p - (char*)_pBuf)), true);
		}
	}
	return *this;
}

Str& Str::TrimLeft(const wchar_t* str)
{
	if(_savedLang != LangCode::UNICODE16)
	{
		return TrimLeft(Str(_savedLang, str));
	}
	int ilen = GetLength();
	int slen = Len(str);
	if(ilen > 0 && slen > 0 && ilen >= slen)
	{
		wchar_t *p = (wchar_t*)_pBuf;
		while(NCmp(p, str, slen) == 0)
		{
			p += slen;
		}
		if(p > (wchar_t*)_pBuf)
		{
			if(ilen - (p - (wchar_t*)_pBuf) > 0)
			{
				Move((wchar_t*)_pBuf, p, (int)(ilen - (p - (wchar_t*)_pBuf)));
			}
			_Alloc((int)(ilen - (p - (wchar_t*)_pBuf)), true);
		}
	}
	return *this;
}

Str& Str::TrimLeft(const Str &str)
{
	if(str._savedLang == LangCode::UNICODE16)
	{
		return TrimLeft((wchar_t*)str.GetBuf());
	}
	else
	{
		return TrimLeft((char*)str.GetBuf(), str.GetLangCode());
	}
}

Str& Str::TrimRight()
{
	return (_savedLang == LangCode::UNICODE16) ? TrimRight(L" ") : TrimRight(" ", _savedLang);
}

Str& Str::TrimRight(const char* str, LangCode::Type fromLangCode)
{
	if(_savedLang != fromLangCode)
	{
		return TrimRight(Str(_savedLang, str, fromLangCode));
	}
	int ilen = GetLength();
	int slen = Len(str);
	if(ilen > 0 && slen > 0 && ilen >= slen)
	{
		char *p = (char*)_pBuf + ilen - slen;
		while(NCmp(p, str, slen) == 0)
		{
			p[0] = '\0';
			p -= slen;
		}
		if(p < (char*)_pBuf + ilen - slen)
		{
			_Alloc((int)(p - (char*)_pBuf + slen), true);
		}
	}
	return *this;
}

Str& Str::TrimRight(const wchar_t* str)
{
	if(_savedLang != LangCode::UNICODE16)
	{
		return TrimRight(Str(_savedLang, str));
	}
	int ilen = GetLength();
	int slen = Len(str);
	if(ilen > 0 && slen > 0 && ilen >= slen)
	{
		wchar_t *p = (wchar_t*)_pBuf + ilen - slen;
		while(NCmp(p, str, slen) == 0)
		{
			p[0] = L'\0';
			p -= slen;
		}
		if(p < (wchar_t*)_pBuf + ilen - slen)
		{
			_Alloc((int)(p - (wchar_t*)_pBuf + slen), true);
		}
	}
	return *this;
}

Str& Str::TrimRight(const Str &str)
{
	if(str._savedLang == LangCode::UNICODE16)
	{
		return TrimRight((wchar_t*)str.GetBuf());
	}
	else
	{
		return TrimRight((char*)str.GetBuf(), str.GetLangCode());
	}
}

Str &Str::ValueOf(const char ch)
{
	Format("%c", ch);
	return *this;
}

Str &Str::ValueOf(const wchar_t ch)
{
	Format(L"%c", ch);
	return *this;
}

Str &Str::ValueOf(int num)
{
	return (_savedLang == LangCode::UNICODE16) ? Format(L"%d", num) : Format("%d", num);
}
//
//Str &Str::ValueOf(long num)
//{
//	return (_savedLang == LangCode::UNICODE16) ? Format(L"%d", num) : Format("%d", num);
//}
//
////for float is auto turn to double,and Digits is changed,so not use it!
//Str &Str::ValueOf(float num)
//{
//	if(_savedLang == LangCode::UNICODE16)
//	{
//		Format(L"%.10f", num);
//		TrimRight(L"0");
//		TrimRight(L".");
//	}
//	else
//	{
//		Format("%.10f", num);
//		TrimRight("0");
//		TrimRight(".");
//	}
//	return *this;
//}
//
////actually the Digits is only 18byte!!
////double nDouble = (double)0.12345678901234567;
//Str &Str::ValueOf(double num)
//{
//	if(_savedLang == LangCode::UNICODE16)
//	{
//		Format(L"%.20f", num);
//		TrimRight(L"0");
//		TrimRight(L".");
//	}
//	else
//	{
//		Format("%.20f", num);
//		TrimRight("0");
//		TrimRight(".");
//	}
//	return *this;
//}

Str &Str::ValueOf(const char* str, LangCode::Type fromLangCode)
{
	if(str == NULL)
	{
		_Free();
		return *this;
	}
	_StrCopy(str, fromLangCode, Len(str), 0, -1);
	return *this;
}

Str &Str::ValueOf(const char* str, int fromIndex, LangCode::Type fromLangCode)
{
	if(str == NULL)
	{
		_Free();
		return *this;
	}
	_StrCopy(str, fromLangCode, Len(str), fromIndex, -1);
	return *this;
}

Str &Str::ValueOf(const char* str, int fromIndex, int len, LangCode::Type fromLangCode)
{
	if(str == NULL)
	{
		_Free();
		return *this;
	}
	_StrCopy(str, fromLangCode, Len(str), fromIndex, len);
	return *this;
}

Str &Str::ValueOf(const wchar_t* str)
{
	if(str == NULL)
	{
		_Free();
		return *this;
	}
	_StrCopy(str, Len(str), 0, -1);
	return *this;
}

Str &Str::ValueOf(const wchar_t* str, int fromIndex)
{
	if(str == NULL)
	{
		_Free();
		return *this;
	}
	_StrCopy(str, Len(str), fromIndex, -1);
	return *this;
}

Str &Str::ValueOf(const wchar_t* str, int fromIndex, int len)
{
	if(str == NULL)
	{
		_Free();
		return *this;
	}
	_StrCopy(str, Len(str), fromIndex, len);
	return *this;
}

Str &Str::ValueOf(const Str& str)
{
	//_savedLang = str.GetLangCode();
	//if(CC_BIT_HAS(str._Flags, CC_STR_NULL))
	//{
	//	_Free();
	//	return *this;
	//}
	//if(str._savedLang == LangCode::UNICODE16)
	//{
	//	_StrCopy((wchar_t*)str.GetBuf(), str.GetLength(), 0, -1);
	//}
	//else
	//{
	//	_StrCopy((char*)str.GetBuf(), str.GetLangCode(), str.GetLength(), 0, -1);
	//}
	ValueOf(str, 0, str.GetLength());
	return *this;
}

Str &Str::ValueOf(const Str& str, int fromIndex)
{
	//_savedLang = str.GetLangCode();
	//if(CC_BIT_HAS(str._Flags, CC_STR_NULL))
	//{
	//	_Free();
	//	return *this;
	//}
	//if(str._savedLang == LangCode::UNICODE16)
	//{
	//	_StrCopy((wchar_t*)str.GetBuf(), str.GetLength(), fromIndex, -1);
	//}
	//else
	//{
	//	_StrCopy((char*)str.GetBuf(), str.GetLangCode(), str.GetLength(), fromIndex, -1);
	//}
	ValueOf(str, fromIndex, str.GetLength() - fromIndex);
	return *this;
}

Str &Str::ValueOf(const Str& str, int fromIndex, int len)
{
	//_savedLang = str.GetLangCode();
	if(CC_BIT_HAS(str._Flags, CC_STR_NULL))
	{
		_Free();
		return *this;
	}
	if(_savedLang == str.GetLangCode())
	{
		if(str._savedLang == LangCode::UNICODE16)
		{
			_StrCopy((wchar_t*)str.GetBuf(), str.GetLength(), fromIndex, len);
		}
		else
		{
			_StrCopy((char*)str.GetBuf(), str.GetLangCode(), str.GetLength(), fromIndex, len);
		}
	}
	else
	{
		ValueOf(cc::Str(_savedLang, str, fromIndex, len));
	}
	return *this;
}

//========================================
//OTHERS
const void *Str::GetBuf() const
{
	//if(_isNULL)
	if(CC_BIT_HAS(_Flags, CC_STR_NULL))
	{
		return (void *)NULL;
	}
	return _pBuf;
};

LangCode::Type Str::GetLangCode() const
{
	return _savedLang;
};

unsigned int Str::GetMinCapacity() const
{
	return _nMinCapacity;
};

unsigned int Str::GetRealCapacity() const
{
	return _nRealCapacity;
};

float Str::GetIncPercent() const
{
	return _nIncrement;
};

void Str::SetCapacity(unsigned int nMinCapacity, double incPercentMoreThan1)
{
	if(incPercentMoreThan1 >= 1)
	{
		_nIncrement = (float)incPercentMoreThan1;
	}
	if((int)nMinCapacity >= 0)
	{
		_nMinCapacity = nMinCapacity;
		if(nMinCapacity == 0 && _nCapacity == 0) {
			_Free();
		}
		else {
			_Alloc(_nCapacity, true);
		}
	}
};

void Str::CompactStringBuf()
{
	unsigned int nMinLen = _nMinCapacity;
	float nIncLen = _nIncrement;
	_nMinCapacity = 0;
	_nIncrement = 1;
	//if(_isNULL)
	if(_pBuf == NULL)
	{
		_Free();
	}
	else
	{
		if(_savedLang == LangCode::UNICODE16)
		{
			_nCapacity = Len((const wchar_t *)_pBuf);
		}
		else
		{
			_nCapacity = Len((const char *)_pBuf);
		}
		_Alloc(_nCapacity, true);
	}
	_nMinCapacity = nMinLen;
	_nIncrement = nIncLen;
};

//The strtol() function converts the string pointed to by nptr to a long int representation.
//This function recognizes (in order) an optional string of spaces, an optional sign, an
//optional base indicator (0 for octal, X or x for hexadecimal), and a string of digits.
//The first unrecognized character ends the string. A pointer to this unrecognized character
//is stored in the object addressed by endptr, if endptr is not NULL.
//If base is non-zero, its value determines the set of recognized digits and overrides the
//optional base indicator character. If base is zero, nptr is assumed to be base 10,
//unless an optional base indicator character is given.
int Str::ToInt(int base) const
{
	if(GetLength() < 1)
	{
		return (int)0;
	}
	//return (_savedLang == LangCode::UNICODE16) ? (int)_wtoi((wchar_t*)_pBuf) : (int)atoi((char*)_pBuf);
	return (_savedLang == LangCode::UNICODE16) ? (int)wcstol((wchar_t*)_pBuf, NULL, base) : (int)strtol((char*)_pBuf, NULL, base);
}

long Str::ToLong(int base) const
{
	if(GetLength() < 1)
	{
		return (long)0;
	}
	//return (_savedLang == LangCode::UNICODE16) ? (long)_wtol((wchar_t*)_pBuf) : (long)atol((char*)_pBuf);
	return (_savedLang == LangCode::UNICODE16) ? wcstol((wchar_t*)_pBuf, NULL, base) : strtol((char*)_pBuf, NULL, base);
}

float Str::ToFloat() const
{
	if(GetLength() < 1)
	{
		return (float)0;
	}
	return (_savedLang == LangCode::UNICODE16) ? (float)_wtof((wchar_t*)_pBuf) : (float)atof((char*)_pBuf);
}

double Str::ToDouble() const
{
	if(GetLength() < 1)
	{
		return (double)0;
	}
	return (_savedLang == LangCode::UNICODE16) ? (double)wcstod((wchar_t*)_pBuf, NULL) : (double)strtod((char*)_pBuf, NULL);
}

bool Str::ToMByte(LangCode::Type toLangCode)
{
	if(_savedLang == toLangCode)
	{
		return true;
	}
	int nLen = GetLength();
	if(nLen <= 0)
	{
		_savedLang = toLangCode;
		//_Alloc(0, false);
		return true;
	}

	void* _pBufOld = NULL;
	void* _pBufOld2 = NULL;
	wchar_t *cDes = NULL;
	int nbytes = 0;
	bool bRetu = false;
	bool isUni = (_savedLang == LangCode::UNICODE16);
	if(!isUni)
	{
		nbytes = MultiByteToWideChar((int)_savedLang, 0, (char*)_pBuf, -1, 0, 0);
		cDes = (wchar_t*)malloc((nbytes + 1) * sizeof(wchar_t));
		memset(cDes, L'\0', (nbytes + 1) * sizeof(wchar_t));
		nbytes = MultiByteToWideChar((int)_savedLang, 0, (char*)_pBuf, -1, cDes, nbytes);
		_pBufOld = cDes;
	}
	else
	{
		cDes = (wchar_t*)_pBuf;
		nbytes = GetLength();
	}
	//nbytes = WideCharToMultiByte((int)toLangCode, 0, cDes, -1, 0, 0, 0, 0);
	nbytes = WideCharToMultiByte((int)toLangCode, 0, cDes, nbytes, 0, 0, 0, 0);
	if(nbytes > 0)
	{
		if(isUni)
		{
			//now cDes is same to _pBuf,so set _pBuf = NULL to alloc new mem
			_pBufOld2 = _pBuf;
			_pBuf = NULL;
			_Free();
		}
		_savedLang = toLangCode;
		_Alloc(nbytes, false);
		WideCharToMultiByte((int)toLangCode, 0, cDes, -1, (char*)_pBuf, nbytes, 0, 0);
		bRetu = true;
	}
	if(_pBufOld != NULL)
	{
		free(_pBufOld);
	}
	if(_pBufOld2 != NULL)
	{
		free(_pBufOld2);
	}
	return bRetu;
};

bool Str::ToWChar()
{
	if(_savedLang == LangCode::UNICODE16)
	{
		return true;
	}
	int nLen = GetLength();
	if(nLen <= 0)
	{
		_savedLang = LangCode::UNICODE16;
		//_Alloc(0, false);
		return true;
	}
	char *cDes = (char*)_pBuf;
	//now cDes is same to _pBuf,so set _pBuf = NULL to alloc new mem
	_pBuf = NULL;
	_Free();
	LangCode::Type oldLangCode = _savedLang;
	_savedLang = LangCode::UNICODE16;
	_StrCopy(cDes, oldLangCode, nLen, 0, -1);
	//now need free cDes(Shu.KK 20080707)
	if(cDes != NULL)
	{
		free(cDes);
	}
	return true;
};

bool Str::ToTChar(LangCode::Type toLangCode)
{
	#if defined(UNICODE)
	return ToWChar();
	#else
	return ToMByte(toLangCode);
	#endif/* #if defined(UNICODE) */
};

Str& Str::ToReverse()
{
	if(!CC_BIT_HAS(_Flags, CC_STR_NULL))
	{
		if(_savedLang == LangCode::UNICODE16)
		{
			_pBuf = _wcsrev((wchar_t*)_pBuf);
		}
		else
		{
			#ifdef __BORLANDC__
			_pBuf = strrev((char*)_pBuf);
			#else
			_pBuf = _strrev((char*)_pBuf);
			#endif /* __BORLANDC__ */
		}
		//mBuf = _tcsrev(mBuf);
	}
	return *this;
}

Str Str::Left(int length) const
{
	if(length < 0)
	{
		length = 0;
	}
	return Str(*this, 0, length);
	//return Substring(0, length);
}

Str Str::Right(int length) const
{
	if(length < 0)
	{
		length = 0;
	}
	if(length > GetLength())
	{
		length = GetLength();
	}
	return Str(*this, GetLength() - length, length);
	//return Substring(GetLength() - length, GetLength());
}

Str Str::Mid(int beginIndex) const
{
	return Substring(beginIndex);
}

Str Str::Mid(int beginIndex, int len) const
{
	return Substring(beginIndex, beginIndex + len);
}

Str Str::SplitBy(const char *str, bool ignoreCase, LangCode::Type fromLangCode)
{
	if(GetLength() < 1 || str == NULL)
	{
		return Str(_savedLang);
	}
	if(_savedLang != fromLangCode)
	{
		return SplitBy(Str(_savedLang, str, fromLangCode), ignoreCase);
	}
	int slen = Len(str);
	//const char* temp = NSearch((char*)_pBuf, (char*)_pBuf + GetLength() - slen, str, slen, ignoreCase);
	//if(temp == NULL)
	int npos = Search((char*)_pBuf, 0, -1, str, slen, ignoreCase);
	if(npos < 0)
	{
		Str r = Str(_savedLang, (char*)_pBuf, _savedLang);
		_Free();
		return r;
	}
	else
	{
		//int npos = (int)(temp - (char*)_pBuf);
		Str r = Str(*this, 0, npos);
		Remove(0, npos + slen);
		return r;
	}
}

Str Str::SplitBy(const wchar_t *str, bool ignoreCase)
{
	if(GetLength() < 1 || str == NULL)
	{
		return Str(_savedLang);
	}
	if(_savedLang != LangCode::UNICODE16)
	{
		return SplitBy(Str(_savedLang, str), ignoreCase);
	}
	int slen = Len(str);
	//const wchar_t* temp = NSearch((wchar_t*)_pBuf, (wchar_t*)_pBuf + GetLength() - slen, str, slen, ignoreCase);
	//if(temp == NULL)
	int npos = Search((wchar_t*)_pBuf, 0, -1, str, slen, ignoreCase);
	if(npos < 0)
	{
		Str r = Str(_savedLang, (wchar_t*)_pBuf);
		_Free();
		return r;
	}
	else
	{
		//int npos = (int)(temp - (wchar_t*)_pBuf);
		Str r = Str(*this, 0, npos);
		Remove(0, npos + slen);
		return r;
	}
}

Str Str::SplitBy(const Str &str, bool ignoreCase)
{
	if(str._savedLang == LangCode::UNICODE16)
	{
		return SplitBy((wchar_t*)str.GetBuf(), ignoreCase);
	}
	else
	{
		return SplitBy((char*)str.GetBuf(), ignoreCase, str.GetLangCode());
	}
}

Str& Str::Append(const char str)
{
	return AppendFormat("%c", str);
}

Str& Str::Append(const wchar_t str)
{
	return AppendFormat(L"%c", str);
}

Str& Str::Append(int num)
{
	return AppendFormat("%d", num);
}

Str& Str::Append(const char* str, LangCode::Type fromLangCode)
{
	CC_ASSERT(fromLangCode != LangCode::UNICODE16);

	int nLen = Len(str);
	_StrIns(str, fromLangCode, nLen, GetLength(), 0, nLen);
	return *this;
}

Str& Str::Append(const char* str, int fromIndex, LangCode::Type fromLangCode)
{
	CC_ASSERT(fromLangCode != LangCode::UNICODE16);

	int nLen = Len(str);
	_StrIns(str, fromLangCode, nLen, GetLength(), fromIndex, nLen - fromIndex);
	return *this;
}

Str& Str::Append(const char* str, int fromIndex, int copyLen, LangCode::Type fromLangCode)
{
	CC_ASSERT(fromLangCode != LangCode::UNICODE16);

	int nLen = Len(str);
	_StrIns(str, fromLangCode, nLen, GetLength(), fromIndex, copyLen);
	return *this;
}

Str& Str::Append(const wchar_t* str)
{
	int nLen = Len(str);
	_StrIns(str, nLen, GetLength(), 0, nLen);
	return *this;
}

Str& Str::Append(const wchar_t* str, int fromIndex)
{
	int nLen = Len(str);
	_StrIns(str, nLen, GetLength(), fromIndex, nLen - fromIndex);
	return *this;
}

Str& Str::Append(const wchar_t* str, int fromIndex, int copyLen)
{
	int nLen = Len(str);
	_StrIns(str, nLen, GetLength(), fromIndex, copyLen);
	return *this;
}

Str& Str::Append(const Str& str)
{
	return Append(str, 0, str.GetLength());
}

Str& Str::Append(const Str& str, int fromIndex)
{
	return Append(str, fromIndex, str.GetLength() - fromIndex);
}

Str& Str::Append(const Str& str, int fromIndex, int copyLen)
{
	int nLen = str.GetLength();
	if(str._savedLang == LangCode::UNICODE16)
	{
		_StrIns((wchar_t*)str.GetBuf(), nLen, GetLength(), fromIndex, copyLen);
	}
	else
	{
		_StrIns((char*)str.GetBuf(), str.GetLangCode(), nLen, GetLength(), fromIndex, copyLen);
	}
	return *this;
}

Str &Str::AppendFormat(const char *strFormat, ...)
{
	va_list argList;
	va_start(argList, strFormat);
	this->AppendFormatV(LangCode::SYSTEM, strFormat, argList);
	va_end(argList);
	return *this;
};

Str &Str::AppendFormatV(const char *strFormat, va_list argList)
{
	this->AppendFormatV(LangCode::SYSTEM, strFormat, argList);
	return *this;
};

Str &Str::AppendFormat(LangCode::Type fromLangCode, const char *strFormat, ...)
{
	va_list argList;
	va_start(argList, strFormat);
	this->AppendFormatV(fromLangCode, strFormat, argList);
	va_end(argList);
	return *this;
}

Str &Str::AppendFormatV(LangCode::Type fromLangCode, const char *strFormat, va_list argList)
{
	Str buf = Str(fromLangCode);
	buf.FormatV(strFormat, argList);
	this->Append(buf);
	return *this;
}

Str &Str::AppendFormat(const wchar_t *strFormat, ...)
{
	va_list argList;
	va_start(argList, strFormat);
	this->AppendFormatV(strFormat, argList);
	va_end(argList);
	return *this;
}

Str &Str::AppendFormatV(const wchar_t *strFormat, va_list argList)
{
	Str buf = Str(LangCode::UNICODE16);
	buf.FormatV(strFormat, argList);
	this->Append(buf);
	return *this;
}

Str& Str::Repeat(const char* str, int nRepeat, LangCode::Type fromLangCode)
{
	if(_savedLang != fromLangCode)
	{
		return Repeat(Str(_savedLang, str, fromLangCode), nRepeat);
	}
	if(str == NULL)
	{
		_Free();
	}
	else if(nRepeat <= 0)
	{
		_Alloc(0, false);
	}
	else
	{
		int nLen = Len(str) * nRepeat;
		_Alloc(nLen, false);
		for(int i = 0; i < nRepeat; i++)
		{
#ifdef __STDC_SECURE_LIB__
			strcat_s((char*)_pBuf, nLen + 1, str);
#else
			strcat((char*)_pBuf, str);
#endif
		}
	}
	return *this;
}

Str& Str::Repeat(const wchar_t* str, int nRepeat)
{
	if(_savedLang != LangCode::UNICODE16)
	{
		return Repeat(Str(_savedLang, str), nRepeat);
	}
	if(str == NULL)
	{
		_Free();
	}
	else if(nRepeat <= 0)
	{
		_Alloc(0, false);
	}
	else
	{
		int nLen = Len(str) * nRepeat;
		_Alloc(nLen, false);
		for(int i = 0; i < nRepeat; i++)
		{
#ifdef __STDC_SECURE_LIB__
			wcscat_s((wchar_t*)_pBuf, nLen + 1, str);
#else
			wcscat((wchar_t*)_pBuf, str);
#endif
		}
	}
	return *this;
}

Str& Str::Repeat(const Str& str, int nRepeat)
{
	if(str._savedLang == LangCode::UNICODE16)
	{
		return Repeat((wchar_t*)str.GetBuf(), nRepeat);
	}
	else
	{
		return Repeat((char*)str.GetBuf(), nRepeat, str.GetLangCode());
	}
}

int Str::FindCount(const char* str, bool ignoreCase) const
{
	return FindCount(str, 0, ignoreCase, LangCode::SYSTEM);
}

int Str::FindCount(const char* str, int fromIndex, bool ignoreCase, LangCode::Type fromLangCode) const
{
	if(_savedLang != fromLangCode)
	{
		return FindCount(Str(_savedLang, str, fromLangCode), ignoreCase);
	}
	if(CC_BIT_HAS(_Flags, CC_STR_NULL) || str == NULL)
	{
		return 0;
	}
	int nlen = Len(str);
	int buflen = GetLength();
	int ncnt = 0;
	if(fromIndex < 0)
	{
		fromIndex = 0;
	}
	if(nlen == 0 || fromIndex > buflen - nlen)
	{
		return 0;
	}
	//char *p = (char*)_pBuf + fromIndex;
	//while((p = NSearch(p, (char*)_pBuf + buflen - nlen, str, nlen, ignoreCase)) != NULL)
	int p = fromIndex;
	while((p = Search((char*)_pBuf, p, -1, str, nlen, ignoreCase)) >= 0)
	{
		ncnt++;
		p += nlen;
	}
	return ncnt;
}

int Str::FindCount(const wchar_t* str, bool ignoreCase) const
{
	return FindCount(str, 0, ignoreCase);
}

int Str::FindCount(const wchar_t* str, int fromIndex, bool ignoreCase) const
{
	if(_savedLang != LangCode::UNICODE16)
	{
		return FindCount(Str(_savedLang, str), ignoreCase);
	}
	if(CC_BIT_HAS(_Flags, CC_STR_NULL) || str == NULL)
	{
		return 0;
	}
	int nlen = Len(str);
	int buflen = GetLength();
	int ncnt = 0;
	if(fromIndex < 0)
	{
		fromIndex = 0;
	}
	if(nlen == 0 || fromIndex > buflen - nlen)
	{
		return 0;
	}
	//wchar_t *p = (wchar_t*)_pBuf + fromIndex;
	//while((p = NSearch(p, (wchar_t*)_pBuf + buflen - nlen, str, nlen, ignoreCase)) != NULL)
	int p = fromIndex;
	while((p = Search((wchar_t*)_pBuf, p, -1, str, nlen, ignoreCase)) >= 0)
	{
		ncnt++;
		p += nlen;
	}
	return ncnt;
}

int Str::FindCount(const Str& str, bool ignoreCase) const
{
	return FindCount(str, 0, ignoreCase);
}

int Str::FindCount(const Str& str, int fromIndex, bool ignoreCase) const
{
	if(str._savedLang == LangCode::UNICODE16)
	{
		return FindCount((wchar_t*)str.GetBuf(), fromIndex, ignoreCase);
	}
	else
	{
		return FindCount((char*)str.GetBuf(), fromIndex, ignoreCase, str.GetLangCode());
	}
}

bool Str::IsNumber(int nIndex) const
{
	if(nIndex < 0 || nIndex >= (int)_nCapacity)
	{
		return false;
	}
	if(_savedLang == LangCode::UNICODE16)
	{
		return iswdigit(((wchar_t*)_pBuf)[nIndex]) != 0;
	}
	else
	{
		return isdigit(((char*)_pBuf)[nIndex]) != 0;
	}
}

bool Str::IsNumber() const
{
	if((int)_nCapacity <= 0)
	{
		return false;
	}
	int nIndex = 0;
	if(_savedLang == LangCode::UNICODE16)
	{
		while(iswdigit(((wchar_t*)_pBuf)[nIndex]) != 0 && nIndex < (int)_nCapacity)
		{
			nIndex++;
		}
	}
	else
	{
		while(isdigit(((char*)_pBuf)[nIndex]) != 0 && nIndex < (int)_nCapacity)
		{
			nIndex++;
		}
	}
	return (nIndex >= (int)_nCapacity);
}

bool Str::IsAlpha(int nIndex) const
{
	if(nIndex < 0 || nIndex >= (int)_nCapacity)
	{
		return false;
	}
	if(_savedLang == LangCode::UNICODE16)
	{
		return iswalpha(((wchar_t*)_pBuf)[nIndex]) != 0;
	}
	else
	{
		return isalpha(((char*)_pBuf)[nIndex]) != 0;
	}
}

bool Str::IsAlpha() const
{
	if((int)_nCapacity <= 0)
	{
		return false;
	}
	int nIndex = 0;
	if(_savedLang == LangCode::UNICODE16)
	{
		while(iswalpha(((wchar_t*)_pBuf)[nIndex]) != 0 && nIndex < (int)_nCapacity)
		{
			nIndex++;
		}
	}
	else
	{
		while(isalpha(((char*)_pBuf)[nIndex]) != 0 && nIndex < (int)_nCapacity)
		{
			nIndex++;
		}
	}
	return (nIndex >= (int)_nCapacity);
}

bool Str::IsUpper(int nIndex) const
{
	if(nIndex < 0 || nIndex >= (int)_nCapacity)
	{
		return false;
	}
	if(_savedLang == LangCode::UNICODE16)
	{
		return iswupper(((wchar_t*)_pBuf)[nIndex]) != 0;
	}
	else
	{
		return isupper(((char*)_pBuf)[nIndex]) != 0;
	}
}

bool Str::IsUpper() const
{
	if((int)_nCapacity <= 0)
	{
		return false;
	}
	int nIndex = 0;
	if(_savedLang == LangCode::UNICODE16)
	{
		while(iswupper(((wchar_t*)_pBuf)[nIndex]) != 0 && nIndex < (int)_nCapacity)
		{
			nIndex++;
		}
	}
	else
	{
		while(isupper(((char*)_pBuf)[nIndex]) != 0 && nIndex < (int)_nCapacity)
		{
			nIndex++;
		}
	}
	return (nIndex >= (int)_nCapacity);
}

bool Str::IsLower(int nIndex) const
{
	if(nIndex < 0 || nIndex >= (int)_nCapacity)
	{
		return false;
	}
	if(_savedLang == LangCode::UNICODE16)
	{
		return iswlower(((wchar_t*)_pBuf)[nIndex]) != 0;
	}
	else
	{
		return islower(((char*)_pBuf)[nIndex]) != 0;
	}
}

bool Str::IsLower() const
{
	if((int)_nCapacity <= 0)
	{
		return false;
	}
	int nIndex = 0;
	if(_savedLang == LangCode::UNICODE16)
	{
		while(iswlower(((wchar_t*)_pBuf)[nIndex]) != 0 && nIndex < (int)_nCapacity)
		{
			nIndex++;
		}
	}
	else
	{
		while(islower(((char*)_pBuf)[nIndex]) != 0 && nIndex < (int)_nCapacity)
		{
			nIndex++;
		}
	}
	return (nIndex >= (int)_nCapacity);
}

bool Str::IsBlankWithTrim() const
{
	if(GetLength() < 1)
	{
		return true;
	}
	//cannot use TRIM, otherwise will change the buffer!
	int nIndex = 0;
	if(_savedLang == LangCode::UNICODE16)
	{
		while(((wchar_t*)_pBuf)[nIndex] == L' ' && nIndex < (int)_nCapacity)
		{
			nIndex++;
		}
	}
	else
	{
		while(((char*)_pBuf)[nIndex] == ' ' && nIndex < (int)_nCapacity)
		{
			nIndex++;
		}
	}
	return (nIndex >= (int)_nCapacity);
};

bool Str::IsBlankWithoutTrim() const
{
	return (GetLength() < 1);
};

bool Str::IsNULL() const
{
	return (CC_BIT_HAS(_Flags, CC_STR_NULL));
};

bool Str::IsUnicode() const
{
	return (_savedLang == LangCode::UNICODE16);
};

bool Str::IsTChar() const
{
	#if defined(UNICODE)
	return (_savedLang == LangCode::UNICODE16);
	#else
	return (_savedLang != LangCode::UNICODE16);
	#endif/* #if defined(UNICODE) */
};

// Assignment Operations
Str& Str::operator=(const char* str)
{
	if(str == NULL)
	{
		_Free();
	}
	else
	{
		_StrCopy(str, LangCode::SYSTEM, Len(str), 0, -1);
	}
	return *this;
}

Str& Str::operator=(const wchar_t* str)
{
	if(str == NULL)
	{
		_Free();
	}
	else
	{
		_StrCopy(str, Len(str), 0, -1);
	}
	return *this;
}

Str& Str::operator=(const Str& str)
{
	//if is self
	if(_pBuf != NULL && _pBuf == str.GetBuf())
	{
		return *this;
	}
	_savedLang = str._savedLang;
	//if(str._isNULL)
	if(CC_BIT_HAS(str._Flags, CC_STR_NULL))
	{
		_Free();
	}
	else
	{
		//_Alloc(str._nCapacity, false);
		//memcpy(_pBuf, str._pBuf, str._nCapacity * ((str._savedLang == LangCode::UNICODE16) ? sizeof(wchar_t) : sizeof(char)));
		//_Flags = str._Flags;
		if(str._savedLang == LangCode::UNICODE16)
		{
			_StrCopy((wchar_t*)str.GetBuf(), str.GetLength(), 0, -1);
		}
		else
		{
			_StrCopy((char*)str.GetBuf(), str.GetLangCode(), str.GetLength(), 0, -1);
		}
	}
	return *this;
}

Str::operator char*()
{
	if(_savedLang == LangCode::UNICODE16)
	{
		ToMByte(LangCode::SYSTEM);
	}
	return (char*)_pBuf;
}

Str::operator wchar_t*()
{
	if(_savedLang != LangCode::UNICODE16)
	{
		ToWChar();
	}
	return (wchar_t*)_pBuf;
}

//please forget this,change with compiler.
int Str::operator[](int Index) const
{
	if(!CC_BIT_HAS(_Flags, CC_STR_NULL) && Index >= 0 && Index < GetLength())
	{
		return (_savedLang == LangCode::UNICODE16) ? ((wchar_t*)_pBuf)[Index] : ((char*)_pBuf)[Index];
	}
	else
	{
		return '\0';
	}
}

// Compare implementations
bool Str::operator==(const char* str) const
{
	if(_savedLang == LangCode::UNICODE16)
	{
		return (operator==(Str(_savedLang, str, LangCode::SYSTEM)));
	}
	return (Cmp((char*)_pBuf, str) == 0);
}

bool Str::operator==(const wchar_t* str) const
{
	if(_savedLang != LangCode::UNICODE16)
	{
		return (operator==(Str(_savedLang, str)));
	}
	return (Cmp((wchar_t*)_pBuf, str) == 0);
}

bool Str::operator==(const Str& str) const
{
	if(_savedLang == str.GetLangCode())
	{
		if(_savedLang == LangCode::UNICODE16)
		{
			return (Cmp((wchar_t*)_pBuf, (wchar_t*)str.GetBuf()) == 0);
		}
		else
		{
			return (Cmp((char*)_pBuf, (char*)str.GetBuf()) == 0);
		}
	}
	else
	{
		return (operator==(Str(_savedLang, str)));
	}
}

bool Str::operator!=(const char* str) const
{
	if(_savedLang == LangCode::UNICODE16)
	{
		return (operator!=(Str(_savedLang, str, LangCode::SYSTEM)));
	}
	return (Cmp((char*)_pBuf, str) != 0);
}

bool Str::operator!=(const wchar_t* str) const
{
	if(_savedLang != LangCode::UNICODE16)
	{
		return (operator!=(Str(_savedLang, str)));
	}
	return (Cmp((wchar_t*)_pBuf, str) != 0);
}

bool Str::operator!=(const Str& str) const
{
	if(_savedLang == str.GetLangCode())
	{
		if(_savedLang == LangCode::UNICODE16)
		{
			return (Cmp((wchar_t*)_pBuf, (wchar_t*)str.GetBuf()) != 0);
		}
		else
		{
			return (Cmp((char*)_pBuf, (char*)str.GetBuf()) != 0);
		}
	}
	else
	{
		return (operator!=(Str(_savedLang, str)));
	}
}

bool Str::operator>(const char* str) const
{
	if(_savedLang == LangCode::UNICODE16)
	{
		return (operator>(Str(_savedLang, str, LangCode::SYSTEM)));
	}
	return (Cmp((char*)_pBuf, str) > 0);
}

bool Str::operator>(const wchar_t* str) const
{
	if(_savedLang != LangCode::UNICODE16)
	{
		return (operator>(Str(_savedLang, str)));
	}
	return (Cmp((wchar_t*)_pBuf, str) > 0);
}

bool Str::operator>(const Str& str) const
{
	if(_savedLang == str.GetLangCode())
	{
		if(_savedLang == LangCode::UNICODE16)
		{
			return (Cmp((wchar_t*)_pBuf, (wchar_t*)str.GetBuf()) > 0);
		}
		else
		{
			return (Cmp((char*)_pBuf, (char*)str.GetBuf()) > 0);
		}
	}
	else
	{
		return (operator>(Str(_savedLang, str)));
	}
}

bool Str::operator>=(const char* str) const
{
	if(_savedLang == LangCode::UNICODE16)
	{
		return (operator>=(Str(_savedLang, str, LangCode::SYSTEM)));
	}
	return (Cmp((char*)_pBuf, str) >= 0);
}

bool Str::operator>=(const wchar_t* str) const
{
	if(_savedLang != LangCode::UNICODE16)
	{
		return (operator>=(Str(_savedLang, str)));
	}
	return (Cmp((wchar_t*)_pBuf, str) >= 0);
}

bool Str::operator>=(const Str& str) const
{
	if(_savedLang == str.GetLangCode())
	{
		if(_savedLang == LangCode::UNICODE16)
		{
			return (Cmp((wchar_t*)_pBuf, (wchar_t*)str.GetBuf()) >= 0);
		}
		else
		{
			return (Cmp((char*)_pBuf, (char*)str.GetBuf()) >= 0);
		}
	}
	else
	{
		return (operator>=(Str(_savedLang, str)));
	}
}

bool Str::operator<(const char* str) const
{
	if(_savedLang == LangCode::UNICODE16)
	{
		return (operator<(Str(_savedLang, str, LangCode::SYSTEM)));
	}
	return (Cmp((char*)_pBuf, str) < 0);
}

bool Str::operator<(const wchar_t* str) const
{
	if(_savedLang != LangCode::UNICODE16)
	{
		return (operator<(Str(_savedLang, str)));
	}
	return (Cmp((wchar_t*)_pBuf, str) < 0);
}

bool Str::operator<(const Str& str) const
{
	if(_savedLang == str.GetLangCode())
	{
		if(_savedLang == LangCode::UNICODE16)
		{
			return (Cmp((wchar_t*)_pBuf, (wchar_t*)str.GetBuf()) < 0);
		}
		else
		{
			return (Cmp((char*)_pBuf, (char*)str.GetBuf()) < 0);
		}
	}
	else
	{
		return (operator<(Str(_savedLang, str)));
	}
}

bool Str::operator<=(const char* str) const
{
	if(_savedLang == LangCode::UNICODE16)
	{
		return (operator<=(Str(_savedLang, str, LangCode::SYSTEM)));
	}
	return (Cmp((char*)_pBuf, str) <= 0);
}

bool Str::operator<=(const wchar_t* str) const
{
	if(_savedLang != LangCode::UNICODE16)
	{
		return (operator<=(Str(_savedLang, str)));
	}
	return (Cmp((wchar_t*)_pBuf, str) <= 0);
}

bool Str::operator<=(const Str& str) const
{
	if(_savedLang == str.GetLangCode())
	{
		if(_savedLang == LangCode::UNICODE16)
		{
			return (Cmp((wchar_t*)_pBuf, (wchar_t*)str.GetBuf()) <= 0);
		}
		else
		{
			return (Cmp((char*)_pBuf, (char*)str.GetBuf()) <= 0);
		}
	}
	else
	{
		return (operator<=(Str(_savedLang, str)));
	}
}

//Friend Functions
/*
bool operator==(const Str& str, const char* str2)
{
	return (str.CompareTo(str2) == 0);
}

bool operator==(const Str& str, const wchar_t* str2)
{
	return (str.CompareTo(str2) == 0);
}
*/

bool operator==(const char* str, const Str& str2)
{
	return (str2.CompareTo(str) == 0);
}

bool operator==(const wchar_t* str, const Str& str2)
{
	return (str2.CompareTo(str) == 0);
}

/*
bool operator==(const Str& str, const Str& str2)
{
	return (str.CompareTo(str2) == 0);
}

bool operator!=(const Str& str, const char* str2)
{
	return (str.CompareTo(str2) != 0);
}

bool operator!=(const Str& str, const wchar_t* str2)
{
	return (str.CompareTo(str2) != 0);
}
*/

bool operator!=(const char* str, const Str& str2)
{
	return (str2.CompareTo(str) != 0);
}

bool operator!=(const wchar_t* str, const Str& str2)
{
	return (str2.CompareTo(str) != 0);
}

/*
bool operator!=(const Str& str, const Str& str2)
{
	return (str.CompareTo(str2) != 0);
}
*/

bool operator>(const char* str, const Str& str2)
{
	if(LangCode::SYSTEM != str2.GetLangCode())
	{
		return (strcmp(str, (char*)(Str(LangCode::SYSTEM, str2).GetBuf())) > 0);
	}
	else
	{
		return (strcmp(str, (char*)str2.GetBuf()) > 0);
	}
}

bool operator>(const wchar_t* str, const Str& str2)
{
	if(!str2.IsUnicode())
	{
		return (wcscmp(str, (wchar_t*)(Str(LangCode::UNICODE16, str2).GetBuf())) > 0);
	}
	else
	{
		return (wcscmp(str, (wchar_t*)str2.GetBuf()) > 0);
	}
}

bool operator>=(const char* str, const Str& str2)
{
	if(LangCode::SYSTEM != str2.GetLangCode())
	{
		return (strcmp(str, (char*)(Str(LangCode::SYSTEM, str2).GetBuf())) >= 0);
	}
	else
	{
		return (strcmp(str, (char*)str2.GetBuf()) >= 0);
	}
}

bool operator>=(const wchar_t* str, const Str& str2)
{
	if(!str2.IsUnicode())
	{
		return (wcscmp(str, (wchar_t*)(Str(LangCode::UNICODE16, str2).GetBuf())) >= 0);
	}
	else
	{
		return (wcscmp(str, (wchar_t*)str2.GetBuf()) >= 0);
	}
}

bool operator<(const char* str, const Str& str2)
{
	if(LangCode::SYSTEM != str2.GetLangCode())
	{
		return (strcmp(str, (char*)(Str(LangCode::SYSTEM, str2).GetBuf())) < 0);
	}
	else
	{
		return (strcmp(str, (char*)str2.GetBuf()) < 0);
	}
}

bool operator<(const wchar_t* str, const Str& str2)
{
	if(!str2.IsUnicode())
	{
		return (wcscmp(str, (wchar_t*)(Str(LangCode::UNICODE16, str2).GetBuf())) < 0);
	}
	else
	{
		return (wcscmp(str, (wchar_t*)str2.GetBuf()) < 0);
	}
}

bool operator<=(const char* str, const Str& str2)
{
	if(LangCode::SYSTEM != str2.GetLangCode())
	{
		return (strcmp(str, (char*)(Str(LangCode::SYSTEM, str2).GetBuf())) <= 0);
	}
	else
	{
		return (strcmp(str, (char*)str2.GetBuf()) <= 0);
	}
}

bool operator<=(const wchar_t* str, const Str& str2)
{
	if(!str2.IsUnicode())
	{
		return (wcscmp(str, (wchar_t*)(Str(LangCode::UNICODE16, str2).GetBuf())) <= 0);
	}
	else
	{
		return (wcscmp(str, (wchar_t*)str2.GetBuf()) <= 0);
	}
}

//Concatenation
Str& Str::operator+=(const char str)
{
	return AppendFormat("%c", str);
}

Str& Str::operator+=(const wchar_t str)
{
	return AppendFormat(L"%c", str);
}

Str& Str::operator+=(int num)
{
	return Append(Str(_savedLang).ValueOf(num));
}
//
//Str& Str::operator+=(long num)
//{
//	return Append(Str(_savedLang).ValueOf(num));
//}
//
//Str& Str::operator+=(float num)
//{
//	return Append(Str(_savedLang).ValueOf(num));
//}
//
//Str& Str::operator+=(double num)
//{
//	return Append(Str(_savedLang).ValueOf(num));
//}

Str& Str::operator+=(const char* str)
{
	int strLen = Len(str);
	_StrIns(str, LangCode::SYSTEM, strLen, GetLength(), 0, strLen);
	return *this;
}

Str& Str::operator+=(const wchar_t* str)
{
	int strLen = Len(str);
	_StrIns(str, strLen, GetLength(), 0, strLen);
	return *this;
}

Str& Str::operator+=(const Str& str)
{
	int strLen = str.GetLength();
	if(str._savedLang == LangCode::UNICODE16)
	{
		_StrIns((wchar_t*)str.GetBuf(), strLen, GetLength(), 0, strLen);
	}
	else
	{
		_StrIns((char*)str.GetBuf(), str.GetLangCode(), strLen, GetLength(), 0, strLen);
	}
	return *this;
}

//Friend Functions
Str cc::operator+(const Str& str, const char* str2)
{
	return Str(str).Append(str2);
}

Str cc::operator+(const Str& str, const wchar_t* str2)
{
	return Str(str).Append(str2);
}

Str cc::operator+(const char* str, const Str& str2)
{
	return Str(str2.GetLangCode(), str).Append(str2);
}

Str cc::operator+(const wchar_t* str, const Str& str2)
{
	return Str(str2.GetLangCode(), str).Append(str2);
}

Str cc::operator+(const Str& str, const Str& str2)
{
	return Str(str).Append(str2);
}

Str cc::operator+(const Str& str, const char ch)
{
	return Str(str).Append(ch);
}

Str cc::operator+(const char ch, const Str& str)
{
	return Str(str.GetLangCode()).Append(ch).Append(str);
}

Str cc::operator+(const Str& str, int num)
{
	return Str(str).Append(Str(str.GetLangCode()).ValueOf(num));
}

Str cc::operator+(int num, const Str& str)
{
	return Str(str.GetLangCode()).ValueOf(num).Append(str);
}
//
//Str cc::operator+(const Str& str, long num)
//{
//	return Str(str).Append(Str(str.GetLangCode()).ValueOf(num));
//}
//
//Str cc::operator+(long num, const Str& str)
//{
//	return Str(str.GetLangCode()).ValueOf(num).Append(str);
//}
//
//Str cc::operator+(const Str& str, float num)
//{
//	return Str(str).Append(Str(str.GetLangCode()).ValueOf(num));
//}
//
//Str cc::operator+(float num, const Str& str)
//{
//	return Str(str.GetLangCode()).ValueOf(num).Append(str);
//}
//
//Str cc::operator+(const Str& str, double num)
//{
//	return Str(str).Append(Str(str.GetLangCode()).ValueOf(num));
//}
//
//Str cc::operator+(double num, const Str& str)
//{
//	return Str(str.GetLangCode()).ValueOf(num).Append(str);
//}

//========================================
//SPECIAL
//this fun will take over void* buf,and relese it in this class also
Str &Str::TakeoverPointer(char **str, int len, LangCode::Type fromLangCode)
{
	_Free();
	if(str == NULL)
	{
		_nCapacity = 0;
		//_isNULL = TRUE;
		CC_BIT_ON(_Flags, CC_STR_NULL);
	}
	else
	{
		if(len < 0)
		{
			_nCapacity = (int)strlen(*str);
		}
		else
		{
			_nCapacity = len;
		}
		//_isNULL = FALSE;
		CC_BIT_OFF(_Flags, CC_STR_NULL);
	}
	_nRealCapacity = _nCapacity;
	_pBuf = (void*)*str;
#if _DEBUG_
	//for show contents with mouse on while debug
	_pBufW = (wchar_t*)_pBuf;
	_pBufC = (char*)_pBuf;
#endif //#if _DEBUG_
	_savedLang = fromLangCode;
	*str = NULL;
	return *this;
}
Str &Str::TakeoverPointer(wchar_t **str, int len)
{
	_Free();
	if(str == NULL)
	{
		_nCapacity = 0;
		//_isNULL = TRUE;
		CC_BIT_ON(_Flags, CC_STR_NULL);
	}
	else
	{
		if(len < 0)
		{
			_nCapacity = (int)wcslen(*str);
		}
		else
		{
			_nCapacity = len;
		}
		//_isNULL = FALSE;
		CC_BIT_OFF(_Flags, CC_STR_NULL);
	}
	_nRealCapacity = _nCapacity;
	_pBuf = (void*)*str;
#if _DEBUG_
	//for show contents with mouse on while debug
	_pBufW = (wchar_t*)_pBuf;
	_pBufC = (char*)_pBuf;
#endif //#if _DEBUG_
	_savedLang = LangCode::UNICODE16;
	*str = NULL;
	return *this;
}
Str &Str::TakeoverPointer(Str &str)
{
	_Free();
	_pBuf = str._pBuf;
#if _DEBUG_
	//for show contents with mouse on while debug
	_pBufW = (wchar_t*)_pBuf;
	_pBufC = (char*)_pBuf;
#endif //#if _DEBUG_
	_nCapacity = str._nCapacity;
	_nMinCapacity = str._nMinCapacity;
	_nRealCapacity = str._nRealCapacity;
	_nIncrement = str._nIncrement;
	_Flags = str._Flags;
	_savedLang = str._savedLang;
	//str._isNULL = TRUE;
	CC_BIT_ON(str._Flags, CC_STR_NULL);
	str._pBuf = NULL;
	str._Free();
	return *this;
}

/*
Str &Str::CopyPointer(char *str, int bufLength, LangCode::Type fromLangCode)
{
	_Alloc(bufLength, false);
	memcpy(_pBuf, str, bufLength);
	_nCapacity = bufLength;
	//_isNULL = FALSE;
	CC_BIT_OFF(_Flags, CC_STR_NULL);
	_nRealCapacity = _nCapacity;
	_savedLang = fromLangCode;
	return *this;
}
Str &Str::CopyPointer(wchar_t *str, int bufLength)
{
	_Alloc(bufLength, false);
	memcpy(_pBuf, str, bufLength * sizeof(wchar_t));
	_nCapacity = bufLength;
	//_isNULL = FALSE;
	CC_BIT_OFF(_Flags, CC_STR_NULL);
	_nRealCapacity = _nCapacity;
	_savedLang = LangCode::UNICODE16;
	return *this;
}

void Str::GiveoutPointer(void **str)
{
	if(*str != NULL)
	{
		delete *str;
		*str = NULL;
	}
	*str = _pBuf;
	_pBuf = NULL;
	_Free();
}
*/

//========================================
//added for treate buffer(2007/07), MUST used GetBufferUpdateLength() after this.
void* Str::GetBuffer() const
{
	return _pBuf;
};

//the real buffer nLength*sizeof(TCHAR)
void* Str::GetBufferSetLength(int nLength, bool isPreserveBuf)
{
	_Alloc(nLength, isPreserveBuf);
	return _pBuf;
};

//added 2014/08/30, recalculate length used of strlen or wcslen
int Str::UpdateStringLength()
{
	if(_savedLang == LangCode::UNICODE16)
	{
		_nCapacity = Len((const wchar_t *)_pBuf);
	}
	else
	{
		_nCapacity = Len((const char *)_pBuf);
	}
	if(_nCapacity > _nRealCapacity) {
		_nCapacity = _nRealCapacity;
	}
	return _nCapacity;
};
