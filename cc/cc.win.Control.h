// cc.win.Control.h
// The C# like Controls class
// Author: Shu.KK
// 2006-01-01
/*

WPARAM MAKEWPARAM(WORD wLow, WORD wHigh);
LRESULT MAKELRESULT(WORD wLow, WORD wHigh);
DWORD MAKELONG(WORD wLow, WORD wHigh);
WORD MAKEWORD(BYTE bLow, BYTE bHigh);
WORD HIWORD(DWORD dwValue);
WORD LOWORD(DWORD dwValue);

int GET_X_LPARAM(LPARAM lParam);
int GET_Y_LPARAM(LPARAM lParam);

*/
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef CC_WIN_CONTROL_H_
#define CC_WIN_CONTROL_H_

#include "cc.win.IECtrl.h"
#include "cc.win.Base.h"

#ifndef GET_X_LPARAM 
#define GET_X_LPARAM(lp)   ((int)(short)LOWORD(lp)) 
#endif 
#ifndef GET_Y_LPARAM 
#define GET_Y_LPARAM(lp)   ((int)(short)HIWORD(lp)) 
#endif 

#ifndef IDC_HAND
#define IDC_HAND            IDC_ARROW
#endif

#ifndef WS_EX_NOINHERITLAYOUT
#define WS_EX_NOINHERITLAYOUT   0x00100000L // Disable inheritence of mirroring by children
#define WS_EX_LAYOUTRTL         0x00400000L // Right to left mirroring
#endif


namespace cc
{
namespace win
{

class WndClassName : public Object
{
public:
	static const TCHAR* Control;
	static const TCHAR* ToolTip;
	static const TCHAR* ToolItem;
	static const TCHAR* Label;
	static const TCHAR* Button;
	static const TCHAR* SpinButton;
	static const TCHAR* ListBox;
	static const TCHAR* ComboBox;
	static const TCHAR* ComboBoxEx;
	static const TCHAR* TextBox;
	static const TCHAR* RichTextBox;
	static const TCHAR* ScrollBar;
	static const TCHAR* ProgressBar;
	static const TCHAR* ListView;
	static const TCHAR* TreeView;
	static const TCHAR* DateTimePicker;
	static const TCHAR* MonthCalendar;
	static const TCHAR* Splitter;
	static const TCHAR* Container;
	static const TCHAR* FloatContainer;
	static const TCHAR* DockBar;
	static const TCHAR* ToolBar;
	static const TCHAR* ScrollContainer;
	static const TCHAR* ReBar;
	static const TCHAR* StatusBar;
	static const TCHAR* UserControl;
	static const TCHAR* MDIChild;
	static const TCHAR* Form;
	static const TCHAR* IEControl;
	static const TCHAR* TabControl;
	static const TCHAR* TabControlEx;
	static const TCHAR* Dialog32770;
};

class MainMenu : public Menu
{
public:
	HWND m_hWndParent;

public:
	MainMenu();
	virtual ~MainMenu();
	virtual void CreateControl();

	bool GetAllOwnerDraw();
	void SetAllOwnerDraw(bool value);

protected:
};

class ContextMenu : public Menu
{
public:
	EventHandler Popup;

	ContextMenu();
	virtual ~ContextMenu();

	void Show(Control* control, LONG x, LONG y);
	void Show(Control* control, POINT pos);
	void Show(HWND m_hWnd, LONG x, LONG y);

	bool GetAllOwnerDraw();
	void SetAllOwnerDraw(bool value);

protected:
	virtual void OnPopup(EventArgs *e);

	UINT _uTrackPopupMenuFlag;
	Control* _pPopupWin;
};

//for control
//enum OuterStyle
// 1111 1111 1111 1111 1111 1111 1111 1111
//                -----------------------
//                           |----------------> Control(20bits)
//        -------
//            |-------------------------------> Other Controls(6bits)
// -------
//    |---------------------------------------> EndUser(6bits)
typedef struct tagOuterStyle
{
	enum Type
	{
		//while StatusItem, equals to Contents
		AutoSize					= 0x00000001,
		Break						= 0x00000002,
		Blank						= 0x00000004,
		//only used by StatusItem, if AutoSize is set,then ignore AutoSizeSprint
		//but if add other controls into statusbar with this flag,then has same action
		AutoSizeSprint				= 0x00000008,
		//show ContextMenu button at right(must GetContextMenu() != NULL or _pFloater != NULL)
		//toolitem(statusitem):if also has PushButton,then show it while onclick
		ContextMenuButton			= 0x00000010,
		//toolitem:click once push,click again pop, if ContextMenuButton then not see this
		PushButton					= 0x00000020,
		//for item of ReBar to be floatble
		Floating					= 0x00000040,
		//while dragover, if Dropable then call it's OnDragMove
		Dropable					= 0x00000080,
		//while parent is sizing,child will not refresh parent
		Sizing						= 0x00000100,
		//if set,will auto save/load config to/from ini
		AutoConfig					= 0x00000200,
		//only used by toolitem(statusitem or ownerdraw control)
		//for toolitem(control) while 1 then show Edge always(even not mouse on)
		//for panel to has group rect
		AlwaysEdge					= 0x00000400,
		//only used by toolitem(statusitem or ownerdraw control)
		//for toolitem while 1 then not show Edge always(even with mouse on)
		//if AlwaysEdge and AlwaysNoEdge both 1, then ignore AlwaysEdge
		//for statusitem,for default not set AlwaysNoEdge,
		//so will draw BDR_RAISEDINNER&BDR_RAISEDOUTER if Break
		//but if WS_TABSTOP then DrawFocusRect
		AlwaysNoEdge				= 0x00000800,
		OuterEdge					= 0x00001000,
		//save brush for childens(transparent while with xp DrawThemeBackground)
		//also if control want paint self's back,should has OuterStyle::TransparentTop
		TransparentTop				= 0x00002000,
		//for toolitem,while mouse on,then no Transparent
		TransparentOffMouseOn		= 0x00004000,
		//use parent bursh to drawback
		TransparentParentBrush		= 0x00008000,
		//return HOLLOW_BRUSH at OnCtlColorEdit(like label,not used current)
		TransparentHollowBrush		= 0x00010000,
		//for child not erase back while need(like ie)
		TransparentNoEraseBack		= 0x00020000,
		//for toolitem to has Transparent while at toolbar or in panel
		//notice while controls have TransparentParentErase then need remove CS_PARENTDC
		TransparentParentErase		= 0x00040000,
		//if 1 then Stretch icon to size(should has icon and not DrawNoIcon).
		DrawIconStretch				= 0x00080000,
		//draw icon up of text.if also DrawIconStretch or DrawNoIcon then ignore this.
		DrawIconTop					= 0x00100000,
		//if has DrawNoIcon and DrawNoText,then draw nothing
		DrawNoIcon					= 0x00200000,
		DrawNoText					= 0x00400000,
		reserve6					= 0x00800000,
		reserve5					= 0x01000000,
		reserve4					= 0x02000000,
		reserve3					= 0x04000000,
		reserve2					= 0x08000000,
		reserve1					= 0x10000000,
		PrivateFlag2				= 0x20000000,
		//next is for other controls(used by this ccProc)
		//used by toolitem(statusitem)
		//for toolitem while 1 then Do toolitem's ClickEvent even has _pContextMenu
		//and while set e->Handled in toolitem's ClickEvent,then not call first item's ClickEvent of _pContextMenu
		//used by TabPage
		//if set this flage,then no right DrawHeadIcon at TabControlEx
		//used by TextBox
		//if set this flage,the select all text while GotFocus
		PrivateFlag1				= 0x40000000,
		PrivateFlag0				= 0x80000000,
	};
}OuterStyle;

typedef struct tagToolItemOuterStyle : tagOuterStyle
{
	enum Type
	{
		//always do toolitem's onclick even has _pContextMenu
		//if not set e->Handled at onclick,then goon _pContextMenu's click
		AlwaysDoClick					= OuterStyle::PrivateFlag1,
	};
}ToolItemOuterStyle;

typedef struct tagTextBoxOuterStyle : tagOuterStyle
{
	enum Type
	{
		//if set this flage,the select all text while GotFocus
		SelAllOnFocus				= OuterStyle::PrivateFlag1,
	};
}TextBoxOuterStyle;

typedef struct tagTabControlExOuterStyle : tagOuterStyle
{
	enum Type
	{
		//if set this flage,then no right DrawHeadIcon at TabControlEx
		NoRightHeadIcon					= OuterStyle::PrivateFlag1,
	};
}TabControlExOuterStyle;

typedef struct tagInnerStyle
{
	enum Type
	{
		//Background??? only for control with TransparentTop
		BackgroundColor				= 0x00000001,
		BackgroundBrush				= 0x00000002,
		//if BackgroundBmp,then stretch(others use CreatePatternBrush to turn to BackgroundBrush)
		BackgroundBmp				= 0x00000004,
		BackgroundTheme				= 0x00000008,
		//draw gradient from HotBackColor to Background. not change for mouseon or off
		//if has this,then not see any Background???.
		BackgroundGradient			= 0x00000010,
		XXXXX						= 0x00000020,
		//for form or float dockbar
		MaxButtonDisable			= 0x00000040,
		CloseButtonDisable			= 0x00000080,
		MinButtonDisable			= 0x00000100,
		//set if need WM_MOUSELEAVE or WM_MOUSEHOVER
		TrackMouse					= 0x00000200,
		//for toolitem while pushed
		//for CheckBox while checked,or RadioButton while selected
		Pushed						= 0x00000400,
		//context menu is showing,use at toolitem,TabControlEx
		ContextMenuShowing			= 0x00000800,
		//will add child control as MDI
		IsMdiContainer				= 0x00001000,
		XXXXXXXXXX1					= 0x00002000,
		XXXXXXXXXX2					= 0x00004000,
		XXXXXXXXXX3					= 0x00008000,
		//not jump out while press tab even at last control
		TabLoopInner				= 0x00010000,
		Dragging					= 0x00020000,
		DeliverFormMessage			= 0x00040000,
		//pass contextmenu msg to parent
		TranceContextMenuUp			= 0x00080000,
		//while Container set with this then CreateDialogIndirect from DLGTEMPLATE
		CreateDialog				= 0x00100000,
		IsDirty						= 0x00200000,
		InnerStyle9					= 0x00400000,
		InnerStyle8					= 0x00800000,
		InnerStyle7					= 0x01000000,
		InnerStyle6					= 0x02000000,
		InnerStyle5					= 0x04000000,
		InnerStyle4					= 0x08000000,
		InnerStyle3					= 0x10000000,
		InnerStyle2					= 0x20000000,
		InnerStyle1					= 0x40000000,
		InnerStyle0					= 0x80000000,
	};
}InnerStyle;

//private flag for self.not update by outer.
typedef struct tagInner2Style
{
	enum Type
	{
		Visible						= 0x00000001,
		BackStyle91					= 0x00000002,
		//while get WM_CREATE,soon set this flage.also this time win is created OK
		HandleCreated				= 0x00000004,
		//while get WM_CREATE,run all befor out,set this flage.also this time child is created
		HandleCreatedEnd			= 0x00000008,
		BackgroundDestroyBrush		= 0x00000010,
		BackgroundDestroyBmp		= 0x00000020,
		OffsetNewline				= 0x00000040,
		//if BackgroundGradient,then see if Vertical
		GradientVertical			= 0x00000080,
		IsMouseEnter				= 0x00000100,
		IsMouseTimer				= 0x00000200,
		//if OuterStyle::TransparentParentBrush|OuterStyle::TransparentParentErase and this then InvalidateRect while WM_HSCROLL&WM_VSCROLL
		IsScrollRepaint				= 0x00000400,
		//if is SubclassWindow,then not destroy hWnd will be deleted
		SubclassWindow				= 0x00000800,
		HandleDestroyed				= 0x00001000,
		BackStyle18					= 0x00002000,
		//if set AutoConfig, but not this,then only index but no listdata for ComboBox&CheckListBox
		AutoConfigListData			= 0x00004000,
		BackStyle20					= 0x00008000,
		WindowPosFlags0				= 0x00010000,
		WindowPosFlagsNoAct			= 0x00020000,
		BackStyle23					= 0x00040000,
		BackStyle24					= 0x00080000,
		BackStyle25					= 0x00100000,
		BackStyle26					= 0x00200000,
		BackStyle27					= 0x00400000,
		BackStyle28					= 0x00800000,
		BackStyle29					= 0x01000000,
		BackStyle30					= 0x02000000,
		BackStyle31					= 0x04000000,
		BackStyle4					= 0x08000000,
		BackStyle3					= 0x10000000,
		BackStyle2					= 0x20000000,
		BackStyle1					= 0x40000000,
		BackStyle0					= 0x80000000,
	};
}Inner2Style;

//this is reserve for end user(developer),used like next:
//control->SetStyle(StyleType::EndUser, EndUserStyle::EndUser01, EndUserStyle::EndUser02);
typedef struct tagEndUserStyle
{
	enum Type
	{
		EndUser01					= 0x00000001,
		EndUser02					= 0x00000002,
		EndUser03					= 0x00000004,
		EndUser04					= 0x00000008,
		EndUser05					= 0x00000010,
		EndUser06					= 0x00000020,
		EndUser07					= 0x00000040,
		EndUser08					= 0x00000080,
		EndUser09					= 0x00000100,
		EndUser10					= 0x00000200,
		EndUser11					= 0x00000400,
		EndUser12					= 0x00000800,
		EndUser13					= 0x00001000,
		EndUser14					= 0x00002000,
		EndUser15					= 0x00004000,
		EndUser16					= 0x00008000,
		EndUser17					= 0x00010000,
		EndUser18					= 0x00020000,
		EndUser19					= 0x00040000,
		EndUser20					= 0x00080000,
		EndUser21					= 0x00100000,
		EndUser22					= 0x00200000,
		EndUser23					= 0x00400000,
		EndUser24					= 0x00800000,
		EndUser25					= 0x01000000,
		EndUser26					= 0x02000000,
		EndUser27					= 0x04000000,
		EndUser28					= 0x08000000,
		EndUser29					= 0x10000000,
		EndUser30					= 0x20000000,
		EndUser31					= 0x40000000,
		EndUser32					= 0x80000000,
	};
}EndUserStyle;

#define CC_CONTROL_STYLETYPE_SIZE 9
typedef struct tagStyleType
{
	enum Type
	{
		Style				= 0,
		ExStyle				= 1,
		//flags for control,access out of ctl
		Outer				= 2,
		//inner flags for control
		Inner				= 3,
		Inner2				= 4,
		//only for owner draw text
		OwnerDrawText		= 5,
		//for RegisterClassEx
		WindowClass			= 6,
		// mask for InitCommonControlsEx(_maskCommonControls)
		InitControl			= 7,
		EndUser				= 8
	};
}StyleType;

//for designtoolbar:
//if want to show designtoolbar while right click on Form's TopRight corner need next:
//  nGlobalFlag&CC_DESIGN_G_DENIAL=0 or form has DesignStyle::ForceDesign then show designtoolbar.
//  but if has DesignStyle::NoDesign,then even nGlobalFlag&CC_DESIGN_G_DENIAL=0 then noshow designtoolbar.
#define CC_DESIGN_G_DENIAL 0x00010000

//flag for DoSetLang,while has any of this,then set rect or color or back image.
#define CC_DESIGN_DS_RECT 0x00000001
#define CC_DESIGN_DS_COLOR 0x00000002
//#define CC_DESIGN_DS_BIMG 0x00000004
#define CC_DESIGN_DS_CTLS 0x00000008
#define CC_DESIGN_DS_ANCHOR 0x00000010
//#define CC_DESIGN_DS_STYLE 0x00000020

//autosize flag
#define CC_DESIGN_DS_ASIZE 0x00000040
//if has this,then is autosize on
#define CC_DESIGN_DS_ASIZEON 0x00000080

//back flag
#define CC_DESIGN_DS_BACK 0x00000100 //Color Back
//if not has any,then is no TransparentTop
#define CC_DESIGN_DS_BACK1 0x00000200 //Color Back
#define CC_DESIGN_DS_BACK2 0x00000400 //Gradient Back(Left-Right)
#define CC_DESIGN_DS_BACK3 0x00000800 //Gradient Back(Up-Down)
#define CC_DESIGN_DS_BACK4 0x00001000 //Image Back
#define CC_DESIGN_DS_BACK5 0x00002000 //while Image Back,if has this then is Repeat,if no this then is Stretch
#define CC_DESIGN_DS_BACK1_4 (CC_DESIGN_DS_BACK1|CC_DESIGN_DS_BACK2|CC_DESIGN_DS_BACK3|CC_DESIGN_DS_BACK4)

//enable flag
#define CC_DESIGN_DS_ENABLE 0x00010000
//if has this,then is enable on
#define CC_DESIGN_DS_ENABLEON 0x00020000
//visible flag
#define CC_DESIGN_DS_VISIBLE 0x00040000
//if has this,then is visible on
#define CC_DESIGN_DS_VISIBLEON 0x00080000

//no need
//this is saved before changed while rect or bimg
//#define CC_DESIGN_DS_AUTOSIZE 0x2000
//#define CC_DESIGN_DS_NOTRANSPARENTTOP 0x4000
#if defined(_DEBUG_) || defined(_DESIGN) || defined(_DESIGNRUN)
class DesignConfig : public ObjectConfig
{
public:
	static DWORD nGlobalFlag;
};
typedef struct tagDesignStyle
{
	enum Type
	{
		None						= 0x00000001,
		NoMoveLeft					= 0x00000002,
		NoMoveTop					= 0x00000004,
		NoMoveRight					= 0x00000008,
		NoMoveBottom				= 0x00000010,
		NoEditColor					= 0x00000020,
		NoEditHColor				= 0x00000040,
		NoEditBColor				= 0x00000080,
		NoEditHBColor				= 0x00000100,
		NoEditText					= 0x00000200,
		NoEditTips					= 0x00000400,
		NoEditTipsExt				= 0x00000800,
		//set this flag for draw hot rect
		Selected					= 0x00001000,
		//for LabelLink
		EditURL						= 0x00002000,
		//move while mouse click on control
		NoMove						= 0x00004000,
		NoAnchor					= 0x00008000,
		AutoSize					= 0x00010000,
		Background					= 0x00020000,
		EndUser19					= 0x00040000,
		EndUser20					= 0x00080000,
		EndUser21					= 0x00100000,
		EndUser22					= 0x00200000,
		CanAddControl				= 0x00400000,
		CanSetVisible				= 0x00800000,
		CanSetEnable				= 0x01000000,
		EndUser26					= 0x02000000,
		EndUser27					= 0x04000000,
		EndUser28					= 0x08000000,
		EndUser29					= 0x10000000,
		//only for form,while has ForceDesign(then ignore),then open designtoolbar
		ForceDesign					= 0x20000000,
		//while has NoDesign,if form then no open designtoolbar,if control then do nothing
		NoDesign					= 0x40000000,
		DynamicCreated				= 0x80000000,
	};
}DesignStyle;
#define CC_DESIGN_NO_MOVE (DesignStyle::NoMoveLeft|DesignStyle::NoMoveTop|DesignStyle::NoMoveRight|DesignStyle::NoMoveBottom|DesignStyle::NoMove)
#define CC_DESIGN_NO_COLOR (DesignStyle::NoEditColor|DesignStyle::NoEditHColor|DesignStyle::NoEditBColor|DesignStyle::NoEditHBColor)
#define CC_DESIGN_NO_TEXTTIPS (DesignStyle::NoEditText|DesignStyle::NoEditTips|DesignStyle::NoEditTipsExt)
#define CC_DESIGN_NO_ALL (CC_DESIGN_NO_MOVE|CC_DESIGN_NO_COLOR|CC_DESIGN_NO_TEXTTIPS|DesignStyle::NoAnchor)
#endif

#define CONTROL_SPACE_ICONTEXT 3
#define CONTROL_SPACE_ICONTEXT_VERT 6
#define CONTROL_CONTEXTMENU_BTN_W 11
#define CONTROL_LINE_SPLIT ";	"

#define BASE_CONTROL_CONFIG_INIT(CTL) \
	_StyleArray[(int)StyleType::Style] = CTL##Config::nStyle; \
	_StyleArray[(int)StyleType::ExStyle] = CTL##Config::nExStyle; \
	_StyleArray[(int)StyleType::Inner] = CTL##Config::nInnerStyle; \
	_StyleArray[(int)StyleType::Outer] = CTL##Config::nOuterStyle; \
	_StyleArray[(int)StyleType::OwnerDrawText] = CTL##Config::nOwnerDrawTextStyle; \
	_corBackground = CTL##Config::corBackground; \
	_corTextground = CTL##Config::corTextground; \
	_corHotBackColor = CTL##Config::corHotBackColor; \
	_corHotTextColor = CTL##Config::corHotTextColor; \
	_idFlags = CC_CLASSID_##CTL;

#define BASE_CONTROL_CONFIG_DEF \
	static DWORD nStyle; \
	static DWORD nExStyle; \
	static DWORD nInnerStyle; \
	static DWORD nOuterStyle; \
	static DWORD nOwnerDrawTextStyle; \
	static COLORREF corBackground; \
	static COLORREF corTextground; \
	static COLORREF corHotBackColor; \
	static COLORREF corHotTextColor;

class FontConfig
{
public:
	static LOGFONT lfControl;
	static LOGFONT lfMenu;
	FontConfig()
	{
		//control
		lfControl.lfHeight = -12;               // height of font
		lfControl.lfWidth = 0;                // average character width
		lfControl.lfEscapement = 0;           // angle of escapement
		lfControl.lfOrientation = 0;          // base-line orientation angle
		lfControl.lfWeight = FW_NORMAL;              // font weight
		lfControl.lfItalic = FALSE;           // italic attribute option
		lfControl.lfUnderline = FALSE;        // underline attribute option
		lfControl.lfStrikeOut = 0;        // strikeout attribute option
		lfControl.lfCharSet = DEFAULT_CHARSET;          // character set identifier
		lfControl.lfOutPrecision = OUT_DEFAULT_PRECIS;  // output precision
		lfControl.lfClipPrecision = CLIP_DEFAULT_PRECIS;    // clipping precision
		lfControl.lfQuality = DEFAULT_QUALITY;          // output quality
		lfControl.lfPitchAndFamily = DEFAULT_PITCH;   // pitch and family
		cc::Buf::Cpy(lfControl.lfFaceName, 32, _T("MS UI Gothic"));

		//menu
		NONCLIENTMETRICS ncm;
		ncm.cbSize = sizeof(NONCLIENTMETRICS);
		SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(ncm), &ncm, 0);
		memcpy(&lfMenu, &ncm.lfMenuFont, sizeof(LOGFONT));
		//HFONT _hFont = ::CreateFontIndirect(&ncm.lfMenuFont);
		//::GetObject(_hFont, sizeof(lfMenu), &lfMenu);
		//::DeleteObject(_hFont);
	};
};

class ControlConfig : public ObjectConfig
{
public:
	BASE_CONTROL_CONFIG_DEF;
	static int nMarginLeft;
	static int nMarginTop;
	static int nMarginRight;
	static int nMarginBottom;
	//while mouse enter,then wait this time to see if is mouse leave
	static UINT nWaitTimerCheckMouseLeave;
};

//flags for _Flags of ITextIcon
//for some unknow error while define bool various,so invoid it with use DWORD _Flags
//here show how to use _InnerFlags
// 0~15 bits of _Flags is for others control(only used by ccProc system)
// 16~31 bits of _Flags is for end user
#define CC_FLAG_CONTROL_START				0x00000001
//#define CC_FLAG_CONTROL_VISIBLE				CC_FLAG_CONTROL_START
//#define CC_FLAG_CONTROL_HANDLECREATED		CC_FLAG_CONTROL_START <<1
//#define CC_FLAG_CONTROL_HANDLECREATEDEND	CC_FLAG_CONTROL_START <<2
//#define CC_FLAG_CONTROL_OFFSETNEWLINE		CC_FLAG_CONTROL_START <<3
#define CC_FLAG_BACKFILE_OK					CC_FLAG_CONTROL_START
#define CC_FLAG_CONTROL_END					CC_FLAG_CONTROL_START <<1

class Control : public ITextIcon
{
public:
	HWND m_hWnd; //must be first data member
	//this is accessed by enduser or others
	void* Tag;

	EventHandler BackColorChanged;
	//EventHandler BackgroundImageChanged;
	//EventHandler BindingContextChanged;
	//EventHandler CausesValidationChanged;
	//UICuesEventHandler ChangeUICues;
	EventHandler Click;
	EventHandler ContextMenuChanged;
	ControlEventHandler ControlAdded;
	ControlEventHandler ControlRemoved;
	//EventHandler SetCursor;
	EventHandler DockChanged;
	EventHandler DoubleClick;
	//DragEventHandler DragEnter;
	//EventHandler DragLeave;
	//DragEventHandler DragOver;
	MouseEventHandler DragMove;
	DragEndEventHandler DragEnd;
	GiveFeedbackEventHandler GiveFeedback;
	EventHandler EnabledChanged;
	EventHandler Enter;
	EventHandler FontChanged;
	EventHandler ForeColorChanged;
	EventHandler GotFocus;
	EventHandler HandleCreated;
	EventHandler HandleDestroyed;
	HelpEventHandler HelpRequested;
	EventHandler ImeModeChanged;
	//InvalidateEventHandler Invalidated;
	KeyEventHandler KeyDown;
	KeyPressEventHandler KeyPress;
	KeyEventHandler KeyUp;
	LayoutEventHandler Layout;
	EventHandler Leave;
	EventHandler LocationChanged;
	EventHandler LostFocus;
	MouseEventHandler MouseDown;
	EventHandler MouseEnter;
	EventHandler MouseHover;
	EventHandler MouseLeave;
	MouseEventHandler MouseMove;
	MouseEventHandler MouseUp;
	EventHandler MouseWheel;
	EventHandler Notify;
	PaintEventHandler PaintBegin;
	PaintEventHandler PaintEnd;
	PaintEventHandler PaintBackground;
	//EventHandler ParentChanged;
	//EventHandler RightToLeftChanged;
	QueryContinueDragEventHandler QueryContinueDrag;
	EventHandler Move;
	EventHandler Moving;
	EventHandler Resize;
	EventHandler Resizing;
	EventHandler SizeChanged;
	EventHandler StyleChanged;
	EventHandler SystemColorsChanged;
	ScrollEventHandler Scroll;
	//EventHandler TabIndexChanged;
	//EventHandler TabStopChanged;
	EventHandler TextChanged;
	//EventHandler Validated;
	//CancelEventHandler Validating;
	EventHandler VisibleChanged;

	//control can eat ley event here while Handled=true 
	EventHandler PreKeyDown;
	EventHandler SysCommand;
	//NextControl.LParam is the next control,if no then NULL
	EventHandler NextControl;
	//ontimer,e->WParam is ID
	EventHandler Tick;

	//for acept files
	EventHandler DropFiles;

	//for list controls
	MeasureItemEventHandler MeasureItem;
	DrawItemEventHandler DrawItem;
	CompareItemEventHandler CompareItem;
	EventHandler DeleteItem;
	CharToItemEventHandler CharToItem;
	VKeyToItemEventHandler VKeyToItem;

// Attributes
public:
	//Constructors and other creation
	Control();
	//destructor
	virtual ~Control();
	virtual void Dispose();
	virtual bool GetIsDispose();

	operator HWND() const;
	bool operator==(const Control& control) const;
	bool operator!=(const Control& control) const;
protected:
	//class name with namespace
	virtual cc::Str DoGetClassName() const;
public:
	AnchorStyles::Type Anchor;
	DockStyles::Type Dock;
	int TabIndex;
	POINT MinPosition;
	static HFONT hFontAppShare;
#if defined(_DEBUG_) || defined(_DESIGN)
	//code for design
	//flag for moveable or others
	DesignStyle::Type Design;

	//sould run this after created control
	void CreateSource(cc::Str& sTxt, cc::Str& sVarList, cc::Str sParentHead);
	virtual void DoCreateSource(cc::Str& sTxt, cc::Str& sVarList, cc::Str sThisHead);
#endif

protected:
	Control* _pParent;
	Control* _pClient;
	//relate to _pRelatePostion while _pRelateControl != NULL
	//POINT _ptOffset;
	//Control* _pOffsetControl;
	ContextMenu* _pContextMenu;
	HFONT _hFont;
	RECT _Rect;
	RECT _OrgRect;
	//is has children,then use this to calcute child's location or size if children has anchor.
	SIZE _ClientSize;
	const TCHAR *_ClassName;
	Control* _pParentBrush;
	LockObject m_lockText;

	DWORD _StyleArray[CC_CONTROL_STYLETYPE_SIZE];

	//DWORD _Style;
	//DWORD _ExStyle;
	//DWORD _OuterStyle; //flags for control,access out of ctl
	//DWORD _InnerStyle; //inner flags for control
	//DWORD _Inner2Style; //private flags for control
	//DWORD _Flags; //flags for only ccProc(this control and others)(put in ITextIcon)
	//DWORD _OwnerDrawTextStyle; //only for owner draw text
	//DWORD _ClassStyle; //for regist windows type
	//WORD _SetWindowPosFlags;
	// mask for InitCommonControlsEx
	//long _maskCommonControls;
	//no set/get
	HMENU _hMenu;
	HBRUSH _hbrBackground;
	COLORREF _corBackground;
	COLORREF _corTextground;
	COLORREF _corHotBackColor;
	COLORREF _corHotTextColor;
	List<Control*>* _lstControls;
	WNDPROC _controlRealWndProc;
	HCURSOR _hCursor;
	HBITMAP _hbmpBackground;
	//0:none, 1:-, 2:|, 3:/, 4:\, 5:#, 6:x
	int _nBackgroundHatch;

	SIZE _minSize;
	RECT _Margin;

	//bool _Visible;
	//while get WM_CREATE,soon set this flage.also this time win is created OK
	//bool _bIsHandleCreated;
	//bool _bIsHandleCreatedEnd;

	int _nMouseHoverCount;
	//bool _bIsAutoSize;
	//the length while draw,if not measure then -1
	SIZE _sizeTextIcon;
	//for DoMeasureSize is do with nExpectCY,while nExpectCY changed,re DoMeasureSize
	int _nExpectCY;

	//while mouse down, then move form
	Control* _moveForm;
	DWORD _moveFormCursor;
	
///////////////////////////////////////////////////////////////////////////////
// native windows
protected:
	virtual WNDPROC* GetSuperWndProcAddr();
	virtual bool PreCreateWindow(CREATESTRUCT& cs);
	void HookWindowCreate(Control *pControl);
	bool UnhookWindowCreate();
	virtual void OnNcDestroy();
	virtual void PostNcDestroy();
	static LRESULT CALLBACK StaticCBTProc(int msg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK StaticWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual HFONT CreateDefaultFont();

	virtual void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
// native windows
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// c# like
protected:
    // WndProc - calls appriate On... function for the give
	virtual LRESULT WndProc(EventArgs *e);
	virtual LRESULT DefWndProc(EventArgs *e);
	virtual LRESULT DefWndProc(int msg, WPARAM wParam, LPARAM lParam);
	//here to create window
	virtual void CreateHandle();
	virtual void DestroyHandle();
public:
	//call by form while form create
	void Show();
public:
//	void CreateGraphics();
	void CreateControl();

	//if not stop even WS_TABSTOP&WS_VISIBLE,then need overwrite this
	virtual bool IsTabStop();
	//bool setLastControl is if have child,set first of last
	virtual bool Focus(bool setLastControl = false);
	//while form then is Activate or Deactivate
	virtual bool Focused();
	static Control* GetFocus();
	//static bool __stdcall WalkPreTranslateTree(HWND hWndStop, MSG* pMsg, UINT_PTR keyData);
	virtual bool PreProcessMessage(MSG* pMsg, UINT_PTR& keyData);
	virtual bool ProcessKeyEventArgs(EventArgs *e);
	virtual bool ProcessKeyMessage(EventArgs *e);
	virtual bool ProcessKeyPreview(EventArgs *e);
	virtual bool PreFormProcessMessage(MSG* pMsg, UINT_PTR& keyData);
	//Control* GetTopLevelParent();
	//static Control* GetActiveWindow();
	//Control* GetTopForm();
	static Control* PASCAL FromHandle(HWND hWnd);
	// Get the next or previous control in the tab order,forward=false for behind
	virtual Control* GetNextControl(Control* ctl, bool forward = false);
	//Control* TopLevelControl();

	//instead of sendmessage,use this call to inner function
	virtual void FreeCall(Object* sender, EventArgs* e);
	//while mouse down, then move form
	virtual void SetMouseDownMoveForm(Control* form, DWORD moveCursor = -1);

	//dynamic position(now real size)
	virtual RECT GetBounds();
	//set real position but not original,so even not _bIsHandleCreated then also resize child to parent
	void SetBoundsNotOriginal(RECT rect, BoundsSpecified::Type specified = BoundsSpecified::All);
	void SetBoundsNotOriginal(int x, int y, int width, int height, BoundsSpecified::Type specified = BoundsSpecified::All);
	//while has parent and not AnchorStyles::None and AnchorStyles::Top|AnchorStyles::Left
	//set Original so right to parent's onresize
	void SetBoundsCalcuteOriginal(int x, int y, int width, int height);
	//set real position also original,so while not _bIsHandleCreated then not resize child to parent
	void SetBounds(RECT rect, BoundsSpecified::Type specified = BoundsSpecified::All);
	void SetBounds(int x, int y, int width, int height, BoundsSpecified::Type specified = BoundsSpecified::All);
	void SetLocation(int xLeft, int yTop);
	void SetLocationOffset(Control* control, bool isNewLine, int offsetX, int offsetY);
	void SetSize(int cxWidth, int cyHeight);

	//is has children,then use this to calcute child's location or size if children has anchor.
	//while form,if children positon is fixed,then recommend use SetSizeBaseOnClient(alse call SetClientSize).
	void SetClientSize(int cxWidth, int cyHeight);
	//original position(set by user at design time),while onresize,calcue from this
	RECT GetOriginalBounds();
	////this for original client size,should after SetSize
	//void SetOriginalSize(int width, int height);
	////this for original client position&size,should after SetSize
	//void SetOriginalBounds(int x, int y, int width, int height);

	void SetMinSize(int minWidth, int minHeight);
	virtual SIZE GetMinSize();
	void SetMargin(int nLeft, int nTop, int nRight, int nBottom);
	RECT GetMargin();
	COLORREF GetTextColor();
	void SetTextColor(COLORREF corText);
	COLORREF GetBackground() const;
	//for set color while WM_CTLCOLOREDIT or treeview or listview(overwrite Themebackground style)
	virtual void SetBackground(COLORREF corBackground);
	virtual void SetBackground(COLORREF corBackground, int nBackgroundHatch);
	//should notice that this only valid for some owerndraw ctl
	void SetHotBackColor(COLORREF corHotBackColor);
	COLORREF GetHotBackColor() const;
	void SetHotTextColor(COLORREF corHotTextColor);
	COLORREF GetHotTextColor() const;
	void SetTrackMouse(bool isTrackMouse);
	HCURSOR GetCursor() const;
	virtual void SetCursor(HCURSOR hCursor);
	void SetBackgroundBrush(HBRUSH hbrBackground, bool needDestroy = true);
	//will return brush by CC_BACK_TYPE_?
	virtual HBRUSH GetBackgroundBrush(bool isFource = false);
	//HBITMAP GetBackgroundImage();
	//for background(overwrite Themebackground style), resType:RT_BITMAP,RT_ICON,RT_CURSOR,...
	//can treate resType as MAKEINTRESOURCE(id) or _T("#99")
	bool SetBackgroundImage(UINT nResID, const TCHAR *resType, bool isStretch = false);
	bool SetBackgroundImage(const TCHAR* filename, bool isStretch = false);
	void SetBackgroundImage(HBITMAP hbmpBackground, bool isStretch = false, bool needDestroy = true);
	void SetBackgroundImage(HICON hiconBackground, bool isStretch, bool needDestroy);

	//only can do what you know to client
	Control* GetClient();

	//sould run this after created control
	//keyArr is param address,but sHead is by value
	void Config(bool isFromIni, KeyArr& keyArr, cc::Str sParentHead);

	//need set InnerStyle::TrackMouse
	bool GetIsMouseOn();

	//20090107, for form want call _lstBars and _lstToolBars's ProcessMnemonic,so put it to public
	virtual bool ProcessMnemonic(TCHAR tchar);
protected:
	//call by Update
	virtual void DoUpdate();
	//keyArr is param address,but sHead is by value
	virtual void DoConfig(bool isFromIni, KeyArr& keyArr, cc::Str sThisHead);
	//keyArr is param address,but sThisHead is by value
	virtual void DoSetLang(KeyArr& keyArr, cc::Str sThisHead);
	//InvalidateBackGround for resize or others
	virtual void InvalidateBackGround();

protected:
	virtual void SetBoundsCore(int x, int y, int width, int height, BoundsSpecified::Type specified, bool isSetSameOriginal);
	virtual bool ProcessCmdKey(MSG* pMsg, UINT_PTR& keyData);
	virtual bool IsInputKey(UINT_PTR& keyData);
	virtual bool ProcessDialogKey(UINT_PTR& keyData);
	virtual bool IsInputChar(TCHAR tchar);
	virtual bool ProcessDialogChar(TCHAR tchar);
	virtual bool DoProcessMnemonic(TCHAR tchar);

// c# like
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Set.../Get.../Add...
public:
	//the last added control has more priority(c# like)
	//if add statusbar1 and then statusbar2,so statusbar2 is bottom and statusbar1 is up of statusbar2
	virtual void AddControl(Control* control, int nIndex = -1);
	virtual Control* RemoveControl(int nIndex);
	virtual void RemoveControl(Control* control);
	//Perform a lowercase comparison of strings
	virtual Control* GetControl(const TCHAR* ctlName) const;
	virtual Control* GetControl(int nIndex) const;
	virtual bool ContainsControl(Control* control) const;
	int GetControlCount() const;
	HFONT GetFont();
	void SetFont(HFONT pFont);
	void SetFont(int nSize, const TCHAR* sName = NULL, bool isBold = false, bool isItalic = false, bool isUnderline = false, bool isStrikeOut = false, long nEscapement = -1);
	ContextMenu* GetContextMenu() const;
	void SetContextMenu(ContextMenu* menu);
	Control* GetParent() const;
	//not only _pParent,but perhaps Control from ::GetParent()
	Control* GetParentReal() const;
	virtual void SetParent(Control* parent, bool notAddToParentList = false);
	const TCHAR* GetWndClassName() const;
	virtual const TCHAR* GetText();
	virtual const char* GetTextA();
	virtual const wchar_t* GetTextW();
	virtual int GetTextLen();
	virtual int GetTextLenA();
	virtual int GetTextLenW();
	virtual void SetText(const TCHAR* sText);
	virtual void SetText(const cc::Str& sText);

	//2008/11/12
	bool HasStyle(StyleType::Type type, DWORD style);
	bool IsStyle(StyleType::Type type, DWORD style);
	DWORD GetStyle(StyleType::Type type);
	//if need redraw,should nFlags=SWP_FRAMECHANGED|SWP_DRAWFRAME
	void SetStyle(StyleType::Type type, DWORD dwRemove, DWORD dwAdd, UINT nFlags = 0, bool needUpdate = false);
	void SetStyleBool(StyleType::Type type, DWORD style, bool value, UINT nFlags = 0, bool needUpdate = false);

	void SetOuterStyle(OuterStyle::Type outerStyle, bool value, bool needUpdate = false);

public:
	virtual bool GetVisible();
	virtual void SetVisible(bool value);

	void SetEnabled(bool isEnabled);
	bool GetEnabled();

	void LockWindowUpdate();
	void UnLockWindowUpdate();

	virtual SIZE BestSize(bool isUpdate = false);
	virtual RECT CalcuteRect(bool updateRect = false, int nExpectX = -1, int nExpectY = -1, int nExpectCX = -1, int nExpectCY = -1, bool isFourceCalcute = false);
protected:
	//call by CalcuteRect if autosize
	virtual SIZE DoMeasureSize(int ctlHeight);
	virtual void DoAdjustRect(long& nRealX, long& nRealY, int& nRealCX, int& nRealCY, int nExpectX, int nExpectY, int nExpectCX, int nExpectCY);

// Set.../Get.../Add...
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// On...
protected:
	virtual void OnBackColorChanged(EventArgs *e);
	//virtual void OnBackgroundImageChanged(EventArgs *e);
	//virtual void OnBindingContextChanged(EventArgs *e);
	//virtual void OnCausesValidationChanged(EventArgs *e);
	//virtual void OnChangeUICues(UICuesEventArgs *e);
	virtual void OnClick(EventArgs *e);
	virtual void PreOnCommand(EventArgs *e);
	virtual void OnCommand(EventArgs *e);
	virtual void OnContextMenu(MouseEventArgs *e);
	virtual void OnContextMenuChanged(EventArgs *e);
	virtual void OnControlAdded(ControlEventArgs *e);
	virtual void OnControlRemoved(ControlEventArgs *e);
	virtual void OnCreateControl();
	virtual void OnSetCursor(EventArgs *e);
	virtual void OnDockChanged(EventArgs *e);
	virtual void OnDoubleClick(EventArgs *e);
	//virtual void OnDragEnter(DragEventArgs *e);
	//virtual void OnDragLeave(EventArgs *e);
	//virtual void OnDragOver(DragEventArgs *e);
	virtual void OnDragMove(MouseEventArgs *e);
	virtual void OnDragEnd(DragEndEventArgs *e);
	virtual void OnPaint(PaintEventArgs *e);
	virtual void OnPaintBackground(PaintEventArgs *e);
	virtual void OnCtlColorEdit(EventArgs *e);
	virtual void OnEnabledChanged(EventArgs *e);
	virtual void OnEnter(EventArgs *e);
	virtual void OnFontChanged(EventArgs *e);
	virtual void OnForeColorChanged(EventArgs *e);
	//esc or ctrl,alt,shift key while draging
	virtual void OnGiveFeedback(GiveFeedbackEventArgs *e);
	virtual void OnGotFocus(EventArgs *e);
	virtual void OnHandleCreated(EventArgs *e);
	virtual void OnHandleDestroyed(EventArgs *e);
	virtual void OnHelpRequested(HelpEventArgs *e);
	virtual void OnImeModeChanged(EventArgs *e);
	//virtual void OnInvalidated(InvalidateEventArgs *e);
	virtual void OnKeyDown(KeyEventArgs *e);
	virtual void OnKeyPress(KeyPressEventArgs *e);
	virtual void OnKeyUp(KeyEventArgs *e);
	virtual void OnLayout(LayoutEventArgs *e);
	virtual void OnLeave(EventArgs *e);
	virtual void OnLocationChanged(EventArgs *e);
	virtual void OnLostFocus(EventArgs *e);
	virtual void OnMouseDown(MouseEventArgs *e);
	virtual void OnMouseEnter(EventArgs *e);
	virtual void OnMouseHover(EventArgs *e);
	virtual void OnMouseLeave(EventArgs *e);
	virtual void OnMouseMove(MouseEventArgs *e);
	virtual void OnTimer(EventArgs *e);
	//special for mousemove
	virtual void OnInnerTimer(EventArgs *e);
	virtual void OnMouseUp(MouseEventArgs *e);
	virtual void OnNotifyMessage(EventArgs *e);
	//virtual void OnParentBackColorChanged(EventArgs *e);
	//virtual void OnParentBackgroundImageChanged(EventArgs *e);
	//virtual void OnParentBindingContextChanged(EventArgs *e);
	//virtual void OnParentChanged(EventArgs *e);
	//virtual void OnParentEnabledChanged(EventArgs *e);
	//virtual void OnParentFontChanged(EventArgs *e);
	//virtual void OnParentForeColorChanged(EventArgs *e);
	//virtual void OnParentRightToLeftChanged(EventArgs *e);
	//virtual void OnParentVisibleChanged(EventArgs *e);
	//virtual void OnQueryContinueDrag(QueryContinueDragEventArgs *e);
	virtual void OnMove(EventArgs *e);
	virtual void OnMoving(EventArgs *e);
	virtual void OnResize(EventArgs *e);
	virtual void OnResizing(EventArgs *e);
	//virtual void OnRightToLeftChanged(EventArgs *e);
	virtual void OnSizeChanged(EventArgs *e);
	virtual void OnStyleChanged(EventArgs *e);
	virtual void OnSystemColorsChanged(EventArgs *e);
	//virtual void OnTabIndexChanged(EventArgs *e);
	//virtual void OnTabStopChanged(EventArgs *e);
	virtual void OnTextChanged(EventArgs *e);
	//virtual void OnValidated(EventArgs *e);
	//virtual void OnValidating(CancelEventArgs *e);
	virtual void OnVisibleChanged(EventArgs *e);

	virtual void OnSysCommand(EventArgs *e);
	virtual void OnNextControl(EventArgs *e);

	virtual void OnScroll(ScrollEventArgs *e);
	virtual void OnMouseWheel(EventArgs *e);
	static void ReflectLastMsg(HWND hWndChild, EventArgs *e);
	void SendChildNotifyLastMsg(EventArgs *e);
	virtual void OnChildNotify(EventArgs *e);
	//virtual void OnNcActivate(EventArgs *e);
	virtual void OnNcHittest(EventArgs *e);

	virtual void OnDropFiles(EventArgs *e);

// On...
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// For MENU
protected:
	virtual void Menu_OnEnterLoop(EventArgs *e);
	virtual void Menu_OnExitLoop(EventArgs *e);
	virtual void Menu_OnClick(EventArgs *e);
	virtual void Menu_OnPopup(EventArgs *e);
	virtual void Menu_OnSelect(EventArgs *e);
	virtual void PreMeasureItem(MeasureItemEventArgs *e);
	virtual void PreDrawItem(DrawItemEventArgs *e);
	virtual void OnMeasureItem(MeasureItemEventArgs *e);
	virtual void OnDrawItem(DrawItemEventArgs *e);

protected:
	Menu* _pActivePopunMenu;
// For MENU
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// For LIST Controls
protected:
	virtual void OnCompareItem(CompareItemEventArgs *e);
	virtual void OnDeleteItem(EventArgs *e);
	virtual void OnCharToItem(CharToItemEventArgs *e);
	virtual void OnVKeyToItem(VKeyToItemEventArgs *e);

///////////////////////////////////////////////////////////////////////////////
// For Subclass
public:
	bool SubclassWindow(HWND hWnd, Control* parent = NULL);
	HWND UnsubclassWindow();

///////////////////////////////////////////////////////////////////////////////
// For Scroll bar helpers
public:
	/*
	ScrollBar* GetScrollBarCtrl(int) const;
	int SetScrollPos(int nBar, int nPos, bool bRedraw);
	int GetScrollPos(int nBar) const;
	void SetScrollRange(int nBar, int nMinPos, int nMaxPos, bool bRedraw);
	void GetScrollRange(int nBar, LPINT lpMinPos, LPINT lpMaxPos) const;
	void EnableScrollBarCtrl(int nBar, bool bEnable);
	bool SetScrollInfo(int nBar, LPSCROLLINFO lpScrollInfo, bool bRedraw);
	bool GetScrollInfo(int nBar, LPSCROLLINFO lpScrollInfo, UINT nMask);
	int GetScrollLimit(int nBar);
	void ScrollWindow(int xAmount, int yAmount, LPCRECT lpRect, LPCRECT lpClipRect);
	*/
// For Scroll bar helpers
///////////////////////////////////////////////////////////////////////////////
}; //class Control


//http://www.codeproject.com/miscctrl/xinfotip.asp
//http://www.codeproject.com/miscctrl/pptooltip.asp
//http://www.vckbase.com/document/viewdoc/?id=1240
class ToolTipConfig : public ObjectConfig
{
public:
	BASE_CONTROL_CONFIG_DEF;
	static int nMarginLeft;
	static int nMarginTop;
	static int nMarginRight;
	static int nMarginBottom;
	static int nWaitHideMilliSecond;
	static int nWindowWidth;
};
#define CC_FLAG_TOOLTIP_ISACTIVE				CC_FLAG_CONTROL_END
//#define CC_FLAG_TOOLTIP_ISSHOWALWAYS			CC_FLAG_CONTROL_END <<1
#define CC_FLAG_TOOLTIP_ISBALLOON				CC_FLAG_CONTROL_END <<2
#define CC_FLAG_TOOLTIP_END						CC_FLAG_CONTROL_END <<3
class ToolTip : public Control
{
public:
	EventHandler Popup;

public:
	ToolTip();
	virtual ~ToolTip();

	//Gets or sets a value indicating whether the ToolTip is currently active(show/not show).
	bool GetActive();
	void SetActive(bool active);
	//Gets or sets the automatic delay for the ToolTip.
	int GetAutomaticDelay();
	void SetAutomaticDelay(int automaticDelay);
	//Gets or sets the period of time the ToolTip remains visible if the pointer is stationary on a control with specified ToolTip text.
	int GetAutopopDelay();
	void SetAutopopDelay(int autopopDelay);
	//Gets or sets the time that passes before the ToolTip appears.
	int GetInitialDelay();
	void SetInitialDelay(int initialDelay);
	//Gets or sets the length of time that must transpire before subsequent ToolTip windows appear as the pointer moves from one control to another.
	int GetReshowDelay();
	void SetReshowDelay(int reshowDelay);
	//Gets or sets a value indicating whether a ToolTip window is displayed, even when its parent control is not active.
	//bool GetShowAlways();
	//void SetShowAlways(bool showAlways);
	//Gets or sets a value indicating whether the ToolTip should use a balloon window.
	bool GetBalloon();
	void SetBalloon(bool balloon);

	void Show(const TCHAR* sText, ImageList* imageList, int nIconID, int duration = -1, bool isForMenu = false, bool isNoWait = false);
	void Show(const TCHAR* sText, ImageList* imageList, int nIconID, POINT pt, int duration = -1, bool isForMenu = false, bool isNoWait = false);
	void Show(const TCHAR* sText, ImageList* imageList, int nIconID, int x, int y, int duration = -1, bool isForMenu = false, bool isNoWait = false);
	void StopTimer();
	bool KeepShow(bool isKeepPosition = false);

protected:
	//status
	typedef struct tagTipsStatus
	{
		enum Type
		{
			Wait, //tipwindow is hide,timer is stop.if show,wait _nInitialDelay
			WaitShowFirst, //tipwindow is hide,timer is _nInitialDelay.if timeout,show window.if not timeout but settext is null, turn to Wait
			WaitShowNext, //tipwindow is hide,timer is _nReshowDelay.when timeout,if has next,show it,otherwise turn to WaitShowFirst
			Showing, //tipwindow is hide,timer is _nAutopopDelay.when timeout turn to WaitShowNext.if not timeout,but settext then turn to WaitShowNext
		};
	}TipsStatus;

	int _nAutomaticDelay;
	int _nAutopopDelay;
	int _nInitialDelay;
	int _nReshowDelay;
	POINT _ptLastPosition;
	TipsStatus::Type _eStatus;
	int _nBringToTopCnt;
	RECT rectBody;
	int nOldDuration;
	int nOldWidth;
	int nOldHeight;

	virtual void OnResize(EventArgs *e);
	virtual void OnPaint(PaintEventArgs *e);
	virtual void OnPopup(EventArgs *e);
	virtual void OnInnerTimer(EventArgs *e);
	virtual void CalcutePosition(int x, int y, int nW, int nH);
	virtual void OnNcHittest(EventArgs *e);
	virtual void DoUpdate();
};

class ToolItemConfig : public ObjectConfig
{
public:
	BASE_CONTROL_CONFIG_DEF;
	static int nMarginLeft;
	static int nMarginTop;
	static int nMarginRight;
	static int nMarginBottom;
};
#define CC_FLAG_TOOLITEM_NEEDDESTROY		CC_FLAG_CONTROL_END
#define CC_FLAG_TOOLITEM_END				CC_FLAG_CONTROL_END<<1
//can pop normal menu or FloatContainer menu.
//  if SetFloatContainer then no right click menu.
//  if has OuterStyle::ContextMenuButton&GetContextMenu then no push&pop style.
class ToolItem : public Control
{
public:
	EventHandler Popup;

public:
	ToolItem();
	virtual ~ToolItem();
	virtual void Dispose();

	bool GetBreak();
	void SetBreak(bool value);
	bool GetBlank();
	void SetBlank(bool value);
	bool GetPushed();
	void SetPushed(bool value);
	bool GetAutoSize();
	void SetAutoSize(bool value);

	bool GetIsPushButton();
	void SetIsPushButton(bool value);
	FloatContainer* GetFloatContainer();
	void SetFloatContainer(FloatContainer* pFloater, bool needDestroy);
protected:
	bool _bIsClicking;
	//show popup menu at paint
	bool _bIsPreShowMenu;
	FloatContainer* _pFloater;

	virtual void OnKeyDown(KeyEventArgs *e);
	virtual void OnKeyUp(KeyEventArgs *e);
	virtual void OnGotFocus(EventArgs *e);
	virtual void OnLostFocus(EventArgs *e);
	//call by Update
	virtual void DoUpdate();
	//call by CalcuteRect if autosize
	virtual SIZE DoMeasureSize(int ctlHeight);
	virtual void DoAdjustRect(long& nRealX, long& nRealY, int& nRealCX, int& nRealCY, int nExpectX, int nExpectY, int nExpectCX, int nExpectCY);
	//keyArr is param address,but sHead is by value
	virtual void DoConfig(bool isFromIni, KeyArr& keyArr, cc::Str sThisHead);

	virtual void OnClick(EventArgs *e);
	virtual void OnPopup(EventArgs *e);
	virtual void OnMouseDown(MouseEventArgs *e);
	virtual void OnMouseEnter(EventArgs *e);
	virtual void OnMouseLeave(EventArgs *e);
	virtual void OnMouseUp(MouseEventArgs *e);
	virtual void OnPaint(PaintEventArgs *e);
	virtual void Menu_OnExitLoop(EventArgs *e);
	virtual void OnInnerTimer(EventArgs* e);
};

class StatusItemConfig : public ObjectConfig
{
public:
	BASE_CONTROL_CONFIG_DEF;
	static int nMarginLeft;
	static int nMarginTop;
	static int nMarginRight;
	static int nMarginBottom;
};
#define CC_FLAG_STATUSITEM_END				CC_FLAG_TOOLITEM_END
class StatusItem : public ToolItem
{
public:
	typedef struct tagAutoSize
	{
		enum Type
		{
			None,
			Sprint,
			Contents,
		};
	}AutoSize;

	StatusItem();
	virtual ~StatusItem();

	//cut strings after "\r\n"
	virtual void SetText(const TCHAR* sText);
	AutoSize::Type GetAutoSize();
	void SetAutoSize(AutoSize::Type eAutoSize);

protected:
	virtual void DoAdjustRect(long& nRealX, long& nRealY, int& nRealCX, int& nRealCY, int nExpectX, int nExpectY, int nExpectCX, int nExpectCY);
};


//draw Gradient Background from HotBackColor to Background
class PushButtonConfig : public ObjectConfig
{
public:
	BASE_CONTROL_CONFIG_DEF;
};
#define CC_FLAG_PUSHBUTTON_END				CC_FLAG_TOOLITEM_END
class PushButton : public ToolItem
{
public:
	PushButton();
	virtual ~PushButton();
protected:
};

/*
	LabelLink* labLink = new LabelLink();
	labLink->SetName(_T("labLink"));
	labLink->SetText(_T("aaa@aaa.com"));
	//labLink->SetLinkType(LinkType::Mail); //default is auto
	labLink->SetLocation(0, 0);
	//labLink->SetSize(100, 14); //for default autosize,not need this
	labLink->SetHotTextColor(RGB(255, 0, 0)); //this is default
	labLink->SetHotBackColor(RGB(230, 230, 230)); //this is default
	labLink->SetTextColor(RGB(0, 0, 255)); //this is default
	labLink->SetBackground(::GetSysColor(COLOR_BTNFACE)); //this is default
	labLink->SetOuterStyle(OuterStyle::TransparentNoEraseBack, true); //if this then no BackColor
	this->AddControl(labLink);
*/
class LabelLinkConfig : public ObjectConfig
{
public:
	BASE_CONTROL_CONFIG_DEF;
	static int nMarginLeft;
	static int nMarginTop;
	static int nMarginRight;
	static int nMarginBottom;
};
#define CC_FLAG_LABELLINK_ISMAILLINK		CC_FLAG_TOOLITEM_END
#define CC_FLAG_LABELLINK_END				CC_FLAG_TOOLITEM_END <<1

typedef struct tagLinkType
{
	enum Type
	{
		Auto			= 1,
		Web				= 2,
		Mail			= 3,
	};
}LinkType;
class LabelLink : public ToolItem
{
public:
	LabelLink();
	virtual ~LabelLink();

	virtual const TCHAR* GetTips();
	virtual const TCHAR* GetURL();
	virtual void SetURL(const TCHAR* sURL);
	//while auto,then if not http:// or ftp:// and has @ then is mail
	void SetLinkType(LinkType::Type linkType);
	LinkType::Type GetLinkType();
	//void SetIsUnderLine(bool isUnderLine);
	//bool GetIsUnderLine();

protected:
	TCHAR *_URL;
	LinkType::Type m_eLinkType;

	virtual void OnClick(EventArgs *e);
	virtual void DoSetLang(KeyArr& keyArr, cc::Str sThisHead);
};

//draw Gradient Background from HotBackColor to Background
class GradientLabelConfig : public ObjectConfig
{
public:
	BASE_CONTROL_CONFIG_DEF;
};
#define CC_FLAG_GRADIENTLABEL_END				CC_FLAG_TOOLITEM_END
class GradientLabel : public ToolItem
{
public:
	GradientLabel();
	virtual ~GradientLabel();
protected:
};

/*
class LabelConfig : public ObjectConfig
{
public:
	BASE_CONTROL_CONFIG_DEF;
};
//if use textcolor or _hIconHot,_hIconGray,need OWNERDRAW.
//if has OuterStyle::Break then draw as "---- TEXT ----"
#define CC_FLAG_LABEL_END				CC_FLAG_CONTROL_END
class Label : public Control
{
public:
	//Constructors
	Label();
	//destructor
	virtual ~Label();

	bool GetAutoSize();
	void SetAutoSize(bool value);
	Control* GetFocusControl();
	void SetFocusControl(Control* ctl);

	virtual void SetCursor(HCURSOR hCursor);
	HBITMAP SetBitmap(HBITMAP hBitmap);
	HBITMAP GetBitmap() const;
	HENHMETAFILE SetEnhMetaFile(HENHMETAFILE hMetaFile);
	HENHMETAFILE GetEnhMetaFile() const;

protected:
	virtual void OnCreateControl();
	virtual void OnChildNotify(EventArgs *e);
	virtual void DoUpdate();
	virtual bool DoProcessMnemonic(TCHAR tchar);
	//virtual void OnPaint(PaintEventArgs *e);

// Overridables (for owner draw only)
	virtual void OnDrawItem(DrawItemEventArgs* e);

///////////////////////////////////////////////////////////////////////////////
//#for hide this fun
protected:
	Control* m_pFocusControl;

	void Show(){Control::Show();};
	void Update(){Control::Update();};
	void CreateControl(){Control::CreateControl();};
	static Control* PASCAL FromHandle(HWND hWnd){return Control::FromHandle(hWnd);};
//#for hide this fun
///////////////////////////////////////////////////////////////////////////////
};
*/

class LabelConfig : public ObjectConfig
{
public:
	BASE_CONTROL_CONFIG_DEF;
};
//if has OuterStyle::Break then draw as "---- TEXT ----"
#define CC_FLAG_LABEL_END				CC_FLAG_CONTROL_END
class Label : public Control
{
public:
	//Constructors
	Label();
	//destructor
	virtual ~Label();

	Control* GetFocusControl();
	void SetFocusControl(Control* ctl);

protected:
	Control* m_pFocusControl;

	virtual bool DoProcessMnemonic(TCHAR tchar);
	virtual void OnPaint(PaintEventArgs *e);
};

class PictureBoxConfig : public ObjectConfig
{
public:
	BASE_CONTROL_CONFIG_DEF;
};
#define CC_FLAG_PICTURE_NEEDDESTROY				CC_FLAG_LABEL_END
#define CC_FLAG_PICTURE_END						CC_FLAG_LABEL_END <<1
class PictureBox : public Control
{
public:
	PictureBox();
	virtual ~PictureBox();

	//only IMAGE_BITMAP or IMAGE_ICON
	void SetImage(UINT nResID, const TCHAR *resType);
	void SetImage(const TCHAR* filename);
	void SetImage(HBITMAP hBitmap, bool needDestroy);
	void SetImage(HICON hIcon, bool needDestroy);
	COLORREF GetTransparentBackColor();
	void SetTransparentBackColor(COLORREF coTransparentBack);
	int GetEnlarge();
	void SetEnlarge(int nEnlarge);

protected:
	HBITMAP m_hBitmap;
	HICON m_hIcon;
	COLORREF m_coTransparentBack;
	SIZE m_szBmpSize;
	int m_nEnlarge;

	void Clear();
	void OnPaint(PaintEventArgs *e);
};

class ButtonConfig : public ObjectConfig
{
public:
	BASE_CONTROL_CONFIG_DEF;
};
//if use textcolor or _hIconHot,_hIconGray,need OWNERDRAW.
#define CC_FLAG_BUTTON_END				CC_FLAG_CONTROL_END
class Button : public Control
{
public:
	Button();
	virtual ~Button();

	void PerformClick(EventArgs *e);
	HBITMAP SetBitmap(HBITMAP hBitmap);
	HBITMAP GetBitmap() const;

protected:
	virtual void OnCreateControl();
	virtual void OnCommand(EventArgs *e);
	virtual void OnChildNotify(EventArgs *e);
	virtual void DoUpdate();
	virtual bool DoProcessMnemonic(TCHAR tchar);
	virtual bool PreCreateWindow(CREATESTRUCT& cs);

// Overridables (for owner draw only)
	virtual void OnDrawItem(DrawItemEventArgs* e);
};

class CheckBoxConfig : public ObjectConfig
{
public:
	BASE_CONTROL_CONFIG_DEF;
};
#define CC_CHECKBOX_BOXWH 16
#define CC_FLAG_CHECKBOX_END				CC_FLAG_CONTROL_END
class CheckBox : public Button
{
public:
	EventHandler CheckedChanged;

public:
	CheckBox();
	virtual ~CheckBox();

	void SetChecked(bool checked);
	bool GetChecked();
	virtual void SetIsDirty(bool isDirty);
	virtual bool GetIsDirty();

protected:
	//bool _IsChecked;
	COLORREF corBackgroundSaved;
	//keyArr is param address,but sHead is by value
	virtual void DoConfig(bool isFromIni, KeyArr& keyArr, cc::Str sThisHead);

	virtual LRESULT WndProc(EventArgs *e);
	virtual void OnHandleCreated(EventArgs *e);
	virtual void OnCheckedChanged(EventArgs *e);
	virtual SIZE DoMeasureSize(int ctlHeight);
};

class RadioButtonConfig : public ObjectConfig
{
public:
	BASE_CONTROL_CONFIG_DEF;
};
#define CC_FLAG_RADIOBUTTON_END				CC_FLAG_BUTTON_END
class RadioButton : public Button
{
public:
	EventHandler CheckedChanged;

public:
	RadioButton();
	virtual ~RadioButton();

	//if checked then also set focus.
	void SetChecked(bool checked);
	bool GetChecked();
	//Group:
	//  the first radio button should have both WS_GROUP | WS_TABSTOP in the control style.
	//  the remaining radio buttons should have neither of those, and must be in sibling order (meaning they immediately follow each other)
	//void SetGroupID(UINT nID);
	//UINT GetGroupID();

protected:
	//keyArr is param address,but sHead is by value
	virtual void DoConfig(bool isFromIni, KeyArr& keyArr, cc::Str sThisHead);

	virtual LRESULT WndProc(EventArgs *e);
	virtual void OnHandleCreated(EventArgs *e);
	virtual void OnCheckedChanged(EventArgs *e);
	virtual SIZE DoMeasureSize(int ctlHeight);
};

class GroupConfig : public ObjectConfig
{
public:
	BASE_CONTROL_CONFIG_DEF;
};
#define CC_FLAG_GROUP_END				CC_FLAG_BUTTON_END
//Group is Button with BS_GROUPBOX, but when xp theme it is not good for add button inner.
//instead of this, recommend to use GroupPanel
class Group : public Button
{
public:
	Group();
	virtual ~Group();

protected:
	//if not stop even WS_TABSTOP&WS_VISIBLE,need overwrite this
	virtual bool IsTabStop();
};

class SpinButtonConfig : public ObjectConfig
{
public:
	BASE_CONTROL_CONFIG_DEF;
};
#define CC_FLAG_SPINBUTTON_END				CC_FLAG_CONTROL_END
class SpinButton : public Control
{
public:
	SpinButton();
	virtual ~SpinButton();

	void GetRange(int& lower, int& upper);
	void SetRange(int lower, int upper);
	int GetBase();
	void SetBase(int value);
	Control* GetBuddy();
	void SetBuddy(Control* ctl);
	int GetPos();
	void SetPos(int value);

protected:
	int _nLower;
	int _nUpper;
	int _nBase;
	Control* _hBuddy;
	int _nPos;

	//keyArr is param address,but sHead is by value
	virtual void DoConfig(bool isFromIni, KeyArr& keyArr, cc::Str sThisHead);
	virtual void OnHandleCreated(EventArgs *e);
};

class ListBoxConfig : public ObjectConfig
{
public:
	BASE_CONTROL_CONFIG_DEF;
};
#define CC_FLAG_LISTBOX_END				CC_FLAG_CONTROL_END
class ListBox : public Control
{
public:
	EventHandler ItemDoubleClick;
	EventHandler SelectedIndexChanged;

public:
	ListBox();
	virtual ~ListBox();

	void SetSelectedIndex(int selectedIndex);
	int GetSelectedIndex() const;
	void SetSort(bool sort);
	bool GetSort();
	int ItemAdd(const TCHAR* txt, int index = -1);
	void ItemDel(const TCHAR* txt);
	void ItemDel(int index);
	void ItemSet(int index, const TCHAR* txt);
	const cc::Str ItemGet(int index);
	int ItemCount() const;
	void ItemClear();
	void ItemUp(int index);
	void ItemDown(int index);
	//if has more than one item,then if not isAppend do ItemClear
	void SetConfigData(const cc::Str& sData, bool isAppend = false);
	cc::Str GetConfigData();

	void SetHExtent(int width);
	int GetHExtent() const;

	int GetSelCount() const;
	int GetSelItems(int nMaxItems, LPINT rgIndex) const;
	int GetTopIndex() const;
	int SetTopIndex(int nIndex);
	virtual DWORD_PTR GetItemData(int nIndex) const;
	virtual int SetItemData(int nIndex, DWORD_PTR dwItemData);
	virtual void* GetItemDataPtr(int nIndex) const;
	virtual int SetItemDataPtr(int nIndex, void* pData);
	int GetItemRect(int nIndex, LPRECT lpRect) const;
	int GetSel(int nIndex) const;
	int SetSel(int nIndex, BOOL bSelect);
	int GetItemTextLen(int nIndex) const;
	void SetColumnWidth(int cxWidth);
	bool SetTabStops(int nTabStops, LPINT rgTabStops);
	void SetTabStops();
	bool SetTabStops(const int& cxEachStop);
	int SetItemHeight(int nIndex, UINT cyItemHeight);
	int GetItemHeight(int nIndex) const;
	int GetCaretIndex() const;
	int SetCaretIndex(int nIndex, bool bScroll);
	int Dir(UINT attr, LPCTSTR lpszWildCard);
	int SelectString(int nStartAfter, LPCTSTR lpszItem);
	int SelItemRange(bool bSelect, int nFirstItem, int nLastItem);
	void SetAnchorIndex(int nIndex);
	int GetAnchorIndex() const;
	LCID GetLocale() const;
	LCID SetLocale(LCID nNewLocale);
	int InitStorage(int nItems, UINT nBytes);

	int FindString(int nStartAfter, LPCTSTR lpszItem) const;
	int FindStringExact(int nIndexStart, LPCTSTR lpszFind) const;
	//earchType=1:startswith; searchType=2:endswith; searchType=else:indexof>=0; 
	int FindStringOption(int nIndexStart, LPCTSTR lpszFind, int searchType, bool IgnoreCase = false);

protected:
	//keyArr is param address,but sHead is by value
	virtual void DoConfig(bool isFromIni, KeyArr& keyArr, cc::Str sThisHead);
	//keyArr is param address,but sThisHead is by value
	virtual void DoSetLang(KeyArr& keyArr, cc::Str sThisHead);
	virtual void OnChildNotify(EventArgs *e);
	virtual void OnCommand(EventArgs *e);
	virtual void OnHandleCreated(EventArgs *e);
// Overridables (for owner draw only)
	//virtual void OnDrawItem(DrawItemEventArgs* e);
	//virtual void OnMeasureItem(MeasureItemEventArgs *e);
	//virtual void OnCompareItem(CompareItemEventArgs* e);
	//virtual void OnDeleteItem(EventArgs* e);
	//virtual void OnVKeyToItem(VKeyToItemEventArgs* e);
	//virtual void OnCharToItem(CharToItemEventArgs* e);

	virtual void OnItemDoubleClick(EventArgs *e);
	virtual void OnSelectedIndexChanged(EventArgs *e);
	//virtual void CopyItemData(void** hbmDes, void* hbmSrc);
};

class CheckListBoxConfig : public ObjectConfig
{
public:
	BASE_CONTROL_CONFIG_DEF;
	static COLORREF corTextgroundDisabled;
};
#define CC_CHECKLISTBOX_BOXH 16
struct CC_CHECK_DATA
{
public:
	//m_nChecks(multi checkbox,max is 4*8/2=16)
	//0000 0000 0000 0000
	//               ||||-----------> index 0 of checkbox,check status:0,not check; 1,checked; 2,grey checked
	// ...           ||-------------> index 1 of checkbox,check status:0,not check; 1,checked; 2,grey checked
	UINT m_nChecks;
	bool m_bEnabled;
	//LPVOID m_ItemData;
	DWORD_PTR m_dwUserData;
	UINT m_nUserFlags;
	COLORREF corTextground;
	COLORREF corHotTextColor;

	CC_CHECK_DATA()
	{
		m_nChecks = 0;
		m_bEnabled = TRUE;
		//m_ItemData = NULL;
		m_dwUserData = 0;
		m_nUserFlags = 0;
		//will init while NewItemData
		//corTextground = CheckListBoxConfig::corTextground;
		//corHotTextColor = CheckListBoxConfig::corHotTextColor;
	};
};

//for control CheckListBox
typedef struct tagCheckStyle
{
	enum Type
	{
		AutoCheckBox			= BS_AUTOCHECKBOX,
		Auto3State				= BS_AUTO3STATE,
	};
}CheckStyle;

//notice MeasureItem.Invoke((Object*)this, e); and no DrawItem.Invoke((Object*)this, e);
#define CC_FLAG_CHECKLISTBOX_END				CC_FLAG_LIXTBOX_END
class CheckListBox : public ListBox
{
public:
	EventHandler ItemCheckChanged;

public:
	CheckListBox();
	virtual ~CheckListBox();

// Attributes
	void SetCheckStyle(CheckStyle::Type eStyle);
	CheckStyle::Type GetCheckStyle();
	//count of multi check box,0~8
	void SetMultiBoxCount(int nMultiBoxCount);
	int GetMultiBoxCount();
	void SetCheck(int nIndex, int nCheck, int nHIndex = 0);
	int GetCheck(int nIndex, int nHIndex = 0);
	int GetCheckAll(int nIndex);
	int GetCheckedCount(int nHIndex = 0);
	void SetItemEnabled(int nIndex, bool bEnabled = true);
	bool GetItemEnabled(int nIndex);
	virtual DWORD_PTR GetItemData(int nIndex);
	virtual int SetItemData(int nIndex, DWORD_PTR dwItemData);
	virtual void* GetItemDataPtr(int nIndex);
	virtual int SetItemDataPtr(int nIndex, void* pData);
	//if has more than one item,then if not isAppend do ItemClear
	void SetConfigData(const cc::Str& sData, bool isAppend = false);
	cc::Str GetConfigData();

protected:
	CheckStyle::Type m_nStyle;
	int _nMultiBoxCount;
	int m_cyText;
	//while Deleting item,then GetItemData will get raw(user) data
	//int m_nDeletingIndex;
	Map<int, void*>* _mapData;

	//keyArr is param address,but sHead is by value
	virtual void DoConfig(bool isFromIni, KeyArr& keyArr, cc::Str sThisHead);
	//keyArr is param address,but sHead is by value
	//virtual void DoConfig(bool isFromIni, KeyArr& keyArr, cc::Str sThisHead);
    // WndProc - calls appriate On... function for the give
	virtual LRESULT WndProc(EventArgs *e);
	//virtual void OnCommand(EventArgs *e);
	virtual void OnChildNotify(EventArgs *e);
	virtual void OnMouseDown(MouseEventArgs *e);
	virtual void OnItemCheckChanged(EventArgs *e);
// Overridables (for owner draw only)
	virtual void PreMeasureItem(MeasureItemEventArgs *e);
	virtual void PreDrawItem(DrawItemEventArgs* e);
	virtual void PreCompareItem(CompareItemEventArgs* e);
	virtual void PreDeleteItem(EventArgs* e);
	virtual void OnMeasureItem(MeasureItemEventArgs *e);
	virtual void OnDrawItem(DrawItemEventArgs* e);
	//virtual void OnCompareItem(CompareItemEventArgs* e);
	virtual void OnDeleteItem(EventArgs* e);

	int CalcMinimumItemHeight();
	int CheckFromPoint(POINT point, BOOL& bInCheck, int& nHIndex);
	void SetSelectionCheck(int nCheck, int nHIndex = 0);
	void InvalidateCheck(int nIndex);
	void InvalidateItem(int nIndex);
	//virtual CC_CHECK_DATA* GetItemDataBase(int nIndex);
	//virtual void CopyItemData(void** hbmDes, void* hbmSrc);
	//virtual void* NewItemData();
	virtual int GetCustomDataIndex(int nID, bool isCreate);
};

//for control ComboBox
typedef struct tagComboBoxStyle
{
	enum Type
	{
		//while StatusItem, equals to Contents
		Simple					= CBS_SIMPLE,
		DropDown				= CBS_DROPDOWN,
		DropDownList			= CBS_DROPDOWNLIST,
	};
}ComboBoxStyle;

class ComboBoxConfig : public ObjectConfig
{
public:
	BASE_CONTROL_CONFIG_DEF;
	static COLORREF corBackgroundDisabled;
};
#define CC_FLAG_COMBOBOX_END				CC_FLAG_CONTROL_END
class ComboBox : public Control
{
public:
	//EventHandler ItemDoubleClick;
	EventHandler SelectedIndexChanged;

public:
	ComboBox();
	virtual ~ComboBox();
	virtual void Dispose();

	void SetEnabled(bool isEnabled);
	void SetComboBoxStyle(ComboBoxStyle::Type comboBoxStyle);
	ComboBoxStyle::Type GetComboBoxStyle();
	virtual void SetText(const TCHAR* sText);
	virtual const TCHAR* GetText();
	void SetSelectedIndex(int selectedIndex);
	int GetSelectedIndex() const;
	virtual int ItemAdd(const TCHAR* txt, int index = -1);
	void ItemDel(const TCHAR* txt);
	void ItemDel(int index);
	void ItemSet(int index, const TCHAR* txt);
	const cc::Str ItemGet(int index);
	int ItemCount() const;
	void ItemClear();
	//if has more than one item,then if not isAppend do ItemClear
	void SetConfigData(const cc::Str& sData, bool isAppend = false);
	cc::Str GetConfigData();

	DWORD GetEditSel() const;
	bool LimitText(int nMaxChars);
	bool SetEditSel(int nStartChar, int nEndChar);
	virtual DWORD_PTR GetItemData(int nIndex) const;
	virtual int SetItemData(int nIndex, DWORD_PTR dwItemData);
	virtual void* GetItemDataPtr(int nIndex) const;
	virtual int SetItemDataPtr(int nIndex, void* pData);
	int GetItemTextLen(int nIndex) const;
	void ShowDropDown(bool bShowIt);
	int Dir(UINT attr, LPCTSTR lpszWildCard);
	int SelectString(int nStartAfter, LPCTSTR lpszString);
	void Clear();
	void Copy();
	void Cut();
	void Paste();
	int SetItemHeight(int nIndex, UINT cyItemHeight);
	int GetItemHeight(int nIndex) const;
	int SetExtendedUI(BOOL bExtended );
	bool GetExtendedUI() const;
	void GetDroppedControlRect(LPRECT lprect) const;
	bool GetDroppedState() const;
	LCID GetLocale() const;
	LCID SetLocale(LCID nNewLocale);
	int GetTopIndex() const;
	int SetTopIndex(int nIndex);
	int InitStorage(int nItems, UINT nBytes);
	void SetHorizontalExtent(UINT nExtent);
	UINT GetHorizontalExtent() const;
	int SetDroppedWidth(UINT nWidth);
	int GetDroppedWidth() const;
	TextBox* GetTextBoxCtrl();

	int FindString(int nStartAfter, LPCTSTR lpszString) const;
	int FindStringExact(int nIndexStart, LPCTSTR lpszFind) const;
	int FindStringOption(int nIndexStart, LPCTSTR lpszFind, int searchType, bool IgnoreCase = false);

protected:
	TextBox* _pInnerTextBox;
	COLORREF corBackgroundSaved;

	//keyArr is param address,but sHead is by value
	virtual void DoConfig(bool isFromIni, KeyArr& keyArr, cc::Str sThisHead);
	//keyArr is param address,but sThisHead is by value
	virtual void DoSetLang(KeyArr& keyArr, cc::Str sThisHead);
	virtual void OnChildNotify(EventArgs *e);
	virtual void OnCommand(EventArgs *e);
	//here to create window
	virtual void CreateHandle();
	//virtual bool PreCreateWindow(CREATESTRUCT& cs);
	//virtual void OnHandleCreated(EventArgs* e);
	void innerTextBox_DoubleClick(Object* sender, EventArgs* e);
	//void innerTextBox_KeyDown(Object* sender, KeyEventArgs* e);
	void innerTextBox_KeyPress(Object* sender, KeyEventArgs* e);
	virtual void OnPaintBackground(PaintEventArgs *e);
// Overridables (for owner draw only)
	//virtual void OnDrawItem(DrawItemEventArgs* e);
	////virtual void OnMeasureItem(LPMEASUREITEMSTRUCT);
	//virtual void OnMeasureItem(MeasureItemEventArgs *e);
	//virtual void OnCompareItem(CompareItemEventArgs* e);
	//virtual void OnDeleteItem(EventArgs* e);
	//virtual void OnVKeyToItem(VKeyToItemEventArgs* e);
	//virtual void OnCharToItem(CharToItemEventArgs* e);

	//virtual void OnItemDoubleClick(EventArgs *e);
	virtual void OnSelectedIndexChanged(EventArgs *e);
};

class ComboBoxExConfig : public ObjectConfig
{
public:
	BASE_CONTROL_CONFIG_DEF;
};
#define CC_FLAG_COMBOBOXEX_END				CC_FLAG_COMBOBOX_END
class ComboBoxEx : public ComboBox
{
public:
	ComboBoxEx();
	virtual ~ComboBoxEx();
	virtual void Dispose();

	DWORD GetExtendedStyle() const;
	DWORD SetExtendedStyle(DWORD dwExMask, DWORD dwExStyles);
	BOOL HasEditChanged();
	ComboBox* GetComboBoxCtrl();
	//iImage:The zero-based index of an image within the image list. The specified image will be displayed for the item when it is not selected. If this member is set to I_IMAGECALLBACK, the control will request the information by using CBEN_GETDISPINFO notification messages. 
	//iSelectedImage:The zero-based index of an image within the image list. The specified image will be displayed for the item when it is selected. If this member is set to I_IMAGECALLBACK, the control will request the information by using CBEN_GETDISPINFO notification messages. 
	//iIndent:The number of indent spaces to display for the item. Each indentation equals 10 pixels. If this member is set to I_INDENTCALLBACK, the control will request the information by using CBEN_GETDISPINFO notification messages.
	virtual int ItemAdd(const TCHAR* txt, int index = -1);
	int ItemAdd(const TCHAR* txt, int index, int iImage, int iSelectedImage, int iIndent = 0);

	HIMAGELIST SetImageList(HIMAGELIST hImageList);
	HIMAGELIST GetImageList() const;

protected:
	ComboBox* _pInnerComboBox;

	// While ComboBoxEx derives from CComboBox, there are some
	// CB_messages the underlying ComboBoxEx control doesn't support.
	int Dir(UINT attr, LPCTSTR lpszWildCard);
	BOOL SetEditSel(int nStartChar, int nEndChar);
	int FindString(int nIndexStart, LPCTSTR lpszFind) const {return -1;};
	int InsertString(int nIndex, LPCTSTR lpszString) {return -1;};
	virtual void CreateHandle();
	virtual void OnPaintBackground(PaintEventArgs *e);
};

class TextBoxConfig : public ObjectConfig
{
public:
	BASE_CONTROL_CONFIG_DEF;
	static COLORREF corBackgroundDisabled;
	static COLORREF corBackgroundReadOnly;
};
#define CC_FLAG_TEXTBOX_END				CC_FLAG_CONTROL_END
class TextBox : public Control
{
public:
	EventHandler MaxText;

public:
	//Constructors
	TextBox();
	//destructor
	virtual ~TextBox();

	void SetEnabled(bool isEnabled);
	void SetReadOnly(bool isReadOnly);
	bool GetReadOnly();
	//notice that isMultiline can't be changed after the control has been created.
	void SetMultiline(bool isMultiline, bool isHScroll = false, bool isVScroll = true);
	bool GetMultiline();
	void SetMaxLength(int nMaxLength);
	int GetMaxLength();
	void SetStyleNumber(bool value);
	void SetStyleUpper(bool value);
	virtual void SetIsDirty(bool isDirty);
	virtual bool GetIsDirty();
	int GetTextInt();
	double GetTextDouble();
	void SetTextInt(int nText);

	virtual void Clear();
	virtual void Copy();
	virtual void Paste();
	virtual void Cut();
	virtual bool Undo();
	virtual void AppendText(const TCHAR* txt, bool isScrollBottom = false);
	virtual void AppendText(bool isScrollBottom, const TCHAR* strFormat, ...);

	bool CanUndo() const;
	int CharFromPos(POINT pt) const;
	void EmptyUndoBuffer();
	bool FmtLines(bool bAddEOL);
	int GetFirstVisibleLine() const;
	HLOCAL GetHandle() const;
	UINT GetLimitText() const;
	int GetLine(int nIndex, LPTSTR lpszBuffer) const;
	int GetLine(int nIndex, LPTSTR lpszBuffer, int nMaxLength) const;
	int GetLineCount() const;
	DWORD GetMargins() const;
	bool GetModify() const;
	TCHAR GetPasswordChar() const;
	void GetRect(LPRECT lpRect) const;
	void GetSel(int& nStartChar, int& nEndChar) const;
	DWORD GetSel() const;
	//The EM_SETLIMITTEXT message is identical to the EM_LIMITTEXT message.
	//void LimitText(int nChars = 0);
	int LineFromChar(int nIndex =-1) const;
	int LineIndex(int nLine =-1) const;
	int LineLength(int nLine =-1) const;
	void LineScroll(int nLines, int nChars = 0);
	POINT PosFromChar(UINT nChar) const;
	void ReplaceSel(LPCTSTR lpszNewText, bool bCanUndo = false);
	void SetHandle(HLOCAL hBuffer);
	void SetLimitText(UINT nMax);
	void SetMargins(UINT nLeft, UINT nRight);
	void SetModify(bool bModified = true);
	void SetPasswordChar(TCHAR ch);
	void SetRect(LPCRECT lpRect);
	void SetRectNP(LPCRECT lpRect);
	void SetSel(DWORD dwSelection, bool bNoScroll = false);
	void SetSel(int nStartChar, int nEndChar, bool bNoScroll = false);
	bool SetTabStops(const int& cxEachStop);
	bool SetTabStops(int nTabStops, LPINT rgTabStops);
	void SetTabStops();

protected:
	int _nMaxLength;
	COLORREF corBackgroundSaved;

	//keyArr is param address,but sHead is by value
	virtual void DoConfig(bool isFromIni, KeyArr& keyArr, cc::Str sThisHead);
	virtual void OnCreateControl();
	virtual bool IsInputKey(UINT_PTR& keyData);

	//virtual void OnResize(EventArgs *e);
	virtual void OnTextChanged(EventArgs *e);
	virtual void OnCommand(EventArgs *e);
	virtual void OnMaxText(EventArgs *e);
};

//i donot want to include <richole.h>
//#include <richole.h>
const IID cc_IID_IRichEditOleCallback = {0x00020D03, 0, 0, 0xC0,0,0,0,0,0,0,0x46};
#undef INTERFACE
#define INTERFACE   cc_IRichEditOleCallback
DECLARE_INTERFACE_(cc_IRichEditOleCallback, IUnknown)
{
    // *** IUnknown methods ***
    STDMETHOD(QueryInterface) (THIS_ REFIID riid, LPVOID FAR * lplpObj) PURE;
    STDMETHOD_(ULONG,AddRef) (THIS)  PURE;
    STDMETHOD_(ULONG,Release) (THIS) PURE;

    // *** IRichEditOleCallback methods ***
	STDMETHOD(GetNewStorage) (THIS_ LPSTORAGE FAR * lplpstg) PURE;
    STDMETHOD(GetInPlaceContext) (THIS_ LPOLEINPLACEFRAME FAR * lplpFrame,
								  LPOLEINPLACEUIWINDOW FAR * lplpDoc,
								  LPOLEINPLACEFRAMEINFO lpFrameInfo) PURE;
	STDMETHOD(ShowContainerUI) (THIS_ BOOL fShow) PURE;
	STDMETHOD(QueryInsertObject) (THIS_ LPCLSID lpclsid, LPSTORAGE lpstg,
									LONG cp) PURE;
	STDMETHOD(DeleteObject) (THIS_ LPOLEOBJECT lpoleobj) PURE;
	STDMETHOD(QueryAcceptData) (THIS_ LPDATAOBJECT lpdataobj,
								CLIPFORMAT FAR * lpcfFormat, DWORD reco,
								BOOL fReally, HGLOBAL hMetaPict) PURE;
	STDMETHOD(ContextSensitiveHelp) (THIS_ BOOL fEnterMode) PURE;
	STDMETHOD(GetClipboardData) (THIS_ CHARRANGE FAR * lpchrg, DWORD reco,
									LPDATAOBJECT FAR * lplpdataobj) PURE;
	STDMETHOD(GetDragDropEffect) (THIS_ BOOL fDrag, DWORD grfKeyState,
									LPDWORD pdwEffect) PURE;
	STDMETHOD(GetContextMenu) (THIS_ WORD seltype, LPOLEOBJECT lpoleobj,
									CHARRANGE FAR * lpchrg,
									HMENU FAR * lphmenu) PURE;
};
class RichEditOleCallback : cc_IRichEditOleCallback
{
public:
	// Constructor / Destructor
	RichEditOleCallback()
	{
		//if from 1,then need Release once where new it.
		//here is from 0,so need new,AddRef,EM_SETOLECALLBACK,Release
		mRefCounter = 0;
	}
	~RichEditOleCallback()
	{
	}

	// Methods of the IUnknown interface
	STDMETHOD_(ULONG, AddRef) (void) { mRefCounter++; return mRefCounter; }
	STDMETHOD_(ULONG, Release) (void) { if(--mRefCounter == 0) try{delete this;}catch(...){}; return mRefCounter; }
	STDMETHOD(QueryInterface) (REFIID iid, void** ppvObject)
	{
		if(iid == IID_IUnknown || iid == cc_IID_IRichEditOleCallback)
		{
			*ppvObject = this;
			AddRef();
			return S_OK;
		}
		else
		{
			return E_NOINTERFACE;
		}
	}

	// Methods of the IRichEditOleCallback interface
	STDMETHOD(ContextSensitiveHelp) (BOOL fEnterMode) { return E_NOTIMPL; }
	STDMETHOD(DeleteObject) (LPOLEOBJECT lpoleobj) { return E_NOTIMPL; }
	STDMETHOD(GetClipboardData) (CHARRANGE FAR *lpchrg, DWORD reco, LPDATAOBJECT FAR *lplpdataobj) { return E_NOTIMPL; }
	STDMETHOD(GetContextMenu) (WORD seltype, LPOLEOBJECT lpoleobj, CHARRANGE FAR *lpchrg, HMENU FAR *lphmenu) { return E_NOTIMPL; }
	STDMETHOD(GetDragDropEffect) (BOOL fDrag, DWORD grfKeyState, LPDWORD pdwEffect)
	{
		*pdwEffect = DROPEFFECT_COPY;
		//return E_NOTIMPL for move
		return S_OK;
	}
	STDMETHOD(GetInPlaceContext) (LPOLEINPLACEFRAME FAR *lplpFrame, LPOLEINPLACEUIWINDOW FAR *lplpDoc, LPOLEINPLACEFRAMEINFO lpFrameInfo) { return E_NOTIMPL; }
	STDMETHOD(GetNewStorage) (LPSTORAGE FAR *lplpstg);
	STDMETHOD(QueryAcceptData) (LPDATAOBJECT lpdataobj, CLIPFORMAT FAR *lpcfFormat, DWORD reco, BOOL fReally, HGLOBAL hMetaPict) { return E_NOTIMPL; }
	STDMETHOD(QueryInsertObject) (LPCLSID lpclsid, LPSTORAGE lpstg, LONG cp) { return S_OK; }
	STDMETHOD(ShowContainerUI) (BOOL fShow) { return E_NOTIMPL; }

	// Data
protected:
	UINT mRefCounter;
};

// cookie class to assist with streaming a cc::Str into the richedit control
class RichTextBoxStreamCookie : public Object
{
public:
	cc::Str m_sText;

	RichTextBoxStreamCookie(const char* sText) : m_sText(cc::Str(LangCode::SYSTEM, sText, LangCode::SYSTEM))
	{
		m_dwCount = 0;
		m_dwLength = m_sText.GetLength();
	}

	DWORD Read(LPBYTE lpszBuffer, DWORD dwCount);
	DWORD Write(LPBYTE lpszBuffer, DWORD dwCount);
	void Reset()
	{
		m_dwCount = 0;
		m_dwLength = m_sText.GetLength();
	}

	static DWORD CALLBACK CallbackStreamIn(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb);
	static DWORD CALLBACK CallbackStreamOut(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb);

protected:
	DWORD m_dwLength;
	DWORD m_dwCount;
};

#define CC_FLAG_RICHTEXTBOX_END				CC_FLAG_TEXTBOX_END
class RichTextBox : public TextBox
{
//public:
//	EventHandler ClickLink;

public:
	//Constructors
	RichTextBox();
	//destructor
	virtual ~RichTextBox();
	virtual const cc::Str GetTextSel();

	BOOL SetAutoURLDetect(BOOL bEnable = TRUE);
	void GetSel(CHARRANGE &cr) const;
	void LimitText(long nChars);
	long LineFromChar(long nIndex) const;
	void SetSel(CHARRANGE &cr);
	DWORD FindWordBreak(UINT nCode, DWORD nStart) const;
	POINT PosFromChar(UINT nChar) const;
	int CharFromPos(POINT pt) const;
	//COLORREF SetBackgroundColor(BOOL bSysColor, COLORREF cr);
	BOOL SetOLECallback(cc_IRichEditOleCallback* pCallback);
	long StreamIn(int nFormat, EDITSTREAM &es);
	long StreamOut(int nFormat, EDITSTREAM &es);
	long StreamIn(int nFormat, const char* buf);
	long StreamOut(int nFormat, cc::Str& buf);

	void SetBackground(COLORREF corBackground);

protected:
	cc_IRichEditOleCallback* m_oleCallback;

	//here to create window
	virtual void CreateHandle();
	virtual void OnChildNotify(EventArgs *e);
	//virtual HFONT CreateDefaultFont();
};

class ScrollBarConfig : public ObjectConfig
{
public:
	BASE_CONTROL_CONFIG_DEF;
};
#define CC_FLAG_SCROLLBAR_END				CC_FLAG_CONTROL_END
class ScrollBar : public Control
{
public:
	EventHandler ValueChanged;

public:
	ScrollBar();
	virtual ~ScrollBar();

	int GetMinValue();
	int GetMaxValue();
	void SetMinMaxValue(int nMin, int nMax, bool bRedraw = true);
	//void SetMinimum(int nMinimum, bool bRedraw = true);
	//void SetMaximum(int nMaximum, bool bRedraw = true);
	int SetValue(int nValue, bool bRedraw = true);
	int GetValue();
	void SetLargeChange(int nLargeChange);
	int GetLargeChange();
	void SetSmallChange(int nSmallChange);
	int GetSmallChange();

	void ShowScrollBar(BOOL bShow);
	//SB_VERT,SB_HORZ,SB_BOTH
	bool EnableScrollBar(UINT nArrowFlags);
	//int GetScrollLimit();
	//void SetScrollRange(int nMinPos, int nMaxPos, bool bRedraw);
	//void GetScrollRange(LPINT lpMinPos, LPINT lpMaxPos) const;

protected:
	SCROLLINFO si;
	int _nMinimum;
	int _nMaximum;
	int _nLargeChange;
	int _nSmallChange;
	int _nValue;

	virtual void OnHandleCreated(EventArgs *e);
	virtual void OnChildNotify(EventArgs *e);
	virtual void OnScroll(ScrollEventArgs *e);
	//int nValue = (int)e->LParam;
	virtual void OnValueChanged(EventArgs *e);
	virtual void DefaultScroll(ScrollEventArgs *e);
};

#define CC_FLAG_HSCROLLBAR_END				CC_FLAG_SCROLLBAR_END
class HScrollBar : public ScrollBar
{
public:
	HScrollBar();
	virtual ~HScrollBar();
};

#define CC_FLAG_VSCROLLBAR_END				CC_FLAG_SCROLLBAR_END
class VScrollBar : public ScrollBar
{
public:
	VScrollBar();
	virtual ~VScrollBar();
};

class ProgressBarConfig : public ObjectConfig
{
public:
	BASE_CONTROL_CONFIG_DEF;
	static int isStyleSmooth;
};
#define CC_FLAG_PROGRESSBAR_END				CC_FLAG_CONTROL_END
class ProgressBar : public Control
{
public:
	EventHandler ValueChanged;

public:
	ProgressBar();
	virtual ~ProgressBar();

	//PBM_SETBARCOLOR
	void SetTextColor(COLORREF corText);
	void SetBackground(COLORREF corBackground);

	void SetRange16(short nLower, short nUpper);
	void SetRange(int nLower, int nUpper);
	void GetRange(int& nLower, int& nUpper);
	int GetPos();
	void SetPos(int nPos);
	void SetStep(int nStep);
	int GetStep();

	void OffsetPos(int nPos);
	int StepIt();

protected:
	int _nLower;
	int _nUpper;
	int _nPos;
	int _nOffPos;
	int _nStep;
	int _nStepIt;

	virtual void OnHandleCreated(EventArgs *e);
};

class ListViewConfig : public ObjectConfig
{
public:
	BASE_CONTROL_CONFIG_DEF;
};
/*
//get selected
int   nItem   =   listctrl.GetNextItem   (-1,   LVNI_SELECTED);  
//cancel selected
listctrl.SetItemState(nItem,NULL,NULL);  
//set selected
listctrl.SetItemState(nItem+1,LVNI_SELECTED,LVNI_SELECTED);	
*/
#define CC_FLAG_LISTVIEW_END				CC_FLAG_CONTROL_END
class ListView : public Control
{
public:
	//EventHandler ItemClick;
	EventHandler ItemDoubleClick;
	EventHandler SelectedIndexChanging;
	EventHandler SelectedIndexChanged;
	EventHandler SetDispinfo;
	EventHandler GetDispinfo;
	EventHandler BeginLabelEdit;
	EventHandler EndLabelEdit;
	EventHandler RClick;

	DragBeginEventHandler DragBegin;
	MouseEventHandler DragMove;
	DragEndEventHandler DragEnd;

public:
	ListView();
	virtual ~ListView();

	BOOL GetItemRect(int nItem, LPRECT lpRect, UINT nCode) const;
	BOOL SetItemCountEx(int iCount, DWORD dwFlags = LVSICF_NOINVALIDATEALL);
	SIZE SetIconSpacing(int cx, int cy);
	SIZE SetIconSpacing(SIZE size);
	BOOL GetSubItemRect(int iItem, int iSubItem, int nArea, RECT& ref);
	int InsertColumn(int nCol, LPCTSTR lpszColumnHeading, int nFormat, int nWidth, int nSubItem = -1);
	int ItemAdd(UINT nMask, int nItem, LPCTSTR lpszItem, UINT nState, UINT nStateMask, int nImage, LPARAM lParam);
	int HitTest(POINT pt, UINT* pFlags) const;
	BOOL SetItem(int nItem, int nSubItem, UINT nMask, LPCTSTR lpszItem,
		int nImage, UINT nState, UINT nStateMask, LPARAM lParam);
	BOOL SetItem(int nItem, int nSubItem, UINT nMask, LPCTSTR lpszItem,
		int nImage, UINT nState, UINT nStateMask, LPARAM lParam, int nIndent);
	BOOL SetItemState(int nItem, UINT nState, UINT nStateMask);
	BOOL SetItemText(int nItem, int nSubItem, LPCTSTR lpszText);
	cc::Str GetItemText(int nItem, int nSubItem) const;
	int GetItemText(int nItem, int nSubItem, LPTSTR lpszText, int nLen) const;
	virtual DWORD_PTR GetItemData(int nItem) const;
	void RemoveImageList(int nImageList);
	ImageList* CreateDragImage(int nItem, LPPOINT lpPoint);

	int GetColumnCount();
	BOOL SetColumnOrderArray(int iCount, LPINT piArray);
	BOOL GetColumnOrderArray(LPINT piArray, int iCount = -1);
	BOOL SetBkImage(HBITMAP hbm, BOOL fTile = TRUE, int xOffsetPercent = 0, int yOffsetPercent = 0);
	BOOL SetBkImage(LPTSTR pszUrl, BOOL fTile = TRUE, int xOffsetPercent = 0, int yOffsetPercent = 0);
	BOOL GetCheck(int nItem) const;
	BOOL SetCheck(int nItem, BOOL fCheck);

	//CToolTipCtrl* GetToolTips() const;
	//CToolTipCtrl* SetToolTips(CToolTipCtrl* pTip);
	COLORREF GetBackground() const;
	//BOOL SetBkColor(COLORREF cr);
	virtual void SetBackground(COLORREF corBackground);
	//ImageList* GetImageList(int nImageListType) const;
	//ImageList* SetImageList(ImageList* pImageList, int nImageList);
	HIMAGELIST GetImageList(int nImageListType) const;
	HIMAGELIST SetImageList(HIMAGELIST hImageList, int nImageList);
	int GetItemCount() const;
	BOOL GetItem(LVITEM* pItem) const;
	BOOL SetItem(const LVITEM* pItem);
	virtual BOOL SetItemData(int nItem, DWORD_PTR dwData);
	int ItemAdd(const LVITEM* pItem);
	int ItemAdd(int nItem, LPCTSTR lpszItem);
	int ItemAdd(int nItem, LPCTSTR lpszItem, int nImage);
	BOOL ItemDel(int nItem);
	BOOL ItemClear();
	UINT GetCallbackMask() const;
	BOOL SetCallbackMask(UINT nMask);
	int GetNextItem(int nItem, int nFlags) const;
//POSITION
	//POINT GetFirstSelectedItemPosition() const;
	//int GetNextSelectedItem(POINT& pos) const;
	int FindItem(LVFINDINFO* pFindInfo, int nStart) const;
	int HitTest(LVHITTESTINFO* pHitTestInfo) const;
	BOOL SetItemPosition(int nItem, POINT pt);
	BOOL GetItemPosition(int nItem, LPPOINT lpPoint) const;
	int GetStringWidth(LPCTSTR lpsz) const;
	BOOL EnsureVisible(int nItem, BOOL bPartialOK);
	BOOL DoScroll(SIZE size);
	BOOL RedrawItems(int nFirst, int nLast);
	BOOL Arrange(UINT nCode);
	//EditBox* EditLabel(int nItem);
	//EditBox* GetEditControl() const;
	BOOL GetColumn(int nCol, LVCOLUMN* pColumn) const;
	BOOL SetColumn(int nCol, const LVCOLUMN* pColumn);
	int InsertColumn(int nCol, const LVCOLUMN* pColumn);
	BOOL DeleteColumn(int nCol);
	int GetColumnWidth(int nCol) const;
	BOOL SetColumnWidth(int nCol, int cx);
	BOOL GetViewRect(LPRECT lpRect) const;
	COLORREF GetTextColor() const;
	BOOL SetTextColor(COLORREF cr);
	COLORREF GetTextBkColor() const;
	BOOL SetTextBkColor(COLORREF cr);
	int GetTopIndex() const;
	int GetCountPerPage() const;
	BOOL GetOrigin(LPPOINT lpPoint) const;
	BOOL Update(int nItem);
	BOOL SetItemState(int nItem, LVITEM* pItem);
	UINT GetItemState(int nItem, UINT nMask) const;
	void SetItemCount(int nItems);
	BOOL SortItems(PFNLVCOMPARE pfnCompare, DWORD_PTR dwData);
	UINT GetSelectedCount() const;

	DWORD SetExtendedStyle(DWORD dwNewStyle);
	HCURSOR SetHotCursor(HCURSOR hc);
	int SetHotItem(int iIndex);
	void SetWorkAreas(int nWorkAreas, LPRECT lpRect);
	int SubItemHitTest(LPLVHITTESTINFO pInfo);
	HCURSOR GetHotCursor();
	int GetHotItem();
	DWORD GetExtendedStyle();
	SIZE ApproximateViewRect(SIZE sz /*= {-1, -1}*/, int iCount = -1) const;
	BOOL GetBkImage(LVBKIMAGE* plvbkImage) const;
	DWORD GetHoverTime() const;
	void GetWorkAreas(int nWorkAreas, LPRECT prc) const;
	BOOL SetBkImage(LVBKIMAGE* plvbkImage);
	DWORD SetHoverTime(DWORD dwHoverTime);
	UINT GetNumberOfWorkAreas() const;
	int SetSelectionMark(int iIndex);
	int GetSelectionMark();
	
protected:
	void OnChildNotify(EventArgs *e);
	void OnNcDestroy();
	virtual void OnRClick(EventArgs *e);
	//virtual void OnItemClick(EventArgs *e);
	virtual void OnItemDoubleClick(EventArgs *e);
	virtual void OnSelectedIndexChanging(EventArgs *e);
	virtual void OnSelectedIndexChanged(EventArgs *e);
	//virtual void OnDrawItem(DrawItemEventArgs* e);
	virtual void OnSetDispinfo(EventArgs *e);
	virtual void OnGetDispinfo(EventArgs *e);
	virtual void OnBeginLabelEdit(EventArgs *e);
	virtual void OnEndLabelEdit(EventArgs *e);

	virtual void OnDragBegin(DragBeginEventArgs *e);
	virtual void OnDragMove(MouseEventArgs *e);
	virtual void OnDragEnd(DragEndEventArgs *e);
};

class TreeViewConfig : public ObjectConfig
{
public:
	BASE_CONTROL_CONFIG_DEF;
	//while dragover,scrolling
	static UINT nWaitTimerDragOver;
};
#define CC_ACCESS_ITEM_DISIABLED		0x00000001
#define CC_ACCESS_CHKBOX_DISIABLED		0x00000002

//#define CC_FLAG_TREEVIEW_DRAGGING			CC_FLAG_CONTROL_END
#define CC_FLAG_TREEVIEW_NOCUSTOMDRAW		CC_FLAG_CONTROL_END <<1
#define CC_FLAG_TREEVIEW_LEFTCLICKDRAG		CC_FLAG_CONTROL_END <<2
#define CC_FLAG_TREEVIEW_RIGHTCLICKDRAG		CC_FLAG_CONTROL_END <<3
#define CC_FLAG_TREEVIEW_LOSTFOCUSSHOW		CC_FLAG_CONTROL_END <<4
#define CC_FLAG_TREEVIEW_END				CC_FLAG_CONTROL_END <<5
//if has TVS_CHECKBOXES then need SetImageList
class TreeView : public Control
{
public:
	EventHandler ItemDoubleClick;	
	EventHandler SelectedIndexChanging;
	EventHandler SelectedIndexChanged;
	EventHandler ItemExpanded;
	EventHandler ItemExpanding;
	EventHandler SingleExpand;
	EventHandler SetDispinfo;
	EventHandler GetDispinfo;
	EventHandler BeginLabelEdit;
	EventHandler EndLabelEdit;
	//EventHandler ItemDelete;
	EventHandler RClick;
	EventHandler ItemCheckChanged;
	EventHandler CustomDraw;

	DragBeginEventHandler DragBegin;
	MouseEventHandler DragMove;
	//WParam for drop item,LParam for drag(source) item
	DragEndEventHandler DragEnd;

public:
	TreeView();
	virtual ~TreeView();
	cc::Str GetItemText(HTREEITEM hItem, int nLen) const;
	int GetItemText(HTREEITEM hItem, LPTSTR lpszText, int nLen) const;
	virtual DWORD_PTR GetItemData(HTREEITEM hItem) const;
	BOOL GetItemChecked(HTREEITEM hItem) const;
	BOOL SetItemChecked(HTREEITEM hItem, BOOL fCheck, BOOL isUpdateParentChild = TRUE);
	void SetDragType(LeftRight::Type type, bool value);
	bool GetDragType(LeftRight::Type type);

	BOOL SetItem(HTREEITEM hItem, UINT nMask, LPCTSTR lpszItem, int nImage,
		int nSelectedImage, UINT nState, UINT nStateMask, LPARAM lParam);
	HTREEITEM ItemAdd(UINT nMask, LPCTSTR lpszItem, int nImage,
		int nSelectedImage, UINT nState, UINT nStateMask, LPARAM lParam,
		HTREEITEM hParent, HTREEITEM hInsertAfter);

	HTREEITEM ItemAdd(LPTVINSERTSTRUCT lpInsertStruct);
	HTREEITEM ItemAdd(LPCTSTR lpszItem, int nImage, int nSelectedImage, HTREEITEM hParent, HTREEITEM hInsertAfter);
	HTREEITEM ItemAdd(LPCTSTR lpszItem, HTREEITEM hParent, HTREEITEM hInsertAfter);
	BOOL ItemDel(HTREEITEM hItem);
	BOOL ItemClear();
	BOOL Expand(HTREEITEM hItem, UINT nCode);
	UINT GetCount() const;
	UINT GetIndent() const;
	void SetIndent(UINT nIndent);
	//ImageList* GetImageList(UINT nImageList) const;
	//ImageList* SetImageList(ImageList* pImageList, int nImageListType);
	HIMAGELIST GetImageList(int nImageListType) const;
	HIMAGELIST SetImageList(HIMAGELIST hImageList, int nImageList);
	 // Sets the maximum time (in milliseconds) the control will
	 // spend smooth scrolling its content.
	UINT SetScrollTime(UINT uScrollTime);
	HTREEITEM GetNextItem(HTREEITEM hItem, UINT nCode) const;
	HTREEITEM GetChildItem(HTREEITEM hItem) const;
	HTREEITEM GetNextSiblingItem(HTREEITEM hItem) const;
	HTREEITEM GetPrevSiblingItem(HTREEITEM hItem) const;
	HTREEITEM GetParentItem(HTREEITEM hItem) const;
	HTREEITEM GetFirstVisibleItem() const;
	HTREEITEM GetNextVisibleItem(HTREEITEM hItem) const;
	HTREEITEM GetPrevVisibleItem(HTREEITEM hItem) const;
	HTREEITEM GetSelectedItem() const;
	 // Retrieves the maximum time (in milliseconds) the control will
	 // spend smooth scrolling its content.
	UINT GetScrollTime() const;
	HTREEITEM GetDropHilightItem() const;
	HTREEITEM GetRootItem() const;
	BOOL Select(HTREEITEM hItem, UINT nCode);
	BOOL SelectItem(HTREEITEM hItem);
	BOOL SelectDropTarget(HTREEITEM hItem);
	BOOL SelectSetFirstVisible(HTREEITEM hItem);
	BOOL GetItem(TVITEM* pItem) const;
	BOOL SetItem(TVITEM* pItem);
	BOOL SetItemText(HTREEITEM hItem, LPCTSTR lpszItem);
	BOOL SetItemImage(HTREEITEM hItem, int nImage, int nSelectedImage);
	virtual BOOL SetItemState(HTREEITEM hItem, UINT nState, UINT nStateMask);
	virtual BOOL SetItemData(HTREEITEM hItem, DWORD_PTR dwData);
	//CEdit* EditLabel(HTREEITEM hItem);
	HTREEITEM HitTest(TVHITTESTINFO* pHitTestInfo) const;
	//CEdit* GetEditControl() const;
	UINT GetVisibleCount() const;
	BOOL SortChildren(HTREEITEM hItem);
	BOOL EnsureVisible(HTREEITEM hItem);
	BOOL SortChildrenCB(LPTVSORTCB pSort);
	//CToolTipCtrl* GetToolTips() const;
	//CToolTipCtrl* SetToolTips(CToolTipCtrl* pWndTip);
	COLORREF GetBackground() const;
	//COLORREF SetBkColor(COLORREF clr);
	virtual void SetBackground(COLORREF corBackground);
	SHORT GetItemHeight() const;
	SHORT SetItemHeight(SHORT cyHeight);
	COLORREF GetTextColor() const;
	COLORREF SetTextColor(COLORREF clr);
	BOOL SetInsertMark(HTREEITEM hItem, BOOL fAfter);
	COLORREF GetInsertMarkColor() const;
	COLORREF SetInsertMarkColor(COLORREF clrNew);

	// Retrieves the bounding rectangle for the specified item.
	BOOL GetItemRect(HTREEITEM hItem, LPRECT lpRect, BOOL bTextOnly) const;
	// Determines if the specified item has children.
	BOOL ItemHasChildren(HTREEITEM hItem) const;
	void SetLostFocusShow(bool value);
	bool GetLostFocusShow();

#if _WIN32_IE >= 0x0500
	COLORREF GetLineColor() const;
	COLORREF SetLineColor(COLORREF clrNew /*= CLR_DEFAULT*/);
#endif

protected:
	HWND _hEdit;
	HTREEITEM hItemDrag;
	Label *labLineTop;

	//keyArr is param address,but sHead is by value
	//virtual void DoConfig(bool isFromIni, KeyArr& keyArr, cc::Str sThisHead);
    // WndProc - calls appriate On... function for the give
	virtual LRESULT WndProc(EventArgs *e);
	//for child class to overwrite to control item disabled or checkbox disabled
	virtual DWORD GetAccessFlag(HTREEITEM hItem);
	//virtual void OnCtlColorEdit(EventArgs *e);
	virtual void OnChildNotify(EventArgs *e);
	virtual void OnRClick(EventArgs *e);

	virtual void OnItemDoubleClick(EventArgs *e);
	virtual void OnSelectedIndexChanging(EventArgs *e);
	virtual void OnSelectedIndexChanged(EventArgs *e);
	virtual void OnItemExpanded(EventArgs *e);
	virtual void OnItemExpanding(EventArgs *e);
	virtual void OnSingleExpand(EventArgs *e);
	virtual void OnSetDispinfo(EventArgs *e);
	virtual void OnGetDispinfo(EventArgs *e);
	virtual void OnBeginLabelEdit(EventArgs *e);
	virtual void OnEndLabelEdit(EventArgs *e);
	//virtual void OnItemDelete(EventArgs *e);
	virtual void OnItemCheckChanged(EventArgs *e);
	virtual void OnCustomDraw(EventArgs *e);

	virtual void OnInnerTimer(EventArgs *e);
	virtual void OnDragBegin(DragBeginEventArgs *e);
	virtual void OnDragMove(MouseEventArgs *e);
	virtual void OnDragEnd(DragEndEventArgs *e);
	virtual void OnGotFocus(EventArgs *e);
	virtual void OnLostFocus(EventArgs *e);
};

class DateTimePickerConfig : public ObjectConfig
{
public:
	BASE_CONTROL_CONFIG_DEF;
};
#define CC_FLAG_DATETIMEPICKER_END				CC_FLAG_CONTROL_END
class DateTimePicker : public Control
{
public:
	DateTimePicker();
	virtual ~DateTimePicker();

	virtual void SetText(const TCHAR* sText);
	virtual void SetText(const cc::Str& sText);

protected:
};

class MonthCalendarConfig : public ObjectConfig
{
public:
	BASE_CONTROL_CONFIG_DEF;
};
#define CC_FLAG_MONTHCALENDAR_END				CC_FLAG_CONTROL_END
class MonthCalendar : public Control
{
public:
	MonthCalendar();
	virtual ~MonthCalendar();

protected:
};

class SplitterConfig : public ObjectConfig
{
public:
	BASE_CONTROL_CONFIG_DEF;
};
#define CC_FLAG_SPLITTER_END				CC_FLAG_CONTROL_END
class Splitter : public Control
{
public:
	Splitter();
	virtual ~Splitter();
};

class ContainerConfig : public ObjectConfig
{
public:
	BASE_CONTROL_CONFIG_DEF;
	static int nMarginLeft;
	static int nMarginTop;
	static int nMarginRight;
	static int nMarginBottom;
};
#define CC_FLAG_CONTAINERCONTROL_END				CC_FLAG_CONTROL_END
class Container : public Control
{
public:
	//Event
	EventHandler Load;

public:
	Container();
	virtual ~Container();
	virtual void Dispose();

	void DisposeItemAdd(Object* needDisposeObj);

protected:
	List<Object*>* _lstDispose;

	static LRESULT CALLBACK CC_ContainerProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	virtual void OnLoad(EventArgs *e);
	//if not stop even WS_TABSTOP&WS_VISIBLE,need overwrite this
	virtual bool IsTabStop();

	//for dialog(tabpage or others)
	void OnInitDialog(EventArgs *e);
	virtual void CreateHandle();
	virtual void OnHandleCreated(EventArgs *e);
	virtual void DoOnInitDialog(EventArgs *e);
};

class PanelConfig : public ObjectConfig
{
public:
	BASE_CONTROL_CONFIG_DEF;
	static int nMarginLeft;
	static int nMarginTop;
	static int nMarginRight;
	static int nMarginBottom;
};
#define CC_FLAG_PANEL_END				CC_FLAG_CONTAINERCONTROL_END
//Panel has transparent while  OuterStyle::TransparentParentErase
//while put in rebar, should use toolbar
class Panel : public Container
{
public:
	Panel();
	virtual ~Panel();

protected:
	virtual void OnPaint(PaintEventArgs *e);
	//virtual void OnChildNotify(EventArgs *e);
};

class GroupPanelConfig : public ObjectConfig
{
public:
	BASE_CONTROL_CONFIG_DEF;
	static DWORD nOuterStyleClient;
	static DWORD nInnerStyleClient;
	static COLORREF corBackgroundClient;
};
#define CC_FLAG_GROUPPANEL_END				CC_FLAG_PANEL_END
//Owner for Group Frame, treated Transparent when xp theme.
class GroupPanel : public Panel
{
public:
	GroupPanel();
	virtual ~GroupPanel();

protected:
	virtual void OnResize(EventArgs *e);
};

class ReBarConfig : public ObjectConfig
{
public:
	BASE_CONTROL_CONFIG_DEF;
};
//the style of toolbar in rebar
typedef struct tagReBarStyle
{
	enum Type
	{
		Fix = 0,
		Moveable = 1,//only move bein rebar
		Floatable = 2
	};
}ReBarStyle;
#define CC_FLAG_REBAR_END				CC_FLAG_CONTAINERCONTROL_END
class ReBar : public Container
{
public:
	ReBar();
	virtual ~ReBar();

	void UpdateBand();
	BOOL InsertBand(Control* control, int uIndex = -1, bool isFix = false);
	BOOL DeleteBand(Control* control);
	int IndexFromHWND(HWND hwnd);

	UINT GetBandCount() const;
	BOOL GetBandInfo(UINT uBand, REBARBANDINFO* prbbi) const;
	UINT GetBarHeight() const;
	BOOL GetBarInfo(REBARINFO* prbi) const;
	COLORREF GetBackground() const;
	//IDropTarget* GetDropTarget() const;
	BOOL GetRect(UINT uBand, LPRECT prc) const;
	UINT GetRowCount() const;
	UINT GetRowHeight(UINT uRow) const;
	COLORREF GetTextColor() const;
	//CToolTipCtrl* GetToolTips() const;
	int IDToIndex(UINT uBandID) const;
	BOOL SetBandInfo(UINT uBand, REBARBANDINFO* prbbi);
	BOOL SetBarInfo(REBARINFO* prbi);
	//COLORREF SetBkColor(COLORREF clr);
	virtual void SetBackground(COLORREF corBackground);
	Control* SetOwner(Control* pWnd);
	COLORREF SetTextColor(COLORREF clr);
	//void SetToolTips(CToolTipCtrl* pTip);
	void BeginDrag(UINT uBand, DWORD dwPos);
	BOOL DeleteBand(UINT uBand);
	void DragMove(DWORD dwPos);
	void EndDrag();
	int HitTest(RBHITTESTINFO* prbht);
	BOOL InsertBand(UINT uIndex, REBARBANDINFO* prbbi);
	void MaximizeBand(UINT uBand);
	void MinimizeBand(UINT uBand);
	void RestoreBand(UINT uBand);
	BOOL SizeToRect(RECT& rect);
	BOOL ShowBand(UINT uBand, BOOL fShow /*= TRUE*/);
	void GetBandBorders(UINT uBand, LPRECT prc) const;
	//CPalette* GetPalette() const;
	//CPalette* SetPalette(HPALETTE hPal);
	BOOL MoveBand(UINT uFrom, UINT uTo);

#if _WIN32_IE >= 0x0500
	void PushChevron(UINT uBand, LPARAM lAppValue);
#endif

protected:
	ToolBar* _toolBand;

	virtual void OnChildNotify(EventArgs *e);
	virtual void DoUpdate();
	virtual void OnResize(EventArgs *e);
	virtual void OnCreateControl();
	virtual void OnControlAdded(ControlEventArgs *e);
	//keyArr is param address,but sHead is by value
	virtual void DoConfig(bool isFromIni, KeyArr& keyArr, cc::Str sThisHead);

	virtual void OnBeginDrag(EventArgs *e);
	virtual void OnEndDrag(EventArgs *e);
	virtual void OnMouseMove(MouseEventArgs *e);
};

class FloatContainerConfig : public ObjectConfig
{
public:
	BASE_CONTROL_CONFIG_DEF;
};
//lparam of CC_WM_MAINMSG_WPARAM_GET_REBAR while to find rebar
struct FindReBarEntity
{
	POINT pt;//pt of mouse
	Control* pToolBar;//hwnd of toolbar
	int nInsertIndex;//index to insert to rebar
	bool bHorizontal;
	ReBarStyle::Type eReBarStyle;//rebar style
	DockStyles::Type eDockable;//toolbar can dockable
};
#define CC_FLAG_FLOATCONTAINER_END				CC_FLAG_CONTAINERCONTROL_END
class FloatContainer : public Container
{
public:
	EventHandler Activated;
	EventHandler Closed;
	EventHandler Deactivate;

public:
	FloatContainer();
	virtual ~FloatContainer();

	void SetCloseButtonEnable(bool enable);
	bool GetCloseButtonEnable();
	//if maxSize is small than minSize, then auto use minSize as maxSize
	void SetMaxSize(int maxWidth, int maxHeight);
	void SetSizeBaseOnClient(int cxWidth, int cyHeight);

protected:
	SIZE _maxSize;
	virtual LRESULT WndProc(EventArgs *e);
	virtual void OnCreateControl();
};

class DockBarConfig : public ObjectConfig
{
public:
	BASE_CONTROL_CONFIG_DEF;
};
#define CC_FLAG_DOCKBAR_ISMOVEFORMONCLICK	CC_FLAG_CONTAINERCONTROL_END
#define CC_FLAG_DOCKBAR_ISFLOATRESIZEABLE	CC_FLAG_CONTAINERCONTROL_END <<1
#define CC_FLAG_DOCKBAR_END					CC_FLAG_CONTAINERCONTROL_END <<2
class DockBar : public Container
{
public:
	DockBar();
	virtual ~DockBar();
	virtual void Dispose();

	virtual FloatContainer* GetFloatContainer();
	//where to put toolbar,while create it
	DockStyles::Type GetInitDockPosition() const;
	void SetInitDockPosition(DockStyles::Type value);
	DockStyles::Type GetDockable() const;
	//can be set like:(DockStyles::Type)DockStyles::Left|DockStyles::Top|DockStyles::Right|DockStyles::Bottom|DockStyles::None|DockStyles::Fill;
	void SetDockable(DockStyles::Type value);
	bool GetFloating();
	virtual void SetFloating(bool value, bool isShowFloater = true);
	virtual void SetFloating(bool value, bool isShowFloater, int left, int top);
	void SetCloseButtonEnable(bool enable);
	bool GetCloseButtonEnable();

protected:
	DockStyles::Type _eInitDockPosition;
	DockStyles::Type _eDockable;
	FloatContainer* _pFloater;

	virtual RECT CalcuteRect(bool updateRect = false, int nExpectX = -1, int nExpectY = -1, int nExpectCX = -1, int nExpectCY = -1, bool isFourceCalcute = false);
	virtual void OnResize(EventArgs *e);
	//valid on _bIsMoveFormOnClick
	virtual void OnMouseDown(MouseEventArgs *e);
	//virtual void OnVisibleChanged(EventArgs *e);
	//for _pFloater
	virtual void Floater_OnMoving(Object* sender, EventArgs* e);
};

class ToolBarConfig : public ObjectConfig
{
public:
	BASE_CONTROL_CONFIG_DEF;
};
#define CC_TOOLBAR_SPLITW 2
#define CC_TOOLBAR_SPLITH 2
//should define _minSize
#define CC_FLAG_TOOLBAR_END				CC_FLAG_DOCKBAR_END
class ToolBar : public DockBar
{
public:
	ToolBar();
	virtual ~ToolBar();
	//if MinSize.cx <= 0,calcute it
	virtual SIZE GetMinSize();
	virtual SIZE BestSize(bool isUpdate = false);
	//if break then insert at newline of rebar.only valid before insert to rebar
	bool GetBreak();
	void SetBreak(bool value);

protected:
	virtual void DoUpdate();
	virtual void OnResize(EventArgs *e);
	virtual void OnCreateControl();

	//for _pFloater
	virtual void Floater_OnMoving(Object* sender, EventArgs* e);
};

class StatusBarConfig : public ObjectConfig
{
public:
	BASE_CONTROL_CONFIG_DEF;
	static int nWaitToDefaultText;
};
//for Container is always used by next controls,so it is good to not take bits of _InnerFlags
#define CC_FLAG_STATUSBAR_END				CC_FLAG_CONTAINERCONTROL_END
class StatusBar : public Container
{
public:
	StatusBar();
	virtual ~StatusBar();

	//if nIndex = -1, use nDefaultIndex
	virtual void SetText(const TCHAR* sText, ImageList* imageList = NULL, int nIconID = 0, int nIndex = -1);
	virtual void SetText(const cc::Str& sText, ImageList* imageList = NULL, int nIconID = 0, int nIndex = -1);
	//only one item has default text,if nTimeOut < 0 then not change TimeOut. if nDefaultIndex is -1,not change defaultindex
	void SetDefault(const TCHAR* pDefaultText, ImageList* imageList = NULL, int nIconID = 0, int nDefaultIndex = -1, int nTimeOut = -1);
	bool KeepShow();

protected:
	TCHAR* _pDefaultText;
	ImageList* pDefaultImageList;
	int _nDefaultIconID;
	int _nDefaultIndex;
	int _nDefaultTimeout;

	//virtual void OnVisibleChanged(EventArgs *e);
	virtual void DoUpdate();
	virtual void OnResize(EventArgs *e);
	virtual void OnInnerTimer(EventArgs *e);
	//keyArr is param address,but sThisHead is by value
	virtual void DoSetLang(KeyArr& keyArr, cc::Str sThisHead);
	virtual void OnCreateControl();
};

#define CC_SCROLLCONTAINER_CLIENT_USEPANEL		0x00000001
class ScrollContainerConfig : public ObjectConfig
{
public:
	BASE_CONTROL_CONFIG_DEF;
	static DWORD nClientFlags;
};
//add at 20070801
#define CC_FLAG_SCROLLCONTAINER_SCROLLAUTO		CC_FLAG_PANEL_END
#define CC_FLAG_SCROLLCONTAINER_END				CC_FLAG_PANEL_END <<1
class ScrollContainer : public Panel
{
public:
	EventHandler UpdatePositionEvent;

public:
	ScrollContainer();
	virtual ~ScrollContainer();
	virtual void Dispose();

	//SB_BOTH, SB_HORZ, SB_VERT
	void EnableScrollBar(int nBar, bool bEnable);
	void UpdateClientPosition();
	void StartMoveClient(Object* sender, MouseEventArgs* e);

	void SetMinMoveLimit(int nMinMoveLimit);
	int GetMinMoveLimit();
	void SetIsScrollAuto(bool value);
	bool GetIsScrollAuto();
	void SetClientBounds(int x, int y, int width, int height);
	virtual RECT GetClientBounds();

protected:
	HScrollBar* m_hHBar;
	VScrollBar* m_hVBar;

	Control* m_pClient;
	Control* m_pClientClient;
	POINT ptOldClient;
	//if mouse move distance big than this,then move client
	int nMinMoveLimit;
	int nOldX;
	int nOldY;
	int nOldH;
	int nOldV;
	//if not,then always show scroll
	//bool bIsScrollAuto;

	virtual void OnCreateControl();
	virtual void OnResize(EventArgs *e);
	virtual void DoUpdateClientPosition();
	//set page size of scroll.also show or hide it if bIsScrollAuto
	virtual void UpdateScrollInfo();

	virtual void Scroll_ValueChanged(Object* sender, EventArgs* e);

	//Move Client
	virtual void Client_MouseDown(Object* sender, MouseEventArgs* e);
	virtual void Client_MouseMove(Object* sender, MouseEventArgs* e);
	virtual void Client_MouseUp(Object* sender, MouseEventArgs* e);
};

class NotifyIconConfig : public ObjectConfig
{
public:
	//while click left button on notifyicon,it is need wait for see if is DBLCLK
	static UINT nWaitTimerSeeIfDoubleClick;
};
//for base is ITextIcon,so from CC_FLAG_CONTROL_START
#define CC_FLAG_NOTIFYICON_LEFTCLICKMENU			CC_FLAG_CONTROL_START
#define CC_FLAG_NOTIFYICON_RIGHTCLICKMENU			CC_FLAG_CONTROL_START <<1
#define CC_FLAG_NOTIFYICON_CREATED					CC_FLAG_CONTROL_START <<2
#define CC_FLAG_NOTIFYICON_HIDDEN					CC_FLAG_CONTROL_START <<3
#define CC_FLAG_NOTIFYICON_LASTDBLCLK				CC_FLAG_CONTROL_START <<4
#define CC_FLAG_NOTIFYICON_VISIBLE					CC_FLAG_CONTROL_START <<5
#define CC_FLAG_NOTIFYICON_END						CC_FLAG_CONTROL_START <<6
class NotifyIcon : public ITextIcon
{
public:
	EventHandler Click;
	EventHandler DoubleClick;
	MouseEventHandler MouseDown;
	MouseEventHandler MouseMove;
	MouseEventHandler MouseUp;

	//typedef struct tagShowMenu
	//{
	//	enum Type
	//	{
	//		RightClick = 0x0001,
	//		LeftClick = 0x0002,
	//		RightLeftClick = 0x0003,
	//	};
	//}ShowMenu;
	//
	NotifyIcon();
	virtual ~NotifyIcon();
	virtual void Dispose();

	virtual ContextMenu* GetContextMenu() const;
	virtual void SetContextMenu(ContextMenu* menu);
	virtual void SetVisible(bool value);
	//void SetShowMenu(ShowMenu::Type type);
	//ShowMenu::Type GetShowMenu();
	void SetShowMenu(LeftRight::Type type, bool value);
	bool GetShowMenu(LeftRight::Type type);

	//nCycles while -1 for dead loop
	void PlayStart(int nCycleMilliseconds = 500, int nCycles = -1);
	void PlayStop();
	void AddAnimate(UINT nIDResource);
	void AddAnimate(HICON hIcon, bool needDestroy = false);

protected:
	virtual void DoUpdate();
	virtual void OnClick(Object* sender, EventArgs *e);
	virtual void OnDoubleClick(Object* sender, EventArgs *e);
	virtual void OnMouseDown(Object* sender, MouseEventArgs *e);
	virtual void OnMouseMove(Object* sender, MouseEventArgs *e);
	virtual void OnMouseUp(Object* sender, MouseEventArgs *e);
	//virtual void OnTick(Object* sender, EventArgs *e);
	//virtual void OnTickAnimate(Object* sender, EventArgs *e);
	virtual void _pPopupWin_Tick(Object* sender, EventArgs *e);

protected:
	//bool _Visible;
	ContextMenu* _pContextMenu;
	UINT _uTrackPopupMenuFlag;
	NOTIFYICONDATA _dataNotify;
	//bool _isCreatedNotifyIcon;
	//bool _isHidden;
	//bool _lastIsDBLCLK;
	//DWORD _Flags; //flags for other controls and endusers(put in ITextIcon)
	//ShowMenu::Type _eShowMenu;
	POINT _ptLeftClick;
	Control* _pPopupWin;
	//Timer* _pTimer;
	//Timer* _pTimer2;
	//list to save HICON
	List<void*>* _lstAnimates;
	List<int>* _lstAniFlg;
	int _nAnimateIndex;
	int _nCycles;
};

#define CC_FLAG_HOTKEY_END				CC_FLAG_CONTROL_END
class Hotkey : public Control
{
public:
	//e->WParam = vk | (fsModifiers <<16)
	EventHandler HotKeyDown;

public:
	Hotkey();
	virtual ~Hotkey();
	virtual void Dispose();

	bool Register(UINT fsModifiers, UINT vk);
	bool Unregister(UINT fsModifiers, UINT vk);

protected:
	List<int>* _lstHotKey;

	virtual void OnCreateControl();
	virtual LRESULT WndProc(EventArgs *e);
	virtual void OnHotKey(EventArgs *e);
};

class HotkeyInputCtl : public TextBox
{
public:
	HotkeyInputCtl();
	virtual ~HotkeyInputCtl();

	/*fsModifiers:MOD_SHIFT, MOD_CONTROL, MOD_ALT, MOD_WIN*/
	int GetHotkey(UINT& fsModifiers, UINT& vk);
	void SetHotkey(UINT fsModifiers, UINT vk);
	static cc::Str GetHotkeyString(UINT fsModifiers, UINT vk);

protected:
	UINT m_fsModifiers;
	UINT m_vk;
	bool isPressedOK;

	virtual LRESULT WndProc(EventArgs *e);
	virtual void OnCreateControl();
};

class IEControlConfig : public ObjectConfig
{
public:
	BASE_CONTROL_CONFIG_DEF;
};
#define CC_FLAG_IECONTROL_END				CC_FLAG_CONTROL_END
class IEControl : public Control
{
protected:
	IEBase *m_pieBase;

///////////////////////////////////////////////////////////////////////////////
protected:
	//ole object
	ie_DocHandler* _pUIHandler;
	ie_InPlaceSite* _pInPlaceSite;
	ie_ClientSite *_pClientSite;
	ie_EventHandler *m_pIEEventHandler; //delete while end this class
	BSTR _initURL;

// c# like
protected:
#ifdef _DEBUG_
	// WndProc - calls appriate On... function for the give
	virtual LRESULT WndProc(EventArgs *e);
#endif
	//here to create window
	virtual void CreateHandle();
	virtual void OnResize(EventArgs *e);
	virtual void OnPaint(PaintEventArgs *e);
public:
	//Constructors
	IEControl();
	//destructor
	virtual ~IEControl();

	virtual bool Focus(bool setLastControl = false);
	//should call this before CreateHandle
	void SetOleObject(ie_DocHandler *pUIHandler, ie_InPlaceSite *pInPlaceSite, ie_ClientSite *pClientSite, BSTR initURL);
	virtual bool PreProcessMessage(MSG* pMsg, UINT_PTR& keyData);

	//control ie
	virtual bool Navigate(BSTR url);
	virtual ie_IWebBrowser2* GetWebBrowser();
	virtual HWND GetWebBrowserHWND();
	virtual ie_IHTMLDocument2* GetDocument();
    virtual bool ExecScript(BSTR code, BSTR language = L"JavaScript");
    virtual bool Write(BSTR txt, bool isAppend = false);
	bool IsComplete();
	bool LoadText(const void* txt, DWORD nLength);
	bool SaveText(void** txt, DWORD& nLength);

	bool SetEventHandler(ie_EventHandler *pIEEventHandler);
};

} //namespace win
} //namespace cc

#endif //#ifndef CC_WIN_CONTROL_H_
