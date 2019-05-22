// MainForm.h
// The C# like Form class
// Author: Shu.KK
// 2008-06-11
//
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef CC_WIN_CMAINFORM_H_
#define CC_WIN_CMAINFORM_H_

#include "..\cc\cc.All.h"
#include "resource.h"

#include <wininet.h>
#pragma comment(lib, "wininet.lib")

#define WWWLINK _T("http://www.uuware.com/")
#define NAMEVER _T("Desktop Timer[uuware.com]")
#ifdef _UNICODE
	#define REG_REGKEY _T("SOFTWARE\\uuware\\DesktopCalendar")
#else
	#define REG_REGKEY _T("SOFTWARE\\uuware\\DesktopCalendar_ansi")
#endif

using namespace cc;
using namespace cc::win;

//timer for mainform(called from uuAssistant)
#define UU_TIMERID_CALENDAR_EXIT   CC_TIMERID_USER + 7

//timer for CMainForm
#define UU_TIMERID_EDITDAY_FRESH   CC_TIMERID_USER + 1
#define UU_TIMERID_EDITDAY_EXIT   CC_TIMERID_USER + 2

class EditDialog : public Form
{
public:
	EditDialog(cc::KeyArr* arrMemo, HWND hWndCalendar);

	void EditDate(cc::Str sDate);

protected:
	int m_oldIndex;
	COLORREF m_oldColor;
	cc::KeyArr* m_arrMemo;
	HWND m_hWndCalendar;

	Button* btnOK;
	Button* btnRemove;
	PickColorCtl* pickColor;
	ListBox* listMemo;
	TextBox* editText;

	void Form_Load(Object* sender, EventArgs* e);
	void Form_Closed(Object* sender, EventArgs* e);
	void Form_Closing(Object* sender, CancelEventArgs* e);

	void Control_Click(Object* sender, EventArgs* e);
	void SaveCurrentText();
};

class SetDialog : public Dialog
{
public:
	SetDialog();

protected:
	Button* btnOK;
	Button* btnCancel;

	ComboBox* comboC1;
	PickColorCtl* pickColor1;
	ComboBox* comboC2;
	PickColorCtl* pickColor2;
	ComboBox* comboC3;
	PickColorCtl* pickColor3;
	ComboBox* comboC0;
	TextBox* editY0;
	SpinButton* spanY0;
	Button* btnDl0;
	Button* btnOpen0;

	void Form_Load(Object* sender, EventArgs* e);
	void Form_Closed(Object* sender, EventArgs* e);

	void Control_Click(Object* sender, EventArgs* e);
};

#define UU_TIMERID_INACTIVE   CC_TIMERID_USER + 6
#define UU_TIMERID_INACTIVE2  CC_TIMERID_USER + 7
#define UU_TIMERID_INACTIVE3  CC_TIMERID_USER + 8
#define UU_TIMERID_MONITOR  CC_TIMERID_USER + 9
#define UU_TIMERID_CHGDISP  CC_TIMERID_USER + 10
class CalendarItem : public Control
{
public:
	CalendarItem();
	virtual ~CalendarItem();

	COLORREF GetColorDot();
	void SetColorDot(COLORREF value);

	COLORREF GetColor1();
	void SetColor1(COLORREF value);
	COLORREF GetColor2();
	void SetColor2(COLORREF value);
	COLORREF GetColor3();
	void SetColor3(COLORREF value);
	bool GetToday();
	void SetToday(bool value);

protected:
	COLORREF colorDot;
	COLORREF color1;
	COLORREF color2;
	COLORREF color3;
	bool isToday;

	virtual void OnPaint(PaintEventArgs *e);
};

class CMainForm : public Form
{
public:
	CMainForm(HWND mainFormHwnd);
	~CMainForm();
	static HFONT dayFont;

protected:
	HWND mainFormHwnd;
	ImageList* imageList;
	LEDControl* led;
	int currentY;
	int currentM;
	int nStep;
	int nMode;
	int dayWidth;
	int dayHeight;
	int dayWidthSpace;
	int dayHeightSpace;
	int dayFontSize;
	int dayBigMultiple;
	int dayBigMultipleSave;

	WinInet* winInet;
	cc::Str sSESN;
	cc::Time tShown;

	Form* formCopy;
	EditDialog* editDialog;
	Time* time;
	CalendarItem* labYM[3];
	CalendarItem* labTitle[3][7];
	CalendarItem* labDay[3][7*6];
	CalendarItem* todayItem;

	MenuItem* menuToday;
	MenuItem* menuOption;
	MenuItem* menuTopmost;
	MenuItem* menuShowBefore;
	MenuItem* menuShowAfter;
	MenuItem* menuShowBig;
	MenuItem* menuShowSmall;
	MenuItem* menuShowTime;
	MenuItem* menuHhmm;
	MenuItem* menuMonFirst;
	MenuItem* menuMemoList;
	//MenuItem* menuNoCaption;
	MenuItem* menuAbout;
	MenuItem* menuExit;
	ContextMenu* contMenu;

	ToolItem* btnYPrev;
	ToolItem* btnMPrev;
	ToolItem* btnMNext;
	ToolItem* btnYNext;
	NotifyIcon* notify1;
	cc::KeyArr calendarArr1;
	cc::KeyArr calendarArr2;
	cc::KeyArr calendarArr3;
	cc::KeyArr calendarMemo;
	cc::Str nameCalendar1;
	cc::Str nameCalendar2;
	cc::Str nameCalendar3;
	COLORREF corCalendar1;
	COLORREF corCalendar2;
	COLORREF corCalendar3;
	COLORREF corWeek7;
	COLORREF corWeek6;

	virtual void OnInnerTimer(EventArgs *e);
	virtual LRESULT WndProc(EventArgs *e);
	void Form_Load(Object* sender, EventArgs* e);
	void Form_Shutdown(Object* sender, EventArgs* e);
	void Form_Closed(Object* sender, EventArgs* e);
	void Control_Click(Object* sender, EventArgs* e);
	void Day_Click(Object* sender, EventArgs* e);
	void notify1_Click(Object* sender, EventArgs* e);
	void formCopy_Deactivate(Object* sender, EventArgs* e);
	void DoSetLang(KeyArr& keyArr, cc::Str sThisHead);

	void PrintMonth_All(bool sizeWindow);
	void PrintMonth(bool showMonFirst, int indMonth, int nYYYY, int nMM, int nBaseLeft, int& nTop);
	void HideMonth(int indMonth);
	void Load_Calendar();
	void AdjustLocation();
};

#endif //#ifndef CC_WIN_CMAINFORM_H_
