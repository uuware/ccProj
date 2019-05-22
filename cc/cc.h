/* cc.h
#define A(x) T_##x
#define B(x) #@x
#define C(x) #x
if x=1 then:
A(1)------T_1
B(1)------'1'
C(1)------"1"

   Global setting
   Author: Shu.KK
   created:2006-01-01
*/
#if _MSC_VER > 1000
#pragma once
#endif //#if _MSC_VER > 1000

#ifndef CC_GLOBAL_H_
#define CC_GLOBAL_H_

/*
  cc will use UNICODE,the file "cc.h" will be included after system head file,
  so,below defined sould be writed in each head file before "#include <system head file>"
*/
#if defined(_UNICODE) && !defined(UNICODE)
#define UNICODE 1
#endif
#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE 1
#endif

//#if defined(_DEBUG_)
//#define DEBUG 1
//#endif
//#if defined(DEBUG) && !defined(_DEBUG_)
//#define _DEBUG_ 1
//#endif

#if defined(_CC_CHECK_MEM_LEAK)
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifdef _DEBUG_
   #ifndef DBG_NEW
      #define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
      #define new DBG_NEW
   #endif
#endif  // _DEBUG_
#endif

#if defined __GNUC__ && __GNUC__ >= 3
#ifndef va_list
#define __need___va_list
#include <stdarg.h>
#define va_list __builtin_va_list
#endif

/* Some versions of MinGW are missing _vscprintf's declaration, although they
 * still provide the symbol in the import library. */
int _vscprintf(const char *format, va_list argptr);
int _vscwprintf(const wchar_t *format, va_list argptr);
#endif

#define _WC2(x)      L ## x
#define _WC(x)      _WC2( x )

//define JOIN
#define CC_JOIN2(X, Y)	X##Y
#define CC_JOIN3(X, Y, Z)	X##Y##Z
#define CC_JOIN4(X, Y, Z, A)	X##Y##Z##A
#define CC_JOIN5(X, Y, Z, A, B)	X##Y##Z##A##B
#define CC_JOIN6(X, Y, Z, A, B, C)	X##Y##Z##A##B##C

#define CC_MAX(a, b) ((a) >= (b) ? (a) : (b))
#define CC_MIN(a, b) ((a) <= (b) ? (a) : (b))
#define CC_ABS(a) ((a) >= 0 ? (a) : -(a))
#define CC_RANGE(x, lower, upper) (((x) >= (lower) && (x) <= (upper)))
#define CC_RANGE_LESS(x, lower, upper) (((x) > (lower) && (x) < (upper)))

//CC_BIT_IS(VAR, BIT1|BIT2|BIT3): return true if has BIT1 and BIT2 and BIT3
#define CC_BIT_IS(VAR, BIT)				( ((VAR) & (BIT)) == (BIT) )
//CC_BIT_HAS(VAR, BIT1|BIT2|BIT3): return true if has any BIT1 or BIT2 or BIT3
#define CC_BIT_HAS(VAR, BIT)			( ((VAR) & (BIT)) != 0 )
#define CC_BIT_SET(VAR, BIT, VALUE)		if(VALUE){(VAR) |= (BIT);} else {(VAR) &= ~(BIT);}
#define CC_BIT_ON(VAR, BIT)				( (VAR) |= (BIT) )
#define CC_BIT_OFF(VAR, BIT)			( (VAR) &= ~(BIT) )

//define DEBUG
#if defined(_DEBUG_)
#define CC_IFDEBUG_RUN(source) {source;}
#else
#define CC_IFDEBUG_RUN(source)
#endif

namespace cc
{

//========================================
//pagecode
typedef struct tagLangCode
{
	enum Type
	{
		SYSTEM = 0,
		UTF8 = 65001,//while file BOM:EF BB BF
		UTF7 = 65000,
		UNICODE16 = 1200,//little-endian,also is UTF-16LE,while file BOM:FF FE
		UNICODE_BIGENDIAN = 1201,//big-endian,also is UTF-16BE,while file BOM:FE FF
		CN_GB2312 = 936,
		CN_BIG5 = 950,
		KO = 936,
		JP_SJIS = 932,
		JP_EUC = 51932,
		JP_JIS = 936,
		#if defined (_UNICODE)
		TCHAR = 1200,
		#else
		TCHAR = 0,
		#endif
	};
}LangCode;

} //namespace cc

#endif //#ifndef CC_GLOBAL_H_

/*
On Windows MultiByteToWideChar and WideCharToMultiByte
support many encodings other than mentioned directly in MSDN.

LANGC_t langs[] = {
{"asmo-708",708},
{"dos-720",720},
{"iso-8859-6",28596},
{"x-mac-arabic",10004},
{"windows-1256",1256},
{"ibm775",775},
{"iso-8859-4",28594},
{"windows-1257",1257},
{"ibm852",852},
{"iso-8859-2",28592},
{"x-mac-ce",10029},
{"windows-1250",1250},
{"euc-cn",51936},
{"gb2312",936},
{"hz-gb-2312",52936},
{"x-mac-chinesesimp",10008},
{"big5",950},
{"x-chinese-cns",20000},
{"x-chinese-eten",20002},
{"x-mac-chinesetrad",10002},
{"cp866",866},
{"iso-8859-5",28595},
{"koi8-r",20866},
{"koi8-u",21866},
{"x-mac-cyrillic",10007},
{"windows-1251",1251},
{"x-europa",29001},
{"x-ia5-german",20106},
{"ibm737",737},
{"iso-8859-7",28597},
{"x-mac-greek",10006},
{"windows-1253",1253},
{"ibm869",869},
{"dos-862",862},
{"iso-8859-8-i",38598},
{"iso-8859-8",28598},
{"x-mac-hebrew",10005},
{"windows-1255",1255},
{"x-ebcdic-arabic",20420},
{"x-ebcdic-cyrillicrussian",20880},
{"x-ebcdic-cyrillicserbianbulgarian",21025},
{"x-ebcdic-denmarknorway",20277},
{"x-ebcdic-denmarknorway-euro",1142},
{"x-ebcdic-finlandsweden",20278},
{"x-ebcdic-finlandsweden-euro",1143},
{"x-ebcdic-finlandsweden-euro",1143},
{"x-ebcdic-france-euro",1147},
{"x-ebcdic-germany",20273},
{"x-ebcdic-germany-euro",1141},
{"x-ebcdic-greekmodern",875},
{"x-ebcdic-greek",20423},
{"x-ebcdic-hebrew",20424},
{"x-ebcdic-icelandic",20871},
{"x-ebcdic-icelandic-euro",1149},
{"x-ebcdic-international-euro",1148},
{"x-ebcdic-italy",20280},
{"x-ebcdic-italy-euro",1144},
{"x-ebcdic-japaneseandkana",50930},
{"x-ebcdic-japaneseandjapaneselatin",50939},
{"x-ebcdic-japaneseanduscanada",50931},
{"x-ebcdic-japanesekatakana",20290},
{"x-ebcdic-koreanandkoreanextended",50933},
{"x-ebcdic-koreanextended",20833},
{"cp870",870},
{"x-ebcdic-simplifiedchinese",50935},
{"x-ebcdic-spain",20284},
{"x-ebcdic-spain-euro",1145},
{"x-ebcdic-thai",20838},
{"x-ebcdic-traditionalchinese",50937},
{"cp1026",1026},
{"x-ebcdic-turkish",20905},
{"x-ebcdic-uk",20285},
{"x-ebcdic-uk-euro",1146},
{"ebcdic-cp-us",37},
{"x-ebcdic-cp-us-euro",1140},
{"ibm861",861},
{"x-mac-icelandic",10079},
{"x-iscii-as",57006},
{"x-iscii-be",57003},
{"x-iscii-de",57002},
{"x-iscii-gu",57010},
{"x-iscii-ka",57008},
{"x-iscii-ma",57009},
{"x-iscii-or",57007},
{"x-iscii-pa",57011},
{"x-iscii-ta",57004},
{"x-iscii-te",57005},
{"euc-jp",51932},
{"iso-2022-jp",50220},
{"iso-2022-jp",50222},
{"csiso2022jp",50221},
{"x-mac-japanese",10001},
{"shift_jis",932},
{"ks_c_5601-1987",949},
{"euc-kr",51949},
{"iso-2022-kr",50225},
{"johab",1361},
{"x-mac-korean",10003},
{"iso-8859-3",28593},
{"iso-8859-15",28605},
{"x-ia5-norwegian",20108},
{"ibm437",437},
{"x-ia5-swedish",20107},
{"windows-874",874},
{"ibm857",857},
{"iso-8859-9",28599},
{"x-mac-turkish",10081},
{"windows-1254",1254},
//{(const char *)L"unicode",1200},
//{"unicodefffe",1201},
{"utf-7",65000},
{"utf-8",65001},
//{"us-ascii",20127},
{"us-ascii",1252},
{"windows-1258",1258},
{"ibm850",850},
{"x-ia5",20105},
{"iso-8859-1",1252}, //was 28591
{"macintosh",10000},
{"windows-1252",1252},
{"system",CP_ACP}
};
*/

/*
WINVER
0x030a // Windows 3.1
0x0400 // Windows 95/ NT4.0
0x0410 // Windows 98
0x0500 // Windows Me/ 2000
0x0501 // Windows XP/ Server 2003
0x0502 // Windows XP SP2/ Server 2003 SP1
0x0600 // Windows Vista/ Server 2008
0x0601 // Windows 7

_WIN32_WINDOWS
0x0400 // Windows 95
0x0410 // Windows 98
0x0500 // Windows Me

_WIN32_WINNT_WIN2K=0x0500 (windows2000)
_WIN32_WINNT_WINXP=0x0501 (windows xp/windows server 2003)
_WIN32_WINNT_WS03=0x0502  (Windows XP SP2/ Server 2003 SP1)
_WIN32_WINNT_VISTA=0x0600 (Windows Vista)
_WIN32_WINNT_WS08=0x0600  (windows server 2008)
_WIN32_WINNT_WIN7=0x0601  (windows 7)

_WIN32_IE
0x0200 // Windows 95/NT 4.0(Comctl32.dll 4.00, Shell32.dll 4.00)
0x0300 // Internet Explorer 3.0, 3.01, 3.02
0x0400 // Internet Explorer 4.0
0x0401 // Internet Explorer 4.01
0x0500 // Internet Explorer 5.0, 5.0a, 5.0b
0x0501 // Internet Explorer 5.01
0x0550 // Internet Explorer 5.5
0x0600 // Internet Explorer 6.0
0x0601 // Internet Explorer 6.0 SP1
0x0603 // Internet Explorer 6.0 SP2
0x0700 // Internet Explorer 7.0
0x0800 // Internet Explorer 8.0
*/