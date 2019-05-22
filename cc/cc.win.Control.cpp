// cc.win.Control.cpp
// The C# like Controls class
// Author: Shu.KK
// 2006-01-01

/* Information on Keyboard handling
 * Key event processing always starts at the control that has the input focus (e.g. a text control).
   From there various methods are called upwards in the window hierarchy(from child to parent). This
   gives parent controls such as forms and panels an opportunity to handle and consume key events
   before they can reach the target control.
   There is only one method (ProcessMnemonic) that is called in the opposite direction (from parent to
   child). Each of the 8 methods returns a boolean value. If a method returns true the key event is
   considered handled (consumed) and processing of that particular key event will stop immediatly (no
   further processing methods will be called).

  Key event processing actually happens in two phases: preprocessing and processing. The preprocessing
  phase starts with a call to PreProcessMessage, the processing phase with a call to ProcessKeyMessage.
  While the processing phase is very similar for all three types of key events (WM_KEYDOWN, WM_KEYUP
  and WM_CHAR) preprocessing differs completely for WM_KEYDOWN and WM_CHAR and there is no
  preprocessing phase for WM_KEYUP.

Here is what happens in detail:

Preprocessing Phase (WM_KEYDOWN):
  1. PreProcessMessage is called on the focused control.
     2. The focused control calls its own ProcessCmdKey method.
        3. ProcessCmdKey by default just returns the result of calling
           ProcessCmdKey on its parent. So effectivly this bubbles the event
           up the windows hierarchy until the top-most control is reached
           which - by default - simply returns false.
     4. The focused control calls IsInputKey on itself. If the result is true
        the PreProcessMessage terminates at this point with a return value of false.
     6. The focused control calls its own ProcessDialogKey method.
        7. Again, ProcessDialogKey is called from child to parent along the parent
           chaim until the top-most control is reached which usually returns false.
  7. PreProcessMessage returns.

  Preprocessing Phase (WM_CHAR):
  1. PreProcessMessage is called on the focused control.
     2. The focused control calls IsInputChar on itself. If the result is true
        the PreProcessMessage terminates at this point with a return value of false.
     3. The focused control calls its own ProcessDialogChar method.
        4. ProcessDialogChar is called from child to parent along the parent
           chain until the top-most control is reached which usually returns false.
           In addition, for each Container the following step is
           performed before the parent's ProcessDialogChar method is invoked:
           5. The container control calls its own ProcessMnemonic method.
              6. ProcessMnemonic is called for all child controls of the
                 container (which in turn might call ProcessMnemonic on their
                 children).
  7. PreProcessMessage returns.

  Processing Phase (WM_KEYDOWN, WM_KEYUP and WM_CHAR):
  1. ProcessKeyMessage is called on the focused control.
     2. The focused control calls ProcessKeyPreview on the parent control.
        3. ProcessKeyPreview is called from child to parent along the parent
           chain until the top-most control is reached which usually returns false.
     4. The focused control calls ProcessKeyEventArgs on itself.
        5. ProcessKeyEventArgs calls one of the methods OnKeyDown, OnKeyUp
           or OnKeyPress depending on the type of key event.
           6. The On* method invokes all event handlers that have been registerd
              for the corresponding event.
   7. ProcessKeyMessage returns.

   Note again that as soon as one method returns true processing will stop and the calling method will immediately return true as well. Also,
  if the preprocessing phase returns true the processing phase will be skipped.

You may have noticed that in preprocessing of WM_CHAR when there are several container controls in the parent chain the ProcessMnemonic method
of child controls will be called unnecessarily often.
*/

#include "cc.win.Control.h"
//#include "cc.win.Form.h"
using namespace cc;
using namespace cc::win;

#if defined(_DEBUG_) || defined(_DESIGN)
#if defined(_DEBUG_)
DWORD DesignConfig::nGlobalFlag = 0;
#else
DWORD DesignConfig::nGlobalFlag = CC_DESIGN_G_DENIAL;
#endif //_DEBUG_
#endif //_DEBUG_||_DESIGN

//===========================================================cc::win::WndClassName
const TCHAR* WndClassName::Control = _T("ccWnd");
const TCHAR* WndClassName::ToolTip = _T("ccTip");
const TCHAR* WndClassName::ToolItem = _T("ccToolItem");
const TCHAR* WndClassName::Label = _T("Static");
const TCHAR* WndClassName::Button = _T("Button");
const TCHAR* WndClassName::SpinButton = _T("msctls_updown32");
const TCHAR* WndClassName::ListBox = _T("ListBox");
const TCHAR* WndClassName::ComboBox = _T("combobox");
const TCHAR* WndClassName::ComboBoxEx = _T("ComboBoxEx32");
const TCHAR* WndClassName::TextBox = _T("Edit");
const TCHAR* WndClassName::RichTextBox = RICHEDIT_CLASS;
const TCHAR* WndClassName::ScrollBar = _T("ScrollBar");
const TCHAR* WndClassName::ProgressBar = _T("msctls_progress32");
const TCHAR* WndClassName::ListView = WC_LISTVIEW; //_T("SysListView32");
const TCHAR* WndClassName::TreeView = WC_TREEVIEW; //_T("SysTreeView32");
const TCHAR* WndClassName::DateTimePicker = _T("SysDateTimePick32");
const TCHAR* WndClassName::MonthCalendar = _T("SysMonthCal32");
const TCHAR* WndClassName::Splitter = _T("ccSplitter");
const TCHAR* WndClassName::Container = _T("ccContainer");
const TCHAR* WndClassName::FloatContainer = _T("ccFloatContainer");
const TCHAR* WndClassName::DockBar = _T("ccDockBar");
const TCHAR* WndClassName::ToolBar = _T("ccToolBar");
const TCHAR* WndClassName::ReBar = REBARCLASSNAME;
const TCHAR* WndClassName::StatusBar = _T("msctls_statusbar32");
const TCHAR* WndClassName::IEControl = _T("ccIE");
const TCHAR* WndClassName::UserControl = _T("ccUser");
const TCHAR* WndClassName::MDIChild = _T("MDICLIENT");
const TCHAR* WndClassName::Form = _T("ccForm");
const TCHAR* WndClassName::TabControl = WC_TABCONTROL;
const TCHAR* WndClassName::TabControlEx = _T("ccTabControlEx");
const TCHAR* WndClassName::Dialog32770 = _T("#32770");

//===========================================================cc::win::MainMenu
///////////////////////////////////////////////////////////////////////////////
// constructors/destructor
MainMenu::MainMenu()
{
#ifdef _DEBUG_
	SetText(_T("MainMenu"));
#endif
	SetName(_T("MainMenu"));
	_eMenuType = MenuType::MAINMENU;
	m_hWndParent = NULL;
}

MainMenu::~MainMenu()
{
}

void MainMenu::CreateControl()
{
	CreateMenuHandle();
}


bool MainMenu::GetAllOwnerDraw()
{
	return IsInnerStyle(MenuInnerStyle::AllOwnerDraw);
}

void MainMenu::SetAllOwnerDraw(bool value)
{
	SetInnerStyle(MenuInnerStyle::AllOwnerDraw, value, true);
}
// constructors/destructor
///////////////////////////////////////////////////////////////////////////////

//===========================================================cc::win::ContextMenu
///////////////////////////////////////////////////////////////////////////////
// constructors/destructor
ContextMenu::ContextMenu()
{
#ifdef _DEBUG_
	SetText(_T("ContextMenu"));
#endif
	SetName(_T("ContextMenu"));
	_eMenuType = MenuType::CONTEXTMENU;
	_uTrackPopupMenuFlag = TPM_LEFTALIGN;
	_pPopupWin = NULL;
}

ContextMenu::~ContextMenu()
{
	if(_pPopupWin != NULL)
	{
		_pPopupWin->Dispose();
		delete _pPopupWin;
		_pPopupWin = NULL;
	}
}

// Show this context menu at the specified control-relative co-ordinates.
void ContextMenu::Show(Control* control, POINT pos)
{
	Show(control, pos.x, pos.y);
}

void ContextMenu::Show(Control* control, LONG x, LONG y)
{
	if(control == NULL)
	{
		Show((HWND)NULL, x, y);
	}
	else
	{
		Show(control->m_hWnd, x, y);
	}
}

void ContextMenu::Show(HWND m_hWnd, LONG x, LONG y)
{
	//for no menu to show
	if(_lstMenus == NULL || _lstMenus->GetSize() < 1)
	{
		return;
	}

	if(m_hWnd == NULL)
	{
		if(_pPopupWin == NULL)
		{
			_pPopupWin = new Control();
			_pPopupWin->SetVisible(false);
			_pPopupWin->SetBoundsNotOriginal(0, 0, -1, -1);
			_pPopupWin->SetStyle(StyleType::Style, 0, WS_POPUP);
			_pPopupWin->SetStyle(StyleType::ExStyle, 0, WS_EX_TOOLWINDOW);
			_pPopupWin->CreateControl();
		}
	}

	//OnPopup
	OnPopup(NULL);

	//here creat Handle
	CreateMenuHandle();
	CC_ASSERT(m_hMenu != NULL);
	if(m_hMenu == NULL)
	{
		CC_LOGSTEP();
		const TCHAR* msg = _T("ContextMenu::Show, create HMENU Failed.");
		cc::Log::fatal(msg);
		throw UserException(msg);
	}

	//show menu
	//This is a known problem. See "PRB: Menus for Notification Icons Don't Work Correctly" in MSDN.
	//http://support.microsoft.com/support/kb/articles/Q135/7/88.asp
	if(m_hWnd != NULL)
	{
		::TrackPopupMenu(m_hMenu, _uTrackPopupMenuFlag, x, y, 0, m_hWnd, NULL);
		::SendMessage(m_hWnd, WM_EXITMENULOOP, TRUE, 0);
		::PostMessage(m_hWnd, WM_NULL, 0, 0);
	}
	else
	{
		_pPopupWin->Show();
		::SetForegroundWindow(_pPopupWin->m_hWnd);
		::TrackPopupMenu(m_hMenu, _uTrackPopupMenuFlag, x, y, 0, _pPopupWin->m_hWnd, NULL);
		::SendMessage(_pPopupWin->m_hWnd, WM_EXITMENULOOP, TRUE, 0);
		::PostMessage(_pPopupWin->m_hWnd, WM_NULL, 0, 0);
		_pPopupWin->SetVisible(false);
	}
}

void ContextMenu::OnPopup(EventArgs* e)
{
	Popup.Invoke((Object*)this, e);
}

bool ContextMenu::GetAllOwnerDraw()
{
	return IsInnerStyle(MenuInnerStyle::AllOwnerDraw);
}

void ContextMenu::SetAllOwnerDraw(bool value)
{
	SetInnerStyle(MenuInnerStyle::AllOwnerDraw, value, true);
}
// constructors/destructor
///////////////////////////////////////////////////////////////////////////////

//===========================================================cc::win::FontConfig
LOGFONT FontConfig::lfControl = {0};
LOGFONT FontConfig::lfMenu = {0};
//to init font in constructors
FontConfig _GLOBAL_ONLY_ONCE_FONTCONFIG;
//===========================================================cc::win::ControlConfig
/*
info for nClassStyle:
CS_BYTEALIGNCLIENT	Aligns the window's client area on a byte boundary (in the x direction). This style affects the width of the window and its horizontal placement on the display.
CS_BYTEALIGNWINDOW	Aligns the window on a byte boundary (in the x direction). This style affects the width of the window and its horizontal placement on the display.
CS_CLASSDC	Allocates one device context to be shared by all windows in the class. Because window classes are process specific, it is possible for multiple threads of an application to create a window of the same class. It is also possible for the threads to attempt to use the device context simultaneously. When this happens, the system allows only one thread to successfully finish its drawing operation.
CS_DBLCLKS	Sends a double-click message to the window procedure when the user double-clicks the mouse while the cursor is within a window belonging to the class.
CS_DROPSHADOW	Windows XP: Enables the drop shadow effect on a window. The effect is turned on and off through SPI_SETDROPSHADOW. Typically, this is enabled for small, short-lived windows such as menus to emphasize their Z order relationship to other windows.
CS_GLOBALCLASS	Specifies that the window class is an application global class. For more information, see Application Global Classes.
CS_HREDRAW	Redraws the entire window if a movement or size adjustment changes the width of the client area.
CS_NOCLOSE	Disables Close on the window menu.
CS_OWNDC	Allocates a unique device context for each window in the class.
CS_PARENTDC	Sets the clipping rectangle of the child window to that of the parent window so that the child can draw on the parent. A window with the CS_PARENTDC style bit receives a regular device context from the system's cache of device contexts. It does not give the child the parent's device context or device context settings. Specifying CS_PARENTDC enhances an application's performance.
CS_SAVEBITS	Saves, as a bitmap, the portion of the screen image obscured by a window of this class. When the window is removed, the system uses the saved bitmap to restore the screen image, including other windows that were obscured. Therefore, the system does not send WM_PAINT messages to windows that were obscured if the memory used by the bitmap has not been discarded and if other screen actions have not invalidated the stored image.

This style is useful for small windows (for example, menus or dialog boxes) that are displayed briefly and then removed before other screen activity takes place. This style increases the time required to display the window, because the system must first allocate memory to store the bitmap.
CS_VREDRAW	Redraws the entire window if a movement or size adjustment changes the height of the client area.
*/
DWORD ControlConfig::nStyle = WS_CHILD | WS_VISIBLE;
DWORD ControlConfig::nExStyle = 0;
//DWORD ControlConfig::nClassStyle = CS_DBLCLKS | CS_GLOBALCLASS;// | CS_PARENTDC;
DWORD ControlConfig::nInnerStyle = InnerStyle::TranceContextMenuUp;
DWORD ControlConfig::nOuterStyle = 0;
DWORD ControlConfig::nOwnerDrawTextStyle = DT_LEFT|DT_VCENTER|DT_SINGLELINE;
COLORREF ControlConfig::corBackground = ::GetSysColor(COLOR_BTNFACE);
COLORREF ControlConfig::corTextground = ::GetSysColor(COLOR_BTNTEXT);
COLORREF ControlConfig::corHotBackColor = ::GetSysColor(COLOR_BTNFACE);
COLORREF ControlConfig::corHotTextColor = ::GetSysColor(COLOR_BTNTEXT);
int ControlConfig::nMarginLeft = 1;
int ControlConfig::nMarginTop = 1;
int ControlConfig::nMarginRight = 1;
int ControlConfig::nMarginBottom = 1;
UINT ControlConfig::nWaitTimerCheckMouseLeave = 100;
//deleted at App::~App()
HFONT Control::hFontAppShare = NULL;

//===========================================================cc::win::Control
///////////////////////////////////////////////////////////////////////////////
// constructors/destructor
Control::Control()
{
	for(int i = 0; i < CC_CONTROL_STYLETYPE_SIZE; i++)
	{
		_StyleArray[i] = 0;
	}
	BASE_CONTROL_CONFIG_INIT(Control);
	_StyleArray[(int)StyleType::EndUser] = 0;
	//_SetWindowPosFlags = SWP_NOZORDER | SWP_FRAMECHANGED | SWP_DRAWFRAME;
	::SetRect(&_Margin, ControlConfig::nMarginLeft, ControlConfig::nMarginTop, ControlConfig::nMarginRight, ControlConfig::nMarginBottom);
	m_hWnd = NULL;
	Tag = NULL;
	_pParent = NULL;
	_pClient = NULL;
	//_ptOffset.x = 0;
	//_ptOffset.y = 0;
	//_pOffsetControl = NULL;
	_pContextMenu = NULL;
	_hFont = NULL;
	_Text = NULL;
#ifdef _DEBUG_
	SetName(_T("Control"));
#endif

	::SetRect(&_Rect, 0, 0, 75, 23);
	::CopyRect(&_OrgRect, &_Rect);
	_ClientSize.cx = 800;
	_ClientSize.cy = 800;
	_ClassName = WndClassName::Control;
	//_Flags = 0;
	// mask for InitCommonControlsEx
	//turn to _StyleArray[(int)StyleType::InitControl]
	//_maskCommonControls = 0;
	_hMenu = NULL;
	//_hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1); //Background wnd colour:(HBRUSH)(COLOR_BTNFACE + 1)
	_hbrBackground = (HBRUSH)NULL;
	_hCursor = NULL;
	_moveForm = NULL;
	_moveFormCursor = -1;

	//_bIsHandleCreated = false;
	//_bIsHandleCreatedEnd = false;
	_lstControls = NULL;
	_controlRealWndProc = NULL;
	//_Visible = true;
	//CC_BIT_ON(_Flags, CC_FLAG_CONTROL_VISIBLE);
	//SetStyle(StyleType::Inner2, 0, Inner2Style::Visible);
	_StyleArray[(int)StyleType::Inner2] |= Inner2Style::Visible;

	MinPosition.x = 0;
	MinPosition.y = 0;
	_minSize.cx = 0;
	_minSize.cy = 0;
	Anchor = (AnchorStyles::Type)(AnchorStyles::Left | AnchorStyles::Top);
	Dock = DockStyles::None;
	TabIndex = 0;
	_pActivePopunMenu = NULL;
	_sizeTextIcon.cx = -1;
	_sizeTextIcon.cy = -1;
	_nExpectCY = -1;
	_hbmpBackground = NULL;
	_nBackgroundHatch = -1;
	_pParentBrush = NULL;
#if defined(_DEBUG_) || defined(_DESIGN)
	//code for design
	//flag for moveable or others
	Design = DesignStyle::None;
#endif
}

Control::~Control()
{
#ifdef _DEBUG_
	//if(CC_APP_GetConfig()->GetIsDebug())
	//{
	//	CC_TRACE((_T("ClassName:%s, Name:%s, isDisposed:%d, ControlsCount:%d"), _ClassName, _Name, GetIsDispose(), _lstControls == NULL ? 0 : _lstControls->GetSize()));
	//}
#endif
	//if(HasStyle(StyleType::Inner2, Inner2Style::SubclassWindow))
	//{
	//	UnsubclassWindow();
	//}
	this->Dispose();
	if(_lstControls != NULL)
	{
		for(int n = _lstControls->GetSize() - 1; n >= 0; n--)
		{
			Control* ctl = _lstControls->GetKey(n);
			delete ctl;
		}
		_lstControls->Clear();
		delete _lstControls;
		_lstControls = NULL;
	}
	//for _pClient is in _lstControls,not need do so
	//if(_pClient != NULL)
	//{
	//	delete _pClient;
	//	_pClient = NULL;
	//}
	this->DestroyHandle();
	//_Text,_Name be free at ITextIcon
	if(_hFont != NULL)
	{
		if(_hFont != hFontAppShare)
		{
			::DeleteObject(_hFont);
		}
		_hFont = NULL;
	}
	if(_hMenu != NULL)
	{
		::DestroyMenu(_hMenu);
		_hMenu = NULL;
	}
	if(_hbrBackground != NULL && HasStyle(StyleType::Inner2, Inner2Style::BackgroundDestroyBrush))
	{
		::DeleteObject(_hbrBackground);
		_hbrBackground = NULL;
	}
	if(_hbmpBackground != NULL && HasStyle(StyleType::Inner2, Inner2Style::BackgroundDestroyBmp))
	{
		::DeleteObject(_hbmpBackground);
		_hbmpBackground = NULL;
	}
}

void Control::Dispose()
{
#ifdef _DEBUG_
	if(CC_APP_GetConfig()->GetIsDebug())
	{
		CC_TRACE((_T("ClassName:%s, Name:%s, Text:%s, isDisposed:%d, ControlsCount:%d"), _ClassName, _Name, _Text, GetIsDispose(), _lstControls == NULL ? 0 : _lstControls->GetSize()));
	}
#endif
	if(!GetIsDispose())
	{
		if(_lstControls != NULL)
		{
			for(int n = _lstControls->GetSize() - 1; n >= 0; n--)
			{
				Control* ctl = _lstControls->GetKey(n);
				ctl->Dispose();
			}
		}
		//for _pClient is in _lstControls,not need do so
		//if(_pClient != NULL)
		//{
		//	_pClient->Dispose();
		//}
		//if(m_hWnd != NULL && IsWindow(m_hWnd))
		//{
		//	//while dispose control,send this for control to do OnHandleDestroyed or OnClosed or others
		//	::SendMessage(m_hWnd, CC_WM_MAINMSG, CC_WM_MAINMSG_WPARAM_DISPOSE, 0);
		//}
		//else
		//{
		if(!HasStyle(StyleType::Inner2, Inner2Style::HandleDestroyed))
		{
			//have a chance to do sth.
			EventArgs ee(m_hWnd, CC_WM_MAINMSG, CC_WM_MAINMSG_WPARAM_DISPOSE, NULL);
			OnHandleDestroyed(&ee);
			if(m_hWnd != NULL && IsWindow(m_hWnd))
			{
				if(HasStyle(StyleType::Inner2, Inner2Style::SubclassWindow))
				{
					this->UnsubclassWindow();
				}
				else
				{
					//special for form, now destroy it
					::DestroyWindow(m_hWnd);
				}
			}
		}
		//}
		//this must be after CC_WM_MAINMSG_WPARAM_DISPOSE
		Object::Dispose();
		//CC_BIT_OFF(_Flags, CC_FLAG_CONTROL_HANDLECREATED);
		SetStyle(StyleType::Inner2, Inner2Style::HandleCreated, 0);
		//_bIsHandleCreated = false;
	}
}

// Compare implementations
Control::operator HWND() const
{
	return m_hWnd;
}

bool Control::operator==(const Control& control) const
{
	return (&control == this);
}

bool Control::operator!=(const Control& control) const
{
	return (&control != this);
}

bool Control::GetIsDispose()
{
	return (ITextIcon::GetIsDispose()
		|| ((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0
		&& !::IsWindow(m_hWnd)));
}

void Control::DestroyHandle()
{
	if(m_hWnd != NULL)
	{
		if(IsWindow(m_hWnd))
		{
			//let send WM_DESTROY and WM_NCDESTROY
			::DestroyWindow(m_hWnd);
		}
		//normal remove at OnNcDestroy
		if(m_hWnd != NULL)
		{
			CC_APP_GetMapHWND()->Remove(m_hWnd);
			m_hWnd = NULL;
		}
	}
}

cc::Str Control::DoGetClassName() const
{
	return cc::Str(_T("cc::win::Control"));
}
// constructors/destructor
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// private/protected/internal used methods
//here to create window
void Control::CreateHandle()
{
	CC_ASSERT(!::IsWindow(m_hWnd));

	//clear all UpdateFlag
	_nUpdateFlag = 0;
	//Only one window per CWin instance
	if(m_hWnd != NULL && ::IsWindow(m_hWnd))
	{
		CC_LOGSTEP();
		const TCHAR* msg = _T("Control::CreateHandle, Handle is created.");
		cc::Log::fatal(msg);
		throw UserException(msg);
	}

	////update offset location
	//if(_pOffsetControl != NULL)
	//{
	//	if(HasStyle(StyleType::Inner2, Inner2Style::OffsetNewline))
	//	{
	//		SetLocation(_pOffsetControl->_Rect.left + _ptOffset.x,
	//			_pOffsetControl->_Rect.bottom + _ptOffset.y);
	//	}
	//	else
	//	{
	//		SetLocation(_pOffsetControl->_Rect.right + _ptOffset.x,
	//			_pOffsetControl->_Rect.top + _ptOffset.y);
	//	}
	//}

	//update size
	CalcuteRect(true);
	CREATESTRUCT cs;
	cs.dwExStyle = _StyleArray[(int)StyleType::ExStyle];
	cs.lpszClass = _ClassName;
	cs.lpszName = _Text;
	cs.style = _StyleArray[(int)StyleType::Style];
	cs.x = _Rect.left;
	cs.y = _Rect.top;
	cs.cx = _Rect.right - _Rect.left;
	cs.cy = _Rect.bottom - _Rect.top;

	//if has Client,should do sth. at OnResize
	if(_pClient != NULL)
	{
		_pClient->SetSize(cs.cx, cs.cy);
		::CopyRect(&_pClient->_OrgRect, &_OrgRect);
	}
	//notice here is owner,but not only parent,and cannot change after CreateWindowEx
	if(_pParent != NULL)
	{
		cs.hwndParent = _pParent->m_hWnd;
	}
	else
	{
		cs.hwndParent = NULL;
	}
	cs.hInstance = CC_APP_GetInstance();
	cs.lpCreateParams = NULL;
	cs.hMenu = _hMenu;
	if(HasStyle(StyleType::Inner2, Inner2Style::Visible))
	{
		cs.style |= WS_VISIBLE;
	}
	else
	{
		cs.style &= ~WS_VISIBLE;
	}
	if(cs.hwndParent == NULL)
	{
		//treate as window
		cs.style |= WS_POPUP;
	}

	if(!HasStyle(StyleType::Inner, InnerStyle::IsMdiContainer) && _pParent != NULL && _pParent->HasStyle(StyleType::Inner, InnerStyle::IsMdiContainer))
	{
		cs.dwExStyle = 0;
	}
	//allow modification of several common create parameters
	if(!PreCreateWindow(cs))
	{
		PostNcDestroy();
		return;
	}

/*

To create a child window, an MDI application either calls the CreateMDIWindow function or sends the WM_MDICREATE message to the MDI client window. 
A more efficient way to create an MDI child window is to call the CreateWindowEx function, specifying the WS_EX_MDICHILD extended style.
HWND CreateMDIWindow(
  LPCTSTR lpClassName,
  LPCTSTR lpWindowName,
  DWORD dwStyle,
  int X,
  int Y,
  int nWidth,
  int nHeight,
  HWND hWndParent,
  HINSTANCE hInstance,
  LPARAM lParam
);
*/
	HookWindowCreate(this);
	_StyleArray[(int)StyleType::Style] = cs.style;
	HWND hWnd = NULL;
	if(!HasStyle(StyleType::Inner, InnerStyle::IsMdiContainer) && _pParent != NULL && _pParent->HasStyle(StyleType::Inner, InnerStyle::IsMdiContainer))
	{
		// now copy into a MDICREATESTRUCT for real create
		MDICREATESTRUCT mcs;
		mcs.szClass = cs.lpszClass;
		mcs.szTitle = cs.lpszName;
		mcs.hOwner = GetModuleHandle(NULL);
		mcs.x = 0;
		mcs.y = 0;
		mcs.cx = 100;
		mcs.cy = 100;
		mcs.style = WS_CHILD | WS_VISIBLE;
		mcs.lParam = (LPARAM)cs.lpCreateParams;

//cc::Log::debug(_T("CreateHandle, _pParent->m_hWnd:%d"), _pParent->m_hWnd);
		hWnd = (HWND)::SendMessage(_pParent->m_hWnd, WM_MDICREATE, 0, (LPARAM)&mcs);
		//cs.dwExStyle |= WS_EX_MDICHILD;
		//hWnd = ::CreateWindowEx(WS_EX_MDICHILD, cs.lpszClass,
		//	cs.lpszName, cs.style, cs.x, cs.y, cs.cx, cs.cy,
		//	cs.hwndParent, cs.hMenu, cs.hInstance, cs.lpCreateParams);
	//hWnd = CreateMDIWindow(
//	  cs.lpszClass,
//	  cs.lpszName,
//	  WS_CHILD | WS_VISIBLE,
//	  0,
//	  0,
//	  100,
//	  100,
//	  _pParent->m_hWnd,
//	  cs.hInstance,
//		(LPARAM)cs.lpCreateParams
//	);
//cc::Log::debug(_T("CreateHandle, hWnd:%d"), hWnd);
	}
	else
	{
		hWnd = ::CreateWindowEx(cs.dwExStyle, cs.lpszClass,
			cs.lpszName, cs.style, cs.x, cs.y, cs.cx, cs.cy,
			cs.hwndParent, cs.hMenu, cs.hInstance, cs.lpCreateParams);
	}

	if(!UnhookWindowCreate())
	{
		PostNcDestroy(); // cleanup if CreateWindowEx fails too soon
	}

	CC_ASSERT(::IsWindow(m_hWnd));
	if(hWnd == NULL)
	{
		CC_LOGSTEP();
		const TCHAR* msg = _T("Control::CreateHandle, error to create Window.");
		cc::Log::fatal(msg);
		throw UserException(msg);
	}
}

bool Control::PreCreateWindow(CREATESTRUCT& cs)
{
	if((long)_StyleArray[(int)StyleType::InitControl] > 0)
	{
		if(!CC_DeferRegisterClass(CC_WNDCOMMCTLS_REG))
		{
			//even register failed,do next
			cc::Log::warn(_T("[%s::PreCreateWindow]CC_DeferRegisterClass,not OK!"), _ClassName);
			//return false;
		}
		//even register failed,do next
		CC_DeferRegisterClass((long)_StyleArray[(int)StyleType::InitControl]);
	}

	//Ensure a window class is registered
	if(cs.lpszClass == NULL)
	{
		cs.lpszClass = WndClassName::Control;
	}
	WNDCLASSEX wcx;    //this structure is used for storing information about the wnd 'class'
	memset(&wcx, 0, sizeof(WNDCLASSEX));   // start with NULL defaults
	//see if this class is created
	if(!::GetClassInfoEx(cs.hInstance, cs.lpszClass, &wcx))
	{
		HCURSOR hCursor = (HCURSOR)LoadImage(0, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_SHARED);
		wcx.cbSize			= sizeof(WNDCLASSEX);				//byte size of WNDCLASSEX struct
		//wcx.style			= _StyleArray[(int)StyleType::WindowClass];	//can ensure wnd is always redrawn
		wcx.style			= CS_DBLCLKS | CS_GLOBALCLASS | CS_HREDRAW | CS_VREDRAW;	//can ensure wnd is always redrawn
		wcx.lpfnWndProc 	= ::DefWindowProc;					//pointer to the Window Procedure
		wcx.cbClsExtra		= 0;								//Extra bytes for 'class' wnds
		wcx.cbWndExtra		= 0;								//Extra bytes for this wnd
		wcx.hInstance		= cs.hInstance;						//Application instance
		wcx.hCursor 		= hCursor; 							//Cursor for wnd
		//_hbrBackground		= (HBRUSH)::CreateSolidBrush(_corBackground);
		//CC_BIT_ON(_BackStyle, CC_BACK_STYLE_BRUSHDESTROY);
		wcx.hbrBackground	= (HBRUSH)::CreateSolidBrush(_corBackground);//Background wnd colour
		wcx.lpszMenuName	= NULL;								//Name of wnd menu
		wcx.lpszClassName	= cs.lpszClass; 					//Name of this wnd 'class'
		//Icon in top-left corner of wnd
		wcx.hIcon		= GetIcon();								//Application icon
		wcx.hIconSm 	= GetIcon();								//Icon in top-left corner of wnd

		//Register the WNDCLASSEX structure
		if(!::RegisterClassEx(&wcx))
		{
			CC_LOGSTEP();
			const TCHAR* msg = _T("Control::PreCreateWindow, Failed to register Window Class.");
			cc::Log::fatal(msg);
			CC_ASSERT(0);
			throw UserException(msg);
		}
		//can not delete brush
		//::DeleteObject(brush);
	}

	return true;
}

void Control::HookWindowCreate(Control *pControl)
{
	//Retrieve the appTHREAD_STATE pointer from TLS
	_APP_THREAD_STATE *appTHREAD_STATE = CC_APP_GetThreadStateData();
	if(appTHREAD_STATE->m_pWndInit == pControl)
	{
		return;
	}

	//Create the CBT Hook
	if (appTHREAD_STATE->m_hHookOldCbtFilter == NULL)
	{
		appTHREAD_STATE->m_hHookOldCbtFilter = ::SetWindowsHookEx(WH_CBT,
			(HOOKPROC)Control::StaticCBTProc, NULL, ::GetCurrentThreadId());

		CC_ASSERT(appTHREAD_STATE->m_hHookOldCbtFilter != NULL);

		if(appTHREAD_STATE->m_hHookOldCbtFilter == NULL)
		{
			CC_LOGSTEP();
			const TCHAR* msg = _T("Control::HookWindowCreate, SetWindowsHookEx Failed");
			cc::Log::fatal(msg);
			throw UserException(msg);
		}
	}

	appTHREAD_STATE->m_pWndInit = pControl;
}

bool Control::UnhookWindowCreate()
{
	//Retrieve the appTHREAD_STATE pointer from TLS
	_APP_THREAD_STATE *appTHREAD_STATE = CC_APP_GetThreadStateData();

	//if not removed at StaticCBTProc
	if(appTHREAD_STATE->m_hHookOldCbtFilter != NULL)
	{
		//Now remove the CBT hook
		::UnhookWindowsHookEx(appTHREAD_STATE->m_hHookOldCbtFilter);
		appTHREAD_STATE->m_hHookOldCbtFilter = NULL;
	}

	if(appTHREAD_STATE->m_pWndInit != NULL)
	{
		appTHREAD_STATE->m_pWndInit = NULL;
		return false; //was not successfully hooked
	}
	return true;
}

// WM_NCDESTROY is the absolute LAST message sent.
void Control::OnNcDestroy()
{
	// call default, unsubclass, and detach from the map
#pragma warning(disable:4244 4312)
	WNDPROC pfnWndProc = WNDPROC(GetWindowLongPtr(m_hWnd, GWLP_WNDPROC));
	// call DefWindowProc with the last message
	_APP_THREAD_STATE *_app_THREAD_STATE = CC_APP_GetThreadStateData();
	DefWndProc(&_app_THREAD_STATE->m_lastSentMsg);
	if(WNDPROC(GetWindowLongPtr(m_hWnd, GWLP_WNDPROC)) == pfnWndProc)
	{
		WNDPROC* pfnSuper = GetSuperWndProcAddr();
		if(pfnSuper != NULL)
		{
			SetWindowLongPtr(m_hWnd, GWLP_WNDPROC, INT_PTR(pfnSuper));
			*pfnSuper = NULL;
		}
	}
#pragma warning(default:4244 4312)

	if(m_hWnd != NULL)
	{
		CC_APP_GetMapHWND()->Remove(m_hWnd);
		m_hWnd = NULL;
	}

	// call special post-cleanup routine
	PostNcDestroy();
}

void Control::PostNcDestroy()
{
	// default to nothing
}

WNDPROC* Control::GetSuperWndProcAddr()
{
	return &_controlRealWndProc;
}

/////////////////////////////////////////////////////////////////////////////
// minimal layout support

// special struct for WM_SIZEPARENT
struct AFX_SIZEPARENTPARAMS
{
	HDWP hDWP;       // handle for DeferWindowPos
	RECT rect;       // parent client rectangle (trim as appropriate)
	SIZE sizeTotal;  // total size on each side as layout proceeds
	BOOL bStretch;   // should stretch to fill all space
};
#define WM_SIZEPARENT       0x0361  // lParam = &AFX_SIZEPARENTPARAMS

LRESULT CALLBACK Control::StaticCBTProc(int msg, WPARAM wParam, LPARAM lParam)
// With a CBTHook in place, the application receives additional messages.
// The HCBT_CREATEWND message is the first message to be generated when
// a window is about to be created.
{
	try
	{
		//Retrieve the appTHREAD_STATE pointer from TLS
		_APP_THREAD_STATE *appTHREAD_STATE = CC_APP_GetThreadStateData();
		//Only proceed for HCBT_CREATEWND messages
//cc::Log::debug(_T("StaticCBTProc, msg:%d"), msg);
		if(msg != HCBT_CREATEWND)
		{
			return ::CallNextHookEx(appTHREAD_STATE->m_hHookOldCbtFilter, msg, wParam, lParam);
		}

//cc::Log::debug(_T("StaticCBTProc, (HWND)wParam:%d"), (HWND)wParam);
		Control* ctl = appTHREAD_STATE->m_pWndInit;
		if(ctl != NULL)
		{
			//Store the HWND in the ctl object early
			ctl->m_hWnd = (HWND)wParam;
			// connect the HWND to MapHWND...
			CC_APP_GetMapHWND()->Add((HWND)wParam, ctl);

			WNDPROC *pOldWndProc = ctl->GetSuperWndProcAddr();

			// subclass the window with standard WndProc
			WNDPROC wndProc = StaticWindowProc;
#pragma warning(disable:4244 4312)
			WNDPROC oldWndProc = (WNDPROC)::SetWindowLongPtr(ctl->m_hWnd, GWLP_WNDPROC, (LONG_PTR)wndProc);
#pragma warning(default:4244 4312)
			if(oldWndProc != wndProc)
			{
				*pOldWndProc = oldWndProc;
			}
			appTHREAD_STATE->m_pWndInit = NULL;
		}

		//Now remove the CBT hook
		::UnhookWindowsHookEx(appTHREAD_STATE->m_hHookOldCbtFilter);
		appTHREAD_STATE->m_hHookOldCbtFilter = NULL;

		return ::CallNextHookEx(appTHREAD_STATE->m_hHookOldCbtFilter, msg, wParam, lParam);
	}
	catch(Exception exp)
	{
		CC_LOGSTEP();
		cc::Log::fatal(_T("[Control::StaticCBTProc]Exception Error:%s"), exp.GetMessage());
		cc::Log::printStackTrace();
		CC_ASSERT(0);
		throw exp;
	}
	catch(...)
	{
		CC_LOGSTEP();
		cc::Log::fatal(_T("[Control::StaticCBTProc]Error!"));
		cc::Log::printStackTrace();
		CC_ASSERT(0);
	}

	return 0;
}

LRESULT CALLBACK Control::StaticWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
#ifndef _DEBUG_
	try
#endif
	{
		//i found that many Control is same as last
		_APP_THREAD_STATE *_app_THREAD_STATE = CC_APP_GetThreadStateData();
		Control* ctl;
		if(_app_THREAD_STATE->m_lastSentMsg.HWnd == hWnd)
		{
			ctl = _app_THREAD_STATE->m_lastControl;
		}
		else
		{
			//Get the Window pointer from the HWND map
			ctl = Control::FromHandle(hWnd);
			_app_THREAD_STATE->m_lastControl = ctl;
			_app_THREAD_STATE->m_lastSentMsg.HWnd = hWnd;
		}
		_app_THREAD_STATE->m_lastSentMsg.Msg = uMsg;
		_app_THREAD_STATE->m_lastSentMsg.WParam = wParam;
		_app_THREAD_STATE->m_lastSentMsg.LParam = lParam;

		if(ctl != NULL)
		{
			EventArgs ee(hWnd, uMsg, wParam, lParam);
#ifdef _DEBUG_
			if(CC_APP_GetConfig()->GetIsDebug())
			{
				CC_LogWndMsg(hWnd, uMsg, wParam, lParam, ctl->GetWndClassName(), ctl->GetName());
			}
#endif
			return ctl->WndProc(&ee);
		}
		else
		{
			cc::Log::warn(_T("[Control::StaticWindowProc]mapWins->ContainsKey(hWnd),not exist control for [%d]!"), hWnd);
		}

		return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
#ifndef _DEBUG_
	catch(Exception exp)
	{
		CC_LOGSTEP();
		cc::Log::fatal(_T("[Control::StaticWindowProc]Exception Error:%s"), exp.GetMessage());
		cc::Log::printStackTrace();
		CC_ASSERT(0);
		throw exp;
	}
	catch(...)
	{
		CC_LOGSTEP();
		cc::Log::fatal(_T("[Control::StaticWindowProc]Error!"));
		cc::Log::printStackTrace();
		CC_ASSERT(0);
	}
#endif

	return 0;
}

// WndProc - calls appriate On... function for the give
// message
LRESULT Control::WndProc(EventArgs *e)
{
	POINT pt;
	RECT rect;
	switch(e->Msg)
	{
	case WM_GETDLGCODE:
		//e->Result = DefWndProc(e);
		//e->Handled = true;
		//if(e->LParam != NULL)
		//{
		//	//MSG* lpmsg = (LPMSG)e->LParam;
		//	break;
		//}
		//2008/11/16,while move toolbar into rebar,then dead loop for WM_GETDLGCODE
		//if(_pParent != NULL && cc::Str::Cmp(_ClassName, _T("Button"), true) == 0)
		if(_pParent != NULL && _ClassName == WndClassName::Button)
		{
			//redio,checkbox need this message
			::GetCursorPos(&pt);
			::GetWindowRect(_pParent->m_hWnd, &rect);
			if(::PtInRect(&rect, pt))
			{
				break;
			}
		}
		return 0;
	case WM_ENTERMENULOOP:
		Menu_OnEnterLoop(e);
		break;
	//case WM_INITMENU:
	//	//not need
	//	break;
	case WM_INITMENUPOPUP:
		Menu_OnPopup(e);
		break;

	case WM_MEASUREITEM:
		{
			//The WM_MEASUREITEM message is sent to the owner window of a combo box, list box, 
			//list view control, or menu item when the control or menu is created.
			//wParam
			//   Contains the value of the CtlID member of the MEASUREITEMSTRUCT structure pointed to 
			//by the lpMeasureItem parameter. This value identifies the control that sent the 
			//WM_MEASUREITEM message. If the value is zero, the message was sent by a menu. 
			//If the value is nonzero, the message was sent by a combo box or by a list box. 
			//If the value is nonzero, and the value of the itemID member of the MEASUREITEMSTRUCT 
			//pointed to by lpMeasureItem is (UINT)-1,the message was sent by a combo edit field.
			MeasureItemEventArgs ee(e);
			ee.data = (LPMEASUREITEMSTRUCT)e->LParam;
			//HDC is created at xxx if needed
			//ee.hDC = ::GetDC(m_hWnd);
			if(ee.data->CtlType == ODT_MENU)
			{
				Control::PreMeasureItem(&ee);
			}
			else
			{
				PreMeasureItem(&ee);
			}
			//::ReleaseDC(m_hWnd, ee.hDC);
		}
		break;
	case WM_DRAWITEM:
		{
			//The WM_DRAWITEM message is sent to the parent window of an owner-drawn button, combo box,
			//list box, or menu when a visual aspect of the button, combo box, list box, or menu has changed.
			DrawItemEventArgs ee(e);
			ee.data = (LPDRAWITEMSTRUCT)e->LParam;
			if(ee.data->CtlType == ODT_MENU)
			{
				Control::PreDrawItem(&ee);
			}
			else
			{
				PreDrawItem(&ee);
			}
		}
		break;
	//case WM_COMPAREITEM:
	//	{
			//wParam:Specifies the identifier of the control that sent the WM_COMPAREITEM message. 
			//lpCompareItem = (LPCOMPAREITEMSTRUCT)lParam;
			//lpCompareItem:Pointer to a COMPAREITEMSTRUCT structure that contains the identifiers and application-supplied data for two items in the combo or list box.
	//		CompareItemEventArgs ee(e, (LPCOMPAREITEMSTRUCT)e->LParam);
	//		OnCompareItem(&ee);
	//	}
	//	break;
	//case WM_DELETEITEM:
	//	{
			//wParam:Specifies the identifier of the control that sent the WM_DELETEITEM message. 
			//lpDeleteItem = (LPDELETEITEMSTRUCT) lParam;
			//lpDeleteItem:Pointer to a DELETEITEMSTRUCT structure that contains information about the item deleted from a list box.
	//		DeleteItemEventArgs ee(e, (LPDELETEITEMSTRUCT)e->LParam);
	//		OnDeleteItem(&ee);
	//	}
	//	break;
	//case WM_VKEYTOITEM:
	//	{
			//wParam:The low-order word specifies the virtual-key code of the key the user pressed. The high-order word specifies the current position of the caret. 
			//lParam:Handle to the list box.
	//		VKeyToItemEventArgs ee(e, LOWORD(e->WParam), HIWORD(e->WParam));
	//		OnVKeyToItem(&ee);
	//	}
	//	break;
	//case WM_CHARTOITEM:
	//	{
			//wParam:The low-order word specifies the value of the key the user pressed. The high-order word specifies the current position of the caret. 
			//lParam:Handle to the list box.
	//		CharToItemEventArgs ee(e, LOWORD(e->WParam), HIWORD(e->WParam));
	//		OnCharToItem(&ee);
	//	}
	//	break;
	//case WM_CAPTURECHANGED:
	//	//do nothing
	//	break;
	//case WM_UNINITMENUPOPUP:
	//	//do nothing
	//	break;
	//case WM_ENTERIDLE:
	//	//can not get this msg under w2000
	//	break;
	case WM_MENUSELECT:
		Menu_OnSelect(e);
		break;
	case WM_EXITMENULOOP:
		Menu_OnExitLoop(e);
		break;
	case WM_COMMAND:
		PreOnCommand(e);
		break;
	case WM_MENUCHAR:
		{
			UINT nChar = (TCHAR)LOWORD(e->WParam);
			MenuItem* item = (MenuItem*)Menu::FromHandle((HMENU)e->LParam);
			if(item != NULL)
			{
				LRESULT lr = item->FindHotKey(nChar);
				if(lr != NULL)
				{
					return lr;
				}
			}
		}
		break;
	case WM_CONTEXTMENU:
		{
			MouseEventArgs ee(e);
			ee.Button = MouseButtons::Right;
			if(e->LParam != -1)
			{
				ee.X = GET_X_LPARAM(e->LParam);
				ee.Y = GET_Y_LPARAM(e->LParam);
			}
			else
			{
				::GetCursorPos(&pt);
				ee.X = pt.x;
				ee.Y = pt.y;
			}
			OnContextMenu(&ee);
		}
		break;
	//Button routines
	case WM_LBUTTONDOWN:
		{
			MouseEventArgs ee(e);
			ee.Button = MouseButtons::Left;
			ee.X = GET_X_LPARAM(e->LParam);
			ee.Y = GET_Y_LPARAM(e->LParam);
			OnMouseDown(&ee);
		}
		break;
	case WM_LBUTTONUP:
		{
			MouseEventArgs ee(e);
			//ee.baseArgs = e;
			//ee.hwnd = e->hwnd;
			//ee.message = e->Msg;
			//ee.lParam = e->LParam;
			//ee.wParam = e->WParam;
			ee.Button = MouseButtons::Left;
			ee.X = GET_X_LPARAM(e->LParam);
			ee.Y = GET_Y_LPARAM(e->LParam);
			OnMouseUp(&ee);
		}
		break;
	case WM_LBUTTONDBLCLK:
		OnDoubleClick(e);
		break;
		// OnDragEnd
		// OnDragEnter
		// OnDragLeave
		// OnDragOver
		// OnQueryContinueDrag
	case WM_NCLBUTTONDBLCLK:
#if defined(_DEBUG_) || defined(_DESIGN)
		//code for design
		//do nothing while design
		if(CC_APP_GetDesignBarActive())
		{
			bool isKey = ((GetKeyState(VK_CONTROL) & 0x8000) != 0); // && (GetKeyState(VK_SHIFT) & 0x8000) != 0
			if((CC_APP_GetConfig()->GetIsDesign() && !isKey) || (!CC_APP_GetConfig()->GetIsDesign() && isKey))
			{
				e->Handled = true;
				return 0;
			}
		}
#endif
		break;
		//return wNcMouseDblClick(LeftButton, wParam, LOWORD(lParam), HIWORD(lParam));
	//case WM_MBUTTONDBLCLK:
	//	break;
		//return wMouseDblClick(MiddleButton, wParam, LOWORD(lParam), HIWORD(lParam));
	//case WM_NCLBUTTONDOWN:
		//break;
		//POINTS p = MAKEPOINTS(lParam);
		//return wNcButtonDown(LeftButton, wParam, p.x, p.y); }
	//case WM_NCRBUTTONDOWN:
		//break;
		//POINTS p = MAKEPOINTS(lParam);
		//return wNcButtonDown(RightButton, wParam, p.x, p.y); }
	//case WM_NCLBUTTONUP:
	//	break;
		//POINTS p = MAKEPOINTS(lParam);
		//return wNcButtonUp(LeftButton, wParam, p.x, p.y); }
	//case WM_NCRBUTTONUP:
	//	break;
		//POINTS p = MAKEPOINTS(lParam);
		//return wNcButtonUp(RightButton, wParam, p.x, p.y); }
	case WM_RBUTTONDOWN:
		{
			MouseEventArgs ee(e);
			ee.Button = MouseButtons::Right;
			ee.X = GET_X_LPARAM(e->LParam);
			ee.Y = GET_Y_LPARAM(e->LParam);
			OnMouseDown(&ee);
		}
		break;
	case WM_RBUTTONUP:
		{
			//wParam:Indicates whether various virtual keys are down. This parameter can be one or more of the following values.
			//  MK_CONTROL:The CTRL key is down.
			//  MK_LBUTTON:The left mouse button is down.
			//  MK_MBUTTON:The middle mouse button is down.
			//  MK_RBUTTON:The SHIFT key is down.
			//  MK_XBUTTON1:Windows 2000/XP: The first X button is down.
			//  MK_XBUTTON2:Windows 2000/XP: The second X button is down.
			//If an application processes this message, it should return zero. 
			MouseEventArgs ee(e);
			ee.Button = MouseButtons::Right;
			ee.X = GET_X_LPARAM(e->LParam);
			ee.Y = GET_Y_LPARAM(e->LParam);
			OnMouseUp(&ee);
		}
		break;
		//return wMouseUp(RightButton, wParam, LOWORD(lParam), HIWORD(lParam));
	//case WM_RBUTTONDBLCLK:
	//	break;
		//return wMouseDblClick(RightButton, wParam, LOWORD(lParam), HIWORD(lParam));
		//Scrolling Routines
		//int nScrollCode = (short)LOWORD(wParam);
		//int nPos = (short)HIWORD(wParam);
	case WM_HSCROLL:
		{
			if(e->LParam == NULL)
			{
				ScrollEventArgs ee(e);
				ee.Horizontal = true;
				ee.NewValue = (int)HIWORD(e->WParam);
				OnScroll(&ee);
			}
			else
			{
				ReflectLastMsg((HWND)e->LParam, e);
			}
		}
		break;
	case WM_VSCROLL:
		{
			if(e->LParam == NULL)
			{
				ScrollEventArgs ee(e);
				ee.Horizontal = false;
				ee.NewValue = (int)HIWORD(e->WParam);
				OnScroll(&ee);
			}
			else
			{
				ReflectLastMsg((HWND)e->LParam, e);
			}
		}
		break;
#if _WIN32_WINNT >= 0x0400
	case WM_MOUSEWHEEL:
		OnMouseWheel(e);
		break;
#endif

//Rest of routines alphabetically
	//case WM_ACTIVATE:
	//	break;
		//return wActivate(LOWORD(wParam), HIWORD(wParam), (HWND)lParam);
	//case WM_ACTIVATEAPP:
	//	break;
		//return wActivateApp(wParam, lParam);
	//case WM_CANCELMODE:
	//	//return wCancelMode();
	//	break;
	case WM_CHAR:
		ProcessKeyMessage(e);
		break;
	//case WM_CHILDACTIVATE:
		//return wChildActivate();
	//	break;
	//case WM_NCCREATE:
	//	break;
	case WM_CREATE:
		OnHandleCreated(e);
		break;
	case WM_ERASEBKGND:
		{
			PaintEventArgs ee(e);
			OnPaintBackground(&ee);
		}
		break;
	case WM_PAINT:
		{
			PaintEventArgs ee(e);
			OnPaint(&ee);
#if defined(_DEBUG_) || defined(_DESIGN)
			if(!e->Handled)
			{
				e->Handled = true;
				e->Result = DefWndProc(e);
			}
			CC_APP_DrawFocusFrame(this);
#endif
		}
		break;
	case WM_CTLCOLORMSGBOX:
	case WM_CTLCOLOREDIT:
	case WM_CTLCOLORLISTBOX:
	case WM_CTLCOLORBTN:
	case WM_CTLCOLORDLG:
	case WM_CTLCOLORSCROLLBAR:
	case WM_CTLCOLORSTATIC:
		OnCtlColorEdit(e);
		break;
	case WM_ENABLE:
		OnEnabledChanged(e);
		break;
	//case WM_ENTERSIZEMOVE:
	//	//return wEnterSizeMove();
	//	break;
	//case WM_EXITSIZEMOVE:
	//	//return wExitSizeMove();
	//	break;
	//case WM_GETMINMAXINFO:
	//	//return wGetMinMaxInfo((MINMAXINFO*) lParam);
	//	break;
	case WM_SETFOCUS:
		OnEnter(e);
		OnGotFocus(e);
		break;
	case WM_FONTCHANGE:
		//from system while system font hanged(to form only)
		OnFontChanged(e);
		break;
	case WM_DESTROY:
		if(!HasStyle(StyleType::Inner2, Inner2Style::HandleDestroyed))
		{
			OnHandleDestroyed(e);
		}
		//if(e->WParam == 1)
		//{
		//	::DestroyWindow(m_hWnd);
		//}
		break;
	//case WM_HELP:
	//	// FIXME:
	//	//OnHelpRequested(e);
	//	break;
	case WM_KEYDOWN:
		ProcessKeyMessage(e);
		break;
	case WM_KEYUP:
		ProcessKeyMessage(e);
		break;
	case WM_KILLFOCUS:
		OnLeave(e);
		OnLostFocus(e);
		break;
	//case WM_MOUSEACTIVATE:
	//	break;
	case WM_MOUSEHOVER: // called by TrackMouseEvent
		OnMouseHover(e);
		break;
	case WM_MOUSELEAVE: // called by TrackMouseEvent
		OnMouseLeave(e);
		break;
	case WM_MOUSEMOVE:
		{
/*
wParam
    Indicates whether various virtual keys are down. This parameter can be one or more of the following values.
    MK_CONTROL  The CTRL key is down.
    MK_LBUTTON  The left mouse button is down.
    MK_MBUTTON  The middle mouse button is down.
    MK_RBUTTON  The right mouse button is down.
    MK_SHIFT    The SHIFT key is down.
    MK_XBUTTON1 Windows 2000/XP: The first X button is down.
    MK_XBUTTON2 Windows 2000/XP: The second X button is down.
lParam
    The low-order word specifies the x-coordinate of the cursor. The coordinate is relative to the upper-left corner of the client area.
    The high-order word specifies the y-coordinate of the cursor. The coordinate is relative to the upper-left corner of the client area. 
*/
			MouseEventArgs ee(e);
			ee.Button = MouseButtons::None;
			if((e->WParam & MK_LBUTTON) == MK_LBUTTON)
			{
				ee.Button = (MouseButtons::Type)(ee.Button | MouseButtons::Left);
//cc::Log::debug(_T("ee.Button:%d, MouseButtons::Left:%d, e->WParam:%d, MK_LBUTTON:%d"), ee.Button,  MouseButtons::Left, e->WParam, MK_LBUTTON);
			}
			if((e->WParam & MK_MBUTTON) == MK_MBUTTON)
			{
				ee.Button = (MouseButtons::Type)(ee.Button | MouseButtons::Middle);
//cc::Log::debug(_T("ee.Button:%d, MouseButtons::Middle:%d, e->WParam:%d, MK_MBUTTON:%d"), ee.Button,  MouseButtons::Middle, e->WParam, MK_MBUTTON);
			}
			if((e->WParam & MK_RBUTTON) == MK_RBUTTON)
			{
				ee.Button = (MouseButtons::Type)(ee.Button | MouseButtons::Right);
//cc::Log::debug(_T("ee.Button:%d, MouseButtons::Right:%d, e->WParam:%d, MK_RBUTTON:%d"), ee.Button,  MouseButtons::Right, e->WParam, MK_RBUTTON);
			}
			POINTS pos = MAKEPOINTS(e->LParam);
			ee.X = pos.x;
			ee.Y = pos.y;
			OnMouseMove(&ee);
		}
		break;
	//case WM_NCACTIVATE:
	//	//return wNcActivate(wParam, (HWND)lParam);
	//	OnNcActivate(e);
	//	break;
	case WM_NCCALCSIZE:
		OnNcCalcSize(static_cast<BOOL>(e->WParam), reinterpret_cast<NCCALCSIZE_PARAMS*>(e->LParam));
		break;
	case WM_NCHITTEST:
		//return wNcHitTest(LOWORD(lParam), HIWORD(lParam));
		OnNcHittest(e);
		break;
	case WM_NCDESTROY:
		//Last message received by a window
		OnNcDestroy();
		break;
	case WM_NOTIFY:
		//NMHDR nmhdr = (NMHDR)Marshal.PtrToStructure(m.LParam, typeof(NMHDR));
		//if(Control.ReflectMessage(nmhdr.hwndFrom, ref m))
		//{
		//}
		// FIXME: get NM_CLICKED msg from pnmh
		// OnClick(e);
		OnNotifyMessage(e);
		break;
	//case WM_PARENTNOTIFY:
	//	break;
	case WM_SETCURSOR:
		OnSetCursor(e);
		//return wSetCursor(window, LOWORD (lParam), HIWORD (lParam));
		break;
	//case WM_SETTINGCHANGE:
	//	//return wSettingChange(wParam, (LPCTSTR) lParam);
	//	break;
	case WM_MOVE:
		OnMove(e);
		//UpdateBounds();
		break;
	case WM_MOVING:
		OnMoving(e);
		break;
	case WM_SIZE:
		OnResize(e);
		//UpdateBounds();
		//if(GetStyle(ControlStyles.ResizeRedraw))
		//{
    	//	Invalidate();		
		//}
		break;
#if defined(WM_SIZING)
	case WM_SIZING:
		//from Windows 2000
		OnResizing(e);
		break;
#endif
	case WM_SYSCOMMAND:
		//return wSysCommand(wParam, LOWORD(lParam), HIWORD(lParam));
		OnSysCommand(e);
		break;
	case WM_WINDOWPOSCHANGED:
		OnLocationChanged(e);
		break;
	case WM_WINDOWPOSCHANGING:
		//return wPosChanging((WINDOWPOS *)lParam);
		break;
	case WM_STYLECHANGED:
		OnStyleChanged(e);
		break;
	case WM_SYSCOLORCHANGE:
		OnSystemColorsChanged(e);
		break;
	case WM_SETTEXT:
		OnTextChanged(e);
		break;
	case WM_SETFONT:
		OnFontChanged(e);
		break;
	case WM_SHOWWINDOW:
		OnVisibleChanged(e);
		break;
//	case WM_CTLCOLORLISTBOX:
		//::SetTextColor(e.wParam, ::RGB(ForeColor));
		//Win32.SetBkColor(m.WParam, 0x00FF00);
		//m.Result = Win32.GetStockObject(GSO_.LTGRAY_BRUSH);
//		break;
	//special for timer
	case WM_TIMER:
		OnInnerTimer(e);
		break;
	case WM_DROPFILES:
		OnDropFiles(e);
		break;
	case CC_WM_MAINMSG:
		{
			switch(e->WParam)
			{
			case CC_WM_MAINMSG_WPARAM_GETFORM:
				if(_pParent != NULL)
				{
					return ::SendMessage(_pParent->m_hWnd, CC_WM_MAINMSG, CC_WM_MAINMSG_WPARAM_GETFORM, 0);
				}
				else
				{
					return ::SendMessage(::GetParent(m_hWnd), CC_WM_MAINMSG, CC_WM_MAINMSG_WPARAM_GETFORM, 0);
				}
			case CC_WM_MAINMSG_WPARAM_NOTIFYICON_ID:
				{
					//special for NotifyIcon,this is NotifyIcon Id
					::GetCursorPos(&pt);
					MouseEventArgs ee(e);
					ee.X = pt.x;
					ee.Y = pt.y;
					switch(LOWORD(e->LParam))
					{
					case WM_LBUTTONUP:
						ee.Button = MouseButtons::Left;
						OnMouseUp(&ee);
						break;
					case WM_RBUTTONUP:
						ee.Button = MouseButtons::Right;
						OnMouseUp(&ee);
						break;
					case WM_LBUTTONDBLCLK:
						ee.Button = MouseButtons::Left;
						OnDoubleClick(e);
						break;
					case WM_MOUSEMOVE:
						ee.Button = MouseButtons::None;
						OnMouseMove(&ee);
						break;
					case WM_LBUTTONDOWN:
						ee.Button = MouseButtons::Left;
						OnMouseDown(&ee);
						break;
					case WM_RBUTTONDOWN:
						ee.Button = MouseButtons::Right;
						OnMouseDown(&ee);
						break;
					default:
						break;
					}
				} //case CC_WM_MAINMSG_WPARAM_NOTIFYICON_ID:
				return 0;
			case CC_WM_MAINMSG_WPARAM_GETINNERSTYLE:
				return _StyleArray[(int)StyleType::Inner];
			case CC_WM_MAINMSG_WPARAM_SETINNERSTYLE:
				//SetInnerStyle((InnerStyle::Type)e->LParam, true);
				_StyleArray[(int)StyleType::Inner] = (DWORD)e->LParam;
				_nUpdateFlag |= UpdateFlag::UpdateInnerStyle;
				Update();
				return 0;
			case CC_WM_MAINMSG_WPARAM_GETOUTERSTYLE:
				return _StyleArray[(int)StyleType::Outer];
			case CC_WM_MAINMSG_WPARAM_SETOUTERSTYLE:
				//SetOuterStyle((OuterStyle::Type)e->LParam, true);
				_StyleArray[(int)StyleType::Outer] = (DWORD)e->LParam;
				_nUpdateFlag |= UpdateFlag::UpdateOuterStyle;
				Update();
				return 0;
			case CC_WM_MAINMSG_WPARAM_CLEARBRUSHPARENT:
				this->_pParentBrush = NULL;
				return 0;
			case CC_WM_MAINMSG_WPARAM_CLEARBRUSHALL:
				this->_pParentBrush = NULL;
				if(_lstControls != NULL)
				{
					for(int n = _lstControls->GetSize() - 1; n >= 0; n--)
					{
						Control* ctl = _lstControls->GetKey(n);
						::SendMessage(ctl->m_hWnd, CC_WM_MAINMSG, CC_WM_MAINMSG_WPARAM_CLEARBRUSHALL, 0);
					}
				}
				return 0;
			////dispose control
			//case CC_WM_MAINMSG_WPARAM_DISPOSE:
			//	if(!GetIsDispose())
			//	{
			//		OnHandleDestroyed(e);
			//	}
			//	break;
			//sample for others
			//case 999:
			//	{
			//	}
			//	break;
			} //switch(e->WParam)
		}
		e->Handled = true;
		break;
	case CC_WM_CHILD_UPDATED:
		{
			MSG msg;
			while(::PeekMessage(&msg, m_hWnd, CC_WM_CHILD_UPDATED, CC_WM_CHILD_UPDATED, PM_REMOVE))
			{
#ifdef _DEBUG_
				if(CC_APP_GetConfig()->GetIsDebug())
				{
					CC_TRACE((_T("Control::WndProc(), CC_WM_CHILD_UPDATED PM_REMOVE, ClassName:%s, Name:%s, Text:%s"), _ClassName, _Name, _Text));
				}
#endif
			}
			_nUpdateFlag |= UpdateFlag::UpdateChildUpdated;
			Update();
		}
		e->Handled = true;
		break;
	default:
		break;
	}

	if(!e->Handled)
	{
		e->Result = DefWndProc(e);
	}
	return e->Result;
}

// This function lets Windows or/and default Windows control process message
// Classes have to call it if they do not handle message in WndProc or
// default handling is needed.
LRESULT Control::DefWndProc(EventArgs *e)
{
	CC_ASSERT(e != NULL);

	if(_controlRealWndProc != NULL)
	{
		return ::CallWindowProc(_controlRealWndProc, e->HWnd, e->Msg, e->WParam, e->LParam);
	}
	else
	{
		return ::DefWindowProc(e->HWnd, e->Msg, e->WParam, e->LParam);
	}
}

LRESULT Control::DefWndProc(int msg, WPARAM wParam, LPARAM lParam)
{
	CC_ASSERT(::IsWindow(m_hWnd));

	if(_controlRealWndProc != NULL)
	{
		return ::CallWindowProc(_controlRealWndProc, m_hWnd, msg, wParam, lParam);
	}
	else
	{
		return ::DefWindowProc(m_hWnd, msg, wParam, lParam);
	}
}

//void Control::CreateGraphics()
//{
	//CPaintDC dc(this); // device context for painting
	//Graphics graphics( dc.m_hDC );
	//graphics.Clear( Color( 255 , 0 , 0 , 0 ) );
	//graphics.DrawString
//}

//hFontAppShare is deleted at App::~App()
HFONT Control::CreateDefaultFont()
{
	if(hFontAppShare == NULL)
	{
		hFontAppShare = ::CreateFontIndirect(&FontConfig::lfControl);
	}
	/*
	HFONT hFont = ::CreateFont(
		-12,					// height of font,size 8:-11;
		0,						// average character width
		0,						// angle of escapement
		0,						// base-line orientation angle
		FW_NORMAL,				// font weight
		FALSE,					// italic attribute option
		FALSE,					// underline attribute option
		0,						// strikeout attribute option
		DEFAULT_CHARSET,		// character set identifier
		OUT_DEFAULT_PRECIS,		// output precision
		CLIP_DEFAULT_PRECIS,	// clipping precision
		DEFAULT_QUALITY,		// output quality
		DEFAULT_PITCH,			// pitch and family
		_T("MS UI Gothic")		// typeface name
	);
	*/
	return hFontAppShare;
}

void Control::CreateControl()
{
	if(!HasStyle(StyleType::Inner2, Inner2Style::HandleCreated))
	{
		//must set _bIsHandleCreated in CreateHandle
		CreateHandle();
	}
}

////2007/2/9 i modified not PreProcessMessage with hWndStop,for will do it at where by called(CC_PreProcessMessage)
//bool __stdcall Control::WalkPreTranslateTree(HWND hWndStop, MSG* pMsg, UINT_PTR keyData)
//{
//	// walk from the target window up to the hWndStop window checking
//	//  if any window wants to translate this message
//	for(HWND hWnd = pMsg->hwnd; hWnd != NULL; hWnd = ::GetParent(hWnd))
//	{
//		// got to hWndStop window without interest
//		if(hWnd == hWndStop)
//		{
//			break;
//		}
//
//		Control* pWnd = Control::FromHandle(hWnd);
//		if(pWnd != NULL)
//		{
//			// target window is a C++ window
//			if(pWnd->PreProcessMessage(pMsg, keyData))
//			{
//				return true; // trapped by target window (eg: accelerators)
//			}
//		}
//	}
//	return false;       // no special processing
//}
//
//from 20070323,topform can get all message at PreProcessMessage
bool Control::PreProcessMessage(MSG* pMsg, UINT_PTR& keyData)
{
//cc::Log::debug(_T("Control::PreProcessMessage[%s,%s]pMsg->message:%d, keyData:%d"), this->_ClassName, this->_Name, pMsg->message, keyData);
	// Handle dialog and command keys.
	if(pMsg->message == WM_KEYDOWN || pMsg->message == WM_SYSKEYDOWN)
	{
		if(ProcessCmdKey(pMsg, keyData))
		{
			return true;
		}
		if(!IsInputKey(keyData))
		{
			EventArgs ee(m_hWnd, pMsg->message, pMsg->wParam, pMsg->lParam);
			PreKeyDown.Invoke((Object*)this, &ee);
			if(ee.Handled)
			{
				return true;
			}
			return ProcessDialogKey(keyData);
		}
		else
		{
			//notice position of called that IsInputKey,so not call form's PreProcessMessage
			keyData = 0;
			return false;
		}
	}
	else if(pMsg->message == WM_CHAR)
	{
		// Alt key presed?
		BOOL bAlt = HIWORD(pMsg->lParam) & KF_ALTDOWN;
		//Shu.KKb 20080518:Alt nevel be true,should move to WM_SYSKEYDOWN
		if(bAlt || !IsInputChar((TCHAR)(pMsg->wParam)))
		{
			return ProcessDialogChar((TCHAR)(pMsg->wParam));
		}
	}
	return false;
}

bool Control::ProcessCmdKey(MSG* pMsg, UINT_PTR& keyData)
{
	if(_pContextMenu != NULL && _pContextMenu->ProcessCmdKey(pMsg, keyData))
	{
		return true;
	}
	if(_pParent != NULL)
	{
		return _pParent->ProcessCmdKey(pMsg, keyData);
	}
	return false;
}

// Determine if a key is recognized by a control as an input key.
bool Control::IsInputKey(UINT_PTR& keyData)
{
	return false;
}

// Process a dialog key.
bool Control::ProcessDialogKey(UINT_PTR& keyData)
{
//err(_T("Control::ProcessDialogKey[%s,%s]_pParent != NULL:%d"), this->_ClassName, this->_Name, _pParent != NULL);
	if((keyData == VK_TAB || keyData == (VK_TAB | SHORTCUT_SHIFT)) && _lstControls != NULL && _lstControls->GetSize() > 0)
	{
		bool isShift = ((keyData & SHORTCUT_SHIFT) != 0);
		//err(_T("ProcessDialogKey[%s,%s]TabIndex:%d, isShift:%d"), this->_ClassName, this->_Name, this->TabIndex, isShift);
		Control* ctlNext = GetNextControl(NULL, isShift);
		if(ctlNext != NULL)
		{
			//err(_T("ProcessDialogKey[%s,%s]TabIndex:%d, isShift:%d, [ctlNext[%s,%s]TabIndex:%d]"), this->_ClassName, this->_Name, this->TabIndex, isShift, ctlNext->_ClassName, ctlNext->_Name, ctlNext->TabIndex);
			ctlNext->Focus(isShift);
			return true;
		}
		//let parent do it
	}
	// By default, pass the message up to our parent.
	if(_pParent != NULL)
	{
		return _pParent->ProcessDialogKey(keyData);
	}
	else
	{
		return false;
	}
}

bool Control::IsInputChar(TCHAR tchar)
{
	// if returns false then continue ProcessDialogChar,if ProcessDialogChar also returns false then loop next control
	// if returns true then loop to next control
	return true;
}

bool Control::ProcessDialogChar(TCHAR tchar)
{
	// By default, pass the message up to our parent.
	if(_pParent != NULL)
	{
		return _pParent->ProcessDialogChar(tchar);
	}
	else
	{
		return false;
	}
}

bool Control::DoProcessMnemonic(TCHAR tchar)
{
	//if(GetMnemonic() == tchar)
	//{
	//	//do sth.
	//	return true;
	//}
	return false;
}

bool Control::ProcessMnemonic(TCHAR tchar)
{
	//it is from parent to child
	if(!GetVisible() || !GetEnabled())
	{
		return false;
	}
	if(DoProcessMnemonic(tchar))
	{
		return true;
	}
	if(_lstControls != NULL)
	{
		for(int n = _lstControls->GetSize() - 1; n >= 0; n--)
		{
			Control* ctl = _lstControls->GetKey(n);
			if(ctl->ProcessMnemonic(tchar))
			{
				return true;
			}
		}
	}
	return false;
}

bool Control::ProcessKeyMessage(EventArgs *e)
{
	GiveFeedbackEventArgs ee(e);
	ee.Cancel = false;
	OnGiveFeedback(&ee);
	if(ee.Cancel)
	{
		return true;
	}
	// By default, pass the message up to our parent.
	if(ProcessKeyPreview(e))
	{
		return true;
	}
	return ProcessKeyEventArgs(e);
}

bool Control::ProcessKeyPreview(EventArgs *e)
{
//cc::Log::debug(_T("ProcessKeyPreview[%s,%s]e->Msg:%d"), this->_ClassName, this->_Name, e->Msg);
	// By default, pass the message up to our parent.
	if(_pParent != NULL)
	{
		return _pParent->ProcessKeyPreview(e);
	}
	return false;
}

bool Control::ProcessKeyEventArgs(EventArgs *e)
{
	switch(e->Msg)
	{
	case WM_KEYDOWN:
		{
			KeyEventArgs ee(e);
			OnKeyDown(&ee);
		}
		break;
	case WM_CHAR:
		{
			KeyPressEventArgs ee(e);
			OnKeyPress(&ee);
		}
		break;
	case WM_KEYUP:
		{
			KeyEventArgs ee(e);
			OnKeyUp(&ee);
		}
		break;
	}
	return e->Handled;
}

//for menubar to get form msg before form
bool Control::PreFormProcessMessage(MSG* pMsg, UINT_PTR& keyData)
{
	//if need pass to child of PreFormProcessMessage
	if(_lstControls != NULL)
	{
		for(int n = _lstControls->GetSize() - 1; n >= 0; n--)
		{
			Control* ctl = _lstControls->GetKey(n);
			if(ctl->HasStyle(StyleType::Inner, InnerStyle::DeliverFormMessage) && ctl->PreFormProcessMessage(pMsg, keyData))
			{
				return true;
			}
		}
	}
	return false;
}
// private/protected/internal used methods
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// public
void Control::Show()
{
	//if(!HasStyle(StyleType::Inner2, Inner2Style::HandleCreated))
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) == 0)
	{
		CreateControl();
	}
	SetVisible(true);
}

//bool setLastControl is if have child,set first of last
bool Control::Focus(bool setLastControl)
{
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) == 0)
	{
		return false;
	}

//err(_T("Focus,this[%s,%s]setLastControl:%d"), this->_ClassName, this->_Name, setLastControl);
	int nCnt;
	if(_lstControls == NULL || (nCnt = _lstControls->GetSize()) <= 0)
	{
		return (::SetFocus(m_hWnd) != NULL);
	}
	//Control* curControl = GetFocus();
	//find the control that it's parent is this
	//for(; curControl != NULL && curControl->_pParent != this; curControl = curControl->_pParent)
	//{
	//}
	//if(curControl == NULL || _lstControls->GetIndex(curControl) < 0)
	//{
		//the focused is not me or child
		Control* ctlMinTabIndex = NULL;
		int nMinTabIndex = -1;
		if(!setLastControl)
		{
			//if the Focus is not child,search the first child
			for(int n = 0; n < nCnt; n++)
			{
				Control* ctl = _lstControls->GetKey(n);
				if(ctl->IsTabStop() && (nMinTabIndex == -1 || nMinTabIndex > ctl->TabIndex))
				{
					nMinTabIndex = ctl->TabIndex;
					ctlMinTabIndex = ctl;
				}
			}
		}
		else
		{
			//search last
			for(int n = nCnt - 1; n >= 0; n--)
			{
				Control* ctl = _lstControls->GetKey(n);
				if(ctl->IsTabStop() && (nMinTabIndex == -1 || nMinTabIndex < ctl->TabIndex))
				{
					nMinTabIndex = ctl->TabIndex;
					ctlMinTabIndex = ctl;
				}
			}
		}
		if(ctlMinTabIndex != NULL)
		{
//err(_T("Focus,ctlMinTabIndex[%s,%s]setLastControl:%d"), ctlMinTabIndex->_ClassName, ctlMinTabIndex->_Name, setLastControl);
			return ctlMinTabIndex->Focus(setLastControl);
		}
		return false;
	//}
	//the focused is child
	//return true;
}

//if not stop even WS_TABSTOP&WS_VISIBLE,need overwrite this
bool Control::IsTabStop()
{
	CC_ASSERT(::IsWindow(m_hWnd));

	if(_pClient != NULL)
	{
		return _pClient->IsTabStop();
	}
	if(HasStyle(StyleType::Style, WS_TABSTOP) && GetVisible() && GetEnabled())
	{
		return true;
	}
	return false;
}

bool Control::Focused()
{
	CC_ASSERT(::IsWindow(m_hWnd));

	return (this->GetFocus() == this);
}

Control* Control::GetFocus()
{
	//meybe container contains ie...,so also search parent
	HWND hWnd = ::GetFocus();
	//  if any window wants to translate this message
	for(; hWnd != NULL; hWnd = ::GetParent(hWnd))
	{
		Control* pWnd = Control::FromHandle(hWnd);
		if(pWnd != NULL)
		{
			return pWnd;
		}
	}
	return NULL;
}
// public
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Set.../Get.../Add...
void Control::AddControl(Control* control, int nIndex)
{
	//need control != _pClient
	if(_pClient != NULL && _pClient != control)
	{
		_pClient->AddControl(control, nIndex);
		return;
	}
	if(_lstControls == NULL)
	{
		//isSort, bIsDESC=false, isDistinct=false, nMinCapacity=0, nIncrement=1.3)
		_lstControls = new List<Control*>(false, false, true);
	}
	int nReal = _lstControls->GetIndex(control);
	if(nReal >= 0)
	{
		if(nReal == nIndex)
		{
			return;
		}
		_lstControls->Remove(control);
	}
	if(nIndex < 0)
	{
		nIndex = _lstControls->GetSize();
	}
	_lstControls->Insert(nIndex, control);
	control->SetParent(this);
	EventArgs ee(0, 0, 0, 0);
	ControlEventArgs eee(&ee);
	eee.control = control;
	OnControlAdded(&eee);
}

//It is the responsibility of the calling application to destroy the Control returned from this function using the delete function.
Control* Control::RemoveControl(int nIndex)
{
	if(_pClient != NULL)
	{
		return _pClient->RemoveControl(nIndex);
	}
	Control* control = GetControl(nIndex);
	if(control != NULL)
	{
		RemoveControl(control);
	}
	return control;
}

void Control::RemoveControl(Control* control)
{
	if(_pClient != NULL)
	{
		_pClient->RemoveControl(control);
		return;
	}
	if(_lstControls == NULL || !_lstControls->ContainsKey(control))
	{
		return;
	}

	//set not visible before remove it
	control->SetVisible(false);
	_lstControls->Remove(control);
	control->SetParent(NULL);
	EventArgs ee(0, 0, 0, 0);
	ControlEventArgs eee(&ee);
	eee.control = control;
	OnControlRemoved(&eee);
}

Control* Control::GetControl(const TCHAR* ctlName) const
{
	CC_ASSERT(ctlName != NULL);

	if(_pClient != NULL)
	{
		return _pClient->GetControl(ctlName);
	}
	if(ctlName != NULL && _lstControls != NULL)
	{
		for(int n = _lstControls->GetSize() - 1; n >= 0; n--)
		{
			Control* ctl = _lstControls->GetKey(n);
			//Perform a lowercase comparison of strings
			if(ctl->GetName() != NULL && _tcsicmp(ctlName, ctl->GetName()) == 0)
			{
				return ctl;
			}
		}
	}
	return NULL;
}

Control* Control::GetControl(int nIndex) const
{
	if(_pClient != NULL)
	{
		return _pClient->GetControl(nIndex);
	}
	if(_lstControls == NULL || nIndex < 0 || nIndex >= _lstControls->GetSize())
	{
		return NULL;
	}
	return _lstControls->GetKey(nIndex);
}

bool Control::ContainsControl(Control* control) const
{
	if(_pClient != NULL)
	{
		return _pClient->ContainsControl(control);
	}
	return (_lstControls == NULL ? false : _lstControls->ContainsKey(control));
}

int Control::GetControlCount() const
{
	if(_pClient != NULL)
	{
		return _pClient->GetControlCount();
	}
	return (_lstControls == NULL ? 0 : _lstControls->GetSize());
}

HFONT Control::GetFont()
{
	if(_hFont == NULL)
	{
		_hFont = CreateDefaultFont();
	}
	return _hFont;
}

//if sName is NULL,then use _T("MS UI Gothic")
void Control::SetFont(int nSize, const TCHAR* sName, bool isBold, bool isItalic, bool isUnderline, bool isStrikeOut, long nEscapement)
{
	//HFONT hFont = GetFont();
	//if(hFont != NULL)
	//{
		LOGFONT lf;
		//memcpy(&lf, &FontConfig::lfControl, sizeof(LOGFONT));
		HFONT hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
		GetObject(hFont, sizeof(LOGFONT), &lf);
		lf.lfOutPrecision = OUT_STROKE_PRECIS;
		if(nEscapement >= 0)
		{
			lf.lfEscapement = nEscapement;
		}
		lf.lfCharSet = DEFAULT_CHARSET;
		lf.lfOrientation = 0;
		lf.lfClipPrecision = CLIP_DEFAULT_PRECIS; //CLIP_LH_ANGLES;
		lf.lfQuality = DEFAULT_QUALITY;
		lf.lfPitchAndFamily = VARIABLE_PITCH|FF_MODERN;
		lf.lfHeight = nSize;
		lf.lfWeight = isBold ? FW_BOLD : FW_NORMAL;
		lf.lfItalic = isItalic;
		lf.lfUnderline = isUnderline;
		lf.lfStrikeOut = isStrikeOut;
		if(sName != NULL)
		{
			cc::Buf::Cpy(lf.lfFaceName, 32, sName);
		}
		//else
		//{
		//	_tcscpy(lf.lfFaceName, _T("MS UI Gothic"));
		//}
		HFONT hFont2 = ::CreateFontIndirect(&lf);
		SetFont(hFont2);
	//}
}

void Control::SetFont(HFONT hFont)
{
	if(hFont != NULL)
	{
		if(_hFont != NULL && _hFont != hFontAppShare && _hFont != hFont)
		{
			::DeleteObject(_hFont);
		}
		_hFont = hFont;
		if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
		{
			SendMessage(m_hWnd, WM_SETFONT, (WPARAM)_hFont, (LPARAM)FALSE);
			//GetText();
			//SetWindowText(m_hWnd, _Text);
		}
	}
}

const TCHAR* Control::GetWndClassName() const
{
	return _ClassName;
}

const TCHAR* Control::GetText()
{
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
	{
		LockAction lock(&m_lockText);

		if(_Text != NULL)
		{
			free(_Text);
			_Text = NULL;
		}
		int nLen = GetWindowTextLength(m_hWnd);
		//nLen = 0, then (int)_tcslen(_Text) = 0
		_Text = (TCHAR*)malloc((nLen + 2) * sizeof(TCHAR));
		memset(_Text, 0, (nLen + 2) * sizeof(TCHAR));
		GetWindowText(m_hWnd, _Text, nLen + 2);
	}
	return _Text;
}

const char* Control::GetTextA()
{
	CC_ASSERT(HasStyle(StyleType::Inner2, Inner2Style::HandleCreated));

	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
	{
		LockAction lock(&m_lockText);

		if(_Text != NULL)
		{
			free(_Text);
			_Text = NULL;
		}
		int nLen = GetWindowTextLengthA(m_hWnd);
		//nLen = 0, then (int)_tcslen(_Text) = 0
		_Text = (TCHAR*)malloc(nLen + 2);
		memset(_Text, 0, nLen + 2);
		GetWindowTextA(m_hWnd, (char*)_Text, nLen + 2);
	}
	return (const char*)_Text;
}

const wchar_t* Control::GetTextW()
{
	CC_ASSERT(HasStyle(StyleType::Inner2, Inner2Style::HandleCreated));

	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
	{
		LockAction lock(&m_lockText);

		if(_Text != NULL)
		{
			free(_Text);
			_Text = NULL;
		}
		int nLen = GetWindowTextLengthW(m_hWnd);
		//nLen = 0, then (int)_tcslen(_Text) = 0
		_Text = (TCHAR*)malloc((nLen + 2) * sizeof(wchar_t));
		memset(_Text, 0, (nLen + 2) * sizeof(wchar_t));
		GetWindowTextW(m_hWnd, (wchar_t*)_Text, nLen + 2);
	}
	return (const wchar_t*)_Text;
}

int Control::GetTextLen()
{
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
	{
		return GetWindowTextLength(m_hWnd);
	}
	if(_Text == NULL)
	{
		return 0;
	}
	return (int)_tcslen(_Text);
}

int Control::GetTextLenA()
{
	CC_ASSERT(HasStyle(StyleType::Inner2, Inner2Style::HandleCreated));

	return GetWindowTextLengthA(m_hWnd);
}

int Control::GetTextLenW()
{
	CC_ASSERT(HasStyle(StyleType::Inner2, Inner2Style::HandleCreated));

	return GetWindowTextLengthW(m_hWnd);
}

void Control::SetText(const TCHAR* sText)
{
	LockAction lock(&m_lockText);

	//ITextIcon::SetText(sText);
	_Text = CC_StrCopy(&_Text, sText);
	//_nUpdateFlag |= UpdateFlag::UpdateText;
	//Update();
	_sizeTextIcon.cx = -1;
	if(_Text == NULL)
	{
		::SetWindowText(m_hWnd, _T(""));
	}
	else
	{
		::SetWindowText(m_hWnd, _Text);
	}
}

void Control::SetText(const cc::Str& sText)
{
	if(sText.GetLangCode() == LangCode::TCHAR)
	{
		Control::SetText((TCHAR*)sText.GetBuf());
	}
	else
	{
		cc::Str s = cc::Str(LangCode::TCHAR, sText);
		Control::SetText((TCHAR*)s.GetBuf());
	}
}

ContextMenu* Control::GetContextMenu() const
{
	return _pContextMenu;
}

void Control::SetContextMenu(ContextMenu* menu)
{
	bool isChg = (_pContextMenu != menu);
	_pContextMenu = menu;
	if(isChg)
	{
		OnContextMenuChanged(NULL);
	}
}

Control* Control::GetParent() const
{
	return _pParent;
}

Control* Control::GetParentReal() const
{
	HWND parent = ::GetParent(m_hWnd);
	if(this->_pParent != NULL && this->_pParent->m_hWnd == parent)
	{
		return _pParent;
	}
	Control* ctl = Control::FromHandle(parent);
	//perhaps like edit in combox
	if(ctl == NULL)
	{
		parent = ::GetParent(parent);
		ctl = Control::FromHandle(parent);
		if(ctl == NULL)
		{
			parent = ::GetParent(parent);
			ctl = Control::FromHandle(parent);
		}
	}
	return ctl;
}

void Control::SetParent(Control* parent, bool notAddToParentList)
{
	if(_pParent != parent)
	{
		//remove from parent
		// add ourself to the parents control
		this->_pParentBrush = NULL;
		if(!notAddToParentList && _pParent != NULL && _pParent->ContainsControl(this))
		{
			_pParent->RemoveControl(this);
		}
		_pParent = parent;

		// add ourself to the parents control
		if(_pParent != NULL && !notAddToParentList && !_pParent->ContainsControl(this))
		{
			_pParent->AddControl(this);
		}
		if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
		{
			//::SetParent(m_hWnd, _pParent == NULL ? NULL : _pParent->m_hWnd);
			::SetParent(m_hWnd, _pParent == NULL ? NULL : (_pParent->_pClient == NULL ? _pParent->m_hWnd : _pParent->_pClient->m_hWnd));
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// Control functions with special function
bool Control::IsStyle(StyleType::Type type, DWORD style)
{
	return ((GetStyle(type) & style) == style);
}

bool Control::HasStyle(StyleType::Type type, DWORD style)
{
	return ((GetStyle(type) & style) != 0);
}

DWORD Control::GetStyle(StyleType::Type type)
{
	CC_ASSERT((int)type >= 0 && (int)type <= CC_CONTROL_STYLETYPE_SIZE);
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
	{
		//should get real status
		if(type == StyleType::Style)
		{
			_StyleArray[(int)type] = (DWORD)GetWindowLong(m_hWnd, GWL_STYLE);
		}
		else if(type == StyleType::ExStyle)
		{
			_StyleArray[(int)type] = (DWORD)GetWindowLong(m_hWnd, GWL_EXSTYLE);
		}
	}
	return _StyleArray[(int)type];
}

void Control::SetStyle(StyleType::Type type, DWORD dwRemove, DWORD dwAdd, UINT nFlags, bool needUpdate)
{
	CC_ASSERT((int)type >= 0 && (int)type <= CC_CONTROL_STYLETYPE_SIZE);
	_StyleArray[(int)type] = GetStyle(type);
	if(dwRemove != 0)
	{
		_StyleArray[(int)type] = (_StyleArray[(int)type] & ~dwRemove);
	}
	_StyleArray[(int)type] |= dwAdd;
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0 && m_hWnd != NULL)
	{
		if(type == StyleType::Style)
		{
			::SetWindowLong(m_hWnd, GWL_STYLE, _StyleArray[(int)type]);
		}
		else if(type == StyleType::ExStyle)
		{
			::SetWindowLong(m_hWnd, GWL_EXSTYLE, _StyleArray[(int)type]);
		}
		else if(type == StyleType::Outer)
		{
			_nUpdateFlag |= UpdateFlag::UpdateOuterStyle;
			if(needUpdate && (_StyleArray[(int)type] & OuterStyle::AutoSize) != 0)
			{
				_nExpectCY = -1;
			}
			if((dwAdd & (OuterStyle::TransparentNoEraseBack | OuterStyle::TransparentParentBrush | OuterStyle::TransparentParentErase | OuterStyle::TransparentHollowBrush)) != 0)
			{
				if(!needUpdate && m_hWnd != NULL)
				{
					::InvalidateRect(m_hWnd, NULL, TRUE);
				}
			}
		}
		else if(type == StyleType::Inner)
		{
			_nUpdateFlag |= UpdateFlag::UpdateInnerStyle;
			if((dwAdd & InnerStyle::BackgroundGradient) != 0 || (dwRemove & InnerStyle::BackgroundGradient) != 0)
			{
				if(HasStyle(StyleType::Inner2, Inner2Style::BackgroundDestroyBmp))
				{
					::DeleteObject(_hbmpBackground);
				}
				_hbmpBackground = NULL;
				InvalidateBackGround();
			}
			if((dwAdd & (InnerStyle::BackgroundColor | InnerStyle::BackgroundBrush | InnerStyle::BackgroundBmp | InnerStyle::BackgroundTheme)) != 0)
			{
				InvalidateBackGround();
			}
			if((dwRemove & InnerStyle::TrackMouse) != 0)
			{
				_StyleArray[(int)StyleType::Inner2] &= ~Inner2Style::IsMouseEnter;
			}
		}
		//do nothing for Inner2,OwnerDrawTextStyle,ClassStyle,InitControlStyle
		if(needUpdate && m_hWnd != NULL)
		{
			Update();
			::InvalidateRect(m_hWnd, NULL, TRUE);
		}
		if(nFlags != 0)
		{
			::SetWindowPos(m_hWnd, NULL, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE|nFlags);
		}
	}
}

void Control::SetStyleBool(StyleType::Type type, DWORD style, bool value, UINT nFlags, bool needUpdate)
{
	CC_ASSERT((int)type >= 0 && (int)type <= CC_CONTROL_STYLETYPE_SIZE);
	if(value)
	{
		SetStyle(type, 0, style, nFlags, needUpdate);
	}
	else
	{
		SetStyle(type, style, 0, nFlags, needUpdate);
	}
}

void Control::SetOuterStyle(OuterStyle::Type outerStyle, bool value, bool needUpdate)
{
	SetStyleBool(StyleType::Outer, outerStyle, value, 0, needUpdate);
}

void Control::SetEnabled(bool isEnabled)
{
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
	{
		::EnableWindow(m_hWnd, isEnabled);
		//do this at OnEnabledChanged
		//_nUpdateFlag |= UpdateFlag::UpdateEnabled;
		//Update();
	}
	else
	{
		//SetStyle(isEnabled ? WS_DISABLED : 0, isEnabled ? 0 : WS_DISABLED);
		SetStyleBool(StyleType::Style, WS_DISABLED, !isEnabled);
	}
}

bool Control::GetEnabled()
{
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
	{
		return (::IsWindowEnabled(m_hWnd) == TRUE);
	}
	else
	{
		return !HasStyle(StyleType::Style, WS_DISABLED);
	}
}

void Control::LockWindowUpdate()
{
	CC_ASSERT(::IsWindow(m_hWnd));

	::LockWindowUpdate(m_hWnd);
}

void Control::UnLockWindowUpdate()
{
	::LockWindowUpdate(NULL);
}

//if this window has WS_VISIBLE then even parent is not show,return true
bool Control::GetVisible()
{
	//for _Visible is updated at OnVisibleChanged,not reget it here
	//if(HasStyle(StyleType::Inner2, Inner2Style::HandleCreated))
	//{
		//If the specified window, its parent window, its parent's parent window,
		//and so forth, have the WS_VISIBLE style, the return value is nonzero.
		//Otherwise, the return value is zero.
		//Because the return value specifies whether the window has the WS_VISIBLE
		//style, it may be nonzero even if the window is totally obscured by other windows. 
		//_Visible = (::IsWindowVisible(m_hWnd) == TRUE);
	//	_Visible = HasStyle(WS_VISIBLE);
	//}
	return HasStyle(StyleType::Inner2, Inner2Style::Visible);
}

void Control::SetVisible(bool value)
{
	//_Visible = value;
	//CC_BIT_SET(_Flags, CC_FLAG_CONTROL_VISIBLE, value);
	SetStyleBool(StyleType::Inner2, Inner2Style::Visible, value);
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
	{
		::ShowWindow(m_hWnd, value ? SW_SHOW : SW_HIDE);
	}
}

//call by CalcuteRect if autosize
SIZE Control::DoMeasureSize(int ctlHeight)
{
	_nExpectCY = ctlHeight;
	return CC_APP_GetDrawActor()->MeasureControl(this, ctlHeight);
}

SIZE Control::BestSize(bool isUpdate)
{
	RECT rect = CalcuteRect(isUpdate, -1, -1, -1, -1, true);
	SIZE size = {rect.right - rect.left, rect.bottom - rect.top};
	return size;
}

RECT Control::CalcuteRect(bool updateRect, int nExpectX, int nExpectY, int nExpectCX, int nExpectCY, bool isFourceCalcute)
{
	RECT rect = GetBounds();
	if(HasStyle(StyleType::Outer, OuterStyle::AutoSize) || isFourceCalcute)
	{
		//2008/12/11 add isFourceCalcute for update toolitem by toolbar
		if(_sizeTextIcon.cx < 0 || _nExpectCY != nExpectCY || isFourceCalcute)
		{
			_sizeTextIcon = DoMeasureSize(nExpectCY);
		}
	}
	else
	{
		_sizeTextIcon.cx = rect.right - rect.left;
		_sizeTextIcon.cy = rect.bottom - rect.top;
	}
	if(_sizeTextIcon.cx < _minSize.cx)
	{
		_sizeTextIcon.cx = _minSize.cx;
	}
	if(_sizeTextIcon.cy < _minSize.cy)
	{
		_sizeTextIcon.cy = _minSize.cy;
	}

//cc::Log::fatal("CalcuteRect; updateRect:%d, nExpectX:%d, nExpectY:%d, nExpectCX:%d, nExpectCY:%d", updateRect, nExpectX, nExpectY, nExpectCX, nExpectCY);
//cc::Log::fatal("CalcuteRect 1; nExpectX:%d, nExpectY:%d, nExpectCX:%d, nExpectCY:%d", rect.left, rect.top, nW, nH);
	DoAdjustRect(rect.left, rect.top, (int&)_sizeTextIcon.cx, (int&)_sizeTextIcon.cy, nExpectX, nExpectY, nExpectCX, nExpectCY);
//cc::Log::fatal("CalcuteRect 2; nExpectX:%d, nExpectY:%d, nExpectCX:%d, nExpectCY:%d", rect.left, rect.top, nW, nH);

	if(updateRect)
	{
		SetBoundsNotOriginal(rect.left, rect.top, _sizeTextIcon.cx, _sizeTextIcon.cy, BoundsSpecified::All);
	}
	rect.right = rect.left + _sizeTextIcon.cx;
	rect.bottom = rect.top + _sizeTextIcon.cy;
	return rect;
}

void Control::DoAdjustRect(long& nRealX, long& nRealY, int& nRealCX, int& nRealCY, int nExpectX, int nExpectY, int nExpectCX, int nExpectCY)
{
	//nExpectCX,nExpectCY is not used here but some sub class for special used
	if(nExpectX != -1)
	{
		nRealX = nExpectX;
	}
	if(nExpectY != -1)
	{
		nRealY = nExpectY + 1;
	}
}
// Set.../Get.../Add...
///////////////////////////////////////////////////////////////////////////////


//#region xxx
///////////////////////////////////////////////////////////////////////////////
// On...
void Control::OnBackColorChanged(EventArgs* e)
{
	BackColorChanged.Invoke((Object*)this, e);
//
//	if(_lstControls != NULL)
//	{
//		for(int n = _lstControls->GetSize() - 1; n >= 0; n--)
//		{
//			Control* ctl = _lstControls->GetKey(n);
//			ctl->OnParentBackColorChanged(e);
//		}
//	}
}
//
//void Control::OnBackgroundImageChanged(EventArgs* e)
//{
//	BackgroundImageChanged.Invoke((Object*)this, e);
//}
//
//void Control::OnBindingContextChanged(EventArgs* e)
//{
//	BindingContextChanged.Invoke((Object*)this, e);
//}
//
//void Control::OnCausesValidationChanged(EventArgs* e)
//{
//	CausesValidationChanged.Invoke((Object*)this, e);
//}
//
//void Control::OnChangeUICues(UICuesEventArgs* e)
//{
//	ChangeUICues.Invoke((Object*)this, e);
//}

void Control::OnClick(EventArgs* e)
{
	Click.Invoke((Object*)this, e);
}

void Control::PreOnCommand(EventArgs* e)
{
//Message Source	wParam (high word)					wParam (low word)				lParam
//Menu				0									Menu identifier (IDM_*)			0
//Accelerator		1									Accelerator identifier (IDM_*)	0
//Control			Control-defined notification code	Control identifier				Handle to the control window
	//if is menu,treate menu click
	if(e->LParam == 0)
	{
		if(HIWORD(e->WParam) == 0)
		{
			Menu_OnClick(e);
		}
		else
		{
#ifdef _DEBUG_
			if(CC_APP_GetConfig()->GetIsDebug())
			{
				CC_TRACE((_T("Control::PreOnCommand(), ClassName:%s, Name:%s, Accelerator identifier:%d"), _ClassName, _Name, LOWORD(e->WParam)));
			}
#endif
		}
		return;
	}
	else
	{
		HWND hWnd = (HWND)e->LParam;
		Control* ctl = FromHandle(hWnd);
		if(ctl != NULL)
		{
#ifdef _DEBUG_
			UINT nID = LOWORD(e->WParam);
			int nCode = HIWORD(e->WParam);
			if(CC_APP_GetConfig()->GetIsDebug())
			{
				CC_TRACE((_T("Control::PreOnCommand(), ClassName:%s, Name:%s, nID:%d, nCode:%d"), _ClassName, _Name, nID, nCode));
			}
#endif
			//sometime delete self while click button(OnCommand),set Handled to not call DefWndProc
			e->Result = DefWndProc(e);
			e->Handled = true;
			ctl->OnCommand(e);
		}
	}
}

void Control::OnCommand(EventArgs* e)
{
	//do nothing
}

void Control::Menu_OnEnterLoop(EventArgs* e)
{
	//always clear tips when start menu,also call by contextmenu
	HWND pMainWnd = CC_APP_GetActiveFormHwnd();
	::SendMessage(pMainWnd, CC_WM_MAINMSG, CC_WM_MAINMSG_WPARAM_CLEARSTATUS, (LPARAM)NULL);
}

void Control::Menu_OnExitLoop(EventArgs* e)
{
	//for OnCommand is after OnExitLoop,so cannot set it NULL!
	//_pActivePopunMenu = NULL;
	//always clear tips when exit menu,also call by contextmenu
	HWND pMainWnd = CC_APP_GetActiveFormHwnd();
	::SendMessage(pMainWnd, CC_WM_MAINMSG, CC_WM_MAINMSG_WPARAM_CLEARSTATUS, (LPARAM)NULL);
}

void Control::Menu_OnClick(EventArgs* e)
{
	if(_pActivePopunMenu != NULL)
	{
		UINT nID = LOWORD(e->WParam);
		MenuItem* item = (MenuItem*)_pActivePopunMenu->ItemGet(nID);
		if(item != NULL)
		{
//err(_T("Menu_OnClick, nID:%d, item text:%s"), nID, item->GetText());
			item->PerformClick(e);
		}
		e->Handled = true;

		//menu is end
		_pActivePopunMenu = NULL;
	}
	//for click is after Menu_OnExitLoop,need clear tips here
	HWND pMainWnd = CC_APP_GetActiveFormHwnd();
	::SendMessage(pMainWnd, CC_WM_MAINMSG, CC_WM_MAINMSG_WPARAM_CLEARSTATUS, (LPARAM)NULL);
}

/////////////////////////////////////////////////////////////////////////////
// CWnd will delegate owner draw messages to self drawing controls

// Measure item implementation relies on unique control/menu IDs
void Control::PreMeasureItem(MeasureItemEventArgs *e)
{
	if(e->data->CtlType == ODT_MENU)
	{
		if(_pActivePopunMenu != NULL)
		{
			//the MeasureItem is not show also,so is _pActivePopunMenu's sub menuitem
			MenuItem* item = (MenuItem*)_pActivePopunMenu->ItemGet(e->data->itemID);
			if(item == NULL)
			{
				item = (MenuItem*)Menu::FromHandle((HMENU)(UINT_PTR)e->data->itemID);
			}
			if(item != NULL)
			{
				e->hDC = ::GetDC(m_hWnd);
				item->PerformMeasureItem(e);
				e->baseArgs->Result = (LRESULT)TRUE;
				e->baseArgs->Handled = true;
				::ReleaseDC(m_hWnd, e->hDC);
				return; // eat it
			}
		}
	}
#ifdef _DEBUG_
	else
	{
			UINT nID = LOWORD(e->baseArgs->WParam);
			int nCode = HIWORD(e->baseArgs->WParam);
			if(CC_APP_GetConfig()->GetIsDebug())
			{
				CC_TRACE((_T("Control::PreMeasureItem(), ClassName:%s, Name:%s, Text:%s, e->data->itemID:%d"), _ClassName, _Name, _Text, e->data->itemID));
			}
	}
#endif
}

// Drawing: for all 4 control types
void Control::PreDrawItem(DrawItemEventArgs *e)
{
	if(e->data->CtlType == ODT_MENU)
	{
		HMENU hMenu = (HMENU)e->data->hwndItem;
		Menu* menu = Menu::FromHandle(hMenu);
		if(menu != NULL)
		{
			MenuItem* item = (MenuItem*)menu->ItemGet(e->data->itemID);
			if(item == NULL)
			{
				item = (MenuItem*)Menu::FromHandle((HMENU)(UINT_PTR)e->data->itemID);
			}
			if(item != NULL)
			{
				item->PerformDrawItem(e);
				e->baseArgs->Handled = true;
				return; // eat it
			}
		}
	}
	else
	{
		// reflect notification to child window control
		ReflectLastMsg(e->data->hwndItem, e->baseArgs);
	}
}

void Control::OnMeasureItem(MeasureItemEventArgs *e)
{
	if(!MeasureItem.IsEmpty())
	{
		e->hDC = ::GetDC(m_hWnd);
		MeasureItem.Invoke((Object*)this, e);
		::ReleaseDC(m_hWnd, e->hDC);
	}
}

void Control::OnDrawItem(DrawItemEventArgs *e)
{
	DrawItem.Invoke((Object*)this, e);
}

// only for list controls
void Control::OnCompareItem(CompareItemEventArgs *e)
{
	// reflect notification to child window control
	//ReflectLastMsg(e->data->hwndItem, e->baseArgs);
	CompareItem.Invoke((Object*)this, e);
}

void Control::OnDeleteItem(EventArgs *e)
{
	//ReflectLastMsg(e->data->hwndItem, e->baseArgs);
	DeleteItem.Invoke((Object*)this, e);
}

void Control::OnVKeyToItem(VKeyToItemEventArgs *e)
{
	//ReflectLastMsg((HWND)e->baseArgs->LParam, e->baseArgs);
	VKeyToItem.Invoke((Object*)this, e);
}

void Control::OnCharToItem(CharToItemEventArgs *e)
{
	//ReflectLastMsg((HWND)e->baseArgs->LParam, e->baseArgs);
	CharToItem.Invoke((Object*)this, e);
}

void Control::Menu_OnPopup(EventArgs* e)
{
	HMENU hMenu = (HMENU)e->WParam;
	Menu* menu = Menu::FromHandle(hMenu);
	_pActivePopunMenu = menu;
	if(menu != NULL)
	{
		if(menu->GetMenuType() == MenuType::MENUITEM)
		{
			MenuItem* item = (MenuItem*)menu;
			item->PerformPopup(e);
			//invoid tips is under menu.
			HWND pMainWnd = CC_APP_GetActiveFormHwnd();
			::SendMessage(pMainWnd, CC_WM_MAINMSG, CC_WM_MAINMSG_WPARAM_MENUSTATUS, (LPARAM)item);
		}
	}
}

void Control::Menu_OnSelect(EventArgs* e)
{
	if(e->LParam != NULL)
	{
		UINT nItemID = LOWORD(e->WParam);
		//UINT nFlags = HIWORD(e->WParam);
		HMENU hMenu = (HMENU)e->LParam;

		Menu* menu = Menu::FromHandle(hMenu);
		if(menu != NULL)
		{
			_pActivePopunMenu = menu;

			//if not mainmenu,point _pActivePopunMenu to popuped menu
			MenuItem* item = (MenuItem*)menu->ItemGet(nItemID);
			if(item != NULL)
			{
				item->PerformSelect(e);
				//always show status&tips,even null.
				HWND pMainWnd = CC_APP_GetActiveFormHwnd();
				::SendMessage(pMainWnd, CC_WM_MAINMSG, CC_WM_MAINMSG_WPARAM_MENUSTATUS, (LPARAM)item);
			}
		}
	}
}

void Control::OnContextMenu(MouseEventArgs* e)
{
	if(_pContextMenu != NULL && ::GetCapture() == NULL)
	{
		POINT pt = {e->X, e->Y};
		_pContextMenu->Show(this, pt);
		//eat it
		e->baseArgs->Handled = true;
	}
	else if(!HasStyle(StyleType::Inner, InnerStyle::TranceContextMenuUp))
	{
		//eat it
		e->baseArgs->Handled = true;
	}
}

void Control::OnContextMenuChanged(EventArgs* e)
{
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
	{
		_nUpdateFlag |= UpdateFlag::UpdateContextMenu;
		Update();
	}
	ContextMenuChanged.Invoke((Object*)this, e);
}

void Control::OnControlAdded(ControlEventArgs* e)
{
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
	{
		if((e->control->_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) == 0)
		{
			e->control->CreateControl();
		}
		//need resize
		OnResize(e->baseArgs);
	}

	ControlAdded.Invoke((Object*)this, e);
}

void Control::OnControlRemoved(ControlEventArgs* e)
{
	ControlRemoved.Invoke((Object*)this, e);
}

void Control::OnCreateControl()
{
	//comment out at 2007/07/26
	//if(_pClient != NULL)
	//{
	//	::CopyRect(&_pClient->_OrgRect, &_OrgRect);
	//}
	if(_lstControls != NULL)
	{
		//changed for add releate position fun at 2008/07/26
		//for(int n = _lstControls->GetSize() - 1; n >= 0; n--)
		for(int n = 0; n < _lstControls->GetSize(); n++)
		{
			Control* ctl = _lstControls->GetKey(n);
			ctl->CreateControl();
			if(ctl->HasStyle(StyleType::Inner2, Inner2Style::Visible))
			{
				ctl->Show();
			}
		}
	}
}

void Control::OnSetCursor(EventArgs* e)
{
#if defined(_DEBUG_) || defined(_DESIGN)
	//code for design
	//show IDC_SIZEALL while on controls
	if(CC_APP_GetDesignBarActive())
	{
		bool isKey = ((GetKeyState(VK_CONTROL) & 0x8000) != 0); // && (GetKeyState(VK_SHIFT) & 0x8000) != 0
		if((CC_APP_GetConfig()->GetIsDesign() && !isKey) || (!CC_APP_GetConfig()->GetIsDesign() && isKey))
		{
			HWND hWnd = CC_APP_GetDesignBarHwnd();
			if(hWnd == m_hWnd || ::IsChild(hWnd, m_hWnd))
			{
				return;
			}
			Control* ctl = _pParent;
			while(hWnd != NULL && ctl != NULL)
			{
				if(hWnd == ctl->m_hWnd || ::IsChild(hWnd, ctl->m_hWnd))
				{
					return;
				}
				ctl = ctl->_pParent;
			}
			int nHitTest = (int)(short)LOWORD(e->LParam);
			if(nHitTest == HTCAPTION || nHitTest == HTSYSMENU || nHitTest == HTMENU || nHitTest == HTCLIENT)
			{
				e->Handled = true;
				::SetCursor(::LoadCursor(NULL, IDC_SIZEALL));
				return;
			}
		}
		return;
	}
#endif
	if(_hCursor != NULL)
	{
		::SetCursor(_hCursor);
		e->Handled = true;
		e->Result = 0;
	}
	//SetCursor.Invoke((Object*)this, e);
}

void Control::OnDockChanged(EventArgs* e)
{
	DockChanged.Invoke((Object*)this, e);
}

void Control::OnDoubleClick(EventArgs* e)
{
	DoubleClick.Invoke((Object*)this, e);
}
//
//void Control::OnDragEnter(DragEventArgs* e)
//{
//	DragEnter.Invoke((Object*)this, e);
//}
//
//void Control::OnDragLeave(EventArgs* e)
//{
//	DragLeave.Invoke((Object*)this, e);
//}
//
//void Control::OnDragOver(DragEventArgs* e)
//{
//	DragOver.Invoke((Object*)this, e);
//}
//
void Control::OnDragMove(MouseEventArgs* e)
{
	DragMove.Invoke((Object*)this, e);
}

void Control::OnDragEnd(DragEndEventArgs* e)
{
	SetStyle(StyleType::Inner, InnerStyle::Dragging, 0);
	DragEnd.Invoke((Object*)this, e);
}

void Control::OnGiveFeedback(GiveFeedbackEventArgs* e)
{
	if(HasStyle(StyleType::Inner, InnerStyle::Dragging))
	{
		if(e->baseArgs->Msg == WM_KEYDOWN && (TCHAR)e->baseArgs->WParam == VK_ESCAPE)
		{
			e->Cancel = true;
		}
		GiveFeedback.Invoke((Object*)this, e);
		if(e->Cancel)
		{
			DragEndEventArgs ee(e->baseArgs);
			ee.Cancel = true;
			OnDragEnd(&ee);
		}
	}
}

void Control::SetBackgroundBrush(HBRUSH hbrBackground, bool needDestroy)
{
	if(_hbrBackground != NULL && _hbrBackground != hbrBackground
		&& HasStyle(StyleType::Inner2, Inner2Style::BackgroundDestroyBrush))
	{
		::DeleteObject(_hbrBackground);
		_hbrBackground = NULL;
	}
	_hbrBackground = hbrBackground;
	SetStyleBool(StyleType::Inner2, Inner2Style::BackgroundDestroyBrush, needDestroy);
	if(_hbrBackground != NULL)
	{
		//also do GetBackgroundBrush and InvalidateRect and update
		_StyleArray[(int)StyleType::Inner] &= ~(InnerStyle::BackgroundColor|InnerStyle::BackgroundBmp|InnerStyle::BackgroundTheme|InnerStyle::BackgroundGradient);
		_StyleArray[(int)StyleType::Inner] |= InnerStyle::BackgroundBrush;
	}
	else
	{
		//also do GetBackgroundBrush and InvalidateRect and update
		_StyleArray[(int)StyleType::Inner] &= ~(InnerStyle::BackgroundBrush|InnerStyle::BackgroundBmp|InnerStyle::BackgroundTheme|InnerStyle::BackgroundGradient);
		_StyleArray[(int)StyleType::Inner] |= InnerStyle::BackgroundColor;
	}
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0 && m_hWnd != NULL)
	{
		GetBackgroundBrush(true);
		::InvalidateRect(m_hWnd, NULL, TRUE);
	}
}

////HS_DIAGCROSS,HS_CROSS,
//_hbrBackground = ::CreateHatchBrush(HS_CROSS, _corBackground);
HBRUSH Control::GetBackgroundBrush(bool isFource)
{
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) == 0)
	{
		return _hbrBackground;
	}

	//if has this,then not see any Background???.
	if(HasStyle(StyleType::Inner, InnerStyle::BackgroundGradient))
	{
		if(_hbmpBackground == NULL)
		{
			_hbmpBackground = CC_APP_GetDrawActor()->GetGradientBmp(_corHotBackColor, _corBackground, -1, -1, ((_StyleArray[(int)StyleType::Inner2] & Inner2Style::GradientVertical) != 0));
			SetStyle(StyleType::Inner2, 0, Inner2Style::BackgroundDestroyBmp);
			isFource = true;
		}
	}
	//if brush is set by out, use it only
	if(_hbrBackground != NULL && HasStyle(StyleType::Inner, InnerStyle::BackgroundBrush))
	{
		return _hbrBackground;
	}
	//for perhaps resize,so do everytime while InnerStyle::BackgroundTheme
	HBRUSH hbrTheme = NULL;
	if(HasStyle(StyleType::Inner, InnerStyle::BackgroundTheme))
	{
		hbrTheme = CC_APP_GetDrawActor()->ThemeBackgroundBrush(this);
	}
	if(_hbrBackground == NULL || isFource || hbrTheme != NULL)
	{
		if(isFource || hbrTheme != NULL)
		{
			if(_hbrBackground != NULL && HasStyle(StyleType::Inner2, Inner2Style::BackgroundDestroyBrush))
			{
				::DeleteObject(_hbrBackground);
			}
			_hbrBackground = NULL;
		}

		if(hbrTheme != NULL)
		{
			_hbrBackground = hbrTheme;
			SetStyle(StyleType::Inner2, Inner2Style::BackgroundDestroyBrush, 0);
		}
		else if(_hbmpBackground != NULL && HasStyle(StyleType::Inner, InnerStyle::BackgroundBmp|InnerStyle::BackgroundGradient))
		{
			RECT rect;
			::GetClientRect(m_hWnd, &rect);
			HBITMAP hbmp = CC_APP_GetDrawActor()->ResizeBmp(_hbmpBackground, rect.right, rect.bottom, true);
			_hbrBackground = ::CreatePatternBrush(hbmp);
			DeleteObject(hbmp);
			SetStyle(StyleType::Inner2, 0, Inner2Style::BackgroundDestroyBrush);
		}

		if(_hbrBackground == NULL)
		{
			if(_nBackgroundHatch > 0 && _nBackgroundHatch <= 6)
			{
				//0:none, 1:-, 2:|, 3:/, 4:\, 5:#, 6:x
				_hbrBackground = ::CreateHatchBrush(_nBackgroundHatch - 1, _corBackground);
			}
			else
			{
				_hbrBackground = ::CreateSolidBrush(_corBackground);
			}
			SetStyle(StyleType::Inner2, 0, Inner2Style::BackgroundDestroyBrush);
		}
	}
	return _hbrBackground;
}

//for use BeginPaint,then no system draw,so let user do it
//PAINTSTRUCT ps;
//HDC hDC = ::BeginPaint(m_hWnd, &ps);
//...
//::EndPaint(m_hWnd, &ps);
void Control::OnPaint(PaintEventArgs* e)
{
	if(!PaintBegin.IsEmpty() || !PaintEnd.IsEmpty())
	{
		bool newHdc = false;
		if(e->data.hdc == NULL)
		{
			::BeginPaint(m_hWnd, &(e->data));
			newHdc = true;
		}
		//notice system not do this again
		e->baseArgs->Result = 0;
		e->baseArgs->Handled = true;
		PaintBegin.Invoke((Object*)this, e);
		PaintEnd.Invoke((Object*)this, e);
		if(newHdc)
		{
			::EndPaint(m_hWnd, &(e->data));
			memset(&(e->data), 0, sizeof(PAINTSTRUCT));
		}
	}
}

//draw custom backgrand like:
//	RECT rect;
//	GetClientRect(this->m_hWnd, &rect);
//	::SetBkColor((HDC)e->baseArgs->WParam, _corBackground);
//	::ExtTextOut((HDC)e->baseArgs->WParam, 0, 0, ETO_OPAQUE, &rect, NULL, 0, 0);
//	e->baseArgs->Handled = true;
//	e->baseArgs->Result = 1;
void Control::OnPaintBackground(PaintEventArgs* e)
{
	if(!GetVisible())
	{
		return;
	}
	RECT rect;
	bool isBrushOrgEx = false;
	HBRUSH brush = NULL;
	POINT pt = {0, 0};
	HDC hDC = (HDC)e->baseArgs->WParam;
	//if parent has NotEraseBackground,perhaps there not draw focusframe right.
	if(HasStyle(StyleType::Outer, OuterStyle::TransparentTop))
	{
		_hbrBackground = GetBackgroundBrush();
		brush = _hbrBackground;
		e->baseArgs->Handled = true;
		e->baseArgs->Result = TRUE;
	}

	else if(HasStyle(StyleType::Outer, OuterStyle::TransparentNoEraseBack))
	{
		//not erase back
		e->baseArgs->Handled = true;
		e->baseArgs->Result = TRUE;
	}

	else if(HasStyle(StyleType::Outer, OuterStyle::TransparentParentErase|OuterStyle::TransparentHollowBrush|OuterStyle::TransparentParentBrush))
	{
		//notice that use of the first parent without TransparentParentErase and TransparentParentBrush
		if(_pParentBrush == NULL || !IsChild(_pParentBrush->m_hWnd, m_hWnd))
		{
			_pParentBrush = this->GetParentReal();
			while(_pParentBrush != NULL && !_pParentBrush->HasStyle(StyleType::Outer, OuterStyle::TransparentTop) && _pParentBrush->HasStyle(StyleType::Outer, OuterStyle::TransparentParentErase|OuterStyle::TransparentParentBrush))
			{
				_pParentBrush = _pParentBrush->GetParentReal();
			}
		}
		if(_pParentBrush != NULL)
		{
			::GetClientRect(this->m_hWnd, &rect);
			HRGN hRgn = CreateRectRgn(rect.left, rect.top, rect.right, rect.bottom);
			ExtSelectClipRgn(hDC, hRgn, RGN_AND); //RGN_COPY
			DeleteObject(hRgn);
			::GetWindowRect(this->m_hWnd, &rect);
			::MapWindowPoints(NULL, _pParentBrush->m_hWnd, (LPPOINT)(&rect), 2);
			if(_pParentBrush->HasStyle(StyleType::Outer, OuterStyle::TransparentTop))
			{
				brush = _pParentBrush->GetBackgroundBrush();
				if(this->HasStyle(StyleType::ExStyle, WS_EX_CLIENTEDGE))
				{
					rect.left += 2;
					rect.top += 2;
				}
				::SetBrushOrgEx(hDC, -rect.left, -rect.top, &pt);
				isBrushOrgEx = true;
			}
			else
			{
				//let parent to EraseBackground
				OffsetWindowOrgEx(hDC, rect.left, rect.top, &pt);
				::SendMessage(_pParentBrush->m_hWnd, WM_ERASEBKGND, (WPARAM)hDC, (LPARAM)this);
				SetWindowOrgEx(hDC, pt.x, pt.y, NULL);
			}
			e->baseArgs->Handled = true;
			e->baseArgs->Result = TRUE;
		}
	}

	if(brush != NULL)
	{
		int nControlCount = 0;
		//remove childrens area from hdc
		if(_lstControls != NULL && (nControlCount = _lstControls->GetSize()) > 0)
		{
			for(int n = nControlCount - 1; n >= 0; n--)
			{
				Control* ctl = _lstControls->GetKey(n);
				if(ctl->GetVisible()
					&& ((ctl->_StyleArray[(int)StyleType::Style] & WS_GROUP) != 0 || ctl->GetWndClassName() != WndClassName::Button))
				//if(ctl->GetVisible() && !ctl->HasStyle(StyleType::Outer, OuterStyle::TransparentHollowBrush))
				{
					::GetWindowRect(ctl->m_hWnd, &rect);
					::MapWindowPoints(NULL, m_hWnd, (LPPOINT)(&rect), 2);
					//err(_T("OnPaintBackground, Class:%s, Name:%s, Text:%s, rect.left:%d, rect.top:%d, rect.right:%d, rect.bottom:%d"), ctl->GetWndClassName(), ctl->GetName(), ctl->GetText(), rect.left, rect.top, rect.right, rect.bottom);
					HRGN hRgn = CreateRectRgn(rect.left, rect.top, rect.right, rect.bottom);
					//Only a copy of the region identified by the hrgn parameter is used.
					//The region itself can be reused after this call or it can be deleted.
					ExtSelectClipRgn(hDC, hRgn, RGN_DIFF);
					DeleteObject(hRgn);
				}
			}
		}

		//Paint the area.
		::GetClipBox(hDC, &rect);
		::FillRect(hDC, &rect, brush);
		if(isBrushOrgEx)
		{
			::SetBrushOrgEx(hDC, pt.x, pt.y, NULL);
		}
		SelectClipRgn(hDC , NULL);
	}
	PaintBackground.Invoke((Object*)this, e);
}

//UINT nCtlType = e->Msg - WM_CTLCOLORMSGBOX;
//if(nCtlType == CTLCOLOR_BTN || nCtlType == CTLCOLOR_STATIC)
void Control::OnCtlColorEdit(EventArgs* e)
{
	if(!GetVisible())
	{
		return;
	}
	HDC hDC = (HDC)e->WParam;
	Control* ctl = FromHandle((HWND)e->LParam);
	if(ctl != NULL)
	{
		e->Handled = true;
		::SetTextColor(hDC, ctl->GetIsMouseOn() ? ctl->_corHotTextColor : ctl->_corTextground);
		if(ctl->HasStyle(StyleType::Outer, OuterStyle::TransparentParentBrush|OuterStyle::TransparentParentErase))
		{
			if(_pParentBrush == NULL || !IsChild(_pParentBrush->m_hWnd, m_hWnd))
			{
				_pParentBrush = ctl->GetParentReal();
				while(_pParentBrush != NULL && !_pParentBrush->HasStyle(StyleType::Outer, OuterStyle::TransparentTop) && _pParentBrush->HasStyle(StyleType::Outer, OuterStyle::TransparentParentErase|OuterStyle::TransparentParentBrush))
				{
					_pParentBrush = _pParentBrush->GetParentReal();
				}
			}
			if(_pParentBrush != NULL)
			{
				RECT rect;
				::SetBkMode(hDC, TRANSPARENT);
				::GetWindowRect(ctl->m_hWnd, &rect);
				::MapWindowPoints(NULL, _pParentBrush->m_hWnd, (LPPOINT)(&rect), 2);
				if(ctl->HasStyle(StyleType::ExStyle, WS_EX_CLIENTEDGE))
				{
					rect.left += 2;
					rect.top += 2;
				}
				::SetBrushOrgEx(hDC, -rect.left, -rect.top, NULL);
				e->Result = (LRESULT)_pParentBrush->GetBackgroundBrush();
				return;
			}
		}

		else if(ctl->HasStyle(StyleType::Outer, OuterStyle::TransparentHollowBrush))
		{
			::SetBkMode(hDC, TRANSPARENT);
			e->Result = (LRESULT)(HBRUSH)::GetStockObject(HOLLOW_BRUSH);
			return;
		}

		//not above, set back color
		::SetBkColor(hDC, ctl->_corBackground);
		e->Result = (LRESULT)ctl->GetBackgroundBrush();
		//SetDCBrushColor(hDC, RGB(255,0,0));
		//return GetStockBrush(DC_BRUSH);
		return;
	}
}

//The WM_ENABLE message is sent when an application changes the enabled state of a window.
//It is sent to the window whose enabled state is changing. This message is sent before the EnableWindow function returns,
//but after the enabled state (WS_DISABLED style bit) of the window has changed. 
//wParam:Specifies whether the window has been enabled or disabled.
//	This parameter is TRUE if the window has been enabled or FALSE if the window has been disabled.
//lParam:This parameter is not used.
void Control::OnEnabledChanged(EventArgs* e)
{
	_nUpdateFlag |= UpdateFlag::UpdateEnabled;
	Update();

	EnabledChanged.Invoke((Object*)this, e);
}

void Control::OnEnter(EventArgs* e)
{
	Enter.Invoke((Object*)this, e);
}

void Control::OnFontChanged(EventArgs* e)
{
	//to end msg loop
	e->Result = DefWndProc(e);
	e->Handled = true;
	_nUpdateFlag |= UpdateFlag::UpdateFont;
	Update();
	FontChanged.Invoke((Object*)this, e);
}

void Control::OnForeColorChanged(EventArgs* e)
{
	ForeColorChanged.Invoke((Object*)this, e);
}
//
//void Control::OnGiveFeedback(GiveFeedbackEventArgs* e)
//{
//	GiveFeedback.Invoke((Object*)this, e);
//}

void Control::OnGotFocus(EventArgs* e)
{
	if(HasStyle(StyleType::Style, WS_TABSTOP))
	{
		HWND pMainWnd = CC_APP_GetActiveFormHwnd();
		::SendMessage(pMainWnd, CC_WM_MAINMSG, CC_WM_MAINMSG_WPARAM_FOCUSCHANGED, (LPARAM)this);
	}
	GotFocus.Invoke((Object*)this, e);
}

void Control::OnHandleCreated(EventArgs* e)
{
#if _DEBUG_
	CC_TRACE((_T("Start, this:0x%08x, ClassName:%s, Name:%s, hWnd:0x%08x, Controls:%d"), this, _ClassName, _Name, m_hWnd, _lstControls == NULL ? 0 : _lstControls->GetSize()));
#endif
	//CC_BIT_ON(_Flags, CC_FLAG_CONTROL_HANDLECREATED);
	//SetStyle(StyleType::Inner2, 0, Inner2Style::HandleCreated);
	_StyleArray[(int)StyleType::Inner2] |= Inner2Style::HandleCreated;
	//_bIsHandleCreated = true;
	//to end msg loop
	e->Result = DefWndProc(e);
	e->Handled = true;
	HandleCreated.Invoke((Object*)this, e);

	//20081106, move here for need create child before WM_SETFONT
	OnCreateControl();
	//_bIsHandleCreatedEnd = true;
	//CC_BIT_ON(_Flags, CC_FLAG_CONTROL_HANDLECREATEDEND);
	//SetStyle(StyleType::Inner2, 0, Inner2Style::HandleCreatedEnd);
	_StyleArray[(int)StyleType::Inner2] |= Inner2Style::HandleCreatedEnd;

	if(_hFont == NULL)
	{
		_hFont = CreateDefaultFont();
	}
	if(_hFont != NULL)
	{
		SendMessage(m_hWnd, WM_SETFONT, (WPARAM)_hFont, (LPARAM)FALSE);
		//SetWindowText(m_hWnd, _Text);
	}

	//this is need
	if(!GetEnabled())
	{
		this->SetEnabled(false);
	}

	//to do something special after created child
	_nUpdateFlag |= UpdateFlag::UpdateFlag;
#ifdef _DEBUG_
	if(CC_APP_GetConfig()->GetIsDebug())
	{
		CC_TRACE((_T("call Update with UpdateFlag:%d, ClassName:%s, Name:%s"), _nUpdateFlag, _ClassName, _Name));
	}
#endif
	Update();
	//SetStyle(StyleType::Inner, InnerStyle::HandleCreatedEnd, true);
	CC_TRACE((_T("End, this:0x%08x, ClassName:%s, Name:%s, hWnd:0x%08x"), this, _ClassName, _Name, m_hWnd));
}

void Control::OnHandleDestroyed(EventArgs* e)
{
	SetStyle(StyleType::Inner2, 0, Inner2Style::HandleDestroyed);
	HandleDestroyed.Invoke((Object*)this, e);
}

void Control::OnHelpRequested(HelpEventArgs* e)
{
	HelpRequested.Invoke((Object*)this, e);
}

void Control::OnImeModeChanged(EventArgs* e)
{
	ImeModeChanged.Invoke((Object*)this, e);
}
//
//void Control::OnInvalidated(InvalidateEventArgs* e)
//{
//	Invalidated.Invoke((Object*)this, e);
//}

void Control::OnKeyDown(KeyEventArgs* e)
{
	KeyDown.Invoke((Object*)this, e);
}

void Control::OnKeyPress(KeyPressEventArgs* e)
{
	KeyPress.Invoke((Object*)this, e);
}

void Control::OnKeyUp(KeyEventArgs* e)
{
	KeyUp.Invoke((Object*)this, e);
}

void Control::OnLayout(LayoutEventArgs* e)
{
	//DoDockAndAnchorLayout(e);
	Layout.Invoke((Object*)this, e);
}

void Control::OnLeave(EventArgs* e)
{
	Leave.Invoke((Object*)this, e);
}

void Control::OnLocationChanged(EventArgs* e)
{
#if defined(_DEBUG_) || defined(_DESIGN)
	HWND hWnd = CC_APP_GetDesignBarHwnd();
	if(hWnd != NULL && ((DWORD)this->Design & DesignStyle::Selected) != 0 && !::IsChild(hWnd, m_hWnd))
	{
		::SendMessage(hWnd, CC_WM_MAINMSG, CC_WM_MAINMSG_WPARAM_DS_LOCCHANGED, (LPARAM)this);
	}
#endif
	LocationChanged.Invoke((Object*)this, e);
}

void Control::OnLostFocus(EventArgs* e)
{
	LostFocus.Invoke((Object*)this, e);
}

void Control::SetMouseDownMoveForm(Control* form, DWORD moveCursor)
{
	_moveForm = form;
	_moveFormCursor = moveCursor;
}

void Control::OnMouseDown(MouseEventArgs* e)
{
	MouseDown.Invoke((Object*)this, e);
	if(e->Button == MouseButtons::Left && _moveForm != NULL && _moveForm->m_hWnd != NULL)
	{
		if(_moveFormCursor != -1)
		{
			::SetCursor(::LoadCursor(NULL, IDC_SIZEALL));
		}
		::ReleaseCapture();
		::SendMessage(_moveForm->m_hWnd, WM_NCLBUTTONDOWN, HTCAPTION, NULL);
	}
}

void Control::OnMouseEnter(EventArgs* e)
{
	//show status&tips
	if(this->GetTipsExt() != NULL)
	{
		HWND pMainWnd = CC_APP_GetActiveFormHwnd();
		::SendMessage(pMainWnd, CC_WM_MAINMSG, CC_WM_MAINMSG_WPARAM_CTLSTATUS, (LPARAM)this);
	}
	MouseEnter.Invoke((Object*)this, e);
}

void Control::OnMouseHover(EventArgs* e)
{
	MouseHover.Invoke((Object*)this, e);
}

void Control::OnMouseLeave(EventArgs* e)
{
	//clear status&tips
	if(this->GetTipsExt() != NULL)
	{
		HWND pMainWnd = CC_APP_GetActiveFormHwnd();
		::SendMessage(pMainWnd, CC_WM_MAINMSG, CC_WM_MAINMSG_WPARAM_CLEARSTATUS, (LPARAM)this);
	}
	MouseLeave.Invoke((Object*)this, e);
}

//OnInnerTimer is not come here
//sample
//::SetTimer(m_hWnd, TIMERID, TIMEOUT, NULL);
//::KillTimer(m_hWnd, TIMERID);
void Control::OnTimer(EventArgs* e)
{
	//e->WParam is ID
	Tick.Invoke((Object*)this, e);
}

void Control::OnInnerTimer(EventArgs* e)
{
	if(e->WParam == CC_TIMERID_INVALIDATERECT)
	{
		::KillTimer(m_hWnd, CC_TIMERID_INVALIDATERECT);
		if(m_hWnd != NULL)
		{
			InvalidateRect(m_hWnd, NULL, TRUE);
		}
		return;
	}
	if(e->WParam != CC_TIMERID_CONTROL_MMOVE)
	{
		OnTimer(e);
		return;
	}
	POINT pt;
	::GetCursorPos(&pt);
	HWND hwnd = ::WindowFromPoint(pt);
	if(hwnd == m_hWnd && GetIsMouseOn())
	{
		//should ControlConfig::nWaitTimerCheckMouseLeave*5 < ToolTipConfig::nWaitHideMilliSecond and ControlConfig::nWaitTimerCheckMouseLeave*5 < CC_STATUSBAR_WAIT_TO_DEFAULTTEXT
		_nMouseHoverCount++;
		if(_nMouseHoverCount >= 3)
		{
			_nMouseHoverCount = 0;
			::PostMessage(m_hWnd, WM_MOUSEHOVER, 0, 0L);
		}
	}
	else
	{
		::KillTimer(m_hWnd, CC_TIMERID_CONTROL_MMOVE);
		::PostMessage(m_hWnd, WM_MOUSELEAVE, 0, 0L);
		//SetStyle(StyleType::Inner2, Inner2Style::IsMouseTimer|Inner2Style::IsMouseEnter, 0);
		_StyleArray[(int)StyleType::Inner2] &= ~(Inner2Style::IsMouseTimer|Inner2Style::IsMouseEnter);
	}
}

//should work with setcapture
// The system default hover time-out is initially the menu drop-down time,
// which is 400 milliseconds. You can call SystemParametersInfo and use
// SPI_GETMOUSEHOVERTIME to retrieve the default hover time-out.
//save hWnd of WindowFromPoint while MouseMove
static Control* Control_MOver_Ctl = NULL;
void Control::OnMouseMove(MouseEventArgs* e)
{
	if(HasStyle(StyleType::Inner, InnerStyle::Dragging))
	{
		//check with the tree control to see if we are on an item
		POINT point = {e->X, e->Y};
		ClientToScreen(m_hWnd, &point);

		//hide tips
		//HWND hWnd = (HWND)::SendMessage(m_hWnd, TVM_GETTOOLTIPS, 0, 0L);
		//::ShowWindow(hWnd, SW_HIDE);

		//paint the image in the new location
		ImageList_DragMove(point.x, point.y);
		//Unlocks the specified window and hides the drag image, allowing the window to be updated. 
		//20140729 not need next for TreeView
		//ImageList_DragLeave(NULL);
		//Shows or hides the image being dragged. 
		//ImageList_DragShowNolock(FALSE);

		HWND hWnd = ::WindowFromPoint(point);
		if(Control_MOver_Ctl == NULL)
		{
			Control_MOver_Ctl = this;
		}
		if(Control_MOver_Ctl->m_hWnd != hWnd)
		{
			//notice old ctl that leaved
			Control_MOver_Ctl->OnDragMove(e);

			//notice that ctl changed
			GiveFeedbackEventArgs ee(e->baseArgs);
			ee.Cancel = false;
			OnGiveFeedback(&ee);

			Control_MOver_Ctl = Control::FromHandle(hWnd);
		}
		if(Control_MOver_Ctl != NULL && Control_MOver_Ctl->HasStyle(StyleType::Outer, OuterStyle::Dropable))
		{
			Control_MOver_Ctl->OnDragMove(e);
		}

		//lock the screen again
		//ImageList_DragEnter(NULL, point.x, point.y);
		//ImageList_DragShowNolock(TRUE);
	}
	if(HasStyle(StyleType::Inner, InnerStyle::TrackMouse) && !GetIsMouseOn())
	{
		//if not GetCapture or capture&mouse in
		if(::GetCapture() != m_hWnd ||
			(e->X >= 0 && e->Y >= 0 && e->X <= (_Rect.right - _Rect.left) && e->Y <= (_Rect.bottom - _Rect.top)))
		{
			//SetStyle(StyleType::Inner2, 0, Inner2Style::IsMouseEnter);
			_StyleArray[(int)StyleType::Inner2] |= Inner2Style::IsMouseEnter;
			if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::IsMouseTimer) == 0)
			{
				//SetStyle(StyleType::Inner2, 0, Inner2Style::IsMouseTimer);
				_StyleArray[(int)StyleType::Inner2] |= Inner2Style::IsMouseTimer;
				_nMouseHoverCount = 3;
				//int nTimeout = 0;
				//SystemParametersInfo(SPI_GETMOUSEHOVERTIME, 0, &nTimeout, 0);
				::SetTimer(m_hWnd, CC_TIMERID_CONTROL_MMOVE, ControlConfig::nWaitTimerCheckMouseLeave, NULL);
			}
			OnMouseEnter(e->baseArgs);
		}
	}
	MouseMove.Invoke((Object*)this, e);
}

void Control::OnMouseUp(MouseEventArgs* e)
{
	MouseUp.Invoke((Object*)this, e);
}

//xPos = (int)(short) LOWORD(lParam);   // horizontal position
//yPos = (int)(short) HIWORD(lParam);   // vertical position
void Control::OnMove(EventArgs* e)
{
	Move.Invoke((Object*)this, e);
}

//lParam:Pointer to a RECT structure with the current position of the window, in screen coordinates.
//To change the position of the drag rectangle, an application must change the members of this structure.
void Control::OnMoving(EventArgs* e)
{
	Moving.Invoke((Object*)this, e);
}

void Control::OnNotifyMessage(EventArgs* e)
{
	if(Notify.IsEmpty())
	{
		LPNMHDR lpnmhdr = (LPNMHDR)e->LParam;
		ReflectLastMsg(lpnmhdr->hwndFrom, e);
	}
	else
	{
		Notify.Invoke((Object*)this, e);
	}
	//if(!e->Handled)
	//{
	//}
}

//void Control::OnParentBackColorChanged(EventArgs* e)
//{
	// FIXME: setting BackColor fires the BackColorChanged event,
	// so we do not need to call this here
	/*
	BackColor = Parent.BackColor;
	BackColorChanged.Invoke((Object*)this, e);
	*/					
//}
//
//void Control::OnParentBackgroundImageChanged(EventArgs* e)
//{
//	BackgroundImageChanged.Invoke((Object*)this, e);
//}
//
//void Control::OnParentBindingContextChanged(EventArgs* e)
//{
//	BindingContextChanged.Invoke((Object*)this, e);
//}
//
//void Control::OnParentChanged(EventArgs* e)
//{
//	ParentChanged.Invoke((Object*)this, e);
//}
//
//void Control::OnParentEnabledChanged(EventArgs* e)
//{
//	EnabledChanged.Invoke((Object*)this, e);
//}
//
//void Control::OnParentFontChanged(EventArgs* e)
//{
//	FontChanged.Invoke((Object*)this, e);
//}
//
//void Control::OnParentForeColorChanged(EventArgs* e)
//{
//	ForeColorChanged.Invoke((Object*)this, e);
//}
//
//void Control::OnParentRightToLeftChanged(EventArgs* e)
//{
//	RightToLeftChanged.Invoke((Object*)this, e);
//}
//
//void Control::OnParentVisibleChanged(EventArgs* e)
//{
//	VisibleChanged.Invoke((Object*)this, e);
//}
//
//void Control::OnQueryContinueDrag(QueryContinueDragEventArgs* e)
//{
//	QueryContinueDrag.Invoke((Object*)this, e);
//}

//http://www.vckbase.com/document/viewdoc/?id=1302
//http://www.programfan.com/article/showarticle.asp?id=2542
void Control::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
{
	//do nothing
}

void Control::OnResize(EventArgs* e)
{
	InvalidateBackGround();
	int nControlCount;
	//20081227 for refresh _Rect
	RECT NewRect = this->GetBounds();
	if(m_hWnd != NULL && _lstControls != NULL && (nControlCount = _lstControls->GetSize()) > 0)
	{
#if defined(_DEBUG_) || defined(_DESIGN)
		HWND hWnd = CC_APP_GetDesignBarHwnd();
		if(hWnd != NULL && !::IsChild(hWnd, m_hWnd))
		{
			::SendMessage(hWnd, CC_WM_MAINMSG, CC_WM_MAINMSG_WPARAM_DS_RESIZE, (LPARAM)this);
		}
#endif
		_StyleArray[(int)StyleType::Outer] |= OuterStyle::Sizing;
		HDWP hDWP = ::BeginDeferWindowPos(nControlCount); // reasonable guess
		RECT mainRect;
		::GetClientRect(m_hWnd, &mainRect);
		int nW = NewRect.right - NewRect.left;
		int nH = NewRect.bottom - NewRect.top;
		int nClientW = mainRect.right - mainRect.left;
		int nClientH = mainRect.bottom - mainRect.top;
		//int _nOldWidth = _OrgRect.right - _OrgRect.left;
		//int _nOldHeight = _OrgRect.bottom - _OrgRect.top;

#ifdef _DEBUG_
		if(CC_APP_GetConfig()->GetIsDebug())
		{
			//CC_TRACE((_T("Control::OnResize[%s,%s]OldL:%d, OldT:%d, OldW:%d, OldH:%d, NewL:%d, NewT:%d, NewW:%d, NewH:%d, ClientW:%d, ClientH:%d"),
			//	this->_ClassName, this->_Name, _OrgRect.left, _OrgRect.top, _nOldWidth, _nOldHeight, NewRect.left, NewRect.top, nW, nH, nClientW, nClientH));
			CC_TRACE((_T("Control::OnResize[%s,%s]OrgClientW:%d, OrgClientH:%d, NewL:%d, NewT:%d, NewW:%d, NewH:%d, ClientW:%d, ClientH:%d"),
				this->_ClassName, this->_Name, _ClientSize.cx, _ClientSize.cy, NewRect.left, NewRect.top, nW, nH, nClientW, nClientH));
		}
#endif

		//the last added control has more priority
		//if add statusbar1 and then statusbar2,so statusbar2 is bottom and statusbar1 is up of statusbar2
		//dock child
		int nDockH = 0;
		int nDockB = nClientH;
		int nDockW = 0;
		int nDockR = nClientW;
		for(int n = nControlCount - 1; n >= 0; n--)
		{
			Control* ctl = _lstControls->GetKey(n);
			if(ctl->Dock == DockStyles::None)
			{
				continue;
			}
			int nChildW = ctl->_OrgRect.right - ctl->_OrgRect.left;
			int nChildH = ctl->_OrgRect.bottom - ctl->_OrgRect.top;
			if((ctl->Dock & DockStyles::Fill) == DockStyles::Fill)
			{
				ctl->_OrgRect.left = 0;
				ctl->_OrgRect.top = 0;
				nChildW = CC_MAX(ctl->_minSize.cx, nClientW);
				nChildH = CC_MAX(ctl->_minSize.cy, nClientH);
			}
			else if((ctl->Dock & DockStyles::Top) != 0
				&& (ctl->Dock & DockStyles::Left) != 0
				&& (ctl->Dock & DockStyles::Right) != 0)
			{
				ctl->_OrgRect.top = nDockH;
				ctl->_OrgRect.left = 0;
				nChildW = nClientW - ctl->_OrgRect.left;
				nChildW = CC_MAX(ctl->_minSize.cx, nChildW);
				nChildH = CC_MAX(ctl->_minSize.cy, nChildH);
				nDockH += nChildH + 2;
			}
			else if((ctl->Dock & DockStyles::Bottom) != 0
				&& (ctl->Dock & DockStyles::Left) != 0
				&& (ctl->Dock & DockStyles::Right) != 0)
			{
				ctl->_OrgRect.top = nDockB - nChildH;
				ctl->_OrgRect.bottom = nDockB;
				ctl->_OrgRect.left = 0;
				nChildW = nClientW - ctl->_OrgRect.left;
				nChildW = CC_MAX(ctl->_minSize.cx, nChildW);
				nChildH = CC_MAX(ctl->_minSize.cy, nChildH);
				nDockB -= (nChildH + 2);
			}
			else if((ctl->Dock & DockStyles::Left) != 0
				&& (ctl->Dock & DockStyles::Top) != 0
				&& (ctl->Dock & DockStyles::Bottom) != 0)
			{
				ctl->_OrgRect.left = nDockW;
				ctl->_OrgRect.top = 0;
				nChildH = nClientH - ctl->_OrgRect.top;
				nChildW = CC_MAX(ctl->_minSize.cx, nChildW);
				nChildH = CC_MAX(ctl->_minSize.cy, nChildH);
				nDockW += nChildW + 2;
			}
			else if((ctl->Dock & DockStyles::Right) != 0
				&& (ctl->Dock & DockStyles::Top) != 0
				&& (ctl->Dock & DockStyles::Bottom) != 0)
			{
				//TODO
				ctl->_OrgRect.left = nDockR - nChildW;
				ctl->_OrgRect.right = nDockR;
				ctl->_OrgRect.top = 0;
				nChildH = nClientH - ctl->_OrgRect.top;
				nChildW = CC_MAX(ctl->_minSize.cx, nChildW);
				nChildH = CC_MAX(ctl->_minSize.cy, nChildH);
				nDockR -= (nChildW + 2);
			}
			else if((ctl->Dock & DockStyles::Top) != 0)
			{
				ctl->_OrgRect.top = nDockH;
				if((ctl->Dock & DockStyles::Bottom) != 0)
				{
					nChildH = nClientH - ctl->_OrgRect.top;
				}
				//resize
				if((ctl->Dock & DockStyles::Left) != 0)
				{
					ctl->_OrgRect.left = 0;
				}
				if((ctl->Dock & DockStyles::Right) != 0)
				{
					nChildW = nClientW - ctl->_OrgRect.left;
				}
				nChildW = CC_MAX(ctl->_minSize.cx, nChildW);
				nChildH = CC_MAX(ctl->_minSize.cy, nChildH);
				nDockH += nChildH + 2;
			}
			else if((ctl->Dock & DockStyles::Bottom) != 0)
			{
				ctl->_OrgRect.top = nDockB - nChildH;
				ctl->_OrgRect.bottom = nDockB;
				//resize
				if((ctl->Dock & DockStyles::Left) != 0)
				{
					ctl->_OrgRect.left = 0;
				}
				if((ctl->Dock & DockStyles::Right) != 0)
				{
					nChildW = nClientW - ctl->_OrgRect.left;
				}
				nChildW = CC_MAX(ctl->_minSize.cx, nChildW);
				nChildH = CC_MAX(ctl->_minSize.cy, nChildH);
				nDockB -= (nChildH + 2);
			}

#ifdef _DEBUG_
			if(CC_APP_GetConfig()->GetIsDebug())
			{
				CC_TRACE((_T("Control::OnResize[Dock][%s,%s]OldL:%d, OldT:%d, OldW:%d, OldH:%d, NewL:%d, NewT:%d, NewW:%d, NewH:%d"),
					ctl->_ClassName, ctl->_Name, ctl->_OrgRect.left, ctl->_OrgRect.top, ctl->_OrgRect.right - ctl->_OrgRect.left, ctl->_OrgRect.bottom - ctl->_OrgRect.top, ctl->_OrgRect.left, ctl->_OrgRect.top, nChildW, nChildH));
			}
#endif

			//ctl->SetBoundsNotOriginal(ctl->_OrgRect.left, ctl->_OrgRect.top, nChildW, nChildH, BoundsSpecified::All);
 			hDWP = ::DeferWindowPos(hDWP, ctl->m_hWnd, NULL, ctl->_OrgRect.left, ctl->_OrgRect.top, nChildW, nChildH, SWP_NOACTIVATE|SWP_NOZORDER);
		}
		//mainRect.left = nDockW;
		//mainRect.top = nDockH;
		//mainRect.right = nClientW - nDockR;
		//mainRect.bottom = nClientH - nDockB;
		//anchor child
		for(int n = nControlCount - 1; n >= 0; n--)
		{
			Control* ctl = _lstControls->GetKey(n);
			if(ctl->Dock != DockStyles::None || ctl->Anchor == AnchorStyles::None || ctl->Anchor == (AnchorStyles::Type)(AnchorStyles::Top | AnchorStyles::Left))
			{
				continue;
			}
			//resize is referent to OriginalBounds
			RECT rectReal;
			::GetWindowRect(ctl->m_hWnd, &rectReal);
			int nNewL = ctl->_OrgRect.left;
			int nNewT = ctl->_OrgRect.top;
			int nNewW = ctl->_OrgRect.right - ctl->_OrgRect.left;
			int nNewH = ctl->_OrgRect.bottom - ctl->_OrgRect.top;
			if((ctl->Anchor & AnchorStyles::Top) == AnchorStyles::Top)
			{
				if((ctl->Anchor & AnchorStyles::Bottom) == AnchorStyles::Bottom)
				{
					nNewH += nH - _ClientSize.cy;
				}
				else
				{
					nNewH = rectReal.bottom - rectReal.top;
				}
			}
			else
			{
				if((ctl->Anchor & AnchorStyles::Bottom) == AnchorStyles::Bottom)
				{
					nNewT += nH - _ClientSize.cy;
				}
				else
				{
					nNewT += (nH - _ClientSize.cy)/2;
				}
			}
			if((ctl->Anchor & AnchorStyles::Left) == AnchorStyles::Left)
			{
				if((ctl->Anchor & AnchorStyles::Right) == AnchorStyles::Right)
				{
					nNewW += nW - _ClientSize.cx;
				}
				else
				{
					nNewW = rectReal.right - rectReal.left;
				}
			}
			else
			{
				if((ctl->Anchor & AnchorStyles::Right) == AnchorStyles::Right)
				{
					nNewL += nW - _ClientSize.cx;
				}
				else
				{
					nNewL += (nW - _ClientSize.cx)/2;
				}
			}
			nNewW = CC_MAX(ctl->_minSize.cx, nNewW);
			nNewH = CC_MAX(ctl->_minSize.cy, nNewH);
			nNewL = CC_MAX(ctl->MinPosition.x, nNewL);
			nNewT = CC_MAX(ctl->MinPosition.y, nNewT);

#ifdef _DEBUG_
			if(CC_APP_GetConfig()->GetIsDebug())
			{
				CC_TRACE((_T("Control::OnResize[Anchor][%s,%s]OldL:%d, OldT:%d, OldW:%d, OldH:%d, NewL:%d, NewT:%d, NewW:%d, NewH:%d"),
					ctl->_ClassName, ctl->_Name, ctl->_OrgRect.left, ctl->_OrgRect.top, ctl->_OrgRect.right - ctl->_OrgRect.left, ctl->_OrgRect.bottom - ctl->_OrgRect.top, nNewL, nNewT, nNewW, nNewH));
			}
#endif

			//ctl->SetBoundsNotOriginal(ctl->_OrgRect.left + nNewL, ctl->_OrgRect.top + nNewT, nNewW, nNewH, BoundsSpecified::All);
 			hDWP = ::DeferWindowPos(hDWP, ctl->m_hWnd, NULL, nNewL, nNewT, nNewW, nNewH, SWP_NOACTIVATE|SWP_NOZORDER);
		}
		::EndDeferWindowPos(hDWP);
		_StyleArray[(int)StyleType::Outer] &= ~OuterStyle::Sizing;
	}
	Resize.Invoke((Object*)this, e);
}

void Control::OnResizing(EventArgs* e)
{
	Resizing.Invoke((Object*)this, e);
}

//
//void Control::OnRightToLeftChanged(EventArgs* e)
//{
//	RightToLeftChanged.Invoke((Object*)this, e);
//}

void Control::OnSizeChanged(EventArgs* e)
{
	SizeChanged.Invoke((Object*)this, e);
}

//wParam:GWL_EXSTYLE or GWL_STYLE
void Control::OnStyleChanged(EventArgs* e)
{
	_nUpdateFlag |= UpdateFlag::UpdateStyle;
	Update();
	StyleChanged.Invoke((Object*)this, e);
}

void Control::OnSystemColorsChanged(EventArgs *e)
{
	SystemColorsChanged.Invoke((Object*)this, e);
}
//
//void Control::OnTabIndexChanged(EventArgs *e)
//{
//	TabIndexChanged.Invoke((Object*)this, e);
//}
//
//void Control::OnTabStopChanged(EventArgs *e)
//{
//	TabStopChanged.Invoke((Object*)this, e);
//}

void Control::OnTextChanged(EventArgs *e)
{
	//to end msg loop
	e->Result = DefWndProc(e);
	e->Handled = true;
	_nUpdateFlag |= UpdateFlag::UpdateText;
	Update();
	TextChanged.Invoke((Object*)this, e);
}
//
//void Control::OnValidated(EventArgs *e)
//{
//	Validated.Invoke((Object*)this, e);
//}
//
//void Control::OnValidating(CancelEventArgs *e)
//{
//	Validating.Invoke((Object*)this, e);
//}

void Control::OnVisibleChanged(EventArgs *e)
{
	//wParam:Specifies whether a window is being shown.
	//  If wParam is TRUE, the window is being shown.
	//  If wParam is FALSE, the window is being hidden.
	//do update at onchange
	if(e != NULL)
	{
		//_Visible = (e->WParam == TRUE);
		//CC_BIT_SET(_Flags, CC_FLAG_CONTROL_VISIBLE, (e->WParam == TRUE));
		SetStyleBool(StyleType::Inner2, Inner2Style::Visible, (e->WParam == TRUE));
		_nUpdateFlag |= UpdateFlag::UpdateVisible;
		Update();

		VisibleChanged.Invoke((Object*)this, e);
	}
}

void Control::OnSysCommand(EventArgs *e)
{
	//UINT nID = wParam, LPARAM lParam
	SysCommand.Invoke((Object*)this, e);
}

//e->LParam is the next control,if not NULL
void Control::OnNextControl(EventArgs *e)
{
	//UINT nID = wParam, LPARAM lParam
	NextControl.Invoke((Object*)this, e);
}
// On...
///////////////////////////////////////////////////////////////////////////////
//#endregion
void Control::DoUpdate()
{
	//this is called from inner
	if(IsUpdate((UpdateFlag::Type)(UpdateFlag::UpdateTips|UpdateFlag::UpdateTipsExt)))
	{
		if(_Tips != NULL || _TipsExt != NULL)
		{
			//need TrackMouse for show tips
			SetTrackMouse(true);
		}
	}

	//if not created, wait for update next
	if(m_hWnd == NULL)
	{
		return;
	}

	if(HasStyle(StyleType::Outer, OuterStyle::AutoSize) && IsUpdate(UpdateFlag::UpdateText))
	{
		//update size
		CalcuteRect(true);
	}
	if(IsUpdate((UpdateFlag::Type)(UpdateFlag::UpdateText|UpdateFlag::UpdateIcon|UpdateFlag::UpdateTextColor|UpdateFlag::UpdateVisible|UpdateFlag::UpdateEnabled|InnerStyle::BackgroundBmp|InnerStyle::BackgroundGradient)))
	{
		if(m_hWnd != NULL)
		{
			::InvalidateRect(m_hWnd, NULL, TRUE);
		}
	}

	//clear flages
	ITextIcon::DoUpdate();
}
//
//Control* Control::GetTopLevelParent()
//{
//	if(m_hWnd == NULL) // no Window attached
//	{
//		return NULL;
//	}
//
//	HWND hWndParent = m_hWnd;
//	HWND hWndT;
//	// return parent in the Windows sense
//	while((hWndT = (::GetWindowLong(hWndParent, GWL_STYLE) & WS_CHILD) ?
//		::GetParent(hWndParent) : ::GetWindow(hWndParent, GW_OWNER)) != NULL)
//	{
//		hWndParent = hWndT;
//	}
//
//	return Control::FromHandle(hWndParent);
//}
//
//Control* Control::GetActiveWindow()
//{
//	HWND hWnd = ::GetActiveWindow();
//	if(hWnd != NULL)
//	{
//		return Control::FromHandle(hWnd);
//	}
//	return NULL;
//}
//
//Control* Control::GetTopForm()
//{
//	CC_ASSERT(m_hWnd != NULL);
//
//	return (Control*)::SendMessage(::GetParent(m_hWnd), CC_WM_MAINMSG, CC_WM_MAINMSG_WPARAM_GETFORM, 0);
//}

Control* PASCAL Control::FromHandle(HWND hWnd)
{
	int nIndex = CC_APP_GetMapHWND()->GetIndex(hWnd);
	if(nIndex >= 0)
	{
		return CC_APP_GetMapHWND()->GetAt(nIndex);
	}
	else
	{
		return NULL;
	}
}

Control* Control::GetNextControl(Control* curControl, bool forward)
{
	int nCnt;
	if(curControl != NULL)
	{
		EventArgs ee(0, 0, (WPARAM)forward, 0);
		curControl->OnNextControl(&ee);
		if(ee.LParam != NULL)
		{
			return (Control*)ee.LParam;
		}
	}
	if(_lstControls == NULL || (nCnt = _lstControls->GetSize()) <= 0)
	{
		return NULL;
	}
	Control* ctlMinTabIndex = NULL;
	int nMinTabIndex = -1;
	if(curControl == NULL || _lstControls->GetIndex(curControl) < 0)
	{
		// find a direct child of this from the window that has focus
		curControl = GetFocus();
		Control* curControlOrg = curControl;
		//find the control that it's parent is this
		for(; curControl != NULL && curControl->_pParent != this; curControl = curControl->_pParent)
		{
		}
		if(curControl != NULL)
		{
			//curControlOrg is child of this,seek next
			EventArgs ee(0, 0, (WPARAM)forward, 0);
			curControlOrg->OnNextControl(&ee);
			if(ee.LParam != NULL)
			{
				return (Control*)ee.LParam;
			}
			//next seek next control of curControl
		}
		else
		{
			//search the first child
			for(int n = 0; n < nCnt; n++)
			{
				Control* ctl = _lstControls->GetKey(n);
				if(ctl->IsTabStop() && (nMinTabIndex == -1 || nMinTabIndex > ctl->TabIndex))
				{
					nMinTabIndex = ctl->TabIndex;
					ctlMinTabIndex = ctl;
				}
			}
			return ctlMinTabIndex;
		}
	}
	Control* ctlBigger = NULL;
	int nBigger = -1;
	int nFromTabIndex = _lstControls->GetIndex(curControl);
	if(!forward)
	{
		//find behind that has big tabindex of this
		for(int n = nFromTabIndex + 1; n < nCnt; n++)
		{
			Control* ctl = _lstControls->GetKey(n);
			if(!ctl->IsTabStop())
			{
				continue;
			}
			if(curControl->TabIndex == ctl->TabIndex)
			{
				//same tabindex,but behind,so is it
				return ctl;
			}
			if(ctl->TabIndex > curControl->TabIndex && (nBigger == -1 || (ctl->TabIndex - curControl->TabIndex) < nBigger))
			{
				nBigger = ctl->TabIndex - curControl->TabIndex;
				ctlBigger = ctl;
			}
			//search min tabindex control.for mabey loop
			if(nMinTabIndex == -1 || nMinTabIndex > ctl->TabIndex)
			{
				nMinTabIndex = ctl->TabIndex;
				ctlMinTabIndex = ctl;
			}
		}
		for(int n = 0; n < nFromTabIndex; n++)
		{
			Control* ctl = _lstControls->GetKey(n);
			if(!ctl->IsTabStop())
			{
				continue;
			}
			if(curControl->TabIndex < ctl->TabIndex && (nBigger == -1 || ctl->TabIndex - curControl->TabIndex < nBigger))
			{
				nBigger = ctl->TabIndex - curControl->TabIndex;
				ctlBigger = ctl;
			}
			//search min tabindex control.for mabey loop
			if(nMinTabIndex == -1 || nMinTabIndex > ctl->TabIndex)
			{
				nMinTabIndex = ctl->TabIndex;
				ctlMinTabIndex = ctl;
			}
		}
		if(ctlBigger != NULL)
		{
			return ctlBigger;
		}
		//no bigger tabindex,set to minindex,if need
		if(HasStyle(StyleType::Inner, InnerStyle::TabLoopInner))
		{
			if(ctlMinTabIndex == NULL)
			{
				return curControl;
			}
			return ctlMinTabIndex;
		}
		//no next let parent do it
		return NULL;
	}
	else
	{
		//find behind that has big tabindex of this
		for(int n = nFromTabIndex - 1; n >= 0; n--)
		{
			Control* ctl = _lstControls->GetKey(n);
			if(!ctl->IsTabStop())
			{
				continue;
			}
			if(curControl->TabIndex == ctl->TabIndex)
			{
				//same tabindex,but behind,so is it
				return ctl;
			}
			if(ctl->TabIndex < curControl->TabIndex && (nBigger == -1 || (curControl->TabIndex - ctl->TabIndex) < nBigger))
			{
				nBigger = curControl->TabIndex - ctl->TabIndex;
				ctlBigger = ctl;
			}
			//search min tabindex control.for mabey loop
			if(nMinTabIndex == -1 || nMinTabIndex < ctl->TabIndex)
			{
				nMinTabIndex = ctl->TabIndex;
				ctlMinTabIndex = ctl;
			}
		}
		for(int n = nCnt - 1; n > nFromTabIndex; n--)
		{
			Control* ctl = _lstControls->GetKey(n);
			if(!ctl->IsTabStop())
			{
				continue;
			}
			if(curControl->TabIndex > ctl->TabIndex && (nBigger == -1 || curControl->TabIndex - ctl->TabIndex < nBigger))
			{
				nBigger = curControl->TabIndex - ctl->TabIndex;
				ctlBigger = ctl;
			}
			//search min tabindex control.for mabey loop
			if(nMinTabIndex == -1 || nMinTabIndex < ctl->TabIndex)
			{
				nMinTabIndex = ctl->TabIndex;
				ctlMinTabIndex = ctl;
			}
		}
		if(ctlBigger != NULL)
		{
			return ctlBigger;
		}
		//no bigger tabindex,set to minindex,if need
		if(HasStyle(StyleType::Inner, InnerStyle::TabLoopInner))
		{
			if(ctlMinTabIndex == NULL)
			{
				return curControl;
			}
			return ctlMinTabIndex;
		}
		//no next let parent do it
		return NULL;
	}
	return NULL;
}
//
//Control* Control::TopLevelControl()
//{
//	Control* ctrl = this;
//	Control* parent;
//	while((parent = ctrl->_pParent) != NULL)
//	{
//		ctrl = ctrl->_pParent;
//	}
//	return ctrl;
//}

void Control::FreeCall(Object* sender, EventArgs* e)
{
	//do nothing.
}

//dynamic position(now real size)
RECT Control::GetBounds()
{
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
	{
		::GetWindowRect(m_hWnd, &_Rect);
		HWND parent = ::GetParent(m_hWnd);
		if(parent != NULL)
		{
			::ScreenToClient(parent, (LPPOINT)&_Rect);
			::ScreenToClient(parent, ((LPPOINT)&_Rect)+1);
		}
	}
	return _Rect;
}

//original position(set by user,but not prg)
RECT Control::GetOriginalBounds()
{
	return _OrgRect;
}

////for _OrgRect is changed at setsize and others,so need call here after those
//void Control::SetOriginalSize(int width, int height)
//{
//	_OrgRect.right = _OrgRect.left + width;
//	_OrgRect.bottom = _OrgRect.top + height;
//	if(_pClient != NULL)
//	{
//		::CopyRect(&_pClient->_OrgRect, &_OrgRect);
//	}
//}
//
//void Control::SetOriginalBounds(int x, int y, int width, int height)
//{
//	_OrgRect.left = x;
//	_OrgRect.top = y;
//	SetOriginalSize(width, height);
//}

//set real position but not original,so even not _bIsHandleCreated then also resize child to parent
void Control::SetBoundsNotOriginal(RECT rect, BoundsSpecified::Type specified)
{
	SetBoundsCore(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, specified, false);
}

void Control::SetBoundsNotOriginal(int x, int y, int width, int height, BoundsSpecified::Type specified)
{
	SetBoundsCore(x, y, width, height, specified, false);
}

void Control::SetBoundsCalcuteOriginal(int x, int y, int width, int height)
{
	SetBoundsCore(x, y, width, height, BoundsSpecified::All, true);
	if(_pParent == NULL || _pParent->m_hWnd == NULL || Dock != DockStyles::None || Anchor == AnchorStyles::None || Anchor == (AnchorStyles::Type)(AnchorStyles::Top | AnchorStyles::Left))
	{
		return;
	}
	RECT rectReal;
	::GetClientRect(_pParent->m_hWnd, &rectReal);
	int nNewW = _pParent->_ClientSize.cx - (rectReal.right - rectReal.left);
	int nNewH = _pParent->_ClientSize.cy - (rectReal.bottom - rectReal.top);
	rectReal = this->GetBounds();
	if((Anchor & AnchorStyles::Top) == AnchorStyles::Top)
	{
		_OrgRect.top = rectReal.top;
		if((Anchor & AnchorStyles::Bottom) == AnchorStyles::Bottom)
		{
			_OrgRect.bottom = rectReal.bottom + nNewH;
		}
		else
		{
			_OrgRect.bottom = rectReal.bottom;
		}
	}
	else
	{
		if((Anchor & AnchorStyles::Bottom) == AnchorStyles::Bottom)
		{
			_OrgRect.bottom = rectReal.bottom + nNewH;
			_OrgRect.top = rectReal.top + nNewH;
		}
	}
	if((Anchor & AnchorStyles::Left) == AnchorStyles::Left)
	{
		_OrgRect.left = rectReal.left;
		if((Anchor & AnchorStyles::Right) == AnchorStyles::Right)
		{
			_OrgRect.right = rectReal.right + nNewW;
		}
		else
		{
			_OrgRect.right = rectReal.right;
		}
	}
	else
	{
		if((Anchor & AnchorStyles::Right) == AnchorStyles::Right)
		{
			_OrgRect.right = rectReal.right + nNewW;
			_OrgRect.left = rectReal.left + nNewW;
		}
	}
}

//set real position also original,so while not _bIsHandleCreated then not resize child to parent
void Control::SetBounds(RECT rect, BoundsSpecified::Type specified)
{
	SetBoundsCore(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, specified, true);
}

void Control::SetBounds(int x, int y, int width, int height, BoundsSpecified::Type specified)
{
	SetBoundsCore(x, y, width, height, specified, true);
}

void Control::SetLocation(int xLeft, int yTop)
{
	SetBoundsCore(xLeft, yTop, 0, 0, BoundsSpecified::Location, true);
}

void Control::SetLocationOffset(Control* control, bool isNewLine, int offsetX, int offsetY)
{
	//_pOffsetControl = control;
	////CC_BIT_SET(_Flags, CC_FLAG_CONTROL_OFFSETNEWLINE, isNewLine);
	//SetStyleBool(StyleType::Inner2, Inner2Style::OffsetNewline, isNewLine);
	//_ptOffset.x = offsetX;
	//_ptOffset.y = offsetY;
	CC_ASSERT(control != NULL);
	if(isNewLine)
	{
		SetLocation(control->_Rect.left + offsetX, control->_Rect.bottom + offsetY);
		if(control->MinPosition.x > 0)
		{
			MinPosition.x = control->MinPosition.x + offsetX;
		}
		if(control->MinPosition.y > 0)
		{
			MinPosition.y = control->MinPosition.y + control->_Rect.bottom - control->_Rect.top + offsetY;
		}
	}
	else
	{
		SetLocation(control->_Rect.right + offsetX, control->_Rect.top + offsetY);
		if(control->MinPosition.x > 0)
		{
			MinPosition.x = control->MinPosition.x + control->_Rect.right - control->_Rect.left + offsetX;
		}
		if(control->MinPosition.y > 0)
		{
			MinPosition.y = control->MinPosition.y + offsetY;
		}
	}
}

//while form,if children positon is fixed,then recommend use SetSizeBaseOnClient(alse call SetClientSize).
void Control::SetClientSize(int cxWidth, int cyHeight)
{
	_ClientSize.cx = cxWidth;
	_ClientSize.cy = cyHeight;
	if(_pClient != NULL)
	{
		_pClient->SetClientSize(cxWidth, cyHeight);
	}
}

void Control::SetSize(int cxWidth, int cyHeight)
{
	SetBoundsCore(0, 0, cxWidth, cyHeight, BoundsSpecified::Size, true);
}

void Control::SetBoundsCore(int left, int top, int width, int height, BoundsSpecified::Type specified, bool isSetSameOriginal)
{
	bool bLocationChanged = false;
	bool bSizeChanged = false;
	//_Rect = this->GetBounds();
	if(specified != BoundsSpecified::Size && specified != BoundsSpecified::Location && specified != BoundsSpecified::All)
	{
		if(specified == BoundsSpecified::X)
		{
			if(_Rect.left != left)
			{
				_Rect.left = left;
				bLocationChanged = true;
			}
		}
		else if(specified == BoundsSpecified::Y)
		{
			if(_Rect.top != top)
			{
				_Rect.top = top;
				bLocationChanged = true;
			}
		}
		else if(specified == BoundsSpecified::Width)
		{
			if(_Rect.right != _Rect.left + width)
			{
				_Rect.right = _Rect.left + width;
				bSizeChanged = true;
			}
		}
		else if(specified == BoundsSpecified::Height)
		{
			if(_Rect.bottom != _Rect.top + height)
			{
				_Rect.bottom = _Rect.top + height;
				bSizeChanged = true;
			}
		}
	}
	else
	{
		int _nOldWidth = _Rect.right - _Rect.left;
		int _nOldHeight = _Rect.bottom - _Rect.top;
		if(specified == BoundsSpecified::Location || specified == BoundsSpecified::All)
		{
			if(_Rect.left != left || _Rect.top != top)
			{
				_Rect.left = left;
				_Rect.top = top;
				_Rect.right = _Rect.left + _nOldWidth;
				_Rect.bottom = _Rect.top + _nOldHeight;
				bLocationChanged = true;
			}
		}
		if(specified == BoundsSpecified::Size || specified == BoundsSpecified::All)
		{
			if(_nOldWidth !=  width || _nOldHeight !=  height)
			{
				_Rect.right = _Rect.left + width;
				_Rect.bottom = _Rect.top + height;
				bSizeChanged = true;
			}
		}
	}
	int nW = _Rect.right - _Rect.left;
	int nH = _Rect.bottom - _Rect.top;
	if(nW < _minSize.cx)
	{
		nW = _minSize.cx;
		_Rect.right = _Rect.left + nW;
	}
	if(nH < _minSize.cy)
	{
		nH = _minSize.cy;
		_Rect.bottom = _Rect.top + nH;
	}
//err(_T("Control::SetBoundsCore 999, left:%d, top:%d, nW:%d, nH:%d"), _Rect.left, _Rect.top, nW, nH);
	if(isSetSameOriginal)
	{
		::CopyRect(&_OrgRect, &_Rect);
	}
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0
		&& (bLocationChanged || bSizeChanged))
	{
		int nFlags = SWP_NOOWNERZORDER;
		//_SetWindowPosFlags | SWP_NOOWNERZORDER;
		if(HasStyle(StyleType::Inner2, Inner2Style::WindowPosFlagsNoAct))
		{
			nFlags |= SWP_NOACTIVATE;
		}
		else if(!HasStyle(StyleType::Inner2, Inner2Style::WindowPosFlags0))
		{
			nFlags |= SWP_NOZORDER | SWP_FRAMECHANGED | SWP_DRAWFRAME;
		}
		if(!bLocationChanged)
		{
			nFlags |= SWP_NOMOVE;
		}
		if(!bSizeChanged)
		{
			nFlags |= SWP_NOSIZE;
		}

#ifdef _DEBUG_
		if(CC_APP_GetConfig()->GetIsDebug())
		{
			CC_TRACE((_T("Control::SetBoundsCore[%s,%s]ParaL:%d, ParaT:%d, ParaW:%d, ParaH:%d, NewL:%d, NewT:%d, NewW:%d, NewH:%d"),
				this->_ClassName, this->_Name, left, top, width, height, _Rect.left, _Rect.top, _Rect.right - _Rect.left, _Rect.bottom - _Rect.top));
		}
#endif

		::SetWindowPos(m_hWnd, NULL, _Rect.left, _Rect.top, nW, nH, nFlags);
		if(bLocationChanged)
		{
			OnLocationChanged(NULL);
		}
		if(bSizeChanged)
		{
			OnSizeChanged(NULL);
		}
	}
}

SIZE Control::GetMinSize()
{
	return _minSize;
}

void Control::SetMinSize(int minWidth, int minHeight)
{
	_minSize.cx = minWidth;
	_minSize.cy = minHeight;
	CalcuteRect(true);
}

RECT Control::GetMargin()
{
	return _Margin;
}

void Control::SetMargin(int nLeft, int nTop, int nRight, int nBottom)
{
	::SetRect(&_Margin, nLeft, nTop, nRight, nBottom);
	CalcuteRect(true);
}

COLORREF Control::GetTextColor()
{
	return _corTextground;
}

//for set color while WM_CTLCOLOREDIT
void Control::SetTextColor(COLORREF corText)
{
	_corTextground = corText;
	//do update at onchange
	_nUpdateFlag |= UpdateFlag::UpdateTextColor;
	Update();
}

COLORREF Control::GetBackground() const
{
	return _corBackground;
}

void Control::SetBackground(COLORREF corBackground)
{
	SetBackground(corBackground, -1);
}
void Control::SetBackground(COLORREF corBackground, int nBackgroundHatch)
{
	_corBackground = corBackground;
	_nBackgroundHatch = nBackgroundHatch;
	EventArgs ee(m_hWnd, 0, 0, (LPARAM)_corBackground);
	OnBackColorChanged(&ee);
	//also do GetBackgroundBrush and InvalidateRect and update
	_StyleArray[(int)StyleType::Inner] &= ~(InnerStyle::BackgroundBrush|InnerStyle::BackgroundBmp|InnerStyle::BackgroundTheme);
	_StyleArray[(int)StyleType::Inner] |= InnerStyle::BackgroundColor;
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0 && m_hWnd != NULL)
	{
		GetBackgroundBrush(true);
		::InvalidateRect(m_hWnd, NULL, TRUE);
	}
}

//should notice that this only valid for some owerndraw ctl
void Control::SetHotBackColor(COLORREF corHotBackColor)
{
	_corHotBackColor = corHotBackColor;
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0 && m_hWnd != NULL)
	{
		CC_ASSERT(::IsWindow(m_hWnd));

		::InvalidateRect(m_hWnd, NULL, FALSE);
	}
}

COLORREF Control::GetHotBackColor() const
{
	return _corHotBackColor;
}

//should notice that this only valid for some owerndraw ctl
void Control::SetHotTextColor(COLORREF corHotTextColor)
{
	_corHotTextColor = corHotTextColor;
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0 && m_hWnd != NULL)
	{
		CC_ASSERT(::IsWindow(m_hWnd));

		::InvalidateRect(m_hWnd, NULL, FALSE);
	}
}

COLORREF Control::GetHotTextColor() const
{
	return _corHotTextColor;
}

//HBITMAP Control::GetBackgroundImage()
//{
//	return _hbmpBackground;
//}
//
bool Control::SetBackgroundImage(UINT nResID, const TCHAR *resType, bool isStretch)
{
	CC_ASSERT(resType != NULL);
	HBITMAP hBitmap = CC_APP_GetDrawActor()->GetBitmap(nResID, resType, _corBackground);
	if(hBitmap != NULL)
	{
		SetBackgroundImage(hBitmap, isStretch, true);
		return true;
	}
	return false;
}

bool Control::SetBackgroundImage(const TCHAR* filename, bool isStretch)
{
	CC_ASSERT(filename != NULL);
	HBITMAP hBitmap = CC_APP_GetDrawActor()->GetBitmap(filename, _corBackground);
	if(hBitmap != NULL)
	{
		SetBackgroundImage(hBitmap, isStretch, true);
		CC_BIT_ON(_Flags, CC_FLAG_BACKFILE_OK);
		return true;
	}
	CC_BIT_OFF(_Flags, CC_FLAG_BACKFILE_OK);
	return false;
}

void Control::SetBackgroundImage(HICON hiconBackground, bool isStretch, bool needDestroy)
{
	HBITMAP hBitmap = CC_APP_GetDrawActor()->Icon2Bmp(hiconBackground, _corBackground);
	SetBackgroundImage(hBitmap, isStretch, true);
}

//if isStretch,then turn hbmpBackground to HBRUSH,otherwise use need save hbmpBackground
void Control::SetBackgroundImage(HBITMAP hbmpBackground, bool isStretch, bool needDestroy)
{
	if(_hbmpBackground != NULL && _hbmpBackground != hbmpBackground
		&& HasStyle(StyleType::Inner2, Inner2Style::BackgroundDestroyBmp))
	{
		::DeleteObject(_hbmpBackground);
		_hbmpBackground = NULL;
	}
	if(hbmpBackground == NULL)
	{
		_hbmpBackground = NULL;
		_StyleArray[(int)StyleType::Inner] &= ~(InnerStyle::BackgroundBrush|InnerStyle::BackgroundBmp|InnerStyle::BackgroundTheme|InnerStyle::BackgroundGradient);
		_StyleArray[(int)StyleType::Inner] |= InnerStyle::BackgroundColor;
		if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0 && m_hWnd != NULL)
		{
			GetBackgroundBrush(true);
			::InvalidateRect(m_hWnd, NULL, TRUE);
		}
		return;
	}
	if(!isStretch)
	{
		HBRUSH hbr = ::CreatePatternBrush(hbmpBackground);
		SetBackgroundBrush(hbr, true);
		if(needDestroy)
		{
			::DeleteObject(hbmpBackground);
		}
		return;
	}
	_hbmpBackground = hbmpBackground;
	SetStyleBool(StyleType::Inner2, Inner2Style::BackgroundDestroyBmp, needDestroy);
	_StyleArray[(int)StyleType::Inner] &= ~(InnerStyle::BackgroundBrush|InnerStyle::BackgroundColor|InnerStyle::BackgroundTheme|InnerStyle::BackgroundGradient);
	_StyleArray[(int)StyleType::Inner] |= InnerStyle::BackgroundBmp;
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0 && m_hWnd != NULL)
	{
		GetBackgroundBrush(true);
		::InvalidateRect(m_hWnd, NULL, TRUE);
	}
}

Control* Control::GetClient()
{
	return _pClient;
}

void Control::SetTrackMouse(bool isTrackMouse)
{
	if(isTrackMouse)
	{
		_StyleArray[(int)StyleType::Inner] |= InnerStyle::TrackMouse;
	}
	else
	{
		_StyleArray[(int)StyleType::Inner] &= ~InnerStyle::TrackMouse;
	}
	//SetStyleBool(StyleType::Inner, InnerStyle::TrackMouse, isTrackMouse);
}

HCURSOR Control::GetCursor() const
{
	return _hCursor;
}

//::LoadCursor(NULL, IDC_HAND)
void Control::SetCursor(HCURSOR hCursor)
{
	_hCursor = hCursor;
}

//keyArr is param address,but sHead is by value
void Control::DoConfig(bool isFromIni, KeyArr& keyArr, cc::Str sThisHead)
{
	//default do nothin,here is sample
	//if(!HasStyle(StyleType::Outer, OuterStyle::AutoConfig))
	//{
	//	return;
	//}
	//if(isFromIni)
	//{
	//	//if not exist,as usually
	//	if(keyArr.Exist(sThisHead))
	//	{
	//		SetChecked(keyArr.GetBool(sThisHead, false));
	//	}
	//}
	//else
	//{
	//	keyArr.SetBool(sThisHead, GetChecked());
	//}
}

//if sParentHead is "MainForm",then sub control maybe "MainForm.Control1"
void Control::Config(bool isFromIni, KeyArr& keyArr, cc::Str sParentHead)
{
	if(_ConfigName == NULL || _tcscmp(_T(""), _ConfigName) != 0)
	{
		if(sParentHead.GetLength() > 0)
		{
			sParentHead.Append(_T("."));
		}
		sParentHead.Append(_ConfigName == NULL ? _Name : _ConfigName);
	}
	DoConfig(isFromIni, keyArr, sParentHead);
	//perhaps child has AutoConfig set
	if(_lstControls != NULL)
	{
		for(int n = _lstControls->GetSize() - 1; n >= 0; n--)
		{
			_lstControls->GetKey(n)->Config(isFromIni, keyArr, sParentHead);
		}
	}
}

#if defined(_DEBUG_) || defined(_DESIGN)
void Control::CreateSource(cc::Str& sTxt, cc::Str& sVarList, cc::Str sParentHead)
{
	//if _ConfigName is "",not add to Head
	if(_ConfigName == NULL || _tcscmp(_T(""), _ConfigName) != 0)
	{
		if(sParentHead.GetLength() > 0)
		{
			sParentHead.Append(_T("."));
		}
		sParentHead.Append(_ConfigName == NULL ? _Name : _ConfigName);
	}
	DoCreateSource(sTxt, sVarList, sParentHead);
	Control* ctl = this;
	while(ctl->_pClient != NULL)
	{
		ctl = ctl->_pClient;
	}
	if(ctl->_lstControls != NULL)
	{
		for(int n = 0; n < ctl->_lstControls->GetSize(); n++)
		{
			ctl->_lstControls->GetKey(n)->CreateSource(sTxt, sVarList, sParentHead);
		}
	}
}

void Control::DoCreateSource(cc::Str& sTxt, cc::Str& sVarList, cc::Str sThisHead)
{
	CC_APP_CreateSource(this, sTxt, sVarList, sThisHead);
}
#endif

void Control::DoSetLang(KeyArr& keyArr, cc::Str sThisHead)
{
	if(_ConfigName == NULL || _tcscmp(_T(""), _ConfigName) != 0)
	{
		ITextIcon::DoSetLang(keyArr, sThisHead);
//while only need load design ini,then need define _DESIGNRUN
#if defined(_DEBUG_) || defined(_DESIGN) || defined(_DESIGNRUN)
		//not log Get, key: [Lang.*._DFlag] is not found
		int nInd = keyArr.GetIndex(sThisHead + _T("._DFlag"));
		if(nInd >= 0)
		{
			DWORD nDFlag = (DWORD)keyArr.GetKey(nInd).ToDouble();
			cc::Str buf;
			cc::Str buf1;
			if((nDFlag & CC_DESIGN_DS_ANCHOR) != 0)
			{
				buf = keyArr.Get(sThisHead + _T("._DAnchor"));
				buf1 = buf.SplitBy(_T(","));
				if(buf1.GetLength() > 0)
				{
					this->Anchor = (AnchorStyles::Type)buf1.ToInt();
				}
				buf1 = buf.SplitBy(_T(","));
				this->MinPosition.x = buf1.ToInt();
				buf1 = buf.SplitBy(_T(","));
				this->MinPosition.y = buf1.ToInt();
			}
			//for design,set location and size
			if((nDFlag & CC_DESIGN_DS_RECT) != 0)
			{
				buf = keyArr.Get(sThisHead + _T("._DRect"));
				buf1 = buf.SplitBy(_T(","));
				cc::Str buf2 = buf.SplitBy(_T(","));
				if(buf1.GetLength() > 0 && buf2.GetLength() > 0)
				{
					SetLocation(buf1.ToInt(), buf2.ToInt());
				}
				buf1 = buf.SplitBy(_T(","));
				buf2 = buf.SplitBy(_T(","));
				if(buf1.GetLength() > 0 && buf2.GetLength() > 0)
				{
					this->SetOuterStyle(OuterStyle::AutoSize, false);
					SetSize(buf1.ToInt(), buf2.ToInt());
				}
			}
			//notice if set rect then do nothing to autosize
			else if((nDFlag & CC_DESIGN_DS_ASIZE) != 0)
			{
				this->SetOuterStyle(OuterStyle::AutoSize, ((nDFlag & CC_DESIGN_DS_ASIZEON) != 0));
			}
			if((nDFlag & CC_DESIGN_DS_COLOR) != 0)
			{
				buf = keyArr.Get(sThisHead + _T("._DColor"));
				buf1 = buf.SplitBy(_T(","));
				if(buf1.GetLength() > 0)
				{
					this->SetTextColor(buf1.ToInt());
				}
				buf1 = buf.SplitBy(_T(","));
				if(buf1.GetLength() > 0)
				{
					this->SetHotTextColor(buf1.ToInt());
				}
				buf1 = buf.SplitBy(_T(","));
				if(buf1.GetLength() > 0)
				{
					int nHatch = keyArr.Get(sThisHead + _T("._DCross"), (DWORD)0);
					this->SetBackground(buf1.ToInt(), nHatch);
				}
				buf1 = buf.SplitBy(_T(","));
				if(buf1.GetLength() > 0)
				{
					this->SetHotBackColor(buf1.ToInt());
				}
			}
			if((nDFlag & CC_DESIGN_DS_ENABLE) != 0)
			{
				if(this->GetEnabled() != ((nDFlag & CC_DESIGN_DS_ENABLEON) != 0))
				{
					this->SetEnabled((nDFlag & CC_DESIGN_DS_ENABLEON) != 0);
				}
			}
			if((nDFlag & CC_DESIGN_DS_VISIBLE) != 0)
			{
				if(this->GetVisible() != ((nDFlag & CC_DESIGN_DS_VISIBLEON) != 0))
				{
					this->SetVisible((nDFlag & CC_DESIGN_DS_VISIBLEON) != 0);
				}
			}
			if((nDFlag & CC_DESIGN_DS_BACK) != 0)
			{
				//#define CC_DESIGN_DS_BACK1 0x0200 //Color Back
				//#define CC_DESIGN_DS_BACK2 0x0400 //Gradient Back(Left-Right)
				//#define CC_DESIGN_DS_BACK3 0x0800 //Gradient Back(Up-Down)
				//#define CC_DESIGN_DS_BACK4 0x1000 //Image Back
				if((nDFlag & (CC_DESIGN_DS_BACK1|CC_DESIGN_DS_BACK2|CC_DESIGN_DS_BACK3|CC_DESIGN_DS_BACK4)) == 0)
				{
					this->SetOuterStyle(OuterStyle::TransparentTop, false);
				}
				else if((nDFlag & CC_DESIGN_DS_BACK1) != 0)
				{
					this->SetOuterStyle(OuterStyle::TransparentTop, true);
					this->SetStyle(StyleType::Inner,
						InnerStyle::BackgroundBrush|InnerStyle::BackgroundGradient|InnerStyle::BackgroundBmp|InnerStyle::BackgroundTheme,
						InnerStyle::BackgroundColor);
				}
				else if((nDFlag & CC_DESIGN_DS_BACK2) != 0 || (nDFlag & CC_DESIGN_DS_BACK3) != 0)
				{
					this->SetOuterStyle(OuterStyle::TransparentTop, true);
					this->SetStyleBool(StyleType::Inner2, Inner2Style::GradientVertical, ((nDFlag & CC_DESIGN_DS_BACK3) != 0));
					this->SetStyle(StyleType::Inner,
						InnerStyle::BackgroundBrush|InnerStyle::BackgroundColor|InnerStyle::BackgroundBmp|InnerStyle::BackgroundTheme,
						InnerStyle::BackgroundGradient);
				}
				else if((nDFlag & CC_DESIGN_DS_BACK4) != 0)
				{
					buf = keyArr.Get(sThisHead + _T("._DFlag_Img"));
					//if(cc::Util::fExist(buf))
					if(buf.GetLength() > 0)
					{
						//may load image from stream like uuInstall
						if(this->SetBackgroundImage(buf, true))
						{
							this->SetOuterStyle(OuterStyle::TransparentTop, true);
						}
					}
				}
				if(m_hWnd != NULL)
				{
					::SendMessage(this->m_hWnd, CC_WM_MAINMSG, CC_WM_MAINMSG_WPARAM_CLEARBRUSHALL, 0);
				}
			}
			if((nDFlag & CC_DESIGN_DS_CTLS) != 0)
			{
				DWORD nCount = keyArr.Get(sThisHead + _T("._DCtlCount"), (DWORD)0);
				for(int n = nCount; n >= 1; n--)
				{
					buf.Format(_T("%s._DCtl%d.Name"), (TCHAR*)sThisHead.GetBuf(), n);
					cc::Str sName = keyArr.Get(buf);
					//if created,need do nothing
					if(this->GetControl(sName) != NULL)
					{
						break;
					}

					buf.Format(_T("%s._DCtl%d.Type"), (TCHAR*)sThisHead.GetBuf(), n);
					buf1 = keyArr.Get(buf);
					Control* control = CC_APP_CreateControl(buf1);
					if(control != NULL)
					{
						control->SetName(sName);

						buf.Format(_T("%s._DCtl%d.CfgName"), (TCHAR*)sThisHead.GetBuf(), n);
						//here  not treate _T("") as no name
						if(keyArr.Exist(buf))
						{
							buf1 = keyArr.Get(buf);
							if(buf1.GetLength() > 0)
							{
								control->SetConfigName(buf1);
							}
						}
#if defined(_DEBUG_) || defined(_DESIGN)
						control->Design = (DesignStyle::Type)((DWORD)control->Design | DesignStyle::DynamicCreated);
#endif
						control->SetText(_T("Dynamic Control"));
						this->AddControl(control);
						//control->SetLang(keyArr, sThisHead);
					}
				}
			}
		}
#endif
	}
	if(_lstControls != NULL)
	{
		for(int n = _lstControls->GetSize() - 1; n >= 0; n--)
		{
			_lstControls->GetKey(n)->SetLang(keyArr, sThisHead);
		}
	}
}

void Control::InvalidateBackGround()
{
	if(m_hWnd != NULL)
	{
		GetBackgroundBrush(true);
		::InvalidateRect(m_hWnd, NULL, TRUE);
	}
}

//need set InnerStyle::TrackMouse
bool Control::GetIsMouseOn()
{
	return ((_StyleArray[(int)StyleType::Inner2] & Inner2Style::IsMouseEnter) != 0);
}

//int nScrollCode = (short)LOWORD(wParam);
//int nPos = (short)HIWORD(wParam);
void Control::OnScroll(ScrollEventArgs *e)
{
	Scroll.Invoke((Object*)this, e);
	if(HasStyle(StyleType::Inner2, Inner2Style::IsScrollRepaint))
	{
		if(HasStyle(StyleType::Outer, OuterStyle::TransparentParentBrush|OuterStyle::TransparentParentErase|OuterStyle::TransparentHollowBrush))
		{
			if(m_hWnd != NULL)
			{
				InvalidateRect(m_hWnd, NULL, TRUE);
			}
			::KillTimer(m_hWnd, CC_TIMERID_INVALIDATERECT);
			::SetTimer(m_hWnd, CC_TIMERID_INVALIDATERECT, 40, NULL);
		}
	}
}

//The low-order word indicates whether various virtual keys are down. This parameter can be one or more of the following values. 
//fwKeys = GET_KEYSTATE_WPARAM(wParam);
//zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
//xPos = GET_X_LPARAM(lParam); 
//yPos = GET_Y_LPARAM(lParam); 
void Control::OnMouseWheel(EventArgs *e)
{
	if(HasStyle(StyleType::Inner2, Inner2Style::IsScrollRepaint))
	{
		if(HasStyle(StyleType::Outer, OuterStyle::TransparentParentBrush|OuterStyle::TransparentParentErase|OuterStyle::TransparentHollowBrush))
		{
			if(m_hWnd != NULL)
			{
				InvalidateRect(m_hWnd, NULL, TRUE);
			}
			::KillTimer(m_hWnd, CC_TIMERID_INVALIDATERECT);
			::SetTimer(m_hWnd, CC_TIMERID_INVALIDATERECT, 40, NULL);
		}
	}
	MouseWheel.Invoke((Object*)this, e);
}

void Control::ReflectLastMsg(HWND hWndChild, EventArgs *e)
{
	//get the map, and if no map, then this message does not need reflection
	Control* control = Control::FromHandle(hWndChild);
	if(control != NULL)
	{
		control->SendChildNotifyLastMsg(e);
	}
#ifdef _DEBUG_
	else
	{
			UINT nID = LOWORD(e->WParam);
			int nCode = HIWORD(e->WParam);
			CC_TRACE((_T("Control::ReflectLastMsg(), hWndChild:0x%08x, e->Msg:0x%04X"), hWndChild, e->Msg));
	}
#endif
}

void Control::SendChildNotifyLastMsg(EventArgs *e)
{
	//call overed fun
	OnChildNotify(e);
}

void Control::OnChildNotify(EventArgs *e)
{
	//donothing
}

//void Control::OnNcActivate(EventArgs *e)
//{
//}

//////////////////
// Normally,	a static control does not get mouse events unless it has
// SS_NOTIFY. This achieves the same effect as SS_NOTIFY, but it's fewer
// lines of code and more reliable than turning on SS_NOTIFY in OnCtlColor
// because Windows doesn't send WM_CTLCOLOR to bitmap static controls.
//
//UINT Label::OnNcHitTest(EventArgs *e)
//{
//	return HTCLIENT;
//}
void Control::OnNcHittest(EventArgs *e)
{
//#if defined(_DEBUG_) || defined(_DESIGN)
//	if(e->Msg == WM_PAINT && ((DWORD)Design & DesignStyle::Selected) != 0)
//	{
//		RECT rect;
//		::GetWindowRect(this->m_hWnd, &rect);
//		HDC hDC = ::GetWindowDC(this->m_hWnd);
//		CC_APP_GetDrawActor()->InvertBorder(0, 0, rect.right - rect.left, rect.bottom - rect.top, hDC, 3 * GetSystemMetrics(SM_CXBORDER));
//		::ReleaseDC(this->m_hWnd, hDC);
//	}
//#endif
#if defined(_DEBUG_) || defined(_DESIGN)
	if(!CC_APP_GetDesignBarActive())
	{
		return;
	}
	static Control* pLastCtl = NULL;
	bool isLbtn = ((GetKeyState(VK_LBUTTON) & 0x80) == 0x80);
	bool isRbtn = ((GetKeyState(VK_RBUTTON) & 0x80) == 0x80);
	if(isLbtn || isRbtn)
	{
		if(pLastCtl != this)
		{
			e->Handled = true;
			return;
		}
	}
	else
	{
		pLastCtl = this;
	}
	bool isKey = ((GetKeyState(VK_CONTROL) & 0x8000) != 0); // && (GetKeyState(VK_SHIFT) & 0x8000) != 0
	if((CC_APP_GetConfig()->GetIsDesign() && !isKey) || (!CC_APP_GetConfig()->GetIsDesign() && isKey))
	{
		HWND hWnd = CC_APP_GetDesignBarHwnd();
		if(hWnd == NULL || hWnd == m_hWnd || ::IsChild(hWnd, m_hWnd))
		{
			return;
		}
		Control* ctlClick = _pParent;
		while(ctlClick != NULL)
		{
			if(hWnd == ctlClick->m_hWnd || ::IsChild(hWnd, ctlClick->m_hWnd))
			{
				return;
			}
			ctlClick = ctlClick->_pParent;
		}
		POINT pt;
		::GetCursorPos(&pt);
		RECT rect;
		ctlClick = this;
		if(isLbtn || isRbtn)
		{
			//can get disabled window?
			if(_lstControls != NULL)
			{
				for(int n = _lstControls->GetSize() - 1; n >= 0; n--)
				{
					Control* ctl = _lstControls->GetKey(n);
					::GetWindowRect(ctl->m_hWnd, &rect);
					RECT rect2 = ctl->GetBounds();
					if(ctl->GetVisible() && ::PtInRect(&rect, pt))
					{
						ctlClick = ctl;
						break;
					}
				}
			}

			CC_Design_Hotcontrol_new = ctlClick;
			::KillTimer(hWnd, CC_TIMERID_DESIGN_SETHOT);
			::SetTimer(hWnd, CC_TIMERID_DESIGN_SETHOT, 50, NULL);
			//::SendMessage(hWnd, CC_WM_MAINMSG, CC_WM_MAINMSG_WPARAM_DS_SETHOT, (LPARAM)ctlClick);
			//Control* hCtlForm = (Control*)::SendMessage(hWnd, CC_WM_MAINMSG, CC_WM_MAINMSG_WPARAM_GETFORM, (LPARAM)NULL);
			//if(hCtlForm != NULL)
			//{
			//	hCtlForm->Focus();
			//}
			return;
		}

		//show mouse point at design's title
		int nPosX = pt.x; 
		int nPosY = pt.y;
		::GetWindowRect(ctlClick->m_hWnd, &rect);
		if(ctlClick->_pParent != NULL)
		{
			::ScreenToClient(ctlClick->_pParent->m_hWnd, (LPPOINT)&rect);
			::ScreenToClient(ctlClick->_pParent->m_hWnd, ((LPPOINT)&rect)+1);
			::ScreenToClient(ctlClick->_pParent->m_hWnd, (LPPOINT)&pt);
		}
		cc::Str sDebug;
		sDebug.Format(_T("X:%d, Y:%d, [%s]x:%d, y:%d, L:%d, T:%d, W:%d, H:%d"),
			nPosX, nPosY, ctlClick->_Name, pt.x, pt.y,
			rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);
		::SendMessage(hWnd, CC_WM_MAINMSG, (WPARAM)CC_WM_MAINMSG_WPARAM_TEXTTIPSONLY, (LPARAM)(const TCHAR*)sDebug.GetBuf());

		e->Handled = true;
		Control* hCtlHot = (Control*)::SendMessage(hWnd, CC_WM_MAINMSG, CC_WM_MAINMSG_WPARAM_DS_GETHOT, (LPARAM)NULL);
		//Control* hCtlHot = CC_Design_Hotcontrol;
		if(hCtlHot == NULL || hCtlHot->m_hWnd != ctlClick->m_hWnd || ((DWORD)Design & CC_DESIGN_NO_MOVE) == CC_DESIGN_NO_MOVE || ((DWORD)ctlClick->Design & DesignStyle::NoDesign) != 0)
		{
			//if self is not selected,do nothing
			return;
		}

		::GetWindowRect(ctlClick->m_hWnd, &rect);
		//ctlClick = (Control*)::SendMessage(hWnd, CC_WM_MAINMSG, CC_WM_MAINMSG_WPARAM_DS_GETCONTAINER, (LPARAM)ctlClick);
		if(nPosX >= rect.right - 2)
		{
			if(nPosY <= rect.top + 2)
			{
				if(((DWORD)Design & (DesignStyle::NoMoveTop|DesignStyle::NoMoveRight)) == 0)
				{
					e->Result = HTTOPRIGHT;
				}
			}
			else if(nPosY >= rect.bottom - 2)
			{
				if(((DWORD)Design & (DesignStyle::NoMoveBottom|DesignStyle::NoMoveRight)) == 0)
				{
					e->Result = HTBOTTOMRIGHT;
				}
			}
			else
			{
				if(((DWORD)Design & DesignStyle::NoMoveRight) == 0)
				{
					e->Result = HTRIGHT;
				}
			}
		}
		else if(nPosX <= rect.left + 2)
		{
			if(nPosY <= rect.top + 2)
			{
				if(((DWORD)Design & (DesignStyle::NoMoveTop|DesignStyle::NoMoveLeft)) == 0)
				{
					e->Result = HTTOPLEFT;
				}
			}
			else if(nPosY >= rect.bottom - 2)
			{
				if(((DWORD)Design & (DesignStyle::NoMoveBottom|DesignStyle::NoMoveLeft)) == 0)
				{
					e->Result = HTBOTTOMLEFT;
				}
			}
			else
			{
				if(((DWORD)Design & DesignStyle::NoMoveLeft) == 0)
				{
					e->Result = HTLEFT;
				}
			}
		}
		else if(nPosY <= rect.top + 2)
		{
			if(((DWORD)Design & DesignStyle::NoMoveTop) == 0)
			{
				e->Result = HTTOP;
			}
		}
		else if(nPosY >= rect.bottom - 2)
		{
			if(((DWORD)Design & DesignStyle::NoMoveBottom) == 0)
			{
				e->Result = HTBOTTOM;
			}
		}
		else
		{
			if(((DWORD)Design & (DesignStyle::NoMove)) == 0)
			{
				e->Result = HTCAPTION;
			}
		}
		return;
	}
#endif
}

void Control::OnDropFiles(EventArgs* e)
{
	DropFiles.Invoke((Object*)this, e);
}

/*
/////////////////////////////////////////////////////////////////////////////
// Scroll bar helpers
//  hook for CWnd functions
//    only works for derived class (eg: CView) that override 'GetScrollBarCtrl'
// if the window doesn't have a _visible_ windows scrollbar - then
//   look for a sibling with the appropriate ID
ScrollBar* Control::GetScrollBarCtrl(int) const
{
	return NULL;        // no special scrollers supported
}

int Control::SetScrollPos(int nBar, int nPos, bool bRedraw)
{
	ScrollBar* pScrollBar;
	if((pScrollBar = GetScrollBarCtrl(nBar)) != NULL)
	{
		return pScrollBar->SetValue(nPos, bRedraw);
	}
	else
	{
		return ::SetScrollPos(m_hWnd, nBar, nPos, bRedraw ? TRUE : FALSE);
	}
}

int Control::GetScrollPos(int nBar) const
{
	ScrollBar* pScrollBar;
	if((pScrollBar = GetScrollBarCtrl(nBar)) != NULL)
	{
		return pScrollBar->GetValue();
	}
	else
	{
		return ::GetScrollPos(m_hWnd, nBar);
	}
}

void Control::SetScrollRange(int nBar, int nMinPos, int nMaxPos, bool bRedraw)
{
	ScrollBar* pScrollBar;
	if((pScrollBar = GetScrollBarCtrl(nBar)) != NULL)
	{
		pScrollBar->SetScrollRange(nMinPos, nMaxPos, bRedraw);
	}
	else
	{
		::SetScrollRange(m_hWnd, nBar, nMinPos, nMaxPos, bRedraw ? TRUE : FALSE);
	}
}

void Control::GetScrollRange(int nBar, LPINT lpMinPos, LPINT lpMaxPos) const
{
	ScrollBar* pScrollBar;
	if ((pScrollBar = GetScrollBarCtrl(nBar)) != NULL)
	{
		pScrollBar->GetScrollRange(lpMinPos, lpMaxPos);
	}
	else
	{
		::GetScrollRange(m_hWnd, nBar, lpMinPos, lpMaxPos);
	}
}

// Turn on/off non-control scrollbars
//   for WS_?SCROLL scrollbars - show/hide them
//   for control scrollbar - enable/disable them
void Control::EnableScrollBarCtrl(int nBar, bool bEnable)
{
	CC_ASSERT(::IsWindow(m_hWnd));

	ScrollBar* pScrollBar;
	if(nBar == SB_BOTH)
	{
		this->EnableScrollBarCtrl(SB_HORZ, bEnable);
		this->EnableScrollBarCtrl(SB_VERT, bEnable);
	}
	else if((pScrollBar = GetScrollBarCtrl(nBar)) != NULL)
	{
		// control scrollbar - enable or disable
		pScrollBar->SetEnabled(bEnable);
	}
	else
	{
		// WS_?SCROLL scrollbar - show or hide
		::ShowScrollBar(m_hWnd, nBar, bEnable ? TRUE : FALSE);
	}
}

bool Control::SetScrollInfo(int nBar, LPSCROLLINFO lpScrollInfo, bool bRedraw)
{
	CC_ASSERT(::IsWindow(m_hWnd));
	CC_ASSERT(lpScrollInfo != NULL);

	HWND hWnd = m_hWnd;
	ScrollBar* pScrollBar;
	if(nBar != SB_CTL && (pScrollBar = GetScrollBarCtrl(nBar)) != NULL)
	{
		hWnd = pScrollBar->m_hWnd;
		nBar = SB_CTL;
	}
	lpScrollInfo->cbSize = sizeof(*lpScrollInfo);
	::SetScrollInfo(hWnd, nBar, lpScrollInfo, bRedraw ? TRUE : FALSE);
	return true;
}

bool Control::GetScrollInfo(int nBar, LPSCROLLINFO lpScrollInfo, UINT nMask)
{
	CC_ASSERT(::IsWindow(m_hWnd));
	CC_ASSERT(lpScrollInfo != NULL);

	HWND hWnd = m_hWnd;
	ScrollBar* pScrollBar;
	if(nBar != SB_CTL && (pScrollBar = GetScrollBarCtrl(nBar)) != NULL)
	{
		hWnd = pScrollBar->m_hWnd;
		nBar = SB_CTL;
	}
	lpScrollInfo->cbSize = sizeof(*lpScrollInfo);
	lpScrollInfo->fMask = nMask;
	return ((BOOL)::GetScrollInfo(hWnd, nBar, lpScrollInfo) == TRUE);
}

int Control::GetScrollLimit(int nBar)
{
	int nMin, nMax;
	this->GetScrollRange(nBar, &nMin, &nMax);
	SCROLLINFO info;
	if(this->GetScrollInfo(nBar, &info, SIF_PAGE))
	{
		nMax -= __max(info.nPage-1, 0);
	}
	return nMax;
}

void Control::ScrollWindow(int xAmount, int yAmount, LPCRECT lpRect, LPCRECT lpClipRect)
{
	CC_ASSERT(::IsWindow(m_hWnd));

	if(GetVisible() || lpRect != NULL || lpClipRect != NULL)
	{
		// When visible, let Windows do the scrolling
		::ScrollWindow(m_hWnd, xAmount, yAmount, lpRect, lpClipRect);
	}
	else
	{
		// Windows does not perform any scrolling if the window is
		// not visible.  This leaves child windows unscrolled.
		// To account for this oversight, the child windows are moved
		// directly instead.
		HWND hWndChild = ::GetWindow(m_hWnd, GW_CHILD);
		if(hWndChild != NULL)
		{
			for(; hWndChild != NULL; hWndChild = ::GetNextWindow(hWndChild, GW_HWNDNEXT))
			{
				RECT rect;
				::GetWindowRect(hWndChild, &rect);
				::ScreenToClient(hWndChild, (LPPOINT)&rect);
				::ScreenToClient(hWndChild, ((LPPOINT)&rect)+1);
				::SetWindowPos(hWndChild, NULL, rect.left+xAmount, rect.top+yAmount, 0, 0,
					SWP_NOSIZE|SWP_NOACTIVATE|SWP_NOZORDER);
			}
		}
	}
}
*/

bool Control::SubclassWindow(HWND hWnd, Control* parent)
{
	CC_ASSERT(m_hWnd == NULL);
	CC_ASSERT(::IsWindow(hWnd));

	// and Detach the HWND from the CC_APP_GetMapHWND
	int nIndex = CC_APP_GetMapHWND()->GetIndex(hWnd);
	if(nIndex >= 0)
	{
		CC_ASSERT(FALSE && _T("Trying to use SubclassWindow with incorrect hWnd"));
		return false;
	}

	m_hWnd = hWnd;
	_pParent = (parent == NULL ? GetParentReal() : parent);
	_StyleArray[(int)StyleType::Inner2] |= Inner2Style::HandleCreated|Inner2Style::HandleCreatedEnd|Inner2Style::SubclassWindow;
	CC_APP_GetMapHWND()->Add(hWnd, this);

	// now hook into the standard WndProc
	WNDPROC* lplpfn = GetSuperWndProcAddr();
	CC_ASSERT(*lplpfn == NULL);

	WNDPROC wndProc = StaticWindowProc;
#pragma warning(disable:4244 4312)
	WNDPROC oldWndProc = (WNDPROC)::SetWindowLongPtr(hWnd, GWLP_WNDPROC, (INT_PTR)wndProc);
#pragma warning(default:4244 4312)
	CC_ASSERT(oldWndProc != wndProc);

	*lplpfn = oldWndProc;   // the first control of that type created
	return true;
}

HWND Control::UnsubclassWindow()
{
	CC_ASSERT(::IsWindow(m_hWnd));

	// set WNDPROC back to original value
	WNDPROC* lplpfn = GetSuperWndProcAddr();
#pragma warning(disable:4244 4312)
	SetWindowLongPtr(m_hWnd, GWLP_WNDPROC, (INT_PTR)*lplpfn);
#pragma warning(default:4244 4312)
	*lplpfn = NULL;

	// and Detach the HWND from the CC_APP_GetMapHWND
	CC_APP_GetMapHWND()->Remove(m_hWnd);
	HWND hWnd = m_hWnd;
	m_hWnd = NULL;

	_StyleArray[(int)StyleType::Inner2] &= ~(Inner2Style::HandleCreated|Inner2Style::HandleCreatedEnd|Inner2Style::SubclassWindow);
	_StyleArray[(int)StyleType::Inner2] |= Inner2Style::HandleDestroyed;
	return hWnd;
}
/////////////////////////////////////////////////////////////////////////////

DWORD ToolTipConfig::nStyle = WS_POPUP;
DWORD ToolTipConfig::nExStyle = WS_EX_TOOLWINDOW | WS_EX_TOPMOST;
//DWORD ToolTipConfig::nClassStyle = CS_DBLCLKS | CS_GLOBALCLASS;
DWORD ToolTipConfig::nInnerStyle = 0;
DWORD ToolTipConfig::nOuterStyle = OuterStyle::AutoSize;
DWORD ToolTipConfig::nOwnerDrawTextStyle = DT_WORDBREAK|DT_EDITCONTROL;
//COLORREF ToolTipConfig::corBackground = ::GetSysColor(COLOR_WINDOW);
COLORREF ToolTipConfig::corBackground = ::GetSysColor(COLOR_INFOBK);
COLORREF ToolTipConfig::corTextground = ::GetSysColor(COLOR_BTNTEXT);
COLORREF ToolTipConfig::corHotBackColor = ::GetSysColor(COLOR_INFOBK);
COLORREF ToolTipConfig::corHotTextColor = ::GetSysColor(COLOR_BTNTEXT);
int ToolTipConfig::nMarginLeft = 6;
int ToolTipConfig::nMarginTop = 6;
int ToolTipConfig::nMarginRight = 6;
int ToolTipConfig::nMarginBottom = 6;
//should ControlConfig::nWaitTimerCheckMouseLeave*5 < CC_TOOLTIP_WAIT_TO_HIDE and ControlConfig::nWaitTimerCheckMouseLeave*5 < CC_STATUSBAR_WAIT_TO_DEFAULTTEXT
//#define CC_TOOLTIP_WAIT_TO_HIDE        5000
//#define CC_TOOLTIP_WIN_WIDTH        300
int ToolTipConfig::nWaitHideMilliSecond = 5000;
int ToolTipConfig::nWindowWidth = 300;
//===========================================================cc::win::ToolTip
///////////////////////////////////////////////////////////////////////////////
// constructors/destructor
ToolTip::ToolTip()
{
	BASE_CONTROL_CONFIG_INIT(ToolTip);
	//_SetWindowPosFlags = SWP_NOACTIVATE;
	SetStyle(StyleType::Inner2, 0, Inner2Style::WindowPosFlagsNoAct);
	::SetRect(&_Margin, ToolTipConfig::nMarginLeft, ToolTipConfig::nMarginTop, ToolTipConfig::nMarginRight, ToolTipConfig::nMarginBottom);
	::SetRect(&_Rect, -1000, -1000, -1000 + 200, -1000 + 21);
	::CopyRect(&_OrgRect, &_Rect);
	_minSize.cx = 64;
	_ClassName = WndClassName::ToolTip;
	//_StyleArray[(int)StyleType::InitControl] = 0;
#ifdef _DEBUG_
	SetName(_T("ToolTip"));
#endif
	//hide it first
	this->SetVisible(false);

	CC_BIT_ON(_Flags, CC_FLAG_TOOLTIP_ISACTIVE);
	_nAutomaticDelay = ToolTipConfig::nWaitHideMilliSecond/10;
	_nAutopopDelay = ToolTipConfig::nWaitHideMilliSecond;
	_nInitialDelay = _nAutomaticDelay;
	_nReshowDelay = _nAutomaticDelay / 5;
	_eStatus = TipsStatus::Wait;
	_ptLastPosition.x = 0;
	_ptLastPosition.y = 0;
	//while show menu's tips,then need bringtop
	_nBringToTopCnt = 99;
	nOldDuration = -1;
	nOldWidth = -1;
	nOldHeight = -1;
}

ToolTip::~ToolTip()
{
}
// constructors/destructor

void ToolTip::OnResize(EventArgs* e)
{
	InvalidateBackGround();
}

void ToolTip::DoUpdate()
{
}

void ToolTip::OnPaint(PaintEventArgs* e)
{
	HDC hDC = BeginPaint(m_hWnd, &(e->data));
	RECT rcItem;
	::CopyRect(&rcItem, &rectBody);
	//::GetClientRect(m_hWnd, &rcItem);
	rcItem.left += _Margin.left;
	rcItem.top += _Margin.top;
	rcItem.right -= _Margin.right;
	rcItem.bottom -= _Margin.bottom;
	HFONT hOldFont = (HFONT)::SelectObject(hDC, this->GetFont());

	PaintBegin.Invoke((Object*)this, e);
	EventArgs ee(m_hWnd, WM_CTLCOLORSTATIC, (WPARAM)hDC, (LPARAM)m_hWnd);
	OnCtlColorEdit(&ee);
	HBRUSH brush = (HBRUSH)ee.Result;
	::SetTextColor(hDC, this->GetTextColor());
	::SetBkMode(hDC, TRANSPARENT);
	HRGN hRgnBody = ::CreateRectRgn(0, 0, 0, 0);
	GetWindowRgn(m_hWnd, hRgnBody);
	::FillRgn(hDC, hRgnBody, brush);

	//CC_APP_GetDrawActor()->DrawControl(this, &ee);
	if(_pImageList != NULL && _nIconID > 0)
	{
		HICON hIcon = _pImageList->GetIcon(_nIconID);
		DrawIconEx(hDC, rcItem.left, rcItem.top - 1, hIcon, 16, 16, 0, brush, DI_NORMAL);
		rcItem.left += 18;
	}
	::DrawText(hDC, _Text, -1, &rcItem, DT_WORDBREAK|DT_EDITCONTROL);
	::FrameRgn(hDC, hRgnBody, (HBRUSH)GetStockObject(DKGRAY_BRUSH), 1, 1);
	::DeleteObject(hRgnBody);
	PaintEnd.Invoke((Object*)this, e);

	::SelectObject(hDC, hOldFont);
	EndPaint(m_hWnd, &(e->data));
}

void ToolTip::OnPopup(EventArgs* e)
{
	Popup.Invoke((Object*)this, e);
}

void ToolTip::OnInnerTimer(EventArgs* e)
{
	//e->WParam is ID
	if(e->WParam != CC_TIMERID_CONTROL_TIMEOUT)
	{
		Control::OnInnerTimer(e);
		return;
	}
	if(_eStatus == TipsStatus::WaitShowFirst)
	{
		StopTimer();
		OnPopup(e);

		_eStatus = TipsStatus::Showing;
		//_Visible = true;
		//CC_BIT_ON(_Flags, CC_FLAG_CONTROL_VISIBLE);
		SetStyle(StyleType::Inner2, 0, Inner2Style::Visible);
		int nW = _Rect.right - _Rect.left;
		int nH = _Rect.bottom - _Rect.top;
		CalcutePosition(_ptLastPosition.x, _ptLastPosition.y, nW, nH);

		//update timer to wait for hide it
		::SetTimer(m_hWnd, CC_TIMERID_CONTROL_TIMEOUT, _nAutopopDelay, NULL);
	}
	else if(_eStatus == TipsStatus::WaitShowNext)
	{
		StopTimer();
		_eStatus = TipsStatus::Wait;
	}
	else if(_eStatus == TipsStatus::Showing)
	{
		StopTimer();
		//hide it
		this->SetVisible(false);
		_eStatus = TipsStatus::WaitShowNext;
		::SetTimer(m_hWnd, CC_TIMERID_CONTROL_TIMEOUT, _nReshowDelay, NULL);
	}
}

void ToolTip::StopTimer()
{
	::KillTimer(m_hWnd, CC_TIMERID_CONTROL_TIMEOUT);
}

bool ToolTip::KeepShow(bool isKeepPosition)
{
	if(_eStatus == TipsStatus::Showing)
	{
		StopTimer();
		if(!isKeepPosition)
		{
			POINT pt;
			::GetCursorPos(&pt);
			if(pt.x < _ptLastPosition.x - 10 || pt.x > _ptLastPosition.x + 10 || pt.y < _ptLastPosition.y - 10 || pt.y > _ptLastPosition.y + 10)
			{
				int nW = _Rect.right - _Rect.left;
				int nH = _Rect.bottom - _Rect.top;
				CalcutePosition(pt.x, pt.y, nW, nH);
			}
			else
			{
				if(_nBringToTopCnt < 2)
				{
					//modify at 2007.10.15, change 14 to 2
					_nBringToTopCnt++;
					//SetWindowPos(m_hWnd, NULL, 0, 0, 0, 0, SWP_HIDEWINDOW|SWP_NOACTIVATE|SWP_NOMOVE|SWP_NOSIZE);
					SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_SHOWWINDOW|SWP_NOACTIVATE|SWP_NOMOVE|SWP_NOSIZE|SWP_FRAMECHANGED);
					if(m_hWnd != NULL)
					{
						::InvalidateRect(m_hWnd, NULL, TRUE);
					}
				}
			}
		}
		::SetTimer(m_hWnd, CC_TIMERID_CONTROL_TIMEOUT, _nAutopopDelay, NULL);
		return true;
	}
	else if(_eStatus == TipsStatus::WaitShowFirst)
	{
		if(!isKeepPosition)
		{
			::GetCursorPos(&_ptLastPosition);
		}
		return true;
	}
	return false;
}

void ToolTip::CalcutePosition(int x, int y, int nW, int nH)
{
	nW = CC_MAX(nW, _minSize.cx);
	nH = CC_MAX(nH, _minSize.cy);
	_ptLastPosition.x = x;
	_ptLastPosition.y = y;
	RECT rect = {0, 0, 0, 0};
	if(!::SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, 0))
	{
		rect.right = (LONG)::GetSystemMetrics(SM_CXSCREEN);
		rect.bottom = (LONG)::GetSystemMetrics(SM_CYSCREEN);
	}

	int nDuration = -1;
	HRGN hrgnRect = NULL;
	HRGN hrgnAnchor = NULL;
	POINT ptAnchor[3];
	if(x + 32 + nW > rect.right)
	{
		x -= nW - 4;
		ptAnchor[0].x = nW - 40;
		ptAnchor[1].x = nW - 16;
		ptAnchor[2].x = nW - 10;
		if(y + 40 + nH > rect.bottom)
		{
			y -= 16 + nH;
			ptAnchor[0].y = nH - 1;
			ptAnchor[1].y = nH - 1;
			ptAnchor[2].y = nH + 16;
			::SetRect(&rectBody, 0, 0, nW, nH);
			nDuration = 1;
		}
		else
		{
			y += 16;
			ptAnchor[0].y = 16;
			ptAnchor[1].y = 16;
			ptAnchor[2].y = 0;
			::SetRect(&rectBody, 0, 16 - 1, nW, 16 - 1 + nH);
			nDuration = 2;
		}
	}
	else
	{
		x += 8;
		ptAnchor[0].x = 40;
		ptAnchor[1].x = 16;
		ptAnchor[2].x = 10;
		if(y + 40 + nH > rect.bottom)
		{
			y -= 16 + nH;
			ptAnchor[0].y = nH - 1;
			ptAnchor[1].y = nH - 1;
			ptAnchor[2].y = nH + 16;
			::SetRect(&rectBody, 0, 0, nW, nH);
			nDuration = 3;
		}
		else
		{
			y += 16;
			ptAnchor[0].y = 16;
			ptAnchor[1].y = 16;
			ptAnchor[2].y = 0;
			::SetRect(&rectBody, 0, 16 - 1, nW, 16 - 1 + nH);
			nDuration = 4;
		}
	}

	_Rect.left = x;
	_Rect.top = y;
	_Rect.right = x + nW;
	_Rect.bottom = y + nH;

	if(_eStatus == TipsStatus::Showing)
	{
		StopTimer();

		if(nDuration != nOldDuration || nW != nOldWidth || nH != nOldHeight)
		{
			hrgnRect = ::CreateRoundRectRgn(rectBody.left, rectBody.top, rectBody.right, rectBody.bottom, 4, 4);
			hrgnAnchor = ::CreatePolygonRgn(ptAnchor, 3, ALTERNATE);
			HRGN hRgnBody = ::CreateRectRgn(0, 0, 0, 0);
			::CombineRgn(hRgnBody, hrgnRect, hrgnAnchor, RGN_OR);
			//After a successful call to SetWindowRgn, the system owns the region specified by the region handle hRgn.
			//The system does not make a copy of the region. Thus, you should not make any further
			//function calls with this region handle. In particular, do not delete this region handle. 
			//The system deletes the region handle when it no longer needed.
			SetWindowRgn(m_hWnd, hRgnBody, TRUE);
			::DeleteObject(hrgnRect);
			::DeleteObject(hrgnAnchor);
		}
		SetWindowPos(m_hWnd, NULL, _Rect.left, _Rect.top, nW, nH + 32, SWP_NOACTIVATE|SWP_SHOWWINDOW/*|SWP_NOCOPYBITS*/);
		if(nDuration != nOldDuration || nW != nOldWidth || nH != nOldHeight)
		{
			nOldDuration = nDuration;
			nOldWidth = nW;
			nOldHeight = nH;
			if(m_hWnd != NULL)
			{
				::InvalidateRect(m_hWnd, NULL, TRUE);
			}
		}
	}
}

void ToolTip::Show(const TCHAR* sText, ImageList* imageList, int nIconID, int duration, bool isForMenu, bool isNoWait)
{
	POINT pt;
	::GetCursorPos(&pt);
	Show(sText, imageList, nIconID, pt.x, pt.y, duration, isForMenu, isNoWait);
}

void ToolTip::Show(const TCHAR* sText, ImageList* imageList, int nIconID, POINT pt, int duration, bool isForMenu, bool isNoWait)
{
	Show(sText, imageList, nIconID, pt.x, pt.y, duration, isForMenu, isNoWait);
}

void ToolTip::Show(const TCHAR* sText, ImageList* imageList, int nIconID, int x, int y, int duration, bool isForMenu, bool isNoWait)
{
	if(!GetActive())
	{
		return;
	}
	//if(cc::Util::GetTopMenuHwnd() != NULL)
	if(isForMenu)
	{
		//has menu popup,invoid under menu
		_nBringToTopCnt = 0;
	}
	_ptLastPosition.x = x;
	_ptLastPosition.y = y;
	bool isChgText = false;
	if(_Text != sText || (_Text != NULL && sText != NULL && _tcscmp(_Text, sText) != 0))
	{
		isChgText = true;
		SetText(sText);
	}

	if(HasStyle(StyleType::Outer, OuterStyle::AutoSize))
	{
		if((isChgText || _pImageList != imageList || _nIconID != nIconID) && (_Text != NULL || imageList != NULL))
		{
			if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) == 0)
			{
				CreateControl();
			}
			int nW = _Margin.left + _Margin.right;
			int nH = _Margin.top + _Margin.bottom;
			if(_Text != NULL)
			{
				HDC hDC = ::GetDC(this->m_hWnd);
				HFONT hOldFont = (HFONT)::SelectObject(hDC, this->GetFont());

				RECT rc = {0, 0, ToolTipConfig::nWindowWidth, 0};
				::DrawText(hDC, _Text, -1, &rc, DT_CALCRECT|DT_WORDBREAK|DT_EDITCONTROL);

				nW += rc.right;
				nH += rc.bottom;
				::GetClientRect(m_hWnd, &rc);
				::GetWindowRect(m_hWnd, &_Rect);
				nW += ((_Rect.right - _Rect.left) - (rc.right - rc.left));
				nH += ((_Rect.bottom - _Rect.top) - (rc.bottom - rc.top));

				::SelectObject(hDC, hOldFont);
				::ReleaseDC(this->m_hWnd, hDC);
			}
			if(imageList != NULL && nIconID > 0)
			{
				nW += 18;
				nH = max(nH, 16 + _Margin.top + _Margin.bottom);
			}
			CalcutePosition(x, y, nW, nH);
		}
	}
	if(imageList != NULL)
	{
		_pImageList = imageList;
	}
	_nIconID = nIconID;
	if(_Text == NULL && (_pImageList == NULL || _nIconID < 1))
	{
		if(_eStatus == TipsStatus::Showing)
		{
			StopTimer();
			this->SetVisible(false);
			_eStatus = TipsStatus::WaitShowNext;
			::SetTimer(m_hWnd, CC_TIMERID_CONTROL_TIMEOUT, _nReshowDelay, NULL);
		}
		else if(_eStatus == TipsStatus::WaitShowFirst)
		{
			StopTimer();
			_eStatus = TipsStatus::Wait;
		}
		return;
	}
	if(_eStatus == TipsStatus::Wait)
	{
		_eStatus = TipsStatus::WaitShowFirst;
		::SetTimer(m_hWnd, CC_TIMERID_CONTROL_TIMEOUT, _nInitialDelay, NULL);
	}
	else if(_eStatus == TipsStatus::WaitShowNext)
	{
		StopTimer();

		int nW = _Rect.right - _Rect.left;
		int nH = _Rect.bottom - _Rect.top;
		_Rect.left = x;
		_Rect.top = y;
		_Rect.right = x + nW;
		_Rect.bottom = y + nH;

		_eStatus = TipsStatus::WaitShowFirst;
		//show it immediately at OnTimer
		::SetTimer(m_hWnd, CC_TIMERID_CONTROL_TIMEOUT, 30, NULL);
	}
	else if(_eStatus == TipsStatus::Showing)
	{
		StopTimer();

		//restart
		::SetTimer(m_hWnd, CC_TIMERID_CONTROL_TIMEOUT, _nAutopopDelay, NULL);
	}
	if(isNoWait && !GetVisible()) {
		StopTimer();
		//show it immediately at OnTimer
		::SetTimer(m_hWnd, CC_TIMERID_CONTROL_TIMEOUT, 30, NULL);
	}
}

bool ToolTip::GetActive()
{
	return CC_BIT_HAS(_Flags, CC_FLAG_TOOLTIP_ISACTIVE);
}

/* if set to false, then do nothing while call Show or others */
void ToolTip::SetActive(bool active)
{
	CC_BIT_SET(_Flags, CC_FLAG_TOOLTIP_ISACTIVE, active);
	if(!active)
	{
		//hide it
		this->SetVisible(false);
	}
}

int ToolTip::GetAutomaticDelay()
{
	return _nAutomaticDelay;
}

void ToolTip::SetAutomaticDelay(int automaticDelay)
{
	_nAutomaticDelay = automaticDelay;
	_nAutopopDelay = _nAutomaticDelay * 10;
	_nInitialDelay = _nAutomaticDelay;
	_nReshowDelay = _nAutomaticDelay / 5;
}

void ToolTip::OnNcHittest(EventArgs *e)
{
	HWND hWnd = CC_APP_GetActiveFormHwnd();
	if(hWnd != NULL)
	{
		::SendMessage(hWnd, CC_WM_MAINMSG, CC_WM_MAINMSG_WPARAM_TEXTTIPSONLY, (LPARAM)NULL);
	}
}

int ToolTip::GetAutopopDelay()
{
	return _nAutopopDelay;
}

void ToolTip::SetAutopopDelay(int autopopDelay)
{
	_nAutopopDelay = autopopDelay;
}

int ToolTip::GetInitialDelay()
{
	return _nInitialDelay;
}

void ToolTip::SetInitialDelay(int initialDelay)
{
	_nInitialDelay = initialDelay;
}

int ToolTip::GetReshowDelay()
{
	return _nReshowDelay;
}

void ToolTip::SetReshowDelay(int reshowDelay)
{
	_nReshowDelay = reshowDelay;
}
//
//bool ToolTip::GetShowAlways()
//{
//	return CC_BIT_HAS(_Flags, CC_FLAG_TOOLTIP_ISSHOWALWAYS);
//}
//
//void ToolTip::SetShowAlways(bool showAlways)
//{
//	CC_BIT_SET(_Flags, CC_FLAG_TOOLTIP_ISSHOWALWAYS, showAlways);
//}

bool ToolTip::GetBalloon()
{
	return CC_BIT_HAS(_Flags, CC_FLAG_TOOLTIP_ISBALLOON);
}

void ToolTip::SetBalloon(bool balloon)
{
	CC_BIT_SET(_Flags, CC_FLAG_TOOLTIP_ISBALLOON, balloon);
}
///////////////////////////////////////////////////////////////////////////////

DWORD ToolItemConfig::nStyle = WS_CHILDWINDOW | WS_VISIBLE;
DWORD ToolItemConfig::nExStyle = 0;
//DWORD ToolItemConfig::nClassStyle = CS_DBLCLKS | CS_GLOBALCLASS | CS_SAVEBITS;
DWORD ToolItemConfig::nInnerStyle = InnerStyle::TranceContextMenuUp|InnerStyle::TrackMouse;
DWORD ToolItemConfig::nOuterStyle = OuterStyle::AutoSize|OuterStyle::ContextMenuButton;
DWORD ToolItemConfig::nOwnerDrawTextStyle = DT_CENTER|DT_VCENTER|DT_SINGLELINE;
COLORREF ToolItemConfig::corBackground = ::GetSysColor(COLOR_BTNFACE);
COLORREF ToolItemConfig::corTextground = ::GetSysColor(COLOR_BTNTEXT);
COLORREF ToolItemConfig::corHotBackColor = RGB(230, 230, 230);
COLORREF ToolItemConfig::corHotTextColor = ::GetSysColor(COLOR_BTNTEXT);
int ToolItemConfig::nMarginLeft = 3;
int ToolItemConfig::nMarginTop = 2;
int ToolItemConfig::nMarginRight = 3;
int ToolItemConfig::nMarginBottom = 2;
//===========================================================cc::win::ToolItem
///////////////////////////////////////////////////////////////////////////////
// constructors/destructor
ToolItem::ToolItem()
{
	BASE_CONTROL_CONFIG_INIT(ToolItem);
	::SetRect(&_Margin, ToolItemConfig::nMarginLeft, ToolItemConfig::nMarginTop, ToolItemConfig::nMarginRight, ToolItemConfig::nMarginBottom);
	::SetRect(&_Rect, 0, 4, 88, 21);
	::CopyRect(&_OrgRect, &_Rect);
#ifdef _DEBUG_
	SetName(_T("ToolItem"));
#endif
	_ClassName = WndClassName::ToolItem;
	//not set default text,for perhaps only img
	SetText(_T(""));
	_bIsClicking = false;
	_bIsPreShowMenu = false;
	_pFloater = NULL;
#if defined(_DEBUG_) || defined(_DESIGN)
	//code for design
	//flag for moveable or others
	Design = (DesignStyle::Type)((DWORD)Design | DesignStyle::AutoSize | DesignStyle::Background);
#endif
}

ToolItem::~ToolItem()
{
	if(_pFloater != NULL && CC_BIT_HAS(_Flags, CC_FLAG_TOOLITEM_NEEDDESTROY))
	{
		delete _pFloater;
		_pFloater = NULL;
	}
}

void ToolItem::Dispose()
{
	if(_pFloater != NULL && CC_BIT_HAS(_Flags, CC_FLAG_TOOLITEM_NEEDDESTROY))
	{
		_pFloater->Dispose();
	}
	Control::Dispose();
}

void ToolItem::DoAdjustRect(long& nRealX, long& nRealY, int& nRealCX, int& nRealCY, int nExpectX, int nExpectY, int nExpectCX, int nExpectCY)
{
	if(nExpectX != -1)
	{
		nRealX = nExpectX;
	}
	if(HasStyle(StyleType::Outer, OuterStyle::Break))
	{
		if(nExpectY != -1)
		{
			nRealY = nExpectY;
		}
		if(nExpectCX != -1)
		{
			nRealCX = nExpectCX;
			nRealCY = 3;
		}
		else
		{
			nRealCX = 3;
			if(nExpectCY != -1)
			{
				nRealCY = nExpectCY;
			}
		}
	}
	else
	{
		if(nExpectY != -1)
		{
			nRealY = nExpectY;
		}
		if(nExpectCX != -1)
		{
			nRealCX = nExpectCX;
		}
		if(nExpectCY != -1)
		{
			nRealCY = nExpectCY;
		}
	}
}

//call by CalcuteRect if autosize
SIZE ToolItem::DoMeasureSize(int ctlHeight)
{
	_nExpectCY = ctlHeight;
	DWORD dwStyle = 0;
	//if(this->HasStyle(StyleType::Outer, OuterStyle::ContextMenuButton) &&
	//	((this->GetContextMenu() != NULL && !this->HasStyle(StyleType::Outer, OuterStyle::PushButton))
	//	|| _pFloater != NULL))
	if(this->HasStyle(StyleType::Outer, OuterStyle::ContextMenuButton) &&
		(this->GetContextMenu() != NULL || _pFloater != NULL))
	{
		dwStyle |= CC_DRAWITEM_RIGHTARROW;
	}
	return CC_APP_GetDrawActor()->MeasureToolItem(this, ctlHeight, dwStyle);
}

void ToolItem::OnGotFocus(EventArgs* e)
{
	CC_ASSERT(::IsWindow(m_hWnd));

	if(m_hWnd != NULL)
	{
		::InvalidateRect(m_hWnd, NULL, FALSE);
	}
	Control::OnGotFocus(e);
}

void ToolItem::OnLostFocus(EventArgs* e)
{
	CC_ASSERT(::IsWindow(m_hWnd));

	if(m_hWnd != NULL)
	{
		::InvalidateRect(m_hWnd, NULL, FALSE);
	}
	Control::OnLostFocus(e);
}

//come here while has WS_TABSTOP
void ToolItem::OnKeyDown(KeyEventArgs* e)
{
	CC_ASSERT(::IsWindow(m_hWnd));

	Control::OnKeyDown(e);
	if((int)e->baseArgs->WParam == ' ' && !_bIsClicking)
	{
//err(_T("OnKeyDown[%s,%s]tchar:%d"), this->_ClassName, this->_Name, (int)e->baseArgs->WParam);
		e->baseArgs->Handled = true;
		//if(!HasStyle(StyleType::Outer, OuterStyle::PushButton) && GetContextMenu() != NULL)
		//if(((_StyleArray[(int)StyleType::Outer] & OuterStyle::PushButton) == 0 && GetContextMenu() != NULL) || _pFloater != NULL)
		if(this->HasStyle(StyleType::Outer, OuterStyle::ContextMenuButton) && (GetContextMenu() != NULL || _pFloater != NULL))
		{
			//show menu at paint
			_bIsPreShowMenu = true;
			OnPopup(e->baseArgs);
		}
		_bIsClicking = true;
		//SetStyle(StyleType::Inner, 0, InnerStyle::IsMouseEnter);
		_StyleArray[(int)StyleType::Inner2] |= Inner2Style::IsMouseEnter;
		if(m_hWnd != NULL)
		{
			::InvalidateRect(m_hWnd, NULL, FALSE);
		}
	
	}
}

//come here while has WS_TABSTOP
void ToolItem::OnKeyUp(KeyEventArgs* e)
{
	CC_ASSERT(::IsWindow(m_hWnd));

	Control::OnKeyUp(e);
	if((int)e->baseArgs->WParam == ' ' && _bIsClicking)
	{
//err(_T("OnKeyUp[%s,%s]tchar:%d"), this->_ClassName, this->_Name, (int)e->baseArgs->WParam);
		e->baseArgs->Handled = true;
		_bIsClicking = false;
		//SetStyle(StyleType::Inner, InnerStyle::IsMouseEnter, 0);
		_StyleArray[(int)StyleType::Inner2] &= ~Inner2Style::IsMouseEnter;
		if(GetIsPushButton())
		{
			//can not use SetPushed(GetPushed());
			//if(HasStyle(StyleType::Inner, InnerStyle::Pushed))
			if((_StyleArray[(int)StyleType::Inner] & InnerStyle::Pushed) != 0)
			{
				//SetStyle(StyleType::Inner, InnerStyle::Pushed, 0);
				_StyleArray[(int)StyleType::Inner] &= ~InnerStyle::Pushed;
			}
			else
			{
				//SetStyle(StyleType::Inner, 0, InnerStyle::Pushed);
				_StyleArray[(int)StyleType::Inner] |= InnerStyle::Pushed;
			}
		}
		OnClick(e->baseArgs);
		if(m_hWnd != NULL)
		{
			::InvalidateRect(m_hWnd, NULL, TRUE);
		}
	
	}
}

//keyArr is param address,but sHead is by value
void ToolItem::DoConfig(bool isFromIni, KeyArr& keyArr, cc::Str sThisHead)
{
	if(!GetIsPushButton() || !HasStyle(StyleType::Outer, OuterStyle::AutoConfig))
	{
		return;
	}
	if(isFromIni)
	{
		//if not exist,as usually
		if(keyArr.Exist(sThisHead))
		{
			SetPushed(keyArr.GetBool(sThisHead, false));
		}
	}
	else
	{
		keyArr.SetBool(sThisHead, GetPushed());
	}
}

void ToolItem::DoUpdate()
{
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreatedEnd) != 0 && _nUpdateFlag > 0 && _pParent != NULL && !_pParent->HasStyle(StyleType::Outer, OuterStyle::Sizing))
	{
		CC_ASSERT(::IsWindow(m_hWnd));

		//must had parent,for had set height by parent
		if(IsUpdate((UpdateFlag::Type)
			(UpdateFlag::UpdateFlag|UpdateFlag::UpdateText|UpdateFlag::UpdateIcon|UpdateFlag::UpdateVisible|UpdateFlag::UpdateFont
				|UpdateFlag::UpdateContextMenu|UpdateFlag::UpdateOuterStyle|UpdateFlag::UpdateInnerStyle)))
		{
#ifdef _DEBUG_
			if(CC_APP_GetConfig()->GetIsDebug())
			{
				CC_TRACE((_T("ToolItem::DoUpdate(), ClassName:%s, Name:%s, Text:%s, _nUpdateFlag:%d"), _ClassName, _Name, _Text, _nUpdateFlag));
			}
#endif
			//for change size
			_sizeTextIcon.cx = -1;
			CalcuteRect(true);
			CC_ASSERT(_pParent != NULL);
			::PostMessage(_pParent->m_hWnd, CC_WM_CHILD_UPDATED, (WPARAM)this, 0);
			//_pParent->Update();
		}
		if(m_hWnd != NULL)
		{
			::InvalidateRect(m_hWnd, NULL, TRUE);
		}
	}
	Control::DoUpdate();
}

void ToolItem::OnInnerTimer(EventArgs* e)
{
	if(e->WParam != CC_TIMERID_CONTROL_TIMEOUT)
	{
		Control::OnInnerTimer(e);
		return;
	}
	::KillTimer(m_hWnd, CC_TIMERID_CONTROL_TIMEOUT);
	if(m_hWnd != NULL)
	{
		::InvalidateRect(m_hWnd, NULL, TRUE);
	}
}

//if do BeginPaint at parent, then also need do GetClientRect(m_hWnd, &e->data.rcPaint)
void ToolItem::OnPaint(PaintEventArgs *e)
{
	bool newHdc = false;
	if(e->data.hdc == NULL)
	{
		::BeginPaint(m_hWnd, &(e->data));
		newHdc = true;
	}

	DRAWITEMSTRUCT data = {0};
	DrawItemEventArgs ee(e->baseArgs);
	ee.data = &data;
	//HDC hDC = BeginPaint(m_hWnd, &(e->data));
	HDC hDC = e->data.hdc;
	//If GetUpdateRect returns zero, the application should not call the BeginPaint and EndPaint functions.
	if(e->data.rcPaint.bottom != 0 && e->data.rcPaint.right != 0)
	{
		data.hDC = hDC;
		if(newHdc)
		{
			GetClientRect(m_hWnd, &data.rcItem);
		}
		else
		{
			CopyRect(&data.rcItem, &e->data.rcPaint);
		}
		//this time for no frame
		if(_bIsClicking || (GetPushed() && GetIsPushButton()))
		{
			data.itemState |= ODS_SELECTED;
		}
		if(HasStyle(StyleType::Style, WS_TABSTOP) && ::GetFocus() == m_hWnd)
		{
			data.itemState |= ODS_FOCUS;
		}
		if(_bIsPreShowMenu || HasStyle(StyleType::Inner, InnerStyle::ContextMenuShowing))
		{
			//SetStyle(StyleType::Inner, 0, InnerStyle::IsMouseEnter);
			_StyleArray[(int)StyleType::Inner2] |= Inner2Style::IsMouseEnter;
			ee.dwStyle |= CC_DRAWITEM_POPMENU;
		}
		if(this->HasStyle(StyleType::Outer, OuterStyle::ContextMenuButton) &&
			(this->GetContextMenu() != NULL || _pFloater != NULL))
		{
			ee.dwStyle |= CC_DRAWITEM_RIGHTARROW;
		}

		CC_APP_GetDrawActor()->DrawToolItem(this, &ee);
		if(_bIsPreShowMenu)
		{
			_bIsPreShowMenu = false;
			RECT rect;
			::GetWindowRect(m_hWnd, &rect);
			//POINT pt = {0, _Rect.bottom - _Rect.top};
			//::ClientToScreen(m_hWnd, &pt);
			//SetStyle(StyleType::Inner, 0, InnerStyle::ContextMenuShowing);
			_StyleArray[(int)StyleType::Inner] |= InnerStyle::ContextMenuShowing;
			if(_pFloater != NULL)
			{
				//perhaps _pFloater is not created, cannot GetWindowRect
				RECT rect2 = _pFloater->GetBounds();
				rect2.right -= rect2.left;
				rect2.bottom -= rect2.top;
				cc::Util::AdjustWindowRect((int&)rect.left, (int&)rect.bottom, (int&)rect2.right, (int&)rect2.bottom);
				_pFloater->SetParent(this, true);
				if(rect.bottom <= rect.top)
				{
					rect.bottom = rect.top - rect2.bottom;
				}
				_pFloater->SetLocation(rect.left, rect.bottom);
				_pFloater->Show();
				_pFloater->Focus();
			}
			else
			{
				GetContextMenu()->Show(this, rect.left, rect.bottom);
			}
			//SetStyle(StyleType::Inner, InnerStyle::ContextMenuShowing, 0);
			_StyleArray[(int)StyleType::Inner] &= ~InnerStyle::ContextMenuShowing;
			_bIsClicking = false;
			//SetStyle(StyleType::Inner, InnerStyle::IsMouseEnter, 0);
			//_StyleArray[(int)StyleType::Inner] &= ~InnerStyle::IsMouseEnter;
			//detect IsMouseEnter or out
			::SetTimer(m_hWnd, CC_TIMERID_CONTROL_MMOVE, 15, NULL);
			data.itemState &= ~ODS_SELECTED;
			ee.dwStyle &= ~CC_DRAWITEM_POPMENU;
			CC_APP_GetDrawActor()->DrawToolItem(this, &ee);

			//while ContextMenu->Show,not paint self well, so wait to repaint self
			::SetTimer(m_hWnd, CC_TIMERID_CONTROL_TIMEOUT, 30, NULL);
		}
	}
	//call PaintBegin&PaintEnd at DrawToolItem
	//Control::OnPaint(e);
	//EndPaint(m_hWnd, &(e->data));
	if(newHdc)
	{
		::EndPaint(m_hWnd, &(e->data));
		//memset(&(e->data), 0, sizeof(PAINTSTRUCT));
	}
}

void ToolItem::OnMouseDown(MouseEventArgs* e)
{
	CC_ASSERT(::IsWindow(m_hWnd));
	if(e->Button == MouseButtons::Left && _moveForm != NULL && _moveForm->m_hWnd != NULL)
	{
		Control::OnMouseDown(e);
		return;
	}

	if(IsTabStop() && ::GetFocus() != m_hWnd)
	{
		::SetFocus(m_hWnd);
	}
	Control::OnMouseDown(e);

	if(::GetCapture() == m_hWnd)
	{
		::ReleaseCapture();
		_bIsClicking = false;
		if(m_hWnd != NULL)
		{
			::InvalidateRect(m_hWnd, NULL, FALSE);
		}
	}
	else
	{
		if(e->Button == MouseButtons::Left)
		{
			//if((_pFloater != NULL || (!HasStyle(StyleType::Outer, OuterStyle::PushButton) && GetContextMenu() != NULL)) &&
			//	(!HasStyle(StyleType::Outer, OuterStyle::ContextMenuButton) || (e->X >= _Rect.right - _Rect.left - CONTROL_CONTEXTMENU_BTN_W)))
			if((_pFloater != NULL || GetContextMenu() != NULL) && HasStyle(StyleType::Outer, OuterStyle::ContextMenuButton) && 
				(HasStyle(StyleType::Outer, OuterStyle::PushButton) || (e->X >= _Rect.right - _Rect.left - CONTROL_CONTEXTMENU_BTN_W)))
			{
				//will show menu at paint
				_bIsPreShowMenu = true;
				OnPopup(e->baseArgs);
				if(_pFloater != NULL)
				{
					//for show _pFloater at paint,then this lost focus and not go to OnMouseUp, so set pushed first.
					_StyleArray[(int)StyleType::Inner] |= InnerStyle::Pushed;
				}
			}
			else
			{
				SetCapture(m_hWnd);
			}
			_bIsClicking = true;
			if(m_hWnd != NULL)
			{
				::InvalidateRect(m_hWnd, NULL, FALSE);
			}
		}
	}
}

void ToolItem::Menu_OnExitLoop(EventArgs* e)
{
	//do redraw face
	if(m_hWnd != NULL)
	{
		::InvalidateRect(m_hWnd, NULL, FALSE);
	}
	Control::Menu_OnExitLoop(e);
}

void ToolItem::OnClick(EventArgs* e)
{
	//for toolitem while 1 then Do toolitem's ClickEvent even has _pContextMenu
	//and while set e->Handled in toolitem's ClickEvent,then not call first item's ClickEvent of _pContextMenu even has _pContextMenu
	if(HasStyle(StyleType::Outer, OuterStyle::PrivateFlag1) || _pContextMenu == NULL || _pContextMenu->ItemCount() <= 0)
	{
		Control::OnClick(e);
	}
	if(!e->Handled && _pContextMenu != NULL && _pContextMenu->ItemCount() > 0)
	{
		((MenuItem*)_pContextMenu->ItemGet(0))->PerformSelect(e);
		((MenuItem*)_pContextMenu->ItemGet(0))->PerformClick(e);
	}
}

void ToolItem::OnPopup(EventArgs* e)
{
	Popup.Invoke((Object*)this, e);
}

void ToolItem::OnMouseUp(MouseEventArgs* e)
{
	CC_ASSERT(::IsWindow(m_hWnd));

	Control::OnMouseUp(e);

	if(::GetCapture() == m_hWnd)
	{
		ReleaseCapture();
		_bIsClicking = false;
		if(GetIsMouseOn())
		{
			if(GetIsPushButton())
			{
				//can not use SetPushed(GetPushed());
				if(HasStyle(StyleType::Inner, InnerStyle::Pushed))
				{
					//SetStyle(StyleType::Inner, InnerStyle::Pushed, 0);
					_StyleArray[(int)StyleType::Inner] &= ~InnerStyle::Pushed;
				}
				else
				{
					//SetStyle(StyleType::Inner, 0, InnerStyle::Pushed);
					_StyleArray[(int)StyleType::Inner] |= InnerStyle::Pushed;
				}
			}
			OnClick(e->baseArgs);
		}
		//perhaps here close form and m_hWnd is turned to NULL
		if(m_hWnd != NULL)
		{
			::InvalidateRect(m_hWnd, NULL, TRUE);
		}
	}
}

// Get or set this item's properties.
bool ToolItem::GetBlank()
{
	return HasStyle(StyleType::Outer, OuterStyle::Blank);
}

void ToolItem::SetBlank(bool value)
{
	SetStyleBool(StyleType::Outer, OuterStyle::Blank, value, SWP_FRAMECHANGED|SWP_DRAWFRAME, true);
}

bool ToolItem::GetBreak()
{
	return HasStyle(StyleType::Outer, OuterStyle::Break);
}

void ToolItem::SetBreak(bool value)
{
	SetStyleBool(StyleType::Outer, OuterStyle::Break, value, SWP_FRAMECHANGED|SWP_DRAWFRAME, true);
}

bool ToolItem::GetPushed()
{
	return HasStyle(StyleType::Inner, InnerStyle::Pushed);
}

void ToolItem::SetPushed(bool value)
{
	SetStyleBool(StyleType::Inner, InnerStyle::Pushed, value, SWP_FRAMECHANGED|SWP_DRAWFRAME, true);
}

bool ToolItem::GetAutoSize()
{
	return HasStyle(StyleType::Outer, OuterStyle::AutoSize);
}

void ToolItem::SetAutoSize(bool value)
{
	SetStyleBool(StyleType::Outer, OuterStyle::AutoSize, value, SWP_FRAMECHANGED|SWP_DRAWFRAME, true);
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0 && value)
	{
		//for change size
		_sizeTextIcon.cx = -1;
		CalcuteRect(true);
		Update();
	}
}

bool ToolItem::GetIsPushButton()
{
	if(this->HasStyle(StyleType::Outer, OuterStyle::ContextMenuButton) && (GetContextMenu() != NULL || _pFloater != NULL))
	{
		return false;
	}
	return HasStyle(StyleType::Outer, OuterStyle::PushButton);
}

void ToolItem::SetIsPushButton(bool value)
{
	SetStyleBool(StyleType::Outer, OuterStyle::PushButton, value, SWP_FRAMECHANGED|SWP_DRAWFRAME, true);
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
	{
		//for change size
		_sizeTextIcon.cx = -1;
		CalcuteRect(true);
		Update();
	}
}

FloatContainer* ToolItem::GetFloatContainer()
{
	return _pFloater;
}

void ToolItem::SetFloatContainer(FloatContainer* pFloater, bool needDestroy)
{
	if(_pFloater != NULL && CC_BIT_HAS(_Flags, CC_FLAG_TOOLITEM_NEEDDESTROY))
	{
		_pFloater->Dispose();
		delete _pFloater;
		_pFloater = NULL;
	}
	_pFloater = pFloater;
	CC_BIT_SET(_Flags, CC_FLAG_TOOLITEM_NEEDDESTROY, needDestroy);
}

void ToolItem::OnMouseEnter(EventArgs* e)
{
	CC_ASSERT(::IsWindow(m_hWnd));

	Control::OnMouseEnter(e);
	if(m_hWnd != NULL)
	{
		::InvalidateRect(m_hWnd, NULL, FALSE);
	}
}

void ToolItem::OnMouseLeave(EventArgs* e)
{
	CC_ASSERT(::IsWindow(m_hWnd));

	Control::OnMouseLeave(e);
	if(m_hWnd != NULL)
	{
		::InvalidateRect(m_hWnd, NULL, FALSE);
	}
}
// constructors/destructor
///////////////////////////////////////////////////////////////////////////////

DWORD StatusItemConfig::nStyle = WS_CHILD | WS_VISIBLE;
DWORD StatusItemConfig::nExStyle = 0;
//DWORD StatusItemConfig::nClassStyle = CS_DBLCLKS | CS_GLOBALCLASS | CS_SAVEBITS;
DWORD StatusItemConfig::nInnerStyle = InnerStyle::TranceContextMenuUp|InnerStyle::TrackMouse;
//need OuterStyle::AlwaysNoEdge for draw break line at DrawActor::DrawControl
DWORD StatusItemConfig::nOuterStyle = OuterStyle::AutoSize|OuterStyle::ContextMenuButton|OuterStyle::AlwaysNoEdge;
DWORD StatusItemConfig::nOwnerDrawTextStyle = DT_LEFT|DT_VCENTER|DT_SINGLELINE;
COLORREF StatusItemConfig::corBackground = ::GetSysColor(COLOR_BTNFACE);
COLORREF StatusItemConfig::corTextground = ::GetSysColor(COLOR_BTNTEXT);
COLORREF StatusItemConfig::corHotBackColor = ::GetSysColor(COLOR_BTNFACE);
COLORREF StatusItemConfig::corHotTextColor = ::GetSysColor(COLOR_BTNTEXT);
int StatusItemConfig::nMarginLeft = 0;
int StatusItemConfig::nMarginTop = 0;
int StatusItemConfig::nMarginRight = 0;
int StatusItemConfig::nMarginBottom = 0;
//===========================================================cc::win::StatusItem
///////////////////////////////////////////////////////////////////////////////
// constructors/destructor
StatusItem::StatusItem()
{
	BASE_CONTROL_CONFIG_INIT(StatusItem);
	::SetRect(&_Margin, StatusItemConfig::nMarginLeft, StatusItemConfig::nMarginTop, StatusItemConfig::nMarginRight, StatusItemConfig::nMarginBottom);
	::SetRect(&_Rect, 0, 5, 88, 21);
	::CopyRect(&_OrgRect, &_Rect);
#ifdef _DEBUG_
	SetName(_T("StatusItem"));
#endif
	//not set default text,for perhaps only img
	//SetText(_T("StatusItem"));
}

StatusItem::~StatusItem()
{
}
// constructors/destructor

//cut strings after "\r\n"
void StatusItem::SetText(const TCHAR* sText)
{
	LockAction lock(&m_lockText);

	_sizeTextIcon.cx = -1;
	_Text = CC_StrCopy(&_Text, sText);
	if(_Text != NULL)
	{
		TCHAR* chr = _tcschr(_Text, _T('\r'));
		if(chr != NULL)
		{
			chr[0] = _T('\0');
		}
		chr = _tcschr(_Text, _T('\n'));
		if(chr != NULL)
		{
			chr[0] = _T('\0');
		}
	}
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
	{
		::SetWindowText(m_hWnd, _Text);
	}
}

void StatusItem::DoAdjustRect(long& nRealX, long& nRealY, int& nRealCX, int& nRealCY, int nExpectX, int nExpectY, int nExpectCX, int nExpectCY)
{
	if(nExpectX != -1)
	{
		nRealX = nExpectX;
	}
	if(HasStyle(StyleType::Outer, OuterStyle::Break))
	{
		if(nExpectY != -1)
		{
			nRealY = nExpectY + 2;
		}
		if(nExpectCY != -1)
		{
			nRealCY = nExpectCY - 1 + 2;
		}
	}
	else
	{
		if(nExpectY != -1)
		{
			nRealY = nExpectY + 3;
		}
		if(nExpectCY != -1)
		{
			nRealCY = nExpectCY - 4;
		}
	}
}

StatusItem::AutoSize::Type StatusItem::GetAutoSize()
{
	if(HasStyle(StyleType::Outer, OuterStyle::AutoSize))
	{
		return AutoSize::Contents;
	}
	else if(HasStyle(StyleType::Outer, OuterStyle::AutoSizeSprint))
	{
		return AutoSize::Sprint;
	}
	return AutoSize::None;
}

void StatusItem::SetAutoSize(StatusItem::AutoSize::Type eAutoSize)
{
	if(eAutoSize == AutoSize::Contents)
	{
		//SetOuterStyle(OuterStyle::AutoSizeSprint, false, true);
		//SetOuterStyle(OuterStyle::AutoSize, true, true);
		SetStyle(StyleType::Outer, OuterStyle::AutoSizeSprint, OuterStyle::AutoSize, SWP_FRAMECHANGED|SWP_DRAWFRAME);
	}
	else if(eAutoSize == AutoSize::Sprint)
	{
		//SetOuterStyle(OuterStyle::AutoSize, false, true);
		//SetOuterStyle(OuterStyle::AutoSizeSprint, true, true);
		SetStyle(StyleType::Outer, OuterStyle::AutoSize, OuterStyle::AutoSizeSprint, SWP_FRAMECHANGED|SWP_DRAWFRAME);
	}
	else
	{
		//SetOuterStyle(OuterStyle::AutoSize, false, true);
		//SetOuterStyle(OuterStyle::AutoSizeSprint, false, true);
		SetStyle(StyleType::Outer, OuterStyle::AutoSizeSprint|OuterStyle::AutoSize, 0, SWP_FRAMECHANGED|SWP_DRAWFRAME);
	}
	Update();
}
///////////////////////////////////////////////////////////////////////////////

DWORD PushButtonConfig::nStyle = WS_CHILD|WS_VISIBLE;
DWORD PushButtonConfig::nExStyle = 0;
//DWORD PushButtonConfig::nClassStyle = CS_DBLCLKS|CS_GLOBALCLASS;
DWORD PushButtonConfig::nInnerStyle = InnerStyle::TranceContextMenuUp|InnerStyle::TrackMouse;
DWORD PushButtonConfig::nOuterStyle = OuterStyle::AutoSize|OuterStyle::ContextMenuButton|OuterStyle::PushButton;
DWORD PushButtonConfig::nOwnerDrawTextStyle = DT_CENTER|DT_VCENTER|DT_SINGLELINE;
COLORREF PushButtonConfig::corBackground = ::GetSysColor(COLOR_BTNFACE);
COLORREF PushButtonConfig::corTextground = ::GetSysColor(COLOR_BTNTEXT);
COLORREF PushButtonConfig::corHotBackColor = RGB(230, 230, 230);
COLORREF PushButtonConfig::corHotTextColor = ::GetSysColor(COLOR_BTNTEXT);
//===========================================================cc::win::PushButton
///////////////////////////////////////////////////////////////////////////////
// constructors/destructor
PushButton::PushButton()
{
	BASE_CONTROL_CONFIG_INIT(PushButton);
	_Rect.right = _Rect.left + 19;
	_Rect.bottom = _Rect.top + 12;
	::CopyRect(&_OrgRect, &_Rect);
	SetText(_T("PushButton"));
	::SetRect(&_Margin, 3, 2, 3, 2);

#ifdef _DEBUG_
	SetName(_T("PushButton"));
#endif
}

PushButton::~PushButton()
{
}
// constructors/destructor
///////////////////////////////////////////////////////////////////////////////

DWORD LabelLinkConfig::nStyle = WS_CHILD|WS_VISIBLE|WS_TABSTOP;
DWORD LabelLinkConfig::nExStyle = 0;
//DWORD LabelLinkConfig::nClassStyle = CS_DBLCLKS|CS_GLOBALCLASS;
DWORD LabelLinkConfig::nInnerStyle = InnerStyle::TranceContextMenuUp|InnerStyle::TrackMouse;
DWORD LabelLinkConfig::nOuterStyle = OuterStyle::AutoSize|OuterStyle::AlwaysNoEdge;
DWORD LabelLinkConfig::nOwnerDrawTextStyle = DT_LEFT|DT_VCENTER|DT_SINGLELINE;
COLORREF LabelLinkConfig::corBackground = ::GetSysColor(COLOR_BTNFACE);
COLORREF LabelLinkConfig::corTextground = RGB(0, 0, 255);
COLORREF LabelLinkConfig::corHotBackColor = RGB(230, 230, 230);
COLORREF LabelLinkConfig::corHotTextColor = RGB(255, 0, 0);
int LabelLinkConfig::nMarginLeft = 3;
int LabelLinkConfig::nMarginTop = 1;
int LabelLinkConfig::nMarginRight = 3;
int LabelLinkConfig::nMarginBottom = 1;
//===========================================================cc::win::LabelLink
///////////////////////////////////////////////////////////////////////////////
// constructors/destructor
LabelLink::LabelLink()
{
	BASE_CONTROL_CONFIG_INIT(LabelLink);
	::SetRect(&_Margin, LabelLinkConfig::nMarginLeft, LabelLinkConfig::nMarginTop, LabelLinkConfig::nMarginRight, LabelLinkConfig::nMarginBottom);
	_Rect.right = _Rect.left + 160;
	_Rect.bottom = _Rect.top + 14;
	::CopyRect(&_OrgRect, &_Rect);
	m_eLinkType = LinkType::Auto;
	SetText(_T("about:blank"));
	_URL = NULL;
	_hCursor = ::LoadCursor(NULL, IDC_HAND);

#ifdef _DEBUG_
	SetName(_T("LabelLink"));
#endif
#if defined(_DEBUG_) || defined(_DESIGN)
	//code for design
	this->Design = (DesignStyle::Type)((DWORD)this->Design | DesignStyle::EditURL | DesignStyle::AutoSize | DesignStyle::Background);
#endif
}

LabelLink::~LabelLink()
{
	if(_URL != NULL)
	{
		free(_URL);
		_URL = NULL;
	}
}
// constructors/destructor

const TCHAR* LabelLink::GetURL()
{
	return _URL;
}

const TCHAR* LabelLink::GetTips()
{
	if(_Tips == NULL)
	{
		return _URL;
	}
	return _Tips;
}

void LabelLink::SetURL(const TCHAR* sURL)
{
	_URL = CC_StrCopy(&_URL, sURL);
}

void LabelLink::SetLinkType(LinkType::Type linkType)
{
	m_eLinkType = linkType;
	//CC_BIT_SET(_Flags, CC_FLAG_LABELLINK_ISMAILLINK, isMailLink);
}

LinkType::Type LabelLink::GetLinkType()
{
	//return CC_BIT_HAS(_Flags, CC_FLAG_LABELLINK_ISMAILLINK);
	return m_eLinkType;
}

void LabelLink::OnClick(EventArgs* e)
{
	cc::Str sLink = cc::Str(_URL == NULL ? _Text : _URL);
	if(sLink.GetLength() <= 0)
	{
		return;
	}
	if(m_eLinkType == LinkType::Mail || 
		(m_eLinkType == LinkType::Auto && sLink.IndexOf(_T("@")) > 0
		&& !sLink.StartsWith(_T("http://"), true) && !sLink.StartsWith(_T("ftp://"), true)))
	{
		if(!sLink.StartsWith(_T("mailto:"), true))
		{
			sLink.Insert(0, _T("mailto:"));
		}
		ShellExecute(NULL, NULL, (TCHAR*)sLink.GetBuf(), NULL, NULL, SW_SHOWNORMAL);
	}
	else
	{
		ShellExecute(NULL, _T("open"), (TCHAR*)sLink.GetBuf(), NULL, NULL, SW_SHOWNORMAL);
	}
}

void LabelLink::DoSetLang(KeyArr& keyArr, cc::Str sThisHead)
{
	Control::DoSetLang(keyArr, sThisHead);
	int nInd;
	if((nInd = keyArr.GetIndex(sThisHead.Append(_T(".URL")))) >= 0)
	{
		SetURL(keyArr.GetKey(nInd));
	}
}
///////////////////////////////////////////////////////////////////////////////

DWORD GradientLabelConfig::nStyle = WS_CHILD|WS_VISIBLE;
DWORD GradientLabelConfig::nExStyle = 0;
//DWORD GradientLabelConfig::nClassStyle = CS_DBLCLKS|CS_GLOBALCLASS;
DWORD GradientLabelConfig::nInnerStyle = InnerStyle::TranceContextMenuUp|InnerStyle::TrackMouse|InnerStyle::BackgroundGradient;
DWORD GradientLabelConfig::nOuterStyle = OuterStyle::ContextMenuButton|OuterStyle::AlwaysNoEdge|OuterStyle::TransparentTop;
DWORD GradientLabelConfig::nOwnerDrawTextStyle = DT_LEFT|DT_VCENTER|DT_SINGLELINE;
COLORREF GradientLabelConfig::corBackground = RGB(230, 245, 235);
COLORREF GradientLabelConfig::corTextground = ::GetSysColor(COLOR_BTNTEXT);
COLORREF GradientLabelConfig::corHotBackColor = RGB(70, 170, 100);
COLORREF GradientLabelConfig::corHotTextColor = ::GetSysColor(COLOR_BTNTEXT);
//===========================================================cc::win::GradientLabel
///////////////////////////////////////////////////////////////////////////////
// constructors/destructor
GradientLabel::GradientLabel()
{
	BASE_CONTROL_CONFIG_INIT(GradientLabel);
	_Rect.right = _Rect.left + 19;
	_Rect.bottom = _Rect.top + 12;
	::CopyRect(&_OrgRect, &_Rect);
	SetText(_T("GradientLabel"));
	::SetRect(&_Margin, 3, 2, 3, 2);

#ifdef _DEBUG_
	SetName(_T("GradientLabel"));
#endif
}

GradientLabel::~GradientLabel()
{
}
// constructors/destructor
///////////////////////////////////////////////////////////////////////////////

/*
Static Styles
SS_BITMAP   Specifies a bitmap is to be displayed in the static control. The given text is the name of a bitmap (not a filename) defined elsewhere in the resource file. The style ignores the nWidth and nHeight parameters; the control automatically sizes itself to accommodate the bitmap.
SS_BLACKFRAME   Specifies a box with a frame drawn with the same color as window frames. The default is black.
SS_BLACKRECT   Specifies a rectangle filled with the color used to draw window frames. The default is black.
SS_CENTER   Designates a simple rectangle and displays the given text centered in the rectangle. The text is formatted before it is displayed. Words that would extend past the end of a line are automatically wrapped to the beginning of the next centered line.
SS_CENTERIMAGE   Specifies that, if the bitmap or icon is smaller than the client area of the static control, the rest of the client area is filled with the color of the pixel in the top left corner of the bitmap or icon. If the static control contains a single line of text, the text is centered vertically in the client area of the control.
SS_ENDELLIPSIS or SS_PATHELLIPSIS   Replaces part of the given string with ellipses, if necessary, so that the result fits in the specified rectangle.
You can specify SS_END_ELLIPSIS to replace characters at the end of the string, or SS_PATHELLIPSIS to replace characters in the middle of the string. If the string contains backslash (\) characters, SS_PATHELLIPSIS preserves as much of the text after the last backslash as possible.
SS_ENHMETAFILE   Specifies an enhanced metafile is to be displayed in the static control. The given text is the name of a metafile. An enhanced metafile static control has a fixed size; the metafile is scaled to fit the static control's client area.
SS_ETCHEDFRAME   Draws the frame of the static control using the EDGE_ETCHED edge style.
SS_ETCHEDHORZ   Draws the top and bottom edges of the static control using the EDGE_ETCHED edge style.
SS_ETCHEDVERT   Draws the left and right edges of the static control using the EDGE_ETCHED edge style.
SS_GRAYFRAME   Specifies a box with a frame drawn with the same color as the screen background (desktop). The default is gray.
SS_GRAYRECT   Specifies a rectangle filled with the color used to fill the screen background. The default is gray.
SS_ICON   Designates an icon displayed in the dialog box. The given text is the name of an icon (not a filename) defined elsewhere in the resource file. The nWidth and nHeight parameters are ignored; the icon automatically sizes itself.
SS_LEFT   Designates a simple rectangle and displays the given text flush-left in the rectangle. The text is formatted before it is displayed. Words that would extend past the end of a line are automatically wrapped to the beginning of the next flush-left line.
SS_LEFTNOWORDWRAP   Designates a simple rectangle and displays the given text flush-left in the rectangle. Tabs are expanded, but words are not wrapped. Text that extends past the end of a line is clipped.
SS_NOPREFIX   Unless this style is specified, Windows will interpret any ampersand (&) characters in the control's text to be accelerator prefix characters. In this case, the ampersand is removed and the next character in the string is underlined. If a static control is to contain text where this feature is not wanted, SS_NOPREFIX may be added. This static-control style may be included with any of the defined static controls. You can combine SS_NOPREFIX with other styles by using the bitwise OR operator. This is most often used when filenames or other strings that may contain an ampersand need to be displayed in a static control in a dialog box.
SS_NOTIFY   Sends the parent window STN_CLICKED, STN_DBLCLK, STN_DISABLE, and STN_ENABLE notification messages when the user clicks or double-clicks the control.
SS_OWNERDRAW   Specifies that the owner of the static control is responsible for drawing the control. The owner window receives a WM_DRAWITEM message whenever the control needs to be drawn.
SS_REALSIZEIMAGE   Prevents a static icon or bitmap control (that is, static controls that have the SS_ICON or SS_BITMAP style) from being resized as it is loaded or drawn. If the icon or bitmap is larger than the destination area, the image is clipped.
SS_RIGHT   Designates a simple rectangle and displays the given text flush-right in the rectangle. The text is formatted before it is displayed. Words that would extend past the end of a line are automatically wrapped to the beginning of the next flush-right line.
SS_RIGHTJUST   Specifies that the lower right corner of a static control with the SS_BITMAP or SS_ICON style is to remain fixed when the control is resized. Only the top and left sides are adjusted to accommodate a new bitmap or icon.
SS_SIMPLE   Designates a simple rectangle and displays a single line of text flush-left in the rectangle. The line of text cannot be shortened or altered in any way. (The control's parent window or dialog box must not process the WM_CTLCOLOR message.)
SS_SUNKEN   Draws a half-sunken border around a static control.
SS_USERITEM   Specifies a user-defined item.
SS_WHITEFRAME   Specifies a box with a frame drawn with the same color as the window background. The default is white.
SS_WHITERECT   Specifies a rectangle filled with the color used to fill the window background. The default is white.
SS_WORDELLIPSIS   Truncates text that does not fit and adds ellipses.
*/
/*
//add SS_NOTIFY for tips
DWORD LabelConfig::nStyle = WS_CHILDWINDOW | WS_VISIBLE | WS_GROUP | SS_LEFT | SS_EDITCONTROL;
DWORD LabelConfig::nExStyle = WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR | WS_EX_NOPARENTNOTIFY;
//DWORD LabelConfig::nClassStyle = CS_DBLCLKS | CS_GLOBALCLASS | CS_PARENTDC;
DWORD LabelConfig::nInnerStyle = InnerStyle::TranceContextMenuUp;
DWORD LabelConfig::nOuterStyle = OuterStyle::AlwaysNoEdge;
DWORD LabelConfig::nOwnerDrawTextStyle = DT_LEFT|DT_WORDBREAK;
COLORREF LabelConfig::corBackground = ::GetSysColor(COLOR_BTNFACE);
COLORREF LabelConfig::corTextground = ::GetSysColor(COLOR_BTNTEXT);
COLORREF LabelConfig::corHotBackColor = ::GetSysColor(COLOR_BTNFACE);
COLORREF LabelConfig::corHotTextColor = ::GetSysColor(COLOR_BTNTEXT);
//===========================================================cc::win::Label
///////////////////////////////////////////////////////////////////////////////
// constructors/destructor
Label::Label()
{
	BASE_CONTROL_CONFIG_INIT(Label);
	_Rect.right = _Rect.left + 19;
	_Rect.bottom = _Rect.top + 12;
	::CopyRect(&_OrgRect, &_Rect);
	_ClassName = WndClassName::Label;
	SetText(_T("Label"));
	m_pFocusControl = NULL;
	::SetRect(&_Margin, 0, 0, 0, 0);

#ifdef _DEBUG_
	SetName(_T("Label"));
#endif
}

Label::~Label()
{
}
// constructors/destructor
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// others
void Label::OnCreateControl()
{
	Control::OnCreateControl();
	if(_pImageList != NULL && _nIconID > 0)
	{
		SetStyle(StyleType::Style, SS_BITMAP|SS_ENHMETAFILE, SS_ICON);
		HICON icon = (HICON)::SendMessage(m_hWnd, STM_SETICON, (WPARAM)GetIcon(GetEnabled() ? IconType::Hot : IconType::Disabled), 0L);
		if(icon != NULL)
		{
			::DestroyIcon(icon);
		}
	}
	if(_hCursor != NULL)
	{
		SetStyle(StyleType::Style, 0, SS_NOTIFY);
	}
}

void Label::DoUpdate()
{
	if(IsUpdate((UpdateFlag::Type)(UpdateFlag::UpdateTips|UpdateFlag::UpdateTipsExt)))
	{
		if((_Tips != NULL || _TipsExt != NULL) && (_StyleArray[(int)StyleType::Style] & (SS_NOTIFY)) == 0)
		{
			this->SetStyle(StyleType::Style, 0, SS_NOTIFY);
		}
	}
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0 &&
		IsUpdate((UpdateFlag::Type)(UpdateFlag::UpdateIcon)))
	{
		CC_ASSERT(::IsWindow(m_hWnd));

		if(IsUpdate((UpdateFlag::Type)(UpdateFlag::UpdateIcon)))
		{
			SetStyle(StyleType::Style, SS_BITMAP|SS_ENHMETAFILE, SS_ICON);
			HICON icon = (HICON)::SendMessage(m_hWnd, STM_SETICON, (WPARAM)GetIcon(GetEnabled() ? IconType::Hot : IconType::Disabled), 0L);
			if(icon != NULL)
			{
				::DestroyIcon(icon);
			}
		}
	}
	Control::DoUpdate();
}

bool Label::DoProcessMnemonic(TCHAR tchar)
{
	if(m_pFocusControl != NULL && GetMnemonic() == tchar)
	{
		m_pFocusControl->Focus();
		return true;
	}
	return false;
}

void Label::OnChildNotify(EventArgs *e)
{
	if(e->Msg != WM_DRAWITEM)
	{
		return Control::OnChildNotify(e);
	}

	DrawItemEventArgs ee(e);
	ee.data = (LPDRAWITEMSTRUCT)e->LParam;
	OnDrawItem(&ee);
}

void Label::OnDrawItem(DrawItemEventArgs* e)
{
	if(DrawItem.IsEmpty())
	{
		if(this->HasStyle(StyleType::Outer, OuterStyle::Break))
		{
			CC_APP_GetDrawActor()->DrawPanel(this, e);
		}
		else
		{
			CC_APP_GetDrawActor()->DrawControl(this, e);
		}
	}
	else
	{
		DrawItem.Invoke((Object*)this, e);
	}
}

//::LoadCursor(NULL, IDC_HAND)
void Label::SetCursor(HCURSOR hCursor)
{
	Control::SetCursor(hCursor);
	//tips also need SS_NOTIFY
	SetStyleBool(StyleType::Style, SS_NOTIFY, true);
}

bool Label::GetAutoSize()
{
	return HasStyle(StyleType::Outer, OuterStyle::AutoSize);
}

void Label::SetAutoSize(bool value)
{
	SetStyleBool(StyleType::Outer, OuterStyle::AutoSize, value, SWP_FRAMECHANGED|SWP_DRAWFRAME);
	CalcuteRect(true);
}

Control* Label:: GetFocusControl()
{
	return m_pFocusControl;
}

void Label::SetFocusControl(Control* ctl)
{
	m_pFocusControl = ctl;
}
//HICON Label::SetIcon(HICON hIcon)
//	{ return (HICON)::SendMessage(m_hWnd, STM_SETICON, (WPARAM)hIcon, 0L); }
//HICON Label::GetIcon() const
//	{ CC_ASSERT(::IsWindow(m_hWnd)); return (HICON)::SendMessage(m_hWnd, STM_GETICON, 0, 0L); }
HENHMETAFILE Label::SetEnhMetaFile(HENHMETAFILE hMetaFile)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (HENHMETAFILE)::SendMessage(m_hWnd, STM_SETIMAGE, IMAGE_ENHMETAFILE, (LPARAM)hMetaFile); }
HENHMETAFILE Label::GetEnhMetaFile() const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (HENHMETAFILE)::SendMessage(m_hWnd, STM_GETIMAGE, IMAGE_ENHMETAFILE, 0L); }
HBITMAP Label::SetBitmap(HBITMAP hBitmap)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (HBITMAP)::SendMessage(m_hWnd, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap); }
HBITMAP Label::GetBitmap() const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (HBITMAP)::SendMessage(m_hWnd, STM_GETIMAGE, IMAGE_BITMAP, 0L); }
//HCURSOR Label::SetCursor(HCURSOR hCursor)
//	{ CC_ASSERT(::IsWindow(m_hWnd)); return (HCURSOR)::SendMessage(m_hWnd, STM_SETIMAGE, IMAGE_CURSOR, (LPARAM)hCursor); }
//HCURSOR Label::GetCursor()
//	{ CC_ASSERT(::IsWindow(m_hWnd)); return (HCURSOR)::SendMessage(m_hWnd, STM_GETIMAGE, IMAGE_CURSOR, 0L); }
// others
///////////////////////////////////////////////////////////////////////////////
*/

DWORD LabelConfig::nStyle = WS_CHILDWINDOW | WS_VISIBLE;
DWORD LabelConfig::nExStyle = 0;
//DWORD LabelConfig::nClassStyle = CS_DBLCLKS | CS_GLOBALCLASS | CS_PARENTDC;
DWORD LabelConfig::nInnerStyle = InnerStyle::TranceContextMenuUp;
DWORD LabelConfig::nOuterStyle = OuterStyle::AlwaysNoEdge;
DWORD LabelConfig::nOwnerDrawTextStyle = DT_LEFT|DT_WORDBREAK;
COLORREF LabelConfig::corBackground = ::GetSysColor(COLOR_BTNFACE);
COLORREF LabelConfig::corTextground = ::GetSysColor(COLOR_BTNTEXT);
COLORREF LabelConfig::corHotBackColor = ::GetSysColor(COLOR_BTNFACE);
COLORREF LabelConfig::corHotTextColor = ::GetSysColor(COLOR_BTNTEXT);
//===========================================================cc::win::Label
///////////////////////////////////////////////////////////////////////////////
// constructors/destructor
Label::Label()
{
	BASE_CONTROL_CONFIG_INIT(Label);
	_Rect.right = _Rect.left + 19;
	_Rect.bottom = _Rect.top + 12;
	::CopyRect(&_OrgRect, &_Rect);
	//_ClassName = WndClassName::Label;
	_ClassName = _T("ccLabel");
	SetText(_T("Label"));
	m_pFocusControl = NULL;
	::SetRect(&_Margin, 0, 0, 0, 0);

#ifdef _DEBUG_
	SetName(_T("Label"));
#endif
#if defined(_DEBUG_) || defined(_DESIGN)
	//code for design
	this->Design = (DesignStyle::Type)((DWORD)this->Design | DesignStyle::AutoSize | DesignStyle::Background);
#endif
}

Label::~Label()
{
}
// constructors/destructor
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// others

bool Label::DoProcessMnemonic(TCHAR tchar)
{
	if(m_pFocusControl != NULL && GetMnemonic() == tchar)
	{
		m_pFocusControl->Focus();
		return true;
	}
	return false;
}

void Label::OnPaint(PaintEventArgs *e)
{
	DRAWITEMSTRUCT data = {0};
	DrawItemEventArgs ee(e->baseArgs);
	ee.data = &data;
	HDC hDC = BeginPaint(m_hWnd, &(e->data));
	//If GetUpdateRect returns zero, the application should not call the BeginPaint and EndPaint functions.
	if(e->data.rcPaint.bottom != 0 && e->data.rcPaint.right != 0)
	{
		data.hDC = hDC;
		GetClientRect(m_hWnd, &data.rcItem);
		DWORD nInner2 = _StyleArray[(int)StyleType::Inner2];
		_StyleArray[(int)StyleType::Inner2] &= ~Inner2Style::IsMouseEnter;
		if(this->HasStyle(StyleType::Outer, OuterStyle::Break))
		{
			//CC_APP_GetDrawActor()->DrawPanel(this, &ee);
			::DrawEdge(hDC, &data.rcItem, EDGE_ETCHED|BDR_SUNKENOUTER|BDR_RAISEDINNER, BF_TOP);
		}
		else
		{
			CC_APP_GetDrawActor()->DrawControl(this, &ee);
		}
		_StyleArray[(int)StyleType::Inner2] = nInner2;
	}
	//call PaintBegin&PaintEnd at DrawToolItem
	EndPaint(m_hWnd, &(e->data));
}

Control* Label:: GetFocusControl()
{
	return m_pFocusControl;
}

void Label::SetFocusControl(Control* ctl)
{
	m_pFocusControl = ctl;
}

DWORD PictureBoxConfig::nStyle = WS_CHILDWINDOW | WS_VISIBLE | SS_BITMAP | SS_CENTERIMAGE;
DWORD PictureBoxConfig::nExStyle = WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR | WS_EX_NOPARENTNOTIFY;
//DWORD PictureBoxConfig::nClassStyle = CS_DBLCLKS | CS_GLOBALCLASS;
DWORD PictureBoxConfig::nInnerStyle = InnerStyle::TranceContextMenuUp;
DWORD PictureBoxConfig::nOuterStyle = 0;
DWORD PictureBoxConfig::nOwnerDrawTextStyle = DT_LEFT|DT_WORDBREAK;
COLORREF PictureBoxConfig::corBackground = ::GetSysColor(COLOR_BTNFACE);
COLORREF PictureBoxConfig::corTextground = ::GetSysColor(COLOR_BTNTEXT);
COLORREF PictureBoxConfig::corHotBackColor = ::GetSysColor(COLOR_BTNFACE);
COLORREF PictureBoxConfig::corHotTextColor = ::GetSysColor(COLOR_BTNTEXT);
//===========================================================cc::win::PictureBox
///////////////////////////////////////////////////////////////////////////////
//	_ClassName = _T("SysAnimate32");
PictureBox::PictureBox()
{
	BASE_CONTROL_CONFIG_INIT(PictureBox);
	m_coTransparentBack = RGB(0, 0, 0);
	SetText(NULL);
	m_hBitmap = NULL;
	m_hIcon = NULL;
	m_nEnlarge = 0;
	m_szBmpSize.cx = 0;
	m_szBmpSize.cy = 0;
#ifdef _DEBUG_
	SetName(_T("PictureBox"));
#endif
}

PictureBox::~PictureBox()
{
	Clear();
}
// constructors/destructor

void PictureBox::Clear()
{
	if(CC_BIT_HAS(_Flags, CC_FLAG_PICTURE_NEEDDESTROY))
	{
		if(m_hBitmap != NULL)
		{
			::DeleteObject(m_hBitmap);
			m_hBitmap = NULL;
		}
		if(m_hIcon != NULL)
		{
			::DestroyIcon(m_hIcon);
			m_hIcon = NULL;
		}
	}
}

COLORREF PictureBox::GetTransparentBackColor()
{
	return m_coTransparentBack;
}

void PictureBox::SetTransparentBackColor(COLORREF coTransparentBack)
{
	m_coTransparentBack = coTransparentBack;
}

int PictureBox::GetEnlarge()
{
	return m_nEnlarge;
}

void PictureBox::SetEnlarge(int nEnlarge)
{
	m_nEnlarge = nEnlarge;
}

void PictureBox::SetImage(UINT nResID, const TCHAR *resType)
{
	CC_ASSERT(resType != NULL);
	HBITMAP hBitmap = CC_APP_GetDrawActor()->GetBitmap(nResID, resType, _corBackground);
	SetImage(hBitmap, true);
}

void PictureBox::SetImage(const TCHAR* filename)
{
	CC_ASSERT(filename != NULL);
	HBITMAP hBitmap = CC_APP_GetDrawActor()->GetBitmap(filename, _corBackground);
	SetImage(hBitmap, true);
}

void PictureBox::SetImage(HBITMAP hBitmap, bool needDestroy)
{
	if(m_hBitmap == hBitmap)
	{
		CC_BIT_SET(_Flags, CC_FLAG_PICTURE_NEEDDESTROY, needDestroy);
		return;
	}
	Clear();
	m_hBitmap = hBitmap;

	BITMAP bmp;
	GetObject(m_hBitmap, sizeof(BITMAP), &bmp);
	m_szBmpSize.cx = bmp.bmWidth;
	m_szBmpSize.cy = bmp.bmHeight;

	CC_BIT_SET(_Flags, CC_FLAG_PICTURE_NEEDDESTROY, needDestroy);
	if(m_hBitmap != NULL && (_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
	{
		if(m_hWnd != NULL)
		{
			::InvalidateRect(m_hWnd, NULL, TRUE);
		}
	}
}

void PictureBox::SetImage(HICON hIcon, bool needDestroy)
{
	if(m_hIcon == hIcon)
	{
		CC_BIT_SET(_Flags, CC_FLAG_PICTURE_NEEDDESTROY, needDestroy);
		return;
	}
	Clear();
	m_hIcon = hIcon;
	//m_hIcon not need WH
	m_szBmpSize.cx = 0;
	m_szBmpSize.cy = 0;

	CC_BIT_SET(_Flags, CC_FLAG_PICTURE_NEEDDESTROY, needDestroy);
	if(m_hIcon != NULL && (_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
	{
		if(m_hWnd != NULL)
		{
			::InvalidateRect(m_hWnd, NULL, TRUE);
		}
	}
}

void PictureBox::OnPaint(PaintEventArgs *e)
{
	bool newHdc = false;
	if(e->data.hdc == NULL)
	{
		::BeginPaint(m_hWnd, &(e->data));
		newHdc = true;
	}

	//PAINTSTRUCT ps;
	//HDC hDC = BeginPaint(m_hWnd, &ps);
	HDC hDC = e->data.hdc;
	RECT rcItem;
	::GetClientRect(m_hWnd, &rcItem);
	int nH;
	int nW;
	if(m_nEnlarge > 0)
	{
		nW = m_nEnlarge * m_szBmpSize.cx;
		nH = m_nEnlarge * m_szBmpSize.cy;
	}
	else
	{
		nW = rcItem.right;
		nH = rcItem.bottom;
	}
	//for maybe draw TRANSPARENT icon or bmp,need erase back always
	//HRGN hRgn = CreateRectRgn(0, 0, nW, nH);
	//ExtSelectClipRgn(hDC, hRgn, RGN_DIFF);
	// set bkcolor
	if(!this->HasStyle(StyleType::Inner, InnerStyle::BackgroundGradient)
		&& (!this->HasStyle(StyleType::Outer, OuterStyle::TransparentParentErase)))
	{
		HBRUSH hbr = this->GetBackgroundBrush();
		::FillRect(hDC, &rcItem, hbr);
	}
	else
	{
		::SendMessage(this->m_hWnd, WM_ERASEBKGND, (WPARAM)hDC, (LPARAM)this);
	}
	//SelectClipRgn(hDC , NULL);
	//DeleteObject(hRgn);

	::SetBkMode(hDC, TRANSPARENT);
	PaintBegin.Invoke((Object*)this, e);
	if(m_hBitmap != NULL)
	{
		HDC hTargetDC = CreateCompatibleDC(hDC);
		HBITMAP hOldBitmap = (HBITMAP)SelectObject(hTargetDC, m_hBitmap);
		CC_APP_GetDrawActor()->TransparentBlt(hDC, 0, 0, nW, nH, hTargetDC, 0, 0, m_szBmpSize.cx, m_szBmpSize.cy, m_coTransparentBack);
		SelectObject(hTargetDC, hOldBitmap);
		DeleteDC(hTargetDC);
	}
	else if(m_hIcon != NULL || (_pImageList != NULL && _nIconID > 0))
	{
		if(m_hIcon != NULL)
		{
			::DrawIconEx(hDC, 0, 0, m_hIcon, nW, nH, 0, NULL, DI_NORMAL);
		}
		else
		{
			::DrawIconEx(hDC, 0, 0, GetIcon(GetEnabled() ? IconType::Hot : IconType::Disabled), nW, nH, 0, NULL, DI_NORMAL);
		}
	}
	//else
	//{
	//	DRAWITEMSTRUCT data = {0};
	//	DrawItemEventArgs ee(e->baseArgs);
	//	ee.data = &data;
	//	CC_APP_GetDrawActor()->DrawControl(this, &ee);
	//}

	if(this->HasStyle(StyleType::Outer, OuterStyle::AlwaysEdge))
	{
		::DrawEdge(hDC, &rcItem, EDGE_ETCHED|BDR_SUNKENOUTER|BDR_RAISEDINNER, BF_RECT);
	}

	PaintEnd.Invoke((Object*)this, e);
	//EndPaint(m_hWnd, &ps);
	if(newHdc)
	{
		::EndPaint(m_hWnd, &(e->data));
		memset(&(e->data), 0, sizeof(PAINTSTRUCT));
	}
}
///////////////////////////////////////////////////////////////////////////////

/*
BS_3STATE 
Creates a button that is the same as a check box, except that the box can be grayed as well as checked or cleared. Use the grayed state to show that the state of the check box is not determined.
BS_AUTO3STATE 
Creates a button that is the same as a three-state check box, except that the box changes its state when the user selects it. The state cycles through checked, grayed, and cleared.
BS_AUTOCHECKBOX 
Creates a button that is the same as a check box, except that the check state automatically toggles between checked and cleared each time the user selects the check box.
BS_AUTORADIOBUTTON 
Creates a button that is the same as a radio button, except that when the user selects it, The system automatically sets the button's check state to checked and automatically sets the check state for all other buttons in the same group to cleared.
BS_CHECKBOX 
Creates a small, empty check box with text. By default, the text is displayed to the right of the check box. To display the text to the left of the check box, combine this flag with the BS_LEFTTEXT style (or with the equivalent BS_RIGHTBUTTON style).
BS_DEFPUSHBUTTON 
Creates a push button that behaves like a BS_PUSHBUTTON style button, but also has a heavy black border. If the button is in a dialog box, the user can select the button by pressing the ENTER key, even when the button does not have the input focus. This style is useful for enabling the user to quickly select the most likely (default) option.
BS_GROUPBOX 
Creates a rectangle in which other controls can be grouped. Any text associated with this style is displayed in the rectangle's upper left corner.
TOTICE:your should remove BS_CENTER from style!(by shu.kk 20080510)
or like:
ctl->SetStyle(-1, WS_OVERLAPPED | WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_DEFPUSHBUTTON | BS_CHECKBOX | BS_RADIOBUTTON | BS_TEXT);
BS_LEFTTEXT 
Places text on the left side of the radio button or check box when combined with a radio button or check box style. Same as the BS_RIGHTBUTTON style.
BS_OWNERDRAW 
Creates an owner-drawn button. The owner window receives a WM_DRAWITEM message when a visual aspect of the button has changed. Do not combine the BS_OWNERDRAW style with any other button styles.
BS_PUSHBUTTON 
Creates a push button that posts a WM_COMMAND message to the owner window when the user selects the button.
BS_RADIOBUTTON 
Creates a small circle with text. By default, the text is displayed to the right of the circle. To display the text to the left of the circle, combine this flag with the BS_LEFTTEXT style (or with the equivalent BS_RIGHTBUTTON style). Use radio buttons for groups of related, but mutually exclusive choices.
BS_USERBUTTON 
Obsolete, but provided for compatibility with 16-bit versions of Windows. Applications should use BS_OWNERDRAW instead.
BS_BITMAP 
Specifies that the button displays a bitmap. See the Remarks section for its interaction with BS_ICON.
BS_BOTTOM 
Places text at the bottom of the button rectangle.
BS_CENTER 
Centers text horizontally in the button rectangle.
BS_ICON 
Specifies that the button displays an icon. See the Remarks section for its interaction with BS_BITMAP.
BS_FLAT 
Specifies that the button is two-dimensional; it does not use the default shading to create a 3-D image. 
BS_LEFT 
Left-justifies the text in the button rectangle. However, if the button is a check box or radio button that does not have the BS_RIGHTBUTTON style, the text is left justified on the right side of the check box or radio button.
BS_MULTILINE 
Wraps the button text to multiple lines if the text string is too long to fit on a single line in the button rectangle.
BS_NOTIFY 
Enables a button to send BN_KILLFOCUS and BN_SETFOCUS notification messages to its parent window. 
Note that buttons send the BN_CLICKED notification message regardless of whether it has this style. To get BN_DBLCLK notification messages, the button must have the BS_RADIOBUTTON or BS_OWNERDRAW style.
BS_PUSHLIKE 
Makes a button (such as a check box, three-state check box, or radio button) look and act like a push button. The button looks raised when it isn't pushed or checked, and sunken when it is pushed or checked.
BS_RIGHT 
Right-justifies text in the button rectangle. However, if the button is a check box or radio button that does not have the BS_RIGHTBUTTON style, the text is right justified on the right side of the check box or radio button.
BS_RIGHTBUTTON 
Positions a radio button's circle or a check box's square on the right side of the button rectangle. Same as the BS_LEFTTEXT style.
BS_TEXT 
Specifies that the button displays text.
BS_TOP 
Places text at the top of the button rectangle.
BS_TYPEMASK 
Microsoft Windows 2000: A composite style bit that results from using the OR operator on BS_* style bits. It can be used to mask out valid BS_* bits from a given bitmask. Note that this is out of date and does not correctly include all valid styles. Thus, you should not use this style. 
BS_VCENTER 
Places text in the middle (vertically) of the button rectangle.
BS_SPLITBUTTON 
BS_DEFSPLITBUTTON 
BS_COMMANDLINK 
BS_DEFCOMMANDLINK 
Remarks
The appearance of text or an icon or both on a button control depends on the BS_ICON and BS_BITMAP styles, and whether the BM_SETIMAGE message is called. The possible results are as follows: 
BS_ICON or BS_BITMAP set? BM_SETIMAGE called? Result 
Yes Yes Show icon only. 
No Yes Show icon and text. 
Yes No Show text only. 
No No Show text only 
*/
DWORD ButtonConfig::nStyle = WS_CHILDWINDOW | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON | BS_TEXT | BS_CENTER;
DWORD ButtonConfig::nExStyle = WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR | WS_EX_NOPARENTNOTIFY;// | WS_EX_TRANSPARENT;
//DWORD ButtonConfig::nClassStyle = CS_DBLCLKS | CS_GLOBALCLASS | CS_PARENTDC;
DWORD ButtonConfig::nInnerStyle = InnerStyle::TranceContextMenuUp;
DWORD ButtonConfig::nOuterStyle = OuterStyle::AlwaysEdge|OuterStyle::PushButton;
DWORD ButtonConfig::nOwnerDrawTextStyle = DT_CENTER|DT_VCENTER|DT_WORDBREAK;
COLORREF ButtonConfig::corBackground = ::GetSysColor(COLOR_BTNFACE);
COLORREF ButtonConfig::corTextground = ::GetSysColor(COLOR_BTNTEXT);
COLORREF ButtonConfig::corHotBackColor = ::GetSysColor(COLOR_BTNFACE);
COLORREF ButtonConfig::corHotTextColor = ::GetSysColor(COLOR_BTNTEXT);
//===========================================================cc::win::Button
///////////////////////////////////////////////////////////////////////////////
// constructors/destructor
Button::Button()
{
	BASE_CONTROL_CONFIG_INIT(Button);
	_Rect.right = _Rect.left + 88;
	_Rect.bottom = _Rect.top + 19;
	::CopyRect(&_OrgRect, &_Rect);
	_ClassName = WndClassName::Button;
	SetText(_T("Button"));
#ifdef _DEBUG_
	SetName(_T("Button"));
#endif
}

Button::~Button()
{
}
// constructors/destructor

void Button::OnCreateControl()
{
	Control::OnCreateControl();
	if(GetIcon() != NULL)
	{
		HICON icon = (HICON)::SendMessage(m_hWnd, BM_SETIMAGE, IMAGE_ICON, (LPARAM)GetIcon());
		if(icon != NULL)
		{
			::DestroyIcon(icon);
		}
	}
}

//auto id for buttons(checkbox or radiobutton)
static UINT CC_Button_Id = 1;
bool Button::PreCreateWindow(CREATESTRUCT& cs)
{
	CC_Button_Id++;
	if(CC_Button_Id <= 0)
	{
		CC_Button_Id = 1;
	}
	cs.hMenu = (HMENU)(DWORD_PTR)CC_Button_Id;
	return Control::PreCreateWindow(cs);
}

void Button::DoUpdate()
{
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0 &&
		IsUpdate((UpdateFlag::Type)(UpdateFlag::UpdateText|UpdateFlag::UpdateIcon|UpdateFlag::UpdateBackground|UpdateFlag::UpdateTextColor)))
	{
		CC_ASSERT(::IsWindow(m_hWnd));

		if(IsUpdate((UpdateFlag::Type)(UpdateFlag::UpdateIcon)))
		{
			HICON icon = (HICON)::SendMessage(m_hWnd, BM_SETIMAGE, IMAGE_ICON, (LPARAM)GetIcon());
			if(icon != NULL)
			{
				::DestroyIcon(icon);
			}
		}
		if(m_hWnd != NULL)
		{
			::InvalidateRect(m_hWnd, NULL, FALSE);
		}
	}
	Control::DoUpdate();
}

bool Button::DoProcessMnemonic(TCHAR tchar)
{
	if(GetMnemonic() == tchar)
	{
		if(m_hWnd != NULL && ::IsWindowEnabled(m_hWnd) == TRUE && ::IsWindowVisible(m_hWnd) == TRUE)
		{
			Focus();
			::SendMessage(m_hWnd, BM_CLICK, 0, 0);
			return true;
		}
	}
	return false;
}

void Button::PerformClick(EventArgs *e)
{
	if(m_hWnd != NULL && ::IsWindowEnabled(m_hWnd) == TRUE && ::IsWindowVisible(m_hWnd) == TRUE)
	{
		OnClick(e);
	}
}

void Button::OnCommand(EventArgs* e)
{
	switch(HIWORD(e->WParam))
	{
	case BN_CLICKED:
//CC_TRACE((_T("Button::OnCommand:BN_CLICKED")));
		OnClick(e);
		break;
	case BN_PAINT:
//CC_TRACE((_T("Button::OnCommand:BN_PAINT")));
		break;
	case BN_HILITE:
//CC_TRACE((_T("Button::OnCommand:BN_HILITE")));
		break;
	case BN_UNHILITE:
//CC_TRACE((_T("Button::OnCommand:BN_UNHILITE")));
		break;
	case BN_DISABLE:
//CC_TRACE((_T("Button::OnCommand:BN_DISABLE")));
		break;
	case BN_DOUBLECLICKED:
//CC_TRACE((_T("Button::OnCommand:BN_DOUBLECLICKED")));
		OnDoubleClick(e);
		break;
		case BN_SETFOCUS:
//CC_TRACE((_T("Button::OnCommand:BN_SETFOCUS")));
		OnEnter(e);
		OnGotFocus(e);
		break;
	case BN_KILLFOCUS:
//CC_TRACE((_T("Button::OnCommand:BN_KILLFOCUS")));
		OnLeave(e);
		OnLostFocus(e);
		break;
	}
}

void Button::OnChildNotify(EventArgs *e)
{
	if(e->Msg != WM_DRAWITEM)
	{
		return Control::OnChildNotify(e);
	}

	DrawItemEventArgs ee(e);
	ee.data = (LPDRAWITEMSTRUCT)e->LParam;
	OnDrawItem(&ee);
}

void Button::OnDrawItem(DrawItemEventArgs* e)
{
	if(DrawItem.IsEmpty())
	{
		e->dwStyle |= CC_DRAWITEM_BUTTON;
		if((_StyleArray[(int)StyleType::Style] & (BS_CHECKBOX|BS_AUTOCHECKBOX)) != 0)
		{
			e->dwStyle |= CC_DRAWITEM_CHECKBOX;
			if((_StyleArray[(int)StyleType::Style] & (BS_3STATE|BS_AUTO3STATE)) != 0)
			{
				e->dwStyle |= CC_DRAWITEM_3STATE;
			}
		}
		else if((_StyleArray[(int)StyleType::Style] & (BS_RADIOBUTTON)) != 0)
		{
			e->dwStyle |= CC_DRAWITEM_RADIOBUTTON;
		}
		else if((_StyleArray[(int)StyleType::Style] & (BS_GROUPBOX)) != 0)
		{
			e->dwStyle |= CC_DRAWITEM_GROUPBOX;
		}
		CC_APP_GetDrawActor()->DrawControl(this, e);
	}
	else
	{
		DrawItem.Invoke((Object*)this, e);
	}
}

//UINT Button::GetState() const
//	{ return (UINT)::SendMessage(m_hWnd, BM_GETSTATE, 0, 0); }
//void Button::SetState(BOOL bHighlight)
//	{ ::SendMessage(m_hWnd, BM_SETSTATE, bHighlight, 0); }
// Win4
//HICON Button::SetIcon(HICON hIcon)
//	{ CC_ASSERT(::IsWindow(m_hWnd)); return (HICON)::SendMessage(m_hWnd, BM_SETIMAGE, IMAGE_ICON, (LPARAM)hIcon); }
//HICON Button::GetIcon() const
//	{ CC_ASSERT(::IsWindow(m_hWnd)); return (HICON)::SendMessage(m_hWnd, BM_GETIMAGE, IMAGE_ICON, 0L); }
HBITMAP Button::SetBitmap(HBITMAP hBitmap)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (HBITMAP)::SendMessage(m_hWnd, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap); }
HBITMAP Button::GetBitmap() const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (HBITMAP)::SendMessage(m_hWnd, BM_GETIMAGE, IMAGE_BITMAP, 0L); }
///////////////////////////////////////////////////////////////////////////////

DWORD CheckBoxConfig::nStyle = WS_CHILDWINDOW | WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX;
DWORD CheckBoxConfig::nExStyle = WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR | WS_EX_NOPARENTNOTIFY;
//DWORD CheckBoxConfig::nClassStyle = CS_DBLCLKS | CS_GLOBALCLASS | CS_PARENTDC;
DWORD CheckBoxConfig::nInnerStyle = InnerStyle::TranceContextMenuUp;
DWORD CheckBoxConfig::nOuterStyle = OuterStyle::AutoSize;
DWORD CheckBoxConfig::nOwnerDrawTextStyle = DT_LEFT|DT_VCENTER|DT_WORDBREAK;
COLORREF CheckBoxConfig::corBackground = ::GetSysColor(COLOR_BTNFACE);
COLORREF CheckBoxConfig::corTextground = ::GetSysColor(COLOR_BTNTEXT);
COLORREF CheckBoxConfig::corHotBackColor = ::GetSysColor(COLOR_BTNFACE);
COLORREF CheckBoxConfig::corHotTextColor = ::GetSysColor(COLOR_BTNTEXT);
//===========================================================cc::win::CheckBox
///////////////////////////////////////////////////////////////////////////////
// constructors/destructor
CheckBox::CheckBox()
{
	BASE_CONTROL_CONFIG_INIT(CheckBox);
	_Rect.right = _Rect.left + 88;
	_Rect.bottom = _Rect.top + 18;
	::CopyRect(&_OrgRect, &_Rect);
	::SetRect(&_Margin, 0, 1, 0, 1);
	SetText(_T("CheckBox"));
#ifdef _DEBUG_
	SetName(_T("CheckBox"));
#endif
	//_IsChecked = false;
}

CheckBox::~CheckBox()
{
}
// constructors/destructor
///////////////////////////////////////////////////////////////////////////////
// others
//keyArr is param address,but sHead is by value
void CheckBox::DoConfig(bool isFromIni, KeyArr& keyArr, cc::Str sThisHead)
{
	if(!HasStyle(StyleType::Outer, OuterStyle::AutoConfig))
	{
		return;
	}
	if(isFromIni)
	{
		//if not exist,as usually
		if(keyArr.Exist(sThisHead))
		{
			SetChecked(keyArr.GetBool(sThisHead, false));
		}
	}
	else
	{
		keyArr.SetBool(sThisHead, GetChecked());
	}
}

LRESULT CheckBox::WndProc(EventArgs *e)
{
	switch(e->Msg)
	{
	case BM_SETCHECK:
		OnCheckedChanged(e);
		break;
	}
	return Control::WndProc(e);
}

void CheckBox::OnHandleCreated(EventArgs* e)
{
	Control::OnHandleCreated(e);
	if(HasStyle(StyleType::Inner, InnerStyle::Pushed))
	{
		SetChecked(true);
	}
}

/*
the check status will change to(is different to now status!):
wParam
    Specifies the check state. This parameter can be one of the following values.
    BST_CHECKED
        Sets the button state to checked.
    BST_INDETERMINATE
        Sets the button state to grayed, indicating an indeterminate state. Use this value only if the button has the BS_3STATE or BS_AUTO3STATE style.
    BST_UNCHECKED
        Sets the button state to cleared.
*/
void CheckBox::OnCheckedChanged(EventArgs* e)
{
	SetStyle(StyleType::Inner, 0, InnerStyle::IsDirty);
	CheckedChanged.Invoke((Object*)this, e);
}

void CheckBox::SetChecked(bool checked)
{
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
	{
		::SendMessage(m_hWnd, BM_SETCHECK, checked ? BST_CHECKED : BST_UNCHECKED, 0);
	}
	else
	{
		SetStyleBool(StyleType::Inner, InnerStyle::Pushed, checked);
	}
}

bool CheckBox::GetChecked()
{
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
	{
		return ((int)::SendMessage(m_hWnd, BM_GETCHECK, 0, 0) != 0);
	}
	return HasStyle(StyleType::Inner, InnerStyle::Pushed);
}

void CheckBox::SetIsDirty(bool isDirty)
{
	SetStyleBool(StyleType::Inner, InnerStyle::IsDirty, isDirty);
}

bool CheckBox::GetIsDirty()
{
	return HasStyle(StyleType::Inner, InnerStyle::IsDirty);
}

SIZE CheckBox::DoMeasureSize(int ctlHeight)
{
	_nExpectCY = ctlHeight;
	SIZE size = CC_APP_GetDrawActor()->MeasureControl(this, ctlHeight);
	size.cx += CC_CHECKBOX_BOXWH + 6;
	return size;
}
// others
///////////////////////////////////////////////////////////////////////////////

DWORD RadioButtonConfig::nStyle = WS_CHILDWINDOW | WS_VISIBLE | WS_TABSTOP | BS_AUTORADIOBUTTON;
DWORD RadioButtonConfig::nExStyle = WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR | WS_EX_NOPARENTNOTIFY;
//DWORD RadioButtonConfig::nClassStyle = CS_DBLCLKS | CS_GLOBALCLASS | CS_PARENTDC;
DWORD RadioButtonConfig::nInnerStyle = InnerStyle::TranceContextMenuUp;
DWORD RadioButtonConfig::nOuterStyle = OuterStyle::AutoSize;
DWORD RadioButtonConfig::nOwnerDrawTextStyle = DT_LEFT|DT_VCENTER|DT_WORDBREAK;
COLORREF RadioButtonConfig::corBackground = ::GetSysColor(COLOR_BTNFACE);
COLORREF RadioButtonConfig::corTextground = ::GetSysColor(COLOR_BTNTEXT);
COLORREF RadioButtonConfig::corHotBackColor = ::GetSysColor(COLOR_BTNFACE);
COLORREF RadioButtonConfig::corHotTextColor = ::GetSysColor(COLOR_BTNTEXT);
//===========================================================cc::win::RadioButton
///////////////////////////////////////////////////////////////////////////////
// constructors/destructor
RadioButton::RadioButton()
{
	BASE_CONTROL_CONFIG_INIT(RadioButton);
	//group ID
	//_hMenu = (HMENU)1;
	_Rect.right = _Rect.left + 77;
	_Rect.bottom = _Rect.top + 15;
	::CopyRect(&_OrgRect, &_Rect);
	::SetRect(&_Margin, 0, 1, 0, 1);
	SetText(_T("RadioButton"));
#ifdef _DEBUG_
	SetName(_T("RadioButton"));
#endif
}

RadioButton::~RadioButton()
{
}

//keyArr is param address,but sHead is by value
void RadioButton::DoConfig(bool isFromIni, KeyArr& keyArr, cc::Str sThisHead)
{
	if(!HasStyle(StyleType::Outer, OuterStyle::AutoConfig))
	{
		return;
	}
	if(isFromIni)
	{
		//if not exist,as usually
		if(keyArr.Exist(sThisHead))
		{
			SetChecked(keyArr.GetBool(sThisHead, false));
			//notice here not doclick even checked on
		}
	}
	else
	{
		keyArr.SetBool(sThisHead, GetChecked());
	}
}

LRESULT RadioButton::WndProc(EventArgs *e)
{
	switch(e->Msg)
	{
	case BM_SETCHECK:
		OnCheckedChanged(e);
		break;
	}
	return Control::WndProc(e);
}

void RadioButton::OnHandleCreated(EventArgs* e)
{
	Control::OnHandleCreated(e);
	if(HasStyle(StyleType::Inner, InnerStyle::Pushed))
	{
		SetChecked(true);
	}
}

/*
the check status will change to(is different to now status!):
wParam
    Specifies the check state. This parameter can be one of the following values.
    BST_CHECKED
        Sets the button state to checked.
    BST_INDETERMINATE
        Sets the button state to grayed, indicating an indeterminate state. Use this value only if the button has the BS_3STATE or BS_AUTO3STATE style.
    BST_UNCHECKED
        Sets the button state to cleared.
*/
void RadioButton::OnCheckedChanged(EventArgs* e)
{
	CheckedChanged.Invoke((Object*)this, e);
}

void RadioButton::SetChecked(bool checked)
{
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
	{
		if(GetChecked() != checked)
		{
			::SendMessage(m_hWnd, BM_SETCHECK, checked ? BST_CHECKED : BST_UNCHECKED, 0);
			//If the button is in a dialog box and the dialog box is not active, the BM_CLICK message might fail.(SetActiveWindow)
			//::SendMessage(m_hWnd, BM_CLICK, 0, 0);
			if(!checked && this->Focused() && _pParent != NULL)
			{
				_pParent->Focus();
			}
		}
	}
	else
	{
		SetStyleBool(StyleType::Inner, InnerStyle::Pushed, checked);
	}
}

bool RadioButton::GetChecked()
{
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
	{
		return ((int)::SendMessage(m_hWnd, BM_GETCHECK, 0, 0) != 0);
	}
	return HasStyle(StyleType::Inner, InnerStyle::Pushed);
}

//void RadioButton::SetGroupID(UINT nID)
//{
//	_hMenu = (HMENU)nID;
//	if(HasStyle(StyleType::Inner2, Inner2Style::HandleCreated))
//	{
//		::SetMenu(m_hWnd, _hMenu);
//	}
//}
//
//UINT RadioButton::GetGroupID()
//{
//	return (UINT)_hMenu;
//}
//
SIZE RadioButton::DoMeasureSize(int ctlHeight)
{
	_nExpectCY = ctlHeight;
	SIZE size = CC_APP_GetDrawActor()->MeasureControl(this, ctlHeight);
	size.cx += 16;
	return size;
}
// constructors/destructor
///////////////////////////////////////////////////////////////////////////////

DWORD GroupConfig::nStyle = WS_CHILDWINDOW | WS_VISIBLE | WS_TABSTOP | BS_GROUPBOX;
DWORD GroupConfig::nExStyle = 0;
//DWORD GroupConfig::nClassStyle = CS_DBLCLKS | CS_GLOBALCLASS | CS_PARENTDC;
DWORD GroupConfig::nInnerStyle = InnerStyle::TranceContextMenuUp;
DWORD GroupConfig::nOuterStyle = 0;
DWORD GroupConfig::nOwnerDrawTextStyle = DT_LEFT|DT_SINGLELINE;
COLORREF GroupConfig::corBackground = ::GetSysColor(COLOR_BTNFACE);
COLORREF GroupConfig::corTextground = ::GetSysColor(COLOR_BTNTEXT);
COLORREF GroupConfig::corHotBackColor = ::GetSysColor(COLOR_BTNFACE);
COLORREF GroupConfig::corHotTextColor = ::GetSysColor(COLOR_BTNTEXT);
//===========================================================cc::win::Group
///////////////////////////////////////////////////////////////////////////////
// constructors/destructor
Group::Group()
{
	BASE_CONTROL_CONFIG_INIT(Group);
	_Rect.right = _Rect.left + 84;
	_Rect.bottom = _Rect.top + 60;
	::CopyRect(&_OrgRect, &_Rect);
	SetText(_T("Group"));
#ifdef _DEBUG_
	SetName(_T("Group"));
#endif
}

Group::~Group()
{
}

//if not stop even WS_TABSTOP&WS_VISIBLE,need overwrite this
bool Group::IsTabStop()
{
	CC_ASSERT(::IsWindow(m_hWnd));

	int nCnt;
	if(Control::IsTabStop() && _lstControls != NULL && (nCnt = _lstControls->GetSize()) > 0)
	{
		for(int n = 0; n < nCnt; n++)
		{
			Control* ctl = _lstControls->GetKey(n);
			if(ctl->IsTabStop())
			{
				return true;
			}
		}
	}

	if(_pClient != NULL)
	{
		return _pClient->IsTabStop();
	}
	return false;
}
// constructors/destructor
///////////////////////////////////////////////////////////////////////////////

/*
UDS_HORZ   Causes the contro's arrows to point left and right instead of up and down.
UDS_WRAP   Causes the position to "wrap" if it is incremented or decremented beyond the ending or beginning of the range.
UDS_ARROWKEYS   Causes the control to increment and decrement the position when the UP ARROW and DOWN ARROW keys are pressed.
UDS_SETBUDDYINT   Causes the control to set the text of the buddy window (using the WM_SETTEXT message) when the position changes.
	The text consists of the position formatted as a decimal or hexadecimal string.
UDS_NOTHOUSANDS   Does not insert a thousands separator between every three decimal digits.
UDS_AUTOBUDDY   Automatically selects the previous window in the Z-order as the contro's buddy window.
UDS_ALIGNRIGHT   Positions the spin button control next to the right edge of the buddy window. The width of the buddy window is decreased to accommodate the width of the control.
UDS_ALIGNLEFT   Positions the spin button control next to the left edge of the buddy window. The buddy window is moved to the right and its width decreased to accommodate the width of the control.
*/
DWORD SpinButtonConfig::nStyle = UDS_ARROWKEYS | UDS_SETBUDDYINT | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE | UDS_NOTHOUSANDS;
DWORD SpinButtonConfig::nExStyle = WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR | WS_EX_NOPARENTNOTIFY;
//DWORD SpinButtonConfig::nClassStyle = CS_DBLCLKS | CS_GLOBALCLASS | CS_PARENTDC;
DWORD SpinButtonConfig::nInnerStyle = InnerStyle::TranceContextMenuUp;
DWORD SpinButtonConfig::nOuterStyle = 0;
DWORD SpinButtonConfig::nOwnerDrawTextStyle = DT_LEFT|DT_VCENTER|DT_SINGLELINE;
COLORREF SpinButtonConfig::corBackground = ::GetSysColor(COLOR_BTNFACE);
COLORREF SpinButtonConfig::corTextground = ::GetSysColor(COLOR_BTNTEXT);
COLORREF SpinButtonConfig::corHotBackColor = ::GetSysColor(COLOR_BTNFACE);
COLORREF SpinButtonConfig::corHotTextColor = ::GetSysColor(COLOR_BTNTEXT);
//===========================================================cc::win::SpinButton
///////////////////////////////////////////////////////////////////////////////
// constructors/destructor
SpinButton::SpinButton()
{
	BASE_CONTROL_CONFIG_INIT(SpinButton);
	_Rect.right = _Rect.left + 17;
	_Rect.bottom = _Rect.top + 17;
	::CopyRect(&_OrgRect, &_Rect);
	_ClassName = WndClassName::SpinButton;
	//not need text
	//SetText(_T("SpinButton"));
#ifdef _DEBUG_
	SetName(_T("SpinButton"));
#endif
	_nLower = UD_MINVAL;
	_nUpper = UD_MAXVAL;
	_nPos = UD_MINVAL;
	_nBase = 10;
	_hBuddy = NULL;
}

SpinButton::~SpinButton()
{
}
// constructors/destructor

//keyArr is param address,but sHead is by value
void SpinButton::DoConfig(bool isFromIni, KeyArr& keyArr, cc::Str sThisHead)
{
	if(!HasStyle(StyleType::Outer, OuterStyle::AutoConfig))
	{
		return;
	}
	if(isFromIni)
	{
		//if not exist,as usually
		if(keyArr.Exist(sThisHead))
		{
			SetPos((int)keyArr.Get(sThisHead, 0));
		}
	}
	else
	{
		keyArr.Set(sThisHead, (DWORD)GetPos());
	}
}

void SpinButton::OnHandleCreated(EventArgs* e)
{
	Control::OnHandleCreated(e);
	SetRange(_nLower, _nUpper);
	SetBuddy(_hBuddy);
	SetPos(_nPos);
}

void SpinButton::GetRange(int& lower, int& upper)
{
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
	{
		LRESULT dw = ::SendMessage(m_hWnd, UDM_GETRANGE, 0, 0l);
		_nLower = (int)(short)HIWORD(dw);
		_nUpper = (int)(short)LOWORD(dw);
	}
	lower = _nLower;
	upper = _nUpper;
}

void SpinButton::SetRange(int lower, int upper)
{
	_nLower = lower;
	_nUpper = upper;
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
	{
		::SendMessage(m_hWnd, UDM_SETRANGE, 0, _nUpper | (_nLower << 16));
	}
}

int SpinButton::GetBase()
{
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
	{
		_nBase = (int)::SendMessage(m_hWnd, UDM_GETBASE, 0, 0);
	}
	return _nBase;
}

void SpinButton::SetBase(int value)
{
	_nBase = value;
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
	{
		::SendMessage(m_hWnd, UDM_SETBASE, (WPARAM)_nBase, 0);
	}
}

Control* SpinButton::GetBuddy()
{
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
	{
		_hBuddy = Control::FromHandle((HWND)::SendMessage(m_hWnd, UDM_GETBUDDY, 0, 0));
	}
	return _hBuddy;
}

void SpinButton::SetBuddy(Control* ctl)
{
	_hBuddy = ctl;
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0 && _hBuddy != NULL)
	{
		::SendMessage(m_hWnd, UDM_SETBUDDY, (WPARAM)_hBuddy->m_hWnd, 0);
	}
}

int SpinButton::GetPos()
{
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
	{
		_nPos = (int)::SendMessage(m_hWnd, UDM_GETPOS, 0, 0);
	}
	return _nPos;
}

void SpinButton::SetPos(int value)
{
	_nPos = value;
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
	{
		::SendMessage(m_hWnd, UDM_SETPOS, 0, (LPARAM)MAKELONG(_nPos, 0));
	}
}

///////////////////////////////////////////////////////////////////////////////

/*
LBS_DISABLENOSCROLL   The list box shows a disabled vertical scroll bar when the list box does not contain enough items to scroll. Without this style, the scroll bar is hidden when the list box does not contain enough items.
LBS_EXTENDEDSEL   The user can select multiple items using the SHIFT key and the mouse or special key combinations.
LBS_HASSTRINGS   Specifies an owner-draw list box that contains items consisting of strings. The list box maintains the memory and pointers for the strings so the application can use the GetText member function to retrieve the text for a particular item.
LBS_MULTICOLUMN   Specifies a multicolumn list box that is scrolled horizontally. The SetColumnWidth member function sets the width of the columns.
LBS_MULTIPLESEL   String selection is toggled each time the user clicks or double-clicks the string. Any number of strings can be selected.
LBS_NODATA   Specifies a no-data list box. Specify this style when the count of items in the list box will exceed one thousand.
  A no-data list box must also have the LBS_OWNERDRAWFIXED style, but must not have the LBS_SORT or LBS_HASSTRINGS style.
  A no-data list box resembles an owner-drawn list box except that it contains no string or bitmap data for an item. Commands to add,
  insert, or delete an item always ignore any given item data; requests to find a string within the list box always fail. The system
  sends the WM_DRAWITEM message to the owner window when an item must be drawn. The itemID member of the DRAWITEMSTRUCT structure passed
  with the WM_DRAWITEM message specifies the line number of the item to be drawn. A no-data list box does not send a WM_DELETEITEM message.
LBS_NOINTEGRALHEIGHT   The size of the list box is exactly the size specified by the application when it created the list box. Usually, Windows sizes a list box so that the list box does not display partial items.
LBS_NOREDRAW   List-box display is not updated when changes are made. This style can be changed at any time by sending a WM_SETREDRAW message.
LBS_NOSEL   Specifies that the list box contains items that can be viewed but not selected.
LBS_NOTIFY   Parent window receives an input message whenever the user clicks or double-clicks a string.
LBS_OWNERDRAWFIXED   The owner of the list box is responsible for drawing its contents; the items in the list box are the same height.
LBS_OWNERDRAWVARIABLE   The owner of the list box is responsible for drawing its contents; the items in the list box are variable in height.
LBS_SORT   Strings in the list box are sorted alphabetically.
LBS_STANDARD   Strings in the list box are sorted alphabetically, and the parent window receives an input message whenever the user clicks or double-clicks a string. The list box contains borders on all sides.
LBS_USETABSTOPS   Allows a list box to recognize and expand tab characters when drawing its strings. The default tab positions are 32 dialog units.
  (A dialog unit is a horizontal or vertical distance. One horizontal dialog unit is equal to one-fourth of the current dialog base width unit.
  The dialog base units are computed based on the height and width of the current system font. The GetDialogBaseUnits Windows function returns
  the current dialog base units in pixels.) This style should not be used with LBS_OWNERDRAWFIXED.
LBS_WANTKEYBOARDINPUT   The owner of the list box receives WM_VKEYTOITEM or WM_CHARTOITEM messages whenever the user presses a key
  while the list box has input focus. This allows an application to perform special processing on the keyboard input.
*/
//!!!20081016, Error for:LBS_WANTKEYBOARDINPUT!!!
DWORD ListBoxConfig::nStyle = WS_CHILDWINDOW | WS_VISIBLE | WS_TABSTOP | LBS_NOTIFY | LBS_NOINTEGRALHEIGHT|WS_HSCROLL|WS_VSCROLL|LBS_HASSTRINGS;
DWORD ListBoxConfig::nExStyle = WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR | WS_EX_NOPARENTNOTIFY | WS_EX_CLIENTEDGE;
//DWORD ListBoxConfig::nClassStyle = CS_DBLCLKS | CS_GLOBALCLASS | CS_PARENTDC;
DWORD ListBoxConfig::nInnerStyle = InnerStyle::TranceContextMenuUp;
DWORD ListBoxConfig::nOuterStyle = 0;
DWORD ListBoxConfig::nOwnerDrawTextStyle = DT_LEFT|DT_VCENTER|DT_SINGLELINE;
COLORREF ListBoxConfig::corBackground = ::GetSysColor(COLOR_WINDOW);
COLORREF ListBoxConfig::corTextground = ::GetSysColor(COLOR_WINDOWTEXT);
COLORREF ListBoxConfig::corHotBackColor = ::GetSysColor(COLOR_WINDOW);
COLORREF ListBoxConfig::corHotTextColor = ::GetSysColor(COLOR_WINDOWTEXT);
//===========================================================cc::win::ListBox
///////////////////////////////////////////////////////////////////////////////
// constructors/destructor
ListBox::ListBox()
{
	BASE_CONTROL_CONFIG_INIT(ListBox);
	_StyleArray[(int)StyleType::Inner2] |= Inner2Style::IsScrollRepaint;
	_Rect.right = _Rect.left + 84;
	_Rect.bottom = _Rect.top + 60;
	::CopyRect(&_OrgRect, &_Rect);
	_ClassName = WndClassName::ListBox;
	SetText(NULL);
	_StyleArray[(int)StyleType::InitControl] = CC_WNDCOMMCTLSNEW_REG;
#ifdef _DEBUG_
	SetName(_T("ListBox"));
#endif
}

ListBox::~ListBox()
{
}
// constructors/destructor

//keyArr is param address,but sHead is by value
void ListBox::DoConfig(bool isFromIni, KeyArr& keyArr, cc::Str sThisHead)
{
	if(!HasStyle(StyleType::Outer, OuterStyle::AutoConfig))
	{
		return;
	}
	int nInd;
	cc::Str sData;
	if(isFromIni)
	{
		//if not exist,as usually
		if(keyArr.Exist(sThisHead))
		{
			if(HasStyle(StyleType::Inner2, Inner2Style::AutoConfigListData))
			{
				sData = keyArr.Get(sThisHead + _T(".Data"));
				SetConfigData(sData);
			}

			nInd = (int)keyArr.Get(sThisHead, 0);
			if(nInd >= 0)
			{
				DefWndProc(LB_SETCURSEL, (WPARAM)nInd, 0);
			}
		}
	}
	else
	{
		nInd = (int)DefWndProc(LB_GETCURSEL, 0, 0);
		keyArr.Set(sThisHead, (DWORD)nInd);
		if(HasStyle(StyleType::Inner2, Inner2Style::AutoConfigListData))
		{
			sData = GetConfigData();
			keyArr.Set(sThisHead + _T(".Data"), sData);
		}
	}
}

void ListBox::DoSetLang(KeyArr& keyArr, cc::Str sThisHead)
{
	Control::DoSetLang(keyArr, sThisHead);
	sThisHead.Append(_T(".Data"));
	if(keyArr.Exist(sThisHead))
	{
		cc::Str sData = keyArr.Get(sThisHead);
		if(m_hWnd != NULL)
		{
			SetConfigData(sData);
		}
		else
		{
			SetText((const TCHAR*)sData.GetBuf());
		}
	}
}

cc::Str ListBox::GetConfigData()
{
	CC_ASSERT(::IsWindow(m_hWnd));

	cc::Str sData;
	int nCnt = (int)DefWndProc(LB_GETCOUNT, 0, 0);
	for(int i = 0; i < nCnt; i++)
	{
		sData.Append(ItemGet(i));
		sData.Append(_T(CONTROL_LINE_SPLIT));
	}
	return sData;
}

void ListBox::SetConfigData(const cc::Str& sData, bool isAppend)
{
	CC_ASSERT(::IsWindow(m_hWnd));

	if(sData.IndexOf(_T(CONTROL_LINE_SPLIT)) >= 0)
	{
		int nInd = (int)DefWndProc(LB_GETCURSEL, 0, 0);
		if(!isAppend)
		{
			DefWndProc(LB_RESETCONTENT, 0, 0);
		}
		cc::Str* strArr = NULL;
		int nCnt = sData.Split(_T(CONTROL_LINE_SPLIT), &strArr);
		for(int i = 0; i < nCnt; i++)
		{
			ItemAdd((TCHAR*)strArr[i].GetBuf());
		}
		if(strArr != NULL)
		{
			delete []strArr;
		}
		DefWndProc(LB_SETCURSEL, (WPARAM)nInd, 0);
	}
}

void ListBox::OnHandleCreated(EventArgs *e)
{
	CC_ASSERT(::IsWindow(m_hWnd));

	Control::OnHandleCreated(e);
	SetConfigData(cc::Str(_Text), false);
}

void ListBox::SetSelectedIndex(int selectedIndex)
{
	CC_ASSERT(::IsWindow(m_hWnd));

//http://www.yesky.com/242/1603242_1.shtml
//UINT nItem;
//nItem=SendMessage(hlist,LB_ADDSTRING,0,(LPARAM)lpstr);
//SendMessage(hlist,LB_SETITEMDATA,nItem,(LPARAM)hbm);
	SendMessage(m_hWnd, LB_SETCURSEL, (WPARAM)selectedIndex, (LPARAM)0L);
}

int ListBox::GetSelectedIndex() const
{
	CC_ASSERT(::IsWindow(m_hWnd));

	return (int)SendMessage(m_hWnd, LB_GETCURSEL, 0, 0);
}

void ListBox::SetSort(bool sort)
{
	SetStyleBool(StyleType::Style, LBS_SORT, sort);
}

bool ListBox::GetSort()
{
	return HasStyle(StyleType::Style, LBS_SORT);
}

int ListBox::ItemAdd(const TCHAR* txt, int index)
{
	CC_ASSERT(::IsWindow(m_hWnd) && txt != NULL);

	int nCnt = ItemCount();
	if(index == -1 || index >= nCnt)
	{
		return (int)::SendMessage(m_hWnd, LB_ADDSTRING, 0, (LPARAM)txt);
	}
	else
	{
		return (int)::SendMessage(m_hWnd, LB_INSERTSTRING, index, (LPARAM)txt);
	}
}

void ListBox::ItemDel(const TCHAR* txt)
{
	CC_ASSERT(::IsWindow(m_hWnd) && txt != NULL);

	int index = (int)SendMessage(m_hWnd, LB_FINDSTRINGEXACT, 0, (LPARAM)txt);
	if(index > -1)
	{
		SendMessage(m_hWnd, LB_DELETESTRING, index, 0);
	}
}

void ListBox::ItemDel(int index)
{
	CC_ASSERT(::IsWindow(m_hWnd));

	SendMessage(m_hWnd, LB_DELETESTRING, index, 0);
}

//void ListBox::CopyItemData(void** hbmDes, void* hbmSrc)
//{
//	*hbmDes = hbmSrc;
//}
//
void ListBox::ItemSet(int index, const TCHAR* txt)
{
	CC_ASSERT(::IsWindow(m_hWnd) && txt != NULL);

	//if item is selected,save it's pos
	int nInd = GetSelectedIndex();
	//int nInd = (int)SendMessage(m_hWnd, LB_GETCURSEL, 0, 0);
	//save ext data
	void* hbm = (void*)SendMessage(m_hWnd, LB_GETITEMDATA, index, 0);
	//set ext data NULL for invoid delete by LB_DELETESTRING
	SendMessage(m_hWnd, LB_SETITEMDATA, index, 0);
	//void* hbmSave = NULL;
	//CopyItemData(&hbmSave, hbm);
	//Delete string at List(i+1)
	SendMessage(m_hWnd, LB_DELETESTRING, (WPARAM)(index), 0L);
	//Insert new string at List(i)
	SendMessage(m_hWnd, LB_INSERTSTRING, (WPARAM)(index), (LPARAM)txt);
	//restore ext data
	SendMessage(m_hWnd, LB_SETITEMDATA, index, (LPARAM)hbm);
	if(nInd == index)
	{
		//Restore the current selection
		SetSelectedIndex(index);
		//SendMessage(m_hWnd, LB_SETCURSEL, (WPARAM)index, 0L);
		//SendMessage(m_hWnd, LB_SETANCHORINDEX, (WPARAM)index, 0L);
		//SendMessage(m_hWnd, LB_SETCARETINDEX, (WPARAM)index, FALSE);
	}
}

const cc::Str ListBox::ItemGet(int index)
{
	CC_ASSERT(::IsWindow(m_hWnd));

	cc::Str str;
	int nLen = (int)SendMessage(m_hWnd, (UINT)LB_GETTEXTLEN, index, 0);
	if(nLen > 0)
	{
		TCHAR* tc = (TCHAR*)str.GetBufferSetLength(nLen);
		SendMessage(m_hWnd, (UINT)LB_GETTEXT, (WPARAM)index, (LPARAM)tc);
		return str;
	}
	return str;
}

int ListBox::ItemCount() const
{
	CC_ASSERT(::IsWindow(m_hWnd));

	return (int)SendMessage(m_hWnd, LB_GETCOUNT, 0, 0);
}

void ListBox::ItemClear()
{
	CC_ASSERT(::IsWindow(m_hWnd));

	SendMessage(m_hWnd, LB_RESETCONTENT, 0, 0);
}
void ListBox::SetHExtent(int width)
{
	CC_ASSERT(::IsWindow(m_hWnd));

	SendMessage(m_hWnd, LB_SETHORIZONTALEXTENT, width, 0);
}

int ListBox::GetHExtent() const
{
	CC_ASSERT(::IsWindow(m_hWnd));

	return (int)::SendMessage(m_hWnd, LB_GETHORIZONTALEXTENT, 0, 0);
}

void ListBox::ItemUp(int index)
{
	CC_ASSERT(::IsWindow(m_hWnd));

	int nCnt = ItemCount();
	if(index <= 0 || nCnt <= 1)
	{
		return;
	}
	void* hbm = (void*)SendMessage(m_hWnd, LB_GETITEMDATA, index - 1, 0);
	//set ext data NULL for invoid delete by LB_DELETESTRING
	SendMessage(m_hWnd, LB_SETITEMDATA, index - 1, 0);
	//void* hbmSave = NULL;
	//CopyItemData(&hbmSave, hbm);
	int nLen = (int)SendMessage(m_hWnd, (UINT)LB_GETTEXTLEN, index - 1, 0);
	TCHAR* tc = (TCHAR*)malloc((nLen + 2) * sizeof(TCHAR));
	memset(tc, 0, (nLen + 2) * sizeof(TCHAR));
	SendMessage(m_hWnd, (UINT)LB_GETTEXT, index - 1, (LPARAM)tc);

	SendMessage(m_hWnd, LB_DELETESTRING, index - 1, 0);
    SendMessage(m_hWnd, LB_INSERTSTRING, index, (LPARAM)tc);
	SendMessage(m_hWnd, LB_SETITEMDATA, index, (LPARAM)hbm);
	SendMessage(m_hWnd, LB_SETCURSEL, index - 1, 0L);
}

void ListBox::ItemDown(int index)
{
	CC_ASSERT(::IsWindow(m_hWnd));

	int nCnt = ItemCount();
	if(index < 0 || index >= nCnt - 1)
	{
		return;
	}
	void* hbm = (void*)SendMessage(m_hWnd, LB_GETITEMDATA, index, 0);
	//set ext data NULL for invoid delete by LB_DELETESTRING
	SendMessage(m_hWnd, LB_SETITEMDATA, index, 0);
	//void* hbmSave = NULL;
	//CopyItemData(&hbmSave, hbm);
	int nLen = (int)SendMessage(m_hWnd, (UINT)LB_GETTEXTLEN, index, 0);
	TCHAR* tc = (TCHAR*)malloc((nLen + 2) * sizeof(TCHAR));
	memset(tc, 0, (nLen + 2) * sizeof(TCHAR));
	SendMessage(m_hWnd, (UINT)LB_GETTEXT, index, (LPARAM)tc);

	SendMessage(m_hWnd, LB_DELETESTRING, index, 0);
    SendMessage(m_hWnd, LB_INSERTSTRING, index + 1, (LPARAM)tc);
	SendMessage(m_hWnd, LB_SETITEMDATA, index + 1, (LPARAM)hbm);
	SendMessage(m_hWnd, LB_SETCURSEL, index + 1, 0L);
}
//int ListBox::GetCount() const
//	{ CC_ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, LB_GETCOUNT, 0, 0); }
//int ListBox::GetCurSel() const
//	{ CC_ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, LB_GETCURSEL, 0, 0); }
//int ListBox::SetCurSel(int nSelect)
//	{ CC_ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, LB_SETCURSEL, nSelect, 0); }
//int ListBox::GetHorizontalExtent() const
//	{ CC_ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, LB_GETHORIZONTALEXTENT, 0, 0); }
//void ListBox::SetHorizontalExtent(int cxExtent)
//	{ CC_ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, LB_SETHORIZONTALEXTENT, cxExtent, 0); }
int ListBox::GetSelCount() const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, LB_GETSELCOUNT, 0, 0); }
int ListBox::GetSelItems(int nMaxItems, LPINT rgIndex) const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, LB_GETSELITEMS, nMaxItems, (LPARAM)rgIndex); }
int ListBox::GetTopIndex() const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, LB_GETTOPINDEX, 0, 0); }
int ListBox::SetTopIndex(int nIndex)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, LB_SETTOPINDEX, nIndex, 0);}
DWORD_PTR ListBox::GetItemData(int nIndex) const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return ::SendMessage(m_hWnd, LB_GETITEMDATA, nIndex, 0); }
int ListBox::SetItemData(int nIndex, DWORD_PTR dwItemData)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, LB_SETITEMDATA, nIndex, (LPARAM)dwItemData); }
void* ListBox::GetItemDataPtr(int nIndex) const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (LPVOID)::SendMessage(m_hWnd, LB_GETITEMDATA, nIndex, 0); }
int ListBox::SetItemDataPtr(int nIndex, void* pData)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return SetItemData(nIndex, (DWORD_PTR)(LPVOID)pData); }
int ListBox::GetItemRect(int nIndex, LPRECT lpRect) const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, LB_GETITEMRECT, nIndex, (LPARAM)lpRect); }
int ListBox::GetSel(int nIndex) const
	{	//If an item is selected, the return value is greater than zero; otherwise, it is zero. If an error occurs, the return value is LB_ERR.
		CC_ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, LB_GETSEL, nIndex, 0); }
int ListBox::SetSel(int nIndex, BOOL bSelect)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, LB_SETSEL, bSelect, nIndex); }
//int ListBox::GetText(int nIndex, LPTSTR lpszBuffer) const
//	{ CC_ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, LB_GETTEXT, nIndex, (LPARAM)lpszBuffer); }
int ListBox::GetItemTextLen(int nIndex) const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, LB_GETTEXTLEN, nIndex, 0); }
void ListBox::SetColumnWidth(int cxWidth)
	{ CC_ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, LB_SETCOLUMNWIDTH, cxWidth, 0); }
bool ListBox::SetTabStops(int nTabStops, LPINT rgTabStops)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return ((BOOL)::SendMessage(m_hWnd, LB_SETTABSTOPS, nTabStops, (LPARAM)rgTabStops) == TRUE); }
void ListBox::SetTabStops()
	{ CC_ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, LB_SETTABSTOPS, 0, 0); }
bool ListBox::SetTabStops(const int& cxEachStop)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return ((BOOL)::SendMessage(m_hWnd, LB_SETTABSTOPS, 1, (LPARAM)(LPINT)&cxEachStop) == TRUE); }
int ListBox::SetItemHeight(int nIndex, UINT cyItemHeight)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, LB_SETITEMHEIGHT, nIndex, MAKELONG(cyItemHeight, 0)); }
int ListBox::GetItemHeight(int nIndex) const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, LB_GETITEMHEIGHT, nIndex, 0L); }
int ListBox::GetCaretIndex() const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, LB_GETCARETINDEX, 0, 0L); }
int ListBox::SetCaretIndex(int nIndex, bool bScroll)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, LB_SETCARETINDEX, nIndex, MAKELONG(bScroll ? TRUE : FALSE, 0)); }
//int ListBox::AddString(LPCTSTR lpszItem)
//	{ return (int)::SendMessage(m_hWnd, LB_ADDSTRING, 0, (LPARAM)lpszItem); }
//int ListBox::DeleteString(UINT nIndex)
//	{ return (int)::SendMessage(m_hWnd, LB_DELETESTRING, nIndex, 0); }
//int ListBox::InsertString(int nIndex, LPCTSTR lpszItem)
//	{ return (int)::SendMessage(m_hWnd, LB_INSERTSTRING, nIndex, (LPARAM)lpszItem); }
//void ListBox::ResetContent()
//	{ ::SendMessage(m_hWnd, LB_RESETCONTENT, 0, 0); }
int ListBox::Dir(UINT attr, LPCTSTR lpszWildCard)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, LB_DIR, attr, (LPARAM)lpszWildCard); }
int ListBox::SelectString(int nStartAfter, LPCTSTR lpszItem)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, LB_SELECTSTRING, nStartAfter, (LPARAM)lpszItem); }
int ListBox::SelItemRange(bool bSelect, int nFirstItem, int nLastItem)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return bSelect ? (int)::SendMessage(m_hWnd, LB_SELITEMRANGEEX, nFirstItem, nLastItem) :
		(int)::SendMessage(m_hWnd, LB_SELITEMRANGEEX, nLastItem, nFirstItem); }
void ListBox::SetAnchorIndex(int nIndex)
	{ CC_ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, LB_SETANCHORINDEX, nIndex, 0); }
int ListBox::GetAnchorIndex() const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, LB_GETANCHORINDEX, 0, 0); }
LCID ListBox::GetLocale() const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (LCID)::SendMessage(m_hWnd, LB_GETLOCALE, 0, 0); }
LCID ListBox::SetLocale(LCID nNewLocale)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (LCID)::SendMessage(m_hWnd, LB_SETLOCALE, (WPARAM)nNewLocale, 0); }
int ListBox::InitStorage(int nItems, UINT nBytes)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, LB_INITSTORAGE, (WPARAM)nItems, nBytes); }
int ListBox::FindString(int nStartAfter, LPCTSTR lpszItem) const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, LB_FINDSTRING, nStartAfter, (LPARAM)lpszItem); }
int ListBox::FindStringExact(int nIndexStart, LPCTSTR lpszFind) const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, LB_FINDSTRINGEXACT, nIndexStart, (LPARAM)lpszFind); }
//earchType=1:startswith; searchType=2:endswith; searchType=else:indexof>=0; 
int ListBox::FindStringOption(int nIndexStart, LPCTSTR lpszFind, int searchType, bool IgnoreCase)
{
	CC_ASSERT(::IsWindow(m_hWnd));

	int len = cc::Str::Len(lpszFind);
	if(len <= 0)
	{
		return -1;
	}
	int cnt = ItemCount();
	for(; nIndexStart < cnt; nIndexStart++)
	{
		int nLen = (int)SendMessage(m_hWnd, (UINT)LB_GETTEXTLEN, nIndexStart, 0);
		if(nLen >= len)
		{
			TCHAR* pText = (TCHAR*)malloc((nLen + 2) * sizeof(TCHAR));
			memset(pText, 0, (nLen + 2) * sizeof(TCHAR));
			SendMessage(m_hWnd, (UINT)LB_GETTEXT, (WPARAM)nIndexStart, (LPARAM)pText);
			if(searchType == 1)
			{
				if(cc::Str::NCmp(pText, lpszFind, len, IgnoreCase) == 0)
				{
					free(pText);
					return nIndexStart;
				}
			}
			else if(searchType == 2)
			{
				if(cc::Str::NCmp(pText + nLen - len, lpszFind, len, IgnoreCase) == 0)
				{
					free(pText);
					return nIndexStart;
				}
			}
			else
			{
				if(cc::Str::Search(pText, 0, nLen, lpszFind, len, IgnoreCase) >= 0)
				{
					free(pText);
					return nIndexStart;
				}
			}
			free(pText);
		}
	}
	return -1;
}

void ListBox::OnChildNotify(EventArgs *e)
{
	switch(e->Msg)
	{
	case WM_MEASUREITEM:
		{
//CC_TRACE((_T("%s, WM_MEASUREITEM"), _Name));
			MeasureItemEventArgs ee(e);
			ee.data = (LPMEASUREITEMSTRUCT)e->LParam;
			//HDC is created at OnMeasureItem if needed
			//ee.hDC = ::GetDC(m_hWnd);
			OnMeasureItem(&ee);
			//::ReleaseDC(m_hWnd, ee.hDC);
			//MeasureItem((LPMEASUREITEMSTRUCT)e->LParam);
		}
		break;
	case WM_DRAWITEM:
		{
//CC_TRACE((_T("%s, WM_DRAWITEM"), _Name));
			DrawItemEventArgs ee(e);
			ee.data = (LPDRAWITEMSTRUCT)e->LParam;
			OnDrawItem(&ee);
			//DrawItem((LPDRAWITEMSTRUCT)e->LParam);
		}
		break;
	case WM_COMPAREITEM:
		{
//CC_TRACE((_T("%s, WM_COMPAREITEM"), _Name));
			CompareItemEventArgs ee(e, (LPCOMPAREITEMSTRUCT)e->LParam);
			OnCompareItem(&ee);
		}
		break;
	case WM_DELETEITEM:
		OnDeleteItem(e);
		break;
	case WM_VKEYTOITEM:
		{
//CC_TRACE((_T("%s, WM_VKEYTOITEM"), _Name));
			VKeyToItemEventArgs ee(e, LOWORD(e->WParam), HIWORD(e->WParam));
			OnVKeyToItem(&ee);
		}
		break;
	case WM_CHARTOITEM:
		{
//CC_TRACE((_T("%s, WM_CHARTOITEM"), _Name));
			CharToItemEventArgs ee(e, LOWORD(e->WParam), HIWORD(e->WParam));
			OnCharToItem(&ee);
		}
	default:
		break;
	}
	Control::OnChildNotify(e);
}

void ListBox::OnCommand(EventArgs* e)
{
	switch(HIWORD(e->WParam))
	{
	case LBN_DBLCLK:
//CC_TRACE((_T("ListBox::OnCommand:LBN_DBLCLK")));
		OnItemDoubleClick(e);
		//OnDoubleClick(e);
		break;
	case LBN_ERRSPACE:
		{
			CC_LOGSTEP();
			const TCHAR* msg = _T("ListBox::OnCommand, cannot allocate enough memory.");
			cc::Log::fatal(msg);
			CC_ASSERT(0);
			throw UserException(msg);
		}
	case LBN_KILLFOCUS:
		OnLeave(e);
		OnLostFocus(e);
		break;
	case LBN_SELCANCEL:
//CC_TRACE((_T("ListBox::OnCommand:LBN_SELCANCEL")));
		break;
	case LBN_SELCHANGE:
//CC_TRACE((_T("ListBox::OnCommand:LBN_SELCHANGE")));
		OnSelectedIndexChanged(e);
		break;
	case LBN_SETFOCUS:
		OnEnter(e);
		OnGotFocus(e);
		break;
	}
}

void ListBox::OnItemDoubleClick(EventArgs* e)
{
	ItemDoubleClick.Invoke((Object*)this, e);
}

void ListBox::OnSelectedIndexChanged(EventArgs* e)
{
	if(!SelectedIndexChanged.IsEmpty())
	{
		e->Result = DefWndProc(e);
		e->Handled = true;

		//must do next for if settext in SelectedIndexChanged then not right for focusframe
		//This notification message is not sent if the LB_SETCURSEL message changes the selection. 
		SelectedIndexChanged.Invoke((Object*)this, e);
		int nInd = (int)SendMessage(m_hWnd, LB_GETCURSEL, 0, 0);
		SendMessage(m_hWnd, LB_SETCURSEL, (WPARAM)nInd, (LPARAM)0L);
	}
}

//// Derived class is responsible for implementing these handlers
////   for owner/self draw controls (except for the optional DeleteItem)
//void ListBox::OnDrawItem(DrawItemEventArgs* e)
//	{ /* default to nothing */ }
//void ListBox::OnMeasureItem(MeasureItemEventArgs *e)
//	{ /* default to nothing */ }
//void ListBox::OnCompareItem(CompareItemEventArgs* e)
//	{ /* default to nothing */ }
//void ListBox::OnDeleteItem(EventArgs* e)
//	{ /* default to nothing */ }
//void ListBox::OnVKeyToItem(VKeyToItemEventArgs* e)
//	{ /* default to nothing */ }
//void ListBox::OnCharToItem(CharToItemEventArgs* e)
//	{ /* default to nothing */ }
///////////////////////////////////////////////////////////////////////////////

DWORD CheckListBoxConfig::nStyle = WS_CHILDWINDOW | WS_VISIBLE | WS_TABSTOP | LBS_NOTIFY | LBS_NOINTEGRALHEIGHT|WS_HSCROLL|WS_VSCROLL|LBS_HASSTRINGS|LBS_OWNERDRAWFIXED;
DWORD CheckListBoxConfig::nExStyle = WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR | WS_EX_NOPARENTNOTIFY | WS_EX_CLIENTEDGE;
//DWORD CheckListBoxConfig::nClassStyle = CS_DBLCLKS | CS_GLOBALCLASS | CS_PARENTDC;
DWORD CheckListBoxConfig::nInnerStyle = InnerStyle::TranceContextMenuUp;
DWORD CheckListBoxConfig::nOuterStyle = 0;
DWORD CheckListBoxConfig::nOwnerDrawTextStyle = DT_LEFT|DT_VCENTER|DT_SINGLELINE;
//COLORREF CheckListBoxConfig::corBackground = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
//COLORREF CheckListBoxConfig::corTextground = ::GetSysColor(COLOR_BTNTEXT);
//COLORREF CheckListBoxConfig::corHotBackColor = ::GetSysColor(COLOR_BTNFACE);
//COLORREF CheckListBoxConfig::corHotTextColor = ::GetSysColor(COLOR_BTNTEXT);
COLORREF CheckListBoxConfig::corBackground = ::GetSysColor(COLOR_WINDOW);
COLORREF CheckListBoxConfig::corTextground = ::GetSysColor(COLOR_WINDOWTEXT);
COLORREF CheckListBoxConfig::corHotBackColor = ::GetSysColor(COLOR_ACTIVECAPTION);
COLORREF CheckListBoxConfig::corHotTextColor = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
COLORREF CheckListBoxConfig::corTextgroundDisabled = RGB(0x80, 0x80, 0x80);
//#ifndef CLBN_CHKCHANGE
//// Check Listbox Notification codes
//#define CLBN_CHKCHANGE (40)
//#endif
//===========================================================cc::win::CheckListBox
///////////////////////////////////////////////////////////////////////////////
// constructors/destructor
CheckListBox::CheckListBox()
{
	BASE_CONTROL_CONFIG_INIT(CheckListBox);
	_Rect.right = _Rect.left + 84;
	_Rect.bottom = _Rect.top + 60;
	::CopyRect(&_OrgRect, &_Rect);
	//_ClassName = WndClassName::ListBox;
	SetText(_T("CheckListBox"));
#ifdef _DEBUG_
	SetName(_T("CheckListBox"));
#endif
	m_cyText = 0;
	m_nStyle = CheckStyle::AutoCheckBox; //nStyle == BS_CHECKBOX || nStyle == BS_AUTOCHECKBOX || nStyle == BS_AUTO3STATE || nStyle == BS_3STATE
	_nMultiBoxCount = 1;
	//m_nDeletingIndex = -1;
	_mapData = NULL;
}

CheckListBox::~CheckListBox()
{
	if(_mapData != NULL)
	{
		for(int i = _mapData->GetSize()-1; i >= 0; i--)
		{
			CC_CHECK_DATA* pData = (CC_CHECK_DATA*)_mapData->GetAt(i);
			delete pData;
		}

		delete _mapData;
		_mapData = NULL;
	}
}

//keyArr is param address,but sHead is by value
//void CheckListBox::DoConfig(bool isFromIni, KeyArr& keyArr, cc::Str sThisHead)
//{
	//if(!HasStyle(StyleType::Outer, OuterStyle::AutoConfig))
	//{
	//	return;
	//}
	//if(isFromIni)
	//{
	//	//if not exist,as usually
	//	if(keyArr.Exist(sThisHead))
	//	{
	//		ItemClear();
	//		SetPos((int)keyArr.Get(sThisHead, 0));
	//	}
	//}
	//else
	//{
	//	keyArr.Set(sThisHead, (DWORD)GetPos());
	//}
//}

//void CheckListBox::CopyItemData(void** hbmDes, void* hbmSrc)
//{
//	if(hbmSrc != NULL)
//	{
//		if(*hbmDes == NULL)
//		{
//			*hbmDes = new CC_CHECK_DATA;
//			memcpy(*hbmDes, hbmSrc, sizeof(CC_CHECK_DATA));
//		}
//	}
//	else
//	{
//		*hbmDes = NULL;
//	}
//}
//
//keyArr is param address,but sHead is by value
void CheckListBox::DoConfig(bool isFromIni, KeyArr& keyArr, cc::Str sThisHead)
{
	if(!HasStyle(StyleType::Outer, OuterStyle::AutoConfig))
	{
		return;
	}
	int nInd;
	cc::Str sData;
	if(isFromIni)
	{
		//if not exist,as usually
		if(keyArr.Exist(sThisHead))
		{
			if(HasStyle(StyleType::Inner2, Inner2Style::AutoConfigListData))
			{
				sData = keyArr.Get(sThisHead + _T(".Data"));
				SetConfigData(sData);
			}

			nInd = (int)keyArr.Get(sThisHead, 0);
			if(nInd >= 0)
			{
				DefWndProc(LB_SETCURSEL, (WPARAM)nInd, 0);
			}
		}
	}
	else
	{
		nInd = (int)DefWndProc(LB_GETCURSEL, 0, 0);
		keyArr.Set(sThisHead, (DWORD)nInd);
		if(HasStyle(StyleType::Inner2, Inner2Style::AutoConfigListData))
		{
			sData = GetConfigData();
			keyArr.Set(sThisHead + _T(".Data"), sData);
		}
	}
}

cc::Str CheckListBox::GetConfigData()
{
	CC_ASSERT(::IsWindow(m_hWnd));

	cc::Str sData;
	int nCnt = (int)DefWndProc(LB_GETCOUNT, 0, 0);
	for(int i = 0; i < nCnt; i++)
	{
		int chk = GetCheck(i);
		sData.Append(cc::Str().ValueOf(chk)).Append(":");
		sData.Append(ItemGet(i));
		sData.Append(_T(CONTROL_LINE_SPLIT));
	}
	return sData;
}

void CheckListBox::SetConfigData(const cc::Str& sData, bool isAppend)
{
	CC_ASSERT(::IsWindow(m_hWnd));

	if(sData.IndexOf(_T(CONTROL_LINE_SPLIT)) >= 0)
	{
		int nInd = (int)DefWndProc(LB_GETCURSEL, 0, 0);
		if(!isAppend)
		{
			DefWndProc(LB_RESETCONTENT, 0, 0);
		}
		cc::Str* strArr = NULL;
		int nCnt = sData.Split(_T(CONTROL_LINE_SPLIT), &strArr);
		for(int i = 0; i < nCnt; i++)
		{
			cc::Str item = strArr[i];
			cc::Str pre = item.SplitBy(":");
			int chk = pre.ToInt();
			ItemAdd((TCHAR*)item);
			SetCheck(i, chk);
		}
		if(strArr != NULL)
		{
			delete []strArr;
		}
		DefWndProc(LB_SETCURSEL, (WPARAM)nInd, 0);
	}
}

int CheckListBox::GetCustomDataIndex(int nID, bool isCreate)
{
	int index = -1;
	if(_mapData != NULL)
	{
		index = _mapData->GetIndex((int)nID);
	}
	if(isCreate && index < 0)
	{
		if(_mapData == NULL)
		{
			_mapData = new Map<int, void*>(true, false, true);
		}
		CC_CHECK_DATA* pData = new CC_CHECK_DATA;
		pData->corTextground = _corTextground;
		pData->corHotTextColor = _corHotTextColor;
		index = _mapData->Add((int)nID, pData);
	}
	return index;
}
//
//void* CheckListBox::NewItemData()
//{
//	CC_CHECK_DATA* pState = new CC_CHECK_DATA;
//	pState->corTextground = _corTextground;
//	pState->corHotTextColor = _corHotTextColor;
//	return pState;
//}
/*
CC_CHECK_DATA* CheckListBox::GetItemDataBase(int nIndex)
{
	CC_ASSERT(::IsWindow(m_hWnd));
	//CC_ASSERT(m_nDeletingIndex != nIndex);

	CC_CHECK_DATA* pState = NULL;
	EventArgs ee(m_hWnd, LB_GETITEMDATA, nIndex, 0);
	LRESULT lResult = this->DefWndProc(&ee);
	if(lResult != LB_ERR)
	{
		pState = (CC_CHECK_DATA*)lResult;
		if(pState == NULL)
		{
			pState = (CC_CHECK_DATA*)NewItemData(); //new CC_CHECK_DATA;
			ee = EventArgs(m_hWnd, LB_SETITEMDATA, nIndex, (LPARAM)pState);
			this->DefWndProc(&ee);// != LB_ERR;
		}
	}
	return pState;
}*/

DWORD_PTR CheckListBox::GetItemData(int nIndex)
{
	//if(m_nDeletingIndex == nIndex)
	//{
		return (DWORD_PTR)this->DefWndProc(LB_GETITEMDATA, nIndex, 0);
	//}
	//CC_CHECK_DATA* pState = GetItemDataBase(nIndex);
	//return (pState == NULL ? NULL : pState->m_dwUserData);
}
int CheckListBox::SetItemData(int nIndex, DWORD_PTR dwItemData)
{
	//if(m_nDeletingIndex == nIndex)
	//{
		this->DefWndProc(LB_SETITEMDATA, nIndex, (LPARAM)dwItemData);
	//}
	//else
	//{
	//	CC_CHECK_DATA* pState = GetItemDataBase(nIndex);
	//	if(pState != NULL)
	//	{
	//		pState->m_dwUserData = dwItemData;
	//	}
	//}
	return nIndex;
}
void* CheckListBox::GetItemDataPtr(int nIndex)
{
	CC_ASSERT(::IsWindow(m_hWnd));
	return (void*)GetItemData(nIndex);
}
int CheckListBox::SetItemDataPtr(int nIndex, void* pData)
{
	CC_ASSERT(::IsWindow(m_hWnd));
	return SetItemData(nIndex, (DWORD_PTR)(LPVOID)pData);
}

void CheckListBox::InvalidateCheck(int nIndex)
{
	CC_ASSERT(::IsWindow(m_hWnd));

	if(m_hWnd != NULL)
	{
		RECT rect;
		GetItemRect(nIndex, &rect);
		rect.right = rect.left + (CC_CHECKLISTBOX_BOXH + 2) * _nMultiBoxCount;
		InvalidateRect(m_hWnd, &rect, FALSE);
	}
}

void CheckListBox::InvalidateItem(int nIndex)
{
	CC_ASSERT(::IsWindow(m_hWnd));

	if(m_hWnd != NULL)
	{
		RECT rect;
		GetItemRect(nIndex, &rect);
		InvalidateRect(m_hWnd, &rect, FALSE);
	}
}

int CheckListBox::CalcMinimumItemHeight()
{
	int nResult;

	if(HasStyle(StyleType::Style, LBS_HASSTRINGS|LBS_OWNERDRAWFIXED))
	{
		HDC hDC = ::GetDC(m_hWnd);
		HFONT hOldFont = (HFONT)::SelectObject(hDC, GetFont());
		TEXTMETRIC tm;
		::GetTextMetrics(hDC, &tm);
		::SelectObject(hDC, hOldFont);
		::ReleaseDC(m_hWnd, hDC);

		m_cyText = tm.tmHeight;
		nResult = max(CC_CHECKLISTBOX_BOXH, m_cyText);
	}
	else
	{
		nResult = CC_CHECKLISTBOX_BOXH;
	}

	return nResult;
}

// Derived class is responsible for implementing these handlers
//   for owner/self draw controls (except for the optional DeleteItem)
void CheckListBox::PreDrawItem(DrawItemEventArgs* e)
{
	OnDrawItem(e);
}

void CheckListBox::PreMeasureItem(MeasureItemEventArgs *e)
{
	int cyItem = CalcMinimumItemHeight();
	MEASUREITEMSTRUCT measureItem = *(e->data);
	measureItem.itemHeight = cyItem;
	measureItem.itemWidth  = (UINT)-1;

	// WINBUG: Windows95 and Windows NT disagree on what this value
	// should be.  According to the docs, they are both wrong
	if(HasStyle(StyleType::Style, LBS_OWNERDRAWVARIABLE))
	{
		EventArgs ee(m_hWnd, LB_GETITEMDATA, measureItem.itemID, 0);
		LRESULT lResult = this->DefWndProc(&ee);
		if(lResult != LB_ERR)
		{
			measureItem.itemData = lResult;
		}
		else
		{
			measureItem.itemData = 0;
		}

		// WINBUG: This is only done in the LBS_OWNERDRAWVARIABLE case
		// because Windows 95 does not initialize itemData to zero in the
		// case of LBS_OWNERDRAWFIXED list boxes (it is stack garbage).
		if(measureItem.itemData != 0 && measureItem.itemData != LB_ERR)
		{
			CC_CHECK_DATA* pState = (CC_CHECK_DATA*)measureItem.itemData;
			measureItem.itemData = pState->m_dwUserData;
		}
	}

	OnMeasureItem(e);

	e->data->itemHeight = max(measureItem.itemHeight, (UINT)cyItem);
	e->data->itemWidth = measureItem.itemWidth;
}

void CheckListBox::PreCompareItem(CompareItemEventArgs* e)
{
	//should set result
	COMPAREITEMSTRUCT compareItem = *(e->data);

	if(compareItem.itemData1 != 0 && compareItem.itemData1 != LB_ERR)
	{
		CC_CHECK_DATA* pState = (CC_CHECK_DATA*)compareItem.itemData1;
		compareItem.itemData1 = pState->m_dwUserData;
	}
	if(compareItem.itemData2 != 0 && compareItem.itemData2 != LB_ERR)
	{
		CC_CHECK_DATA* pState = (CC_CHECK_DATA*)compareItem.itemData2;
		compareItem.itemData2 = pState->m_dwUserData;
	}
	OnCompareItem(e);
}

//Windows NT/2000/XP: The system sends a WM_DELETEITEM message only for items deleted
//  from an owner-drawn list box (with the LBS_OWNERDRAWFIXED or LBS_OWNERDRAWVARIABLE style)
//  or owner-drawn combo box (with the CBS_OWNERDRAWFIXED or CBS_OWNERDRAWVARIABLE style).
//Windows 95/98/Me: The system sends the WM_DELETEITEM message for any deleted list box
//  or combo box item with nonzero item data.
void CheckListBox::PreDeleteItem(EventArgs* e)
{
	DELETEITEMSTRUCT deleteItem = *((DELETEITEMSTRUCT*)e->LParam);

	// WINBUG: The following if block is required because Windows NT
	// version 3.51 does not properly fill out the LPDELETEITEMSTRUCT.
	if(deleteItem.itemData == 0)
	{
		//LRESULT lResult = DefWindowProc(LB_GETITEMDATA, deleteItem.itemID, 0);
		EventArgs ee(m_hWnd, LB_GETITEMDATA, deleteItem.itemID, 0);
		LRESULT lResult = this->DefWndProc(&ee);
		if(lResult != LB_ERR)
		{
			deleteItem.itemData = lResult;
		}
	}

	if(deleteItem.itemData != 0 && deleteItem.itemData != LB_ERR)
	{
		//CC_CHECK_DATA* pState = (CC_CHECK_DATA*)deleteItem.itemData;
		//deleteItem.itemData = pState->m_dwUserData;
		//delete pState;
		EventArgs ee = EventArgs(m_hWnd, LB_SETITEMDATA, deleteItem.itemID, (LPARAM)deleteItem.itemData);
		this->DefWndProc(&ee);// != LB_ERR;
	}
	OnDeleteItem(e);
}

void CheckListBox::OnDrawItem(DrawItemEventArgs* e)
{
	// You must override DrawItem and MeasureItem for LBS_OWNERDRAWVARIABLE
	CC_ASSERT(HasStyle(StyleType::Style, LBS_OWNERDRAWFIXED|LBS_HASSTRINGS));

	LPDRAWITEMSTRUCT lpDrawItemStruct = e->data;
	HDC hDC = e->data->hDC;

	if(((LONG)(lpDrawItemStruct->itemID) >= 0) && (lpDrawItemStruct->itemAction & (ODA_DRAWENTIRE | ODA_SELECT)))
	{
//cc::Log::error(_T("DrawItem, lpDrawItemStruct->itemID:%d"), lpDrawItemStruct->itemID);
		int nCheckAll = 0;
		BOOL fDisabled = (::IsWindowEnabled(m_hWnd) != TRUE) || !GetItemEnabled(lpDrawItemStruct->itemID);
		COLORREF newTextColor = fDisabled ? CheckListBoxConfig::corTextgroundDisabled : _corTextground;  // light gray
		COLORREF hotColor = _corHotTextColor;
		int ind = GetCustomDataIndex((int)lpDrawItemStruct->itemID, false);
		if(ind >= 0)
		{
			CC_CHECK_DATA* pState = (CC_CHECK_DATA*)_mapData->GetAt(ind);
			//CC_CHECK_DATA* pState = GetItemDataBase(lpDrawItemStruct->itemID);
			CC_ASSERT(pState != NULL);
			//int nCheck = GetCheck(lpDrawItemStruct->itemID);
			//int nCheckAll = GetCheckAll(lpDrawItemStruct->itemID);
			nCheckAll = pState->m_nChecks;
			if(!fDisabled)
			{
				//COLORREF newTextColor = fDisabled ? RGB(0x80, 0x80, 0x80) : GetSysColor(COLOR_WINDOWTEXT);  // light gray
				//newTextColor = fDisabled ? CheckListBoxConfig::corTextgroundDisabled : pState->corTextground;  // light gray
				newTextColor = pState->corTextground;  // light gray
			}
			hotColor = pState->corHotTextColor;
		}
		int cyItem = GetItemHeight(lpDrawItemStruct->itemID);

		COLORREF oldTextColor = ::SetTextColor(hDC, newTextColor);
		//COLORREF newBkColor = GetSysColor(COLOR_WINDOW);
		COLORREF newBkColor = _corBackground;
		COLORREF oldBkColor = ::SetBkColor(hDC, newBkColor);
		if(newTextColor == newBkColor)
		{
			newTextColor = RGB(0xC0, 0xC0, 0xC0);   // dark gray
			::SetTextColor(hDC, newTextColor);
		}
		RECT rect;
		HBRUSH brush = NULL;
		POINT pt = {0, 0};
		if(HasStyle(StyleType::Outer, OuterStyle::TransparentParentErase))
		{
			if(_pParentBrush != NULL)
			{
				::GetWindowRect(this->m_hWnd, &rect);
				::MapWindowPoints(NULL, _pParentBrush->m_hWnd, (LPPOINT)(&rect), 2);
				brush = _pParentBrush->GetBackgroundBrush();
				::SetBrushOrgEx(hDC, -rect.left, -rect.top, &pt);
				::FillRect(hDC, &lpDrawItemStruct->rcItem, brush);
				::SetBkMode(hDC, TRANSPARENT);
			}
		}
		if(brush == NULL)
		{
			//clear back use window color
			::ExtTextOut(hDC, lpDrawItemStruct->rcItem.left, lpDrawItemStruct->rcItem.top, ETO_OPAQUE, &(lpDrawItemStruct->rcItem), NULL, 0, NULL);
		}

		//if(!fDisabled && ((lpDrawItemStruct->itemState & ODS_SELECTED) != 0))
		if((lpDrawItemStruct->itemState & ODS_SELECTED) != 0)
		{
			if(!fDisabled)
			{
				::SetTextColor(hDC, hotColor);
			}
			::SetBkColor(hDC, _corHotBackColor);
		}

		if(m_cyText == 0)
		{
			CalcMinimumItemHeight();
		}

		for(int nHIndex = 0; nHIndex < _nMultiBoxCount; nHIndex++)
		{
			int nCheck = ((nCheckAll >> nHIndex * 2) & 0x0003);
			// draw a frame around the button
			UINT uStyle = DFCS_BUTTONCHECK;
			if(nCheck > 0)
			{
				uStyle |= DFCS_CHECKED;
			}
			if(nCheck > 1)
			{
				uStyle |= DFCS_BUTTON3STATE;
			}
			RECT rect = {lpDrawItemStruct->rcItem.left, lpDrawItemStruct->rcItem.top + (cyItem - CC_CHECKLISTBOX_BOXH)/2,
				//lpDrawItemStruct->rcItem.left + CC_CHECKLISTBOX_BOXH + 2, lpDrawItemStruct->rcItem.top + CC_CHECKLISTBOX_BOXH};
				lpDrawItemStruct->rcItem.left + CC_CHECKLISTBOX_BOXH, lpDrawItemStruct->rcItem.top + CC_CHECKLISTBOX_BOXH};
			::DrawFrameControl(hDC, &rect, DFC_BUTTON, uStyle);
			lpDrawItemStruct->rcItem.left += CC_CHECKLISTBOX_BOXH + 2;
		}
		lpDrawItemStruct->rcItem.left += 2;

		cc::Str str = ItemGet(lpDrawItemStruct->itemID);
		if(brush == NULL || (lpDrawItemStruct->itemState & ODS_SELECTED) != 0)
		{
			::ExtTextOut(hDC, lpDrawItemStruct->rcItem.left, lpDrawItemStruct->rcItem.top + max(0, (cyItem - m_cyText) / 2),
				ETO_OPAQUE, &(lpDrawItemStruct->rcItem), (TCHAR*)str.GetBuf(), str.GetLength(), NULL);
		}
		else
		{
			::DrawText(hDC, (TCHAR*)str.GetBuf(), -1, &lpDrawItemStruct->rcItem, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
		}

		::SetTextColor(hDC, oldTextColor);
		::SetBkColor(hDC, oldBkColor);
		if(brush != NULL)
		{
			::SetBrushOrgEx(hDC, pt.x, pt.y, NULL);
		}
	}

	if((lpDrawItemStruct->itemAction & ODA_FOCUS) != 0)
	{
		::DrawFocusRect(hDC, &(lpDrawItemStruct->rcItem));
	}
}

void CheckListBox::OnMeasureItem(MeasureItemEventArgs *e)
{
	// You must override DrawItem and MeasureItem for LBS_OWNERDRAWVARIABLE
	CC_ASSERT(HasStyle(StyleType::Style, LBS_OWNERDRAWFIXED|LBS_HASSTRINGS));
}

//void CheckListBox::CompareItem(CompareItemEventArgs* e)
//{
//}
//
//use Control::OnDeleteItem to DeleteItem 
void CheckListBox::OnDeleteItem(EventArgs* e)
{
	 Control::OnDeleteItem(e);

	DELETEITEMSTRUCT deleteItem = *((DELETEITEMSTRUCT*)e->LParam);
	int ind = GetCustomDataIndex((int)deleteItem.itemID, false);
	if(ind >= 0)
	{
		CC_CHECK_DATA* pData = (CC_CHECK_DATA*)_mapData->GetAt(ind);
		delete pData;
		_mapData->RemoveAt(ind);
	}
}

int CheckListBox::CheckFromPoint(POINT point, BOOL& bInCheck, int& nHIndex)
{
	// assume did not hit anything
	bInCheck = FALSE;
	int nIndex = -1;

	//if((GetStyle() & (LBS_OWNERDRAWFIXED|LBS_MULTICOLUMN)) == LBS_OWNERDRAWFIXED)
	if(HasStyle(StyleType::Style, LBS_OWNERDRAWFIXED))
	{
		// optimized case for ownerdraw fixed, single column
		int cyItem = GetItemHeight(0);
		if(point.y < cyItem * ItemCount())
		{
			int curPos = ::GetScrollPos(m_hWnd, SB_HORZ);
			point.x += curPos;
			nIndex = this->GetTopIndex() + point.y / cyItem;
			if(point.x < (CC_CHECKLISTBOX_BOXH + 2) * _nMultiBoxCount + 1)
			{
				++bInCheck;
				for(int i = 0; i < _nMultiBoxCount; i++)
				{
					if(point.x <= (CC_CHECKLISTBOX_BOXH + 2) * (i + 1))
					{
						nHIndex = i;
						break;
					}
				}
			}
		}
	}
	else
	{
		// general case for ownerdraw variable or multiple column
		int curPos = ::GetScrollPos(m_hWnd, SB_HORZ);
		point.x += curPos;
		for(int i = this->GetTopIndex(); i < ItemCount(); i++)
		{
			RECT itemRect;
			this->GetItemRect(i, &itemRect);
			if(::PtInRect(&itemRect, point))
			{
				nIndex = i;
				if(point.x - itemRect.left < (CC_CHECKLISTBOX_BOXH + 2) * _nMultiBoxCount + 1)
				{
					++bInCheck;
					for(int i = 0; i < _nMultiBoxCount; i++)
					{
						if(point.x - itemRect.left <= (CC_CHECKLISTBOX_BOXH + 2) * (i + 1))
						{
							nHIndex = i;
							break;
						}
					}
				}
				break;
			}
		}
	}
	return nIndex;
}

void CheckListBox::SetSelectionCheck(int nCheck, int nHIndex)
{
	int* piSelectedItems;
	int nSelectedItems;
	int iSelectedItem;

	nSelectedItems = GetSelCount();
	if(nSelectedItems > 0)
	{
		piSelectedItems = (int*)malloc(nSelectedItems * sizeof(int));
		memset(piSelectedItems, 0, nSelectedItems * sizeof(int));
		GetSelItems(nSelectedItems, piSelectedItems);
		for(iSelectedItem = 0; iSelectedItem < nSelectedItems; iSelectedItem++)
		{
			if(GetItemEnabled(piSelectedItems[iSelectedItem]))
			{
				SetCheck(piSelectedItems[iSelectedItem], nCheck, nHIndex);
				InvalidateCheck(piSelectedItems[iSelectedItem]);
			}
		}
		free(piSelectedItems);
	}
}

void CheckListBox::OnMouseDown(MouseEventArgs* e)
{
	if(e->Button != MouseButtons::Left)
	{
		ListBox::OnMouseDown(e);
		return;
	}
	Focus();

	// determine where the click is
	BOOL bInCheck = (BOOL)0;
	POINT point = {e->X, e->Y};
	int nHIndex = 0;
	int nIndex = CheckFromPoint(point, bInCheck, nHIndex);
//cc::Log::error(_T("CheckListBox::OnMouseDown,nIndex:%d, bInCheck:%d, nHIndex:%d"), nIndex, bInCheck, nHIndex);
	if(nIndex < 0)
	{
		ListBox::OnMouseDown(e);
		return;
	}

	// if the item is disabled, then eat the click
	if(!GetItemEnabled(nIndex))
	{
		return;
	}

	if((int)m_nStyle != BS_CHECKBOX && (int)m_nStyle != BS_3STATE)
	{
		// toggle the check mark automatically if the check mark was hit
		if(bInCheck)
		{
			Control* pParent = GetParent();
			CC_ASSERT(pParent != NULL);

			int nModulo = ((int)m_nStyle == BS_AUTO3STATE) ? 3 : 2;
			int nCheck;
			int nNewCheck;

			nCheck = GetCheck(nIndex, nHIndex);
			nCheck = (nCheck == nModulo) ? nCheck - 1 : nCheck;
			nNewCheck = (nCheck + 1) % nModulo;
//cc::Log::error(_T("CheckListBox::OnMouseDown,nNewCheck:%d"), nNewCheck);
			SetCheck(nIndex, nNewCheck, nHIndex);
			InvalidateCheck(nIndex);

			if(HasStyle(StyleType::Style, LBS_EXTENDEDSEL|LBS_MULTIPLESEL) && GetSel(nIndex))
			{
				// The listbox is a multi-select listbox, and the user clicked on
				// a selected check, so change the check on all of the selected 
				// items.
				SetSelectionCheck(nNewCheck, nHIndex);
			}
			else
			{
				ListBox::OnMouseDown(e);
			}

			// Inform parent of check
			//change GetDlgCtrlID to the index of item
			//SendMessage(pParent->m_hWnd, WM_COMMAND, MAKEWPARAM( ((UINT)(WORD)::GetDlgCtrlID(m_hWnd)), CLBN_CHKCHANGE ), (LPARAM)m_hWnd);
			//for do something while CLBN_CHKCHANGE,need set e->baseArgs->Handled
			e->baseArgs->Handled = true;
			//SendMessage(pParent->m_hWnd, WM_COMMAND, MAKEWPARAM(nIndex, CLBN_CHKCHANGE), (LPARAM)m_hWnd);
			PostMessage(m_hWnd, CC_WM_CHECKCHANGED, MAKEWPARAM(nNewCheck, nHIndex), (LPARAM)nIndex);
			return;
		}
	}

	ListBox::OnMouseDown(e);
}

//PostMessage(m_hWnd, CC_WM_CHECKCHANGED, MAKEWPARAM(nNewCheck, nHIndex), (LPARAM)nIndex)
//HIWORD(e->WParam) = nHIndex, LOWORD(e->WParam) = nNewCheck, e->LParam = nIndex 
void CheckListBox::OnItemCheckChanged(EventArgs* e)
{
	ItemCheckChanged.Invoke((Object*)this, e);
}

//void CheckListBox::OnCommand(EventArgs* e)
//{
//	switch(HIWORD(e->WParam))
//	{
//	case CLBN_CHKCHANGE:
//		OnItemCheckChanged(e);
//		break;
//	}
//	ListBox::OnCommand(e);
//}
//
// WndProc - calls appriate On... function for the give
// message
LRESULT CheckListBox::WndProc(EventArgs *e)
{
	switch(e->Msg)
	{
	case CC_WM_CHECKCHANGED:
		OnItemCheckChanged(e);
		return (LRESULT)0;
	}
	return ListBox::WndProc(e);
}

void CheckListBox::OnChildNotify(EventArgs *e)
{
	switch(e->Msg)
	{
	case WM_MEASUREITEM:
		{
//CC_TRACE((_T("%s, WM_MEASUREITEM"), _Name));
			MeasureItemEventArgs ee(e);
			ee.data = (LPMEASUREITEMSTRUCT)e->LParam;
			//for HDC is must used,create here
			ee.hDC = ::GetDC(m_hWnd);
			PreMeasureItem(&ee);
			::ReleaseDC(m_hWnd, ee.hDC);
		}
		break;
	case WM_DRAWITEM:
		{
//CC_TRACE((_T("%s, WM_DRAWITEM"), _Name));
			DrawItemEventArgs ee(e);
			ee.data = (LPDRAWITEMSTRUCT)e->LParam;
			PreDrawItem(&ee);
		}
		break;
	case WM_COMPAREITEM:
		{
//CC_TRACE((_T("%s, WM_COMPAREITEM"), _Name));
			CompareItemEventArgs ee(e, (LPCOMPAREITEMSTRUCT)e->LParam);
			PreCompareItem(&ee);
		}
		break;
	case WM_DELETEITEM:
		{
//CC_TRACE((_T("%s, WM_DELETEITEM"), _Name));
			//DeleteItemEventArgs ee(e, (LPDELETEITEMSTRUCT)e->LParam);
			PreDeleteItem(e);
		}
		break;
	}
	Control::OnChildNotify(e);
}

void CheckListBox::SetCheckStyle(CheckStyle::Type eStyle)
{
	if((int)eStyle == 0 || (int)eStyle == BS_CHECKBOX || (int)eStyle == BS_AUTOCHECKBOX || (int)eStyle == BS_AUTO3STATE || (int)eStyle == BS_3STATE)
	{
		m_nStyle = eStyle;
	}
}

CheckStyle::Type CheckListBox::GetCheckStyle()
{
	return m_nStyle;
}

void CheckListBox::SetCheck(int nIndex, int nCheck, int nHIndex)
{
	CC_ASSERT(::IsWindow(m_hWnd));
	CC_ASSERT(nCheck >= 0 && nCheck < 3);
	CC_ASSERT(nHIndex >= 0 && nHIndex < _nMultiBoxCount);

	if(nCheck == 2)
	{
		if((int)m_nStyle == BS_CHECKBOX || (int)m_nStyle == BS_AUTOCHECKBOX)
		{
			// Fire accessibility event
			// nIndex is 0 based and accessibility index is 1 based
#ifdef EVENT_OBJECT_STATECHANGE
			NotifyWinEvent(EVENT_OBJECT_STATECHANGE, m_hWnd, OBJID_CLIENT, nIndex + 1);
#endif
			//if (m_pfnNotifyWinEvent != NULL) (*m_pfnNotifyWinEvent)(event, m_hWnd, idObjectType, idObject);
			return;
		}
	}

	//CC_CHECK_DATA* pState = GetItemDataBase(nIndex);
	CC_CHECK_DATA* pState = NULL;
	int ind = GetCustomDataIndex(nIndex, true);
	if(ind >= 0)
	{
		pState = (CC_CHECK_DATA*)_mapData->GetAt(ind);
	}
	if(pState != NULL)
	{
		//clear old status
		pState->m_nChecks &= ~(0x0003 << nHIndex * 2);
		//set new status
		pState->m_nChecks |= (nCheck << nHIndex * 2);
		InvalidateCheck(nIndex);
		// Fire accessibility event
		// nIndex is 0 based and accessibility index is 1 based
#ifdef EVENT_OBJECT_STATECHANGE
		NotifyWinEvent(EVENT_OBJECT_STATECHANGE, m_hWnd, OBJID_CLIENT, nIndex + 1);
#endif
	}
}

int CheckListBox::GetCheck(int nIndex, int nHIndex)
{
	CC_ASSERT(::IsWindow(m_hWnd));
	CC_ASSERT(nHIndex >= 0 && nHIndex < _nMultiBoxCount);

	//CC_CHECK_DATA* pState = GetItemDataBase(nIndex);
	CC_CHECK_DATA* pState = NULL;
	int ind = GetCustomDataIndex(nIndex, false);
	if(ind >= 0)
	{
		pState = (CC_CHECK_DATA*)_mapData->GetAt(ind);
	}
	if(pState != NULL)
	{
		int nCheck = ((pState->m_nChecks >> nHIndex * 2) & 0x0003);
		return nCheck;
	}
	return 0; // The default
}

int CheckListBox::GetCheckAll(int nIndex)
{
	CC_ASSERT(::IsWindow(m_hWnd));

	//CC_CHECK_DATA* pState = GetItemDataBase(nIndex);
	CC_CHECK_DATA* pState = NULL;
	int ind = GetCustomDataIndex(nIndex, false);
	if(ind >= 0)
	{
		pState = (CC_CHECK_DATA*)_mapData->GetAt(ind);
	}
	if(pState != NULL)
	{
		return pState->m_nChecks;
	}
	return 0; // The default
}

int CheckListBox::GetCheckedCount(int nHIndex)
{
	int result = 0;
	for (int i = ItemCount() - 1; i >= 0; i--)
	{
		if(GetCheck(i, nHIndex))
		{
			result++;
		}
	}
	return result;
}

void CheckListBox::SetItemEnabled(int nIndex, bool bEnabled)
{
	CC_ASSERT(::IsWindow(m_hWnd));

	//CC_CHECK_DATA* pState = GetItemDataBase(nIndex);
	CC_CHECK_DATA* pState = NULL;
	int ind = GetCustomDataIndex(nIndex, true);
	if(ind >= 0)
	{
		pState = (CC_CHECK_DATA*)_mapData->GetAt(ind);
	}
	if(pState != NULL)
	{
		pState->m_bEnabled = bEnabled;
		InvalidateItem(nIndex);
	}
}

bool CheckListBox::GetItemEnabled(int nIndex)
{
	CC_ASSERT(::IsWindow(m_hWnd));

	//CC_CHECK_DATA* pState = GetItemDataBase(nIndex);
	CC_CHECK_DATA* pState = NULL;
	int ind = GetCustomDataIndex(nIndex, false);
	if(ind >= 0)
	{
		pState = (CC_CHECK_DATA*)_mapData->GetAt(ind);
	}
	if(pState != NULL)
	{
		return pState->m_bEnabled;
	}
	return true; // The default
}

void CheckListBox::SetMultiBoxCount(int nMultiBoxCount)
{
	CC_ASSERT(nMultiBoxCount >= 0 && nMultiBoxCount <= 16);

	_nMultiBoxCount = nMultiBoxCount;
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
	{
		CC_ASSERT(::IsWindow(m_hWnd));

		if(m_hWnd != NULL)
		{
			//repaint
			InvalidateRect(m_hWnd, NULL, FALSE);
		}
	}
}

int CheckListBox::GetMultiBoxCount()
{
	return _nMultiBoxCount;
}
// constructors/destructor
///////////////////////////////////////////////////////////////////////////////

/*
CBS_AUTOHSCROLL 
    Automatically scrolls the text in an edit control to the right when the user types a character at the end of the line.
    If this style is not set, only text that fits within the rectangular boundary is allowed.
CBS_DISABLENOSCROLL 
    Shows a disabled vertical scroll bar in the list box when the box does not contain enough items to scroll. Without this style,
    the scroll bar is hidden when the list box does not contain enough items.
CBS_DROPDOWN 
    Similar to CBS_SIMPLE, except that the list box is not displayed unless the user selects an icon next to the edit control.
CBS_DROPDOWNLIST 
    Similar to CBS_DROPDOWN, except that the edit control is replaced by a static text item that displays the current selection in the list box.
CBS_HASSTRINGS 
    Specifies that an owner-drawn combo box contains items consisting of strings. The combo box maintains the memory
    and address for the strings so the application can use the CB_GETLBTEXT message to retrieve the text for a particular item.
    For accessibility issues, see Exposing Owner-Drawn Combo Box Items
CBS_LOWERCASE 
    Converts to lowercase all text in both the selection field and the list. 
CBS_NOINTEGRALHEIGHT 
    Specifies that the size of the combo box is exactly the size specified by the application when it created the combo box.
    Normally, the system sizes a combo box so that it does not display partial items.
CBS_OEMCONVERT 
    Converts text entered in the combo box edit control from the Windows character set to the OEM character set and then
    back to the Windows character set. This ensures proper character conversion when the application calls the CharToOem
    function to convert a Windows string in the combo box to OEM characters. This style is most useful for combo boxes that
    contain file names and applies only to combo boxes created with the CBS_SIMPLE or CBS_DROPDOWN style.
CBS_OWNERDRAWFIXED 
    Specifies that the owner of the list box is responsible for drawing its contents and that the items in the list box
    are all the same height. The owner window receives a WM_MEASUREITEM message when the combo box is created and a
    WM_DRAWITEM message when a visual aspect of the combo box has changed.
CBS_OWNERDRAWVARIABLE 
    Specifies that the owner of the list box is responsible for drawing its contents and that the items in the list box
    are variable in height. The owner window receives a WM_MEASUREITEM message for each item in the combo box when you
    create the combo box and a WM_DRAWITEM message when a visual aspect of the combo box has changed.
CBS_SIMPLE 
    Displays the list box at all times. The current selection in the list box is displayed in the edit control.
CBS_SORT 
    Automatically sorts strings added to the list box.
CBS_UPPERCASE 
    Converts to uppercase all text in both the selection field and the list.
*/
DWORD ComboBoxConfig::nStyle = WS_CHILDWINDOW | WS_VISIBLE | CBS_DROPDOWN | WS_TABSTOP|CBS_AUTOHSCROLL|WS_VSCROLL; //CBS_SORT
DWORD ComboBoxConfig::nExStyle = WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR | WS_EX_NOPARENTNOTIFY;// | WS_EX_CLIENTEDGE;
//DWORD ComboBoxConfig::nClassStyle = CS_DBLCLKS | CS_GLOBALCLASS | CS_PARENTDC;
DWORD ComboBoxConfig::nInnerStyle = InnerStyle::TranceContextMenuUp;
DWORD ComboBoxConfig::nOuterStyle = 0;
DWORD ComboBoxConfig::nOwnerDrawTextStyle = DT_LEFT|DT_VCENTER|DT_SINGLELINE;
//COLORREF ComboBoxConfig::corBackground = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
COLORREF ComboBoxConfig::corBackground = ::GetSysColor(COLOR_WINDOW);
COLORREF ComboBoxConfig::corTextground = ::GetSysColor(COLOR_WINDOWTEXT);
COLORREF ComboBoxConfig::corHotBackColor = ::GetSysColor(COLOR_WINDOW);
COLORREF ComboBoxConfig::corHotTextColor = ::GetSysColor(COLOR_WINDOWTEXT);
//COLORREF ComboBoxConfig::corHotBackColor = ::GetSysColor(COLOR_ACTIVECAPTION);
//COLORREF ComboBoxConfig::corHotTextColor = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
COLORREF ComboBoxConfig::corBackgroundDisabled = ::GetSysColor(COLOR_BTNFACE);
//===========================================================cc::win::ComboBox
///////////////////////////////////////////////////////////////////////////////
// constructors/destructor
//SendMessage(cboCountries, CB_SETDROPPEDWIDTH, 240, 0);
//CB_GETDROPPEDWIDTH
ComboBox::ComboBox()
{
	BASE_CONTROL_CONFIG_INIT(ComboBox);
	corBackgroundSaved = ComboBoxConfig::corBackground;
	_Rect.right = _Rect.left + 84;
	_Rect.bottom = _Rect.top + 120;
	::CopyRect(&_OrgRect, &_Rect);
	_ClassName = WndClassName::ComboBox;
	SetText(NULL);
#ifdef _DEBUG_
	SetName(_T("ComboBox"));
#endif
	_pInnerTextBox = NULL;
}

ComboBox::~ComboBox()
{
	Dispose();
}

void ComboBox::Dispose()
{
	if(!GetIsDispose())
	{
		if(_pInnerTextBox != NULL)
		{
			_pInnerTextBox->UnsubclassWindow();
			delete _pInnerTextBox;
			_pInnerTextBox = NULL;
		}
		Control::Dispose();
	}
}

//keyArr is param address,but sHead is by value
void ComboBox::DoConfig(bool isFromIni, KeyArr& keyArr, cc::Str sThisHead)
{
	if(!HasStyle(StyleType::Outer, OuterStyle::AutoConfig))
	{
		return;
	}
	int nInd;
	cc::Str sData;
	if(isFromIni)
	{
		//if not exist,as usually
		if(keyArr.Exist(sThisHead))
		{
			if(HasStyle(StyleType::Inner2, Inner2Style::AutoConfigListData))
			{
				sData = keyArr.Get(sThisHead + _T(".Data"));
				SetConfigData(sData);
			}

			nInd = (int)keyArr.Get(sThisHead, 0);
			if(nInd >= 0)
			{
				DefWndProc(CB_SETCURSEL, (WPARAM)nInd, 0);
			}
			else if(GetComboBoxStyle() != ComboBoxStyle::DropDownList)
			{
				this->SetText((TCHAR*)keyArr.Get(sThisHead + _T(".Text")).GetBuf());
			}
		}
	}
	else
	{
		nInd = (int)DefWndProc(CB_GETCURSEL, 0, 0);
		keyArr.Set(sThisHead, (DWORD)nInd);
		if(nInd < 0 && GetComboBoxStyle() != ComboBoxStyle::DropDownList)
		{
			keyArr.Set(sThisHead + _T(".Text"), cc::Str(this->GetText()));
		}

		if(HasStyle(StyleType::Inner2, Inner2Style::AutoConfigListData))
		{
			sData = GetConfigData();
			keyArr.Set(sThisHead + _T(".Data"), sData);
		}
	}
}

void ComboBox::DoSetLang(KeyArr& keyArr, cc::Str sThisHead)
{
	Control::DoSetLang(keyArr, sThisHead);
	sThisHead.Append(_T(".Data"));
	if(keyArr.Exist(sThisHead))
	{
		cc::Str sData = keyArr.Get(sThisHead);
		if(m_hWnd != NULL)
		{
			SetConfigData(sData, false);
		}
		else
		{
			SetText((const TCHAR*)sData.GetBuf());
		}
	}
}

cc::Str ComboBox::GetConfigData()
{
	CC_ASSERT(::IsWindow(m_hWnd));

	cc::Str sData;
	int nCnt = (int)DefWndProc(CB_GETCOUNT, 0, 0);
	for(int i = 0; i < nCnt; i++)
	{
		sData.Append(ItemGet(i));
		sData.Append(_T(CONTROL_LINE_SPLIT));
	}
	return sData;
}

void ComboBox::SetConfigData(const cc::Str& sData, bool isAppend)
{
	CC_ASSERT(::IsWindow(m_hWnd));

	if(sData.IndexOf(_T(CONTROL_LINE_SPLIT)) >= 0)
	{
		int nInd = (int)DefWndProc(CB_GETCURSEL, 0, 0);
		if(!isAppend)
		{
			DefWndProc(CB_RESETCONTENT, 0, 0);
		}
		cc::Str* strArr = NULL;
		int nCnt = sData.Split(_T(CONTROL_LINE_SPLIT), &strArr);
		for(int i = 0; i < nCnt; i++)
		{
			ItemAdd((TCHAR*)strArr[i].GetBuf());
		}
		if(strArr != NULL)
		{
			delete []strArr;
		}
		DefWndProc(CB_SETCURSEL, (WPARAM)nInd, 0);
	}
}

void ComboBox::CreateHandle()
{
	bool isList = ((_StyleArray[(int)StyleType::Style] & CBS_DROPDOWNLIST) == CBS_DROPDOWNLIST);
	if(isList)
	{
		_StyleArray[(int)StyleType::Style] &= ~CBS_DROPDOWNLIST;
		_StyleArray[(int)StyleType::Style] |= CBS_DROPDOWN;
	}
	Control::CreateHandle();
	GetTextBoxCtrl();
	CC_ASSERT(_pInnerTextBox != NULL);
	_pInnerTextBox->DoubleClick += EventHandler((Object*)this, (EventFun)&ComboBox::innerTextBox_DoubleClick);
	//_pInnerTextBox->KeyDown += KeyEventHandler((Object*)this, (KeyEventFun)&ComboBox::innerTextBox_KeyDown);
	_pInnerTextBox->KeyPress += KeyPressEventHandler((Object*)this, (KeyPressEventFun)&ComboBox::innerTextBox_KeyPress);
	if(isList)
	{
		SetStyle(StyleType::Style, CBS_SIMPLE|CBS_DROPDOWN, CBS_DROPDOWNLIST);
		_pInnerTextBox->SetReadOnly(true);
	}
	SetConfigData(cc::Str(_Text), false);
	_pInnerTextBox->SetTips(_Tips);
	_pInnerTextBox->SetTipsExt(_TipsExt);
}
//
//bool ComboBox::PreCreateWindow(CREATESTRUCT& cs)
//{
//	if((cs.style & CBS_DROPDOWNLIST) == CBS_DROPDOWNLIST)
//	{
//		cs.style &= ~CBS_DROPDOWNLIST;
//		cs.style |= CBS_DROPDOWN;
//	}
//	return Control::PreCreateWindow(cs);
//}
//
//void ComboBox::OnHandleCreated(EventArgs* e)
//{
//	CC_ASSERT(::IsWindow(m_hWnd));
//
//	bool isList = ((_StyleArray[(int)StyleType::Style] & CBS_DROPDOWNLIST) == CBS_DROPDOWNLIST);
//	Control::OnHandleCreated(e);
//	GetTextBoxCtrl();
//	CC_ASSERT(_pInnerTextBox != NULL);
//	_pInnerTextBox->DoubleClick += EventHandler((Object*)this, (EventFun)&ComboBox::innerTextBox_DoubleClick);
//	_pInnerTextBox->KeyDown += KeyEventHandler((Object*)this, (KeyEventFun)&ComboBox::innerTextBox_KeyDown);
//	if(isList)
//	{
//		SetStyle(StyleType::Style, CBS_SIMPLE|CBS_DROPDOWN, CBS_DROPDOWNLIST);
//		_pInnerTextBox->SetReadOnly(true);
//	}
//	SetConfigData(cc::Str(_Text), false);
//}

void ComboBox::innerTextBox_DoubleClick(Object* sender, EventArgs* e)
{
	//!GetDroppedState() && 
	if(HasStyle(StyleType::Style, CBS_DROPDOWNLIST))
	{
		ShowDropDown(true);
		e->Handled = true;
		//http://support.microsoft.com/default.aspx?scid=kb;en-us;326254
		//Call SendMessage(WM_SETCURSOR,0,0) from inside the dropdown handler function.
		//This causes the pointer to appear when the ComboBox opens.
		::SendMessage(m_hWnd, WM_SETCURSOR, 0, 0);
	}
}
//
//void ComboBox::innerTextBox_KeyDown(Object* sender, KeyEventArgs* e)
//{
//	//!GetDroppedState() && 
//	if(HasStyle(StyleType::Style, CBS_DROPDOWNLIST) && (int)e->baseArgs->WParam == ' ')
//	{
//		innerTextBox_DoubleClick(sender, e->baseArgs);
//	}
//}

void ComboBox::innerTextBox_KeyPress(Object* sender, KeyEventArgs* e)
{
	if(e->baseArgs->Msg == WM_CHAR)
	{
		if((::GetKeyState(VK_CONTROL) & 0x8000) != 0)
		{
			return;
		}
		if(e->baseArgs->WParam == VK_RETURN)
		{
			return;
		}

		int nSel = (int)HIWORD(GetEditSel());
		ShowDropDown(true);
		_pInnerTextBox->Focus();
		cc::Str s = _pInnerTextBox->GetText();
		if(nSel < 1)
		{
			nSel = s.GetLength();
		}
		s = s.Substring(0, nSel);
		if(e->baseArgs->WParam == VK_BACK)
		{
			if(nSel > 0)
			{
				s = s.Substring(0, nSel - 1);
			}
		}
		else
		{
			s.AppendFormat(_T("%c"), e->baseArgs->WParam);
		}
		int nInd = this->FindStringOption(0, s, 1, true);
		if(nInd >= 0 || s.GetLength() < 1)
		{
			this->SetSelectedIndex(nInd);
			this->SetEditSel(0, s.GetLength());
		}
		else
		{
			if(IsStyle(StyleType::Style, CBS_DROPDOWNLIST))
			{
			}
			else
			{
				this->SetSelectedIndex(-1);
				_pInnerTextBox->SetText(s);
				this->SetEditSel(0, s.GetLength());
			}
		}
		e->baseArgs->Handled = true;
	}
}

void ComboBox::OnPaintBackground(PaintEventArgs* e)
{
	Control::OnPaintBackground(e);
	if(_pInnerTextBox != NULL && _pInnerTextBox->m_hWnd != NULL)
	{
		::InvalidateRect(_pInnerTextBox->m_hWnd, NULL, TRUE);
	}
}

//if readonly&disabled,then use backcolor of disabled
void ComboBox::SetEnabled(bool isEnabled)
{
	if(GetEnabled())
	{
		corBackgroundSaved = _corBackground;
	}
	Control::SetEnabled(isEnabled);
	if(isEnabled)
	{
		SetBackground(corBackgroundSaved);
	}
	else
	{
		SetBackground(ComboBoxConfig::corBackgroundDisabled);
	}
}

TextBox* ComboBox::GetTextBoxCtrl()
{
	CC_ASSERT(::IsWindow(m_hWnd));
	if(_pInnerTextBox == NULL)
	{
		HWND hEdit = FindWindowEx(m_hWnd, 0, _T("Edit"), NULL);
		if(hEdit == NULL)
		{
			HWND hChild = ::GetWindow(m_hWnd, GW_CHILD);
			if(hChild != NULL)
			{
				hEdit = FindWindowEx(hChild, 0, _T("Edit"), NULL);
			}
		}
		if(hEdit != NULL)
		{
			_pInnerTextBox = new TextBox();
			_pInnerTextBox->SubclassWindow(hEdit, this);
#if defined(_DEBUG_) || defined(_DESIGN)
			//code for design
			//flag for moveable or others
			_pInnerTextBox->Design = (DesignStyle::Type)DesignStyle::NoDesign;
#endif
		}
	}
	return _pInnerTextBox;
}

void ComboBox::SetComboBoxStyle(ComboBoxStyle::Type comboBoxStyle)
{
	if(comboBoxStyle == ComboBoxStyle::DropDown)
	{
		SetStyle(StyleType::Style, CBS_SIMPLE|CBS_DROPDOWNLIST, CBS_DROPDOWN);
	}
	else if(comboBoxStyle == ComboBoxStyle::DropDownList)
	{
		SetStyle(StyleType::Style, CBS_SIMPLE|CBS_DROPDOWN, CBS_DROPDOWNLIST);
	}
	else
	{
		SetStyle(StyleType::Style, CBS_DROPDOWN|CBS_DROPDOWNLIST, CBS_SIMPLE);
	}
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0 && m_hWnd != NULL)
	{
		if(GetTextBoxCtrl() != NULL)
		{
			_pInnerTextBox->SetReadOnly(comboBoxStyle == ComboBoxStyle::DropDownList);
		}
	}
}

ComboBoxStyle::Type ComboBox::GetComboBoxStyle()
{
	if(HasStyle(StyleType::Style, CBS_DROPDOWN))
	{
		return ComboBoxStyle::DropDown;
	}
	else if(HasStyle(StyleType::Style, CBS_DROPDOWNLIST))
	{
		return ComboBoxStyle::DropDownList;
	}
	return ComboBoxStyle::Simple;
}

void ComboBox::SetText(const TCHAR* sText)
{
	if(sText != _Text)
	{
		LockAction lock(&m_lockText);
		_Text = CC_StrCopy(&_Text, sText);
	}
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
	{
		SendMessage(m_hWnd, WM_SETTEXT, 0, (LPARAM)sText);
	}
}

const TCHAR* ComboBox::GetText()
{
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
	{
		LockAction lock(&m_lockText);
		int nLen = (int)SendMessage(m_hWnd, (UINT)WM_GETTEXTLENGTH, 0, 0);
		if(_Text != NULL)
		{
			free(_Text);
			_Text = NULL;
		}
		if(nLen > 0)
		{
			_Text = (TCHAR*)malloc((nLen + 2) * sizeof(TCHAR));
			memset(_Text, 0, (nLen + 2) * sizeof(TCHAR));
			SendMessage(m_hWnd, (UINT)WM_GETTEXT, (WPARAM)(nLen + 1), (LPARAM)_Text);
		}
	}
	return _Text;
}

void ComboBox::SetSelectedIndex(int selectedIndex)
{
	CC_ASSERT(::IsWindow(m_hWnd));

	SendMessage(m_hWnd, CB_SETCURSEL, (WPARAM)selectedIndex, 0);
}

int ComboBox::GetSelectedIndex() const
{
	CC_ASSERT(::IsWindow(m_hWnd));

	return (int)SendMessage(m_hWnd, CB_GETCURSEL, 0, 0);
}

int ComboBox::ItemAdd(const TCHAR* txt, int index)
{
	CC_ASSERT(::IsWindow(m_hWnd));

	int nCnt = ItemCount();
	if(index == -1 || index >= nCnt)
	{
		return (int)::SendMessage(m_hWnd, CB_ADDSTRING, 0, (LPARAM)txt);
	}
	else
	{
		return (int)::SendMessage(m_hWnd, CB_INSERTSTRING, index, (LPARAM)txt);
	}
}

void ComboBox::ItemDel(const TCHAR* txt)
{
	CC_ASSERT(::IsWindow(m_hWnd));

	int index = (int)SendMessage(m_hWnd, CB_FINDSTRINGEXACT, 0, (LPARAM)txt);
	if(index != -1)
	{
		ItemDel(index);
	}
}

void ComboBox::ItemDel(int index)
{
	CC_ASSERT(::IsWindow(m_hWnd));

	//modify for not leave sometext even no item in it
	if(index == 0 && ItemCount() == 1)
	{
		ItemClear();
	}
	else
	{
		SendMessage(m_hWnd, CB_DELETESTRING, index, 0);
	}
}

void ComboBox::ItemSet(int index, const TCHAR* txt)
{
	CC_ASSERT(::IsWindow(m_hWnd));

	//if item is selected,save it's pos
	int nInd = GetSelectedIndex();
	//Insert new string at List(i)
	//save ext data
	void* hbm = (void*)SendMessage(m_hWnd, CB_GETITEMDATA, index, 0);
	//set ext data NULL for invoid delete by LB_DELETESTRING
	SendMessage(m_hWnd, CB_SETITEMDATA, index, 0);
	//Delete string at List(i+1)
	//ItemDel(index + 1);
	SendMessage(m_hWnd, CB_DELETESTRING, index, 0);
	//ItemAdd(txt, index);
	::SendMessage(m_hWnd, CB_INSERTSTRING, index, (LPARAM)txt);
	//restore ext data
	SendMessage(m_hWnd, CB_SETITEMDATA, index, (LPARAM)hbm);
	if(nInd == index)
	{
		//Restore the current selection
		SetSelectedIndex(index);
	}
}

const cc::Str ComboBox::ItemGet(int index)
{
	CC_ASSERT(::IsWindow(m_hWnd));

	cc::Str str;
	int nLen = (int)SendMessage(m_hWnd, (UINT)CB_GETLBTEXTLEN, index, 0);
	if(nLen > 0)
	{
		TCHAR* tc = (TCHAR*)str.GetBufferSetLength(nLen);
		SendMessage(m_hWnd, (UINT)CB_GETLBTEXT, (WPARAM)index, (LPARAM)tc);
		return str;
	}
	return str;
}

int ComboBox::ItemCount() const
{
	CC_ASSERT(::IsWindow(m_hWnd));

	return (int)SendMessage(m_hWnd, CB_GETCOUNT, 0, 0);
}

void ComboBox::ItemClear()
{
	CC_ASSERT(::IsWindow(m_hWnd));

	SendMessage(m_hWnd, CB_RESETCONTENT, 0, 0);
}
//int ComboBox::GetCount() const
//	{ return (int)::SendMessage(m_hWnd, CB_GETCOUNT, 0, 0); }
//int ComboBox::GetCurSel() const
//	{ return (int)::SendMessage(m_hWnd, CB_GETCURSEL, 0, 0); }
//int ComboBox::SetCurSel(int nSelect)
//	{ return (int)::SendMessage(m_hWnd, CB_SETCURSEL, nSelect, 0); }
//IA64: Assuming retval of CB_GETEDITSEL won't be expanded
DWORD ComboBox::GetEditSel() const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return DWORD(::SendMessage(m_hWnd, CB_GETEDITSEL, 0, 0)); }
bool ComboBox::LimitText(int nMaxChars)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return ((BOOL)::SendMessage(m_hWnd, CB_LIMITTEXT, nMaxChars, 0) == TRUE); }
bool ComboBox::SetEditSel(int nStartChar, int nEndChar)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return ((BOOL)::SendMessage(m_hWnd, CB_SETEDITSEL, 0, MAKELONG(nStartChar, nEndChar)) == TRUE); }
DWORD_PTR ComboBox::GetItemData(int nIndex) const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return ::SendMessage(m_hWnd, CB_GETITEMDATA, nIndex, 0); }
int ComboBox::SetItemData(int nIndex, DWORD_PTR dwItemData)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, CB_SETITEMDATA, nIndex, (LPARAM)dwItemData); }
void* ComboBox::GetItemDataPtr(int nIndex) const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (LPVOID)GetItemData(nIndex); }
int ComboBox::SetItemDataPtr(int nIndex, void* pData)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return SetItemData(nIndex, (DWORD_PTR)(LPVOID)pData); }
//int ComboBox::GetLBText(int nIndex, LPTSTR lpszText) const
//	{ CC_ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, CB_GETLBTEXT, nIndex, (LPARAM)lpszText); }
int ComboBox::GetItemTextLen(int nIndex) const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, CB_GETLBTEXTLEN, nIndex, 0); }
void ComboBox::ShowDropDown(bool bShowIt)
	{ CC_ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, CB_SHOWDROPDOWN, bShowIt ? TRUE : FALSE, 0); }
//int ComboBox::AddString(LPCTSTR lpszString)
//	{ CC_ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, CB_ADDSTRING, 0, (LPARAM)lpszString); }
//int ComboBox::DeleteString(UINT nIndex)
//	{ CC_ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, CB_DELETESTRING, nIndex, 0);}
//int ComboBox::InsertString(int nIndex, LPCTSTR lpszString)
//	{ CC_ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, CB_INSERTSTRING, nIndex, (LPARAM)lpszString); }
//void ComboBox::ResetContent()
//	{ CC_ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, CB_RESETCONTENT, 0, 0); }
int ComboBox::Dir(UINT attr, LPCTSTR lpszWildCard)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, CB_DIR, attr, (LPARAM)lpszWildCard); }
int ComboBox::SelectString(int nStartAfter, LPCTSTR lpszString)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, CB_SELECTSTRING, nStartAfter, (LPARAM)lpszString); }
void ComboBox::Clear()
	{ CC_ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, WM_CLEAR, 0, 0); }
void ComboBox::Copy()
	{ CC_ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, WM_COPY, 0, 0); }
void ComboBox::Cut()
	{ CC_ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, WM_CUT, 0, 0); }
void ComboBox::Paste()
	{ CC_ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, WM_PASTE, 0, 0); }
int ComboBox::SetItemHeight(int nIndex, UINT cyItemHeight)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, CB_SETITEMHEIGHT, nIndex, MAKELONG(cyItemHeight, 0)); }
int ComboBox::GetItemHeight(int nIndex) const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, CB_GETITEMHEIGHT, nIndex, 0L); }
int ComboBox::SetExtendedUI(BOOL bExtended )
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, CB_SETEXTENDEDUI, bExtended, 0L); }
bool ComboBox::GetExtendedUI() const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return ((BOOL)::SendMessage(m_hWnd, CB_GETEXTENDEDUI, 0, 0L) == TRUE); }
void ComboBox::GetDroppedControlRect(LPRECT lprect) const
	{ CC_ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, CB_GETDROPPEDCONTROLRECT, 0, (LPARAM)lprect); }
bool ComboBox::GetDroppedState() const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return ((BOOL)::SendMessage(m_hWnd, CB_GETDROPPEDSTATE, 0, 0L) == TRUE); }
LCID ComboBox::GetLocale() const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (LCID)::SendMessage(m_hWnd, CB_GETLOCALE, 0, 0); }
LCID ComboBox::SetLocale(LCID nNewLocale)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (LCID)::SendMessage(m_hWnd, CB_SETLOCALE, (WPARAM)nNewLocale, 0); }
int ComboBox::GetTopIndex() const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, CB_GETTOPINDEX, 0, 0); }
int ComboBox::SetTopIndex(int nIndex)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, CB_SETTOPINDEX, nIndex, 0); }
int ComboBox::InitStorage(int nItems, UINT nBytes)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, CB_INITSTORAGE, (WPARAM)nItems, nBytes); }
void ComboBox::SetHorizontalExtent(UINT nExtent)
	{ CC_ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, CB_SETHORIZONTALEXTENT, nExtent, 0); }
UINT ComboBox::GetHorizontalExtent() const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (UINT)::SendMessage(m_hWnd, CB_GETHORIZONTALEXTENT, 0, 0); }
int ComboBox::SetDroppedWidth(UINT nWidth)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, CB_SETDROPPEDWIDTH, nWidth, 0); }
int ComboBox::GetDroppedWidth() const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, CB_GETDROPPEDWIDTH, 0, 0); }
int ComboBox::FindString(int nStartAfter, LPCTSTR lpszString) const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, CB_FINDSTRING, nStartAfter, (LPARAM)lpszString); }
int ComboBox::FindStringExact(int nIndexStart, LPCTSTR lpszFind) const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, CB_FINDSTRINGEXACT, nIndexStart, (LPARAM)lpszFind); }
int ComboBox::FindStringOption(int nIndexStart, LPCTSTR lpszFind, int searchType, bool IgnoreCase)
{
	CC_ASSERT(::IsWindow(m_hWnd));

	int len = cc::Str::Len(lpszFind);
	if(len <= 0)
	{
		return -1;
	}
	int cnt = ItemCount();
	for(; nIndexStart < cnt; nIndexStart++)
	{
		int nLen = (int)SendMessage(m_hWnd, (UINT)CB_GETLBTEXTLEN, nIndexStart, 0);
		if(nLen >= len)
		{
			TCHAR* pText = (TCHAR*)malloc((nLen + 2) * sizeof(TCHAR));
			memset(pText, 0, (nLen + 2) * sizeof(TCHAR));
			SendMessage(m_hWnd, (UINT)CB_GETLBTEXT, (WPARAM)nIndexStart, (LPARAM)pText);
			if(searchType == 1)
			{
				if(cc::Str::NCmp(pText, lpszFind, len, IgnoreCase) == 0)
				{
					free(pText);
					return nIndexStart;
				}
			}
			else if(searchType == 2)
			{
				if(cc::Str::NCmp(pText + nLen - len, lpszFind, len, IgnoreCase) == 0)
				{
					free(pText);
					return nIndexStart;
				}
			}
			else
			{
				if(cc::Str::Search(pText, 0, nLen, lpszFind, len, IgnoreCase) >= 0)
				{
					free(pText);
					return nIndexStart;
				}
			}
			free(pText);
		}
	}
	return -1;
}

void ComboBox::OnChildNotify(EventArgs *e)
{
	switch(e->Msg)
	{
	case WM_MEASUREITEM:
		{
//CC_TRACE((_T("ComboBox::OnChildNotify:%s, WM_MEASUREITEM"), _Name));
			MeasureItemEventArgs ee(e);
			ee.data = (LPMEASUREITEMSTRUCT)e->LParam;
			//HDC is created at OnMeasureItem if needed
			//ee.hDC = ::GetDC(m_hWnd);
			OnMeasureItem(&ee);
			//::ReleaseDC(m_hWnd, ee.hDC);
			//MeasureItem((LPMEASUREITEMSTRUCT)e->LParam);
		}
		break;
	case WM_DRAWITEM:
		{
//CC_TRACE((_T("ComboBox::OnChildNotify:%s, WM_DRAWITEM"), _Name));
			DrawItemEventArgs ee(e);
			ee.data = (LPDRAWITEMSTRUCT)e->LParam;
			OnDrawItem(&ee);
			//DrawItem((LPDRAWITEMSTRUCT)e->LParam);
		}
		break;
	case WM_COMPAREITEM:
		{
//CC_TRACE((_T("ComboBox::OnChildNotify:%s, WM_COMPAREITEM"), _Name));
			CompareItemEventArgs ee(e, (LPCOMPAREITEMSTRUCT)e->LParam);
			OnCompareItem(&ee);
		}
		break;
	case WM_DELETEITEM:
		OnDeleteItem(e);
		break;
	case WM_VKEYTOITEM:
		{
//CC_TRACE((_T("ComboBox::OnChildNotify:%s, WM_VKEYTOITEM"), _Name));
			VKeyToItemEventArgs ee(e, LOWORD(e->WParam), HIWORD(e->WParam));
			OnVKeyToItem(&ee);
		}
		break;
	case WM_CHARTOITEM:
		{
//CC_TRACE((_T("ComboBox::OnChildNotify:%s, WM_CHARTOITEM"), _Name));
			CharToItemEventArgs ee(e, LOWORD(e->WParam), HIWORD(e->WParam));
			OnCharToItem(&ee);
		}
		break;
	default:
		break;
	}
	Control::OnChildNotify(e);
}

void ComboBox::OnCommand(EventArgs* e)
{
	switch(HIWORD(e->WParam))
	{
	case CBN_DBLCLK:
//CC_TRACE((_T("ComboBox::OnCommand:CBN_DBLCLK")));
		//OnItemDoubleClick(e);
		OnDoubleClick(e);
		break;
	case CBN_ERRSPACE:
		{
			CC_LOGSTEP();
			const TCHAR* msg = _T("ComboBox::OnCommand, cannot allocate enough memory.");
			cc::Log::fatal(msg);
			CC_ASSERT(0);
			throw UserException(msg);
		}
	case CBN_KILLFOCUS:
//CC_TRACE((_T("ComboBox::OnCommand:CBN_KILLFOCUS")));
		OnLeave(e);
		OnLostFocus(e);
		break;
	case CBN_SELENDOK:
//CC_TRACE((_T("ComboBox::OnCommand:CBN_SELENDOK")));
		break;
	case CBN_SELENDCANCEL:
//CC_TRACE((_T("ComboBox::OnCommand:CBN_SELENDCANCEL")));
		break;
	case CBN_SELCHANGE:
//CC_TRACE((_T("ComboBox::OnCommand:CBN_SELCHANGE")));
		OnSelectedIndexChanged(e);
		break;
	case CBN_SETFOCUS:
//CC_TRACE((_T("ComboBox::OnCommand:CBN_SETFOCUS")));
		OnEnter(e);
		OnGotFocus(e);
		break;
	case CBN_EDITCHANGE:
//CC_TRACE((_T("ComboBox::OnCommand:CBN_EDITCHANGE")));
		OnTextChanged(e);
		break;
	case CBN_EDITUPDATE:
//CC_TRACE((_T("ComboBox::OnCommand:CBN_EDITUPDATE")));
		break;
	case CBN_DROPDOWN:
//CC_TRACE((_T("ComboBox::OnCommand:CBN_DROPDOWN")));
		break;
	}
}
//
//void ComboBox::OnItemDoubleClick(EventArgs* e)
//{
//	ItemDoubleClick.Invoke((Object*)this, e);
//}

void ComboBox::OnSelectedIndexChanged(EventArgs* e)
{
	SelectedIndexChanged.Invoke((Object*)this, e);
}

//// Derived class is responsible for implementing these handlers
////   for owner/self draw controls (except for the optional DeleteItem)
//void ComboBox::DrawItem(DrawItemEventArgs* e)
//	{ /* default to nothing */ }
//void ComboBox::MeasureItem(MeasureItemEventArgs *e)
//	{ /* default to nothing */ }
//void ComboBox::CompareItem(CompareItemEventArgs* e)
//	{ /* default to nothing */ }
//void ComboBox::DeleteItem(EventArgs* e)
//	{ /* default to nothing */ }
//void ComboBox::VKeyToItem(VKeyToItemEventArgs* e)
//	{ /* default to nothing */ }
//void ComboBox::CharToItem(CharToItemEventArgs* e)
//	{ /* default to nothing */ }
// constructors/destructor
///////////////////////////////////////////////////////////////////////////////

DWORD ComboBoxExConfig::nStyle = WS_CHILDWINDOW | WS_VISIBLE | WS_TABSTOP | CCS_NOMOVEY | 0x0100|ES_AUTOHSCROLL|WS_VSCROLL;
DWORD ComboBoxExConfig::nExStyle = WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR | WS_EX_NOPARENTNOTIFY;// | WS_EX_CLIENTEDGE;
//DWORD ComboBoxExConfig::nClassStyle = CS_DBLCLKS | CS_GLOBALCLASS | CS_PARENTDC;
DWORD ComboBoxExConfig::nInnerStyle = InnerStyle::TranceContextMenuUp;
DWORD ComboBoxExConfig::nOuterStyle = 0;
DWORD ComboBoxExConfig::nOwnerDrawTextStyle = DT_LEFT|DT_VCENTER|DT_SINGLELINE;
COLORREF ComboBoxExConfig::corBackground = ::GetSysColor(COLOR_WINDOW);
COLORREF ComboBoxExConfig::corTextground = ::GetSysColor(COLOR_WINDOWTEXT);
COLORREF ComboBoxExConfig::corHotBackColor = ::GetSysColor(COLOR_WINDOW);
COLORREF ComboBoxExConfig::corHotTextColor = ::GetSysColor(COLOR_WINDOWTEXT);
//===========================================================cc::win::ComboBoxEx
///////////////////////////////////////////////////////////////////////////////
// constructors/destructor
ComboBoxEx::ComboBoxEx()
{
	BASE_CONTROL_CONFIG_INIT(ComboBoxEx);
	_ClassName = WndClassName::ComboBoxEx;
	_StyleArray[(int)StyleType::InitControl] = CC_WNDCOMMCTL_USEREX_REG;
	SetText(_T("ComboBoxEx"));
#ifdef _DEBUG_
	SetName(_T("ComboBoxEx"));
#endif
	_pInnerComboBox = NULL;
}

ComboBoxEx::~ComboBoxEx()
{
	Dispose();
}

void ComboBoxEx::Dispose()
{
	if(!GetIsDispose())
	{
		if(_pInnerComboBox != NULL)
		{
			_pInnerComboBox->UnsubclassWindow();
			delete _pInnerComboBox;
			_pInnerComboBox = NULL;
		}
		Control::Dispose();
	}
}

int ComboBoxEx::ItemAdd(const TCHAR* txt, int index)
{
	return ItemAdd(txt, index, -1, -1, 0);
}

int ComboBoxEx::ItemAdd(const TCHAR* txt, int index, int iImage, int iSelectedImage, int iIndent)
{
	CC_ASSERT(::IsWindow(m_hWnd));
	CC_ASSERT(txt != NULL);

	//  Declare and init locals.
	COMBOBOXEXITEM cbei = {0};
	// Set the mask common to all items.
	cbei.mask = CBEIF_TEXT | CBEIF_INDENT | CBEIF_IMAGE| CBEIF_SELECTEDIMAGE;
	// Initialize the COMBOBOXEXITEM struct.
	cbei.iItem          = index;

	cc::Str s           = cc::Str(LangCode::TCHAR, txt);
	cbei.pszText        = (TCHAR*)s.GetBuf();
	cbei.cchTextMax     = s.GetLength();

	cbei.iImage         = iImage;
	cbei.iSelectedImage = iSelectedImage;
	cbei.iIndent        = iIndent;

	// Tell the ComboBoxEx to add the item. Return FALSE if this fails.
	if((index = (int)SendMessage(m_hWnd, CBEM_INSERTITEM, 0, (LPARAM)&cbei)) == -1)
	{
		return -1;
	}
	return index;
}

void ComboBoxEx::CreateHandle()
{
	ComboBox::CreateHandle();
	GetComboBoxCtrl();
	CC_ASSERT(_pInnerComboBox != NULL);
}

void ComboBoxEx::OnPaintBackground(PaintEventArgs* e)
{
	ComboBox::OnPaintBackground(e);
	if(_pInnerComboBox != NULL && _pInnerComboBox->m_hWnd != NULL)
	{
		::InvalidateRect(_pInnerComboBox->m_hWnd, NULL, TRUE);
	}
}

DWORD ComboBoxEx::GetExtendedStyle() const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, CBEM_GETEXTENDEDSTYLE, 0, 0); }
DWORD ComboBoxEx::SetExtendedStyle(DWORD dwExMask, DWORD dwExStyles)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (DWORD) ::SendMessage(m_hWnd, CBEM_SETEXTENDEDSTYLE, (DWORD) dwExMask, (LPARAM) dwExStyles); }
BOOL ComboBoxEx::HasEditChanged()
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, CBEM_HASEDITCHANGED, 0, 0); }
ComboBox* ComboBoxEx::GetComboBoxCtrl()
{
	CC_ASSERT(::IsWindow(m_hWnd));
	if(_pInnerComboBox == NULL)
	{
		HWND hWnd = (HWND)::SendMessage(m_hWnd, CBEM_GETCOMBOCONTROL, 0, 0);
		if(hWnd != NULL)
		{
			_pInnerComboBox = new ComboBox();
			_pInnerComboBox->SubclassWindow(hWnd, this);
#if defined(_DEBUG_) || defined(_DESIGN)
			//code for design
			//flag for moveable or others
			_pInnerComboBox->Design = (DesignStyle::Type)DesignStyle::NoDesign;
#endif
		}
	}
	return _pInnerComboBox;
}
HIMAGELIST ComboBoxEx::SetImageList(HIMAGELIST hImageList)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (HIMAGELIST)::SendMessage(m_hWnd, CBEM_SETIMAGELIST, 0, (LPARAM)hImageList); }
HIMAGELIST ComboBoxEx::GetImageList() const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (HIMAGELIST)::SendMessage(m_hWnd, CBEM_GETIMAGELIST, 0, 0); }

// While ComboBoxEx derives from CComboBox, there are some
// CB_messages the underlying ComboBoxEx control doesn't support.

int ComboBoxEx::Dir(UINT attr, LPCTSTR lpszWildCard)
	{ CC_ASSERT(FALSE && _T("NOT VALID WITH ComboBoxEx")); return CB_ERR; }
BOOL ComboBoxEx::SetEditSel(int nStartChar, int nEndChar)
	{ CC_ASSERT(FALSE && _T("NOT VALID WITH ComboBoxEx")); return FALSE; }
// constructors/destructor
///////////////////////////////////////////////////////////////////////////////

/*
ES_AUTOHSCROLL 
Automatically scrolls text to the right by 10 characters when the user types a character at the end of the line. When the user presses the ENTER key, the control scrolls all text back to position zero.
ES_AUTOVSCROLL 
Automatically scrolls text up one page when the user presses the ENTER key on the last line.
ES_CENTER 
Windows 98/Me, Windows 2000/XP: Centers text in a single-line or multiline edit control.
Windows 95, Windows NT 4.0 and earlier: Centers text in a multiline edit control.

ES_LEFT 
Left aligns text.
ES_LOWERCASE 
Converts all characters to lowercase as they are typed into the edit control.
To change this style after the control has been created, use SetWindowLong.
ES_MULTILINE 
Designates a multiline edit control. The default is single-line edit control. 
When the multiline edit control is in a dialog box, the default response to pressing the ENTER key is to activate the default button. To use the ENTER key as a carriage return, use the ES_WANTRETURN style.
When the multiline edit control is not in a dialog box and the ES_AUTOVSCROLL style is specified, the edit control shows as many lines as possible and scrolls vertically when the user presses the ENTER key. If you do not specify ES_AUTOVSCROLL, the edit control shows as many lines as possible and beeps if the user presses the ENTER key when no more lines can be displayed.
If you specify the ES_AUTOHSCROLL style, the multiline edit control automatically scrolls horizontally when the caret goes past the right edge of the control. To start a new line, the user must press the ENTER key. If you do not specify ES_AUTOHSCROLL, the control automatically wraps words to the beginning of the next line when necessary. A new line is also started if the user presses the ENTER key. The window size determines the position of the Wordwrap. If the window size changes, the Wordwrapping position changes and the text is redisplayed.
Multiline edit controls can have scroll bars. An edit control with scroll bars processes its own scroll bar messages. Note that edit controls without scroll bars scroll as described in the previous paragraphs and process any scroll messages sent by the parent window.
ES_NOHIDESEL 
Negates the default behavior for an edit control. The default behavior hides the selection when the control loses the input focus and inverts the selection when the control receives the input focus. If you specify ES_NOHIDESEL, the selected text is inverted, even if the control does not have the focus.
ES_NUMBER 
Allows only digits to be entered into the edit control. Note that, even with this set, it is still possible to paste non-digits into the edit control.
To change this style after the control has been created, use SetWindowLong.
ES_OEMCONVERT 
Converts text entered in the edit control. The text is converted from the Windows character set to the OEM character set and then back to the Windows character set. This ensures proper character conversion when the application calls the CharToOem function to convert a Windows string in the edit control to OEM characters. This style is most useful for edit controls that contain file names that will be used on file systems that do not support Unicode. 
To change this style after the control has been created, use SetWindowLong. 
ES_PASSWORD 
Displays an asterisk (*) for each character typed into the edit control. This style is valid only for single-line edit controls. 
Windows XP: If the edit control is from user32.dll, the default password character is an asterisk. However, if the edit control is from comctl32.dll version 6, the default character is a black circle. 
To change the characters that is displayed, or set or clear this style, use the EM_SETPASSWORDCHAR message. 
Note  Comctl32.dll version 6 is not redistributable but it is included in Microsoft Windows XP or later. To use Comctl32.dll version 6, specify it in a manifest. For more information on manifests, see Using Windows XP Visual Styles.
ES_READONLY 
Prevents the user from typing or editing text in the edit control.
To change this style after the control has been created, use the EM_SETREADONLY message. 
ES_RIGHT 
Windows 98/Me, Windows 2000/XP: Right aligns text in a single-line or multiline edit control. Windows 95, Windows NT 4.0 and earlier: Right aligns text in a multiline edit control.
ES_UPPERCASE 
Converts all characters to uppercase as they are typed into the edit control. 
To change this style after the control has been created, use SetWindowLong.
ES_WANTRETURN 
Specifies that a carriage return be inserted when the user presses the ENTER key while entering text into a multiline edit control in a dialog box. If you do not specify this style, pressing the ENTER key has the same effect as pressing the dialog box's default push button. This style has no effect on a single-line edit control. 
To change this style after the control has been created, use SetWindowLong.
*/
//with ES_AUTOHSCROLL for more chars input than show length
DWORD TextBoxConfig::nStyle = WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_LEFT | ES_NOHIDESEL | ES_AUTOHSCROLL;
DWORD TextBoxConfig::nExStyle = WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR | WS_EX_CLIENTEDGE | WS_EX_NOPARENTNOTIFY;
//DWORD TextBoxConfig::nClassStyle = CS_DBLCLKS | CS_GLOBALCLASS | CS_PARENTDC;
DWORD TextBoxConfig::nInnerStyle = InnerStyle::TranceContextMenuUp;
DWORD TextBoxConfig::nOuterStyle = 0;
DWORD TextBoxConfig::nOwnerDrawTextStyle = DT_LEFT|DT_WORDBREAK;
COLORREF TextBoxConfig::corBackground = ::GetSysColor(COLOR_WINDOW);
COLORREF TextBoxConfig::corTextground = ::GetSysColor(COLOR_WINDOWTEXT);
COLORREF TextBoxConfig::corHotBackColor = ::GetSysColor(COLOR_WINDOW);
COLORREF TextBoxConfig::corHotTextColor = ::GetSysColor(COLOR_WINDOWTEXT);
COLORREF TextBoxConfig::corBackgroundDisabled = ::GetSysColor(COLOR_BTNFACE);
COLORREF TextBoxConfig::corBackgroundReadOnly = (COLORREF)((UINT)TextBoxConfig::corBackgroundDisabled + 0x00101010);
//===========================================================cc::win::TextBox
///////////////////////////////////////////////////////////////////////////////
// constructors/destructor
TextBox::TextBox()
{
	BASE_CONTROL_CONFIG_INIT(TextBox);
	_StyleArray[(int)StyleType::Inner2] |= Inner2Style::IsScrollRepaint;
	corBackgroundSaved = TextBoxConfig::corBackground;
	_Rect.right = _Rect.left + 70;
	_Rect.bottom = _Rect.top + 21;
	::CopyRect(&_OrgRect, &_Rect);
	_ClassName = WndClassName::TextBox;
	SetText(_T("TextBox"));
#ifdef _DEBUG_
	SetName(_T("TextBox"));
#endif
	_nMaxLength = 0;
}

TextBox::~TextBox()
{
}
// constructors/destructor

//keyArr is param address,but sHead is by value
void TextBox::DoConfig(bool isFromIni, KeyArr& keyArr, cc::Str sThisHead)
{
	if(!HasStyle(StyleType::Outer, OuterStyle::AutoConfig))
	{
		return;
	}
	if(isFromIni)
	{
		//if not exist,as usually
		//TODO:\r\n
		if(keyArr.Exist(sThisHead))
		{
			SetText(keyArr.Get(sThisHead, cc::Str()));
		}
	}
	else
	{
		keyArr.Set(sThisHead, cc::Str(GetText()));
	}
}

//here to create window
void TextBox::OnCreateControl()
{
	if(_nMaxLength > 0)
	{
		SetMaxLength(_nMaxLength);
	}
	Control::OnCreateControl();
}

void TextBox::OnCommand(EventArgs* e)
{
	switch(HIWORD(e->WParam))
	{
	case EN_SETFOCUS:
//CC_TRACE((_T("TextBox::OnCommand:EN_SETFOCUS")));
		//if set this flage,the select all text while GotFocus
		if(HasStyle(StyleType::Outer, OuterStyle::PrivateFlag1))
		{
			long nPos = (long)SendMessage(m_hWnd, WM_GETTEXTLENGTH, 0, 0);
			SendMessage(m_hWnd, EM_SETSEL, 0, nPos);
		}
		OnEnter(e);
		OnGotFocus(e);
		break;
	case EN_KILLFOCUS:
//CC_TRACE((_T("TextBox::OnCommand:EN_KILLFOCUS")));
		OnLeave(e);
		OnLostFocus(e);
		break;
	case EN_CHANGE:
//CC_TRACE((_T("TextBox::OnCommand:EN_CHANGE")));
		OnTextChanged(e);
		break;
	case EN_UPDATE:
//CC_TRACE((_T("TextBox::OnCommand:EN_UPDATE")));
		break;
	case EN_ERRSPACE:
		{
			CC_LOGSTEP();
			const TCHAR* msg = _T("TextBox::OnCommand, cannot allocate enough memory.");
			cc::Log::fatal(msg);
			CC_ASSERT(0);
			throw UserException(msg);
		}
	case EN_MAXTEXT:
//CC_TRACE((_T("TextBox::OnCommand:EN_MAXTEXT")));
		break;
	case EN_HSCROLL:
	case EN_VSCROLL:
		if(HasStyle(StyleType::Outer, OuterStyle::TransparentParentBrush|OuterStyle::TransparentParentErase|OuterStyle::TransparentHollowBrush))
		{
			InvalidateRect(m_hWnd, NULL, TRUE);
			::KillTimer(m_hWnd, CC_TIMERID_INVALIDATERECT);
			::SetTimer(m_hWnd, CC_TIMERID_INVALIDATERECT, 40, NULL);
		}
		break;
	}
}

void TextBox::OnTextChanged(EventArgs *e)
{
//CC_TRACE((_T("TextBox::OnTextChanged")));
	SetStyle(StyleType::Inner, 0, InnerStyle::IsDirty);
	Control::OnTextChanged(e);
}

void TextBox::OnMaxText(EventArgs *e)
{
//CC_TRACE((_T("TextBox::OnMaxText")));
	MaxText.Invoke((Object*)this, e);
}

bool TextBox::IsInputKey(UINT_PTR& keyData)
{
	if(keyData == Shortcut::CtrlA)
	{
		long nPos = (long)SendMessage(m_hWnd, WM_GETTEXTLENGTH, 0, 0);
		SendMessage(m_hWnd, EM_SETSEL, 0, nPos);
		return true;
	}
	if(keyData == VK_TAB && HasStyle(StyleType::Style, ES_MULTILINE))
	{
		return true;
	}
	//if(LOWORD(keyData) == VK_RETURN && HasStyle(StyleType::Style, ES_MULTILINE) && (keyData & (SHORTCUT_CTRL|SHORTCUT_ALT|SHORTCUT_SHIFT)) == 0)
	if(keyData == VK_RETURN && HasStyle(StyleType::Style, ES_MULTILINE))
	{
		return true;
	}
	return false;
}

//if readonly&disabled,then use backcolor of disabled
void TextBox::SetEnabled(bool isEnabled)
{
	bool isReadOnly = GetReadOnly();
	if(GetEnabled() && !isReadOnly)
	{
		corBackgroundSaved = _corBackground;
	}
	Control::SetEnabled(isEnabled);
	if(!isEnabled || isReadOnly)
	{
		SetBackground(!isEnabled ? TextBoxConfig::corBackgroundDisabled : TextBoxConfig::corBackgroundReadOnly);
	}
	else
	{
		SetBackground(corBackgroundSaved);
	}
}
//if readonly&disabled,then use backcolor of disabled
void TextBox::SetReadOnly(bool isReadOnly)
{
	bool isEnabled = GetEnabled();
	if(isEnabled && !GetReadOnly())
	{
		corBackgroundSaved = _corBackground;
	}
	if(isReadOnly)
	{
		SetStyle(StyleType::Style, 0, ES_READONLY);
	}
	else
	{
		SetStyle(StyleType::Style, ES_READONLY, 0);
	}
	if(!isEnabled || isReadOnly)
	{
		SetBackground(!isEnabled ? TextBoxConfig::corBackgroundDisabled : TextBoxConfig::corBackgroundReadOnly);
	}
	else
	{
		SetBackground(corBackgroundSaved);
	}
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
	{
		DefWndProc(EM_SETREADONLY, (WPARAM)(isReadOnly ? TRUE : FALSE), 0);
	}
}

bool TextBox::GetReadOnly()
{
	return HasStyle(StyleType::Style, ES_READONLY);
}

void TextBox::SetMultiline(bool isMultiline, bool isHScroll, bool isVScroll)
{
	DWORD dwAdd = 0;
	DWORD dwRemove = 0;
	if(isMultiline)
	{
		dwAdd |= ES_MULTILINE|ES_WANTRETURN;
	}
	else
	{
		dwRemove |= ES_MULTILINE|ES_WANTRETURN;
	}
	if(isHScroll)
	{
		dwAdd |= WS_HSCROLL;
	}
	else
	{
		dwRemove |= WS_HSCROLL;
		if(isMultiline)
		{
			//if not set this,then no autowrap text
			dwRemove |= ES_AUTOHSCROLL;
		}
		else
		{
			//if no ES_MULTILINE,without ES_AUTOHSCROLL then no more input than show length
			dwAdd |= ES_AUTOHSCROLL;
		}
	}
	if(isVScroll)
	{
		dwAdd |= WS_VSCROLL;
	}
	else
	{
		dwRemove |= WS_VSCROLL;
	}

	//SetStyle(StyleType::Style, 0, ES_MULTILINE|ES_WANTRETURN|ES_AUTOVSCROLL|WS_VSCROLL);
	SetStyle(StyleType::Style, dwRemove, dwAdd);
}

bool TextBox::GetMultiline()
{
	return HasStyle(StyleType::Style, ES_MULTILINE);
}
//
////for auto vscroll(while readonly and multiline)
//void TextBox::OnResize(EventArgs* e)
//{
//	if(m_hWnd != NULL && HasStyle(StyleType::Style, ES_READONLY|ES_MULTILINE|ES_AUTOVSCROLL))
//	{
//		int nRealH = 0;
//		RECT NewRect = this->GetBounds();
//		if(_Text != NULL)
//		{
//				HDC hDC = ::GetDC(this->m_hWnd);
//				HFONT hOldFont = (HFONT)::SelectObject(hDC, this->GetFont());
//
//				RECT rc = {0, 0, NewRect.right - NewRect.left, 0};
//				::DrawText(hDC, _Text, -1, &rc, DT_CALCRECT|DT_WORDBREAK|DT_EDITCONTROL);
//				nRealH = rc.bottom;
//				::SelectObject(hDC, hOldFont);
//				::ReleaseDC(this->m_hWnd, hDC);
//		}
//		if(nRealH > NewRect.bottom - NewRect.top)
//		{
//			_StyleArray[(int)StyleType::Style] |= WS_VSCROLL;
//		}
//		else
//		{
//			_StyleArray[(int)StyleType::Style] = _StyleArray[(int)StyleType::Style] & ~WS_VSCROLL;
//		}
//		::SetWindowLong(m_hWnd, GWL_STYLE, _StyleArray[(int)StyleType::Style]);
//		InvalidateRect(m_hWnd, NULL, TRUE);
//	}
//	Control::OnResize(e);
//}

void TextBox::SetMaxLength(int nMaxLength)
{
	_nMaxLength = nMaxLength;
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
	{
		::SendMessage(m_hWnd, EM_SETLIMITTEXT, _nMaxLength, 0);
	}
}

int TextBox::GetMaxLength()
{
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
	{
		_nMaxLength = (int)::SendMessage(m_hWnd, EM_GETLIMITTEXT, 0, 0);;
	}
	return _nMaxLength;
}

void TextBox::SetStyleNumber(bool value)
{
	SetStyleBool(StyleType::Style, ES_NUMBER|ES_RIGHT, value);
}

void TextBox::SetStyleUpper(bool value)
{
	SetStyleBool(StyleType::Style, ES_UPPERCASE, value);
}

void TextBox::SetIsDirty(bool isDirty)
{
	SetStyleBool(StyleType::Inner, InnerStyle::IsDirty, isDirty);
}

bool TextBox::GetIsDirty()
{
	return HasStyle(StyleType::Inner, InnerStyle::IsDirty);
}

int TextBox::GetTextInt()
{
	//return GetDlgItemInt(m_hWnd);
	const TCHAR* tc = GetText();
	if(tc == NULL) return 0;
	return _tstoi(tc);
}

double TextBox::GetTextDouble()
{
	const TCHAR* tc = GetText();
	if(tc == NULL) return 0;
	return _tstof(tc);
}

void TextBox::SetTextInt(int nText)
{
	//SetDlgItemInt;
	cc::Str str = cc::Str().ValueOf(nText);
	SetText(str);
}

bool TextBox::Undo()
	{ CC_ASSERT(::IsWindow(m_hWnd)); return ((BOOL)::SendMessage(m_hWnd, EM_UNDO, 0, 0) == TRUE); }
void TextBox::Clear()
{
	SetText(NULL);
	//::SendMessage(m_hWnd, WM_CLEAR, 0, 0);
}
void TextBox::Copy()
	{ CC_ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, WM_COPY, 0, 0); }
void TextBox::Cut()
	{ CC_ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, WM_CUT, 0, 0); }
void TextBox::Paste()
	{ CC_ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, WM_PASTE, 0, 0); }
bool TextBox::CanUndo() const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return ((BOOL)::SendMessage(m_hWnd, EM_CANUNDO, 0, 0) == TRUE); }
void TextBox::AppendText(const TCHAR* txt, bool isScrollBottom)
{
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
	{
		long nPos = (long)SendMessage(m_hWnd, WM_GETTEXTLENGTH, 0, 0);
		int nLen;
		if(_nMaxLength > 0 && _nMaxLength < nPos + (   nLen = (txt == NULL ? 0 : (int)_tcslen(txt))   ))
		{
			//cut head of text
			SendMessage(m_hWnd, EM_SETSEL, 0, nPos + nLen - _nMaxLength + 1);
			SendMessage(m_hWnd, EM_REPLACESEL, FALSE, (LPARAM)_T(""));
		}
		nPos = (long)SendMessage(m_hWnd, WM_GETTEXTLENGTH, 0, 0);
		SendMessage(m_hWnd, EM_SETSEL, nPos, nPos);
		SendMessage(m_hWnd, EM_REPLACESEL, false, (LPARAM)txt);
		if(isScrollBottom)
		{
			::SendMessage(m_hWnd, WM_VSCROLL, SB_BOTTOM, 0);
		}
	}
	else
	{
		SetText(cc::Str(GetText()).Append(txt));
	}
}
void TextBox::AppendText(bool isScrollBottom, const TCHAR* strFormat, ...)
{
	va_list argList;
	va_start(argList, strFormat);

	cc::Str str;
	str.FormatV(strFormat, argList);
	AppendText((const TCHAR*)str.GetBuf(), isScrollBottom);

	va_end(argList);
}
int TextBox::GetLineCount() const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, EM_GETLINECOUNT, 0, 0); }
bool TextBox::GetModify() const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return ((BOOL)::SendMessage(m_hWnd, EM_GETMODIFY, 0, 0) == TRUE); }
void TextBox::SetModify(bool bModified)
	{ CC_ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, EM_SETMODIFY, bModified ? TRUE : FALSE, 0); }
void TextBox::GetRect(LPRECT lpRect) const
	{ CC_ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, EM_GETRECT, 0, (LPARAM)lpRect); }
void TextBox::GetSel(int& nStartChar, int& nEndChar) const
	{ CC_ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, EM_GETSEL, (WPARAM)&nStartChar,(LPARAM)&nEndChar); }
DWORD TextBox::GetSel() const
//IA64: Assuming retval of EM_GETSEL won't be expanded
	{ CC_ASSERT(::IsWindow(m_hWnd)); return DWORD(::SendMessage(m_hWnd, EM_GETSEL, 0, 0)); }
HLOCAL TextBox::GetHandle() const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (HLOCAL)::SendMessage(m_hWnd, EM_GETHANDLE, 0, 0); }
void TextBox::SetHandle(HLOCAL hBuffer)
	{ CC_ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, EM_SETHANDLE, (WPARAM)hBuffer, 0); }
int TextBox::GetLine(int nIndex, LPTSTR lpszBuffer) const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, EM_GETLINE, nIndex, (LPARAM)lpszBuffer); }
int TextBox::GetLine(int nIndex, LPTSTR lpszBuffer, int nMaxLength) const
	{
		CC_ASSERT(::IsWindow(m_hWnd));

		*(LPWORD)lpszBuffer = (WORD)nMaxLength;
		return (int)::SendMessage(m_hWnd, EM_GETLINE, nIndex, (LPARAM)lpszBuffer);
	}
void TextBox::EmptyUndoBuffer()
	{ CC_ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, EM_EMPTYUNDOBUFFER, 0, 0); }
bool TextBox::FmtLines(bool bAddEOL)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return ((BOOL)::SendMessage(m_hWnd, EM_FMTLINES, bAddEOL ? TRUE : FALSE, 0) == TRUE); }
//void TextBox::LimitText(int nChars)
//	{ CC_ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, EM_LIMITTEXT, nChars, 0); }
int TextBox::LineFromChar(int nIndex) const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, EM_LINEFROMCHAR, nIndex, 0); }
int TextBox::LineIndex(int nLine) const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, EM_LINEINDEX, nLine, 0); }
int TextBox::LineLength(int nLine) const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, EM_LINELENGTH, nLine, 0); }
void TextBox::LineScroll(int nLines, int nChars)
	{ CC_ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, EM_LINESCROLL, nChars, nLines); }
void TextBox::ReplaceSel(LPCTSTR lpszNewText, bool bCanUndo)
	{ CC_ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, EM_REPLACESEL, (WPARAM)bCanUndo ? TRUE : FALSE, (LPARAM)lpszNewText); }
void TextBox::SetPasswordChar(TCHAR ch)
	{ CC_ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, EM_SETPASSWORDCHAR, ch, 0); }
void TextBox::SetRect(LPCRECT lpRect)
	{ CC_ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, EM_SETRECT, 0, (LPARAM)lpRect); }
void TextBox::SetRectNP(LPCRECT lpRect)
	{ CC_ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, EM_SETRECTNP, 0, (LPARAM)lpRect); }
void TextBox::SetSel(DWORD dwSelection, bool bNoScroll)
{
	CC_ASSERT(::IsWindow(m_hWnd));

	::SendMessage(m_hWnd, EM_SETSEL, LOWORD(dwSelection), HIWORD(dwSelection));
	if(!bNoScroll)
	{
		::SendMessage(m_hWnd, EM_SCROLLCARET, 0, 0);
	}
}
void TextBox::SetSel(int nStartChar, int nEndChar, bool bNoScroll)
{
	CC_ASSERT(::IsWindow(m_hWnd));

	::SendMessage(m_hWnd, EM_SETSEL, nStartChar, nEndChar);
	if(!bNoScroll)
	{
		::SendMessage(m_hWnd, EM_SCROLLCARET, 0, 0);
	}
}
bool TextBox::SetTabStops(int nTabStops, LPINT rgTabStops)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return ((BOOL)::SendMessage(m_hWnd, EM_SETTABSTOPS, nTabStops, (LPARAM)rgTabStops) == TRUE); }
void TextBox::SetTabStops()
	{ CC_ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, EM_SETTABSTOPS, 0, 0); }
bool TextBox::SetTabStops(const int& cxEachStop)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return ((BOOL)::SendMessage(m_hWnd, EM_SETTABSTOPS, 1, (LPARAM)(LPINT)&cxEachStop) == TRUE); }
//bool TextBox::SetReadOnly(bool bReadOnly)
//	{ CC_ASSERT(::IsWindow(m_hWnd)); return ((BOOL)::SendMessage(m_hWnd, EM_SETREADONLY, bReadOnly ? TRUE : FALSE, 0L) == TRUE); }
int TextBox::GetFirstVisibleLine() const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, EM_GETFIRSTVISIBLELINE, 0, 0L); }
TCHAR TextBox::GetPasswordChar() const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (TCHAR)::SendMessage(m_hWnd, EM_GETPASSWORDCHAR, 0, 0L); }
void TextBox::SetMargins(UINT nLeft, UINT nRight)
	{ CC_ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, EM_SETMARGINS, EC_LEFTMARGIN|EC_RIGHTMARGIN, MAKELONG(nLeft, nRight)); }
DWORD TextBox::GetMargins() const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (DWORD)::SendMessage(m_hWnd, EM_GETMARGINS, 0, 0); }
//void TextBox::SetLimitText(UINT nMax)
//	{ CC_ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, EM_SETLIMITTEXT, nMax, 0); }
//UINT TextBox::GetLimitText() const
//	{ CC_ASSERT(::IsWindow(m_hWnd)); return (UINT)::SendMessage(m_hWnd, EM_GETLIMITTEXT, 0, 0); }
POINT TextBox::PosFromChar(UINT nChar) const
{
	CC_ASSERT(::IsWindow(m_hWnd));

	POINT pt;
	DWORD d = (DWORD)::SendMessage(m_hWnd, EM_POSFROMCHAR, nChar, 0);
	pt.x = GET_X_LPARAM(d);
	pt.y = GET_Y_LPARAM(d);
	return pt;
}
int TextBox::CharFromPos(POINT pt) const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, EM_CHARFROMPOS, 0, MAKELPARAM(pt.x, pt.y)); }
///////////////////////////////////////////////////////////////////////////////

// Class implementation
STDMETHODIMP RichEditOleCallback::GetNewStorage(LPSTORAGE FAR *lplpstg)
{
	// Initialize a Storage Object from a DocFile in memory
	LPLOCKBYTES lpLockBytes = NULL;
	SCODE sc = ::CreateILockBytesOnHGlobal(NULL, TRUE, &lpLockBytes);
	if(sc != S_OK)
	{
		return sc;
	}
	sc = ::StgCreateDocfileOnILockBytes(lpLockBytes, STGM_SHARE_EXCLUSIVE|STGM_CREATE|STGM_READWRITE, 0, lplpstg);
	if(sc != S_OK)
	{
		lpLockBytes->Release();
	}
	return sc;
}

// read dwCount bytes into lpszBuffer, and return number read
// stop if source is empty, or when end of string reached
DWORD RichTextBoxStreamCookie::Read(LPBYTE lpBuffer, DWORD dwCount)
{
	if(lpBuffer == NULL)
	{
		return (DWORD)-1;
	}

	// have we already had it all?
	DWORD dwLeft = m_dwLength - m_dwCount;
	if(dwLeft <= 0)  // all done
	{
		return 0;
	}

	// start the source string from where we left off
	LPCSTR lpszText = (LPCSTR)m_sText.GetBuf() + m_dwCount;

	// only copy what we've got left
	if(dwLeft < dwCount)
	{
		dwCount = dwLeft;
	}

	// copy the text
#if __STDC_SECURE_LIB__
	strncpy_s((LPSTR)lpBuffer, dwCount, lpszText, _TRUNCATE);
#else
	strncpy((LPSTR)lpBuffer, lpszText, dwCount);
#endif // _MSC_VER

	// keep where we got to
	m_dwCount += dwCount;

	// return how many we copied
	return dwCount;
}

DWORD RichTextBoxStreamCookie::Write(LPBYTE lpBuffer, DWORD dwCount)
{
	if(lpBuffer == NULL)
	{
		return (DWORD)-1;
	}

	m_sText.Append((const char*)lpBuffer);
	// return how many we copied
	return dwCount;
}

DWORD CALLBACK RichTextBoxStreamCookie::CallbackStreamIn(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{
	// the cookie is a pointer to the text data struct
	RichTextBoxStreamCookie* pBuf = (RichTextBoxStreamCookie*)(DWORD_PTR)dwCookie;
	if(pBuf == NULL)
	{
	  return 1;
	}
	*pcb = pBuf->Read(pbBuff, cb);
	return 0;
}

DWORD CALLBACK RichTextBoxStreamCookie::CallbackStreamOut(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{
	// the cookie is a pointer to the text data struct
	RichTextBoxStreamCookie* pBuf = (RichTextBoxStreamCookie*)(DWORD_PTR)dwCookie;
	if(pBuf == NULL)
	{
	  return 1;
	}
	*pcb = pBuf->Write(pbBuff, cb);
	return 0;
}

//===========================================================cc::win::RichTextBox
///////////////////////////////////////////////////////////////////////////////
// constructors/destructor
RichTextBox::RichTextBox()
{
	//use of TextBox
	//BASE_CONTROL_CONFIG_INIT(RichTextBox);
	_Rect.right = _Rect.left + 70;
	_Rect.bottom = _Rect.top + 21;
	::CopyRect(&_OrgRect, &_Rect);
	_ClassName = WndClassName::RichTextBox;
	_StyleArray[(int)StyleType::InitControl] = CC_WNDCOMMCTLSNEW_REG;
	SetText(_T("RichTextBox"));
#ifdef _DEBUG_
	SetName(_T("RichTextBox"));
#endif
	m_oleCallback = NULL;
}

RichTextBox::~RichTextBox()
{
	//if(m_oleCallback != NULL)
	//{
	//	delete m_oleCallback;
	//	m_oleCallback = NULL;
	//}
}

//here to create window
void RichTextBox::CreateHandle()
{
	_APP_THREAD_STATE* pState = CC_APP_GetThreadStateData();
	//will be released at ~App
	if(pState->m_hInstRichEdit == NULL)
	{
		pState->m_hInstRichEdit = LoadLibrary(_T("RICHED20.DLL"));//Rich Edit Version 2.0/3.0
		if(pState->m_hInstRichEdit == NULL)
		{
			pState->m_hInstRichEdit = LoadLibrary(_T("RICHED32.DLL"));//Rich Edit Version 1.0
		}
	}
	Control::CreateHandle();
	::SendMessage(m_hWnd, EM_SETBKGNDCOLOR, FALSE, _corBackground);

	m_oleCallback = (cc_IRichEditOleCallback*)(new RichEditOleCallback);
	m_oleCallback->AddRef();
	::SendMessage(m_hWnd, EM_SETOLECALLBACK, 0, (LPARAM) m_oleCallback);
	m_oleCallback->Release();
}

void RichTextBox::OnChildNotify(EventArgs *e)
{
	//LPNMHDR p_pNotifyStruct = (LPNMHDR)e->LParam;
	//if(p_pNotifyStruct->code == EN_LINK)
	//{
	//	ENLINK *pLink = (ENLINK*)e->LParam;
	//	if(pLink->msg == WM_LBUTTONUP)
	//	{
	//		ClickLink.Invoke((Object*)this, e);
	//	}
	//	//CC_TRACE((_T("RichTextBox::OnChildNotify, EN_LINK, msg:%d"), e->Msg));
	//}
	TextBox::OnChildNotify(e);
}

const cc::Str RichTextBox::GetTextSel()
{
	cc::Str str;
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
	{
		int nStartChar;
		int nEndChar;
		TextBox::GetSel(nStartChar, nEndChar);
		if(nStartChar >=0 && nEndChar > nStartChar)
		{
			TCHAR* tc = (TCHAR*)str.GetBufferSetLength(nEndChar - nStartChar);
			//_Text = (TCHAR*)malloc((nEndChar - nStartChar + 2) * sizeof(TCHAR));
			//memset(_Text, 0, (nEndChar - nStartChar + 2) * sizeof(TCHAR));
			::SendMessage(m_hWnd, EM_GETSELTEXT, 0, (LPARAM)tc);
		}
	}
	return str;
}

void RichTextBox::SetBackground(COLORREF corBackground)
{
	Control::SetBackground(corBackground);
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
	{
		CC_ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, EM_SETBKGNDCOLOR, FALSE, _corBackground);
	}
}

BOOL RichTextBox::SetAutoURLDetect(BOOL bEnable /* = TRUE */)
{
	CC_ASSERT(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, EM_AUTOURLDETECT, (WPARAM)bEnable, 0);
}
//retrieves the starting and ending character positions of the selection in a rich edit control.
void RichTextBox::GetSel(CHARRANGE &cr) const
{
	CC_ASSERT(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, EM_EXGETSEL, 0, (LPARAM)&cr);
}
//sets an upper limit to the amount of text the user can type or paste into a rich edit control.
void RichTextBox::LimitText(long nChars)
	{ CC_ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, EM_EXLIMITTEXT, 0, nChars); }
long RichTextBox::LineFromChar(long nIndex) const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (long)::SendMessage(m_hWnd, EM_EXLINEFROMCHAR, 0, nIndex); }
void RichTextBox::SetSel(CHARRANGE &cr)
	{ CC_ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, EM_EXSETSEL, 0, (LPARAM)&cr); }
DWORD RichTextBox::FindWordBreak(UINT nCode, DWORD nStart) const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (DWORD)::SendMessage(m_hWnd, EM_FINDWORDBREAK, (WPARAM) nCode, (LPARAM) nStart); }
POINT RichTextBox::PosFromChar(UINT nChar) const
{ CC_ASSERT(::IsWindow(m_hWnd)); POINTL pt; ::SendMessage(m_hWnd, EM_POSFROMCHAR, (WPARAM)&pt, nChar); POINT pt2 = {pt.x, pt.y}; return pt2; }
int RichTextBox::CharFromPos(POINT pt) const
	{ CC_ASSERT(::IsWindow(m_hWnd)); POINTL ptl = {pt.x, pt.y}; return (int)::SendMessage(m_hWnd, EM_CHARFROMPOS, 0, (LPARAM)&ptl); }
//COLORREF RichTextBox::SetBackgroundColor(BOOL bSysColor, COLORREF cr)
//	{ CC_ASSERT(::IsWindow(m_hWnd)); return (COLORREF)::SendMessage(m_hWnd, EM_SETBKGNDCOLOR, bSysColor, cr); }
BOOL RichTextBox::SetOLECallback(cc_IRichEditOleCallback* pCallback)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (BOOL)::SendMessage(m_hWnd, EM_SETOLECALLBACK, 0, (LPARAM)pCallback); }
long RichTextBox::StreamIn(int nFormat, EDITSTREAM &es)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (long)::SendMessage(m_hWnd, EM_STREAMIN, nFormat, (LPARAM)&es); }
long RichTextBox::StreamOut(int nFormat, EDITSTREAM &es)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (long)::SendMessage(m_hWnd, EM_STREAMOUT, nFormat, (LPARAM)&es); }
long RichTextBox::StreamIn(int nFormat, const char* buf)
{
	CC_ASSERT(::IsWindow(m_hWnd));

	EDITSTREAM es;
	RichTextBoxStreamCookie cookie(buf);
	es.dwCookie = (DWORD)(DWORD_PTR)&cookie;
	es.pfnCallback = (EDITSTREAMCALLBACK)RichTextBoxStreamCookie::CallbackStreamIn;
	return StreamIn(nFormat, es);
}
long RichTextBox::StreamOut(int nFormat, cc::Str& buf)
{
	CC_ASSERT(::IsWindow(m_hWnd));

	EDITSTREAM es;
	RichTextBoxStreamCookie cookie((char*)"");
	es.dwCookie = (DWORD)(DWORD_PTR)&cookie;
	es.pfnCallback = (EDITSTREAMCALLBACK)RichTextBoxStreamCookie::CallbackStreamOut;
	long n = StreamOut(nFormat, es);
	buf.TakeoverPointer(cookie.m_sText);
	return n;
}

//	// set to reveal multiple undo
//	::SendMessage(m_hWnd, EM_SETTEXTMODE, TM_RICHTEXT | TM_MULTILEVELUNDO | TM_MULTICODEPAGE, 0);
//#define EM_UNDO 0xC7
//#define EM_REDO (WM_USER + 84)
//#define EM_CANUNDO 0xC6
//#define EM_CANREDO (WM_USER + 85)
//#define EM_GETUNDONAME (WM_USER + 86)
//#define EM_GETREDONAME (WM_USER + 87)
// constructors/destructor
///////////////////////////////////////////////////////////////////////////////

DWORD ScrollBarConfig::nStyle = WS_CHILDWINDOW | WS_VISIBLE | SBS_HORZ | SBS_BOTTOMALIGN;
DWORD ScrollBarConfig::nExStyle = WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR | WS_EX_NOPARENTNOTIFY;
//DWORD ScrollBarConfig::nClassStyle = CS_DBLCLKS | CS_GLOBALCLASS | CS_PARENTDC;
DWORD ScrollBarConfig::nInnerStyle = InnerStyle::TranceContextMenuUp;
DWORD ScrollBarConfig::nOuterStyle = 0;
DWORD ScrollBarConfig::nOwnerDrawTextStyle = DT_LEFT|DT_VCENTER|DT_SINGLELINE;
COLORREF ScrollBarConfig::corBackground = ::GetSysColor(COLOR_SCROLLBAR);
COLORREF ScrollBarConfig::corTextground = ::GetSysColor(COLOR_BTNTEXT);
COLORREF ScrollBarConfig::corHotBackColor = ::GetSysColor(COLOR_SCROLLBAR);
COLORREF ScrollBarConfig::corHotTextColor = ::GetSysColor(COLOR_BTNTEXT);
//===========================================================cc::win::ScrollBar
///////////////////// constructors //////////////////////////
ScrollBar::ScrollBar()
{
	BASE_CONTROL_CONFIG_INIT(ScrollBar);
	//_StyleArray[(int)StyleType::Inner2] |= Inner2Style::IsScrollRepaint;
	_Rect.right = _Rect.left + 70;
	_Rect.bottom = _Rect.top + 17;
	::CopyRect(&_OrgRect, &_Rect);
	_ClassName = WndClassName::ScrollBar;
#ifdef _DEBUG_
	SetName(_T("ScrollBar"));
#endif

	ZeroMemory(&si, sizeof(si));
	si.cbSize = sizeof(si);
	_nMinimum = 0;
	_nMaximum = 100;
	_nLargeChange = 10;
	_nSmallChange = 1;
	_nValue = 0;
}

///////////////////// destructor //////////////////////////
ScrollBar::~ScrollBar()
{
}

void ScrollBar::SetMinMaxValue(int nMin, int nMax, bool bRedraw)
{
	_nMinimum = nMin;
	_nMaximum = nMax;
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
	{
		si.fMask = SIF_RANGE;
		//::GetScrollInfo(m_hWnd, SB_CTL, &si);
		si.nMin   = _nMinimum;
		si.nMax   = _nMaximum;
		::SetScrollInfo(m_hWnd, SB_CTL, &si, bRedraw ? TRUE : FALSE);
	}
}

int ScrollBar::GetMinValue()
{
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
	{
		//::GetScrollRange(m_hWnd, SB_CTL, &_nMinimum, &_nMaximum);
		si.fMask = SIF_RANGE;
		::GetScrollInfo(m_hWnd, SB_CTL, &si);
		_nMinimum = si.nMin;
	}
	return _nMinimum;
}

//void ScrollBar::SetMaximum(int nMaximum, bool bRedraw)
//{
//	_nMaximum = nMaximum;
//	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
//	{
//		si.cbSize = sizeof(si);
//		si.fMask = SIF_ALL;
//		::GetScrollInfo(m_hWnd, SB_CTL, &si);
//		si.fMask  = SIF_RANGE;
//		si.nMax   = _nMaximum;
//		::SetScrollInfo(m_hWnd, SB_CTL, &si, bRedraw ? TRUE : FALSE);
//	}
//}

int ScrollBar::GetMaxValue()
{
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
	{
		//::GetScrollRange(m_hWnd, SB_CTL, &_nMinimum, &_nMaximum);
		si.fMask = SIF_RANGE;
		::GetScrollInfo(m_hWnd, SB_CTL, &si);
		_nMaximum = si.nMax;
	}
	return _nMaximum;
}

void ScrollBar::SetLargeChange(int nLargeChange)
{
	_nLargeChange = nLargeChange;
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
	{
		si.fMask = SIF_PAGE;
		si.nPage = _nLargeChange;
		::SetScrollInfo(m_hWnd, SB_CTL, &si, TRUE);
	}
}

int ScrollBar::GetLargeChange()
{
	return _nLargeChange;
}

void ScrollBar::SetSmallChange(int nSmallChange)
{
	_nSmallChange = nSmallChange;
}

int ScrollBar::GetSmallChange()
{
	return _nSmallChange;
}

int ScrollBar::SetValue(int nValue, bool bRedraw)
{
	_nValue = nValue;
	if(_nValue < _nMinimum)
	{
		_nValue = _nMinimum;
	}
	if(_nValue > _nMaximum)
	{
		_nValue = _nMaximum;
	}
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
	{
		si.fMask = SIF_POS;
		//::GetScrollInfo(m_hWnd, SB_CTL, &si);
		si.nPos   = _nValue;
		_nValue = ::SetScrollInfo(m_hWnd, SB_CTL, &si, bRedraw ? TRUE : FALSE);
		EventArgs ee(m_hWnd,
			HasStyle(StyleType::Style, SBS_HORZ) ? WM_HSCROLL : WM_VSCROLL, MAKEWPARAM(SB_THUMBPOSITION, _nValue), _nValue);
		//NewValue = (int)HIWORD(e->WParam);
		//int nValue = (int)e->LParam;
		OnValueChanged(&ee);
	}
	return _nValue;
}

int ScrollBar::GetValue()
{
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
	{
		si.fMask = SIF_POS;
		::GetScrollInfo(m_hWnd, SB_CTL, &si);
		_nValue = si.nPos; //::GetScrollPos(m_hWnd, SB_CTL);
	}
	return _nValue;
}

void ScrollBar::ShowScrollBar(BOOL bShow)
	{ CC_ASSERT(::IsWindow(m_hWnd)); ::ShowScrollBar(m_hWnd, SB_CTL, bShow); }
bool ScrollBar::EnableScrollBar(UINT nArrowFlags)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return ((BOOL)::EnableScrollBar(m_hWnd, SB_CTL, nArrowFlags) == TRUE); }
//int ScrollBar::GetScrollLimit()
//	{ CC_ASSERT(::IsWindow(m_hWnd)); return Control::GetScrollLimit(SB_CTL); }
//	{ return _nMaximum; }
//void ScrollBar::SetScrollRange(int nMinPos, int nMaxPos, bool bRedraw)
//{
//	CC_ASSERT(::IsWindow(m_hWnd));
//
//	_nMinimum = nMinPos;
//	_nMaximum = nMaxPos;
//	::SetScrollRange(m_hWnd, SB_CTL, nMinPos, nMaxPos, bRedraw ? TRUE : FALSE);
//}
//void ScrollBar::GetScrollRange(LPINT lpMinPos, LPINT lpMaxPos) const
//	{ CC_ASSERT(::IsWindow(m_hWnd)); ::GetScrollRange(m_hWnd, SB_CTL, lpMinPos, lpMaxPos); }

void ScrollBar::OnHandleCreated(EventArgs *e)
{
	CC_ASSERT(::IsWindow(m_hWnd));

	Control::OnHandleCreated(e);
	ZeroMemory(&si, sizeof(si));
	si.cbSize = sizeof(si);
	si.fMask = SIF_ALL;
	::GetScrollInfo(m_hWnd, SB_CTL, &si);
	si.nMin = _nMinimum;
	si.nPos = _nValue;
	//if(_nLargeChange > 0)
	//{
		si.nPage = _nLargeChange;
		si.nMax = _nMaximum + _nLargeChange - 1;
	//}
	//else
	//{
	//	si.nMax = _nMaximum;
	//}
	::SetScrollInfo(m_hWnd, SB_CTL, &si, TRUE);
}

/*
wParam
    The low-order word specifies a scroll bar value that indicates the user's scrolling request. This word can be one of the following values.
    The high-order word specifies the current position of the scroll box if the low-order word is SB_THUMBPOSITION or SB_THUMBTRACK;
    otherwise, this word is not used.

    SB_ENDSCROLL
        Ends scroll.
    SB_LEFT
        Scrolls to the upper left.
    SB_RIGHT
        Scrolls to the lower right.
    SB_LINELEFT
        Scrolls left by one unit.
    SB_LINERIGHT
        Scrolls right by one unit.
    SB_PAGELEFT
        Scrolls left by the width of the window.
    SB_PAGERIGHT
        Scrolls right by the width of the window.
    SB_THUMBPOSITION
        The user has dragged the scroll box (thumb) and released the mouse button. The high-order word indicates
        the position of the scroll box at the end of the drag operation.
    SB_THUMBTRACK
        The user is dragging the scroll box. This message is sent repeatedly until the user releases the mouse button.
        The high-order word indicates the position that the scroll box has been dragged to.
lParam
    If the message is sent by a scroll bar, then this parameter is the handle to the scroll bar control. If the message
    is not sent by a scroll bar, this parameter is NULL.
*/
void ScrollBar::OnChildNotify(EventArgs *e)
{
	if(e->Msg != WM_HSCROLL && e->Msg != WM_VSCROLL)
	{
		return Control::OnChildNotify(e);
	}
	ScrollEventArgs ee(e);
	ee.Horizontal = (e->Msg == WM_HSCROLL);
	ee.NewValue = (int)HIWORD(e->WParam);
	OnScroll(&ee);
}

void ScrollBar::OnScroll(ScrollEventArgs *e)
{
	int nos = _nValue;
	//if(!Scroll.IsEmpty())
	//{
	//	Scroll.Invoke((Object*)this, e);
	//}
	//else
	//{
	//	DefaultScroll(e);
	//}
	Control::OnScroll(e);
	if(!e->baseArgs->Handled)
	{
		DefaultScroll(e);
	}
	if(nos != _nValue)
	{
		e->baseArgs->WParam = MAKEWPARAM(LOWORD(e->baseArgs->WParam), (LPARAM)_nValue);
		e->baseArgs->LParam = (LPARAM)_nValue;
		//int nValue = (int)e->LParam;
		OnValueChanged(e->baseArgs);
	}
}

//int nValue = (int)e->LParam;
void ScrollBar::OnValueChanged(EventArgs *e)
{
	ValueChanged.Invoke((Object*)this, e);
}

void ScrollBar::DefaultScroll(ScrollEventArgs *e)
{
	si.cbSize = sizeof(si);
	si.fMask = SIF_ALL;
	::GetScrollInfo(m_hWnd, SB_CTL, &si);
	//err(_T("nMin:%d, nMax:%d, Value:%d, nTrackPos:%d, nPos:%d, HI WParam:%d, LO WParam:%d"), si.nMin, si.nMax, _nValue, si.nTrackPos, si.nPos, HIWORD(e->baseArgs->WParam), LOWORD(e->baseArgs->WParam));
	switch (LOWORD(e->baseArgs->WParam))
	{
	case SB_TOP:
		si.nPos = si.nMin;
		break;
	case SB_BOTTOM:
		si.nPos = si.nMax;
		break;
	case SB_PAGELEFT:
		si.nPos -= _nLargeChange;
		if(si.nPos < si.nMin)
		{
			si.nPos = si.nMin;
		}
		break;
	case SB_PAGERIGHT:
		si.nPos += _nLargeChange;
		if(si.nPos > si.nMax)
		{
			si.nPos = si.nMax;
		}
		break;
	case SB_LINELEFT:
		si.nPos -= _nSmallChange;
		if(si.nPos < si.nMin)
		{
			si.nPos = si.nMin;
		}
		break;
	case SB_LINERIGHT:
		si.nPos += _nSmallChange;
		if(si.nPos > si.nMax)
		{
			si.nPos = si.nMax;
		}
		break;
	case SB_THUMBTRACK:
	case SB_THUMBPOSITION:
		si.nPos = si.nTrackPos;
		//si.nPos = HIWORD(e->baseArgs->WParam);
		break;
	case SB_ENDSCROLL:
		break;
	}
	si.fMask  = SIF_POS;
	_nValue = si.nPos;
	//err(_T("nPos:%d, _nValue:%d, HIWORD(e->baseArgs->WParam):%d, si.nTrackPos:%d"), si.nPos, _nValue, HIWORD(e->baseArgs->WParam), si.nTrackPos);
	::SetScrollInfo(m_hWnd, SB_CTL, &si, TRUE);
}

//===========================================================cc::win::HScrollBar
///////////////////// constructors //////////////////////////
HScrollBar::HScrollBar()
{
	//use of base
	//BASE_CONTROL_CONFIG_INIT(HScrollBar);
	//nStyle = WS_CHILDWINDOW | WS_VISIBLE | SBS_HORZ | SBS_BOTTOMALIGN;
	SetStyle(StyleType::Style, SBS_VERT|SBS_RIGHTALIGN, SBS_HORZ|SBS_BOTTOMALIGN);
	_Rect.right = _Rect.left + 70;
	_Rect.bottom = _Rect.top + 17;
	::CopyRect(&_OrgRect, &_Rect);
#ifdef _DEBUG_
	SetName(_T("HScrollBar"));
#endif
}

///////////////////// destructor //////////////////////////
HScrollBar::~HScrollBar()
{
}

//===========================================================cc::win::VScrollBar
///////////////////// constructors //////////////////////////
VScrollBar::VScrollBar()
{
	//use of base
	//BASE_CONTROL_CONFIG_INIT(VScrollBar);
	//nStyle = WS_CHILDWINDOW | WS_VISIBLE | SBS_VERT | SBS_RIGHTALIGN;
	SetStyle(StyleType::Style, SBS_HORZ|SBS_BOTTOMALIGN, SBS_VERT|SBS_RIGHTALIGN);
	_Rect.right = _Rect.left + 18;
	_Rect.bottom = _Rect.top + 60;
	::CopyRect(&_OrgRect, &_Rect);
#ifdef _DEBUG_
	SetName(_T("VScrollBar"));
#endif
}

///////////////////// destructor //////////////////////////
VScrollBar::~VScrollBar()
{
}

DWORD ProgressBarConfig::nStyle = WS_CHILDWINDOW | WS_VISIBLE;
DWORD ProgressBarConfig::nExStyle = 0;
//DWORD ProgressBarConfig::nClassStyle = CS_DBLCLKS | CS_GLOBALCLASS | CS_PARENTDC;
DWORD ProgressBarConfig::nInnerStyle = InnerStyle::TranceContextMenuUp;
DWORD ProgressBarConfig::nOuterStyle = 0;
DWORD ProgressBarConfig::nOwnerDrawTextStyle = DT_LEFT|DT_VCENTER|DT_SINGLELINE;
COLORREF ProgressBarConfig::corBackground = ::GetSysColor(COLOR_SCROLLBAR);
COLORREF ProgressBarConfig::corTextground = ::GetSysColor(COLOR_ACTIVECAPTION);
COLORREF ProgressBarConfig::corHotBackColor = ::GetSysColor(COLOR_SCROLLBAR);
COLORREF ProgressBarConfig::corHotTextColor = ::GetSysColor(COLOR_ACTIVECAPTION);
int ProgressBarConfig::isStyleSmooth = 0;
//===========================================================cc::win::ProgressBar
///////////////////// constructors //////////////////////////
ProgressBar::ProgressBar()
{
	BASE_CONTROL_CONFIG_INIT(ProgressBar);
	_Rect.right = _Rect.left + 70;
	_Rect.bottom = _Rect.top + 17;
	::CopyRect(&_OrgRect, &_Rect);
	_ClassName = WndClassName::ProgressBar;
	_StyleArray[(int)StyleType::InitControl] = CC_WNDCOMMCTL_PROGRESS_REG;
#ifdef PBS_SMOOTH
	if(ProgressBarConfig::isStyleSmooth != 0)
	{
		_StyleArray[(int)StyleType::Style] |= PBS_SMOOTH;
	}
#endif
#ifdef _DEBUG_
	SetName(_T("ProgressBar"));
#endif

	_nLower = 0;
	_nUpper = 100;
	_nPos = 0;
	_nOffPos = 0;
	_nStep = 10;
	_nStepIt = 0;
}

///////////////////// destructor //////////////////////////
ProgressBar::~ProgressBar()
{
}

//#define PBM_SETBARCOLOR         (WM_USER+9)             // lParam = bar color
void ProgressBar::SetTextColor(COLORREF corText)
{
	_corTextground = corText;
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
	{
		::SendMessage(m_hWnd, PBM_SETBARCOLOR, 0, (LPARAM)_corTextground);
	}
}
//The SetBarColor method sets the progress bar color only if a Windows Vista theme is not in effect.
void ProgressBar::SetBackground(COLORREF clrNew)
{
	_corBackground = clrNew;
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
	{
		::SendMessage(m_hWnd, PBM_SETBKCOLOR, 0, (LPARAM)_corBackground);
	}
}
void ProgressBar::SetRange16(short nLower, short nUpper)
{
	CC_ASSERT(nLower < nUpper);
	_nLower = (int)nLower;
	_nUpper = (int)nUpper;
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
	{
		::SendMessage(m_hWnd, PBM_SETRANGE, 0, MAKELPARAM(nLower, nUpper));
	}
}
void ProgressBar::SetRange(int nLower, int nUpper)
{
	CC_ASSERT(nLower < nUpper);
	_nLower = nLower;
	_nUpper = nUpper;
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
	{
		::SendMessage(m_hWnd, PBM_SETRANGE32, (WPARAM)_nLower, (LPARAM)_nUpper);
	}
}
void ProgressBar::GetRange(int& nLower, int& nUpper)
{
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
	{
		PBRANGE range;
		::SendMessage(m_hWnd, PBM_GETRANGE, (WPARAM)FALSE, (LPARAM)&range);
		_nLower = range.iLow;
		_nUpper = range.iHigh;
	}
	nLower = _nLower;
	nUpper = _nUpper;
}
int ProgressBar::GetPos()
{
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
	{
		_nPos = (int)::SendMessage(m_hWnd, PBM_GETPOS, 0, 0);
	}
	return _nPos;
}
void ProgressBar::SetPos(int nPos)
{
	_nPos = nPos;
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
	{
		::SendMessage(m_hWnd, PBM_SETPOS, _nPos, 0L);
	}
}
void ProgressBar::OffsetPos(int nPos)
{
	_nOffPos = nPos;
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
	{
		::SendMessage(m_hWnd, PBM_DELTAPOS, _nOffPos, 0L);
	}
}
int ProgressBar::GetStep()
{
	return _nStep;
}
void ProgressBar::SetStep(int nStep)
{
	_nStep = nStep;
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
	{
		::SendMessage(m_hWnd, PBM_SETSTEP, _nStep, 0L);
	}
}
int ProgressBar::StepIt()
{
	CC_ASSERT(::IsWindow(m_hWnd));
	return (int)::SendMessage(m_hWnd, PBM_STEPIT, 0, 0L);
}

void ProgressBar::OnHandleCreated(EventArgs *e)
{
	CC_ASSERT(::IsWindow(m_hWnd));

	Control::OnHandleCreated(e);
	if(_corBackground != ::GetSysColor(COLOR_SCROLLBAR))
	{
		::SendMessage(m_hWnd, PBM_SETBARCOLOR, 0, (LPARAM)_corTextground);
	}
	if(_corTextground != ::GetSysColor(COLOR_ACTIVECAPTION))
	{
		::SendMessage(m_hWnd, PBM_SETBKCOLOR, 0, (LPARAM)_corBackground);
	}
	::SendMessage(m_hWnd, PBM_SETRANGE32, (WPARAM)_nLower, (LPARAM)_nUpper);
	::SendMessage(m_hWnd, PBM_SETPOS, _nPos, 0L);
	::SendMessage(m_hWnd, PBM_SETSTEP, _nStep, 0L);
}

DWORD ListViewConfig::nStyle = WS_CHILDWINDOW | WS_VISIBLE | WS_TABSTOP | LVS_ICON | LVS_ALIGNLEFT;
DWORD ListViewConfig::nExStyle = WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR | WS_EX_NOPARENTNOTIFY | WS_EX_CLIENTEDGE;
//DWORD ListViewConfig::nClassStyle = CS_DBLCLKS | CS_GLOBALCLASS | CS_PARENTDC;
DWORD ListViewConfig::nInnerStyle = InnerStyle::TranceContextMenuUp;
DWORD ListViewConfig::nOuterStyle = 0;
DWORD ListViewConfig::nOwnerDrawTextStyle = DT_LEFT|DT_VCENTER|DT_SINGLELINE;
COLORREF ListViewConfig::corBackground = ::GetSysColor(CTLCOLOR_LISTBOX);
COLORREF ListViewConfig::corTextground = ::GetSysColor(COLOR_BTNTEXT);
COLORREF ListViewConfig::corHotBackColor = ::GetSysColor(CTLCOLOR_LISTBOX);
COLORREF ListViewConfig::corHotTextColor = ::GetSysColor(COLOR_BTNTEXT);
//===========================================================cc::win::ListView
///////////////////////////////////////////////////////////////////////////////
// constructors/destructor
ListView::ListView()
{
	BASE_CONTROL_CONFIG_INIT(ListView);
	_StyleArray[(int)StyleType::Inner2] |= Inner2Style::IsScrollRepaint;
	_Rect.right = _Rect.left + 105;
	_Rect.bottom = _Rect.top + 75;
	::CopyRect(&_OrgRect, &_Rect);
	_ClassName = WndClassName::ListView;
	_StyleArray[(int)StyleType::InitControl] = CC_WNDCOMMCTL_LISTVIEW_REG;
#ifdef _DEBUG_
	SetName(_T("ListView"));
#endif
}

ListView::~ListView()
{
}
// constructors/destructor

void ListView::OnSelectedIndexChanging(EventArgs* e)
{
	SelectedIndexChanging.Invoke((Object*)this, e);
}

//void ListView::OnItemClick(EventArgs* e)
//{
//	ItemClick.Invoke((Object*)this, e);
//}

void ListView::OnItemDoubleClick(EventArgs* e)
{
	ItemDoubleClick.Invoke((Object*)this, e);
}

void ListView::OnSelectedIndexChanged(EventArgs* e)
{
	SelectedIndexChanged.Invoke((Object*)this, e);
}

void ListView::OnSetDispinfo(EventArgs* e)
{
	CC_ASSERT(::IsWindow(m_hWnd));
	SetDispinfo.Invoke((Object*)this, e);
}

void ListView::OnGetDispinfo(EventArgs* e)
{
	CC_ASSERT(::IsWindow(m_hWnd));
	GetDispinfo.Invoke((Object*)this, e);
}
void ListView::OnBeginLabelEdit(EventArgs* e)
{
	CC_ASSERT(::IsWindow(m_hWnd));
	BeginLabelEdit.Invoke((Object*)this, e);
}
void ListView::OnEndLabelEdit(EventArgs* e)
{
	CC_ASSERT(::IsWindow(m_hWnd));
	EndLabelEdit.Invoke((Object*)this, e);
}

void ListView::OnChildNotify(EventArgs *e)
{
	if(e->Msg == WM_DRAWITEM)
	{
		DrawItemEventArgs ee(e);
		ee.data = (LPDRAWITEMSTRUCT)e->LParam;
		OnDrawItem(&ee);
		return;
	}
	LPNMHDR p_pNotifyStruct = (LPNMHDR)e->LParam;

	// Custom control notification messages handled here

	// Control handle is g_hControl
	// Parent handle is g_hContainer
	// Main Dialog handle is g_hDialog
	// Instance handle is g_hInstance

	//NMLVDISPINFO* pNMLVDispInfo;
	//LPNMLVGETINFOTIP pNMLVGetInfoTip;
	////LPNMITEMACTIVATE pNMItemActivate;
	//LPNMLVKEYDOWN pNMLVKeyDown;
	//NMLVCACHEHINT* pNMLVCacheHint;
	//NMLVFINDITEM* pNMLVFindItem;
	//LPNMLVODSTATECHANGE pNMLVODStateChange;
	//LPNMLISTVIEW pNMListView;
	//LPNMLVCUSTOMDRAW pNMLVCustomDraw;
	switch(p_pNotifyStruct->code)
	{
		case LVN_BEGINDRAG:  // Drag-and-drop operation involving the left mouse, track via NMLISTVIEW
			//pNMListView = (LPNMLISTVIEW)p_pNotifyStruct;
			// Add code here
			{
				DragBeginEventArgs ee(e);
				ee.Button = MouseButtons::Left;
				OnDragBegin(&ee);
			}
			break;
		case LVN_BEGINLABELEDIT:  // Item label in-place edit started, track via NMLVDISPINFO
			//pNMLVDispInfo = (NMLVDISPINFO*)p_pNotifyStruct;
			// Add code here
			OnBeginLabelEdit(e);
			break;
		case LVN_BEGINRDRAG:  // Drag-and-drop operation involving the right mouse, track via NMLISTVIEW
			//pNMListView = (LPNMLISTVIEW)p_pNotifyStruct;
			// Add code here
			{
				DragBeginEventArgs ee(e);
				ee.Button = MouseButtons::Right;
				OnDragBegin(&ee);
			}
			break;
		case LVN_COLUMNCLICK:  // User clicked a column, track via NMLISTVIEW
			//pNMListView = (LPNMLISTVIEW)p_pNotifyStruct;
			// Add code here
			break;
		case LVN_DELETEALLITEMS:  // All items in control are about to be deleted, track via NMLISTVIEW
			//pNMListView = (LPNMLISTVIEW)p_pNotifyStruct;
			// Add code here
			break;
		case LVN_DELETEITEM:  // An items in about to be deleted, track via NMLISTVIEW
			//pNMListView = (LPNMLISTVIEW)p_pNotifyStruct;
			// Add code here
			OnDeleteItem(e);
			break;
		case LVN_ENDLABELEDIT:  // Item label in-place edit ended, track via NMLVDISPINFO
			//pNMLVDispInfo = (NMLVDISPINFO*)p_pNotifyStruct;
			// Add code here
			OnEndLabelEdit(e);
			break;
		case LVN_GETDISPINFO:  // Display information request, track via NMLVDISPINFO
			//pNMLVDispInfo = (NMLVDISPINFO*)p_pNotifyStruct;
			//// Add code here
			//if(pNMLVDispInfo->item.mask&LVIF_TEXT)
			//	strcpy(pNMLVDispInfo->item.pszText, _T("Callback"));
			//if(pNMLVDispInfo->item.mask&LVIF_IMAGE)
			//	pNMLVDispInfo->item.iImage = 9;
			//if(pNMLVDispInfo->item.mask&LVIF_STATE)
			//	pNMLVDispInfo->item.state = 0;
			OnGetDispinfo(e);
			break;
		case LVN_GETINFOTIP:  // Tooltip text request, track via NMLVGETINFOTIP
			//pNMLVGetInfoTip = (LPNMLVGETINFOTIP)p_pNotifyStruct;
			// Add code here
			break;
		case LVN_HOTTRACK:  // User moved mouse over an item, track via NMLISTVIEW
			//pNMListView = (LPNMLISTVIEW)p_pNotifyStruct;
			// Add code here
			break;
		case LVN_INSERTITEM:  // An item as insert, track via NMLISTVIEW
			//pNMListView = (LPNMLISTVIEW)p_pNotifyStruct;
			// Add code here
			break;
		case LVN_ITEMACTIVATE:  // User activated an item, track via NMITEMACTIVATE
			//pNMItemActivate = (LPNMITEMACTIVATE)p_pNotifyStruct;
			// Add code here
			break;
		case LVN_ITEMCHANGED:  // An item as changed, track via NMLISTVIEW
			//pNMListView = (LPNMLISTVIEW)p_pNotifyStruct;
			// Add code here
			OnSelectedIndexChanged(e);
			break;
		case LVN_ITEMCHANGING:  // An item is changing, track via NMLISTVIEW
			//pNMListView = (LPNMLISTVIEW)p_pNotifyStruct;
			// Add code here
			OnSelectedIndexChanging(e);
			break;
		case LVN_KEYDOWN:  // A key has been pressed, track via NNLVKEYDOWN
			//pNMLVKeyDown = (LPNMLVKEYDOWN)p_pNotifyStruct;
			// Add code here
			break;
		case LVN_MARQUEEBEGIN:  // A bounding box selection has begun, track via NMLISTVIEW
			//pNMListView = (LPNMLISTVIEW)p_pNotifyStruct;
			// Add code here
			break;
		case LVN_ODCACHEHINT:  // Virtual list view notify that display contents changed, track via NMLVCACHEHINT
			//pNMLVCacheHint = (NMLVCACHEHINT*)p_pNotifyStruct;
			// Add code here
			break;
		case LVN_ODFINDITEM:  // Virtual list view callback item location query, track via NMLVFINDITEM
			//pNMLVFindItem = (NMLVFINDITEM*)p_pNotifyStruct;
			// Add code here
			break;
		case LVN_ODSTATECHANGED:  // Virtual list view notify that an item or range of items changed, track via NMLVODSTATECHANGE
			//pNMLVODStateChange = (LPNMLVODSTATECHANGE)p_pNotifyStruct;
			// Add code here
			break;
		case LVN_SETDISPINFO:  // Display information update request, track via NMLVDISPINFO
			//pNMLVDispInfo = (NMLVDISPINFO*)p_pNotifyStruct;
			// Add code here
			OnSetDispinfo(e);
			break;
		// Common notifications
		case NM_CLICK:  // User clicked left mouse button, track via NMLISTVIEW
			//pNMListView = (LPNMLISTVIEW)p_pNotifyStruct;
			// Add code here
			OnClick(e);
			//OnItemClick(e);
			break;
		case NM_CUSTOMDRAW:  // Drawing operation notification, track via NMLVCUSTOMDRAW
			//pNMLVCustomDraw = (LPNMLVCUSTOMDRAW)p_pNotifyStruct;
			// Add code here
			break;
		case NM_DBLCLK:  // User double clicked left mouse button, track via NMLISTVIEW
			//pNMListView = (LPNMLISTVIEW)p_pNotifyStruct;
			// Add code here
			//OnDoubleClick(e);
			OnItemDoubleClick(e);
			break;
		case NM_HOVER:  // Mouse is hovering over an item, track via NMHDR
			// Use p_pNotifyStruct
			// Add code here
			break;
		case NM_KILLFOCUS:  // Control has lost input focus, track via NMHDR
			// Use p_pNotifyStruct
			// Add code here
			OnLeave(e);
			OnLostFocus(e);
			break;
		case NM_RCLICK:  // User clicked right mouse button, track via NMLISTVIEW
			//pNMListView = (LPNMLISTVIEW)p_pNotifyStruct;
			// Add code here
			OnRClick(e);
			break;
		case NM_RDBLCLK:  // User double clicked right mouse button, track via NMLISTVIEW
			//pNMListView = (LPNMLISTVIEW)p_pNotifyStruct;
			// Add code here
			break;
		case NM_RELEASEDCAPTURE:  // Control is releasing mouse capture, track via NMHDR
			// Use p_pNotifyStruct
			// Add code here
			break;
		case NM_RETURN:  // User pressed ENTER, track via NMHDR
			// Use p_pNotifyStruct
			// Add code here
			break;
		case NM_SETFOCUS:  // Control received input focus, track via NMHDR
			// Use p_pNotifyStruct
			// Add code here
			OnEnter(e);
			OnGotFocus(e);
			break;
	}

	Control::OnChildNotify(e);
}

void ListView::OnDragBegin(DragBeginEventArgs* e)
{
	DragBegin.Invoke((Object*)this, e);
}

void ListView::OnDragMove(MouseEventArgs* e)
{
	DragMove.Invoke((Object*)this, e);
}

void ListView::OnDragEnd(DragEndEventArgs* e)
{
	SetStyle(StyleType::Inner, InnerStyle::Dragging, 0);
	DragEnd.Invoke((Object*)this, e);
}

void ListView::OnRClick(EventArgs* e)
{
	RClick.Invoke((Object*)this, e);
}

BOOL ListView::GetItemRect(int nItem, LPRECT lpRect, UINT nCode) const
{
	CC_ASSERT(::IsWindow(m_hWnd));
	lpRect->left = nCode;
	return (BOOL) ::SendMessage(m_hWnd, LVM_GETITEMRECT, (WPARAM)nItem, (LPARAM)lpRect);
}

BOOL ListView::SetItemCountEx(int iCount, DWORD dwFlags /* = LVSICF_NOINVALIDATEALL */)
{
	CC_ASSERT(::IsWindow(m_hWnd));

	// can't have dwFlags on a control that isn't virutal
	CC_ASSERT(dwFlags == 0 || HasStyle(StyleType::Style, LVS_OWNERDATA));

	return (BOOL) ::SendMessage(m_hWnd, LVM_SETITEMCOUNT, (WPARAM) iCount, (LPARAM) dwFlags);
}

SIZE ListView::SetIconSpacing(int cx, int cy)
{
	CC_ASSERT(::IsWindow(m_hWnd));
	DWORD dwRet = (DWORD) ::SendMessage(m_hWnd, LVM_SETICONSPACING, 0, (LPARAM) MAKELONG(cx, cy));

	SIZE size = {LOWORD(dwRet), HIWORD(dwRet)};
	return size;
}

SIZE ListView::SetIconSpacing(SIZE size)
{
	CC_ASSERT(::IsWindow(m_hWnd));
	DWORD dwRet = (DWORD) ::SendMessage(m_hWnd, LVM_SETICONSPACING, 0, (LPARAM) MAKELONG(size.cx, size.cy));

	SIZE size2 = {LOWORD(dwRet), HIWORD(dwRet)};
	return size2;
}

BOOL ListView::GetSubItemRect(int iItem, int iSubItem, int nArea, RECT& ref)
{
	CC_ASSERT(::IsWindow(m_hWnd));
	CC_ASSERT(nArea == LVIR_BOUNDS || nArea == LVIR_ICON || nArea == LVIR_LABEL);

	RECT rect;
	rect.top = iSubItem;
	rect.left = nArea;
	BOOL bRet = (BOOL) ::SendMessage(m_hWnd, LVM_GETSUBITEMRECT, iItem, (LPARAM) &rect);

	if (bRet)
	{
		::CopyRect(&ref, &rect);
	}
	return bRet;
}

int ListView::InsertColumn(int nCol, LPCTSTR lpszColumnHeading, int nFormat, int nWidth, int nSubItem)
{
	LVCOLUMN column;
	column.mask = LVCF_TEXT|LVCF_FMT;
	column.pszText = (LPTSTR)lpszColumnHeading;
	column.fmt = nFormat;
	if (nWidth != -1)
	{
		column.mask |= LVCF_WIDTH;
		column.cx = nWidth;
	}
	if (nSubItem != -1)
	{
		column.mask |= LVCF_SUBITEM;
		column.iSubItem = nSubItem;
	}
	return ListView::InsertColumn(nCol, &column);
}

int ListView::ItemAdd(UINT nMask, int nItem, LPCTSTR lpszItem, UINT nState, UINT nStateMask, int nImage, LPARAM lParam)
{
	CC_ASSERT(::IsWindow(m_hWnd));
	LVITEM item;
	item.mask = nMask;
	item.iItem = nItem;
	item.iSubItem = 0;
	item.pszText = (LPTSTR)lpszItem;
	item.state = nState;
	item.stateMask = nStateMask;
	item.iImage = nImage;
	item.lParam = lParam;
	return ListView::ItemAdd(&item);
}

int ListView::HitTest(POINT pt, UINT* pFlags) const
{
	CC_ASSERT(::IsWindow(m_hWnd));
	LVHITTESTINFO hti;
	hti.pt = pt;
	int nRes = (int) ::SendMessage(m_hWnd, LVM_HITTEST, 0, (LPARAM)&hti);
	if (pFlags != NULL)
	{
		*pFlags = hti.flags;
	}
	return nRes;
}

BOOL ListView::SetItem(int nItem, int nSubItem, UINT nMask, LPCTSTR lpszItem,
	int nImage, UINT nState, UINT nStateMask, LPARAM lParam)
{
	CC_ASSERT((nMask & LVIF_INDENT) == 0);
	return SetItem(nItem, nSubItem, nMask, lpszItem, nImage, nState, nStateMask, lParam, 0);
}

BOOL ListView::SetItem(int nItem, int nSubItem, UINT nMask, LPCTSTR lpszItem,
	int nImage, UINT nState, UINT nStateMask, LPARAM lParam, int nIndent)
{
	CC_ASSERT(::IsWindow(m_hWnd));
	CC_ASSERT(HasStyle(StyleType::Style, LVS_OWNERDATA));

	LVITEM lvi;
	lvi.mask = nMask;
	lvi.iItem = nItem;
	lvi.iSubItem = nSubItem;
	lvi.stateMask = nStateMask;
	lvi.state = nState;
	lvi.pszText = (LPTSTR) lpszItem;
	lvi.iImage = nImage;
	lvi.lParam = lParam;
	lvi.iIndent = nIndent;

	return (BOOL) ::SendMessage(m_hWnd, LVM_SETITEM, 0, (LPARAM)&lvi);
}

BOOL ListView::SetItemState(int nItem, UINT nState, UINT nStateMask)
{
	CC_ASSERT(::IsWindow(m_hWnd));
	LVITEM lvi;
	lvi.stateMask = nStateMask;
	lvi.state = nState;
	return (BOOL) ::SendMessage(m_hWnd, LVM_SETITEMSTATE, nItem, (LPARAM)&lvi);
}

BOOL ListView::SetItemText(int nItem, int nSubItem, LPCTSTR lpszText)
{
	CC_ASSERT(::IsWindow(m_hWnd));
	//CC_ASSERT(HasStyle(StyleType::Style, LVS_OWNERDATA));
	LVITEM lvi;
	lvi.iSubItem = nSubItem;
	lvi.pszText = (LPTSTR) lpszText;
	return (BOOL) ::SendMessage(m_hWnd, LVM_SETITEMTEXT, nItem, (LPARAM)&lvi);
}

cc::Str ListView::GetItemText(int nItem, int nSubItem) const
{
	CC_ASSERT(::IsWindow(m_hWnd));
	LVITEM lvi;
	memset(&lvi, 0, sizeof(LVITEM));
	lvi.iSubItem = nSubItem;
	cc::Str str;
	int nLen = 128;
	int nRes;
	do
	{
		nLen *= 2;
		lvi.cchTextMax = nLen;
		lvi.pszText = (TCHAR*)str.GetBufferSetLength(nLen, false);
		nRes  = (int)::SendMessage(m_hWnd, LVM_GETITEMTEXT, (WPARAM)nItem, (LPARAM)&lvi);
	} while (nRes == nLen-1);
	str.UpdateStringLength();
	return str;
}

int ListView::GetItemText(int nItem, int nSubItem, LPTSTR lpszText, int nLen) const
{
	CC_ASSERT(::IsWindow(m_hWnd));
	LVITEM lvi;
	memset(&lvi, 0, sizeof(LVITEM));
	lvi.iSubItem = nSubItem;
	lvi.cchTextMax = nLen;
	lvi.pszText = lpszText;
	return (int)::SendMessage(m_hWnd, LVM_GETITEMTEXT, (WPARAM)nItem, (LPARAM)&lvi);
}

DWORD_PTR ListView::GetItemData(int nItem) const
{
	CC_ASSERT(::IsWindow(m_hWnd));
	LVITEM lvi;
	memset(&lvi, 0, sizeof(LVITEM));
	lvi.iItem = nItem;
	lvi.mask = LVIF_PARAM;
	CC_VERIFY(::SendMessage(m_hWnd, LVM_GETITEM, 0, (LPARAM)&lvi));
	return lvi.lParam;
}

//void ListView::OnDrawItem(DrawItemEventArgs* e)
//{
//	CC_ASSERT(FALSE);
//}
//
void ListView::RemoveImageList(int nImageList)
{
	HIMAGELIST h = (HIMAGELIST)SendMessage(m_hWnd, LVM_GETIMAGELIST, (WPARAM)nImageList, NULL);
	//if (CImageList::FromHandlePermanent(h) != NULL)
	if(h != NULL)
	{
		ImageList_Destroy(h);
		SendMessage(m_hWnd, LVM_SETIMAGELIST, (WPARAM)nImageList, NULL);
	}
}

void ListView::OnNcDestroy()
{
	RemoveImageList(LVSIL_NORMAL);
	RemoveImageList(LVSIL_SMALL);
	RemoveImageList(LVSIL_STATE);

	Control::OnNcDestroy();
}

ImageList* ListView::CreateDragImage(int nItem, LPPOINT lpPoint)
{
	CC_ASSERT(::IsWindow(m_hWnd));

	HIMAGELIST hImageList = (HIMAGELIST)::SendMessage(m_hWnd, LVM_CREATEDRAGIMAGE, nItem, (LPARAM)lpPoint);
	if(hImageList == NULL)
	{
		return NULL;
	}

	ImageList* pImageList = ImageList::FromHandle(hImageList);
	CC_VERIFY(pImageList);
	return pImageList;
}


/////////////////////////////////////////////////////////////////////////////
// ListView
BOOL ListView::SetColumnOrderArray(int iCount, LPINT piArray)
{
	CC_ASSERT(::IsWindow(m_hWnd));
	//CC_ASSERT(AfxIsValidAddress(piArray, iCount * sizeof(int), FALSE));

	return (BOOL) ::SendMessage(m_hWnd, LVM_SETCOLUMNORDERARRAY, (WPARAM) iCount, (LPARAM) piArray);
}

int ListView::GetColumnCount()
{
	CC_ASSERT(::IsWindow(m_hWnd));

	//CHeaderCtrl* pCtrl = GetHeaderCtrl();
	HWND hWnd = (HWND)::SendMessage(m_hWnd, LVM_GETHEADER, 0, 0);
	CC_ASSERT(hWnd != NULL);
	return (int)::SendMessage(hWnd, HDM_GETITEMCOUNT, 0, 0L);
}

BOOL ListView::GetColumnOrderArray(LPINT piArray, int iCount /* = -1 */)
{
	CC_ASSERT(::IsWindow(m_hWnd));

	// if -1 was passed, find the count ourselves
	int nCount = iCount;
	if(nCount == -1)
	{
		//CHeaderCtrl* pCtrl = GetHeaderCtrl();
		HWND hWnd = (HWND)::SendMessage(m_hWnd, LVM_GETHEADER, 0, 0);
		CC_ASSERT(hWnd != NULL);
		if(hWnd != NULL)
		{
			//nCount = pCtrl->GetItemCount();
			nCount = (int)::SendMessage(hWnd, HDM_GETITEMCOUNT, 0, 0L);
		}
	}
	if(nCount == -1)
	{
		return FALSE;
	}

	//CC_ASSERT(AfxIsValidAddress(piArray, nCount * sizeof(int)));
	return (BOOL)::SendMessage(m_hWnd, LVM_GETCOLUMNORDERARRAY, (WPARAM)nCount, (LPARAM)piArray);
}

BOOL ListView::SetBkImage(HBITMAP hbm, BOOL fTile /*= TRUE*/, int xOffsetPercent /*= 0*/, int yOffsetPercent /*= 0*/)
{
	LVBKIMAGE lv;

	lv.ulFlags = LVBKIF_SOURCE_HBITMAP;
	if( fTile )
	{
		lv.ulFlags |= LVBKIF_STYLE_TILE;
	}
	else
	{
		lv.ulFlags |= LVBKIF_STYLE_NORMAL;
	}
	lv.hbm = hbm;
	lv.xOffsetPercent = xOffsetPercent;
	lv.yOffsetPercent = yOffsetPercent;
	return SetBkImage(&lv);
}

BOOL ListView::SetBkImage(LPTSTR pszUrl, BOOL fTile /*= TRUE*/, int xOffsetPercent /*= 0*/, int yOffsetPercent /*= 0*/)
{
	LVBKIMAGE lv;

	lv.ulFlags = LVBKIF_SOURCE_URL;
	if( fTile )
	{
		lv.ulFlags |= LVBKIF_STYLE_TILE;
	}
	else
	{
		lv.ulFlags |= LVBKIF_STYLE_NORMAL;
	}
	lv.pszImage = pszUrl;
	lv.xOffsetPercent = xOffsetPercent;
	lv.yOffsetPercent = yOffsetPercent;
	return SetBkImage(&lv);
}

BOOL ListView::GetCheck(int nItem) const
{
	CC_ASSERT(::IsWindow(m_hWnd));
	int nState = (int)::SendMessage(m_hWnd, LVM_GETITEMSTATE, (WPARAM)nItem, (LPARAM)LVIS_STATEIMAGEMASK);
	// Return zero if it's not checked, or nonzero otherwise.
	return ((BOOL)(nState >> 12) -1);
}

BOOL ListView::SetCheck(int nItem, BOOL fCheck)
{
	CC_ASSERT(::IsWindow(m_hWnd));
	LVITEM lvi;
	lvi.stateMask = LVIS_STATEIMAGEMASK;

	//Since state images are one-based, 1 in this macro turns the check off, and 2 turns it on.
	lvi.state = INDEXTOSTATEIMAGEMASK((fCheck ? 2 : 1));
	return (BOOL) ::SendMessage(m_hWnd, LVM_SETITEMSTATE, nItem, (LPARAM)&lvi);
}

/*
CHeaderCtrl* ListView::GetHeaderCtrl()
{
	CC_ASSERT(::IsWindow(m_hWnd));

	HWND hWnd = (HWND) ::SendMessage(m_hWnd, LVM_GETHEADER, 0, 0);
	if (hWnd == NULL)
	{
		return NULL;
	}
	else
	{
		return (CHeaderCtrl*) CHeaderCtrl::FromHandle(hWnd);
	}
}
*/


//CToolTipCtrl* ListView::GetToolTips() const
//	{ CC_ASSERT(::IsWindow(m_hWnd)); return (CToolTipCtrl*)CWnd::FromHandle((HWND)::SendMessage(m_hWnd, LVM_GETTOOLTIPS, 0, 0L)); }
//CToolTipCtrl* ListView::SetToolTips(CToolTipCtrl* pTip)
//	{ CC_ASSERT(::IsWindow(m_hWnd)); return (CToolTipCtrl*)CWnd::FromHandle((HWND)::SendMessage(m_hWnd, LVM_SETTOOLTIPS, 0, (LPARAM) pTip->GetSafeHwnd())); }
COLORREF ListView::GetBackground() const
{
	//CC_ASSERT(::IsWindow(m_hWnd));
	if(m_hWnd != NULL)
	{
		return (COLORREF)::SendMessage(m_hWnd, LVM_GETBKCOLOR, 0, 0L);
	}
	return _corBackground;
}
void ListView::SetBackground(COLORREF corBackground)
{
	::SendMessage(m_hWnd, LVM_SETBKCOLOR, 0, (LPARAM)corBackground);
	Control::SetBackground(corBackground);
}
//BOOL ListView::SetBkColor(COLORREF cr)
//	{ CC_ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, LVM_SETBKCOLOR, 0, cr); }
//ImageList* ListView::GetImageList(int nImageListType) const
//	{ CC_ASSERT(::IsWindow(m_hWnd)); return ImageList::FromHandle((HIMAGELIST) ::SendMessage(m_hWnd, LVM_GETIMAGELIST, nImageListType, 0L)); }
//ImageList* ListView::SetImageList(ImageList* pImageList, int nImageList)
//	{ CC_ASSERT(::IsWindow(m_hWnd)); return ImageList::FromHandle((HIMAGELIST) ::SendMessage(m_hWnd, LVM_SETIMAGELIST, nImageList, (LPARAM)pImageList->GetSafeHandle())); }
HIMAGELIST ListView::GetImageList(int nImageListType) const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (HIMAGELIST) ::SendMessage(m_hWnd, LVM_GETIMAGELIST, nImageListType, 0L); }
HIMAGELIST ListView::SetImageList(HIMAGELIST hImageList, int nImageList)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (HIMAGELIST) ::SendMessage(m_hWnd, LVM_SETIMAGELIST, nImageList, (LPARAM)hImageList); }
int ListView::GetItemCount() const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (int) ::SendMessage(m_hWnd, LVM_GETITEMCOUNT, 0, 0L); }
BOOL ListView::GetItem(LVITEM* pItem) const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, LVM_GETITEM, 0, (LPARAM)pItem); }
BOOL ListView::SetItem(const LVITEM* pItem)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, LVM_SETITEM, 0, (LPARAM)pItem); }
BOOL ListView::SetItemData(int nItem, DWORD_PTR dwData)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return SetItem(nItem, 0, LVIF_PARAM, NULL, 0, 0, 0, (LPARAM)dwData); }
int ListView::ItemAdd(const LVITEM* pItem)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (int) ::SendMessage(m_hWnd, LVM_INSERTITEM, 0, (LPARAM)pItem); }
int ListView::ItemAdd(int nItem, LPCTSTR lpszItem)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return ItemAdd(LVIF_TEXT, nItem, lpszItem, 0, 0, 0, 0); }
int ListView::ItemAdd(int nItem, LPCTSTR lpszItem, int nImage)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return ItemAdd(LVIF_TEXT|LVIF_IMAGE, nItem, lpszItem, 0, 0, nImage, 0); }
BOOL ListView::ItemDel(int nItem)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, LVM_DELETEITEM, nItem, 0L); }
BOOL ListView::ItemClear()
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, LVM_DELETEALLITEMS, 0, 0L); }
UINT ListView::GetCallbackMask() const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (UINT) ::SendMessage(m_hWnd, LVM_GETCALLBACKMASK, 0, 0); }
BOOL ListView::SetCallbackMask(UINT nMask)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, LVM_SETCALLBACKMASK, nMask, 0); }
/*
flag for LVM_GETNEXTITEM of GetNextItem:
Specifies the relationship to the item specified in iStart. This can be one or a combination of the following values:

Searches by index.
LVNI_ALL
    Searches for a subsequent item by index, the default value.
LVNI_PREVIOUS
    Microsoft Windows Vista and later: Searches for an item that is ordered before the item specified in plvii. The LVNI_PREVIOUS flag is not directional (LVNI_ABOVE will find the item positioned above, while LVNI_PREVIOUS will find the item ordered before.) The LVNI_PREVIOUS flag basically reverses the logic of the search performed by the LVM_GETNEXTITEM or LVM_GETNEXTITEMINDEX messages.

Searches by physical relationship to the index of the item where the search is to begin.
LVNI_ABOVE
    Searches for an item that is above the specified item.
LVNI_BELOW
    Searches for an item that is below the specified item.
LVNI_TOLEFT
    Searches for an item to the left of the specified item.
LVNI_TORIGHT
    Searches for an item to the right of the specified item.
LVNI_DIRECTIONMASK
    Microsoft Windows Vista and later: A directional flag mask with value as follows: LVNI_ABOVE | LVNI_BELOW | LVNI_TOLEFT | LVNI_TORIGHT.

The state of the item to find can be specified with one or a combination of the following values:
LVNI_CUT
    The item has the LVIS_CUT state flag set.
LVNI_DROPHILITED
    The item has the LVIS_DROPHILITED state flag set
LVNI_FOCUSED
    The item has the LVIS_FOCUSED state flag set.
LVNI_SELECTED
    The item has the LVIS_SELECTED state flag set.
LVNI_STATEMASK
    Microsoft Windows Vista and later: A state flag mask with value as follows: LVNI_FOCUSED | LVNI_SELECTED | LVNI_CUT | LVNI_DROPHILITED.

Searches by appearance of items or by group
LVNI_VISIBLEORDER
    Microsoft Windows Vista and later: Search the visible order.
LVNI_VISIBLEONLY
    Microsoft Windows Vista and later: Search the visible items.
LVNI_SAMEGROUPONLY
    Microsoft Windows Vista and later: Search the current group.

If an item does not have all of the specified state flags set, the search continues with the next item.
*/
int ListView::GetNextItem(int nItem, int nFlags) const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (int) ::SendMessage(m_hWnd, LVM_GETNEXTITEM, nItem, MAKELPARAM(nFlags, 0)); }
//POSITION
//POINT ListView::GetFirstSelectedItemPosition() const
//	{ CC_ASSERT(::IsWindow(m_hWnd)); return (POINT)(DWORD_PTR)(1+GetNextItem(-1, LVIS_SELECTED)); }
//int ListView::GetNextSelectedItem(POINT& pos) const
//	{ CC_ASSERT(::IsWindow(m_hWnd)); DWORD_PTR nOldPos = (DWORD_PTR)pos-1; pos = (POINT)(DWORD_PTR)(1+GetNextItem((UINT)nOldPos, LVIS_SELECTED)); return (UINT)nOldPos; }
int ListView::FindItem(LVFINDINFO* pFindInfo, int nStart) const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (int) ::SendMessage(m_hWnd, LVM_FINDITEM, nStart, (LPARAM)pFindInfo); }
int ListView::HitTest(LVHITTESTINFO* pHitTestInfo) const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (int) ::SendMessage(m_hWnd, LVM_HITTEST, 0, (LPARAM)pHitTestInfo); }
BOOL ListView::SetItemPosition(int nItem, POINT pt)
	{ CC_ASSERT(::IsWindow(m_hWnd)); CC_ASSERT(HasStyle(StyleType::Style, LVS_OWNERDATA)); return (BOOL) ::SendMessage(m_hWnd, LVM_SETITEMPOSITION32, nItem, (LPARAM)&pt); }
BOOL ListView::GetItemPosition(int nItem, LPPOINT lpPoint) const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, LVM_GETITEMPOSITION, nItem, (LPARAM)lpPoint); }
int ListView::GetStringWidth(LPCTSTR lpsz) const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (int) ::SendMessage(m_hWnd, LVM_GETSTRINGWIDTH, 0, (LPARAM)lpsz); }
BOOL ListView::EnsureVisible(int nItem, BOOL bPartialOK)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, LVM_ENSUREVISIBLE, nItem, MAKELPARAM(bPartialOK, 0)); }
BOOL ListView::DoScroll(SIZE size)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, LVM_SCROLL, size.cx, size.cy); }
BOOL ListView::RedrawItems(int nFirst, int nLast)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, LVM_REDRAWITEMS, nFirst, nLast); }
BOOL ListView::Arrange(UINT nCode)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, LVM_ARRANGE, nCode, 0L); }
//EditBox* ListView::EditLabel(int nItem)
//	{ CC_ASSERT(::IsWindow(m_hWnd)); return (CEdit*)CWnd::FromHandle( (HWND)::SendMessage(m_hWnd, LVM_EDITLABEL, nItem, 0L)); }
//EditBox* ListView::GetEditControl() const
//	{ CC_ASSERT(::IsWindow(m_hWnd)); return (CEdit*)CWnd::FromHandle( (HWND)::SendMessage(m_hWnd, LVM_GETEDITCONTROL, 0, 0L)); }
BOOL ListView::GetColumn(int nCol, LVCOLUMN* pColumn) const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, LVM_GETCOLUMN, nCol, (LPARAM)pColumn); }
BOOL ListView::SetColumn(int nCol, const LVCOLUMN* pColumn)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, LVM_SETCOLUMN, nCol, (LPARAM)pColumn); }
int ListView::InsertColumn(int nCol, const LVCOLUMN* pColumn)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (int) ::SendMessage(m_hWnd, LVM_INSERTCOLUMN, nCol, (LPARAM)pColumn); }
BOOL ListView::DeleteColumn(int nCol)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, LVM_DELETECOLUMN, nCol, 0); }
int ListView::GetColumnWidth(int nCol) const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (int) ::SendMessage(m_hWnd, LVM_GETCOLUMNWIDTH, nCol, 0); }
BOOL ListView::SetColumnWidth(int nCol, int cx)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, LVM_SETCOLUMNWIDTH, nCol, MAKELPARAM(cx, 0)); }
BOOL ListView::GetViewRect(LPRECT lpRect) const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, LVM_GETVIEWRECT, 0, (LPARAM)lpRect); }
COLORREF ListView::GetTextColor() const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (COLORREF) ::SendMessage(m_hWnd, LVM_GETTEXTCOLOR, 0, 0L); }
BOOL ListView::SetTextColor(COLORREF cr)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, LVM_SETTEXTCOLOR, 0, cr); }
COLORREF ListView::GetTextBkColor() const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (COLORREF) ::SendMessage(m_hWnd, LVM_GETTEXTBKCOLOR, 0, 0L); }
BOOL ListView::SetTextBkColor(COLORREF cr)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, LVM_SETTEXTBKCOLOR, 0, cr); }
int ListView::GetTopIndex() const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (int) ::SendMessage(m_hWnd, LVM_GETTOPINDEX, 0, 0); }
int ListView::GetCountPerPage() const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (int) ::SendMessage(m_hWnd, LVM_GETCOUNTPERPAGE, 0, 0); }
BOOL ListView::GetOrigin(LPPOINT lpPoint) const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, LVM_GETORIGIN, 0, (LPARAM)lpPoint); }
BOOL ListView::Update(int nItem)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, LVM_UPDATE, nItem, 0L); }
BOOL ListView::SetItemState(int nItem, LVITEM* pItem)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, LVM_SETITEMSTATE, nItem, (LPARAM)pItem); }
UINT ListView::GetItemState(int nItem, UINT nMask) const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (UINT) ::SendMessage(m_hWnd, LVM_GETITEMSTATE, nItem, nMask); }
void ListView::SetItemCount(int nItems)
	{ CC_ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, LVM_SETITEMCOUNT, nItems, 0); }
BOOL ListView::SortItems(PFNLVCOMPARE pfnCompare, DWORD_PTR dwData)
	{ CC_ASSERT(::IsWindow(m_hWnd)); CC_ASSERT(HasStyle(StyleType::Style, LVS_OWNERDATA)); return (BOOL) ::SendMessage(m_hWnd, LVM_SORTITEMS, dwData, (LPARAM)pfnCompare); }
UINT ListView::GetSelectedCount() const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (UINT) ::SendMessage(m_hWnd, LVM_GETSELECTEDCOUNT, 0, 0L); }

DWORD ListView::SetExtendedStyle(DWORD dwNewStyle)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (DWORD) ::SendMessage(m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, (LPARAM) dwNewStyle); }
HCURSOR ListView::SetHotCursor(HCURSOR hc)
	{ CC_ASSERT(::IsWindow(m_hWnd) && hc != NULL); return (HCURSOR) ::SendMessage(m_hWnd, LVM_SETHOTCURSOR, 0, (LPARAM) hc); }
int ListView::SetHotItem(int iIndex)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (int) ::SendMessage(m_hWnd, LVM_SETHOTITEM, (WPARAM) iIndex, 0); }
void ListView::SetWorkAreas(int nWorkAreas, LPRECT lpRect)
	{ CC_ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, LVM_SETWORKAREAS, nWorkAreas, (LPARAM) lpRect); }
int ListView::SubItemHitTest(LPLVHITTESTINFO pInfo)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (int) ::SendMessage(m_hWnd, LVM_SUBITEMHITTEST, 0, (LPARAM) pInfo); }
HCURSOR ListView::GetHotCursor()
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (HCURSOR) ::SendMessage(m_hWnd, LVM_GETHOTCURSOR, 0, 0); }
int ListView::GetHotItem()
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (int) ::SendMessage(m_hWnd, LVM_GETHOTITEM, 0, 0); }
DWORD ListView::GetExtendedStyle()
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (DWORD) ::SendMessage(m_hWnd, LVM_GETEXTENDEDLISTVIEWSTYLE, 0, 0); }
SIZE ListView::ApproximateViewRect(SIZE sz, int iCount) const
	{ CC_ASSERT(::IsWindow(m_hWnd)); DWORD dwRet = (DWORD) ::SendMessage(m_hWnd, LVM_APPROXIMATEVIEWRECT, iCount, MAKELPARAM(sz.cx, sz.cy));
	SIZE size = {LOWORD(dwRet), HIWORD(dwRet)}; return size;}
BOOL ListView::GetBkImage(LVBKIMAGE* plvbkImage) const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, LVM_GETBKIMAGE, 0, (LPARAM)plvbkImage); }
DWORD ListView::GetHoverTime() const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (DWORD) ::SendMessage(m_hWnd, LVM_GETHOVERTIME, 0, 0); }
void ListView::GetWorkAreas(int nWorkAreas, LPRECT prc) const
	{ CC_ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, LVM_GETWORKAREAS, nWorkAreas, (LPARAM)prc); }
BOOL ListView::SetBkImage(LVBKIMAGE* plvbkImage)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (BOOL)::SendMessage(m_hWnd, LVM_SETBKIMAGE, 0, (LPARAM)plvbkImage); }
DWORD ListView::SetHoverTime(DWORD dwHoverTime)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (DWORD)::SendMessage(m_hWnd, LVM_SETHOVERTIME, 0, dwHoverTime); }
UINT ListView::GetNumberOfWorkAreas() const
	{ CC_ASSERT(::IsWindow(m_hWnd)); UINT nWorkAreas; ::SendMessage(m_hWnd, LVM_GETNUMBEROFWORKAREAS, 0, (LPARAM)&nWorkAreas); return nWorkAreas; }
int ListView::SetSelectionMark(int iIndex)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (int) ::SendMessage(m_hWnd, LVM_SETSELECTIONMARK, 0, (LPARAM) iIndex); }
int ListView::GetSelectionMark()
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (int) ::SendMessage(m_hWnd, LVM_GETSELECTIONMARK, 0, 0); }

///////////////////////////////////////////////////////////////////////////////

/*
TVS_CHECKBOXES 
    Version 4.70. Enables check boxes for items in a tree-view control. A check box is displayed only if an image is associated with the item. When set to this style, the control effectively uses DrawFrameControl to create and set a state image list containing two images. State image 1 is the unchecked box and state image 2 is the checked box. Setting the state image to zero removes the check box altogether. For more information, see Working with state image indexes.
    Version 5.80. Displays a check box even if no image is associated with the item.
    Once a tree-view control is created with this style, the style cannot be removed. Instead, you must destroy the control and create a new one in its place. Destroying the tree-view control does not destroy the check box state image list. You must destroy it explicitly. Get the handle to the state image list by sending the tree-view control a TVM_GETIMAGELIST message. Then destroy the image list with ImageList_Destroy.
    If you want to use this style, you must set the TVS_CHECKBOXES style with SetWindowLong after you create the treeview control, and before you populate the tree. Otherwise, the checkboxes might appear unchecked, depending on timing issues.
TVS_DISABLEDRAGDROP 
    Prevents the tree-view control from sending TVN_BEGINDRAG notification messages. 
TVS_EDITLABELS 
    Allows the user to edit the labels of tree-view items. 
TVS_FULLROWSELECT 
    Version 4.71. Enables full-row selection in the tree view. The entire row of the selected item is highlighted, and clicking anywhere on an item's row causes it to be selected. This style cannot be used in conjunction with the TVS_HASLINES style. 
TVS_HASBUTTONS 
    Displays plus (+) and minus (-) buttons next to parent items. The user clicks the buttons to expand or collapse a parent item's list of child items. To include buttons with items at the root of the tree view, TVS_LINESATROOT must also be specified. 
TVS_HASLINES 
    Uses lines to show the hierarchy of items. 
TVS_INFOTIP 
    Version 4.71. Obtains ToolTip information by sending the TVN_GETINFOTIP notification. 
TVS_LINESATROOT 
    Uses lines to link items at the root of the tree-view control. This value is ignored if TVS_HASLINES is not also specified. 
TVS_NOHSCROLL 
    Version 5.80. Disables horizontal scrolling in the control. The control will not display any horizontal scroll bars. 
TVS_NONEVENHEIGHT 
    Version 4.71 Sets the height of the items to an odd height with the TVM_SETITEMHEIGHT message. By default, the height of items must be an even value. 
TVS_NOSCROLL 
    Version 4.71. Disables both horizontal and vertical scrolling in the control. The control will not display any scroll bars. 
TVS_NOTOOLTIPS 
    Version 4.70. Disables ToolTips. 
TVS_RTLREADING 
    Version 4.70. Causes text to be displayed from right-to-left (RTL). Usually, windows display text left-to-right (LTR). Windows can be mirrored to display languages such as Hebrew or Arabic that read RTL. Typically, tree-view text is displayed in the same direction as the text in its parent window. If TVS_RTLREADING is set, tree-view text reads in the opposite direction from the text in the parent window. 
TVS_SHOWSELALWAYS 
    Causes a selected item to remain selected when the tree-view control loses focus. 
TVS_SINGLEEXPAND 
    Version 4.71. Causes the item being selected to expand and the item being unselected to collapse upon selection in the tree view. If the mouse is used to single-click the selected item and that item is closed, it will be expanded. If the user holds down the CTRL key while selecting an item, the item being unselected will not be collapsed.
    Version 5.80. Causes the item being selected to expand and the item being unselected to collapse upon selection in the tree view. If the user holds down the CTRL key while selecting an item, the item being unselected will not be collapsed.
TVS_TRACKSELECT 
    Version 4.70. Enables hot tracking in a tree-view control.
*/
DWORD TreeViewConfig::nStyle = WS_CHILD|WS_VISIBLE|WS_TABSTOP|TVS_HASBUTTONS|TVS_HASLINES|TVS_LINESATROOT|TVS_EDITLABELS|TVS_SHOWSELALWAYS;
DWORD TreeViewConfig::nExStyle = WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR | WS_EX_NOPARENTNOTIFY | WS_EX_CLIENTEDGE;
//DWORD TreeViewConfig::nClassStyle = CS_DBLCLKS | CS_GLOBALCLASS | CS_PARENTDC;
DWORD TreeViewConfig::nInnerStyle = InnerStyle::TranceContextMenuUp;
DWORD TreeViewConfig::nOuterStyle = 0;
DWORD TreeViewConfig::nOwnerDrawTextStyle = DT_LEFT|DT_VCENTER|DT_SINGLELINE;
COLORREF TreeViewConfig::corBackground = ::GetSysColor(CTLCOLOR_LISTBOX);
COLORREF TreeViewConfig::corTextground = ::GetSysColor(COLOR_BTNTEXT);
COLORREF TreeViewConfig::corHotBackColor = ::GetSysColor(CTLCOLOR_LISTBOX);
COLORREF TreeViewConfig::corHotTextColor = ::GetSysColor(COLOR_BTNTEXT);
UINT TreeViewConfig::nWaitTimerDragOver = 400;
//===========================================================cc::win::TreeView
///////////////////////////////////////////////////////////////////////////////
// constructors/destructor
TreeView::TreeView()
{
	BASE_CONTROL_CONFIG_INIT(TreeView);
	_StyleArray[(int)StyleType::Inner2] |= Inner2Style::IsScrollRepaint;
	_Rect.right = _Rect.left + 105;
	_Rect.bottom = _Rect.top + 75;
	::CopyRect(&_OrgRect, &_Rect);
	_ClassName = WndClassName::TreeView;
	_StyleArray[(int)StyleType::InitControl] = CC_WNDCOMMCTL_TREEVIEW_REG;
	_hEdit = NULL;
	hItemDrag = NULL;
	labLineTop = NULL;
	CC_BIT_ON(_Flags, CC_FLAG_TREEVIEW_LEFTCLICKDRAG);
	CC_BIT_ON(_Flags, CC_FLAG_TREEVIEW_RIGHTCLICKDRAG);
#ifdef _DEBUG_
	SetName(_T("TreeView"));
#endif
}

TreeView::~TreeView()
{
}
//
//void TreeView::OnCtlColorEdit(EventArgs* e)
//{
//	//must do nothing
//	Control::OnCtlColorEdit(e);
//}

DWORD TreeView::GetAccessFlag(HTREEITEM hItem)
{
	return 0;
}

// Custom control notification messages handled here
void TreeView::OnChildNotify(EventArgs *e)
{
	LPNMHDR p_pNotifyStruct = (LPNMHDR)e->LParam;
	HTREEITEM selected = NULL;

	//LPNMTVCUSTOMDRAW pNMTVCustomDraw;
	//LPNMMOUSE pNMMouse;
	//LPNMTREEVIEW pNMTreeView;
	//LPNMTVDISPINFO pNMTVDispInfo;
	//LPNMTVGETINFOTIP pNMTVGetInfoTip;
	//LPNMTVKEYDOWN pNMTVKeyDown;

	switch(p_pNotifyStruct->code)
	{
		case NM_CLICK:  // User clicked left mouse button, track via NMHDR
			// Use p_pNotifyStruct
			// Add code here
			{
				TVHITTESTINFO ht = {0};
				DWORD dwpos = GetMessagePos();
				ht.pt.x = GET_X_LPARAM(dwpos);
				ht.pt.y = GET_Y_LPARAM(dwpos);
				MapWindowPoints(HWND_DESKTOP, m_hWnd, &ht.pt, 1);
				HitTest(&ht);
				if(ht.hItem != NULL)
				{
					int nAccessFlag = GetAccessFlag(ht.hItem);
					if(!CC_BIT_HAS(nAccessFlag, CC_ACCESS_ITEM_DISIABLED))
					{
						SelectItem(ht.hItem);
					}
					e->Handled = true;
					e->Result = TRUE;
					if(!CC_BIT_HAS(nAccessFlag, CC_ACCESS_CHKBOX_DISIABLED))
					{
						//set checked status.
						e->Result = DefWndProc(e);
						if(TVHT_ONITEMSTATEICON & ht.flags && (_StyleArray[(int)StyleType::Style] & TVS_CHECKBOXES) == TVS_CHECKBOXES)
						{
							int nChecked = (int)TreeView_GetCheckState(m_hWnd, ht.hItem);
							//notice:need use postmessage,and also need !isChecked!
							PostMessage(m_hWnd, CC_WM_CHECKCHANGED, MAKEWPARAM(nChecked == 1 ? 0 : 1, 0), (LPARAM)ht.hItem);
						}
					}
				}
			}
			OnClick(e);
			break;
		case NM_CUSTOMDRAW:  // Drawing operation notification, track via NMTVCUSTOMDRAW
			//pNMTVCustomDraw = (LPNMTVCUSTOMDRAW)p_pNotifyStruct;
			// Add code here
			OnCustomDraw(e);
			break;
		case NM_DBLCLK:  // User double clicked left mouse button, track via NMHDR
			// Use p_pNotifyStruct
			// Add code here
			selected = GetDropHilightItem();
			if(selected != NULL)
			{
				int nAccessFlag = GetAccessFlag(selected);
				if(!CC_BIT_HAS(nAccessFlag, CC_ACCESS_ITEM_DISIABLED))
				{
					SelectItem(selected);
				}
			}
			OnItemDoubleClick(e);
			//OnDoubleClick(e);
			break;
		case NM_KILLFOCUS:  // Control has lost input focus, track via NMHDR
			// Use p_pNotifyStruct
			// Add code here
			OnLeave(e);
			OnLostFocus(e);
			break;
		case NM_RCLICK:  // User clicked right mouse button, track via NMHDR
			// Use p_pNotifyStruct
			// Add code here
			selected = GetDropHilightItem();
			if(selected != NULL)
			{
				int nAccessFlag = GetAccessFlag(selected);
				if(!CC_BIT_HAS(nAccessFlag, CC_ACCESS_ITEM_DISIABLED))
				{
					SelectItem(selected);
				}
			}
			OnRClick(e);
			break;
		case NM_RETURN:  // User pressed ENTER, track via NMHDR
			// Use p_pNotifyStruct
			// Add code here
			//KeyEventArgs ee(e);
			//OnKeyDown(&ee);
			break;
		case NM_SETCURSOR:  // Control is setting cursor, track via NMMOUSE
			//pNMMouse = (LPNMMOUSE)p_pNotifyStruct;
			// Add code here
			OnSetCursor(e);
			break;
		case NM_SETFOCUS:  // Control received input focus, track via NMHDR
			// Use p_pNotifyStruct
			// Add code here
			OnEnter(e);
			OnGotFocus(e);
			break;
		case TVN_BEGINDRAG:  // Drag-and-drop operation with left mouse button is occuring, track via NMTREEVIEW
			//pNMTreeView = (LPNMTREEVIEW)p_pNotifyStruct;
			// Add code here
			if(CC_BIT_HAS(_Flags, CC_FLAG_TREEVIEW_LEFTCLICKDRAG))
			{
				DragBeginEventArgs ee(e);
				ee.Button = MouseButtons::Left;
				OnDragBegin(&ee);
			}
			break;
		case TVN_BEGINLABELEDIT:  // Item label edit start
			//pNMTVDispInfo = (LPNMTVDISPINFO)p_pNotifyStruct;
			// Add code here
			OnBeginLabelEdit(e);
			break;
		case TVN_BEGINRDRAG:  // Drag-and-drop operation with right mouse button is occuring, track via NMTREEVIEW
			//pNMTreeView = (LPNMTREEVIEW)p_pNotifyStruct;
			// Add code here
			if(CC_BIT_HAS(_Flags, CC_FLAG_TREEVIEW_RIGHTCLICKDRAG))
			{
				DragBeginEventArgs ee(e);
				ee.Button = MouseButtons::Right;
				OnDragBegin(&ee);
			}
			break;
		case TVN_DELETEITEM:  // Item is being deleted, track via NMTREEVIEW
			//Pointer to an NMTREEVIEW structure.
			//The itemOld member is a TVITEM structure whose hItem and lParam members contain valid information about the item being deleted.
			//pNMTreeView = (LPNMTREEVIEW)p_pNotifyStruct;
			// Add code here
			OnDeleteItem(e);
			break;
		case TVN_ENDLABELEDIT:  // End of item label edit
			//pNMTVDispInfo = (LPNMTVDISPINFO)p_pNotifyStruct;
			// Add code here
			OnEndLabelEdit(e);
			break;
		case TVN_GETDISPINFO:  // Display or sort information
			//pNMTVDispInfo = (LPNMTVDISPINFO)p_pNotifyStruct;
			// Add code here
			// Handle display callbacks
			// Text callback
			//if(pNMTVDispInfo->item.mask&TVIF_TEXT)
			//	strcpy(pNMTVDispInfo->item.pszText,"Callback");
			// Image callback
			//if(pNMTVDispInfo->item.mask&TVIF_IMAGE)
			//	pNMTVDispInfo->item.iImage = 13;
			// Selected image callback
			//if(pNMTVDispInfo->item.mask&TVIF_SELECTEDIMAGE)
			//	pNMTVDispInfo->item.iSelectedImage = 13;
			// Children callback
			//if(pNMTVDispInfo->item.mask&TVIF_CHILDREN)
			//	pNMTVDispInfo->item.cChildren = 0;
			OnGetDispinfo(e);
			break;
		case TVN_GETINFOTIP:  // Additional tooltip text request
			//pNMTVGetInfoTip = (LPNMTVGETINFOTIP)p_pNotifyStruct;
			// Add code here
			//OnGetInfotip(e);
			break;
		case TVN_ITEMEXPANDED:  // A list of child items has been expanded/collapsed
			//pNMTreeView = (LPNMTREEVIEW)p_pNotifyStruct;
			// Add code here
			if(HasStyle(StyleType::Outer, OuterStyle::TransparentParentBrush|OuterStyle::TransparentParentErase|OuterStyle::TransparentHollowBrush))
			{
				InvalidateRect(m_hWnd, NULL, TRUE);
			}
			OnItemExpanded(e);
			break;
		case TVN_ITEMEXPANDING:  // A list of child items is about to expand/collapse
			//pNMTreeView = (LPNMTREEVIEW)p_pNotifyStruct;
			// Add code here
			OnItemExpanding(e);
			break;
		case TVN_KEYDOWN:  // User pressed a key
			//pNMTVKeyDown = (LPNMTVKEYDOWN)p_pNotifyStruct;
			// Add code here
			//KeyEventArgs ee(e);
			//OnKeyDown(&ee);
			break;
	 	case TVN_SELCHANGED:  // Selection changed
			//pNMTreeView = (LPNMTREEVIEW)p_pNotifyStruct;
			// Add code here
			OnSelectedIndexChanged(e);
			break;
		case TVN_SELCHANGING:  // Selection is about to change
			//pNMTreeView = (LPNMTREEVIEW)p_pNotifyStruct;
			// Add code here
			OnSelectedIndexChanging(e);
			break;
		case TVN_SETDISPINFO:  // Must update item information
			//pNMTVDispInfo = (LPNMTVDISPINFO)p_pNotifyStruct;
			// Add code here
			OnSetDispinfo(e);
			break;
		case TVN_SINGLEEXPAND:  // User opened/closed a tree item with the mouse
			//pNMTreeView = (LPNMTREEVIEW)p_pNotifyStruct;
			// Add code here
			OnSingleExpand(e);
			break;
	}

	Control::OnChildNotify(e);
}

// WndProc - calls appriate On... function for the give
// message
LRESULT TreeView::WndProc(EventArgs *e)
{
	switch(e->Msg)
	{
	case CC_WM_CHECKCHANGED:
		OnItemCheckChanged(e);
		return (LRESULT)0;
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
		if(HasStyle(StyleType::Inner, InnerStyle::Dragging))
		{
			//notice drop on another ctl
			//WParam for drop item,LParam for drag(source) item
			e->WParam = (WPARAM)GetDropHilightItem();
			e->LParam = (LPARAM)hItemDrag;
			DragEndEventArgs ee(e);
			ee.Cancel = false;
			OnDragEnd(&ee);
		}
		break;
	case WM_KILLFOCUS:
	case WM_RBUTTONDOWN:
	case WM_LBUTTONDOWN:
		if(HasStyle(StyleType::Inner, InnerStyle::Dragging))
		{
			//while draging,if another click down,cancel draging.
			DragEndEventArgs ee(e);
			ee.Cancel = true;
			OnDragEnd(&ee);
		}
		break;
	}
	return Control::WndProc(e);
}

void TreeView::OnGotFocus(EventArgs* e)
{
	if(GetLostFocusShow() && labLineTop != NULL) {
		labLineTop->SetVisible(false);
	}
	Control::OnLostFocus(e);
}

void TreeView::OnLostFocus(EventArgs* e)
{
	HTREEITEM selected = GetSelectedItem();
	if(GetLostFocusShow() && selected != NULL)
	{
		if(labLineTop == NULL)
		{
			labLineTop = new Label();
			labLineTop->SetName(_T("labLineTop"));
			labLineTop->SetText(_T(""));
			labLineTop->SetLocation(0, 0);
			labLineTop->SetSize(100, 2);
			labLineTop->SetStyle(StyleType::Outer, 0, OuterStyle::Break);
			labLineTop->SetEnabled(false);
			this->AddControl(labLineTop);
		}

		RECT rect;
		GetItemRect(selected, &rect, TRUE);
		labLineTop->SetLocation(rect.left, rect.bottom - 1);
		if(rect.right - rect.left < 1)
		{
			rect.right = rect.left + 5;
		}
		labLineTop->SetBounds(0, 0, rect.right - rect.left, -1, BoundsSpecified::Width);
		labLineTop->SetVisible(true);
	}
	Control::OnLostFocus(e);
}

void TreeView::OnSelectedIndexChanged(EventArgs* e)
{
	CC_ASSERT(::IsWindow(m_hWnd));
	SelectedIndexChanged.Invoke((Object*)this, e);
	if(!this->Focused())
	{
		this->OnLostFocus(e);
	}
}
void TreeView::OnSelectedIndexChanging(EventArgs* e)
{
	CC_ASSERT(::IsWindow(m_hWnd));
	SelectedIndexChanging.Invoke((Object*)this, e);
}
void TreeView::OnItemDoubleClick(EventArgs* e)
{
	CC_ASSERT(::IsWindow(m_hWnd));
	ItemDoubleClick.Invoke((Object*)this, e);
}
void TreeView::OnItemExpanded(EventArgs* e)
{
	CC_ASSERT(::IsWindow(m_hWnd));
	ItemExpanded.Invoke((Object*)this, e);
}
void TreeView::OnItemExpanding(EventArgs* e)
{
	CC_ASSERT(::IsWindow(m_hWnd));
	ItemExpanding.Invoke((Object*)this, e);
}
void TreeView::OnSingleExpand(EventArgs* e)
{
	CC_ASSERT(::IsWindow(m_hWnd));
	SingleExpand.Invoke((Object*)this, e);
}
void TreeView::OnSetDispinfo(EventArgs* e)
{
	CC_ASSERT(::IsWindow(m_hWnd));
	SetDispinfo.Invoke((Object*)this, e);
}
void TreeView::OnGetDispinfo(EventArgs* e)
{
	CC_ASSERT(::IsWindow(m_hWnd));
	GetDispinfo.Invoke((Object*)this, e);
}
void TreeView::OnBeginLabelEdit(EventArgs* e)
{
	CC_ASSERT(::IsWindow(m_hWnd));
	_hEdit = TreeView_GetEditControl(m_hWnd);
	BeginLabelEdit.Invoke((Object*)this, e);
}
void TreeView::OnEndLabelEdit(EventArgs* e)
{
	CC_ASSERT(::IsWindow(m_hWnd));
	if(_hEdit != NULL)
	{
		cc::Str str;
		int nLen = GetWindowTextLength(_hEdit);
		TCHAR* tc = (TCHAR*)str.GetBufferSetLength(nLen + 2);
		GetWindowText(_hEdit, tc, nLen + 2);

		HTREEITEM selected = GetSelectedItem();
		SetItemText(selected, tc);
	}
	EndLabelEdit.Invoke((Object*)this, e);
}
//void TreeView::OnItemDelete(EventArgs* e)
//{
//	CC_ASSERT(::IsWindow(m_hWnd));
//	ItemDelete.Invoke((Object*)this, e);
//}

void TreeView::OnRClick(EventArgs* e)
{
	RClick.Invoke((Object*)this, e);
}

void TreeView::OnInnerTimer(EventArgs* e)
{
	//e->WParam is ID
	if(e->WParam != CC_TIMERID_CONTROL_DRAGOVER)
	{
		Control::OnInnerTimer(e);
		return;
	}
	::KillTimer(m_hWnd, CC_TIMERID_CONTROL_DRAGOVER);
	if(!HasStyle(StyleType::Inner, InnerStyle::Dragging))
	{
		return;
	}

	TVHITTESTINFO ht = {0};
	::GetCursorPos(&ht.pt);
	MapWindowPoints(HWND_DESKTOP, m_hWnd, &ht.pt, 1);
	if(ht.pt.y < 15)
	{
		HTREEITEM h_item = HitTest(&ht);
		h_item = GetPrevVisibleItem(h_item);
		EnsureVisible(h_item);
		::SetTimer(m_hWnd, CC_TIMERID_CONTROL_DRAGOVER, TreeViewConfig::nWaitTimerDragOver, NULL);
	}
	else
	{
		RECT rect;
		::GetClientRect(m_hWnd, &rect);
		if(ht.pt.y > rect.bottom - 15)
		{
			HTREEITEM h_item = HitTest(&ht);
			h_item = GetNextVisibleItem(h_item);
			EnsureVisible(h_item);
			::SetTimer(m_hWnd, CC_TIMERID_CONTROL_DRAGOVER, TreeViewConfig::nWaitTimerDragOver, NULL);
		}
	}
}

//if Cancel in DragBegin,then dothing
void TreeView::OnDragBegin(DragBeginEventArgs* e)
{
	LPNMTREEVIEW lpnmtv = (LPNMTREEVIEW)e->baseArgs->LParam;
	hItemDrag = lpnmtv->itemNew.hItem;
	CC_ASSERT(hItemDrag != NULL);
	SelectItem(hItemDrag);

	e->data = (void*)hItemDrag;
	DragBegin.Invoke((Object*)this, e);
	if(e->Cancel)
	{
		return;
	}
	SetStyle(StyleType::Inner, 0, InnerStyle::Dragging);
	//notice not need OnCustomDraw while draw dragimage
	CC_BIT_SET(_Flags, CC_FLAG_TREEVIEW_NOCUSTOMDRAW, true);
	HIMAGELIST hImg = TreeView_CreateDragImage(m_hWnd, lpnmtv->itemNew.hItem);
	CC_BIT_SET(_Flags, CC_FLAG_TREEVIEW_NOCUSTOMDRAW, false);
	ImageList_BeginDrag(hImg, 0, 0, 0);
	ClientToScreen(m_hWnd, &(lpnmtv->ptDrag));
	ImageList_DragEnter(NULL, lpnmtv->ptDrag.x, lpnmtv->ptDrag.y);
	ShowCursor(FALSE);
	SetCapture(m_hWnd);
}

//notice that e->X,e->Y perhaps relate to another control
void TreeView::OnDragMove(MouseEventArgs* e)
{
	TVHITTESTINFO ht = {0};
	::GetCursorPos(&ht.pt);
	MapWindowPoints(HWND_DESKTOP, m_hWnd, &ht.pt, 1);

	HTREEITEM h_item = HitTest(&ht);
	e->baseArgs->WParam = (WPARAM)h_item;

	DragMove.Invoke((Object*)this, e);
	h_item = (HTREEITEM)e->baseArgs->WParam;
	Select(h_item, TVGN_DROPHILITE);

	::SetTimer(m_hWnd, CC_TIMERID_CONTROL_DRAGOVER, 100, NULL);
//cc::Log::debug(_T("x:%d, y:%d, hwnd:%d, item:%d, FirstVisible:%d, Next:%d, Prev:%d, "), ht.pt.x, ht.pt.y, m_hWnd, h_item, h1, h2, h3);
}

void TreeView::OnDragEnd(DragEndEventArgs* e)
{
	if(HasStyle(StyleType::Inner, InnerStyle::Dragging))
	{
		SetStyle(StyleType::Inner, InnerStyle::Dragging, 0);
		ImageList_DragLeave(m_hWnd);
		ImageList_EndDrag();
		ReleaseCapture();
		ShowCursor(TRUE);
		SelectItem(NULL);
		HTREEITEM hItem = NULL;
		if(e->Cancel)
		{
			Select(NULL, TVGN_DROPHILITE);
		}
		else
		{
			hItem = GetDropHilightItem();
		}
		if(hItem == NULL)
		{
			hItem = GetSelectedItem();
		}
		SelectItem(hItem);

		//WParam for drop item,LParam for drag(source) item
		e->baseArgs->WParam = (WPARAM)GetDropHilightItem();
		e->baseArgs->LParam = (LPARAM)hItemDrag;
		DragEnd.Invoke((Object*)this, e);
	}
}

//notice:here(OnItemCheckChanged) status of checkbox is right
//PostMessage(m_hWnd, CC_WM_CHECKCHANGED, MAKEWPARAM(!isChecked, 0), (LPARAM)ht.hItem);
//HIWORD(e->WParam) = 0, LOWORD(e->WParam) = isChecked, e->LParam = ht.hItem(SelectedItem)
void TreeView::OnItemCheckChanged(EventArgs* e)
{
	//cc::Log::debug(_T("TreeView::OnItemCheckChanged, LOWORD(e->WParam):%d, GetCheckState:%d"), (int)LOWORD(e->WParam), TreeView_GetCheckState(m_hWnd, GetSelectedItem()));
	ItemCheckChanged.Invoke((Object*)this, e);
}

void TreeView::OnCustomDraw(EventArgs* e)
{
	//LPNMTVCUSTOMDRAW pCustomDraw = (LPNMTVCUSTOMDRAW)e->LParam;
	CustomDraw.Invoke((Object*)this, e);
}

cc::Str TreeView::GetItemText(HTREEITEM hItem, int nLen) const
{
	CC_ASSERT(::IsWindow(m_hWnd));
	TVITEM item;
	memset(&item, 0, sizeof(TVITEM));
	item.hItem = hItem;
	item.mask = TVIF_HANDLE | TVIF_TEXT;

	cc::Str str;
	item.cchTextMax = nLen;
	item.pszText = (TCHAR*)str.GetBufferSetLength(nLen, false);
	GetItem(&item);
	str.UpdateStringLength();
	return str;
}

int TreeView::GetItemText(HTREEITEM hItem, LPTSTR lpszText, int nLen) const
{
	CC_ASSERT(::IsWindow(m_hWnd));
	TVITEM item;
	memset(&item, 0, sizeof(TVITEM));
	item.hItem = hItem;
	item.mask = TVIF_HANDLE | TVIF_TEXT;
	item.cchTextMax = nLen;
	item.pszText = (LPTSTR)lpszText;
	GetItem(&item);
	return (int)item.cchTextMax;
}

DWORD_PTR TreeView::GetItemData(HTREEITEM hItem) const
{
	CC_ASSERT(::IsWindow(m_hWnd));
	TVITEM item;
	memset(&item, 0, sizeof(TVITEM));
	item.hItem = hItem;
	item.mask = TVIF_HANDLE | TVIF_PARAM;
	GetItem(&item);
	return item.lParam;
}

//#define IIL_UNCHECKED 1
//#define IIL_CHECKED 2
BOOL TreeView::GetItemChecked(HTREEITEM hItem) const
{
	CC_ASSERT(::IsWindow(m_hWnd));
	if(hItem == NULL) {
		return FALSE;
	}
	return (BOOL)TreeView_GetCheckState(m_hWnd, hItem);
	//TVITEM item;
	//memset(&item, 0, sizeof(TVITEM));
	//item.hItem = hItem;
	//item.mask = TVIF_HANDLE | TVIF_STATE;
	//item.stateMask = TVIS_STATEIMAGEMASK;
	//item.state = 0;
	//GetItem(&item);
	////? Return zero if it's not checked, or nonzero otherwise.
	//return ((BOOL)(item.state >> 12) -1);
	////return ((BOOL)((item.state & INDEXTOSTATEIMAGEMASK(IIL_CHECKED)));
}

BOOL TreeView::SetItemChecked(HTREEITEM hItem, BOOL fCheck, BOOL isUpdateParentChild)
{
	CC_ASSERT(::IsWindow(m_hWnd));
	//TreeView_SetCheckState(m_hWnd, hItem, fCheck);
	//TreeView_SetItemState(hwndTV, hti, INDEXTOSTATEIMAGEMASK((fCheck)?2:1), TVIS_STATEIMAGEMASK)
	TreeView_SetItemState(m_hWnd, hItem, INDEXTOSTATEIMAGEMASK(fCheck >= 2 ? 3 : (fCheck == 1 ? 2 : 1)), TVIS_STATEIMAGEMASK)
	if(isUpdateParentChild == TRUE)
	{
		PostMessage(m_hWnd, CC_WM_CHECKCHANGED, MAKEWPARAM(fCheck, 0), (LPARAM)hItem);
	}
	//TVITEM item;
	//memset(&item, 0, sizeof(TVITEM));
	//item.hItem = hItem;
	//item.mask = TVIF_HANDLE | TVIF_STATE;
	//item.stateMask = TVIS_STATEIMAGEMASK;
	////Since state images are one-based, 1 in this macro turns the check off, and 2 turns it on.
	//item.state = INDEXTOSTATEIMAGEMASK((fCheck ? 2 : 1));
	//SetItem(&item);
	return TRUE;
}
void TreeView::SetDragType(LeftRight::Type type, bool value)
{
	if((type & LeftRight::Left) != 0)
	{
		CC_BIT_SET(_Flags, CC_FLAG_TREEVIEW_LEFTCLICKDRAG, value);
	}
	if((type & LeftRight::Right) != 0)
	{
		CC_BIT_SET(_Flags, CC_FLAG_TREEVIEW_RIGHTCLICKDRAG, value);
	}
}

bool TreeView::GetDragType(LeftRight::Type type)
{
	if((type & LeftRight::Left) != 0)
	{
		return CC_BIT_HAS(_Flags, CC_FLAG_TREEVIEW_LEFTCLICKDRAG);
	}
	return CC_BIT_HAS(_Flags, CC_FLAG_TREEVIEW_RIGHTCLICKDRAG);
}

void TreeView::SetLostFocusShow(bool value)
{
	CC_BIT_SET(_Flags, CC_FLAG_TREEVIEW_LOSTFOCUSSHOW, value);
}

bool TreeView::GetLostFocusShow()
{
	return CC_BIT_HAS(_Flags, CC_FLAG_TREEVIEW_LOSTFOCUSSHOW);
}

BOOL TreeView::SetItem(HTREEITEM hItem, UINT nMask, LPCTSTR lpszItem, int nImage,
	int nSelectedImage, UINT nState, UINT nStateMask, LPARAM lParam)
{
	CC_ASSERT(::IsWindow(m_hWnd));
	TVITEM item;
	item.hItem = hItem;
	item.mask = nMask;
	item.pszText = (LPTSTR) lpszItem;
	item.iImage = nImage;
	item.iSelectedImage = nSelectedImage;
	item.state = nState;
	item.stateMask = nStateMask;
	item.lParam = lParam;
	return (BOOL)::SendMessage(m_hWnd, TVM_SETITEM, 0, (LPARAM)&item);
}

HTREEITEM TreeView::ItemAdd(UINT nMask, LPCTSTR lpszItem, int nImage,
	int nSelectedImage, UINT nState, UINT nStateMask, LPARAM lParam,
	HTREEITEM hParent, HTREEITEM hInsertAfter)
{
	CC_ASSERT(::IsWindow(m_hWnd));
	TVINSERTSTRUCT tvis;
	tvis.hParent = hParent;
	tvis.hInsertAfter = hInsertAfter;
	tvis.item.mask = nMask;
	tvis.item.pszText = (LPTSTR) lpszItem;
	tvis.item.iImage = nImage;
	tvis.item.iSelectedImage = nSelectedImage;
	tvis.item.state = nState;
	tvis.item.stateMask = nStateMask;
	tvis.item.lParam = lParam;
	return (HTREEITEM)::SendMessage(m_hWnd, TVM_INSERTITEM, 0, (LPARAM)&tvis);
}
HTREEITEM TreeView::ItemAdd(LPTVINSERTSTRUCT lpInsertStruct)
{
	CC_ASSERT(::IsWindow(m_hWnd));
	return (HTREEITEM)::SendMessage(m_hWnd, TVM_INSERTITEM, 0, (LPARAM)lpInsertStruct);
}
HTREEITEM TreeView::ItemAdd(LPCTSTR lpszItem, int nImage,
	int nSelectedImage, HTREEITEM hParent, HTREEITEM hInsertAfter)
{
	CC_ASSERT(::IsWindow(m_hWnd));
	return ItemAdd(TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE, lpszItem, nImage, nSelectedImage, 0, 0, 0, hParent, hInsertAfter);
}
HTREEITEM TreeView::ItemAdd(LPCTSTR lpszItem, HTREEITEM hParent, HTREEITEM hInsertAfter)
{
	CC_ASSERT(::IsWindow(m_hWnd));
	return ItemAdd(TVIF_TEXT, lpszItem, 0, 0, 0, 0, 0, hParent, hInsertAfter);
}
BOOL TreeView::ItemDel(HTREEITEM hItem)
{
	CC_ASSERT(::IsWindow(m_hWnd));
	CC_ASSERT(GetItemData(hItem) == NULL);
	return (BOOL)::SendMessage(m_hWnd, TVM_DELETEITEM, 0, (LPARAM)hItem);
}
#pragma warning(disable: 4310)
BOOL TreeView::ItemClear()
{
	CC_ASSERT(::IsWindow(m_hWnd));
#ifdef _DEBUG_
	HTREEITEM hRoot = GetRootItem();
	if(hRoot != NULL)
	{
		CC_ASSERT(GetItemData(hRoot) == NULL);
	}
#endif
	return (BOOL)::SendMessage(m_hWnd, TVM_DELETEITEM, 0, (LPARAM)TVI_ROOT);
}
#pragma warning(default: 4310)
/*
flag for TVM_EXPAND of Expand:
Action flag. This parameter can be one or more of the following values:
TVE_COLLAPSE
    Collapses the list. 
TVE_COLLAPSERESET
    Collapses the list and removes the child items. The TVIS_EXPANDEDONCE state flag is reset. This flag must be used with the TVE_COLLAPSE flag.
TVE_EXPAND
    Expands the list.
TVE_EXPANDPARTIAL
    Version 4.70. Partially expands the list. In this state the child items are visible and the parent item's plus sign (+), indicating that it can be expanded, is displayed. This flag must be used in combination with the TVE_EXPAND flag.
TVE_TOGGLE
    Collapses the list if it is expanded or expands it if it is collapsed.
*/
BOOL TreeView::Expand(HTREEITEM hItem, UINT nCode)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (BOOL)::SendMessage(m_hWnd, TVM_EXPAND, nCode, (LPARAM)hItem); }
UINT TreeView::GetCount() const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (UINT)::SendMessage(m_hWnd, TVM_GETCOUNT, 0, 0); }
UINT TreeView::GetIndent() const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (UINT)::SendMessage(m_hWnd, TVM_GETINDENT, 0, 0); }
void TreeView::SetIndent(UINT nIndent)
	{ CC_ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, TVM_SETINDENT, nIndent, 0); }
//CImageList* TreeView::GetImageList(UINT nImageList) const
//	{ CC_ASSERT(::IsWindow(m_hWnd)); return CImageList::FromHandle((HIMAGELIST)::SendMessage(m_hWnd, TVM_GETIMAGELIST, (UINT)nImageList, 0)); }
//CImageList* TreeView::SetImageList(CImageList* pImageList, int nImageListType)
//	{ CC_ASSERT(::IsWindow(m_hWnd)); return CImageList::FromHandle((HIMAGELIST)::SendMessage(m_hWnd, TVM_SETIMAGELIST, (UINT)nImageListType, (LPARAM)pImageList->GetSafeHandle())); }
HIMAGELIST TreeView::GetImageList(int nImageList) const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (HIMAGELIST) ::SendMessage(m_hWnd, TVM_GETIMAGELIST, nImageList, 0L); }
HIMAGELIST TreeView::SetImageList(HIMAGELIST hImageList, int nImageListType)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (HIMAGELIST) ::SendMessage(m_hWnd, TVM_SETIMAGELIST, nImageListType, (LPARAM)hImageList); }
UINT TreeView::SetScrollTime(UINT uScrollTime)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (UINT) ::SendMessage(m_hWnd, TVM_SETSCROLLTIME, (WPARAM) uScrollTime, 0); }
/*
flag for TVM_GETNEXTITEM of GetNextItem:
Flag specifying the item to retrieve. This parameter can be one of the following values:
TVGN_CARET
    Retrieves the currently selected item. You can use the TreeView_GetSelection macro to send this message.
TVGN_CHILD
    Retrieves the first child item of the item specified by the hitem parameter. You can use the TreeView_GetChild macro to send this message.
TVGN_DROPHILITE
    Retrieves the item that is the target of a drag-and-drop operation. You can use the TreeView_GetDropHilight macro to send this message.
TVGN_FIRSTVISIBLE
    Retrieves the first item that is visible in the tree-view window. You can use the TreeView_GetFirstVisible macro to send this message.
TVGN_LASTVISIBLE
    Version 4.71. Retrieves the last expanded item in the tree. This does not retrieve the last item visible in the tree-view window. You can use the TreeView_GetLastVisible macro to send this message.
TVGN_NEXT
    Retrieves the next sibling item. You can use the TreeView_GetNextSibling macro to send this message.
TVGN_NEXTSELECTED
    Microsoft Windows Vista and later. Retrieves the next selected item. You can use the TreeView_GetNextSelected macro to send this message.
TVGN_NEXTVISIBLE
    Retrieves the next visible item that follows the specified item. The specified item must be visible. Use the TVM_GETITEMRECT message to determine whether an item is visible. You can use the TreeView_GetNextVisible macro to send this message.
TVGN_PARENT
    Retrieves the parent of the specified item. You can use the TreeView_GetParent macro to send this message.
TVGN_PREVIOUS
    Retrieves the previous sibling item. You can use the TreeView_GetPrevSibling macro to send this message.
TVGN_PREVIOUSVISIBLE
    Retrieves the first visible item that precedes the specified item. The specified item must be visible. Use the TVM_GETITEMRECT message to determine whether an item is visible. You can use the TreeView_GetPrevVisible macro to send this message.
TVGN_ROOT
    Retrieves the topmost or very first item of the tree-view control. You can use the TreeView_GetRoot macro to send this message. 
*/
HTREEITEM TreeView::GetNextItem(HTREEITEM hItem, UINT nCode) const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (HTREEITEM)::SendMessage(m_hWnd, TVM_GETNEXTITEM, nCode, (LPARAM)hItem); }
HTREEITEM TreeView::GetChildItem(HTREEITEM hItem) const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (HTREEITEM)::SendMessage(m_hWnd, TVM_GETNEXTITEM, TVGN_CHILD, (LPARAM)hItem); }
HTREEITEM TreeView::GetNextSiblingItem(HTREEITEM hItem) const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (HTREEITEM)::SendMessage(m_hWnd, TVM_GETNEXTITEM, TVGN_NEXT, (LPARAM)hItem); }
HTREEITEM TreeView::GetPrevSiblingItem(HTREEITEM hItem) const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (HTREEITEM)::SendMessage(m_hWnd, TVM_GETNEXTITEM, TVGN_PREVIOUS, (LPARAM)hItem); }
HTREEITEM TreeView::GetParentItem(HTREEITEM hItem) const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (HTREEITEM)::SendMessage(m_hWnd, TVM_GETNEXTITEM, TVGN_PARENT, (LPARAM)hItem); }
HTREEITEM TreeView::GetFirstVisibleItem() const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (HTREEITEM)::SendMessage(m_hWnd, TVM_GETNEXTITEM, TVGN_FIRSTVISIBLE, 0); }
HTREEITEM TreeView::GetNextVisibleItem(HTREEITEM hItem) const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (HTREEITEM)::SendMessage(m_hWnd, TVM_GETNEXTITEM, TVGN_NEXTVISIBLE, (LPARAM)hItem); }
HTREEITEM TreeView::GetPrevVisibleItem(HTREEITEM hItem) const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (HTREEITEM)::SendMessage(m_hWnd, TVM_GETNEXTITEM, TVGN_PREVIOUSVISIBLE, (LPARAM)hItem); }
HTREEITEM TreeView::GetSelectedItem() const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (HTREEITEM)::SendMessage(m_hWnd, TVM_GETNEXTITEM, TVGN_CARET, 0); }
UINT TreeView::GetScrollTime() const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (UINT) ::SendMessage(m_hWnd, TVM_GETSCROLLTIME, 0, 0); }
HTREEITEM TreeView::GetDropHilightItem() const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (HTREEITEM)::SendMessage(m_hWnd, TVM_GETNEXTITEM, TVGN_DROPHILITE, 0); }
HTREEITEM TreeView::GetRootItem() const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (HTREEITEM)::SendMessage(m_hWnd, TVM_GETNEXTITEM, TVGN_ROOT, 0); }
/*
flag for TVM_SELECTITEM of Select:
Action flag. This parameter can be one of the following values:
TVGN_CARET
    Sets the selection to the specified item. The tree-view control's parent window receives the TVN_SELCHANGING and TVN_SELCHANGED notification messages. 
TVGN_DROPHILITE
    Redraws the specified item in the style used to indicate the target of a drag-and-drop operation.
TVGN_FIRSTVISIBLE
    Ensures that the specified item is visible, and, if possible, displays it at the top of the control's window. Tree-view controls display as many items as will fit in the window. If the specified item is near the bottom of the control's hierarchy of items, it might not become the first visible item, depending on how many items fit in the window.
TVSI_NOSINGLEEXPAND
    Windows XP and later: When a single item is selected, ensures that the treeview does not expand the children of that item. This is valid only if used with the TVGN_CARET flag.
    Note  To use this flag, you must provide a manifest specifying Comclt32.dll version 6.0. For more information on manifests, see Enabling Visual Styles.
*/
BOOL TreeView::Select(HTREEITEM hItem, UINT nCode)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (BOOL)::SendMessage(m_hWnd, TVM_SELECTITEM, nCode, (LPARAM)hItem); }
BOOL TreeView::SelectItem(HTREEITEM hItem)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, TVM_SELECTITEM, TVGN_CARET, (LPARAM)hItem); }
BOOL TreeView::SelectDropTarget(HTREEITEM hItem)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, TVM_SELECTITEM, TVGN_DROPHILITE, (LPARAM)hItem); }
BOOL TreeView::SelectSetFirstVisible(HTREEITEM hItem)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, TVM_SELECTITEM, TVGN_FIRSTVISIBLE, (LPARAM)hItem); }
BOOL TreeView::GetItem(TVITEM* pItem) const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (BOOL)::SendMessage(m_hWnd, TVM_GETITEM, 0, (LPARAM)pItem); }
BOOL TreeView::SetItem(TVITEM* pItem)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (BOOL)::SendMessage(m_hWnd, TVM_SETITEM, 0, (LPARAM)pItem); }
BOOL TreeView::SetItemText(HTREEITEM hItem, LPCTSTR lpszItem)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return SetItem(hItem, TVIF_TEXT, lpszItem, 0, 0, 0, 0, NULL); }
BOOL TreeView::SetItemImage(HTREEITEM hItem, int nImage, int nSelectedImage)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return SetItem(hItem, TVIF_IMAGE|TVIF_SELECTEDIMAGE, NULL, nImage, nSelectedImage, 0, 0, NULL); }
BOOL TreeView::SetItemState(HTREEITEM hItem, UINT nState, UINT nStateMask)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return SetItem(hItem, TVIF_STATE, NULL, 0, 0, nState, nStateMask, NULL); }
BOOL TreeView::SetItemData(HTREEITEM hItem, DWORD_PTR dwData)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return SetItem(hItem, TVIF_PARAM, NULL, 0, 0, 0, 0, (LPARAM)dwData); }
//CEdit* TreeView::EditLabel(HTREEITEM hItem)
//	{ CC_ASSERT(::IsWindow(m_hWnd)); return (CEdit*)CWnd::FromHandle((HWND)::SendMessage(m_hWnd, TVM_EDITLABEL, 0, (LPARAM)hItem)); }
HTREEITEM TreeView::HitTest(TVHITTESTINFO* pHitTestInfo) const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (HTREEITEM)::SendMessage(m_hWnd, TVM_HITTEST, 0, (LPARAM)pHitTestInfo); }
//CEdit* TreeView::GetEditControl() const
//	{ CC_ASSERT(::IsWindow(m_hWnd)); return (CEdit*)CWnd::FromHandle((HWND)::SendMessage(m_hWnd, TVM_GETEDITCONTROL, 0, 0)); }
UINT TreeView::GetVisibleCount() const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (UINT)::SendMessage(m_hWnd, TVM_GETVISIBLECOUNT, 0, 0); }
BOOL TreeView::SortChildren(HTREEITEM hItem)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (BOOL)::SendMessage(m_hWnd, TVM_SORTCHILDREN, 0, (LPARAM)hItem); }
BOOL TreeView::EnsureVisible(HTREEITEM hItem)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (BOOL)::SendMessage(m_hWnd, TVM_ENSUREVISIBLE, 0, (LPARAM)hItem); }
BOOL TreeView::SortChildrenCB(LPTVSORTCB pSort)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (BOOL)::SendMessage(m_hWnd, TVM_SORTCHILDRENCB, 0, (LPARAM)pSort); }
//CToolTipCtrl* TreeView::GetToolTips() const
//	{ CC_ASSERT(::IsWindow(m_hWnd)); return (CToolTipCtrl*)CWnd::FromHandle((HWND)::SendMessage(m_hWnd, TVM_GETTOOLTIPS, 0, 0L)); }
//CToolTipCtrl* TreeView::SetToolTips(CToolTipCtrl* pWndTip)
//	{ CC_ASSERT(::IsWindow(m_hWnd)); return (CToolTipCtrl*)CWnd::FromHandle((HWND)::SendMessage(m_hWnd, TVM_SETTOOLTIPS, (WPARAM)pWndTip->GetSafeHwnd(), 0L)); }
COLORREF TreeView::GetBackground() const
{
	//CC_ASSERT(::IsWindow(m_hWnd));
	if(m_hWnd != NULL)
	{
		return (COLORREF)::SendMessage(m_hWnd, TVM_GETBKCOLOR, 0, 0L);
	}
	return _corBackground;
}
//COLORREF TreeView::SetBkColor(COLORREF clr)
//	{ CC_ASSERT(::IsWindow(m_hWnd)); return (COLORREF) ::SendMessage(m_hWnd, TVM_SETBKCOLOR, 0, (LPARAM)clr); }
void TreeView::SetBackground(COLORREF corBackground)
{
	::SendMessage(m_hWnd, TVM_SETBKCOLOR, 0, (LPARAM)corBackground);
	Control::SetBackground(corBackground);
}
SHORT TreeView::GetItemHeight() const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (SHORT) ::SendMessage(m_hWnd, TVM_GETITEMHEIGHT, 0, 0L); }
SHORT TreeView::SetItemHeight(SHORT cyHeight)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (SHORT) ::SendMessage(m_hWnd, TVM_SETITEMHEIGHT, cyHeight, 0L); }
COLORREF TreeView::GetTextColor() const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (COLORREF) ::SendMessage(m_hWnd, TVM_GETTEXTCOLOR, 0, 0L); }
COLORREF TreeView::SetTextColor(COLORREF clr)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (COLORREF) ::SendMessage(m_hWnd, TVM_SETTEXTCOLOR, 0, (LPARAM)clr); }
BOOL TreeView::SetInsertMark(HTREEITEM hItem, BOOL fAfter)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, TVM_SETINSERTMARK, fAfter, (LPARAM)hItem); }
COLORREF TreeView::GetInsertMarkColor() const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (COLORREF) ::SendMessage(m_hWnd, TVM_GETINSERTMARKCOLOR, 0, 0L); }
COLORREF TreeView::SetInsertMarkColor(COLORREF clrNew)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (COLORREF) ::SendMessage(m_hWnd, TVM_SETINSERTMARKCOLOR, 0, (LPARAM)clrNew); }
BOOL TreeView::GetItemRect(HTREEITEM hItem, LPRECT lpRect, BOOL bTextOnly) const
{
	CC_ASSERT(::IsWindow(m_hWnd));
	*(HTREEITEM*)lpRect = hItem;
	return (BOOL)::SendMessage(m_hWnd, TVM_GETITEMRECT, (WPARAM)bTextOnly, (LPARAM)lpRect);
}
BOOL TreeView::ItemHasChildren(HTREEITEM hItem) const
{
	CC_ASSERT(::IsWindow(m_hWnd));
	TVITEM item;
	item.hItem = hItem;
	item.mask = TVIF_CHILDREN;
	::SendMessage(m_hWnd, TVM_GETITEM, 0, (LPARAM)&item);
	return item.cChildren;
}

#if _WIN32_IE >= 0x0500
COLORREF TreeView::GetLineColor() const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (COLORREF)::SendMessage(m_hWnd, TVM_GETLINECOLOR, 0, 0L); }
COLORREF TreeView::SetLineColor(COLORREF clrNew /*= CLR_DEFAULT*/)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (COLORREF)::SendMessage(m_hWnd, TVM_SETLINECOLOR, 0, (LPARAM)clrNew); }
#endif
///////////////////////////////////////////////////////////////////////////////

DWORD DateTimePickerConfig::nStyle = WS_CHILDWINDOW | WS_VISIBLE | WS_TABSTOP | DTS_RIGHTALIGN | DTS_SHORTDATEFORMAT;
DWORD DateTimePickerConfig::nExStyle = WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR | WS_EX_NOPARENTNOTIFY | WS_EX_CLIENTEDGE;
//DWORD DateTimePickerConfig::nClassStyle = CS_DBLCLKS | CS_GLOBALCLASS | CS_PARENTDC;
DWORD DateTimePickerConfig::nInnerStyle = InnerStyle::TranceContextMenuUp;
DWORD DateTimePickerConfig::nOuterStyle = 0;
DWORD DateTimePickerConfig::nOwnerDrawTextStyle = DT_LEFT|DT_VCENTER|DT_SINGLELINE;
COLORREF DateTimePickerConfig::corBackground = ::GetSysColor(COLOR_BTNFACE);
COLORREF DateTimePickerConfig::corTextground = ::GetSysColor(COLOR_BTNTEXT);
COLORREF DateTimePickerConfig::corHotBackColor = ::GetSysColor(COLOR_BTNFACE);
COLORREF DateTimePickerConfig::corHotTextColor = ::GetSysColor(COLOR_BTNTEXT);
//===========================================================cc::win::DateTimePicker
///////////////////////////////////////////////////////////////////////////////
// constructors/destructor
DateTimePicker::DateTimePicker()
{
	BASE_CONTROL_CONFIG_INIT(DateTimePicker);
	_Rect.right = _Rect.left + 175;
	_Rect.bottom = _Rect.top + 23;
	::CopyRect(&_OrgRect, &_Rect);
	_ClassName = WndClassName::DateTimePicker;
	_StyleArray[(int)StyleType::InitControl] = CC_WNDCOMMCTLSNEW_REG;
#ifdef _DEBUG_
	SetName(_T("DateTimePicker"));
#endif
}

DateTimePicker::~DateTimePicker()
{
}

void DateTimePicker::SetText(const TCHAR* sText)
{
	SetText(cc::Str(sText));
}

void DateTimePicker::SetText(const cc::Str& sText)
{
	if(sText.GetLength() != 10)
	{
		return;
	}
	LockAction lock(&m_lockText);
	_Text = CC_StrCopy(&_Text, (const TCHAR*)sText.GetBuf());
	_sizeTextIcon.cx = -1;

	SYSTEMTIME sysTime;
	ZeroMemory(&sysTime, sizeof(sysTime));
	sysTime.wYear = sText.Substring(0, 4).ToInt();
	sysTime.wMonth = sText.Substring(5, 7).ToInt();
	sysTime.wDay = sText.Substring(8, 10).ToInt();
	DateTime_SetSystemtime(m_hWnd, GDT_VALID, &sysTime);

	_nUpdateFlag |= UpdateFlag::UpdateText;
	Update();
}
// constructors/destructor
///////////////////////////////////////////////////////////////////////////////

DWORD MonthCalendarConfig::nStyle = WS_CHILDWINDOW | WS_VISIBLE | WS_TABSTOP | MCS_NOTODAYCIRCLE | MCS_NOTODAY;
DWORD MonthCalendarConfig::nExStyle = WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR | WS_EX_NOPARENTNOTIFY | WS_EX_CLIENTEDGE;
//DWORD MonthCalendarConfig::nClassStyle = CS_DBLCLKS | CS_GLOBALCLASS | CS_PARENTDC;
DWORD MonthCalendarConfig::nInnerStyle = InnerStyle::TranceContextMenuUp;
DWORD MonthCalendarConfig::nOuterStyle = 0;
DWORD MonthCalendarConfig::nOwnerDrawTextStyle = DT_LEFT|DT_VCENTER|DT_SINGLELINE;
COLORREF MonthCalendarConfig::corBackground = ::GetSysColor(COLOR_BTNFACE);
COLORREF MonthCalendarConfig::corTextground = ::GetSysColor(COLOR_BTNTEXT);
COLORREF MonthCalendarConfig::corHotBackColor = ::GetSysColor(COLOR_BTNFACE);
COLORREF MonthCalendarConfig::corHotTextColor = ::GetSysColor(COLOR_BTNTEXT);
//===========================================================cc::win::MonthCalendar
///////////////////////////////////////////////////////////////////////////////
// constructors/destructor
MonthCalendar::MonthCalendar()
{
	BASE_CONTROL_CONFIG_INIT(MonthCalendar);
	_Rect.right = _Rect.left + 245;
	_Rect.bottom = _Rect.top + 150;
	::CopyRect(&_OrgRect, &_Rect);
	_ClassName = WndClassName::MonthCalendar;
	_StyleArray[(int)StyleType::InitControl] = CC_WNDCOMMCTLSNEW_REG;
#ifdef _DEBUG_
	SetName(_T("MonthCalendar"));
#endif
}

MonthCalendar::~MonthCalendar()
{
}
// constructors/destructor
///////////////////////////////////////////////////////////////////////////////

DWORD SplitterConfig::nStyle = WS_CHILDWINDOW | WS_VISIBLE;
DWORD SplitterConfig::nExStyle = WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR | WS_EX_NOPARENTNOTIFY;
//DWORD SplitterConfig::nClassStyle = CS_DBLCLKS | CS_GLOBALCLASS | CS_PARENTDC;
DWORD SplitterConfig::nInnerStyle = InnerStyle::TranceContextMenuUp;
DWORD SplitterConfig::nOuterStyle = 0;
DWORD SplitterConfig::nOwnerDrawTextStyle = DT_LEFT|DT_VCENTER|DT_SINGLELINE;
COLORREF SplitterConfig::corBackground = ::GetSysColor(COLOR_BTNFACE);
COLORREF SplitterConfig::corTextground = ::GetSysColor(COLOR_BTNTEXT);
COLORREF SplitterConfig::corHotBackColor = ::GetSysColor(COLOR_BTNFACE);
COLORREF SplitterConfig::corHotTextColor = ::GetSysColor(COLOR_BTNTEXT);
//===========================================================cc::win::Splitter
///////////////////////////////////////////////////////////////////////////////
// constructors/destructor
Splitter::Splitter()
{
	BASE_CONTROL_CONFIG_INIT(Splitter);
	_Rect.right = _Rect.left + 3;
	_Rect.bottom = _Rect.top + 303;
	::CopyRect(&_OrgRect, &_Rect);
	_ClassName = WndClassName::Splitter;
	//_StyleArray[(int)StyleType::InitControl] = CC_WNDCOMMCTLS_REG;
#ifdef _DEBUG_
	SetName(_T("Splitter"));
#endif
}

Splitter::~Splitter()
{
}
// constructors/destructor
///////////////////////////////////////////////////////////////////////////////

DWORD ContainerConfig::nStyle = WS_CHILD | WS_VISIBLE | WS_TABSTOP;
DWORD ContainerConfig::nExStyle = 0;
//DWORD ContainerConfig::nClassStyle = CS_DBLCLKS | CS_GLOBALCLASS;
DWORD ContainerConfig::nInnerStyle = InnerStyle::TranceContextMenuUp;
DWORD ContainerConfig::nOuterStyle = OuterStyle::Blank;
DWORD ContainerConfig::nOwnerDrawTextStyle = DT_LEFT|DT_VCENTER|DT_SINGLELINE;
COLORREF ContainerConfig::corBackground = ::GetSysColor(COLOR_BTNFACE);
COLORREF ContainerConfig::corTextground = ::GetSysColor(COLOR_BTNTEXT);
COLORREF ContainerConfig::corHotBackColor = ::GetSysColor(COLOR_BTNFACE);
COLORREF ContainerConfig::corHotTextColor = ::GetSysColor(COLOR_BTNTEXT);
int ContainerConfig::nMarginLeft = 3;
int ContainerConfig::nMarginTop = 1;
int ContainerConfig::nMarginRight = 3;
int ContainerConfig::nMarginBottom = 1;
//===========================================================cc::win::Container
///////////////////////////////////////////////////////////////////////////////
// constructors/destructor
Container::Container()
{
	BASE_CONTROL_CONFIG_INIT(Container);
	::SetRect(&_Margin, ContainerConfig::nMarginLeft, ContainerConfig::nMarginTop, ContainerConfig::nMarginRight, ContainerConfig::nMarginBottom);
	_Rect.right = _Rect.left + 160;
	_Rect.bottom = _Rect.top + 72;
	::CopyRect(&_OrgRect, &_Rect);
	_ClassName = WndClassName::Container;
	_StyleArray[(int)StyleType::InitControl] = CC_WNDCOMMCTLS_REG|CC_WNDCOMMCTLSNEW_REG;
	SetText(NULL);

	_lstDispose = NULL;
#ifdef _DEBUG_
	SetName(_T("Container"));
#endif
#if defined(_DEBUG_) || defined(_DESIGN)
	//code for design
	//flag for moveable or others
	Design = (DesignStyle::Type)(DesignStyle::CanAddControl);
#endif
}

Container::~Container()
{
#ifdef _DEBUG_
	if(CC_APP_GetConfig()->GetIsDebug())
	{
		CC_TRACE((_T("Container::~Container(), ClassName:%s, Name:%s, isDisposed:%d, ControlsCount:%d"), _ClassName, _Name, GetIsDispose(), _lstControls == NULL ? 0 : _lstControls->GetSize()));
	}
#endif
	Dispose();
	if(_lstDispose != NULL)
	{
		for(int n = _lstDispose->GetSize() - 1; n >= 0; n--)
		{
			Object* obj = _lstDispose->GetKey(n);
			delete obj;
		}
		delete _lstDispose;
		_lstDispose = NULL;
	}
}
// constructors/destructor

void Container::Dispose()
{
#ifdef _DEBUG_
	if(CC_APP_GetConfig()->GetIsDebug())
	{
		CC_TRACE((_T("Container::Dispose(), ClassName:%s, Name:%s, Text:%s, isDisposed:%d, ControlsCount:%d"), _ClassName, _Name, _Text, GetIsDispose(), _lstControls == NULL ? 0 : _lstControls->GetSize()));
	}
#endif
	if(!GetIsDispose())
	{
		if(_lstDispose != NULL)
		{
			for(int n = _lstDispose->GetSize() - 1; n >= 0; n--)
			{
				Object* obj = _lstDispose->GetKey(n);
				obj->Dispose();
			}
		}
		Control::Dispose();
	}
}

void Container::DisposeItemAdd(Object* needDisposeObj)
{
	if(_lstDispose == NULL)
	{
		//isSort, bIsDESC=false, isDistinct=false, nMinCapacity=0, nIncrement=1.3)
		_lstDispose = new List<Object*>(false, false, true);
	}
	if(!_lstDispose->ContainsKey(needDisposeObj))
	{
		_lstDispose->Add(needDisposeObj);
	}
}

//if not stop even WS_TABSTOP&WS_VISIBLE,need overwrite this
bool Container::IsTabStop()
{
	CC_ASSERT(::IsWindow(m_hWnd));

	int nCnt;
	if(Control::IsTabStop() && _lstControls != NULL && (nCnt = _lstControls->GetSize()) > 0)
	{
		for(int n = 0; n < nCnt; n++)
		{
			Control* ctl = _lstControls->GetKey(n);
			if(ctl->IsTabStop())
			{
				return true;
			}
		}
	}

	if(_pClient != NULL)
	{
		return _pClient->IsTabStop();
	}
	return false;
}

void Container::OnLoad(EventArgs* e)
{
	Load.Invoke((Object*)this, e);
}

LRESULT CALLBACK Container::CC_ContainerProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if(message == WM_INITDIALOG)
	{
		// special case for WM_INITDIALOG
		Container* pDlg = (Container*)Control::FromHandle(hWnd);
		if(pDlg != NULL)
		{
			EventArgs e(hWnd, message, wParam, lParam);
			pDlg->OnInitDialog(&e);
		}
		return 1;
	}
	return 0;
}

void Container::OnInitDialog(EventArgs *e)
{
	DoOnInitDialog(e);
}

void Container::OnHandleCreated(EventArgs* e)
{
	//do nothing for Dialog
	if(!HasStyle(StyleType::Inner, InnerStyle::CreateDialog))
	{
		Control::OnHandleCreated(e);
	}
}

//this is like OnHandleCreated of control
void Container::DoOnInitDialog(EventArgs* e)
{
	if(!HasStyle(StyleType::Inner, InnerStyle::CreateDialog))
	{
		return;
	}
	//_bIsHandleCreated = true;
	//CC_BIT_ON(_Flags, CC_FLAG_CONTROL_HANDLECREATED);
	//SetStyle(StyleType::Inner2, 0, Inner2Style::HandleCreated);
	_StyleArray[(int)StyleType::Inner2] |= Inner2Style::HandleCreated;
	HandleCreated.Invoke((Object*)this, e);

	if(_hFont == NULL)
	{
		_hFont = CreateDefaultFont();
	}
	if(_hFont != NULL)
	{
		SendMessage(m_hWnd, WM_SETFONT, (WPARAM)_hFont, (LPARAM)FALSE);
		//SetWindowText(m_hWnd, _Text);
	}
	if(GetIcon() != NULL)
	{
		//refresh icon(only top-left corner of wnd)
		::SendMessage(m_hWnd, WM_SETICON, true /*bBigIcon*/, (LPARAM)GetIcon());
	}

	if(!GetEnabled())
	{
		this->SetEnabled(false);
	}

	//cs.lpszName
	::SetWindowText(m_hWnd, _Text);
	//cs.hMenu
	if(_hMenu != NULL)
	{
		::SetMenu(m_hWnd, _hMenu);
	}

	OnCreateControl();
	int nFlags = 0;
	if(HasStyle(StyleType::Inner2, Inner2Style::WindowPosFlagsNoAct))
	{
		nFlags |= SWP_NOACTIVATE;
	}
	else if(!HasStyle(StyleType::Inner2, Inner2Style::WindowPosFlags0))
	{
		nFlags |= SWP_NOZORDER | SWP_FRAMECHANGED | SWP_DRAWFRAME;
	}
	::SetWindowPos(m_hWnd, NULL, _Rect.left, _Rect.top, _Rect.right - _Rect.left, _Rect.bottom - _Rect.top, nFlags);

	//to do something special after created child
	_nUpdateFlag |= UpdateFlag::UpdateFlag;
	Update();

	//do next at Form::OnLoad that call from CreateHandle
	//if(_Visible && ((_Style & WS_VISIBLE) == 0))
	//{
	//	::ShowWindow(m_hWnd, SW_SHOW);
	//}
	//_bIsHandleCreatedEnd = true;
	//CC_BIT_ON(_Flags, CC_FLAG_CONTROL_HANDLECREATEDEND);
	//SetStyle(StyleType::Inner2, 0, Inner2Style::HandleCreatedEnd);
	//SetStyle(StyleType::Inner, InnerStyle::HandleCreatedEnd, true);
	_StyleArray[(int)StyleType::Inner2] |= Inner2Style::HandleCreatedEnd;
}

//http://cd.textfiles.com/cica9308/UNZIPPED/NT/DYNDLG/BASE.C
void Container::CreateHandle()
{
	if(!HasStyle(StyleType::Inner, InnerStyle::CreateDialog))
	{
		Control::CreateHandle();
		EventArgs ee(m_hWnd, 0, 0, 0);
		OnLoad(&ee);
	}
	else
	{
		//Only one window per CWin instance
		if(m_hWnd != NULL && ::IsWindow(m_hWnd))
		{
			const TCHAR* msg = _T("Dialog::CreateHandle, Handle is created.");
			cc::Log::fatal(msg);
			throw UserException(msg);
		}

		//update size
		CalcuteRect(true);
		CREATESTRUCT cs;
		cs.dwExStyle = _StyleArray[(int)StyleType::ExStyle];
		cs.lpszClass = _ClassName;
		cs.lpszName = _Text;
		cs.style = _StyleArray[(int)StyleType::Style];
		cs.x = _Rect.left;
		cs.y = _Rect.top;
		cs.cx = _Rect.right - _Rect.left;
		cs.cy = _Rect.bottom - _Rect.top;
		if(_pParent != NULL)
		{
			cs.hwndParent = _pParent->m_hWnd;
		}
		else
		{
			cs.hwndParent = NULL;
		}
		cs.hInstance = CC_APP_GetInstance();
		cs.lpCreateParams = NULL;
		cs.hMenu = _hMenu;
		cs.style &= ~WS_VISIBLE;
		if(cs.hwndParent == NULL)
		{
			//treate as window
			cs.style |= WS_POPUP;
		}

		//allow modification of several common create parameters
		if(!PreCreateWindow(cs))
		{
			PostNcDestroy();
			return;
		}

		if(cs.lpszName != _Text)
		{
			SetText(cs.lpszName);
		}
		_StyleArray[(int)StyleType::Style] = cs.style;
		_hMenu = cs.hMenu;
		HookWindowCreate(this);

		//need space(sizeof(WORD) * 3) for Menu,Class,Title Str of end of ppDlgTemp
		DLGTEMPLATE* pDlgTemp = (DLGTEMPLATE*)malloc(sizeof(DLGTEMPLATE) + sizeof(WORD) * 3);
		memset(pDlgTemp, 0, sizeof(DLGTEMPLATE) + sizeof(WORD) * 3);
		pDlgTemp->style = cs.style;
		pDlgTemp->dwExtendedStyle = cs.dwExStyle;
		pDlgTemp->cdit = 0;
		pDlgTemp->x = cs.x;
		pDlgTemp->y = cs.y;
		pDlgTemp->cx = cs.cx;
		pDlgTemp->cy = cs.cy;

		/*
		DLGTEMPLATE* pDlgTemp = (DLGTEMPLATE*)malloc(sizeof(DLGTEMPLATE) * 2 + sizeof(WORD) * 3 * 2 + 20);
		memset(pDlgTemp, 0, sizeof(DLGTEMPLATE) * 2 + sizeof(WORD) * 3 * 2 + 20);
		WORD* pWordPtr = (WORD*)(pDlgTemp + 1);
		// no menu
		*pWordPtr++ = 0;
		// default dialog box class
		*pWordPtr++ = 0;
		// NUL termination for string
		*pWordPtr++ = 0;

		//for use xp theme,at lease one control,here add Label
		// Fill out the structure and following bytes for the control.
		// DLGITEMTEMPLATE structures should be aligned on DWORD boundaries.
		DLGITEMTEMPLATE* pDialogItem = (DLGITEMTEMPLATE*) (((DWORD_PTR)pWordPtr + 3) & ~3);
		pDialogItem->style = WS_CHILD | WS_VISIBLE;
		pDialogItem->dwExtendedStyle = 0;
		pDialogItem->x  = 0;
		pDialogItem->y  = 0;
		pDialogItem->cx = 0;
		pDialogItem->cy = 0;
		pDialogItem->id = 0;

		// Get a pointer to the WORD after the structure
		pWordPtr = (WORD*) (pDialogItem + 1);
		// Default to "Static" control if no control class or atom is provided
		*pWordPtr++ = 0xFFFF;
		*pWordPtr++ = 0x0082;
		*/

		//20110101 XP,W7:to show underline for buttons with text [&Save]
		bool isAlt = false;
		if((::GetKeyState(VK_MENU) & 0x8000) == 0)
		{
			isAlt = true;
			keybd_event(VK_MENU, MapVirtualKey(VK_MENU, 0), 0, 0); //KEYEVENTF_EXTENDEDKEY
		}
		HWND hWnd = ::CreateDialogIndirect(cs.hInstance, pDlgTemp, cs.hwndParent, (DLGPROC)CC_ContainerProc);
		free(pDlgTemp);
		if(isAlt)
		{
			//if not send some key, then the VK_MENU key will not up!
			keybd_event(VK_F24, MapVirtualKey(VK_F24, 0), 0, 0);
			keybd_event(VK_F24, MapVirtualKey(VK_F24, 0), KEYEVENTF_KEYUP, 0);
			keybd_event(VK_MENU, MapVirtualKey(VK_MENU, 0), KEYEVENTF_KEYUP, 0);
		}

		if(!UnhookWindowCreate())
		{
			PostNcDestroy(); // cleanup if CreateWindowEx fails too soon
		}

		EventArgs ee(m_hWnd, 0, 0, 0);
		OnLoad(&ee);

		if(hWnd == NULL)
		{
			const TCHAR* msg = _T("Dialog::CreateHandle, error to create Window.");
			cc::Log::fatal(msg);
			//throw UserException(msg);
		}
	}
}
///////////////////////////////////////////////////////////////////////////////

DWORD PanelConfig::nStyle = WS_CHILDWINDOW | WS_VISIBLE | WS_TABSTOP;
DWORD PanelConfig::nExStyle = 0;
//DWORD PanelConfig::nClassStyle = CS_DBLCLKS | CS_GLOBALCLASS;
DWORD PanelConfig::nInnerStyle = InnerStyle::TranceContextMenuUp;
//Blank for not draw text
DWORD PanelConfig::nOuterStyle = OuterStyle::Blank;
DWORD PanelConfig::nOwnerDrawTextStyle = DT_LEFT|DT_SINGLELINE;
COLORREF PanelConfig::corBackground = ::GetSysColor(COLOR_BTNFACE);
COLORREF PanelConfig::corTextground = ::GetSysColor(COLOR_BTNTEXT);
COLORREF PanelConfig::corHotBackColor = ::GetSysColor(COLOR_BTNFACE);
COLORREF PanelConfig::corHotTextColor = ::GetSysColor(COLOR_BTNTEXT);
int PanelConfig::nMarginLeft = 0;
int PanelConfig::nMarginTop = 0;
int PanelConfig::nMarginRight = 0;
int PanelConfig::nMarginBottom = 0;
//===========================================================cc::win::Panel
///////////////////////////////////////////////////////////////////////////////
// constructors/destructor
Panel::Panel()
{
	BASE_CONTROL_CONFIG_INIT(Panel);
	::SetRect(&_Margin, PanelConfig::nMarginLeft, PanelConfig::nMarginTop, PanelConfig::nMarginRight, PanelConfig::nMarginBottom);
	_Rect.right = _Rect.left + 160;
	_Rect.bottom = _Rect.top + 72;
	::CopyRect(&_OrgRect, &_Rect);
	//_ClassName = WndClassName::Container;
	//_StyleArray[(int)StyleType::InitControl] = CC_WNDCOMMCTLS_REG;
	SetText(NULL);

#ifdef _DEBUG_
	SetName(_T("Panel"));
#endif
#if defined(_DEBUG_) || defined(_DESIGN)
	//code for design
	//flag for moveable or others
	Design = (DesignStyle::Type)(DesignStyle::CanAddControl);
#endif
}

Panel::~Panel()
{
}
// constructors/destructor

void Panel::OnPaint(PaintEventArgs *e)
{
	DRAWITEMSTRUCT data = {0};
	DrawItemEventArgs ee(e->baseArgs);
	ee.data = &data;
	HDC hDC = BeginPaint(m_hWnd, &(e->data));
	//If GetUpdateRect returns zero, the application should not call the BeginPaint and EndPaint functions.
	if(e->data.rcPaint.bottom != 0 && e->data.rcPaint.right != 0)
	{
		data.hDC = hDC;
		GetClientRect(m_hWnd, &data.rcItem);
		CC_APP_GetDrawActor()->DrawPanel(this, &ee);
	}
	Control::OnPaint(e);
	EndPaint(m_hWnd, &(e->data));
}
///////////////////////////////////////////////////////////////////////////////

DWORD GroupPanelConfig::nStyle = WS_CHILDWINDOW | WS_VISIBLE | WS_TABSTOP;
DWORD GroupPanelConfig::nExStyle = 0;
//DWORD GroupPanelConfig::nClassStyle = CS_DBLCLKS | CS_GLOBALCLASS;
DWORD GroupPanelConfig::nInnerStyle = InnerStyle::TranceContextMenuUp;
DWORD GroupPanelConfig::nOuterStyle = OuterStyle::AlwaysEdge;
DWORD GroupPanelConfig::nOwnerDrawTextStyle = DT_CENTER|DT_SINGLELINE;
COLORREF GroupPanelConfig::corBackground = ::GetSysColor(COLOR_BTNFACE);
COLORREF GroupPanelConfig::corTextground = ::GetSysColor(COLOR_BTNTEXT);
COLORREF GroupPanelConfig::corHotBackColor = ::GetSysColor(COLOR_BTNFACE);
COLORREF GroupPanelConfig::corHotTextColor = ::GetSysColor(COLOR_BTNTEXT);
DWORD GroupPanelConfig::nOuterStyleClient = OuterStyle::Blank;
DWORD GroupPanelConfig::nInnerStyleClient = InnerStyle::TranceContextMenuUp;
COLORREF GroupPanelConfig::corBackgroundClient = GroupPanelConfig::corBackground;
//===========================================================cc::win::GroupPanel
///////////////////////////////////////////////////////////////////////////////
// constructors/destructor
GroupPanel::GroupPanel()
{
	BASE_CONTROL_CONFIG_INIT(GroupPanel);
	_pClient = new Panel();
	_pClient->SetConfigName(_T(""));
	_pClient->SetLocation(3, 11);
	//_pClient->SetOuterStyle((OuterStyle::Type)GroupPanelConfig::nOuterStyleClient, true);
	//_pClient->SetStyle(StyleType::Inner, InnerStyle::TranceContextMenuUp, true);
	_pClient->SetStyle(StyleType::Outer, (DWORD)0xFFFFFFFF, GroupPanelConfig::nOuterStyleClient);
	_pClient->SetStyle(StyleType::Inner, (DWORD)0xFFFFFFFF, GroupPanelConfig::nInnerStyleClient);
	_pClient->SetBackground(GroupPanelConfig::corBackgroundClient);
	//_pClient->SetBackgroundType(GroupPanelConfig::nInnerStyleClient);
#if defined(_DEBUG_) || defined(_DESIGN)
	//code for design
	//flag for moveable or others
	Design = (DesignStyle::Type)(DesignStyle::CanAddControl);
	_pClient->Design = (DesignStyle::Type)DesignStyle::NoDesign;
#endif
	//here AddControl will add to self's _lstControls,the next AddControl will add to _pClient
	Panel::AddControl(_pClient);
	_Rect.right = _Rect.left + 160;
	_Rect.bottom = _Rect.top + 72;
	::CopyRect(&_OrgRect, &_Rect);
	//_ClassName = WndClassName::Container;
	//_StyleArray[(int)StyleType::InitControl] = CC_WNDCOMMCTLS_REG;
	SetText(_T("GroupPanel"));

#ifdef _DEBUG_
	SetName(_T("GroupPanel"));
#endif
}

GroupPanel::~GroupPanel()
{
}
// constructors/destructor

void GroupPanel::OnResize(EventArgs* e)
{
	if(m_hWnd == NULL)
	{
		return;
	}
	InvalidateBackGround();
	RECT rect;
	::GetClientRect(m_hWnd, &rect);
	_pClient->SetBoundsNotOriginal(0, 0, rect.right - 8, rect.bottom - 16, BoundsSpecified::Size);
	Resize.Invoke((Object*)this, e);
}
///////////////////////////////////////////////////////////////////////////////

DWORD ReBarConfig::nStyle = WS_OVERLAPPED | WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_BORDER | RBS_AUTOSIZE | RBS_BANDBORDERS | RBS_DBLCLKTOGGLE | RBS_REGISTERDROP | RBS_VARHEIGHT | CCS_NODIVIDER | CCS_NOPARENTALIGN | CCS_TOP;
DWORD ReBarConfig::nExStyle = WS_EX_TOOLWINDOW | WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR;
//DWORD ReBarConfig::nClassStyle = CS_DBLCLKS | CS_GLOBALCLASS;
DWORD ReBarConfig::nInnerStyle = InnerStyle::DeliverFormMessage;
DWORD ReBarConfig::nOuterStyle = 0;
DWORD ReBarConfig::nOwnerDrawTextStyle = DT_LEFT|DT_VCENTER|DT_SINGLELINE;
COLORREF ReBarConfig::corBackground = ::GetSysColor(COLOR_BTNFACE);
COLORREF ReBarConfig::corTextground = ::GetSysColor(COLOR_BTNTEXT);
COLORREF ReBarConfig::corHotBackColor = ::GetSysColor(COLOR_BTNFACE);
COLORREF ReBarConfig::corHotTextColor = ::GetSysColor(COLOR_BTNTEXT);
//===========================================================cc::win::ReBar
///////////////////////////////////////////////////////////////////////////////
// constructors/destructor
ReBar::ReBar()
{
	BASE_CONTROL_CONFIG_INIT(ReBar);
	_Rect.right = _Rect.left + 160;
	_Rect.bottom = _Rect.top + 72;
	::CopyRect(&_OrgRect, &_Rect);
	_ClassName = WndClassName::ReBar;
	// mask for InitCommonControlsEx
	_StyleArray[(int)StyleType::InitControl] = CC_WNDCOMMCTL_COOL_REG|CC_WNDCOMMCTL_USEREX_REG|CC_WNDCOMMCTL_BAR_REG;
#ifdef _DEBUG_
	SetName(_T("ReBar"));
#endif
	_toolBand = NULL;
#if defined(_DEBUG_) || defined(_DESIGN)
	//code for design
	//flag for moveable or others
	this->Design = (DesignStyle::Type)DesignStyle::NoDesign;
#endif
}

ReBar::~ReBar()
{
}
// constructors/destructor

void ReBar::DoUpdate()
{
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreatedEnd) != 0 &&
		IsUpdate((UpdateFlag::Type)(UpdateFlag::UpdateFlag|UpdateFlag::UpdateChildUpdated)) &&
		!HasStyle(StyleType::Outer, OuterStyle::Sizing))
	{
		UpdateBand();
	}
	//clear flages
	Container::DoUpdate();
}

void ReBar::OnResize(EventArgs* e)
{
	Resize.Invoke((Object*)this, e);
	if(m_hWnd != NULL)
	{
		::InvalidateRect(m_hWnd, NULL, TRUE);
	}
}

void ReBar::UpdateBand()
{
	CC_ASSERT(::IsWindow(m_hWnd));
	CC_ASSERT(_pParent != NULL);

	//SetOuterStyle(OuterStyle::Sizing, true);
	_StyleArray[(int)StyleType::Outer] |= OuterStyle::Sizing;
	ReBarStyle::Type eStyle = (ReBarStyle::Type)::SendMessage(_pParent->m_hWnd, CC_WM_MAINMSG, CC_WM_MAINMSG_WPARAM_GET_REBARSTYLE, 0);
	//int nCount = (int)::SendMessage(m_hWnd, RB_GETBANDCOUNT, 0, 0L);
	int nCount = (int)DefWndProc(RB_GETBANDCOUNT, 0, 0);
	for(int i = 0; i < nCount; i++)
	{
		REBARBANDINFO rbbInfo;
		rbbInfo.cbSize = sizeof(REBARBANDINFO);
		rbbInfo.fMask = RBBIM_STYLE|RBBIM_CHILD|RBBIM_CHILDSIZE|RBBIM_SIZE;
		//::SendMessage(m_hWnd, RB_GETBANDINFO, i, (LPARAM)&rbbInfo);
		DefWndProc(RB_GETBANDINFO, i, (LPARAM)&rbbInfo);
		if(eStyle == ReBarStyle::Fix)
		{
			rbbInfo.fStyle &= ~RBBS_GRIPPERALWAYS;
			rbbInfo.fStyle |= RBBS_NOGRIPPER;
		}
		else
		{
			rbbInfo.fStyle &= ~RBBS_NOGRIPPER;
			rbbInfo.fStyle |= RBBS_GRIPPERALWAYS;
		}

		Control* control = Control::FromHandle(rbbInfo.hwndChild);
		CC_ASSERT(control != NULL);
		if(control->GetVisible())
		{
			rbbInfo.fStyle &= ~RBBS_HIDDEN;
		}
		else
		{
			rbbInfo.fStyle |= RBBS_HIDDEN;
		}
		RECT rect = control->GetBounds();
		if((_StyleArray[(int)StyleType::Style] & CCS_VERT) == 0)
		{
			rbbInfo.cyMinChild = control->GetMinSize().cy;
			if(rbbInfo.cyMinChild <= 0)
			{
				rbbInfo.cyMinChild = rect.bottom - rect.top;
			}
			//rbbInfo.cx = rect.right - rect.left;
		}
		else
		{
			rbbInfo.cyMinChild = control->GetMinSize().cx;
			if(rbbInfo.cyMinChild <= 0)
			{
				rbbInfo.cyMinChild = rect.right - rect.left;
			}
			//rbbInfo.cx = rect.bottom - rect.top;
		}
		//::SendMessage(m_hWnd, RB_SETBANDINFO, i, (LPARAM)&rbbInfo);
		DefWndProc(RB_SETBANDINFO, i, (LPARAM)&rbbInfo);
	}
	//::SendMessage(m_hWnd, WM_SIZE, 0, 0);
	DefWndProc(WM_SIZE, 0, 0);
	//SetOuterStyle(OuterStyle::Sizing, false);
	_StyleArray[(int)StyleType::Outer] &= ~OuterStyle::Sizing;
}

BOOL ReBar::InsertBand(Control* control, int uIndex, bool isFix)
{
	CC_ASSERT(::IsWindow(m_hWnd));
#if defined(_DEBUG_) || defined(_DESIGN)
	//code for design
	//flag for moveable or others
	control->Design = (DesignStyle::Type)((DWORD)control->Design | CC_DESIGN_NO_MOVE);
#endif

	// Add Rebar band
	REBARBANDINFO rbbInfo;
	rbbInfo.cbSize = sizeof(REBARBANDINFO);
	rbbInfo.fMask = RBBIM_STYLE|RBBIM_CHILD|RBBIM_CHILDSIZE|RBBIM_SIZE|RBBIM_ID;
	rbbInfo.fStyle = RBBS_GRIPPERALWAYS|RBBS_CHILDEDGE;
	if(isFix)
	{
		rbbInfo.fStyle &= ~RBBS_GRIPPERALWAYS;
		rbbInfo.fStyle |= RBBS_NOGRIPPER;
	}
	if(control->HasStyle(StyleType::Outer, OuterStyle::Break))
	{
		rbbInfo.fStyle |= RBBS_BREAK;
	}
	rbbInfo.hwndChild = control->m_hWnd;
	rbbInfo.wID = control->TabIndex;
	rbbInfo.cxMinChild = 0; //control->GetMinSize().cx;
	if((_StyleArray[(int)StyleType::Style] & CCS_VERT) == 0)
	{
		control->SetStyle(StyleType::Style, CCS_VERT, 0);
		SIZE size = control->BestSize();
		rbbInfo.cyMinChild = control->GetMinSize().cy;
		if(rbbInfo.cyMinChild <= 0)
		{
			rbbInfo.cyMinChild = size.cy;
		}
		rbbInfo.cx = size.cx + 10;
	}
	else
	{
		control->SetStyle(StyleType::Style, 0, CCS_VERT);
		SIZE size = control->BestSize();
		rbbInfo.cyMinChild = control->GetMinSize().cx;
		if(rbbInfo.cyMinChild <= 0)
		{
			rbbInfo.cyMinChild = size.cx;
		}
		rbbInfo.cx = size.cy + 10;
	}
	//return (BOOL)::SendMessage(m_hWnd, RB_INSERTBAND, uIndex, (LPARAM)&rbbInfo);
	return (BOOL)DefWndProc(RB_INSERTBAND, uIndex, (LPARAM)&rbbInfo);
}

BOOL ReBar::DeleteBand(Control* control)
{
	CC_ASSERT(::IsWindow(m_hWnd));
	CC_ASSERT(control != NULL);

	int uBand;
	if(control->TabIndex > 0)
	{
		uBand = IDToIndex(control->TabIndex);
	}
	else
	{
		uBand = IndexFromHWND(control->m_hWnd);
	}
	if(uBand < 0)
	{
		return FALSE;
	}
	// hide then delete the band:
	::SendMessage(m_hWnd, RB_SHOWBAND, uBand, FALSE);
	::SendMessage(m_hWnd, RB_DELETEBAND, uBand, 0);
	return TRUE;
}

void ReBar::OnCreateControl()
{
	CC_ASSERT(_pParent != NULL);

	Container::OnCreateControl();
	SetBackground(_corBackground);
	if(_lstControls != NULL)
	{
		ReBarStyle::Type eStyle = (ReBarStyle::Type)::SendMessage(_pParent->m_hWnd, CC_WM_MAINMSG, CC_WM_MAINMSG_WPARAM_GET_REBARSTYLE, 0);
		int nCnt = _lstControls->GetSize();
		for(int n = 0; n < nCnt; n++)
		{
			Control* ctl = _lstControls->GetKey(n);
			// Add Rebar band
			if(ctl->TabIndex <= 0)
			{
				ctl->TabIndex = n + 1;
			}
			InsertBand(ctl, -1, eStyle == ReBarStyle::Fix);
		}
	}
}

void ReBar::OnChildNotify(EventArgs *e)
{
	LPNMHDR lpnmhdr = (LPNMHDR)e->LParam;
	switch(lpnmhdr->code)
	{
	case RBN_HEIGHTCHANGE:
		{
			CC_ASSERT(_pParent != NULL);
			//let parent form to resize and redraw
			::SendMessage(_pParent->m_hWnd, CC_WM_CHILD_UPDATED, (WPARAM)this, 0);
		}
		break;
	case RBN_BEGINDRAG:
		{
			OnBeginDrag(e);
		}
		break;
	case RBN_ENDDRAG:
		{
			OnEndDrag(e);
		}
		break;
	}
}

void ReBar::OnControlAdded(ControlEventArgs* e)
{
	Container::OnControlAdded(e);

	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
	{
		CC_ASSERT(_pParent != NULL);

		ReBarStyle::Type eStyle = (ReBarStyle::Type)::SendMessage(_pParent->m_hWnd, CC_WM_MAINMSG, CC_WM_MAINMSG_WPARAM_GET_REBARSTYLE, 0);
		InsertBand(e->control, -1, eStyle == ReBarStyle::Fix);
	}
}

int ReBar::IndexFromHWND(HWND hwnd)
{
	REBARBANDINFO rbi;
	memset(&rbi, 0, sizeof(rbi));
	rbi.cbSize = sizeof(rbi);
	rbi.fMask  = RBBIM_CHILD;

	int cCnt = (int)::SendMessage(m_hWnd, RB_GETBANDCOUNT, 0, 0);
	for(int i = 0; i < cCnt ; i++)
	{
		::SendMessage(m_hWnd, RB_GETBANDINFO, i, (LPARAM)&rbi);
		if(rbi.hwndChild == hwnd)
		{
			return i;
		}
	}

	return -1;
}

void ReBar::OnBeginDrag(EventArgs *e)
{
	CC_ASSERT(_pParent != NULL);

	ReBarStyle::Type eStyle = (ReBarStyle::Type)::SendMessage(_pParent->m_hWnd, CC_WM_MAINMSG, CC_WM_MAINMSG_WPARAM_GET_REBARSTYLE, 0);
	if(eStyle != ReBarStyle::Floatable)
	{
		return;
	}
	NMREBAR* pnmrb = reinterpret_cast<NMREBAR*>(e->LParam);

	// cache info about the band that is being dragged:
	REBARBANDINFO rbi;
	memset(&rbi, 0, sizeof(rbi));
	rbi.cbSize = sizeof(rbi);
	rbi.fMask  = RBBIM_CHILD;
	::SendMessage(m_hWnd, RB_GETBANDINFO, pnmrb->uBand, (LPARAM)&rbi);
	_toolBand = (ToolBar*)Control::FromHandle(rbi.hwndChild);
	//activate while from _pFloater
	::SetActiveWindow(_pParent->m_hWnd);
}

void ReBar::OnEndDrag(EventArgs *e)
{
	if(_toolBand != NULL)
	{
		int nCnt = _toolBand->GetControlCount();
		for(int n = 0; n < nCnt; n++)
		{
			Control* ctl = _toolBand->GetControl(n);
			::SendMessage(ctl->m_hWnd, CC_WM_MAINMSG, CC_WM_MAINMSG_WPARAM_CLEARBRUSHPARENT, 0);
		}
	}
	_toolBand = NULL;
}

void ReBar::OnMouseMove(MouseEventArgs *e)
{
	// default:
	Container::OnMouseMove(e);

	// ensure dragging:
	if(_toolBand == NULL || m_hWnd != ::GetCapture())
	{
		return;
	}

	// are we going to tear off?
	CC_ASSERT(_pParent != NULL);
	POINT pt;
	RECT rect;
	::GetCursorPos(&pt);
	::GetWindowRect(m_hWnd, &rect);
	::InflateRect(&rect, 10, 10);
	if(::PtInRect(&rect, pt))
	{
		return;
	}
	// remove _toolBand,for _toolBand is set to NULL while delete it,so save it to item
	ToolBar* item = _toolBand;
	//can floatable?
	if((item->GetDockable() & DockStyles::None) == 0)
	{
		//is not floatable,find another rebar
		FindReBarEntity entity;
		entity.pt = pt;
		entity.pToolBar = item;
		entity.nInsertIndex = -1;
		entity.bHorizontal = true;
		entity.eDockable = item->GetDockable();
		ReBar* pReBar = (ReBar*)::SendMessage(_pParent->m_hWnd, CC_WM_MAINMSG, CC_WM_MAINMSG_WPARAM_GET_REBAR, (LPARAM)&entity);
		if(pReBar == NULL || pReBar == this)
		{
			return;
		}
		//e->Result = DefWndProc(e);
		e->baseArgs->Handled = true;
		// we're done tracking...
		::ReleaseCapture();

		// remove item
		item->SetVisible(false);
		item->SetBoundsNotOriginal(-1, -1, 0, 0);
		// hide then delete the band:
		DeleteBand(item);

		//reset item to another rebar
		::SetParent(item->m_hWnd, pReBar->m_hWnd);
		::SendMessage(item->m_hWnd, CC_WM_MAINMSG, CC_WM_MAINMSG_WPARAM_CLEARBRUSHPARENT, 0);
		//resize self while SetStyle
		if(entity.bHorizontal)
		{
			item->SetStyle(StyleType::Style, CCS_VERT, 0);
		}
		else
		{
			item->SetStyle(StyleType::Style, 0, CCS_VERT);
		}
		//this->CalcFixedLayout(0, bHorizontal);
		item->SetVisible(true);
		pReBar->InsertBand(item, entity.nInsertIndex, false);

		RECT rect = item->GetBounds();
		int nPos = entity.bHorizontal ? 5 : -5;
		::PostMessage(pReBar->m_hWnd, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(rect.left - nPos, rect.top + nPos));
		return;
	}

	// we're done tracking...
	::ReleaseCapture();

	item->SetVisible(false);
	item->SetBoundsNotOriginal(-1, -1, 0, 0);
	// hide then delete the band:
	DeleteBand(item);

	//let parent form to resize and redraw
	::SendMessage(_pParent->m_hWnd, CC_WM_CHILD_UPDATED, (WPARAM)this, 0);
	item->GetFloatContainer()->SetLocation(pt.x, pt.y);
	item->SetFloating(true);
	item->SetVisible(true);
	::SetCursor(::LoadCursor(NULL, IDC_ARROW));

	// user continues to drag the new parent-frame:
	::SendMessage(item->GetFloatContainer()->m_hWnd, WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(pt.x, pt.y));
	_toolBand = NULL;
}

//keyArr is param address,but sHead is by value
void ReBar::DoConfig(bool isFromIni, KeyArr& keyArr, cc::Str sThisHead)
{
	if(!HasStyle(StyleType::Outer, OuterStyle::AutoConfig))
	{
		return;
	}
	CC_ASSERT(::IsWindow(m_hWnd));

	//int itemCount = (int)::SendMessage(m_hWnd, RB_GETBANDCOUNT, 0, 0);
	int itemCount = (int)DefWndProc(RB_GETBANDCOUNT, 0, 0);
	if(isFromIni)
	{
		//if not exist,as usually
		if(keyArr.Exist(sThisHead))
		{
			//sThisHead is count
			int cfgCount = keyArr.Get(sThisHead, 0);
			if(cfgCount == itemCount)
			{
				for(int i = 0; i < itemCount; i++)
				{
					cc::Str stri = sThisHead + i;
					int nID = keyArr.Get(stri + _T(".ID"), 0);
					if(nID <= 0)
					{
						continue;
					}
					//int nPos = (int)::SendMessage(m_hWnd, RB_IDTOINDEX, nID, 0L);
					int nPos = (int)DefWndProc(RB_IDTOINDEX, nID, 0);
					//::SendMessage(m_hWnd, RB_MOVEBAND, nPos, i);
					DefWndProc(RB_MOVEBAND, nPos, i);
					REBARBANDINFO rbi;
					rbi.cbSize = sizeof(rbi);
					rbi.fMask = RBBIM_ID | RBBIM_SIZE | RBBIM_STYLE;
					//::SendMessage(m_hWnd, RB_GETBANDINFO, i, (LPARAM)&rbi);
					DefWndProc(RB_GETBANDINFO, i, (LPARAM)&rbi);

					rbi.cx = keyArr.Get(stri + _T(".cx"), rbi.cx);
					if(keyArr.GetBool(stri + _T(".Break"), ((rbi.fStyle & RBBS_BREAK) != 0)))
					{
						rbi.fStyle |= RBBS_BREAK;
					}
					else
					{
						rbi.fStyle &= ~RBBS_BREAK;
					}
					if(keyArr.GetBool(stri + _T(".Visible"), ((rbi.fStyle & RBBS_HIDDEN) != 0)))
					{
						rbi.fStyle |= RBBS_HIDDEN;
					}
					else
					{
						rbi.fStyle &= ~RBBS_HIDDEN;
					}
					//::SendMessage(m_hWnd, RB_SETBANDINFO, i, (LPARAM)&rbi);
					DefWndProc(RB_SETBANDINFO, i, (LPARAM)&rbi);
				}
			}
		}
	}
	else
	{
		//sThisHead is count
		keyArr.Set(sThisHead, itemCount);

		for(int i = 0; i < itemCount; i++)
		{
			cc::Str stri = sThisHead + i;
			REBARBANDINFO rbi;
			rbi.cbSize = sizeof(rbi);
			rbi.fMask = RBBIM_ID | RBBIM_SIZE | RBBIM_STYLE;
			//::SendMessage(m_hWnd, RB_GETBANDINFO, i, (LPARAM)&rbi);
			DefWndProc(RB_GETBANDINFO, i, (LPARAM)&rbi);
			keyArr.Set(stri + _T(".ID"), rbi.wID);
			keyArr.Set(stri + _T(".cx"), rbi.cx);
			keyArr.SetBool(stri + _T(".Break"), ((rbi.fStyle & RBBS_BREAK) != 0));
			keyArr.SetBool(stri + _T(".Visible"), ((rbi.fStyle & RBBS_HIDDEN) != 0));
		}
	}
}

UINT ReBar::GetBandCount() const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (UINT) ::SendMessage(m_hWnd, RB_GETBANDCOUNT, 0, 0L); }
BOOL ReBar::GetBandInfo(UINT uBand, REBARBANDINFO* prbbi) const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, RB_GETBANDINFO, uBand, (LPARAM)prbbi); }
UINT ReBar::GetBarHeight() const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (UINT) ::SendMessage(m_hWnd, RB_GETBARHEIGHT, 0, 0L); }
BOOL ReBar::GetBarInfo(REBARINFO* prbi) const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, RB_GETBARINFO, 0, (LPARAM)prbi); }
COLORREF ReBar::GetBackground() const
{
	//CC_ASSERT(::IsWindow(m_hWnd));
	if(m_hWnd != NULL)
	{
		return (COLORREF)::SendMessage(m_hWnd, RB_GETBKCOLOR, 0, 0L);
	}
	return _corBackground;
}
//IDropTarget* ReBar::GetDropTarget() const
//	{ CC_ASSERT(::IsWindow(m_hWnd)); IDropTarget* pdt; ::SendMessage(m_hWnd, RB_GETDROPTARGET, 0, (LPARAM)&pdt); return pdt; }
BOOL ReBar::GetRect(UINT uBand, LPRECT prc) const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, RB_GETRECT, uBand, (LPARAM)prc); }
UINT ReBar::GetRowCount() const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (UINT) ::SendMessage(m_hWnd, RB_GETROWCOUNT, 0, 0L); }
UINT ReBar::GetRowHeight(UINT uRow) const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (UINT) ::SendMessage(m_hWnd, RB_GETROWHEIGHT, uRow, 0L); }
COLORREF ReBar::GetTextColor() const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (COLORREF) ::SendMessage(m_hWnd, RB_GETTEXTCOLOR, 0, 0L); }
//CToolTipCtrl* ReBar::GetToolTips() const
//	{ CC_ASSERT(::IsWindow(m_hWnd)); return (CToolTipCtrl*)CWnd::FromHandle((HWND)::SendMessage(m_hWnd, RB_GETTOOLTIPS, 0, 0L)); }
int ReBar::IDToIndex(UINT uBandID) const
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (int) ::SendMessage(m_hWnd, RB_IDTOINDEX, uBandID, 0L); }
BOOL ReBar::SetBandInfo(UINT uBand, REBARBANDINFO* prbbi)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, RB_SETBANDINFO, uBand, (LPARAM)prbbi); }
BOOL ReBar::SetBarInfo(REBARINFO* prbi)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, RB_SETBARINFO, 0, (LPARAM)prbi); }
//COLORREF ReBar::SetBkColor(COLORREF clr)
//	{ CC_ASSERT(::IsWindow(m_hWnd)); return (COLORREF) ::SendMessage(m_hWnd, RB_SETBKCOLOR, 0, (LPARAM)clr); }
void ReBar::SetBackground(COLORREF corBackground)
{
	::SendMessage(m_hWnd, RB_SETBKCOLOR, 0, (LPARAM)corBackground);
	Control::SetBackground(corBackground);
}
Control* ReBar::SetOwner(Control* pWnd)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return Control::FromHandle((HWND)::SendMessage(m_hWnd, RB_SETPARENT, (WPARAM)pWnd->m_hWnd, 0L)); }
COLORREF ReBar::SetTextColor(COLORREF clr)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (COLORREF) ::SendMessage(m_hWnd, RB_SETTEXTCOLOR, 0, (LPARAM)clr); }
//void ReBar::SetToolTips(CToolTipCtrl* pTip)
//	{ CC_ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, RB_SETTOOLTIPS, (WPARAM)pTip->m_hWnd, 0L); }
void ReBar::BeginDrag(UINT uBand, DWORD dwPos)
	{ CC_ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, RB_BEGINDRAG, uBand, dwPos); }
BOOL ReBar::DeleteBand(UINT uBand)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, RB_DELETEBAND, uBand, 0L); }
void ReBar::DragMove(DWORD dwPos)
	{ CC_ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, RB_DRAGMOVE, 0, dwPos); }
void ReBar::EndDrag()
	{ CC_ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, RB_ENDDRAG, 0, 0L); }
int ReBar::HitTest(RBHITTESTINFO* prbht)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (int) ::SendMessage(m_hWnd, RB_HITTEST, 0, (LPARAM)prbht); }
BOOL ReBar::InsertBand(UINT uIndex, REBARBANDINFO* prbbi)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, RB_INSERTBAND, uIndex, (LPARAM)prbbi); }
void ReBar::MaximizeBand(UINT uBand)
	{ CC_ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, RB_MAXIMIZEBAND, uBand, 0L); }
void ReBar::MinimizeBand(UINT uBand)
	{ CC_ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, RB_MINIMIZEBAND, uBand, 0L); }
void ReBar::RestoreBand(UINT uBand)
	{ CC_ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, RB_MAXIMIZEBAND, uBand, 1L); }
BOOL ReBar::SizeToRect(RECT& rect)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, RB_SIZETORECT, 0, (LPARAM)&rect); }
BOOL ReBar::ShowBand(UINT uBand, BOOL fShow /*= TRUE*/)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, RB_SHOWBAND, uBand, fShow); }
void ReBar::GetBandBorders(UINT uBand, LPRECT prc) const
	{ CC_ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, RB_GETBANDBORDERS, uBand, (LPARAM)prc); }
//CPalette* ReBar::GetPalette() const
//	{ CC_ASSERT(::IsWindow(m_hWnd)); return CPalette::FromHandle((HPALETTE)::SendMessage(m_hWnd, RB_GETPALETTE, 0, 0L)); }
//CPalette* ReBar::SetPalette(HPALETTE hPal)
//	{ CC_ASSERT(::IsWindow(m_hWnd)); return CPalette::FromHandle((HPALETTE)::SendMessage(m_hWnd, RB_SETPALETTE, 0, (LPARAM)hPal)); }
BOOL ReBar::MoveBand(UINT uFrom, UINT uTo)
	{ CC_ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, RB_MOVEBAND, uFrom, uTo); }

#if _WIN32_IE >= 0x0500
void ReBar::PushChevron(UINT uBand, LPARAM lAppValue)
	{ CC_ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, RB_PUSHCHEVRON, uBand, lAppValue); }
#endif
///////////////////////////////////////////////////////////////////////////////

DWORD FloatContainerConfig::nStyle = WS_OVERLAPPED | WS_POPUP | WS_CLIPSIBLINGS | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME;
DWORD FloatContainerConfig::nExStyle = WS_EX_TOOLWINDOW | WS_EX_WINDOWEDGE | WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR;
//DWORD FloatContainerConfig::nClassStyle = CS_DBLCLKS | CS_GLOBALCLASS;
DWORD FloatContainerConfig::nInnerStyle = InnerStyle::TranceContextMenuUp;
DWORD FloatContainerConfig::nOuterStyle = 0;
DWORD FloatContainerConfig::nOwnerDrawTextStyle = DT_LEFT|DT_VCENTER|DT_SINGLELINE;
COLORREF FloatContainerConfig::corBackground = ::GetSysColor(COLOR_BTNFACE);
COLORREF FloatContainerConfig::corTextground = ::GetSysColor(COLOR_BTNTEXT);
COLORREF FloatContainerConfig::corHotBackColor = ::GetSysColor(COLOR_BTNFACE);
COLORREF FloatContainerConfig::corHotTextColor = ::GetSysColor(COLOR_BTNTEXT);
//===========================================================cc::win::FloatContainer
///////////////////////////////////////////////////////////////////////////////
// constructors/destructor
FloatContainer::FloatContainer()
{
	BASE_CONTROL_CONFIG_INIT(FloatContainer);
	::SetRect(&_Rect, 0, 0, 100, 31);
	::CopyRect(&_OrgRect, &_Rect);
	_ClassName = WndClassName::FloatContainer;
	// mask for InitCommonControlsEx
	_StyleArray[(int)StyleType::InitControl] = 0;
#ifdef _DEBUG_
	SetText(_T("FloatContainer"));
	SetName(_T("FloatContainer"));
#endif
	//_Visible = false;
	//CC_BIT_OFF(_Flags, CC_FLAG_CONTROL_VISIBLE);
	SetStyle(StyleType::Inner2, Inner2Style::Visible, 0);
	_maxSize.cx = 0;
	_maxSize.cy = 0;
}

FloatContainer::~FloatContainer()
{
}
// constructors/destructor

bool FloatContainer::GetCloseButtonEnable()
{
	return !HasStyle(StyleType::Inner, InnerStyle::CloseButtonDisable);
}

void FloatContainer::SetCloseButtonEnable(bool enable)
{
	SetStyleBool(StyleType::Inner, InnerStyle::CloseButtonDisable, !enable);
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
	{
		//modifying container's system menu.
		HMENU sysMenu = ::GetSystemMenu(m_hWnd, FALSE);
		if(sysMenu != NULL)
		{
			::EnableMenuItem(sysMenu, SC_CLOSE, enable ? MF_BYCOMMAND | MF_ENABLED : MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		}
	}
}

void FloatContainer::SetMaxSize(int maxWidth, int maxHeight)
{
	_maxSize.cx = maxWidth;
	_maxSize.cy = maxHeight;
}

void FloatContainer::SetSizeBaseOnClient(int cxWidth, int cyHeight)
{
	RECT rect;
	rect.left = 0;
	rect.right = cxWidth;
	rect.top = 0;
	rect.bottom = cyHeight;
	SetClientSize(cxWidth, cyHeight);
	::AdjustWindowRectEx(&rect, GetStyle(StyleType::Style), (_hMenu != NULL), GetStyle(StyleType::ExStyle));
	SetSize(rect.right - rect.left, rect.bottom - rect.top);
}

LRESULT FloatContainer::WndProc(EventArgs *e)
{
	switch(e->Msg)
	{
	case WM_CLOSE:
		{
			SetVisible(false);
			Closed.Invoke((Object*)this, e);
			return 0;
		}
		break;
	case WM_ACTIVATE:
		{
			Control* topForm = (Control*)::SendMessage(m_hWnd, CC_WM_MAINMSG, CC_WM_MAINMSG_WPARAM_GETFORM, 0);
			if(topForm == NULL || ::SendMessage(topForm->m_hWnd, CC_WM_MAINMSG, CC_WM_MAINMSG_WPARAM_ACTIVATE, (LPARAM)e) == 0)
			{
				::SendMessage(CC_APP_GetActiveFormHwnd(), CC_WM_MAINMSG, CC_WM_MAINMSG_WPARAM_ACTIVATE, (LPARAM)e);
			}
			//HWND hwnd = m_hWnd;
			//if(_pParent != NULL)
			//{
			//	hwnd = _pParent->m_hWnd;
			//}
			//HWND hwnd2;
			//while((hwnd2 = ::GetParent(hwnd)) != NULL)
			//{
			//	hwnd = hwnd2;
			//}
			//if(::SendMessage(hwnd, CC_WM_MAINMSG, CC_WM_MAINMSG_WPARAM_ACTIVATE, (LPARAM)e) == 0)
			//{
			//	::SendMessage(CC_APP_GetActiveFormHwnd(), CC_WM_MAINMSG, CC_WM_MAINMSG_WPARAM_ACTIVATE, (LPARAM)e);
			//}
			if(LOWORD(e->WParam) == WA_INACTIVE)
			{
				Deactivate.Invoke((Object*)this, e);
			}
			else
			{
				Activated.Invoke((Object*)this, e);
			}
		}
		break;
	case WM_GETMINMAXINFO:
		{
			MINMAXINFO* pmmi = (MINMAXINFO*)e->LParam;
			pmmi->ptMinTrackSize.x = _minSize.cx;
			pmmi->ptMinTrackSize.y = _minSize.cy;
			if(_maxSize.cx > 0)
			{
				pmmi->ptMaxTrackSize.x = CC_MAX(_minSize.cx, _maxSize.cx);
			}
			if(_maxSize.cy > 0)
			{
				pmmi->ptMaxTrackSize.y = CC_MAX(_minSize.cy, _maxSize.cy);
			}
		}
	}
	return Container::WndProc(e);
}

void FloatContainer::OnCreateControl()
{
	Container::OnCreateControl();
	if(HasStyle(StyleType::Inner, InnerStyle::CloseButtonDisable))
	{
		SetCloseButtonEnable(false);
	}
	HMENU sysMenu = ::GetSystemMenu(m_hWnd, FALSE);
	if(sysMenu != NULL)
	{
		::EnableMenuItem(sysMenu, SC_MAXIMIZE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		::EnableMenuItem(sysMenu, SC_MINIMIZE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		//::EnableMenuItem(sysMenu, SC_SIZE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	}
}
//cc::win::FloatContainer
///////////////////////////////////////////////////////////////////////////////

DWORD DockBarConfig::nStyle = WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS;
DWORD DockBarConfig::nExStyle = 0;
//DWORD DockBarConfig::nClassStyle = CS_DBLCLKS | CS_GLOBALCLASS;
DWORD DockBarConfig::nInnerStyle = InnerStyle::TranceContextMenuUp|InnerStyle::TrackMouse;
DWORD DockBarConfig::nOuterStyle = 0;
DWORD DockBarConfig::nOwnerDrawTextStyle = DT_LEFT|DT_VCENTER|DT_SINGLELINE;
COLORREF DockBarConfig::corBackground = ::GetSysColor(COLOR_BTNFACE);
COLORREF DockBarConfig::corTextground = ::GetSysColor(COLOR_BTNTEXT);
COLORREF DockBarConfig::corHotBackColor = ::GetSysColor(COLOR_BTNFACE);
COLORREF DockBarConfig::corHotTextColor = ::GetSysColor(COLOR_BTNTEXT);
//===========================================================cc::win::DockBar
///////////////////////////////////////////////////////////////////////////////
// constructors/destructor
DockBar::DockBar()
{
	BASE_CONTROL_CONFIG_INIT(DockBar);
	::SetRect(&_Rect, 0, 0, 100, 26);
	::CopyRect(&_OrgRect, &_Rect);
	_ClassName = WndClassName::DockBar;
	_StyleArray[(int)StyleType::InitControl] = CC_WNDCOMMCTL_BAR_REG;
	Dock = DockStyles::Top;
#ifdef _DEBUG_
	SetText(_T("DockBar"));
	SetName(_T("DockBar"));
#endif
	_eInitDockPosition = DockStyles::Top;
	_eDockable = (DockStyles::Type)(DockStyles::Left|DockStyles::Top|DockStyles::Right|DockStyles::Bottom|DockStyles::None|DockStyles::Fill);
	_pFloater = NULL;
	CC_BIT_SET(_Flags, CC_FLAG_DOCKBAR_ISMOVEFORMONCLICK, true);
	CC_BIT_SET(_Flags, CC_FLAG_DOCKBAR_ISFLOATRESIZEABLE, true);
#if defined(_DEBUG_) || defined(_DESIGN)
	//code for design
	//flag for moveable or others
	this->Design = (DesignStyle::Type)(CC_DESIGN_NO_MOVE | CC_DESIGN_NO_COLOR | DesignStyle::CanAddControl | DesignStyle::NoAnchor);
#endif
}

DockBar::~DockBar()
{
	if(_pFloater != NULL)
	{
		delete _pFloater;
		_pFloater = NULL;
	}
}
// constructors/destructor

void DockBar::Dispose()
{
	if(_pFloater != NULL)
	{
		_pFloater->Dispose();
	}
	Container::Dispose();
}

FloatContainer* DockBar::GetFloatContainer()
{
	if(_pFloater == NULL)
	{
		_pFloater = new FloatContainer();
		if(!CC_BIT_HAS(_Flags, CC_FLAG_DOCKBAR_ISFLOATRESIZEABLE))
		{
			_pFloater->SetStyle(StyleType::Style, WS_THICKFRAME, 0);
		}
		_pFloater->Moving += EventHandler((Object*)this, (EventFun)&DockBar::Floater_OnMoving);
	}
	if(_pFloater->m_hWnd == NULL && (_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
	{
		_pFloater->SetParent(this->_pParent, true);
		_pFloater->CreateControl();
	}
	return _pFloater;
}

DockStyles::Type DockBar::GetInitDockPosition() const
{
	return _eInitDockPosition;
}

void DockBar::SetInitDockPosition(DockStyles::Type value)
{
	_eInitDockPosition = value;
}

DockStyles::Type DockBar::GetDockable() const
{
	return _eDockable;
}

//can be set like:(DockStyles::Type)DockStyles::Left|DockStyles::Top|DockStyles::Right|DockStyles::Bottom|DockStyles::None|DockStyles::Fill;
void DockBar::SetDockable(DockStyles::Type value)
{
	_eDockable = value;
}

bool DockBar::GetFloating()
{
	return HasStyle(StyleType::Outer, OuterStyle::Floating);
}

void DockBar::SetFloating(bool value, bool isShowFloater, int left, int top)
{
	_pFloater = GetFloatContainer();
	_pFloater->SetLocation(left, top);
	SetFloating(value, isShowFloater);
}

void DockBar::SetFloating(bool value, bool isShowFloater)
{
	//SetOuterStyle(OuterStyle::Floating, value);
	if(value != GetFloating())
	{
		SetStyleBool(StyleType::Outer, OuterStyle::Floating, value);
		if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
		{
			_pFloater = GetFloatContainer();
			if(value)
			{
				::SetParent(m_hWnd, _pFloater->m_hWnd);
				this->_pParentBrush = NULL;
				SIZE size = BestSize();
//err(_T("DockBar::SetFloating,text:%s, left:%d, top:%d, right-left:%d, bottom-top:%d"), _Text, rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top);
				//calculate parent size
				RECT rect = {0, 0, size.cx, size.cy};
				::AdjustWindowRectEx(&rect, _pFloater->GetStyle(StyleType::Style),
					FALSE, _pFloater->GetStyle(StyleType::ExStyle)); 
#ifdef _DEBUG_
				if(_pFloater->GetName() == NULL || _tcscmp(_pFloater->GetName(), _T("FloatContainer")) == 0)
				{
					_pFloater->SetName(GetName());
				}
#endif
				_pFloater->SetText(GetText());
				_pFloater->SetSize(rect.right - rect.left, rect.bottom - rect.top - 2);
				SetLocation(0, 0);
				_pFloater->SetCloseButtonEnable(!HasStyle(StyleType::Inner, InnerStyle::CloseButtonDisable));
				//should be true,for use as flag at Form::Load
				SetVisible(true);
				_pFloater->SetVisible(isShowFloater);
			}
			else
			{
				_pFloater->SetVisible(false);
				if(_pParent != NULL)
				{
					//if has _pParent,then remove to parent
					::SetParent(m_hWnd, _pParent->m_hWnd);
				}
				this->_pParentBrush = NULL;
			}
		}
	}
	else
	{
		if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
		{
			_pFloater = GetFloatContainer();
			SetVisible(true);
			_pFloater->SetVisible(isShowFloater);
		}
	}
}

bool DockBar::GetCloseButtonEnable()
{
	return !HasStyle(StyleType::Inner, InnerStyle::CloseButtonDisable);
}

void DockBar::SetCloseButtonEnable(bool enable)
{
	if(enable != GetCloseButtonEnable())
	{
		SetStyleBool(StyleType::Inner, InnerStyle::CloseButtonDisable, !enable);
		if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
		{
			_pFloater = GetFloatContainer();
			_pFloater->SetCloseButtonEnable(enable);
		}
	}
}

//void DockBar::OnVisibleChanged(EventArgs *e)
//{
//	Container::OnVisibleChanged(e);
//	if(GetFloating())
//	{
//		CC_ASSERT(_pFloater != NULL);
//		_pFloater->SetVisible(_Visible);
//	}
//}
//

//2008/12/11 add for update toolitem by toolbar via SetMinSize
RECT DockBar::CalcuteRect(bool updateRect, int nExpectX, int nExpectY, int nExpectCX, int nExpectCY, bool isFourceCalcute)
{
	RECT rect = Container::CalcuteRect(updateRect, nExpectX, nExpectY, nExpectCX, nExpectCY, isFourceCalcute);
	Update(UpdateFlag::UpdateFlag);
	return rect;
}

void DockBar::OnResize(EventArgs* e)
{
	Container::OnResize(e);
	if(m_hWnd != NULL)
	{
		::InvalidateRect(m_hWnd, NULL, TRUE);
	}
}

void DockBar::OnMouseDown(MouseEventArgs* e)
{
	Container::OnMouseDown(e);
	if(CC_BIT_HAS(_Flags, CC_FLAG_DOCKBAR_ISMOVEFORMONCLICK) && GetFloating() && e->Button == MouseButtons::Left)
	{
		//e->baseArgs->Result = DefWndProc(e->baseArgs);
		e->baseArgs->Handled = true;
		::ReleaseCapture();
		::SendMessage(::GetParent(m_hWnd), WM_NCLBUTTONDOWN, HTCAPTION, NULL);
	}
}

void DockBar::Floater_OnMoving(Object* sender, EventArgs* e)
{
}
//cc::win::DockBar
///////////////////////////////////////////////////////////////////////////////

DWORD ToolBarConfig::nStyle = WS_OVERLAPPED | WS_CHILD | WS_VISIBLE | TBSTYLE_FLAT | TBSTYLE_TRANSPARENT | CCS_NODIVIDER | CCS_NORESIZE | CCS_TOP;
DWORD ToolBarConfig::nExStyle = WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR;
//DWORD ToolBarConfig::nClassStyle = CS_DBLCLKS | CS_GLOBALCLASS;
DWORD ToolBarConfig::nInnerStyle = InnerStyle::TranceContextMenuUp|InnerStyle::DeliverFormMessage;
DWORD ToolBarConfig::nOuterStyle = 0;
DWORD ToolBarConfig::nOwnerDrawTextStyle = DT_LEFT|DT_VCENTER|DT_SINGLELINE;
COLORREF ToolBarConfig::corBackground = ::GetSysColor(COLOR_BTNFACE);
COLORREF ToolBarConfig::corTextground = ::GetSysColor(COLOR_BTNTEXT);
COLORREF ToolBarConfig::corHotBackColor = ::GetSysColor(COLOR_BTNFACE);
COLORREF ToolBarConfig::corHotTextColor = ::GetSysColor(COLOR_BTNTEXT);
//http://www.codeproject.com/docking/dockwnd.asp
//===========================================================cc::win::ToolBar
///////////////////////////////////////////////////////////////////////////////
// constructors/destructor
ToolBar::ToolBar()
{
	BASE_CONTROL_CONFIG_INIT(ToolBar);
	//::SetRect(&_Rect, 0, 0, 100, 31);
	//::CopyRect(&_OrgRect, &_Rect);
	//should define _minSize.cy,if cx <= 0,then calcuit.also,should use as GetMinSize().cx
	_minSize.cx = 0;
	_minSize.cy = 33;
	//_ClassName = TOOLBARCLASSNAME;
	_ClassName = WndClassName::ToolBar;
	// mask for InitCommonControlsEx
	_StyleArray[(int)StyleType::InitControl] = CC_WNDCOMMCTL_COOL_REG|CC_WNDCOMMCTL_USEREX_REG|CC_WNDCOMMCTL_BAR_REG;
#ifdef _DEBUG_
	SetName(_T("ToolBar"));
#endif
	CC_BIT_SET(_Flags, CC_FLAG_DOCKBAR_ISFLOATRESIZEABLE, false);
#if defined(_DEBUG_) || defined(_DESIGN)
	//code for design
	//flag for moveable or others
	this->Design = (DesignStyle::Type)(CC_DESIGN_NO_MOVE | CC_DESIGN_NO_COLOR | DesignStyle::CanAddControl | DesignStyle::NoAnchor);
#endif
}

ToolBar::~ToolBar()
{
}
// constructors/destructor

//even not isUpdate,fource update,called at ReBar::InsertBand
SIZE ToolBar::BestSize(bool isUpdate)
{
	_nUpdateFlag |= UpdateFlag::UpdateFlag;
	Update();
	RECT rect = GetBounds();
	SIZE size = {rect.right - rect.left, rect.bottom - rect.top};
	return size;
}

bool ToolBar::GetBreak()
{
	return HasStyle(StyleType::Outer, OuterStyle::Break);
}

void ToolBar::SetBreak(bool value)
{
	//SetOuterStyle(OuterStyle::Break, value, true);
	SetStyleBool(StyleType::Outer, OuterStyle::Break, value, SWP_FRAMECHANGED|SWP_DRAWFRAME);
}

SIZE ToolBar::GetMinSize()
{
	CC_ASSERT(_minSize.cy >= 0);
	if(_minSize.cx > 0 || _lstControls == NULL)
	{
		return _minSize;
	}
	SIZE sz;
	sz.cy = _minSize.cy;
	sz.cx = 0;
	RECT rect;
	int nCnt = _lstControls->GetSize();
	for(int n = 0; n < nCnt; n++)
	{
		Control* ctl = _lstControls->GetKey(n);
		if(ctl->GetVisible())
		{
			rect = ctl->CalcuteRect(false, -1, -1, -1, _minSize.cy - 5);
			sz.cx = CC_MAX(sz.cx, rect.right - rect.left);
		}
	}
	return sz;
}

void ToolBar::DoUpdate()
{
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreatedEnd) != 0 &&
		IsUpdate((UpdateFlag::Type)(UpdateFlag::UpdateFlag|UpdateFlag::UpdateChildUpdated|UpdateFlag::UpdateVisible)) &&
		!HasStyle(StyleType::Outer, OuterStyle::Sizing) && _lstControls != NULL)
	{
#ifdef _DEBUG_
		if(CC_APP_GetConfig()->GetIsDebug())
		{
			CC_TRACE((_T("ToolBar::DoUpdate(), ClassName:%s, Name:%s, Text:%s, ControlsCount:%d"), _ClassName, _Name, _Text, _lstControls == NULL ? 0 : _lstControls->GetSize()));
		}
#endif
		CC_ASSERT(::IsWindow(m_hWnd));

		//SetOuterStyle(OuterStyle::Sizing, true);
		_StyleArray[(int)StyleType::Outer] |= OuterStyle::Sizing;
		RECT rect;
		int nW = 0;
		int nH = 0;
		int nCnt = _lstControls->GetSize();
		//calcute layerout from _minSize
		if(!HasStyle(StyleType::Style, CCS_VERT))
		{
			for(int n = 0; n < nCnt; n++)
			{
				Control* ctl = _lstControls->GetKey(n);
				if(ctl->GetVisible())
				{
					rect = ctl->CalcuteRect(true, nW, 2, -1, _minSize.cy - 5, true);
					nW += rect.right - rect.left + CC_TOOLBAR_SPLITW;
				}
			}
			//here cannot be SetBoundsNotOriginal
			nW += 2;
			nH = _minSize.cy;
		}
		else
		{
			nH = 2;
			SIZE msize = GetMinSize();
			for(int n = 0; n < nCnt; n++)
			{
				Control* ctl = _lstControls->GetKey(n);
				if(ctl->GetVisible())
				{
					if(ctl->HasStyle(StyleType::Outer, OuterStyle::Break))
					{
						rect = ctl->CalcuteRect(true, 0, nH, msize.cx, _minSize.cy - 5, true);
					}
					else
					{
						rect = ctl->CalcuteRect(true, 0, nH, -1, _minSize.cy - 5, true);
					}
					nH += rect.bottom - rect.top + CC_TOOLBAR_SPLITH;
				}
			}
			//here cannot be SetBoundsNotOriginal
			nW = msize.cx;
			nH += 1;
		}
		SetBounds(0, 0, nW, nH, BoundsSpecified::Size);
		//tells parent,that i has changed size
		if(GetFloating())
		{
			_pFloater = GetFloatContainer();
			CC_ASSERT(_pFloater != NULL);
			RECT rect = {0, 0, nW, nH};
			//calculate parent size
			::AdjustWindowRectEx(&rect, _pFloater->GetStyle(StyleType::Style),
				FALSE, _pFloater->GetStyle(StyleType::ExStyle)); 
			_pFloater->SetText(GetText());
			_pFloater->SetSize(rect.right - rect.left, rect.bottom - rect.top - 2);
		}
		else
		{
			//for _pParent is form,but need to notice rebar
			CC_ASSERT(::GetParent(m_hWnd) != NULL);
			::PostMessage(::GetParent(m_hWnd), CC_WM_CHILD_UPDATED, (WPARAM)this, 0);
		}
//err(_T("ToolBar::DoUpdate, left:%d, top:%d, right-left:%d, bottom-top:%d"), rect.left, rect.top, nLeft + 2, nH + 2);
		//need draw back
		if(m_hWnd != NULL)
		{
			::InvalidateRect(m_hWnd, NULL, TRUE);
		}
		//SetOuterStyle(OuterStyle::Sizing, false);
		_StyleArray[(int)StyleType::Outer] &= ~OuterStyle::Sizing;
	}
	//clear flages
	DockBar::DoUpdate();
}

void ToolBar::OnResize(EventArgs* e)
{
	InvalidateBackGround();
	Resize.Invoke((Object*)this, e);
	//do nothing others
	if(m_hWnd != NULL)
	{
		::InvalidateRect(m_hWnd, NULL, TRUE);
	}
}

void ToolBar::Floater_OnMoving(Object* sender, EventArgs* e)
{
	//find rebar
	FindReBarEntity entity;
	::GetCursorPos(&entity.pt);
	entity.pToolBar = this;
	entity.nInsertIndex = -1;
	entity.bHorizontal = true;
	entity.eDockable = _eDockable;
	HWND hwnd = CC_APP_GetActiveFormHwnd();
	ReBar* pReBar = (ReBar*)::SendMessage(hwnd, CC_WM_MAINMSG, CC_WM_MAINMSG_WPARAM_GET_REBAR, (LPARAM)&entity);
	if(pReBar == NULL)
	{
		return;
	}

	//e->Result = DefWndProc(e);
	e->Handled = true;
	// we're done tracking...
	::ReleaseCapture();

	//get the toolbar
	SetFloating(false);
	::SetParent(m_hWnd, pReBar->m_hWnd);
	this->_pParentBrush = NULL;
	//resize self while SetStyle
	if(entity.bHorizontal)
	{
		SetStyle(StyleType::Style, CCS_VERT, 0);
	}
	else
	{
		SetStyle(StyleType::Style, 0, CCS_VERT);
	}
	//this->CalcFixedLayout(0, bHorizontal);
	pReBar->InsertBand(this, entity.nInsertIndex, false);

	RECT rect = GetBounds();
	int nPos = entity.bHorizontal ? 5 : -5;
	::PostMessage(pReBar->m_hWnd, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(rect.left - nPos, rect.top + nPos));
}

void ToolBar::OnCreateControl()
{
#if defined(_DEBUG_) || defined(_DESIGN)
	//code for design
	//flag for moveable or others
	int nCnt = _lstControls == NULL ? 0 : _lstControls->GetSize();
	for(int n = 0; n < nCnt; n++)
	{
		Control* ctl = _lstControls->GetKey(n);
		ctl->Design = (DesignStyle::Type)CC_DESIGN_NO_MOVE;
	}
#endif
	DockBar::OnCreateControl();
}
//cc::win::ToolBar
///////////////////////////////////////////////////////////////////////////////

DWORD StatusBarConfig::nStyle = WS_CHILDWINDOW | WS_VISIBLE | WS_CLIPSIBLINGS | SBT_NOBORDERS | SBT_TOOLTIPS | CCS_NOPARENTALIGN | CCS_NOMOVEY | CCS_NODIVIDER | CCS_NORESIZE;
DWORD StatusBarConfig::nExStyle = WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR;
//DWORD StatusBarConfig::nClassStyle = CS_DBLCLKS | CS_GLOBALCLASS;
DWORD StatusBarConfig::nInnerStyle = 0;
DWORD StatusBarConfig::nOuterStyle = 0;
DWORD StatusBarConfig::nOwnerDrawTextStyle = DT_LEFT|DT_VCENTER|DT_SINGLELINE;
COLORREF StatusBarConfig::corBackground = ::GetSysColor(COLOR_BTNFACE);
COLORREF StatusBarConfig::corTextground = ::GetSysColor(COLOR_BTNTEXT);
COLORREF StatusBarConfig::corHotBackColor = ::GetSysColor(COLOR_BTNFACE);
COLORREF StatusBarConfig::corHotTextColor = ::GetSysColor(COLOR_BTNTEXT);
#define CC_STATUSBAR_WAIT_TO_DEFAULTTEXT        3000
int StatusBarConfig::nWaitToDefaultText = 3000;
//===========================================================cc::win::StatusBar
#define CC_STATUSBAR_SPLITW 0
#define CC_STATUSITEM_BREAKW 6
///////////////////////////////////////////////////////////////////////////////
// constructors/destructor
StatusBar::StatusBar()
{
	BASE_CONTROL_CONFIG_INIT(StatusBar);
	_Rect.right = _Rect.left + 100;
	_Rect.bottom = _Rect.top + 20;
	::CopyRect(&_OrgRect, &_Rect);
	_ClassName = _T("msctls_statusbar32");
	_StyleArray[(int)StyleType::InitControl] = CC_WNDCOMMCTL_BAR_REG;
	Dock = (DockStyles::Type)(DockStyles::Bottom | DockStyles::Left | DockStyles::Right);
#ifdef _DEBUG_
	SetName(_T("StatusBar"));
#endif
	_pDefaultText = NULL;
	_pDefaultText = CC_StrCopy(&_pDefaultText, _T("Ready."));
	pDefaultImageList = NULL;
	_nDefaultIconID = 0;
	_nDefaultIndex = 0;
	_nDefaultTimeout = StatusBarConfig::nWaitToDefaultText;
#if defined(_DEBUG_) || defined(_DESIGN)
	//code for design
	//flag for moveable or others
	this->Design = (DesignStyle::Type)(CC_DESIGN_NO_MOVE | CC_DESIGN_NO_COLOR | DesignStyle::NoAnchor);
#endif
}

StatusBar::~StatusBar()
{
	if(_pDefaultText != NULL)
	{
		free(_pDefaultText);
		_pDefaultText = NULL;
	}
}

void StatusBar::SetText(const TCHAR* sText, ImageList* imageList, int nIconID, int nIndex)
{
	if(nIndex < 0)
	{
		nIndex = _nDefaultIndex;
	}
	Control* ctl = GetControl(nIndex);
	if(ctl != NULL)
	{
		//SetOuterStyle(OuterStyle::Sizing, true);
		_StyleArray[(int)StyleType::Outer] |= OuterStyle::Sizing;
		ctl->SetText(sText);
		ctl->SetIcon(imageList, nIconID);
		if(m_hWnd != NULL && nIndex == _nDefaultIndex)
		{
			::SetTimer(m_hWnd, CC_TIMERID_CONTROL_TIMEOUT, _nDefaultTimeout, NULL);
		}
		//SetOuterStyle(OuterStyle::Sizing, false);
		_StyleArray[(int)StyleType::Outer] &= ~OuterStyle::Sizing;
		if(m_hWnd != NULL)
		{
			OnResize(NULL);
			CC_APP_DoEvents();
		}
	}
}

void StatusBar::SetText(const cc::Str& sText, ImageList* imageList, int nIconID, int nIndex)
{
	if(sText.GetLangCode() == LangCode::TCHAR)
	{
		SetText((TCHAR*)sText.GetBuf(), imageList, nIconID, nIndex);
	}
	else
	{
		cc::Str s = cc::Str(LangCode::TCHAR, sText);
		SetText((TCHAR*)s.GetBuf(), imageList, nIconID, nIndex);
	}
}

void StatusBar::SetDefault(const TCHAR* pDefaultText, ImageList* imageList, int nIconID, int nDefaultIndex, int nTimeOut)
{
	_pDefaultText = CC_StrCopy(&_pDefaultText, pDefaultText);
	pDefaultImageList = imageList;
	_nDefaultIconID = nIconID;
	if(nDefaultIndex >= 0)
	{
		_nDefaultIndex = nDefaultIndex;
	}
	if(nTimeOut >= 0)
	{
		_nDefaultTimeout = nTimeOut;
	}
}

void StatusBar::DoSetLang(KeyArr& keyArr, cc::Str sThisHead)
{
#ifdef _DEBUG_
	if(CC_APP_GetConfig()->GetIsDebug())
	{
		//notice the head should be "Lang."
		cc::Str sThisHeadLog = sThisHead.Substring(5);
		if(sThisHeadLog.GetLength() > 0)
		{
			CC_TRACE((_T("ITextIcon::DoSetLang(), Name:%s, Text:%s\r\n%s.Default="), _Name, _Text, (TCHAR*)sThisHeadLog.GetBuf()));
		}
	}
#endif
	Container::DoSetLang(keyArr, sThisHead);
	if(keyArr.Exist(sThisHead.Append(_T(".Default"))))
	{
		_pDefaultText = CC_StrCopy(&_pDefaultText, (TCHAR*)keyArr.Get(sThisHead).GetBuf());
	}
}

void StatusBar::OnInnerTimer(EventArgs *e)
{
	//e->WParam is ID
	if(e->WParam != CC_TIMERID_CONTROL_TIMEOUT)
	{
		Control::OnInnerTimer(e);
		return;
	}
	::KillTimer(m_hWnd, CC_TIMERID_CONTROL_TIMEOUT);
	Control* ctl = GetControl(_nDefaultIndex);
	if(ctl != NULL)
	{
		//SetOuterStyle(OuterStyle::Sizing, true);
		_StyleArray[(int)StyleType::Outer] |= OuterStyle::Sizing;
		ctl->SetText(_pDefaultText);
		ctl->SetIcon(pDefaultImageList, _nDefaultIconID);
		//SetOuterStyle(OuterStyle::Sizing, false);
		_StyleArray[(int)StyleType::Outer] &= ~OuterStyle::Sizing;
		OnResize(NULL);
	}
}

bool StatusBar::KeepShow()
{
	//for not know whether or not is default text,return true always
	::KillTimer(m_hWnd, CC_TIMERID_CONTROL_TIMEOUT);
	::SetTimer(m_hWnd, CC_TIMERID_CONTROL_TIMEOUT, _nDefaultTimeout, NULL);
	return true;
}

//void StatusBar::OnVisibleChanged(EventArgs *e)
//{
//	OnResize(NULL);
//	Container::OnVisibleChanged(e);
//}
//
void StatusBar::DoUpdate()
{
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreatedEnd) != 0)
	{
		if(!HasStyle(StyleType::Outer, OuterStyle::Sizing))
		{
			OnResize(NULL);
		}
		if(IsUpdate((UpdateFlag::Type)(UpdateFlag::UpdateVisible)))
		{
			CC_ASSERT(_pParent != NULL);
			//let parent form to resize and redraw
			::SendMessage(_pParent->m_hWnd, CC_WM_CHILD_UPDATED, (WPARAM)this, 0);
		}
		//clear flages
		Container::DoUpdate();
	}
}

void StatusBar::OnResize(EventArgs* e)
{
	InvalidateBackGround();
	Resize.Invoke((Object*)this, e);
	if(_lstControls != NULL && m_hWnd != NULL)
	{
		CC_ASSERT(::IsWindow(m_hWnd));

		//SetOuterStyle(OuterStyle::Sizing, true);
		_StyleArray[(int)StyleType::Outer] |= OuterStyle::Sizing;
		RECT rectP = GetBounds();
		int nH = rectP.bottom - rectP.top;
		int nW = rectP.right - rectP.left - 15;
		int nCnt = _lstControls->GetSize();
		RECT rect;
		Control* ctl;

		// determine extra space for stretchy pane
		int nLeft = 2;
		int cxExtra = nW - nLeft;
		int nStretchyCount = 0;
		for(int n = 0; n < nCnt; n++)
		{
			ctl = _lstControls->GetKey(n);
			if(ctl->GetVisible())
			{
				if(ctl->HasStyle(StyleType::Outer, OuterStyle::Break))
				{
					cxExtra -= CC_STATUSITEM_BREAKW;
				}
				else if(ctl->HasStyle(StyleType::Outer, OuterStyle::AutoSize))
				{
					rect = ctl->CalcuteRect(false, -1, -1, -1, nH, true);
					cxExtra -= (rect.right - rect.left + CC_STATUSBAR_SPLITW);
				}
				else if(ctl->HasStyle(StyleType::Outer, OuterStyle::AutoSizeSprint))
				{
					++nStretchyCount;
					rect = ctl->CalcuteRect(false, -1, -1, -1, nH, true);
					cxExtra -= (rect.right - rect.left + CC_STATUSBAR_SPLITW);
				}
				else//None
				{
					rect = ctl->GetOriginalBounds();
					cxExtra -= (rect.right - rect.left + CC_STATUSBAR_SPLITW);
				}
			}
		}

		// determine right edge of each pane
		for(int n = 0; n < nCnt; n++)
		{
			ctl = _lstControls->GetKey(n);
			if(ctl->GetVisible())
			{
				if(nLeft >= nW)
				{
					rect = ctl->GetOriginalBounds();
					rect.right = rect.left;
					ctl->SetBoundsNotOriginal(rect, BoundsSpecified::Size);
					continue;
				}
				if(ctl->HasStyle(StyleType::Outer, OuterStyle::Break))
				{
					nLeft += (CC_STATUSITEM_BREAKW/2);
					::SetRect(&rect, nLeft - 1, 2, nLeft + 1, nH - 1);
					nLeft += (CC_STATUSITEM_BREAKW/2);
				}
				else if(ctl->HasStyle(StyleType::Outer, OuterStyle::AutoSize))
				{
					rect = ctl->CalcuteRect(false, nLeft, 0, -1, nH, true);
					nLeft += rect.right - rect.left + CC_STATUSBAR_SPLITW;
				}
				else if(ctl->HasStyle(StyleType::Outer, OuterStyle::AutoSizeSprint))
				{
					rect = ctl->CalcuteRect(false, nLeft, 0, -1, nH, true);
					int nCtlW = rect.right - rect.left;

					int cxAddExtra = cxExtra / nStretchyCount;
					nCtlW += cxAddExtra;
					--nStretchyCount;
					cxExtra -= cxAddExtra;

					if(nCtlW < 0)
					{
						nCtlW = 0;
					}
					rect.right = nLeft + nCtlW;
					nLeft += nCtlW + CC_STATUSBAR_SPLITW;
				}
				else//None
				{
					rect = ctl->GetOriginalBounds();
					int nCtlW = rect.right - rect.left;
					rect.left = nLeft;
					rect.right = nLeft + nCtlW;
					nLeft += nCtlW + CC_STATUSBAR_SPLITW;
				}
				if(rect.right >= nW)
				{
					if(rect.left < nW)
					{
						rect.right = nW;
					}
					else
					{
						rect.right = rect.left;
					}
				}
				ctl->SetBoundsNotOriginal(rect);
			}
		}

		if(m_hWnd != NULL)
		{
			::InvalidateRect(m_hWnd, NULL, FALSE);
		}
		//SetOuterStyle(OuterStyle::Sizing, false);
		_StyleArray[(int)StyleType::Outer] &= ~OuterStyle::Sizing;
	}
}

void StatusBar::OnCreateControl()
{
#if defined(_DEBUG_) || defined(_DESIGN)
	//code for design
	//flag for moveable or others
	int nCnt = _lstControls == NULL ? 0 : _lstControls->GetSize();
	for(int n = 0; n < nCnt; n++)
	{
		Control* ctl = _lstControls->GetKey(n);
		ctl->Design = (DesignStyle::Type)CC_DESIGN_NO_MOVE;
	}
#endif
	Container::OnCreateControl();
}
// constructors/destructor
///////////////////////////////////////////////////////////////////////////////

DWORD ScrollContainerConfig::nStyle = WS_CHILD | WS_VISIBLE | WS_TABSTOP;
DWORD ScrollContainerConfig::nExStyle = 0;
//DWORD ScrollContainerConfig::nClassStyle = CS_DBLCLKS | CS_GLOBALCLASS;
DWORD ScrollContainerConfig::nInnerStyle = InnerStyle::TranceContextMenuUp;
DWORD ScrollContainerConfig::nOuterStyle = 0;
DWORD ScrollContainerConfig::nOwnerDrawTextStyle = DT_LEFT|DT_VCENTER|DT_SINGLELINE;
COLORREF ScrollContainerConfig::corBackground = ::GetSysColor(COLOR_BTNFACE);
COLORREF ScrollContainerConfig::corTextground = ::GetSysColor(COLOR_BTNTEXT);
COLORREF ScrollContainerConfig::corHotBackColor = ::GetSysColor(COLOR_BTNFACE);
COLORREF ScrollContainerConfig::corHotTextColor = ::GetSysColor(COLOR_BTNTEXT);
DWORD ScrollContainerConfig::nClientFlags = CC_SCROLLCONTAINER_CLIENT_USEPANEL;
//===========================================================cc::win::ScrollContainer
///////////////////////////////////////////////////////////////////////////////
ScrollContainer::ScrollContainer()
{
	BASE_CONTROL_CONFIG_INIT(ScrollContainer);
#ifdef _DEBUG_
	SetName(_T("ScrollContainer"));
#endif
	//m_pClient = new ScrollClient();
	//m_pClientClient = m_pClient->GetClient();
	if(CC_BIT_HAS(ScrollContainerConfig::nClientFlags, CC_SCROLLCONTAINER_CLIENT_USEPANEL))
	{
		m_pClient = new Panel();
		m_pClientClient = new Panel();
	}
	else
	{
		m_pClient = new Control();
		m_pClientClient = new Control();
	}
	m_pClient->SetConfigName(_T(""));
	m_pClientClient->SetConfigName(_T(""));
	m_pClient->MouseDown += MouseEventHandler((Object*)this, (MouseEventFun)&ScrollContainer::StartMoveClient);
	m_pClientClient->MouseDown += MouseEventHandler((Object*)this, (MouseEventFun)&ScrollContainer::StartMoveClient);
	m_pClientClient->MouseMove += MouseEventHandler((Object*)this, (MouseEventFun)&ScrollContainer::Client_MouseMove);
	m_pClientClient->MouseUp += MouseEventHandler((Object*)this, (MouseEventFun)&ScrollContainer::Client_MouseUp);
#if defined(_DEBUG_) || defined(_DESIGN)
	//code for design
	//flag for moveable or others
	m_pClient->Design = (DesignStyle::Type)DesignStyle::NoDesign;
	m_pClientClient->Design = (DesignStyle::Type)DesignStyle::NoDesign;
#endif

	Panel::AddControl(m_pClient);
	m_pClient->AddControl(m_pClientClient);
	//_pClient = m_pClient;
	_pClient = m_pClientClient;
	//here AddControl will add to self's _lstControls,the next AddControl will add to _pClient
	//Panel::AddControl(_pClient);

	m_hHBar = NULL;
	m_hVBar = NULL;
	nMinMoveLimit = 3;
	nOldX = 0;
	nOldY = 0;
	nOldH = 0;
	nOldV = 0;
	//bIsScrollAuto = true;
	CC_BIT_ON(_Flags, CC_FLAG_SCROLLCONTAINER_SCROLLAUTO);
}

ScrollContainer::~ScrollContainer()
{
	if(m_hHBar != NULL)
	{
		delete m_hHBar;
		m_hHBar = NULL;
	}
	if(m_hVBar != NULL)
	{
		delete m_hVBar;
		m_hVBar = NULL;
	}
	//if(m_pClient != NULL)
	//{
	//	delete m_pClient;
	//	m_pClient = NULL;
	//}
}

void ScrollContainer::Dispose()
{
	if(m_hHBar != NULL)
	{
		m_hHBar->Dispose();
	}
	if(m_hVBar != NULL)
	{
		m_hVBar->Dispose();
	}
	if(m_pClient != NULL)
	{
		m_pClient->Dispose();
	}
	Panel::Dispose();
}

void ScrollContainer::StartMoveClient(Object* sender, MouseEventArgs* e)
{
	Client_MouseDown(sender, e);
}

void ScrollContainer::Client_MouseDown(Object* sender, MouseEventArgs* e)
{
	POINT pt;
	GetCursorPos(&pt);
	HWND hWnd = ::WindowFromPoint(pt);
	if((hWnd == m_pClient->m_hWnd || ::IsChild(m_pClient->m_hWnd, hWnd) ) && e->Button == MouseButtons::Left)
	{
		SetCapture(m_pClientClient->m_hWnd);
		GetCursorPos(&ptOldClient);
		::SetCursor(::LoadCursor(NULL, IDC_SIZEALL));
	}
}

void ScrollContainer::Client_MouseMove(Object* sender, MouseEventArgs* e)
{
	POINT pt;
	GetCursorPos(&pt);
	HWND hWnd = ::WindowFromPoint(pt);
	//if(::IsChild(m_hWnd, ::GetCapture()))
	if((hWnd == m_pClient->m_hWnd || ::IsChild(m_pClient->m_hWnd, hWnd) ) && ::IsChild(m_hWnd, ::GetCapture()))
	{
		POINT pt;
		GetCursorPos(&pt);

		int x = pt.x - ptOldClient.x;
		int y = pt.y - ptOldClient.y;
		if(x > nMinMoveLimit || y >nMinMoveLimit || x < -nMinMoveLimit || y < -nMinMoveLimit)
		{
			int cur;
			if(m_hHBar != NULL && m_hHBar->GetLargeChange() > 0)
			{
				cur = m_hHBar->GetValue();
				m_hHBar->SetValue(cur - x);
			}
			if(m_hVBar != NULL && m_hVBar->GetLargeChange() > 0)
			{
				cur = m_hVBar->GetValue();
				m_hVBar->SetValue(cur - y);
			}
			UpdateClientPosition();
			//cc::Log::debug(_T("now x:%d, y:%d, m_hHBar->GetValue():%d, m_hVBar->GetValue():%d, old x:%d, y:%d"), x, y, m_hHBar->GetValue(), m_hVBar->GetValue(), ptOldClient.x, ptOldClient.y);
			ptOldClient.x = pt.x;
			ptOldClient.y = pt.y;
		}
	}
}

void ScrollContainer::Client_MouseUp(Object* sender, MouseEventArgs* e)
{
	UpdateClientPosition();
	::SetCursor(::LoadCursor(NULL, IDC_ARROW));
	ReleaseCapture();
}

void ScrollContainer::Scroll_ValueChanged(Object* sender, EventArgs* e)
{
	CC_ASSERT(::IsWindow(m_hWnd));

	if(e != NULL)
	{
		int x = 0;
		int y = 0;
		if(e->Msg == WM_HSCROLL)
		{
			x = (int)e->LParam - nOldX;
		}
		else
		{
			y = (int)e->LParam - nOldY;
		}
		if(LOWORD(e->WParam) != SB_THUMBTRACK || x > nMinMoveLimit || y > nMinMoveLimit || x < -nMinMoveLimit || y < -nMinMoveLimit)
		{
			if(e->Msg == WM_HSCROLL)
			{
				nOldX = (int)e->LParam;
			}
			else
			{
				nOldY = (int)e->LParam;
			}
			UpdateClientPosition();
			//cc::Log::debug(_T("nOldX:%d, nOldY:%d, x:%d, y:%d, (int)HIWORD(e->WParam):%d"), nOldX, nOldY, x, y, (int)HIWORD(e->WParam));
		}
	}
}

void ScrollContainer::UpdateClientPosition()
{
	DoUpdateClientPosition();
}

void ScrollContainer::DoUpdateClientPosition()
{
	int nH = (m_hHBar != NULL && m_hHBar->GetVisible()) ? m_hHBar->GetValue() : 0;
	int nV = (m_hVBar != NULL && m_hVBar->GetVisible()) ? m_hVBar->GetValue() : 0;
	if(nOldH != nH || nOldV != nV)
	{
		nOldH = nH;
		nOldV = nV;
		RECT rect = m_pClientClient->GetOriginalBounds();
		//cc::Log::debug(_T("now rect.left:%d, rect.top:%d, new rect.left:%d, rect.top:%d"), rect.left, rect.top, rect.left - nH, rect.top - nV);
		m_pClientClient->SetBoundsNotOriginal(rect.left - nH, rect.top - nV, 0, 0, BoundsSpecified::Location);

		EventArgs e(m_hWnd, CC_WM_MAINMSG, 0, MAKELONG(nV, nH));
		UpdatePositionEvent.Invoke((Object*)this, &e);
		//only need erase back while clientclient is small than client
		//if(_pClient->GetClassID() == CC_CLASSID_Panel && _pClient->HasStyle(StyleType::Outer, OuterStyle::TransparentParentErase))
		if((m_hHBar != NULL || m_hVBar != NULL) && _pClient->HasStyle(StyleType::Outer, OuterStyle::TransparentParentBrush|OuterStyle::TransparentParentErase|OuterStyle::TransparentHollowBrush))
		{
			//::GetClientRect(m_pClientClient->m_hWnd, &rect);
			//HRGN hRgn = CreateRectRgn(rect.left, rect.top, rect.right, rect.bottom);
			//HRGN hRgn2 = CreateRectRgn(rect.left, rect.top, rect.right, rect.bottom);
			//::CombineRgn(hRgn, hRgn, hRgn2, RGN_DIFF);
			//DeleteObject(hRgn2);
			//InvalidateRgn(m_pClientClient->m_hWnd, hRgn, FALSE);
			//DeleteObject(hRgn);
			if(_pClient->m_hWnd != NULL)
			{
				::InvalidateRect(_pClient->m_hWnd, NULL, FALSE);
			}
			//::KillTimer(_pClient->m_hWnd, CC_TIMERID_INVALIDATERECT);
			//::SetTimer(_pClient->m_hWnd, CC_TIMERID_INVALIDATERECT, 5, NULL);
		}
	}
}

void ScrollContainer::UpdateScrollInfo()
{
	int nWH;
	int nBarWH;
	RECT rect2;
	RECT rect = m_pClientClient->GetBounds();
	if(m_hHBar != NULL)
	{
		nWH = rect.right - rect.left;
		rect2 = m_hHBar->GetBounds();
		nBarWH = rect2.right - rect2.left + 1;
		m_hHBar->SetLargeChange((nWH > nBarWH) ? nBarWH : -1);
	}
	if(m_hVBar != NULL)
	{
		nWH = rect.bottom - rect.top;
		rect2 = m_hVBar->GetBounds();
		nBarWH = rect2.bottom - rect2.top + 1;
		m_hVBar->SetLargeChange((nWH > nBarWH) ? nBarWH : -1);
	}
	//move client
	nOldH = -1;
	nOldV = -1;
	UpdateClientPosition();
}

void ScrollContainer::EnableScrollBar(int nBar, bool bEnable)
{
	RECT rect = m_pClientClient->GetBounds();
	if(nBar == SB_BOTH || nBar == SB_HORZ)
	{
		if(bEnable && m_hHBar == NULL)
		{
			m_hHBar = new HScrollBar();
			m_hHBar->SetParent(this, true);
			m_hHBar->SetMinMaxValue(0, rect.right - rect.left, true);
			m_hHBar->ValueChanged += EventHandler((Object*)this, (EventFun)&ScrollContainer::Scroll_ValueChanged);
			if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
			{
				m_hHBar->CreateControl();
			}
#if defined(_DEBUG_) || defined(_DESIGN)
			//code for design
			//flag for moveable or others
			m_hHBar->Design = (DesignStyle::Type)DesignStyle::NoDesign;
#endif
		}
		if(!bEnable && m_hHBar != NULL)
		{
			m_hHBar->Dispose();
			delete m_hHBar;
			m_hHBar = NULL;
		}
	}
	if(nBar == SB_BOTH || nBar == SB_VERT)
	{
		if(bEnable && m_hVBar == NULL)
		{
			m_hVBar = new VScrollBar();
			m_hVBar->SetParent(this, true);
			m_hVBar->SetMinMaxValue(0, rect.bottom - rect.top, true);
			m_hVBar->ValueChanged += EventHandler((Object*)this, (EventFun)&ScrollContainer::Scroll_ValueChanged);
			if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
			{
				m_hVBar->CreateControl();
			}
#if defined(_DEBUG_) || defined(_DESIGN)
			//code for design
			//flag for moveable or others
			m_hVBar->Design = (DesignStyle::Type)DesignStyle::NoDesign;
#endif
		}
		if(!bEnable && m_hVBar != NULL)
		{
			m_hVBar->Dispose();
			delete m_hVBar;
			m_hVBar = NULL;
		}
	}
	OnResize(NULL);
}

void ScrollContainer::OnCreateControl()
{
	Panel::OnCreateControl();

	if(m_hHBar != NULL)
	{
		m_hHBar->CreateControl();
	}
	if(m_hVBar != NULL)
	{
		m_hVBar->CreateControl();
	}
}

void ScrollContainer::OnResize(EventArgs* e)
{
	CC_ASSERT(_pClient != NULL);
	InvalidateBackGround();
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
	{
		//SetOuterStyle(OuterStyle::Sizing, true);
		_StyleArray[(int)StyleType::Outer] |= OuterStyle::Sizing;
		HDWP hDWP = ::BeginDeferWindowPos(3); // reasonable guess
		RECT rect;
		int nW = 0;
		RECT frameRect;
		::GetClientRect(m_hWnd, &frameRect);
		rect = m_pClientClient->GetBounds();
		if(m_hVBar != NULL && m_hVBar->m_hWnd != NULL)
		{
			bool isShow = !CC_BIT_HAS(_Flags, CC_FLAG_SCROLLCONTAINER_SCROLLAUTO) || (rect.bottom - rect.top > frameRect.bottom - frameRect.top);
			m_hVBar->SetVisible(isShow);
			if(isShow)
			{
				RECT rect = m_hVBar->GetBounds();
				nW = rect.right - rect.left;
			}
		}
		if(m_hHBar != NULL && m_hHBar->m_hWnd != NULL)
		{
			bool isShow = !CC_BIT_HAS(_Flags, CC_FLAG_SCROLLCONTAINER_SCROLLAUTO) || (rect.right - rect.left > frameRect.right - frameRect.left);
			m_hHBar->SetVisible(isShow);
			if(isShow)
			{
				RECT rect = m_hHBar->GetBounds();
				int nH = rect.bottom - rect.top;
				frameRect.bottom -= nH;
				hDWP = ::DeferWindowPos(hDWP, m_hHBar->m_hWnd, NULL, frameRect.left, frameRect.bottom, frameRect.right - frameRect.left - nW, nH, SWP_NOACTIVATE|SWP_NOZORDER);
			}
		}

		if(nW > 0)
		{
			frameRect.right -= nW;
			hDWP = ::DeferWindowPos(hDWP, m_hVBar->m_hWnd, NULL, frameRect.right, frameRect.top, nW, frameRect.bottom - frameRect.top, SWP_NOACTIVATE|SWP_NOZORDER);
		}
		//_pClient->SetBoundsNotOriginal(frameRect);
		hDWP = ::DeferWindowPos(hDWP, m_pClient->m_hWnd, NULL, frameRect.left, frameRect.top, frameRect.right - frameRect.left, frameRect.bottom - frameRect.top, SWP_NOACTIVATE|SWP_NOZORDER);
		::EndDeferWindowPos(hDWP);

		_StyleArray[(int)StyleType::Outer] &= ~OuterStyle::Sizing;
		UpdateScrollInfo();
		Resize.Invoke((Object*)this, e);
	}
}

void ScrollContainer::SetIsScrollAuto(bool value)
{
	//bIsScrollAuto = value;
	CC_BIT_SET(_Flags, CC_FLAG_SCROLLCONTAINER_SCROLLAUTO, value);
	OnResize(NULL);
}

bool ScrollContainer::GetIsScrollAuto()
{
	return CC_BIT_HAS(_Flags, CC_FLAG_SCROLLCONTAINER_SCROLLAUTO);
}

RECT ScrollContainer::GetClientBounds()
{
	CC_ASSERT(m_pClientClient != NULL);
	return m_pClientClient->GetBounds();
	//SIZE size = {rect.right - rect.left, rect.bottom - rect.top};
	//return size;
}

void ScrollContainer::SetClientBounds(int x, int y, int width, int height)
{
	CC_ASSERT(m_pClientClient != NULL);
	m_pClientClient->SetBounds(x, y, width, height);
	if(m_hHBar != NULL)
	{
		m_hHBar->SetMinMaxValue(0, width, true);
	}
	if(m_hVBar != NULL)
	{
		m_hVBar->SetMinMaxValue(0, height, true);
	}
	OnResize(NULL);
}

int ScrollContainer::GetMinMoveLimit()
{
	return nMinMoveLimit;
}

void ScrollContainer::SetMinMoveLimit(int nMinMoveLimit)
{
	this->nMinMoveLimit = nMinMoveLimit;
}

UINT NotifyIconConfig::nWaitTimerSeeIfDoubleClick = 500;
//===========================================================cc::win::NotifyIcon
///////////////////////////////////////////////////////////////////////////////
// constructors/destructor
NotifyIcon::NotifyIcon()
{
	//_Flags = 0;
#ifdef _DEBUG_
	SetName(_T("NotifyIcon"));
#endif
	//_Visible = false;
	//CC_BIT_OFF(_Flags, CC_FLAG_CONTROL_VISIBLE);
	//_isCreatedNotifyIcon = false;
	//_isHidden = false;
	//_lastIsDBLCLK = false;
	_pContextMenu = NULL;

	_uTrackPopupMenuFlag = TPM_LEFTALIGN;
	//_eShowMenu = ShowMenu::RightLeftClick;
	CC_BIT_ON(_Flags, CC_FLAG_NOTIFYICON_LEFTCLICKMENU);
	CC_BIT_ON(_Flags, CC_FLAG_NOTIFYICON_RIGHTCLICKMENU);
	_pPopupWin = NULL;
	//_pTimer = NULL;
	//_pTimer2 = NULL;
	_lstAnimates = NULL;
	_lstAniFlg = NULL;
	_nAnimateIndex = 0;
	_nCycles = -1;
	_ptLeftClick.x = 0;
	_ptLeftClick.y = 0;
}

NotifyIcon::~NotifyIcon()
{
	Dispose();
	if(_pPopupWin != NULL)
	{
		delete _pPopupWin;
		_pPopupWin = NULL;
	}
	//if(_pTimer != NULL)
	//{
	//	delete _pTimer;
	//	_pTimer = NULL;
	//}
	//if(_pTimer2 != NULL)
	//{
	//	delete _pTimer2;
	//	_pTimer2 = NULL;
	//}
	if(_lstAnimates != NULL)
	{
		for(int n = _lstAnimates->GetSize() - 1; n >= 0; n--)
		{
			if(_lstAniFlg->GetKey(n) == 1)
			{
				::DestroyIcon((HICON)_lstAnimates->GetKey(n));
			}
		}
		_lstAnimates->Clear();
		delete _lstAnimates;
		_lstAnimates = NULL;
	}
	if(_lstAniFlg != NULL)
	{
		_lstAniFlg->Clear();
		delete _lstAniFlg;
		_lstAniFlg = NULL;
	}
}

void NotifyIcon::Dispose()
{
	if(CC_BIT_HAS(_Flags, CC_FLAG_NOTIFYICON_CREATED) && !CC_BIT_HAS(_Flags, CC_FLAG_NOTIFYICON_HIDDEN))
	{
		::KillTimer(_pPopupWin->m_hWnd, CC_TIMERID_NOTIFYICON_DBCLICK);
		::KillTimer(_pPopupWin->m_hWnd, CC_TIMERID_NOTIFYICONL_ANIMATE);
		_dataNotify.uFlags = 0;
		Shell_NotifyIcon(NIM_DELETE, &_dataNotify);
		_dataNotify.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
		CC_BIT_ON(_Flags, CC_FLAG_NOTIFYICON_HIDDEN);
	}
	if(!GetIsDispose())
	{
		if(_pPopupWin != NULL)
		{
			_pPopupWin->Dispose();
		}
		Object::Dispose();
	}
}

void NotifyIcon::DoUpdate()
{
	if(CC_BIT_HAS(_Flags, CC_FLAG_NOTIFYICON_CREATED) && !CC_BIT_HAS(_Flags, CC_FLAG_NOTIFYICON_HIDDEN))
	{
		if(IsUpdate(UpdateFlag::UpdateIcon))
		{
			//_dataNotify.uFlags = NIF_ICON;
			_dataNotify.hIcon = GetIcon();
			Shell_NotifyIcon(NIM_MODIFY, &_dataNotify);
		}
		if(IsUpdate(UpdateFlag::UpdateText))
		{
			//_dataNotify.uFlags = NIF_TIP;
			//_tcsncpy(_dataNotify.szTip, _Text, CC_MIN(_tcslen(_Text), 127));
			int n = (int)_tcslen(_Text);
			if(n > 127)
			{
				cc::Buf::Cpy(_dataNotify.szTip, 128, _Text);
			}
			else
			{
				cc::Buf::Cpy(_dataNotify.szTip, 128, cc::Str(_Text, 0, 127));
			}
			Shell_NotifyIcon(NIM_MODIFY, &_dataNotify);
		}
	}
	//clear flages
	ITextIcon::DoUpdate();
}

void NotifyIcon::SetVisible(bool value)
{
	if(!CC_BIT_HAS(_Flags, CC_FLAG_NOTIFYICON_CREATED))
	{
		if(_pPopupWin == NULL)
		{
			_pPopupWin = new Control();
#ifdef _DEBUG_
			_pPopupWin->SetName(_T("NotifyIcon_PopupWin"));
			_pPopupWin->SetText(_T("NotifyIcon_PopupWin"));
#endif
			_pPopupWin->SetVisible(false);
			_pPopupWin->SetBoundsNotOriginal(0, 0, -1, -1);
			_pPopupWin->SetStyle(StyleType::Style, 0, WS_POPUP);
			_pPopupWin->SetStyle(StyleType::ExStyle, 0, WS_EX_TOOLWINDOW);
			_pPopupWin->DoubleClick += EventHandler((Object*)this, (EventFun)&NotifyIcon::OnDoubleClick);
			_pPopupWin->MouseDown += MouseEventHandler((Object*)this, (MouseEventFun)&NotifyIcon::OnMouseDown);
			_pPopupWin->MouseMove += MouseEventHandler((Object*)this, (MouseEventFun)&NotifyIcon::OnMouseMove);
			_pPopupWin->MouseUp += MouseEventHandler((Object*)this, (MouseEventFun)&NotifyIcon::OnMouseUp);
			_pPopupWin->Tick += EventHandler((Object*)this, (EventFun)&NotifyIcon::_pPopupWin_Tick);
			_pPopupWin->CreateControl();
		}

		//load up the NOTIFYICONDATA structure
		//memset(&_dataNotify, 0, sizeof(NOTIFYICONDATA));   // start with NULL defaults
		ZeroMemory(&_dataNotify, sizeof(NOTIFYICONDATA));
		_dataNotify.cbSize = sizeof(NOTIFYICONDATA);
		_dataNotify.hWnd = _pPopupWin->m_hWnd;
		_dataNotify.uID = CC_WM_MAINMSG_WPARAM_NOTIFYICON_ID;
		_dataNotify.hIcon = GetIcon();
		_dataNotify.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
		_dataNotify.uCallbackMessage = CC_WM_MAINMSG;
		if(_Text != NULL)
		{
			//_tcsncpy(_dataNotify.szTip, _Text, CC_MIN(_tcslen(_Text), 127));
			int n = (int)_tcslen(_Text);
			if(n > 127)
			{
				cc::Buf::Cpy(_dataNotify.szTip, 128, _Text);
			}
			else
			{
				cc::Buf::Cpy(_dataNotify.szTip, 128, cc::Str(_Text, 0, 127));
			}
		}
		////create it
		//BOOL isOK = Shell_NotifyIcon(NIM_ADD, &_dataNotify);
		////for some time error
		//int nCnt = 0;
		//while(isOK != TRUE && nCnt <= 10)
		//{
		//	nCnt++;
		//	isOK = Shell_NotifyIcon(NIM_MODIFY, &_dataNotify);
		//	if(isOK != TRUE)
		//	{
		//		Sleep(150);
		//		isOK = Shell_NotifyIcon(NIM_ADD, &_dataNotify);
		//	}
		//}
		//_isCreatedNotifyIcon = (isOK == TRUE);
		//CC_BIT_SET(_Flags, CC_FLAG_NOTIFYICON_CREATED, (isOK == TRUE));
		//if(!CC_BIT_HAS(_Flags, CC_FLAG_NOTIFYICON_CREATED))
		//{
		//	CC_LOGSTEP();
		//	cc::Log::fatal(_T("NotifyIcon::CreateHandle, error to create NotifyIcon."));
		//	CC_ASSERT(CC_BIT_HAS(_Flags, CC_FLAG_NOTIFYICON_CREATED));
		//	throw UserException(_T("NotifyIcon::CreateHandle, error to create NotifyIcon."));
		//}
		CC_BIT_ON(_Flags, CC_FLAG_NOTIFYICON_CREATED);
		CC_BIT_ON(_Flags, CC_FLAG_NOTIFYICON_HIDDEN);
	}

	//_Visible = value;
	CC_BIT_SET(_Flags, CC_FLAG_NOTIFYICON_VISIBLE, value);

	if(CC_BIT_HAS(_Flags, CC_FLAG_NOTIFYICON_CREATED))
	{
		if(CC_BIT_HAS(_Flags, CC_FLAG_NOTIFYICON_VISIBLE))
		{
			//show
			if(CC_BIT_HAS(_Flags, CC_FLAG_NOTIFYICON_HIDDEN))
			{
				_dataNotify.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
				if(Shell_NotifyIcon(NIM_ADD, &_dataNotify) == FALSE)
				{
					::KillTimer(_pPopupWin->m_hWnd, CC_TIMERID_NOTIFYICONL_RESHOW);
					::SetTimer(_pPopupWin->m_hWnd, CC_TIMERID_NOTIFYICONL_RESHOW, 1000, NULL);
				}
				//_isHidden = false;
				CC_BIT_OFF(_Flags, CC_FLAG_NOTIFYICON_HIDDEN);
			}
		}
		else
		{
			::KillTimer(_pPopupWin->m_hWnd, CC_TIMERID_NOTIFYICON_DBCLICK);
			::KillTimer(_pPopupWin->m_hWnd, CC_TIMERID_NOTIFYICONL_ANIMATE);
			//hide
			if(!CC_BIT_HAS(_Flags, CC_FLAG_NOTIFYICON_HIDDEN))
			{
				_dataNotify.uFlags = 0;
				Shell_NotifyIcon(NIM_DELETE, &_dataNotify);
				//_isHidden = true;
				_dataNotify.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
				CC_BIT_ON(_Flags, CC_FLAG_NOTIFYICON_HIDDEN);
			}
		}
	}
}

ContextMenu* NotifyIcon::GetContextMenu() const
{
	return _pContextMenu;
}

void NotifyIcon::SetContextMenu(ContextMenu* menu)
{
	_pContextMenu = menu;
}
//
//void NotifyIcon::SetShowMenu(NotifyIcon::ShowMenu::Type type)
//{
//	_eShowMenu = type;
//}
//
//NotifyIcon::ShowMenu::Type NotifyIcon::GetShowMenu()
//{
//	return _eShowMenu;
//}

void NotifyIcon::SetShowMenu(LeftRight::Type type, bool value)
{
	if((type & LeftRight::Left) != 0)
	{
		CC_BIT_SET(_Flags, CC_FLAG_NOTIFYICON_LEFTCLICKMENU, value);
	}
	if((type & LeftRight::Right) != 0)
	{
		CC_BIT_SET(_Flags, CC_FLAG_NOTIFYICON_RIGHTCLICKMENU, value);
	}
}

bool NotifyIcon::GetShowMenu(LeftRight::Type type)
{
	if((type & LeftRight::Left) != 0)
	{
		return CC_BIT_HAS(_Flags, CC_FLAG_NOTIFYICON_LEFTCLICKMENU);
	}
	return CC_BIT_HAS(_Flags, CC_FLAG_NOTIFYICON_RIGHTCLICKMENU);
}

void NotifyIcon::OnClick(Object* sender, EventArgs* e)
{
	Click.Invoke((Object*)this, e);
}

void NotifyIcon::OnDoubleClick(Object* sender, EventArgs* e)
{
	//_lastIsDBLCLK = true;
	CC_BIT_ON(_Flags, CC_FLAG_NOTIFYICON_LASTDBLCLK);
	DoubleClick.Invoke((Object*)this, e);
}

void NotifyIcon::OnMouseDown(Object* sender, MouseEventArgs* e)
{
	MouseDown.Invoke((Object*)this, e);
}

void NotifyIcon::OnMouseMove(Object* sender, MouseEventArgs* e)
{
	MouseMove.Invoke((Object*)this, e);
}

void NotifyIcon::OnMouseUp(Object* sender, MouseEventArgs* e)
{
	MouseUp.Invoke((Object*)this, e);
	OnClick(sender, e->baseArgs);

	//show menu
	if(e->Button == MouseButtons::Left)
	{
		//if last is dblclk,do nothing
		if(CC_BIT_HAS(_Flags, CC_FLAG_NOTIFYICON_LASTDBLCLK))
		{
			::KillTimer(_pPopupWin->m_hWnd, CC_TIMERID_NOTIFYICON_DBCLICK);
			//if(_pTimer != NULL)
			//{
			//	_pTimer->Stop();
			//}
			//_lastIsDBLCLK = false;
			CC_BIT_OFF(_Flags, CC_FLAG_NOTIFYICON_LASTDBLCLK);
			return;
		}
		///if(_eShowMenu == ShowMenu.LeftClick || _eShowMenu == ShowMenu.RightLeftClick)
		if(CC_BIT_HAS(_Flags, CC_FLAG_NOTIFYICON_LEFTCLICKMENU))
		{
			//wait for a while to see if is DBLCLK
			_ptLeftClick.x = e->X;
			_ptLeftClick.y = e->Y;
			::KillTimer(_pPopupWin->m_hWnd, CC_TIMERID_NOTIFYICON_DBCLICK);
			::SetTimer(_pPopupWin->m_hWnd, CC_TIMERID_NOTIFYICON_DBCLICK, NotifyIconConfig::nWaitTimerSeeIfDoubleClick, NULL);
			//if(_pTimer == NULL)
			//{
			//	_pTimer = new Timer();
			//	_pTimer->SetInterval(NotifyIconConfig::nWaitTimerSeeIfDoubleClick);
			//	_pTimer->Tick += EventHandler((Object*)this, (EventFun)&NotifyIcon::OnTick);
			//}
			////restart
			//if(_pTimer->GetEnabled())
			//{
			//	_pTimer->Stop();
			//}
			//_pTimer->Start();
		}
	}
	else if(e->Button == MouseButtons::Right)
	{
		//if(_eShowMenu == ShowMenu::RightClick || _eShowMenu == ShowMenu::RightLeftClick)
		if(CC_BIT_HAS(_Flags, CC_FLAG_NOTIFYICON_RIGHTCLICKMENU))
		{
			::KillTimer(_pPopupWin->m_hWnd, CC_TIMERID_NOTIFYICON_DBCLICK);
			if(_pContextMenu != NULL)
			{
				POINT pt;
				pt.x = e->X;
				pt.y = e->Y;
				_pPopupWin->Show();
				::SetForegroundWindow(_pPopupWin->m_hWnd);
				_pContextMenu->Show(_pPopupWin, pt);
				//WM_NULL msg is not need,for send WM_EXITMENULOOP at _pContextMenu->Show
				_pPopupWin->SetVisible(false);
			}
		}
	}
}
//
//void NotifyIcon::OnTick(Object* sender, EventArgs *e)
//{
//	if(_pContextMenu != NULL)
//	{
//		_pTimer->Stop();
//		_pPopupWin->Show();
//		::SetForegroundWindow(_pPopupWin->m_hWnd);
//		_pContextMenu->Show(NULL, _ptLeftClick);
//		PostMessage(_pPopupWin->m_hWnd, WM_NULL, 0, 0);
//		_pPopupWin->SetVisible(false);
//	}
//}

void NotifyIcon::AddAnimate(UINT nIDResource)
{
	AddAnimate(::LoadIcon(CC_APP_GetInstance(), MAKEINTRESOURCE(nIDResource)), true);
}

void NotifyIcon::AddAnimate(HICON hIcon, bool needDestroy)
{
	if(_lstAnimates == NULL)
	{
		//isSort, bIsDESC=false, isDistinct=false, nMinCapacity=0, nIncrement=1.3)
		_lstAnimates = new List<void*>(false, false, true);
		_lstAniFlg = new List<int>(false, false, false);
	}
	if(!_lstAnimates->ContainsKey((void*)hIcon))
	{
		_lstAnimates->Add((void*)hIcon);
		_lstAniFlg->Add((int)needDestroy);
	}
}

//nCycles while -1 for dead loop
void NotifyIcon::PlayStart(int nCycleMilliseconds, int nCycles)
{
	CC_ASSERT(CC_BIT_HAS(_Flags, CC_FLAG_NOTIFYICON_CREATED));

	if(CC_BIT_HAS(_Flags, CC_FLAG_NOTIFYICON_CREATED) && !CC_BIT_HAS(_Flags, CC_FLAG_NOTIFYICON_HIDDEN) && _lstAnimates != NULL)
	{
		//if(_pTimer2 == NULL)
		//{
		//	_pTimer2 = new Timer();
		//	_pTimer2->Tick += EventHandler((Object*)this, (EventFun)&NotifyIcon::OnTickAnimate);
		//}
		//if(_pTimer2->GetEnabled())
		//{
		//	_pTimer2->Stop();
		//}
		_nAnimateIndex = 0;
		_nCycles = nCycles;
		//_pTimer2->SetInterval(nCycleMilliseconds);
		//_pTimer2->Start();
		::KillTimer(_pPopupWin->m_hWnd, CC_TIMERID_NOTIFYICONL_ANIMATE);
		::SetTimer(_pPopupWin->m_hWnd, CC_TIMERID_NOTIFYICONL_ANIMATE, nCycleMilliseconds, NULL);
	}
}

void NotifyIcon::PlayStop()
{
	::KillTimer(_pPopupWin->m_hWnd, CC_TIMERID_NOTIFYICONL_ANIMATE);
	if(CC_BIT_HAS(_Flags, CC_FLAG_NOTIFYICON_CREATED) && !CC_BIT_HAS(_Flags, CC_FLAG_NOTIFYICON_HIDDEN))
	{
		//if(_pTimer2 != NULL)
		//{
		//	_pTimer2->Stop();
		//}
		//_dataNotify.uFlags = NIF_ICON;
		_dataNotify.hIcon = GetIcon();
		Shell_NotifyIcon(NIM_MODIFY, &_dataNotify);
	}
}
//
//void NotifyIcon::OnTickAnimate(Object* sender, EventArgs *e)
//{
//	if(_lstAnimates != NULL)
//	{
//		if(_nCycles >= 0)
//		{
//			_nCycles--;
//			if(_nCycles < 0)
//			{
//				_pTimer2->Stop();
//				return;
//			}
//		}
//		_nAnimateIndex++;
//		if(_nAnimateIndex >= _lstAnimates->GetSize())
//		{
//			_nAnimateIndex = 0;
//		}
//		//_dataNotify.uFlags = NIF_ICON;
//		_dataNotify.hIcon = (HICON)_lstAnimates->GetKey(_nAnimateIndex);
//		Shell_NotifyIcon(NIM_MODIFY, &_dataNotify);
//	}
//}

void NotifyIcon::_pPopupWin_Tick(Object* sender, EventArgs *e)
{
	if(e->WParam == CC_TIMERID_NOTIFYICON_DBCLICK)
	{
		::KillTimer(_pPopupWin->m_hWnd, CC_TIMERID_NOTIFYICON_DBCLICK);
		if(_pContextMenu != NULL)
		{
			_pPopupWin->Show();
			::SetForegroundWindow(_pPopupWin->m_hWnd);
			_pContextMenu->Show(NULL, _ptLeftClick);
			PostMessage(_pPopupWin->m_hWnd, WM_NULL, 0, 0);
			_pPopupWin->SetVisible(false);
		}
	}
	else if(e->WParam == CC_TIMERID_NOTIFYICONL_ANIMATE)
	{
		if(_lstAnimates != NULL)
		{
			if(_nCycles >= 0)
			{
				_nCycles--;
				if(_nCycles < 0)
				{
					::KillTimer(_pPopupWin->m_hWnd, CC_TIMERID_NOTIFYICONL_ANIMATE);
					//_pTimer2->Stop();
					return;
				}
			}
			_nAnimateIndex++;
			if(_nAnimateIndex >= _lstAnimates->GetSize())
			{
				_nAnimateIndex = 0;
			}
			//_dataNotify.uFlags = NIF_ICON;
			_dataNotify.hIcon = (HICON)_lstAnimates->GetKey(_nAnimateIndex);
			Shell_NotifyIcon(NIM_MODIFY, &_dataNotify);
		}
	}
	else if(e->WParam == CC_TIMERID_NOTIFYICONL_RESHOW)
	{
		::KillTimer(_pPopupWin->m_hWnd, CC_TIMERID_NOTIFYICONL_RESHOW);
		if(Shell_NotifyIcon(NIM_MODIFY, &_dataNotify) == FALSE)
		{
			if(Shell_NotifyIcon(NIM_ADD, &_dataNotify) == FALSE)
			{
				::SetTimer(_pPopupWin->m_hWnd, CC_TIMERID_NOTIFYICONL_RESHOW, 1000, NULL);
			}
		}
	}
}
// constructors/destructor
///////////////////////////////////////////////////////////////////////////////

//===========================================================cc::win::Hotkey
///////////////////////////////////////////////////////////////////////////////
// constructors/destructor
Hotkey::Hotkey()
{
	_Text = CC_StrCopy(&_Text, NULL);
	_Name = CC_StrCopy(&_Name, NULL);
	::SetRect(&_Rect, 0, 0, 0, 0);
	//::CopyRect(&_OrgRect, &_Rect);
	//_Visible = false;
	//CC_BIT_OFF(_Flags, CC_FLAG_CONTROL_VISIBLE);
	SetStyle(StyleType::Inner2, Inner2Style::Visible, 0);
#ifdef _DEBUG_
	SetName(_T("Hotkey"));
#endif
	_lstHotKey = NULL;
}

Hotkey::~Hotkey()
{
	Dispose();
	if(_lstHotKey != NULL)
	{
		delete _lstHotKey;
		_lstHotKey = NULL;
	}
}

void Hotkey::Dispose()
{
	if(!GetIsDispose())
	{
		if(_lstHotKey != NULL)
		{
			for(int n = _lstHotKey->GetSize() - 1; n >= 0; n--)
			{
				int nID = _lstHotKey->GetKey(n);
				Unregister((nID >>16), nID & 0xffff);
			}
			_lstHotKey->Clear();
		}
		Control::Dispose();
	}
}

void Hotkey::OnCreateControl()
{
	if(_lstHotKey != NULL)
	{
		for(int n = _lstHotKey->GetSize() - 1; n >= 0; n--)
		{
			int nID = _lstHotKey->GetKey(n);
			Register((nID >>16), nID & 0xffff);
		}
	}
}

LRESULT Hotkey::WndProc(EventArgs *e)
{
	switch(e->Msg)
	{
	case WM_HOTKEY:
		OnHotKey(e);
		break;
	}

	return Control::WndProc(e);
}

//MOD_ALT,MOD_CONTROL,MOD_KEYUP,MOD_SHIFT,MOD_WIN+VK_F2
bool Hotkey::Register(UINT fsModifiers, UINT vk)
{
	CC_ASSERT(::IsWindow(m_hWnd));

	if(_lstHotKey == NULL)
	{
		//isSort, bIsDESC=false, isDistinct=false, nMinCapacity=0, nIncrement=1.3)
		_lstHotKey = new List<int>(false, false, true);
	}
	if(!_lstHotKey->ContainsKey(vk | (fsModifiers <<16)))
	{
		_lstHotKey->Add(vk | (fsModifiers <<16));
	}
	return (::RegisterHotKey(m_hWnd, (vk | (fsModifiers <<16)), fsModifiers, vk) == TRUE);
}

bool Hotkey::Unregister(UINT fsModifiers, UINT vk)
{
	CC_ASSERT(::IsWindow(m_hWnd));

	if(_lstHotKey != NULL && _lstHotKey->ContainsKey(vk | (fsModifiers <<16)))
	{
		_lstHotKey->Remove(vk | (fsModifiers <<16));
		return (::UnregisterHotKey(m_hWnd, vk | (fsModifiers <<16)) == TRUE);
	}
	return false;
}

//e->WParam = vk | (fsModifiers <<16)
//HIWORD(e->LParam) == VK_F2; // virtual-key , LOWORD(e->LParam); // key-modifier flags 
void Hotkey::OnHotKey(EventArgs *e)
{
	HotKeyDown.Invoke((Object*)this, e);
}
// constructors/destructor
///////////////////////////////////////////////////////////////////////////////


//===========================================================cc::win::HotkeyInputCtl
///////////////////////////////////////////////////////////////////////////////
// constructors/destructor
HotkeyInputCtl::HotkeyInputCtl()
{
	isPressedOK = 0;
	m_fsModifiers = 0;
	m_vk = 0;
	SetText(_T(""));
#ifdef _DEBUG_
	SetName(_T("HotkeyInputCtl"));
#endif
}
HotkeyInputCtl::~HotkeyInputCtl()
{
}
// constructors/destructor
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// private/protected/internal used methods
int HotkeyInputCtl::GetHotkey(UINT& fsModifiers, UINT& vk)
{
	fsModifiers = m_fsModifiers;
	vk = m_vk;
	return 0;
}

void HotkeyInputCtl::SetHotkey(UINT fsModifiers, UINT vk)
{
	m_fsModifiers = fsModifiers;
	m_vk = vk;
}

cc::Str HotkeyInputCtl::GetHotkeyString(UINT fsModifiers, UINT vk)
{
	cc::Str str;
	if((fsModifiers & MOD_WIN) == MOD_WIN)
	{
		if(str.GetLength() > 0)
		{
			str.Append(_T(" + "));
		}
		str.Append(_T("Win"));
	}
	if((fsModifiers & MOD_CONTROL) == MOD_CONTROL)
	{
		if(str.GetLength() > 0)
		{
			str.Append(_T(" + "));
		}
		str.Append(_T("Ctrl"));
		fsModifiers |= MOD_CONTROL;
	}
	if((fsModifiers & MOD_ALT) == MOD_ALT)
	{
		if(str.GetLength() > 0)
		{
			str.Append(_T(" + "));
		}
		str.Append(_T("Alt"));
	}
	if(str.GetLength() > 0)
	{
		if((fsModifiers & MOD_SHIFT) == MOD_SHIFT)
		{
			str.Append(_T(" + Shift"));
		}

		TCHAR c = vk;
		if(c >= 112 && c <= 112+12)
		{
			//F1~F12
			str.AppendFormat(_T(" + F%d"), (int)(c-111));
		}
		else if(c >= '!' && c <= '}')
		{
			str.AppendFormat(_T(" + %C"), c);
		}
	}
	return str;
}

LRESULT HotkeyInputCtl::WndProc(EventArgs *e)
{
	if(e->Msg == WM_CHAR)
	{
		return 0;
	}
	if(e->Msg == WM_KEYUP)
	{
		if((::GetKeyState(VK_SHIFT) & 0x8000) == 0 && (::GetKeyState(VK_CONTROL) & 0x8000) == 0 && (::GetKeyState(VK_MENU) & 0x8000) == 0)
		{
			if(isPressedOK == 0)
			{
				this->SetText(_T(""));
				m_fsModifiers = 0;
				m_vk = 0;
			}
			isPressedOK = 0;
		}
		return 0;
	}
	if((isPressedOK == 0 || (e->WParam >= '!' && e->WParam <= '}')) && (e->Msg == WM_KEYDOWN || e->Msg == WM_KEYUP))
	{
		cc::Str str;
		UINT fsModifiers = 0;
		if(((::GetKeyState(VK_LWIN) | ::GetKeyState(VK_RWIN)) & 0x8000) != 0)
		{
			fsModifiers |= MOD_WIN;
		}
		if((::GetKeyState(VK_CONTROL) & 0x8000) != 0)
		{
			fsModifiers |= MOD_CONTROL;
		}
		if((::GetKeyState(VK_MENU) & 0x8000) != 0)
		{
			fsModifiers |= MOD_ALT;
		}
		if(fsModifiers != 0)
		{
			if((::GetKeyState(VK_SHIFT) & 0x8000) != 0)
			{
				fsModifiers |= MOD_SHIFT;
			}
		}

		m_fsModifiers = 0;
		m_vk = 0;
		//while WIN then e->WParam='['
		if(fsModifiers != 0 && (e->WParam >= '!' && e->WParam <= '}') && e->WParam != '[')
		{
			isPressedOK = 1;
			m_fsModifiers = fsModifiers;
			m_vk = LOWORD(e->WParam);
			str = GetHotkeyString(m_fsModifiers, m_vk);
		}
		else
		{
			str = GetHotkeyString(fsModifiers, 0);
		}
		this->SetText(str);
		return 0;
	}
	return TextBox::WndProc(e);
}

void HotkeyInputCtl::OnCreateControl()
{
	cc::Str str;
	if(m_fsModifiers != 0 && m_vk != 0)
	{
		str = GetHotkeyString(m_fsModifiers, m_vk);
	}
	this->SetText(str);
}
// private/protected/internal used methods
///////////////////////////////////////////////////////////////////////////////

DWORD IEControlConfig::nStyle = WS_CHILD | WS_VISIBLE | WS_TABSTOP;//WS_THICKFRAME
DWORD IEControlConfig::nExStyle = 0;
//DWORD IEControlConfig::nClassStyle = CS_DBLCLKS | CS_GLOBALCLASS | CS_PARENTDC;
DWORD IEControlConfig::nInnerStyle = 0;
DWORD IEControlConfig::nOuterStyle = OuterStyle::TransparentNoEraseBack;
DWORD IEControlConfig::nOwnerDrawTextStyle = DT_LEFT|DT_VCENTER|DT_SINGLELINE;
COLORREF IEControlConfig::corBackground = ::GetSysColor(COLOR_BTNFACE);
COLORREF IEControlConfig::corTextground = ::GetSysColor(COLOR_BTNTEXT);
COLORREF IEControlConfig::corHotBackColor = ::GetSysColor(COLOR_BTNFACE);
COLORREF IEControlConfig::corHotTextColor = ::GetSysColor(COLOR_BTNTEXT);
//===========================================================cc::win::IECtrl
///////////////////////////////////////////////////////////////////////////////
// constructors/destructor
IEControl::IEControl()
{
	BASE_CONTROL_CONFIG_INIT(IEControl);
	//_SetWindowPosFlags = 0;
	SetStyle(StyleType::Inner2, 0, Inner2Style::WindowPosFlags0);
//IHTMLDocument2* pDoc = (IHTMLDocument2* )GetHtmlDocument();
//pDoc->put_designMode(L"On"); 
	_Rect.right = _Rect.left + 84;
	_Rect.bottom = _Rect.top + 60;
	::CopyRect(&_OrgRect, &_Rect);
	_ClassName = WndClassName::IEControl;
	_StyleArray[(int)StyleType::InitControl] = CC_WNDCOMMCTLS_REG;
	m_pieBase = NULL;
	//ole object
	_pUIHandler = NULL;
	_pInPlaceSite = NULL;
	_pClientSite = NULL;
	_initURL = NULL;
	m_pIEEventHandler = NULL;
#ifdef _DEBUG_
	SetName(_T("IEControl"));
#endif
}

IEControl::~IEControl()
{
	if(m_pieBase != NULL)
	{
		delete m_pieBase;
		m_pieBase = NULL;
	}
	if(_initURL != NULL)
	{
		::SysFreeString(_initURL);
		_initURL = NULL;
	}
}
// constructors/destructor
///////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG_
// WndProc - calls appriate On... function for the give
// message
LRESULT IEControl::WndProc(EventArgs *e)
{
	return Control::WndProc(e);
}
#endif

void IEControl::OnPaint(PaintEventArgs* e)
{
	if(m_pieBase != NULL)
	{
		HWND hWnd = m_pieBase->getHWND();
		//::SendMessage(hWnd, WM_PAINT, 0, 0);
		::ShowWindow(hWnd, SW_HIDE);
		::ShowWindow(hWnd, SW_SHOW);
	}
	Control::OnPaint(e);
}

//bool setLastControl is if have child,set first of last
bool IEControl::Focus(bool setLastControl)
{
	CC_ASSERT(m_pieBase != NULL);

	return m_pieBase->SetFocus();
}

void IEControl::SetOleObject(ie_DocHandler *pUIHandler, ie_InPlaceSite *pInPlaceSite, ie_ClientSite *pClientSite, BSTR initURL)
{
	//ole object
	_pUIHandler = pUIHandler;
	_pInPlaceSite = pInPlaceSite;
	_pClientSite = pClientSite;
	if(_initURL != NULL)
	{
		::SysFreeString(_initURL);
		_initURL = NULL;
	}
	if(initURL != NULL)
	{
		_initURL = ::SysAllocString(initURL);
	}
}

bool IEControl::PreProcessMessage(MSG* pMsg, UINT_PTR& keyData)
{
	CC_ASSERT(m_pieBase != NULL);

	if(m_pieBase->TranslateAccelerator(pMsg))
	{
		return true;
	}
	return Control::PreProcessMessage(pMsg, keyData);
}

//here to create window
void IEControl::CreateHandle()
{
	Control::CreateHandle();
	if(m_hWnd != NULL)
	{
		if(m_pieBase == NULL)
		{
			m_pieBase = new IEBase();
		}
		if(m_pIEEventHandler != NULL)
		{
			m_pieBase->SetEventHandler(m_pIEEventHandler);
		}
		m_pieBase->OnCreate(m_hWnd, CC_APP_GetInstance(), _pUIHandler, _pInPlaceSite, _pClientSite, _initURL);
		OnResize(NULL);
	}
}

void IEControl::OnResize(EventArgs* e)
{
	Resize.Invoke((Object*)this, e);
	if(m_pieBase != NULL)
	{
		RECT m_Rect;
		GetClientRect(m_hWnd, &m_Rect);

#ifdef _DEBUG_
		if(CC_APP_GetConfig()->GetIsDebug())
		{
			CC_TRACE((_T("IEControl::OnResize[%s,%s]NewL:%d, NewT:%d, NewW:%d, NewH:%d"),
				this->_ClassName, this->_Name, m_Rect.left, m_Rect.top, m_Rect.right - m_Rect.left, m_Rect.bottom - m_Rect.top));
		}
#endif

		m_pieBase->setPosition(m_Rect);
	}
}

bool IEControl::Navigate(BSTR url)
{
	CC_ASSERT(m_pieBase != NULL);

	return m_pieBase->Navigate(url);
}

//can NOT Release() it
ie_IWebBrowser2* IEControl::GetWebBrowser()
{
	CC_ASSERT(m_pieBase != NULL);

	return m_pieBase->pWebBrowser;
}

//can NOT Release() it
HWND IEControl::GetWebBrowserHWND()
{
	CC_ASSERT(m_pieBase != NULL);

	return m_pieBase->getHWND();
}

//need Release() out of this
ie_IHTMLDocument2* IEControl::GetDocument()
{
	CC_ASSERT(m_pieBase != NULL);

	return m_pieBase->getDocument();
}

//BSTR bstrCode = ::SysAllocString(L"alert(\"sss\");document.write(\"dddddd\");");
//ie1->ExecScript(bstrCode);
//::SysFreeString(bstrCode);
bool IEControl::ExecScript(BSTR code, BSTR language)
{
	CC_ASSERT(m_pieBase != NULL);

	return m_pieBase->execScript(code, language);
}

//BSTR bstrCode = ::SysAllocString(L"text<br>text2<br>");
//ie1->Write(bstrCode);
//::SysFreeString(bstrCode);
bool IEControl::Write(BSTR txt, bool isAppend)
{
	CC_ASSERT(m_pieBase != NULL);

	return m_pieBase->write(txt, isAppend);
}

bool IEControl::IsComplete()
{
	CC_ASSERT(m_pieBase != NULL);

	return m_pieBase->isComplete();
}

bool IEControl::LoadText(const void* txt, DWORD nLength)
{
	CC_ASSERT(m_pieBase != NULL);

	return m_pieBase->loadText(txt, nLength);
}

//need free point out this fun
bool IEControl::SaveText(void** txt, DWORD& nLength)
{
	CC_ASSERT(m_pieBase != NULL);

	return m_pieBase->saveText(txt, nLength);
}

bool IEControl::SetEventHandler(ie_EventHandler *pIEEventHandler)
{
	m_pIEEventHandler = pIEEventHandler;
	if(m_pieBase != NULL)
	{
		return m_pieBase->SetEventHandler(m_pIEEventHandler);
	}
	return false;
}

