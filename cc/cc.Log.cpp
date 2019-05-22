// cc.Log.cpp
// log to file and/or screen
// Author: Shu.KK
// 2005-09-01
#include "cc.Log.h"
#include <windows.h>
#include <process.h>
#include "dbghelp.h"
//#pragma comment(lib, "DbgHelp.lib")

using namespace cc;

#if defined(_DEBUG_)
//////////////////////////////////////////////////////////////////////////////// ASSERT & TRACE
int _cdecl cc::CC_AssertReport(const TCHAR *file, const TCHAR *fun, int nLine, const TCHAR *expr)
{
	const TCHAR *p;
	p = _tcsrchr(file, _T('\\'));
	if(p == NULL) {
		p = file;
	}
	else {
		p++;
	}
	cc::Log::fatal(_T("[%s %s:%d]ASSERT[Expr:%s]"), p, fun, nLine, expr);
	TCHAR* buf = NULL;
	Buf::Format(&buf, 
		_T("ASSERT ERROR:\r\n  Expr:%s\r\n  File:%s\r\n  Function:%s\r\n  Line:%d\r\n\r\nYes: End Application;\r\nNo: Set Debug Point;\r\nCancel: SKip Assert Error;")
		, expr, p, fun, nLine);
	
	int nOK = (int)::MessageBox(NULL, buf, _T("Assert"), MB_YESNOCANCEL | MB_ICONERROR);
	free(buf);
	buf = NULL;
	if(IDYES == nOK)
	{
		//end app
		exit(1);
	}
	else if(IDNO == nOK)
	{
		//set debug
		return 1;
	}
	return 0;
	//else skip error
}

void cc::CC_LogStep(const TCHAR *file, const TCHAR *fun, int nLine)
{
	const TCHAR *p;
	p = _tcsrchr(file, _T('\\'));
	if(p == NULL) {
		p = file;
	}
	else {
		p++;
	}
	cc::Log::trace(_T("[%s %s:%d]"), p, fun, nLine);
}

void cc::CC_LogSource(int type, const wchar_t *file, const wchar_t *fun, int nLine, wchar_t *strFormat)
{
	const wchar_t *p;
	p = wcsrchr(file, L'\\');
	if(p == NULL) {
		p = file;
	}
	else {
		p++;
	}
	if(type == 4) {
		cc::Log::debug(L"[%s %s:%d]%s", p, fun, nLine, strFormat);
	}
	else {
		cc::Log::trace(L"[%s %s:%d]%s", p, fun, nLine, strFormat);
	}
	if(strFormat != NULL) {
		free(strFormat);
	}
}

/* need free the result! */
wchar_t * cc::CC_LogSource_StrPoint(const wchar_t *strFormat, ...)
{
	wchar_t* buf = NULL;
	va_list argList;
	va_start(argList, strFormat);
	Buf::FormatV(&buf, strFormat, argList);
	va_end(argList);
	return buf;
}

/* need free the result! */
wchar_t * cc::CC_LogSource_StrPoint(const char *strFormat, ...)
{
	char* buf = NULL;
	va_list argList;
	va_start(argList, strFormat);
	Buf::FormatV(&buf, strFormat, argList);
	va_end(argList);

	wchar_t *cDes = NULL;
	int nLen = MultiByteToWideChar(CP_ACP, 0, buf, -1, NULL, 0);
	if(nLen >= 0)
	{
		cDes = (wchar_t*)malloc((nLen + 1) * sizeof(wchar_t));
		memset(cDes, L'\0', (nLen + 1) * sizeof(wchar_t));
		MultiByteToWideChar(CP_ACP, 0, buf, -1, cDes, nLen);
	}
	free(buf);
	buf = NULL;
	return cDes;
}

void cc::CC_LogTimeStart(LARGE_INTEGER *_LogTime_liStart, LONGLONG *_LogTime_llFreq, const wchar_t *file, const wchar_t *fun, int nLine) {
	const wchar_t *p;
	p = wcsrchr(file, L'\\');
	if(p == NULL) {
		p = file;
	}
	else {
		p++;
	}
	::QueryPerformanceFrequency(_LogTime_liStart);
	*_LogTime_llFreq = _LogTime_liStart->QuadPart/1000;
	::QueryPerformanceCounter(_LogTime_liStart);
	cc::Log::trace(L"[%s %s:%d]Log Elapsed Time, Start:%.0f'ms", p, fun, nLine, (float)(_LogTime_liStart->QuadPart / *_LogTime_llFreq));
}
void cc::CC_LogTimeReset(LARGE_INTEGER *_LogTime_liStart, LONGLONG _LogTime_llFreq, const wchar_t *file, const wchar_t *fun, int nLine) {
	const wchar_t *p;
	p = wcsrchr(file, L'\\');
	if(p == NULL) {
		p = file;
	}
	else {
		p++;
	}
	::QueryPerformanceCounter(_LogTime_liStart);
	cc::Log::trace(L"[%s %s:%d]Log Elapsed Time, Reset:%.0f'ms", file, fun, nLine, (float)(_LogTime_liStart->QuadPart / _LogTime_llFreq));
}
void cc::CC_LogTimePrint(LARGE_INTEGER _LogTime_liStart, LONGLONG _LogTime_llFreq, const wchar_t *file, const wchar_t *fun, int nLine) {
	const wchar_t *p;
	p = wcsrchr(file, L'\\');
	if(p == NULL) {
		p = file;
	}
	else {
		p++;
	}
	LARGE_INTEGER _LogTime_liEnd;
	::QueryPerformanceCounter(&_LogTime_liEnd);
	cc::Log::trace(L"[%s %s:%d]Log Elapsed Time:%.3f'ms(MilliSecond), Start:%.0f'ms, Now:%.0f'ms", p, fun, nLine, (float)(_LogTime_liEnd.QuadPart - _LogTime_liStart.QuadPart) / _LogTime_llFreq, (float)(_LogTime_liStart.QuadPart / _LogTime_llFreq), (float)(_LogTime_liEnd.QuadPart / _LogTime_llFreq));
}
////////////////////////////////////////////////////////////////////////////////
#else
#endif

typedef void (WINAPI *PGNSI)(LPSYSTEM_INFO);
#ifdef VER_SUITE_WH_SERVER
#define CC_VER_SUITE_WH_SERVER                 VER_SUITE_WH_SERVER
#else
#define CC_VER_SUITE_WH_SERVER                 0x00008000
#endif
#ifdef SM_SERVERR2
#define CC_SM_SERVERR2             SM_SERVERR2
#else
#define CC_SM_SERVERR2             89
#endif

typedef BOOL (WINAPI *VerSetConditionMaskFun)(ULONGLONG conditionMask, DWORD typeMask, BYTE conditionOperator);
typedef BOOL (WINAPI *VerifyVersionInfoFun)(LPOSVERSIONINFOEXA versionInformation, DWORD typeMask, DWORDLONG conditionMask);
BOOL IsWindowsVersionChk(DWORD Major, DWORD Minor, int sp){
	OSVERSIONINFOEXA os;
	DWORDLONG dwlConditionMask = 0;
	ZeroMemory(&os, sizeof(os));
	os.dwOSVersionInfoSize = sizeof(os);
	BYTE cond = VER_EQUAL;
	DWORD flag = VER_MAJORVERSION | VER_MINORVERSION;
	//VerSetConditionMask and VerifyVersionInfo didn't appear until Windows 2000, need to check for them at runtime
	VerSetConditionMaskFun mask = (VerSetConditionMaskFun)GetProcAddress(GetModuleHandle(TEXT("kernel32.dll")), "VerSetConditionMask");
	if(mask == NULL)
	{
		return FALSE;
	}
	dwlConditionMask = mask(dwlConditionMask, VER_MAJORVERSION, cond);
	dwlConditionMask = mask(dwlConditionMask, VER_MINORVERSION, cond);
	os.dwMajorVersion = Major;
	os.dwMinorVersion = Minor;
	if(sp >= 0)
	{
		dwlConditionMask = mask(dwlConditionMask, VER_SERVICEPACKMAJOR, cond);
		os.wServicePackMajor = (WORD)sp;
		flag |= VER_SERVICEPACKMAJOR;
	}

	VerifyVersionInfoFun verInfo = (VerifyVersionInfoFun)GetProcAddress(GetModuleHandle(TEXT("kernel32.dll")), "VerifyVersionInfoA");
	if(verInfo == NULL)
	{
		return FALSE;
	}
	return verInfo(&os, flag, dwlConditionMask);
}

//get windows system
//http://msdn.microsoft.com/en-us/library/windows/desktop/ms724833(v=vs.85).aspx
int CC_GetWindowsVersion(int* is64)
{
	static int winType = 0;
	static DWORD winVer = 0;
	static int winBit64 = 0;

	if(is64 != NULL) {
		*is64 = winBit64;
	}
	if(winVer == 0)
	{
		winVer = ::GetVersion();
		if(winVer < 0x80000000)
		{
			OSVERSIONINFOEX osvi = {0};
			SYSTEM_INFO si = {0};
			ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
			ZeroMemory(&si, sizeof(SYSTEM_INFO));
			osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
			if(::GetVersionEx((OSVERSIONINFO *)&osvi) != TRUE)
			{
				osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
				::GetVersionEx((OSVERSIONINFO *)&osvi);
			}

			PGNSI pGNSI;
			pGNSI = (PGNSI)GetProcAddress(GetModuleHandle(TEXT("kernel32.dll")), "GetNativeSystemInfo");
			if(NULL != pGNSI)
			{
				pGNSI(&si);
			}
			else
			{
				GetSystemInfo(&si);
			}

			//NT
			if(osvi.dwMajorVersion == 4L)
			{
				winType = CC_WindowsNT4;
			}
			else if(osvi.dwMajorVersion == 5L && osvi.dwMinorVersion == 0L)
			{
				winType = CC_Windows2000;
			}
			else if(osvi.dwMajorVersion == 5L && osvi.dwMinorVersion == 1L)
			{
				winType = CC_WindowsXP;
			}
			else if(osvi.dwMajorVersion == 5L && osvi.dwMinorVersion == 2L)
			{
				if(osvi.wSuiteMask & CC_VER_SUITE_WH_SERVER){
					winType = CC_WindowsHomeServer;
				}
				else if(osvi.wProductType == VER_NT_WORKSTATION && si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
				{
					winType = CC_WindowsXP; //Windows XP Professional x64 Edition
				}
				else if(GetSystemMetrics(CC_SM_SERVERR2) == 0){
					winType = CC_WindowsServer2003;
				}
				else if(GetSystemMetrics(CC_SM_SERVERR2) != 0){
					winType = CC_WindowsServer2003R2;
				}
			}
			else if(osvi.dwMajorVersion == 6L) // && osvi.dwMinorVersion == 0L
			{
				if(osvi.dwMinorVersion == 0L)
				{
					if(osvi.wProductType == VER_NT_WORKSTATION)
					{
						winType = CC_WindowsVista;
					}
					else
					{
						winType = CC_WindowsServer2008;
					}
				}
				else if(osvi.dwMinorVersion == 1L)
				{
					if(osvi.wProductType != VER_NT_WORKSTATION)
					{
						winType = CC_WindowsServer2008R2; //Windows Server 2008 R2
					}
					else
					{
						winType = CC_Windows7;
					}
				}
				else if(osvi.dwMinorVersion == 2L || osvi.dwMinorVersion == 3L || osvi.dwMinorVersion > 3L)
				{
					//GetVersionEx may be altered or unavailable for releases after Windows 8.1. Instead, use the Version Helper APIs
					if(IsWindowsVersionChk(6, 2, -1) == TRUE)
					{
						osvi.dwMinorVersion = 2;
						if(osvi.wProductType != VER_NT_WORKSTATION)
						{
							winType = CC_WindowsServer2012; //Windows Server 2012
						}
						else
						{
							winType = CC_Windows8;
						}
					}
					else if(IsWindowsVersionChk(6, 3, -1) == TRUE)
					{
						osvi.dwMinorVersion = 3;
						if(osvi.wProductType != VER_NT_WORKSTATION)
						{
							winType = CC_WindowsServer2012R2;
						}
						else
						{
							winType = CC_Windows8_1;
						}
					}
					else
					{
						winType = CC_WindowsNext;
					}
				}
			}
			else if(osvi.dwMajorVersion >= 7L)
			{
				winType = CC_WindowsNext;
			}
			else
			{
				winType = CC_WindowsNT3;
			}

			if(osvi.dwMajorVersion >= 6)
			{
				if(si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
				{
					//64-bit
					winBit64 = 1;
				}
				else if(si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_INTEL)
				{
					//32-bit
					winBit64 = 0;
				}
			}

			//CC_DEBUG(("winVer:%d, dwMajorVersion:%d, dwMinorVersion:%d, wSuiteMask:%d, wProcessor:%d, GetSystemMetrics(CC_SM_SERVERR2):%d, Bit64:%d, winType:%d",
			//	winVer, osvi.dwMajorVersion, osvi.dwMinorVersion, osvi.wSuiteMask,
			//	si.wProcessorArchitecture, (int)GetSystemMetrics(CC_SM_SERVERR2), winBit64, winType));
		}
		//i thins ccProc cannot run on Windows32s
		//else if(LOBYTE(LOWORD(winVer)) < 4)
		//{
		//	winType = CC_Windows32s;
		//}
		else
		{
			OSVERSIONINFO osvi = {0};
			//Windows 95 fails if dwOSVersionInfoSize==sizeof(OSVERSIONINFOEX)
			//You have to retry with dwOSVersionInfoSize==sizeof(OSVERSIONINFO)
			osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
			::GetVersionEx((OSVERSIONINFO *)&osvi);
			if(osvi.dwMajorVersion == 4L && osvi.dwMinorVersion == 10L)
			{
				winType = CC_Windows98;
			}
			else if(osvi.dwMajorVersion == 4L && osvi.dwMinorVersion == 90L)
			{
				winType = CC_WindowsME;
			}
			else
			{
				winType = CC_Windows95;
			}
		}
	}

	return winType;
}

//need free out of this
int CC_GetEnv(const TCHAR *varname, TCHAR** buf)
{
	if(*buf != NULL)
	{
		free(*buf);
		*buf = NULL;
	}
#ifdef __STDC_SECURE_LIB__
	size_t len = 0;
	//_dupenv_s allocates the buffer itself; you have to pass it a pointer to a pointer and 
	//it sets this to the address of the newly allocated buffer.
	//notice len is buffer len(strlen+1)
	errno_t e = _tdupenv_s(buf, &len, varname);
	if(e || *buf == NULL)
	{
		return -1;
	}
	return (int)_tcsclen(*buf);
#else
	//The getenv function returns a pointer to a string associated with the matched list member.
	//The string pointed to shall not be modified by the program, but may be overwritten 
	//by a subsequent call to the getenv function.
	TCHAR* p = _tgetenv(varname);
	if(p)
	{
		int len = (int)_tcsclen(p);
		*buf = (TCHAR*)malloc((len + 1) * sizeof(TCHAR));
		memset(*buf, 0x0, len + 1);
		_tcsncpy(*buf, p, len);
		return len;
	}
	return -1;
#endif
}

//next is used by cc::Log
void CC_getLogPathWithStartupFilename(TCHAR* buf, int buflen)
{
	int bit64 = 0;
	int wv = CC_GetWindowsVersion(&bit64);
	bool isDataPath = false;

	TCHAR* sSoftPath = NULL;
	memset(buf, 0, buflen * sizeof(TCHAR));
	::GetModuleFileName(NULL, buf, buflen);

	if(wv >= CC_WindowsVista)
	{
		TCHAR* buf2 = NULL;
		int nLen = CC_GetEnv(_T("ProgramFiles"), &buf2);
		if(nLen > 0 && buf2 != NULL && _tcsnccmp(buf2, buf, nLen) == 0)
		{
			sSoftPath = buf + nLen;
			isDataPath = true;
		}
		else
		{
			if(buf2 != NULL)
			{
				free(buf2);
				buf2 = NULL;
			}
			int nLen = CC_GetEnv(_T("ProgramFiles(x86)"), &buf2);
			if(nLen > 0 && buf2 != NULL && _tcsnccmp(buf2, buf, nLen) == 0)
			{
				//sStart = sStart.Substring(sTmp.GetLength() + 1);
				sSoftPath = buf + nLen;
				isDataPath = true;
			}
			else
			{
				if(buf2 != NULL)
				{
					free(buf2);
					buf2 = NULL;
				}
				int nLen = CC_GetEnv(_T("ProgramW6432"), &buf2);
				if(nLen > 0 && buf2 != NULL && _tcsnccmp(buf2, buf, nLen) == 0)
				{
					sSoftPath = buf + nLen;
					isDataPath = true;
				}
			}
		}
		if(buf2 != NULL)
		{
			free(buf2);
			buf2 = NULL;
		}

		if(isDataPath)
		{
			TCHAR buf3[_MAX_PATH];
			memset(buf3, 0x0, _MAX_PATH * sizeof(TCHAR));
			//Minimum operating systems	Windows 2000, Windows NT 4.0 with Internet Explorer 4.0, Windows 98, Windows 95 with Internet Explorer 4.0
			if(::SHGetSpecialFolderPath(NULL, buf3, CSIDL_LOCAL_APPDATA, FALSE) == TRUE)
			{
				if(sSoftPath[0] == _T('\\'))
				{
					sSoftPath++;
				}
				while(true)
				{
					int buf3len = _tcslen(buf3);
					if(buf3[buf3len-1] != _T('\\'))
					{
						//add \ to end
						buf3[buf3len] = _T('\\');
						buf3[buf3len+1] = _T('\0');
					}

					if(0 != _taccess(buf3, 0))
					{
						if(!(::CreateDirectory(buf3, NULL) == TRUE))
						{
							//return startup path:buf
							return;
						}
					}

					TCHAR* pRet = _tcschr(sSoftPath, _T('\\'));
					if(pRet == NULL)
					{
						cc::Buf::Cat(buf3, _MAX_PATH, sSoftPath);
						break;
					}

					cc::Buf::Mov(buf3 + _tcslen(buf3), _MAX_PATH, sSoftPath, pRet - sSoftPath);
					sSoftPath = pRet + 1;
				}
				memset(buf, 0, buflen * sizeof(TCHAR));
				cc::Buf::Cpy(buf, _MAX_PATH, buf3);
			}
			else
			{
				//return startup path:buf
			}
		}
		else
		{
			//return startup path:buf
		}
	}
}

//===========================================================cc::Buf
///////////////////// constructors //////////////////////////
//Buf::Buf()
//{
	//m_vBuf = NULL;
	//m_nBufMinLen = 128;
	//m_nBufRealLen = 1;
//}
//
//Buf::Buf(int nBufMinLen)
//{
//	m_vBuf = NULL;
//	m_nBufMinLen = nBufMinLen;
//	m_nBufRealLen = 1;
//}

///////////////////// destructor //////////////////////////
//Buf::~Buf()
//{
	//if(m_vBuf != NULL)
	//{
	//	free(m_vBuf);
	//	m_vBuf = NULL;
	//}
//}

void Buf::Cpy(char *dst, int bufLen, const char *src)
{
#ifdef __STDC_SECURE_LIB__
		strcpy_s(dst, bufLen, src);
#else
		strcpy(dst, src);
#endif
}

void Buf::Cpy(wchar_t *dst, int bufLen, const wchar_t *src)
{
#ifdef __STDC_SECURE_LIB__
		wcscpy_s(dst, bufLen, src);
#else
		wcscpy(dst, src);
#endif
}

void Buf::Mov(char *dst, int bufLen, const char *src, int maxCount)
{
#ifdef __STDC_SECURE_LIB__
		strncpy_s(dst, bufLen, src, maxCount);
#else
		strncpy(dst, src, maxCount);
#endif
}

void Buf::Mov(wchar_t *dst, int bufLen, const wchar_t *src, int maxCount)
{
#ifdef __STDC_SECURE_LIB__
		wcsncpy_s(dst, bufLen, src, maxCount);
#else
		wcsncpy(dst, src, maxCount);
#endif
}

void Buf::Cat(char *dst, int bufLen, const char *src)
{
#ifdef __STDC_SECURE_LIB__
		strcat_s(dst, bufLen, src);
#else
		strcat(dst, src);
#endif
}

void Buf::Cat(wchar_t *dst, int bufLen, const wchar_t *src)
{
#ifdef __STDC_SECURE_LIB__
		wcscat_s(dst, bufLen, src);
#else
		wcscat(dst, src);
#endif
}

bool Buf::Format(char *str, int bufLen, const char *strFormat, ...)
{
	va_list argList;
	va_start(argList, strFormat);
	bool ret = FormatV(str, bufLen, strFormat, argList);
	va_end(argList);
	return ret;
}

bool Buf::Format(wchar_t *str, int bufLen, const wchar_t *strFormat, ...)
{
	va_list argList;
	va_start(argList, strFormat);
	bool ret = FormatV(str, bufLen, strFormat, argList);
	va_end(argList);
	return ret;
}

bool Buf::FormatV(char *str, int bufLen, const char *strFormat, va_list argList)
{
	CC_ASSERT(strFormat != NULL);

	int nLen = _vscprintf(strFormat, argList);
	if(nLen >= bufLen)
	{
		return false;
	}
	memset(str, 0, bufLen * sizeof(char));
#ifdef __STDC_SECURE_LIB__
	int nRLen = _vsnprintf_s(str, bufLen, nLen, strFormat, argList);
#else
	int nRLen = _vsnprintf(str, bufLen, strFormat, argList);
#endif
	return (nRLen == nLen);
}

bool Buf::FormatV(wchar_t *str, int bufLen, const wchar_t *strFormat, va_list argList)
{
	CC_ASSERT(strFormat != NULL);

	int nLen = _vscwprintf(strFormat, argList);
	if(nLen >= bufLen)
	{
		return false;
	}
	memset(str, 0, bufLen * sizeof(wchar_t));
#ifdef __STDC_SECURE_LIB__
	int nRLen = _vsnwprintf_s(str, bufLen, nLen, strFormat, argList);
#else
	int nRLen = _vsnwprintf(str, bufLen, strFormat, argList);
#endif
	return (nRLen == nLen);
}

char *Buf::Format(char **desNeedFree, const char *strFormat, ...)
{
	va_list argList;
	va_start(argList, strFormat);
	FormatV(desNeedFree, strFormat, argList);
	va_end(argList);
	return *desNeedFree;
}

wchar_t *Buf::Format(wchar_t **desNeedFree, const wchar_t *strFormat, ...)
{
	va_list argList;
	va_start(argList, strFormat);
	FormatV(desNeedFree, strFormat, argList);
	va_end(argList);
	return *desNeedFree;
}

char *Buf::FormatV(char **desNeedFree, const char *strFormat, va_list argList)
{
	if(*desNeedFree != NULL)
	{
		free(*desNeedFree);
		*desNeedFree = NULL;
	}
	if(strFormat == NULL)
	{
		return *desNeedFree;
	}
	int nLen = _vscprintf(strFormat, argList);
	*desNeedFree = (char *)malloc((nLen + 1) * sizeof(char));
	memset(*desNeedFree, 0, (nLen + 1) * sizeof(char));
#ifdef __STDC_SECURE_LIB__
	_vsnprintf_s(*desNeedFree, nLen + 1, nLen, strFormat, argList);
#else
	_vsnprintf(*desNeedFree, nLen + 1, strFormat, argList);
#endif
	return *desNeedFree;
}

wchar_t *Buf::FormatV(wchar_t **desNeedFree, const wchar_t *strFormat, va_list argList)
{
	if(*desNeedFree != NULL)
	{
		free(*desNeedFree);
		*desNeedFree = NULL;
	}
	if(strFormat == NULL)
	{
		return *desNeedFree;
	}
	int nLen = _vscwprintf(strFormat, argList);
	*desNeedFree = (wchar_t *)malloc((nLen + 1) * sizeof(wchar_t));
	memset(*desNeedFree, 0, (nLen + 1) * sizeof(wchar_t));
#ifdef __STDC_SECURE_LIB__
	_vsnwprintf_s(*desNeedFree, nLen + 1, nLen, strFormat, argList);
#else
	_vsnwprintf(*desNeedFree, nLen + 1, strFormat, argList);
#endif
	return *desNeedFree;
}

////////////////////////////////////////////////////////////////////////////////
#if !defined(_NOCCLOG) || defined(_DEBUG_)
////////////////////////////////////////////////////////////////////////////////

//===========================================================cc::LogMsg
///////////////////// constructors //////////////////////////
LogMsg::LogMsg(HWND hEdit)
{
	m_hWnd = hEdit;
	m_nMaxLength = 1024 * 64;
	m_DefaultTextColor = RGB(0, 0, 0);
	m_fmt.cbSize = sizeof(CHARFORMAT);
	m_fmt.dwMask = CFM_COLOR;
	m_fmt.dwEffects = 0;
	nErrCount = 0;
}

///////////////////// destructor //////////////////////////
LogMsg::~LogMsg()
{
}

void LogMsg::setMsg_(COLORREF textColor, const void *msg, bool isUnicode)
{
	if(m_hWnd != NULL && ::IsWindow(m_hWnd))
	{
		DWORD lpdwResult = 0;
		if(::SendMessageTimeout(m_hWnd, WM_GETTEXTLENGTH, 0, 0, SMTO_ABORTIFHUNG, 500, &lpdwResult) == 0)
		{
			nErrCount++;
			if(nErrCount > 2)
			{
				//while exit...
				m_hWnd = NULL;
			}
			return;
		}
		nErrCount = 0;
		long nPos = (long)lpdwResult;
		if(m_nMaxLength > 0 && nPos > m_nMaxLength)
		{
			//cut 1/2 of text
			::SendMessageTimeout(m_hWnd, EM_SETSEL, 0, (int)(nPos / 2), SMTO_ABORTIFHUNG, 300, &lpdwResult);
			::SendMessageTimeout(m_hWnd, EM_REPLACESEL, false, (LPARAM)_T(""), SMTO_ABORTIFHUNG, 300, &lpdwResult);
		}
		::SendMessageTimeout(m_hWnd, EM_SETSEL, nPos, nPos, SMTO_ABORTIFHUNG, 300, &lpdwResult);
		m_fmt.crTextColor = textColor;
		m_fmt.dwMask = CFM_COLOR;
		::SendMessageTimeout(m_hWnd, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&m_fmt, SMTO_ABORTIFHUNG, 300, &lpdwResult);
		if(isUnicode)
		{
			SendMessageTimeoutW(m_hWnd, EM_REPLACESEL, false, (LPARAM)msg, SMTO_ABORTIFHUNG, 300, &lpdwResult);
		}
		else
		{
			SendMessageTimeoutA(m_hWnd, EM_REPLACESEL, false, (LPARAM)msg, SMTO_ABORTIFHUNG, 300, &lpdwResult);
		}
		::SendMessageTimeout(m_hWnd, WM_VSCROLL, SB_BOTTOM, 0, SMTO_ABORTIFHUNG, 300, &lpdwResult);
	}
}

void LogMsg::setDefaultTextColor(COLORREF defaultTextColor)
{
	m_DefaultTextColor = defaultTextColor;
}

void LogMsg::clear()
{
	if(::IsWindow(m_hWnd))
	{
		::SetWindowText(m_hWnd, _T(""));
	}
}

void LogMsg::write(const char* strMsg)
{
	setMsg_(m_DefaultTextColor, strMsg, false);
}

void LogMsg::write(const wchar_t* strMsg)
{
	setMsg_(m_DefaultTextColor, strMsg, true);
}

void LogMsg::write(COLORREF textColor, const char* strMsg)
{
	setMsg_(textColor, strMsg, false);
}

void LogMsg::write(COLORREF textColor, const wchar_t* strMsg)
{
	setMsg_(textColor, strMsg, true);
}

//===========================================================cc::Log
#define CC_LOG_CONFIG "Log"
#define CC_LOG_OPENCONS "OpenConsole"
#define CC_LOG_OPENFILE "OpenFile"
#define CC_LOG_F_MAXSIZE "FileMaxSize"
#define CC_LOG_F_MAXCOUNT "FileMaxCount"
#define CC_LOG_C_MAXSIZE "ConsoleMaxSize"
#define CC_LOG_CHK_SAVEPOS "SavePositionCheckBox"
#define CC_LOG_CON_X "ConsoleX"
#define CC_LOG_CON_Y "ConsoleY"
#define CC_LOG_CON_W "ConsoleW"
#define CC_LOG_CON_H "ConsoleH"
#define CC_LOG_ONTOP "OnTop"

HANDLE Log::m_handLogFile = INVALID_HANDLE_VALUE;
//FILE *Log::m_fileLogFile = NULL;
bool Log::m_isStart = false;
int Log::m_nFileMaxSize = 1024 * 1000;
int Log::m_nFileMaxCount = 10;
int Log::m_nConsoleMaxSize = 1024 * 64;
//int Log::m_nFileLogLevel = 2;
//int Log::m_nConsoleLogLevel = 4;
TCHAR Log::m_tcIniFile[_MAX_FNAME * 2 + 1] = {'\0'};
TCHAR Log::m_tcLogFile[_MAX_FNAME * 2 + 1] = {'\0'};
HINSTANCE Log::m_hInstance = NULL;
HWND Log::m_hEdit = NULL;
HINSTANCE Log::m_hRichLib = NULL;

LogMsg *Log::m_Logmsg = NULL;
HWND Log::m_hWConsole = NULL;
HWND Log::m_hWOption = NULL;
UINT Log::m_uLevels = 0;
//CreateEvent:Attributes, Mannual reset, Initial state, Object name
HANDLE Log::m_hWait = ::CreateEvent(NULL, TRUE, TRUE, NULL);
//
bool Log::m_isConfiged = false;
RECT Log::m_Rect = {0, 0, 600, 200};
UINT Log::m_uCfg = 0x0001 | 0x0004;
LockObject Log::lockWrite = LockObject();

HMODULE Log::m_hDbhHelp = NULL;
HMODULE Log::m_hDbhKernel32 = NULL;

//for auto run cc::Log,need one and only one
cc::Log _GLOBAL_ONLY_ONCE_LOG;

///////////////////// constructors //////////////////////////
Log::Log()
{
	//not need new twice
	if(m_isStart)
	{
		throw(_T("Log is started."));
	}
	_Start();
}

///////////////////// destructor //////////////////////////
Log::~Log()
{
	if(m_Logmsg != NULL)
	{
		delete m_Logmsg;
		m_Logmsg = NULL;
	}
	if(m_hRichLib)
	{
		::FreeLibrary(m_hRichLib);
		m_hRichLib = NULL;
	}
	//if(m_fileLogFile != NULL)
	//{
	//	//fclose(m_fileLogFile);
	//	m_fileLogFile = NULL;
	//}
	if(m_handLogFile != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_handLogFile);
		m_handLogFile = INVALID_HANDLE_VALUE;
	}
	if(m_hWait != NULL)
	{
		::CloseHandle(m_hWait);
		m_hWait = NULL;
	}
	if(m_hDbhHelp != NULL)  // if not already loaded, try to load a default-one
	{
		::FreeLibrary(m_hDbhHelp);
		m_hDbhHelp = NULL;
	}
	if(m_hDbhKernel32 != NULL)
	{
		::FreeLibrary(m_hDbhKernel32);
		m_hDbhKernel32 = NULL;
	}
}

///////////////////// private/internal used methods //////////////////////////
//notice that anything in this scode cannot use _waitEnter
void Log::_Start()
{
	//when is app startup
	if(m_isStart) return;
	//if is working,wait
	_waitEnter();
	//if done by other process while _waitEnter(),return
	if(m_isStart)
	{
		_waitEnd();
		return;
	}

	m_isStart = true;
	try
	{
#ifdef _DEBUG_
		int nDebug = 1;
#else
		int nDebug = 0;
#endif
		//set ini filename:(app.exe).ini
		CC_getLogPathWithStartupFilename(m_tcIniFile, _MAX_FNAME * 2);
		//GetModuleFileName(NULL, m_tcIniFile, _MAX_FNAME * 2);
		cc::Buf::Cpy(m_tcLogFile, _MAX_FNAME * 2, m_tcIniFile);
		cc::Buf::Cat(m_tcIniFile, _MAX_FNAME * 2, _T(".ini\0"));
		//set log filename:(app.exe).log
		cc::Buf::Cat(m_tcLogFile, _MAX_FNAME * 2, _T(".log\0"));
		int nIni;
		m_uLevels = 0;
		m_uCfg = 0;
		nIni = CC_LOG_GETCONFIG(_T(CC_LOG_CONFIG), _T(CC_LOG_CHK_SAVEPOS), 1, m_tcIniFile);
		if(nIni != 0)
		{
			m_uCfg |= 0x0001;
		}
		nIni = CC_LOG_GETCONFIG(_T(CC_LOG_CONFIG), _T(CC_LOG_OPENCONS), nDebug, m_tcIniFile);
		if(nIni != 0)
		{
			m_uCfg |= 0x0002;
		}
		nIni = CC_LOG_GETCONFIG(_T(CC_LOG_CONFIG), _T(CC_LOG_OPENFILE), 1, m_tcIniFile);
		if(nIni != 0)
		{
			m_uCfg |= 0x0004;
		}
		nIni = CC_LOG_GETCONFIG(_T(CC_LOG_CONFIG), _T(CC_LOG_ONTOP), 0, m_tcIniFile);
		if(nIni != 0)
		{
			m_uCfg |= 0x0008;
		}
		nIni = CC_LOG_GETCONFIG(_T(CC_LOG_CONFIG), _T("LogFatal"), 1, m_tcIniFile);
		if(nIni != 0)
		{
			m_uLevels |= LEVEL_FATAL;
		}
		nIni = CC_LOG_GETCONFIG(_T(CC_LOG_CONFIG), _T("LogError"), 1, m_tcIniFile);
		if(nIni != 0)
		{
			m_uLevels |= LEVEL_ERROR;
		}
		nIni = CC_LOG_GETCONFIG(_T(CC_LOG_CONFIG), _T("LogWarn"), 1, m_tcIniFile);
		if(nIni != 0)
		{
			m_uLevels |= LEVEL_WARN;
		}
		nIni = CC_LOG_GETCONFIG(_T(CC_LOG_CONFIG), _T("LogInfo"), 1, m_tcIniFile);
		if(nIni != 0)
		{
			m_uLevels |= LEVEL_INFO;
		}
		nIni = CC_LOG_GETCONFIG(_T(CC_LOG_CONFIG), _T("LogDebug"), nDebug, m_tcIniFile);
		if(nIni != 0)
		{
			m_uLevels |= LEVEL_DEBUG;
		}
		nIni = CC_LOG_GETCONFIG(_T(CC_LOG_CONFIG), _T("LogTraceFile"), nDebug, m_tcIniFile);
		if(nIni != 0)
		{
			m_uLevels |= LEVEL_TRACEFILE;
		}
		nIni = CC_LOG_GETCONFIG(_T(CC_LOG_CONFIG), _T("LogTraceConsole"), 0, m_tcIniFile);
		if(nIni != 0)
		{
			m_uLevels |= LEVEL_TRACECONSOLE;
		}

		nIni = CC_LOG_GETCONFIG(_T(CC_LOG_CONFIG), _T(CC_LOG_F_MAXSIZE), 0, m_tcIniFile);
		m_nFileMaxSize = nIni >= 1024 * 10 ? nIni : 1024 * 1000;
		nIni = CC_LOG_GETCONFIG(_T(CC_LOG_CONFIG), _T(CC_LOG_F_MAXCOUNT), 0, m_tcIniFile);
		m_nFileMaxCount = nIni >= 1 ? nIni : 10;
		nIni = CC_LOG_GETCONFIG(_T(CC_LOG_CONFIG), _T(CC_LOG_C_MAXSIZE), 0, m_tcIniFile);
		m_nConsoleMaxSize = nIni >= 1024 * 10 ? nIni : 1024 * 64;

		if((m_uCfg & 0x0002) != 0)
		{
			if(!CreateConsole())
			{
				MessageBox(NULL, _T("Error for create Log Console."), _T("Message"), 0);
			}
			//traceConsole(RGB(100, 100, 100), _T("open ini file:%s"), m_tcIniFile);
			if(m_Logmsg != NULL)
			{
				TCHAR* buf = NULL;
				Buf::Format(&buf, _T("open ini file:%s\r\n"), m_tcIniFile);
				m_Logmsg->write(RGB(100, 100, 100), buf);
				free(buf);
				buf = NULL;
			}
		}

		if((m_uCfg & 0x0004) != 0)
		{
			if(!_CreateFile())
			{
				MessageBox(NULL, _T("Error for create Log File."), _T("Message"), 0);
			}
		}
	}
	catch(...)
	{
	}
	_waitEnd();
	//notice that in info(...),used _waitEnter()/_waitEnd()
	info(_T("Log is starting"));
	return;
}

bool Log::_IsConAlive()
{
	//return (::IsWindow(m_hWConsole) == TRUE);
	return (m_Logmsg != NULL);
}

bool Log::_IsLogAlive()
{
	//return (m_fileLogFile != NULL);
	return (m_handLogFile != INVALID_HANDLE_VALUE);
}

void Log::_waitEnter()
{
	while(true)
	{
		DWORD result;
		MSG msg;

		//result = ::MsgWaitForMultipleObjects(1, &m_hWait, FALSE, INFINITE, QS_SENDMESSAGE);
		result = ::WaitForMultipleObjects(1, &m_hWait, FALSE, INFINITE);
		if(result == WAIT_OBJECT_0)
		{
			::ResetEvent(m_hWait);
			break;
		}
		//result == WAIT_OBJECT_0 + 1
		else if(::IsWindow(m_hWConsole))
		{
			if(PeekMessage(&msg, m_hWConsole,  0, 0, PM_REMOVE) != 0)
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
		}
	}
}

void Log::_waitEnd()
{
	::SetEvent(m_hWait);
	return;
}

#define IDC_BTN_EXIT 1202
#define IDC_BTN_CLEAR 1201
#define IDC_BTN_OPTION 1203
#define IDC_BTN_ONTOP 1204
#define IDC_TXT_COMM 1206
LRESULT CALLBACK Log::WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	if(m_hWOption == hwnd)
	{
		return OptWndProc(hwnd, Message, wParam, lParam);
	}
	switch (Message)
	{
	case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
			case IDC_BTN_EXIT:
				{
					SendMessage(hwnd, WM_CLOSE, 0, 0);
					break;
				}
			case IDC_BTN_CLEAR:
				{
					if(m_Logmsg != NULL)
					{
						m_Logmsg->clear();
					}
					break;
				}
			case IDC_BTN_OPTION:
				{
					::EnableWindow(hwnd, FALSE);
					_CreateOption();
					break;
				}
			case IDC_BTN_ONTOP:
				{
					//::SendMessage(m_hWnd, BM_SETCHECK, nCheck, 0);
					int nCheck = (int)::SendMessage(GetDlgItem(hwnd, IDC_BTN_ONTOP), BM_GETCHECK, 0, 0);
					if(nCheck == 0)
					{
						m_uCfg &= ~0x0008;
						::SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
					}
					else
					{
						m_uCfg |= 0x0008;
						::SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
					}
					m_isConfiged = true;
					SaveConfig(m_tcIniFile);
					break;
				}
			} //switch (LOWORD(wParam))
			return 0;
		}
	case WM_CREATE:
		{
			if(m_hWConsole == NULL)
			{
				m_hWConsole = hwnd;
				_CreateConsoleItem(hwnd);
			}
			else
			{
				m_hWOption = hwnd;
				_CreateOptionItem(hwnd);
			}
			return 0;
		}
	case WM_SIZE:
		{
			RECT rect;
			GetClientRect(hwnd, &rect);
			MoveWindow(m_hEdit, 2 , 24, rect.right - 3, rect.bottom - 24, TRUE);
			break;
		}
	case WM_CLOSE:
		{
			//save position
			GetWindowRect(hwnd, &m_Rect);
			m_hWConsole = NULL;
			::DestroyWindow(hwnd);
		}
		break;
	case WM_NCDESTROY:
		{
			if(m_Logmsg != NULL)
			{
				delete m_Logmsg;
				m_Logmsg = NULL;
			}
			m_hWConsole = NULL;
		}
		break;
	default:
			break;
	}
	return DefWindowProc(hwnd,Message,wParam,lParam);  //let system deal with msg
}

void Log::_CreateConsoleItem(HWND hwnd)
{
	//create BUTTON control
	CreateWindowEx(BS_PUSHBUTTON,						  //more or 'extended' styles
					TEXT("BUTTON"),						  //'class' of control to create
					TEXT("Close"), 					  //the control caption
					WS_CHILD|WS_VISIBLE,		  //control style: how it looks
					2, 2, 80, 20,						  //control position: left,top,width,height
					hwnd,								  //parent window handle
					(HMENU)IDC_BTN_EXIT,				  //control's ID
					m_hInstance, 							  //application instance
					NULL);
	//create BUTTON control
	CreateWindowEx(BS_PUSHBUTTON,						  //more or 'extended' styles
					TEXT("BUTTON"),						  //'class' of control to create
					TEXT("Clear"),					   //the control caption
					WS_CHILD|WS_VISIBLE,		  //control style: how it looks
					84, 2, 80, 20,						 //control position: left,top,width,height
					hwnd,								  //parent window handle
					(HMENU)IDC_BTN_CLEAR, 					  //control's ID
					m_hInstance, 							  //application instance
					NULL);
	CreateWindowEx(BS_PUSHBUTTON,
					TEXT("BUTTON"),
					TEXT("Option"),
					WS_CHILD|WS_VISIBLE,
					166, 2, 70, 20, //left,top,width,height
					hwnd,
					(HMENU)IDC_BTN_OPTION,
					m_hInstance,
					NULL);
	//create checkbox control
	CreateWindowEx(WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR | WS_EX_NOPARENTNOTIFY,						  //more or 'extended' styles
					TEXT("BUTTON"),
					TEXT("OnTop"),
					WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX,
					240, 2, 120, 20, //left,top,width,height
					hwnd,
					(HMENU)IDC_BTN_ONTOP,
					m_hInstance,
					NULL);
	//create a multi-line edit control
	//try to load latest version of rich edit control
	m_hRichLib = LoadLibrary(TEXT("RICHED20.DLL"));        //Rich Edit Version 2.0/3.0
	int nRich = 1;
	if (!m_hRichLib)
	{
		//can't get latest version so try to get earlier one
		m_hRichLib = LoadLibrary(TEXT("RICHED32.DLL"));    //Rich Edit Version 1.0
		if (!m_hRichLib)
		{
			//can't get this version so inform user
			CreateWindowEx(WS_EX_CLIENTEDGE,					  //more or 'extended' styles
						_T("EDIT"),						  //'class' of control to create
						_T(""),	//the control caption
						WS_CHILD|WS_VISIBLE|WS_BORDER|		  //control style: how it looks
						ES_MULTILINE|WS_VSCROLL|ES_NOHIDESEL|ES_READONLY,
						2, 200, 40, 80,						 //control position: left,top,width,height
						hwnd,								  //parent window handle
						(HMENU)IDC_TXT_COMM, 					  //control's ID
						m_hInstance, 							  //application instance
						NULL);  
		}
		else
		{
			//RICHEDIT version 1.0 is good
			nRich = 1;
		}
	}
	else
	{
		//RICHEDIT version 2.0/3.0 is good
		nRich = 2;
	}
	//create a rich edit control
	CreateWindowEx(WS_EX_CLIENTEDGE,                    //more or 'extended' styles
				nRich == 1 ? _T("RICHEDIT") : RICHEDIT_CLASS,                         //'class' of control to create
				_T(""),            //the control caption
				WS_CHILD|WS_VISIBLE|WS_BORDER|       //control style: how it looks
				WS_VSCROLL|ES_MULTILINE|ES_READONLY,
				2, 200, 40, 80,						 //control position: left,top,width,height
				hwnd,                                //parent window handle
				(HMENU)IDC_TXT_COMM,                                //control's ID
				m_hInstance,                             //application instance
				NULL);                               //user defined information  
	//here create LogMsg
	m_hEdit = GetDlgItem(hwnd, IDC_TXT_COMM);
	if(m_Logmsg == NULL)
	{
		m_Logmsg = new LogMsg(m_hEdit);
		//m_Logmsg->setMaxLen(m_nConsoleMaxSize);
	}
	m_Logmsg->m_hWnd = m_hEdit;
	m_Logmsg->m_nMaxLength = m_nConsoleMaxSize;
	//set MS UI Gothic Font for show others language
	HFONT m_hFont;
	LOGFONT lf = {0};
	lf.lfHeight			= -14;
	lf.lfWidth			= 0;
	lf.lfEscapement		= 0;
	lf.lfOrientation	= 0;
	lf.lfWeight			= 400;
	lf.lfItalic			= 0x0;
	lf.lfUnderline		= 0x0;
	lf.lfStrikeOut		= 0x0;
	lf.lfCharSet		= 0x80;
	lf.lfOutPrecision	= 0x3;
	lf.lfClipPrecision	= 0x2;
	lf.lfQuality		= 0x1;
	lf.lfPitchAndFamily	= 0x31;
	cc::Buf::Cpy(lf.lfFaceName, 32, _T("MS UI Gothic"));
	m_hFont = ::CreateFontIndirect(&lf);
	if(m_hFont != NULL)
	{
		SendMessage(m_hEdit, WM_SETFONT, (WPARAM)m_hFont, (LPARAM)FALSE);
	}
	//backgrand color
	SendMessage(m_hEdit, EM_SETBKGNDCOLOR, (WPARAM)0, (LPARAM)RGB(220,220,220));
}

#define OPTION_W 600
#define OPTION_H 240
#define IDC_O_BTN_OK 1251
#define IDC_O_BTN_CANCEL 1252
#define IDC_O_BTN_SAVEPOS 1253
#define IDC_O_BTN_FILE 1254
#define IDC_O_BTN_CONS 1255
#define IDC_O_BTN_FA 1256
#define IDC_O_BTN_ER 1257
#define IDC_O_BTN_WA 1258
#define IDC_O_BTN_IN 1259
#define IDC_O_BTN_DE 1260
#define IDC_O_BTN_FI 1261
#define IDC_O_BTN_CO 1262
#define IDC_O_EDT_FS 1263
#define IDC_O_EDT_FC 1264
#define IDC_O_EDT_CS 1265
void Log::_CreateOptionItem(HWND hwnd)
{
	CreateWindowEx(WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR | WS_EX_NOPARENTNOTIFY,
					TEXT("BUTTON"),
					TEXT("Save Position, save position of console when click \"OK\""),
					WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX,
					2, 2, 405, 20, //left,top,width,height
					hwnd,
					(HMENU)IDC_O_BTN_SAVEPOS,
					m_hInstance,
					NULL);
	if((m_uCfg & 0x0001) != 0)
	{
		::SendMessage(GetDlgItem(hwnd, IDC_O_BTN_SAVEPOS), BM_SETCHECK, BST_CHECKED, 0);
	}

	CreateWindowEx(WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR | WS_EX_NOPARENTNOTIFY,
					TEXT("BUTTON"),
					TEXT("Enable Log to File"),
					WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX,
					2, 25, 140, 20, //left,top,width,height
					hwnd,
					(HMENU)IDC_O_BTN_FILE,
					m_hInstance,
					NULL);
	if(_IsLogAlive())
	{
		::SendMessage(GetDlgItem(hwnd, IDC_O_BTN_FILE), BM_SETCHECK, BST_CHECKED, 0);
	}

	CreateWindowEx(WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR | WS_EX_NOPARENTNOTIFY,
					TEXT("BUTTON"),
					TEXT("Open Console while startup and Enable Log to Console"),
					WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX,
					153, 25, 405, 20, //left,top,width,height
					hwnd,
					(HMENU)IDC_O_BTN_CONS,
					m_hInstance,
					NULL);
	if((m_uCfg & 0x0002) != 0)
	{
		::SendMessage(GetDlgItem(hwnd, IDC_O_BTN_CONS), BM_SETCHECK, BST_CHECKED, 0);
	}

	CreateWindowEx(WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR | WS_EX_NOPARENTNOTIFY,
					TEXT("BUTTON"),
					TEXT("Log fatal"),
					WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX,
					32, 50, 86, 20, //left,top,width,height
					hwnd,
					(HMENU)IDC_O_BTN_FA,
					m_hInstance,
					NULL);
	if((m_uLevels & LEVEL_FATAL) != 0)
	{
		::SendMessage(GetDlgItem(hwnd, IDC_O_BTN_FA), BM_SETCHECK, BST_CHECKED, 0);
	}
	CreateWindowEx(WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR | WS_EX_NOPARENTNOTIFY,
					TEXT("BUTTON"),
					TEXT("Log error"),
					WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX,
					132, 50, 85, 20, //left,top,width,height
					hwnd,
					(HMENU)IDC_O_BTN_ER,
					m_hInstance,
					NULL);
	if((m_uLevels & LEVEL_ERROR) != 0)
	{
		::SendMessage(GetDlgItem(hwnd, IDC_O_BTN_ER), BM_SETCHECK, BST_CHECKED, 0);
	}
	CreateWindowEx(WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR | WS_EX_NOPARENTNOTIFY,
					TEXT("BUTTON"),
					TEXT("Log warn"),
					WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX,
					232, 50, 85, 20, //left,top,width,height
					hwnd,
					(HMENU)IDC_O_BTN_WA,
					m_hInstance,
					NULL);
	if((m_uLevels & LEVEL_WARN) != 0)
	{
		::SendMessage(GetDlgItem(hwnd, IDC_O_BTN_WA), BM_SETCHECK, BST_CHECKED, 0);
	}
	CreateWindowEx(WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR | WS_EX_NOPARENTNOTIFY,
					TEXT("BUTTON"),
					TEXT("Log info"),
					WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX,
					332, 50, 85, 20, //left,top,width,height
					hwnd,
					(HMENU)IDC_O_BTN_IN,
					m_hInstance,
					NULL);
	if((m_uLevels & LEVEL_INFO) != 0)
	{
		::SendMessage(GetDlgItem(hwnd, IDC_O_BTN_IN), BM_SETCHECK, BST_CHECKED, 0);
	}
	CreateWindowEx(WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR | WS_EX_NOPARENTNOTIFY,
					TEXT("BUTTON"),
					TEXT("Log debug"),
					WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX,
					432, 50, 94, 20, //left,top,width,height
					hwnd,
					(HMENU)IDC_O_BTN_DE,
					m_hInstance,
					NULL);
	if((m_uLevels & LEVEL_DEBUG) != 0)
	{
		::SendMessage(GetDlgItem(hwnd, IDC_O_BTN_DE), BM_SETCHECK, BST_CHECKED, 0);
	}

	CreateWindowEx(WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR | WS_EX_NOPARENTNOTIFY,
					TEXT("BUTTON"),
					TEXT("Log trace to File"),
					WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX,
					32, 75, 140, 20, //left,top,width,height
					hwnd,
					(HMENU)IDC_O_BTN_FI,
					m_hInstance,
					NULL);
	if((m_uLevels & LEVEL_TRACEFILE) != 0)
	{
		::SendMessage(GetDlgItem(hwnd, IDC_O_BTN_FI), BM_SETCHECK, BST_CHECKED, 0);
	}

	CreateWindowEx(WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR | WS_EX_NOPARENTNOTIFY,
					TEXT("BUTTON"),
					TEXT("Log trace to Console"),
					WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX,
					232, 75, 170, 20, //left,top,width,height
					hwnd,
					(HMENU)IDC_O_BTN_CO,
					m_hInstance,
					NULL);
	if((m_uLevels & LEVEL_TRACECONSOLE) != 0)
	{
		::SendMessage(GetDlgItem(hwnd, IDC_O_BTN_CO), BM_SETCHECK, BST_CHECKED, 0);
	}

	CreateWindowEx(WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR | WS_EX_NOPARENTNOTIFY,
					TEXT("STATIC"),
					TEXT("Log File Size(KB):"),
					WS_CHILD | WS_VISIBLE,
					2, 102, 135, 17, //left,top,width,height
					hwnd,
					(HMENU)0,
					m_hInstance,
					NULL);
	TCHAR buf[10];
	cc::Buf::Format(buf, 10, _T("%d"), (int)(m_nFileMaxSize/1024));
	CreateWindowEx(WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR | WS_EX_NOPARENTNOTIFY | WS_EX_CLIENTEDGE,
					TEXT("EDIT"),
					buf,
					WS_CHILD | WS_VISIBLE | ES_NUMBER | ES_RIGHT,
					137, 102, 40, 22, //left,top,width,height
					hwnd,
					(HMENU)IDC_O_EDT_FS,
					m_hInstance,
					NULL);
	CreateWindowEx(WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR | WS_EX_NOPARENTNOTIFY,
					TEXT("STATIC"),
					TEXT("Log File Count(1~99):"),
					WS_CHILD | WS_VISIBLE,
					185, 102, 150, 20, //left,top,width,height
					hwnd,
					(HMENU)0,
					m_hInstance,
					NULL);
	cc::Buf::Format(buf, 10, _T("%d"), m_nFileMaxCount);
	CreateWindowEx(WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR | WS_EX_NOPARENTNOTIFY | WS_EX_CLIENTEDGE,
					TEXT("EDIT"),
					buf,
					WS_CHILD | WS_VISIBLE | ES_NUMBER | ES_RIGHT,
					335, 102, 30, 22, //left,top,width,height
					hwnd,
					(HMENU)IDC_O_EDT_FC,
					m_hInstance,
					NULL);
	CreateWindowEx(WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR | WS_EX_NOPARENTNOTIFY,
					TEXT("STATIC"),
					TEXT("Console Size(KB):"),
					WS_CHILD | WS_VISIBLE,
					2, 126, 135, 20, //left,top,width,height
					hwnd,
					(HMENU)0,
					m_hInstance,
					NULL);
	cc::Buf::Format(buf, 10, _T("%d"), (int)(m_nConsoleMaxSize/1024));
	CreateWindowEx(WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR | WS_EX_NOPARENTNOTIFY | WS_EX_CLIENTEDGE,
					TEXT("EDIT"),
					buf,
					WS_CHILD | WS_VISIBLE | ES_NUMBER | ES_RIGHT,
					137, 126, 40, 22, //left,top,width,height
					hwnd,
					(HMENU)IDC_O_EDT_CS,
					m_hInstance,
					NULL);

	CreateWindowEx(BS_PUSHBUTTON,
					TEXT("BUTTON"),
					TEXT("OK"),
					WS_CHILD|WS_VISIBLE,
					(OPTION_W - 190), (OPTION_H - 60), 80, 20, //left,top,width,height
					hwnd,
					(HMENU)IDC_O_BTN_OK,
					m_hInstance,
					NULL);
	CreateWindowEx(BS_PUSHBUTTON,
					TEXT("BUTTON"),
					TEXT("Cancel"),
					WS_CHILD|WS_VISIBLE,
					(OPTION_W - 100), (OPTION_H - 60), 80, 20, //left,top,width,height
					hwnd,
					(HMENU)IDC_O_BTN_CANCEL,
					m_hInstance,
					NULL);
}

LRESULT CALLBACK Log::OptWndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch (Message)
	{
	case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
			case IDC_O_BTN_OK:
				{
					TCHAR buf[10];
					GetWindowText(GetDlgItem(hwnd, IDC_O_EDT_FS), buf, 10);
					m_nFileMaxSize = (int)_tcstol(buf, NULL, 10) * 1024;
					m_nFileMaxSize = m_nFileMaxSize >= 1024 * 10 ? m_nFileMaxSize : 1024 * 1000;
					GetWindowText(GetDlgItem(hwnd, IDC_O_EDT_FC), buf, 10);
					m_nFileMaxCount = (int)_tcstol(buf, NULL, 10);
					m_nFileMaxCount = m_nFileMaxCount >= 1 ? m_nFileMaxCount : 1;
					GetWindowText(GetDlgItem(hwnd, IDC_O_EDT_CS), buf, 10);
					m_nConsoleMaxSize = (int)_tcstol(buf, NULL, 10) * 1024;
					m_nConsoleMaxSize = m_nConsoleMaxSize >= 1024 * 10 ? m_nConsoleMaxSize : 1024 * 64;
					m_isConfiged = true;
					m_uLevels = 0;
					m_uCfg = 0;
					bool isChecked = (BST_CHECKED == ::SendMessage(GetDlgItem(hwnd, IDC_O_BTN_SAVEPOS), BM_GETCHECK, 0, 0));
					if(isChecked)
					{
						m_uCfg |= 0x0001;
						//save position
						GetWindowRect(m_hWConsole, &m_Rect);
					}
					isChecked = (BST_CHECKED == ::SendMessage(GetDlgItem(hwnd, IDC_O_BTN_CONS), BM_GETCHECK, 0, 0));
					if(isChecked)
					{
						m_uCfg |= 0x0002;
					}
					isChecked = (BST_CHECKED == ::SendMessage(GetDlgItem(hwnd, IDC_O_BTN_FILE), BM_GETCHECK, 0, 0));
					if(isChecked)
					{
						m_uCfg |= 0x0004;
					}
					isChecked = (BST_CHECKED == ::SendMessage(GetDlgItem(m_hWConsole, IDC_BTN_ONTOP), BM_GETCHECK, 0, 0));
					if(isChecked)
					{
						m_uCfg |= 0x0008;
					}
					if(BST_CHECKED == ::SendMessage(GetDlgItem(hwnd, IDC_O_BTN_FA), BM_GETCHECK, 0, 0))
					{
						m_uLevels |= LEVEL_FATAL;
					}
					if(BST_CHECKED == ::SendMessage(GetDlgItem(hwnd, IDC_O_BTN_ER), BM_GETCHECK, 0, 0))
					{
						m_uLevels |= LEVEL_ERROR;
					}
					if(BST_CHECKED == ::SendMessage(GetDlgItem(hwnd, IDC_O_BTN_WA), BM_GETCHECK, 0, 0))
					{
						m_uLevels |= LEVEL_WARN;
					}
					if(BST_CHECKED == ::SendMessage(GetDlgItem(hwnd, IDC_O_BTN_IN), BM_GETCHECK, 0, 0))
					{
						m_uLevels |= LEVEL_INFO;
					}
					if(BST_CHECKED == ::SendMessage(GetDlgItem(hwnd, IDC_O_BTN_DE), BM_GETCHECK, 0, 0))
					{
						m_uLevels |= LEVEL_DEBUG;
					}
					if(BST_CHECKED == ::SendMessage(GetDlgItem(hwnd, IDC_O_BTN_FI), BM_GETCHECK, 0, 0))
					{
						m_uLevels |= LEVEL_TRACEFILE;
					}
					if(BST_CHECKED == ::SendMessage(GetDlgItem(hwnd, IDC_O_BTN_CO), BM_GETCHECK, 0, 0))
					{
						m_uLevels |= LEVEL_TRACECONSOLE;
					}
					if(((m_uLevels & 0x0004) != 0) != _IsLogAlive())
					{
						if(((m_uLevels & 0x0004) != 0))
						{
							_CreateFile();
						}
						else
						{
							//if(m_fileLogFile != NULL)
							//{
							//	//fclose(m_fileLogFile);
							//	m_fileLogFile = NULL;
							//}
							if(m_handLogFile != INVALID_HANDLE_VALUE)
							{
								CloseHandle(m_handLogFile);
								m_handLogFile = INVALID_HANDLE_VALUE;
							}
						}
					}
					SendMessage(hwnd, WM_CLOSE, 0, 0);
					SaveConfig(m_tcIniFile);
					break;
				}
			case IDC_O_BTN_CANCEL:
				{
					SendMessage(hwnd, WM_CLOSE, 0, 0);
					break;
				}
			} //switch (LOWORD(wParam))
			return 0;
		}
	case WM_SIZE:
		{
			//RECT rect;
			//GetClientRect(hwnd, &rect);
		}
		break;
	case WM_NCDESTROY:
		{
			::EnableWindow(m_hWConsole, TRUE);
			m_hWOption = NULL;
		}
		break;
	default:
			break;
	}
	return DefWindowProc(hwnd,Message,wParam,lParam);  //let system deal with msg
}

//very very important:hEdit'GWLP_WNDPROC must not be Control::StaticWindowProc,if so,will come in dead loop
//if hEdit is NULL,not set nMaxLength
bool Log::SetEditHwnd(HWND hEdit, int nMaxLength)
{
	if(hEdit != NULL)
	{
		//first close log window,there delete m_Logmsg
		if(::IsWindow(m_hWConsole))
		{
			::SendMessage(m_hWConsole, WM_CLOSE, 0, 0);
		}
		if(m_Logmsg == NULL)
		{
			m_Logmsg = new LogMsg(hEdit);
		}
		else
		{
			m_Logmsg->m_hWnd = hEdit;
		}
		if(nMaxLength > 0)
		{
			m_Logmsg->m_nMaxLength = nMaxLength;
		}
	}
	else
	{
		if(::IsWindow(m_hWConsole))
		{
			//perhaps is a miss or console us opend after SetEditHwnd.do nothing
			return false;
		}
		//restore to old
		if(m_Logmsg != NULL)
		{
			delete m_Logmsg;
			m_Logmsg = NULL;
		}
	}
	return true;
}

HWND Log::GetHWND()
{
	return m_hWConsole;
}

//this is only for Console program.if has main window or has msg loop, not need this.
void Log::WaitClose()
{
	if(!m_isStart)
	{
		_Start();
	}
	// acquire and dispatch messages until the modal state is done
	for (;;)
	{
		if(m_hWConsole == NULL || !::IsWindow(m_hWConsole))
		{
			return;
		}
		MSG msg;
		do
		{
			// pump message, but quit on WM_QUIT
			if(!::GetMessage(&msg, NULL, NULL, NULL))
			{
				return;
			}
			//process this message
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		} while(::PeekMessage(&msg, NULL, NULL, NULL, PM_NOREMOVE));
	}
}

//bool Log::CreateConsoleThread()
//{
//	//only first time need to start thread
//	//why not use _beginthreadex?see head of "cc.File.h"
//	HANDLE _threadHandle = CreateThread(NULL, 0, ConsoleThread, (void*)NULL, 0, NULL);
//	return true;
//}
//
//unsigned long _stdcall Log::ConsoleThread(void *data)
//{
//	CreateConsole();
//	return 0;
//}

bool Log::CreateConsole()
{
	if(m_hInstance == NULL)
	{
		m_hInstance = GetModuleHandle(NULL);
	}
	if(!m_isStart)
	{
		_Start();
	}
	if(m_hWConsole != NULL && ::IsWindow(m_hWConsole))
	{
		return true;
	}

	WNDCLASSEX wcx; //this structure is used for storing information about the wnd 'class'
	memset(&wcx, 0, sizeof(WNDCLASSEX)); // start with NULL defaults
	wcx.cbSize			= sizeof(WNDCLASSEX); //byte size of WNDCLASSEX struct
	TCHAR chClassName[]	= TEXT("LOGWND");
	int nConsoleW;
	int nConsoleH;
	int nConsoleX;
	int nConsoleY;
	//see if this class is created
	if(!::GetClassInfoEx(m_hInstance, TEXT("LOGWND"), &wcx))
	{
		HICON	hIcon;	 //window icon
		HCURSOR hCursor; //window cursor
		//declare and initialise wnd registration information variables
		HINSTANCE g_hInst = NULL; //store the application instance
		hIcon = (HICON)LoadImage(0, IDI_APPLICATION, IMAGE_ICON, 0, 0, LR_SHARED);
		hCursor = (HCURSOR)LoadImage(0, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_SHARED);
		wcx.style			 = CS_HREDRAW|CS_VREDRAW;			//ensure wnd is always redrawn
		wcx.lpfnWndProc 	 = (WNDPROC)WndProc;				//pointer to the Window Procedure
		wcx.cbClsExtra		 = 0;								//Extra bytes for 'class' wnds
		wcx.cbWndExtra		 = 0;								//Extra bytes for this wnd
		wcx.hInstance		 = m_hInstance;						//Application instance
		wcx.hIcon			 = hIcon;							//Application icon
		wcx.hCursor 		 = hCursor; 						//Cursor for wnd
		wcx.hbrBackground	 = (HBRUSH)(COLOR_BTNFACE+1);		//Background wnd colour
		wcx.lpszMenuName	 = NULL;							//Name of wnd menu
		wcx.lpszClassName	 = chClassName; 					//Name of this wnd 'class'
		wcx.hIconSm 		 = NULL;							//Icon in top-left corner of wnd
		//Register the wnd class with the Windows system
		if(!RegisterClassEx(&wcx))
		{
			return false;
		}

		//get last pos from ini
		int nScreenX = GetSystemMetrics(SM_CXSCREEN);
		int nScreenY = GetSystemMetrics(SM_CYSCREEN);
		nConsoleW = CC_LOG_GETCONFIG(_T(CC_LOG_CONFIG), _T(CC_LOG_CON_W), 600, m_tcIniFile);
		nConsoleH = CC_LOG_GETCONFIG(_T(CC_LOG_CONFIG), _T(CC_LOG_CON_H), 200, m_tcIniFile);
		if(nConsoleW > nScreenX)
		{
			nConsoleW = nScreenX;
		}
		if(nConsoleH > nScreenY)
		{
			nConsoleH = nScreenY;
		}
		nConsoleX = CC_LOG_GETCONFIG(_T(CC_LOG_CONFIG), _T(CC_LOG_CON_X), (nScreenX - nConsoleW - 20), m_tcIniFile);
		nConsoleY = CC_LOG_GETCONFIG(_T(CC_LOG_CONFIG), _T(CC_LOG_CON_Y), (nScreenY - nConsoleH - 30), m_tcIniFile);
		if(nConsoleX < 0)
		{
			nConsoleX = 0;
		}
		else if(nConsoleX > nScreenX)
		{
			nConsoleX = nScreenX - 20;
		}
		if(nConsoleY < 0)
		{
			nConsoleY = 0;
		}
		else if(nConsoleY > nScreenY)
		{
			nConsoleY = nScreenY - 30;
		}
	}
	else
	{
		//get last pos from m_Rect
		nConsoleX = m_Rect.left;
		nConsoleY = m_Rect.top;
		nConsoleW = m_Rect.right -m_Rect.left;
		nConsoleH = m_Rect.bottom - m_Rect.top;
	}

	HWND hwnd;	 //the wnd handle
	//create wnd of the 'class' just registered
	hwnd = CreateWindowEx(0,									//more or 'extended' styles
						chClassName,						//the 'class' of window to create
						TEXT("Log Message"),  //the window title
						WS_OVERLAPPEDWINDOW,				//window style: how it looks
						nConsoleX,	//window position: left
						nConsoleY,	//window position: top
						nConsoleW,	//window width
						nConsoleH,	//window height
						NULL,								//parent window handle
						NULL,								//handle to this windows's menu
						m_hInstance,							//application instance
						NULL);								//user defined information
	if(!hwnd)
	{
		return false;
	}
	if((m_uCfg & 0x0008) != 0)
	{
		::SendMessage(GetDlgItem(hwnd, IDC_BTN_ONTOP), BM_SETCHECK, ((m_uCfg & 0x0008) != 0), 0);
		::SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}
	m_hWConsole = hwnd;
	//Display the wnd
	ShowWindow(hwnd, SW_NORMAL);
	UpdateWindow(hwnd);
	//save position
	GetWindowRect(hwnd, &m_Rect);
	return true;
}

void Log::_CreateOption()
{
	int nOptX = (GetSystemMetrics(SM_CXSCREEN) - OPTION_W) / 2;
	int nOptY = (GetSystemMetrics(SM_CYSCREEN) - OPTION_H) / 2;
	TCHAR chClassName[] = TEXT("LOGWND");
	//create wnd of the 'class' just registered
	HWND hOpt = CreateWindowEx(0,			//more or 'extended' styles
						chClassName,		//the 'class' of window to create
						TEXT("Log Option"),//the window title
						WS_POPUPWINDOW|WS_CAPTION,		//window style: how it looks
						nOptX,				//window position: left
						nOptY,				//window position: top
						OPTION_W,				//window width
						OPTION_H,				//window height
						m_hWConsole,		//parent window handle
						NULL,				//handle to this windows's menu
						m_hInstance,		//application instance
						NULL);				//user defined information
	if(!hOpt)
	{
		return;
	}
	ShowWindow(hOpt, SW_NORMAL);
	UpdateWindow(hOpt);
}

void Log::_checkSize()
{
	DWORD  dwSizeHi;
	int nSize = GetFileSize(m_handLogFile, &dwSizeHi);
	if(nSize >= m_nFileMaxSize)
	{
		int nFileNameLen = (int)_tcslen(m_tcLogFile);
		TCHAR *cFileFrom = (TCHAR *)(void*)malloc((nFileNameLen + 5) * sizeof(TCHAR));
		TCHAR *cFileTo = (TCHAR *)(void*)malloc((nFileNameLen + 5) * sizeof(TCHAR));
		for(int n = m_nFileMaxCount - 1; n >= 1; n--)
		{
			cc::Buf::Format(cFileFrom, nFileNameLen + 5, _T("%s.%02d"), m_tcLogFile, n - 1);
			cc::Buf::Format(cFileTo, nFileNameLen + 5, _T("%s.%02d"), m_tcLogFile, n);
			//]File(cFileFrom, cFileTo, FALSE);
			MoveFileEx(cFileFrom, cFileTo, MOVEFILE_COPY_ALLOWED | MOVEFILE_WRITE_THROUGH | MOVEFILE_REPLACE_EXISTING);
		}
		cc::Buf::Format(cFileTo, nFileNameLen + 5, _T("%s.%02d"), m_tcLogFile, 0);
		//TCHAR *newFileName = (TCHAR *)(void*)malloc((nFileNameLen + 5) * sizeof(TCHAR));
		//_tcscpy(newFileName, m_tcLogFile);
		//_tcscat(newFileName, _T(".bak\0"));
		////traceConsole(RGB(100, 100, 100), _T("backup log file(size:%d):%s"), nSize, newFileName);
		//fflush(m_fileLogFile);
		CloseHandle(m_handLogFile);
		m_handLogFile = INVALID_HANDLE_VALUE;
		//CopyFile(m_tcLogFile, cFileTo, FALSE);
		MoveFileEx(m_tcLogFile, cFileTo, MOVEFILE_COPY_ALLOWED | MOVEFILE_WRITE_THROUGH | MOVEFILE_REPLACE_EXISTING);
		_CreateFile();
		if(m_Logmsg != NULL)
		{
			TCHAR* buf = NULL;
			Buf::Format(&buf, _T("backup log file(size:%d):%s\r\n"), nSize, cFileTo);
			m_Logmsg->write(RGB(100, 100, 100), buf);
			free(buf);
			buf = NULL;
		}
		free(cFileFrom);
		free(cFileTo);
		//SetFilePointer(m_handLogFile, 0, 0, FILE_BEGIN);
		//SetEndOfFile(m_handLogFile);
	}
	if(nSize == 0 || nSize > m_nFileMaxSize)
	{
		//big-endian:0xFEFF, little-endian:0xFFFE
		WORD wBOM = 0xFEFF;
		WriteFile(m_handLogFile, &wBOM, sizeof(WORD), &dwSizeHi, NULL);

		wchar_t* buf = NULL;
		Buf::Format(&buf, L"Log File(UNICODE, COMPILED:%s %s)\r\n", _WC(__DATE__), _WC (__TIME__) );
		_fout(buf);
		free(buf);
		buf = NULL;
	}
}

bool Log::_fout(const wchar_t* buf)
{
	DWORD  dwSizeHi;
	return (WriteFile(m_handLogFile, buf, (DWORD)(wcslen(buf) * sizeof(wchar_t)), &dwSizeHi, NULL) == TRUE);
}

bool Log::_CreateFile()
{
	try
	{
		//log:app.exe.log
		//traceConsole(RGB(100, 100, 100), _T("open log file:%s"), m_tcLogFile);
		if(m_Logmsg != NULL)
		{
			TCHAR* buf = NULL;
			Buf::Format(&buf, _T("open log file:%s\r\n"), m_tcLogFile);
			m_Logmsg->write(RGB(100, 100, 100), buf);
			free(buf);
			buf = NULL;
		}

		// if file exist then open,otherwise create new file
		//FILE *hFileLog = _tfopen(m_tcExeNameBuf, _T("a+t"));
		HANDLE handLogFile = ::CreateFile(
			m_tcLogFile,
			GENERIC_READ | GENERIC_WRITE, // DWORD dwDesiredAccess,
			FILE_SHARE_READ | FILE_SHARE_WRITE,	// DWORD dwShareMode
			NULL,						// securite
			OPEN_ALWAYS,				// create type
			FILE_FLAG_SEQUENTIAL_SCAN,	// attribute
			NULL						// 
		);
		if(handLogFile == INVALID_HANDLE_VALUE) return false;
		m_handLogFile = handLogFile;

		//use as stream
		//TODO:LOWORD perhaps has problom
		//int osfhandle = _open_osfhandle(LOWORD(handLogFile), _O_BINARY);
		//m_fileLogFile = _fdopen(osfhandle, "w");
		return true;
	}
	catch(...)
	{
		return false;
	}
}

const wchar_t *Log::_GetLogLevelName(int index)
{
    const static wchar_t *names[6] = {L"FATAL", L"ERROR", L"WARN", L"INFO", L"DEBUG", L"TRACE"};
    return ((index < 0) || (index > 5)) ? names[5] : names[index];
}

void Log::_log(int nLogLevel, const char* szOutput, va_list argList, bool notFile, bool notCons, COLORREF textColor)
{
	//if(!m_isStart) Start();
	bool bCons = !notCons && _IsConAlive();
	bool bFile = !notFile && _IsLogAlive();
	if(szOutput == NULL || (!bCons && !bFile))
	{
		return;
	}

	//if is working,wait
	lockWrite.lock();
	_waitEnter();
	try
	{
		SYSTEMTIME systime;
		GetLocalTime(&systime);
		//struct tm *tm;
		//time_t t;
		//t = time(NULL);
		//tm = localtime(&t); 

		//const wchar_t *cPre = Buf::Format(L"%04d/%02d/%02d %02d:%02d:%02d %s:", 
		//		tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, 
		//		tm->tm_min, tm->tm_sec, _GetLogLevelName(nLogLevel));
		wchar_t* buf = NULL;
		Buf::Format(&buf, L"%04d/%02d/%02d %02d:%02d:%02d.%03d %s:", 
				systime.wYear, systime.wMonth, systime.wDay, systime.wHour, 
				systime.wMinute, systime.wSecond, systime.wMilliseconds, _GetLogLevelName(nLogLevel));
		if(bFile)
		{
			_checkSize();
			//fseek(m_fileLogFile, 0L, SEEK_END);
			LONG lDistance = 0;
			SetFilePointer(m_handLogFile, lDistance, NULL, FILE_END);
			//fwprintf(m_fileLogFile, cPre);
			_fout(buf);
		}
		if(bCons)
		{
			if(textColor != (COLORREF)-1)
			{
				m_Logmsg->write(textColor, buf);
			}
			else
			{
				m_Logmsg->write(buf);
			}
		}
		free(buf);
		buf = NULL;

		char* buf2 = NULL;
		Buf::FormatV(&buf2, szOutput, argList);
		wchar_t *cDes = NULL;
		int nLen = MultiByteToWideChar(CP_ACP, 0, buf2, -1, NULL, 0);
		if(nLen >= 0)
		{
			cDes = (wchar_t*)malloc((nLen + 1) * sizeof(wchar_t));
			memset(cDes, L'\0', (nLen + 1) * sizeof(wchar_t));
			MultiByteToWideChar(CP_ACP, 0, buf2, -1, cDes, nLen);
		}
		free(buf2);
		buf2 = NULL;

		if(bFile)
		{
			//fwprintf(m_fileLogFile, cDes);
			_fout(cDes);
			//fwprintf(m_fileLogFile, L"\r\n");
			//fflush(m_fileLogFile);
			_fout(L"\r\n");
		}
		if(bCons)
		{
			if(textColor != (COLORREF)-1)
			{
				m_Logmsg->write(textColor, cDes);
				m_Logmsg->write(textColor, L"\r\n");
			}
			else
			{
				m_Logmsg->write(cDes);
				m_Logmsg->write(L"\r\n");
			}
		}
		if(cDes != NULL)
		{
			free(cDes);
			cDes = NULL;
		}
	}
	catch(...)
	{
	}
	_waitEnd();
	lockWrite.unlock();
}

void Log::_log(int nLogLevel, const wchar_t* szOutput, va_list argList, bool notFile, bool notCons, COLORREF textColor)
{
	//if(!m_isStart) Start();
	bool bCons = _IsConAlive() && !notCons;
	bool bFile = _IsLogAlive() && !notFile;
	if(szOutput == NULL || (!bCons && !bFile))
	{
		return;
	}

	//if is working,wait
	lockWrite.lock();
	_waitEnter();
	try
	{
		SYSTEMTIME systime;
		GetLocalTime(&systime);
		//struct tm *tm;
		//time_t t;
		//t = time(NULL);
		//tm = localtime(&t); 

		//const wchar_t *cPre = Buf::Format(L"%04d/%02d/%02d %02d:%02d:%02d %s:",
		//		tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour,
		//		tm->tm_min, tm->tm_sec, _GetLogLevelName(nLogLevel));
		wchar_t* buf = NULL;
		Buf::Format(&buf, L"%04d/%02d/%02d %02d:%02d:%02d.%03d %s:", 
				systime.wYear, systime.wMonth, systime.wDay, systime.wHour, 
				systime.wMinute, systime.wSecond, systime.wMilliseconds, _GetLogLevelName(nLogLevel));
		if(bFile)
		{
			_checkSize();
			//fseek(m_fileLogFile, 0L, SEEK_END);
			LONG lDistance = 0;
			SetFilePointer(m_handLogFile, lDistance, NULL, FILE_END);
			//fwprintf(m_fileLogFile, cPre);
			_fout(buf);
		}
		if(bCons)
		{
			if(textColor != (COLORREF)-1)
			{
				m_Logmsg->write(textColor, buf);
			}
			else
			{
				m_Logmsg->write(buf);
			}
		}
		free(buf);
		buf = NULL;

		Buf::FormatV(&buf, szOutput, argList);
		if(bFile)
		{
			//fwprintf(m_fileLogFile, cOut);
			//fwprintf(m_fileLogFile, L"\r\n");
			//fflush(m_fileLogFile);
			_fout(buf);
			_fout(L"\r\n");
		}
		if(bCons)
		{
			if(textColor != (COLORREF)-1)
			{
				m_Logmsg->write(textColor, buf);
				m_Logmsg->write(textColor, L"\r\n");
			}
			else
			{
				m_Logmsg->write(buf);
				m_Logmsg->write(L"\r\n");
			}
		}
		free(buf);
		buf = NULL;
	}
	catch(...)
	{
	}
	_waitEnd();
	lockWrite.unlock();
}

//bool Log::isFilter(const TCHAR* filterKey)
//{
//	if(!_IsLogAlive() && !_IsConAlive())
//	{
//		return false;
//	}
//	return true;
//}
//
bool Log::IsConfiged()
{
	return m_isConfiged;
}

bool Log::SaveConfig(const TCHAR *fullpath)
{
	TCHAR buf[100];
	CC_LOG_SETCONFIG(_T(CC_LOG_CONFIG), _T(CC_LOG_OPENCONS), ((m_uCfg & 0x0002) != 0) ? _T("1") : _T("0"), fullpath);
	CC_LOG_SETCONFIG(_T(CC_LOG_CONFIG), _T(CC_LOG_OPENFILE), ((m_uCfg & 0x0004) != 0) ? _T("1") : _T("0"), fullpath);
	CC_LOG_SETCONFIG(_T(CC_LOG_CONFIG), _T(CC_LOG_CHK_SAVEPOS), ((m_uCfg & 0x0001) != 0) ? _T("1") : _T("0"), fullpath);
	if((m_uCfg & 0x0001) != 0)
	{
		//save position
		cc::Buf::Format(buf, 100, _T("%d"), m_Rect.left);
		CC_LOG_SETCONFIG(_T(CC_LOG_CONFIG), _T(CC_LOG_CON_X), buf, fullpath);
		cc::Buf::Format(buf, 100, _T("%d"), m_Rect.top);
		CC_LOG_SETCONFIG(_T(CC_LOG_CONFIG), _T(CC_LOG_CON_Y), buf, fullpath);
		cc::Buf::Format(buf, 100, _T("%d"), m_Rect.right - m_Rect.left);
		CC_LOG_SETCONFIG(_T(CC_LOG_CONFIG), _T(CC_LOG_CON_W), buf, fullpath);
		cc::Buf::Format(buf, 100, _T("%d"), m_Rect.bottom - m_Rect.top);
		CC_LOG_SETCONFIG(_T(CC_LOG_CONFIG), _T(CC_LOG_CON_H), buf, fullpath);
	}
	CC_LOG_SETCONFIG(_T(CC_LOG_CONFIG), _T(CC_LOG_ONTOP), ((m_uCfg & 0x0008) != 0) ? _T("1") : _T("0"), fullpath);
	CC_LOG_SETCONFIG(_T(CC_LOG_CONFIG), _T("LogFatal"), (m_uLevels & LEVEL_FATAL) ? _T("1") : _T("0"), fullpath);
	CC_LOG_SETCONFIG(_T(CC_LOG_CONFIG), _T("LogError"), (m_uLevels & LEVEL_ERROR) ? _T("1") : _T("0"), fullpath);
	CC_LOG_SETCONFIG(_T(CC_LOG_CONFIG), _T("LogWarn"), (m_uLevels & LEVEL_WARN) ? _T("1") : _T("0"), fullpath);
	CC_LOG_SETCONFIG(_T(CC_LOG_CONFIG), _T("LogInfo"), (m_uLevels & LEVEL_INFO) ? _T("1") : _T("0"), fullpath);
	CC_LOG_SETCONFIG(_T(CC_LOG_CONFIG), _T("LogDebug"), (m_uLevels & LEVEL_DEBUG) ? _T("1") : _T("0"), fullpath);
	CC_LOG_SETCONFIG(_T(CC_LOG_CONFIG), _T("LogTraceFile"), (m_uLevels & LEVEL_TRACEFILE) ? _T("1") : _T("0"), fullpath);
	CC_LOG_SETCONFIG(_T(CC_LOG_CONFIG), _T("LogTraceConsole"), (m_uLevels & LEVEL_TRACECONSOLE) ? _T("1") : _T("0"), fullpath);

	cc::Buf::Format(buf, 100, _T("%d"), m_nFileMaxSize);
	CC_LOG_SETCONFIG(_T(CC_LOG_CONFIG), _T(CC_LOG_F_MAXSIZE), buf, fullpath);
	cc::Buf::Format(buf, 100, _T("%d"), m_nFileMaxCount);
	CC_LOG_SETCONFIG(_T(CC_LOG_CONFIG), _T(CC_LOG_F_MAXCOUNT), buf, fullpath);
	cc::Buf::Format(buf, 100, _T("%d"), m_nConsoleMaxSize);
	CC_LOG_SETCONFIG(_T(CC_LOG_CONFIG), _T(CC_LOG_C_MAXSIZE), buf, fullpath);
	return true;
}

void Log::fatal(const char* szOutput, ...)
{
	if((m_uLevels & LEVEL_FATAL) != 0)
	{
		va_list argList;
		va_start(argList, szOutput);
		_log(0, szOutput, argList, false, false, RGB(255, 0, 0));
		va_end(argList);
	}
}

void Log::fatal(const wchar_t* szOutput, ...)
{
	if((m_uLevels & LEVEL_FATAL) != 0)
	{
		va_list argList;
		va_start(argList, szOutput);
		_log(0, szOutput, argList, false, false, RGB(255, 0, 0));
		va_end(argList);
	}
}

void Log::error(const char* szOutput, ...)
{
	if((m_uLevels & LEVEL_ERROR) != 0)
	{
		va_list argList;
		va_start(argList, szOutput);
		_log(1, szOutput, argList, false, false, RGB(255, 0, 255));
		va_end(argList);
	}
}

void Log::error(const wchar_t* szOutput, ...)
{
	if((m_uLevels & LEVEL_ERROR) != 0)
	{
		va_list argList;
		va_start(argList, szOutput);
		_log(1, szOutput, argList, false, false, RGB(255, 0, 255));
		va_end(argList);
	}
}

void Log::warn(const char* szOutput, ...)
{
	if((m_uLevels & LEVEL_WARN) != 0)
	{
		va_list argList;
		va_start(argList, szOutput);
		_log(2, szOutput, argList, false, false, RGB(255, 128, 0));
		va_end(argList);
	}
}

void Log::warn(const wchar_t* szOutput, ...)
{
	if((m_uLevels & LEVEL_WARN) != 0)
	{
		va_list argList;
		va_start(argList, szOutput);
		_log(2, szOutput, argList, false, false, RGB(255, 128, 0));
		va_end(argList);
	}
}

void Log::info(const char* szOutput, ...)
{
	if((m_uLevels & LEVEL_INFO) != 0)
	{
		va_list argList;
		va_start(argList, szOutput);
		_log(3, szOutput, argList);
		va_end(argList);
	}
}

void Log::info(const wchar_t* szOutput, ...)
{
	if((m_uLevels & LEVEL_INFO) != 0)
	{
		va_list argList;
		va_start(argList, szOutput);
		_log(3, szOutput, argList);
		va_end(argList);
	}
}

void Log::debug(const char* szOutput, ...)
{
	if((m_uLevels & LEVEL_DEBUG) != 0)
	{
		va_list argList;
		va_start(argList, szOutput);
		_log(4, szOutput, argList);
		va_end(argList);
	}
}

void Log::debug(const wchar_t* szOutput, ...)
{
	if((m_uLevels & LEVEL_DEBUG) != 0)
	{
		va_list argList;
		va_start(argList, szOutput);
		_log(4, szOutput, argList);
		va_end(argList);
	}
}

void Log::trace(const char* szOutput, ...)
{
	if( ((m_uLevels & LEVEL_TRACEFILE) != 0 && _IsLogAlive()) || ((m_uLevels & LEVEL_TRACECONSOLE) != 0 && _IsConAlive()) )
	{
		va_list argList;
		va_start(argList, szOutput);
		_log(5, szOutput, argList, (m_uLevels & LEVEL_TRACEFILE) == 0, (m_uLevels & LEVEL_TRACECONSOLE) == 0);
		va_end(argList);
	}
}

void Log::trace(const wchar_t* szOutput, ...)
{
	if( ((m_uLevels & LEVEL_TRACEFILE) != 0 && _IsLogAlive()) || ((m_uLevels & LEVEL_TRACECONSOLE) != 0 && _IsConAlive()) )
	{
		va_list argList;
		va_start(argList, szOutput);
		_log(5, szOutput, argList, (m_uLevels & LEVEL_TRACEFILE) == 0, (m_uLevels & LEVEL_TRACECONSOLE) == 0);
		va_end(argList);
	}
}

void Log::traceConsole(COLORREF textColor, const char* szOutput, ...)
{
	if(_IsConAlive() && (m_uLevels & LEVEL_TRACECONSOLE) != 0)
	{
		va_list argList;
		va_start(argList, szOutput);
		_log(5, szOutput, argList, true, false, textColor);
		va_end(argList);
	}
}

void Log::traceConsole(COLORREF textColor, const wchar_t* szOutput, ...)
{
	if(_IsConAlive() && (m_uLevels & LEVEL_TRACECONSOLE) != 0)
	{
		va_list argList;
		va_start(argList, szOutput);
		_log(5, szOutput, argList, true, false, textColor);
		va_end(argList);
	}
}

// SymInitialize()
typedef BOOL (__stdcall *tSI)( IN HANDLE hProcess, IN PSTR UserSearchPath, IN BOOL fInvadeProcess );
// SymGetSymFromAddr64()
typedef BOOL (__stdcall *tSGSFA)( IN HANDLE hProcess, IN DWORD64 dwAddr,
	OUT PDWORD64 pdwDisplacement, OUT PIMAGEHLP_SYMBOL64 Symbol );
// SymGetLineFromAddr64()
typedef BOOL (__stdcall *tSGLFA)( IN HANDLE hProcess, IN DWORD64 dwAddr,
	OUT PDWORD pdwDisplacement, OUT PIMAGEHLP_LINE64 Line );
void Log::printStackTrace()
{
	/*
	static tSGSFA pSGSFA = NULL;
	static tSGLFA pSGLFA = NULL;
	static tSI pSI = NULL;
	if(m_hDbhHelp == NULL)  // if not already loaded, try to load a default-one
	{
		m_hDbhHelp = LoadLibrary( _T("dbghelp.dll") );
	}
	if(m_hDbhKernel32 == NULL)
	{
		m_hDbhKernel32 = ::LoadLibrary(_T("kernel32.dll"));
	}
	if(m_hDbhHelp == NULL || m_hDbhKernel32 == NULL)
	{
		return;
	}

	if(pSI == NULL)
	{
		pSI = (tSI) GetProcAddress(m_hDbhHelp, "SymInitialize" );
	}
	if(pSGSFA == NULL)
	{
		pSGSFA = (tSGSFA) GetProcAddress(m_hDbhHelp, "SymGetSymFromAddr64" );
	}
	if(pSGLFA == NULL)
	{
		pSGLFA = (tSGLFA) GetProcAddress(m_hDbhHelp, "SymGetLineFromAddr64" );
	}
	if(pSGSFA == NULL || pSGSFA == NULL || pSI == NULL)
	{
		return;
	}

	static USHORT (WINAPI *pfnCaptureStackBackTrace)(ULONG, ULONG, PVOID*, PULONG) = NULL;
	if(pfnCaptureStackBackTrace == NULL)
	{
		reinterpret_cast<void*&>(pfnCaptureStackBackTrace) = ::GetProcAddress(m_hDbhKernel32, "RtlCaptureStackBackTrace");
	}
	if(pfnCaptureStackBackTrace == NULL)
	{
		return;
	}
	
	enum { STACKWALK_MAX_NAMELEN = 1024 }; // max name length for found symbols
	IMAGEHLP_SYMBOL64 *pSym = NULL;
	pSym = (IMAGEHLP_SYMBOL64 *) malloc(sizeof(IMAGEHLP_SYMBOL64) + STACKWALK_MAX_NAMELEN);
	if(!pSym)
	{
		return;
	}
	memset(pSym, 0, sizeof(IMAGEHLP_SYMBOL64) + STACKWALK_MAX_NAMELEN);
	pSym->SizeOfStruct = sizeof(IMAGEHLP_SYMBOL64);
	pSym->MaxNameLength = STACKWALK_MAX_NAMELEN;

	IMAGEHLP_LINE64 line;
	memset(&line, 0x0, sizeof(line));
	line.SizeOfStruct = sizeof(line);
	DWORD offsetFromLine = 0;

	void *stack[1024];
	memset(stack, 0x0, 1024);
	HANDLE process = GetCurrentProcess();
	pSI(process, NULL, TRUE);
	WORD numberOfFrames = pfnCaptureStackBackTrace(0, 1000, stack, NULL);
	for(int i = 0; i < numberOfFrames; i++)
	{
		DWORD64 offsetFromSmybol = 0;
		DWORD dwDisplacement = 0;
		if(pSGSFA(process, (DWORD64)(stack[i]), &offsetFromSmybol, pSym) == FALSE)
		{
			break;
		}
		if(pSGLFA(process, (DWORD)(stack[i]), &offsetFromLine, &line) == FALSE)
		{
			break;
		}
		cc::Log::info("at %s in %s:%d, address 0x%0X", pSym->Name, line.FileName, line.LineNumber, pSym->Address);
	}
	free(pSym);
	*/
}

//print to VS output window 
void Log::OutputDebug(char* pszFormat, ...)
{
#if defined(_DEBUG_)
    va_list	argp;
    va_start(argp, pszFormat);
	char* buf = NULL;
	cc::Buf::FormatV(&buf, pszFormat, argp);
    va_end(argp);
    OutputDebugStringA(buf);
#endif
}
void Log::OutputDebug(wchar_t* pszFormat, ...)
{
#if defined(_DEBUG_)
    va_list	argp;
    va_start(argp, pszFormat);
	wchar_t* buf = NULL;
	cc::Buf::FormatV(&buf, pszFormat, argp);
    va_end(argp);
    OutputDebugStringW(buf);
#endif
}
////////////////////////////////////////////////////////////////////////////////
#endif // #if !defined(_NOCCLOG) || defined(_DEBUG_)
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
