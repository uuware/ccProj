// cc.Util.h
// cc.Util class
// Author: Shu.KK
// 2006-01-01
#include "cc.Util.h"
#include <limits.h>
//#include <intrin.h>
#include <comdef.h>
#include <wbemidl.h>
#include <algorithm>
//#include <atlconv.h>
#include <ntddndis.h>

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "comsuppw.lib")
#pragma comment(lib, "wbemuuid.lib")

#include <windows.h>
#include <wininet.h>
#pragma comment(lib,"wininet.lib")

using namespace cc;

#if _MSC_VER >=1400  // VC2005intrin.h
#include <intrin.h>   // Intrinsics
#endif

#if defined(_WIN64)
// 64. __cpuid?__cpuidexIntrinsics?
#else
#if _MSC_VER < 1400  // VS2010. VC2008 SP1__cpuidex
void __cpuidex(INT32 CPUInfo[4], INT32 InfoType, INT32 ECXValue)
{
    if(CPUInfo == NULL)
	{
		return;
	}
    _asm{
        // load. 
        mov edi, CPUInfo;   // ediCPUInfo
        mov eax, InfoType;
        mov ecx, ECXValue;
        // CPUID
        cpuid;
        // save. CPUInfo
        mov [edi], eax;
        mov [edi+4], ebx;
        mov [edi+8], ecx;
        mov [edi+12], edx;
    }
}
#endif  // #if _MSC_VER < 1600   // VS2010. VC2008 SP1__cpuidex

#if _MSC_VER < 1400  // VC2005__cpuid
void __cpuid(INT32 CPUInfo[4], INT32 InfoType)
{
    __cpuidex(CPUInfo, InfoType, 0);
}
#endif  // #if _MSC_VER < 1400   // VC2005__cpuid
#endif  // #if defined(_WIN64)

/*
//next is used by cc::Log
void cc::CC_getLogPathWithStartupFilename(TCHAR* buf, int buflen)
{
	int bit64 = 0;
	tagWindowsVersion::Type wv = cc::Util::GetWindowsVersion(&bit64);
	bool isDataPath = false;

	memset(buf, 0, buflen * sizeof(TCHAR));
	::GetModuleFileName(NULL, buf, buflen);
	cc::Str sStart = cc::Util::getStartupPath(false, NULL);
	if((int)wv >= (int)WindowsVersion::WindowsVista)
	{
		cc::Str sTmp = cc::Util::GetEnv("ProgramFiles");
		if(sTmp.GetLength() > 0 && sStart.StartsWith(sTmp))
		{
			sStart = sStart.Substring(sTmp.GetLength() + 1);
			isDataPath = true;
		}
		else
		{
			//sRet = cc::Util::getSpecialFolderPath(CSIDL_PROGRAM_FILES);
			sTmp = cc::Util::GetEnv("ProgramFiles(x86)");
			if(sTmp.GetLength() > 0 && sStart.StartsWith(sTmp))
			{
				sStart = sStart.Substring(sTmp.GetLength() + 1);
				isDataPath = true;
			}
		}

		if(isDataPath)
		{
			//from Windows 2000 and Windows Millennium Edition (Windows Me). 
			//nCsidl = CSIDL_LOCAL_APPDATA;
			//LOCALAPPDATA=C:\Users\user\AppData\Local
			sTmp = cc::Util::getSpecialFolderPath(CSIDL_LOCAL_APPDATA);
			if(sTmp.GetLength() < 1)
			{
				sTmp = cc::Util::GetEnv("LOCALAPPDATA");
			}
			if(sTmp.GetLength() > 0)
			{
				if(!sTmp.EndsWith(_T("\\")))
				{
					sTmp.Append(_T("\\"));
				}
				sStart = sTmp + sStart;
				cc::Str sDir = cc::Util::getParentDirName(sStart);
				cc::Util::dCreateAll(sDir);
				CC_TRACE((_T("Save Data Path:%s"), sStart));
			}
			else
			{
				//return startup path
				sStart = cc::Util::getStartupPath(false, NULL);
			}
		}
		else
		{
			//return startup path
		}
	}

	cc::Buf::Cpy(buf, buflen, sStart);
}

//get windows system
//http://msdn.microsoft.com/en-us/library/windows/desktop/ms724833(v=vs.85).aspx
WindowsVersion::Type Util::GetWindowsVersion(int* is64)
{
	static WindowsVersion::Type winType = (WindowsVersion::Type)0;
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
				winType = WindowsVersion::WindowsNT4;
			}
			else if(osvi.dwMajorVersion == 5L && osvi.dwMinorVersion == 0L)
			{
				winType = WindowsVersion::Windows2000;
			}
			else if(osvi.dwMajorVersion == 5L && osvi.dwMinorVersion == 1L)
			{
				winType = WindowsVersion::WindowsXP;
			}
			else if(osvi.dwMajorVersion == 5L && osvi.dwMinorVersion == 2L)
			{
				if(osvi.wSuiteMask & CC_VER_SUITE_WH_SERVER){
					winType = WindowsVersion::WindowsHomeServer;
				}
				else if(osvi.wProductType == VER_NT_WORKSTATION && si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
				{
					winType = WindowsVersion::WindowsXP; //Windows XP Professional x64 Edition
				}
				else if(GetSystemMetrics(CC_SM_SERVERR2) == 0){
					winType = WindowsVersion::WindowsServer2003;
				}
				else if(GetSystemMetrics(CC_SM_SERVERR2) != 0){
					winType = WindowsVersion::WindowsServer2003R2;
				}
			}
			else if(osvi.dwMajorVersion == 6L) // && osvi.dwMinorVersion == 0L
			{
				if(osvi.dwMinorVersion == 0L)
				{
					if(osvi.wProductType == VER_NT_WORKSTATION)
					{
						winType = WindowsVersion::WindowsVista;
					}
					else
					{
						winType = WindowsVersion::WindowsServer2008;
					}
				}
				else if(osvi.dwMinorVersion == 1L)
				{
					if(osvi.wProductType != VER_NT_WORKSTATION)
					{
						winType = WindowsVersion::WindowsServer2008R2; //Windows Server 2008 R2
					}
					else
					{
						winType = WindowsVersion::Windows7;
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
							winType = WindowsVersion::WindowsServer2012; //Windows Server 2012
						}
						else
						{
							winType = WindowsVersion::Windows8;
						}
					}
					else if(IsWindowsVersionChk(6, 3, -1) == TRUE)
					{
						osvi.dwMinorVersion = 3;
						if(osvi.wProductType != VER_NT_WORKSTATION)
						{
							winType = WindowsVersion::WindowsServer2012R2;
						}
						else
						{
							winType = WindowsVersion::Windows8_1;
						}
					}
					else
					{
						winType = WindowsVersion::WindowsNext;
					}
				}
			}
			else if(osvi.dwMajorVersion >= 7L)
			{
				winType = WindowsVersion::WindowsNext;
			}
			else
			{
				winType = WindowsVersion::WindowsNT3;
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

			CC_DEBUG(("winVer:%d, dwMajorVersion:%d, dwMinorVersion:%d, wSuiteMask:%d, wProcessor:%d, GetSystemMetrics(CC_SM_SERVERR2):%d, Bit64:%d, winType:%d",
				winVer, osvi.dwMajorVersion, osvi.dwMinorVersion, osvi.wSuiteMask,
				si.wProcessorArchitecture, (int)GetSystemMetrics(CC_SM_SERVERR2), winBit64, winType));
		}
		//i thins ccProc cannot run on Windows32s
		//else if(LOBYTE(LOWORD(winVer)) < 4)
		//{
		//	winType = WindowsVersion::Windows32s;
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
				winType = WindowsVersion::Windows98;
			}
			else if(osvi.dwMajorVersion == 4L && osvi.dwMinorVersion == 90L)
			{
				winType = WindowsVersion::WindowsME;
			}
			else
			{
				winType = WindowsVersion::Windows95;
			}
		}
	}

	return winType;
}


cc::Str Util::GetEnv(const char *varname)
{
	cc::Str ret;
#ifdef __STDC_SECURE_LIB__
	char *p = NULL;
	size_t len = 0;
	errno_t e = _dupenv_s(&p, &len, varname);
	if(!e)
	{
		ret.ValueOf(p);
	}
	if(p != NULL)
	{
		free(p);
		p = NULL;
	}
#else
	char* p = getenv(varname);
	ret.ValueOf(p);
#endif
	return ret;
}
*/

char* CC_StrCopy(char** sDec, const char* sSrc, int nLen)
{
	if(*sDec != NULL)
	{
		if(*sDec == sSrc)
		{
			//des is same to src
			return *sDec;
		}
		free(*sDec);
		*sDec = NULL;
	}
	if(sSrc != NULL)
	{
		if(nLen <= 0)
		{
			nLen = (int)strlen(sSrc);
		}
		*sDec = (char*)malloc(nLen + 1);
		//strncpy(*sDec, sSrc, nLen);
		cc::Buf::Cpy(*sDec, nLen + 1, sSrc);
		(*sDec)[nLen] = '\0';
	}
	return *sDec;
}

wchar_t* CC_StrCopy(wchar_t** sDec, const wchar_t* sSrc, int nLen)
{
	if(*sDec != NULL)
	{
		if(*sDec == sSrc)
		{
			//des is same to src
			return *sDec;
		}
		free(*sDec);
		*sDec = NULL;
	}
	if(sSrc != NULL)
	{
		if(nLen <= 0)
		{
			nLen = (int)wcslen(sSrc);
		}
		*sDec = (wchar_t*)malloc((nLen + 1) * sizeof(wchar_t));
		//wcsncpy(*sDec, sSrc, nLen);
		cc::Buf::Cpy(*sDec, nLen + 1, sSrc);
		(*sDec)[nLen] = L'\0';
	}
	return *sDec;
}

//===========================================================cc::Registry
///////////////////////////////////////////////////////////////////////////////
RegistryKey* Registry::GetClassesRoot()
{
	return new RegistryKey(RegistryHive::ClassesRoot);
}

RegistryKey* Registry::GetCurrentConfig()
{
	return new RegistryKey(RegistryHive::CurrentConfig);
}

RegistryKey* Registry::GetCurrentUser()
{
	return new RegistryKey(RegistryHive::CurrentUser);
}

RegistryKey* Registry::GetDynData()
{
	return new RegistryKey(RegistryHive::DynData);
}

RegistryKey* Registry::GetLocalMachine()
{
	return new RegistryKey(RegistryHive::LocalMachine);
}

RegistryKey* Registry::GetPerformanceData()
{
	return new RegistryKey(RegistryHive::PerformanceData);
}

RegistryKey* Registry::GetUsers()
{
	return new RegistryKey(RegistryHive::Users);
}
///////////////////////////////////////////////////////////////////////////////

//===========================================================cc::RegistryKey
///////////////////////////////////////////////////////////////////////////////
/*
%EXPORT_TAGS= (
    Func =>	[qw(		regConstant		regLastError
	AllowPriv		AbortSystemShutdown	InitiateSystemShutdown
	RegCloseKey		RegConnectRegistry	RegCreateKey
	RegCreateKeyEx		RegDeleteKey		RegDeleteValue
	RegEnumKey		RegEnumKeyEx		RegEnumValue
	RegFlushKey		RegGetKeySecurity	RegLoadKey
	RegNotifyChangeKeyValue	RegOpenKey		RegOpenKeyEx
	RegQueryInfoKey		RegQueryMultipleValues	RegQueryValue
	RegQueryValueEx		RegReplaceKey		RegRestoreKey
	RegSaveKey		RegSetKeySecurity	RegSetValue
	RegSetValueEx		RegUnLoadKey )],
    FuncA =>	[qw(
	AbortSystemShutdownA	InitiateSystemShutdownA
	RegConnectRegistryA	RegCreateKeyA		RegCreateKeyExA
	RegDeleteKeyA		RegDeleteValueA		RegEnumKeyA
	RegEnumKeyExA		RegEnumValueA		RegLoadKeyA
	RegOpenKeyA		RegOpenKeyExA		RegQueryInfoKeyA
	RegQueryMultipleValuesA	RegQueryValueA		RegQueryValueExA
	RegReplaceKeyA		RegRestoreKeyA		RegSaveKeyA
	RegSetValueA		RegSetValueExA		RegUnLoadKeyA )],
    FuncW =>	[qw(
	AbortSystemShutdownW	InitiateSystemShutdownW
	RegConnectRegistryW	RegCreateKeyW		RegCreateKeyExW
	RegDeleteKeyW		RegDeleteValueW		RegEnumKeyW
	RegEnumKeyExW		RegEnumValueW		RegLoadKeyW
	RegOpenKeyW		RegOpenKeyExW		RegQueryInfoKeyW
	RegQueryMultipleValuesW	RegQueryValueW		RegQueryValueExW
	RegReplaceKeyW		RegRestoreKeyW		RegSaveKeyW
	RegSetValueW		RegSetValueExW		RegUnLoadKeyW )],
    HKEY_ =>	[qw(
	HKEY_CLASSES_ROOT	HKEY_CURRENT_CONFIG	HKEY_CURRENT_USER
	HKEY_DYN_DATA		HKEY_LOCAL_MACHINE	HKEY_PERFORMANCE_DATA
	HKEY_USERS )],
    KEY_ =>	[qw(
	KEY_QUERY_VALUE		KEY_SET_VALUE		KEY_CREATE_SUB_KEY
	KEY_ENUMERATE_SUB_KEYS	KEY_NOTIFY		KEY_CREATE_LINK
	KEY_READ		KEY_WRITE		KEY_EXECUTE
	KEY_ALL_ACCESS),
	'KEY_DELETE',		# DELETE          (0x00010000L)
	'KEY_READ_CONTROL',	# READ_CONTROL    (0x00020000L)
	'KEY_WRITE_DAC',	# WRITE_DAC       (0x00040000L)
	'KEY_WRITE_OWNER',	# WRITE_OWNER     (0x00080000L)
	'KEY_SYNCHRONIZE',	# SYNCHRONIZE     (0x00100000L) (not used)
	],
    REG_ =>	[qw(
	REG_OPTION_RESERVED	REG_OPTION_NON_VOLATILE	REG_OPTION_VOLATILE
	REG_OPTION_CREATE_LINK	REG_OPTION_BACKUP_RESTORE
	REG_OPTION_OPEN_LINK	REG_LEGAL_OPTION	REG_CREATED_NEW_KEY
	REG_OPENED_EXISTING_KEY	REG_WHOLE_HIVE_VOLATILE	REG_REFRESH_HIVE
	REG_NO_LAZY_FLUSH	REG_NOTIFY_CHANGE_ATTRIBUTES
	REG_NOTIFY_CHANGE_NAME	REG_NOTIFY_CHANGE_LAST_SET
	REG_NOTIFY_CHANGE_SECURITY			REG_LEGAL_CHANGE_FILTER
	REG_NONE		REG_SZ			REG_EXPAND_SZ
	REG_BINARY		REG_DWORD		REG_DWORD_LITTLE_ENDIAN
	REG_DWORD_BIG_ENDIAN	REG_LINK		REG_MULTI_SZ
	REG_RESOURCE_LIST	REG_FULL_RESOURCE_DESCRIPTOR
	REG_RESOURCE_REQUIREMENTS_LIST )],
    SE_ =>	[qw(
	SE_ASSIGNPRIMARYTOKEN_NAME	SE_AUDIT_NAME
	SE_BACKUP_NAME			SE_CHANGE_NOTIFY_NAME
	SE_CREATE_PAGEFILE_NAME		SE_CREATE_PERMANENT_NAME
	SE_CREATE_TOKEN_NAME		SE_DEBUG_NAME
	SE_INCREASE_QUOTA_NAME		SE_INC_BASE_PRIORITY_NAME
	SE_LOAD_DRIVER_NAME		SE_LOCK_MEMORY_NAME
	SE_MACHINE_ACCOUNT_NAME		SE_PROF_SINGLE_PROCESS_NAME
	SE_REMOTE_SHUTDOWN_NAME		SE_RESTORE_NAME
	SE_SECURITY_NAME		SE_SHUTDOWN_NAME
	SE_SYSTEMTIME_NAME		SE_SYSTEM_ENVIRONMENT_NAME
	SE_SYSTEM_PROFILE_NAME		SE_TAKE_OWNERSHIP_NAME
	SE_TCB_NAME			SE_UNSOLICITED_INPUT_NAME )],
);
*/
// constructors/destructor
RegistryKey::RegistryKey(RegistryHive::Type hKeyRoot)
{
	_hKeyRoot = hKeyRoot;
	_hOpenedKey = NULL;
}

RegistryKey::~RegistryKey()
{
	Close();
}
// constructors/destructor

void RegistryKey::Close()
{
	if(_hOpenedKey != NULL)
	{
		::RegCloseKey(_hOpenedKey);
		_hOpenedKey = NULL;
	}
}

//RegistryKey key = Registry.LocalMachine.OpenSubKey("Software", true);
//RegistryKey newkey = key.CreateSubKey("MCBInc");
//#define KEY_ALL_ACCESS ((STANDARD_RIGHTS_ALL | KEY_QUERY_VALUE | KEY_SET_VALUE | KEY_CREATE_SUB_KEY | KEY_ENUMERATE_SUB_KEYS | KEY_NOTIFY | KEY_CREATE_LINK) & (~SYNCHRONIZE))
//if not OK,not close current key
bool RegistryKey::OpenSubKey(const TCHAR* subkey, bool writable, bool createIfNotExist)
{
	long regret;
	HKEY hKey = NULL;
	DWORD flag = (writable ? KEY_ALL_ACCESS : KEY_QUERY_VALUE);
	if(_hOpenedKey == NULL)
	{
		regret = ::RegOpenKeyEx((HKEY)_hKeyRoot, subkey, 0, flag, &hKey);
	}
	else
	{
		regret = ::RegOpenKeyEx((HKEY)_hOpenedKey, subkey, 0, flag, &hKey);
	}
	if(regret != ERROR_SUCCESS)
	{
		if(createIfNotExist)
		{
			if(_hOpenedKey == NULL)
			{
				regret = ::RegCreateKeyEx((HKEY)_hKeyRoot, subkey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL);
			}
			else
			{
				regret = ::RegCreateKeyEx(_hOpenedKey, subkey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL);
			}
		}
		if(regret != ERROR_SUCCESS)
		{
			return false;
		}
	}
	if(_hOpenedKey != NULL)
	{
		::RegCloseKey(_hOpenedKey);
		_hOpenedKey = NULL;
	}
	_hOpenedKey = hKey;
	return true;
}

RegistryKey* RegistryKey::OpenSubKeyNewHandle(const TCHAR* subkey, bool writable)
{
	long regret;
	HKEY hKey = NULL;
	DWORD flag = (writable ? KEY_ALL_ACCESS : KEY_QUERY_VALUE);
	if(_hOpenedKey == NULL)
	{
		regret = ::RegOpenKeyEx((HKEY)_hKeyRoot, subkey, 0, flag, &hKey);
	}
	else
	{
		regret = ::RegOpenKeyEx((HKEY)_hOpenedKey, subkey, 0, flag, &hKey);
	}
	if(regret != ERROR_SUCCESS)
	{
		return NULL;
	}

	RegistryKey* regKey = new RegistryKey(_hKeyRoot);
	regKey->_hOpenedKey = hKey;
	return regKey;
}

//if not OK,not close current key
bool RegistryKey::CreateSubKey(const TCHAR* subkey)
{
	long regret;
	HKEY hKey = NULL;
	if(_hOpenedKey == NULL)
	{
		regret = ::RegCreateKeyEx((HKEY)_hKeyRoot, subkey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL);
	}
	else
	{
		regret = ::RegCreateKeyEx(_hOpenedKey, subkey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL);
	}
	if(regret != ERROR_SUCCESS)
	{
		return false;
	}
	if(_hOpenedKey != NULL)
	{
		::RegCloseKey(_hOpenedKey);
		_hOpenedKey = NULL;
	}
	_hOpenedKey = hKey;
	return true;
}

RegistryKey* RegistryKey::CreateSubKeyNewHandle(const TCHAR* subkey)
{
	long regret;
	HKEY hKey = NULL;
	if(_hOpenedKey == NULL)
	{
		regret = ::RegCreateKeyEx((HKEY)_hKeyRoot, subkey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL);
	}
	else
	{
		regret = ::RegCreateKeyEx(_hOpenedKey, subkey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL);
	}
	if(regret != ERROR_SUCCESS)
	{
		return NULL;
	}

	RegistryKey* regKey = new RegistryKey(_hKeyRoot);
	regKey->_hOpenedKey = hKey;
	return regKey;
}

bool RegistryKey::DeleteSubKey(const TCHAR* subkey)
{
	long regret;
	if(_hOpenedKey == NULL)
	{
		regret = ::RegDeleteKey((HKEY)_hKeyRoot, subkey);
	}
	else
	{
		regret = ::RegDeleteKey(_hOpenedKey, subkey);
	}
	return (regret == ERROR_SUCCESS);
}

bool RegistryKey::DeleteSubKeyTree(const TCHAR* subkey)
{
	if(_hOpenedKey == NULL)
	{
		return false;
	}
	return (DeleteSubKeyTreeSub(_hOpenedKey, subkey) == ERROR_SUCCESS);
}

long RegistryKey::DeleteSubKeyTreeSub(HKEY hKeyRecurse, const TCHAR* subkey)
{
	HKEY hSubKey = NULL;
	long regret = ::RegOpenKeyEx((HKEY)hKeyRecurse, subkey, 0, KEY_ENUMERATE_SUB_KEYS | 0x00010000L, &hSubKey);
	if(regret != ERROR_SUCCESS)
	{
		return regret;
	}

	while(ERROR_SUCCESS == regret)
	{
		TCHAR szSubKey[MAX_PATH];
		DWORD cbSubkeySize(sizeof(szSubKey));

		regret = ::RegEnumKeyEx(hSubKey, 0, szSubKey, &cbSubkeySize, 0, 0, 0, 0);
		if(ERROR_NO_MORE_ITEMS == regret)
		{
			break;
		}
		else if(ERROR_SUCCESS != regret)
		{
			CC_TRACE((_T("::RegEnumKeyEx() failed for subkey <%s> with error <%d>."), subkey, regret));
			return regret;
		}

		regret = DeleteSubKeyTreeSub(hSubKey, szSubKey);
		if(ERROR_SUCCESS != regret)
		{
			CC_TRACE((_T("RegistryKey::DeleteSubKeyTreeSub() failed for subkey <%s> with error <%d>."), subkey, regret));
		}
	}
	// Close (but may not have been there).
	if(hSubKey != NULL)
	{
		::RegCloseKey(hSubKey);
		regret = ::RegDeleteKey(hKeyRecurse, subkey);

		if(ERROR_SUCCESS != regret)
		{
			CC_TRACE((_T("::RegDeleteKey() failed for subkey <%s> with error <%d>."), subkey, regret));
		}
	}

	return regret;
}

bool RegistryKey::DeleteValue(const TCHAR* subkey)
{
	if(_hOpenedKey == NULL)
	{
		return false;
	}
	return (::RegDeleteValue(_hOpenedKey, subkey) == ERROR_SUCCESS);
}

//notice:here return length in bytes and without end zero(\0)
//[in, out] Pointer to a variable that specifies the size, in bytes, of the buffer 
//pointed to by the lpData parameter. When the function returns, this variable 
//contains the size of the data copied to lpData.
//If the data has the REG_SZ, REG_MULTI_SZ or REG_EXPAND_SZ type, then lpcbData 
//will also include the size of the terminating null character. 
DWORD RegistryKey::GetValueLength(const TCHAR* subkey, DWORD& dwType)
{
	if(_hOpenedKey != NULL)
	{
		//DWORD dwType = REG_NONE;
		DWORD lpcbData = 0;
		long regret = ::RegQueryValueEx(_hOpenedKey, subkey, NULL, &dwType, NULL, &lpcbData);
		if(regret == ERROR_SUCCESS)
		{
			if(lpcbData > 0 && (dwType == REG_SZ || dwType == REG_MULTI_SZ || dwType == REG_EXPAND_SZ))
			{
				#ifdef _UNICODE
				lpcbData -= 2;
				#else
				lpcbData--;
				#endif
			}
			return lpcbData;
		}
	}
	return (DWORD)-1;
}

cc::Str RegistryKey::GetValue(const TCHAR* subkey, const TCHAR* defaultValue)
{
	DWORD dwType = REG_NONE;
	DWORD lpcbData = GetValueLength(subkey, dwType);
	if(lpcbData <= 0)
	{
		return cc::Str(defaultValue);
	}

	//CC_ASSERT(dwType == REG_SZ);
	lpcbData += 2;
	TCHAR* lpstrValue = (TCHAR*)malloc(lpcbData);
	memset((void*)lpstrValue, 0, lpcbData);
	long regret = ::RegQueryValueEx(_hOpenedKey, subkey, NULL, &dwType, (BYTE*)(LPCTSTR)lpstrValue, &lpcbData);
	if(regret != ERROR_SUCCESS)
	{
		free(lpstrValue);
		return cc::Str(defaultValue);
	}
	cc::Str sRetu;
	//sRetu.TakeoverPointer(&lpstrValue, (int)lpcbData);
	sRetu.TakeoverPointer(&lpstrValue);
	return sRetu;
}

cc::Str RegistryKey::GetValue(const cc::Str& subkey, const cc::Str& defaultValue)
{
	return GetValue((TCHAR*)subkey.GetBuf(), (TCHAR*)defaultValue.GetBuf());
}

DWORD RegistryKey::GetValue(const TCHAR* subkey, DWORD dwValue)
{
	if(_hOpenedKey == NULL)
	{
		return dwValue;
	}
	DWORD dwType = REG_DWORD;
	DWORD lpcbData = sizeof(DWORD);
	long regret = ::RegQueryValueEx(_hOpenedKey, subkey, NULL, &dwType, (LPBYTE)&dwValue, &lpcbData);
	if(regret != ERROR_SUCCESS)
	{
		return dwValue;
	}
	return dwValue;
}

DWORD RegistryKey::GetValue(const cc::Str& subkey, DWORD dwValue)
{
	return GetValue((TCHAR*)subkey.GetBuf(), dwValue);
}

bool RegistryKey::GetValue(const TCHAR* subkey, BYTE* pValue, DWORD& dwValueLenInBytes)
{
	if(_hOpenedKey == NULL || pValue == NULL)
	{
		return false;
	}
	DWORD dwType = REG_NONE;
	long regret = ::RegQueryValueEx(_hOpenedKey, subkey, NULL, &dwType, pValue, &dwValueLenInBytes);
	return (regret == ERROR_SUCCESS);
}

DWORD RegistryKey::GetValue(const TCHAR* subkey, BYTE** outbuf)
{
	if(_hOpenedKey == NULL)
	{
		return -1;
	}
	DWORD dwType = REG_NONE;
	DWORD dwValueLenInBytes = GetValueLength(subkey, dwType);
	if(dwValueLenInBytes <= 0)
	{
		return -1;
	}
	BYTE* des = (BYTE *)malloc((size_t)(dwValueLenInBytes + 3));
	if(des == NULL)
	{
		return -1;
	}
	if(*outbuf != NULL)
	{
		free(*outbuf);
	}
	*outbuf = (BYTE*)des;
	memset((void*)des, '\0', (size_t)(dwValueLenInBytes + 3));

	long regret = ::RegQueryValueEx(_hOpenedKey, subkey, NULL, &dwType, des, &dwValueLenInBytes);
	if(regret != ERROR_SUCCESS)
	{
		free(*outbuf);
		*outbuf = NULL;
		dwValueLenInBytes = -1;
	}
	return dwValueLenInBytes;
}

bool RegistryKey::SetValue(const TCHAR* subkey, const TCHAR* value)
{
	if(_hOpenedKey == NULL || value == NULL)
	{
		return false;
	}
	DWORD lpcbData = (DWORD)((int)_tcslen(value) * sizeof(TCHAR));
	long regret = ::RegSetValueEx(_hOpenedKey, subkey, NULL, REG_SZ, (const BYTE*)value, lpcbData);
	return (regret == ERROR_SUCCESS);
}

bool RegistryKey::SetValue(const cc::Str& subkey, const cc::Str& value)
{
	return SetValue((TCHAR*)subkey.GetBuf(), (TCHAR*)value.GetBuf());
}

bool RegistryKey::SetValue(const TCHAR* subkey, DWORD dwValue)
{
	if(_hOpenedKey == NULL)
	{
		return false;
	}
	long regret = ::RegSetValueEx(_hOpenedKey, subkey, NULL, REG_DWORD, (const BYTE*)&dwValue, sizeof(DWORD));
	return (regret == ERROR_SUCCESS);
}

bool RegistryKey::SetValue(const cc::Str& subkey, DWORD dwValue)
{
	return SetValue((TCHAR*)subkey.GetBuf(), dwValue);
}

bool RegistryKey::SetValue(const TCHAR* subkey, const BYTE* value, DWORD dwValueLenInBytes)
{
	if(_hOpenedKey == NULL || value == NULL)
	{
		return false;
	}
	long regret = ::RegSetValueEx(_hOpenedKey, subkey, NULL, REG_BINARY, value, dwValueLenInBytes);
	return (regret == ERROR_SUCCESS);
}
//BOOL CRegistry::SaveKey(LPCTSTR lpszFileName)
//{
//	ASSERT(m_hKey != NULL);
//	return ::RegSaveKey(m_hKey, lpszFileName, NULL) == ERROR_SUCCESS;
//}
//
//BOOL CRegistry::RestoreKey(LPCSTR lpszFileName, DWORD dwFlags)
//{
//	ASSERT(m_hKey != NULL);
//	return ::RegRestoreKey(m_hKey, lpszFileName, dwFlags) == ERROR_SUCCESS;
//}
//
//BOOL CRegistry::LoadKey(enum Keys hKey, LPCTSTR lpszSubKey, LPCSTR lpszFileName)
//{
//	return ::RegLoadKey((HKEY)hKey, lpszSubKey, lpszFileName) == ERROR_SUCCESS;
//}
///////////////////////////////////////////////////////////////////////////////

//===========================================================turnWord for ini
void turnWord(char *pBuf)
{
	char *p = (char*)pBuf;
	char *newp = (char*)pBuf;
	while(*p != '\0')
	{
		if(*p == '\\')
		{
			if(*(p + 1) == '\\')
			{
				*newp = '\\';
				p++;
			}
			else if(*(p + 1) == 'n')
			{
				*p = '\n';
				p++;
			}
			else if(*(p + 1) == 'r')
			{
				*p = '\r';
				p++;
			}
			else if(*(p + 1) == 't')
			{
				*p = '\t';
				p++;
			}
			else
			{
				*newp = '\\';
			}
		}
		else if(*newp != *p)
		{
			*newp = *p;
		}
		p++;
		newp++;
	}
	*newp = '\0';
}
void turnWord(wchar_t *pBuf)
{
	wchar_t *p = (wchar_t*)pBuf;
	wchar_t *newp = (wchar_t*)pBuf;
	while(*p != L'\0')
	{
		if(*p == L'\\')
		{
			if(*(p + 1) == L'\\')
			{
				*newp = L'\\';
				p++;
			}
			else if(*(p + 1) == L'n')
			{
				*newp = L'\n';
				p++;
			}
			else if(*(p + 1) == L'r')
			{
				*newp = L'\r';
				p++;
			}
			else if(*(p + 1) == L't')
			{
				*newp = L'\t';
				p++;
			}
			else
			{
				*newp = L'\\';
			}
		}
		else if(*newp != *p)
		{
			*newp = *p;
		}
		p++;
		newp++;
		//cc::Log::debug(L"p:%s, newp:%s, org:%s", p, newp, pBuf);
	}
	*newp = L'\0';
}


/////////////////////////////////////////////////////////////////////////////
// ARACrypt
ARACrypt::ARACrypt()
	:
	// Initialize the shift registers to non-zero 
	// values. If the shift register contains all 
	// 0's when the generator starts, it will not 
	// produce a usable sequence of bits. The 
	// numbers used here have no special features 
	// except for being non-zero.
	m_LFSR_A( 0x13579BDF ),
	m_LFSR_B( 0x2468ACE0 ),
	m_LFSR_C( 0xFDB97531 ),

	// Initialize the masks to magic numbers. 
	// These values are primitive polynomials mod 
	// 2, described in Applied Cryptography, 
	// second edition, by Bruce Schneier (New York: 
	// John Wiley and Sons, 1994). See Chapter 15: 
	// Random Sequence Generators and Stream 
	// Ciphers, particularly the discussion on 
	// Linear Feedback Shift Registers.
	//
	// The primitive polynomials used here are:
	// Register A:	( 32, 7, 6, 2, 0 )
	// Register B:	( 31, 6, 0 )
	// Register C:	( 29, 2, 0 )
	//
	// The bits that must be set to "1" in the 
	// XOR masks are:
	// Register A:	( 31, 6, 5, 1 )
	// Register B:	( 30, 5 )
	// Register C:	( 28, 1 )
	//
	// Developer's Note
	// DO NOT CHANGE THESE NUMBERS WITHOUT 
	// REFERRING TO THE DISCUSSION IN SCHNEIER'S 
	// BOOK. They are some of very few 
	// near-32-bit values that will act as 
	// maximal-length random generators.
	m_Mask_A( 0x80000062 ), 
	m_Mask_B( 0x40000020 ), 
	m_Mask_C( 0x10000002 ), 

	// Set up LFSR "rotate" masks.
	// These masks limit the number of bits 
	// used in the shift registers. Each one 
	// provides the most-significant bit (MSB) 
	// when performing a "rotate" operation. Here 
	// are the shift register sizes and the byte 
	// mask needed to place a "1" bit in the MSB 
	// for Rotate-1, and a zero in the MSB for 
	// Rotate-0. All the shift registers are stored
	// in an unsigned 32-bit integer, but these 
	// masks effectively make the registers 32 
	// bits (A), 31 bits (B), and 29 bits (C).
	//
	//	Bit	  |  3            2             1            0
	//	Pos'n | 1098 7654  3210 9876  5432 1098  7654 3210
	//	===== | ==========================================
	//	Value | 8421-8421  8421-8421  8421-8421  8421-8421
	//	===== | ==========================================
	//		  | 
	// A-Rot0 | 0111 1111  1111 1111  1111 1111  1111 1111  
	// A-Rot1 | 1000 0000  0000 0000  0000 0000  0000 0000 
	//		  | 
	// B-Rot0 | 0011 1111  1111 1111  1111 1111  1111 1111  
	// B-Rot1 | 1100 0000  0000 0000  0000 0000  0000 0000  
	//		  | 
	// C-Rot0 | 0000 1111  1111 1111  1111 1111  1111 1111  
	// C-Rot1 | 1111 0000  0000 0000  0000 0000  0000 0000  
	//
	//	
	// Reg Size	MSB Position	Rotate-0 Mask	Rotate-1 Mask
	//	A	32		31			0x7FFFFFFF		0x80000000
	//	B	31		30			0x3FFFFFFF		0xC0000000
	//	C	29		28			0x0FFFFFFF		0xF0000000
	//
	m_Rot0_A( 0x7FFFFFFF ), 
	m_Rot0_B( 0x3FFFFFFF ), 
	m_Rot0_C( 0x0FFFFFFF ),
	m_Rot1_A( 0x80000000 ), 
	m_Rot1_B( 0xC0000000 ), 
	m_Rot1_C( 0xF0000000 )
{
}


// Everything is on the frame.
ARACrypt::~ARACrypt()
{
}

/////////////////////////////////////////////////////////////////////////////
// ARACrypt operations
void ARACrypt::SetKey(const char* key0, int keylen)
{
	const unsigned char* key = (const unsigned char*)key0;
	unsigned char seed[12];
	int nIdx = 0;
	// Make sure seed is at least 12 bytes long . 
	for(nIdx = 0; nIdx < 12; nIdx++)
	{
		seed[nIdx] = key[nIdx%keylen];
		seed[nIdx] |= 0x0001<<(nIdx%8);
	}

	// LFSR A, B, and C get the first, second, and
	// third four bytes of the seed, respectively.
	for(nIdx = 0; nIdx < 4; nIdx++)
	{
		m_LFSR_A = ((m_LFSR_A <<= 8) | ((unsigned long int) seed[nIdx + 0]));
		m_LFSR_B = (( m_LFSR_B <<= 8) | ((unsigned long int) seed[nIdx + 4]));
		m_LFSR_C = (( m_LFSR_C <<= 8) | ((unsigned long int) seed[nIdx + 8]));
	}

	// If any LFSR contains 0x00000000, load a 
	// non-zero default value instead. There is
	// no particular "good" value. The ones here
	// were selected to be distinctive during
	// testing and debugging.
	if(0x00000000 == m_LFSR_A)
	{
		m_LFSR_A = 0x13579BDF;
	}
	if(0x00000000 == m_LFSR_B)
	{
		m_LFSR_B = 0x2468ACE0;
	}
	if(0x00000000 == m_LFSR_C)
	{
		m_LFSR_C = 0xFDB97531;
	}
}

//***********************************************
// ARACrypt::TransformChar
//***********************************************
// Transform a single character. If it is 
// plaintext, it will be encrypted. If it is
// encrypted, and if the LFSRs are in the same
// state as when it was encrypted (that is, the
// same keys loaded into them and the same number
// of calls to TransformChar after the keys
// were loaded), the character will be decrypted
// to its original value.
//
// DEVELOPER'S NOTE
// This code contains corrections to the LFSR
// operations that supercede the code examples
// in Applied Cryptography (first edition, up to
// at least the 4th printing, and second edition,
// up to at least the 6th printing). More recent
// errata sheets may show the corrections.
//***********************************************
void ARACrypt::TransformChar(unsigned char& cTarget)
{
	int					Counter	= 0;
	unsigned char		Crypto	= '\0';
	unsigned long int	Out_B	= (m_LFSR_B & 0x00000001);
	unsigned long int	Out_C	= (m_LFSR_C & 0x00000001);

	// Cycle the LFSRs eight times to get eight 
	// pseudo-random bits. Assemble these into 
	// a single random character (Crypto).
	for(Counter = 0; Counter < 8; Counter++)
	{
		if(m_LFSR_A & 0x00000001)
		{
			// The least-significant bit of LFSR 
			// A is "1". XOR LFSR A with its 
			// feedback mask.
			m_LFSR_A = (((m_LFSR_A ^ m_Mask_A) >> 1) | m_Rot1_A);
			
			// Clock shift register B once.
			if( m_LFSR_B & 0x00000001 )
			{
				// The LSB of LFSR B is "1". XOR 
				// LFSR B with its feedback mask.
				m_LFSR_B = (((m_LFSR_B ^ m_Mask_B) >> 1) | m_Rot1_B);
				Out_B = 0x00000001;
			}
			else
			{
				// The LSB of LFSR B is "0". Rotate 
				// the LFSR contents once.
				m_LFSR_B = (( m_LFSR_B >> 1) & m_Rot0_B);
				Out_B = 0x00000000;
			}
		}
		else
		{
			// The LSB of LFSR A is "0". 
			// Rotate the LFSR contents once.
			m_LFSR_A = (( m_LFSR_A >> 1) & m_Rot0_A);

			// Clock shift register C once.
			if( m_LFSR_C & 0x00000001 )
			{
				// The LSB of LFSR C is "1". 
				// XOR LFSR C with its feedback mask.
				m_LFSR_C = ((( m_LFSR_C ^ m_Mask_C) >> 1) | m_Rot1_C);
				Out_C = 0x00000001;
			}
			else
			{
				// The LSB of LFSR C is "0". Rotate 
				// the LFSR contents once.
				m_LFSR_C = ((m_LFSR_C >> 1) & m_Rot0_C);
				Out_C = 0x00000000;
			}
			
		}

		// XOR the output from LFSRs B and C and 
		// rotate it into the right bit of Crypto.

		//The follwing conversion warning is unecessary here as 
		//'loss of data' is a side effect and harmless.
		#pragma warning(disable : 4244)
		Crypto = ((Crypto << 1) | (Out_B ^ Out_C));
		#pragma warning(default : 4244)
	}

	// XOR the resulting character with the 
	// input character to encrypt/decrypt it.
	
	//The follwing conversion warning not necessary here either.
	//The 'loss of data', so to speak is a side effect and harmless.
	#pragma warning(disable : 4244)
	cTarget = ( cTarget ^ Crypto );
	if (cTarget == NULL)				//No nulls allowed here. There is
	{
		cTarget = ( cTarget ^ Crypto );	//no working std::string available.
	}
	#pragma warning( default : 4244 )
}


//***********************************************
// ARACrypt::TransformString
//***********************************************
// Encrypt or decrypt a standard string in place.
// The string to transform is passed in as 
// Target.
//***********************************************
void ARACrypt::TransformString(char* csTarget0, int len)
{
	// Transform each character in the string.
	// 
	// DEVELOPER'S NOTE
	// ==========================================
	// DO NOT TREAT THE OUTPUT STRING AS A NULL-
	// TERMINATED STRING. 
	// ==========================================
	// The transformation process can create '\0' 
	// characters in the output string. Always 
	// use the length() method to retrieve the full 
	// string when it has been transformed.

	// bek NOTE: The above note does not apply to this
	// implementation of Warren Ward's method.
	// ARACrypt knows about NULLs and transforms them
	// into XOR NULLs so the entire result can be
	// treated as a 'normal' NULL terminated string.
	unsigned char* csTarget = (unsigned char*)csTarget0;
	for(int nPos = 0; nPos < len; nPos++)
	{
		//The follwing conversion warning not necessary here either.
		//The 'loss of data', so to speak is a side effect and harmless.
		#pragma warning(disable : 4244)
		unsigned char cBuff = csTarget[nPos];
		#pragma warning( default : 4244 )
		TransformChar((unsigned char&)cBuff);
		csTarget[nPos] = cBuff;
	}
}


//===========================================================cc::Config
///////////////////////////////////////////////////////////////////////////////
COLORREF g_CustColors[16] = 
{
	RGB(240, 240, 240), RGB(225, 225, 225), RGB(210, 210, 210), RGB(195, 195, 195), 
	RGB(180, 180, 180), RGB(165, 165, 165), RGB(150, 150, 150), RGB(135, 135, 135), 
	RGB(120, 120, 120), RGB(105, 105, 105), RGB(90, 90, 90), RGB(75, 75, 75), 
	RGB(60, 60, 60), RGB(45, 45, 45), RGB(30, 30, 30), RGB(15, 15, 15)
};
// Lang ANSI code page->Locale name table
typedef struct tagPageCodeLocalName
{
	int pageCode;					// ANSI code page
	TCHAR* localeName;		// Locale name
} PageCodeLocalName;
PageCodeLocalName g_PageCodeLocalName[] =
{
	{ 874,		_T("th-TH") }, //Thai (Thailand)
	{ 932,		_T("ja-JP") }, //Japanese (Japan)
	{ 936,		_T("zh-CN") }, //Chinese (PRC)
	{ 949,		_T("ko-KR") }, //Korean (Korea)
	{ 950,		_T("zh-HK") }, //Chinese (Hong Kong SAR, PRC)
	{ 1250,		_T("hr-BA") }, //? //Croatian (Bosnia and Herzegovina, Latin)
	{ 1251,		_T("be-BY") }, //? //Belarusian (Belarus)
	{ 1252,		_T("en-US") }, //English (United States)
	{ 1253,		_T("el-GR") }, //Greek (Greece)
	{ 1254,		_T("tr-TR") }, //? //Turkish (Turkey)
	{ 1255,		_T("he-IL") }, //Hebrew (Israel)
	{ 1256,		_T("ar-DZ") }, //? //Arabic (Algeria)
	{ 1257,		_T("lt-LT") }, //? //Lithuanian (Lithuania)
	{ 1258,		_T("vi-VN") }, //Vietnamese (Vietnam)
	{ 0000,		_T("             ") },// CC_LCALNAME_MAX_LENGTH + 1
	{ 0000,		_T("             ") },
	{ 0000,		_T("             ") },
	{ 0000,		_T("             ") },
	{ 0000,		_T("             ") },
	{ 0000,		_T("             ") },
	{ 0000,		_T("             ") },
	{ 0000,		_T("             ") },
	{ 0000,		_T("             ") },
	{ 0000,		_T("             ") },
	{ 0000,		_T("             ") }
};
//TCHAR* Util::_tcStartupPathOnly = NULL;
//TCHAR* Util::_tcStartupFullPath = NULL;
//TCHAR Util::_tcStartupPathOnly[_MAX_PATH + 1] = _T("\0");
//TCHAR Util::_tcStartupPathFull[_MAX_PATH + 1] = _T("\0");
///////////////////// constructors //////////////////////////
Util::Util()
{
}

///////////////////// destructor //////////////////////////
Util::~Util()
{
}

//========================================
//CodePage
int Util::GetCurrentCodePage(UINT nFlage)
{
	static UINT nLocaleCodePage = 0;
	if(nLocaleCodePage == 0)
	{
		TCHAR tcBuf[9];
		int nChars = ::GetLocaleInfo(nFlage, LOCALE_IDEFAULTANSICODEPAGE, tcBuf, 8);

		//If OK
		if(nChars > 0)
		{
			nLocaleCodePage = (UINT)_ttoi(tcBuf);
		}
	}

	//O means either: no ANSI code page (Unicode-only locale?) or failed to get locale
	//In the case of Unicode-only locales, what do multibyte apps do? Answers on a postcard.
	return nLocaleCodePage;
}

cc::Str Util::GetCurrentLocaleName(bool withSISO3166CTRYNAME, UINT nFlage)
{
	cc::Str str;
	TCHAR tcBuf[85 + 1];
	int nChars = ::GetLocaleInfo(nFlage, LOCALE_SISO639LANGNAME, tcBuf, 85);

	//If OK
	if(nChars > 0)
	{
		str.ValueOf(tcBuf);
		if(withSISO3166CTRYNAME)
		{
			nChars = ::GetLocaleInfo(nFlage, LOCALE_SISO3166CTRYNAME, tcBuf, 85);
			if(nChars > 0)
			{
				str.Append(_T("-"));
				str.Append(tcBuf);
			}
		}
	}

	return str;
}

bool Util::SetPageCodeToLocalName(int pageCode, const TCHAR* localeName)
{
	for(int n = 0; n < sizeof(g_PageCodeLocalName) / sizeof(PageCodeLocalName); n++)
	{
		if(g_PageCodeLocalName[n].pageCode == pageCode)
		{
			//_tcsncpy(g_PageCodeLocalName[n].localeName, localeName, CC_LCALNAME_MAX_LENGTH);
			cc::Buf::Cpy(g_PageCodeLocalName[n].localeName, CC_LCALNAME_MAX_LENGTH, localeName);
			return true;
		}
	}
	for(int n = 0; n < sizeof(g_PageCodeLocalName) / sizeof(PageCodeLocalName); n++)
	{
		if(g_PageCodeLocalName[n].pageCode == 0)
		{
			g_PageCodeLocalName[n].pageCode = pageCode;
			//_tcsncpy(g_PageCodeLocalName[n].localeName, localeName, CC_LCALNAME_MAX_LENGTH);
			cc::Buf::Cpy(g_PageCodeLocalName[n].localeName, CC_LCALNAME_MAX_LENGTH, localeName);
			return true;
		}
	}
	return false;
}

const TCHAR* Util::GetSimilarLocalName()
{
	//load from res/file that has same pagecode to system
	int nPageCode = cc::Util::GetCurrentCodePage();
	if(nPageCode > 0)
	{
		for(int n = 0; n < sizeof(g_PageCodeLocalName) / sizeof(PageCodeLocalName); n++)
		{
			if(g_PageCodeLocalName[n].pageCode == nPageCode)
			{
				return g_PageCodeLocalName[n].localeName;
			}
		}
	}
	return NULL;
}

const unsigned char BASE64_CODE[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
#define BASE64_PADDING '='
int Util::codeB64Encode(const char* input, unsigned int data_size, char** output)
{
	CC_LOGTIME_START();
	unsigned long nDesLen;
	int nMod = data_size % 3;
	if(nMod != 0)
	{
		nDesLen = (data_size + 3 - nMod) * 4/3;
	}
	else
	{
		nDesLen = (data_size) * 4/3;
	}

	unsigned char* des = (unsigned char *)malloc(nDesLen + 5);
	if(des == NULL)
	{
		return -1;
	}
	if(*output != NULL)
	{
		free(*output);
	}
	*output = (char*)des;
	memset(des, '\0', nDesLen + 5);

	const unsigned char* src = (const unsigned char*)input;
    const unsigned char* src_end = &src[data_size];
    for(; src + 2 < src_end; src += 3, des += 4)
    {
        des[0] = BASE64_CODE[src[0] >> 2];
        des[1] = BASE64_CODE[((src[0] << 4) | (src[1] >> 4)) & 0x3F];
        des[2] = BASE64_CODE[((src[1] << 2) | (src[2] >> 6)) & 0x3F];
        des[3] = BASE64_CODE[src[2] & 0x3F];
    }

    if(nMod == 1)
    {
        des[0] = BASE64_CODE[src[0] >> 2];
        des[1] = BASE64_CODE[(src[0] << 4) & 0x3F];
        des[2] = BASE64_PADDING;
        des[3] = BASE64_PADDING;
    }
    else if(nMod == 2)
    {
        des[0] = BASE64_CODE[src[0] >> 2];
        des[1] = BASE64_CODE[((src[0] << 4) | (src[1] >> 4)) & 0x3F];
        des[2] = BASE64_CODE[(src[1] << 2) & 0x3F];
        des[3] = BASE64_PADDING;
    }
    des[4] = '\0';
	CC_LOGTIME_PRINT();
	return nDesLen;
}

int Util::codeB64Decode(const char* input, unsigned int data_size, char** output)
{
	CC_LOGTIME_START();
	if(data_size < 3 || data_size % 4 != 0)
	{
		return -1;
	}
	unsigned long nDesLen = (data_size) * 3/4;

	//invoid wchar_t
	unsigned char* des = (unsigned char *)malloc(nDesLen + 10);
	if(des == NULL)
	{
		return -1;
	}
	if(*output != NULL)
	{
		free(*output);
	}
	*output = (char*)des;
	memset(des, 0, nDesLen + 10);

	unsigned char src_chg[4];
	const unsigned char* src = (const unsigned char*)input;
    const unsigned char* src_end = &src[data_size];
    for(; src + 2 < src_end; src += 4, des += 3)
    {
		for(int n = 0; n < 4; n++)
		{
			const char *p = strchr((const char*)BASE64_CODE, src[n]);
			if(p)
			{
				src_chg[n] = (unsigned char)(p - (const char*)BASE64_CODE);
			}
			else
			{
				src_chg[n] = src[n] == '=' ? '\0' : '?';
			}
		}
		des[0] = (src_chg[0] << 2) + ((src_chg[1] & 0x30) >> 4);
		des[1] = ((src_chg[1] & 0x0F) << 4) + ((src_chg[2] & 0x3C) >> 2);
		des[2] = ((src_chg[2] & 0x03) <<6 ) + src_chg[3];
        des[3] = '\0';
    }
	if(input[data_size - 2] == BASE64_PADDING)
	{
		nDesLen--;
		nDesLen--;
	}
	else if(input[data_size - 1] == BASE64_PADDING)
	{
		nDesLen--;
	}
	CC_LOGTIME_PRINT();
	return nDesLen;
}

void Util::codeB64_ToPost(char* input, unsigned int data_size)
{
	for(unsigned int n = 0; n < data_size; n++)
	{
		//+/=  <-> @#-
		if(input[n] == '+')
		{
			input[n] = '-';
		}
		else if(input[n] == '/')
		{
			input[n] = '_';
		}
		else if(input[n] == '=')
		{
			input[n] = '.';
		}
	}
}
void Util::codeB64_FromPost(char* input, unsigned int data_size)
{
	for(unsigned int n = 0; n < data_size; n++)
	{
		//+/=  <-> @#-
		if(input[n] == '-')
		{
			input[n] = '+';
		}
		else if(input[n] == '_')
		{
			input[n] = '/';
		}
		else if(input[n] == '.')
		{
			input[n] = '=';
		}
	}
}

int Util::codeURLEncode(const char* input, unsigned int data_size, char** output)
{
	CC_LOGTIME_START();
	unsigned char* buf = (unsigned char*)input;
	unsigned long nDesLen = data_size;
    for(unsigned int n = 0; n < data_size; n++)
    {
		if(buf[n] == 0 || buf[n] == '=' || buf[n] == '%' || buf[n] == '.' || buf[n] == '/' || (buf[n] & 0x80))
		{
			nDesLen += 2;
		}
    }

	unsigned char* des = (unsigned char *)malloc(nDesLen + 12);
	if(des == NULL)
	{
		return -1;
	}
	if(*output != NULL)
	{
		free(*output);
	}
	*output = (char*)des;
	memset(des, 0, nDesLen + 12);

	char strbuf[] = "    ";
    for(unsigned int n = 0; n < data_size; n++)
    {
		if(buf[n] == ' ')
		{
			des[0] = '+';
		}
		else if(buf[n] == 0 || buf[n] == '=' || buf[n] == '%' || buf[n] == '.' || buf[n] == '/' || (buf[n] & 0x80))
		{
			//sprintf((char*)des, "%%%02X", (unsigned char)buf[n]);
			//cc::Buf::Format((char*)des, nDesLen + 12, "%%%02X", (unsigned char)buf[n]);
			cc::Buf::Format(strbuf, 4, "%%%02X", (unsigned char)buf[n]);
			des[0] = strbuf[0];
			des[1] = strbuf[1];
			des[2] = strbuf[2];
			des = des + 2;
		}
		else
		{
			des[0] = buf[n];
		}
		des++;
		des[0] = '\0';
    }
	CC_LOGTIME_PRINT();
	return nDesLen;
}

int Util::codeURLDecode(const char* input, unsigned int data_size, char** output)
{
	CC_LOGTIME_START();
	unsigned long nDesLen = data_size;
    for(unsigned int n = 0; n < data_size; n++)
    {
		if(input[n] == '%')
		{
			nDesLen -= 2;
		}
    }

	//invoid wchar_t
	unsigned char* des = (unsigned char *)malloc(nDesLen + 12);
	if(des == NULL)
	{
		return -1;
	}
	if(*output != NULL)
	{
		free(*output);
	}
	*output = (char*)des;
	memset(des, 0, nDesLen + 12);

	char sscanfbuf[3];
	sscanfbuf[2] = '\0';
    for(unsigned int n = 0; n < data_size; n++)
    {
		if(input[n] == '+')
		{
			des[0] = ' ';
		}
		else if(input[n] == '%' && n + 2 < data_size)
		{
			if(input[n + 1] == '0' && input[n + 2] == '0')
			{
				des[0] = '\0';
			}
			else
			{
				int code;
				sscanfbuf[0] = input[n + 1];
				sscanfbuf[1] = input[n + 2];
				//not need "%02X"
#ifdef __STDC_SECURE_LIB__
				if(sscanf_s(sscanfbuf, "%02x", &code) != 1)
#else
				if(sscanf(sscanfbuf, "%02x", &code) != 1)
#endif
				{
					code = '?';
				}
				des[0] = code;
			}
			n += 2;
		}
		else
		{
			des[0] = (unsigned char)input[n];
		}
		des++;
		des[0] = '\0';
    }
	CC_LOGTIME_PRINT();
	return nDesLen;
}

//Wheeler & Needham’s original formulation (in C) of corrected block TEA (aka xxtea)
//notice:here modified orignal long to char,and 4-long size of key to any size.
//       so it is better to use at least 16-char of key(=4-long size).
//       also notice that loop count:minloop + 16/keylen + 52/len
//char key[] = "encryption-pwzxfgdfhgfnvbnbvbvjhgfjjh";
//char txt[] = "some highly secret text to be encryptedasdfasdfasdf123dfdsf3243";
//cc::Util::teaEnCrypt(txt, (long)strlen(txt), key, (long)strlen(key));
//cc::Log::debug("encrypt:[%s]", txt);
//cc::Util::teaDeCrypt(txt, (long)strlen(txt), key, (long)strlen(key));
//cc::Log::debug("decrypt:[%s]", txt);
//#define MX  (( (((z>>5)^(y<<2))+((y>>3)^(z<<4)))^((sum^y)+(key[(p&3)^e]^z)) ) & 0xffffffff)
#define MX  (( (((z>>5)^(y<<2))+((y>>3)^(z<<4)))^((sum^y)+(key[(p^e)%keylen]^z)) ) & 0xffffffff)
void  Util::teaEnCrypt(const char* inbuf, int len, const char* key, int keylen, int minloop, char** outbuf)
{
	//invoid wchar_t
	char* des = (char *)malloc(len + 3);
	if(des == NULL)
	{
		return;
	}
	if(*outbuf != NULL)
	{
		free(*outbuf);
	}
	*outbuf = (char*)des;
	memset(des, '\0', len + 3);
	memcpy(des, inbuf, len);
	teaEnCrypt(des, len, key, keylen, minloop);
}
/*
void  Util::teaEnCrypt(char* v, int len, const char* key, int keylen, int minloop)
{
	CC_LOGTIME_START();
	unsigned char z=v[len-1], y=v[0], sum=0, e, DELTA=(unsigned char)0x9e3779b9;
	int p, q;
	//q = 6 + 52/len;
	q = minloop + 16/keylen + 52/len;
	while(q-- > 0)
	{
		sum += DELTA;
		//e = (sum >> 2) & 3;
		e = (sum >> 2);
		for(p=0; p<len-1; p++)
		{
			y = v[p+1];
			z = v[p] += MX;
		}
		y = v[0];
		z = v[len-1] += MX;
	}
	CC_LOGTIME_PRINT();
}
*/
void Util::teaDeCrypt(const char* inbuf, int len, const char* key, int keylen, int minloop, char** outbuf)
{
	//invoid wchar_t
	char* des = (char *)malloc(len + 3);
	if(des == NULL)
	{
		return;
	}
	if(*outbuf != NULL)
	{
		free(*outbuf);
	}
	*outbuf = (char*)des;
	memset(des, '\0', len + 3);
	memcpy(des, inbuf, len);
	teaDeCrypt(des, len, key, keylen, minloop);
}
/*
void Util::teaDeCrypt(char* v, int len, const char* key, int keylen, int minloop)
{
	CC_LOGTIME_START();
	unsigned char z=v[len-1], y=v[0], sum=0, e, DELTA=(unsigned char)0x9e3779b9;
	int p, q;
	//q = 6 + 52/len;
	q = minloop + 16/keylen + 52/len;
	sum = q*DELTA ;
	while(sum != 0)
	{
		//e = (sum >> 2) & 3;
		e = (sum >> 2);
		for(p=len-1; p>0; p--)
		{
			z = v[p-1];
			y = v[p] -= MX;
		}
		z = v[len-1];
		y = v[0] -= MX;
		sum -= DELTA;
	}
	CC_LOGTIME_PRINT();
}
*/
/*
	//modify at 2015/09/05 for php
	for(int j = 1; j < 100; j++)
	{
		for(int n = 0; n < 240; n++)
		{
			cc::Str str = cc::Util::GetRandomString(cc::Str(LangCode::SYSTEM), n, n + 10);
			cc::Str strorg = str;
			cc::Str key = cc::Util::GetRandomString(cc::Str(LangCode::SYSTEM), n, n + 10);
			cc::Util::teaEnCrypt2((char*)str.GetBuf(), str.GetLength(), (char*)key.GetBuf(), key.GetLength(), j*n);
			cc::Util::teaDeCrypt2((char*)str.GetBuf(), str.GetLength(), (char*)key.GetBuf(), key.GetLength(), j*n);
			if(!strorg.Equals(str))
			{
				CC_TRACE(("err:[%d, %d]", j, n));
			}
		}
	}

	//php code
	//modify at 2015/09/05 for php
	//modify at 2015/09/05 for php
	set_time_limit(60 * 10);
	$t1 = '';
	for($j = 1; $j < 15; $j++)
	{
		$k = "asdfas!@#fasdfer!@#4123";
		echo "===============================[$j]<br>";
		$t1 .= 'a';
		for($n = 0; $n < 240; $n++)
		{
			$t1{$j-1} = $n;
			$k{$j-1} = $n;
			$ret = teaEnCrypt2($t1, $k, $j*$n);

			$ret2 = teaDeCrypt2($ret, $k, $j*$n);
			if($t1 != $ret2)
			{
				echo "err:[$j, $n]<br>";
			}
		}
	}



function tea_MX($y, $z, $sum, $p, $e, $key, $keylen)
{
	return ((((($z>>5)&0x07ffffff)^($y<<2))+((($y>>3)&0x1fffffff)^($z<<4))+(($sum+$e)^$p))^(($sum^$y)+($key{($p^$e)%$keylen}^$z)))&0x00ff;
}
function teaEnCrypt($v, $key, $minloop)
{
	$DELTA=0x00b9;
	$sum=0;
	$len = strlen($v);
	$keylen = strlen($key);
	if($len < 1 || $keylen < 1)
	{
		return $v;
	}
	$q = (1 + $minloop + floor(16/$keylen) + floor(52/$len))%255;
	while($q-- > 0)
	{
		$sum += $DELTA;
		$e = ($sum >> 2) & 3;
		if($len == 1)
		{
			$p = 0;
			$y = 0;
			$z = 0;
			$v{$p} = pack('C', ord($v{$p}) + tea_MX($y, $z, $sum, $p, $e, $key, $keylen));
		}
		else
		{
			for($p=0; $p<$len; $p++)
			{
				$y = ord($v{( $p + $len - 1)%$len});
				$z = ord($v{( $p + $len + 1)%$len});
				$v{$p} = pack('C', ord($v{$p}) + tea_MX($y, $z, $sum, $p, $e, $key, $keylen));
			}
		}
	}
	return $v;
}
function teaDeCrypt($v, $key, $minloop)
{
	$DELTA=0x00b9;
	$sum=0;
	$len = strlen($v);
	$keylen = strlen($key);
	if($len < 1 || $keylen < 1)
	{
		return $v;
	}
	$q = (1 + $minloop + floor(16/$keylen) + floor(52/$len))%255;
	$sum = $q*$DELTA;
	while($sum != 0)
	{
		$q--;
		$e = ($sum >> 2) & 3;
		if($len == 1)
		{
			$p = 0;
			$y = 0;
			$z = 0;
			$v{$p} = pack('C', ord($v{$p}) - tea_MX($y, $z, $sum, $p, $e, $key, $keylen));
		}
		else
		{
			for($p=$len-1; $p>=0; $p--)
			{
				$y = ord($v{( $p + $len - 1)%$len});
				$z = ord($v{( $p + $len + 1)%$len});
				$v{$p} = pack('C', ord($v{$p}) - tea_MX($y, $z, $sum, $p, $e, $key, $keylen));
			}
		}
		$sum -= $DELTA;
	}
	return $v;
}

function teaEnCrypt2($v, $key, $minloop)
{
	$DELTA=0x00b9;
	$sum=0;
	$len = strlen($v);
	$keylen = strlen($key);
	if($len < 1 || $keylen < 1)
	{
		return $v;
	}
	$q = (1 + $minloop + floor(16/$keylen) + floor(52/$len))%255;
	while($q-- > 0)
	{
		$sum += $DELTA;
		$e = ($sum >> 2) & 3;
		if($len == 1)
		{
			$p = 0;
			$y = 0;
			$z = 0;
			$k0 = (ord($key{($p+$minloop)%$keylen})|($p^$minloop))&0x00ff;
			$v{$p} = pack('C', ord($v{$p})^$k0);
			$v{$p} = pack('C', ord($v{$p}) + tea_MX($y, $z, $sum, $p, $e, $key, $keylen));
		}
		else
		{
			for($p = 0; $p < $len; $p++)
			{
				$k0 = (ord($key{($p+$minloop)%$keylen})|($p^$minloop))&0x00ff;
				$v{$p} = pack('C', ord($v{$p})^$k0);
			}
			for($p = 0; $p < $len; $p++)
			{
				$k0 = (ord($key{($p+$minloop)%$keylen})|($minloop^$p))&0x00ff;
				$jump = 1+(ord($key{$minloop%$keylen})^$minloop)%($len-1);
				$pp = ($p+$jump)%$len;
				$y0 = ord($v{$p}) & ~$k0;
				$y1 = ord($v{$p}) & $k0;
				$z0 = ord($v{$pp}) & ~$k0;
				$z1 = ord($v{$pp}) & $k0;

				//echo "p:$p, pp:$pp, y0:$y0, y1:$y1, z0:$z0, z1:$z1, jump:$jump, k0:$k0<br>";
				$v{$p} = pack('C', $y0|$z1);
				$v{$pp} = pack('C', $y1|$z0);
			}
			for($p=0; $p<$len; $p++)
			{
				$y = ord($v{( $p + $len - 1)%$len});
				$z = ord($v{( $p + $len + 1)%$len});
				$v{$p} = pack('C', ord($v{$p}) + tea_MX($y, $z, $sum, $p, $e, $key, $keylen));
			}
		}
	}
	return $v;
}
function teaDeCrypt2($v, $key, $minloop)
{
	$DELTA=0x00b9;
	$sum=0;
	$len = strlen($v);
	$keylen = strlen($key);
	if($len < 1 || $keylen < 1)
	{
		return $v;
	}
	$q = (1 + $minloop + floor(16/$keylen) + floor(52/$len))%255;
	$sum = $q*$DELTA;
	while($sum != 0)
	{
		$q--;
		$e = ($sum >> 2) & 3;
		if($len == 1)
		{
			$p = 0;
			$y = 0;
			$z = 0;
			$v{$p} = pack('C', ord($v{$p}) - tea_MX($y, $z, $sum, $p, $e, $key, $keylen));
			$k0 = (ord($key{($p+$minloop)%$keylen})|($p^$minloop))&0x00ff;
			$v{$p} = pack('C', ord($v{$p})^$k0);
		}
		else
		{
			for($p=$len-1; $p>=0; $p--)
			{
				$y = ord($v{( $p + $len - 1)%$len});
				$z = ord($v{( $p + $len + 1)%$len});
				$v{$p} = pack('C', ord($v{$p}) - tea_MX($y, $z, $sum, $p, $e, $key, $keylen));
			}
			for($p=$len-1; $p>=0; $p--)
			{
				$k0 = (ord($key{($p+$minloop)%$keylen})|($minloop^$p))&0x00ff;
				$jump = 1+(ord($key{$minloop%$keylen})^$minloop)%($len-1);
				$pp = ($p+$jump)%$len;
				$y0 = ord($v{$p}) & ~$k0;
				$y1 = ord($v{$p}) & $k0;
				$z0 = ord($v{$pp}) & ~$k0;
				$z1 = ord($v{$pp}) & $k0;

				//echo "p:$p, pp:$pp, y0:$y0, y1:$y1, z0:$z0, z1:$z1, jump:$jump, k0:$k0<br>";
				$v{$p} = pack('C', $y0|$z1);
				$v{$pp} = pack('C', $y1|$z0);
			}
			for($p = 0; $p < $len; $p++)
			{
				$k0 = (ord($key{($p+$minloop)%$keylen})|($p^$minloop))&0x00ff;
				$v{$p} = pack('C', ord($v{$p})^$k0);
			}
		}
		$sum -= $DELTA;
	}
	return $v;
}

*/
unsigned char tea_MX(unsigned int y, unsigned int z, unsigned int sum, unsigned int p, unsigned int e, const char* key, unsigned int keylen)
{
	return ((((z>>5)^(y<<2))+((y>>3)^(z<<4))+((sum+e)^p))^((sum^y)+(key[(p^e)%keylen]^z)))&0x00ff;
}
void  Util::teaEnCrypt(char* v, int len, const char* key, int keylen, int minloop)
{
	CC_LOGTIME_START();
	unsigned char z=v[len-1], y=v[0], e, DELTA=(unsigned char)0x00b9;
	int p, q, sum=0;
	q = (1 + minloop + (int)(16/keylen) + (int)(52/len))%255;
	//OutputDebug("\n\n==========\nkey:%s, len:%d, loop:%d, q:%u\n", key, keylen, minloop, q);
	while(q-- > 0)
	{
		sum += DELTA;
		e = (sum >> 2) & 3;
		//OutputDebug("q:%u, sum:%u, e:%u\n", q, sum, e);
		if(len == 1)
		{
			p = 0;
			y = 0;
			z = 0;
			unsigned char mx = tea_MX(y, z, sum, p, e, key, keylen);
			unsigned char xx0 = v[p];
			v[p] += tea_MX(y, z, sum, p, e, key, keylen);
		}
		else
		{
			for(p=0; p<len; p++)
			{
				y = v[( p + len - 1)%len];
				z = v[( p + len + 1)%len];
				v[p] += tea_MX(y, z, sum, p, e, key, keylen);
				//OutputDebug("p:%u, MX:%u(y:%u, z:%u), v:%u\n", p, tea_MX(y, z, sum, p, e, key, keylen), y, z, v[p]);
			}
		}
	}
	CC_LOGTIME_PRINT();
}
void Util::teaDeCrypt(char* v, int len, const char* key, int keylen, int minloop)
{
	CC_LOGTIME_START();
	unsigned char z=v[len-1], y=v[0], e, DELTA=(unsigned char)0x00b9;
	int p, q, sum=0;
	q = (1 + minloop + (int)(16/keylen) + (int)(52/len))%255;
	sum = q*DELTA;
	while(sum != 0)
	{
		q--;
		e = (sum >> 2) & 3;
		if(len == 1)
		{
			p = 0;
			y = 0;
			z = 0;
			v[p] -= tea_MX(y, z, sum, p, e, key, keylen);
		}
		else
		{
			for(p=len-1; p>=0; p--)
			{
				y = v[( p + len - 1)%len];
				z = v[( p + len + 1)%len];
				v[p] -= tea_MX(y, z, sum, p, e, key, keylen);
			}
		}
		sum -= DELTA;
	}
	CC_LOGTIME_PRINT();
}

void  Util::teaEnCrypt2(char* v, int len, const char* key, int keylen, int minloop)
{
	CC_LOGTIME_START();
	unsigned char z=v[len-1], y=v[0], e, DELTA=(unsigned char)0x00b9;
	int p, q, sum=0;
	q = (1 + minloop + (int)(16/keylen) + (int)(52/len))%255;
	while(q-- > 0)
	{
		sum += DELTA;
		e = (sum >> 2) & 3;
		if(len == 1)
		{
			p = 0;
			y = 0;
			z = 0;
			unsigned char k0 = (key[(p+minloop)%keylen]|(p^minloop))&0x00ff;
			v[p] = v[p]^k0;
			v[p] += tea_MX(y, z, sum, p, e, key, keylen);
		}
		else
		{
			for(p = 0; p < len; p++)
			{
				unsigned char k0 = (key[(p+minloop)%keylen]|(p^minloop))&0x00ff;
				v[p] = v[p]^k0;
			}
			for(p = 0; p < len; p++)
			{
				unsigned char k0 = (key[(p+minloop)%keylen]|(minloop^p))&0x00ff;
				int jump = 1+(key[minloop%keylen]^minloop)%(len-1);
				int pp = (p+jump)%len;
				unsigned char y0 = v[p] & ~k0;
				unsigned char y1 = v[p] & k0;
				unsigned char z0 = v[pp] & ~k0;
				unsigned char z1 = v[pp] & k0;

				v[p] = y0|z1;
				v[pp] = y1|z0;
			}
			for(p=0; p<len; p++)
			{
				y = v[( p + len - 1)%len];
				z = v[( p + len + 1)%len];
				v[p] += tea_MX(y, z, sum, p, e, key, keylen);
			}
		}
	}
	CC_LOGTIME_PRINT();
}
void Util::teaDeCrypt2(char* v, int len, const char* key, int keylen, int minloop)
{
	CC_LOGTIME_START();
	unsigned char z=v[len-1], y=v[0], e, DELTA=(unsigned char)0x00b9;
	int p, q, sum=0;
	q = (1 + minloop + (int)(16/keylen) + (int)(52/len))%255;
	sum = q*DELTA;
	while(sum != 0)
	{
		q--;
		e = (sum >> 2) & 3;
		if(len == 1)
		{
			p = 0;
			y = 0;
			z = 0;
			v[p] -= tea_MX(y, z, sum, p, e, key, keylen);
			unsigned char k0 = (key[(p+minloop)%keylen]|(p^minloop))&0x00ff;
			v[p] = v[p]^k0;
		}
		else
		{
			for(p=len-1; p>=0; p--)
			{
				y = v[( p + len - 1)%len];
				z = v[( p + len + 1)%len];
				v[p] -= tea_MX(y, z, sum, p, e, key, keylen);
			}
			for(p=len-1; p>=0; p--)
			{
				unsigned char k0 = (key[(p+minloop)%keylen]|(minloop^p))&0x00ff;
				int jump = 1+(key[minloop%keylen]^minloop)%(len-1);
				int pp = (p+jump)%len;
				unsigned char y0 = v[p] & ~k0;
				unsigned char y1 = v[p] & k0;
				unsigned char z0 = v[pp] & ~k0;
				unsigned char z1 = v[pp] & k0;

				v[p] = y0|z1;
				v[pp] = y1|z0;
			}
			for(p = 0; p < len; p++)
			{
				unsigned char k0 = (key[(p+minloop)%keylen]|(p^minloop))&0x00ff;
				v[p] = v[p]^k0;
			}
		}
		sum -= DELTA;
	}
	CC_LOGTIME_PRINT();
}

//used only for treate myself's regkey
void  Util::araCrype(const char* inbuf, int len, const char* key, int keylen, char** outbuf)
{
	CC_LOGTIME_START();
	//invoid wchar_t
	char* des = (char *)malloc(len + 3);
	if(des == NULL)
	{
		return;
	}
	if(*outbuf != NULL)
	{
		free(*outbuf);
	}
	*outbuf = (char*)des;
	memset(des, '\0', len + 3);
	memcpy(des, inbuf, len);
	ARACrypt ara;
	ara.SetKey(key, keylen);
	ara.TransformString(des, len);
	CC_LOGTIME_PRINT();
}
void  Util::araCrype(char* inoutbuf, int len, const char* key, int keylen)
{
	CC_LOGTIME_START();
	ARACrypt ara;
	ara.SetKey(key, keylen);
	ara.TransformString(inoutbuf, len);
	CC_LOGTIME_PRINT();
}

/*
	wchar_t wch[] = {L'1', L'2', L'a', L'b', 0x6c49,0x8bed,0x6f22,0x8a9e,0}; //chinese:hanyu,japanese:kanngo
	cc::Str sSrc = cc::Str(wch);
	cc::Str sDes;
	char*yy = NULL;
	cc::Log::debug(_T("[xx:%s]"), (TCHAR*)sSrc.GetBuf());
	int nlen = cc::Util::codeB64Encode((char*)sSrc.GetBuf(), (int)sSrc.GetLength() * (sSrc.IsUnicode() ? 2 : 1), &yy);
	cc::Log::debug("codeB64Encode[yy:%s]", yy);
	char*zz = NULL;
	cc::Util::codeB64Decode(yy, nlen, &zz);
	cc::Log::debug(_T("codeB64Decode[zz:%s]"), zz);

	sSrc.ToMByte(LangCode::UTF8);
	nlen = cc::Util::codeB64Encode((char*)sSrc.GetBuf(), sSrc.GetLength(), &yy);
	cc::Log::debug("codeB64Encode[yy:%s]", yy);
	cc::Util::codeB64Decode(yy, nlen, &zz);
	sDes.ValueOf(zz, LangCode::UTF8);
	cc::Log::debug(_T("codeB64Decode[zz:%s]"), (TCHAR*)sDes.GetBuf());

	nlen = cc::Util::codeURLEncode((char*)sSrc.GetBuf(), sSrc.GetLength(), &yy);
	cc::Log::debug("codeURLEncode[yy:%s]", yy);
	cc::Util::codeURLDecode(yy, nlen, &zz);
	sDes.ValueOf(zz, LangCode::UTF8);
	cc::Log::debug(_T("codeURLDecode[zz:%s]"), (TCHAR*)sDes.GetBuf());

	nlen = cc::Util::codeMyEncode((char*)sSrc.GetBuf(), sSrc.GetLength(), &yy);
	cc::Log::debug("codeMyEncode[yy:%s]", yy);
	cc::Util::codeMyDecode(yy, nlen, &zz);
	sDes.ValueOf(zz, LangCode::UTF8);
	cc::Log::debug(_T("codeMyDecode[zz:%s]"), (TCHAR*)sDes.GetBuf());
*/

#define CC_CRYPTCODE_KEY "Cq6Ih,9YA.b8FP0@xuS-igo=XZgN#LsQFjl,BOkr"
cc::Str Util::EnCryptEnCode(const char* inbuf, int len, const char* key, int keylen, int minloop, LangCode::Type toLangCode)
{
	if(len < 0)
	{
		len = (int)strlen(inbuf);
	}
	if(key == NULL)
	{
		key = CC_CRYPTCODE_KEY;
		keylen = 40;
	}
	else if(keylen < 0)
	{
		keylen = (int)strlen(key);
	}
	cc::Str sRet = cc::Str(toLangCode);
	char*outbuf = NULL;
	char*outbuf2 = NULL;
	teaEnCrypt(inbuf, len, key, keylen, minloop, &outbuf);
	len = codeB64Encode(outbuf, len, &outbuf2);
	sRet.TakeoverPointer(&outbuf2, len, LangCode::SYSTEM);
	free(outbuf);
	return sRet;
}
cc::Str Util::DeCryptDeCode(const char* inbuf, int len, const char* key, int keylen, int minloop, LangCode::Type toLangCode, LangCode::Type orgLangCode)
{
	if(len < 0)
	{
		len = (int)strlen(inbuf);
	}
	if(key == NULL)
	{
		key = CC_CRYPTCODE_KEY;
		keylen = 40;
	}
	else if(keylen < 0)
	{
		keylen = (int)strlen(key);
	}
	cc::Str sRet = cc::Str(toLangCode);
	char*outbuf = NULL;
	len = codeB64Decode(inbuf, len, &outbuf);
	if(len < 1) {
		return sRet;
	}
	teaDeCrypt(outbuf, len, key, keylen, minloop);
	sRet.TakeoverPointer(&outbuf, len, orgLangCode);
	return sRet;
}

//need free memory
int Util::codeASC2UNI(wchar_t **desNeedFree, const char *src, LangCode::Type fromLangCode)
{
	int nRet = -1;
	if(*desNeedFree != NULL)
	{
		free(*desNeedFree);
		*desNeedFree = NULL;
	}
	if(src != NULL)
	{
		nRet = MultiByteToWideChar((int)fromLangCode, 0, src, -1, NULL, 0);
		if(nRet >= 0)
		{
			*desNeedFree = (wchar_t*)malloc((nRet + 1) * sizeof(wchar_t));
			memset(*desNeedFree, '\0', (nRet + 1) * sizeof(wchar_t));
			nRet = MultiByteToWideChar((int)fromLangCode, 0, src, -1, *desNeedFree, nRet);
		}
	}
	return nRet;
}

//need free memory
int Util::codeUNI2ASC(char **desNeedFree, LangCode::Type toLangCode, const wchar_t *src)
{
	int nRet = -1;
	if(*desNeedFree != NULL)
	{
		free(*desNeedFree);
		*desNeedFree = NULL;
	}
	if(src != NULL)
	{
		nRet = WideCharToMultiByte((int)toLangCode, 0, src, -1, NULL, 0, NULL, NULL);
		if(nRet >= 0)
		{
			*desNeedFree = (char*)malloc((nRet + 1) * sizeof(char));
			memset(*desNeedFree, '\0', (nRet + 1) * sizeof(char));
			nRet = WideCharToMultiByte((int)toLangCode, 0, src, -1, *desNeedFree, nRet, NULL, NULL);
		}
	}
	return nRet;
}

//<FORM NAME="frmMain" ACTION="/servlet/lzca.pc.cfr001.servlet.CFr00101" METHOD="POST" TARGET="_parent">
//<INPUT TYPE="HIDDEN" NAME="SESN" VALUE="43OK10N4Z7T1DL9018928GMX9BIUXQUI853">
//[main]\r\nkey1=abc\r\nkey2=cba
//if end is NULL then to end of buf
//return the pos of next read(end of "end")
int Util::GetHtmlTag(const char* buf, int from, const char* start, int nStartLen, const char* end, int nEndLen, cc::Str& sValue, const char* startend, int nStartEndLen, LangCode::Type fromLangCode, bool IgnoreCase)
{
	if(buf == NULL || from < 0 || start == NULL)
	{
		return -1;
	}

	//const char* tc = strstr(buf + from, start);
	if(nStartLen <= 0)
	{
		nStartLen = (int)strlen(start);
	}
	int t1 = cc::Str::Search(buf, from, -1, start, nStartLen, IgnoreCase);
	if(t1 < 0)
	{
		return -1;
	}
	//tc += nStartLen;
	t1 += nStartLen;
	if(startend != NULL)
	{
		//tc = strstr(tc, startend);
		if(nStartEndLen <= 0)
		{
			nStartEndLen = (int)strlen(startend);
		}
		t1 = cc::Str::Search(buf, t1, -1, startend, nStartEndLen, IgnoreCase);
		if(t1 < 0)
		{
			return -1;
		}
		t1 += nStartEndLen;
	}
	if(end != NULL)
	{
		//const char* tc2 = strstr(tc, end);
		if(nEndLen <= 0)
		{
			nEndLen = (int)strlen(end);
		}
		int t2 = cc::Str::Search(buf, t1, -1, end, nEndLen, IgnoreCase);
		if(t2 < 0)
		{
			return -1;
		}
		sValue.ValueOf(buf, t1, t2 - t1, fromLangCode);
		return (t2 + nEndLen);
	}
	sValue.ValueOf(buf, t1, fromLangCode);
	return (int)strlen(buf);
}

int Util::GetHtmlTag(const wchar_t* buf, int from, const wchar_t* start, int nStartLen, const wchar_t* end, int nEndLen, cc::Str& sValue, const wchar_t* startend, int nStartEndLen, bool IgnoreCase)
{
	if(buf == NULL || from < 0 || start == NULL)
	{
		return -1;
	}

	//const char* tc = strstr(buf + from, start);
	if(nStartLen <= 0)
	{
		nStartLen = (int)wcslen(start);
	}
	int t1 = cc::Str::Search(buf, from, -1, start, nStartLen, IgnoreCase);
	if(t1 < 0)
	{
		return -1;
	}
	//tc += nStartLen;
	t1 += nStartLen;
	if(startend != NULL)
	{
		//tc = strstr(tc, startend);
		if(nStartEndLen <= 0)
		{
			nStartEndLen = (int)wcslen(startend);
		}
		t1 = cc::Str::Search(buf, t1, -1, startend, nStartEndLen, IgnoreCase);
		if(t1 < 0)
		{
			return -1;
		}
		t1 += nStartEndLen;
	}
	if(end != NULL)
	{
		//const char* tc2 = strstr(tc, end);
		if(nEndLen <= 0)
		{
			nEndLen = (int)wcslen(end);
		}
		int t2 = cc::Str::Search(buf, t1, -1, end, nEndLen, IgnoreCase);
		if(t2 < 0)
		{
			return -1;
		}
		sValue.ValueOf(buf, t1, t2 - t1);
		return (t2 + nEndLen);
	}
	sValue.ValueOf(buf, t1);
	return (int)wcslen(buf);
}

//========================================
//file&dir
Str Util::getSpecialFolderPath(int nCsidl, const TCHAR* tcDefaultPath)
{
	TCHAR buf[_MAX_PATH];
	memset(buf, 0x0, _MAX_PATH * sizeof(TCHAR));
	cc::Str sRetu;
	//Minimum operating systems	Windows 2000, Windows NT 4.0 with Internet Explorer 4.0, Windows 98, Windows 95 with Internet Explorer 4.0
	if(::SHGetSpecialFolderPath(NULL, buf, nCsidl, FALSE) == TRUE)
	{
		sRetu.ValueOf(buf);
		//err(_T("sRetu:%s"), (TCHAR*)sRetu.GetBuf());
	}
	if(sRetu.GetLength() <= 0 && tcDefaultPath != NULL)
	{
		sRetu.ValueOf(tcDefaultPath);
	}
	if(sRetu.GetLength() > 0 && !sRetu.EndsWith(_T("\\")))
	{
		sRetu.Append(_T("\\"));
	}
	return sRetu;
}

Str Util::getCurrentDirectory()
{
	Str retu;
	TCHAR tcBuf[_MAX_PATH + 1];

	memset(tcBuf, 0, (_MAX_PATH + 1) * sizeof(TCHAR));
	::GetCurrentDirectory(_MAX_PATH, tcBuf);
	retu.ValueOf(tcBuf);
	return retu;
}

Str Util::getStartupPath(bool isNoExeFileName, HMODULE hModule)
{
	Str retu;
	TCHAR tcBuf[_MAX_PATH + 1];

	memset(tcBuf, 0, (_MAX_PATH + 1) * sizeof(TCHAR));
	::GetModuleFileName(hModule, tcBuf, _MAX_PATH);
	retu.ValueOf(tcBuf);
	if(isNoExeFileName)
	{
		retu.Remove(retu.LastIndexOf(_T("\\")) + 1);
	}
	return retu;
}

Str Util::getStartupRelatePath(const TCHAR *fullpath, HMODULE hModule)
{
	if(fullpath != NULL && (*fullpath == _T('\\') || *fullpath == _T('/') || _tcschr(fullpath, _T(':')) == fullpath + 1))
	{
		return cc::Str(fullpath);
	}
	return getStartupPath(true, hModule).Append(fullpath);
}

Str Util::getUserLocalAppDataDir(bool isAddStartupPathLastSubdir, HMODULE hModule)
{
	//_T("C:\\Documents and Settings\\All Users\\Application Data\\")
	Str retu = cc::Util::getSpecialFolderPath(CSIDL_LOCAL_APPDATA);
	if(retu.GetLength() < 1)
	{
		//maybe winver < w2000
		cc::Str sWindir = cc::Util::GetEnv(_T("windir"));
		retu = sWindir + _T("Local Settings\\Application Data\\");
	}
	if(!retu.EndsWith(_T("\\")))
	{
		retu.Append(_T("\\"));
	}
	if(isAddStartupPathLastSubdir)
	{
		cc::Str sDir = cc::Util::getStartupPath(true, hModule);
		sDir = cc::Util::getLastSubDirName(sDir);
		retu = retu + sDir;
		if(!retu.EndsWith(_T("\\")))
		{
			retu.Append(_T("\\"));
		}
	}
	return retu;
}

/*
Str Util::getStartupPathForSave(bool isNoExeFileName, HMODULE hModule)
{
	Str retu;
	TCHAR tcBuf[_MAX_PATH + 1];

	memset(tcBuf, 0, (_MAX_PATH + 1) * sizeof(TCHAR));
	::GetModuleFileName(hModule, tcBuf, _MAX_PATH);
	retu.ValueOf(tcBuf);

	int bit64 = 0;
	tagWindowsVersion::Type wv = cc::Util::GetWindowsVersion(&bit64);
	bool isDataPath = false;
	if((int)wv >= (int)WindowsVersion::Windows2000)
	{
		//cc::Str sRet = cc::Util::getSpecialFolderPath(CSIDL_PROGRAM_FILESX86);
		cc::Str sRet = cc::Util::GetEnv("ProgramFiles");
		if(sRet.GetLength() > 0 && retu.StartsWith(sRet))
		{
			retu = retu.Substring(sRet.GetLength() + 1);
			isDataPath = true;
		}
		else
		{
			//sRet = cc::Util::getSpecialFolderPath(CSIDL_PROGRAM_FILES);
			sRet = cc::Util::GetEnv("ProgramFiles(x86)");
			if(sRet.GetLength() > 0 && retu.StartsWith(sRet))
			{
				retu = retu.Substring(sRet.GetLength() + 1);
				isDataPath = true;
			}
		}

		if(isDataPath)
		{
			//Windows 2000 and Windows Millennium Edition (Windows Me). 
			//nCsidl = CSIDL_LOCAL_APPDATA;
			//LOCALAPPDATA=C:\Users\user\AppData\Local
			sRet = cc::Util::getSpecialFolderPath(CSIDL_LOCAL_APPDATA);
			if(sRet.GetLength() < 1)
			{
				sRet = cc::Util::GetEnv("LOCALAPPDATA");
			}
			if(!sRet.EndsWith(_T("\\")))
			{
				sRet.Append(_T("\\"));
			}
			retu = sRet + retu;
			cc::Str sDir = cc::Util::getParentDirName(retu);
			cc::Util::dCreateAll(sDir);
			CC_TRACE((_T("Save Data Path:%s"), retu));
		}
	}

	if(isNoExeFileName)
	{
		retu.Remove(retu.LastIndexOf(_T("\\")) + 1);
	}
	return retu;
}
*/

Str Util::getTempPath()
{
	Str retu;
	TCHAR szTempPath[_MAX_PATH + 1];

	memset(szTempPath, 0, (_MAX_PATH + 1) * sizeof(TCHAR));
	::GetTempPath(_MAX_PATH, szTempPath);
	retu.ValueOf(szTempPath);
	if(retu.GetLength() > 0 && !retu.EndsWith(_T("\\")))
	{
		retu.Append(_T("\\"));
	}
	return retu;
}

Str Util::getTempFileName(TCHAR *prefix)
{
	Str retu;
	TCHAR szTempPath[_MAX_PATH + 1];
	TCHAR tcBuf[_MAX_PATH + 1];

	memset(szTempPath, 0, (_MAX_PATH + 1) * sizeof(TCHAR));
	memset(tcBuf, 0, (_MAX_PATH + 1) * sizeof(TCHAR));
	::GetTempPath(_MAX_PATH, szTempPath);
	::GetTempFileName(szTempPath, prefix, 0, tcBuf);
	retu.ValueOf(tcBuf);
	return retu;
}

//change relation path to absolutely
Str Util::getFullPath(const TCHAR *fullpath)
{
	cc::Str str;
	TCHAR tcBuf[_MAX_PATH + 1];
	if(fullpath != NULL && fullpath[0] != '\0')
	{
		memset(tcBuf, 0, (_MAX_PATH + 1) * sizeof(TCHAR));
		_tfullpath(tcBuf, fullpath, _MAX_PATH);
		str.ValueOf(tcBuf);
	}
	return str;
}

Str Util::getParentDirName(const TCHAR *fullpath)
{
	Str retu = getFullPath(fullpath);
	if(retu.EndsWith(_T("\\")) || retu.EndsWith(_T("/")))
	{
		retu.Remove(retu.GetLength() - 1);
	}
	return getFilePath((TCHAR*)retu.GetBuf());
}

Str Util::getLastSubDirName(const TCHAR *fullpath)
{
	Str retu = getFullPath(fullpath);
	if(retu.EndsWith(_T("\\")) || retu.EndsWith(_T("/")))
	{
		retu.Remove(retu.GetLength() - 1);
	}
	return getFileName((TCHAR*)retu.GetBuf());
}

Str Util::getFilePath(const TCHAR *fullpath)
{
	Str retu = getFullPath(fullpath);
	int npos1 = retu.LastIndexOf(_T("\\"));
	int npos2 = retu.LastIndexOf(_T("/"));
	if(npos2 > npos1)
	{
		npos1 = npos2;
	}
	if(npos1 >= 0)
	{
		retu.Remove(npos1 + 1);
	}
	else
	{
		retu.Append(_T("\\"));
	}
	return retu;
}

Str Util::getFileName(const TCHAR *fullpath)
{
	Str retu = getFullPath(fullpath);
	int npos1 = retu.LastIndexOf(_T("\\"));
	int npos2 = retu.LastIndexOf(_T("/"));
	if(npos2 > npos1)
	{
		npos1 = npos2;
	}
	if(npos1 >= 0)
	{
		retu = retu.Substring(npos1 + 1);
	}
	return retu;
}

Str Util::getFileNameNoExt(const TCHAR *fullpath)
{
	Str retu = getFullPath(fullpath);
	int npos1 = retu.LastIndexOf(_T("\\"));
	int npos2 = retu.LastIndexOf(_T("/"));
	if(npos2 > npos1)
	{
		npos1 = npos2;
	}
	int npos3 = retu.LastIndexOf(_T("."));
	if(npos1 >= 0 && npos3 > npos1)
	{
		return retu.Substring(npos1 + 1, npos3);
	}
	if(npos1 >= 0)
	{
		retu = retu.Substring(npos1 + 1);
	}
	return retu;
}
Str Util::getFileExt(const TCHAR *fullpath)
{
	Str retu = getFullPath(fullpath);
	int npos1 = retu.LastIndexOf(_T("\\"));
	int npos2 = retu.LastIndexOf(_T("/"));
	if(npos2 > npos1)
	{
		npos1 = npos2;
	}
	int npos3 = retu.LastIndexOf(_T("."));
	if(npos3 > npos1)
	{
		return retu.Substring(npos3 + 1);
	}
	return Str(_T(""));
}

//return the byte of read(TCHAR is as 2),buf can be null and if buflen is -1,then read all
int Util::fRead(void** buf, const TCHAR *fullpath, int readLen)
{
#if _DEBUG_
	cc::Log::trace(_T("[Util::fRead]Path:%s, readLen:%d"), fullpath, readLen);
	CC_LOGTIME_START();
#endif
	if(fullpath == NULL || !fExist(fullpath))
	{
		return -1;
	}
	HANDLE hFile = ::CreateFile(
		fullpath,
		GENERIC_READ, 				// DWORD dwDesiredAccess,
		FILE_SHARE_READ | FILE_SHARE_WRITE,			// DWORD dwShareMode
		NULL,						// securite
		OPEN_EXISTING,				// create type
		FILE_FLAG_SEQUENTIAL_SCAN,	// attribute
		NULL						// 
	);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		return -1;
	}

	DWORD nFileSizeHigh;
	DWORD nFileSize = ::GetFileSize(hFile, &nFileSizeHigh);
	CC_TRACE((_T("[Util::fRead]FileSize:%d, fullpath:%s"), nFileSize, fullpath));
	//file is so big,error!
	CC_ASSERT(nFileSizeHigh <= 0);
	DWORD dwReadLen = (DWORD)readLen;
	if(dwReadLen <=0 || dwReadLen > nFileSize)
	{
		dwReadLen = nFileSize;
	}
	//unsigned long readSize;
	//if(readLen <= 0)
	//{
	//	readLen = SetFilePointer(hFile, 0, NULL, FILE_END);
	//	SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
	//}
	if(dwReadLen <= 0)
	{
		return (int)dwReadLen;
	}
	if(*buf != NULL)
	{
		free(*buf);
		*buf = NULL;
	}
	*buf = malloc((dwReadLen + 4) * sizeof(char));
	if(*buf == NULL)
	{
		::CloseHandle(hFile);
		return -1;
	}
	memset(*buf, '\0', (dwReadLen + 4) * sizeof(char));
	if(::ReadFile(hFile, *buf, dwReadLen, &nFileSizeHigh, NULL) == FALSE)
	{
		::CloseHandle(hFile);
		return -1;
	}
	::CloseHandle(hFile);
	CC_LOGTIME_PRINT();
	return dwReadLen;
}

//if langcode of buf is diff with fileLangCode, may lost data if has '\0' in file!
//return the byte of read(TCHAR is as 2)
int Util::fRead(Str &buf, const TCHAR *fullpath, LangCode::Type fileLangCode, int readLen)
{
#if _DEBUG_
	cc::Log::trace(_T("[Util::fRead]Path:%s, fileLangCode:%d, readLen:%d"), fullpath, fileLangCode, readLen);
	CC_LOGTIME_START();
#endif
	if(fullpath == NULL || !fExist(fullpath))
	{
		return -1;
	}
	HANDLE hFile = ::CreateFile(
		fullpath,
		GENERIC_READ, 				// DWORD dwDesiredAccess,
		FILE_SHARE_READ | FILE_SHARE_WRITE,			// DWORD dwShareMode
		NULL,						// securite
		OPEN_EXISTING,				// create type
		FILE_FLAG_SEQUENTIAL_SCAN,	// attribute
		NULL						// 
	);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		return -1;
	}

	DWORD nFileSizeHigh;
	DWORD nFileSize = ::GetFileSize(hFile, &nFileSizeHigh);
	CC_TRACE((_T("[Util::fRead]FileSize:%d, fullpath:%s"), nFileSize, fullpath));
	//file is so big,error!
	CC_ASSERT(nFileSizeHigh <= 0);
	//unsigned long readSize;
	//UINT nflen = SetFilePointer(hFile, 0, NULL, FILE_END);
	//SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
	if(nFileSize > 1)
	{
		unsigned char p[3];
		memset(p, '\0', 3);
		if(::ReadFile(hFile, p, 2, &nFileSizeHigh, NULL) == false)
		{
			::CloseHandle(hFile);
			return -1;
		}
		if(p[0] == 0xFF && p[1] == 0xFE)
		{
			//litter
			fileLangCode = LangCode::UNICODE16;
			nFileSize -= 2;
		}
		else if(p[0] == 0xFE && p[1] == 0xFF)
		{
			//big
			fileLangCode = LangCode::UNICODE_BIGENDIAN;
			nFileSize -= 2;
		}
		else if(p[0] == 0xEF && p[1] == 0xBB && nFileSize > 2)
		{
			memset(p, '\0', 3);
			if(::ReadFile(hFile, p, 1, &nFileSizeHigh, NULL) == false)
			{
				::CloseHandle(hFile);
				return false;
			}
			if(p[0] == 0xBF)
			{
				fileLangCode = LangCode::UTF8;
				nFileSize -= 3;
			}
			else
			{
				SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
				//if(fileLangCode == LangCode::UNICODE16 || fileLangCode == LangCode::UNICODE_BIGENDIAN
				//	|| fileLangCode == LangCode::UTF8)
				//{
				//	fileLangCode = LangCode::SYSTEM;
				//}
			}
		}
		else
		{
			SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
			//if(fileLangCode == LangCode::UNICODE16 || fileLangCode == LangCode::UNICODE_BIGENDIAN
			//	|| fileLangCode == LangCode::UTF8)
			//{
			//	fileLangCode = LangCode::SYSTEM;
			//}
		}
	}
	DWORD dwReadLen = (DWORD)readLen;
	if(dwReadLen <=0 || dwReadLen > nFileSize)
	{
		dwReadLen = nFileSize;
	}
	//if(readLen <= 0 || readLen > (int)nflen)
	//{
	//	readLen = nflen;
	//}
	void *des = malloc((dwReadLen + 4) * sizeof(char));
	memset(des, '\0', (dwReadLen + 4) * sizeof(char));
	if(::ReadFile(hFile, des, dwReadLen, &nFileSizeHigh, NULL) == false)
	{
		::CloseHandle(hFile);
		return -1;
	}
	::CloseHandle(hFile);
	if(buf.GetLangCode() == fileLangCode)
	{
		if(fileLangCode == LangCode::UNICODE16)
		{
			CC_ASSERT(dwReadLen%2 == 0);
			buf.TakeoverPointer((wchar_t**)&des, dwReadLen/2);
		}
		else
		{
			buf.TakeoverPointer((char**)&des, dwReadLen, fileLangCode);
		}
	}
	else
	{
		if(fileLangCode == LangCode::UNICODE16)
		{
			CC_ASSERT(dwReadLen%2 == 0);
			buf.ValueOf((wchar_t*)des);
		}
		else
		{
			buf.ValueOf((char*)des, fileLangCode);
		}
		free(des);
		des = NULL;
	}
	CC_LOGTIME_PRINT();
	return dwReadLen;
}

bool Util::fWrite(const void* buf, DWORD len, LangCode::Type langCode, const TCHAR *fullpath, bool isAppend, bool isHeadBOM)
{
#if _DEBUG_
	cc::Log::trace(_T("[Util::fWrite]Path:%s, langCode:%d, len:%d, isAppend:%d"), fullpath, langCode, len, isAppend);
	CC_LOGTIME_START();
#endif
	if(fullpath == NULL)
	{
		return false;
	}
	DWORD openflg;
	if(isAppend)
	{
		openflg = OPEN_ALWAYS;
	}
	else
	{
		fDel(fullpath);
		openflg = CREATE_ALWAYS;
	}
	HANDLE hFile = ::CreateFile(
		fullpath,
		GENERIC_WRITE, 				// DWORD dwDesiredAccess,
		FILE_SHARE_WRITE,			// DWORD dwShareMode
		NULL,						// securite
		openflg,				// create type
		FILE_FLAG_SEQUENTIAL_SCAN,	// attribute
		NULL						// 
	);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		cc::Log::trace(_T("CreateFile NG."));
		return false;
	}
	unsigned long writeSize;
	UINT nSize = SetFilePointer(hFile, 0, NULL, FILE_END);
	if(nSize == 0)
	{
		if(isHeadBOM)
		{
			//write bom
			unsigned char ch[3];
			int n = 0;
			if(langCode == LangCode::UTF8)
			{
				ch[0] = 0xEF;
				ch[1] = 0xBB;
				ch[2] = 0xBF;
				n = 3;
			}
			else if(langCode == LangCode::UNICODE16)
			{
				ch[0] = 0xFF;
				ch[1] = 0xFE;
				n = 2;
			}
			else if(langCode == LangCode::UNICODE_BIGENDIAN)
			{
				//big-endian:0xFEFF, little-endian:0xFFFE
				ch[0] = 0xFE;
				ch[1] = 0xFF;
				n = 2;
			}
			if(n > 0)
			{
				if(::WriteFile(hFile, ch, n, &writeSize, NULL) == FALSE)
				{
					::CloseHandle(hFile);
					return false;
				}
			}
		}
	}

	if(len > 0)
	{
		if(::WriteFile(hFile, buf, len, &writeSize, NULL) == FALSE)
		{
			::CloseHandle(hFile);
			return false;
		}
	}
	FlushFileBuffers(hFile);
	::CloseHandle(hFile);
	CC_LOGTIME_PRINT();
	return true;
}

bool Util::fWrite(const Str &buf, const TCHAR *fullpath, bool isAppend)
{
	DWORD len = 0;
	if(buf.GetLangCode() == cc::LangCode::UNICODE16)
	{
		len = buf.GetLength() * sizeof(wchar_t);
	}
	else
	{
		len = buf.GetLength() * sizeof(char);
	}
	return fWrite((const char*)buf.GetBuf(), len, buf.GetLangCode(), fullpath, isAppend);
}

bool Util::fAppend(Str &buf, const TCHAR *fullpath)
{
	return fWrite(buf, fullpath, true);
}

bool Util::fCopy(const TCHAR *fromFullpath, const TCHAR *toFullpath, bool bFailIfExists)
{
	int retu = CopyFile(fromFullpath, toFullpath, bFailIfExists ? TRUE : FALSE);
	return (retu > 0);
}

bool Util::fMove(const TCHAR *fromFullpath, const TCHAR *toFullpath, bool bFailIfExists)
{
	//moves the file or folder named source to the location destination, regardless of the read-only attribute of destination.
	int retu = MoveFileEx(fromFullpath, toFullpath, 
		MOVEFILE_COPY_ALLOWED | MOVEFILE_WRITE_THROUGH | (bFailIfExists ? 0: MOVEFILE_REPLACE_EXISTING));
	return (retu > 0);
}

bool Util::fDel(const TCHAR *fullpath)
{
	if(fullpath == NULL)
	{
		return false;
	}
	::SetFileAttributes(fullpath, FILE_ATTRIBUTE_NORMAL);
	int retu = ::DeleteFile(fullpath);
	return (retu > 0);
}

bool Util::fExist(const TCHAR *fullpath)
{
	if(fullpath == NULL || _tcscmp(_T(""), fullpath) == 0)
	{
		return false;
	}
	//if end with "\"
	int nLen = (int)_tcslen(fullpath);
	if('\\' == fullpath[nLen - 1] || '/' == fullpath[nLen - 1])
	{
		return false;
	}

	//20110102 -1 for w7
	//if(-1 == _taccess(fullpath, 0))
	//{
	//	return false;
	//}

	bool isExist = false;
	WIN32_FIND_DATA findData = {0};
	//lstrcpy(findData.cFileName, fullpath);
	HANDLE hFindFile = ::FindFirstFile(fullpath, (WIN32_FIND_DATA*)&findData);
	//if(hFindFile != INVALID_HANDLE_VALUE && (findData.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE))
	if(hFindFile != INVALID_HANDLE_VALUE && (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
	{
		if(getFileName(fullpath).Equals(findData.cFileName, true))
		{
			isExist = true;
		}
	}

	//delete pFindData;
	FindClose(hFindFile);
	return isExist;
}

DWORD Util::fSize(const TCHAR *fullpath)
{
	HANDLE hFile; // the file handle
	DWORD dwFileSize; // temporary storage for file sizes
	hFile = CreateFile(fullpath, FILE_READ_EA, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		return -1;
	}
	dwFileSize = GetFileSize(hFile, NULL);
	CloseHandle(hFile);
	return dwFileSize;
}

bool Util::fReadonly(const TCHAR *fullpath, bool isReadonly)
{
	if(isReadonly)
	{
		_tchmod(fullpath, S_IREAD);
	}
	else
	{
		_tchmod(fullpath, _S_IWRITE);
	}
	return true;
}

bool Util::fFindFirst(const TCHAR *fullpath, DWORD *dwFileAttributes, FILETIME *ftLastWriteTime, SYSTEMTIME *stLastWriteTime, DWORD *nFileSizeHigh, DWORD *nFileSizeLow)
{
	HANDLE hFind;
	WIN32_FIND_DATA fd = {0};

	hFind = FindFirstFile(fullpath, &fd);
	if(hFind == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	if(dwFileAttributes != NULL)
	{
		//FILE_ATTRIBUTE_DIRECTORY
		*dwFileAttributes = fd.dwFileAttributes;
	}
	if(ftLastWriteTime != NULL)
	{
		FileTimeToLocalFileTime(&fd.ftLastWriteTime, ftLastWriteTime);
	}
	if(stLastWriteTime != NULL)
	{
		FILETIME ft;
		FileTimeToLocalFileTime(&fd.ftLastWriteTime, &ft);
		FileTimeToSystemTime(&ft, stLastWriteTime);
	}
	if(nFileSizeHigh != NULL)
	{
		*nFileSizeHigh = fd.nFileSizeHigh;
	}
	if(nFileSizeLow != NULL)
	{
		*nFileSizeLow = fd.nFileSizeLow;
	}
	FindClose(hFind);
	return true;
	//struct _stat stat;
	//int nRet = _tstat(fullpath, &stat);
	//return (nRet == 0);
}

bool Util::dCopy(const TCHAR *fromFullpath, const TCHAR *toFullpath)
{
	if(fromFullpath == NULL || toFullpath == NULL)
	{
		return false;
	}
	if(!dExist(fromFullpath))
	{
		return false;
	}
	if(!dExist(toFullpath))
	{
		if(!dCreate(toFullpath))
		{
			return false;
		}
	}
	TCHAR tcFrom[_MAX_PATH + 1];
	TCHAR tcTo[_MAX_PATH + 1];
	memset(tcFrom, 0, (_MAX_PATH + 1) * sizeof(TCHAR));
	memset(tcTo, 0, (_MAX_PATH + 1) * sizeof(TCHAR));
	//_stprintf(tcFrom, _T("%s\\*.*"), fromFullpath);
	cc::Buf::Format(tcFrom, _MAX_PATH + 1, _T("%s\\*.*"), fromFullpath);

	WIN32_FIND_DATA findData = {0};
	//lstrcpy(findData.cFileName, tcFrom);
	cc::Buf::Format(findData.cFileName, _MAX_PATH, tcFrom);
	HANDLE hFindFile = ::FindFirstFile(tcFrom, (WIN32_FIND_DATA*)&findData);

	int nRes = 1;
	bool isOK = true;
	while(hFindFile != INVALID_HANDLE_VALUE && nRes > 0)
	{
		//turn to fullpath
		//_stprintf(tcFrom, _T("%s\\%s"), fromFullpath, findData.cFileName);
		cc::Buf::Format(tcFrom, _MAX_PATH + 1, _T("%s\\%s"), fromFullpath, findData.cFileName);
		//_stprintf(tcTo, _T("%s\\%s"), toFullpath, findData.cFileName);
		cc::Buf::Format(tcTo, _MAX_PATH + 1, _T("%s\\%s"), toFullpath, findData.cFileName);
		if(findData.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE)
		{
			if(CopyFile(tcFrom, tcTo, false) <= 0)
			{
				isOK = false;
				break;
			}
		}
		else
		{
			//if(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			if(findData.cFileName != NULL && _tcscmp(findData.cFileName, _T(".")) != 0 && _tcscmp(findData.cFileName, _T("..")) != 0)
			{
				//self loop
				if(!dCopy(tcFrom, tcTo))
				{
					isOK = false;
					break;
				}
			}
		}
		nRes = ::FindNextFile(hFindFile, (LPWIN32_FIND_DATA)&findData);
	}

	FindClose(hFindFile);
	return isOK;
}

bool Util::dMove(const TCHAR *fromFullpath, const TCHAR *toFullpath)
{
	if(fromFullpath == NULL || toFullpath == NULL)
	{
		return false;
	}
	if(!dExist(fromFullpath))
	{
		return false;
	}
	if(!dExist(toFullpath))
	{
		if(!dCreate(toFullpath))
		{
			return false;
		}
	}
	TCHAR tcFrom[_MAX_PATH + 1];
	TCHAR tcTo[_MAX_PATH + 1];
	memset(tcFrom, 0, (_MAX_PATH + 1) * sizeof(TCHAR));
	memset(tcTo, 0, (_MAX_PATH + 1) * sizeof(TCHAR));

	//_stprintf(tcFrom, _T("%s\\*.*"), fromFullpath);
	cc::Buf::Format(tcFrom, _MAX_PATH + 1, _T("%s\\*.*"), fromFullpath);
	WIN32_FIND_DATA findData = {0};
	//lstrcpy(findData.cFileName, tcFrom);
	cc::Buf::Cpy(findData.cFileName, _MAX_PATH, tcFrom);
	HANDLE hFindFile = ::FindFirstFile(tcFrom, (WIN32_FIND_DATA*)&findData);

	int nRes = 1;
	bool isOK = true;
	while(hFindFile != INVALID_HANDLE_VALUE && nRes > 0)
	{
		//turn to fullpath
		//_stprintf(tcFrom, _T("%s\\%s"), fromFullpath, findData.cFileName);
		cc::Buf::Format(tcFrom, _MAX_PATH + 1, _T("%s\\%s"), fromFullpath, findData.cFileName);
		//_stprintf(tcTo, _T("%s\\%s"), toFullpath, findData.cFileName);
		cc::Buf::Format(tcTo, _MAX_PATH + 1, _T("%s\\%s"), toFullpath, findData.cFileName);
		if(findData.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE)
		{
			int retu = MoveFileEx(tcFrom, tcTo, MOVEFILE_COPY_ALLOWED | MOVEFILE_WRITE_THROUGH | MOVEFILE_REPLACE_EXISTING);
			if(retu <= 0)
			{
				isOK = false;
				break;
			}
		}
		else
		{
			//if(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			if(findData.cFileName != NULL && _tcscmp(findData.cFileName, _T(".")) != 0 && _tcscmp(findData.cFileName, _T("..")) != 0)
			{
				//self loop
				if(!dMove(tcFrom, tcTo))
				{
					isOK = false;
					break;
				}
			}
		}
		nRes = ::FindNextFile(hFindFile, (LPWIN32_FIND_DATA)&findData);
	}
	FindClose(hFindFile);

	//must move dir after close it
	if(RemoveDirectory(fromFullpath) <= 0)
	{
		isOK = false;
	}
	return isOK;
}

bool Util::dCreate(const TCHAR *fullpath)
{
	if(fullpath == NULL)
	{
		return false;
	}
	if(dExist(fullpath))
	{
		return true;
	}
	return (::CreateDirectory(fullpath, NULL) == TRUE);
}

bool Util::dCreateAll(const TCHAR *fullpath)
{
	if(fullpath == NULL)
	{
		return false;
	}
	if(dExist(fullpath))
	{
		return true;
	}
	cc::Str sParent = getParentDirName(fullpath);
	if(!dCreateAll(sParent))
	{
		return false;
	}
	return (::CreateDirectory(fullpath, NULL) == TRUE);
}

bool Util::dDel(const TCHAR *fullpath, bool isDelSubFilesDirs)
{
	if(fullpath == NULL || fullpath[0] == '\0')
	{
		return false;
	}

	if(isDelSubFilesDirs)
	{
		TCHAR tcBuf[_MAX_PATH + 1];
		memset(tcBuf, 0, (_MAX_PATH + 1) * sizeof(TCHAR));
		//_stprintf(tcBuf, _T("%s\\*.*"), fullpath);
		cc::Buf::Format(tcBuf, _MAX_PATH + 1, _T("%s\\*.*"), fullpath);
		WIN32_FIND_DATA findData = {0};
		lstrcpy(findData.cFileName, tcBuf);
		HANDLE hFindFile = ::FindFirstFile(tcBuf, (WIN32_FIND_DATA*)&findData);

		int nRes = 1;
		while(hFindFile != INVALID_HANDLE_VALUE && nRes > 0)
		{
			//turn to fullpath
			//_stprintf(tcBuf, _T("%s\\%s"), fullpath, findData.cFileName);
			memset(tcBuf, 0, (_MAX_PATH + 1) * sizeof(TCHAR));
			cc::Buf::Format(tcBuf, _MAX_PATH + 1, _T("%s\\%s"), fullpath, findData.cFileName);
			if(findData.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE)
			{
				if(!fDel(tcBuf))
				{
					break;
				}
			}
			else
			{
				//if(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				if(findData.cFileName != NULL && _tcscmp(findData.cFileName, _T(".")) != 0 && _tcscmp(findData.cFileName, _T("..")) != 0)
				{
					//self loop
					if(!dDel(tcBuf, isDelSubFilesDirs))
					{
						break;
					}
				}
			}
			nRes = ::FindNextFile(hFindFile, (LPWIN32_FIND_DATA)&findData);
		}

		FindClose(hFindFile);
	}

	int retu = RemoveDirectory(fullpath);
	return (retu > 0);
}

//auto add \ to end
bool Util::dExist(const TCHAR *fullpath)
{
	if(fullpath == NULL || _tcscmp(_T(""), fullpath) == 0)
	{
		return false;
	}
	//add "\" to last
	int nLen = (int)_tcslen(fullpath);
	if('\\' != fullpath[nLen - 1])
	{
		cc::Str sPath = cc::Str(fullpath).Append(_T("\\"));
		//TCHAR szNew[nLen + 3];
		//lstrcpy(szNew, fullpath);
		//_tcscat(szNew, _T("\\"));
		if(0 == _taccess((const TCHAR*)sPath.GetBuf(), 0))
		{
			return true;
		}
	}
	else
	{
		if(0 == _taccess(fullpath, 0))
		{
			return true;
		}
	}
	return false;
	//if(fullpath == NULL || _tcscmp(_T(""), fullpath) == 0)
	//{
	//	return false;
	//}
	//Str lastSubDir = getLastSubDirName(fullpath);
	//WIN32_FIND_DATA *pFindData = new WIN32_FIND_DATA;
	//lstrcpy(pFindData->cFileName, fullpath);
	////must remove last "\"
	//if(pFindData->cFileName[_tcslen(pFindData->cFileName) - 1] == _T('\\') ||
	//	pFindData->cFileName[_tcslen(pFindData->cFileName) - 1] == _T('/'))
	//{
	//	pFindData->cFileName[_tcslen(pFindData->cFileName) - 1] = _T('\0');
	//}
	//HANDLE hFindFile = ::FindFirstFile(pFindData->cFileName, (WIN32_FIND_DATA*)pFindData);

	//bool isExist = false;
	//if(hFindFile != INVALID_HANDLE_VALUE && (pFindData->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
	//{
	//	if(lastSubDir.Equals(pFindData->cFileName, true))
	//	//if(_tcscmp((TCHAR*)lastSubDir.GetBuf(), pFindData->cFileName) == 0)
	//	{
	//		isExist = true;
	//	}
	//}

	//delete pFindData;
	//FindClose(hFindFile);
	//return isExist;
}

//seach what you given,if end with "\\",then get nothing
int Util::getFiles(cc::List<cc::Str> &arrFileName, const TCHAR *path_with_wildcard, bool isGetOnlyFileName)
{
	Str parentPath = isGetOnlyFileName ? Str() : getParentDirName(path_with_wildcard);

	WIN32_FIND_DATA findData = {0};
	lstrcpy(findData.cFileName, path_with_wildcard);
	HANDLE hFindFile = ::FindFirstFile(path_with_wildcard, (WIN32_FIND_DATA*)&findData);

	int nCount = 0;
	int nRes = 1;
	while(hFindFile != INVALID_HANDLE_VALUE && nRes > 0)
	{
		//modify 20080206
		if(!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			if(findData.cFileName != NULL && _tcscmp(findData.cFileName, _T(".")) != 0 && _tcscmp(findData.cFileName, _T("..")) != 0)
			{
				nCount++;
				if(isGetOnlyFileName)
				{
					arrFileName.Add(Str(findData.cFileName));
				}
				else
				{
					arrFileName.Add(parentPath + findData.cFileName);
				}
			}
		}
		nRes = ::FindNextFile(hFindFile, (LPWIN32_FIND_DATA)&findData);
	}

	FindClose(hFindFile);
	return nCount;
}

//seach what you given,if end with "\\",then only return this path if exist
int Util::getDirs(cc::List<cc::Str> &arrFileName, const TCHAR *path_with_wildcard, bool isGetOnlyFileName)
{
	Str parentPath = isGetOnlyFileName ? Str() : getFilePath(path_with_wildcard);

	WIN32_FIND_DATA findData = {0};
	lstrcpy(findData.cFileName, path_with_wildcard);
	HANDLE hFindFile = ::FindFirstFile(path_with_wildcard, (WIN32_FIND_DATA*)&findData);

	int nCount = 0;
	int nRes = 1;
	while(hFindFile != INVALID_HANDLE_VALUE && nRes > 0)
	{
		if(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if(findData.cFileName != NULL && _tcscmp(findData.cFileName, _T(".")) != 0 && _tcscmp(findData.cFileName, _T("..")) != 0)
			{
				nCount++;
				if(isGetOnlyFileName)
				{
					arrFileName.Add(Str(findData.cFileName));
				}
				else
				{
					arrFileName.Add(parentPath + findData.cFileName + _T("\\"));
				}
			}
		}
		nRes = ::FindNextFile(hFindFile, (LPWIN32_FIND_DATA)&findData);
	}

	FindClose(hFindFile);
	return nCount;
}

//http://msdn2.microsoft.com/en-us/library/ms646960.aspx
//sample Filters:_T("HTM Files(*.htm,*.hmf)\0*.htm;*.hmf\0HTML Files (*.html)\0*.html\0")
//sample Ext:_T("txt")
cc::Str Util::selectOpenFile(const TCHAR *location, const TCHAR *filter, const TCHAR *defaultExt, HWND parent, const TCHAR *caption, DWORD dwFlags, bool isMulti, cc::List<cc::Str>* arrMultiFiles)
{
	CC_ASSERT(!isMulti || arrMultiFiles != NULL);

	int nBufLen = (_MAX_PATH + 1) * sizeof(TCHAR);
	if(isMulti)
	{
		nBufLen *= 10;
	}
	TCHAR *tcBuf = (TCHAR*)malloc(nBufLen);
	memset(tcBuf, 0, nBufLen);
	if(location != NULL && location[0] != '\0')
	{
		//_tcsncpy(tcBuf, location, _tcslen(location));
		//cc::Buf::Cpy(tcBuf, _MAX_PATH + 1, location);
		_tfullpath(tcBuf, location, _MAX_PATH);
	}

	cc::Str initPath;
	if(cc::Util::dExist(tcBuf))
	{
		initPath = tcBuf;
		tcBuf[0] = '\0';
	}
	else
	{
		initPath = cc::Util::getParentDirName(tcBuf);
	}
	OPENFILENAME ofn;
	memset(&ofn, 0, sizeof(OPENFILENAME)); //Zero out the memory for the filename info
	ofn.lStructSize = sizeof(OPENFILENAME); //Size of the structure
	ofn.hwndOwner = parent; //Window that is calling this
	ofn.lpstrFilter = filter; //Filters
	ofn.lpstrFile = tcBuf; //Where to store it
	ofn.nMaxFile = _MAX_PATH * (isMulti ? 10 : 1); //Tell it how big the TCHAR array is
	ofn.lpstrTitle = caption; //Title
	ofn.lpstrDefExt = defaultExt;
	ofn.lpstrInitialDir = initPath;

	if(dwFlags != 0 && dwFlags != -1)
	{
		ofn.Flags = dwFlags|OFN_EXPLORER;
	}
	else
	{
		ofn.Flags = OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST|OFN_EXPLORER;
	}
	if(isMulti)
	{
		ofn.Flags |= OFN_ALLOWMULTISELECT;
	}
	//^These flags together mean the file name/path typed must exist
	//And it won't ask for the user if they want to create it.
	int retu = ::GetOpenFileName(&ofn); //Display dialog box
	cc::Str str;
	if(retu > 0)
	{
		if(isMulti && arrMultiFiles != NULL)
		{
			//BOOL bExplorer = ofn.Flags & OFN_EXPLORER;
			TCHAR chDelimiter;
			//if (bExplorer)
			//{
				chDelimiter = '\0';
			//}
			//else
			//{
			//	chDelimiter = ' ';
			//}
			// next is not treate chDelimiter = ' '
			// find char pos after first Delimiter
			LPTSTR lpsz = tcBuf;
			while(*lpsz != chDelimiter && *lpsz != '\0')
			{
				lpsz = _tcsinc(lpsz);
			}
			lpsz = _tcsinc(lpsz);

			// if single selection then return only selection
			if(*lpsz == 0)
			{
				arrMultiFiles->Add(cc::Str(tcBuf));
			}
			else
			{
				cc::Str strPath = tcBuf;
				if(!strPath.EndsWith(_T("\\")))
				{
					strPath.Append(_T("\\"));
				}
				while(true)
				{
					arrMultiFiles->Add(strPath + lpsz);

					// find char pos at next Delimiter
					while(*lpsz != chDelimiter && *lpsz != '\0')
					{
						lpsz = _tcsinc(lpsz);
					}
					lpsz = _tcsinc(lpsz);
					if(*lpsz == '\0') // if double terminated then done
					{
						break;
					}
				}
			}
		}
		str.TakeoverPointer(&tcBuf);
	}
	else
	{
		free(tcBuf);
	}
	return str;
}

//filter:_T("ICO Files(*.ico)\0*.ico\0All Files (*.*)\0*.*\0")
cc::Str Util::selectSaveAs(const TCHAR *location, const TCHAR *filter, const TCHAR *defaultExt, HWND parent, const TCHAR *caption)
{
	cc::Str str;
	TCHAR tcBuf[_MAX_PATH + 1];
	memset(tcBuf, 0, (_MAX_PATH + 1) * sizeof(TCHAR));
	if(location != NULL && location[0] != '\0')
	{
		//int nLen = (int)_tcslen(location);
		//_tcsncpy(tcBuf, location, _MAX_PATH);
		//cc::Buf::Cpy(tcBuf, _MAX_PATH + 1, location);
		_tfullpath(tcBuf, location, _MAX_PATH);
	}

	cc::Str initPath;
	if(cc::Util::dExist(tcBuf))
	{
		initPath = tcBuf;
		tcBuf[0] = '\0';
	}
	else
	{
		initPath = cc::Util::getParentDirName(tcBuf);
	}
	OPENFILENAME ofn;
	memset(&ofn, 0, sizeof(OPENFILENAME)); //Zero out the memory for the filename info
	ofn.lStructSize = sizeof(OPENFILENAME); //Size of the structure
	ofn.hwndOwner = parent; //Window that is calling this
	ofn.lpstrFilter = filter; //Filters
	ofn.lpstrFile = tcBuf; //Where to store it
	ofn.nMaxFile = _MAX_PATH; //Tell it how big the TCHAR array is
	ofn.lpstrTitle = caption; //Title
	ofn.lpstrDefExt = defaultExt;
	ofn.Flags |= OFN_PATHMUSTEXIST|OFN_OVERWRITEPROMPT|OFN_EXPLORER; //Any flags here
	ofn.lpstrInitialDir = initPath;

	//^These flags together mean the file name/path typed must exist
	//And it won't ask for the user if they want to create it.
	int retu = ::GetSaveFileName(&ofn); //Display dialog box
	if(retu > 0)
	{
		str.ValueOf(tcBuf);
	}
	return str;
}

typedef struct tagSelectDirEntry
{
	const TCHAR *caption;
	const TCHAR *location;
}SelectDirEntry;
//for selectDir to set title&startpath(not thread safe)
int __stdcall selectDirProc_(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	SelectDirEntry *entry = (SelectDirEntry *)lpData;

	switch(uMsg)
	{
		case BFFM_INITIALIZED:
			//set window title
			if(entry != NULL && entry->caption != NULL)
			{
				SendMessage(hwnd, WM_SETTEXT, 0, (LPARAM)entry->caption);
			}

			// Set the initial directory. If WPARAM is TRUE, then LPARAM is a
			// string that contains the path. If WPARAM is FALSE, then LPARAM
			// should be a lovely PIDL
			if(entry != NULL && entry->location != NULL)
			{
				SendMessage(hwnd, BFFM_SETSELECTION, TRUE, (LPARAM)entry->location);
			}
			break;
		case BFFM_SELCHANGED:
			break;
	}
	return 0;
}

cc::Str Util::selectDir(const TCHAR *location, HWND parent, const TCHAR *caption, bool isEditBox)
{
	TCHAR *tcBuf = (TCHAR*)malloc((_MAX_PATH + 1) * sizeof(TCHAR));
	memset(tcBuf, 0, (_MAX_PATH + 1) * sizeof(TCHAR));

	SelectDirEntry *entry = new SelectDirEntry[1];
	entry->caption = caption;
	entry->location = location;

	BROWSEINFO m_brinfo;
	memset(&m_brinfo, 0, sizeof(BROWSEINFO));
	m_brinfo.hwndOwner = parent;
	//m_brinfo.pidlRoot = 0;
	m_brinfo.pszDisplayName = tcBuf;
	m_brinfo.lpszTitle = caption;
	m_brinfo.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
	if (isEditBox)
	{
		m_brinfo.ulFlags |= BIF_EDITBOX;
	}
	m_brinfo.lParam = (LPARAM)entry;
	m_brinfo.lpfn = selectDirProc_;

	LPCITEMIDLIST pidl = SHBrowseForFolder(&m_brinfo);
	delete entry;
	cc::Str str;
	if(pidl)
	{
		memset(tcBuf, 0, (_MAX_PATH + 1) * sizeof(TCHAR));
		SHGetPathFromIDList(pidl, tcBuf);
		str.TakeoverPointer(&tcBuf);
		if(str.GetLength() > 0 && !str.EndsWith(_T("\\")))
		{
			str.Append(_T("\\"));
		}
	}
	else
	{
		free(tcBuf);
	}
	//GlobalFree(pidl);
	return str;
}

//http://msdn2.microsoft.com/en-us/library/ms646829.aspx
HFONT Util::selectFont(HFONT hfInitial, DWORD dwFlags, HWND parent)
{
	COLORREF corInit = (COLORREF)-1;
	return selectFont(hfInitial, corInit, dwFlags, parent);
}

HFONT Util::selectFont(HFONT hfInitial, COLORREF& corInit, DWORD dwFlags, HWND parent)
{
	CHOOSEFONT cf;
	memset(&cf, 0, sizeof(cf));
	cf.lStructSize = sizeof(cf);
	LOGFONT lf;
	GetObject(hfInitial, sizeof(LOGFONT), &lf);

	cf.Flags = CF_INITTOLOGFONTSTRUCT | CF_SCREENFONTS;
	if(corInit != (COLORREF)-1)
	{
		cf.Flags |= CF_EFFECTS;
		cf.rgbColors = corInit;
	}
	cf.hwndOwner = parent;
	cf.lpLogFont = &lf;

	if(ChooseFont(&cf))
	{
		HFONT hf = CreateFontIndirect(&lf);
		corInit = cf.rgbColors;
		if(hf)
		{
			return hf;
		}
	}
	return NULL;
}

void Util::SetCustColors(int nInd, COLORREF color)
{
	CC_ASSERT(nInd >= 0 && nInd <= 15);
	g_CustColors[nInd] = color;
}
COLORREF Util::GetCustColors(int nInd)
{
	CC_ASSERT(nInd >= 0 && nInd <= 15);
	return g_CustColors[nInd];
}
bool Util::selectColor(COLORREF& color, bool isInitColor, COLORREF rgbCustom[16], DWORD dwFlags, HWND parent)
{
	CHOOSECOLOR cc;
	memset(&cc, 0, sizeof(cc));
	cc.lStructSize = sizeof(cc);
	cc.Flags = (dwFlags == -1) ? CC_FULLOPEN | CC_ANYCOLOR : dwFlags;
	cc.hwndOwner = parent;
	cc.lpCustColors = rgbCustom;
	if(cc.lpCustColors == NULL)
	{
		cc.lpCustColors = g_CustColors;
	}
	if(isInitColor)
	{
		cc.Flags |= CC_RGBINIT;
		cc.rgbResult = color;
	}

	if(::ChooseColor(&cc) == TRUE)
	{
		color = cc.rgbResult;
		return true;
	}
	return false;
}

PRINTDLG Util::selectPrint(HWND parent, HINSTANCE hInstance)
{
	PRINTDLG pd;

	// Initialize PRINTDLG
	memset(&pd, 0, sizeof(pd));
	pd.lStructSize = sizeof(pd);
	pd.hwndOwner   = parent;
	pd.hInstance   = hInstance;
	pd.hDevMode    = NULL;     // Don't forget to free or store hDevMode
	pd.hDevNames   = NULL;     // Don't forget to free or store hDevNames
	pd.Flags       = PD_USEDEVMODECOPIESANDCOLLATE | PD_RETURNDC; 
	pd.nCopies     = 1;
	pd.nFromPage   = 0xFFFF; 
	pd.nToPage     = 0xFFFF; 
	pd.nMinPage    = 1; 
	pd.nMaxPage    = 0xFFFF; 

	if(PrintDlg(&pd) == TRUE) 
	{
		// GDI calls to render output. 

		DOCINFO di;
		di.cbSize = sizeof(DOCINFO);
		di.lpszDocName = _T("ListCtrl Data Printing");
		//di.lpszOutput = (LPTSTR)NULL;
		//di.lpszDatatype = (LPTSTR)NULL;
		//di.fwType = 0:
		//StartDoc(pd.hDC, &di);
		StartPage(pd.hDC);

		//TextOut（pd.hDC, ca[i].nPrintX, nYMargin, ca[i].strColText, strlen（ca[i].strColText））；
		//EndPage（pd.hDC）；
		//EndDoc（pd.hDC）；

		// Delete DC when done.
		DeleteDC(pd.hDC);
	}
	return pd;
}

void Util::openExplorerSelectedFile(const cc::Str fullpath, HWND hWnd)
{
	cc::Str sSelect;
	if(fullpath.EndsWith(_T("\\"))) {
		sSelect = fullpath;
	}
	else {
		//cc::Str sSelect = cc::Util::getParentDirName(sInsFileName);
		sSelect = _T("/select, \"");
		sSelect.Append(fullpath).Append(_T("\""));
	}
	ShellExecute(hWnd, _T("open"), _T("explorer.exe"), sSelect, NULL, SW_SHOW);
}

//run cmd
//must use like:
//	cc::Str sRet = cc::Util::RunCmd(_T("wmic cpu get processorid"));
cc::Str Util::RunCmd(const TCHAR *sCmd, bool isShow, bool isNowait)
{
	const long MAX_COMMAND_SIZE = 10240;
	cc::Str sRet = cc::Str(LangCode::SYSTEM);

	char szBuffer[MAX_COMMAND_SIZE+1] = {0};
	HANDLE hReadPipe = NULL;
	HANDLE hWritePipe = NULL;
	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	SECURITY_ATTRIBUTES sa;

	memset(&pi, 0, sizeof(pi));
	memset(&si, 0, sizeof(si));
	memset(&sa, 0, sizeof(sa));

	pi.hProcess = NULL;
	pi.hThread = NULL;
	si.cb = sizeof(STARTUPINFO);
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;

	// CreatePipe
	BOOL bret = CreatePipe(&hReadPipe, &hWritePipe, &sa, 0);
	if(!bret)
	{
		return sRet;
	}
  
	// set process info
	GetStartupInfo(&si);
	si.hStdError    = hWritePipe;
	si.hStdOutput   = hWritePipe;
	si.wShowWindow  = isShow ? SW_SHOWNORMAL : SW_HIDE; //hide cmd window
	si.dwFlags      = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;

	// CreateProcess, need alloc a buf for new process
	cc::Str sBuf = cc::Str(sCmd);
	bret = CreateProcess(NULL, sBuf, NULL, NULL, TRUE, isNowait ? CREATE_NEW_PROCESS_GROUP : 0, NULL, NULL, &si, &pi);
	if(!bret)
	{
		CloseHandle(hWritePipe);
		CloseHandle(hReadPipe);
		return sRet;
	}
  
	// read result
	DWORD count = 0;
	char* pBuffer = (char*)sRet.GetBufferSetLength(MAX_COMMAND_SIZE, false);
	WaitForSingleObject(pi.hProcess, 500/*INFINITE*/);
	bret = ReadFile(hReadPipe, pBuffer, MAX_COMMAND_SIZE, &count, 0);

	//CloseHandle
	CloseHandle(hWritePipe);
	CloseHandle(hReadPipe);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	if(!bret)
	{
		sRet.Clear();
		return sRet;
	}

	sRet.GetBufferSetLength(count, true);
	return sRet;
}

cc::Str Util::GetEnv(const TCHAR *varname)
{
	cc::Str ret;
	TCHAR* buf = NULL;
	int nRet = CC_GetEnv(varname, &buf);
	if(nRet > 0 && buf != NULL)
	{
		ret.ValueOf(buf);
		free(buf);
	}
	return ret;
}

#define SPACECHAR   _T(' ')
#define DQUOTECHAR  _T('\"')
//split commane line, first(0) is exe file name with full path,next is params
int Util::GetCommandLine(cc::List<cc::Str> &arrCommands)
{
	CC_LOGTIME_START();
	/*
	int nCount = 0;
	//CommandLineToArgvW allocates a block of contiguous memory for pointers to the argument strings,
	// and for the argument strings themselves; the calling application must free the memory used
	// by the argument list when it is no longer needed. To free the memory, use a single call to the LocalFree function.
	TCHAR* szArglist = CommandLineToArgv(::GetCommandLine(), &nCount);
	if(szArglist != NULL)
	{
		for(int n = 0; n < nCount; n++)
		{
			arrCommands.Add(cc::Str(szArglist[n]));
		}
		LocalFree(szArglist);
	}
	return nCount;
	*/
	int nCount = 0;
	const TCHAR *lpszOneStart;
	TCHAR* lpszCommandLine = ::GetCommandLine();
#ifdef _DEBUG_
	cc::Log::trace(_T("Util::GetCommandLine:%s"), lpszCommandLine);
#endif

	// Skip past program name (first token in command line).
	// Check for and handle quoted program name.
	while(*lpszCommandLine)
	{
		if(*lpszCommandLine == DQUOTECHAR)
		{
			lpszCommandLine = ::CharNext(lpszCommandLine);
			lpszOneStart = lpszCommandLine;
			// Scan, and skip over, subsequent characters until
			// another double-quote or a null is encountered.
			while((*lpszCommandLine != DQUOTECHAR) && (*lpszCommandLine != _T('\0')))
			{
				lpszCommandLine = ::CharNext(lpszCommandLine);
			}

			//found one param
			arrCommands.Add(cc::Str(lpszOneStart, 0, (int)(lpszCommandLine - lpszOneStart)));
			nCount++;

			// If we stopped on a double-quote (usual case), skip over it.
			if(*lpszCommandLine == DQUOTECHAR)
			{
				lpszCommandLine = ::CharNext(lpszCommandLine);
			}
		}
		else
		{
			lpszOneStart = lpszCommandLine;
			//for .exe aaa bbb"ccc"
			//while(*lpszCommandLine > SPACECHAR && (*lpszCommandLine != DQUOTECHAR))
			while(*lpszCommandLine > SPACECHAR)
			{
				lpszCommandLine = ::CharNext(lpszCommandLine);
			}

			//found one param
			arrCommands.Add(cc::Str(lpszOneStart, 0, (int)(lpszCommandLine - lpszOneStart)));
			nCount++;
		}

		// Skip past any white space preceeding the second token.
		while(*lpszCommandLine && (*lpszCommandLine <= SPACECHAR))
		{
			lpszCommandLine = ::CharNext(lpszCommandLine);
		}
	}

#ifdef _DEBUG_
	for(int n = 0; n < arrCommands.GetSize(); n++)
	{
		cc::Log::trace(_T("Util::GetCommandLine Params[%d]:%s"), n, (const TCHAR*)arrCommands.GetKey(n).GetBuf());
	}
	CC_LOGTIME_PRINT();
#endif
	return nCount;
}

int Util::GetDragFiles(HDROP hdrop, cc::List<cc::Str> &arrFiles)
{
	TCHAR szFileName[MAX_PATH];
	UINT nFiles = DragQueryFile(hdrop, (UINT)-1, NULL, 0);
	for(UINT i = 0; i < nFiles; i++)
	{
		DragQueryFile(hdrop, i, szFileName, sizeof(szFileName));
		arrFiles.Add(cc::Str(szFileName));
	}

#ifdef _DEBUG_
	for(int n = 0; n < arrFiles.GetSize(); n++)
	{
		cc::Log::trace(_T("Util::GetDragFiles Params[%d]:%s"), n, (const TCHAR*)arrFiles.GetKey(n).GetBuf());
	}
#endif
	return nFiles;
}

cc::Str Util::GetClipboardText(HWND m_hWnd)
{
	cc::Str str;
	GetClipboardText(str, m_hWnd);
	return str;
}
bool Util::GetClipboardText(cc::Str& str, HWND m_hWnd)
{
	UINT format = str.IsUnicode() ? CF_UNICODETEXT : CF_TEXT;
	if(::IsClipboardFormatAvailable(format))
	{
		if(::OpenClipboard(m_hWnd) == TRUE)
		{
			HANDLE hClipBrdData = ::GetClipboardData(format);
			if(hClipBrdData != NULL)
			{
				void* lpClipBrdText = (void*)::GlobalLock(hClipBrdData);
				if(lpClipBrdText != NULL)
				{
					if(str.IsUnicode())
					{
						str.ValueOf((wchar_t*)lpClipBrdText);
					}
					else
					{
						str.ValueOf((char*)lpClipBrdText);
					}
					::GlobalUnlock(hClipBrdData);
					::CloseClipboard();
					return true;
				}
			}
			::CloseClipboard();
		}
	}
	return false;
}

bool Util::SetClipboardText(const char* str, HWND m_hWnd)
{
	if(::OpenClipboard(m_hWnd))
	{
		::EmptyClipboard();
		if(str != NULL)
		{
			int nLen = cc::Str::Len(str);
			HGLOBAL hMem = GlobalAlloc(GHND | GMEM_DDESHARE, nLen + 1);
			if(hMem != NULL)
			{
				char* lpMem = (char *)GlobalLock(hMem);
				memset(lpMem, 0, nLen + 1);
				memcpy(lpMem, (char*)str, nLen);
				GlobalUnlock(hMem);
				SetClipboardData(CF_TEXT, hMem);
			}
			::CloseClipboard();
		}
		return true;
	}
	return false;
}
bool Util::SetClipboardText(const wchar_t* str, HWND m_hWnd)
{
	if(::OpenClipboard(m_hWnd))
	{
		::EmptyClipboard();
		if(str != NULL)
		{
			int nLen = cc::Str::Len(str) * 2;
			HGLOBAL hMem = GlobalAlloc(GHND | GMEM_DDESHARE, nLen + 2);
			if(hMem != NULL)
			{
				char* lpMem = (char *)GlobalLock(hMem);
				memset(lpMem, 0, nLen + 2);
				memcpy(lpMem, (char*)str, nLen);
				GlobalUnlock(hMem);
				SetClipboardData(CF_UNICODETEXT, hMem);
			}
			::CloseClipboard();
		}
		return true;
	}
	return false;
}
bool Util::SetClipboardText(const cc::Str& str, HWND m_hWnd)
{
	if(str.IsUnicode())
	{
		return SetClipboardText((const wchar_t*)str.GetBuf(), m_hWnd);
	}
	else
	{
		return SetClipboardText((const char*)str.GetBuf(), m_hWnd);
	}
}

bool Util::SetClipboardBitmap(HBITMAP bmp, HWND m_hWnd)
{
	bool ret = false;
	if(::OpenClipboard(m_hWnd))
	{
		::EmptyClipboard();
		if(::SetClipboardData(CF_BITMAP, bmp) != NULL)
		{
			ret = true;
		}
		::CloseClipboard();
	}
	return ret;
}
//need release HBITMAP outer!
HBITMAP Util::GetClipboardBitmap(HWND m_hWnd)
{
	HBITMAP hClipBrdData = NULL;
	if(::IsClipboardFormatAvailable(CF_BITMAP))
	{
		if(::OpenClipboard(m_hWnd) == TRUE)
		{
			HBITMAP hClipBrdData2 = (HBITMAP)::GetClipboardData(CF_BITMAP);
			if(hClipBrdData2 != NULL)
			{
				//need copy?
				hClipBrdData = CopyBitmap(hClipBrdData2);
			}
			::CloseClipboard();
		}
	}
	return hClipBrdData;
}

HBITMAP Util::CopyBitmap(HBITMAP hSrcBitmap)
{
	BITMAP tBitmap;

	::GetObject(hSrcBitmap, sizeof(tBitmap), &tBitmap);
	HDC hDC = ::GetDC(NULL);
	HBITMAP hDstBitmap = ::CreateCompatibleBitmap(hDC, tBitmap.bmWidth, tBitmap.bmHeight);

	HDC hSrcDC = ::CreateCompatibleDC(hDC);
	HDC hDstDC = ::CreateCompatibleDC(hDC);

	::SelectObject(hSrcDC, hSrcBitmap);
	::SelectObject(hDstDC, hDstBitmap);

	::BitBlt(hDstDC, 0, 0, tBitmap.bmWidth, tBitmap.bmHeight, hSrcDC, 0, 0, SRCCOPY);

	::DeleteDC(hSrcDC);
	::DeleteDC(hDstDC);

	::ReleaseDC(NULL, hDC);

	return hDstBitmap;
}
HBITMAP Util::ResizeBmp(HBITMAP hBmpSource, int nDestW, int nDestH, bool isBitsFromScreenDC, int bmBitsPixel)
{
	return ResizeBmp(hBmpSource, 0, 0, nDestW, nDestH, 0, 0, isBitsFromScreenDC, bmBitsPixel);
}
HBITMAP Util::ResizeBmp(HBITMAP hBmpSource, int nDestW, int nDestH, int fromX, int fromY, int srcW, int srcH, bool isBitsFromScreenDC, int bmBitsPixel)
{
	BITMAP bmp;
	GetObject(hBmpSource, sizeof(BITMAP), &bmp);
	HBITMAP hBmpDest;
	HDC hScreenDC = ::GetDC(NULL);
	if(isBitsFromScreenDC)
	{
		hBmpDest = CreateCompatibleBitmap(hScreenDC, nDestW, nDestH);
	}
	else
	{
		//nWidth [in] Specifies the bitmap width, in pixels.
		//nHeight [in] Specifies the bitmap height, in pixels.
		//cPlanes [in] Specifies the number of color planes used by the device.
		//The value of this parameter must be 1.
		//cBitsPerPel [in] Specifies the number of bits required to identify the color of a single pixel.
		//lpvBits [in] Long void pointer to an array of color data used to set the colors in a rectangle of pixels.
		//Each scan line in the rectangle must be word aligned (scan lines that are not word aligned must be padded with zeros).
		//If this parameter is NULL, the new bitmap is undefined.
		if(bmBitsPixel <= 0)
		{
			bmBitsPixel = bmp.bmBitsPixel;
		}
		hBmpDest = CreateBitmap(nDestW, nDestH, bmp.bmPlanes, bmBitsPixel, NULL);
	}

	HDC hMemDC1 = CreateCompatibleDC(hScreenDC);
	HDC hMemDC2 = CreateCompatibleDC(hScreenDC);
	HBITMAP hOldBmp1 = (HBITMAP)SelectObject(hMemDC1, hBmpSource);
	HBITMAP hOldBmp2 = (HBITMAP)SelectObject(hMemDC2, hBmpDest);

	SetStretchBltMode(hMemDC2, COLORONCOLOR);
	if(fromX > bmp.bmWidth)
	{
		fromX = 0;
	}
	if(fromY > bmp.bmHeight)
	{
		fromY = 0;
	}
	if(srcW <= 0 || srcW > bmp.bmWidth)
	{
		srcW = bmp.bmWidth;
	}
	if(srcH <= 0 || srcH > bmp.bmHeight)
	{
		srcH = bmp.bmHeight;
	}
	StretchBlt(hMemDC2, 0, 0, nDestW, nDestH, hMemDC1, fromX, fromY, srcW, srcH, SRCCOPY);

	SelectObject(hMemDC1, hOldBmp1);
	SelectObject(hMemDC2, hOldBmp2);
	DeleteDC(hMemDC1);
	DeleteDC(hMemDC2);
	ReleaseDC(NULL, hScreenDC);
	return hBmpDest;
}

//for EnumTypesFunc&EnumNamesFunc(not thread safe)
cc::Map<cc::Str, cc::Str>* g_arrRes = NULL;

//for readResTypes(not thread safe)
BOOL __stdcall EnumTypesFunc(HANDLE hModule, LPTSTR lpType, LONG lParam)
{
	if(g_arrRes == NULL)
	{
		return FALSE;
	}
//#pragma warning(disable:4311)
//	if(((ULONG)lpType & 0xFFFF0000) != 0)
	if(!IS_INTRESOURCE(lpType))
	{
		g_arrRes->Add(cc::Str(lpType), cc::Str());
	}
//#pragma warning(default:4311)
	else
	{
		g_arrRes->Add(Str().Format(_T("#%d"), (USHORT)lpType), cc::Str(_T("#")));
	}
	return TRUE;
}

// The name may be a string or an unsigned decimal integer, if integer,map's valus is "#"
int Util::readResTypes(cc::Map<cc::Str, cc::Str> &arrRes, HMODULE hRes)
{
	g_arrRes = &arrRes;
	EnumResourceTypes(hRes, (ENUMRESTYPEPROC)EnumTypesFunc, 0);
	g_arrRes = NULL;
	return arrRes.GetSize();
}

//for readResNames(not thread safe)
BOOL __stdcall EnumNamesFunc(HANDLE hModule, LPCTSTR lpType, LPTSTR lpName, LONG lParam)
{
	if(g_arrRes == NULL)
	{
		return FALSE;
	}
//#pragma warning(disable:4311)
//	if(((ULONG)lpName & 0xFFFF0000) != 0)
	if(!IS_INTRESOURCE(lpType))
	{
		g_arrRes->Add(cc::Str(lpName), cc::Str());
	}
//#pragma warning(default:4311)
	else
	{
		g_arrRes->Add(Str().Format(_T("#%d"), (USHORT)lpName), cc::Str(_T("#")));
	}
	return TRUE;
}

// The name may be a string or an unsigned decimal integer, if integer,map's valus is "#"
int Util::readResNames(cc::Map<cc::Str, cc::Str> &arrRes, const TCHAR *lpType, HMODULE hRes)
{
	g_arrRes = &arrRes;
	EnumResourceNames(hRes, lpType, (ENUMRESNAMEPROC)EnumNamesFunc, 0);
	g_arrRes = NULL;
	return arrRes.GetSize();
}

//========================================
//ini if loadAllLine then arrIni cannot be sort
int Util::treateIni(cc::Map<cc::Str, cc::Str> &arrIni, const Str &sBuf, bool turnToTCHAR, bool loadAllLine)
{
	CC_LOGTIME_START();
	//arrIni.setDistinct(true);
	//key is or not turn to tchar
	LangCode::Type retuLangCode = (turnToTCHAR ? cc::LangCode::TCHAR : sBuf.GetLangCode());

	Str *sLines = NULL;
	int nLine;
	if(sBuf.GetLangCode() == LangCode::UNICODE16)
	{
		nLine = sBuf.Split(L"\n", &sLines, 0, false, retuLangCode);
	}
	else
	{
		nLine = sBuf.Split("\n", &sLines, 0, false, sBuf.GetLangCode(), retuLangCode);
	}

	//isSort, bIsDESC = false, isDistinct = false, nMinCapacity = 0, nIncrement = 1.3
	cc::Map<cc::Str, cc::Str> arrIndex(true, false, true);
	Str sHead(retuLangCode, _T(""));
	int nCount = 0;
	if(retuLangCode == LangCode::UNICODE16)
	{
		for(int i = 0; i < nLine; i++)
		{
			sLines[i].Trim().Replace(L"\r", L"");
			if(sLines[i].GetLength() <= 0 || sLines[i].StartsWith(L"#") || sLines[i].StartsWith(L";") || sLines[i].StartsWith(L"/"))
			{
				if(loadAllLine)
				{
					arrIni.Add(Str(retuLangCode).Format(L"[#]%d", i), sLines[i]);
				}
				continue;
			}
			else if(sLines[i].StartsWith(L"[") && sLines[i].EndsWith(L"]"))
			{
				if(sLines[i].GetLength() > 2)
				{
					sHead = sLines[i].Substring(1, sLines[i].GetLength() - 1).Append(L".");
					if(loadAllLine)
					{
						//if loadAllLine,then also add head section as key like "head."
						arrIni.Add(Str(retuLangCode, sHead), Str(retuLangCode, L""));
					}
				}
				continue;
			}

			int npos = sLines[i].IndexOf(L"=");
			if(npos > 0)
			{
				//modify 2007/07/11
				//cc::Str sItem = Str(retuLangCode, sLines[i].Substring(npos + 1)).Replace(_T("\\n"), _T("\n")).Replace(_T("\\r"), _T(""));
				cc::Str str = sLines[i].Substring(npos + 1);
				if(str.GetLength() > 0)
				{
					//after GetBuffer, not need GetBufferSetLength() bucause not change sring length here
					turnWord((wchar_t*)str.GetBuffer());
				}
				else
				{
					//modify 2009/02/05,treate for "key="
					str.ValueOf(L"");
				}
				//cc::Str sItem = Str(retuLangCode, sLines[i].Substring(npos + 1)).Replace(_T("\\n"), _T("\n"));
				cc::Str sKey = sLines[i].Substring(0, npos);
				if(sKey.GetLength() > 1 && sKey.EndsWith(L"?"))
				{
					sKey.Remove(sKey.GetLength() - 1);
					cc::Str sKeyCount = cc::Str(LangCode::UNICODE16);
					sKeyCount.Format(L"%s%s.Count", (wchar_t*)sHead.GetBuf(), (wchar_t*)sKey.GetBuf());
					int n = arrIndex.GetIndex(sKeyCount);
					if(n < 0)
					{
						n = 0;
					}
					else
					{
						n = arrIndex.GetAt(n).ToInt();
					}
					//cc::Str sKey2 = cc::Str(LangCode::UNICODE16);
					//sKey2.Format(L"%s%d", (wchar_t*)sKey.GetBuf(), n);
					arrIni.Add(Str(retuLangCode, sHead + sKey + n), Str(retuLangCode, (wchar_t*)str.GetBuf()));
					//save count
					sKey.Format(L"%d", n + 1);
					arrIndex.Add(sKeyCount, sKey);
				}
				else
				{
					arrIni.Add(Str(retuLangCode, sHead + sKey), Str(retuLangCode, (wchar_t*)str.GetBuf()));
				}
				nCount++;
			}
		}
	}
	else
	{
		for(int i = 0; i < nLine; i++)
		{
			sLines[i].Trim().Replace("\r", "", false, retuLangCode);
			if(sLines[i].GetLength() <= 0 || sLines[i].StartsWith("#", false, retuLangCode) || sLines[i].StartsWith(";", false, retuLangCode) || sLines[i].StartsWith("/", false, retuLangCode))
			{
				if(loadAllLine)
				{
					arrIni.Add(Str(retuLangCode).Format(retuLangCode, "[#]%d", i), sLines[i]);
				}
				continue;
			}
			else if(sLines[i].StartsWith("[", false, retuLangCode) && sLines[i].EndsWith("]", false, retuLangCode))
			{
				if(sLines[i].GetLength() > 2)
				{
					sHead = sLines[i].Substring(1, sLines[i].GetLength() - 1).Append(".", 0, retuLangCode);
					if(loadAllLine)
					{
						//if loadAllLine,then also add head section as key like "head."
						arrIni.Add(Str(retuLangCode, sHead), Str(retuLangCode, ""));
					}
				}
				continue;
			}

			int npos = sLines[i].IndexOf("=", false, retuLangCode);
			if(npos > 0)
			{
				//modify 2007/07/11
				//cc::Str sItem = Str(retuLangCode, sLines[i].Substring(npos + 1)).Replace(_T("\\n"), _T("\n")).Replace(_T("\\r"), _T(""));
				cc::Str str = sLines[i].Substring(npos + 1);
				if(str.GetLength() > 0)
				{
					//after GetBuffer, not need GetBufferSetLength() bucause not change sring length here
					turnWord((char*)str.GetBuffer());
				}
				else
				{
					//modify 2009/02/05,treate for "key="
					str.ValueOf("");
				}
				//cc::Str sItem = Str(retuLangCode, sLines[i].Substring(npos + 1)).Replace(_T("\\n"), _T("\n"));
				cc::Str sKey = sLines[i].Substring(0, npos);
				if(sKey.GetLength() > 1 && sKey.EndsWith("?", false, retuLangCode))
				{
					sKey.Remove(sKey.GetLength() - 1);
					cc::Str sKeyCount = cc::Str(retuLangCode);
					sKeyCount.Format(retuLangCode, "%s%s.Count", (char*)sHead.GetBuf(), (char*)sKey.GetBuf());
					int n = arrIndex.GetIndex(sKeyCount);
					if(n < 0)
					{
						n = 0;
					}
					else
					{
						n = arrIndex.GetAt(n).ToInt();
					}
					//cc::Str sKey2 = cc::Str(retuLangCode);
					//sKey2.Format(retuLangCode, "%s%d", (char*)sKey.GetBuf(), n);
					arrIni.Add(Str(retuLangCode, sHead + sKey + n), Str(retuLangCode, (char*)str.GetBuf(), retuLangCode));
					//save count
					sKey.Format(retuLangCode, "%d", n + 1);
					arrIndex.Add(sKeyCount, sKey);
				}
				else
				{
					arrIni.Add(Str(retuLangCode, sHead + sKey), Str(retuLangCode, (char*)str.GetBuf(), retuLangCode));
				}
				nCount++;
			}
		}
	}
	arrIni.Add(arrIndex);
	delete []sLines;
	CC_LOGTIME_PRINT();
	return nCount;
}

//must be:
//\n[section]
//\nkey=value
cc::Str Util::readOneIni(const TCHAR* section, const TCHAR* key, const TCHAR *fullpath, LangCode::Type fileLangCode, bool turnToTCHAR, int readLen)
{
	cc::Str sTxt(fileLangCode);
	int nRead = cc::Util::fRead(sTxt, fullpath, fileLangCode, readLen);
	if(nRead >= 0)
	{
		return readOneIni(section, key, sTxt, turnToTCHAR);
	}
	if(turnToTCHAR)
	{
		return cc::Str(LangCode::TCHAR);
	}
	return cc::Str(fileLangCode);
}
cc::Str Util::readOneIni(const TCHAR* section, const TCHAR* key, const Str &sBuf, bool turnToTCHAR)
{
	CC_LOGTIME_START();
	LangCode::Type retuLangCode = turnToTCHAR ? cc::LangCode::TCHAR : sBuf.GetLangCode();
	cc::Str sSection = cc::Str(sBuf.GetLangCode(), _T("[")).Append(section).Append(_T("]"));
	int nPos;
	if(sBuf.StartsWith(sSection, true))
	{
		nPos = 0;
	}
	else
	{
		sSection.Insert(0, _T("\n"));
		nPos = sBuf.IndexOf(sSection, true);
	}
	if(nPos < 0)
	{
		return cc::Str(retuLangCode);
	}
	//key=
	cc::Str sKey = cc::Str(sBuf.GetLangCode(), _T("\n")).Append(key).Append(_T("="));
	int nPos2 = sBuf.IndexOf(sKey, nPos + 1);
	//next session
	int nPos3 = sBuf.IndexOf(_T("\n["), nPos + 1);
	if(nPos2 < 0 || (nPos3 > 0 && nPos2 > nPos3))
	{
		//not found key
		return cc::Str(retuLangCode);
	}
	//end of value
	nPos = sBuf.IndexOf(_T("\n"), nPos2 + 1);
	cc::Str sRetu = cc::Str(retuLangCode, sBuf.Substring(nPos2 + sKey.GetLength(), nPos < 0 ? sBuf.GetLength() : nPos - 1));
	CC_LOGTIME_PRINT();
	return sRetu;
}

int Util::readIni(cc::Map<cc::Str, cc::Str> &arrIni, const TCHAR *fullpath, LangCode::Type fileLangCode, bool turnToTCHAR, bool loadAllLine)
{
#if _DEBUG_
	cc::Log::trace(_T("[Util::readIni]Path:%s, fileLangCode:%d, turnToTCHAR:%d, loadAllLine:%d"), fullpath, fileLangCode, turnToTCHAR, loadAllLine);
	CC_LOGTIME_START();
#endif
	//arrIni.setDistinct(true);
	//key is or not turn to tchar
	LangCode::Type retuLangCode;
	if(turnToTCHAR)
	{
		retuLangCode = cc::LangCode::TCHAR;
	}
	else
	{
		retuLangCode = fileLangCode;
	}
	Str sTxt(retuLangCode);
	int nRead = fRead(sTxt, fullpath, fileLangCode);
	if(nRead < 0)
	{
		return -1;
	}
	//nRead = treateIni_Del(arrIni, sTxt, turnToTCHAR, loadAllLine);
	//arrIni.Clear();
	nRead = treateIni(arrIni, sTxt, turnToTCHAR, loadAllLine);
	CC_LOGTIME_PRINT();
	return nRead;
}
int Util::readIni(cc::Map<cc::Str, cc::Str> &arrIni, const Str &fullpath, LangCode::Type fileLangCode, bool turnToTCHAR, bool loadAllLine)
{
	const Str *tcstr;
	if(!fullpath.IsTChar())
	{
		//here will create Str with system pagecode(has TCHAR* buf)
		//not forget to relese it!!
		tcstr = new Str(fullpath);
	}
	else
	{
		tcstr = &fullpath;
	}

	int retu = readIni(arrIni, (const TCHAR*)tcstr->GetBuf(), fileLangCode, turnToTCHAR, loadAllLine);
	if(!fullpath.IsTChar())
	{
		delete tcstr;
		tcstr = NULL;
	}
	return retu;
}
//we thing key&value in arrIni is TCHAR*.
int Util::writeIniToString(const cc::Map<cc::Str, cc::Str> &arrIni, Str &sTxt)
{
	CC_LOGTIME_START();
	int nSize = arrIni.GetSize();
	if(nSize <= 0)
	{
		return -1;
	}
	List<int> _lstSkip;
	bool isSort = arrIni.IsSort();
	cc::Str sHead(sTxt.GetLangCode(), _T("."));
	cc::Str sCR(sTxt.GetLangCode(), _T("\r\n"));
	cc::Str sEqual(sTxt.GetLangCode(), _T("="));
	int i;
	for(i = 0; i < nSize; i++)
	{
		cc::Str skey = arrIni.GetKey(i);
		cc::Str sval = arrIni.GetAt(i);
		if(skey.StartsWith(_T("[#]")))
		{
			//this is comment
			sTxt.Append(sval).Append(sCR);
		}
		else if(!sval.IsNULL())
		{
			int nDot = skey.IndexOf(_T("."));
			if(nDot > 0 && (isSort || !_lstSkip.ContainsKey(i)))
			{
				if(!skey.StartsWith(sHead))
				{
					//this time treate with others with same head
					if(!isSort && sHead.GetLength() > 1) //not do this at first time
					{
						for(int j = i; j < nSize; j++)
						{
							cc::Str skey2 = arrIni.GetKey(j);
							//not need do with "head."
							if(skey2.StartsWith(sHead) && !arrIni.GetAt(j).IsNULL() && skey2.GetLength() > sHead.GetLength())
							{
								sTxt.Append(skey2.Substring(sHead.GetLength())).Append(sEqual).Append(arrIni.GetAt(j)).Append(sCR);
								_lstSkip.Add(j);
							}
						}
					}
					if(sHead.GetLength() > 1) //not do this at first time
					{
						sTxt.Append(_T("\r\n"));
					}
					sHead = cc::Str(sTxt.GetLangCode(), skey.Substring(0, nDot));
					sTxt.Append(_T("[")).Append(sHead).Append(_T("]\r\n"));
					sHead.Append(_T("."));
				}
				//if is "head.",write nothing
				if(skey.GetLength() > sHead.GetLength())
				{
					sval.Replace(_T("\\"), _T("\\\\")).Replace(_T("\n"), _T("\\n")).Replace(_T("\r"), _T("\\r"));
					sTxt.Append(skey.Substring(sHead.GetLength())).Append(sEqual).Append(sval).Append(sCR);
				}
			}
			else
			{
				sval.Replace(_T("\\"), _T("\\\\")).Replace(_T("\n"), _T("\\n")).Replace(_T("\r"), _T("\\r"));
				sTxt.Append(skey).Append(sEqual).Append(sval).Append(sCR);
			}
		}
	}
	CC_LOGTIME_PRINT();
	return nSize;
}
int Util::writeIni(const cc::Map<cc::Str, cc::Str> &arrIni, const TCHAR *fullpath, LangCode::Type fileLangCode)
{
	CC_LOGTIME_START();
	int nSize = arrIni.GetSize();
	if(nSize <= 0)
	{
		if(fWrite(_T(""), 0, fileLangCode, fullpath, false))
		{
			return 0;
		}
		return -1;
	}
	cc::Str sTxt((LangCode::Type)fileLangCode, (unsigned int)nSize * 20);
	int i = writeIniToString(arrIni, sTxt);
	if(fWrite(sTxt, fullpath, false))
	{
		return (i - 1);
	}
	CC_LOGTIME_PRINT();
	return -1;
}
int Util::writeIni(const cc::Map<cc::Str, cc::Str> &arrIni, const Str &fullpath, LangCode::Type fileLangCode)
{
	const Str *tcstr;
	if(!fullpath.IsTChar())
	{
		//here will create Str with system pagecode(has TCHAR* buf)
		//not forget to relese it!!
		tcstr = new Str(fullpath);
	}
	else
	{
		tcstr = &fullpath;
	}

	int retu = writeIni(arrIni, (const TCHAR*)tcstr->GetBuf(), fileLangCode);
	if(!fullpath.IsTChar())
	{
		delete tcstr;
		tcstr = NULL;
	}
	return retu;
}

int Util::readRes(cc::Str& sTxt, unsigned int nResID, const TCHAR *resType, LangCode::Type fileLangCode)
{
	// if use MAKEINTRESOURCE(nResID),next cannot do warn with %s
	return readRes(sTxt, (TCHAR*)cc::Str().Format(_T("#%d"), nResID).GetBuf(), resType, fileLangCode);
}

int Util::readRes(cc::Str& sTxt, const TCHAR *resName, const TCHAR *resType, LangCode::Type fileLangCode)
{
	CC_LOGTIME_START();
	HRSRC hres = ::FindResource(NULL, resName, resType);
	if(hres <= 0)
	{
		cc::Log::warn(_T("[Util::readRes]Could Not Locate Resource File[name:%s,type%s]."), resName, resType);
		return -1;
	}
	//If resource is found a handle to the resource is returned now just load the resource
	HGLOBAL hbytes = ::LoadResource(NULL, hres);
	//Lock the resource
	LPVOID pdata = ::LockResource(hbytes);
	int nLen = (int)::SizeofResource(NULL, hres);
	if(nLen <= 0)
	{
		return nLen;
	}
	unsigned char* p = (unsigned char*)pdata;
	if(nLen > 1)
	{
		if(p[0] == 0xFF && p[1] == 0xFE)
		{
			//litter
			fileLangCode = LangCode::UNICODE16;
			p = p + 2;
			nLen = nLen - 2;
		}
		else if(p[0] == 0xFE && p[1] == 0xFF)
		{
			//big
			fileLangCode = LangCode::UNICODE_BIGENDIAN;
			p = p + 2;
			nLen = nLen - 2;
		}
		else if(p[0] == 0xEF && p[1] == 0xBB && nLen > 2 && p[2] == 0xBF)
		{
			fileLangCode = LangCode::UTF8;
			p = p + 3;
			nLen = nLen - 3;
		}
	}

	void *des = malloc(nLen + 5);
	memset(des, '\0', nLen + 5);
	memcpy(des, p, nLen);
	if(sTxt.GetLangCode() == fileLangCode)
	{
		if(fileLangCode == LangCode::UNICODE16)
		{
			CC_ASSERT(nLen%2 == 0);
			sTxt.TakeoverPointer((wchar_t**)&des, nLen/2);
		}
		else
		{
			sTxt.TakeoverPointer((char**)&des, nLen, fileLangCode);
		}
	}
	else
	{
		if(fileLangCode == LangCode::UNICODE16)
		{
			sTxt.ValueOf((wchar_t*)des);
		}
		else
		{
			sTxt.ValueOf((char*)des, 0, nLen, fileLangCode);
		}
		free(des);
		des = NULL;
	}

	::FreeResource(hbytes);
	CC_LOGTIME_PRINT();
	return nLen;
}

bool Util::savePicture(IPicture* pPicture, const cc::Str& sFilePath)
{
	if(pPicture == NULL)
	{
		return false;
	}

	LPPICTUREDISP pDisp = NULL;
	HRESULT hr = pPicture->QueryInterface(IID_IPictureDisp, (LPVOID*)&pDisp);
	if(SUCCEEDED(hr))
	{
		BSTR bstrFilePath;
		if(sFilePath.GetLangCode() == LangCode::UNICODE16)
		{
			bstrFilePath = ::SysAllocString((const wchar_t*)sFilePath.GetBuf());
		}
		else
		{
			cc::Str sFilePath2 = cc::Str(LangCode::UNICODE16, sFilePath);
			bstrFilePath = ::SysAllocString((const wchar_t*)sFilePath2.GetBuf());
		}
		hr = ::OleSavePictureFile(pDisp, bstrFilePath);
		::SysFreeString(bstrFilePath);
	}
	if(!(SUCCEEDED(hr)))
	{
		CC_TRACE(("Could not save image (hr failure)"));
		return false;
	}
	return true;
}

//http://www.codeproject.com/KB/graphics/render.aspx
//http://www.codeproject.com/Articles/218368/WTLIPicture-Viewer-An-Update-to-the-WTL-BmpView-Sa
short Util::readPictureType(IPicture* pPicture)
{
	short sPixType = (short)PICTYPE_UNINITIALIZED;
	if(pPicture != NULL)
	{
		pPicture->get_Type(&sPixType);
	}
	return sPixType;
}
void Util::readPictureSize(IPicture* pPicture, SIZE& size)
{
	if(pPicture == NULL)
	{
		return;
	}

	pPicture->get_Width(&size.cx);
	pPicture->get_Height(&size.cy);
}
IPicture* Util::readPicture(HGLOBAL hGlobal, DWORD dwFileSize)
{
	IPicture* pPicture = NULL;
	if(hGlobal == NULL)
	{
		return pPicture;
	}
	LPSTREAM pstm = NULL;
	// create IStream* from global memory
	HRESULT hr = CreateStreamOnHGlobal(hGlobal,
										TRUE,
										&pstm);
	if(!(SUCCEEDED(hr)))
	{
		CC_TRACE(("CreateStreamOnHGlobal() failed"));
		if(pstm != NULL)
		{
			pstm->Release();
		}
		return NULL;
	}
	else if(pstm == NULL)
	{
		//AfxMessageBox ("CreateStreamOnHGlobal() failed");
		CC_TRACE(("pstm == NULL"));
		return NULL;
	}

	// Create IPicture from image file
	//if(pPicture)
	//{
	//	(pPicture)->Release();
	//}
	//The stream must be in BMP (bitmap), WMF (metafile), or ICO (icon) format.
	//(BUT real is GIF, JPG, BMP, ICO, EMF and WMF)
	hr = ::OleLoadPicture(pstm,
				dwFileSize,
				FALSE,
				IID_IPicture,
				(LPVOID *)&pPicture);
	pstm->Release();
	if(!(SUCCEEDED(hr)))
	{
		CC_TRACE(("Could not load image (hr failure)"));
		return NULL;
	}
	else if(pPicture == NULL)
	{
		CC_TRACE(("Could not load image (pPicture failure)"));
		return pPicture;
	}
	return pPicture;
}
IPicture* Util::readPicture(const TCHAR* szFile)
{
	IPicture* pPicture = NULL;
	// open file
	HANDLE hFile = CreateFile(szFile,
								GENERIC_READ,
								0,
								NULL,
								OPEN_EXISTING,
								0,
								NULL);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		CC_TRACE(("Could not read file"));
		return pPicture;
	}

    // get file size
	DWORD dwFileSize = GetFileSize(hFile, NULL);
	if(dwFileSize == (DWORD)-1)
	{
		CloseHandle(hFile);
		CC_TRACE(("File seems to be empty"));
		return pPicture;
	}

	LPVOID pvData = NULL;
	// alloc memory based on file size
	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, dwFileSize);
	if(hGlobal == NULL)
	{
		CloseHandle(hFile);
		CC_TRACE(("Could not allocate memory for image"));
		return pPicture;
	}

	pvData = GlobalLock(hGlobal);
	if(pvData == NULL)
	{
		GlobalUnlock(hGlobal);
		CloseHandle(hFile);
		CC_TRACE(("Could not lock memory for image"));
		return pPicture;
	}

	DWORD dwBytesRead = 0;
	// read file and store in global memory
	BOOL bRead = ReadFile(hFile,
							pvData,
							dwFileSize,
							&dwBytesRead,
							NULL);
	GlobalUnlock(hGlobal);
	CloseHandle(hFile);
	if(!bRead)
	{
		CC_TRACE(("Could not read file"));
		return pPicture;
	}

	pPicture = readPicture(hGlobal, dwFileSize);
	::GlobalFree(hGlobal);
	return pPicture;
}
IPicture* Util::readPicture(UINT nResID, const TCHAR *resType)
{
	IPicture* pPicture = NULL;
	BOOL		bResult = FALSE;
	HGLOBAL		hGlobal = NULL;
	HRSRC		hSource = NULL;
	LPVOID		lpVoid  = NULL;
	DWORD		dwSize   = 0;
	//hSource = FindResource(CC_APP_GetInstance(), (TCHAR*)cc::Str().Format(_T("#%d"), nResID).GetBuf(), resType);
	hSource = FindResource(CC_APP_GetInstance(), MAKEINTRESOURCE(nResID), resType);
	if(hSource == NULL)
	{
		hSource = FindResource(CC_APP_GetInstance(), MAKEINTRESOURCE(nResID), resType);
		if(hSource == NULL)
		{
			return pPicture;
		}
	}

	hGlobal = LoadResource(CC_APP_GetInstance(), hSource);
	if(hGlobal != NULL)
	{
		lpVoid = LockResource(hGlobal);
		if(lpVoid != NULL)
		{
			dwSize = (DWORD)SizeofResource(CC_APP_GetInstance(), hSource);
			HGLOBAL hGlobal2 = ::GlobalAlloc(GMEM_MOVEABLE, dwSize);
			if(hGlobal2 != NULL)
			{
				void* pData = (void *)GlobalLock(hGlobal2);
				CopyMemory(pData, lpVoid, dwSize);
				::GlobalUnlock(hGlobal2);
				pPicture = readPicture(hGlobal2, dwSize);
				::GlobalFree(hGlobal2);
			}
		}
		UnlockResource(hGlobal); // 16Bit Windows Needs This
		FreeResource(hGlobal); // 16Bit Windows Needs This (32Bit - Automatic Release)
	}
	return pPicture;
}
HBITMAP Util::readBitmap(const TCHAR* szFile)
{
	HBITMAP hBitmap = NULL;
	IPicture* pPicture = cc::Util::readPicture(szFile);
	if(pPicture != NULL)
	{
		HBITMAP hBitmap2 = NULL;
		pPicture->get_Handle((OLE_HANDLE *)&hBitmap2);
		if(hBitmap2 != NULL)
		{
			hBitmap = (HBITMAP)CopyImage(hBitmap2, IMAGE_BITMAP, 0, 0, LR_COPYRETURNORG); 
		}
		pPicture->Release();
	}
	return hBitmap;
}
HBITMAP Util::readBitmap(const char* sBuf, int bufLen)
{
	HGLOBAL hMem = GlobalAlloc(GHND | GMEM_DDESHARE, bufLen + 1);
	if(hMem == NULL)
	{
		return NULL;
	}

	char* lpMem = (char *)GlobalLock(hMem);
	memset(lpMem, 0, bufLen + 1);
	memcpy(lpMem, (char*)sBuf, bufLen);
	GlobalUnlock(hMem);

	HBITMAP hBitmap = NULL;
	IPicture* pPicture = readPicture(hMem, bufLen);
	if(pPicture != NULL)
	{
		HBITMAP hBitmap2 = NULL;
		pPicture->get_Handle((OLE_HANDLE *)&hBitmap2);
		if(hBitmap2 != NULL)
		{
			hBitmap = (HBITMAP)CopyImage(hBitmap2, IMAGE_BITMAP, 0, 0, LR_COPYRETURNORG); 
		}
		pPicture->Release();
	}
	::GlobalFree(hMem);
	return hBitmap;
}
bool Util::saveBitmap(const TCHAR* szFile, HBITMAP hBitmap)
{
	HDC hDC;
	BITMAP Bitmap0;
	DWORD dwPaletteSize = 0, dwBmBitsSize = 0, dwDIBSize = 0, dwWritten = 0;
	WORD wBitCount;
	HANDLE fh, hDib, hPal,hOldPal2 = NULL;
	BITMAPINFOHEADER bi;
	LPBITMAPINFOHEADER lpbi;
	BITMAPFILEHEADER bmfHdr;
	/*
	int iBits;
	HANDLE fh, hDib, hPal,hOldPal2 = NULL;
	hDC = CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
	iBits = GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES);
	DeleteDC(hDC);
	if(iBits <= 1)
	{
		wBitCount = 1;
	}
	else if(iBits <= 4)
	{
		wBitCount = 4;
	}
	else if(iBits <= 8)
	{
		wBitCount = 8;
	}
	else if(iBits <= 16)
	{
		wBitCount = 16;
	}
	else if(iBits <= 24)
	{
		wBitCount = 24;
	}
	else
	{
		wBitCount = 32;
	}*/
	GetObject(hBitmap, sizeof(Bitmap0), (LPSTR)&Bitmap0);
	wBitCount = Bitmap0.bmBitsPixel;
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = Bitmap0.bmWidth;
	bi.biHeight = -Bitmap0.bmHeight;
	bi.biPlanes = 1;
	bi.biBitCount = wBitCount;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrImportant = 0;
	bi.biClrUsed = 256;
	dwBmBitsSize = (((Bitmap0.bmWidth * wBitCount +31) & ~31) /8) * Bitmap0.bmHeight; 
	hDib = GlobalAlloc(GHND,dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER));
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
	*lpbi = bi;

	hPal = GetStockObject(DEFAULT_PALETTE);
	if(hPal)
	{ 
		hDC = GetDC(NULL);
		hOldPal2 = SelectPalette(hDC, (HPALETTE)hPal, FALSE);
		RealizePalette(hDC);
	}

	GetDIBits(hDC, hBitmap, 0, (UINT) Bitmap0.bmHeight, (LPSTR)lpbi + sizeof(BITMAPINFOHEADER) +dwPaletteSize,
		(BITMAPINFO *)lpbi, DIB_RGB_COLORS);

	if(hOldPal2)
	{
		SelectPalette(hDC, (HPALETTE)hOldPal2, TRUE);
		RealizePalette(hDC);
		ReleaseDC(NULL, hDC);
	}

	fh = CreateFile(szFile, GENERIC_WRITE,0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL); 
	if(fh == INVALID_HANDLE_VALUE)
	{
		return false; 
	}

	bmfHdr.bfType = 0x4D42; // "BM"
	dwDIBSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize;
	bmfHdr.bfSize = dwDIBSize;
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + dwPaletteSize;

	WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);
	WriteFile(fh, (LPSTR)lpbi, dwDIBSize, &dwWritten, NULL);

	GlobalUnlock(hDib);
	GlobalFree(hDib);
	CloseHandle(fh);
	return true;
}
//IPicture* Util::IPicture2Bmp(IPicture* pPicture)
//{
//	if(pPicture == NULL)
//	{
//		return NULL;
//	}
//	HBITMAP hBitmap = NULL;
//	pPicture->get_Handle((OLE_HANDLE *)&hBitmap);
//	return hBitmap;
//	//HDC hDC2 = hDC;
//	//if(hDC == NULL)
//	//{
//	//	hDC2 = GetDC(NULL);
//	//}
//	//long hmWidth;
//	//long hmHeight; // HIMETRIC units
//	//pPicture->get_Width(&hmWidth);
//	 //pPicture->get_Height(&hmHeight);
//	//if(sizeW <= 0)
//	//{
//	//	sizeW = (int)hmWidth;
//	//}
//	//if(sizeH <= 0)
//	//{
//	//	sizeH = (int)hmHeight;
//	//}
//	//HDC dcMem = ::CreateCompatibleDC(hDC2);
//	//HBITMAP hBitmap = ::CreateCompatibleBitmap(hDC2, sizeW, sizeH);
//	//HBITMAP OldBmp = (HBITMAP)::SelectObject(dcMem, hBitmap);
//
//	//RECT rc = {0, 0, sizeW, sizeH};
//	//if(corBack != (COLORREF)CLR_INVALID)
//	//{
//	//	HBRUSH hBrush = ::CreateSolidBrush(corBack);
//	//	::FillRect(dcMem, &rc, hBrush);
//	//	::DeleteObject(hBrush);
//	//}
//	///*
//	//HRESULT   Render(  
//	//HDC   hdc,   //Handle   of   device   context   on   which   to   render   the   image  
//	//long   x,     //Horizontal   position   of   image   in   hdc  
//	//long   y,     //Vertical   position   of   image   in   hdc  
//	//long   cx,   //Horizontal   dimension   of   destination   rectangle  
//	//long   cy,   //Vertical   dimension   of   destination   rectangle  
//	//OLE_XPOS_HIMETRIC   xSrc,		//Horizontal   offset   in   source   picture  
//	//OLE_YPOS_HIMETRIC   ySrc,		//Vertical   offset   in   source   picture  
//	//OLE_XSIZE_HIMETRIC   cxSrc,		//Amount to copy horizontally in source picture  
//	//OLE_YSIZE_HIMETRIC   cySrc,		//Amount to copy vertically in source picture  
//	//LPCRECT   prcWBounds			//Pointer to position of destination for a metafile hdc  
//	//);
//	//*/
//	//HRESULT hr = pPicture->Render(dcMem,
//	//								0,
//	//								0,
//	//								sizeW,
//	//								sizeH,
//	//								0,
//	//								hmHeight,
//	//								hmWidth,
//	//								-hmHeight,
//	//								&rc);
//	////::DrawIconEx(dcMem, 0, 0, hicon, sizeW, sizeH, 0, hBrush, DI_NORMAL);
//
//	//::SelectObject(dcMem, OldBmp);
//	//DeleteDC(dcMem);
//	//if(hDC == NULL)
//	//{
//	//	ReleaseDC(NULL, hDC2);
//	//}
//	//return hBitmap;
//}

int Util::MsgBox(HWND hWnd, const TCHAR *msg, const TCHAR *title, DWORD nButton, DWORD nResourceID)
{
	if(title == NULL)
	{
		title = _T("Message...");
	}
	MSGBOXPARAMS msgBox;
	memset(&msgBox, 0, sizeof(MSGBOXPARAMS));
	msgBox.cbSize = sizeof(MSGBOXPARAMS);
	msgBox.hInstance = ::GetModuleHandle(NULL);
	msgBox.hwndOwner = hWnd;
	msgBox.lpszCaption = title;
	if(nResourceID > 0)
	{
		msgBox.lpszIcon = MAKEINTRESOURCE(nResourceID);
		nButton |= MB_USERICON;
	}
	msgBox.dwStyle = nButton;
	msgBox.lpszText = msg;

	return (int)::MessageBoxIndirect(&msgBox);
}

//fdwSound=SND_FILENAME|SND_ASYNC|SND_NOWAIT|SND_NODEFAULT
bool Util::PlaySound(LPCWSTR pszSound, HMODULE hmod, DWORD fdwSound)
{
	BOOL ret = ::PlaySoundW(pszSound, hmod, fdwSound);
	return (ret == TRUE);
}

//get windows system
//http://msdn.microsoft.com/en-us/library/windows/desktop/ms724833(v=vs.85).aspx
WindowsVersion::Type Util::GetWindowsVersion(int* is64)
{
	WindowsVersion::Type winType = (WindowsVersion::Type)CC_GetWindowsVersion(is64);
	return winType;
}

cc::Str Util::GetWindowsVersionName(WindowsVersion::Type ver, int is64)
{
	cc::Str sRet;
	if((int)ver == (int)cc::WindowsVersion::WindowsNT3)
	{
		sRet = _T("WindowsNT3");
	}
	else if((int)ver == (int)cc::WindowsVersion::Windows95)
	{
		sRet = _T("Windows95");
	}
	else if((int)ver == (int)cc::WindowsVersion::WindowsNT4)
	{
		sRet = _T("WindowsNT4");
	}
	else if((int)ver == (int)cc::WindowsVersion::Windows98)
	{
		sRet = _T("Windows98");
	}
	else if((int)ver == (int)cc::WindowsVersion::WindowsME)
	{
		sRet = _T("WindowsME");
	}
	else if((int)ver == (int)cc::WindowsVersion::Windows2000)
	{
		sRet = _T("Windows2000");
	}
	else if((int)ver == (int)cc::WindowsVersion::WindowsXP)
	{
		sRet = _T("WindowsXP");
	}
	else if((int)ver == (int)cc::WindowsVersion::WindowsServer2003)
	{
		sRet = _T("WindowsServer2003");
	}
	else if((int)ver == (int)cc::WindowsVersion::WindowsHomeServer)
	{
		sRet = _T("WindowsHomeServer");
	}
	else if((int)ver == (int)cc::WindowsVersion::WindowsServer2003R2)
	{
		sRet = _T("WindowsServer2003R2");
	}
	else if((int)ver == (int)cc::WindowsVersion::WindowsVista)
	{
		sRet = _T("WindowsVista");
	}
	else if((int)ver == (int)cc::WindowsVersion::WindowsServer2008)
	{
		sRet = _T("WindowsServer2008");
	}
	else if((int)ver == (int)cc::WindowsVersion::WindowsServer2008R2)
	{
		sRet = _T("WindowsServer2008R2");
	}
	else if((int)ver == (int)cc::WindowsVersion::Windows7)
	{
		sRet = _T("Windows7");
	}
	else if((int)ver == (int)cc::WindowsVersion::WindowsNT3)
	{
		sRet = _T("WindowsNT3");
	}
	else if((int)ver == (int)cc::WindowsVersion::WindowsServer2012)
	{
		sRet = _T("WindowsServer2012");
	}
	else if((int)ver == (int)cc::WindowsVersion::Windows8)
	{
		sRet = _T("Windows8");
	}
	else if((int)ver == (int)cc::WindowsVersion::WindowsServer2012R2)
	{
		sRet = _T("WindowsServer2012R2");
	}
	else if((int)ver == (int)cc::WindowsVersion::Windows8_1)
	{
		sRet = _T("Windows8_1");
	}
	else if((int)ver == (int)cc::WindowsVersion::WindowsNext)
	{
		sRet = _T("WindowsNext");
	}
	else
	{
		sRet = _T("Unknow");
	}
	if(is64 == 0)
	{
		sRet.Append(_T("(32bit)"));
	}
	else if(is64 == 1)
	{
		sRet.Append(_T("(64bit)"));
	}
	return sRet;
}

bool Util::GetIsNT()
{
	static DWORD winVer = 0;
	if(winVer == 0)
	{
		winVer = ::GetVersion();
	}
	return (winVer < 0x80000000);
}

//only valid for w2k&xp,nParcent is 0..255
int Util::GetTransparent(HWND hWnd)
{
	int ret = 255;
	if(::IsWindow(hWnd))
	{
		typedef BOOL (WINAPI *lpfn) (HWND hWnd, COLORREF* cr, BYTE* bAlpha, DWORD* dwFlags);
		lpfn pFun;
		// get SetLayeredWindowAttributes from User32.dll
		HMODULE hUser32 = GetModuleHandle(_T("USER32.DLL"));
		if(hUser32 != NULL)
		{
			//BOOL WINAPI SetLayeredWindowAttributes(
			//__in  HWND hwnd,
			//__in  COLORREF crKey,
			//__in  BYTE bAlpha,
			//__in  DWORD dwFlags
			//);
			pFun = (lpfn)GetProcAddress(hUser32, "GetLayeredWindowAttributes");
			if(pFun != NULL)
			{
				#define CC_WS_EX_LAYERED 0x00080000 
				LONG lWindowLong = GetWindowLong(hWnd, GWL_EXSTYLE) | CC_WS_EX_LAYERED;
				::SetWindowLong(hWnd, GWL_EXSTYLE, lWindowLong);
				COLORREF pcrKey = (COLORREF)NULL;
				BYTE pbAlpha = (BYTE)255;
				DWORD pdwFlags = (DWORD)2;
				if(pFun(hWnd, &pcrKey, &pbAlpha, &pdwFlags) == TRUE)
				{
					ret = (int)pbAlpha;
				}
				::RedrawWindow(hWnd, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_FRAME | RDW_ALLCHILDREN);
			}
			FreeLibrary(hUser32);
		}
	}
	return ret;
}
//only valid for w2k&xp,nParcent is 0..255
void Util::SetTransparent(HWND hWnd, int nParcent)
{
	if(::IsWindow(hWnd))
	{
		typedef BOOL (WINAPI *lpfn) (HWND hWnd, COLORREF cr, BYTE bAlpha, DWORD dwFlags);
		lpfn pFun;
		// get SetLayeredWindowAttributes from User32.dll
		HMODULE hUser32 = GetModuleHandle(_T("USER32.DLL"));
		if(hUser32 != NULL)
		{
			//BOOL WINAPI SetLayeredWindowAttributes(
			//__in  HWND hwnd,
			//__in  COLORREF crKey,
			//__in  BYTE bAlpha,
			//__in  DWORD dwFlags
			//);
			pFun = (lpfn)GetProcAddress(hUser32, "SetLayeredWindowAttributes");
			if(pFun != NULL)
			{

				#define CC_WS_EX_LAYERED 0x00080000 
				LONG lWindowLong = GetWindowLong(hWnd, GWL_EXSTYLE) | CC_WS_EX_LAYERED;
				::SetWindowLong(hWnd, GWL_EXSTYLE, lWindowLong);
				pFun(hWnd, 0, (BYTE)nParcent, 2);//LWA_ALPHA
				::RedrawWindow(hWnd, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_FRAME | RDW_ALLCHILDREN);
			}
			FreeLibrary(hUser32);
		}
	}
}

//not same to AdjustWindowRectEx(from client size to window size).
void Util::AdjustWindowRect(int& left, int& top, int& width, int& height, int minWidth, int minHeight, int maxWidth, int maxHeight)
{
	RECT rect = {0, 0, 0, 0};
	POINT pt;
	pt.x = left;
	pt.y = top;
	HMONITOR hm = ::MonitorFromPoint(pt, MONITOR_DEFAULTTONEAREST);
	if(hm != NULL)
	{
		MONITORINFO mix;
		mix.cbSize = sizeof(mix);
		if(GetMonitorInfo(hm, &mix))
		{
			CopyRect(&rect, &mix.rcWork);
		}
		else
		{
			hm = NULL;
		}
	}
	if(hm == NULL)
	{
		if(!::SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, 0))
		{
			rect.right = (LONG)::GetSystemMetrics(SM_CXSCREEN);
			rect.bottom = (LONG)::GetSystemMetrics(SM_CYSCREEN);
		}
	}
	CC_DEBUG(("SM_CXSCREEN:%d, SM_CYSCREEN:%d, SM_CXFULLSCREEN:%d, SM_CYFULLSCREEN:%d, t:%d, l:%d, b:%d, r:%d",
		(int)::GetSystemMetrics(SM_CXSCREEN), (int)::GetSystemMetrics(SM_CYSCREEN),
		(int)::GetSystemMetrics(SM_CXFULLSCREEN), (int)::GetSystemMetrics(SM_CYFULLSCREEN),
		rect.top, rect.left, rect.bottom, rect.right));
	if(width < minWidth)
	{
		width = minWidth;
	}
	if(maxWidth > 0 && width > maxWidth)
	{
		width = maxWidth;
	}
	//if(width > rect.right - rect.left)
	//{
	//	width = rect.right - rect.left;
	//}
	if(height < minHeight)
	{
		height = minHeight;
	}
	if(maxHeight > 0 && height > maxHeight)
	{
		height = maxHeight;
	}
	//if(height > rect.bottom - rect.top)
	//{
	//	height = rect.bottom - rect.top;
	//}

	if(left + width > rect.right)
	{
		left = rect.right - width;
	}
	if(left < rect.left)
	{
		left = rect.left;
	}
	if(top + height > rect.bottom)
	{
		top = rect.bottom - height;
	}
	if(top < rect.top)
	{
		top = rect.top;
	}
	CC_DEBUG(("t:%d, l:%d, b:%d, r:%d", rect.top, rect.left, rect.bottom, rect.right));
}

void Util::FourceWindowTop(HWND hWnd)
{
	HWND hCurrWnd = ::GetForegroundWindow();
	int iMyTID   = GetCurrentThreadId();
	int iCurrTID = GetWindowThreadProcessId(hCurrWnd, 0);

	::AttachThreadInput(iMyTID, iCurrTID, TRUE);
	::SetForegroundWindow(hWnd);
	::AttachThreadInput(iMyTID, iCurrTID, FALSE);
	::BringWindowToTop(hWnd);

	//typedef void (WINAPI *PROCSWITCHTOTHISWINDOW)(HWND, BOOL);
	//PROCSWITCHTOTHISWINDOW SwitchToThisWindow;
	//HMODULE hUser32 = GetModuleHandle("user32");
	//SwitchToThisWindow = (PROCSWITCHTOTHISWINDOW)GetProcAddress(hUser32, "SwitchToThisWindow");

	//2:
	//SetWindowPos(HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	//SetWindowPos(HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);

	//3:
	/*
 	HMODULE hModule = NULL;
	ASFW_PTR pProcAddress = NULL;
	hModule = GetModuleHandle("User32");
	if(hModule != NULL)
	{
		long (*pStartMsgHook)(HINSTANCE, HWND) = NULL;
		pStartMsgHook = (long(*)(HINSTANCE, HWND))::GetProcAddress(glhInstance, "StartMsgHook");

		pProcAddress = (ASFW_PTR)GetProcAddress(hModule, "AllowSetForegroundWindow");
		if(pProcAddress != NULL)
		{
			nResult = (*pProcAddress)(-1);

			if(nResult == TRUE )
			{
				return;
			}
		}
	}
	*/
}

//////////////////
// Note that windows are enumerated in top-down Z-order, so the menu
// window should always be the first one found.
static BOOL CALLBACK EnumTopMenuProc(HWND hwnd, LPARAM lParam)
{
	TCHAR buf[16];
	GetClassName(hwnd, buf, sizeof(buf) / sizeof(TCHAR));
	if(buf != NULL && _tcscmp(buf, _T("#32768")) == 0) // special classname for menus
	{
		*((HWND*)lParam) = hwnd; // found it
		return FALSE;
	}
	return TRUE;
}

HWND Util::GetTopMenuHwnd()
{
	HWND hwnd = NULL;
	::EnumWindows(EnumTopMenuProc, (LPARAM)&hwnd);
	return hwnd;
}

cc::Str Util::GetToday(const TCHAR* splitDate, bool isTime)
{
	struct tm *tm;
	time_t t;
	t = time(NULL);
#ifdef __STDC_SECURE_LIB__
	struct tm ptmTemp1 = {0};
	localtime_s(&ptmTemp1, &t);
	tm = &ptmTemp1;
#else
	tm = localtime(&t);
#endif
	cc::Str sRet;
	if(splitDate == NULL)
	{
		splitDate = _T("");
	}
	if(isTime)
	{
		sRet.Format(_T("%04d%s%02d%s%02d %02d:%02d:%02d"), 
			tm->tm_year + 1900, splitDate, tm->tm_mon + 1, splitDate, tm->tm_mday,
			tm->tm_hour, tm->tm_min, tm->tm_sec);
	}
	else
	{
		sRet.Format(_T("%04d%s%02d%s%02d"), 
			tm->tm_year + 1900, splitDate, tm->tm_mon + 1, splitDate, tm->tm_mday);
	}
	return sRet;
}

int Util::GetTodayInt()
{
	//now from BIOS
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	return (int)(sysTime.wYear * 10000 + sysTime.wMonth * 100 + sysTime.wDay);
}

int Util::GetDateInt(const cc::Str& sDate)
{
	int nRet = 0;
	if(sDate.GetLength() == 8)
	{
		nRet = sDate.Substring(0, 4).ToInt() * 10000 + sDate.Substring(4, 6).ToInt() * 100 + sDate.Substring(6, 8).ToInt();
	}
	else if(sDate.GetLength() == 10)
	{
		nRet = sDate.Substring(0, 4).ToInt() * 10000 + sDate.Substring(5, 7).ToInt() * 100 + sDate.Substring(8, 10).ToInt();
	}
	return nRet;
}

int Util::GetDateAdd(int nDate, int nAddY, int nAddM, int nAddD, int nAddHour, int nAddMin, int nAddSec)
{
	struct tm tm;
	tm.tm_sec = 0;
	tm.tm_min = 0;
	tm.tm_hour = 0;
	tm.tm_year = (int)(nDate/10000) - 1900;     // tm_year is 1900 based
	tm.tm_mon = (int)((nDate - tm.tm_year*10000)/100) - 1;        // tm_mon is 0 based
	tm.tm_mday = nDate - tm.tm_year*10000 - tm.tm_mon*100;
	tm.tm_isdst = 0;

	tm.tm_year += nAddY;
	tm.tm_mon += nAddM;

	time_t t;
	t = mktime(&tm);
	t += (((nAddD * 24 + nAddHour) * 60 + nAddMin) * 60 + nAddSec);
#ifdef __STDC_SECURE_LIB__
	struct tm ptmTemp1 = {0};
	localtime_s(&ptmTemp1, &t);
	struct tm *ptmTemp = &ptmTemp1;
#else
	struct tm *ptmTemp = localtime(&t);
#endif
	return (ptmTemp->tm_year * 10000 + ptmTemp->tm_mon * 100 + ptmTemp->tm_mday);
}

int Util::GetSystemDate(int& nBios, int& nFile)
{
	//now from BIOS
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	nBios = (int)(sysTime.wYear * 10000 + sysTime.wMonth * 100 + sysTime.wDay);
	if(nFile == -1)
	{
		nFile = nBios;
		return nFile;
	}

	nFile = 0;
	//now from system.dat
	TCHAR m_tcFile[_MAX_FNAME];
	memset(m_tcFile, 0, _MAX_FNAME * sizeof(TCHAR));
	::GetWindowsDirectory(m_tcFile, _MAX_FNAME);
	DWORD winVer = ::GetVersion();
	if(winVer < 0x80000000)
	{
		//_tcscat(m_tcFile, _T("\\system32\\config\\system"));
		cc::Buf::Cat(m_tcFile, _MAX_FNAME, _T("\\system32\\config\\system"));
	}
	else
	{
		//_tcscat(m_tcFile, _T("\\system.dat"));
		cc::Buf::Cat(m_tcFile, _MAX_FNAME, _T("\\system.dat"));
	}
	WIN32_FIND_DATA find_buf;
	HANDLE hFile = FindFirstFile(m_tcFile, &find_buf);
	if(hFile != INVALID_HANDLE_VALUE)
	{
		FILETIME localTime;
		if(FileTimeToLocalFileTime(&find_buf.ftLastWriteTime, &localTime) == TRUE)
		{
			// then convert that time to system time
			if(FileTimeToSystemTime(&localTime, &sysTime))
			{
				nFile = sysTime.wYear * 10000 + sysTime.wMonth * 100 + sysTime.wDay;
#if _DEBUG_
				cc::Log::trace(_T("Util::GetSystemDate, File:%s, Time:%d, BIOS:%d"), m_tcFile, nFile, nBios);
#endif
			}
		}
		::FindClose(hFile);
	}
	return (CC_MAX(nBios, nFile));
}

FILETIME Util::GetFiletime(const TCHAR *fullpath, bool isToLocal)
{
	FILETIME localTime = {0};
	WIN32_FIND_DATA find_buf;
	HANDLE hFile = FindFirstFile(fullpath, &find_buf);
	if(hFile != INVALID_HANDLE_VALUE)
	{
		::FindClose(hFile);
		if(!isToLocal)
		{
			return find_buf.ftLastWriteTime;
		}
		FileTimeToLocalFileTime(&find_buf.ftLastWriteTime, &localTime);
	}
	return localTime;
}

bool Util::setFileTime(const TCHAR *fullpath, FILETIME& creationTime, FILETIME& lastAccessTime, FILETIME& lastWriteTime)
{
	HANDLE hFile = ::CreateFile(fullpath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, NULL, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	::SetFileTime(hFile, &creationTime, &lastAccessTime, &lastWriteTime);
	::CloseHandle(hFile);
	return true;
}

time_t Util::Filetime2Time(const FILETIME& ft)  
{  
	ULARGE_INTEGER ui;
	ui.LowPart = ft.dwLowDateTime;
	ui.HighPart = ft.dwHighDateTime;
	return ((time_t)(ui.QuadPart - 116444736000000000) / 10000000);
}

int Util::CompareFiletime(const FILETIME &a, const FILETIME &b)
{
	LARGE_INTEGER la, lb;
	la.LowPart = a.dwLowDateTime;
	la.HighPart = a.dwHighDateTime;
	lb.LowPart = b.dwLowDateTime;
	lb.HighPart = b.dwHighDateTime;
	if(la.QuadPart == lb.QuadPart)
	{
		return 0;
	}
	else if(la.QuadPart > lb.QuadPart)
	{
		return 1;
	}
	return -1;
}

int Util::WMI_Query(const char* sql, const wchar_t* field, cc::Str &sResult)
{
    HRESULT hres;

	// Initialize COM.
    hres =  CoInitializeEx(0, COINIT_MULTITHREADED);
    if(FAILED(hres))
    {
        //cout << "Failed to initialize COM library. " 
        //    << "Error code = 0x" 
        //    << hex << hres << endl;
        return -1;              // Program has failed.
    }

    // Initialize
    hres =  CoInitializeSecurity(
        NULL,
        -1,      // COM negotiates service           
        NULL,    // Authentication services
        NULL,    // Reserved
        RPC_C_AUTHN_LEVEL_DEFAULT,    // authentication
        RPC_C_IMP_LEVEL_IMPERSONATE,  // Impersonation
        NULL,             // Authentication info
        EOAC_NONE,        // Additional capabilities
        NULL              // Reserved
        );
    if(FAILED(hres))
    {
        //cout << "Failed to initialize security. " 
        //    << "Error code = 0x" 
        //    << hex << hres << endl;
        //CoUninitialize();
		//error in thread, no return OK!
        //return -1;          // Program has failed.
    }

    // Obtain the initial locator to Windows Management
    // on a particular host computer.
    IWbemLocator *pLoc = 0;
    hres = CoCreateInstance(
        CLSID_WbemLocator,
        0,
        CLSCTX_INPROC_SERVER,
        IID_IWbemLocator,
		(LPVOID *) &pLoc);
	if(FAILED(hres))
    {
        //cout << "Failed to create IWbemLocator object. "
        //    << "Error code = 0x"
        //    << hex << hres << endl;
        CoUninitialize();
        return -1;       // Program has failed.
    }

    IWbemServices *pSvc = 0;
    // Connect to the root\cimv2 namespace with the
    // current user and obtain pointer pSvc
    // to make IWbemServices calls.
    hres = pLoc->ConnectServer(
        _bstr_t(L"ROOT\\CIMV2"), // WMI namespace
        NULL,                    // User name
        NULL,                    // User password
        0,                       // Locale
        NULL,                    // Security flags                 
        0,                       // Authority       
        0,                       // Context object
        &pSvc                    // IWbemServices proxy
        );                              
    if(FAILED(hres))
    {
        //cout << "Could not connect. Error code = 0x" 
        //    << hex << hres << endl;
        pLoc->Release();
        CoUninitialize();
        return -1;                // Program has failed.
    }

    //cout << "Connected to ROOT\\CIMV2 WMI namespace" << endl;
    // Set the IWbemServices proxy so that impersonation
    // of the user (client) occurs.
    hres = CoSetProxyBlanket(
       pSvc,                         // the proxy to set
       RPC_C_AUTHN_WINNT,            // authentication service
       RPC_C_AUTHZ_NONE,             // authorization service
       NULL,                         // Server principal name
       RPC_C_AUTHN_LEVEL_CALL,       // authentication level
       RPC_C_IMP_LEVEL_IMPERSONATE,  // impersonation level
       NULL,                         // client identity 
       EOAC_NONE                     // proxy capabilities     
    );
    if(FAILED(hres))
    {
        //cout << "Could not set proxy blanket. Error code = 0x" 
        //     << hex << hres << endl;
        pSvc->Release();
        pLoc->Release();     
        CoUninitialize();
        return -1;               // Program has failed.
    }

    // Use the IWbemServices pointer to make requests of WMI. 
    // Make requests here:
    // For example, query for all the running processes
    IEnumWbemClassObject* pEnumerator = NULL;
    hres = pSvc->ExecQuery(
        bstr_t("WQL"),
        bstr_t(sql), //"SELECT * FROM Win32_Process"
        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
        NULL,
        &pEnumerator);
    if(FAILED(hres))
    {
        //cout << "Query for processes failed. "
        //     << "Error code = 0x"
        //     << hex << hres << endl;
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return -1;               // Program has failed.
    }

	cc::Log::debug("sql:%s", sql);
    while (pEnumerator)
    {
		IWbemClassObject *pclsObj = NULL;
		ULONG uReturn = 0;

		hres = pEnumerator->Next(WBEM_INFINITE, 1,
            &pclsObj, &uReturn);
        if(uReturn == 0)
        {
            break;
        }

        VARIANT vtProp;
		VariantInit(&vtProp);
        // Get the value of the Name property
        hres = pclsObj->Get(field, 0, &vtProp, 0, 0);
		//StringCchCopy( properties[iTotal].szProperty, PROPERTY_MAX_LEN, W2T(vtProperty.bstrVal) );
        //wcout << "Process Name : " << vtProp.bstrVal << endl;
		if(!sResult.IsBlankWithoutTrim())
		{
			sResult.Append(_T("|"));
		}
		cc::Str sTemp;
		if(vtProp.vt == (VT_BSTR | VT_ARRAY))
		{
			BSTR firstField;
			long index = 0;
			hres = ::SafeArrayGetElement(V_ARRAY(&vtProp), &index, &firstField);
			cc::Log::debug(L"sql result:%s", (const wchar_t*)firstField);
			sTemp = cc::Str((const wchar_t*)firstField);
			::SysFreeString(firstField);
		}
		else
		{
			cc::Log::debug(L"sql result:%s", (const wchar_t*)vtProp.bstrVal);
			sTemp = cc::Str((const wchar_t*)vtProp.bstrVal);
		}
		sTemp.Trim();
		sResult.Append(sTemp);
        VariantClear(&vtProp);

		pclsObj->Release(); 
	}

    // Cleanup
    // ========
	if(pEnumerator)
	{
		pEnumerator->Release();
	}
    pSvc->Release();
    pLoc->Release();     
    CoUninitialize();

    return 0;   // Program successfully completed.
}

//this may spent 4seconds
//sample:BFEBFBFF00040651
int Util::WMI_ProcessorId(cc::Str &sResult)
{
//	cc::Str sRet = cc::Util::RunCmd(_T("wmic cpu get processorid"));
	const char* sql = "SELECT * FROM Win32_Processor WHERE (ProcessorId IS NOT NULL)";
	const wchar_t* field = L"ProcessorId";
	return WMI_Query(sql, field, sResult);
}
//sample:TF0501Y93YAD3R|002352101547
int Util::WMI_DiskSerialNumber(int nDriver, cc::Str &sResult)
{
//	cc::Str sRet = cc::Util::RunCmd(_T("wmic diskdrive WHERE index=0 get SerialNumber"));
	//const char* sql = "SELECT * FROM Win32_PhysicalMedia WHERE (SerialNumber IS NOT NULL) ";
	const char* sql = "SELECT * FROM Win32_DiskDrive WHERE (SerialNumber IS NOT NULL) AND (MediaType LIKE 'Fixed hard disk%%') AND (Index = %d)";
	cc::Str str = cc::Str(LangCode::SYSTEM);
	str.Format(sql, nDriver);
	const wchar_t* field = L"SerialNumber";
	return WMI_Query((const char*)str.GetBuf(), field, sResult);
}
//sample:CNU402C03W
int Util::WMI_BIOSSerialNumber(cc::Str &sResult)
{
	const char* sql = "SELECT * FROM Win32_BIOS WHERE (SerialNumber IS NOT NULL)";
	const wchar_t* field = L"SerialNumber";
	return WMI_Query(sql, field, sResult);
}
//sample:PDXVC001X5XKY4
int Util::WMI_BoardSerialNumber(cc::Str &sResult)
{
//	cc::Str sRet = cc::Util::RunCmd(_T("wmic BaseBoard get SerialNumber"));
	const char* sql = "SELECT * FROM Win32_BaseBoard WHERE (SerialNumber IS NOT NULL)";
	const wchar_t* field = L"SerialNumber";
	return WMI_Query(sql, field, sResult);
}
//cc::Str WinUtil::GetMAC(cc::Str sSkipIp_Pre, bool isWithIpAddress)
//sample:A0:1D:48:F7:62:8C|FC:F8:AE:B3:6A:A5
int Util::WMI_NetworkMac(cc::Str &sResult, bool noWireless)
{
	//const char* sql = "SELECT * FROM Win32_NetworkAdapter WHERE (MACAddress IS NOT NULL) AND (NOT (PNPDeviceID LIKE 'ROOT%')) AND (Manufacturer != 'Microsoft') AND (ConfigManagerErrorCode = 0 OR (ConfigManagerErrorCode = 22 AND NetConnectionStatus = 0))";
	cc::Str sql = cc::Str(LangCode::SYSTEM, "SELECT * FROM Win32_NetworkAdapter WHERE (PhysicalAdapter='TRUE') AND (AdapterTypeId=0) AND (NOT (PNPDeviceID LIKE 'ROOT%')) AND (NOT (Name LIKE '% Virtual %'))");
	if(noWireless)
	{
		sql.Append(" AND (NOT (Name LIKE '%Wireless%'))");
	}
	const wchar_t* field = L"MACAddress";
	return WMI_Query(sql, field, sResult);
}
//cc::Str WinUtil::GetMAC(cc::Str sSkipIp_Pre, bool isWithIpAddress)
//sample:A0:1D:48:F7:62:8C|FC:F8:AE:B3:6A:A5
int Util::WMI_NetworkIp(cc::Str &sResult, bool noWireless)
{
	//const char* sql = "SELECT * FROM Win32_NetworkAdapter WHERE (MACAddress IS NOT NULL) AND (NOT (PNPDeviceID LIKE 'ROOT%')) AND (Manufacturer != 'Microsoft') AND (ConfigManagerErrorCode = 0 OR (ConfigManagerErrorCode = 22 AND NetConnectionStatus = 0))";
	cc::Str sql = cc::Str(LangCode::SYSTEM, "SELECT * FROM Win32_NetworkAdapterConfiguration WHERE (MACAddress IS NOT NULL) AND (IPEnabled='TRUE') AND (ServiceName<>'msloop') AND (ServiceName<>'VMnetAdapter') AND (NOT (Caption LIKE '% Virtual %'))");
	if(noWireless)
	{
		sql.Append(" AND (NOT (Caption LIKE '%Wireless%'))");
	}
	const wchar_t* field = L"IPAddress";
	return WMI_Query(sql, field, sResult);
}

/*
https://msdn.microsoft.com/library/hskdteyh(v=vs.100).aspx

When the InfoType argument is 0, the following table describes the output.
Information Returned
Array index Bit range Description
0 0-31 Maximum value for the InfoType parameter that will return valid information.
1 0-31 Identification string (part 1)
2 0-31 Identification string (part 3)
3 0-31 Identification string (part 2)

When the InfoType?argument is 1, the following table describes the output.
CPU Information Returned
Array index	Bit range	Description
0	0-3	Stepping ID
0	4-7	Model
0	8-11	Family
0	12-13	Processor type (Intel), Reserved (AMD)
0	14-15	Reserved
0	16-19	Extended model
0	20-27	Extended family
0	28-31	Reserved
1	0-7	Brand index
1	8-15	The size of the CLFLUSH cache line, in quadwords
1	16-23	Logical processor count
1	24-31	Initial APIC ID
2	0	SSE3 new instructions
2	1-2	Reserved
2	3	MONITOR/MWAIT
2	4	CPL Qualified Debug Store (Intel), Reserved (AMD)
2	5	Virtual machine extensions (Intel), Reserved (AMD)
2	6	Safer mode extensions (Intel), Reserved (AMD)
2	7	Enhanced Intel SpeedStep technology (Intel), Reserved (AMD)
2	8	Thermal Monitor (Intel), Reserved (AMD)2
2	9	Supplemental Streaming SIMD Extensions 3 (SSSE3)
2	10	L1 context ID (Intel), Reserved (AMD)
2	11	Reserved
2	12	256-bit FMA extensions (Intel), Reserved (AMD)
2	13	CMPXCHG16B support
2	14	xTPR update control
2	15	Perf/Debug capability MSR
2	16-17	Reserved
2	18	Direct cache access (DCA) support (Intel), Reserved (AMD)
2	19	SSE4.1 extensions
2	20	SSE4.2 extensions
2	21	x2APIC support (Intel), Reserved (AMD)
2	22	MOVBE support (Intel), Reserved (AMD)
2	23	POPCNT instruction support
2	24	Reserved
2	25	AES support (Intel), Reserved (AMD)
2	26	XSAVE support (Intel), Reserved (AMD)
2	27	OSXSAVE support (Intel), Reserved (AMD)
2	28	256-bit Intel advanced vector extensions (Intel), Reserved (AMD)
2	29-31	Reserved
3	0-31	Feature information (see later in the topic)
*/

// pbrand: Intel(R) Core(TM) i5-4300U CPU @ 1.90GHz
int cpu_getbrand(char* pbrand)
{
	INT32 dwBuf[4];
	if(pbrand == NULL)
	{
		return 0;
	}
	// Function 0x80000000: Largest Extended Function Number
	__cpuid(dwBuf, 0x80000000);
	if (dwBuf[0] < 0x80000004)
	{
		return 0;
	}
	// Function 80000002h,80000003h,80000004h: Processor Brand String
	__cpuid((INT32*)&pbrand[0], 0x80000002);    // 16
	__cpuid((INT32*)&pbrand[16], 0x80000003);   // 16
	__cpuid((INT32*)&pbrand[32], 0x80000004);   // 16
	pbrand[48] = '\0';
	return 48;
}

/* _asm:error C4235 while x64 */
cc::Str Util::GetCpuId(bool isFullInfo)
{
	cc::Str sRet;
	INT32 regs[4]; //0:EA, 1:EB, 2:EC, 3:ED
    __cpuid(regs, (int)1); // Identification string (part 1)
	sRet.Format(_T("%u-%u"), regs[0], regs[3]);
	if(isFullInfo)
	{
		char szBuf[64];
		/*
		memset(szBuf, 0x0, sizeof(szBuf));
		// Function 0: Vendor-ID and Largest Standard Function, sapmle:GenuineIntel
		__cpuid(regs, (int)0);
		// save. pvendor
		*(INT32*)&szBuf[0] = regs[1];    // ebx:
		*(INT32*)&szBuf[4] = regs[3];    // edx:
		*(INT32*)&szBuf[8] = regs[2];    // ecx:
		szBuf[12] = '\0';
		//GenuineIntel
		sRet.Append(_T(":")).Append(szBuf);
		*/

		//Intel(R) Core(TM) i5-4300U CPU @ 1.90GHz
		memset(szBuf, 0x0, sizeof(szBuf));
		cpu_getbrand(szBuf);
		cc::Str sRet2 = szBuf;
		sRet.Append(_T(":")).Append(sRet2.Trim());

		/*
		__cpuid(regs, (int)3); // Identification string (part 3)
		cc::Str sRet2;
		sRet2.Format(_T("%u-%u-%u"), regs[0], regs[2], regs[3]);
		sRet.Append(_T(":")).Append(sRet2);
		*/
	}
	return sRet;
}

bool Util::GetDriveSerialNumber(int nDriver, cc::Str& strSerialNumber)
{
    DWORD dwRet = NO_ERROR;
	strSerialNumber.Clear();
 
    // Format physical drive path (may be '\\.\PhysicalDrive0', '\\.\PhysicalDrive1' and so on).
    cc::Str strDrivePath;
    strDrivePath.Format(_T("\\\\.\\PhysicalDrive%u"), nDriver);
 
    // Get a handle to physical drive
    HANDLE hDevice = ::CreateFile(
		strDrivePath, 0, FILE_SHARE_READ|FILE_SHARE_WRITE,
        NULL, OPEN_EXISTING, 0, NULL);
    if(INVALID_HANDLE_VALUE == hDevice)
	{
        return false;
	}
 
    // Set the input data structure
    STORAGE_PROPERTY_QUERY storagePropertyQuery;
    ZeroMemory(&storagePropertyQuery, sizeof(STORAGE_PROPERTY_QUERY));
    storagePropertyQuery.PropertyId = StorageDeviceProperty;
    storagePropertyQuery.QueryType = PropertyStandardQuery;
 
    // Get the necessary output buffer size
    STORAGE_DESCRIPTOR_HEADER storageDescriptorHeader = {0};
    DWORD dwBytesReturned = 0;
    if(!::DeviceIoControl(hDevice, IOCTL_STORAGE_QUERY_PROPERTY,
        &storagePropertyQuery, sizeof(STORAGE_PROPERTY_QUERY),
        &storageDescriptorHeader, sizeof(STORAGE_DESCRIPTOR_HEADER),
        &dwBytesReturned, NULL))
    {
        ::CloseHandle(hDevice);
        return false;
    }
	if((int)storageDescriptorHeader.Size < 1)
	{
        ::CloseHandle(hDevice);
        return false;
	}
 
    // Alloc the output buffer
    const DWORD dwOutBufferSize = storageDescriptorHeader.Size;
    BYTE* pOutBuffer = new BYTE[dwOutBufferSize];
    ZeroMemory(pOutBuffer, dwOutBufferSize);
 
    // Get the storage device descriptor
    if(!::DeviceIoControl(hDevice, IOCTL_STORAGE_QUERY_PROPERTY,
            &storagePropertyQuery, sizeof(STORAGE_PROPERTY_QUERY),
            pOutBuffer, dwOutBufferSize,
            &dwBytesReturned, NULL))
    {
        //dwRet = ::GetLastError();
        delete []pOutBuffer;
        ::CloseHandle(hDevice);
        return false;
    }
 
    // Now, the output buffer points to a STORAGE_DEVICE_DESCRIPTOR structure
    // followed by additional info like vendor ID, product ID, serial number, and so on.
    STORAGE_DEVICE_DESCRIPTOR* pDeviceDescriptor = (STORAGE_DEVICE_DESCRIPTOR*)pOutBuffer;
    const DWORD dwSerialNumberOffset = pDeviceDescriptor->SerialNumberOffset;

	//some machine, here will  be 0!!!
	if((int)dwSerialNumberOffset < 1)
	{
        delete []pOutBuffer;
        ::CloseHandle(hDevice);
        return false;
	}
    // Finally, get the serial number
    strSerialNumber = cc::Str((const char*)(pOutBuffer + dwSerialNumberOffset));
	strSerialNumber.Trim();
 
    // Do cleanup and return
    delete []pOutBuffer;
    ::CloseHandle(hDevice);
    return true;
}

cc::Str Util::GetDriveSerialNumber()
{
	cc::Str str;
	WMI_DiskSerialNumber(0, str);
	if(str.GetLength() > 0)
	{
		return str;
	}

	GetDriveSerialNumber(0, str);
	if(str.GetLength() > 0)
	{
		return str;
	}

	//if error, then get volume number, but changed after format
	TCHAR cVolume[256];
	TCHAR cFileSysName[256];
	DWORD dwSerialNum; //harddisk num
	DWORD dwFileNameLength;
	DWORD dwFileSysFlag;
  
	::GetVolumeInformation(_T("C://"), cVolume, 256, &dwSerialNum, &dwFileNameLength, &dwFileSysFlag, cFileSysName, 256);  
	str.Format(_T("%u"), dwSerialNum);
	return str;
}

cc::Str Util::GetDiskSize(int nDriver, bool isBytes)
{
	DISK_GEOMETRY pdg;            // disk drive geometry structure
	HANDLE hDevice;               // handle to the drive to be examined 
	BOOL bResult;                 // results flag
	DWORD junk;                   // discard results

	cc::Str sResult;
	cc::Str str;
	str.Format(_T("\\\\.\\PhysicalDrive%d"), nDriver);
	hDevice = CreateFile((TCHAR*)str,  // drive to open
		0,                // no access to the drive
		FILE_SHARE_READ | // share mode
		FILE_SHARE_WRITE, 
		NULL,             // default security attributes
		OPEN_EXISTING,    // disposition
		0,                // file attributes
		NULL);            // do not copy file attributes
	if(hDevice == INVALID_HANDLE_VALUE) // cannot open the drive
	{
		return sResult;
	}

	bResult = DeviceIoControl(hDevice,  // device to be queried
		IOCTL_DISK_GET_DRIVE_GEOMETRY,  // operation to perform
		NULL, 0, // no input buffer
		&pdg, sizeof(pdg),     // output buffer
		&junk,                 // # bytes returned
		(LPOVERLAPPED)NULL);  // synchronous I/O
	CloseHandle(hDevice);
	if(bResult != TRUE)
	{
		return sResult;
	}

	// size of the drive, in bytes
	ULONGLONG nDiskSize = pdg.Cylinders.QuadPart * (ULONG)pdg.TracksPerCylinder * (ULONG)pdg.SectorsPerTrack * (ULONG)pdg.BytesPerSector;
	if(isBytes)
	{
		sResult.Format(_T("%I64u"), nDiskSize);
	}
	else
	{
		sResult.Format(_T("%I64u"), nDiskSize/(1024 * 1024));
	}
	return sResult;
}

bool Util::GetDriveSize(const TCHAR* sDriver, bool isBytes, cc::Str& sTotal, cc::Str& sAvailable)
{
    DWORDLONG dwlFreeBytesAvailableToCaller;
    DWORDLONG dwlTotalNumberOfBytes;
    DWORDLONG dwlTotalNumberOfFreeBytes;

    if(GetDiskFreeSpaceEx(sDriver, 
        reinterpret_cast<PULARGE_INTEGER>(&dwlFreeBytesAvailableToCaller),
        reinterpret_cast<PULARGE_INTEGER>(&dwlTotalNumberOfBytes),
        reinterpret_cast<PULARGE_INTEGER>(&dwlTotalNumberOfFreeBytes))) 
    {
		if(isBytes)
		{
			sTotal.Format(_T("%I64u"), dwlTotalNumberOfBytes);
			sAvailable.Format(_T("%I64u"), dwlTotalNumberOfFreeBytes);
		}
		else
		{
			sTotal.Format(_T("%u"), static_cast<unsigned int>(dwlTotalNumberOfBytes/(1024*1024)));
			sAvailable.Format(_T("%u"), static_cast<unsigned int>(dwlTotalNumberOfFreeBytes/(1024*1024)));
		}
		//sTotal.Format(_T("%dM"), static_cast<int>(dwlTotalNumberOfBytes/(1024*1024)));
		//sAvailable.Format(_T("%dM"),  static_cast<int>(dwlTotalNumberOfFreeBytes/(1024*1024)));
		return true;
	}
	return false;
}

DWORDLONG Util::GetPhysicalMemory()
{
	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof (statex);
	GlobalMemoryStatusEx (&statex);

	return statex.ullTotalPhys;
}

//#define RandomNum(min, max) ((rand() % (int)(((max) + 1) - (min))) + (min))
int Util::GetRandom(int min, int max, bool init_srand)
{
	//Intializes random number generator
	static int srand_flg = 0;
	if(srand_flg == 0 || init_srand)
	{
		DWORD d1 = GetTickCount();
		DWORD d2 = GetTickCount();
		while(d1 == d2)
		{
			d2 = GetTickCount();
		}
		srand((unsigned int)d2);
		srand_flg = 1;
	}

	int n = ((rand() % (int)(((max) + 1) - (min))) + (min));
	return n;
}

cc::Str Util::GetRandomString(cc::Str sChars, int nMinLen, int nMaxLen)
{
	if(sChars.GetLength() < 1)
	{
		sChars.ValueOf("ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890abcdefghijklmnopqrstuvwxyz");
	}
	//// initialize random numbers generator
	//srand((unsigned int)GetTickCount());
	cc::Util::GetRandom(0, 10000, true);
	cc::Str strPass(sChars.GetLangCode());
	int nPosLast = -1;
	int nMod = CC_MAX(2, (nMaxLen - nMinLen + 1));
	for(int i = 0; i < nMaxLen; i++)
	{
		int nPos = cc::Util::GetRandom(0, sChars.GetLength() - 1);
		if(nPos == nPosLast)
		{
			nPos = cc::Util::GetRandom(0, sChars.GetLength() - 1);
		}
		nPosLast = nPos;
		strPass.Append(sChars.Substring(nPos, nPos + 1));
		if(i >= nMinLen)
		{
			nPos = cc::Util::GetRandom(0, nMod);
			if(nPos%nMod == 0)
			{
				break;
			}
			nMod--;
		}
	}
	return strPass;
}

//proxyAdressStr=http://proxy:80
//or http=[IPADDRESS:PORT];https=[IPADDRESS:PORT]
bool Util::SetIEProxy(wchar_t * proxyAdressStr, wchar_t* byPass, wchar_t * connNameStr)
{
    INTERNET_PER_CONN_OPTION_LIST conn_options;
    BOOL    bReturn;
    DWORD   dwBufferSize = sizeof(conn_options);
    conn_options.dwSize = dwBufferSize;
    conn_options.pszConnection = connNameStr;//NULL == LAN  

	conn_options.dwOptionCount = 3;
	INTERNET_PER_CONN_OPTION option[3];
	memset(&option, 0x0, sizeof(INTERNET_PER_CONN_OPTION) * 3);
    conn_options.pOptions = option;

	conn_options.pOptions[0].dwOption = INTERNET_PER_CONN_FLAGS;
    conn_options.pOptions[0].Value.dwValue = PROXY_TYPE_DIRECT|PROXY_TYPE_PROXY;
   
    conn_options.pOptions[1].dwOption = INTERNET_PER_CONN_PROXY_SERVER;
    conn_options.pOptions[1].Value.pszValue = proxyAdressStr;
    conn_options.pOptions[2].dwOption = INTERNET_PER_CONN_PROXY_BYPASS;
    conn_options.pOptions[2].Value.pszValue = byPass == NULL ? L"<local>" : byPass;

	bReturn = InternetSetOption(NULL, INTERNET_OPTION_PER_CONNECTION_OPTION, &conn_options, dwBufferSize);

	InternetSetOption(NULL, INTERNET_OPTION_SETTINGS_CHANGED, NULL, 0);
    InternetSetOption(NULL, INTERNET_OPTION_REFRESH , NULL, 0);
    return (bReturn == TRUE);
}

bool Util::SetIEProxyDirect(wchar_t* connectionNameStr)
{
	INTERNET_PER_CONN_OPTION_LIST conn_options;
	BOOL    bReturn;
	DWORD   dwBufferSize = sizeof(conn_options);
		
	conn_options.dwSize = dwBufferSize;
		
	conn_options.pszConnection = connectionNameStr; //NULL - LAN	
	conn_options.dwOptionCount = 1;
	INTERNET_PER_CONN_OPTION option[1];
	memset(&option, 0x0, sizeof(INTERNET_PER_CONN_OPTION) * 1);
    conn_options.pOptions = option;
 
	conn_options.pOptions[0].dwOption = INTERNET_PER_CONN_FLAGS;
	conn_options.pOptions[0].Value.dwValue = PROXY_TYPE_DIRECT  ;
	
	bReturn = InternetSetOption(NULL, INTERNET_OPTION_PER_CONNECTION_OPTION, &conn_options, dwBufferSize);
 
	InternetSetOption(NULL, INTERNET_OPTION_SETTINGS_CHANGED, NULL, 0);
	InternetSetOption(NULL, INTERNET_OPTION_REFRESH , NULL, 0);
	
	return (bReturn == TRUE);
}

bool Util::GetIEProxy(int& nFlag, cc::Str& sProxy, int& nPort, cc::Str& sByPass, wchar_t * connNameStr)
{
	nFlag = 0;
    INTERNET_PER_CONN_OPTION_LIST conn_options;
    BOOL    bReturn;
    DWORD   dwBufferSize = sizeof(conn_options);
    conn_options.dwSize = dwBufferSize;
    conn_options.pszConnection = connNameStr;//NULL == LAN  

	conn_options.dwOptionCount = 3;
	conn_options.dwOptionError = 0;
	INTERNET_PER_CONN_OPTION option[3];
	memset(&option, 0x0, sizeof(INTERNET_PER_CONN_OPTION) * 3);
    conn_options.pOptions = option;

	//INTERNET_PER_CONN_AUTOCONFIG_URL, INTERNET_PER_CONN_AUTODISCOVERY_FLAGS
	conn_options.pOptions[0].dwOption = INTERNET_PER_CONN_FLAGS;
    conn_options.pOptions[1].dwOption = INTERNET_PER_CONN_PROXY_SERVER;
    conn_options.pOptions[2].dwOption = INTERNET_PER_CONN_PROXY_BYPASS;

	bReturn = InternetQueryOption(NULL, INTERNET_OPTION_PER_CONNECTION_OPTION, &conn_options, &dwBufferSize);

	if(conn_options.pOptions[0].Value.pszValue != NULL)
	{
		//not need free
		if((conn_options.pOptions[0].Value.dwValue & INTERNET_PER_CONN_PROXY_SERVER) == INTERNET_PER_CONN_PROXY_SERVER)
		{
			nFlag |= CC_GETIEPROXY_PROXY_SERVER;
		}
		if((conn_options.pOptions[0].Value.dwValue & INTERNET_PER_CONN_PROXY_BYPASS) == INTERNET_PER_CONN_PROXY_BYPASS)
		{
			nFlag |= CC_GETIEPROXY_PROXY_BYPASS;
		}
	}
	if(conn_options.pOptions[1].Value.pszValue != NULL)
	{
		sProxy.ValueOf(conn_options.pOptions[1].Value.pszValue);
		int nPos = sProxy.IndexOf(_T(":"));
		if(nPos > 0)
		{
			nPort = sProxy.Substring(nPos + 1).ToInt();
			sProxy.Remove(nPos);
		}
		else
		{
			nPort = 80;
		}
		GlobalFree(conn_options.pOptions[1].Value.pszValue);
	}
	if(conn_options.pOptions[2].Value.pszValue != NULL)
	{
		sByPass.ValueOf(conn_options.pOptions[2].Value.pszValue);
		GlobalFree(conn_options.pOptions[2].Value.pszValue);
	}
    return (bReturn == TRUE);
}

//===========================================================cc::win::KeyArr
///////////////////////////////////////////////////////////////////////////////
// constructors/destructor
KeyArr::KeyArr(bool isReturnKeyIfNoFound, bool isIgnorecase)
{
	//_bIsDirty = false;
	//_bIsReturnKey = false;
	//_bIsIgnoreCase = false;
	_Flags = 0;
	_KeyArr = NULL;
	CC_BIT_SET(_Flags, CC_FLAG_KEYARR_RETURNKEY, isReturnKeyIfNoFound);
	CC_BIT_SET(_Flags, CC_FLAG_KEYARR_IGNORECASE, isIgnorecase);
}

KeyArr::~KeyArr()
{
	if(_KeyArr != NULL)
	{
		delete _KeyArr;
		_KeyArr = NULL;
	}
}
// constructors/destructor

void KeyArr::Clear()
{
	if(_KeyArr == NULL)
	{
		//isSort, bIsDESC = false, isDistinct = false, nMinCapacity = 0, nIncrement = 1.3
		_KeyArr = new cc::Map<cc::Str, cc::Str>(true, false, true);
		_KeyArr->SetIsIgnoreCase(CC_BIT_HAS(_Flags, CC_FLAG_KEYARR_IGNORECASE));
	}
	else
	{
		//_bIsDirty = true;
		CC_BIT_ON(_Flags, CC_FLAG_KEYARR_DIRTY);
		_KeyArr->Clear();
	}
}

//clear one part
void KeyArr::Clear(const TCHAR* keyStartWith)
{
	if(_KeyArr == NULL)
	{
		return;
	}

	for(int i = _KeyArr->GetSize() - 1; i >= 0; i--)
	{
		cc::Str skey = _KeyArr->GetKey(i);
		if(skey.StartsWith(keyStartWith, CC_BIT_HAS(_Flags, CC_FLAG_KEYARR_IGNORECASE)))
		{
			//_bIsDirty = true;
			CC_BIT_ON(_Flags, CC_FLAG_KEYARR_DIRTY);
			//_KeyArr->SetValueAt(i, cc::Str());
			_KeyArr->RemoveAt(i);
		}
	}
}

//clear one
bool KeyArr::Remove(const TCHAR* key)
{
	if(_KeyArr != NULL)
	{
		int nIndex = GetIndex(key);
		if(nIndex >= 0)
		{
			_KeyArr->RemoveAt(nIndex);
			return true;
		}
	}
	return false;
}

//clear one
bool KeyArr::Remove(int nIndex)
{
	if(_KeyArr != NULL && nIndex >= 0 && nIndex < _KeyArr->GetSize())
	{
		_KeyArr->RemoveAt(nIndex);
		return true;
	}
	return false;
}

void KeyArr::Add(const KeyArr& keyArr, const TCHAR* keyStartWith, const TCHAR* preKeyTurnTo)
{
	if(keyArr._KeyArr == NULL || &keyArr == this)
	{
		return;
	}
	if(_KeyArr == NULL)
	{
		Clear();
	}
	int nCapacity = keyArr._KeyArr->GetSize();
	int nFrom = keyStartWith == NULL ? 0 : (int)_tcslen(keyStartWith);
	int nTo = preKeyTurnTo == NULL ? 0 : (int)_tcslen(preKeyTurnTo);
	for(int i = 0; i < nCapacity; i++)
	{
		cc::Str key = keyArr._KeyArr->GetKey(i);
		if(keyStartWith == NULL || key.StartsWith(keyStartWith, CC_BIT_HAS(_Flags, CC_FLAG_KEYARR_IGNORECASE)))
		{
			if(preKeyTurnTo != NULL)
			{
				if(nFrom > 0)
				{
					key.Remove(0, nFrom);
				}
				if(nTo > 0)
				{
					key.Insert(0, preKeyTurnTo);
				}
			}
			Set(key, keyArr._KeyArr->GetAt(i));
		}
	}
}
//
//void KeyArr::Add(const cc::Map<cc::Str, cc::Str>& arrIni, const TCHAR* keyStartWith)
//{
//	if(_KeyArr == NULL)
//	{
//		Clear();
//	}
//	int nCapacity = arrIni.GetSize();
//	for(int i = 0; i < nCapacity; i++)
//	{
//		cc::Str key = arrIni.GetKey(i);
//		if(keyStartWith == NULL || key.StartsWith(keyStartWith, CC_BIT_HAS(_Flags, CC_FLAG_KEYARR_IGNORECASE)))
//		{
//			Set(key, arrIni.Get(i));
//		}
//	}
//}

void KeyArr::Add(const cc::Map<cc::Str, cc::Str>& arrIni, const TCHAR* keyStartWith, const TCHAR* preKeyTurnTo)
{
	if(_KeyArr == NULL)
	{
		Clear();
	}
	int nCapacity = arrIni.GetSize();
	int nFrom = keyStartWith == NULL ? 0 : (int)_tcslen(keyStartWith);
	int nTo = preKeyTurnTo == NULL ? 0 : (int)_tcslen(preKeyTurnTo);
	for(int i = 0; i < nCapacity; i++)
	{
		cc::Str key = arrIni.GetKey(i);
		if(keyStartWith == NULL || key.StartsWith(keyStartWith, CC_BIT_HAS(_Flags, CC_FLAG_KEYARR_IGNORECASE)))
		{
			if(preKeyTurnTo != NULL)
			{
				if(nFrom > 0)
				{
					key.Remove(0, nFrom);
				}
				if(nTo > 0)
				{
					key.Insert(0, preKeyTurnTo);
				}
			}
			Set(key, arrIni.GetAt(i));
		}
	}
}

int KeyArr::Add(const TCHAR* keyFile, LangCode::Type fileLangCode, bool turnToTCHAR)
{
	CC_LOGTIME_START();
#if _DEBUG_
	cc::Log::trace(_T("[KeyArr::Add]keyFile:%s, fileLangCode:%d, turnToTCHAR:%d"), keyFile, fileLangCode, turnToTCHAR);
#endif
	if(_KeyArr == NULL)
	{
		Clear();
	}
	//read ini
	//isSort, bIsDESC = false, isDistinct = false, nMinCapacity = 0, nIncrement = 1.3
	//cc::Map<cc::Str, cc::Str> _ArrIni(false, false, true);
	int nSize = cc::Util::readIni(*_KeyArr, keyFile, fileLangCode, turnToTCHAR);
	//if(nSize < 0)
	//{
	//	//not read any
	//	return -1;
	//}
	//Add(_ArrIni);
	//return _ArrIni.GetSize();
	CC_LOGTIME_PRINT();
	return nSize;
}

void KeyArr::Save(const TCHAR* keyFile, LangCode::Type fileLangCode, bool isForce)
{
	if(!CC_BIT_HAS(_Flags, CC_FLAG_KEYARR_DIRTY) && !isForce)
	{
		return;
	}
	if(_KeyArr == NULL)
	{
		Clear();
	}
	//_bIsDirty = false;
	CC_BIT_OFF(_Flags, CC_FLAG_KEYARR_DIRTY);
	cc::Util::writeIni(*_KeyArr, (const TCHAR*)keyFile, fileLangCode);
}

void KeyArr::SaveToString(cc::Str& sTxt)
{
	if(_KeyArr == NULL)
	{
		Clear();
	}
	cc::Util::writeIniToString(*_KeyArr, sTxt);
}

int KeyArr::AddFromString(cc::Str& sTxt, bool turnToTCHAR)
{
	if(_KeyArr == NULL)
	{
		Clear();
	}
	int nSize = cc::Util::treateIni(*_KeyArr, sTxt, turnToTCHAR);
	return nSize;
}

int KeyArr::Add(unsigned int nResID, const TCHAR *resType, LangCode::Type fileLangCode, bool turnToTCHAR)
{
	// if use MAKEINTRESOURCE(nResID),next cannot do warn with %s
	return Add((TCHAR*)cc::Str().Format(_T("#%d"), nResID).GetBuf(), resType, fileLangCode, turnToTCHAR);
}

//if not found res,return -1.used by config
int KeyArr::Add(const TCHAR *resName, const TCHAR *resType, LangCode::Type fileLangCode, bool turnToTCHAR)
{
	if(_KeyArr == NULL)
	{
		Clear();
	}
	//read res
	//isSort, bIsDESC = false, isDistinct = false, nMinCapacity = 0, nIncrement = 1.3
	//cc::Map<cc::Str, cc::Str> _ArrIni(false, false, true);
	cc::Str sTxt(fileLangCode);
	int nSize = cc::Util::readRes(sTxt, resName, resType, fileLangCode);
	if(nSize < 0)
	{
		//not found this res
		return -1;
	}
	nSize = cc::Util::treateIni(*_KeyArr, sTxt, turnToTCHAR);
	//if(_ArrIni.GetSize() > 0)
	//{
	//	Add(_ArrIni);
	//}
	//return _ArrIni.GetSize();
	return nSize;
}

const cc::Map<cc::Str, cc::Str>* KeyArr::GetArr()
{
	if(_KeyArr == NULL)
	{
		Clear();
	}
	return _KeyArr;
}

int KeyArr::GetIndex(const TCHAR* key)
{
	if(_KeyArr != NULL)
	{
		return _KeyArr->GetIndex(key);
	}
	return -1;
}

int KeyArr::GetIndex(const cc::Str& key)
{
	if(_KeyArr != NULL)
	{
		return _KeyArr->GetIndex(key);
	}
	return -1;
}

bool KeyArr::Exist(const TCHAR* key, cc::Str& value)
{
	int nIndex = GetIndex(key);
	if(nIndex < 0)
	{
		return false;
	}
	value = _KeyArr->GetAt(nIndex);
	return true;
}

bool KeyArr::Exist(const cc::Str& key, cc::Str& value)
{
	int nIndex = GetIndex(key);
	if(nIndex < 0)
	{
		return false;
	}
	value = _KeyArr->GetAt(nIndex);
	return true;
}

bool KeyArr::Exist(const TCHAR* key)
{
	return (GetIndex(key) >= 0);
}

bool KeyArr::Exist(const cc::Str& key)
{
	return (GetIndex(key) >= 0);
}

cc::Str KeyArr::GetKey(int index)
{
	if(_KeyArr != NULL && index >= 0 && index < _KeyArr->GetSize())
	{
		return _KeyArr->GetKey(index);
	}
	else
	{
		cc::Str str;
		if(CC_BIT_HAS(_Flags, CC_FLAG_KEYARR_RETURNKEY))
		{
			str.ValueOf(index).Append(_T("[NOFOUND]"));
		}
		return str;
	}
}

cc::Str KeyArr::GetAt(int index)
{
	if(_KeyArr != NULL && index >= 0 && index < _KeyArr->GetSize())
	{
		return _KeyArr->GetAt(index);
	}
	else
	{
		cc::Str str;
		if(CC_BIT_HAS(_Flags, CC_FLAG_KEYARR_RETURNKEY))
		{
			str.ValueOf(index).Append(_T("[NOFOUND]"));
		}
		return str;
	}
}

void KeyArr::SetAt(int index, const TCHAR* value)
{
	if(_KeyArr != NULL && index >= 0 && index < _KeyArr->GetSize())
	{
		_KeyArr->SetValueAt(index, value);
	}
}

void KeyArr::SetAt(int index, const cc::Str& value)
{
	if(_KeyArr != NULL && index >= 0 && index < _KeyArr->GetSize())
	{
		_KeyArr->SetValueAt(index, value);
	}
}

cc::Str KeyArr::Get(const TCHAR* key)
{
	return Get(cc::Str(key));
}

cc::Str KeyArr::Get(const cc::Str& key)
{
	int nIndex = GetIndex(key);
	if(nIndex < 0)
	{
		if(CC_BIT_HAS(_Flags, CC_FLAG_KEYARR_LOGNOTFOUNT))
		{
			cc::Log::debug(_T("KeyArr::Get, key: [%s] is not found."), (const TCHAR*)key.GetBuf());
		}
		if(CC_BIT_HAS(_Flags, CC_FLAG_KEYARR_RETURNKEY))
		{
			return cc::Str(key).Append(_T("[NOFOUND]"));
		}
		else
		{
			return cc::Str();
		}
	}
	return _KeyArr->GetAt(nIndex);
}

cc::Str KeyArr::Get(const TCHAR* key, cc::Str defaultValue)
{
	return Get(cc::Str(key), defaultValue);
}

cc::Str KeyArr::Get(const cc::Str& key, cc::Str defaultValue)
{
	int nIndex = GetIndex(key);
	if(nIndex < 0)
	{
		if(CC_BIT_HAS(_Flags, CC_FLAG_KEYARR_LOGNOTFOUNT))
		{
			cc::Log::debug(_T("KeyArr::Get, key: [%s] is not found."), (const TCHAR*)key.GetBuf());
		}
		return defaultValue;
	}
	return _KeyArr->GetAt(nIndex);
}

DWORD KeyArr::Get(const TCHAR* key, DWORD dwValue)
{
	return Get(cc::Str(key), dwValue);
}

DWORD KeyArr::Get(const cc::Str& key, DWORD dwValue)
{
	int nIndex = GetIndex(key);
	if(nIndex >= 0)
	{
		cc::Str& buf = _KeyArr->GetAt(nIndex);
		if(buf.GetLength() < 1)
		{
			//20090330,if no text then use default value(only for DWORD,bool)
			//dwValue = (DWORD)0;
		}
		else if(buf.IsUnicode())
		{
			dwValue = (DWORD)_wtoi((wchar_t*)buf.GetBuf());
		}
		else
		{
			dwValue = (DWORD)atoi((char*)buf.GetBuf());
		}
	}
	else if(CC_BIT_HAS(_Flags, CC_FLAG_KEYARR_LOGNOTFOUNT))
	{
		cc::Log::debug(_T("KeyArr::Get, key: [%s] is not found."), (const TCHAR*)key.GetBuf());
	}
	return dwValue;
}

bool KeyArr::GetBool(const TCHAR* key, bool bValue)
{
	int nIndex = GetIndex(key);
	if(nIndex >= 0)
	{
		cc::Str& buf = _KeyArr->GetAt(nIndex);
		if(buf.GetLength() < 1)
		{
			//20090330,if no text then use default value(only for DWORD,bool)
		}
		else if(buf.Equals(_T("1")) || buf.Equals(_T("Yes"), true) || buf.Equals(_T("true"), true))
		{
			return true;
		}
		return false;
	}
	else if(CC_BIT_HAS(_Flags, CC_FLAG_KEYARR_LOGNOTFOUNT))
	{
		cc::Log::debug(_T("KeyArr::GetBool, key: [%s] is not found."), key);
	}
	return bValue;
	//return (Get(key, (int)bValue) != 0);
}

bool KeyArr::GetBool(const cc::Str& key, bool bValue)
{
	return GetBool((const TCHAR*)key.GetBuf(), bValue);
	//return (Get(key, (int)bValue) != 0);
}

void KeyArr::Set(const TCHAR* key, const TCHAR* value)
{
	if(_KeyArr == NULL)
	{
		Clear();
	}
	//_bIsDirty = true;
	CC_BIT_ON(_Flags, CC_FLAG_KEYARR_DIRTY);
	int nIndex = GetIndex(key);
	if(nIndex > -1)
	{
		if(value == NULL)
		{
			_KeyArr->RemoveAt(nIndex);
			return;
		}
		_KeyArr->SetValueAt(nIndex, cc::Str(value));
	}
	else
	{
		if(value == NULL)
		{
			return;
		}
		_KeyArr->Add(key, cc::Str(value));
	}
}

void KeyArr::Set(const cc::Str& key, const cc::Str& value)
{
	if(_KeyArr == NULL)
	{
		Clear();
	}
	//_bIsDirty = true;
	CC_BIT_ON(_Flags, CC_FLAG_KEYARR_DIRTY);
	int nIndex = GetIndex(key);
	if(nIndex > -1)
	{
		if(value.IsNULL())
		{
			_KeyArr->RemoveAt(nIndex);
			return;
		}
		_KeyArr->SetValueAt(nIndex, value);
	}
	else
	{
		if(value.IsNULL())
		{
			return;
		}
		_KeyArr->Add(key, value);
	}
}

void KeyArr::Set(const TCHAR* key, const cc::Str& value)
{
	Set(cc::Str(key), value);
}

void KeyArr::Set(const cc::Str& key, const TCHAR* value)
{
	Set(key, cc::Str(value));
}

void KeyArr::Set(const TCHAR* key, DWORD dwValue)
{
	Set(cc::Str(key), dwValue);
}

void KeyArr::Set(const cc::Str& key, DWORD dwValue)
{
	if(_KeyArr == NULL)
	{
		Clear();
	}
	//_bIsDirty = true;
	CC_BIT_ON(_Flags, CC_FLAG_KEYARR_DIRTY);
	int nIndex = GetIndex(key);
	if(nIndex > -1)
	{
		_KeyArr->SetValueAt(nIndex, cc::Str().ValueOf((int)dwValue));
	}
	else
	{
		_KeyArr->Add(key, cc::Str().ValueOf((int)dwValue));
	}
}

void KeyArr::SetBool(const TCHAR* key, bool bValue)
{
	Set(key, bValue ? _T("1") : _T("0"));
}

void KeyArr::SetBool(const cc::Str& key, bool bValue)
{
	Set(key, bValue ? _T("1") : _T("0"));
}

cc::Str KeyArr::GetFormatDefaultValue(cc::Str defaultValue, const TCHAR* key, ...)
{
	int nIndex = GetIndex(key);
	if(nIndex >= 0)
	{
		defaultValue = _KeyArr->GetAt(nIndex);
	}
	else {
		if(CC_BIT_HAS(_Flags, CC_FLAG_KEYARR_LOGNOTFOUNT))
		{
			cc::Log::debug(_T("KeyArr::GetFormatDefaultValue, key: [%s] is not found."), key);
		}
	}
	va_list argList;
	va_start(argList, key);
#ifdef _UNICODE
	defaultValue = cc::Str().FormatV((TCHAR*)defaultValue.GetBuf(), argList);
#else
	defaultValue = cc::Str().FormatV(LangCode::TCHAR, (TCHAR*)defaultValue.GetBuf(), argList);
#endif
	va_end(argList);
	return defaultValue;
}

cc::Str KeyArr::GetFormat(const TCHAR* key, ...)
{
	int nIndex = GetIndex(key);
	if(nIndex < 0)
	{
		if(CC_BIT_HAS(_Flags, CC_FLAG_KEYARR_LOGNOTFOUNT))
		{
			cc::Log::debug(_T("KeyArr::GetFormat, key: [%s] is not found."), key);
		}
		if(CC_BIT_HAS(_Flags, CC_FLAG_KEYARR_RETURNKEY))
		{
			return cc::Str(key).Append(_T("[NOFOUND]"));
		}
		else
		{
			return cc::Str();
		}
	}
	cc::Str defaultValue = _KeyArr->GetAt(nIndex);
	va_list argList;
	va_start(argList, key);
#ifdef _UNICODE
	defaultValue = cc::Str().FormatV((TCHAR*)defaultValue.GetBuf(), argList);
#else
	defaultValue = cc::Str().FormatV(LangCode::TCHAR, (TCHAR*)defaultValue.GetBuf(), argList);
#endif
	va_end(argList);
	return defaultValue;
}

void KeyArr::SetIsDirty(bool isDirty)
{
	//_bIsDirty = isDirty;
	CC_BIT_SET(_Flags, CC_FLAG_KEYARR_DIRTY, isDirty);
}

bool KeyArr::GetIsDirty()
{
	return CC_BIT_HAS(_Flags, CC_FLAG_KEYARR_DIRTY);
}

bool KeyArr::GetIsEmpty()
{
	return (_KeyArr == NULL || _KeyArr->GetSize() < 1);
}

//return cc::Str(key).Append(_T("[NOFOUND]")); if not found
void KeyArr::SetReturnNoFound(bool isReturnKey)
{
	//_bIsReturnKey = isReturnKey;
	CC_BIT_SET(_Flags, CC_FLAG_KEYARR_RETURNKEY, isReturnKey);
}

bool KeyArr::GetIsIgnoreCase()
{
	return CC_BIT_HAS(_Flags, CC_FLAG_KEYARR_IGNORECASE);
}

void KeyArr::SetIsIgnoreCase(bool isIgnoreCase)
{
	//_bIsIgnoreCase = isIgnoreCase;
	CC_BIT_SET(_Flags, CC_FLAG_KEYARR_IGNORECASE, isIgnoreCase);
	if(_KeyArr != NULL)
	{
		return _KeyArr->SetIsIgnoreCase(isIgnoreCase);
	}
}

void KeyArr::SetIsLogNotFound(bool isLogNotFound)
{
	CC_BIT_SET(_Flags, CC_FLAG_KEYARR_LOGNOTFOUNT, isLogNotFound);
}

bool KeyArr::GetIsLogNotFound()
{
	return CC_BIT_HAS(_Flags, CC_FLAG_KEYARR_LOGNOTFOUNT);
}

int KeyArr::GetSize() const
{
	if(_KeyArr != NULL)
	{
		return _KeyArr->GetSize();
	}
	return 0;
}

void KeyArr::Dump(const TCHAR* info)
{
#ifdef _DEBUG_
	cc::Log::trace(_T("%s[KeyArr::Dump(), Size:%d]"), info, _KeyArr == NULL ? 0 : _KeyArr->GetSize());
	if(_KeyArr != NULL)
	{
		int nCapacity = _KeyArr->GetSize();
		for(int i = 0; i < nCapacity; i++)
		{
			cc::Log::trace(_T("key:%s, value:%s"), (TCHAR*)_KeyArr->GetKey(i).GetBuf(), (TCHAR*)_KeyArr->GetAt(i).GetBuf());
		}
	}
#endif
}

//===========================================================cc::Json
Json::Json(LangCode::Type saveLangCode)
{
	_Flags = 0;
	m_JsonArr = NULL;
	m_s = NULL;
	m_pos = 0;
	m_len = 0;
	m_ArrInd = -1;
	m_nValue = 0;
	m_sValue = cc::Str(LangCode::UTF8);
	_savedLang = saveLangCode;
}

Json::~Json()
{
	Clear();
}

void Json::Clear()
{
	if(m_JsonArr != NULL)
	{
		int nCapacity = m_JsonArr->GetSize();
		for(int i = 0; i < nCapacity; i++)
		{
			cc::Json* json = m_JsonArr->GetAt(i);
			if(json != NULL)
			{
				delete json;
			}
		}
		delete m_JsonArr;
		m_JsonArr = NULL;
	}
}

bool Json::IsEmpty()
{
	return (_Flags == 0);
}
bool Json::IsNull()
{
	return (_Flags == JSON_TYPE_NULL);
}
bool Json::IsBool()
{
	return (_Flags == JSON_TYPE_BOOL);
}
bool Json::IsNumber()
{
	return (_Flags == JSON_TYPE_NUMBER);
}
bool Json::IsString()
{
	return (_Flags == JSON_TYPE_STRING);
}
bool Json::IsArray()
{
	return (_Flags == JSON_TYPE_ARRAY);
}
bool Json::IsObject()
{
	return (_Flags == JSON_TYPE_OBJECT);
}

int Json::GetSize()
{
	if(m_JsonArr != NULL)
	{
		return m_JsonArr->GetSize();
	}
	return 0;
}

const cc::Str Json::GetKey(int index)
{
	if(m_JsonArr != NULL)
	{
		return m_JsonArr->GetKey(index);
	}
	return cc::Str(LangCode::UTF8);
}

cc::Str Json::GetString()
{
	return m_sValue;
}

bool Json::GetBool()
{
	return (m_nValue == 1);
}

int Json::GetNumber()
{
	return m_nValue;
}

LangCode::Type Json::GetLangCode()
{
	return _savedLang;
}
//
//cc::Json* Json::operator[](const char* key)
//{
//	return Get(key, fromLangCode);
//}

cc::Json* Json::Get(const char* key, LangCode::Type fromLangCode)
{
	if(m_JsonArr == NULL)
	{
		return NULL;
	}

	cc::Str sKey = cc::Str(_savedLang, key, fromLangCode);
	if(!m_JsonArr->ContainsKey(sKey))
	{
		return NULL;
	}
	return m_JsonArr->Get(sKey);
}

cc::Json* Json::GetDeepSub(const char* key, const char* splitChar, LangCode::Type fromLangCode)
{
	cc::Str sKey = cc::Str(_savedLang, key, fromLangCode);
	Json* json = this;
	while(1)
	{
		cc::Str sOne = sKey.SplitBy(splitChar, true, fromLangCode);
		Json* jsonsub = json->Get(sOne);
		if(jsonsub == NULL)
		{
			break;
		}
		if(sKey.GetLength() <= 1)
		{
			return jsonsub;
		}
		json = jsonsub;
	}
	return NULL;
}

cc::Json* Json::GetAt(int index)
{
	if(m_JsonArr == NULL || index < 0 || index >= m_JsonArr->GetSize())
	{
		return NULL;
	}
	return m_JsonArr->GetAt(index);
}

char Json::GetNextToken()
{
	while(m_pos < m_len && (m_s[m_pos] == ' ' || m_s[m_pos] == '\r' || m_s[m_pos] == '\n' || m_s[m_pos] == '\t'))
	{
		m_pos++;
	}
	if(m_pos == m_len)
	{
		return 0;
	}

	char ch = m_s[m_pos];
	m_pos++;
	return ch;
}

int chk_escape_char(const char* m_s, int len)
{
	for(int j = 0; j < len; j++)
	{
		if(!CC_RANGE(m_s[j], 'a', 'f') && !CC_RANGE(m_s[j], 'A', 'F') && !CC_RANGE(m_s[j], '0', '9'))
		{
			return -1;
		}
	}
	return 0;
}

int Json::Parse_String(cc::Str &out)
{
	unsigned int savepos = m_pos;
	while(m_pos < m_len)
	{
		char buf[7];
		int bufi = 0;
		memset(buf, 0x0, sizeof(buf));
		char ch = m_s[m_pos];
		m_pos++;
		if(ch == '"')
		{
			if(savepos < m_pos - 1)
			{
				out.Append(m_s, savepos, m_pos - savepos - 1, m_FromLang);
				savepos = m_pos;
			}
			return JSON_ERR_NO_ERROR;
		}
		else if(ch == '\\')
		{
			if(savepos < m_pos - 1)
			{
				out.Append(m_s, savepos, m_pos - savepos - 1, m_FromLang);
				savepos = m_pos;
			}
			ch = m_s[m_pos];
			m_pos++;

            if (ch == 'b')
			{
                out += '\b';
            }
			else if (ch == 'f')
			{
                out += '\f';
            }
			else if (ch == 'n')
			{
                out += '\n';
            }
			else if (ch == 'r')
			{
                out += '\r';
            }
			else if (ch == 't')
			{
                out += '\t';
            }
			else if (ch == 'u')
			{
				if(m_len - m_pos < 4)
				{
					return JSON_ERR_U_LEN_ERR;
				}
				if(chk_escape_char(m_s + m_pos, 4) != 0)
				{
					return JSON_ERR_U_RANGE_ERR;
				}

				cc::Str sTmp = cc::Str(LangCode::UTF8, m_s, m_pos, 4, LangCode::UTF8);
				long codepoint = sTmp.ToLong(16);
				if(0xd800 <= codepoint && codepoint <= 0xdfff)
				{
					if(0xdc00 <= codepoint) {
						// a second 16-bit of a surrogate pair appeared
						return JSON_ERR_U_SURROGATE_ERR1;
					}
					if(m_len - m_pos < 10)
					{
						return JSON_ERR_U_NEXT_ERR1;
					}
					if(m_s[m_pos+4] != '\\' || m_s[m_pos+5] != 'u')
					{
						return JSON_ERR_U_NEXT_ERR2;
					}
					if(chk_escape_char(m_s + m_pos + 6, 4) != 0)
					{
						return JSON_ERR_U_NEXT_ERR3;
					}
					cc::Str sTmp2 = cc::Str(LangCode::UTF8, m_s, m_pos + 6, 4, LangCode::UTF8);
					long codepoint2 = sTmp2.ToLong(16);
					if( !(0xdc00 <= codepoint2 && codepoint2 <= 0xdfff) ) {
						return JSON_ERR_U_SURROGATE_ERR2;
					}
					codepoint = ((codepoint - 0xd800) << 10) | ((codepoint2 - 0xdc00) & 0x3ff);
					codepoint += 0x10000;
					m_pos = m_pos + 6;
				}
				else
				{
					m_pos = m_pos + 4;
				}

				if(codepoint < 0x80)
				{
					buf[bufi++] = (const char)(codepoint);
					//out.Append((const char)codepoint);
				}
				else
				{
					if(codepoint < 0x800)
					{
						buf[bufi++] = (const char)(0xc0 | (codepoint >> 6));
					}
					else
					{
						if(codepoint < 0x10000)
						{
							buf[bufi++] = (const char)(0xe0 | (codepoint >> 12));
						}
						else
						{
							buf[bufi++] = (const char)(0xf0 | (codepoint >> 18));
							buf[bufi++] = (const char)(0x80 | ((codepoint >> 12) & 0x3f));
						}
						buf[bufi++] = (const char)(0x80 | ((codepoint >> 6) & 0x3f));
					}
					buf[bufi++] = (const char)(0x80 | (codepoint & 0x3f));
					//here not be fromLangCode!
					out.Append(buf, LangCode::UTF8);
#if _DEBUG_
					cc::Str sBuf = cc::Str(LangCode::TCHAR, buf, LangCode::UTF8);
					cc::Log::debug((TCHAR*)sBuf);
#endif
				}
            }
			else if (ch == '"' || ch == '\\' || ch == '/') {
				out += ch;
				buf[bufi++] = (const char)ch;
				out.Append(buf, m_FromLang);
            }
			else {
                return JSON_ERR_ESCAPE_ERR;
            }
		}
		else
		{
			//buf[bufi++] = (const char)ch;
			//out.Append(buf, LangCode::UTF8);
		}
	}
	if(savepos < m_pos)
	{
		out.Append(m_s, savepos, m_pos - savepos, m_FromLang);
		savepos = m_pos;
	}
	return JSON_ERR_NO_ERROR;
}

int Json::Parse_Number(int &out, cc::Str &out2)
{
	if(m_s[m_pos] == '-' || m_s[m_pos] == '+')
	{
		m_pos++;
	}

	unsigned int savepos = m_pos;
	while(m_pos < m_len && (('0' <= m_s[m_pos] && m_s[m_pos] <= '9') || m_s[m_pos] == 'e' || m_s[m_pos] == 'E' || m_s[m_pos] == '.'))
	{
		m_pos++;
	}
	if(savepos == m_pos)
	{
		return JSON_ERR_NO_NUM;
	}
	out2 = cc::Str(_savedLang, m_s, savepos, m_pos - savepos, m_FromLang);
	out = out2.ToInt();
	return JSON_ERR_NO_ERROR;
}

void Json::Add(const char* key, cc::Json* json, LangCode::Type fromLangCode)
{
	if(m_JsonArr == NULL)
	{
		//isSort, bIsDESC = false, isDistinct = false, nMinCapacity = 0, nIncrement = 1.3
		m_JsonArr = new cc::Map<cc::Str, cc::Json*>(false, false, true);
	}
	cc::Str sKey = cc::Str(_savedLang, key, fromLangCode);
	m_JsonArr->Add(key, json);
}

int Json::Parse_Object(cc::Json &out)
{
	cc::Str sKey = cc::Str(_savedLang);
	while(m_pos < m_len)
	{
		int ch = GetNextToken();

		if(ch == ',')
		{
			continue;
		}
		if(ch == '}')
		{
			return JSON_ERR_NO_ERROR;
		}
		if(ch == ']')
		{
			return JSON_ERR_NO_ERROR;
		}

		if(out._Flags != JSON_TYPE_ARRAY)
		{
			if(ch == '"')
			{
				sKey.Clear();
				int ret = Parse_String(sKey);
				if(ret < 0)
				{
					return ret;
				}
			}
			else
			{
				return JSON_ERR_NO_KEY_START;
			}

			ch = GetNextToken();
			if(ch != ':')
			{
				return JSON_ERR_NO_KEY_SPLIT;
			}

			ch = GetNextToken();
		}
		else
		{
			out.m_ArrInd++;
			sKey.Format("%d", out.m_ArrInd);
		}

		if(ch == '"')
		{
			cc::Json* json = new cc::Json(_savedLang);
			json->_Flags = JSON_TYPE_STRING;
			int ret = Parse_String(json->m_sValue);
			if(ret < 0)
			{
				return ret;
			}
			out.Add(sKey, json);
			continue;
		}
		else if(ch == '-' || (ch >= '0' && ch <= '9'))
		{
			m_pos--;
			cc::Json* json = new cc::Json(_savedLang);
			json->_Flags = JSON_TYPE_NUMBER;
			int ret = Parse_Number(json->m_nValue, json->m_sValue);
			if(ret < 0)
			{
				return ret;
			}
			out.Add(sKey, json);
			continue;
		}
		else if(ch == 't' && m_len - m_pos >= 3
			&& m_s[m_pos] == 'r' && m_s[m_pos+1] == 'u' && m_s[m_pos+2] == 'e')
		{
			m_pos += 3;
			cc::Json* json = new cc::Json(_savedLang);
			json->_Flags = JSON_TYPE_BOOL;
			json->m_nValue = 1;
			out.Add(sKey, json);
			continue;
		}
		else if(ch == 'f' && m_len - m_pos >= 4
			&& m_s[m_pos] == 'a' && m_s[m_pos+1] == 'l' && m_s[m_pos+2] == 's' && m_s[m_pos+3] == 'e')
		{
			m_pos += 4;
			cc::Json* json = new cc::Json(_savedLang);
			json->_Flags = JSON_TYPE_BOOL;
			out.Add(sKey, json);
			continue;
		}
		else if(ch == 'n' && m_len - m_pos >= 3
			&& m_s[m_pos] == 'u' && m_s[m_pos+1] == 'l' && m_s[m_pos+2] == 'l')
		{
			m_pos += 3;
			cc::Json* json = new cc::Json(_savedLang);
			json->_Flags = JSON_TYPE_NULL;
			out.Add(sKey, json);
			continue;
		}

		else if(ch == '{')
		{
			cc::Json* json = new cc::Json(_savedLang);
			json->_Flags = JSON_TYPE_OBJECT;
			int ret = Parse_Object(*json);
			if(ret < 0)
			{
				return ret;
			}
			out.Add(sKey, json);
			continue;
		}

		else if(ch == '[')
		{
			cc::Json* json = new cc::Json(_savedLang);
			json->_Flags = JSON_TYPE_ARRAY;
			json->m_ArrInd = -1;
			int ret = Parse_Object(*json);
			if(ret < 0)
			{
				return ret;
			}
			out.Add(sKey, json);
			continue;
		}
		else
		{
			return JSON_ERR_NO_OBJ;
		}
	}
	return JSON_ERR_NO_ERROR;
}

int Json::Parse(const char* s, LangCode::Type fromLangCode)
{
	CC_LOGTIME_START();
	Clear();

	m_pos = 0;
	m_s = s;
	m_FromLang = fromLangCode;
	if(m_s == NULL || (m_len = cc::Str::Len(m_s)) == 0)
	{
		return JSON_ERR_NO_PARSE;
	}

	int ch = GetNextToken();
	if(ch == '{')
	{
		this->_Flags = JSON_TYPE_OBJECT;
	}
	else if(ch == '[')
	{
		this->_Flags = JSON_TYPE_ARRAY;
	}
	else
	{
		return JSON_ERR_NO_OBJ_START;
	}

	int ret = Parse_Object(*this);
	if(ret < 0)
	{
		return ret;
	}

	//should no any others!
	ch = GetNextToken();
	if(ch != 0)
	{
		return JSON_ERR_NO_NORMAL_END;
	}
	CC_LOGTIME_PRINT();
	return 0;
}

int Json::ToJSon(LangCode::Type toLangCode, cc::Str& sKey, cc::Json& oJson, cc::Str& out, const char* crln, int index, int deep)
{
	cc::Str space = cc::Str(toLangCode);
	if(crln != NULL && deep > 1)
	{
		space.Repeat("  ", deep-1, LangCode::UTF8);
	}
	if(index > 0)
	{
		out.Append(",", LangCode::UTF8);
		if(crln != NULL && deep > 0)
		{
			out.Append(crln, LangCode::UTF8);
		}
	}
	if(crln != NULL && deep > 0)
	{
		out.Append(space);
	}
	if(!sKey.IsNULL())
	{
		out.Append("\"", LangCode::UTF8).Append(sKey).Append("\":", LangCode::UTF8);
	}
	if(oJson.IsObject() || oJson.IsArray())
	{
		if(oJson.IsObject())
		{
			out.Append("{", LangCode::UTF8);
		}
		else if(oJson.IsArray())
		{
			out.Append("[", LangCode::UTF8);
		}
		if(crln != NULL)
		{
			out.Append(crln, LangCode::UTF8);
		}
		int i;
		int nCapacity = oJson.GetSize();
		for(i = 0; i < nCapacity; i++)
		{
			cc::Json* json = oJson.GetAt(i);
			cc::Str sKey2 = cc::Str(toLangCode);
			if(!oJson.IsArray())
			{
				sKey2 = cc::Str(toLangCode, oJson.GetKey(i));
			}
			int ret = json->ToJSon(toLangCode, sKey2, *json, out, crln, i, deep + 1);
			if(ret < 0)
			{
				return ret;
			}
		}
		out.Append(crln, LangCode::UTF8).Append(space);
		if(oJson.IsObject())
		{
			out.Append("}", LangCode::UTF8);
		}
		else if(oJson.IsArray())
		{
			out.Append("]", LangCode::UTF8);
		}
	}
	else
	{
		if(oJson.IsNull())
		{
			out.Append("null", LangCode::UTF8);
		}
		else if(oJson.IsBool())
		{
			if(oJson.GetBool())
			{
				out.Append("true", LangCode::UTF8);
			}
			else
			{
				out.Append("false", LangCode::UTF8);
			}
		}
		else if(oJson.IsString())
		{
			out.Append("\"", LangCode::UTF8);
			cc::Str s = cc::Str(LangCode::UTF8, oJson.GetString());
			s.Replace("\\", "\\\\", LangCode::UTF8).Replace("\n", "\\n", LangCode::UTF8).Replace("\r", "\\r", LangCode::UTF8).Replace("\t", "\\t", LangCode::UTF8).Replace("\"", "\\\"", LangCode::UTF8);
			out.Append(s);
			out.Append("\"", LangCode::UTF8);
		}
		else if(oJson.IsNumber())
		{
			out.Append(oJson.GetString());
		}
	}
	return 0;
}

cc::Str Json::ToJSon(LangCode::Type toLangCode, const char* crln)
{
	CC_LOGTIME_START();

	cc::Str sKey = cc::Str(toLangCode);
	cc::Str out = cc::Str(toLangCode);
	int ret = ToJSon(toLangCode, sKey, *this, out, crln, 0, 0);
	if(ret < 0)
	{
		return cc::Str();
	}

	CC_LOGTIME_PRINT();
	return out;
}

//===========================================================cc::Config
TCHAR ConfigConfig::tcDefaultLang[CC_CONFIG_BUFLEN + 1] = _T("en-US");
TCHAR ConfigConfig::tcRegistyConfigKey[CC_CONFIG_BUFLEN + 1] = _T("CONFIG_STRING");
char ConfigConfig::tcRegistyConfigPWD[CC_CONFIG_BUFLEN + 1] = "0okE73bUf4gUvM}We&:x";
UINT ConfigConfig::nSaveRegCryptLoopCount = 4;
///////////////////////////////////////////////////////////////////////////////
// constructors/destructor
Config::Config()
{
	//_bIsDebug = false;
	//_bIsLoding = false;
	//_bIsAutoSave = true;
	_Flags = 0;
	CC_BIT_ON(_Flags, CC_FLAG_CONFIG_AUTOSAVE);
	_tcIniFile = NULL;
	_tcIniReg = NULL;
	_tcLangPath = NULL;
	_Ini = NULL;
	_IniLangCode = LangCode::TCHAR;
	_Lang = NULL;
}

Config::~Config()
{
	if(CC_BIT_HAS(_Flags, CC_FLAG_CONFIG_AUTOSAVE))
	{
		SaveIni();
	}
	if(_tcIniFile != NULL)
	{
		free(_tcIniFile);
		_tcIniFile = NULL;
	}
	if(_tcIniReg != NULL)
	{
		free(_tcIniReg);
		_tcIniReg = NULL;
	}
	if(_tcLangPath != NULL)
	{
		free(_tcLangPath);
		_tcLangPath = NULL;
	}
	if(_Ini != NULL)
	{
		delete _Ini;
		_Ini = NULL;
	}
	if(_Lang != NULL)
	{
		delete _Lang;
		_Lang = NULL;
	}
}
// constructors/destructor

KeyArr* Config::GetIni()
{
	if(_Ini == NULL)
	{
		LoadIni();
	}
	return _Ini;
}

KeyArr* Config::GetLang()
{
	if(_Lang == NULL)
	{
		//en-US is English (United States),if not found any others,use this
		LoadLang(ConfigConfig::tcDefaultLang, true);
	}
	return _Lang;
}

const TCHAR* Config::GetIniPath()
{
	if(_tcIniFile == NULL)
	{
		_tcIniFile = (TCHAR*)malloc((_MAX_PATH + 1) * sizeof(TCHAR));
		//GetModuleFileName(NULL, _tcIniFile, _MAX_PATH);
		//cc::Str path = cc::Util::getStartupPathForSave(false);
		//cc::Buf::Cpy(_tcIniFile, _MAX_PATH, path);
		CC_getLogPathWithStartupFilename(_tcIniFile, _MAX_PATH + 1);
		//_tcscat(_tcIniFile, _T(".ini"));
		cc::Buf::Cat(_tcIniFile, _MAX_PATH + 1, _T(".ini"));
	}
	return _tcIniFile;
}

const TCHAR* Config::GetLangPath()
{
	if(_tcLangPath == NULL)
	{
		CC_StrCopy(&_tcLangPath, _T("Lang\\"));
	}
	return _tcLangPath;
}

bool Config::GetIsDebug()
{
	if(_Ini == NULL)
	{
		//care for dead loop
		LoadIni();
	}
	return CC_BIT_HAS(_Flags, CC_FLAG_CONFIG_DEBUG);
}

//this is not set to ini
void Config::SetIsDebug(bool isDebug)
{
	if(_Ini == NULL)
	{
		LoadIni();
	}
	CC_BIT_SET(_Flags, CC_FLAG_CONFIG_DEBUG, isDebug);
}

bool Config::GetIsDesign()
{
	if(_Ini == NULL)
	{
		//care for dead loop
		LoadIni();
	}
	return CC_BIT_HAS(_Flags, CC_FLAG_CONFIG_DESIGN);
}

//this is not set to ini
void Config::SetIsDesign(bool isDesign)
{
	if(_Ini == NULL)
	{
		LoadIni();
	}
	CC_BIT_SET(_Flags, CC_FLAG_CONFIG_DESIGN, isDesign);
}

bool Config::LoadIni(const TCHAR* iniFile)
{
	//invoid of dead loop
	if(CC_BIT_HAS(_Flags, CC_FLAG_CONFIG_LODING))
	{
		return false;
	}
	//_bIsLoding = true;
	CC_BIT_ON(_Flags, CC_FLAG_CONFIG_LODING);
	//if _tcIniFile = NULL,then be set to default at GetIniPath
	CC_StrCopy(&_tcIniFile, iniFile);
	GetIniPath();
	if(_Ini == NULL)
	{
		_Ini = new KeyArr();
		_Ini->SetIsIgnoreCase(true);
	}
	else
	{
		_Ini->Clear();
	}
	if(CC_BIT_HAS(_Flags, CC_FLAG_CONFIG_SAVEREG))
	{
#ifdef _DEBUG_
		CC_TRACE((_T("Config::LoadIni(), get from registy, _tcIniReg:%s"), _tcIniReg));
#endif
		CC_ASSERT(_tcIniReg != NULL);
		if(_tcIniReg != NULL)
		{
			HKEY hKey = NULL;
			long regret = ::RegOpenKeyEx((HKEY)HKEY_LOCAL_MACHINE, _tcIniReg, 0, KEY_QUERY_VALUE, &hKey);
			if(regret == ERROR_SUCCESS)
			{
				DWORD dwType = REG_NONE;
				DWORD lpcbData = 0;
				//get length
				regret = ::RegQueryValueEx(hKey, ConfigConfig::tcRegistyConfigKey, NULL, &dwType, NULL, &lpcbData);
				if(regret == ERROR_SUCCESS)
				{
					CC_ASSERT(dwType == REG_BINARY);
					TCHAR* lpstrValue = (TCHAR*)malloc(lpcbData + 2);
					memset((void*)lpstrValue, 0, lpcbData + 2);
					regret = ::RegQueryValueEx(hKey, ConfigConfig::tcRegistyConfigKey, NULL, &dwType, (BYTE*)(LPCTSTR)lpstrValue, &lpcbData);
					if(regret == ERROR_SUCCESS)
					{
						cc::Util::teaDeCrypt((char*)lpstrValue, (int)lpcbData, ConfigConfig::tcRegistyConfigPWD, (int)strlen(ConfigConfig::tcRegistyConfigPWD), ConfigConfig::nSaveRegCryptLoopCount);
						cc::Str sRetu;
						sRetu.TakeoverPointer(&lpstrValue);
						_Ini->AddFromString(sRetu, true);
					}
					if(lpstrValue != NULL)
					{
						free(lpstrValue);
					}
				}
				::RegCloseKey(hKey);
			}
		}
	}
	else
	{
		//read ini
		//isSort, bIsDESC = false, isDistinct = false, nMinCapacity = 0, nIncrement = 1.3
		_Ini->Add(_tcIniFile, _IniLangCode, true);
	}
	//_bIsDebug = (_Ini->Get(CC_CONFIG_DEBUGMODE, 0) == 1);
	CC_BIT_SET(_Flags, CC_FLAG_CONFIG_DEBUG, (_Ini->Get(CC_CONFIG_DEBUGMODE, 0) == 1));
	//_bIsLoding = false;
	CC_BIT_OFF(_Flags, CC_FLAG_CONFIG_LODING);
	return true;
}

bool Config::LoadLang(unsigned int nDefaultLangResID, bool isLoadDefaultAlways, const TCHAR *resLangType, const TCHAR *langPath, bool isNotLoadFile)
{
	return LoadLang((TCHAR*)cc::Str().Format(_T("#%d"), nDefaultLangResID).GetBuf(), isLoadDefaultAlways, resLangType, langPath, isNotLoadFile);
}

//LoadIni in LoadLang
bool Config::LoadLang(const TCHAR *tcDefaultLangResName, bool isLoadDefaultAlways, const TCHAR *resLangType, const TCHAR *langPath, bool isNotLoadFile)
{
	bool bRet = PreDoLoadLang(tcDefaultLangResName, isLoadDefaultAlways, resLangType, langPath, isNotLoadFile);
#if defined(_DEBUG_) || defined(_DESIGN)
	if(_Ini != NULL && _Lang != NULL)
	{
		cc::Str langName = _Ini->Get(CC_CONFIG_LANGFILE);
		if(langName.GetLength() > 0)
		{
			langName.Append(_T("."));
			//load lang from ini for design
			CC_TRACE((_T("[Config::LoadLang]load language from ini for design[%s -> %s]."), (TCHAR*)langName.GetBuf(), CC_CONFIG_LANGHEADDOT));
			_Lang->Add(*_Ini, (TCHAR*)langName.GetBuf(), CC_CONFIG_LANGHEADDOT);
			_Lang->Add(*_Ini, CC_CONFIG_DESIGNHEADDOT, CC_CONFIG_LANGHEADDOT);
		}
	}
#endif
	//set LinkLang=1
	//then if "Lang.key1=>>key2" will link key1 to key2(copy key2's value to key1)
	if(_Lang->Get(_T("Lang.CopyLangFromLang"), 0) == 1)
	{
		for(int n = _Lang->GetSize() - 1; n >= 0; n--)
		{
			cc::Str val = _Lang->GetAt(n);
			if(val.StartsWith(_T(">>")))
			{
				val = val.Substring(2).Insert(0, _T("Lang."));
				int n2 = _Lang->GetIndex(val);
				if(n2 >= 0)
				{
					val = _Lang->GetAt(n2);
					_Lang->SetAt(n, val);
				}
			}
		}
	}
	return bRet;
}

bool Config::PreDoLoadLang(const TCHAR *tcDefaultLangResName, bool isLoadDefaultAlways, const TCHAR *resLangType, const TCHAR *langPath, bool isNotLoadFile)
{
	CC_LOGTIME_START();
	//for need,read ini first
	if(_Ini == NULL)
	{
		LoadIni();
	}
	//invoid of dead loop
	if(CC_BIT_HAS(_Flags, CC_FLAG_CONFIG_LODING))
	{
		return false;
	}
	CC_BIT_ON(_Flags, CC_FLAG_CONFIG_LODING);
	try
	{
		if(_Lang == NULL)
		{
			_Lang = new KeyArr();
			//if the key of Lang is not exist,then return key[NOFOUND]
			_Lang->SetReturnNoFound(true);
			_Lang->SetIsIgnoreCase(true);
			_Lang->SetIsLogNotFound(true);
		}
		else
		{
			_Lang->Clear();
		}

		int nSize;
		bool isLoaded = false;
		cc::Str langName;
		if(tcDefaultLangResName == NULL)
		{
			tcDefaultLangResName = ConfigConfig::tcDefaultLang;
		}
		if(resLangType == NULL)
		{
			resLangType = CC_CONFIG_LANGTYPE;
		}
		if(langPath != NULL)
		{
			CC_StrCopy(&_tcLangPath, langPath);
		}
		GetLangPath();
		if(isLoadDefaultAlways)
		{
			//load default res
			_Lang->Add(tcDefaultLangResName, resLangType, LangCode::UTF8, true);
		}
		if(_Ini->Exist(CC_CONFIG_LANGFILE))
		{
			//if defined lang,load it
			langName = _Ini->Get(CC_CONFIG_LANGFILE);
			if(langName.Equals(CC_CONFIG_LANGDEFAULT))
			{
				if(!isLoadDefaultAlways)
				{
					//load default res
					nSize = _Lang->Add(tcDefaultLangResName, resLangType, LangCode::UTF8, true);
#ifdef _DEBUG_
				CC_TRACE((_T("[Config::LoadLang]load default language from resource[name:%s, type:%s, count:%d]."), tcDefaultLangResName, resLangType, nSize));
#endif
				}
				isLoaded = true;
			}
			else
			{
#ifdef _DEBUG_
				CC_TRACE((_T("[Config::LoadLang]load configed language[name:%s]."), (TCHAR*)langName.GetBuf()));
#endif
				isLoaded = DoLoadLang(langName, resLangType, isNotLoadFile, false);
			}
		}
		if(isLoaded)
		{
			CC_BIT_OFF(_Flags, CC_FLAG_CONFIG_LODING);
			//_bIsLoding = false;
			return true;
		}
		//search lang from CurrentLocaleName
		cc::Str langName2 = cc::Util::GetCurrentLocaleName(true);
		if(!langName2.Equals(langName, true))
		{
#ifdef _DEBUG_
			CC_TRACE((_T("[Config::LoadLang]search CurrentLocaleName language[name:%s]."), (TCHAR*)langName2.GetBuf()));
#endif
			isLoaded = DoLoadLang(langName2, resLangType, isNotLoadFile, true);
		}
		if(!isLoaded)
		{
			//use similar lang
			cc::Str langName3 = cc::Str(cc::Util::GetSimilarLocalName());
			if(!langName3.Equals(langName, true) && !langName3.Equals(langName2, true))
			{
#ifdef _DEBUG_
				CC_TRACE((_T("[Config::LoadLang]search SimilarLocalName language[name:%s]."), (TCHAR*)langName3.GetBuf()));
#endif
				isLoaded = DoLoadLang(langName3, resLangType, isNotLoadFile, true);
			}
		}
		//not found any,then default
		if(!isLoadDefaultAlways && !isLoaded)
		{
			//load default res
			nSize = _Lang->Add(tcDefaultLangResName, resLangType, LangCode::UTF8, true);
			//save status for next not seach file again
			_Ini->Set(CC_CONFIG_LANGFILE, CC_CONFIG_LANGDEFAULT);
#ifdef _DEBUG_
			CC_TRACE((_T("[Config::LoadLang]not found any, load default language from resource[name:%s, file:%s, count:%d]."), tcDefaultLangResName, resLangType, nSize));
#endif
		}
	}
	catch(...)
	{
		CC_LOGSTEP();
		cc::Log::fatal(_T("[Config::LoadLang]Error!"));
	}
	//_bIsLoding = false;
	CC_BIT_OFF(_Flags, CC_FLAG_CONFIG_LODING);
	CC_LOGTIME_PRINT();
	return true;
}

bool Config::DoLoadLang(cc::Str langName, const TCHAR *resLangType, bool isNotLoadFile, bool isSaveStatusToIni)
{
	CC_ASSERT(resLangType != NULL);

	int nSize;
	bool isLoaded = false;
	if(langName.GetLength() < 1)
	{
		return false;
	}
	if(!isNotLoadFile)
	{
		//search file
		cc::Str langFile;
		if(!langName.StartsWith(_T("\\")) && !langName.StartsWith(_T("/")) && langName.IndexOf(_T(":")) < 0)
		{
			langFile = cc::Util::getStartupPath(true);
			if(langName.IndexOf(_T("\\")) < 0 && langName.IndexOf(_T("/")) < 0)
			{
				langFile.Append(_tcLangPath);
			}
		}
		langFile.Append(langName);
		if(!langName.EndsWith(CC_CONFIG_LANGEXT, true))
		{
			langFile.Append(CC_CONFIG_LANGEXT);
		}
#ifdef _DEBUG_
		if(!cc::Util::fExist((TCHAR*)langFile.GetBuf()))
		{
			langFile = cc::Util::getStartupPath(true);
			langFile.Append(_T("..\\")).Append(_tcLangPath).Append(langName);
			if(!langName.EndsWith(CC_CONFIG_LANGEXT, true))
			{
				langFile.Append(CC_CONFIG_LANGEXT);
			}
		}
#endif
#ifdef _DEBUG_
		CC_TRACE((_T("[Config::DoLoadLang]try to load file[name:%s, file:%s]."), (TCHAR*)langName.GetBuf(), (TCHAR*)langFile.GetBuf()));
#endif
		if(cc::Util::fExist((TCHAR*)langFile.GetBuf()))
		{
			nSize = _Lang->Add((TCHAR*)langFile.GetBuf(), LangCode::UTF8, true);
			isLoaded = (nSize >= 0);
#ifdef _DEBUG_
			CC_TRACE((_T("[Config::DoLoadLang]load language from file[name:%s, file:%s, count:%d]."), (TCHAR*)langName.GetBuf(), (TCHAR*)langFile.GetBuf(), nSize));
#endif
		}
	}
	if(!isLoaded)
	{
		//search res
		nSize = _Lang->Add((TCHAR*)langName.GetBuf(), resLangType, LangCode::UTF8, true);
		isLoaded = (nSize >= 0);
#ifdef _DEBUG_
		CC_TRACE((_T("[Config::DoLoadLang]load language from resource[name:%s, file:%s, count:%d]."), (TCHAR*)langName.GetBuf(), resLangType, nSize));
#endif
	}
	if(isSaveStatusToIni && isLoaded)
	{
		//save status for next not seach file again
		_Ini->Set(CC_CONFIG_LANGFILE, (TCHAR*)langName.GetBuf());
	}
	return isLoaded;
}

bool Config::SaveIni()
{
	if(_Ini == NULL || (_tcIniFile == NULL && _tcIniReg == NULL) || !_Ini->GetIsDirty())
	{
		return false;
	}
	if(CC_BIT_HAS(_Flags, CC_FLAG_CONFIG_SAVEREG))
	{
#ifdef _DEBUG_
	CC_TRACE((_T("Config::SaveIni(), save to registy, _tcIniReg:%s"), _tcIniReg));
#endif
		CC_ASSERT(_tcIniReg != NULL);
		if(_tcIniReg != NULL)
		{
			HKEY hKey = NULL;
			long regret = ::RegOpenKeyEx((HKEY)HKEY_LOCAL_MACHINE, _tcIniReg, 0, KEY_ALL_ACCESS, &hKey);
			if(regret != ERROR_SUCCESS)
			{
				regret = ::RegCreateKeyEx((HKEY)HKEY_LOCAL_MACHINE, _tcIniReg, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL);
			}
			if(regret == ERROR_SUCCESS)
			{
				cc::Str sRetu;
				//save origal
				_Ini->SaveToString(sRetu);
				DWORD lpcbData = (DWORD)(sRetu.GetLength() * sizeof(TCHAR));
				//after GetBuffer, not need GetBufferSetLength() bucause not change sring length here
				cc::Util::teaEnCrypt((char*)sRetu.GetBuffer(), (int)lpcbData, ConfigConfig::tcRegistyConfigPWD, (int)strlen(ConfigConfig::tcRegistyConfigPWD), ConfigConfig::nSaveRegCryptLoopCount);
				regret = ::RegSetValueEx(hKey, ConfigConfig::tcRegistyConfigKey, NULL, REG_BINARY, (const BYTE*)sRetu.GetBuf(), lpcbData);
				::RegCloseKey(hKey);
			}
		}
	}
	else
	{
		CC_ASSERT(_tcIniFile != NULL);
		if(_tcIniFile != NULL)
		{
			//treate for cc::Log
			if(cc::Log::IsConfiged())
			{
				//delete origal Log's ini
				_Ini->Clear(_T("Log."));
				//save to file
				cc::Util::writeIni(*(_Ini->GetArr()), _tcIniFile, _IniLangCode);
				//add log config
				cc::Log::SaveConfig(_tcIniFile);
			}
			else
			{
				//save origal
				_Ini->Save(_tcIniFile, _IniLangCode, true);
			}
		}
	}
	return true;
}
void Config::SetIniLangCode(LangCode::Type iniLangCode)
{
	_IniLangCode = iniLangCode;
}
void Config::SetIsAutoSave(bool value, bool isToReg, const TCHAR *tcRegKey)
{
	//_bIsAutoSave = value;
	CC_BIT_SET(_Flags, CC_FLAG_CONFIG_AUTOSAVE, value);
	CC_BIT_SET(_Flags, CC_FLAG_CONFIG_SAVEREG, isToReg);
	CC_StrCopy(&_tcIniReg, tcRegKey);
}

void Config::Dump(const TCHAR* info)
{
#ifdef _DEBUG_
	CC_TRACE((_T("%s[Config::Dump(), Ini Size:%d, Lang Size:%d]"), info, _Ini == NULL ? 0 : _Ini->GetSize(), _Lang == NULL ? 0 : _Lang->GetSize()));
	if(_Ini != NULL)
	{
		_Ini->Dump();
	}
	if(_Lang != NULL)
	{
		_Lang->Dump();
	}
#endif
}
///////////////////////////////////////////////////////////////////////////////

//need one and only once
Theme _GLOBAL_ONLY_ONCE_THEME;

//===========================================================cc::Theme
DWORD cc::Theme::_Flags = 0;
HMODULE cc::Theme::m_hThemeDll = NULL;
///////////////////////////////////////////////////////////////////////////////
// constructors/destructor
Theme::Theme()
{
	//m_hThemeDll = NULL;
}

Theme::~Theme()
{
	if(m_hThemeDll != NULL)
	{
		FreeLibrary(m_hThemeDll);
		m_hThemeDll = NULL;
	}
}
// constructors/destructor


void* Theme::GetProc(LPCSTR szProc)
{
	if(!CC_BIT_HAS(_Flags, CC_FLAG_THEME_Loaded))
	{
		CC_BIT_ON(_Flags, CC_FLAG_THEME_Loaded);
		m_hThemeDll = LoadLibrary(_T("UxTheme.dll"));
	}
	if(m_hThemeDll != NULL)
	{
		return GetProcAddress(m_hThemeDll, szProc);
	}
	return NULL;
}

//HTHEME
HANDLE Theme::OpenThemeData(HWND hwnd, LPCWSTR pszClassList)
{
	static PFNOPENTHEMEDATA pfnOpenThemeData = NULL;
	if(!CC_BIT_HAS(_Flags, CC_FLAG_THEME_OpenThemeData))
	{
		CC_BIT_ON(_Flags, CC_FLAG_THEME_OpenThemeData);
		pfnOpenThemeData = (PFNOPENTHEMEDATA)GetProc("OpenThemeData");
	}
	if(pfnOpenThemeData)
	{
		return (*pfnOpenThemeData)(hwnd, pszClassList);
	}
	return NULL;
}

HRESULT Theme::CloseThemeData(/*HTHEME*/HANDLE hTheme)
{
	static PFNCLOSETHEMEDATA pfnCloseThemeData = NULL;
	if(!CC_BIT_HAS(_Flags, CC_FLAG_THEME_OpenThemeData))
	{
		CC_BIT_ON(_Flags, CC_FLAG_THEME_OpenThemeData);
		pfnCloseThemeData = (PFNCLOSETHEMEDATA)GetProc("CloseThemeData");
	}
	if(pfnCloseThemeData)
	{
		return (*pfnCloseThemeData)(hTheme);
	}
	return NULL;
}

BOOL Theme::IsThemeActive(bool isRefresh)
{
	static PFNISTHEMEACTIVE pfn = NULL;
	static BOOL bRet = -1;
	if(isRefresh || bRet == -1)
	{
		if(bRet == -1)
		{
			bRet = FALSE;
			pfn = (PFNISTHEMEACTIVE)GetProc("IsThemeActive");
		}
		if(pfn)
		{
			bRet = (*pfn)();
		}
	}
	return bRet;
}

BOOL Theme::IsAppThemed(bool isRefresh)
{
	static PFNISAPPTHEMED pfnIsAppThemed = NULL;
	static BOOL bRet = -1;
	if(isRefresh || bRet == -1)
	{
		if(bRet == -1)
		{
			bRet = FALSE;
			pfnIsAppThemed = (PFNISAPPTHEMED)GetProc("IsAppThemed");
		}
		if(pfnIsAppThemed)
		{
			bRet = (*pfnIsAppThemed)();
		}
	}
	return bRet;
}

HRESULT Theme::DrawThemeBackground(/*HTHEME*/HANDLE hTheme, HDC hdc, int iPartId, int iStateId, const RECT *pRect, const RECT *pClipRect)
{
	static PFNDRAWTHEMEBACKGROUND pfnDrawThemeBackground = NULL;
	if(!CC_BIT_HAS(_Flags, CC_FLAG_THEME_DrawThemeBackground))
	{
		CC_BIT_ON(_Flags, CC_FLAG_THEME_DrawThemeBackground);
		pfnDrawThemeBackground = (PFNDRAWTHEMEBACKGROUND)GetProc("DrawThemeBackground");
	}
	if(pfnDrawThemeBackground)
	{
		return (*pfnDrawThemeBackground)(hTheme, hdc, iPartId, iStateId, pRect, pClipRect);
	}
	return NULL;
}

HRESULT Theme::DrawThemeEdge(/*HTHEME*/HANDLE hTheme, HDC hdc, int iPartId, int iStateId, 
									   const RECT *pDestRect, UINT uEdge, UINT uFlags, RECT *pContentRect)
{
	static PFNDRAWTHEMEEDGE pfn = NULL;
	if(!CC_BIT_HAS(_Flags, CC_FLAG_THEME_DrawThemeEdge))
	{
		CC_BIT_ON(_Flags, CC_FLAG_THEME_DrawThemeEdge);
		pfn = (PFNDRAWTHEMEEDGE)GetProc("DrawThemeEdge");
	}
	if(pfn)
	{
		return (*pfn)(hTheme, hdc, iPartId, iStateId, pDestRect, uEdge, uFlags, pContentRect);
	}
	return NULL;
}

HRESULT Theme::EnableThemeDialogTexture(HWND hwnd, DWORD dwFlags)
{
	static PFNENABLETHEMEDIALOGTEXTURE pfn = NULL;
	if(!CC_BIT_HAS(_Flags, CC_FLAG_THEME_EnableThemeDialogTexture))
	{
		CC_BIT_ON(_Flags, CC_FLAG_THEME_EnableThemeDialogTexture);
		pfn = (PFNENABLETHEMEDIALOGTEXTURE)GetProc("EnableThemeDialogTexture");
	}
	if(pfn)
	{
		return (*pfn)(hwnd, dwFlags);
	}
	return NULL;
}
