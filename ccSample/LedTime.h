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
	#define NAMEVER _T("Led Time[uusetup.com]")
	#ifdef _UNICODE
		#define REG_REGKEY _T("SOFTWARE\\uusetup\\LedTime")
	#else
		#define REG_REGKEY _T("SOFTWARE\\uusetup\\LedTime_ansi")
	#endif
#else
	#define WWWLINK _T("http://www.uuware.com/")
	#define NAMEVER _T("Led Time[uuware.com]")
	#ifdef _UNICODE
		#define REG_REGKEY _T("SOFTWARE\\uuware\\LedTime")
	#else
		#define REG_REGKEY _T("SOFTWARE\\uuware\\LedTime_ansi")
	#endif
#endif

using namespace cc;
using namespace cc::win;

class MainForm : public Form
{
public:
	MainForm();

protected:
	ImageList* imageList;
	LabelLink* link;
	LEDControl* led;

	MenuItem* menuTopmost;
	MenuItem* menuNolink;
	MenuItem* menuHhmm;
	MenuItem* menuNoCaption;
	MenuItem* menuAbout;
	MenuItem* menuExit;
	ContextMenu* contextMenu;

	void InitializeComponent();
	void Form_Load(Object* sender, EventArgs* e);
	void Form_Closed(Object* sender, EventArgs* e);
	void OnMouseDown(Object* sender, MouseEventArgs* e);
	void Control_Click(Object* sender, EventArgs* e);
};

#endif //#ifndef CC_WIN_MAINFORM_H_
