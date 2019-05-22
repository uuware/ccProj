// cc.Log.h
// log to file and/or screen
// Author: Shu.KK
// 2005-09-01
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef cc_Log_H_
#define cc_Log_H_

#ifdef __BORLANDC__
#endif /* __BORLANDC__ */

#include "cc.h"
#include <windows.h>
#include <commctrl.h>
#include <wchar.h>
#include <tchar.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>
#include <fcntl.h>
#include <io.h>
#include <richedit.h>          //required to use rich edit controls
//for browDir
#include <shlobj.h>

#define CC_WindowsNT3 1
#define CC_Windows95 2
#define CC_WindowsNT4 3
#define CC_Windows98 4
#define CC_WindowsME 5
#define CC_Windows2000 6
#define CC_WindowsXP 7
#define CC_WindowsServer2003 8
#define CC_WindowsHomeServer 9
#define CC_WindowsServer2003R2 10
#define CC_WindowsVista 11
#define CC_WindowsServer2008 12
#define CC_WindowsServer2008R2 13
#define CC_Windows7 14
#define CC_WindowsServer2012 15
#define CC_Windows8 16
#define CC_WindowsServer2012R2 17
#define CC_Windows8_1 18
#define CC_WindowsNext 19
int CC_GetWindowsVersion(int* is64);
int CC_GetEnv(const TCHAR *varname, TCHAR** buf);
void CC_getLogPathWithStartupFilename(TCHAR* buf, int buflen);

//can read config other way
#if !defined(CC_LOG_GETCONFIG)
#define CC_LOG_GETCONFIG(tcSection, tcKey, nValue, tcFilePath)	::GetPrivateProfileInt(tcSection, tcKey, nValue, tcFilePath);
#define CC_LOG_SETCONFIG(tcSection, tcKey, tcValue, tcFilePath)	::WritePrivateProfileString(tcSection, tcKey, tcValue, tcFilePath);
#endif

namespace cc
{

//define debug output
#if defined(_DEBUG_)
//////////////////////////////////////////////////////////////////////////////// ASSERT & TRACE
int _cdecl CC_AssertReport(const TCHAR *file, const TCHAR *fun, int nLine, const TCHAR *expr);
void CC_LogStep(const TCHAR *file, const TCHAR *fun, int nLine);
#define CC_LOGSTEP() CC_LogStep(_T(__FILE__), _T(__FUNCTION__), __LINE__);
#define CC_ASSERT(expr) do { if(!(expr) && 1 == cc::CC_AssertReport(_T(__FILE__), _T(__FUNCTION__), __LINE__, _T(#expr))) \
		{ __asm { int 3 }; } } while (0)
#define CC_VERIFY(expr) do { if(!(expr) && 1 == cc::CC_AssertReport(_T(__FILE__), _T(__FUNCTION__), __LINE__, _T(#expr))) \
		{ __asm { int 3 }; } } while (0)
/*
for __VA_ARGS__ is from C99, not use it! also while no args then be error:DEBUG_PRINT(format,)
#define DEBUG_PRINT(format, ...) debug(format, __VA_ARGS__)
*/
/* default output to file only */
void CC_LogSource(int type, const wchar_t *file, const wchar_t *fun, int nLine, wchar_t *strFormat);
wchar_t * CC_LogSource_StrPoint(const wchar_t *strFormat, ...);
wchar_t * CC_LogSource_StrPoint(const char *strFormat, ...);
//#define CC_TRACE cc::Log::trace(_T("[%s %s %d]"), _T(__FILE__), _T(__FUNCTION__), __LINE__); cc::Log::trace
#define CC_TRACE(args)  CC_LogSource(5, _WC(__FILE__), _WC(__FUNCTION__), __LINE__, CC_LogSource_StrPoint args)
/* output to file and console */
//#define CC_DEBUG cc::Log::debug(_T("[%s %s %d]"), _T(__FILE__), _T(__FUNCTION__), __LINE__); cc::Log::debug
#define CC_DEBUG(args)  CC_LogSource(4, _WC(__FILE__), _WC(__FUNCTION__), __LINE__, CC_LogSource_StrPoint args)
#else
#define CC_LOGSTEP()
#define CC_ASSERT(x)
#define CC_VERIFY(x) (x)

//#if _MSC_VER >= 1000
//#define CC_TRACE  __noop
//#define CC_DEBUG  __noop
//#else
//#define CC_TRACE  if(false) cc::Log::trace
//#define CC_DEBUG  if(false) cc::Log::debug
//#endif // _MSC_VER >= 1000
#define CC_TRACE(args)
#define CC_DEBUG(args)
//////////////////////////////////////////////////////////////////////////////// ASSERT & TRACE
#endif

//for temp debug while want output some message
#if defined(_DEBUG_)
//#define err cc::Log::error(_T("TEMP DEBUG[Fun:%s, Line:%d]"), _T(__FUNCTION__), __LINE__);cc::Log::error
void CC_LogTimeStart(LARGE_INTEGER *_LogTime_liStart, LONGLONG *_LogTime_llFreq, const wchar_t *file, const wchar_t *fun, int nLine);
void CC_LogTimeReset(LARGE_INTEGER *_LogTime_liStart, LONGLONG _LogTime_llFreq, const wchar_t *file, const wchar_t *fun, int nLine);
void CC_LogTimePrint(LARGE_INTEGER _LogTime_liStart, LONGLONG _LogTime_llFreq, const wchar_t *file, const wchar_t *fun, int nLine);
//#define CC_LOGTIME_START() LARGE_INTEGER _LogTime_liStart;LARGE_INTEGER _LogTime_liEnd;::QueryPerformanceFrequency(&_LogTime_liStart);LONGLONG _LogTime_llFreq = _LogTime_liStart.QuadPart/1000;::QueryPerformanceCounter(&_LogTime_liStart);cc::Log::trace(_T("[Fun:%s, Line:%d]Log Elapsed Time, Start:%.0f'ms"), _T(__FUNCTION__), __LINE__, (float)(_LogTime_liStart.QuadPart/_LogTime_llFreq));
#define CC_LOGTIME_START() LARGE_INTEGER _LogTime_liStart;LONGLONG _LogTime_llFreq;CC_LogTimeStart(&_LogTime_liStart, &_LogTime_llFreq, _WC(__FILE__), _WC(__FUNCTION__), __LINE__);
//#define CC_LOGTIME_RESET() ::QueryPerformanceCounter(&_LogTime_liStart);cc::Log::trace(_T("[Fun:%s, Line:%d]Log Elapsed Time, Reset:%.0f'ms"), _T(__FUNCTION__), __LINE__, (float)(_LogTime_liStart.QuadPart/_LogTime_llFreq));;
#define CC_LOGTIME_RESET() CC_LogTimeReset(&_LogTime_liStart, _LogTime_llFreq, _WC(__FILE__), _WC(__FUNCTION__), __LINE__);
//#define CC_LOGTIME_PRINT() ::QueryPerformanceCounter(&_LogTime_liEnd);cc::Log::trace(_T("[Fun:%s, Line:%d]Log Elapsed Time:%.3f'ms(MilliSecond), Start:%.0f'ms, Now:%.0f'ms"), _T(__FUNCTION__), __LINE__, (float)(_LogTime_liEnd.QuadPart - _LogTime_liStart.QuadPart)/_LogTime_llFreq, (float)(_LogTime_liStart.QuadPart/_LogTime_llFreq), (float)(_LogTime_liEnd.QuadPart/_LogTime_llFreq));
#define CC_LOGTIME_PRINT() CC_LogTimePrint(_LogTime_liStart, _LogTime_llFreq, _WC(__FILE__), _WC(__FUNCTION__), __LINE__);
#else
#if _MSC_VER >= 1000
#define err  __noop
#else
#define err  if(false) cc::Log::error
#endif // _MSC_VER >= 1000
#define CC_LOGTIME_START()
#define CC_LOGTIME_RESET()
#define CC_LOGTIME_PRINT()
#endif

class LockObject
{
public:
	LockObject()
	{
		InitializeCriticalSection(&m_criticalSection);
	}
	~LockObject()
	{
		DeleteCriticalSection(&m_criticalSection);
	}
	void lock()
    {
        EnterCriticalSection(&m_criticalSection);
    }
	bool lockNoWait()
    {
        return (TryEnterCriticalSection(&m_criticalSection) == TRUE);
    }
	void unlock()
    {
        LeaveCriticalSection(&m_criticalSection);
    }

protected:
		CRITICAL_SECTION m_criticalSection;
};

class LockAction
{
public:
	LockAction(LockObject* lockObject) : m_lockObject(lockObject)
	{
		m_lockObject->lock();
	}
	~LockAction()
	{
		m_lockObject->unlock();
	}

protected:
		LockObject* m_lockObject;
};

template <class _KTy>
class SafeCount
{
protected:
	LockObject lockObj;
	_KTy m_count;

public:
	SafeCount()
	{
	}

	~SafeCount()
	{
	}

	_KTy operator++()
	{
		lockObj.lock();
		m_count++;
		_KTy tmp = m_count;
		lockObj.unlock();
		return tmp;
	}

	_KTy operator--()
	{
		lockObj.lock();
		m_count--;
		_KTy tmp = m_count;
		lockObj.unlock();
		return tmp;
	}

	_KTy operator++(int)
	{
		lockObj.lock();
		m_count++;
		_KTy tmp = m_count;
		lockObj.unlock();
		return tmp;
	}

	_KTy operator--(int)
	{
		lockObj.lock();
		m_count--;
		_KTy tmp = m_count;
		lockObj.unlock();
		return tmp;
	}

	_KTy operator+=(const SafeCount& m)
	{
		lockObj.lock();
		_KTy tmp = (_KTy)m;
		m_count += tmp;
		_KTy tmp = m_count;
		lockObj.unlock();
		return tmp;
	}

	_KTy operator +=(_KTy m)
	{
		lockObj.lock();
		m_count += m;
		_KTy tmp = m_count;
		lockObj.unlock();
		return tmp;
	}

	_KTy operator -=(const SafeCount& m)
	{
		lockObj.lock();
		_KTy tmp = (_KTy)m;
		m_count -= tmp;
		_KTy tmp = m_count;
		lockObj.unlock();
		return tmp;
	}

	_KTy operator -=(_KTy m)
	{
		lockObj.lock();
		m_count -= m;
		_KTy tmp = m_count;
		lockObj.unlock();
		return tmp;
	}

	_KTy operator=(const SafeCount& m)
	{
		lockObj.lock();
		m_count = m.current();
		_KTy tmp = m_count;
		lockObj.unlock();
		return tmp;
	}

	_KTy operator=(_KTy m)
	{
		lockObj.lock();
		m_count = m;
		_KTy tmp = m_count;
		lockObj.unlock();
		return tmp;
	}

	operator _KTy()
	{
		lockObj.lock();
		_KTy tmp = m_count;
		lockObj.unlock();
		return tmp;
	}
	_KTy current()
	{
		lockObj.lock();
		_KTy tmp = m_count;
		lockObj.unlock();
		return tmp;
	}
	bool lockCount(_KTy current, _KTy count)
	{
		bool ret = false;
		lockObj.lock();
		if(current == m_count)
		{
			ret = true;
			m_count += count;
		}
		lockObj.unlock();
		return ret;
	}
};

class Buf
{
protected:
	//void *m_vBuf; //here has more than m_nBufMinLen buf
	//int m_nBufMinLen; //the min length of m_vBuf
	//int m_nBufRealLen; //the Real length of m_vBuf

public:
	// constructors
	//Buf();
	//LogBuf(int nBufMinLen);
	// destructor
	//virtual ~Buf();

	//========================================
	//MAIN FUNCTION
	static void Cpy(char *dst, int bufLen, const char *src);
	static void Cpy(wchar_t *dst, int bufLen, const wchar_t *src);
	static void Mov(char *dst, int bufLen, const char *src, int maxCount);
	static void Mov(wchar_t *dst, int bufLen, const wchar_t *src, int maxCount);
	static void Cat(char *dst, int bufLen, const char *src);
	static void Cat(wchar_t *dst, int bufLen, const wchar_t *src);

	static bool Format(char *str, int bufLen, const char *strFormat, ...);
	static bool Format(wchar_t *str, int bufLen, const wchar_t *strFormat, ...);
	static bool FormatV(char *str, int bufLen, const char *strFormat, va_list argList);
	static bool FormatV(wchar_t *str, int bufLen, const wchar_t *strFormat, va_list argList);

	static char *Format(char **desNeedFree, const char *strFormat, ...);
	static wchar_t *Format(wchar_t **desNeedFree, const wchar_t *strFormat, ...);
	static char *FormatV(char **desNeedFree, const char *strFormat, va_list argList);
	static wchar_t *FormatV(wchar_t **desNeedFree, const wchar_t *strFormat, va_list argList);
};

////////////////////////////////////////////////////////////////////////////////
#if !defined(_NOCCLOG) || defined(_DEBUG_)
////////////////////////////////////////////////////////////////////////////////

class LogMsg
{
public:
	HWND m_hWnd;
	long m_nMaxLength;
protected:
	COLORREF m_DefaultTextColor;
	CHARFORMAT m_fmt;
	int nErrCount;

public:
	// constructors
	LogMsg(HWND hEdit);
	// destructor
	virtual ~LogMsg();

	//========================================
	//MAIN FUNCTION
	void setDefaultTextColor(COLORREF defaultTextColor);

	void clear();
	void write(const char *strMsg);
	void write(COLORREF textColor, const char *strMsg);
	void write(const wchar_t *strMsg);
	void write(COLORREF textColor, const wchar_t *strMsg);

protected:
	void setMsg_(COLORREF textColor, const void *msg, bool isUnicode);
};

class Log
{
protected:
	typedef enum tagLogLevels
	{
		LEVEL_FATAL = 0x0001,
		LEVEL_ERROR = 0x0002,
		LEVEL_WARN = 0x0004,
		LEVEL_INFO = 0x0008,
		LEVEL_DEBUG = 0x0010,
		LEVEL_TRACEFILE = 0x1000,
		LEVEL_TRACECONSOLE = 0x2000,
	}LogLevels;
public:
	// constructors
	Log();
	// destructor
	virtual ~Log();

	//========================================
	//MAIN FUNCTION
	//static bool CreateConsoleThread();
	//static unsigned long _stdcall ConsoleThread(void *thisClass);
	static bool CreateConsole();
	//very very important:hEdit'GWLP_WNDPROC must not be Control::StaticWindowProc,if so,will come in dead loop
	//if hEdit is NULL,not set nMaxLength
	static bool SetEditHwnd(HWND hEdit, int nMaxLength = 0);
	static HWND GetHWND();
	static void WaitClose();
	//static bool isFilter(const TCHAR* filterKey);
	static bool IsConfiged();
	static bool SaveConfig(const TCHAR *fullpath);
	static void fatal(const char* szOutput, ...);
	static void fatal(const wchar_t* szOutput, ...);
	static void error(const char* szOutput, ...);
	static void error(const wchar_t* szOutput, ...);
	static void warn(const char* szOutput, ...);
	static void warn(const wchar_t* szOutput, ...);
	static void info(const char* szOutput, ...);
	static void info(const wchar_t* szOutput, ...);
	static void debug(const char* szOutput, ...);
	static void debug(const wchar_t* szOutput, ...);
	static void trace(const char* szOutput, ...);
	static void trace(const wchar_t* szOutput, ...);
	static void traceConsole(COLORREF textColor, const char* szOutput, ...);
	static void traceConsole(COLORREF textColor, const wchar_t* szOutput, ...);
	static void printStackTrace();
	//print to VS output window 
	static void OutputDebug(char* pszFormat, ...);
	static void OutputDebug(wchar_t* pszFormat, ...);

protected:
	static HANDLE m_handLogFile;
	static FILE *m_fileLogFile;
	static bool m_isStart;
	static int m_nFileMaxSize;
	static int m_nFileMaxCount;
	static int m_nConsoleMaxSize;
	//static int m_nFileLogLevel;
	//static int m_nConsoleLogLevel;
	static TCHAR m_tcIniFile[_MAX_FNAME * 2 + 1];
	static TCHAR m_tcLogFile[_MAX_FNAME * 2 + 1];
	static HINSTANCE m_hRichLib; //used to link with rich edit control library

	//static Buf *m_LogBuf;
	static LogMsg *m_Logmsg;
	static HWND m_hWConsole; //console window HWND
	static HWND m_hWOption;
	static HWND m_hEdit;
	static HINSTANCE m_hInstance;
	static UINT m_uLevels;
	static HANDLE m_hWait;

	static bool m_isConfiged;
	static RECT m_Rect;
	static UINT m_uCfg;
	static LockObject lockWrite;

	//for printStackTrace
	static HMODULE m_hDbhHelp;
	static HMODULE m_hDbhKernel32;

	//protected constants
	static void _Start();
	static void _waitEnter();
	static void _waitEnd();
	static void _checkSize();
	static void _CreateConsoleItem(HWND hwnd);
	static void _CreateOption();
	static void _CreateOptionItem(HWND hwnd);
	static bool _fout(const wchar_t* buf);
	static bool _CreateFile();
	static bool _IsConAlive();
	static bool _IsLogAlive();
	static const wchar_t *_GetLogLevelName(int index);
	static void _log(int nLogLevel, const char* szOutput, va_list argList,
		bool notFile = false, bool notCons = false, COLORREF textColor = (COLORREF)-1);
	static void _log(int nLogLevel, const wchar_t* szOutput, va_list argList,
		bool notFile = false, bool notCons = false, COLORREF textColor = (COLORREF)-1);
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK OptWndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
};

////////////////////////////////////////////////////////////////////////////////
#else //#if !defined(_NOCCLOG) || defined(_DEBUG_)
////////////////////////////////////////////////////////////////////////////////

class Log
{
public:
	static bool CreateConsole(){return true;};
	static bool SetEditHwnd(HWND hEdit, int nMaxLength = 0){return true;};
	static HWND GetHWND(){return NULL;};
	static void WaitClose(){};
	static bool IsConfiged(){return true;};
	static bool SaveConfig(const TCHAR *fullpath){return true;};
#ifdef _DEBUG_
	static void SetTitle(const char* szOutput, ...){};
	static void SetTitle(const wchar_t* szOutput, ...){};
#endif
	static void fatal(const char* szOutput, ...){};
	static void fatal(const wchar_t* szOutput, ...){};
	static void error(const char* szOutput, ...){};
	static void error(const wchar_t* szOutput, ...){};
	static void warn(const char* szOutput, ...){};
	static void warn(const wchar_t* szOutput, ...){};
	static void info(const char* szOutput, ...){};
	static void info(const wchar_t* szOutput, ...){};
	static void debug(const char* szOutput, ...){};
	static void debug(const wchar_t* szOutput, ...){};
	static void trace(const char* szOutput, ...){};
	static void trace(const wchar_t* szOutput, ...){};
	static void traceConsole(COLORREF textColor, const char* szOutput, ...){};
	static void traceConsole(COLORREF textColor, const wchar_t* szOutput, ...){};
	static void printStackTrace(){};
	static void OutputDebug(char* pszFormat, ...){};
	static void OutputDebug(wchar_t* pszFormat, ...){};
};

////////////////////////////////////////////////////////////////////////////////
#endif // #if !defined(_NOCCLOG) || defined(_DEBUG_)
////////////////////////////////////////////////////////////////////////////////

} //namespace cc

#endif //#ifndef cc_Log_H_
