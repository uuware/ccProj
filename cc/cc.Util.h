// cc.Util.h
// cc.Util class
// Author: Shu.KK
// 2006-01-01
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef CC_Util_H_
#define CC_Util_H_

#include "cc.h"
#include <windows.h>
#include <commctrl.h>
//#include <string.h>
#include <wchar.h>
#include <tchar.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
//for browDir
#include <shlobj.h>
//for ::OleLoadPicture
#include <olectl.h>
//for PlaySound
#include "Mmsystem.h"
#include <sys/stat.h>

#include "cc.Log.h"
#include "cc.Str.h"
#include "cc.Exception.h"
#include "cc.List.h"
#include "cc.File.h"

char* CC_StrCopy(char** sDec, const char* sSrc, int nLen = -1);
wchar_t* CC_StrCopy(wchar_t** sDec, const wchar_t* sSrc, int nLen = -1);
void turnWord(char *pBuf);
void turnWord(wchar_t *pBuf);

typedef HANDLE HTHEME;

namespace cc
{

class RegistryKey;
class Registry : public Object
{
public:
	//need release this point
	static RegistryKey* GetClassesRoot();
	static RegistryKey* GetCurrentConfig();
	static RegistryKey* GetCurrentUser();
	static RegistryKey* GetDynData();
	static RegistryKey* GetLocalMachine();
	static RegistryKey* GetPerformanceData();
	static RegistryKey* GetUsers();
};

//namespace Microsoft.Win32.Registry
typedef struct tagRegistryHive
{
	enum Type
	{
		ClassesRoot = 0x80000000,
		CurrentConfig = 0x80000005,
		CurrentUser = 0x80000001,
		DynData = 0x80000006,
		LocalMachine = 0x80000002,
		PerformanceData = 0x80000004,
		Users = 0x80000003
	};
}RegistryHive;

class RegistryKey : public Object
{
friend class Registry;
protected:
	RegistryKey(RegistryHive::Type hKeyRoot);

public:
	virtual ~RegistryKey();
	void Close();
	//if not OK,not close current key
	bool OpenSubKey(const TCHAR* subkey, bool writable, bool createIfNotExist = false);
	RegistryKey* OpenSubKeyNewHandle(const TCHAR* subkey, bool writable);
	//if not OK,not close current key
	bool CreateSubKey(const TCHAR* subkey);
	RegistryKey* CreateSubKeyNewHandle(const TCHAR* subkey);
	bool DeleteSubKey(const TCHAR* subkey);
	bool DeleteValue(const TCHAR* subkey);//, bool throwOnMissingSubKey = false
	bool DeleteSubKeyTree(const TCHAR* subKey);
	//void Flush();
	//cc::List<cc::Str>* GetSubKeyNames(const TCHAR* subKey);
	DWORD GetValueLength(const TCHAR* subkey, DWORD& dwType);
	cc::Str GetValue(const TCHAR* subkey, const TCHAR* defaultValue = NULL);
	cc::Str GetValue(const cc::Str& subkey, const cc::Str& defaultValue = cc::Str());
	DWORD GetValue(const TCHAR* subkey, DWORD dwValue);
	DWORD GetValue(const cc::Str& subkey, DWORD dwValue);
	bool GetValue(const TCHAR* subkey, BYTE* pValue, DWORD& dwValueLenInBytes);
	DWORD GetValue(const TCHAR* subkey, BYTE** outbuf);
	//cc::Map<cc::Str. cc::Str>* GetValueNames(const TCHAR* subKey);
	bool SetValue(const TCHAR* subkey, const TCHAR* value);
	bool SetValue(const cc::Str& subkey, const cc::Str& value);
	bool SetValue(const TCHAR* subkey, DWORD dwValue);
	bool SetValue(const cc::Str& subkey, DWORD dwValue);
	bool SetValue(const TCHAR* subkey, const BYTE* value, DWORD dwValueLenInBytes);

protected:
	RegistryHive::Type _hKeyRoot;
	HKEY _hOpenedKey;

	long DeleteSubKeyTreeSub(HKEY hKeyRecurse, const TCHAR* subKey);
};

//========================================
//Windows Version
typedef struct tagWindowsVersion
{
	enum Type
	{
		WindowsNT3 = CC_WindowsNT3,
		Windows95 = CC_Windows95,
		WindowsNT4 = CC_WindowsNT4,
		Windows98 = CC_Windows98,
		WindowsME = CC_WindowsME,
		Windows2000 = CC_Windows2000,
		WindowsXP = CC_WindowsXP,
		WindowsServer2003 = CC_WindowsServer2003,
		WindowsHomeServer = CC_WindowsHomeServer,
		WindowsServer2003R2 = CC_WindowsServer2003R2,
		WindowsVista = CC_WindowsVista,
		WindowsServer2008 = CC_WindowsServer2008,
		WindowsServer2008R2 = CC_WindowsServer2008R2,
		Windows7 = CC_Windows7,
		WindowsServer2012 = CC_WindowsServer2012,
		Windows8 = CC_Windows8,
		WindowsServer2012R2 = CC_WindowsServer2012R2,
		Windows8_1 = CC_Windows8_1,
		WindowsNext = CC_WindowsNext,
	};
}WindowsVersion;

// Note: A Special Thanks to Mr. Warren Ward for his Sept. 1998 CUJ article:
// "Stream Encryption" Copyright (c) 1998 by Warren Ward
/////////////////////////////////////////////////////////////////////////////
// ARACrypt window

class ARACrypt
{
// Construction
public:
	ARACrypt();

// Attributes
public:

// Operations
public:
	virtual void TransformString(char* csTarget, int len);
	virtual void SetKey(const char* key, int keylen);

protected:
	virtual void TransformChar(unsigned char& csTarget);

// Implementation
public:
	virtual ~ARACrypt();

// Attributes
protected:
    unsigned long           m_LFSR_A;
    unsigned long           m_LFSR_B;
    unsigned long           m_LFSR_C;
    const unsigned long     m_Mask_A;
    const unsigned long     m_Mask_B;
    const unsigned long     m_Mask_C;
    const unsigned long     m_Rot0_A;
    const unsigned long     m_Rot0_B;
    const unsigned long     m_Rot0_C;
    const unsigned long     m_Rot1_A;
    const unsigned long     m_Rot1_B;
    const unsigned long     m_Rot1_C;
};

class Util : public Object
{
protected:
	// protected constants
	//static TCHAR _tcStartupPathOnly[_MAX_PATH + 1]; //endwith /
	//static TCHAR _tcStartupPathFull[_MAX_PATH + 1]; //with filename
	TCHAR *mBuf;
	int mLen;
	//static const TCHAR *mselDirProc_title;
	//static const TCHAR *mselDirProc_location;

public:
	// constructors
	Util();
	// destructor
	virtual ~Util();

protected:
	//for selectDir to set title&startpath(not thread safe)
	//static int __stdcall selectDirProc_(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData);
	//for readResTypes(not thread safe)
	//static BOOL __stdcall EnumTypesFunc(HANDLE hModule, LPTSTR lpType, LONG lParam);
	//for readResNames(not thread safe)
	//static BOOL __stdcall EnumNamesFunc(HANDLE hModule, LPCTSTR lpType, LPTSTR lpName, LONG lParam);
public:
	//========================================
	//pagecode,GetCurrentLocaleCodePage
	static int GetCurrentCodePage(UINT nFlage = LOCALE_USER_DEFAULT);
	//if withSISO3166CTRYNAME,return like zh-CN or ja-JP or zh-TW.
	static cc::Str GetCurrentLocaleName(bool withSISO3166CTRYNAME, UINT nFlage = LOCALE_USER_DEFAULT);
	//now is used at Config for while cannot found lang file to GetCurrentLocaleName,
	//then use pagecode to get spetial SimilarLocalName as lang file.
	//length of localeName should <= CC_LCALNAME_MAX_LENGTH
	static bool SetPageCodeToLocalName(int pageCode, const TCHAR* localeName);
	//if not found lang for current LCID,then find similar one use ANSI code page.NULL for not found
	static const TCHAR* GetSimilarLocalName();
	static int codeASC2UNI(wchar_t **desNeedFree, const char *src, LangCode::Type fromLangCode);
	static int codeUNI2ASC(char **desNeedFree, LangCode::Type fromLangCode, const wchar_t *src);
	static int codeB64Encode(const char* input, unsigned int data_size, char** output);
	static int codeB64Decode(const char* input, unsigned int data_size, char** output);
	static void codeB64_ToPost(char* input, unsigned int data_size);
	static void codeB64_FromPost(char* input, unsigned int data_size);
	//notice that while encode '+',then decode is ' ', !!!NOT SAME to orignal!!!
	static int codeURLEncode(const char* input, unsigned int data_size, char** output);
	static int codeURLDecode(const char* input, unsigned int data_size, char** output);

	//it is better to use at least 16-char of key. also notice that loop count:minloop + 16/keylen + 52/len
	static void teaEnCrypt(char* v, int len, const char* key, int keylen, int minloop = 3);
	static void teaEnCrypt(const char* inbuf, int len, const char* key, int keylen, int minloop, char** outbuf);
	static void teaDeCrypt(char* v, int len, const char* key, int keylen, int minloop = 3);
	static void teaDeCrypt(const char* inbuf, int len, const char* key, int keylen, int minloop, char** outbuf);
	static void teaEnCrypt2(char* v, int len, const char* key, int keylen, int minloop = 3);
	static void teaDeCrypt2(char* v, int len, const char* key, int keylen, int minloop = 3);

	//used only for treate myself's regkey
	//first is crypto,more again then decrypto
	static void araCrype(char* inoutbuf, int len, const char* key, int keylen);
	static void araCrype(const char* inbuf, int len, const char* key, int keylen, char** outbuf);

	//inbuf should be LangCode::UTF8, simple encode fixed string
	static cc::Str EnCryptEnCode(const char* inbuf, int len = -1, const char* key = NULL, int keylen = -1, int minloop = 4, LangCode::Type toLangCode = LangCode::TCHAR);
	static cc::Str DeCryptDeCode(const char* inbuf, int len = -1, const char* key = NULL, int keylen = -1, int minloop = 4, LangCode::Type toLangCode = LangCode::TCHAR, LangCode::Type orgLangCode = LangCode::UTF8);

	static int GetHtmlTag(const char* buf, int from, const char* start, int nStartLen, const char* end, int nEndLen, cc::Str& sValue, const char* startend = NULL, int nStartEndLen = -1, LangCode::Type fromLangCode = LangCode::SYSTEM, bool IgnoreCase = true);
	static int GetHtmlTag(const wchar_t* buf, int from, const wchar_t* start, int nStartLen, const wchar_t* end, int nEndLen, cc::Str& sValue, const wchar_t* startend = NULL, int nStartEndLen = -1, bool IgnoreCase = true);

	//========================================
	//file&dir
	//end with "\"
	static Str getSpecialFolderPath(int nCsidl, const TCHAR* tcDefaultPath = NULL);
	static Str getCurrentDirectory();
	//return fullpath with filename of current runned exe
	static Str getStartupPath(bool isNoExeFileName = false, HMODULE hModule = NULL);
	static Str getStartupRelatePath(const TCHAR *fullpath, HMODULE hModule = NULL);
	//static Str getStartupPathForSave(bool isNoExeFileName = false, HMODULE hModule = NULL);
	static Str getUserLocalAppDataDir(bool isAddStartupPathLastSubdir, HMODULE hModule);
	//end with "\"
	static Str getTempPath();
	//when retu,the temp file is created!!
	static Str getTempFileName(TCHAR *prefix);
	//static Str getRelatePath(const TCHAR *fullpath, const TCHAR *parentpath = NULL);
	static Str getFullPath(const TCHAR *fullpath);
	static Str getParentDirName(const TCHAR *fullpath);
	static Str getLastSubDirName(const TCHAR *fullpath);
	static Str getFilePath(const TCHAR *fullpath);
	static Str getFileName(const TCHAR *fullpath);
	static Str getFileNameNoExt(const TCHAR *fullpath);
	static Str getFileExt(const TCHAR *fullpath);
	static bool fExist(const TCHAR *fullpath);
	static DWORD fSize(const TCHAR *fullpath);
	//return the byte of read(TCHAR is as 2),buf can be null and if buflen is -1,then read all
	static int fRead(void** buf, const TCHAR *fullpath, int readLen = -1);
	//return the byte of read(TCHAR is as 2)
	static int fRead(Str &buf, const TCHAR *fullpath, LangCode::Type fileLangCode = LangCode::SYSTEM, int readLen = -1);
	static bool fWrite(const void* buf, DWORD len, LangCode::Type langCode, const TCHAR *fullpath, bool isAppend = false, bool isHeadBOM = true);
	static bool fWrite(const Str &buf, const TCHAR *fullpath, bool isAppend = false);
	static bool fAppend(Str &buf, const TCHAR *fullpath);
	static bool fCopy(const TCHAR *fromFullpath, const TCHAR *toFullpath, bool bFailIfExists = false);
	static bool fMove(const TCHAR *fromFullpath, const TCHAR *toFullpath, bool bFailIfExists = false);
	static bool fDel(const TCHAR *fullpath);
	static bool fReadonly(const TCHAR *fullpath, bool isReadonly = true);
	static bool fFindFirst(const TCHAR *fullpath, DWORD *dwFileAttributes, FILETIME *ftLastWriteTime, SYSTEMTIME *stLastWriteTime, DWORD *nFileSizeHigh, DWORD *nFileSizeLow);
	static bool dExist(const TCHAR *fullpath);
	static bool dCopy(const TCHAR *fromFullpath, const TCHAR *toFullpath);
	static bool dMove(const TCHAR *fromFullpath, const TCHAR *toFullpath);
	static bool dDel(const TCHAR *fullpath, bool isDelSubFilesDirs);
	static bool dCreate(const TCHAR *fullpath);
	static bool dCreateAll(const TCHAR *fullpath);
	//seach what you given,if end with "\\",then get nothing(must with "*.*" or others)
	static int getFiles(cc::List<cc::Str> &arrFullName, const TCHAR *path_with_wildcard, bool isGetOnlyFileName = false);
	//seach what you given,if end with "\\",then only return this path if exist(must with "*.*" or others)
	static int getDirs(cc::List<cc::Str> &arrFullName, const TCHAR *path_with_wildcard, bool isGetOnlyFileName = false);
	static cc::Str selectOpenFile(const TCHAR *location, const TCHAR *filter = _T("All Files (*.*)\0*.*\0"), const TCHAR *defaultExt = NULL, HWND parent = NULL, const TCHAR *caption = NULL, DWORD dwFlags = 0, bool isMulti = false, cc::List<cc::Str>* arrMultiFiles = NULL);
	static cc::Str selectSaveAs(const TCHAR *location, const TCHAR *filter = _T("All Files (*.*)\0*.*\0"), const TCHAR *defaultExt = NULL, HWND parent = NULL, const TCHAR *caption = NULL);
	static cc::Str selectDir(const TCHAR *location, HWND parent = NULL, const TCHAR *caption = NULL, bool isEditBox = true);
	static HFONT selectFont(HFONT hfInitial = NULL, DWORD dwFlags = 0, HWND parent = NULL);
	static HFONT selectFont(HFONT hfInitial, COLORREF& corInit, DWORD dwFlags = 0, HWND parent = NULL);
	static void SetCustColors(int nInd, COLORREF color);
	static COLORREF GetCustColors(int nInd);
	static bool selectColor(COLORREF& color, bool isInitColor = false, COLORREF rgbCustom[16] = NULL, DWORD dwFlags = -1, HWND parent = NULL);
	static PRINTDLG selectPrint(HWND parent = NULL, HINSTANCE hInstance = NULL);
	//static cc::Map<TCHAR*, cc::Str> selectOpenFiles(const TCHAR *location, const TCHAR *filter = _T("All Files (*.*)\0*.*\0"), HWND parent = NULL, const TCHAR *caption = NULL);
	//static cc::Map<TCHAR*, cc::Str> selectDirs(const TCHAR *location, const TCHAR *filter = _T("All Files (*.*)\0*.*\0"), HWND parent = NULL, const TCHAR *caption = NULL);
	static void openExplorerSelectedFile(const cc::Str fullpath, HWND hWnd = NULL);

	static cc::Str RunCmd(const TCHAR *sCmd, bool isShow = false, bool isNowait = false);
	static cc::Str GetEnv(const TCHAR *varname);

	//split commane line, if isGetExeFileName then first(0) is exe file name with full path,next is params
	static int GetCommandLine(cc::List<cc::Str> &arrCommands);
	static int GetDragFiles(HDROP hdrop, cc::List<cc::Str> &arrCommands);

	//if error then cc::Str.IsNULL()
	static cc::Str GetClipboardText(HWND m_hWnd = NULL);
	static bool GetClipboardText(cc::Str& str, HWND m_hWnd = NULL);
	static bool SetClipboardText(const char* str, HWND m_hWnd = NULL);
	static bool SetClipboardText(const wchar_t* str, HWND m_hWnd = NULL);
	static bool SetClipboardText(const cc::Str& str, HWND m_hWnd = NULL);
	//need DeleteObject(hbmp) out of this
	static bool SetClipboardBitmap(HBITMAP bmp, HWND m_hWnd = NULL);
	static HBITMAP GetClipboardBitmap(HWND m_hWnd = NULL);

	//need DeleteObject(hbmp) out of this
	static HBITMAP CopyBitmap(HBITMAP hSrcBitmap);
	static HBITMAP ResizeBmp(HBITMAP hBmpOld, int cx, int cy, bool isBitsFromScreenDC = false, int bmBitsPixel = -1);
	static HBITMAP ResizeBmp(HBITMAP hBmpOld, int cx, int cy, int fromX, int fromY, int srcW, int srcH, bool isBitsFromScreenDC = false, int bmBitsPixel = -1);

	//========================================
	//ini
	static int treateIni(cc::Map<cc::Str, cc::Str> &arrIni, const Str &sBuf, bool turnToTCHAR = false, bool loadAllLine = false);
	static int readIni(cc::Map<cc::Str, cc::Str> &arrIni, const TCHAR *fullpath, LangCode::Type fileLangCode = LangCode::SYSTEM, bool turnToTCHAR = false, bool loadAllLine = false);
	static int readIni(cc::Map<cc::Str, cc::Str> &arrIni, const Str &fullpath, LangCode::Type fileLangCode = LangCode::SYSTEM, bool turnToTCHAR = false, bool loadAllLine = false);
	static int writeIniToString(const cc::Map<cc::Str, cc::Str> &arrIni, Str &sTxt);
	static int writeIni(const cc::Map<cc::Str, cc::Str> &arrIni, const TCHAR *fullpath, LangCode::Type fileLangCode = LangCode::SYSTEM);
	static int writeIni(const cc::Map<cc::Str, cc::Str> &arrIni, const Str &fullpath, LangCode::Type fileLangCode = LangCode::SYSTEM);
	static cc::Str readOneIni(const TCHAR* section, const TCHAR* key, const Str &sBuf, bool turnToTCHAR = false);
	//if readLen=-1,then read all
	static cc::Str readOneIni(const TCHAR* section, const TCHAR* key, const TCHAR *fullpath, LangCode::Type fileLangCode = LangCode::SYSTEM, bool turnToTCHAR = false, int readLen = -1);

	//========================================
	//res
	// The name may be a string or an unsigned decimal integer, if integer,map's valus is "#"
	static int readResTypes(cc::Map<cc::Str, cc::Str> &arrRes, HMODULE hRes = NULL);
	static int readResNames(cc::Map<cc::Str, cc::Str> &arrRes, const TCHAR *lpType, HMODULE hRes = NULL);
	//return length of file
	static int readRes(cc::Str& sTxt, const TCHAR *resName, const TCHAR *resType, LangCode::Type fileLangCode = LangCode::SYSTEM);
	static int readRes(cc::Str& sTxt, unsigned int nResID, const TCHAR *resType, LangCode::Type fileLangCode = LangCode::SYSTEM);

	static bool savePicture(IPicture* pPicture, const cc::Str& sFilePath);
	static IPicture* readPicture(HGLOBAL hGlobal, DWORD dwFileSize);
	static IPicture* readPicture(const TCHAR* szFile);
	static IPicture* readPicture(UINT nResID, const TCHAR *resType);
	static HBITMAP readBitmap(const TCHAR* szFile);
	static HBITMAP readBitmap(const char* sBuf, int bufLen);
	static bool saveBitmap(const TCHAR* szFile, HBITMAP hBitmap);
	/*
	short sPixType = GetType();
	switch(sPixType) {
	case PICTYPE_UNINITIALIZED: strType = _T("Uninitialized"); break;
	case PICTYPE_NONE: strType = _T("None"); break;
	case PICTYPE_BITMAP: strType = _T("Windows 3.x Bitmap (BMP)"); break;
	case PICTYPE_METAFILE: strType = _T("Windows Metafile (WMF)"); break;
	case PICTYPE_ICON: strType = _T("Windows Icon (ICO)"); break;
	case PICTYPE_ENHMETAFILE: strType = _T("Enhanced Metafile (EMF)"); break;
	*/
	static short readPictureType(IPicture* pPicture);
	static void readPictureSize(IPicture* pPicture, SIZE& size);
	//if sizeW or sizeH < 0 then orignal size; if corBack=CLR_INVALID then not clear back
	//static HBITMAP IPicture2Bmp(IPicture* pPicture);

	//========================================MsgBox
	//#define MB_OK                       0x00000000L
	//#define MB_OKCANCEL                 0x00000001L
	//#define MB_ABORTRETRYIGNORE         0x00000002L
	//#define MB_YESNOCANCEL              0x00000003L
	//#define MB_YESNO                    0x00000004L
	//#define MB_RETRYCANCEL              0x00000005L
	//#define MB_ICONHAND                 0x00000010L
	//#define MB_ICONQUESTION             0x00000020L
	//#define MB_ICONEXCLAMATION          0x00000030L
	//#define MB_ICONASTERISK             0x00000040L
	//#if(WINVER >= 0x0400)
	//#define MB_USERICON                 0x00000080L
	//#define MB_ICONWARNING              MB_ICONEXCLAMATION
	//#define MB_ICONERROR                MB_ICONHAND
	//#endif /* WINVER >= 0x0400 */
	//#define MB_ICONINFORMATION          MB_ICONASTERISK
	//#define MB_ICONSTOP                 MB_ICONHAND
	//#define MB_DEFBUTTON1               0x00000000L
	//#define MB_DEFBUTTON2               0x00000100L
	//#define MB_DEFBUTTON3               0x00000200L
	//#if(WINVER >= 0x0400)
	//#define MB_DEFBUTTON4               0x00000300L
	//#endif /* WINVER >= 0x0400 */
	//#define MB_APPLMODAL                0x00000000L
	//#define MB_SYSTEMMODAL              0x00001000L
	//#define MB_TASKMODAL                0x00002000L
	//#if(WINVER >= 0x0400)
	//#define MB_HELP                     0x00004000L // Help Button
	//#endif /* WINVER >= 0x0400 */
	//#define MB_NOFOCUS                  0x00008000L
	//#define MB_SETFOREGROUND            0x00010000L
	//#define MB_DEFAULT_DESKTOP_ONLY     0x00020000L
	//#if(WINVER >= 0x0400)
	//#define MB_TOPMOST                  0x00040000L
	//#define MB_RIGHT                    0x00080000L
	//#define MB_RTLREADING               0x00100000L
	//#endif
	//IDABORT	Abort button was selected.
	//IDCANCEL	Cancel button was selected.
	//IDCONTINUE	Continue button was selected.
	//IDIGNORE	Ignore button was selected.
	//IDNO	No button was selected.
	//IDOK	OK button was selected.
	//IDRETRY	Retry button was selected.
	//IDTRYAGAIN	Try Again button was selected.
	//IDYES	Yes button was selected.
	//::MessageBox(m_hWnd, (const TCHAR*)strAlreadyRunMsg.GetBuf(), TCHAR_LANG("Lang.S_MsgBox_Title_Critical"), MB_OK | MB_ICONSTOP);
	static int MsgBox(HWND hWnd, const TCHAR *msg, const TCHAR *title = NULL, DWORD nButton = 0, DWORD nResourceID = 0);
	static bool PlaySound(LPCWSTR pszSound, HMODULE hmod, DWORD fdwSound);

	//========================================getHTTPPage
	//static Str& getHTTPPage(const TCHAR *sURL, LangCode::Type pageLangCode = LangCode::SYSTEM);

	static WindowsVersion::Type GetWindowsVersion(int* is64 = NULL);
	static cc::Str GetWindowsVersionName(WindowsVersion::Type ver, int is64 = -1);
	static bool GetIsNT();

	static int GetTransparent(HWND hWnd);
	static void SetTransparent(HWND hWnd, int nParcent);
	static void AdjustWindowRect(int& left, int& top, int& width, int& height, int minWidth = 0, int minHeight = 0, int maxWidth = 0, int maxHeight = 0);
	static void FourceWindowTop(HWND hWnd);
	//find running menu's window hwnd
	static HWND Util::GetTopMenuHwnd();
	//return pid
	//static DWORD GetProcessByName(const TCHAR* name);

	//sMidOfDayTime is between day and time,if space then[2007/03/19 16:31],if NULL,then no time like[2007/03/19]
	//static void GetCurrentTime(const TCHAR* sPre, const TCHAR* sExt, const TCHAR* sMidOfDay, const TCHAR* sMidOfDayTime, const TCHAR* sMidOfTime, int nMini);

	//date
	//if split="/" then return "2009/03/05"
	static cc::Str Util::GetToday(const TCHAR* splitDate = _T(""), bool isTime = false);
	//return YYYY*10000 + MM*100 + DD
	static int GetTodayInt();
	//only treate 8byte(YYYYMMDD) or 10byte(YYYY?MM?DD) to YYYY*10000 + MM*100 + DD,if not that or is not valid date then return -1
	static int Util::GetDateInt(const cc::Str& sDate);
	//nDate is YYYY*10000 + MM*100 + DD,and nAddY or nAddM or nAddD can < 0
	static int Util::GetDateAdd(int nDate, int nAddY, int nAddM, int nAddD, int nAddHour = 0, int nAddMin = 0, int nAddSec = 0);

	//return newst(biggest) of nBios and nFile.if nFile=-1,then not get from system.dat
	static int Util::GetSystemDate(int& nBios, int& nFile);
	//return LastWriteTime
	static FILETIME Util::GetFiletime(const TCHAR *fullpath, bool isToLocal = true);
	//set for LastWriteTime,LastAccessTime,CreationTime
	static bool setFileTime(const TCHAR *fullpath, FILETIME& creationTime, FILETIME& lastAccessTime, FILETIME& lastWriteTime);
	static time_t Util::Filetime2Time(const FILETIME& ft);
	static int Util::CompareFiletime(const FILETIME &a, const FILETIME &b);
	//static bool setNormalAttributes(const TCHAR *fullpath);

	static int WMI_Query(const char* sql, const wchar_t* field, cc::Str &sResult);
	//return all processors split with |(this may spent 4seconds)
	static int WMI_ProcessorId(cc::Str &sResult);
	//return all disks split with |
	static int WMI_DiskSerialNumber(int nDriver, cc::Str &sResult);
	static int WMI_BIOSSerialNumber(cc::Str &sResult);
	static int WMI_BoardSerialNumber(cc::Str &sResult);
	//return all mac address split with | for Physical and not Wireless or Virtual
	static int WMI_NetworkMac(cc::Str &sResult, bool noWireless);
	//return all ip address split with | for Physical and not Wireless or Virtual
	static int WMI_NetworkIp(cc::Str &sResult, bool noWireless);

	static cc::Str Util::GetCpuId(bool isFullInfo = false);
	//hard disk serialnumber
	static bool GetDriveSerialNumber(int nDriver, cc::Str& strSerialNumber);
	//Physical Drive 0
	static cc::Str GetDriveSerialNumber();
	static cc::Str GetDiskSize(int nDriver, bool isBytes);
	static bool GetDriveSize(const TCHAR* sDriver, bool isBytes, cc::Str& sTotal, cc::Str& sAvailable);
	static DWORDLONG GetPhysicalMemory();

	static int GetRandom(int min, int max, bool init_srand = false);
	static cc::Str GetRandomString(cc::Str sChars, int nMinLen, int nMaxLen);

	static bool SetIEProxy(wchar_t * proxyAdressStr, wchar_t* byPass = NULL, wchar_t * connNameStr = NULL);
	static bool SetIEProxyDirect(wchar_t* connectionNameStr = NULL);
#define CC_GETIEPROXY_PROXY_SERVER INTERNET_PER_CONN_PROXY_SERVER
#define CC_GETIEPROXY_PROXY_BYPASS INTERNET_PER_CONN_PROXY_BYPASS
	static bool GetIEProxy(int& nFlag, cc::Str& sProxy, int& nPort, cc::Str& sByPass, wchar_t * connNameStr = NULL);
};

#define CC_FLAG_KEYARR_START				0x0001
#define CC_FLAG_KEYARR_DIRTY				CC_FLAG_KEYARR_START
#define CC_FLAG_KEYARR_RETURNKEY			CC_FLAG_KEYARR_START <<1
#define CC_FLAG_KEYARR_IGNORECASE			CC_FLAG_KEYARR_START <<2
#define CC_FLAG_KEYARR_LOGNOTFOUNT			CC_FLAG_KEYARR_START <<3
//this class si array of Str,key is IgnoreCase&Distinct
//default no CC_FLAG_KEYARR_IGNORECASE and no CC_FLAG_KEYARR_RETURNKEY
class KeyArr : public Object
{
public:
	KeyArr(bool isReturnKeyIfNoFound = false, bool isIgnorecase = false);
	virtual ~KeyArr();

	void Clear();
	//clear one pare
	void Clear(const TCHAR* keyStartWith);
	bool Remove(const TCHAR* key);
	bool Remove(int nIndex);
	void Add(const KeyArr& keyArr, const TCHAR* keyStartWith = NULL, const TCHAR* preKeyTurnTo = NULL);
	//cannot be self!
	void Add(const cc::Map<cc::Str, cc::Str>& arrIni, const TCHAR* keyStartWith = NULL, const TCHAR* preKeyTurnTo = NULL);
	int Add(const TCHAR* keyFile, LangCode::Type fileLangCode, bool turnToTCHAR);
	//void Save(const TCHAR* keyFile, LangCode::Type fileLangCode = LangCode::SYSTEM, bool append = false, bool turnToTCHAR = false);
	void Save(const TCHAR* keyFile, LangCode::Type fileLangCode, bool isForce);
	void SaveToString(cc::Str& sTxt);
	int AddFromString(cc::Str& sTxt, bool turnToTCHAR);
	int Add(unsigned int nResID, const TCHAR *resType, LangCode::Type fileLangCode, bool turnToTCHAR);
	int Add(const TCHAR *resName, const TCHAR *resType, LangCode::Type fileLangCode, bool turnToTCHAR);
	const cc::Map<cc::Str, cc::Str>* GetArr();
	//string iconPath = System.Configuration.ConfigurationSettings.AppSettings["Form1.Icon"];
	bool Exist(const TCHAR* key);
	bool Exist(const cc::Str& key);
	//if exist,then set value
	bool Exist(const TCHAR* key, cc::Str& value);
	bool Exist(const cc::Str& key, cc::Str& value);
	cc::Str GetKey(int index);
	int GetIndex(const TCHAR* key);
	int GetIndex(const cc::Str& key);
	cc::Str GetAt(int index);
	//even value=NULL,not del this key
	void SetAt(int index, const TCHAR* value);
	//even value=NULL,not del this key
	void SetAt(int index, const cc::Str& value);
	//key:like:Main.title,if not found,return cc::Str(key).Append(_T("[NOFOUND]")) of _bIsReturnKey or cc::Str()
	cc::Str Get(const TCHAR* key);
	cc::Str Get(const cc::Str& key);
	cc::Str Get(const TCHAR* key, cc::Str defaultValue);
	cc::Str Get(const cc::Str& key, cc::Str defaultValue);
	DWORD Get(const TCHAR* key, DWORD dwValue);
	DWORD Get(const cc::Str& key, DWORD dwValue);
	bool GetBool(const TCHAR* key, bool bValue);
	bool GetBool(const cc::Str& key, bool bValue);
	//if value==NULL,then del this key
	void Set(const TCHAR* key, const TCHAR* value);
	//if value==NULL,then del this key
	void Set(const cc::Str& key, const cc::Str& value);
	void Set(const TCHAR* key, const cc::Str& value);
	void Set(const cc::Str& key, const TCHAR* value);
	void Set(const TCHAR* key, DWORD dwValue);
	void Set(const cc::Str& key, DWORD dwValue);
	void SetBool(const TCHAR* key, bool bValue);
	void SetBool(const cc::Str& key, bool bValue);
	cc::Str GetFormat(const TCHAR* key, ...);
	cc::Str GetFormatDefaultValue(cc::Str defaultValue, const TCHAR* key, ...);
	void SetIsDirty(bool isDirty);
	bool GetIsDirty();
	bool GetIsEmpty();
	void SetReturnNoFound(bool isReturnKey);
	bool GetIsIgnoreCase();
	void SetIsIgnoreCase(bool isIgnoreCase);
	bool GetIsLogNotFound();
	void SetIsLogNotFound(bool isLogNotFound);
	int GetSize() const;

	void Dump(const TCHAR* info = NULL);

protected:
	// protected constants
	//bool _bIsDirty;
	//bool _bIsReturnKey;
	//bool _bIsIgnoreCase;
	DWORD _Flags;
	cc::Map<cc::Str, cc::Str>* _KeyArr;
};

enum {
    null_type,
    boolean_type,
    number_type,
    string_type,
    array_type,
    object_type
};

#define JSON_TYPE_NULL 1
#define JSON_TYPE_BOOL 2
#define JSON_TYPE_NUMBER 3
#define JSON_TYPE_STRING 4
#define JSON_TYPE_ARRAY 5
#define JSON_TYPE_OBJECT 6

#define JSON_ERR_NO_ERROR 0
#define JSON_ERR_NO_PARSE -100
#define JSON_ERR_NO_OBJ_START -101
#define JSON_ERR_NO_NORMAL_END -102
#define JSON_ERR_U_RANGE_ERR -103
#define JSON_ERR_U_LEN_ERR -104
#define JSON_ERR_U_NEXT_ERR1 -105
#define JSON_ERR_U_NEXT_ERR2 -106
#define JSON_ERR_U_NEXT_ERR3 -107
#define JSON_ERR_U_SURROGATE_ERR1 -108
#define JSON_ERR_U_SURROGATE_ERR2 -109
#define JSON_ERR_ESCAPE_ERR -110
#define JSON_ERR_NO_NUM -111
#define JSON_ERR_NO_KEY_START -112
#define JSON_ERR_NO_KEY_SPLIT -113
#define JSON_ERR_NO_OBJ -114

class Json : public Object
{
public:
	Json(LangCode::Type saveLangCode = LangCode::UTF8);
	virtual ~Json();

	//not init with any string
	bool IsEmpty();
	//init with [] or {}
	bool IsNull();
	bool IsBool();
	bool IsNumber();
	bool IsString();
	//init []
	bool IsArray();
	//init {}
	bool IsObject();
	cc::Str GetString();
	bool GetBool();
	int GetNumber();
	LangCode::Type GetLangCode();

	//cc::Json* operator[](const char* key);
	int GetSize();
	const cc::Str GetKey(int index);
	cc::Json* Get(const char* key, LangCode::Type fromLangCode = LangCode::UTF8);
	cc::Json* GetAt(int index);
	void Add(const char* key, cc::Json* json, LangCode::Type fromLangCode = LangCode::UTF8);
	cc::Json* GetDeepSub(const char* key, const char* splitChar = ".", LangCode::Type fromLangCode = LangCode::UTF8);

	//virtual cc::Str ToString() const;
	int Parse(const char* s, LangCode::Type fromLangCode = LangCode::UTF8);
	void Clear();
	//bool Remove(const char* key);
	//bool Remove(int nIndex);
	virtual cc::Str ToJSon(LangCode::Type toLangCode = LangCode::UTF8, const char* crln = NULL);

protected:
	DWORD _Flags;
	//global parse string
	const char* m_s;
	//global parse pos
	unsigned int m_pos;
	//global parse len
	unsigned int m_len;
	unsigned int m_ArrInd;
	LangCode::Type m_FromLang;

	//save int value
	int m_nValue;
	//save string value
	cc::Str m_sValue;
	cc::Map<cc::Str, cc::Json*>* m_JsonArr;
	LangCode::Type _savedLang;

	virtual int Parse_String(cc::Str &out);
	virtual int Parse_Object(cc::Json &out);
	virtual char GetNextToken();
	virtual int Parse_Number(int &out, cc::Str &out2);
	virtual int ToJSon(LangCode::Type toLangCode, cc::Str& key, cc::Json &oJson, cc::Str& out, const char* crln, int index, int deep);
};

#define CC_CONFIG_LANGSECTION _T("Lang")
#define CC_CONFIG_LANGHEADDOT _T("Lang.")
#define CC_CONFIG_LANGNAME _T("LangName")
#define CC_CONFIG_LANGFILE _T("Main.Lang")
#define CC_CONFIG_LANGDEFAULT _T("<default>")
#define CC_CONFIG_LANGEXT _T(".ini")
#define CC_CONFIG_DEBUGMODE _T("Main.Debug")
#define CC_CONFIG_LANGTYPE _T("LANGFILE")
#define CC_CONFIG_LANGENG _T("en-US")

//for design
#define CC_CONFIG_DESIGNSECTION _T("Design")
#define CC_CONFIG_DESIGNHEADDOT _T("Design.")
#define CC_CONFIG_DESIGNMODE _T("Main.Design")
//#define CC_CONFIG_REGINI _T("CONFIG_STRING")
//#define CC_CONFIG_REGPWD "0okE73bUf4gUvM}We&:x"

//details for Locale name:http://msdn2.microsoft.com/en-us/library/ms776294.aspx
//LocalName of Table(ANSI code page->LocaleName) 's max length
#define CC_LCALNAME_MAX_LENGTH    12
#define CC_FLAG_CONFIG_START				0x0001
#define CC_FLAG_CONFIG_DEBUG				CC_FLAG_CONFIG_START
#define CC_FLAG_CONFIG_LODING				CC_FLAG_CONFIG_START <<1
#define CC_FLAG_CONFIG_AUTOSAVE				CC_FLAG_CONFIG_START <<2
#define CC_FLAG_CONFIG_SAVEREG				CC_FLAG_CONFIG_START <<3
#define CC_FLAG_CONFIG_DESIGN				CC_FLAG_CONFIG_START <<4

#define CC_CONFIG_BUFLEN 20
class ConfigConfig : public ObjectConfig
{
public:
	static TCHAR tcDefaultLang[CC_CONFIG_BUFLEN + 1];
	static TCHAR tcRegistyConfigKey[CC_CONFIG_BUFLEN + 1];
	static char tcRegistyConfigPWD[CC_CONFIG_BUFLEN + 1];
	static UINT nSaveRegCryptLoopCount;
};
class Config : public Object
{
public:
	Config();
	virtual ~Config();

	virtual KeyArr* GetIni();
	virtual KeyArr* GetLang();
	//auto load while get Get.if iniFile NULL then use GetModuleFileName
	bool LoadIni(const TCHAR* iniFile = NULL);
	//1. auto load while get GetLang.for custum need do this before any Get&GetLang.
	//2. if resLangType NULL then _T("LANGFILE"),if langPath NULL then _T("Lang\\")
	//3. if isLoadDefaultAlways,then first load DefaultLangRes,then plus others below.
	//3.1 if CC_CONFIG_LANGFILE is not set then get system's localname(with SISO3166CTRYNAME),
	//    first search file(if not isNotLoadFile) then res
	//3.2 if CC_CONFIG_LANGFILE is set then search it first file(if not isNotLoadFile),not found then res.
	//4. if not isLoadDefaultAlways and not found any,then load DefaultLangRes(usually en-US)
	bool LoadLang(unsigned int nDefaultLangResID, bool isLoadDefaultAlways = false, const TCHAR *resLangType = NULL, const TCHAR *langPath = NULL, bool isNotLoadFile = false);
	//while not found the lang same to system,then use tcDefaultLangResName
	bool LoadLang(const TCHAR *tcDefaultLangResName = NULL, bool isLoadDefaultAlways = false, const TCHAR *resLangType = NULL, const TCHAR *langPath = NULL, bool isNotLoadFile = false);
	const TCHAR* GetIniPath();
	const TCHAR* GetLangPath();
	bool GetIsDebug();
	void SetIsDebug(bool isDebug);
	bool GetIsDesign();
	void SetIsDesign(bool isDesign);
	bool SaveIni();
	//this should set before LoadIni&SaveIni,called like this:
	//cc::win::App::GetConfig()->SetIniLangCode(LangCode::TCHAR);
	void SetIniLangCode(LangCode::Type iniLangCode);
	//if value&isToReg,then need tcRegKey like _T("SOFTWARE\\uuware\\uuSoft"),cc::Log's ini is not save to reg
	void SetIsAutoSave(bool value, bool isToReg, const TCHAR *tcRegKey);

	void Dump(const TCHAR* info = NULL);

protected:
	// protected constants
	//bool _bIsDebug;
	//bool _bIsLoding;
	//bool _bIsAutoSave;
	DWORD _Flags;
	TCHAR* _tcIniFile;
	TCHAR* _tcIniReg;
	TCHAR* _tcLangPath;
	KeyArr* _Ini;
	LangCode::Type _IniLangCode;
	KeyArr* _Lang;

	virtual bool PreDoLoadLang(const TCHAR *tcDefaultLangResName = NULL, bool isLoadDefaultAlways = false, const TCHAR *resLangType = NULL, const TCHAR *langPath = NULL, bool isNotLoadFile = false);
	virtual bool DoLoadLang(cc::Str langName, const TCHAR *resLangType, bool isNotLoadFile, bool isSaveStatusToIni);
};

#ifndef CC_Theme_H_
#define CC_Theme_H_
//#pragma once

#pragma warning(push, 3)
#include <uxtheme.h>
#if (NTDDI_VERSION >= NTDDI_LONGHORN)
#include <vssym32.h>
#else
#include <tmschema.h>
#endif    // (NTDDI_VERSION >= NTDDI_LONGHORN)

#define CC_FLAG_THEME_START					0x0001
#define CC_FLAG_THEME_Loaded				CC_FLAG_THEME_START
#define CC_FLAG_THEME_OpenThemeData			CC_FLAG_THEME_START <<1
#define CC_FLAG_THEME_CloseThemeData		CC_FLAG_THEME_START <<2
#define CC_FLAG_THEME_DrawThemeBackground	CC_FLAG_THEME_START <<3
#define CC_FLAG_THEME_DrawThemeEdge			CC_FLAG_THEME_START <<4
#define CC_FLAG_THEME_DrawThemeText			CC_FLAG_THEME_START <<5
#define CC_FLAG_THEME_EnableThemeDialogTexture	CC_FLAG_THEME_START <<6
class Theme : public Object
{
public:
	Theme(void);
	~Theme(void);
	static BOOL IsThemeActive(bool isRefresh = false);
	static BOOL IsAppThemed(bool isRefresh = false);
	static HTHEME OpenThemeData(HWND hwnd, LPCWSTR pszClassList);
	static HRESULT CloseThemeData(HTHEME hTheme);
	static HRESULT DrawThemeBackground(HTHEME hTheme, HDC hdc,
		int iPartId, int iStateId, const RECT *pRect, const RECT *pClipRect);
	static HRESULT DrawThemeEdge(HTHEME hTheme, HDC hdc, int iPartId, int iStateId,
		const RECT *pDestRect, UINT uEdge, UINT uFlags,   RECT *pContentRect);
	static HRESULT DrawThemeText(HTHEME hTheme, HDC hdc, int iPartId,
		int iStateId, LPCWSTR pszText, int iCharCount, DWORD dwTextFlags,
		DWORD dwTextFlags2, const RECT *pRect);
	static HRESULT EnableThemeDialogTexture(HWND hwnd, DWORD dwFlags);

protected:
	// protected constants
	static DWORD _Flags;
	static HMODULE m_hThemeDll;

	static void* GetProc(LPCSTR szProc);
	typedef HTHEME(__stdcall *PFNOPENTHEMEDATA)(HWND hwnd, LPCWSTR pszClassList);
	typedef HRESULT(__stdcall *PFNCLOSETHEMEDATA)(HTHEME hTheme);
	typedef BOOL (__stdcall *PFNISTHEMEACTIVE)();
	typedef BOOL(__stdcall *PFNISAPPTHEMED)();
	typedef HRESULT(__stdcall *PFNDRAWTHEMEBACKGROUND)(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, const RECT *pRect, const RECT *pClipRect);
	typedef HRESULT (__stdcall *PFNDRAWTHEMEEDGE)(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, 
		const RECT *pDestRect, UINT uEdge, UINT uFlags,   RECT *pContentRect);
	typedef HRESULT (__stdcall *PFNDRAWTHEMETEXT)(HTHEME hTheme, HDC hdc, int iPartId, 
		int iStateId, LPCWSTR pszText, int iCharCount, DWORD dwTextFlags, DWORD dwTextFlags2, const RECT *pRect);
	typedef HRESULT (__stdcall *PFNENABLETHEMEDIALOGTEXTURE)(HWND hwnd, DWORD dwFlags);
};
#endif //#ifndef CC_Theme_H_

} //namespace cc

#define GET_LANG(keyLang) CC_APP_GetLang()->Get(_T( keyLang ))
#define TCHAR_LANG(keyLang) (TCHAR*)CC_APP_GetLang()->Get(_T( keyLang )).GetBuf()
#define TCHAR_LANGDEF(keyLang, def) (TCHAR*)CC_APP_GetLang()->Get(_T( keyLang ), def).GetBuf()
#define TCHAR_LANGFMT1(keyLang, p1) (TCHAR*)CC_APP_GetLang()->GetFormat(_T( keyLang ), p1).GetBuf()
#define TCHAR_LANGFMT2(keyLang, p1, p2) (TCHAR*)CC_APP_GetLang()->GetFormat(_T( keyLang ), p1, p2).GetBuf()
#define TCHAR_LANGFMT3(keyLang, p1, p2, p3) (TCHAR*)CC_APP_GetLang()->GetFormat(_T( keyLang ), p1, p2, p3).GetBuf()

//below is implemented at cc.win.form.cpp
HINSTANCE CC_APP_GetInstance();
cc::Config* CC_APP_GetConfig();
cc::KeyArr* CC_APP_GetLang();
cc::KeyArr* CC_APP_GetIni();
void CC_APP_SetVarious(const TCHAR* key, void* value);
void* CC_APP_GetVarious(const TCHAR* key);

void CC_APP_DoEvents();
HWND CC_APP_GetMainFormHwnd();
HWND CC_APP_GetActiveFormHwnd();
bool CC_APP_GetDesignBarActive();
HWND CC_APP_GetDesignBarHwnd();

#endif //#ifndef CC_Util_H_

