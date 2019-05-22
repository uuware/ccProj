// cc.win.Base.h
// The C# like Controls class
// Author: Shu.KK
// 2006-01-01

/*
system icon:
	IDI_APPLICATION = 32512
	IDI_HAND = 32513
	IDI_EXCLAMATION = 32515
	IDI_ASTERISK = 32516
	IDI_QUESTION = 32514
	IDI_WARNING
	IDI_INFORMATION
	IDI_ERROR
	IDI_WINLOGO
*/
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef CC_WIN_BASE_H_
#define CC_WIN_BASE_H_

#pragma comment(lib, "COMCTL32.lib")
//Tell the linker that the DLL should be delay loaded
//#pragma comment(linker, "/DelayLoad:COMCTL32.lib")

#include "cc.h"
#include <tchar.h>
#include <windows.h>
#include <shlwapi.h>
#include <commctrl.h>
//#include <process.h>

#include "cc.Log.h"
#include "cc.Str.h"
#include "cc.Exception.h"
#include "cc.List.h"
#include "cc.Delegate.h"
#include "cc.Util.h"

#include "cc.win.OleBase.h"
#include "cc.win.IECtrl.h"

//for base of control
#define CC_WND_REG                             0x00001
#define CC_WNDCONTROLBAR_REG                   0x00002
#define CC_WNDMDIFRAME_REG                     0x00004
#define CC_WNDFRAMEORVIEW_REG                  0x00008
#define CC_WNDCOMMCTLS_REG                     0x00010 // means all original Win95
#define CC_WNDOLECONTROL_REG                   0x00020
#define CC_WNDCOMMCTL_UPDOWN_REG               0x00040 // these are original Win95
#define CC_WNDCOMMCTL_TREEVIEW_REG             0x00080
#define CC_WNDCOMMCTL_TAB_REG                  0x00100
#define CC_WNDCOMMCTL_PROGRESS_REG             0x00200
#define CC_WNDCOMMCTL_LISTVIEW_REG             0x00400
#define CC_WNDCOMMCTL_HOTKEY_REG               0x00800
#define CC_WNDCOMMCTL_BAR_REG                  0x01000
#define CC_WNDCOMMCTL_ANIMATE_REG              0x02000
#define CC_WNDCOMMCTL_INTERNET_REG             0x04000 // these are new in IE4
#define CC_WNDCOMMCTL_COOL_REG                 0x08000
#define CC_WNDCOMMCTL_USEREX_REG               0x10000
#define CC_WNDCOMMCTL_DATE_REG                 0x20000
#define CC_WNDCOMMCTL_LINK_REG                 0x40000 // new in IE6

#define CC_WIN95CTLS_MASK                      0x03FC0 // UPDOWN -> PictureBox
#define CC_WNDCOMMCTLSALL_REG                  0x7C010 // COMMCTLS|INTERNET|COOL|USEREX|DATE|LINK
#define CC_WNDCOMMCTLSNEW_REG                  0x7C000 // INTERNET|COOL|USEREX|DATE

//about CC_WM
//  1024(0x0400) ~4999(0x1387) for end user
//  5000~6999 used by ccProj
//  7000~7999 for devlop level1
//  8000~8999 for devlop level2
//  9000~9999 for devlop level3
//  max 32767(0x7FFF)
// Private Window Messages Start Here:
//#define WM_USER                         0x0400
#define CC_WM                             5000
#define CC_WM_END                         6999
#define CC_WM1                            7000
#define CC_WM1_END                        7999
#define CC_WM2                            8000
#define CC_WM2_END                        8999
#define CC_WM3                            9000
#define CC_WM3_END                        9999
//start from this for end use by private window classes.
#define CC_WM_USER                        WM_USER + 100
#define CC_WM_USER_END                    0x1387
//also start from this for end use by applications.
//#define WM_APP                          0x8000

//In the Microsoft Windows environment, an application can define a private message for its own use
//without calling the RegisterWindowMessage API. 
//Message numbers between 0x8000 and 0xBFFF are reserved for this purpose. 
//   0 through WM_USER-1      Messages reserved for use by Windows.
//   WM_USER through 0x7FFF   Integer messages for use by private window classes.
//   WM_APP through 0xBFFF    Messages available for use by applications.
//   0xC000 through 0xFFFF    String messages for use by applications.
//   Greater than 0xFFFF      Reserved by Windows for future use.

//private,special for ccProc,and splite to others by switch WParam
#define CC_WM_MAINMSG                        CC_WM + 1
//sub message is switch to e->WParam
//sub CC_WM_MAINMSG,special for NotifyIcon ID.(once one window only has one NotifyIcon)
#define CC_WM_MAINMSG_WPARAM_NOTIFYICON_ID      1
//while dispose control,send this for control to do OnHandleDestroyed or OnClosed or others
//from 20070301,this is not sendmessage,but only as param of OnClose and others
#define CC_WM_MAINMSG_WPARAM_DISPOSE            2
#define CC_WM_MAINMSG_WPARAM_GETFORM            3
//show status&tiptool
#define CC_WM_MAINMSG_WPARAM_MENUSTATUS         11
#define CC_WM_MAINMSG_WPARAM_CTLSTATUS          12
#define CC_WM_MAINMSG_WPARAM_TEXTSTATUS         13
#define CC_WM_MAINMSG_WPARAM_TEXTTIPSONLY       14
#define CC_WM_MAINMSG_WPARAM_TEXTSTATUSONLY     15
#define CC_WM_MAINMSG_WPARAM_CLEARSTATUS        16
//get rebar from form,and with lpoint at lparam
#define CC_WM_MAINMSG_WPARAM_GET_REBAR          20
#define CC_WM_MAINMSG_WPARAM_GET_REBARSTYLE     21
#define CC_WM_MAINMSG_WPARAM_ACTIVATE           22
#define CC_WM_MAINMSG_WPARAM_GETINNERSTYLE      30
#define CC_WM_MAINMSG_WPARAM_SETINNERSTYLE      31
#define CC_WM_MAINMSG_WPARAM_GETOUTERSTYLE      32
#define CC_WM_MAINMSG_WPARAM_SETOUTERSTYLE      33

#define CC_WM_MAINMSG_WPARAM_CLEARBRUSHPARENT   40
#define CC_WM_MAINMSG_WPARAM_CLEARBRUSHALL      41
//while control get focus then notice activeform,LParam is focused control
#define CC_WM_MAINMSG_WPARAM_FOCUSCHANGED       42

//for design,controls send self's hwnd(lparam) to design toolbar
//#define CC_WM_MAINMSG_WPARAM_DS_SETHOT         100
#define CC_WM_MAINMSG_WPARAM_DS_GETHOT         101
//if design control's rect is not save,then if resize parent so restore it's rect
#define CC_WM_MAINMSG_WPARAM_DS_RESIZE         102
//while design control is locchanged,then need refresh
#define CC_WM_MAINMSG_WPARAM_DS_LOCCHANGED     103

//while child is resize or other updated,notice parent
//WPARAM is the point of control
#define CC_WM_CHILD_UPDATED                      CC_WM + 2

//while item's check status chenged,send this to self
//while CheckListBox:WPARAM is check status,LPARAM is nIndex
//	PostMessage(m_hWnd, CC_WM_CHECKCHANGED, MAKEWPARAM(nHIndex, nNewCheck), (LPARAM)nIndex);
//while TreeView:WPARAM is check status,LPARAM is SelectedItem
//	PostMessage(m_hWnd, CC_WM_CHECKCHANGED, MAKEWPARAM(nHIndex, !isChecked), (LPARAM)ht.hItem);
#define CC_WM_CHECKCHANGED                       CC_WM + 3

//about CC_TIMERID
//  1    ~49999 for end user
//  50000~69999 used by ccProj
//  70000~79999 for devlop level1
//  80000~89999 for devlop level2
//  90000~99999 for devlop level3
#define CC_TIMERID                     50000
#define CC_TIMERID_END                 69999
#define CC_TIMERID1                    70000
#define CC_TIMERID1_END                79999
#define CC_TIMERID2                    80000
#define CC_TIMERID2_END                89999
#define CC_TIMERID3                    90000
#define CC_TIMERID3_END                99999
//start from this ID for end user to use Timer
#define CC_TIMERID_USER                00100
#define CC_TIMERID_USER_END            49999

//timerid for control
//to detect if mouse is enter,use SetTimer.this is id,and wait time is ControlConfig::nWaitTimerCheckMouseLeave
#define CC_TIMERID_CONTROL_MMOVE       50101
//while dragover,scrolling
#define CC_TIMERID_CONTROL_DRAGOVER    50102
//timer for timeout for status,tooltip,form and others
//and while for form,the wait time is FormConfig::nWaitTimerToUpdateTips
//spetial fot toolitem,while ContextMenu->Show,not paint self well,so wait to repaint self
#define CC_TIMERID_CONTROL_TIMEOUT     50103
//timer for NotifyIcon to see if is doubleclick then show left click menu.
#define CC_TIMERID_NOTIFYICON_DBCLICK  50104
//timer for NotifyIcon to show Animate
#define CC_TIMERID_NOTIFYICONL_ANIMATE 50105
//timer for NotifyIcon to show Animate
#define CC_TIMERID_NOTIFYICONL_RESHOW  50106
//while theme changed,need resize of form
#define CC_TIMERID_FORM_CHGTHEME       50107
#define CC_TIMERID_DIALOG_ACTIVATED    50108
//for toolitem or textbox to repaint after SCROLL event
#define CC_TIMERID_INVALIDATERECT      50109
//while _DESIGN, SETHOT
#define CC_TIMERID_DESIGN_SETHOT       50110

//start from this ID for other controls in ccProj
#define CC_TIMERID_CCPROJ_INNER        51000

/* MFC messages [360-38f] */
/*
#define WM_QUERYAFXWNDPROC  0x0360
#define WM_SIZEPARENT       0x0361
#define WM_SETMESSAGESTRING 0x0362
#define WM_IDLEUPDATECMDUI  0x0363 
#define WM_INITIALUPDATE    0x0364
#define WM_COMMANDHELP      0x0365
#define WM_HELPHITTEST      0x0366
#define WM_EXITHELPMODE     0x0367
#define WM_RECALCPARENT     0x0368
#define WM_SIZECHILD        0x0369
#define WM_KICKIDLE         0x036A 
#define WM_QUERYCENTERWND   0x036B
#define WM_DISABLEMODAL     0x036C
#define WM_FLOATSTATUS      0x036D 
#define WM_ACTIVATETOPLEVEL 0x036E 
#define WM_QUERY3DCONTROLS  0x036F 
#define WM_SOCKET_NOTIFY    0x0373
#define WM_SOCKET_DEAD      0x0374
#define WM_POPMESSAGESTRING 0x0375
#define WM_OCC_LOADFROMSTREAM           0x0376
#define WM_OCC_LOADFROMSTORAGE          0x0377
#define WM_OCC_INITNEW                  0x0378
#define WM_OCC_LOADFROMSTREAM_EX        0x037A
#define WM_OCC_LOADFROMSTORAGE_EX       0x037B
#define WM_QUEUE_SENTINEL   0x0379

#define WM_PENWINFIRST      0x0380
#define WM_PENWINLAST       0x038F
*/
/* end of MFC messages */

namespace cc
{
namespace win
{
//define at cc.Delegate.h
//#define CC_CLASSID_Delegate                   520010
#define CC_CLASSID_EventHandler               520020
#define CC_CLASSID_ImageList                  520040
#define CC_CLASSID_Timer                      520050
#define CC_CLASSID_Thread                     520060
#define CC_CLASSID_DrawActor                  520070
#define CC_CLASSID_ITextIcon                  520080
#define CC_CLASSID_Menu                       520090
#define CC_CLASSID_MenuItem                   520100
#define CC_CLASSID_ContextMenu                520110
#define CC_CLASSID_MainMenu                   520120
#define CC_CLASSID_Control                    520130
#define CC_CLASSID_ToolTip                    520140
#define CC_CLASSID_ToolItem                   520150
#define CC_CLASSID_StatusItem                 520160
#define CC_CLASSID_LabelLink                  520170
#define CC_CLASSID_Label                      520180
#define CC_CLASSID_PictureBox                 520190
#define CC_CLASSID_Button                     520200
#define CC_CLASSID_CheckBox                   520210
#define CC_CLASSID_RadioButton                520220
#define CC_CLASSID_Group                      520230
#define CC_CLASSID_SpinButton                 520240
#define CC_CLASSID_ListBox                    520250
#define CC_CLASSID_CheckListBox               520260
#define CC_CLASSID_ComboBox                   520270
#define CC_CLASSID_ComboBoxEx                 520280
#define CC_CLASSID_TextBox                    520290
#define CC_CLASSID_RichTextBox                520300
#define CC_CLASSID_ScrollBar                  520310
#define CC_CLASSID_HScrollBar                 520320
#define CC_CLASSID_VScrollBar                 520330
#define CC_CLASSID_ProgressBar                520340
#define CC_CLASSID_ListView                   520350
#define CC_CLASSID_TreeView                   520360
#define CC_CLASSID_DateTimePicker             520370
#define CC_CLASSID_MonthCalendar              520380
#define CC_CLASSID_Splitter                   520390
#define CC_CLASSID_Container                  520400
#define CC_CLASSID_Panel                      520410
#define CC_CLASSID_ScrollContainer            520420
#define CC_CLASSID_ReBar                      520430
#define CC_CLASSID_FloatContainer             520440
#define CC_CLASSID_DockBar                    520450
#define CC_CLASSID_ToolBar                    520460
#define CC_CLASSID_StatusBar                  520470
#define CC_CLASSID_MainMenuBar                520480
#define CC_CLASSID_UserControl                520490
#define CC_CLASSID_TabPage                    520500
#define CC_CLASSID_FormClient                 520510
#define CC_CLASSID_MDIChild                   520520
#define CC_CLASSID_FormFrame                  520530
#define CC_CLASSID_Form                       520540
#define CC_CLASSID_Dialog                     520550
#define CC_CLASSID_Hotkey                     520560
#define CC_CLASSID_IEControl                  520570
#define CC_CLASSID_NotifyIcon                 520580
#define CC_CLASSID_TabControl                 520590
#define CC_CLASSID_TabControlEx               520600
#define CC_CLASSID_TreeViewEx                 520610
#define CC_CLASSID_GroupPanel                 520620
#define CC_CLASSID_GradientLabel              520630
#define CC_CLASSID_LEDControl                 520640
#define CC_CLASSID_PushButton                 520650

//class Delegate;
//class _BaseEventHandler;
class EventHandler;
class ImageList;
class Timer;
class Thread;
class DrawActor;
class ITextIcon;
	class Menu;
		class MenuItem;
		class ContextMenu;
		class MainMenu;
	class Control;
		class ToolTip;
		class ToolItem;
			class StatusItem;
			class LabelLink;
			class GradientLabel;
		class Label;
			class PictureBox;
		class Button;
			class CheckBox;
			class RadioButton;
			class Group;
		class SpinButton;
		class ListBox;
			class CheckListBox;
		class ComboBox;
			class ComboBoxEx;
		class TextBox;
			class RichTextBox;
		class ScrollBar;
			class HScrollBar;
			class VScrollBar;
		class ProgressBar;
		class ListView;
		class TreeView;
			class TreeViewEx;
		class DateTimePicker;
		class MonthCalendar;
		class Splitter;
		class Container;
			class Panel;
					class ScrollContainer;
					class GroupPanel;
			class ReBar;
			class FloatContainer;
			class DockBar;
				class ToolBar;
				class StatusBar;
				class MainMenuBar;
			class UserControl;
			class TabPage;
			class FormClient;
			class MDIChild;
			class FormFrame;
				class Form;
					class Dialog;
			class TabControlEx;
		class Hotkey;
		class IEControl;
	class NotifyIcon;
	class TabControl;


///////////////////////////////////////////////////////////////////////////////
//#common for all win
//System.Windows.Forms.Shortcut
typedef struct tagShortcut
{
#define SHORTCUT_CTRL 0x0100
#define SHORTCUT_ALT 0x0200
#define SHORTCUT_SHIFT 0x0400
	enum Type
	{
		Alt0 = SHORTCUT_ALT | '0',
		Alt1 = SHORTCUT_ALT | '1',
		Alt2 = SHORTCUT_ALT | '2',
		Alt3 = SHORTCUT_ALT | '3',
		Alt4 = SHORTCUT_ALT | '4',
		Alt5 = SHORTCUT_ALT | '5',
		Alt6 = SHORTCUT_ALT | '6',
		Alt7 = SHORTCUT_ALT | '7',
		Alt8 = SHORTCUT_ALT | '8',
		Alt9 = SHORTCUT_ALT | '9',
		AltBksp = SHORTCUT_ALT | VK_BACK,
		AltF1 = SHORTCUT_ALT | VK_F1,
		AltF2 = SHORTCUT_ALT | VK_F2,
		AltF3 = SHORTCUT_ALT | VK_F3,
		AltF4 = SHORTCUT_ALT | VK_F4,
		AltF5 = SHORTCUT_ALT | VK_F5,
		AltF6 = SHORTCUT_ALT | VK_F6,
		AltF7 = SHORTCUT_ALT | VK_F7,
		AltF8 = SHORTCUT_ALT | VK_F8,
		AltF9 = SHORTCUT_ALT | VK_F9,
		AltF10 = SHORTCUT_ALT | VK_F10,
		AltF11 = SHORTCUT_ALT | VK_F11,
		AltF12 = SHORTCUT_ALT | VK_F12,
		Ctrl0 = SHORTCUT_CTRL | '0',
		Ctrl1 = SHORTCUT_CTRL | '1',
		Ctrl2 = SHORTCUT_CTRL | '2',
		Ctrl3 = SHORTCUT_CTRL | '3',
		Ctrl4 = SHORTCUT_CTRL | '4',
		Ctrl5 = SHORTCUT_CTRL | '5',
		Ctrl6 = SHORTCUT_CTRL | '6',
		Ctrl7 = SHORTCUT_CTRL | '7',
		Ctrl8 = SHORTCUT_CTRL | '8',
		Ctrl9 = SHORTCUT_CTRL | '9',
		CtrlA = SHORTCUT_CTRL | 'A',
		CtrlB = SHORTCUT_CTRL | 'B',
		CtrlC = SHORTCUT_CTRL | 'C',
		CtrlD = SHORTCUT_CTRL | 'D',
		CtrlDel = SHORTCUT_CTRL | VK_DELETE,
		CtrlE = SHORTCUT_CTRL | 'E',
		CtrlF = SHORTCUT_CTRL | 'F',
		CtrlF1 = SHORTCUT_CTRL | VK_F1,
		CtrlF2 = SHORTCUT_CTRL | VK_F2,
		CtrlF3 = SHORTCUT_CTRL | VK_F3,
		CtrlF4 = SHORTCUT_CTRL | VK_F4,
		CtrlF5 = SHORTCUT_CTRL | VK_F5,
		CtrlF6 = SHORTCUT_CTRL | VK_F6,
		CtrlF7 = SHORTCUT_CTRL | VK_F7,
		CtrlF8 = SHORTCUT_CTRL | VK_F8,
		CtrlF9 = SHORTCUT_CTRL | VK_F9,
		CtrlF10 = SHORTCUT_CTRL | VK_F10,
		CtrlF11 = SHORTCUT_CTRL | VK_F11,
		CtrlF12 = SHORTCUT_CTRL | VK_F12,
		CtrlG = SHORTCUT_CTRL | 'G',
		CtrlH = SHORTCUT_CTRL | 'H',
		CtrlI = SHORTCUT_CTRL | 'I',
		CtrlIns = SHORTCUT_CTRL | VK_INSERT,
		CtrlJ = SHORTCUT_CTRL | 'J',
		CtrlK = SHORTCUT_CTRL | 'K',
		CtrlL = SHORTCUT_CTRL | 'L',
		CtrlM = SHORTCUT_CTRL | 'M',
		CtrlN = SHORTCUT_CTRL | 'N',
		CtrlO = SHORTCUT_CTRL | 'O',
		CtrlP = SHORTCUT_CTRL | 'P',
		CtrlQ = SHORTCUT_CTRL | 'Q',
		CtrlR = SHORTCUT_CTRL | 'R',
		CtrlS = SHORTCUT_CTRL | 'S',
		CtrlT = SHORTCUT_CTRL | 'T',
		CtrlU = SHORTCUT_CTRL | 'U',
		CtrlV = SHORTCUT_CTRL | 'V',
		CtrlW = SHORTCUT_CTRL | 'W',
		CtrlX = SHORTCUT_CTRL | 'X',
		CtrlY = SHORTCUT_CTRL | 'Y',
		CtrlZ = SHORTCUT_CTRL | 'Z',
		CtrlShift0 = SHORTCUT_CTRL | SHORTCUT_SHIFT | '0',
		CtrlShift1 = SHORTCUT_CTRL | SHORTCUT_SHIFT | '1',
		CtrlShift2 = SHORTCUT_CTRL | SHORTCUT_SHIFT | '2',
		CtrlShift3 = SHORTCUT_CTRL | SHORTCUT_SHIFT | '3',
		CtrlShift4 = SHORTCUT_CTRL | SHORTCUT_SHIFT | '4',
		CtrlShift5 = SHORTCUT_CTRL | SHORTCUT_SHIFT | '5',
		CtrlShift6 = SHORTCUT_CTRL | SHORTCUT_SHIFT | '6',
		CtrlShift7 = SHORTCUT_CTRL | SHORTCUT_SHIFT | '7',
		CtrlShift8 = SHORTCUT_CTRL | SHORTCUT_SHIFT | '8',
		CtrlShift9 = SHORTCUT_CTRL | SHORTCUT_SHIFT | '9',
		CtrlShiftA = SHORTCUT_CTRL | SHORTCUT_SHIFT | 'A',
		CtrlShiftB = SHORTCUT_CTRL | SHORTCUT_SHIFT | 'B',
		CtrlShiftC = SHORTCUT_CTRL | SHORTCUT_SHIFT | 'C',
		CtrlShiftD = SHORTCUT_CTRL | SHORTCUT_SHIFT | 'D',
		CtrlShiftE = SHORTCUT_CTRL | SHORTCUT_SHIFT | 'E',
		CtrlShiftF = SHORTCUT_CTRL | SHORTCUT_SHIFT | 'F',
		CtrlShiftF1 = SHORTCUT_CTRL | SHORTCUT_SHIFT | VK_F1,
		CtrlShiftF2 = SHORTCUT_CTRL | SHORTCUT_SHIFT | VK_F2,
		CtrlShiftF3 = SHORTCUT_CTRL | SHORTCUT_SHIFT | VK_F3,
		CtrlShiftF4 = SHORTCUT_CTRL | SHORTCUT_SHIFT | VK_F4,
		CtrlShiftF5 = SHORTCUT_CTRL | SHORTCUT_SHIFT | VK_F5,
		CtrlShiftF6 = SHORTCUT_CTRL | SHORTCUT_SHIFT | VK_F6,
		CtrlShiftF7 = SHORTCUT_CTRL | SHORTCUT_SHIFT | VK_F7,
		CtrlShiftF8 = SHORTCUT_CTRL | SHORTCUT_SHIFT | VK_F8,
		CtrlShiftF9 = SHORTCUT_CTRL | SHORTCUT_SHIFT | VK_F9,
		CtrlShiftF10 = SHORTCUT_CTRL | SHORTCUT_SHIFT | VK_F10,
		CtrlShiftF11 = SHORTCUT_CTRL | SHORTCUT_SHIFT | VK_F11,
		CtrlShiftF12 = SHORTCUT_CTRL | SHORTCUT_SHIFT | VK_F12,
		CtrlShiftG = SHORTCUT_CTRL | SHORTCUT_SHIFT | 'G',
		CtrlShiftH = SHORTCUT_CTRL | SHORTCUT_SHIFT | 'H',
		CtrlShiftI = SHORTCUT_CTRL | SHORTCUT_SHIFT | 'I',
		CtrlShiftJ = SHORTCUT_CTRL | SHORTCUT_SHIFT | 'J',
		CtrlShiftK = SHORTCUT_CTRL | SHORTCUT_SHIFT | 'K',
		CtrlShiftL = SHORTCUT_CTRL | SHORTCUT_SHIFT | 'L',
		CtrlShiftM = SHORTCUT_CTRL | SHORTCUT_SHIFT | 'M',
		CtrlShiftN = SHORTCUT_CTRL | SHORTCUT_SHIFT | 'N',
		CtrlShiftO = SHORTCUT_CTRL | SHORTCUT_SHIFT | 'O',
		CtrlShiftP = SHORTCUT_CTRL | SHORTCUT_SHIFT | 'P',
		CtrlShiftQ = SHORTCUT_CTRL | SHORTCUT_SHIFT | 'Q',
		CtrlShiftR = SHORTCUT_CTRL | SHORTCUT_SHIFT | 'R',
		CtrlShiftS = SHORTCUT_CTRL | SHORTCUT_SHIFT | 'S',
		CtrlShiftT = SHORTCUT_CTRL | SHORTCUT_SHIFT | 'T',
		CtrlShiftU = SHORTCUT_CTRL | SHORTCUT_SHIFT | 'U',
		CtrlShiftV = SHORTCUT_CTRL | SHORTCUT_SHIFT | 'V',
		CtrlShiftW = SHORTCUT_CTRL | SHORTCUT_SHIFT | 'W',
		CtrlShiftX = SHORTCUT_CTRL | SHORTCUT_SHIFT | 'X',
		CtrlShiftY = SHORTCUT_CTRL | SHORTCUT_SHIFT | 'Y',
		CtrlShiftZ = SHORTCUT_CTRL | SHORTCUT_SHIFT | 'Z',
		Del = VK_DELETE,
		F1 = VK_F1,
		F2 = VK_F2,
		F3 = VK_F3,
		F4 = VK_F4,
		F5 = VK_F5,
		F6 = VK_F6,
		F7 = VK_F7,
		F8 = VK_F8,
		F9 = VK_F9,
		F10 = VK_F10,
		F11 = VK_F11,
		F12 = VK_F12,
		Ins = VK_INSERT,
		None = 0,
		ShiftDel = SHORTCUT_SHIFT | VK_DELETE,
		ShiftF1 = SHORTCUT_SHIFT | VK_F1,
		ShiftF2 = SHORTCUT_SHIFT | VK_F2,
		ShiftF3 = SHORTCUT_SHIFT | VK_F3,
		ShiftF4 = SHORTCUT_SHIFT | VK_F4,
		ShiftF5 = SHORTCUT_SHIFT | VK_F5,
		ShiftF6 = SHORTCUT_SHIFT | VK_F6,
		ShiftF7 = SHORTCUT_SHIFT | VK_F7,
		ShiftF8 = SHORTCUT_SHIFT | VK_F8,
		ShiftF9 = SHORTCUT_SHIFT | VK_F9,
		ShiftF10 = SHORTCUT_SHIFT | VK_F10,
		ShiftF11 = SHORTCUT_SHIFT | VK_F11,
		ShiftF12 = SHORTCUT_SHIFT | VK_F12,
		ShiftIns = SHORTCUT_SHIFT | VK_INSERT,
	};
}Shortcut;

//System.Windows.Forms.DialogResult
//for form,also for button's DialogResult
typedef struct tagDialogResult
{
	enum Type
	{
		Empty = 0,
		Abort = 1,
		Cancel = 2,
		Ignore = 3,
		No = 4,
		None = 5,
		OK = 6,
		Retry = 7,
		Yes = 8,
	};
}DialogResult;

//System.Windows.Forms.AnchorStyles
//for form,also for button's AnchorStyles
typedef struct tagAnchorStyles
{
	enum Type
	{
		Bottom = 2,
		Left = 4,
		None = 0,
		Right = 8,
		Top = 1,
	};
}AnchorStyles;

//System.Windows.Forms.DockStyle
//for form,also for button's DockStyle
typedef struct tagDockStyles
{
	enum Type
	{
		Bottom = 0x0002,
		Left = 0x0004,
		//none is for floatable,so cannot be 0
		None = 0x0010,
		Right = 0x0008,
		Top = 0x0001,
		Fill = Left | Top | Right | Bottom,
	};
}DockStyles;

typedef struct tagBoundsSpecified
{
	enum Type
	{
		All = 0x0001,
		Height,
		Location,
		None,
		Size,
		Width,
		X,
		Y,
	};
}BoundsSpecified;

typedef struct tagDrawItemState
{
	enum Type
	{
		None			= 0x0000,
		Selected		= 0x0001,
		Grayed			= 0x0002,
		Disabled		= 0x0004,
		Checked			= 0x0008,
		Focus			= 0x0010,
		Default			= 0x0020,
		HotLight		= 0x0040,
		Inactive		= 0x0080,
		NoAccelerator	= 0x0100,
		NoFocusRect		= 0x0200,
		ComboBoxEdit	= 0x1000,
	};
}DrawItemState;

typedef struct tagMouseButtons
{
	enum Type
	{
		None		= 0x00000000,
		Left		= 0x00100000,
		Right		= 0x00200000,
		Middle		= 0x00400000,
		XButton1	= 0x00800000,
		XButton2	= 0x01000000,
	}; // enum ControlStyles
}MouseButtons;

typedef struct tagMenuStyle
{
	enum Type
	{
		XP,
		Office,
		VS,
		Default,
	};
}MenuStyle;

typedef struct tagIconType
{
	enum Type
	{
		Hot			= 0x0001,
		Gloom		= 0x0002,
		Disabled	= 0x0003,
		Pushed		= 0x0004,
	};
}IconType;

//namespace System.Windows.Forms {
typedef struct tagHorizontalAlignment
{
	enum Type
	{
		Center = 2,
		Left = 0,
		Right = 1
	};
}HorizontalAlignment;

//namespace System.Windows.Forms.VisualStyles
typedef struct tagVerticalAlignment
{
	enum Type
	{
		Top = 0,
		Center = 1,
		Bottom = 2
	};
}VerticalAlignment;

//for set left or right
typedef struct tagLeftRight
{
	enum Type
	{
		Left = 0x0001,
		Right = 0x0002,
	};
}LeftRight;

//#common for all win
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//#only for control
//Message structure
typedef struct tagEventArgs
{
	HWND        HWnd;
	UINT        Msg;
	WPARAM      WParam;
	LPARAM      LParam;
	bool        Handled;
	LRESULT     Result;
	tagEventArgs(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		HWnd = hWnd;
		Msg = msg;
		WParam = wParam;
		LParam = lParam;
		Handled = false;
		Result = 0;
	};
} EventArgs;

typedef struct tagUICuesEventArgs
{
	EventArgs* baseArgs;
	bool ChangeFocus;
	bool ChangeKeyboard;
	bool ShowFocus;
	bool ShowKeyboard;
	tagUICuesEventArgs(EventArgs* baseArgs)
	{
		this->baseArgs = baseArgs;
		ChangeFocus = false;
		ChangeKeyboard = false;
		ShowFocus = false;
		ShowKeyboard = false;
	};
} UICuesEventArgs;

typedef struct tagControlEventArgs
{
	EventArgs* baseArgs;
	Control* control;
	tagControlEventArgs(EventArgs* baseArgs)
	{
		this->baseArgs = baseArgs;
		control = NULL;
	};
} ControlEventArgs;

typedef struct tagDragBeginEventArgs
{
	EventArgs* baseArgs;
	MouseButtons::Type Button;
	void* data;
	//if can not drag this item
	bool Cancel;
	tagDragBeginEventArgs(EventArgs* baseArgs)
	{
		this->baseArgs = baseArgs;
		Button = MouseButtons::None;
		data = NULL;
		Cancel = false;
	};
} DragBeginEventArgs;

typedef struct tagDragEndEventArgs
{
	EventArgs* baseArgs;
	MouseButtons::Type Button;
	void* data;
	bool Cancel;
	tagDragEndEventArgs(EventArgs* baseArgs)
	{
		this->baseArgs = baseArgs;
		Button = MouseButtons::None;
		data = 0;
		Cancel = false;
	};
} DragEndEventArgs;

typedef struct tagGiveFeedbackEventArgs
{
	EventArgs* baseArgs;
	//if while esc to cancel
	bool Cancel;
	tagGiveFeedbackEventArgs(EventArgs* baseArgs)
	{
		this->baseArgs = baseArgs;
		Cancel = false;
	};
} GiveFeedbackEventArgs;

typedef struct tagHelpEventArgs
{
	EventArgs* baseArgs;
	tagHelpEventArgs(EventArgs* baseArgs)
	{
		this->baseArgs = baseArgs;
	};
} HelpEventArgs;

typedef struct tagInvalidateEventArgs
{
	EventArgs* baseArgs;
	tagInvalidateEventArgs(EventArgs* baseArgs)
	{
		this->baseArgs = baseArgs;
	};
} InvalidateEventArgs;

typedef struct tagKeyEventArgs
{
	EventArgs* baseArgs;
	tagKeyEventArgs(EventArgs* baseArgs)
	{
		this->baseArgs = baseArgs;
	};
} KeyEventArgs;

typedef struct tagKeyPressEventArgs
{
	EventArgs* baseArgs;
	tagKeyPressEventArgs(EventArgs* baseArgs)
	{
		this->baseArgs = baseArgs;
	};
} KeyPressEventArgs;

typedef struct tagLayoutEventArgs
{
	EventArgs* baseArgs;
	tagLayoutEventArgs(EventArgs* baseArgs)
	{
		this->baseArgs = baseArgs;
	};
} LayoutEventArgs;

typedef struct tagMouseEventArgs
{
	EventArgs* baseArgs;
	MouseButtons::Type Button;
	int Clicks;
	int Delta;
	int X;
	int Y;
	tagMouseEventArgs(EventArgs* baseArgs)
	{
		this->baseArgs = baseArgs;
		Button = MouseButtons::None;
		Clicks = 0;
		Delta = 0;
		X = 0;
		Y = 0;
	};
} MouseEventArgs;

typedef struct tagPaintEventArgs
{
	EventArgs* baseArgs;
	PAINTSTRUCT data;
	tagPaintEventArgs(EventArgs* baseArgs)
	{
		this->baseArgs = baseArgs;
		memset(&data, 0, sizeof(PAINTSTRUCT));
	};
} PaintEventArgs;

typedef struct tagQueryContinueDragEventArgs
{
	EventArgs* baseArgs;
	tagQueryContinueDragEventArgs(EventArgs* baseArgs)
	{
		this->baseArgs = baseArgs;
	};
} QueryContinueDragEventArgs;

typedef struct tagCancelEventArgs
{
	EventArgs* baseArgs;
	bool Cancel;
	tagCancelEventArgs(EventArgs* baseArgs)
	{
		this->baseArgs = baseArgs;
		Cancel = false;
	};
} CancelEventArgs;

typedef struct tagScrollEventArgs
{
	EventArgs* baseArgs;
	bool Horizontal;
	int NewValue;
	tagScrollEventArgs(EventArgs* baseArgs)
	{
		this->baseArgs = baseArgs;
		NewValue = -1;
		Horizontal = false;
	};
} ScrollEventArgs;

//for listbox
typedef struct tagCompareItemEventArgs
{
	EventArgs* baseArgs;
	LPCOMPAREITEMSTRUCT data;
//	int nIDCtl;
	tagCompareItemEventArgs(EventArgs* baseArgs, LPCOMPAREITEMSTRUCT data = NULL)
	{
		this->baseArgs = baseArgs;
		this->data = data;
//		this->nIDCtl = nIDCtl;
	};
} CompareItemEventArgs;
//typedef struct tagDeleteItemEventArgs
//{
//	EventArgs* baseArgs;
//	LPDELETEITEMSTRUCT data;
//	tagDeleteItemEventArgs(EventArgs* baseArgs, LPDELETEITEMSTRUCT data = NULL)
//	{
//		this->baseArgs = baseArgs;
//		this->data = data;
//	};
//} DeleteItemEventArgs;
typedef struct tagVKeyToItemEventArgs
{
	EventArgs* baseArgs;
	UINT nKey;
	UINT nIndex;
	tagVKeyToItemEventArgs(EventArgs* baseArgs, UINT nKey = 0, UINT nIndex = 0)
	{
		this->baseArgs = baseArgs;
		this->nKey = nKey;
		this->nIndex = nIndex;
	};
} VKeyToItemEventArgs;
typedef struct tagCharToItemEventArgs
{
	EventArgs* baseArgs;
	UINT nChar;
	UINT nIndex;
	tagCharToItemEventArgs(EventArgs* baseArgs, UINT nChar = 0, UINT nIndex = 0)
	{
		this->baseArgs = baseArgs;
		this->nChar = nChar;
		this->nIndex = nIndex;
	};
} CharToItemEventArgs;
//#only for control
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
//#only for menu
typedef COLORREF Color;
//this is used to info DrawActor::DoDrawControl that this popuping menu
#define CC_DRAWITEM_POPMENU      0x0001
#define CC_DRAWITEM_BUTTON       0x0002
#define CC_DRAWITEM_CHECKBOX     0x0004
#define CC_DRAWITEM_RADIOBUTTON  0x0008
#define CC_DRAWITEM_GROUPBOX     0x0010
#define CC_DRAWITEM_3STATE       0x0020
//for item if has pop menu or pop floatcontainer
#define CC_DRAWITEM_RIGHTARROW   0x0030
typedef struct tagDrawItemEventArgs
{
	EventArgs* baseArgs;
	LPDRAWITEMSTRUCT data;
	DWORD dwStyle; //flags for DrawActor::DrawControl
	tagDrawItemEventArgs(EventArgs* baseArgs)
	{
		this->baseArgs = baseArgs;
		data = NULL;
		dwStyle = 0;
	};
} DrawItemEventArgs;

typedef struct tagMeasureItemEventArgs
{
	EventArgs* baseArgs;
	LPMEASUREITEMSTRUCT data;
	HDC hDC;
	tagMeasureItemEventArgs(EventArgs* baseArgs)
	{
		this->baseArgs = baseArgs;
		data = NULL;
		hDC = NULL;
	};
} MeasureItemEventArgs;
//#only for menu
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//===========================================================cc::win::EventHandler
template<class _Param>
class _BaseEventHandler : public Delegate<Object, void (Object::*)(Object*, _Param*)>
{
public:
	_BaseEventHandler()
	{
		_lstDele = NULL;
		//_mapDele = NULL;
	}
	_BaseEventHandler(Object* obj, void (Object::*method)(Object*, _Param*))
		: Delegate<Object, void (Object::*)(Object*, _Param*)>(obj, method)
	{
		_lstDele = new _BaseEventHandlerObj();
		_lstDele->obj = obj;
		_lstDele->method = method;
		_lstDele->next = NULL;
	}
	virtual ~_BaseEventHandler()
	{
		Clear();
	}

	void Invoke(Object* p1, _Param* p2)
	{
		if(_lstDele != NULL)
		{
			_BaseEventHandlerObj* temp = _lstDele;
			while(temp != NULL)
			{
				((temp->obj)->*(temp->method))(p1, p2);
				temp = temp->next;
			}
		}
	}

	void Clear()
	{
		if(_lstDele != NULL)
		{
			_BaseEventHandlerObj* temp = _lstDele;
			while(temp != NULL)
			{
				_BaseEventHandlerObj* temp2 = temp->next;
				delete temp;
				temp = temp2;
			}
			_lstDele = NULL;
		}
	}

	void operator()(Object* p1, _Param* p2)
	{
		Invoke(p1, p2);
	}

	//assignment operators
	_BaseEventHandler& operator=(const _BaseEventHandler& handler)
	{
		Clear();
		return operator+=(handler);
	}

	_BaseEventHandler& operator+=(const _BaseEventHandler& handler)
	{
		if(handler._lstDele != NULL)
		{
			_BaseEventHandlerObj* temp = handler._lstDele;
			while(temp != NULL)
			{
				Add(temp->obj, temp->method);
				temp = temp->next;
			}
		}
		return *this;
	}
	_BaseEventHandler& operator-=(const _BaseEventHandler& handler)
	{
		if(_lstDele != NULL && handler._lstDele != NULL)
		{
			_BaseEventHandlerObj* temp = handler._lstDele;
			while(temp != NULL)
			{
				Remove(temp->obj, temp->method);
				temp = temp->next;
			}
		}
		return *this;
	}

	virtual bool IsEmpty()
	{
		return (_lstDele == NULL);
	}
protected:
	struct _BaseEventHandlerObj
	{
		_BaseEventHandlerObj* next;
		Object* obj;
		void (Object::*method)(Object*, _Param*);
	};
	_BaseEventHandlerObj* _lstDele;

	void Add(Object* obj, void (Object::*method)(Object*, _Param*))
	{
		bool found = false;
		_BaseEventHandlerObj* temp = _lstDele;
		while(temp != NULL)
		{
			if(temp->obj == obj && temp->method == method)
			{
				found = true;
				break;
			}
			temp = temp->next;
		}

		if(!found)
		{
			_BaseEventHandlerObj* newone = new _BaseEventHandlerObj();
			newone->obj = obj;
			newone->method = method;
			newone->next = NULL;

			if(_lstDele == NULL)
			{
				_lstDele = newone;
			}
			else
			{
				_BaseEventHandlerObj* last = _lstDele;
				while(last->next != NULL)
				{
					last = last->next;
				}
				last->next = newone;
			}
		}
		else {
			CC_TRACE((_T("Exist this event. this:%d, obj name:%s, obj:%d, method:%d"), this, (TCHAR*)obj->GetClassName(), obj, method));
		}
	}

	void Remove(Object* obj, void (Object::*method)(Object*, _Param*))
	{
		_BaseEventHandlerObj* temp = _lstDele;
		_BaseEventHandlerObj* last = temp;
		while(temp != NULL)
		{
			if(temp->method == method)
			{
				if(temp == _lstDele)
				{
					_lstDele = temp->next;
				}
				else
				{
					last->next = temp->next;
				}
				delete temp;
				temp = NULL;
				break;
			}
			last = temp;
			temp = temp->next;
		}
	}
};

#define CC_DEF_EVENTHANDLER(Handler_Name)	\
class Handler_Name##Handler : public _BaseEventHandler<Handler_Name##Args>	\
{	\
public:	\
	Handler_Name##Handler(){}	\
	Handler_Name##Handler(Object* obj, void (Object::*method)(Object*, Handler_Name##Args*))	\
		: _BaseEventHandler<Handler_Name##Args>(obj, method){}	\
};	\
typedef void (Object::* Handler_Name##Fun)(Object* sender, Handler_Name##Args* e);

//as sample
class EventHandler : public _BaseEventHandler<EventArgs>
{
public:
	EventHandler()
	{
	};
	EventHandler(Object* obj, void (Object::*method)(Object*, EventArgs*))
		: _BaseEventHandler<EventArgs>(obj, method)
	{
	};
};
typedef void (Object::* EventFun)(Object* sender, EventArgs* e);

CC_DEF_EVENTHANDLER(UICuesEvent);
CC_DEF_EVENTHANDLER(ControlEvent);
CC_DEF_EVENTHANDLER(DragBeginEvent);
CC_DEF_EVENTHANDLER(DragEndEvent);
CC_DEF_EVENTHANDLER(HelpEvent);
CC_DEF_EVENTHANDLER(InvalidateEvent);
CC_DEF_EVENTHANDLER(KeyEvent);
CC_DEF_EVENTHANDLER(KeyPressEvent);
CC_DEF_EVENTHANDLER(LayoutEvent);
CC_DEF_EVENTHANDLER(MouseEvent);
CC_DEF_EVENTHANDLER(PaintEvent);
CC_DEF_EVENTHANDLER(QueryContinueDragEvent);
CC_DEF_EVENTHANDLER(CancelEvent);
CC_DEF_EVENTHANDLER(GiveFeedbackEvent);
CC_DEF_EVENTHANDLER(ScrollEvent);

//for menu
CC_DEF_EVENTHANDLER(MeasureItemEvent);
CC_DEF_EVENTHANDLER(DrawItemEvent);

//for list item
CC_DEF_EVENTHANDLER(CompareItemEvent);
//CC_DEF_EVENTHANDLER(DeleteItemEvent);
CC_DEF_EVENTHANDLER(CharToItemEvent);
CC_DEF_EVENTHANDLER(VKeyToItemEvent);
//===========================================================cc::win::EventHandler
///////////////////////////////////////////////////////////////////////////////

////method type for Delegate
//typedef void (Object::* EventFun)(Object* sender, EventArgs* e);
//typedef void (Object::* UICuesEventFun)(Object* sender, UICuesEventArgs* e);
//typedef void (Object::* ControlEventFun)(Object* sender, ControlEventArgs* e);
//typedef void (Object::* DragEventFun)(Object* sender, DragEventArgs* e);
//typedef void (Object::* GiveFeedbackEventFun)(Object* sender, GiveFeedbackEventArgs* e);
//typedef void (Object::* HelpEventFun)(Object* sender, HelpEventArgs* e);
//typedef void (Object::* InvalidateEventFun)(Object* sender, InvalidateEventArgs* e);
//typedef void (Object::* KeyEventFun)(Object* sender, KeyEventArgs* e);
//typedef void (Object::* KeyPressEventFun)(Object* sender, KeyPressEventArgs* e);
//typedef void (Object::* LayoutEventFun)(Object* sender, LayoutEventArgs* e);
//typedef void (Object::* MouseEventFun)(Object* sender, MouseEventArgs* e);
//typedef void (Object::* PaintEventFun)(Object* sender, PaintEventArgs* e);
//typedef void (Object::* QueryContinueDragEventFun)(Object* sender, QueryContinueDragEventArgs* e);
//typedef void (Object::* CancelEventFun)(Object* sender, CancelEventArgs* e);
//
////for menu
//typedef void (Object::* DrawItemEventFun)(Object* sender, DrawItemEventArgs* e);
////typedef Delegate<Object, DrawItemEventFun> DrawItemEvent;
//typedef void (Object::* MeasureItemEventFun)(Object* sender, MeasureItemEventArgs* e);
////typedef Delegate<Object, MeasureItemEventFun> MeasureItemEvent;

//Delegate that has class object and a pointer of class method
//typedef Delegate2<Object, void, Object*, EventArgs*> Event;
//typedef Delegate2<Object, void, Object*, EventArgs*> Event;
//typedef Delegate2<Object, void, Object*, UICuesEventArgs*> UICuesEvent;
//typedef Delegate2<Object, void, Object*, ControlEventArgs*> ControlEvent;
//typedef Delegate2<Object, void, Object*, DragEventArgs*> DragEvent;
//typedef Delegate2<Object, void, Object*, GiveFeedbackEventArgs*> GiveFeedbackEvent;
//typedef Delegate2<Object, void, Object*, HelpEventArgs*> HelpEvent;
//typedef Delegate2<Object, void, Object*, InvalidateEventArgs*> InvalidateEvent;
//typedef Delegate2<Object, void, Object*, KeyEventArgs*> KeyEvent;
//typedef Delegate2<Object, void, Object*, KeyPressEventArgs*> KeyPressEvent;
//typedef Delegate2<Object, void, Object*, LayoutEventArgs*> LayoutEvent;
//typedef Delegate2<Object, void, Object*, MouseEventArgs*> MouseEvent;
//typedef Delegate2<Object, void, Object*, PaintEventArgs*> PaintEvent;
//typedef Delegate2<Object, void, Object*, QueryContinueDragEventArgs*> QueryContinueDragEvent;
//typedef Delegate2<Object, void, Object*, CancelEventArgs*> CancelEvent;
//up or low is all right,only need one
//typedef Delegate<Object, EventFun> Event;
//typedef Delegate<Object, UICuesEventFun> UICuesEvent;
//typedef Delegate<Object, ControlEventFun> ControlEvent;
//typedef Delegate<Object, DragEventFun> DragEvent;
//typedef Delegate<Object, GiveFeedbackEventFun> GiveFeedbackEvent;
//typedef Delegate<Object, HelpEventFun> HelpEvent;
//typedef Delegate<Object, InvalidateEventFun> InvalidateEvent;
//typedef Delegate<Object, KeyEventFun> KeyEvent;
//typedef Delegate<Object, KeyPressEventFun> KeyPressEvent;
//typedef Delegate<Object, LayoutEventFun> LayoutEvent;
//typedef Delegate<Object, MouseEventFun> MouseEvent;
//typedef Delegate<Object, PaintEventFun> PaintEvent;
//typedef Delegate<Object, QueryContinueDragEventFun> QueryContinueDragEvent;
//typedef Delegate<Object, CancelEventFun> CancelEvent;

class _APP_THREAD_STATE
{
public:
	_APP_THREAD_STATE() : m_lastSentMsg(0, 0, 0, 0)
	{
		m_pWndInit = NULL;
		m_hWndInit = NULL;
		m_hHookOldCbtFilter = NULL;
		m_hHookOldMsgFilter = NULL;

		m_fRegisteredClasses = 0;

		m_hInstRichEdit = NULL;
		m_hInstComCtl = NULL;

		m_msgCur.message = WM_NULL;
		m_nMsgLast = WM_NULL;
		m_lastSentMsg.HWnd = NULL;
		m_lastControl = NULL;
		//::GetCursorPos(&(m_ptCursorLast));
		//m_ptCursorLast = CPoint(0,0);

		m_pActiveWnd = NULL;
		m_pMainWnd = NULL;
	};
	virtual ~_APP_THREAD_STATE()
	{
		if(m_hInstRichEdit != NULL)
		{
			::FreeLibrary(m_hInstRichEdit);
		}
		if(m_hInstComCtl != NULL)
		{
			::FreeLibrary(m_hInstComCtl);
		}
	};

	// Control create, gray dialog hook, and other hook data
	Control* m_pWndInit;
	HWND m_hWndInit;
	HHOOK m_hHookOldCbtFilter;
	HHOOK m_hHookOldMsgFilter;
	EventArgs m_lastSentMsg;
	Control* m_lastControl;

	//flags for registered window classes
	DWORD m_fRegisteredClasses;

	HINSTANCE m_hInstRichEdit; // handle to RICHED32.DLL or RICHED20.DLL
	HINSTANCE m_hInstComCtl; // handle to COMCTL32.DLL

	// message pump for Run
	MSG m_msgCur;                   // current message
	//CPoint m_ptCursorLast;          // last mouse position
	UINT m_nMsgLast;                // last mouse message

	//current win
	HWND m_pActiveWnd;
	HWND m_pMainWnd;
};

class ImageList : public Object
{
protected:
	struct IconInfo
	{
		//this is normal icon be set
		HICON Hot;
		//this 50% grey icon
		HICON Gloom;
		//this is grey for disabled icon
		HICON Disabled;
		HICON Pushed;
		IconInfo()
		{
			Hot = NULL;
			Gloom = NULL;
			Disabled = NULL;
			Pushed = NULL;
		}
		~IconInfo()
		{
			if(this->Hot != NULL)
			{
				::DestroyIcon(this->Hot);
				this->Hot = NULL;
			}
			if(this->Gloom != NULL)
			{
				::DestroyIcon(this->Gloom);
				this->Gloom = NULL;
			}
			if(this->Disabled != NULL)
			{
				::DestroyIcon(this->Disabled);
				this->Disabled = NULL;
			}
			if(this->Pushed != NULL)
			{
				::DestroyIcon(this->Pushed);
				this->Pushed = NULL;
			}
		}
	};

public:
	ImageList();
	virtual ~ImageList();

	void SetIconSize(int cx, int cy);
	SIZE GetIconSize();

	int SetIcon(int nID, UINT nResourceID, IconType::Type iconType = IconType::Hot);
	int SetIcon(int nID, HICON hIcon, IconType::Type iconType = IconType::Hot);
	//not need destroy out GetIconXXX
	HICON GetIcon(int nID, IconType::Type iconType = IconType::Hot);
	HICON GetIconCopy(int nID, IconType::Type iconType = IconType::Hot);
	HICON GetIconAt(int nIndex, IconType::Type iconType = IconType::Hot);
	HICON GetIconCopyAt(int nIndex, IconType::Type iconType = IconType::Hot);

	int Count();
	void Clear();
	void Remove(int nID);

	HIMAGELIST CreateHIMAGELIST(int cx, int cy, IconType::Type iconType = IconType::Hot, DWORD mask = ILC_COLORDDB | ILC_MASK);
	static ImageList* PASCAL FromHandle(HIMAGELIST hImage);
	int GetIndex(int nID);
protected:
	SIZE _sizeIcon;
	Map<int, void*>* _mapIcon;
};

	//for menu&control(DWORD)
typedef struct tagUpdateFlag
{
	enum Type
	{
		//Update?? is set befor call DoUpdate()
		//or UpdateFlag is set for update interface for ownerdraw,ex:tabcontrol
		//UpdateFlag&_bIsHandleCreatedEnd is set after OnHandleCreated then call DoUpdate
		UpdateNone					= 0x00000001,
		UpdateFlag					= 0x00000001,
		UpdateText					= 0x00000002,
		UpdateIcon					= 0x00000004,
		UpdateEnabled				= 0x00000008,
		UpdateVisible				= 0x00000010,
		UpdateFont					= 0x00000020,
		UpdateContextMenu			= 0x00000040,
		UpdateTextColor				= 0x00000080,
		UpdateBackground			= 0x00000100,
		UpdateOuterStyle			= 0x00000200,
		UpdateInnerStyle			= 0x00000400,
		UpdateShortcut				= 0x00000800,
		UpdateTips					= 0x00001000,
		UpdateTipsExt				= 0x00002000,
		XXXXXXXXXXXXXX				= 0x00004000,
		UpdateStyle					= 0x00008000,
		UpdateChildUpdated			= 0x00010000,
		DUMY3						= 0x00020000,
		DUMY2						= 0x00040000,
		DUMY1						= 0x00080000,
	}; // enum UpdateFlag,used 0xFF,FF
}UpdateFlag;

//class for contain text and icon for menu and control class.
class ITextIcon : public Object
{
public:
	ITextIcon();
	virtual ~ITextIcon();

	const TCHAR* GetName() const;
	void SetName(const TCHAR* sName);
	virtual const TCHAR* GetText();
	//notice,control is not call this
	virtual void SetText(const TCHAR* sText);
	ImageList* GetImageList();
	int GetIconID();
	HICON GetIcon(IconType::Type iconType = IconType::Hot);
	void SetIcon(ImageList* imageList, int nIconID);
	virtual const TCHAR* GetTips();
	int GetTipsIconID();
	virtual void SetTips(const TCHAR* sTips, ImageList* imageList = NULL, int nIconID = -1);
	int GetTipsExtIconID();
	virtual const TCHAR* GetTipsExt();
	virtual void SetTipsExt(const TCHAR* sTips, ImageList* imageList = NULL, int nIconID = -1);
	//if ConfigName is NULL then use Name,but if is "" then do nothing while SetLang
	const TCHAR* GetConfigName() const;
	void SetConfigName(const TCHAR* sName);
	//keyArr is param address,but sParentHead is by value
	void SetLang(KeyArr& keyArr, cc::Str sParentHead);

	bool IsUpdate(UpdateFlag::Type updateFlag);
	TCHAR GetMnemonic();
	//call Update
	void Update(UpdateFlag::Type updateFlag = UpdateFlag::UpdateNone);

protected:
	TCHAR *_Name;
	TCHAR *_Text;
	TCHAR *_Tips;
	TCHAR *_TipsExt;
	TCHAR *_ConfigName;
	int _nIconID;
	int _nTipsIconID;
	int _nTipsExtIconID;
	ImageList* _pImageList;
	DWORD _nUpdateFlag; //flags for DoUpdate
	DWORD _Flags; //flags for only ccProc(this control and others)

	//call by Update
	virtual void DoUpdate();
	//keyArr is param address,but sThisHead is by value
	virtual void DoSetLang(KeyArr& keyArr, cc::Str sThisHead);
};

// Flag bits for the menu item's state.
typedef struct tagMenuStyles
{
	enum Type
	{
		String			= MF_STRING,
		Enabled			= MF_ENABLED,
		Break			= MF_SEPARATOR,
		Checked			= MF_CHECKED,
		Disabled		= MF_DISABLED|MF_GRAYED,
		OwnerDraw		= MF_OWNERDRAW,
#if(WINVER >= 0x0400)
		RadioCheck		= MFT_RADIOCHECK,
#else
		RadioCheck		= 0x00000200L,
#endif /* WINVER >= 0x0400 */
		Selected        = MF_MOUSESELECT,
	}; // enum InnerStyle,used 0x000FFFFF
}MenuStyles;

typedef struct tagMenuInnerStyle
{
	enum Type
	{
		//while StatusItem, equals to Contents
		MdiList						= 0x00000010,
		Visible						= 0x00000020,
		AllOwnerDraw				= 0x00000040,
		HandleCreated				= 0x00000100,
		ShowShortcut				= 0x00000200,
		TY_MENU						= 0x00002000,
		TY_MENUITEM					= 0x00004000,
		TY_TOOLITEM					= 0x00010000,
		TY_CONTEXTMENU				= 0x00020000,
		TY_MAINMENU					= 0x00040000,
	};
}MenuInnerStyle;

typedef struct tagMenuType
{
	enum Type
	{
		MENU,
		MENUITEM,
		TOOLITEM,
		CONTEXTMENU,
		MAINMENU,
	};
}MenuType;

#define CC_FLAG_MENU_START					0x00000001
#define CC_FLAG_MENU_NEEDCREATED			CC_FLAG_MENU_START
#define CC_FLAG_MENU_END					CC_FLAG_MENU_START <<1
class Menu : public ITextIcon
{
public:
	HMENU m_hMenu;

public:
	Menu();
	virtual ~Menu();
	ContextMenu* GetContextMenu() const;
	MainMenu* GetMainMenu() const;

	virtual bool GetShowShortcut();
	virtual void SetShowShortcut(bool value);
	virtual const TCHAR* GetShortcutText();
	virtual Shortcut::Type GetShortcut();
	virtual void SetShortcut(Shortcut::Type shortcut);
	MenuType::Type GetMenuType();

	bool GetIsParent() const;
	bool GetIsNeedUpdate();
	int ItemAdd(Menu* menu, int nIndex = -1);
	Menu* ItemRemove(int nIndex);
	Menu* ItemGet(int nIndex) const;

	int ItemGetIndex(Menu* menu) const;
	void ItemSetIndex(Menu* menu, int index);

	void ItemSwap(int fromIndex, int toIndex);
	int ItemCount() const;
	void ItemClear();

	virtual bool ProcessCmdKey(MSG* baseArgs, UINT_PTR keyData);
	LRESULT FindHotKey(UINT nChar);
	static Menu* PASCAL FromHandle(HMENU hMenu);

	bool IsInnerStyle(MenuInnerStyle::Type innerStyle) const;
	void SetInnerStyle(MenuInnerStyle::Type innerStyle, bool value, bool needUpdate = false);
protected:
	List<Menu*>* _lstMenus;
	//bool _bNeedCreate;
	MenuType::Type _eMenuType;

	Menu* _Parent;
	DWORD _uMenuStyle;
	DWORD _InnerStyle;
	Shortcut::Type _eShortcut;

	virtual void CreateMenuHandle();
	virtual void DestroyMenu();
	//20081025,default needUpdate to true
	void SetStyle(MenuStyles::Type style, bool value, bool needUpdate = true);
	bool HasStyle(MenuStyles::Type style);
	virtual void DoUpdate();
	//keyArr is param address,but sThisHead is by value
	virtual void DoSetLang(KeyArr& keyArr, cc::Str sThisHead);
};

class MenuItem : public Menu
{
public:
	EventHandler Click;
	DrawItemEventHandler DrawItem;
	MeasureItemEventHandler MeasureItem;
	EventHandler Popup;
	EventHandler Select;

public:
	MenuItem();
	virtual ~MenuItem();

	bool GetBreak();
	void SetBreak(bool value);
	bool GetChecked();
	void SetChecked(bool value);
	bool GetEnabled();
	void SetEnabled(bool value);
	void SetIndex(int index);
	int GetIndex();

	void SetSelected(bool value);
	bool GetSelected();
	bool GetMdiList();
	void SetMdiList(bool value);

	bool GetOwnerDraw();
	void SetOwnerDraw(bool value);

	Menu* GetParent();

	bool GetRadioCheck();
	void SetRadioCheck(bool value);
	bool GetVisible();
	void SetVisible(bool value);

	int GetMenuID();

	//invoid to use (friend),define PerformXXX
	void PerformClick(EventArgs *e);
	void PerformDrawItem(DrawItemEventArgs *e);
	void PerformMeasureItem(MeasureItemEventArgs *e);
	void PerformPopup(EventArgs *e);
	void PerformSelect(EventArgs *e);

protected:
	virtual void OnClick(EventArgs *e);
	virtual void OnDrawItem(DrawItemEventArgs *e);
	virtual void OnMeasureItem(MeasureItemEventArgs *e);
	virtual void OnPopup(EventArgs *e);
	virtual void OnSelect(EventArgs *e);
};

class Timer : public Object
{
public:
	EventHandler Tick;

public:
	Timer();
	virtual ~Timer();

	void SetEnabled(bool isEnabled);
	bool GetEnabled() const;
	void SetInterval(int nInterval);
	int GetInterval() const;
	void Start();
	void Stop();

protected:
	static void CALLBACK StaticProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);

	virtual void OnTick(EventArgs *e);

protected:
	int _isEnabled; //1:isEnabled
	int _nInterval;
	int _nID;
};

//namespace System.Threading
typedef struct tagThreadState
{
	enum Type
	{
		Running = 0x00000000,
		StopRequested = 0x00000001,
		//SuspendRequested = 0x00000002,
		//Background = 0x00000004,
		Unstarted = 0x00000008,
		Stopped = 0x00000010,
		//WaitSleepJoin = 0x00000020,
		//Suspended = 0x00000040,
		//AbortRequested = 0x00000080,
		//Aborted = 0x00000100,
	};
}ThreadState;

class Thread : public Object
{
public:
	EventHandler ThreadProc;

public:
	//Constructors
	Thread();
	//destructor
	virtual ~Thread();

	// only valid while running
	HANDLE m_hThread;       // this thread's HANDLE
	operator HANDLE() const;
	DWORD m_nThreadID;      // this thread's ID

	int GetThreadPriority();
	BOOL SetThreadPriority(int nPriority);
	DWORD SuspendThread();
	DWORD ResumeThread();
	BOOL PostThreadMessage(UINT message, WPARAM wParam, LPARAM lParam);

	bool Start(DWORD dwCreateFlags = 0, UINT nStackSize = 0, LPSECURITY_ATTRIBUTES lpSecurityAttrs = 0);
	void SendStopRequested();
	bool IsStopRequested();
	//wait until Terminate
	void Terminate(int millisecondsTimeout = 0);
	bool IsAlive();

	static bool StartThread(Object* obj, void (Object::*method)(Object*, EventArgs*), WPARAM wParam = NULL, LPARAM lParam = NULL);

protected:
	int _nThreadState;

	static unsigned long _stdcall ThreadEntry(void *pParam);
	static unsigned long _stdcall ThreadEntry_static(void *pParam);
	virtual void CallThreadProc(EventArgs* e);
};

class DrawActorConfig : public ObjectConfig
{
public:
	static int nMenuMarginLeft;
	static int nMenuMarginRight;
	static int nMenuMarginShortcut;
	static int nMenuIconWH;
	static int nMenuMinWidth;
	static int nMenuMaxWidth;
	static int nMenuBreakHeight;
	static double nDisabledIconRScale;
	static double nDisabledIconGScale;
	static double nDisabledIconBScale;
	static COLORREF corMenuBackground;
	static COLORREF corMenuTextground;
	static COLORREF corMenuHotBackColor;
	static COLORREF corMenuHotTextColor;
	static COLORREF corMenuDisabledTextground;
	static COLORREF corFrameColor;
};
#define DRAWACTOR_THEMEBRUSHBACK_COUNT 5
//#define DRAWACTOR_GRADIBRUSHBACK_COUNT 32
class DrawActor : public Object
{
public:
	DrawActor();
	virtual ~DrawActor();

	virtual void DrawSpecialChar(HDC hdc, RECT rc, TCHAR tc, bool bBold, COLORREF color = -1, long fontHeight = -1, UINT nFlag = DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	virtual HFONT GetMenuFont(bool isFourceRefresh = false);
	void OnThemeChanged();
	//need destroy icon out this scope
	HICON CreateGloomIcon(HICON hIcon, int nScale = 50);
	//need destroy icon out this scope
	HICON CreateDisabledIcon(HICON hIcon);
	//need destroy icon out this scope
	HICON ResizeIcon(HICON hIconOld, int cx, int cy);
	//need DeleteObject(hbmp) out of this
	HBITMAP ResizeBmp(HBITMAP hBmpOld, int cx, int cy, bool isBitsFromScreenDC = false);
	void DrawFocusFrame(HDC hDC, RECT rect);
	void DrawFocusFrame(HDC hDC, int nLeft, int nTop, int nWidth, int nHeight);
	void DrawIcon(HDC hDC, int xLeft, int yTop, HICON hIcon, int cxWidth, int cyWidth, HBRUSH hBrush = NULL, UINT uFlags = DI_NORMAL);
	void MeasureMenu(MenuItem* item, MeasureItemEventArgs *e);
	void DrawMenu(MenuItem* item, DrawItemEventArgs *e);
	void DrawMenuTitle(MenuItem* item, DrawItemEventArgs *e);

	SIZE MeasureControl(Control* control, int ctlHeight, DWORD dwStyle = 0);
	void DrawControl(Control* control, DrawItemEventArgs *e);
	SIZE MeasureToolItem(ToolItem* control, int ctlHeight, DWORD dwStyle = 0);
	void DrawToolItem(ToolItem* control, DrawItemEventArgs *e);
	SIZE MeasureStatusItem(StatusItem* control, int ctlHeight, DWORD dwStyle = 0);
	void DrawStatusItem(StatusItem* control, DrawItemEventArgs *e);
	//if has OuterStyle::Break then draw as "---- TEXT ----"
	void DrawPanel(Control* control, DrawItemEventArgs *e);
	void DrawThemeBackground(HDC hDC, const RECT *pRect, const RECT *pClipRect);
	void DrawGradient(HDC hDC, RECT rect, COLORREF corFrom, COLORREF corTo, bool isVertical = false);
	//cannot delete Brush out this class
	HBRUSH ThemeBackgroundBrush(Control* control);

	//if icon then set back use of corBack
	HBITMAP GetBitmap(const TCHAR* szFile, COLORREF corBack = CLR_INVALID);
	//if icon then set back use of corBack
	HBITMAP GetBitmap(UINT nResID, const TCHAR *resType, COLORREF corBack = CLR_INVALID);
	//need release HBITMAP out this,if hDC is NULL, then from screen
	//if hDCDes != NULL, then draw to hDCDes
	static HBITMAP GetGradientBmp(COLORREF corFrom, COLORREF corTo, int sizeW, int sizeH, bool isVertical, HDC hDC = NULL, HDC hDCDes = NULL, int nFromX = 0, int nFromY = 0);
	//need release HBITMAP out this
	static HBITMAP CopyBitmap(HBITMAP hSrcBitmap);
	//need release HBITMAP out this,if hDC is NULL, then from screen
	//0	The number of bits-per-pixel is specified or is implied by the JPEG or PNG format.
	//1 - 2 colors
	//4 - 16 colors
	//8 - 256 colors
	//(16 - 2^16=65536 colors(high color) ?)
	//24 - 2^32 colors(true color)
	//32 - 2^32 colors(true color)
	static HBITMAP GetBmpFromHDC(int nFromX, int nFromY, int nFromW, int nFromH, int sizeW, int sizeH, HDC hDC = NULL, int nBitCount = -1);
	//if sizeW or sizeH < 0 then orignal size; if corBack=CLR_INVALID then not clear back
	static HBITMAP Icon2Bmp(HICON hicon, COLORREF corBack, int sizeW = -1, int sizeH = -1, HDC hDC = NULL);
	static void DotLine(HDC hDC, int fromX, int fromY, int toX, int toY, float jumpX, float jumpY, COLORREF color, bool isTurnColor);
	static void InvertBorder(int x, int y, int cx, int cy, HDC hDC = NULL, int nPenWidth = 2);
	static void TransparentBlt(
		HDC hdcDest,            // handle to Dest DC
		int nXOriginDest,       // x-coord of destination upper-left corner
		int nYOriginDest,       // y-coord of destination upper-left corner
		int nWidthDest,         // width of destination rectangle
		int nHeightDest,        // height of destination rectangle
		HDC hdcSrc,             // handle to source DC
		int nXOriginSrc,        // x-coord of source upper-left corner
		int nYOriginSrc,        // y-coord of source upper-left corner
		int nWidthSrc,          // width of source rectangle
		int nHeightSrc,         // height of source rectangle
		COLORREF crTransparent  // color to make transparent
	);

protected:
	HFONT hFontMenu;
	//300X250; 300X450; 300X650; 300X900; MAX
	HBRUSH hbrThemeBack[DRAWACTOR_THEMEBRUSHBACK_COUNT];
	//HBRUSH hbrGradientFill[DRAWACTOR_GRADIBRUSHBACK_COUNT];

	virtual HBITMAP DoGetBitmap(const TCHAR* szFile, COLORREF corBack);
	virtual HBITMAP DoGetBitmap(UINT nResID, const TCHAR *resType, COLORREF corBack);
	virtual HICON DoCreateGloomIcon(HICON hIcon, int nScale);
	virtual HICON DoCreateDisabledIcon(HICON hIcon);
	virtual HICON DoResizeIcon(HICON hIconOld, int cx, int cy);
	virtual HBITMAP DoResizeBmp(HBITMAP hBmpOld, int cx, int cy, bool isBitsFromScreenDC);
	virtual void DoDrawFocusFrame(HDC hDC, int nLeft, int nTop, int nWidth, int nHeight);
	virtual void DoDrawIcon(HDC hDC, int xLeft, int yTop, HICON hIcon, int cxWidth, int cyWidth, HBRUSH hBrush, UINT uFlags);
	virtual void DoMeasureMenu(MenuItem* item, MeasureItemEventArgs *e);
	virtual void DoDrawMenu(MenuItem* item, DrawItemEventArgs *e);
	virtual void DoDrawMenuTitle(MenuItem* item, DrawItemEventArgs *e);

	virtual SIZE DoMeasureControl(Control* control, int ctlHeight, DWORD dwStyle);
	virtual void DoDrawControl(Control* control, DrawItemEventArgs *e);
	virtual SIZE DoMeasureToolItem(ToolItem* control, int ctlHeight, DWORD dwStyle);
	virtual void DoDrawToolItem(ToolItem* control, DrawItemEventArgs *e);
	virtual SIZE DoMeasureStatusItem(StatusItem* control, int ctlHeight, DWORD dwStyle);
	virtual void DoDrawStatusItem(StatusItem* control, DrawItemEventArgs *e);
	virtual void DoDrawPanel(Control* control, DrawItemEventArgs *e);
	virtual void DoDrawThemeBackground(HDC hDC, const RECT *pRect, const RECT *pClipRect);
	virtual void DoDrawGradient(HDC hDC, RECT rect, COLORREF corFrom, COLORREF corTo, bool isVertical);
	virtual HBRUSH DoThemeBackgroundBrush(Control* control);
};

} //namespace win
} //namespace cc

/////////////////////////////////////////////////////////////////////////////
// General DLL Version Helpers

#define CC_HRESULT_FROM_WIN32(x) ((HRESULT)(x) <= 0 ? ((HRESULT)(x)) : ((HRESULT) (((x) & 0x0000FFFF) | (FACILITY_WIN32 << 16) | 0x80000000)))
inline HRESULT CC_HresultFromLastError() throw()
{
	DWORD dwErr = ::GetLastError();
	return CC_HRESULT_FROM_WIN32(dwErr);
}

#pragma warning(push)
#pragma warning(disable : 4191)	// 'type cast' : unsafe conversion from 'FARPROC' to 'DLLGETVERSIONPROC'

inline HRESULT CC_GetDllVersion(HINSTANCE hInstDLL, DLLVERSIONINFO* pDllVersionInfo)
{
	CC_ASSERT(!::IsBadWritePtr(pDllVersionInfo, sizeof(DLLVERSIONINFO)));

	// We must get this function explicitly because some DLLs don't implement it.
	DLLGETVERSIONPROC pfnDllGetVersion = (DLLGETVERSIONPROC)::GetProcAddress(hInstDLL, "DllGetVersion");
	if(pfnDllGetVersion == NULL)
	{
		return E_NOTIMPL;
	}

	return (*pfnDllGetVersion)(pDllVersionInfo);
}

#pragma warning(pop)

inline HRESULT CC_GetDllVersion(LPCTSTR lpstrDllName, DLLVERSIONINFO* pDllVersionInfo)
{
	HINSTANCE hInstDLL = ::LoadLibrary(lpstrDllName);
	if(hInstDLL == NULL)
	{
		return CC_HresultFromLastError();
	}
	HRESULT hRet = CC_GetDllVersion(hInstDLL, pDllVersionInfo);
	::FreeLibrary(hInstDLL);
	return hRet;
}

// Common Control Versions:
//   Win95/WinNT 4.0    maj=4 min=00
//   IE 3.x             maj=4 min=70
//   IE 4.0             maj=4 min=71
//   IE 5.0             maj=5 min=80
//   Win2000            maj=5 min=81
inline HRESULT CC_GetCommCtrlVersion(LPDWORD pdwMajor, LPDWORD pdwMinor)
{
	CC_ASSERT(!::IsBadWritePtr(pdwMajor, sizeof(DWORD)) && !::IsBadWritePtr(pdwMinor, sizeof(DWORD)));

	DLLVERSIONINFO dvi;
	memset(&dvi, 0, sizeof(dvi));
	dvi.cbSize = sizeof(dvi);

	HRESULT hRet = CC_GetDllVersion(_T("comctl32.dll"), &dvi);

	if(SUCCEEDED(hRet))
	{
		*pdwMajor = dvi.dwMajorVersion;
		*pdwMinor = dvi.dwMinorVersion;
	}
	else if(hRet == E_NOTIMPL)
	{
		// If DllGetVersion is not there, then the DLL is a version
		// previous to the one shipped with IE 3.x
		*pdwMajor = 4;
		*pdwMinor = 0;
		hRet = S_OK;
	}

	return hRet;
}

// Shell Versions:
//   Win95/WinNT 4.0                                maj=4 min=00
//   IE 3.x, IE 4.0 without Web Integrated Desktop  maj=4 min=00
//   IE 4.0 with Web Integrated Desktop             maj=4 min=71
//   IE 4.01 with Web Integrated Desktop            maj=4 min=72
//   Win2000                                        maj=5 min=00
inline HRESULT CC_GetShellVersion(LPDWORD pdwMajor, LPDWORD pdwMinor)
{
	CC_ASSERT(!::IsBadWritePtr(pdwMajor, sizeof(DWORD)) && !::IsBadWritePtr(pdwMinor, sizeof(DWORD)));

	DLLVERSIONINFO dvi;
	memset(&dvi, 0, sizeof(dvi));
	dvi.cbSize = sizeof(dvi);
	HRESULT hRet = CC_GetDllVersion(_T("shell32.dll"), &dvi);

	if(SUCCEEDED(hRet))
	{
		*pdwMajor = dvi.dwMajorVersion;
		*pdwMinor = dvi.dwMinorVersion;
	}
	else if(hRet == E_NOTIMPL)
	{
		// If DllGetVersion is not there, then the DLL is a version
		// previous to the one shipped with IE 4.x
		*pdwMajor = 4;
		*pdwMinor = 0;
		hRet = S_OK;
	}

	return hRet;
}

const TCHAR* __stdcall CC_TurnWndMsg(const TCHAR* ctlClass, UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0);
void __stdcall CC_LogWndMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, const TCHAR* ctlClass, const TCHAR* ctlName);
long __stdcall CC_InitCommonControls(LPINITCOMMONCONTROLSEX lpInitCtrls, LONG fToRegister);
long __stdcall CC_InitCommonControls(HINSTANCE hInst, LPINITCOMMONCONTROLSEX lpInitCtrls, LONG fToRegister);
bool __stdcall CC_DeferRegisterClass(long fToRegister);
const TCHAR* CC_GetShortcutText(cc::win::Shortcut::Type shortcut);

//below is implemented at cc.win.form.cpp
cc::win::_APP_THREAD_STATE* CC_APP_GetThreadStateData();
cc::Map<HMENU, cc::win::Menu*>* CC_APP_GetMapHMENU();
cc::Map<HWND, cc::win::Control*>* CC_APP_GetMapHWND();
cc::Map<int, cc::win::Timer*>* CC_APP_GetMapTimer();
cc::win::Control* CC_APP_GetMainForm();
cc::win::Control* CC_APP_GetActiveForm();
cc::win::DrawActor* CC_APP_GetDrawActor();

#if defined(_DEBUG_) || defined(_DESIGN)
extern cc::win::Control* CC_Design_Hotcontrol;
extern cc::win::Control* CC_Design_Hotcontrol_new;

//next is implemented at cc.win.Design.hpp(include in cc.win.Module.cpp)
//!!!import,if use this,then many class is included(compiled) into your exe
cc::win::FloatContainer* CC_APP_CreateDesignBar();
cc::win::Control* CC_APP_CreateControl(int nClassID);
bool CC_APP_CreateSource(cc::win::Control* control, cc::Str& sTxt, cc::Str& sVarList, cc::Str sThisHead);
void CC_APP_DrawFocusFrame(cc::win::Control* control);
#endif
#if defined(_DEBUG_) || defined(_DESIGN) || defined(_DESIGNRUN)
cc::win::Control* CC_APP_CreateControl(cc::Str sClassName);
#endif

#endif //#ifndef CC_WIN_BASE_H_
