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

// MainForm.h
// The C# like Form class
// Author: Shu.KK
// 2008-06-11
//
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef CC_WIN_MAINFORM_H_
#define CC_WIN_MAINFORM_H_

#include "..\cc\cc.All.h"
#include "resource.h"
#include "..\ccExt\AES.h"
#include "../ccExt/unzip.h"

//loop count for crypt key list in page RegkeyPage, also used while make setup.exe
#define UU_REG_CRYPT_TEMPKEY_LOOPCNT 8
#define UU_REG_CRYPT_DATA_LOOPCNT 4
//loop count for crypt key in page RegkeyPage2, also must same to keymaker
#define UU_REG_CRYPT_KEYLIST_LOOPCNT 4

using namespace cc;
using namespace cc::win;


//#define BUF_LEN 256
class MainForm : public Dialog
{
public:
	MainForm();
	virtual ~MainForm();

protected:
	//TCHAR buf[BUF_LEN];
	ImageList* imageList;

	CheckBox* toolOnTop;
	Label* label;
	Button* btnGenPass;
	TextBox* editPMin;
	TextBox* editPMax;
	CheckBox* chkPUpper;
	CheckBox* chkPLower;
	CheckBox* chkPNumber;
	CheckBox* chkPSymbol1;
	TextBox* editPSymbol1;
	CheckBox* chkPSymbol2;
	TextBox* editPSymbol2;
	TextBox* editPass;
	SpinButton* spanPMin;
	SpinButton* spanPMax;

	void InitializeComponent();
	void Form_Load(Object* sender, EventArgs* e);
	void Form_Closed(Object* sender, EventArgs* e);

	void Control_Click(Object* sender, EventArgs* e);
	void menuExit_Click(Object* sender, EventArgs* e);
	void toolOnTop_Click(Object* sender, EventArgs* e);
};

#endif //#ifndef CC_WIN_MAINFORM_H_
