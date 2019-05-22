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

#ifdef _UUSAMPLE
	#define WWWLINK _T("http://www.uusetup.com/")
	#define NAMEVER _T("Desktop Timer[uusetup.com]")
	#ifdef _UNICODE
		#define REG_REGKEY _T("SOFTWARE\\uusetup\\DesktopTimer")
	#else
		#define REG_REGKEY _T("SOFTWARE\\uusetup\\DesktopTimer_ansi")
	#endif
#else
	#define WWWLINK _T("http://www.uuware.com/")
	#define NAMEVER _T("Desktop Timer[uuware.com]")
	#ifdef _UNICODE
		#define REG_REGKEY _T("SOFTWARE\\uuware\\DesktopTimer")
	#else
		#define REG_REGKEY _T("SOFTWARE\\uuware\\DesktopTimer_ansi")
	#endif
#endif

using namespace cc;
using namespace cc::win;

class SetDialog : public Dialog
{
public:
	SetDialog();

protected:
	Button* btnOK;
	Button* btnCancel;
	RadioButton* radio1;
	RadioButton* radio2;
	TextBox* editM0;
	TextBox* editS0;
	SpinButton* spanM0;
	SpinButton* spanS0;
	TextBox* editH;
	TextBox* editM;
	TextBox* editS;
	SpinButton* spanH;
	SpinButton* spanM;
	SpinButton* spanS;
	CheckBox* chkBeep;
	Button* btnPlayBeep;
	CheckBox* chkSound;
	TextBox* editSound;
	PathButton* btnSelFile;
	Button* btnPlaySound;
	CheckBox* chkText;
	TextBox* editText;

	void Form_Load(Object* sender, EventArgs* e);
	void Form_Closed(Object* sender, EventArgs* e);

	void Control_Click(Object* sender, EventArgs* e);
};

class MainForm : public Form
{
public:
	MainForm();

protected:
	ImageList* imageList;
	LabelLink* link;
	LEDControl* led;
	int nResetSec;

	MenuItem* menuOption;
	MenuItem* menuQuick;
	MenuItem* menu1M;
	MenuItem* menu5M;
	MenuItem* menu10M;
	MenuItem* menu15M;
	MenuItem* menu30M;
	MenuItem* menu1H;
	MenuItem* menuTopmost;
	MenuItem* menuHhmm;
	MenuItem* menuNoCaption;
	MenuItem* menuNolink;
	MenuItem* menuAbout;
	MenuItem* menuExit;
	ContextMenu* contextMenu;

	ToolItem* chkTimer;
	ToolItem* btnStart;
	ToolItem* btnClear;
	ToolItem* btnSet;

	void Form_Load(Object* sender, EventArgs* e);
	void Form_Closed(Object* sender, EventArgs* e);
	void OnMouseDown(Object* sender, MouseEventArgs* e);
	void Control_Click(Object* sender, EventArgs* e);
};

#endif //#ifndef CC_WIN_MAINFORM_H_
