#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

//for test CreatePropSheet
#define IDD_ABOUTBOX                    103
#define IDS_APP_TITLE                   103
#define IDM_ABOUT                       104
#define IDD_PROPPAGE1                   104
#define IDM_EXIT                        105
#define IDD_PROPPAGE2                   106
#define IDI_WIN32EMBEDPROPSHEET         107
#define IDI_SMALL                       108
#define IDC_WIN32EMBEDPROPSHEET         109
#define IDR_MAINFRAME                   128
#define IDC_COMBO1                      1000
#define IDC_EDIT1                       1001
#define IDC_BUTTON1                     1002
#define IDC_CHECK1                      1003
#define IDC_RADIO1                      1004
#define IDC_RADIO2                      1005
#define IDC_RADIO3                      1006
#define IDC_RADIO4                      1007
#define IDC_CHECK2                      1008
#define IDC_CHECK3                      1009
#define ID_CREATE_PROP                  32771
#define IDC_STATIC                  1100

#ifndef _UUVERSION
//define for version
#ifndef _CC_ASSISTANT

#define FILEVER           1, 2, 1, 0
#define STRFILEVER       "1, 2, 1, 0"
#define PRODUCTVER       FILEVER
#define STRPRODUCTVER    STRFILEVER
#define FILENAME         "ccSample"
#define APPNAME          "ccSample[uuware.com]"
#define APPVERSION       "1.21"
//#define APPBUILD         "build 20080915"
#define APPBUILD         "build 20130115"

#endif
#endif

#define IDS_APP_TITLE			103

#define IDR_MAINFRAME			128

#define IDF_LANG_INNER          104
#define IDI_ICON_MAIN           102
#define IDI_ICON_COPY           103
#define IDI_ICON_CUT            104
#define IDI_ICON_PASTE          105
#define IDI_ICON_ATTACH         106
#define IDI_ICON_MOVE1          107
#define IDI_ICON_MOVE2          108
#define IDI_ICON_DELETE         110
#define IDI_ICON_EXIT           111
#define IDI_ICON_FONT           112
#define IDI_ICON_LOCK           113
#define IDI_ICON_SAVE           114
#define IDI_ICON_REFRESH        115
#define IDI_ICON_PRINT          116
#define IDI_ICON_NEW            117
#define IDI_ICON_NEWWIN         118
#define IDI_ICON_COLOR          119
#define IDI_ICON_NOTE           120
#define IDI_ICON1               121
#define IDI_ICON2               122
#define IDI_ICON3               123
#define IDI_ICON4               124
#define IDI_ICON5               125
#define IDI_ICON_RED16               126
#define IDI_ICON_BLUE32              127
#define IDI_ICON_R16B32              128
#define IDI_ICON_TESTH               129
#define IDI_ICON_TESTG               130
#define IDI_ICON_TESTD               131
#define IDI_ICON_BTN_HOT                132
#define IDI_ICON_BTN_DIS                133
#define IDI_ICON_BTNAERO_HOT         135
#define IDI_ICON_BTNAERO_GLOOM       136
#define IDI_ICON_BTNAERO_PUSHED      137
#ifndef _CC_ASSISTANT
#define IDI_ICON_SETBACK      138
#endif

//for DesktopCalendar
#define IDI_ICON_LEFT         200
#define IDI_ICON_LEFTEND      201
#define IDI_ICON_RIGHT        202
#define IDI_ICON_RIGHTEND     203

#define IDI_BMP_BACK16               1001

#define IDI_IMG_BACK1               2001
#define IDI_IMG_BACK2               2002

#ifdef APSTUDIO_INVOKED
#ifndef APSTUDIO_READONLY_SYMBOLS

#define _APS_NO_MFC					130
#define _APS_NEXT_RESOURCE_VALUE	129
#define _APS_NEXT_COMMAND_VALUE		32771
#define _APS_NEXT_CONTROL_VALUE		1000
#define _APS_NEXT_SYMED_VALUE		110
#endif
#endif
