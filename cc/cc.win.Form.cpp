// cc.win.Form.h
// The C# like Form class
// Author: Shu.KK
// 2006-01-01
#include "cc.win.Form.h"
using namespace cc;
using namespace cc::win;


DWORD MainMenuBarConfig::nStyle = WS_CHILD | WS_VISIBLE | TBSTYLE_TOOLTIPS | TBSTYLE_FLAT | CCS_NODIVIDER | CCS_NORESIZE;
DWORD MainMenuBarConfig::nExStyle = WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR | WS_EX_NOPARENTNOTIFY;
//DWORD MainMenuBarConfig::nClassStyle = CS_DBLCLKS | CS_GLOBALCLASS | CS_SAVEBITS;
//need DeliverFormMessage to me,not need TranceContextMenuUp
DWORD MainMenuBarConfig::nInnerStyle = InnerStyle::TrackMouse|InnerStyle::DeliverFormMessage;
DWORD MainMenuBarConfig::nOuterStyle = 0;
DWORD MainMenuBarConfig::nOwnerDrawTextStyle = DT_LEFT|DT_VCENTER|DT_SINGLELINE;
//also need set at OnSysColorChanged
COLORREF MainMenuBarConfig::corBackground = ::GetSysColor(COLOR_BTNFACE);
COLORREF MainMenuBarConfig::corTextground = ::GetSysColor(COLOR_MENUTEXT);
COLORREF MainMenuBarConfig::corHotBackColor = ::GetSysColor(COLOR_HIGHLIGHT);
COLORREF MainMenuBarConfig::corHotTextColor = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
int MainMenuBarConfig::nMarginLeft = 3;
int MainMenuBarConfig::nMarginTop = 1;
int MainMenuBarConfig::nMarginRight = 3;
int MainMenuBarConfig::nMarginBottom = 1;

#if !defined(_CCTHINFORM)
//===========================================================cc::win::MainMenuBar
///////////////////////////////////////////////////////////////////////////////
// constructors/destructor
MainMenuBar::MainMenuBar()
{
	BASE_CONTROL_CONFIG_INIT(MainMenuBar);
	::SetRect(&_Margin, MainMenuBarConfig::nMarginLeft, MainMenuBarConfig::nMarginTop, MainMenuBarConfig::nMarginRight, MainMenuBarConfig::nMarginBottom);
	_Rect.right = _Rect.left + 100;
	_Rect.bottom = _Rect.top + 20;
	//_ClassName = WndClassName::Container;
#ifdef _DEBUG_
	SetText(_T("MainMenuBar"));
	SetName(_T("MainMenuBar"));
#endif

	Dock = (DockStyles::Type)(DockStyles::Top | DockStyles::Left | DockStyles::Right);
	Anchor = (AnchorStyles::Type)(AnchorStyles::Left | AnchorStyles::Right);

	_pMainMenu = NULL;
	m_eTrackingState = TR_NONE;
	_ItemsRect = NULL;
	m_nHotIndex = -1;
	_nNewIndex = -1;
	_pTopLevelForm = NULL;
#if defined(_DEBUG_) || defined(_DESIGN)
	//code for design
	//flag for moveable or others
	Design = (DesignStyle::Type)(CC_DESIGN_NO_MOVE|CC_DESIGN_NO_COLOR);
#endif
}

MainMenuBar::~MainMenuBar()
{
	//_pMainMenu is delete at form
	if(_ItemsRect != NULL)
	{
		delete _ItemsRect;
		_ItemsRect = NULL;
	}
}
// constructors/destructor

void MainMenuBar::DoUpdate()
{
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreatedEnd) != 0 && IsUpdate((UpdateFlag::Type)(UpdateFlag::UpdateFlag)))
	{
		if(_ItemsRect != NULL)
		{
			delete _ItemsRect;
			_ItemsRect = NULL;
		}
	}
	UpdateMainMenu();
	//clear flages
	ToolBar::DoUpdate();
}

void MainMenuBar::UpdateMainMenu()
{
	if(m_hWnd != NULL && (_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreatedEnd) != 0 && ((_pMainMenu != NULL && _pMainMenu->GetIsNeedUpdate()) || _ItemsRect == NULL))
	{
		if(_ItemsRect != NULL)
		{
			delete _ItemsRect;
			_ItemsRect = NULL;
		}
		_pMainMenu->CreateControl();
		CC_BIT_ON(_Flags, CC_FLAG_MAINMENUBAR_PROCESSRIGHTARROW);
		CC_BIT_ON(_Flags, CC_FLAG_MAINMENUBAR_PROCESSLEFTARROW);
		CC_BIT_OFF(_Flags, CC_FLAG_MAINMENUBAR_IGNOREALTKEY);

		HDC hDC = ::GetDC(m_hWnd);
		HFONT hOldFont = (HFONT)::SelectObject(hDC, GetMenuFont());

		int nCnt = _pMainMenu->ItemCount();
		_ItemsRect = new RECT[nCnt];
		RECT* pRC = _ItemsRect;
		//int nLeft = GetPreWidth();
		int nLeft = 0;
		int nH = 0;
		for(int i = 0; i < nCnt; i++)
		{
			SetRectEmpty(pRC);
			MenuItem* item = (MenuItem*)_pMainMenu->ItemGet(i);
			MEASUREITEMSTRUCT entity = {0};
			EventArgs e(0, 0, 0, 0);
			MeasureItemEventArgs ee(&e);
			ee.data = &entity;
			ee.hDC = hDC;
			CC_APP_GetDrawActor()->MeasureMenu(item, &ee);
			nH = CC_MAX(nH, (int)ee.data->itemHeight);
			pRC->top = 2;
			pRC->bottom = ee.data->itemHeight;
			pRC->left = nLeft;
			//the width of menu has plus CC_MAINMENUBAR_MEARGIN
			nLeft += ee.data->itemWidth + CC_MAINMENUBAR_MEARGIN * 2;
			pRC->right = nLeft;
			pRC++;
		}
		//_Rect.right = _Rect.left + nLeft;
		//_Rect.bottom = _Rect.top + nH + 2;
		SetBoundsNotOriginal(_Rect.left, _Rect.top, nLeft, nH + 2, BoundsSpecified::Size);

		::SelectObject(hDC, hOldFont);
		::ReleaseDC(m_hWnd, hDC);
		::InvalidateRect(m_hWnd, NULL, TRUE);
	}
}

HFONT MainMenuBar::GetMenuFont()
{
	return CC_APP_GetDrawActor()->GetMenuFont();
}

int MainMenuBar::GetItemCount() const
{
	return (_pMainMenu == NULL ? 0 : _pMainMenu->ItemCount());
}

int MainMenuBar::PointToIndex(POINT pt) const
{
	int nCnt = GetItemCount();
	for(int i = 0; i < nCnt; i++)
	{
		RECT* pRC = _ItemsRect + i;
		if(::PtInRect(pRC, pt))
		{
			return i;
		}
	}
	return -1;
}

void MainMenuBar::SetHit(int index, HDC hDC, bool isOn)
{
	if(index < 0 || index >= GetItemCount())
	{
		return;
	}
	RECT* pRC = _ItemsRect + index;
	MenuItem* item = (MenuItem*)_pMainMenu->ItemGet(index);
	DRAWITEMSTRUCT entity;
	ZeroMemory(&entity, sizeof(DRAWITEMSTRUCT));
	EventArgs e(0, 0, 0, 0);
	DrawItemEventArgs ee(&e);
	ee.data = &entity;
	entity.hDC = hDC;
	CopyRect(&entity.rcItem, pRC);
	entity.itemState = isOn ? ODS_SELECTED : 0;
	entity.itemAction = ODA_SELECT;
	CC_APP_GetDrawActor()->DrawMenuTitle(item, &ee);
}

void MainMenuBar::SetHotHit(int index)
{
	if(m_nHotIndex == index)
	{
		return;
	}

	HDC hDC = ::GetDC(m_hWnd);
	HFONT hOldFont = (HFONT)::SelectObject(hDC, GetMenuFont());

	if(m_nHotIndex != -1)
	{
		SetHit(m_nHotIndex, hDC, false);
	}
	if(index != -1)
	{
		SetHit(index, hDC, true);
	}

	::SelectObject(hDC, hOldFont);
	::ReleaseDC(m_hWnd, hDC);
	m_nHotIndex = index;
}

MainMenuBar* MainMenuBar::cc_win_pMenuBar = NULL;
static HHOOK cc_win_hMsgHook = NULL;
void MainMenuBar::ShowMenu(int index)
{
	if(index < 0 || index >= GetItemCount())
	{
		return;
	}

	_nNewIndex = index;
	while(_nNewIndex != -1)
	{
		UpdateMainMenu();
		m_eTrackingState = TR_POPUP;
		MenuItem* item = (MenuItem*)_pMainMenu->ItemGet(_nNewIndex);
		RECT* pRC = _ItemsRect + _nNewIndex;
		POINT pt;
		pt.x = pRC->left;
		pt.y = pRC->bottom;
		::ClientToScreen(m_hWnd, &pt);
		UINT _uTrackPopupMenuFlag = TPM_LEFTALIGN;

		cc_win_pMenuBar = this;
		cc_win_hMsgHook = ::SetWindowsHookEx(WH_MSGFILTER, MenuInputFilter, CC_APP_GetInstance(), ::GetCurrentThreadId());
		SetHotHit(_nNewIndex);
		_nNewIndex = -1;
		CC_BIT_ON(_Flags, CC_FLAG_MAINMENUBAR_PROCESSRIGHTARROW);
		CC_BIT_ON(_Flags, CC_FLAG_MAINMENUBAR_PROCESSLEFTARROW);

		::TrackPopupMenu(item->m_hMenu, _uTrackPopupMenuFlag, pt.x, pt.y, 0, m_hWnd, NULL);
		::UnhookWindowsHookEx(cc_win_hMsgHook);
		::SendMessage(m_hWnd, WM_EXITMENULOOP, TRUE, 0);
		cc_win_pMenuBar = NULL;
	}
}

#define TY_HOOK 0
#define TY_PRE 1
#define TY_FORM 2
LRESULT CALLBACK MainMenuBar::MenuInputFilter(int code, WPARAM wParam, LPARAM lParam)
{
	if(code == MSGF_MENU && cc_win_pMenuBar != NULL)
	{
		MSG* msg = (MSG*)lParam;
#ifdef _DEBUG_
			if(CC_APP_GetConfig()->GetIsDebug())
			{
				CC_LogWndMsg(cc_win_pMenuBar->m_hWnd, msg->message, msg->wParam, msg->lParam, cc_win_pMenuBar->GetWndClassName(), cc_win_pMenuBar->GetName());
			}
#endif
		if(cc_win_pMenuBar->MenuMessageCenter(msg->message, msg->wParam, msg->lParam, 0, TY_HOOK))
		{
			return 1;
		}
	}
	return ::CallNextHookEx(cc_win_hMsgHook, code, wParam, lParam);
}

bool MainMenuBar::PreFormProcessMessage(MSG* pMsg, UINT_PTR& keyData)
{
	if(_pTopLevelForm != NULL && !_pTopLevelForm->Focused())
	{
		return false;
	}
	if(pMsg->message == WM_KEYDOWN || pMsg->message == WM_SYSKEYDOWN)
	{
		if(_pMainMenu != NULL && _pMainMenu->ProcessCmdKey(pMsg, keyData))
		{
			return true;
		}
	}

	return MenuMessageCenter(pMsg->message, pMsg->wParam, pMsg->lParam, keyData, TY_PRE);
}

bool MainMenuBar::MenuMessageCenter(UINT nMsg, WPARAM wParam, LPARAM lParam, UINT_PTR keyData, int nType)
{
	if(m_eTrackingState == TR_MENU)
	{
		if((nMsg == WM_LBUTTONDOWN || nMsg == WM_RBUTTONDOWN) && !GetIsMouseOn())
		{
			SetHotHit(-1);
			m_eTrackingState = TR_NONE;
		}
	}
	else if(m_eTrackingState == TR_POPUP)
	{
		if(nMsg == WM_LBUTTONDOWN || nMsg == WM_RBUTTONDOWN)
		{
			POINT pt;
			//pt.x = ((int)(short)LOWORD(lParam));
			//pt.y = ((int)(short)HIWORD(lParam));
			pt.x = (int)GET_X_LPARAM(lParam);
			pt.y = (int)GET_Y_LPARAM(lParam);
			if(m_hWnd == ::WindowFromPoint(pt))
			{
				::ScreenToClient(m_hWnd, &pt);
				if(PointToIndex(pt) >= 0)
				{
					CC_BIT_ON(_Flags, CC_FLAG_MAINMENUBAR_ISCALCELBYMCLK);
				}
			}
		}
	}

	switch (nMsg)
	{
	case WM_MOUSEMOVE:
		{
			if(m_eTrackingState == TR_POPUP)
			{
				POINT pt;
				pt.x = (int)GET_X_LPARAM(lParam);
				pt.y = (int)GET_Y_LPARAM(lParam);
				if(m_hWnd == ::WindowFromPoint(pt))
				{
					::ScreenToClient(m_hWnd, &pt);
					int nIndex = PointToIndex(pt);
					if(nIndex >= 0 && nIndex != m_nHotIndex)
					{
						_nNewIndex = nIndex;
						// destroy popupped menu
						::PostMessage(m_hWnd, WM_CANCELMODE, 0, 0);
						return true;
					}
				}
			}
		}
		break;
	case WM_SYSKEYDOWN:
	case WM_KEYDOWN:
	case WM_SYSKEYUP:
		return MenuMessageOnKey(nMsg, wParam, lParam, keyData, nType);
	}
	return false;
}

bool MainMenuBar::MenuMessageOnKey(UINT nMsg, WPARAM wParam, LPARAM lParam, UINT_PTR& keyData, int nType)
{
	bool isCtrl;
	bool isShift;
	if(nType == TY_PRE)
	{
		isCtrl = ((keyData & SHORTCUT_CTRL) != 0);
		isShift = ((keyData & SHORTCUT_SHIFT) != 0);
	}
	else
	{
		isCtrl = ((GetKeyState(VK_CONTROL) & 0x8000) != 0);
		isShift = ((GetKeyState(VK_SHIFT) & 0x8000) != 0);
	}
	bool isAlt = ((HIWORD(lParam) & KF_ALTDOWN) != 0);
	// + X key
	TCHAR vkey = (TCHAR)wParam;
	if(vkey == VK_MENU || (vkey == VK_F10 && (!isShift || isCtrl || isAlt)))
	{
		if(m_eTrackingState == TR_POPUP)
		{
			//come to here is Hook,then next PreFormProcessMessage go here should not show menu
			CC_BIT_ON(_Flags, CC_FLAG_MAINMENUBAR_IGNOREALTKEY);
			return false;
		}
		// only alt key pressed,and no capture
		if(nMsg == WM_SYSKEYUP && ::GetCapture() == NULL)
		{
			//if last alt key had canced menu,not trace it again
			if(CC_BIT_HAS(_Flags, CC_FLAG_MAINMENUBAR_IGNOREALTKEY))
			{
				CC_BIT_OFF(_Flags, CC_FLAG_MAINMENUBAR_IGNOREALTKEY);
				return true;
			}
			switch (m_eTrackingState)
			{
			case TR_NONE:
				m_eTrackingState = TR_MENU;
				SetHotHit(0);
				//should focus to this control
				this->Focus();
				return true;
			case TR_POPUP:
			case TR_MENU:
				m_eTrackingState = TR_NONE;
				SetHotHit(-1);
				return true;
			}
		}
		return false;
	}
	else if((nMsg == WM_SYSKEYDOWN || nMsg == WM_KEYDOWN))
	{
		if(m_eTrackingState == TR_MENU)
		{
			switch (vkey)
			{
			case VK_LEFT:
				{
					int nIndex = m_nHotIndex - 1;
					if(nIndex < 0)
					{
						nIndex = GetItemCount() - 1;
					}
					SetHotHit(nIndex);
				}
				return true;
			case VK_RIGHT:
				{
					int nIndex = m_nHotIndex + 1;
					if(nIndex >= GetItemCount())
					{
						nIndex = 0;
					}
					SetHotHit(nIndex);
				}
				return true;
			case VK_RETURN:
			case VK_UP:
			case VK_DOWN:
				ShowMenu(m_nHotIndex);
				return true;
			case VK_ESCAPE:
				m_eTrackingState = TR_NONE;
				SetHotHit(-1);
				return true;
			}
		}
		else if(m_eTrackingState == TR_POPUP)
		{
			switch (vkey)
			{
			case VK_LEFT:
				{
					if(!CC_BIT_HAS(_Flags, CC_FLAG_MAINMENUBAR_PROCESSLEFTARROW))
					{
						return false;
					}
					//let wndproc to showmenu,cannot do it here after WM_CANCELMODE
					_nNewIndex = m_nHotIndex - 1;
					if(_nNewIndex < 0)
					{
						_nNewIndex = GetItemCount() - 1;
					}
					//destroy popupped menu
					::PostMessage(m_hWnd, WM_CANCELMODE, 0, 0);
				}
				return true;
			case VK_RIGHT:
				{
					if(!CC_BIT_HAS(_Flags, CC_FLAG_MAINMENUBAR_PROCESSRIGHTARROW))
					{
						return false;
					}
					//let wndproc to showmenu,cannot do it here after WM_CANCELMODE
					_nNewIndex = m_nHotIndex + 1;
					if(_nNewIndex >= GetItemCount())
					{
						_nNewIndex = 0;
					}
					//destroy popupped menu
					::PostMessage(m_hWnd, WM_CANCELMODE, 0, 0);
				}
				return true;
			case VK_ESCAPE:
				//SetHotHit(-1);
				CC_BIT_ON(_Flags, CC_FLAG_MAINMENUBAR_ISCALCELBYESC);
				return false;
			}
			return false;
		}

		//popup system menu
		if((m_eTrackingState == TR_MENU && vkey == VK_SPACE)
			|| (isAlt && vkey == VK_SPACE && m_eTrackingState != TR_POPUP))
		{
			if(_pParent != NULL)
			{
				//Undocument message ID
				#define WM_POPUPSYSTEMMENU 0x0313
				POINT pt = {0, 0};
				::ClientToScreen(_pParent->m_hWnd, &pt);
				::DefWindowProc(_pParent->m_hWnd, WM_POPUPSYSTEMMENU, 0, MAKELPARAM(pt.x, pt.y));
				return true;
			}
		}

		// Alt + X pressed
		if((isAlt || m_eTrackingState == TR_MENU) && _istalnum(vkey))
		{
			LRESULT lr = _pMainMenu->FindHotKey(vkey);
			if(lr)
			{
				ShowMenu(LOWORD(lr));
				return true;    // eat it!
			}
		}
	}
	return false;
}

void MainMenuBar::OnMouseLeave(EventArgs* e)
{
	ToolBar::OnMouseLeave(e);
	if(m_eTrackingState == TR_NONE)
	{
		SetHotHit(-1);
	}
}

void MainMenuBar::OnMouseMove(MouseEventArgs* e)
{
	if(_pTopLevelForm != NULL && !_pTopLevelForm->Focused())
	{
		return;
	}
	ToolBar::OnMouseMove(e);
	POINT pt = {e->X, e->Y};
	int nIndex = PointToIndex(pt);
	if(nIndex != m_nHotIndex && (m_eTrackingState == TR_NONE || nIndex >= 0))
	{
		SetHotHit(nIndex);
	}
}

// WndProc - calls appriate On... function for the give
// message
LRESULT MainMenuBar::WndProc(EventArgs *e)
{
	switch(e->Msg)
	{
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hDC = BeginPaint(m_hWnd, &ps);
			RECT* pRC = NULL;
			HFONT hOldFont = (HFONT)::SelectObject(hDC, GetMenuFont());

			int nCnt = GetItemCount();
			for(int i = 0; i < nCnt; i++)
			{
				SetHit(i, hDC, (i == m_nHotIndex));
			}
			::SelectObject(hDC, hOldFont);
			::ReleaseDC(m_hWnd, hDC);
			EndPaint(m_hWnd, &ps);
		}
		break;
	case WM_LBUTTONDOWN:
		{
			if(CC_BIT_HAS(_Flags, CC_FLAG_MAINMENUBAR_ISCALCELBYMCLK))
			{
				CC_BIT_OFF(_Flags, CC_FLAG_MAINMENUBAR_ISCALCELBYMCLK);
			}
			else
			{
				POINT pt;
				//pt.x = ((int)(short)LOWORD(e->LParam));
				//pt.y = ((int)(short)HIWORD(e->LParam));
				pt.x = (int)GET_X_LPARAM(e->LParam);
				pt.y = (int)GET_Y_LPARAM(e->LParam);
				int nIndex = PointToIndex(pt);
				SetHotHit(nIndex);
				if(nIndex != -1)
				{
					ShowMenu(nIndex);
				}
			}
		}
		return 0;
	case WM_COMMAND:
		{
			SetHotHit(-1);
			m_eTrackingState = TR_NONE;
		}
		break;
	case WM_MENUSELECT:
		{
			HMENU hMenu = (HMENU)e->LParam;
			UINT nIndex = (UINT)LOWORD(e->WParam);
			Menu* menu = Menu::FromHandle(hMenu);
			CC_BIT_SET(_Flags, CC_FLAG_MAINMENUBAR_PROCESSRIGHTARROW, (::GetSubMenu(hMenu, nIndex) == NULL));
			CC_BIT_SET(_Flags, CC_FLAG_MAINMENUBAR_PROCESSLEFTARROW, (_pMainMenu->ItemGetIndex(menu) >= 0));
		}
		break;
	case WM_INITMENUPOPUP:
		{
			HMENU hMenu = (HMENU)e->WParam;
			Menu* menu = Menu::FromHandle(hMenu);
			CC_BIT_ON(_Flags, CC_FLAG_MAINMENUBAR_PROCESSRIGHTARROW);
			CC_BIT_SET(_Flags, CC_FLAG_MAINMENUBAR_PROCESSLEFTARROW, (_pMainMenu->ItemGetIndex(menu) >= 0));
		}
		break;
	case WM_UNINITMENUPOPUP:
		{
			HMENU hMenu = (HMENU)e->WParam;
			Menu* menu = Menu::FromHandle(hMenu);
			if(_pMainMenu->ItemGetIndex(menu) >= 0)
			{
				if(CC_BIT_HAS(_Flags, CC_FLAG_MAINMENUBAR_ISCALCELBYESC))
				{
					m_eTrackingState = TR_MENU;
				}
				else
				{
					SetHotHit(-1);
					m_eTrackingState = TR_NONE;
				}
			}
			if(CC_BIT_HAS(_Flags, CC_FLAG_MAINMENUBAR_ISCALCELBYESC))
			{
				CC_BIT_OFF(_Flags, CC_FLAG_MAINMENUBAR_ISCALCELBYESC);
			}
		}
		break;
	}

	return ToolBar::WndProc(e);
}

void MainMenuBar::OnCreateControl()
{
	ToolBar::OnCreateControl();

	UpdateMainMenu();
	_pTopLevelForm = (Control*)::SendMessage(m_hWnd, CC_WM_MAINMSG, CC_WM_MAINMSG_WPARAM_GETFORM, 0);
	return;
}

void MainMenuBar::SetMainMenu(MainMenu* mainMenu)
{
	_pMainMenu = mainMenu;
	UpdateMainMenu();
}

MainMenu* MainMenuBar::GetMainMenu()
{
	return _pMainMenu;
}


void MainMenuBar::DoSetLang(KeyArr& keyArr, cc::Str sThisHead)
{
	ToolBar::DoSetLang(keyArr, sThisHead);
	if(_pMainMenu != NULL)
	{
		_pMainMenu->SetLang(keyArr, sThisHead);
		UpdateMainMenu();
	}
}
// cc::win::MainMenu
///////////////////////////////////////////////////////////////////////////////
#endif //#if !defined(_CCTHINFORM)

DWORD UserControlConfig::nStyle = WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS|WS_DLGFRAME;
DWORD UserControlConfig::nExStyle = 0;
//DWORD UserControlConfig::nClassStyle = CS_DBLCLKS | CS_GLOBALCLASS;
DWORD UserControlConfig::nInnerStyle = InnerStyle::TranceContextMenuUp;
DWORD UserControlConfig::nOuterStyle = 0;
DWORD UserControlConfig::nOwnerDrawTextStyle = DT_LEFT|DT_VCENTER|DT_SINGLELINE;
COLORREF UserControlConfig::corBackground = ::GetSysColor(COLOR_BTNFACE);
COLORREF UserControlConfig::corTextground = ::GetSysColor(COLOR_BTNTEXT);
COLORREF UserControlConfig::corHotBackColor = ::GetSysColor(COLOR_BTNFACE);
COLORREF UserControlConfig::corHotTextColor = ::GetSysColor(COLOR_BTNTEXT);
//===========================================================cc::win::UserControl
///////////////////////////////////////////////////////////////////////////////
// constructors/destructor
UserControl::UserControl() : Container()
{
	BASE_CONTROL_CONFIG_INIT(UserControl);
	//use window default pos
	_Rect.left = 0;
	_Rect.top = 0;
	_Rect.right = 200;
	_Rect.bottom = 100;
	::CopyRect(&_OrgRect, &_Rect);

	//set a default value for the wnd caption
	SetText(_T("UserControl"));
	//set a default value for the wnd class name
	_ClassName = WndClassName::UserControl;
#ifdef _DEBUG_
	SetName(_T("UserControl"));
#endif
#if defined(_DEBUG_) || defined(_DESIGN)
	//code for design
	//flag for moveable or others
	Design = (DesignStyle::Type)(DesignStyle::CanAddControl);
#endif
}

UserControl::~UserControl()
{
}
// constructors/destructor
///////////////////////////////////////////////////////////////////////////////

DWORD TabPageConfig::nStyle = WS_OVERLAPPED | WS_CHILD | WS_VISIBLE | WS_TABSTOP | DS_SETFONT | DS_3DLOOK | DS_CONTROL;
DWORD TabPageConfig::nExStyle = WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR | WS_EX_NOPARENTNOTIFY;
//DWORD TabPageConfig::nClassStyle = CS_DBLCLKS | CS_GLOBALCLASS;
//if has InnerStyle::CreateDialog then while in a dialog will CreateDialogIndirect inner CreateDialogIndirect,so dead!!!
DWORD TabPageConfig::nInnerStyle = InnerStyle::TranceContextMenuUp|InnerStyle::BackgroundTheme;
DWORD TabPageConfig::nOuterStyle = OuterStyle::TransparentTop;
DWORD TabPageConfig::nOwnerDrawTextStyle = DT_LEFT|DT_VCENTER|DT_SINGLELINE;
COLORREF TabPageConfig::corBackground = ::GetSysColor(COLOR_BTNFACE);
COLORREF TabPageConfig::corTextground = ::GetSysColor(COLOR_BTNTEXT);
//only for parent that owner draw(like TabControlEx)
//notice that here when activate is corBackground,not active is corHotBackColor!
COLORREF TabPageConfig::corHotBackColor = (COLORREF)((UINT)TabPageConfig::corBackground - 0x00111111);
//notice that here when activate is Textground,not active is HotTextColor!
COLORREF TabPageConfig::corHotTextColor = (COLORREF)((UINT)TabPageConfig::corTextground + 0x00212121);
//not used anyaway
int TabPageConfig::nMarginLeft = 4;
int TabPageConfig::nMarginTop = 2;
int TabPageConfig::nMarginRight = 4;
int TabPageConfig::nMarginBottom = 1;
//===========================================================cc::win::TabPage
///////////////////////////////////////////////////////////////////////////////
// constructors/destructor
TabPage::TabPage()
{
	BASE_CONTROL_CONFIG_INIT(TabPage);
	::SetRect(&_Margin, TabPageConfig::nMarginLeft, TabPageConfig::nMarginTop, TabPageConfig::nMarginRight, TabPageConfig::nMarginBottom);
	::SetRect(&_Rect, 0, 0, 100, 100);
	::CopyRect(&_OrgRect, &_Rect);
	_ClassName = WndClassName::Dialog32770;
	SetText(_T("TabPage"));
	PageData = NULL;

#ifdef _DEBUG_
	SetName(_T("TabPage"));
#endif
	_hFocusBeforeDeactivate = NULL;
#if defined(_DEBUG_) || defined(_DESIGN)
	//code for design
	//flag for moveable or others
	Design = (DesignStyle::Type)(CC_DESIGN_NO_MOVE | CC_DESIGN_NO_COLOR | DesignStyle::CanAddControl);
#endif
}

///////////////////// destructor //////////////////////////
TabPage::~TabPage()
{
}
// constructors/destructor

void TabPage::DoUpdate()
{
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreatedEnd) != 0)
	{
		bool isUpdate = false;
		if(IsUpdate((UpdateFlag::Type)(UpdateFlag::UpdateText|UpdateFlag::UpdateIcon|UpdateFlag::UpdateEnabled))
			&& _pParent != NULL && !_pParent->HasStyle(StyleType::Outer, OuterStyle::Sizing) && (_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreatedEnd) != 0)
		{
			isUpdate = true;
		}
		//first clear flag
		Container::DoUpdate();
		//then update parent
		if(isUpdate)
		{
			//parent see this to DoModPage
			_nUpdateFlag |= UpdateFlag::UpdateFlag;
			_pParent->Update();
			_nUpdateFlag = 0;
		}
	}
}

bool TabPage::SetActivateFocus(HWND _hFocus)
{
	if(_hFocus == NULL)
	{
		_hFocus = ::GetFocus();
	}
	if(_hFocus != NULL && ::IsChild(m_hWnd, _hFocus))
	{
		_hFocusBeforeDeactivate = _hFocus;
		return true;
	}
	return false;
}

void TabPage::OnVisibleChanged(EventArgs *e)
{
	//wParam:Specifies whether a window is being shown.
	//  If wParam is TRUE, the window is being shown.
	//  If wParam is FALSE, the window is being hidden.
	if(e != NULL)
	{
		if((BOOL)e->WParam == TRUE)
		{
			//while Focus is parent,do nothing
			if(::GetFocus() != ::GetParent(m_hWnd))
			{
				if(_hFocusBeforeDeactivate == NULL || ::SetFocus(_hFocusBeforeDeactivate) == NULL)
				{
					//if not success,set to first tabIndex
					Focus();
				}
			}
		}
		else
		{
			SetActivateFocus();
		}
	}
	Container::OnVisibleChanged(e);
}
///////////////////////////////////////////////////////////////////////////////
//===========================================================cc::win::TabPage

///////////////////////////////////////////////////////////////////////////////
//===========================================================cc::win::TabControl
/*
Tab Control Styles
    TCS_BOTTOM 
        Version 4.70. Tabs appear at the bottom of the control. This value equals TCS_RIGHT. This style is not supported if you use ComCtl32.dll version 6.
    TCS_BUTTONS 
        Tabs appear as buttons, and no border is drawn around the display area.
    TCS_FIXEDWIDTH 
        All tabs are the same width. This style cannot be combined with the TCS_RIGHTJUSTIFY style.
    TCS_FLATBUTTONS 
        Version 4.71. Selected tabs appear as being indented into the background while other tabs appear as being on the same plane as the background. This style only affects tab controls with the TCS_BUTTONS style.
    TCS_FOCUSNEVER 
        The tab control does not receive the input focus when clicked.
    TCS_FOCUSONBUTTONDOWN 
        The tab control receives the input focus when clicked.
    TCS_FORCEICONLEFT 
        Icons are aligned with the left edge of each fixed-width tab. This style can only be used with the TCS_FIXEDWIDTH style.
    TCS_FORCELABELLEFT 
        Labels are aligned with the left edge of each fixed-width tab; that is, the label is displayed immediately to the right of the icon instead of being centered. This style can only be used with the TCS_FIXEDWIDTH style, and it implies the TCS_FORCEICONLEFT style.
    TCS_HOTTRACK 
        Version 4.70. Items under the pointer are automatically highlighted. You can check whether or not hot tracking is enabled by calling SystemParametersInfo. 
    TCS_MULTILINE 
        Multiple rows of tabs are displayed, if necessary, so all tabs are visible at once.
    TCS_MULTISELECT 
        Version 4.70. Multiple tabs can be selected by holding down the CTRL key when clicking. This style must be used with the TCS_BUTTONS style.
    TCS_OWNERDRAWFIXED 
        The parent window is responsible for drawing tabs.
    TCS_RAGGEDRIGHT 
        Rows of tabs will not be stretched to fill the entire width of the control. This style is the default.
    TCS_RIGHT 
        Version 4.70. Tabs appear vertically on the right side of controls that use the TCS_VERTICAL style. This value equals TCS_BOTTOM. This style is not supported if you use visual styles.
    TCS_RIGHTJUSTIFY 
        The width of each tab is increased, if necessary, so that each row of tabs fills the entire width of the tab control. This window style is ignored unless the TCS_MULTILINE style is also specified.
    TCS_SCROLLOPPOSITE 
        Version 4.70. Unneeded tabs scroll to the opposite side of the control when a tab is selected.
    TCS_SINGLELINE 
        Only one row of tabs is displayed. The user can scroll to see more tabs, if necessary. This style is the default.
    TCS_TABS 
        Tabs appear as tabs, and a border is drawn around the display area. This style is the default.
    TCS_TOOLTIPS 
        The tab control has a ToolTip control associated with it. 
    TCS_VERTICAL 
        Version 4.70. Tabs appear at the left side of the control, with tab text displayed vertically. This style is valid only when
        used with the TCS_MULTILINE style. To make tabs appear on the right side of the control, also use the TCS_RIGHT style.
        This style is not supported if you use ComCtl32.dll version 6.
Remarks
    The following styles can be modified after the control is created.
        * TCS_BOTTOM
        * TCS_BUTTONS
        * TCS_FIXEDWIDTH
        * TCS_FLATBUTTONS
        * TCS_FORCEICONLEFT
        * TCS_FORCELABELLEFT
        * TCS_MULTILINE
        * TCS_OWNERDRAWFIXED
        * TCS_RAGGEDRIGHT
        * TCS_RIGHT
        * TCS_VERTICAL 
//notice:if TCM_SETITEMSIZE then need TCS_FIXEDWIDTH or owner-drawn
SetStyle(StyleType::Style, 0, TCS_FIXEDWIDTH);
::SendMessage(m_hWnd, TCM_SETITEMSIZE, 0, (LPARAM)MAKELPARAM(22, _nVHeadWidth));
*/
DWORD TabControlConfig::nStyle = WS_CHILD|WS_VISIBLE|TCS_SINGLELINE|WS_TABSTOP;
DWORD TabControlConfig::nExStyle = WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR | WS_EX_NOPARENTNOTIFY;
//DWORD TabControlConfig::nClassStyle = CS_DBLCLKS | CS_GLOBALCLASS;
DWORD TabControlConfig::nInnerStyle = InnerStyle::TranceContextMenuUp;
DWORD TabControlConfig::nOuterStyle = 0;
DWORD TabControlConfig::nOwnerDrawTextStyle = DT_LEFT|DT_VCENTER|DT_SINGLELINE;
COLORREF TabControlConfig::corBackground = ::GetSysColor(COLOR_BTNFACE);
COLORREF TabControlConfig::corTextground = ::GetSysColor(COLOR_BTNTEXT);
//if hot,now use page's
COLORREF TabControlConfig::corHotBackColor = ::GetSysColor(COLOR_BTNFACE);
COLORREF TabControlConfig::corHotTextColor = ::GetSysColor(COLOR_BTNTEXT);
//===========================================================cc::win::TabControl
///////////////////////////////////////////////////////////////////////////////
// constructors/destructor
TabControl::TabControl()
{
	BASE_CONTROL_CONFIG_INIT(TabControl);
	::SetRect(&_Rect, 0, 0, 75, 23);
	::CopyRect(&_OrgRect, &_Rect);
	_ClassName = WndClassName::TabControl;
	_StyleArray[(int)StyleType::InitControl] = CC_WNDCOMMCTL_TAB_REG;
	_nSelectedIndex = -1;
	_nLastSelectedIndex = -1;
	_eAlignment = TabAlignment::Top;
	_eAppearance = TabAppearance::Normal;
	_pHeadContextMenu = NULL;

#ifdef _DEBUG_
	SetName(_T("TabControl"));
#endif
#if defined(_DEBUG_) || defined(_DESIGN)
	//code for design
	//flag for moveable or others
	Design = (DesignStyle::Type)(CC_DESIGN_NO_COLOR);
#endif
}

TabControl::~TabControl()
{
}
// constructors/destructor

//keyArr is param address,but sHead is by value
void TabControl::DoConfig(bool isFromIni, KeyArr& keyArr, cc::Str sThisHead)
{
	if(!HasStyle(StyleType::Outer, OuterStyle::AutoConfig))
	{
		return;
	}
	sThisHead.Append(_T(".Index"));
	if(isFromIni)
	{
		//if not exist,as usually
		if(keyArr.Exist(sThisHead))
		{
			int nInd = keyArr.Get(sThisHead, 0);
			if(nInd >= 0 && nInd < GetTabCount())
			{
				SetSelectedIndex(nInd);
			}
		}
	}
	else
	{
		keyArr.Set(sThisHead, GetSelectedIndex());
	}
}

bool TabControl::Focus(bool setLastControl)
{
	CC_ASSERT(::IsWindow(m_hWnd));

	return (::SetFocus(m_hWnd) != NULL);
}

void TabControl::OnChildNotify(EventArgs *e)
{
	switch(e->Msg)
	{
	case WM_DRAWITEM:
		{
//CC_TRACE((_T("%s, WM_DRAWITEM"), _Name));
			DrawItemEventArgs ee(e);
			ee.data = (LPDRAWITEMSTRUCT)e->LParam;
			DrawItem(&ee);
		}
		break;
	//Pointer to an NMHDR structure. The hwndFrom member is the handle to the tab control.
	//The idFrom member is the child window identifier of the tab control. The code member is TCN_SELCHANGING.
	//Returns TRUE to prevent the selection from changing, or FALSE to allow the selection to change.
	case WM_NOTIFY:
		{
			LPNMHDR lpnmhdr = (LPNMHDR)e->LParam;
			switch(lpnmhdr->code)
			{
			//Return nonzero to not allow the default processing, or zero to allow the default processing.
			case NM_RCLICK:
//		CC_TRACE((_T("%s, NM_RCLICK"), _Name));
				OnHeadRClick(e);
				break;
#ifdef _DEBUG_
		//	case NM_CLICK:
		//CC_TRACE((_T("%s, NM_CLICK"), _Name));
		//		break;
		//	case NM_RELEASEDCAPTURE:
		//CC_TRACE((_T("%s, NM_RELEASEDCAPTURE"), _Name));
		//		break;
		//	case TCN_FOCUSCHANGE:
		//CC_TRACE((_T("%s, TCN_FOCUSCHANGE"), _Name));
		//		break;
		//	case TCN_GETOBJECT:
		//CC_TRACE((_T("%s, TCN_GETOBJECT"), _Name));
		//		break;
		//	case TCN_KEYDOWN:
		//CC_TRACE((_T("%s, TCN_KEYDOWN"), _Name));
		//		break;
#endif //_DEBUG_
			case TCN_SELCHANGE:
//		CC_TRACE((_T("%s, TCN_SELCHANGE"), _Name));
				OnSelectedIndexChanged(e);
				break;
			case TCN_SELCHANGING:
//		CC_TRACE((_T("%s, TCN_SELCHANGING"), _Name));
				OnSelectedIndexChanging(e);
				break;
			default:
				break;
			}
		}
		break;
	default:
		break;
	}
	Control::OnChildNotify(e);
}

//if need show disabled head,show set TCS_OWNERDRAWFIXED and go here
void TabControl::DrawItem(DrawItemEventArgs* e)
{
	//here do a sample for u
	Control* control = _lstControls->GetKey(e->data->itemID);
	HDC hDC = e->data->hDC;
	RECT rect = e->data->rcItem;
	if(rect.right - rect.left < 10)
	{
		return;
	}

	bool isActive = ((e->data->itemState & ODS_SELECTED) != 0);
	//cannot for xp while IsThemeActive,so donot clear back.2008/11/21
	//set bkcolor
	//::SetBkColor(hDC, isActive ? control->GetBackground() : control->GetHotBackColor());
	//::ExtTextOut(hDC, 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL);

	::SetBkMode(hDC, TRANSPARENT);
	// draw icon,only exist normal icon,then others
	HICON hIcon = control->GetIcon(IconType::Hot);
	if(hIcon != NULL)
	{
		if(!control->GetEnabled())
		{
			hIcon = control->GetIcon(IconType::Disabled);
		}
		else if(isActive)
		{
		}
		else
		{
			hIcon = control->GetIcon(IconType::Gloom);
		}
	}
	if(isActive)
	{
		rect.left += 7;
		if(_eAlignment == TabAlignment::Left)
		{
			rect.left -= 4;
		}
	}
	else
	{
		rect.left += 4;
		if(_eAlignment == TabAlignment::Left)
		{
			rect.left -= 1;
		}
		if(_eAlignment == TabAlignment::Bottom)
		{
			rect.top -= 2;
		}
		else
		{
			rect.top += 3;
		}
	}
	if(hIcon != NULL)
	{
		int nT = rect.top - 1;
		if(isActive)
		{
			nT = rect.top + (rect.bottom -rect.top -CC_TABCONTROL_ICONSIZE)/2;
		}
		//DrawIconEx(hDC, rect.left, nT, hIcon, CC_TABCONTROL_ICONSIZE, CC_TABCONTROL_ICONSIZE, 0, NULL, DI_NORMAL);
		CC_APP_GetDrawActor()->DrawIcon(hDC, rect.left, nT, hIcon, CC_TABCONTROL_ICONSIZE, CC_TABCONTROL_ICONSIZE, NULL, DI_NORMAL);
		rect.left += CC_TABCONTROL_ICONSIZE + 3;
	}

	int len = control->GetTextLen();
	if(len > 0)
	{
		//use font of tab
		const TCHAR* tcText = control->GetText();
		HFONT holdFont = (HFONT)::SelectObject(hDC, GetFont());
		if(!control->GetEnabled())
		{
			OffsetRect(&rect, 1, 1);
			::SetTextColor(hDC, ::GetSysColor(COLOR_BTNHIGHLIGHT));
			::DrawText(hDC, tcText, -1, &rect, DT_SINGLELINE|DT_VCENTER);
			OffsetRect(&rect, -1, -1);
			::SetTextColor(hDC, ::GetSysColor(COLOR_BTNSHADOW));
			::DrawText(hDC, tcText, -1, &rect, DT_SINGLELINE|DT_VCENTER);
		}
		else
		{
			::SetTextColor(hDC, isActive ? control->GetTextColor() : control->GetHotTextColor());
			::DrawText(hDC, tcText, -1, &rect, DT_SINGLELINE|DT_VCENTER);
		}
		holdFont = (HFONT)::SelectObject(hDC, holdFont);
		if(GetFont() != holdFont)
		{
			::DeleteObject(holdFont);
		}
	}
}

bool TabControl::ProcessCmdKey(MSG* pMsg, UINT_PTR& keyData)
{
	// Handle VK_LEFT and VK_RIGHT keys.
	if(    (_StyleArray[(int)StyleType::Style] & TCS_OWNERDRAWFIXED) > 0 && pMsg->message == WM_KEYDOWN &&
		(  ((pMsg->wParam == VK_LEFT || pMsg->wParam == VK_RIGHT) && (_StyleArray[(int)StyleType::Style] & TCS_VERTICAL) == 0) ||
		((pMsg->wParam == VK_DOWN || pMsg->wParam == VK_UP) && (_StyleArray[(int)StyleType::Style] & TCS_VERTICAL) != 0)  )    )
	{
		int nNext = -1;
		int nCur = GetSelectedIndex();
		int nCnt = _lstControls == NULL ? 0 : _lstControls->GetSize();
		if(pMsg->wParam == VK_RIGHT || pMsg->wParam == VK_DOWN)
		{
			for(int n = nCur + 1; n < nCnt; n++)
			{
				if(_lstControls->GetKey(n)->GetEnabled())
				{
					nNext = n;
					break;
				}
			}
		}
		else
		{
			for(int n = nCur - 1; n >= 0; n--)
			{
				if(_lstControls->GetKey(n)->GetEnabled())
				{
					nNext = n;
					break;
				}
			}
		}
		//if next items is all disabled,then do nothing
		if(nNext >= 0)
		{
			SetSelectedIndex(nNext);
		}
		return true;
	}
	return Control::ProcessCmdKey(pMsg, keyData);
}

void TabControl::OnCreateControl()
{
	GetBodyRect(_rectPage);
	int nCnt = (_lstControls == NULL) ? 0 : _lstControls->GetSize();
	if(nCnt > 0)
	{
		//first resize page to tab,then create it
		for(int n = 0; n < nCnt; n++)
		{
			Control* ctl = _lstControls->GetKey(n);
			//ctl->SetVisible(n ==_nSelectedIndex);
			ctl->SetVisible(false);
			DoInsPage((TabPage*)ctl, n);
		}
		SetSelectedIndex(_nSelectedIndex);
	}
	Control::OnCreateControl();
}

void TabControl::DoUpdate()
{
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreatedEnd) != 0)
	{
		if(_lstControls != NULL)
		{
			for(int n = _lstControls->GetSize() - 1; n >= 0; n--)
			{
				Control* ctl = _lstControls->GetKey(n);
				if(ctl->IsUpdate((UpdateFlag::Type)(UpdateFlag::UpdateFlag|UpdateFlag::UpdateText|UpdateFlag::UpdateIcon|UpdateFlag::UpdateEnabled)))
				{
					DoModPage((TabPage*)ctl, n);
				}
			}
		}
		if(m_hWnd != NULL)
		{
			::InvalidateRect(m_hWnd, NULL, FALSE);
		}
	}
}

void TabControl::OnResize(EventArgs* e)
{
	Resize.Invoke((Object*)this, e);
	if(_lstControls != NULL)
	{
		//SetOuterStyle(OuterStyle::Sizing, true);
		_StyleArray[(int)StyleType::Outer] |= OuterStyle::Sizing;
		GetBodyRect(_rectPage);

#ifdef _DEBUG_
		if(CC_APP_GetConfig()->GetIsDebug())
		{
			CC_TRACE((_T("TabControl::OnResize[BodyRect]left:%d, top:%d, right-left:%d, bottom-top:%d"),
				_rectPage.left, _rectPage.top, _rectPage.right - _rectPage.left, _rectPage.bottom - _rectPage.top));
		}
#endif

		//only change curpage size
		//for(int n = _lstControls->GetSize() - 1; n >= 0; n--)
		int nIndex = GetSelectedIndex();
		if(nIndex != -1)
		{
			//_lstControls->GetKey(n)->SetBoundsNotOriginal(body.left, body.top, body.right - body.left, body.bottom - body.top, BoundsSpecified::All);
			_lstControls->GetKey(nIndex)->SetBoundsNotOriginal(_rectPage, BoundsSpecified::All);
		}
		//SetOuterStyle(OuterStyle::Sizing, false);
		_StyleArray[(int)StyleType::Outer] &= ~OuterStyle::Sizing;
	}
}

void TabControl::OnSelectedIndexChanged(EventArgs* e)
{
	int nIndex = GetSelectedIndex();
	if(nIndex >= 0 && _lstControls != NULL)
	{
		Control* page = NULL;
		if(_nLastSelectedIndex != nIndex && _nLastSelectedIndex >= 0 && _nLastSelectedIndex < _lstControls->GetSize())
		{
			page = _lstControls->GetKey(_nLastSelectedIndex);
			((TabPage*)page)->SetActivateFocus();
		}
		Control* ctl = _lstControls->GetKey(nIndex);
		if(!::EqualRect(&_rectPage, &ctl->GetBounds()))
		{
			ctl->SetBoundsNotOriginal(_rectPage, BoundsSpecified::All);
		}
		if(!ctl->GetVisible())
		{
			ctl->SetVisible(true);
		}
		else
		{
			if(ctl->m_hWnd != NULL)
			{
				::InvalidateRect(ctl->m_hWnd, NULL, TRUE);
			}
		}
		if(page != NULL)
		{
			if(page->GetVisible())
			{
				page->SetVisible(false);
			}
		}
		_nLastSelectedIndex = nIndex;
	}
	SelectedIndexChanged.Invoke((Object*)this, e);
}

//Returns TRUE to prevent the selection from changing, or FALSE to allow the selection to change.
//	e->Handled = true;
//	e->Result = TRUE;
void TabControl::OnSelectedIndexChanging(EventArgs* e)
{
	SelectedIndexChanging.Invoke((Object*)this, e);
	//if TCS_OWNERDRAWFIXED,see has not disabled item
	if((_StyleArray[(int)StyleType::Style] & TCS_OWNERDRAWFIXED) > 0 && !e->Handled)
	{
		POINT pt;
		::GetCursorPos(&pt);
		::ScreenToClient(m_hWnd, &pt);
		TCHITTESTINFO hti;
		hti.pt = pt;
		hti.flags = TCHT_ONITEMICON|TCHT_ONITEMLABEL;
		int nIndex = (int)::SendMessage(m_hWnd, TCM_HITTEST, 0, (LPARAM)&hti);
		if(nIndex >= 0)
		{
			Control* ctl = _lstControls->GetKey(nIndex);
			if(!ctl->GetEnabled())
			{
				e->Handled = true;
				e->Result = TRUE;
				return;
			}
		}
	}
}

void TabControl::OnDoubleClick(EventArgs* e)
{
	POINT pt;
	::GetCursorPos(&pt);
	::ScreenToClient(m_hWnd, &pt);
	TCHITTESTINFO hti;
	hti.pt = pt;
	hti.flags = TCHT_ONITEMICON|TCHT_ONITEMLABEL;
	int nIndex = (int)::SendMessage(m_hWnd, TCM_HITTEST, 0, (LPARAM)&hti);
	if(nIndex >= 0)
	{
		//eat it
		e->Handled = true;
		e->Result = TRUE;
		Control* ctl = _lstControls->GetKey(nIndex);
		if(!ctl->GetEnabled())
		{
			return;
		}
		HeadDoubleClick.Invoke((Object*)this, e);
	}
	else
	{
		Control::OnDoubleClick(e);
	}
}

void TabControl::OnHeadRClick(EventArgs* e)
{
	CC_ASSERT(::IsWindow(m_hWnd));

	//not rightclick menu of tab contentmenu but only headcontentmenu
	e->Handled = true;
	e->Result = TRUE;
	if(_pHeadContextMenu == NULL || ::GetCapture() != NULL)
	{
		return;
	}

	//POINT pt = MAKEPOINTS(::GetMessagePos());
	POINT pt;
	::GetCursorPos(&pt);
	POINT pt2 = {pt.x, pt.y};
	::ScreenToClient(m_hWnd, &pt2);
	TCHITTESTINFO hti;
	hti.pt = pt2;
	hti.flags = TCHT_ONITEMICON|TCHT_ONITEMLABEL;
	int nIndex = (int)::SendMessage(m_hWnd, TCM_HITTEST, 0, (LPARAM)&hti);
	if(nIndex >= 0)
	{
		if((_StyleArray[(int)StyleType::Style] & TCS_OWNERDRAWFIXED) > 0)
		{
			Control* ctl = _lstControls->GetKey(nIndex);
			if(!ctl->GetEnabled())
			{
				return;
			}
		}
		if(nIndex != GetSelectedIndex())
		{
			//set clicked index
			SetSelectedIndex(nIndex);
		}
		_pHeadContextMenu->Show(this, pt);
	}
}

void TabControl::AddPage(TabPage *control, int nIndex)
{
	//assert that not added
	CC_ASSERT(_lstControls == NULL || _lstControls->GetIndex(control) < 0);

	//hide it first
	((Control*)control)->SetVisible(false);
	Control::AddControl(control, nIndex);
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
	{
		DoInsPage(control, nIndex);
		//if end item is selected,then show this page
		SetSelectedIndex(_nSelectedIndex);
	}
}

//show delete TabPage out this scope
TabPage* TabControl::RemovePage(int nIndex)
{
	return (TabPage*)RemoveControl(nIndex);
}

Control* TabControl::RemoveControl(int nIndex)
{
	CC_ASSERT(nIndex >= 0 && _lstControls != NULL && nIndex < _lstControls->GetSize());

	return Control::RemoveControl(nIndex);
}

void TabControl::RemoveControl(Control* control)
{
	//before delete it,set selindex to others
	int nCurIndex = GetSelectedIndex();
	int nIndex = _lstControls->GetIndex(control);
	//TabPage* control = (TabPage*)_lstControls->GetKey(nIndex);
	Control::RemoveControl(control);
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0 && m_hWnd != NULL)
	{
		::SendMessage(m_hWnd, TCM_DELETEITEM, nIndex, 0);
		if(nCurIndex == nIndex)
		{
			int nSize;
			if((nSize = _lstControls->GetSize()) > nCurIndex)
			{
				SetSelectedIndex(nIndex);
			}
			else
			{
				SetSelectedIndex(nSize - 1);
			}
		}
		::InvalidateRect(m_hWnd, NULL, TRUE);
	}
}

bool TabControl::SetImageList(TabPage* page, int nIndex)
{
	CC_ASSERT(::IsWindow(m_hWnd) && _lstControls != NULL);

	//if ownerdraw,not save icon
	//if((_StyleArray[(int)StyleType::Style] & TCS_OWNERDRAWFIXED) > 0 || page->GetIcon() == NULL)
	if(page->GetIcon() == NULL)
	{
		return false;
	}

	HIMAGELIST Handle = (HIMAGELIST)::SendMessage(m_hWnd, TCM_GETIMAGELIST, 0, 0L);
	if(Handle == NULL)
	{
		Handle = ImageList_Create(CC_TABCONTROL_ICONSIZE, CC_TABCONTROL_ICONSIZE, ILC_COLOR32 | ILC_MASK, 0, 1);
		::SendMessage(m_hWnd, TCM_SETIMAGELIST, 0, (LPARAM)Handle);
	}
	if(ImageList_GetImageCount(Handle) != _lstControls->GetSize())
	{
		ImageList_SetImageCount(Handle, _lstControls->GetSize());
		for(int n = _lstControls->GetSize() - 1; n >= 0; n--)
		{
			Control* ctl = _lstControls->GetKey(n);
			if(ctl->GetIcon() != NULL)
			{
				ImageList_ReplaceIcon(Handle, n, ctl->GetIcon());
			}
		}
	}
	else
	{
		ImageList_ReplaceIcon(Handle, nIndex, page->GetIcon());
	}
	return true;
}

//the index must be index of _lstControls,or -1 for auto get real index
int TabControl::DoInsPage(TabPage *control, int nIndex)
{
	CC_ASSERT(::IsWindow(m_hWnd));
	CC_ASSERT(nIndex != -1 || (nIndex == -1 && _lstControls != NULL && _lstControls->GetIndex(control) >= 0));

	TCITEM item;
	ZeroMemory(&item, sizeof(TCITEM));
	if(control->GetTextLen() > 0)
	{
		item.mask = TCIF_TEXT;
		item.pszText = (LPTSTR)control->GetText();
	}
	item.iImage = -1;
	item.lParam = 0;
	//item.cchTextMax = control->GetTextLen();
	item.dwState = 0;
	//dwStateMask:Version 4.70. Specifies which bits of the dwState member contain valid information. This member is ignored in the TCM_INSERTITEM message.
	item.dwStateMask = 0;

	if(nIndex == -1)
	{
		nIndex = _lstControls->GetIndex(control);
	}
	if(SetImageList(control, nIndex))
	{
		item.mask |= TCIF_IMAGE;
		item.iImage = nIndex;
	}
	nIndex = (int)::SendMessage(m_hWnd, TCM_INSERTITEM, nIndex, (LPARAM)&item);
	GetBodyRect(_rectPage);

#ifdef _DEBUG_
	if(CC_APP_GetConfig()->GetIsDebug())
	{
		CC_TRACE((_T("TabControl::DoInsPageleft:%d, top:%d, right-left:%d, bottom-top:%d"),
			_rectPage.left, _rectPage.top, _rectPage.right - _rectPage.left, _rectPage.bottom - _rectPage.top));
	}
#endif

	//control->SetBoundsNotOriginal(_rectPage.left, _rectPage.top, _rectPage.right - _rectPage.left, _rectPage.bottom - _rectPage.top, BoundsSpecified::All);
	control->SetBoundsNotOriginal(_rectPage, BoundsSpecified::All);
	return nIndex;
}

//the index must be index of _lstControls,or -1 for auto get real index
int TabControl::DoModPage(TabPage *control, int nIndex)
{
	CC_ASSERT(::IsWindow(m_hWnd) && _lstControls != NULL && nIndex < GetTabCount());

	TCITEM item;
	ZeroMemory(&item, sizeof(TCITEM));
	::SendMessage(m_hWnd, TCM_GETITEM, nIndex, (LPARAM)&item);
	//if(tc != NULL && item.pszText != NULL && _tcscmp(tc, item.pszText) == 0)
	//{
	//	return -1;
	//}
	if(control->GetTextLen() > 0)
	{
		const TCHAR* tc = control->GetText();
		item.mask |= TCIF_TEXT;
		item.pszText = (LPTSTR)tc;
	}
	else
	{
		item.mask &= ~TCIF_TEXT;
	}

	if(nIndex == -1)
	{
		nIndex = _lstControls->GetIndex(control);
	}
	if(nIndex == -1)
	{
		throw;
	}
	item.mask |= TCIF_IMAGE;
	if(SetImageList(control, nIndex))
	{
		item.iImage = nIndex;
	}
	else
	{
		item.iImage = -1;
	}
	::SendMessage(m_hWnd, TCM_SETITEM, nIndex, (LPARAM)&item);
	return nIndex;
}

TabPage* TabControl::GetPage(TCHAR* const ctlName) const
{
	return (TabPage*)Control::GetControl(ctlName);
}

TabPage* TabControl::GetPage(int nIndex) const
{
	return (TabPage*)Control::GetControl(nIndex);
}

int TabControl::GetTabCount() const
{
	return Control::GetControlCount();
}

void TabControl::SetSelectedIndex(int selectedIndex)
{
	if(_lstControls == NULL || _lstControls->GetSize() <= 0)
	{
		_nSelectedIndex = -1;
		return;
	}
	_nSelectedIndex = selectedIndex;
	if(_nSelectedIndex < 0)
	{
		_nSelectedIndex = 0;
	}
	if(_nSelectedIndex >= _lstControls->GetSize())
	{
		_nSelectedIndex = _lstControls->GetSize() - 1;
	}

	//if TCS_OWNERDRAWFIXED,check enabled
	if((_StyleArray[(int)StyleType::Style] & TCS_OWNERDRAWFIXED) != 0 && !_lstControls->GetKey(_nSelectedIndex)->GetEnabled())
	{
		bool isFound = false;
		for(int n = _nSelectedIndex + 1; n < _lstControls->GetSize(); n++)
		{
			if(_lstControls->GetKey(n)->GetEnabled())
			{
				isFound = true;
				_nSelectedIndex = n;
				break;
			}
		}
		if(!isFound)
		{
			for(int n = 0; n < _nSelectedIndex; n++)
			{
				if(_lstControls->GetKey(n)->GetEnabled())
				{
					_nSelectedIndex = n;
					break;
				}
			}
		}
	}
	//curent is not enabled,and no next enabled page,set current disabled page
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
	{
		if(_nSelectedIndex >= 0)
		{
			::SendMessage(m_hWnd, TCM_SETCURSEL, _nSelectedIndex, 0);
		}
		//sometimes the OnSelectedIndexChanged is not fired
		OnSelectedIndexChanged(NULL);
	}
}

int TabControl::GetSelectedIndex()
{
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
	{
		_nSelectedIndex = (int)::SendMessage(m_hWnd, TCM_GETCURSEL, 0, 0);
	}
	return _nSelectedIndex;
}

void TabControl::SetSelectedTab(TabPage* selectedTab)
{
	int nInd;
	if(_lstControls == NULL || (nInd = _lstControls->GetIndex(selectedTab)) < 0)
	{
		throw;
	}
	SetSelectedIndex(nInd);
}

TabPage* TabControl::GetSelectedTab()
{
	_nSelectedIndex = GetSelectedIndex();
	if(_lstControls == NULL || _nSelectedIndex < 0)
	{
		throw;
	}
	return (TabPage*)_lstControls->GetKey(_nSelectedIndex);
}

ContextMenu* TabControl::GetHeadContextMenu() const
{
	return _pHeadContextMenu;
}

void TabControl::SetHeadContextMenu(ContextMenu* menu)
{
	_pHeadContextMenu = menu;
}

//this should be down before create control
void TabControl::SetTabAlignment(TabAlignment::Type alignment)
{
	_eAlignment = alignment;
	if(alignment == TabAlignment::Top || alignment == TabAlignment::None)
	{
		SetStyle(StyleType::Style, TCS_VERTICAL|TCS_BOTTOM, 0);
	}
	else if(alignment == TabAlignment::Bottom)
	{
		SetStyle(StyleType::Style, TCS_VERTICAL, TCS_BOTTOM);
	}
	else if(alignment == TabAlignment::Left)
	{
		//maybe need TCS_MULTILINE
		SetStyle(StyleType::Style, TCS_BOTTOM, TCS_VERTICAL);
	}
	else //if(alignment == TabAlignment::Right)
	{
		//maybe need TCS_MULTILINE
		SetStyle(StyleType::Style, 0, TCS_VERTICAL|TCS_RIGHT);
	}
	//::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED | SWP_DRAWFRAME);
	Update();
	OnResize(NULL);
}

TabAlignment::Type TabControl::GetTabAlignment() const
{
	return _eAlignment;
}

void TabControl::SetTabAppearance(TabAppearance::Type appearance)
{
	_eAppearance = appearance;
	if(_eAppearance == TabAppearance::Normal)
	{
		SetStyle(StyleType::Style, TCS_BUTTONS|TCS_FLATBUTTONS, 0);
	}
	else if(_eAppearance == TabAppearance::Buttons)
	{
		SetStyle(StyleType::Style, TCS_FLATBUTTONS, TCS_BUTTONS);
		//this is valid before create
		SetStyle(StyleType::ExStyle, 0, TCS_EX_FLATSEPARATORS);
	}
	else //FlatButtons
	{
		SetStyle(StyleType::Style, 0, TCS_BUTTONS|TCS_FLATBUTTONS);
		//this is valid before create
		SetStyle(StyleType::ExStyle, 0, TCS_EX_FLATSEPARATORS);
	}
	//::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED | SWP_DRAWFRAME);
	Update();
	OnResize(NULL);
}

TabAppearance::Type TabControl::GetTabAppearance() const
{
	return _eAppearance;
}

void TabControl::GetBodyRect(RECT &rect)
{
	CC_ASSERT(::IsWindow(m_hWnd));

	::GetClientRect(m_hWnd, &rect);
	TabCtrl_AdjustRect(m_hWnd, FALSE, &rect);
	if(_eAlignment == TabAlignment::Top || _eAlignment == TabAlignment::Bottom)
	{
		if(rect.left > 1)
		{
			rect.left--;
		}
	}
}

void TabControl::GetItemRect(int nIndex, RECT &rect)
{
	CC_ASSERT(::IsWindow(m_hWnd));

	::SendMessage(m_hWnd, TCM_GETITEMRECT, nIndex, (LPARAM)&rect);
}
///////////////////////////////////////////////////////////////////////////////
//===========================================================cc::win::TabControl

DWORD FormClientConfig::nStyle = WS_CHILD | WS_VISIBLE | WS_TABSTOP;
DWORD FormClientConfig::nExStyle = 0;
//DWORD FormClientConfig::nClassStyle = CS_DBLCLKS | CS_GLOBALCLASS;
//need DeliverFormMessage to me
//nBackStyle will be overwrite by Form or Dialog
DWORD FormClientConfig::nInnerStyle = InnerStyle::TranceContextMenuUp|InnerStyle::DeliverFormMessage|InnerStyle::TabLoopInner;
//others will be added by Form or Dialog
DWORD FormClientConfig::nOuterStyle = 0;
DWORD FormClientConfig::nOwnerDrawTextStyle = DT_LEFT|DT_VCENTER|DT_SINGLELINE;
//corBackground will be overwrited by Form or Dialog
COLORREF FormClientConfig::corBackground = ::GetSysColor(COLOR_BTNFACE);
COLORREF FormClientConfig::corTextground = ::GetSysColor(COLOR_BTNTEXT);
COLORREF FormClientConfig::corHotBackColor = ::GetSysColor(COLOR_BTNFACE);
COLORREF FormClientConfig::corHotTextColor = ::GetSysColor(COLOR_BTNTEXT);
//===========================================================cc::win::FormClient
///////////////////////////////////////////////////////////////////////////////
// constructors/destructor
FormClient::FormClient() : Container()
{
	BASE_CONTROL_CONFIG_INIT(FormClient);
	//::SetRect(&_Rect, 0, 0, 75, 23);
	//::CopyRect(&_OrgRect, &_Rect);
	//set a default value for the wnd class name
	//_ClassName = CC_WND_FORMCLIENT;
	_ClassName = WndClassName::Dialog32770;
	// mask for InitCommonControlsEx
	_StyleArray[(int)StyleType::InitControl] = CC_WNDMDIFRAME_REG;

#ifdef _DEBUG_
	SetName(_T("FormClient"));
#endif
	//not add this to config or lang id
	SetConfigName(_T(""));
#if defined(_DEBUG_) || defined(_DESIGN)
	//code for design
	//flag for moveable or others
	Design = (DesignStyle::Type)(DesignStyle::NoDesign);
#endif
}

FormClient::~FormClient()
{
}

bool FormClient::PreCreateWindow(CREATESTRUCT& cs)
{
	CLIENTCREATESTRUCT ccs = {0};
	if(HasStyle(StyleType::Inner, InnerStyle::IsMdiContainer))
	{
		_ClassName = WndClassName::MDIChild;
		_StyleArray[(int)StyleType::Style] = WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_VSCROLL | WS_HSCROLL;
		_StyleArray[(int)StyleType::ExStyle] = WS_EX_CLIENTEDGE;

		// Retrieve the handle of the Window menu and assign the
		// first child window identifier.
		ccs.hWindowMenu = GetSubMenu(GetMenu(m_hWnd), 2 );
		ccs.idFirstChild = 700;

		cs.lpszClass = _ClassName;
		cs.style = _StyleArray[(int)StyleType::Style];
		cs.dwExStyle = _StyleArray[(int)StyleType::ExStyle];
		cs.hMenu = (HMENU)0xCAC;
		cs.lpCreateParams = (LPVOID)&ccs;
	}

	return Container::PreCreateWindow(cs);
}

void FormClient::SetIsMdiContainer()
{
	CC_ASSERT(m_hWnd == NULL);

	SetStyle(StyleType::Inner, 0, InnerStyle::IsMdiContainer);
}

//void FormClient::OnGotFocus(EventArgs* e)
//{
//	//if(::IsWindow(_hFocusBeforeDeactivate) || ::IsChild(m_hWnd, _hFocusBeforeDeactivate))
//	//{
//	//	// otherwise, set focus to the last known focus window
//	//	::SetFocus(_hFocusBeforeDeactivate);
//	//}
//	//else
//	//{
//		Control* pWndNext = GetNextControl(NULL);
//		//cc::Log::debug(_T("---------------------------\r\n"));
//		if(pWndNext != NULL)
//		{
//			//cc::Log::debug(_T("focus:%s, %s\r\n"), (TCHAR*)pWndNext->GetClassName().GetBuf(), pWndNext->GetText());
//			pWndNext->Focus();
//		}
//	//}
//	Container::OnGotFocus(e);
//}
//
DWORD MDIChildConfig::nStyle = WS_OVERLAPPED | WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_GROUP | WS_TABSTOP;
DWORD MDIChildConfig::nExStyle = WS_EX_MDICHILD | WS_EX_WINDOWEDGE | WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR;
//DWORD MDIChildConfig::nClassStyle = CS_DBLCLKS | CS_GLOBALCLASS;
//need DeliverFormMessage to me
DWORD MDIChildConfig::nInnerStyle = InnerStyle::TranceContextMenuUp|InnerStyle::DeliverFormMessage;
DWORD MDIChildConfig::nOuterStyle = 0;
DWORD MDIChildConfig::nOwnerDrawTextStyle = DT_LEFT|DT_VCENTER|DT_SINGLELINE;
COLORREF MDIChildConfig::corBackground = ::GetSysColor(COLOR_BTNFACE);
COLORREF MDIChildConfig::corTextground = ::GetSysColor(COLOR_BTNTEXT);
COLORREF MDIChildConfig::corHotBackColor = ::GetSysColor(COLOR_BTNFACE);
COLORREF MDIChildConfig::corHotTextColor = ::GetSysColor(COLOR_BTNTEXT);
//===========================================================cc::win::MDIChild
///////////////////////////////////////////////////////////////////////////////
// constructors/destructor
MDIChild::MDIChild() : Container()
{
	BASE_CONTROL_CONFIG_INIT(MDIChild);
	//use window default pos
	::SetRect(&_Rect, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT + 300, CW_USEDEFAULT + 300);
	::CopyRect(&_OrgRect, &_Rect);
	//set a default value for the wnd class name
	_ClassName = WndClassName::Dialog32770;
	// mask for InitCommonControlsEx
	_StyleArray[(int)StyleType::InitControl] = CC_WNDMDIFRAME_REG;

#ifdef _DEBUG_
	SetName(_T("MDIChild"));
#endif
	//not add this to config or lang id
	SetConfigName(_T(""));
#if defined(_DEBUG_) || defined(_DESIGN)
	//code for design
	//flag for moveable or others
	Design = (DesignStyle::Type)(DesignStyle::NoDesign);
#endif
}

MDIChild::~MDIChild()
{
}

//DWORD FormFrameConfig::nStyle = WS_CHILD | WS_VISIBLE | WS_TABSTOP;
//===========================================================cc::win::FormFrame
///////////////////////////////////////////////////////////////////////////////
// constructors/destructor
FormFrame::FormFrame() : Container()
{
	//BASE_CONTROL_CONFIG_INIT(FormFrame);
	//if is dialog then changed to WndClassName::Dialog32770
	//_ClassName = WndClassName::Form;
#ifdef _DEBUG_
	SetName(_T("FormFrame"));
#endif
	_pClient = new FormClient();
	//here AddControl will add to self's _lstControls,the next AddControl will add to _pClient
	FormFrame::AddControl(_pClient);
#if !defined(_CCTHINFORM)
	_eRebarStyle = ReBarStyle::Floatable;
	for(int n = 0; n < 4; n++)
	{
		_arrBarCont[n] = NULL;
	}
#if defined(_DEBUG_) || defined(_DESIGN)
	//for design while debug
	_designBar = NULL;
	//code for design
	//flag for moveable or others
	Design = (DesignStyle::Type)(DesignStyle::CanAddControl|CC_DESIGN_NO_ALL);
#endif
	_lstBars = NULL;
	_lstToolBars = NULL;
#endif //#if !defined(_CCTHINFORM)
}

#if !defined(_CCTHINFORM)
FormFrame::~FormFrame()
{
#if defined(_DEBUG_) || defined(_DESIGN)
	if(_designBar != NULL && CC_APP_GetDesignBarActive())
	{
		//for design while debug
		_designBar->SetVisible(false);
	}
#endif
	if(_lstBars != NULL)
	{
		for(int n = _lstBars->GetSize() - 1; n >= 0; n--)
		{
			delete _lstBars->GetKey(n);
		}
		_lstBars->Clear();
		delete _lstBars;
		_lstBars = NULL;
	}
	if(_lstToolBars != NULL)
	{
		for(int n = _lstToolBars->GetSize() - 1; n >= 0; n--)
		{
			delete _lstToolBars->GetKey(n);
		}
		_lstToolBars->Clear();
		delete _lstToolBars;
		_lstToolBars = NULL;
	}
	if(_arrBarCont[0] != NULL)
	{
		for(int n = 0; n < 4; n++)
		{
			delete _arrBarCont[n];
			_arrBarCont[n] = NULL;
		}
	}
}

void FormFrame::Dispose()
{
	Container::Dispose();
	if(_lstBars != NULL)
	{
		for(int n = _lstBars->GetSize() - 1; n >= 0; n--)
		{
			_lstBars->GetKey(n)->Dispose();
		}
	}
	if(_lstToolBars != NULL)
	{
		for(int n = _lstToolBars->GetSize() - 1; n >= 0; n--)
		{
			_lstToolBars->GetKey(n)->Dispose();
		}
	}
	if(_arrBarCont[0] != NULL)
	{
		for(int n = 0; n < 4; n++)
		{
			_arrBarCont[n]->Dispose();
		}
	}
}

LRESULT FormFrame::WndProc(EventArgs *e)
{
	switch(e->Msg)
	{
	case CC_WM_MAINMSG:
		{
			switch(e->WParam)
			{
			case CC_WM_MAINMSG_WPARAM_GET_REBAR:
				{
					//no toolbar container
					if(_arrBarCont[0] == NULL)
					{
						return NULL;
					}
					CC_ASSERT(e->LParam != NULL);
					FindReBarEntity* entity = (FindReBarEntity*)e->LParam;
					//the toolbar should be child of _arrBarCont[n]
					if(!ContainsToolBar(entity->pToolBar))
					{
						return NULL;
					}

					::ScreenToClient(m_hWnd, &entity->pt);
					RECT rect;
					::GetClientRect(m_hWnd, &rect);
					::InflateRect(&rect, 6, 6);
					if(!::PtInRect(&rect, entity->pt))
					{
						return NULL;
					}
					for(int n = 0; n < 4; n++)
					{
						if((n == 0 && (entity->eDockable & DockStyles::Left) != 0) ||
							(n == 1 && (entity->eDockable & DockStyles::Top) != 0) ||
							(n == 2 && (entity->eDockable & DockStyles::Right) != 0) ||
							(n == 3 && (entity->eDockable & DockStyles::Bottom) != 0))
						{
						}
						else
						{
							continue;
						}
						rect = _arrBarCont[n]->GetBounds();
						::InflateRect(&rect, 6, 6);
						if(::PtInRect(&rect, entity->pt))
						{
							entity->eReBarStyle = _eRebarStyle;
							entity->bHorizontal = !_arrBarCont[n]->HasStyle(StyleType::Style, CCS_VERT);
							//err(_T("entity->bHorizontal:%d, entity->pt.y:%d, entity->pt.x:%d, rect.left:%d, rect.top:%d, rect.bottom:%d"), entity->bHorizontal, entity->pt.y, entity->pt.x, rect.left, rect.top, rect.bottom);
							entity->nInsertIndex = entity->bHorizontal ? (entity->pt.y < (rect.top + rect.bottom) / 2 ? 0 : -1) :
																		(entity->pt.x < (rect.left + rect.right)/2 ? 0 : -1);;
							return (LRESULT)_arrBarCont[n];
						}
					}
				}
				return NULL;
			case CC_WM_MAINMSG_WPARAM_GET_REBARSTYLE:
				return (LRESULT)_eRebarStyle;
			} //switch(e->WParam)
		}
		break;
	}
	return Container::WndProc(e);
}

bool FormFrame::PreFormProcessMessage(MSG* pMsg, UINT_PTR& keyData)
{
#if defined(_DEBUG_) || defined(_DESIGN)
	if(_designBar != NULL && CC_APP_GetDesignBarActive())
	{
		if(_designBar->PreFormProcessMessage(pMsg, keyData))
		{
			return true;
		}
	}
#endif
	if(_lstBars != NULL)
	{
		for(int n = _lstBars->GetSize() - 1; n >= 0; n--)
		{
			if(_lstBars->GetKey(n)->PreFormProcessMessage(pMsg, keyData))
			{
				return true;
			}
		}
	}
	if(_lstToolBars != NULL)
	{
		for(int n = _lstToolBars->GetSize() - 1; n >= 0; n--)
		{
			if(_lstToolBars->GetKey(n)->PreFormProcessMessage(pMsg, keyData))
			{
				return true;
			}
		}
	}
	return Container::PreFormProcessMessage(pMsg, keyData);
}

bool FormFrame::ProcessMnemonic(TCHAR tchar)
{
#if defined(_DEBUG_) || defined(_DESIGN)
	if(_designBar != NULL && CC_APP_GetDesignBarActive())
	{
		if(_designBar->ProcessMnemonic(tchar))
		{
			return true;
		}
	}
#endif
	if(Container::ProcessMnemonic(tchar))
	{
		return true;
	}
	if(_lstBars != NULL)
	{
		for(int n = _lstBars->GetSize() - 1; n >= 0; n--)
		{
			if(_lstBars->GetKey(n)->ProcessMnemonic(tchar))
			{
				return true;
			}
		}
	}
	if(_lstToolBars != NULL)
	{
		for(int n = _lstToolBars->GetSize() - 1; n >= 0; n--)
		{
			if(_lstToolBars->GetKey(n)->ProcessMnemonic(tchar))
			{
				return true;
			}
		}
	}
	return false;
}

void FormFrame::OnCreateControl()
{
	Container::OnCreateControl();
	int nCnt;
	if(_lstBars != NULL && (nCnt = _lstBars->GetSize()) > 0)
	{
		for(int n = 0; n < nCnt; n++)
		{
			DockBar* ctl = (DockBar*)_lstBars->GetKey(n);
			ctl->CreateControl();
#if defined(_DEBUG_) || defined(_DESIGN)
			//code for design
			//flag for moveable or others
			ctl->Design = (DesignStyle::Type)(CC_DESIGN_NO_MOVE|CC_DESIGN_NO_COLOR);
#endif
		}
	}
	if(_lstToolBars != NULL && (nCnt = _lstToolBars->GetSize()) > 0)
	{
		for(int n = 0; n < 4; n++)
		{
			_arrBarCont[n] = new ReBar();
			_arrBarCont[n]->SetParent(this, true);
			_arrBarCont[n]->CreateControl();
#if defined(_DEBUG_) || defined(_DESIGN)
			//code for design
			//flag for moveable or others
			_arrBarCont[n]->Design = (DesignStyle::Type)(DesignStyle::NoDesign);
#endif
		}
		_arrBarCont[0]->SetName(_T("Left"));
		_arrBarCont[1]->SetName(_T("Top"));
		_arrBarCont[2]->SetName(_T("Right"));
		_arrBarCont[3]->SetName(_T("Bottom"));
		_arrBarCont[0]->SetStyle(StyleType::Style, CCS_TOP, CCS_LEFT);
		_arrBarCont[2]->SetStyle(StyleType::Style, CCS_TOP, CCS_RIGHT);
		_arrBarCont[3]->SetStyle(StyleType::Style, CCS_TOP, CCS_BOTTOM);
#ifdef _DEBUG_
		_arrBarCont[0]->SetText(_T("reBarLeft"));
		_arrBarCont[1]->SetText(_T("reBarTop"));
		_arrBarCont[2]->SetText(_T("reBarRight"));
		_arrBarCont[3]->SetText(_T("reBarBottom"));
#endif
		//REBARBANDINFO rbbInfo;
		for(int n = 0; n < nCnt; n++)
		{
			ToolBar* ctl = (ToolBar*)_lstToolBars->GetKey(n);
			DockStyles::Type eType = ctl->GetInitDockPosition();
			ctl->CreateControl();
			if(eType == DockStyles::Left)
			{
				//_arrBarCont[0]->AddControl(ctl);
				_arrBarCont[0]->InsertBand(ctl, -1, _eRebarStyle == ReBarStyle::Fix);
			}
			else if(eType == DockStyles::Top)
			{
				//_arrBarCont[1]->AddControl(ctl);
				_arrBarCont[1]->InsertBand(ctl, -1, _eRebarStyle == ReBarStyle::Fix);
			}
			else if(eType == DockStyles::Right)
			{
				//_arrBarCont[2]->AddControl(ctl);
				_arrBarCont[2]->InsertBand(ctl, -1, _eRebarStyle == ReBarStyle::Fix);
			}
			else if(eType == DockStyles::Bottom)
			{
				//_arrBarCont[3]->AddControl(ctl);
				_arrBarCont[3]->InsertBand(ctl, -1, _eRebarStyle == ReBarStyle::Fix);
			}
			else
			{
				//treate Fill as None as float
				//not show Floater here,for perhaps load Config at Form_Load,show it after that
				ctl->SetFloating(true, false);
			}
		}
	}
}

void FormFrame::OnResizeFrame(HDWP hDWP, RECT& frameRect)
{
	CC_ASSERT(_pClient != NULL);

	RECT rect2;
	REBARBANDINFO rbbInfo;
	rbbInfo.cbSize = sizeof(REBARBANDINFO);
	if(_arrBarCont[0] != NULL)
	{
		//top
		int nH;
		int itemCount = (int)::SendMessage(_arrBarCont[1]->m_hWnd, RB_GETBANDCOUNT, 0, 0);
		bool isVisible = false;
		for(int i = 0; i < itemCount; i++)
		{
			rbbInfo.fMask = RBBIM_STYLE;
			::SendMessage(_arrBarCont[1]->m_hWnd, RB_GETBANDINFO, i, (LPARAM)&rbbInfo);
			if((rbbInfo.fStyle & RBBS_HIDDEN) == 0)
			{
				isVisible = true;
				break;
			}
		}
		_arrBarCont[1]->SetVisible(isVisible);
		if(!isVisible)
		{
			nH = 0;
		}
		else
		{
			rect2 = _arrBarCont[1]->GetBounds();
			nH = rect2.bottom - rect2.top;
		}
		hDWP = ::DeferWindowPos(hDWP, _arrBarCont[1]->m_hWnd, NULL, frameRect.left, frameRect.top, frameRect.right, nH, SWP_NOACTIVATE|SWP_NOZORDER);
		frameRect.top += nH;
		//bottom
		itemCount = (int)::SendMessage(_arrBarCont[3]->m_hWnd, RB_GETBANDCOUNT, 0, 0);
		isVisible = false;
		for(int i = 0; i < itemCount; i++)
		{
			rbbInfo.fMask = RBBIM_STYLE;
			::SendMessage(_arrBarCont[3]->m_hWnd, RB_GETBANDINFO, i, (LPARAM)&rbbInfo);
			if((rbbInfo.fStyle & RBBS_HIDDEN) == 0)
			{
				isVisible = true;
				break;
			}
		}
		_arrBarCont[3]->SetVisible(isVisible);
		if(!isVisible)
		{
			nH = 0;
		}
		else
		{
			rect2 = _arrBarCont[3]->GetBounds();
			nH = rect2.bottom - rect2.top;
		}
		frameRect.bottom -= nH;
		hDWP = ::DeferWindowPos(hDWP, _arrBarCont[3]->m_hWnd, NULL, frameRect.left, frameRect.bottom, frameRect.right, nH, SWP_NOACTIVATE|SWP_NOZORDER);

		//left
		int nW;
		itemCount = (int)::SendMessage(_arrBarCont[0]->m_hWnd, RB_GETBANDCOUNT, 0, 0);
		isVisible = false;
		for(int i = 0; i < itemCount; i++)
		{
			rbbInfo.fMask = RBBIM_STYLE;
			::SendMessage(_arrBarCont[0]->m_hWnd, RB_GETBANDINFO, i, (LPARAM)&rbbInfo);
			if((rbbInfo.fStyle & RBBS_HIDDEN) == 0)
			{
				isVisible = true;
				break;
			}
		}
		_arrBarCont[0]->SetVisible(isVisible);
		if(!isVisible)
		{
			nW = 0;
		}
		else
		{
			rect2 = _arrBarCont[0]->GetBounds();
			nW = rect2.right - rect2.left;
		}
		hDWP = ::DeferWindowPos(hDWP, _arrBarCont[0]->m_hWnd, NULL, frameRect.left, frameRect.top, nW, frameRect.bottom - frameRect.top, SWP_NOACTIVATE|SWP_NOZORDER);
		frameRect.left += nW;
		//right
		itemCount = (int)::SendMessage(_arrBarCont[2]->m_hWnd, RB_GETBANDCOUNT, 0, 0);
		isVisible = false;
		for(int i = 0; i < itemCount; i++)
		{
			rbbInfo.fMask = RBBIM_STYLE;
			::SendMessage(_arrBarCont[2]->m_hWnd, RB_GETBANDINFO, i, (LPARAM)&rbbInfo);
			if((rbbInfo.fStyle & RBBS_HIDDEN) == 0)
			{
				isVisible = true;
				break;
			}
		}
		_arrBarCont[2]->SetVisible(isVisible);
		if(!isVisible)
		{
			nW = 0;
		}
		else
		{
			rect2 = _arrBarCont[2]->GetBounds();
			nW = rect2.right - rect2.left;
		}
		frameRect.right -= nW;
		hDWP = ::DeferWindowPos(hDWP, _arrBarCont[2]->m_hWnd, NULL, frameRect.right, frameRect.top, nW, frameRect.bottom - frameRect.top, SWP_NOACTIVATE|SWP_NOZORDER);
	}
	//_pClient->SetBoundsNotOriginal(frameRect);
	//for first Form::Resize,_pClient is not CreateControl
	if(_pClient->m_hWnd != NULL)
	{
		hDWP = ::DeferWindowPos(hDWP, _pClient->m_hWnd, NULL, frameRect.left, frameRect.top, frameRect.right - frameRect.left, frameRect.bottom - frameRect.top, SWP_NOACTIVATE|SWP_NOZORDER);
	}
}
#endif //#if !defined(_CCTHINFORM)

FormClient* FormFrame::GetClient()
{
	CC_ASSERT(_pClient != NULL);

	return (FormClient*)_pClient;
}

#if !defined(_CCTHINFORM)
void FormFrame::AddDockBar(DockBar* control, int nIndex)
{
	if(_lstBars == NULL)
	{
		//isSort, bIsDESC=false, isDistinct=false, nMinCapacity=0, nIncrement=1.3)
		_lstBars = new List<Control*>(false, false, true);
	}
	int nReal = _lstBars->GetIndex(control);
	if(nReal >= 0)
	{
		if(nReal == nIndex)
		{
			return;
		}
		_lstBars->Remove(control);
	}
	if(nIndex < 0)
	{
		nIndex = _lstBars->GetSize();
	}
	_lstBars->Insert(nIndex, control);
	//to add to this->controllist at SetParent
	control->SetParent(this, true);
	EventArgs e(0, 0, 0, 0);
	ControlEventArgs ee(&e);
	ee.control = control;
	OnControlAdded(&ee);
}

//It is the responsibility of the calling application to destroy the Control returned from this function using the delete function.
DockBar* FormFrame::RemoveDockBar(int nIndex)
{
	DockBar* control = GetDockBar(nIndex);
	if(control != NULL)
	{
		RemoveDockBar(control);
	}
	return control;
}

void FormFrame::RemoveDockBar(DockBar* control)
{
	if(_lstBars == NULL || !_lstBars->ContainsKey(control))
	{
		throw;
	}

	//set not visible before remove it
	control->SetVisible(false);
	_lstBars->Remove(control);
	control->SetParent(NULL);
	EventArgs e(0, 0, 0, 0);
	ControlEventArgs ee(&e);
	ee.control = control;
	OnControlRemoved(&ee);
}

DockBar* FormFrame::GetDockBar(TCHAR* const ctlName) const
{
	CC_ASSERT(ctlName != NULL);

	if(ctlName != NULL && _lstBars != NULL)
	{
		for(int n = _lstBars->GetSize() - 1; n >= 0; n--)
		{
			DockBar* ctl = (DockBar*)_lstBars->GetKey(n);
			if(ctl->GetName() != NULL && _tcscmp(ctlName, ctl->GetName()) == 0)
			{
				return ctl;
			}
		}
	}
	return NULL;
}

DockBar* FormFrame::GetDockBar(int nIndex) const
{
	if(_lstBars == NULL || nIndex < 0 || nIndex >= _lstBars->GetSize())
	{
		return NULL;
	}
	return (DockBar*)_lstBars->GetKey(nIndex);
}

bool FormFrame::ContainsDockBar(Control* control) const
{
	return (_lstBars == NULL ? false : _lstBars->ContainsKey(control));
}

int FormFrame::GetDockBarCount() const
{
	return (_lstBars == NULL ? 0 : _lstBars->GetSize());
}

ReBarStyle::Type FormFrame::GetReBarStyle()
{
	return _eRebarStyle;
}

void FormFrame::SetReBarStyle(ReBarStyle::Type type)
{
	_eRebarStyle = type;
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0 && _arrBarCont[0] != NULL)
	{
		for(int n = 0; n < 4; n++)
		{
			_arrBarCont[n]->UpdateBand();
		}
	}
}

//each ToolBar should has different nID,get it from Tabindex(if < 1,from index+1)
void FormFrame::AddToolBar(ToolBar* control, int nIndex)
{
	if(_lstToolBars == NULL)
	{
		//isSort, bIsDESC=false, isDistinct=false, nMinCapacity=0, nIncrement=1.3)
		_lstToolBars = new List<Control*>(false, false, true);
	}
	int nReal = _lstToolBars->GetIndex(control);
	if(nReal >= 0)
	{
		if(nReal == nIndex)
		{
			return;
		}
		_lstToolBars->Remove(control);
	}
	if(nIndex < 0)
	{
		nIndex = _lstToolBars->GetSize();
	}
	//need this!
	if(control->TabIndex < 1)
	{
		control->TabIndex = _lstToolBars->GetSize() + 1;
	}
	_lstToolBars->Insert(nIndex, control);
	//to add to this->controllist at SetParent
	control->SetParent(this, true);
	EventArgs e(0, 0, 0, 0);
	ControlEventArgs ee(&e);
	ee.control = control;
	OnControlAdded(&ee);
}

//It is the responsibility of the calling application to destroy the Control returned from this function using the delete function.
ToolBar* FormFrame::RemoveToolBar(int nIndex)
{
	ToolBar* control = GetToolBar(nIndex);
	if(control != NULL)
	{
		RemoveToolBar(control);
	}
	return control;
}

void FormFrame::RemoveToolBar(ToolBar* control)
{
	if(_lstToolBars == NULL || !_lstToolBars->ContainsKey(control))
	{
		throw;
	}

	//set not visible before remove it
	control->SetVisible(false);
	_lstToolBars->Remove(control);
	control->SetParent(NULL);
	EventArgs e(0, 0, 0, 0);
	ControlEventArgs ee(&e);
	ee.control = control;
	OnControlRemoved(&ee);
}

ToolBar* FormFrame::GetToolBar(TCHAR* const ctlName) const
{
	CC_ASSERT(ctlName != NULL);

	if(ctlName != NULL && _lstToolBars != NULL)
	{
		for(int n = _lstToolBars->GetSize() - 1; n >= 0; n--)
		{
			ToolBar* ctl = (ToolBar*)_lstToolBars->GetKey(n);
			if(ctl->GetName() != NULL && _tcscmp(ctlName, ctl->GetName()) == 0)
			{
				return ctl;
			}
		}
	}
	return NULL;
}

ToolBar* FormFrame::GetToolBar(int nIndex) const
{
	if(_lstToolBars == NULL || nIndex < 0 || nIndex >= _lstToolBars->GetSize())
	{
		return NULL;
	}
	return (ToolBar*)_lstToolBars->GetKey(nIndex);
}

bool FormFrame::ContainsToolBar(Control* control) const
{
	return (_lstToolBars == NULL ? false : _lstToolBars->ContainsKey(control));
}

int FormFrame::GetToolBarCount() const
{
	return (_lstToolBars == NULL ? 0 : _lstToolBars->GetSize());
}

int FormFrame::GetToolBarDockSide(HWND hWnd) const
{
	CC_ASSERT(::IsWindow(hWnd));

	for(int n = 0; n < 4; n++)
	{
		if(::IsChild(_arrBarCont[n]->m_hWnd, hWnd))
		{
			return n;
		}
	}
	return -1;
}

//keyArr is param address,but sHead is by value
void FormFrame::DoConfig(bool isFromIni, KeyArr& keyArr, cc::Str sThisHead)
{
#if defined(_DEBUG_) || defined(_DESIGN)
	if(_designBar != NULL && CC_APP_GetDesignBarActive())
	{
		_designBar->Config(isFromIni, keyArr, sThisHead);
	}
#endif
	if(_lstBars != NULL)
	{
		for(int n = _lstBars->GetSize() - 1; n >= 0; n--)
		{
			_lstBars->GetKey(n)->Config(isFromIni, keyArr, sThisHead);
		}
	}
	if(_lstToolBars != NULL)
	{
		for(int n = _lstToolBars->GetSize() - 1; n >= 0; n--)
		{
			_lstToolBars->GetKey(n)->Config(isFromIni, keyArr, sThisHead);
		}
	}
	if(_arrBarCont[0] == NULL || !HasStyle(StyleType::Outer, OuterStyle::AutoConfig))
	{
		return;
	}

	CC_ASSERT(_lstToolBars != NULL);
	sThisHead.Append(_T(".ReBar"));
	int nCnt = _lstToolBars->GetSize();
	if(isFromIni)
	{
		for(int i = 0; i < nCnt; i++)
		{
			cc::Str stri = sThisHead + i;
			ToolBar* ctl = (ToolBar*)_lstToolBars->GetKey(i);
			int nDockSide = keyArr.Get(stri + _T(".DockSide"), -1);
			if(nDockSide < 0)
			{
				if(!ctl->GetFloating())
				{
					//origanl dockside
					int nOrgDockSide = GetToolBarDockSide(ctl->m_hWnd);
					CC_ASSERT(nOrgDockSide>= 0 && nOrgDockSide < 4);
					_arrBarCont[nOrgDockSide]->DeleteBand(ctl);
				}
				//get last pos
				int nConsoleX = keyArr.Get(stri + _T(".Left"), 0);
				int nConsoleY = keyArr.Get(stri + _T(".Top"), 0);
				if(keyArr.GetBool(stri + _T(".Vert"), false))
				{
					ctl->SetStyle(StyleType::Style, 0, CCS_VERT);
				}
				else
				{
					ctl->SetStyle(StyleType::Style, CCS_VERT, 0);
				}
				ctl->SetFloating(true, nDockSide > -2, nConsoleX, nConsoleY);
				if(nDockSide <= -2)
				{
					//Config is hide,set flag for OnLoad not show it
					ctl->SetVisible(false);
				}
			}
			else if(nDockSide < 4)
			{
				//origanl dockside
				if(ctl->GetFloating())
				{
					ctl->SetFloating(false);
					_arrBarCont[nDockSide]->InsertBand(ctl, -1, _eRebarStyle == ReBarStyle::Fix);
				}
				else
				{
					//origanl dockside
					int nOrgDockSide = GetToolBarDockSide(ctl->m_hWnd);
					CC_ASSERT(nOrgDockSide>= 0 && nOrgDockSide < 4);
					if(nOrgDockSide != nDockSide)
					{
						_arrBarCont[nOrgDockSide]->DeleteBand(ctl);
						_arrBarCont[nDockSide]->InsertBand(ctl, -1, _eRebarStyle == ReBarStyle::Fix);
					}
				}
			}
		}
	}
	else
	{
		for(int i = 0; i < nCnt; i++)
		{
			cc::Str stri = sThisHead + i;
			ToolBar* ctl = (ToolBar*)_lstToolBars->GetKey(i);
			//0:left;1:top;2:right;3:bottom;-1 for float,-2 for hied
			keyArr.Set(stri + _T(".DockSide"), ctl->GetFloating() ? (ctl->GetFloatContainer()->GetVisible() ? -1 : -2) : GetToolBarDockSide(ctl->m_hWnd));
			if(ctl->GetFloating())
			{
				//save position
				CC_ASSERT(ctl->GetFloatContainer() != NULL);
				CC_ASSERT(::IsWindow(ctl->GetFloatContainer()->m_hWnd));
				RECT rect;
				::GetWindowRect(ctl->GetFloatContainer()->m_hWnd, &rect);
				keyArr.Set(stri + _T(".Left"), rect.left);
				keyArr.Set(stri + _T(".Top"), rect.top);
				keyArr.SetBool(stri + _T(".Vert"), ctl->HasStyle(StyleType::Style, CCS_VERT));
			}
		}
	}

	for(int n = 0; n < 4; n++)
	{
		_arrBarCont[n]->SetStyle(StyleType::Outer, 0, OuterStyle::AutoConfig, SWP_FRAMECHANGED|SWP_DRAWFRAME);
		_arrBarCont[n]->Config(isFromIni, keyArr, sThisHead);
	}
}

void FormFrame::DoSetLang(KeyArr& keyArr, cc::Str sThisHead)
{
#if defined(_DEBUG_) || defined(_DESIGN)
	if(_designBar != NULL && CC_APP_GetDesignBarActive())
	{
		_designBar->SetLang(keyArr, sThisHead);
	}
#endif
	if(_lstBars != NULL)
	{
		for(int n = _lstBars->GetSize() - 1; n >= 0; n--)
		{
			_lstBars->GetKey(n)->SetLang(keyArr, sThisHead);
		}
	}
	if(_lstToolBars != NULL)
	{
		for(int n = _lstToolBars->GetSize() - 1; n >= 0; n--)
		{
			_lstToolBars->GetKey(n)->SetLang(keyArr, sThisHead);
		}
	}
	Container::DoSetLang(keyArr, sThisHead);
}

void FormFrame::OnNcHittest(EventArgs *e)
{
}
#endif //#if !defined(_CCTHINFORM)

DWORD FormConfig::nStyle = WS_CAPTION | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_SYSMENU | WS_THICKFRAME | WS_OVERLAPPED | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
DWORD FormConfig::nExStyle = WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR | WS_EX_WINDOWEDGE;
//must no CS_PARENTDC
//DWORD FormConfig::nClassStyle = CS_DBLCLKS | CS_BYTEALIGNWINDOW;
DWORD FormConfig::nInnerStyle = InnerStyle::TabLoopInner;
DWORD FormConfig::nOuterStyle = 0;
DWORD FormConfig::nOwnerDrawTextStyle = DT_LEFT|DT_VCENTER|DT_SINGLELINE;
COLORREF FormConfig::corBackground = ::GetSysColor(COLOR_BTNFACE);
COLORREF FormConfig::corTextground = ::GetSysColor(COLOR_BTNTEXT);
COLORREF FormConfig::corHotBackColor = ::GetSysColor(COLOR_BTNFACE);
COLORREF FormConfig::corHotTextColor = ::GetSysColor(COLOR_BTNTEXT);
DWORD FormConfig::nOuterStyleClient = OuterStyle::TransparentTop;
DWORD FormConfig::nInnerStyleClient = FormClientConfig::nInnerStyle;
COLORREF FormConfig::corBackgroundClient = FormConfig::corBackground;
UINT FormConfig::nWaitTimerToUpdateTips = 103;
//===========================================================cc::win::Form
///////////////////////////////////////////////////////////////////////////////
// constructors/destructor
Form::Form() : FormFrame()
{
	BASE_CONTROL_CONFIG_INIT(Form);
	//GetClient()->SetOuterStyle((OuterStyle::Type)FormConfig::nOuterStyleClient, true);
	GetClient()->SetBackground(FormConfig::corBackgroundClient);
	GetClient()->SetStyle(StyleType::Outer, (DWORD)0xFFFFFFFF, FormConfig::nOuterStyleClient);
	GetClient()->SetStyle(StyleType::Inner, (DWORD)0xFFFFFFFF, FormConfig::nInnerStyleClient);
	//GetClient()->SetBackgroundType(FormConfig::nInnerStyleClient);
	//for onclick to moveform
	_pClient->MouseDown += MouseEventHandler((Object*)this, (MouseEventFun)&Form::Client_OnMouseDown);
	//use window default pos
	::SetRect(&_Rect, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT + 300, CW_USEDEFAULT + 300);
	::CopyRect(&_OrgRect, &_Rect);

	//set a default value for the wnd caption
	SetText(_T("Form"));
	//set a default value for the wnd class name
	//_ClassName = CC_WND_32770;
	_ClassName = WndClassName::Form;

	//_bIsAlwaysOnTop = false;
	//_bIsCalledClosed = false;
	_bMessageLoopStopRequest = true;
	//_bIsMoveFormOnClick = false;
	SetIsMdiContainer(false);
	//_bIsActivated = false;
#if !defined(_CCTHINFORM)
	_pMainMenu = NULL;
	_pStatusBar = NULL;
#endif //#if !defined(_CCTHINFORM)
	_btnOK = NULL;
	_btnCancel = NULL;
#ifdef _DEBUG_
	SetName(_T("Form"));
#endif
	_eInitWindowState = FormWindowState::Normal;
	_eDialogResult = DialogResult::Empty;
	_hFocusBeforeDeactivate = NULL;
	_pLastToolTip = NULL;
	_hTipsHwnd = NULL;
	_maxSize.cx = 0;
	_maxSize.cy = 0;
	//_FormFlags = 0;
}

Form::~Form()
{
	//now release memory
	Dispose();
#if !defined(_CCTHINFORM)
	if(_pMainMenu != NULL)
	{
		delete _pMainMenu;
		_pMainMenu = NULL;
	}
	if(_pStatusBar != NULL)
	{
		delete _pStatusBar;
		_pStatusBar = NULL;
	}
#endif //#if !defined(_CCTHINFORM)
}
// constructors/destructor
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// private/protected/internal used methods
void Form::Dispose()
{
	if(!GetIsDispose())
	{
		if(!CC_BIT_HAS(_Flags, CC_FLAG_FORM_ISCALLEDCLOSED))
		{
			EventArgs e(m_hWnd, CC_WM_MAINMSG, CC_WM_MAINMSG_WPARAM_DISPOSE, NULL);
			OnClosed(&e);
		}
		FormFrame::Dispose();
#if !defined(_CCTHINFORM)
		if(_pMainMenu != NULL)
		{
			_pMainMenu->Dispose();
		}
		if(_pStatusBar != NULL)
		{
			_pStatusBar->Dispose();
		}
#endif //#if !defined(_CCTHINFORM)
		if(m_hWnd != NULL && IsWindow(m_hWnd))
		{
			//special for form, now destroy it
			::DestroyWindow(m_hWnd);
		}
	}
}

//keyArr is param address,but sHead is by value
void Form::DoConfig(bool isFromIni, KeyArr& keyArr, cc::Str sThisHead)
{
	CC_LOGTIME_START();
	if(!HasStyle(StyleType::Outer, OuterStyle::AutoConfig))
	{
#if !defined(_CCTHINFORM)
		if(_pStatusBar != NULL)
		{
			_pStatusBar->Config(isFromIni, keyArr, sThisHead);
		}
#endif //#if !defined(_CCTHINFORM)
		FormFrame::DoConfig(isFromIni, keyArr, sThisHead);
		return;
	}
	if(isFromIni)
	{
		//if not exist,as usually
		if(keyArr.Exist(sThisHead + _T(".Visible")))
		{
			if(cc::Str::Cmp(_ClassName, WndClassName::Dialog32770) != 0)
			{
				//get last pos
				RECT rect = this->GetBounds();
				int nConsoleX = keyArr.Get(sThisHead + _T(".Left"), rect.left);
				int nConsoleY = keyArr.Get(sThisHead + _T(".Top"), rect.top);
				int nConsoleW = keyArr.Get(sThisHead + _T(".Width"), rect.right - rect.left);
				int nConsoleH = keyArr.Get(sThisHead + _T(".Height"), rect.bottom - rect.top);
				cc::Util::AdjustWindowRect(nConsoleX, nConsoleY, nConsoleW, nConsoleH, _minSize.cx, _minSize.cy, _maxSize.cx, _maxSize.cy);
				this->SetBoundsNotOriginal(nConsoleX, nConsoleY, nConsoleW, nConsoleH);
			}

			//here set state,do it at setvisible.but the perfect is do it at constructors
			int nState = (int)keyArr.Get(sThisHead + _T(".State"), (DWORD)FormWindowState::Normal);
			//this->SetWindowState((FormWindowState::Type)nState);
			_eInitWindowState = (FormWindowState::Type)nState;

			//before show it
			FormFrame::DoConfig(isFromIni, keyArr, sThisHead);
#if !defined(_CCTHINFORM)
			if(_pStatusBar != NULL)
			{
				_pStatusBar->Config(isFromIni, keyArr, sThisHead);
			}
#endif //#if !defined(_CCTHINFORM)

			//sThisHead as Visible
			int nVis = keyArr.Get(sThisHead + _T(".Visible"), 1);
			this->SetVisible(nVis == 1);
		}
	}
	else
	{
		keyArr.SetBool(sThisHead + _T(".Visible"), GetVisible());
		if(cc::Str::Cmp(_ClassName, WndClassName::Dialog32770) != 0)
		{
			//save position
			RECT rect = this->GetNormalBounds();
			keyArr.Set(sThisHead + _T(".Left"), rect.left);
			keyArr.Set(sThisHead + _T(".Top"), rect.top);
			keyArr.Set(sThisHead + _T(".Width"), rect.right - rect.left);
			keyArr.Set(sThisHead + _T(".Height"), rect.bottom - rect.top);
		}
		int nState = (int)this->GetWindowState();
		keyArr.Set(sThisHead + _T(".State"), (DWORD)nState);

		FormFrame::DoConfig(isFromIni, keyArr, sThisHead);
#if !defined(_CCTHINFORM)
		if(_pStatusBar != NULL)
		{
			_pStatusBar->Config(isFromIni, keyArr, sThisHead);
		}
#endif //#if !defined(_CCTHINFORM)
	}
	CC_LOGTIME_PRINT();
}

void Form::DoSetLang(KeyArr& keyArr, cc::Str sThisHead)
{
	FormFrame::DoSetLang(keyArr, sThisHead);
#if !defined(_CCTHINFORM)
	if(_pStatusBar != NULL)
	{
		_pStatusBar->SetLang(keyArr, sThisHead);
	}
	if(_pMainMenu != NULL)
	{
		_pMainMenu->SetLang(keyArr, sThisHead);
	}
#endif //#if !defined(_CCTHINFORM)
	Update();
}

// WndProc - calls appriate On... function for the give
// message
LRESULT Form::WndProc(EventArgs *e)
{
	//CC_LogWndMsg(m_hWnd, e->Msg, e->WParam, e->LParam, _ClassName, _Name);
	/*
	from C#
	Form1_LocationChanged
	Form1_StyleChanged
	Form1_Load
	Form1_Layout
	Form1_VisibleChanged
	Form1_Activated
	Form1_Deactivate
	*/
	switch(e->Msg)
	{
	/*case WM_CTLCOLORSTATIC:
		{
			HWND hWnd = (HWND)e->LParam;
			if(hWnd != NULL)
			{
				Control* ctl = Control::FromHandle(hWnd);
				if(ctl != NULL)
				{
				}
			}
		}
		break;
	*/
	case WM_CLOSE:
		{
			CancelEventArgs ee(e);
			OnClosing(&ee);
			if(ee.Cancel)
			{
				return 0;
			}
    			OnClosed(e);
		}
		break;
	//case WM_FONTCHANGE:
	//	OnFontChanged(e);
	//	return 0;
	//case WM_CREATE:
	//	OnHandleCreated(e);
	//	break;
	case WM_QUIT:
	case WM_DESTROY:
	case WM_NCDESTROY:
		//Last message received by a window
		if(!CC_BIT_HAS(_Flags, CC_FLAG_FORM_ISCALLEDCLOSED))
		{
			OnClosed(e);
		}
		break;
	case WM_QUERYENDSESSION:
		{
			CC_TRACE((_T("WM_QUERYENDSESSION, m_hWnd:%d, ClassName:%s, Name:%s, Text:%s"), m_hWnd, _ClassName, _Name, _Text));
			e->Result = 1;
			//if cancel Windows's shutting down,then need set e->Result = 0
			OnShutdowning(e);
			//if(e->Result == 1)
			//{
			//}
		}
		break;
	case WM_ENDSESSION:
		CC_TRACE((_T("WM_ENDSESSION, m_hWnd:%d, ClassName:%s, Name:%s, Text:%s"), m_hWnd, _ClassName, _Name, _Text));
		OnShutdown(e);
		break;
	//case WM_INPUTLANGCHANGE:
		//InputLanguageChangedEventArgs ilChangedArgs =
		//	new InputLanguageChangedEventArgs();
		//OnInputLanguageChanged (ilChangedArgs);
	//	break;
	//case WM_INPUTLANGCHANGEREQUEST:
		//InputLanguageChangingEventArgs ilChangingArgs =
		//	new InputLanguageChangingEventArgs();
		//OnInputLanguagedChanging (ilChangingArgs);
	//	break;
	//case WM_MDIACTIVATE:
		//EventArgs mdiActivateArgs = new EventArgs();
		//OnMdiChildActivate (mdiActivateArgs);
	//	break;
	case WM_ACTIVATE:
		{
			//child may need this
			OnToolBarActivated(e);
			//MSG msg = {e->HWnd, e->Msg, e->WParam, e->LParam, 0, 0};
			//PreFormProcessMessage(&msg, 0);
			if(LOWORD(e->WParam) == WA_INACTIVE)
			{
				SetTips(NULL);
				// save focus window if focus is on this window's controls
				HWND hWndFocus = ::GetFocus();
				if(hWndFocus != NULL && ::IsChild(m_hWnd, hWndFocus))
				{
					_hFocusBeforeDeactivate = hWndFocus;
				}
				OnDeactivate(e);
			}
			else
			{
				OnActivated(e);
				if(_ClassName == WndClassName::Dialog32770)
				{
					//::SetTimer(m_hWnd, CC_TIMERID_DIALOG_ACTIVATED, 100, NULL);
					HWND hWndFocus = ::GetFocus();
					if(hWndFocus != NULL && ::IsChild(m_hWnd, hWndFocus))
					{
						// invalid or unknown focus window... let windows handle it
						_hFocusBeforeDeactivate = hWndFocus;
					}
					if(!::IsWindow(_hFocusBeforeDeactivate) || !::IsChild(m_hWnd, _hFocusBeforeDeactivate))
					{
						// invalid or unknown focus window... let windows handle it
						_hFocusBeforeDeactivate = NULL;
					}
					else
					{
						// otherwise, set focus to the last known focus window
						::SetFocus(_hFocusBeforeDeactivate);
						::InvalidateRect(_hFocusBeforeDeactivate, NULL, TRUE);
#ifdef _DEBUG_
						//Control* c = Control::FromHandle(_hFocusBeforeDeactivate);
						//if(c != NULL)
						//{
						//	cc::Log::debug(_T("WM_ACTIVATE, FocusControl, Text:%s, Name:%s"), c->GetText(), c->GetName());
						//}
#endif
					}
					break;
				}
			}
		}
		break;
	//case WM_NCACTIVATE:
	//	break;
	case WM_SETFOCUS:
		{
			if(!::IsWindow(_hFocusBeforeDeactivate) || !::IsChild(m_hWnd, _hFocusBeforeDeactivate))
			{
				// invalid or unknown focus window... let windows handle it
				_hFocusBeforeDeactivate = NULL;
				//Control* pWndNext = GetNextControl(NULL);
				//if(pWndNext != NULL)
				//{
				//	pWndNext->Focus();
				//}
			}
			else
			{
				//Control* pWndNext = Control::FromHandle(_hFocusBeforeDeactivate);
				//cc::Log::debug(_T("focus:%s, %s\r\n"), (TCHAR*)pWndNext->GetClassName().GetBuf(), pWndNext->GetText());
				// otherwise, set focus to the last known focus window
				::SetFocus(_hFocusBeforeDeactivate);
				//An application should return zero if it processes this message.
				e->Result = 0;
				e->Handled = true;
			}
		}
		break;
	case WM_ACTIVATEAPP:
		{
			OnActivateApp(e);
		}
		break;
		//return wActivateApp(wParam, lParam);
	case WM_GETMINMAXINFO:
		OnGetMinMaxInfo(e);
		break;
	case CC_WM_MAINMSG:
		{
			switch(e->WParam)
			{
			////dispose control
			//case CC_WM_MAINMSG_WPARAM_DISPOSE:
			//	if(!_bIsCalledClosed)
			//	{
			//		OnClosed(e);
			//	}
			//	break;
			//status&tooltip
			case CC_WM_MAINMSG_WPARAM_FOCUSCHANGED:
				{
					OnFocusChanged(e);
				}
				return (LRESULT)0;
			case CC_WM_MAINMSG_WPARAM_GETFORM:
				return (LRESULT)this;
#if !defined(_CCTHINFORM)
			case CC_WM_MAINMSG_WPARAM_MENUSTATUS:
			case CC_WM_MAINMSG_WPARAM_CTLSTATUS:
				{
					if(_pStatusBar != NULL || App::GetToolTipActive())
					{
						::KillTimer(m_hWnd, CC_TIMERID_CONTROL_TIMEOUT);
						if(Focused() || e->WParam == CC_WM_MAINMSG_WPARAM_MENUSTATUS || App::GetAlwaysTips())
						{
							if(e->WParam == CC_WM_MAINMSG_WPARAM_MENUSTATUS)
							{
								_hTipsHwnd = cc::Util::GetTopMenuHwnd();
							}
							else
							{
								_hTipsHwnd = NULL;
							}
							if(_pLastToolTip != e->LParam)
							{
								//come next tips
								_pLastToolTip = e->LParam;
								SetTips(NULL);
								SetStatus(NULL);
							}

							ITextIcon* item = (ITextIcon*)e->LParam;
							if(item != NULL)
							{
								SetStatus(item->GetTips(), item->GetImageList(), item->GetTipsIconID());
								SetTips(item->GetTipsExt(), item->GetImageList(), item->GetTipsExtIconID());
							}

							//start timer
							::SetTimer(m_hWnd, CC_TIMERID_CONTROL_TIMEOUT, FormConfig::nWaitTimerToUpdateTips, NULL);
						}
					}
				}
				return (LRESULT)TRUE;
			case CC_WM_MAINMSG_WPARAM_TEXTSTATUSONLY:
				{
					SetStatus((const TCHAR*)e->LParam);
				}
				return (LRESULT)TRUE;
			case CC_WM_MAINMSG_WPARAM_TEXTTIPSONLY:
				{
					SetTips((const TCHAR*)e->LParam);
				}
				return (LRESULT)TRUE;
			case CC_WM_MAINMSG_WPARAM_TEXTSTATUS:
				{
					SetTips((const TCHAR*)e->LParam);
					SetStatus((const TCHAR*)e->LParam);
				}
				return (LRESULT)TRUE;
			case CC_WM_MAINMSG_WPARAM_CLEARSTATUS:
				{
					//e->LParam == NULL is menu,and _pLastToolTip == e->LParam is for control
					//  for sometime clear msg is slowly than settips msg
					if(e->LParam == NULL || _pLastToolTip == e->LParam)
					{
						::KillTimer(m_hWnd, CC_TIMERID_CONTROL_TIMEOUT);
						_pLastToolTip = NULL;
						_hTipsHwnd = NULL;
						SetStatus(NULL);
						SetTips(NULL);
					}
				}
				return (LRESULT)TRUE;
			case CC_WM_MAINMSG_WPARAM_ACTIVATE:
                {
					EventArgs *msg = (EventArgs*)e->LParam;
					OnToolBarActivated(msg);
				}
				return (LRESULT)TRUE;
#endif //#if !defined(_CCTHINFORM)
			//sample for others
			//case 999:
			//	{
			//	}
			//	break;
			} //switch(e->WParam)
		}
		break;
	//case WM_WININICHANGE:
	//	{
	//		RECT rect;
	//		::GetWindowRect(m_hWnd, &rect);
	//		int nConsoleW = rect.right - rect.left;
	//		int nConsoleH = rect.bottom - rect.top;
	//		cc::Util::AdjustWindowRect((int&)rect.left, (int&)rect.top, nConsoleW, nConsoleH, _minSize.cx, _minSize.cy, _maxSize.cx, _maxSize.cy);
	//		this->SetBoundsNotOriginal(rect.left, rect.top, nConsoleW, nConsoleH);
	//	}
	//	break;
	case 0x031A: //WM_THEMECHANGED:
		{
			OnThemeChanged();
		}
		break;
	case WM_SYSCOLORCHANGE:
		{
			OnSysColorChanged();
		}
		break;
//	case WM_POWERBROADCAST:
//	case WM_DISPLAYCHANGE:
/*
		wParam:
		  The new image depth of the display, in bits per pixel.
		lParam:
		  The low-order word specifies the horizontal resolution of the screen.
		  The high-order word specifies the vertical resolution of the screen.
*/
//		break;
#if !defined(_CCTHINFORM)
#if defined(_DEBUG_) || defined(_DESIGN)
	case WM_NCRBUTTONUP:
		if(CC_BIT_HAS((DWORD)Design, DesignStyle::ForceDesign) || (!CC_BIT_HAS(DesignConfig::nGlobalFlag, CC_DESIGN_G_DENIAL) && !CC_BIT_HAS((DWORD)Design, DesignStyle::NoDesign)))
		{
			RECT rect;
			::GetWindowRect(m_hWnd, &rect);
			POINTS pt = MAKEPOINTS(e->LParam);
			if(pt.x > rect.right - 12 && pt.x <= rect.right && pt.y >= rect.top && pt.y < rect.top + 12)
			{
				//for design while debug
				_designBar = App::ShowDesignBar(this);
			}
		}
		break;
#endif
#endif //#if !defined(_CCTHINFORM)
	default:
		break;
	}
	return FormFrame::WndProc(e);
}

#if defined(_DEBUG_) || defined(_DESIGN)
FloatContainer* Form::CreateDesignBar()
{
	return CC_APP_CreateDesignBar();
}
#endif

void Form::OnInnerTimer(EventArgs *e)
{
	//e->WParam is ID
//	if(e->WParam == CC_TIMERID_DIALOG_ACTIVATED)
//	{
//		::KillTimer(m_hWnd, CC_TIMERID_DIALOG_ACTIVATED);
//		HWND hWndFocus = ::GetFocus();
//		if(hWndFocus != NULL && ::IsChild(m_hWnd, hWndFocus))
//		{
//			// invalid or unknown focus window... let windows handle it
//			_hFocusBeforeDeactivate = hWndFocus;
//		}
//		if(!::IsWindow(_hFocusBeforeDeactivate) || !::IsChild(m_hWnd, _hFocusBeforeDeactivate))
//		{
//			// invalid or unknown focus window... let windows handle it
//			_hFocusBeforeDeactivate = NULL;
//		}
//		else
//		{
//			// otherwise, set focus to the last known focus window
//			::SetFocus(_hFocusBeforeDeactivate);
//			::InvalidateRect(_hFocusBeforeDeactivate, NULL, TRUE);
//#ifdef _DEBUG_
//			Control* c = Control::FromHandle(_hFocusBeforeDeactivate);
//			cc::Log::debug(_T("c:%s, n:%s"), c->GetText(), c->GetName());
//#endif
//		}
//		return;
//	}
	if(e != NULL && e->WParam == CC_TIMERID_FORM_CHGTHEME)
	{
		::KillTimer(m_hWnd, CC_TIMERID_FORM_CHGTHEME);
		RECT rect;
		::GetWindowRect(m_hWnd, &rect);
		this->SetBounds(0, 0, rect.right - rect.left + 5, rect.bottom - rect.top + 5, BoundsSpecified::Size);
		return;
	}
	if(e != NULL && e->WParam != CC_TIMERID_CONTROL_TIMEOUT)
	{
		FormFrame::OnInnerTimer(e);
		return;
	}
	::KillTimer(m_hWnd, CC_TIMERID_CONTROL_TIMEOUT);
#if !defined(_CCTHINFORM)
	//for when menu of notifyicon then form is not _bIsActivated
	if(_pLastToolTip == NULL || (!Focused() && _hTipsHwnd == NULL && !App::GetAlwaysTips()))
	{
		SetStatus(NULL);
		SetTips(NULL);
		return;
	}
	//clear tips for mouse is out of menu
	if(_hTipsHwnd != NULL)
	{
		POINT pt;
		GetCursorPos(&pt);
		if(_hTipsHwnd != ::WindowFromPoint(pt))
		{
			_hTipsHwnd = NULL;
			SetTips(NULL);
		}
	}
#endif //#if !defined(_CCTHINFORM)
	//keep show tips
	if(App::GetToolTipActive())
	{
		App::GetToolTip()->KeepShow();
	}
#if !defined(_CCTHINFORM)
	if(_pStatusBar != NULL && _pStatusBar->GetVisible())
	{
		_pStatusBar->KeepShow();
	}
#endif //#if !defined(_CCTHINFORM)
	::SetTimer(m_hWnd, CC_TIMERID_CONTROL_TIMEOUT, FormConfig::nWaitTimerToUpdateTips, NULL);
}
// private/protected/internal used methods
///////////////////////////////////////////////////////////////////////////////
//bool Form::PreProcessMessage(MSG* pMsg, UINT_PTR keyData)
//{
//	return FormFrame::PreProcessMessage(pMsg, keyData);
//}
//

// This function lets Windows or/and default Windows control process message
// Classes have to call it if they do not handle message in WndProc or
// default handling is needed.
LRESULT Form::DefWndProc(EventArgs *e)
{
	CC_ASSERT(e != NULL);

	if(HasStyle(StyleType::Inner, InnerStyle::IsMdiContainer))
	{
		CC_ASSERT(_pClient != NULL);
		return ::DefFrameProc(e->HWnd, _pClient->m_hWnd, e->Msg, e->WParam, e->LParam);
	}
	else
	{
		return FormFrame::DefWndProc(e);
	}
}

LRESULT Form::DefWndProc(int msg, WPARAM wParam, LPARAM lParam)
{
	CC_ASSERT(::IsWindow(m_hWnd));

	if(HasStyle(StyleType::Inner, InnerStyle::IsMdiContainer))
	{
		CC_ASSERT(_pClient != NULL);
		return ::DefFrameProc(m_hWnd, _pClient->m_hWnd, msg, wParam, lParam);
	}
	else
	{
		return FormFrame::DefWndProc(msg, wParam, lParam);
	}
}

bool Form::PreFormProcessMessage(MSG* pMsg, UINT_PTR& keyData)
{
#if !defined(_CCTHINFORM)
	if(_pStatusBar != NULL)
	{
		//force to deliver
		if(_pStatusBar->PreFormProcessMessage(pMsg, keyData))
		{
			return true;
		}
	}
#endif //#if !defined(_CCTHINFORM)
	if(FormFrame::PreFormProcessMessage(pMsg, keyData))
	{
		return true;
	}
	//20080828, for child dialog cannot get key,turn to GetParent
	//if(_pParent == NULL && (keyData & SHORTCUT_ALT) == SHORTCUT_ALT && pMsg->message == WM_SYSKEYDOWN)
	if(pMsg->message == WM_SYSKEYDOWN && (keyData & SHORTCUT_ALT) == SHORTCUT_ALT && ::GetParent(m_hWnd) == NULL)
	{
		//if(GetTopLevel() && ProcessMnemonic(charCode))
		//This method is called to give a control the opportunity to process a mnemonic character.
		return ProcessMnemonic((TCHAR)(pMsg->wParam));
	}
	return false;
}

bool Form::ProcessCmdKey(MSG* pMsg, UINT_PTR& keyData)
{
#if !defined(_CCTHINFORM)
	if(_pMainMenu != NULL && _pMainMenu->ProcessCmdKey(pMsg, keyData))
	{
		return true;
	}
#endif //#if !defined(_CCTHINFORM)
	//first child.
	if(FormFrame::ProcessCmdKey(pMsg, keyData))
	{
		return true;
	}
	return false;
}

// Process a dialog key.
bool Form::ProcessDialogKey(UINT_PTR& keyData)
{
	if(_btnCancel != NULL && keyData == VK_ESCAPE)
	{
		EventArgs e(m_hWnd, WM_KEYDOWN, VK_ESCAPE, NULL);
		_btnCancel->PerformClick(&e);
		return true;
	}
	else if(_btnOK != NULL && keyData == VK_RETURN)
	{
		EventArgs e(m_hWnd, WM_KEYDOWN, VK_RETURN, NULL);
		_btnOK->PerformClick(&e);
		return true;
	}
	return FormFrame::ProcessDialogKey(keyData);
}

bool Form::ProcessKeyPreview(EventArgs *e)
{
	if(CC_BIT_HAS(_Flags, CC_FLAG_FORM_ISKEYPREVIEW))
	{
		if(ProcessKeyEventArgs(e))
		{
			return true;
		}
	}
	return FormFrame::ProcessKeyPreview(e);
}

bool Form::GetKeyPreview()
{
	return CC_BIT_HAS(_Flags, CC_FLAG_FORM_ISKEYPREVIEW);
}

void Form::SetKeyPreview(bool value)
{
	CC_BIT_SET(_Flags, CC_FLAG_FORM_ISKEYPREVIEW, value);
}

bool Form::GetCloseButtonEnable()
{
	return !HasStyle(StyleType::Inner, InnerStyle::CloseButtonDisable);
}

void Form::SetCloseButtonEnable(bool value)
{
	SetStyleBool(StyleType::Inner, InnerStyle::CloseButtonDisable, !value);
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
	{
		//modifying container's system menu.
		HMENU sysMenu = ::GetSystemMenu(m_hWnd, FALSE);
		if(sysMenu != NULL)
		{
			::EnableMenuItem(sysMenu, SC_CLOSE, value ? MF_BYCOMMAND | MF_ENABLED : MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		}
	}
}

bool Form::GetMinButtonEnable()
{
	return !HasStyle(StyleType::Inner, InnerStyle::MinButtonDisable);
}

void Form::SetMinButtonEnable(bool value)
{
	SetStyleBool(StyleType::Inner, InnerStyle::MinButtonDisable, !value);
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
	{
		SetStyleBool(StyleType::Style, WS_MINIMIZEBOX, value);
	}
}

bool Form::GetMaxButtonEnable()
{
	return !HasStyle(StyleType::Inner, InnerStyle::MaxButtonDisable);
}

void Form::SetMaxButtonEnable(bool value)
{
	SetStyleBool(StyleType::Inner, InnerStyle::MaxButtonDisable, !value);
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
	{
		SetStyleBool(StyleType::Style, WS_MAXIMIZEBOX, value);
	}
}

bool Form::GetAlwaysOnTop()
{
	//return _bIsAlwaysOnTop;
	return CC_BIT_HAS(_Flags, CC_FLAG_FORM_ISALWAYSONTOP);
}

void Form::SetAlwaysOnTop(bool value)
{
	//_bIsAlwaysOnTop = value;
	CC_BIT_SET(_Flags, CC_FLAG_FORM_ISALWAYSONTOP, value);
	//if(value)
	//{
	//	_Flags = _Flags | CC_FLAG_FORM_ISALWAYSONTOP;
	//}
	//else
	//{
	//	_Flags = _Flags & ~CC_FLAG_FORM_ISALWAYSONTOP;
	//}
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
	{
		::SetWindowPos(m_hWnd, value ? HWND_TOPMOST : HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}
}

FormWindowState::Type Form::GetWindowState()
{
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
	{
		WINDOWPLACEMENT lpwndpl;
		::GetWindowPlacement(m_hWnd, &lpwndpl);
		_eInitWindowState = (FormWindowState::Type)lpwndpl.showCmd;
	}
	return _eInitWindowState;
}

void Form::SetWindowState(FormWindowState::Type value, bool notSetWhileHide)
{
	_eInitWindowState = value;
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
	{
		if(notSetWhileHide && !GetVisible())
		{
			return;
		}
		if(value == FormWindowState::Maximized)
		{
			::ShowWindow(m_hWnd, SW_MAXIMIZE);
		}
		else if(value == FormWindowState::Minimized)
		{
			::ShowWindow(m_hWnd, SW_MINIMIZE);
		}
		else
		{
			::ShowWindow(m_hWnd, SW_SHOW);
			::ShowWindow(m_hWnd, SW_RESTORE);
		}
		//WINDOWPLACEMENT lpwndpl;
		//::GetWindowPlacement(m_hWnd, &lpwndpl);
		//lpwndpl.showCmd = (int)value;
		//::SetWindowPlacement(m_hWnd, &lpwndpl);
	}
}

void Form::Client_OnMouseDown(Object* sender, MouseEventArgs* e)
{
	if(GetMoveFormOnClick() && e->Button == MouseButtons::Left)
	{
		e->baseArgs->Result = DefWndProc(e->baseArgs);
		e->baseArgs->Handled = true;
		::ReleaseCapture();
		::SendMessage(m_hWnd, WM_NCLBUTTONDOWN, HTCAPTION, NULL);
	}
}

void Form::OnMouseDown(MouseEventArgs* e)
{
	FormFrame::OnMouseDown(e);
	Client_OnMouseDown(this, e);
}

Button* Form::GetCancelButton()
{
	return _btnCancel;
}

void Form::SetCancelButton(Button* btn)
{
	_btnCancel = btn;
}

Button* Form::GetOKButton()
{
	return _btnOK;
}

void Form::SetOKButton(Button* btn)
{
	_btnOK = btn;
}

bool Form::GetMoveFormOnClick()
{
	return CC_BIT_HAS(_Flags, CC_FLAG_FORM_ISMOVEFORMONCLICK);
}

void Form::SetMoveFormOnClick(bool value)
{
	//_bIsMoveFormOnClick = value;
	CC_BIT_SET(_Flags, CC_FLAG_FORM_ISMOVEFORMONCLICK, value);
	//if(value)
	//{
	//	_Flags = _Flags | CC_FLAG_FORM_ISMOVEFORMONCLICK;
	//}
	//else
	//{
	//	_Flags = _Flags & ~CC_FLAG_FORM_ISMOVEFORMONCLICK;
	//}
}

bool Form::GetIsMdiContainer()
{
	return HasStyle(StyleType::Inner, InnerStyle::IsMdiContainer);
}

void Form::SetIsMdiContainer()
{
	SetStyle(StyleType::Inner, 0, InnerStyle::IsMdiContainer);
	((FormClient*)_pClient)->SetIsMdiContainer();
}

void Form::SetIsMdiContainer(bool value)
{
	CC_ASSERT(_pClient != NULL);

	SetStyleBool(StyleType::Inner, InnerStyle::IsMdiContainer, value);
	if(value)
	{
		//TODO:add FormConfig for Mdis
		//_pClient->SetTextColor(RGB(0, 0, 0));
		//int nType = _pClient->GetBackgroundType();
		//_pClient->SetBackground(::GetSysColor(COLOR_APPWORKSPACE));
		//_pClient->SetBackgroundType(nType);
		//_pClient->SetStyle(StyleType::Style, 0, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_VSCROLL | WS_HSCROLL);
		//_pClient->SetStyle(StyleType::Style, 0, WS_OVERLAPPED | WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
		//_pClient->SetStyle(StyleType::ExStyle, 0, WS_EX_CLIENTEDGE | WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR);
	}
	//20081025,do nothing(also if SetBackground, then SetBackgroundType of _pClient auto changed to CC_BACK_TYPE_COLOR)
	//else
	//{
	//	//_pClient->SetTextColor(::GetSysColor(COLOR_BTNTEXT));
	//	_pClient->SetBackground(::GetSysColor(COLOR_BTNFACE));
	//	_pClient->SetStyle(StyleType::Style, WS_OVERLAPPED | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, WS_CHILD | WS_VISIBLE);
	//	_pClient->SetStyle(StyleType::ExStyle, 1, 0);
	//}
}

void Form::SetVisible(bool value)
{
	//_Visible = value;
	//CC_BIT_SET(_Flags, CC_FLAG_CONTROL_VISIBLE, value);
	SetStyleBool(StyleType::Inner2, Inner2Style::Visible, value);
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
	{
		if(!value)
		{
			//save state
			GetWindowState();
			::ShowWindow(m_hWnd, SW_HIDE);
		}
		else
		{
			FormWindowState::Type nState = _eInitWindowState;
			if(nState != GetWindowState())
			{
				SetWindowState(nState);
			}
			::ShowWindow(m_hWnd, SW_SHOW);
			::SetForegroundWindow(m_hWnd);
		}
	}
}

void Form::SetMainMenu(int nIDMenu)
{
	_hMenu = LoadMenu(CC_APP_GetInstance(), MAKEINTRESOURCE(nIDMenu));
#if !defined(_CCTHINFORM)
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0 && _pMainMenu == NULL)
#else
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
#endif //#if !defined(_CCTHINFORM)
	{
		::SetMenu(m_hWnd, _hMenu);
	}
}

#if !defined(_CCTHINFORM)
void Form::SetMainMenu(MainMenu* mainMenu)
{
	_pMainMenu = mainMenu;
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
	{
		_pMainMenu->CreateControl();
		::SetMenu(m_hWnd, _pMainMenu->m_hMenu);
	}
}

MainMenu* Form::GetMainMenu()
{
	return _pMainMenu;
}

StatusBar* Form::GetStatusBar()
{
	return _pStatusBar;
}

void Form::SetStatusBar(StatusBar* statusBar)
{
	_pStatusBar = statusBar;
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0 && _pStatusBar != NULL)
	{
		_pStatusBar->SetParent(this, true);
		if(_pStatusBar->m_hWnd == NULL)
		{
			_pStatusBar->CreateControl();
		}
		EventArgs e(m_hWnd, 0, 0, 0);
		OnResize(&e);
	}
}

void Form::SetStatus(const TCHAR* status, ImageList* imageList, int nIconID, int nIndex, bool isSetDefault)
{
	if(_pStatusBar != NULL && _pStatusBar->GetVisible())
	{
		_pStatusBar->SetText(status, imageList, nIconID, nIndex);
		if(isSetDefault)
		{
			//this time,nIndex is for defaultindex
			_pStatusBar->SetDefault(status, imageList, nIconID, nIndex);
		}
	}
}

void Form::SetTips(const TCHAR* status, ImageList* imageList, int nIconID)
{
	if(App::GetToolTipActive() || App::GetAlwaysTips())
	{
		POINT pt;
		GetCursorPos(&pt);
		App::GetToolTip()->Show(status, imageList, nIconID, pt.x, pt.y, -1, _hTipsHwnd != NULL);
	}
}

void Form::SetTips(const TCHAR* status, ImageList* imageList, int nIconID, int x, int y)
{
	if(App::GetToolTipActive() || App::GetAlwaysTips())
	{
		App::GetToolTip()->Show(status, imageList, nIconID, x, y, -1, _hTipsHwnd != NULL);
	}
}
#endif //#if !defined(_CCTHINFORM)

//SetStype&SetExStyle should be called before this,also ignore toolbars
//while Controls's positon is fixed,then recommend use SetSizeBaseOnClient(alse call SetClientSize).
//while form'size is fixed,recommend to use this
void Form::SetSizeBaseOnClient(int cxWidth, int cyHeight)
{
	RECT rect;
	rect.left = 0;
	rect.right = cxWidth;
	rect.top = 0;
	rect.bottom = cyHeight;
	SetClientSize(cxWidth, cyHeight);
#if !defined(_CCTHINFORM)
	::AdjustWindowRectEx(&rect, GetStyle(StyleType::Style), (_pMainMenu != NULL || _hMenu != NULL), GetStyle(StyleType::ExStyle));
#else
	::AdjustWindowRectEx(&rect, GetStyle(StyleType::Style), (_hMenu != NULL), GetStyle(StyleType::ExStyle));
#endif //#if !defined(_CCTHINFORM)
	SetSize(rect.right - rect.left, rect.bottom - rect.top);
}

RECT Form::CalcuteRect(bool updateRect, int nExpectX, int nExpectY, int nExpectCX, int nExpectCY, bool isFourceCalcute)
{
	return GetBounds();
}

//return Bounds relate to screen
RECT Form::GetBounds()
{
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
	{
		::GetWindowRect(m_hWnd, &_Rect);
	}
	return _Rect;
}

void Form::SetBoundsCore(int left, int top, int width, int height, BoundsSpecified::Type specified, bool isSetSameOriginal)
{
	if(specified == BoundsSpecified::Size || specified == BoundsSpecified::All)
	{
		//we thing _maxSize is bigger than _minSize
		if(_maxSize.cx > 0 && width > _maxSize.cx)
		{
			width = _maxSize.cx;
		}
		else if(width < _minSize.cx)
		{
			width = _minSize.cx;
		}
		if(_maxSize.cy > 0 && height > _maxSize.cy)
		{
			height = _maxSize.cy;
		}
		else if(height < _minSize.cy)
		{
			height = _minSize.cy;
		}
	}
	FormFrame::SetBoundsCore(left, top, width, height, specified, isSetSameOriginal);
}

//should do this after SetSize,parent must be Form
//!!if has DS_CENTER then SetCenter at FormLoad without isOnScreen
void Form::SetCenter(bool isOnScreen)
{
	Form* frmParent = (Form*)_pParent;
	RECT rcScreen = {0, 0, 0, 0};
	if(!::SystemParametersInfo(SPI_GETWORKAREA, 0, &rcScreen, 0))
	{
		rcScreen.right = (LONG)::GetSystemMetrics(SM_CXSCREEN);
		rcScreen.bottom = (LONG)::GetSystemMetrics(SM_CYSCREEN);
	}

	RECT rcCenter;
	if(!isOnScreen && _pParent != NULL && frmParent->GetVisible() && frmParent->GetWindowState() != FormWindowState::Minimized)
	{
		rcCenter = frmParent->GetNormalBounds();
	}
	else
	{
		::CopyRect(&rcCenter, &rcScreen);
	}

	_Rect = GetBounds();
	int w = _Rect.right - _Rect.left;
	int h = _Rect.bottom - _Rect.top;
	_Rect.left = (rcCenter.left + rcCenter.right) / 2 - w / 2;
	_Rect.top = (rcCenter.top + rcCenter.bottom) / 2 - h / 2;

	//CC_TRACE((_T("Control::SetBoundsCore[%s,%s]ParaL:%d, ParaT:%d, ParaW:%d, ParaH:%d, NewL:%d, NewT:%d, NewW:%d, NewH:%d"),
	//			this->_ClassName, this->_Name, _Rect.left, _Rect.top, w, h, rcScreen.left, rcScreen.top, rcScreen.right - rcScreen.left, rcScreen.bottom - rcScreen.top));
	
	if(_Rect.left + w > rcScreen.right)
	{
		_Rect.left = rcScreen.right - w;
	}
	if(_Rect.top + h > rcScreen.bottom)
	{
		_Rect.top = rcScreen.bottom - h;
	}

	//CC_TRACE((_T("Control::SetBoundsCore[%s,%s]ParaL:%d, ParaT:%d, ParaW:%d, ParaH:%d, NewL:%d, NewT:%d, NewW:%d, NewH:%d"),
	//			this->_ClassName, this->_Name, _Rect.left, _Rect.top, w, h, rcScreen.left, rcScreen.top, rcScreen.right - rcScreen.left, rcScreen.bottom - rcScreen.top));
	
	if(_Rect.left < rcScreen.left)
	{
		_Rect.left = rcScreen.left;
	}
	if(_Rect.top < rcScreen.top)
	{
		_Rect.top = rcScreen.top;
	}
	_Rect.right = _Rect.left + w;
	_Rect.bottom = _Rect.top + h;


	//CC_TRACE((_T("Control::SetBoundsCore[%s,%s]ParaL:%d, ParaT:%d, ParaW:%d, ParaH:%d, NewL:%d, NewT:%d, NewW:%d, NewH:%d"),
	//			this->_ClassName, this->_Name, _Rect.left, _Rect.top, w, h, rcScreen.left, rcScreen.top, rcScreen.right - rcScreen.left, rcScreen.bottom - rcScreen.top));
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
	{
		// map screen coordinates to child coordinates
		::SetWindowPos(m_hWnd, NULL, _Rect.left, _Rect.top, -1, -1,
			SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
	}
}

bool Form::Focused()
{
	CC_ASSERT(::IsWindow(m_hWnd));

	return CC_BIT_HAS(_Flags, CC_FLAG_FORM_ISACTIVATED);
	//return _bIsActivated;
}

#if !defined(_CCTHINFORM)
void Form::Menu_OnEnterLoop(EventArgs* e)
{
	UpdateMainMenu();
}

void Form::UpdateMainMenu()
{
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0 && _pMainMenu != NULL && _pMainMenu->GetIsNeedUpdate())
	{
		_pMainMenu->CreateControl();
		::SetMenu(m_hWnd, _pMainMenu->m_hMenu);
	}
}
#endif //#if !defined(_CCTHINFORM)

void Form::OnThemeChanged()
{
	//refresh to get IsAppThemed
	cc::Theme::IsAppThemed(true);
	cc::Theme::IsThemeActive(true);
	CC_APP_GetDrawActor()->OnThemeChanged();
#if !defined(_CCTHINFORM)
	if(m_hWnd != NULL && _arrBarCont[0] != NULL)
	{
		//reshow rebar
		RECT rect;
		::GetWindowRect(m_hWnd, &rect);
		this->SetBounds(0, 0, rect.right - rect.left + 5, rect.bottom - rect.top + 5, BoundsSpecified::Size);
		this->SetBounds(0, 0, rect.right - rect.left - 10, rect.bottom - rect.top - 10, BoundsSpecified::Size);

		//start timer
		::SetTimer(m_hWnd, CC_TIMERID_FORM_CHGTHEME, 500, NULL);
	}
#endif //#if !defined(_CCTHINFORM)
}

void Form::OnSysColorChanged()
{
	MainMenuBarConfig::corBackground = ::GetSysColor(COLOR_BTNFACE);
	MainMenuBarConfig::corTextground = ::GetSysColor(COLOR_MENUTEXT);
	MainMenuBarConfig::corHotBackColor = ::GetSysColor(COLOR_HIGHLIGHT);
	MainMenuBarConfig::corHotTextColor = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
	DrawActorConfig::corMenuBackground = ::GetSysColor(COLOR_MENU);
	DrawActorConfig::corMenuTextground = ::GetSysColor(COLOR_MENUTEXT);
	DrawActorConfig::corMenuHotBackColor = ::GetSysColor(COLOR_HIGHLIGHT);
	DrawActorConfig::corMenuHotTextColor = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
	DrawActorConfig::corMenuDisabledTextground = ::GetSysColor(COLOR_GRAYTEXT);
	InvalidateBackGround();
}

void Form::Show()
{
	//deactivate mainform,for savefocus for that form
	Control* ctl = App::GetActiveForm();
	if(ctl != NULL && ctl->m_hWnd != m_hWnd)
	{
		SendMessage(ctl->m_hWnd, WM_ACTIVATE, WA_INACTIVE, 0);
	}
	FormFrame::Show();
	Update();
}

//Bounds relate to screen
RECT Form::GetNormalBounds()
{
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
	{
		WINDOWPLACEMENT lpwndpl;
		::GetWindowPlacement(m_hWnd, &lpwndpl);
		::CopyRect(&_Rect, &lpwndpl.rcNormalPosition);
	}
	return _Rect;
}

///////////////////////////////////////////////////////////////////////////////
// On...
void Form::OnActivated(EventArgs* e)
{
	CC_BIT_ON(_Flags, CC_FLAG_FORM_ISACTIVATED);
	//_Flags |= CC_FLAG_FORM_ISACTIVATED;
	//_bIsActivated = true;
	_APP_THREAD_STATE* pState = App::GetThreadStateData();
	pState->m_pActiveWnd = this->m_hWnd;
	Activated.Invoke((Object*)this, e);
}

int* Form::GetMessageLoopFlag()
{
	return &_bMessageLoopStopRequest;
}

void Form::OnClosing(CancelEventArgs* e)
{
	Closing.Invoke((Object*)this, e);

	//if(Modal)
	//{
	//	e.Cancel = true; // don't destroy modal form
	//	DialogResult = DialogResult::Cancel;
	//}
}

void Form::OnClosed(EventArgs* e)
{
#if _DEBUG_
	CC_LOGTIME_START();
#endif
	//end meg loop for showdialog perhaps
	_bMessageLoopStopRequest = true;
	//_bIsCalledClosed = true;
	CC_BIT_ON(_Flags, CC_FLAG_FORM_ISCALLEDCLOSED);
	//_Flags |= CC_FLAG_FORM_ISCALLEDCLOSED;
	Closed.Invoke((Object*)this, e);
	if(!e->Handled && _eDialogResult == DialogResult::Empty)
	{
		e->Handled = true;
		this->EndDialog(DialogResult::Empty);
		this->Dispose();
	}
#if _DEBUG_
	CC_LOGTIME_PRINT();
#endif
}

void Form::OnShutdowning(EventArgs* e)
{
	this->Shutdowning.Invoke((Object*)this, e);
}

void Form::OnShutdown(EventArgs* e)
{
	this->Shutdown.Invoke((Object*)this, e);
}

//for load Config while Form_Load,so even _Visible not show form whil create.only show it at OnLoad
bool Form::PreCreateWindow(CREATESTRUCT& cs)
{
	//not show for first time
	cs.style &= ~WS_VISIBLE;
	if(_eInitWindowState == FormWindowState::Maximized)
	{
		cs.style |= WS_MAXIMIZE;
	}
	else if(_eInitWindowState == FormWindowState::Minimized)
	{
		cs.style |= WS_MINIMIZE;
	}
#if !defined(_CCTHINFORM)
	if(_pMainMenu != NULL)
	{
		_pMainMenu->CreateControl();
		cs.hMenu = _pMainMenu->m_hMenu;
	}
#endif //#if !defined(_CCTHINFORM)

	return FormFrame::PreCreateWindow(cs);
}

void Form::OnCreateControl()
{
	//set self before create child
	if(HasStyle(StyleType::Inner, InnerStyle::CloseButtonDisable))
	{
		SetCloseButtonEnable(false);
	}
	if(HasStyle(StyleType::Inner, InnerStyle::MinButtonDisable))
	{
		SetMinButtonEnable(false);
	}
	if(HasStyle(StyleType::Inner, InnerStyle::MaxButtonDisable))
	{
		SetMaxButtonEnable(false);
	}

#if !defined(_CCTHINFORM)
	if(_pStatusBar != NULL)
	{
		_pStatusBar->SetParent(this, true);
		_pStatusBar->CreateControl();
	}
#endif //#if !defined(_CCTHINFORM)

	CC_BIT_OFF(_Flags, CC_FLAG_FORM_ISCALLEDCLOSED);
	FormFrame::OnCreateControl();

	::SetFocus(NULL);
	Control* pWndNext = GetNextControl(NULL);
	if(pWndNext != NULL)
	{
		pWndNext->Focus();
	}
	//if(IsMdiChild)
	//{
	//	activateMdiChild();
	//}
}

//for always load Config at Form_Load,so need SetVisible after invoke of Load
void Form::OnLoad(EventArgs* e)
{
#if _DEBUG_
	CC_TRACE((_T("[Form::OnLoad]Visible:%d, Center:%d"), (HasStyle(StyleType::Inner2, Inner2Style::Visible)), ((_StyleArray[(int)StyleType::Style] & DS_CENTER) == DS_CENTER)));
#endif
	if((_StyleArray[(int)StyleType::Style] & DS_CENTER) == DS_CENTER)
	{
		SetCenter();
	}
	if(_btnOK != NULL)
	{
		_btnOK->SetStyle(StyleType::Style, 0, BS_DEFPUSHBUTTON);
	}
#if _DEBUG_
	CC_LOGTIME_START();
#endif
	Load.Invoke((Object*)this, e);
#if _DEBUG_
	CC_LOGTIME_PRINT();
#endif
	//now show it with windowstate
	if(HasStyle(StyleType::Inner2, Inner2Style::Visible))
	{
		SetVisible(true);
	}
#if !defined(_CCTHINFORM)
	//for toolbars perhaps move at load Config,so first hide it and now show it
	int nCnt;
	if(_lstToolBars != NULL && (nCnt = _lstToolBars->GetSize()) > 0)
	{
		for(int n = 0; n < nCnt; n++)
		{
			ToolBar* ctl = (ToolBar*)_lstToolBars->GetKey(n);
			//if hide Flater,so hide ctl at load Config
			if(ctl->GetFloating() && ctl->GetVisible())
			{
				ctl->SetFloating(true, true);
			}
		}
	}
#endif //#if !defined(_CCTHINFORM)
	if(GetAlwaysOnTop())
	{
		SetAlwaysOnTop(true);
	}
}

void Form::OnDeactivate(EventArgs* e)
{
	if(_hTipsHwnd != NULL)
	{
		//let ontimer to clear tips
		_hTipsHwnd = NULL;
	}
	CC_BIT_OFF(_Flags, CC_FLAG_FORM_ISACTIVATED);
	//_Flags &= ~CC_FLAG_FORM_ISACTIVATED;
	//_bIsActivated = false;
	_APP_THREAD_STATE* pState = App::GetThreadStateData();
	if(pState->m_pActiveWnd == this->m_hWnd)
	{
		pState->m_pActiveWnd = NULL;
	}
	Deactivate.Invoke((Object*)this, e);
}

//The WM_ENABLE message is sent when an application changes the enabled state of a window.
//It is sent to the window whose enabled state is changing. This message is sent before the EnableWindow function returns,
//but after the enabled state (WS_DISABLED style bit) of the window has changed. 
//wParam:Specifies whether the window has been enabled or disabled.
//	This parameter is TRUE if the window has been enabled or FALSE if the window has been disabled.
//lParam:This parameter is not used.
#if !defined(_CCTHINFORM)
void Form::OnEnabledChanged(EventArgs* e)
{
	int nCnt;
	if(_lstToolBars != NULL && (nCnt = _lstToolBars->GetSize()) > 0)
	{
		for(int n = 0; n < nCnt; n++)
		{
			ToolBar* ctl = (ToolBar*)_lstToolBars->GetKey(n);
			//if hide Flater,so hide ctl at load Config
			if(ctl->GetFloating() && ctl->GetVisible())
			{
				ctl->GetFloatContainer()->SetEnabled((BOOL)e->WParam == TRUE);
			}
		}
	}
	FormFrame::OnEnabledChanged(e);
}
#endif //#if !defined(_CCTHINFORM)

void Form::OnHandleDestroyed(EventArgs* e)
{
	FormFrame::OnHandleDestroyed(e);

	_APP_THREAD_STATE* pState = App::GetThreadStateData();
	if(pState->m_pMainWnd == m_hWnd)
	{
		//the mainform is closed,so exit app
		pState->m_pMainWnd = NULL;
		App::Exit();
	}
}

void Form::DoUpdate()
{
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreatedEnd) != 0)
	{
		if(IsUpdate(UpdateFlag::UpdateIcon))
		{
			//refresh icon(only top-left corner of wnd)
			::SendMessage(m_hWnd, WM_SETICON, true /*bBigIcon*/, (LPARAM)GetIcon());
		}
		if(IsUpdate(UpdateFlag::UpdateChildUpdated))
		{
			EventArgs e(m_hWnd, 0, 0, 0);
			OnResize(&e);
		}
		//no need InvalidateRect for this
		_nUpdateFlag &= ~(UpdateFlag::UpdateText|UpdateFlag::UpdateIcon|UpdateFlag::UpdateBackground|UpdateFlag::UpdateTextColor|UpdateFlag::UpdateVisible|UpdateFlag::UpdateEnabled);
		//if((_nUpdateFlag & UpdateFlag::UpdateText|UpdateFlag::UpdateIcon|UpdateFlag::UpdateBackground|UpdateFlag::UpdateTextColor|UpdateFlag::UpdateVisible|UpdateFlag::UpdateEnabled) != 0)
		//{
		//	_nUpdateFlag = 0;
		//	return;
		//}
	}
#if !defined(_CCTHINFORM)
	UpdateMainMenu();
#endif //#if !defined(_CCTHINFORM)
	//clear flages
	FormFrame::DoUpdate();
}

void Form::OnResize(EventArgs* e)
{
	if(m_hWnd == NULL)
	{
		return;
	}
//wParam:
//SIZE_MAXHIDE
//Message is sent to all pop-up windows when some other window is maximized.
//SIZE_MAXIMIZED
//The window has been maximized.
//SIZE_MAXSHOW
//Message is sent to all pop-up windows when some other window has been restored to its former size.
//SIZE_MINIMIZED
//The window has been minimized.
//SIZE_RESTORED
//The window has been resized, but neither the SIZE_MINIMIZED nor SIZE_MAXIMIZED value applies.
//lParam:
//The low-order word of lParam specifies the new width of the client area. 
//The high-order word of lParam specifies the new height of the client area. 
	//if(e != NULL && (e->WParam == SIZE_MINIMIZED || e->WParam == SIZE_MAXIMIZED))
#ifdef _DEBUG_
		RECT mainRect;
		::GetClientRect(m_hWnd, &mainRect);
		RECT NewRect = this->GetBounds();

		int nW = NewRect.right - NewRect.left;
		int nH = NewRect.bottom - NewRect.top;
		int nClientW = mainRect.right - mainRect.left;
		int nClientH = mainRect.bottom - mainRect.top;
		int _nOldWidth = _OrgRect.right - _OrgRect.left;
		int _nOldHeight = _OrgRect.bottom - _OrgRect.top;
		if(CC_APP_GetConfig()->GetIsDebug())
		{
			CC_TRACE((_T("Form::OnResize[%s,%s]OldL:%d, OldT:%d, OldW:%d, OldH:%d, NewL:%d, NewT:%d, NewW:%d, NewH:%d, ClientW:%d, ClientH:%d"),
				this->_ClassName, this->_Name, _OrgRect.left, _OrgRect.top, _nOldWidth, _nOldHeight, NewRect.left, NewRect.top, nW, nH, nClientW, nClientH));
		}
#endif
	//do this invoid MDIClisnt resize after EndDeferWindowPos
	e->Result = DefWndProc(e);
	e->Handled = true;
	Resize.Invoke((Object*)this, e);
	if(e != NULL && e->WParam == SIZE_MINIMIZED)
	{
		return;
	}
	//SetOuterStyle(OuterStyle::Sizing, true);
	_StyleArray[(int)StyleType::Outer] |= OuterStyle::Sizing;
	HDWP hDWP = ::BeginDeferWindowPos(9); // reasonable guess
	RECT frameRect;
	::GetClientRect(m_hWnd, &frameRect);
	//RECT NewRect = this->GetBounds();
#if !defined(_CCTHINFORM)
	if(_pStatusBar != NULL && _pStatusBar->GetVisible())
	{
		RECT rect2;
		rect2 = _pStatusBar->GetBounds();
		int nH = rect2.bottom - rect2.top;
		//must before SetBoundsNotOriginal
		frameRect.bottom -= nH;
		//_pStatusBar->SetBoundsNotOriginal(frameRect.left, frameRect.bottom, frameRect.right, nH);
		hDWP = ::DeferWindowPos(hDWP, _pStatusBar->m_hWnd, NULL, frameRect.left, frameRect.bottom, frameRect.right, nH, SWP_NOACTIVATE|SWP_NOZORDER);
	}
	FormFrame::OnResizeFrame(hDWP, frameRect);
#else
	hDWP = ::DeferWindowPos(hDWP, _pClient->m_hWnd, NULL, frameRect.left, frameRect.top, frameRect.right - frameRect.left, frameRect.bottom - frameRect.top, SWP_NOACTIVATE|SWP_NOZORDER);
#endif //#if !defined(_CCTHINFORM)

	::EndDeferWindowPos(hDWP);
	if(GetIsMdiContainer() && _pClient->m_hWnd != NULL)
	{
		::InvalidateRect(_pClient->m_hWnd, NULL, TRUE);
	}
	//SetOuterStyle(OuterStyle::Sizing, false);
	_StyleArray[(int)StyleType::Outer] &= ~OuterStyle::Sizing;
}

void Form::OnActivateApp(EventArgs *e)
{
	ActivateApp.Invoke((Object*)this, e);
}

//e->LParam is focused control
void Form::OnFocusChanged(EventArgs *e)
{
	FocusChanged.Invoke((Object*)this, e);
}

void Form::OnGetMinMaxInfo(EventArgs *e)
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
	//GetMinMaxInfo.Invoke((Object*)this, e);
}

void Form::SetMaxSize(int maxWidth, int maxHeight)
{
	_maxSize.cx = maxWidth;
	_maxSize.cy = maxHeight;
}

void Form::OnToolBarActivated(EventArgs* e)
{
#if !defined(_CCTHINFORM)
//cc::Log::debug(_T("Form::OnToolBarActivated(), ClassName:%s, Name:%s, Text:%s, _bIsActivated:%d, e->WParam:%d"), _ClassName, _Name, _Text, Focused(), e->WParam);
	int nBarSize = (_lstToolBars == NULL ? 0 : _lstToolBars->GetSize());
	if(Focused())
	{
		if(LOWORD(e->WParam) == WA_INACTIVE)
		{
			bool isChild = false;
			for(int n = nBarSize - 1; n >= 0; n--)
			{
				ToolBar* ctl = (ToolBar*)_lstToolBars->GetKey(n);
				if(ctl->GetFloating() && ctl->GetFloatContainer()->m_hWnd == (HWND)e->LParam)
				{
					isChild = true;
					break;
				}
			}
//cc::Log::debug(_T("Form::WA_INACTIVE(), e->WParam:%d, WA_ACTIVE:%d, IsChild:%d"), e->WParam, WA_ACTIVE, isChild);
			if(!isChild)
			{
				OnDeactivate(e);
				DefWndProc(WM_NCACTIVATE, FALSE, 0);
				for(int n = nBarSize - 1; n >= 0; n--)
				{
					ToolBar* ctl = (ToolBar*)_lstToolBars->GetKey(n);
					if(ctl->GetFloating())
					{
						::SendMessage(ctl->GetFloatContainer()->m_hWnd, WM_NCACTIVATE, FALSE, 0);
					}
				}
			}
			else
			{
				::SendMessage(e->HWnd, WM_NCACTIVATE, TRUE, 0);
			}
#if defined(_DEBUG_) || defined(_DESIGN)
			if(_designBar != NULL && CC_APP_GetDesignBarActive())
			{
				//for design while debug
				::SendMessage(_designBar->m_hWnd, WM_NCACTIVATE, FALSE, 0);
			}
#endif
		}
	}
	else
	{
//cc::Log::debug(_T("Form::WA_ACTIVE(), e->WParam:%d, WA_ACTIVE:%d, WA_INACTIVE:%d"), e->WParam, WA_ACTIVE, WA_INACTIVE);
		//if((LOWORD(e->WParam) == WA_ACTIVE || LOWORD(e->WParam) == WA_CLICKACTIVE) && _lstToolBars != NULL)
		if((LOWORD(e->WParam) == WA_ACTIVE || LOWORD(e->WParam) == WA_CLICKACTIVE))
		{
			OnActivated(e);
			DefWndProc(WM_NCACTIVATE, TRUE, 0);
			for(int n = nBarSize - 1; n >= 0; n--)
			{
				ToolBar* ctl = (ToolBar*)_lstToolBars->GetKey(n);
				if(ctl->GetFloating())
				{
					CC_ASSERT(ctl->GetFloatContainer() != NULL);
					::SendMessage(ctl->GetFloatContainer()->m_hWnd, WM_NCACTIVATE, TRUE, 0);
				}
			}
#if defined(_DEBUG_) || defined(_DESIGN)
			if(_designBar != NULL && CC_APP_GetDesignBarActive())
			{
				//for design while debug
				::SendMessage(_designBar->m_hWnd, WM_NCACTIVATE, TRUE, 0);
			}
#endif
		}
	}
#endif //#if !defined(_CCTHINFORM)
}
// On...
///////////////////////////////////////////////////////////////////////////////

DialogResult::Type Form::ShowDialog()
{
	//save mainform
	//_APP_THREAD_STATE* pState = App::GetThreadStateData();
	//HWND m_pActiveWnd = pState->m_pActiveWnd;
	//HWND m_pMainWnd = pState->m_pMainWnd;
	HWND m_hWndParent = NULL;
	if(_pParent != NULL)
	{
		m_hWndParent = _pParent->m_hWnd;
	}
	else
	{
		Control* actForm = App::GetActiveForm();
		if(actForm != NULL)
		{
			m_hWndParent = actForm->m_hWnd;
		}
	}

	// disable parent (before creating dialog)
	//HWND m_hWndParent = ::GetParent(hWnd);
	//bool bEnableParent = false;
	if(m_hWndParent && m_hWndParent != ::GetDesktopWindow() && ::IsWindowEnabled(m_hWndParent))
	{
		::EnableWindow(m_hWndParent, FALSE);
		CC_BIT_ON(_Flags, CC_FLAG_FORM_ENABLEPARENT);
		//bEnableParent = true;
	}
	else
	{
		CC_BIT_OFF(_Flags, CC_FLAG_FORM_ENABLEPARENT);
	}

#ifndef _DEBUG_
	try
#endif
	{
		this->Show();
		//let meg loop
		_bMessageLoopStopRequest = false;
		cc::win::App::StartMessageLoop(&_bMessageLoopStopRequest);
		//2007/11/12 comment out,all this is done at EndDialog
		//hide the window before enabling the parent, etc.
		//if(m_hWndParent != NULL)
		//{
		//	::SetWindowPos(m_hWnd, NULL, 0, 0, 0, 0, SWP_HIDEWINDOW|SWP_NOSIZE|SWP_NOMOVE|SWP_NOACTIVATE|SWP_NOZORDER);
		//}
	}
#ifndef _DEBUG_
	catch(Exception exp)
	{
		CC_LOGSTEP();
		cc::Log::fatal(_T("[Form::ShowDialog]Exception Error:%s"), exp.GetMessage());
		cc::Log::printStackTrace();
		CC_ASSERT(0);
		throw exp;
	}
	catch(...)
	{
		CC_LOGSTEP();
		cc::Log::fatal(_T("[Form::ShowDialog]ERROR! while this->Show()."));
		cc::Log::printStackTrace();
		CC_ASSERT(0);
	}
#endif

	//2007/11/12 comment out,all this is done at EndDialog
	//if(bEnableParent)
	//{
	//	::EnableWindow(m_hWndParent, true);
	//}

	//if(m_hWndParent != NULL && ::GetActiveWindow() == m_hWnd)
	//if(m_hWndParent != NULL)
	//{
	//	::SetActiveWindow(m_hWndParent);
	//}

	//this form is hidden,need dispose&delete it next time

//cc::Log::debug(_T("Form::ShowDialog, now:%d"), _eDialogResult);
	return _eDialogResult;
}

void Form::EndDialog(DialogResult::Type dialogResult)
{
	CC_TRACE((_T("Form::EndDialog, set:%d, now:%d"), (int)dialogResult, (int)_eDialogResult));
	CC_ASSERT(::IsWindow(m_hWnd));

	_eDialogResult = dialogResult;
	//::SetParent(m_hWnd, NULL);

	//2007/11/12 added for show parent first
	HWND m_hWndParent = NULL;
	if(_pParent != NULL)
	{
		m_hWndParent = _pParent->m_hWnd;
		if(!CC_BIT_HAS(_Flags, CC_FLAG_FORM_ENABLEPARENT) && !::IsWindowEnabled(_pParent->m_hWnd))
		{
			m_hWndParent = NULL;
		}
	}
	if(m_hWndParent == NULL)
	{
		//m_hWndParent = App::GetActiveForm();
		_APP_THREAD_STATE* pState = App::GetThreadStateData();
		if(pState->m_pActiveWnd != NULL && ::IsWindow(pState->m_pActiveWnd) && pState->m_pActiveWnd != m_hWnd)
		{
			m_hWndParent = pState->m_pActiveWnd;
		}
		else if(pState->m_pMainWnd != NULL && ::IsWindow(pState->m_pMainWnd) && pState->m_pMainWnd != m_hWnd)
		{
			m_hWndParent = pState->m_pMainWnd;
		}

	}
	if(m_hWndParent != NULL)
	{
		if(CC_BIT_HAS(_Flags, CC_FLAG_FORM_ENABLEPARENT))
		{
			::EnableWindow(m_hWndParent, TRUE);
		}
		::SetActiveWindow(m_hWndParent);
	}

	//first hide it
	::ShowWindow(m_hWnd, SW_HIDE);

	//should give a chance for OnClosed
	if(!CC_BIT_HAS(_Flags, CC_FLAG_FORM_ISCALLEDCLOSED))
	{
		EventArgs e(m_hWnd, CC_WM_MAINMSG, CC_WM_MAINMSG_WPARAM_DISPOSE, NULL);
		OnClosed(&e);
	}

	//end meg loop for showdialog
	_bMessageLoopStopRequest = true;
	::EndDialog(m_hWnd, (int)_eDialogResult);
}
///////////////////////////////////////////////////////////////////////////////
//===========================================================cc::win::Form

DWORD DialogConfig::nStyle = DS_MODALFRAME | WS_CAPTION | WS_SYSMENU | WS_VISIBLE; //WS_THICKFRAME
DWORD DialogConfig::nExStyle = WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR | WS_EX_WINDOWEDGE;
//must no CS_PARENTDC
//DWORD DialogConfig::nClassStyle = CS_DBLCLKS | CS_GLOBALCLASS;
DWORD DialogConfig::nInnerStyle = InnerStyle::TabLoopInner|InnerStyle::CreateDialog;
DWORD DialogConfig::nOuterStyle = 0;
DWORD DialogConfig::nOwnerDrawTextStyle = DT_LEFT|DT_VCENTER|DT_SINGLELINE;
COLORREF DialogConfig::corBackground = ::GetSysColor(COLOR_BTNFACE);
COLORREF DialogConfig::corTextground = ::GetSysColor(COLOR_BTNTEXT);
COLORREF DialogConfig::corHotBackColor = ::GetSysColor(COLOR_BTNFACE);
COLORREF DialogConfig::corHotTextColor = ::GetSysColor(COLOR_BTNTEXT);
DWORD DialogConfig::nOuterStyleClient = OuterStyle::TransparentTop;
DWORD DialogConfig::nInnerStyleClient = InnerStyle::BackgroundTheme|FormClientConfig::nInnerStyle;
COLORREF DialogConfig::corBackgroundClient = ::GetSysColor(COLOR_BTNFACE);
//===========================================================cc::win::Dialog
///////////////////////////////////////////////////////////////////////////////
// constructors/destructor
Dialog::Dialog()
{
	BASE_CONTROL_CONFIG_INIT(Dialog);
	//GetClient()->SetOuterStyle((OuterStyle::Type)DialogConfig::nOuterStyleClient, true);
	GetClient()->SetBackground(DialogConfig::corBackgroundClient);
	GetClient()->SetStyle(StyleType::Outer, (DWORD)0xFFFFFFFF, DialogConfig::nOuterStyleClient);
	GetClient()->SetStyle(StyleType::Inner, (DWORD)0xFFFFFFFF, DialogConfig::nInnerStyleClient);
	//GetClient()->SetBackgroundType(DialogConfig::nInnerStyleClient);
	::SetRect(&_Rect, 0, 0, 300, 300);
	::CopyRect(&_OrgRect, &_Rect);
	// mask for InitCommonControlsEx
	_StyleArray[(int)StyleType::InitControl] = CC_WNDCOMMCTLS_REG|CC_WNDCOMMCTLSNEW_REG;
	//not used by dialog
	//_ClassName = CC_WND_DIALOG;
	_ClassName = WndClassName::Dialog32770;
	SetText(_T("Dialog"));

#ifdef _DEBUG_
	SetName(_T("Dialog"));
#endif
}

Dialog::~Dialog()
{
}
// constructors/destructor
///////////////////////////////////////////////////////////////////////////////
//===========================================================cc::win::Dialog

WNDPROC AppConfig::GlobalPreProcessMessage = NULL;
//===========================================================cc::win::App
//Static variable for the pointer to the CApp object
const DWORD App::_dWIN32_WINNT = _WIN32_WINNT;
#if defined(UNICODE) || defined(_UNICODE)
const DWORD App::_dUNICODE = 1;
#else
const DWORD App::_dUNICODE = 0;
#endif
#if defined(_DEBUG_)
const DWORD App::_dDEBUG = 1;
#else
const DWORD App::_dDEBUG = 0;
#endif
bool App::_bIsConstructored = false;
//HANDLE App::_hMutexRunFlage = NULL;
const TCHAR* App::_tcAppName = _T("ccProj");
//const TCHAR* App::_tcAppVersion = _T("Ver 1.0 build 20061116");
//const TCHAR* App::_tcAppVersion = _T("Ver 1.0 build 20090311");
const TCHAR* App::_tcAppVersion = _T("1.0");

// Map(bool isSort, bool bIsDESC = false, bool isDistinct = false, int nMinCapacity = 0, double nIncrement = 1.3);
Map<HWND, Control*>* App::_mapHWND = new Map<HWND, Control*>(true, false, true);
Map<HMENU, Menu*>* App::_mapMENU = new Map<HMENU, Menu*>(true, false, true);
Map<int, Timer*>* App::_mapTimer = new Map<int, Timer*>(true, false, true);
Map<const TCHAR*, void*>* App::_mapVarious = new Map<const TCHAR*, void*>(true, false, true);
HINSTANCE App::_appHINSTANCE = NULL;
ToolTip* App::_pToolTip = NULL;
DrawActor* App::_pDrawActor = new DrawActor();
cc::Config* App::_pConfig = NULL;
#if defined(_DEBUG_) || defined(_DESIGN)
FloatContainer* App::_designBar = NULL;
#endif

DWORD App::_dwTlsIndex = TLS_OUT_OF_INDEXES;
CRITICAL_SECTION App::_CRITICAL_SECTION = {0};
int App::_nMessageLoopStarted = 0;
bool App::_bMessageLoopStopRequest = false;
int App::_nShareMemorySize = sizeof(_APP_SHARE_MEMORY);
void* App::_pShareMemory = NULL;
HANDLE App::_pShareMemoryMap = NULL;
//bool App::_bIsCreatedShareMemory = false;
DWORD App::_dwAppFlag = 0;

//need one and only once
App _GLOBAL_ONLY_ONCE_APP;

///////////////////// constructors //////////////////////////
App::App()
{
#if defined(_CC_CHECK_MEM_LEAK)
	//check memory leak
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	//CC_TRACE((_T("App::App() Start.")));
	CC_LOGTIME_START();
	if(_bIsConstructored)
	{
		//We get here if the framework is used incorrectly, as more than one instance
		// of a App derived class is started.
		CC_LOGSTEP();
		const TCHAR* msg = _T("App::App, ERROR! An instance of App (or a class derrived from App) is already running");
		cc::Log::fatal(msg);
 		throw UserException(msg);
	}
	_bIsConstructored = true;

	_appHINSTANCE = GetModuleHandle(NULL);
	_dwTlsIndex = ::TlsAlloc();
	if(::TlsGetValue(_dwTlsIndex) == NULL)
	{
		_APP_THREAD_STATE *_app_THREAD_STATE = new _APP_THREAD_STATE();
		::TlsSetValue(_dwTlsIndex, _app_THREAD_STATE);
	}

	if(_dwTlsIndex == TLS_OUT_OF_INDEXES)
	{
		throw UserException(_T("App::App, Failed to allocate TLS Index"));
	}

	//for thread safe
	::InitializeCriticalSection(&_CRITICAL_SECTION);
	//CC_TRACE((_T("App::App() End.")));
	CC_LOGTIME_PRINT();
}

///////////////////// destructor //////////////////////////
//!!!when window is down,this is not run,so need CC_APP_GetConfig()->Save(); where form's onclosed
App::~App()
{
	//CC_TRACE((_T("App::~App() Start.")));
	CC_LOGTIME_START();
	//Before exiting, close the object.
	//if(_hMutexRunFlage != NULL)
	//{
	//	::CloseHandle(_hMutexRunFlage);
	//}

	try
	{
		if(_pToolTip != NULL)
		{
			_pToolTip->Dispose();
			delete _pToolTip;
			_pToolTip = NULL;
		}
#if defined(_DEBUG_) || defined(_DESIGN)
		if(_designBar != NULL && CC_APP_GetDesignBarActive())
		{
			_designBar->Dispose();
			delete _designBar;
			_designBar = NULL;
		}
#endif
		//Ensure all windows are destroyed
		//Note: The trace window is destroyed here
		//normall time, size of _mapHWND should be 0
		if(_mapHWND != NULL)
		{
			int ind;
			_APP_THREAD_STATE* pState = App::GetThreadStateData();
			HWND hWnd = pState->m_pMainWnd;
			if(hWnd != NULL && (ind = _mapHWND->GetIndex(hWnd)) >= 0)
			{
				if(::IsWindow(hWnd))
				{
					Control* ctl = _mapHWND->GetAt(ind);
					ctl->Dispose();
					delete ctl;
				}
				if(_mapHWND->GetIndex(hWnd) >= 0)
				{
					_mapHWND->Remove(hWnd);
				}
			}
			while(_mapHWND->GetSize() > 0)
			{
				try
				{
					HWND hWnd = _mapHWND->GetKey(0);
					if(::IsWindow(hWnd))
					{
						HWND hWnd2 = ::GetParent(hWnd);
						while(hWnd2 != NULL && (ind = _mapHWND->GetIndex(hWnd2)) >= 0) {
							hWnd = hWnd2;
							hWnd2 = ::GetParent(hWnd);
						}

						Control* ctl = _mapHWND->Get(hWnd);
						ctl->Dispose();
						delete ctl;
					}
					if(_mapHWND->GetIndex(hWnd) >= 0)
					{
						_mapHWND->Remove(hWnd);
					}
				}
				catch(...)
				{
					CC_TRACE((_T("App::~App, Error while delete all windows.")));
					cc::Log::printStackTrace();
				}
			}
			delete _mapHWND;
			_mapHWND = NULL;
		}

		if(_mapMENU != NULL)
		{
			while(_mapMENU->GetSize() > 0)
			{
				try
				{
					HMENU hMenu = _mapMENU->GetKey(0);
					if(::IsMenu(hMenu))
					{
						Menu* menu = _mapMENU->GetAt(0);
						::DestroyMenu(hMenu);
						delete menu;
					}
					if(_mapMENU->GetIndex(hMenu) >= 0)
					{
						_mapMENU->Remove(hMenu);
					}
				}
				catch(...)
				{
					CC_TRACE((_T("App::~App, Error while delete all menus.")));
					cc::Log::printStackTrace();
				}
			}
			delete _mapMENU;
			_mapMENU = NULL;
		}

		if(_mapTimer != NULL)
		{
			while(_mapTimer->GetSize() > 0)
			{
				try
				{
					int nInd = _mapTimer->GetKey(0);
					Timer* timer = _mapTimer->GetAt(0);
					delete timer;
					if(_mapTimer->GetIndex(nInd) >= 0)
					{
						_mapTimer->Remove(nInd);
					}
				}
				catch(...)
				{
					CC_TRACE((_T("App::~App, Error while delete timer.")));
					cc::Log::printStackTrace();
				}
			}
			delete _mapTimer;
			_mapTimer = NULL;
		}
		if(_pDrawActor != NULL)
		{
			delete _pDrawActor;
			_pDrawActor = NULL;
		}
		if(_pConfig != NULL)
		{
			delete _pConfig;
			_pConfig = NULL;
		}
		if(_mapVarious != NULL)
		{
			delete _mapVarious;
			_mapVarious = NULL;
		}
		if(Control::hFontAppShare != NULL)
		{
			::DeleteObject(Control::hFontAppShare);
			Control::hFontAppShare = NULL;
		}
	}
	catch(Exception exp)
	{
		CC_LOGSTEP();
		cc::Log::fatal(_T("[App::~App]Exception Error:%s"), exp.GetMessage());
		cc::Log::printStackTrace();
		CC_ASSERT(0);
		throw exp;
	}
	catch(...)
	{
		CC_LOGSTEP();
		cc::Log::fatal(_T("[App::~App]ERROR! while DestroyWindow all windows."));
		cc::Log::printStackTrace();
		CC_ASSERT(0);
	}

	//Now do remaining tidy up
	if(_dwTlsIndex != TLS_OUT_OF_INDEXES)
	{
		_APP_THREAD_STATE *_app_THREAD_STATE = GetThreadStateData();
		if(_app_THREAD_STATE != NULL)
		{
			delete _app_THREAD_STATE;
		}
		::TlsSetValue(_dwTlsIndex, NULL);
		::TlsFree(_dwTlsIndex);
		_dwTlsIndex = TLS_OUT_OF_INDEXES;
	}

	if(_pShareMemory != NULL)
	{
		::UnmapViewOfFile(_pShareMemory);
		::CloseHandle(_pShareMemoryMap);
	}
	//? if locked,need release if?
	::DeleteCriticalSection(&_CRITICAL_SECTION);
	_bIsConstructored = false;
	//CC_TRACE((_T("App::~App() End.")));
	CC_LOGTIME_PRINT();
}

///////////////////// private/internal used methods //////////////////////////
void App::SetInstance(HINSTANCE appHINSTANCE)
{
	_appHINSTANCE = appHINSTANCE;
}

HINSTANCE App::GetInstance()
{
	return _appHINSTANCE;
}

Map<HWND, Control*>* App::GetMapHWND()
{
	return _mapHWND;
}

Map<HMENU, Menu*>* App::GetMapHMENU()
{
	return _mapMENU;
}

Map<int, Timer*>* App::GetMapTimer()
{
	return _mapTimer;
}

ToolTip* App::GetToolTip()
{
	if(_pToolTip == NULL)
	{
		_pToolTip = new ToolTip();
	}
	return _pToolTip;
}

void App::SetDefaultTransparent(bool isTransparent)
{
	//set for xp theme to has Transparent(while need backcolor, should remove this style)
	int bit64 = 0;
	WindowsVersion::Type winType = cc::Util::GetWindowsVersion(&bit64);
	if(isTransparent)
	{
		//children will use TransparentTop's back
		cc::win::TabPageConfig::nOuterStyle |= OuterStyle::TransparentTop;
		cc::win::FormConfig::nOuterStyle |= OuterStyle::TransparentTop;
		cc::win::FormConfig::nOuterStyleClient |= OuterStyle::TransparentTop;
		cc::win::DialogConfig::nOuterStyle |= OuterStyle::TransparentTop;
		cc::win::DialogConfig::nOuterStyleClient |= OuterStyle::TransparentTop;
		cc::win::ContainerConfig::nOuterStyle |= OuterStyle::TransparentTop;
		//controls put on rebar
		cc::win::MainMenuBarConfig::nOuterStyle |= OuterStyle::TransparentParentErase;
		cc::win::ToolBarConfig::nOuterStyle |= OuterStyle::TransparentParentErase;
		//controls put on rebar also form(formclient)
		cc::win::ToolItemConfig::nOuterStyle |= OuterStyle::TransparentParentErase;
		cc::win::LabelLinkConfig::nOuterStyle |= OuterStyle::TransparentParentErase;
		cc::win::PictureBoxConfig::nOuterStyle |= OuterStyle::TransparentParentErase;
		//if ListBox has LBS_MULTIPLESEL then no TRANSPARENT
		cc::win::ListBoxConfig::nExStyle |= WS_EX_TRANSPARENT;
		cc::win::ListBoxConfig::nOuterStyle |= OuterStyle::TransparentParentErase;
		cc::win::CheckListBoxConfig::nOuterStyle |= OuterStyle::TransparentParentErase;
		cc::win::ListViewConfig::nExStyle |= WS_EX_TRANSPARENT;
		cc::win::ListViewConfig::nOuterStyle |= OuterStyle::TransparentParentErase;
		//cc::win::TreeViewConfig::nExStyle |= WS_EX_TRANSPARENT;
		//cc::win::TreeViewConfig::nOuterStyle |= OuterStyle::TransparentParentErase;
		cc::win::ComboBoxConfig::nExStyle |= WS_EX_TRANSPARENT;
		cc::win::ComboBoxConfig::nOuterStyle |= OuterStyle::TransparentParentErase;
		cc::win::ComboBoxExConfig::nExStyle |= WS_EX_TRANSPARENT;
		cc::win::ComboBoxExConfig::nOuterStyle |= OuterStyle::TransparentParentErase;
		//controls put on dialog
		cc::win::ScrollBarConfig::nOuterStyle |= OuterStyle::TransparentParentBrush;
		cc::win::LabelConfig::nOuterStyle |= OuterStyle::TransparentParentErase;
		cc::win::DateTimePickerConfig::nOuterStyle |= OuterStyle::TransparentParentErase;
		if((DWORD)winType >= (DWORD)WindowsVersion::WindowsXP)
		{
			//cc::win::LabelConfig::nOuterStyle |= OuterStyle::TransparentParentBrush;
			cc::win::ButtonConfig::nOuterStyle |= OuterStyle::TransparentParentBrush;
			cc::win::GroupConfig::nOuterStyle |= OuterStyle::TransparentParentBrush;
			cc::win::RadioButtonConfig::nOuterStyle |= OuterStyle::TransparentParentBrush;
			cc::win::CheckBoxConfig::nOuterStyle |= OuterStyle::TransparentParentBrush;
		}
		else
		{
			//cc::win::LabelConfig::nOuterStyle |= OuterStyle::TransparentHollowBrush;
			cc::win::ButtonConfig::nOuterStyle |= OuterStyle::TransparentHollowBrush;
			cc::win::GroupConfig::nOuterStyle |= OuterStyle::TransparentHollowBrush;
			cc::win::RadioButtonConfig::nOuterStyle |= OuterStyle::TransparentHollowBrush;
			cc::win::CheckBoxConfig::nOuterStyle |= OuterStyle::TransparentHollowBrush;
		}
		cc::win::PanelConfig::nOuterStyle |= OuterStyle::TransparentParentErase;
		cc::win::GroupPanelConfig::nOuterStyle |= OuterStyle::TransparentParentErase;
		cc::win::GroupPanelConfig::nOuterStyleClient |= OuterStyle::TransparentParentErase;
		cc::win::ScrollContainerConfig::nOuterStyle |= OuterStyle::TransparentParentErase;
	}
	else
	{
		//children will use TransparentTop's back
		cc::win::TabPageConfig::nOuterStyle |= ~(OuterStyle::TransparentTop);
		cc::win::FormConfig::nOuterStyle |= ~(OuterStyle::TransparentTop);
		cc::win::FormConfig::nOuterStyleClient |= ~(OuterStyle::TransparentTop);
		cc::win::DialogConfig::nOuterStyle |= ~(OuterStyle::TransparentTop);
		cc::win::DialogConfig::nOuterStyleClient |= ~(OuterStyle::TransparentTop);
		cc::win::ContainerConfig::nOuterStyle |= ~(OuterStyle::TransparentTop);
		//controls put on rebar
		cc::win::MainMenuBarConfig::nOuterStyle |= ~(OuterStyle::TransparentParentErase);
		cc::win::ToolBarConfig::nOuterStyle |= ~(OuterStyle::TransparentParentErase);
		//controls put on rebar also form(formclient)
		cc::win::ToolItemConfig::nOuterStyle |= ~(OuterStyle::TransparentParentErase);
		cc::win::LabelLinkConfig::nOuterStyle |= ~(OuterStyle::TransparentParentErase);
		cc::win::PictureBoxConfig::nOuterStyle |= ~(OuterStyle::TransparentParentErase);
		cc::win::ListBoxConfig::nExStyle |= ~(WS_EX_TRANSPARENT);
		cc::win::ListBoxConfig::nOuterStyle |= ~(OuterStyle::TransparentParentErase);
		cc::win::CheckListBoxConfig::nOuterStyle |= ~(OuterStyle::TransparentParentErase);
		cc::win::ListViewConfig::nExStyle |= ~(WS_EX_TRANSPARENT);
		cc::win::ListViewConfig::nOuterStyle |= ~(OuterStyle::TransparentParentErase);
		//cc::win::TreeViewConfig::nExStyle |= ~(WS_EX_TRANSPARENT);
		//cc::win::TreeViewConfig::nOuterStyle |= ~(OuterStyle::TransparentParentErase);
		cc::win::ComboBoxConfig::nExStyle |= ~(WS_EX_TRANSPARENT);
		cc::win::ComboBoxConfig::nOuterStyle |= ~(OuterStyle::TransparentParentErase);
		cc::win::ComboBoxExConfig::nExStyle |= ~(WS_EX_TRANSPARENT);
		cc::win::ComboBoxExConfig::nOuterStyle |= ~(OuterStyle::TransparentParentErase);
		//controls put on dialog
		cc::win::ScrollBarConfig::nOuterStyle |= ~(OuterStyle::TransparentParentBrush);
		cc::win::LabelConfig::nOuterStyle |= ~(OuterStyle::TransparentParentErase);
		cc::win::DateTimePickerConfig::nOuterStyle |= ~(OuterStyle::TransparentParentErase);
		if((DWORD)winType >= (DWORD)WindowsVersion::WindowsXP)
		{
			//cc::win::LabelConfig::nOuterStyle |= ~(OuterStyle::TransparentParentBrush);
			cc::win::ButtonConfig::nOuterStyle |= ~(OuterStyle::TransparentParentBrush);
			cc::win::GroupConfig::nOuterStyle |= ~(OuterStyle::TransparentParentBrush);
			cc::win::RadioButtonConfig::nOuterStyle |= ~(OuterStyle::TransparentParentBrush);
			cc::win::CheckBoxConfig::nOuterStyle |= ~(OuterStyle::TransparentParentBrush);
		}
		else
		{
			//cc::win::LabelConfig::nOuterStyle |= ~(OuterStyle::TransparentHollowBrush);
			cc::win::ButtonConfig::nOuterStyle |= ~(OuterStyle::TransparentHollowBrush);
			cc::win::GroupConfig::nOuterStyle |= ~(OuterStyle::TransparentHollowBrush);
			cc::win::RadioButtonConfig::nOuterStyle |= ~(OuterStyle::TransparentHollowBrush);
			cc::win::CheckBoxConfig::nOuterStyle |= ~(OuterStyle::TransparentHollowBrush);
		}
		cc::win::PanelConfig::nOuterStyle |= ~(OuterStyle::TransparentParentErase);
		cc::win::GroupPanelConfig::nOuterStyle |= ~(OuterStyle::TransparentParentErase);
		cc::win::GroupPanelConfig::nOuterStyleClient |= ~(OuterStyle::TransparentParentErase);
		cc::win::ScrollContainerConfig::nOuterStyle |= ~(OuterStyle::TransparentParentErase);
	}
}

#if defined(_DEBUG_) || defined(_DESIGN)
FloatContainer* App::ShowDesignBar(Form* parent)
{
	if(_designBar != NULL && (_designBar->GetParent() != parent || !::IsWindow(_designBar->m_hWnd)))
	{
		_designBar->Dispose();
		delete _designBar;
		_designBar = NULL;
	}
	if(_designBar == NULL)
	{
		_designBar = parent->CreateDesignBar();
		if(_designBar == NULL)
		{
			return NULL;
		}
		//must set this before createcontrol
		_designBar->SetParent(parent, true);
		_designBar->CreateControl();
	}
	_designBar->Show();
	return _designBar;
}

bool App::GetDesignBarActive()
{
	if(_designBar != NULL && ::IsWindowVisible(_designBar->m_hWnd) == TRUE)
	{
		return true;
	}
	return false;
}

HWND App::GetDesignBarHwnd()
{
	if(_designBar != NULL && ::IsWindowVisible(_designBar->m_hWnd) == TRUE)
	{
		return _designBar->m_hWnd;
	}
	return NULL;
}
#endif

bool CC_APP_GetDesignBarActive()
{
#if defined(_DEBUG_) || defined(_DESIGN)
	return App::GetDesignBarActive();
#else
	return false;
#endif
}

HWND CC_APP_GetDesignBarHwnd()
{
#if defined(_DEBUG_) || defined(_DESIGN)
	return App::GetDesignBarHwnd();
#else
	return NULL;
#endif
}

void App::SetToolTip(ToolTip* toolTip)
{
	if(_pToolTip != NULL)
	{
		delete _pToolTip;
	}
	_pToolTip = toolTip;
}

bool App::GetToolTipActive()
{
	return GetToolTip()->GetActive();
}

void App::SetToolTipActive(bool isActive)
{
	GetToolTip()->SetActive(isActive);
}

bool App::GetAlwaysTips()
{
	return CC_BIT_HAS(_dwAppFlag, CC_FLAG_APP_ALWAYSTIPS);
}

void App::SetAlwaysTips(bool value)
{
	CC_BIT_SET(_dwAppFlag, CC_FLAG_APP_ALWAYSTIPS, value);
}

void App::SetDrawActor(DrawActor* drawActor)
{
	if(_pDrawActor != NULL && _pDrawActor != drawActor)
	{
		delete _pDrawActor;
	}
	_pDrawActor = drawActor;
}

DrawActor* App::GetDrawActor()
{
	return _pDrawActor;
}

void App::SetConfig(cc::Config* cfg)
{
	if(_pConfig != NULL && _pConfig != cfg)
	{
		delete _pConfig;
	}
	_pConfig = cfg;
}

cc::Config* App::GetConfig()
{
	if(_pConfig == NULL)
	{
		_pConfig = new cc::Config();
	}
	return _pConfig;
}

void App::SetVarious(const TCHAR* key, void* value)
{
	if(value == NULL)
	{
		int nInd = _mapVarious->GetIndex(key);
		if(nInd >= 0)
		{
			_mapVarious->RemoveAt(nInd);
		}
		return;
	}
	_mapVarious->Add(key, value);
}

void* App::GetVarious(const TCHAR* key)
{
	int nInd = _mapVarious->GetIndex(key);
	if(nInd < 0)
	{
		return NULL;
	}
	return _mapVarious->GetAt(nInd);
}

void App::SetShareMemorySize(int nShareMemorySize)
{
	CC_ASSERT(nShareMemorySize > 0);
	CC_ASSERT(_pShareMemory == NULL);

	_nShareMemorySize = nShareMemorySize;
}

int App::GetShareMemorySize()
{
	return _nShareMemorySize;
}

void* App::GetShareMemory()
{
	if(_pShareMemory == NULL)
	{
		CC_TRACE((_T("App::GetShareMemory, _nShareMemorySize:%d"), _nShareMemorySize));

		_pShareMemoryMap = ::OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, GetName());
		if(_pShareMemoryMap == NULL)
		{
			_pShareMemoryMap = ::CreateFileMapping( (HANDLE)INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, _nShareMemorySize, GetName() );
			//_bIsCreatedShareMemory = true;
			CC_BIT_SET(_dwAppFlag, CC_FLAG_APP_SHAREDMEM, true);
		}
		if(_pShareMemoryMap == NULL)
		{
			CC_LOGSTEP();
			const TCHAR* msg = _T("App::GetShareMemory(), ERROR for CreateFileMapping");
			cc::Log::fatal(msg);
	 		throw UserException(msg);
		}
		
		_pShareMemory = ::MapViewOfFile(_pShareMemoryMap, FILE_MAP_ALL_ACCESS, 0, 0, _nShareMemorySize);

		CC_ASSERT(_pShareMemory != NULL);
	}
	return _pShareMemory;
}

bool App::IsFirstInstance()
{
	//for set flage of _bIsCreatedShareMemory
	GetShareMemory();
	//return _bIsCreatedShareMemory;
	return CC_BIT_HAS(_dwAppFlag, CC_FLAG_APP_SHAREDMEM);
	//_hMutexRunFlage = ::CreateMutex(NULL, FALSE, GetName());
	//if(GetLastError() == ERROR_ALREADY_EXISTS)
	//{
	//	::CloseHandle(_hMutexRunFlage);
	//	_hMutexRunFlage = NULL;
	//	return true;
	//}
	//return false;
}

//if already run then show message, and if NULL then inner msg.
bool App::IsFirstInstance(const TCHAR* msg)
{
	if(!IsFirstInstance())
	{
		_APP_SHARE_MEMORY* appMemory = (_APP_SHARE_MEMORY*)GetShareMemory();
		HWND hWnd = appMemory->m_pMainWnd;
		if(::IsWindow(hWnd))
		{
			::ShowWindow(hWnd, SW_SHOW);
			::ShowWindow(hWnd, SW_RESTORE);
			cc::Util::FourceWindowTop(hWnd);
		}
		/*
		else
		{
			Control* ctl = GetMainForm();
			if(ctl != NULL && !ctl->GetEnabled())
			{
				ctl = GetActiveForm();
			}
			hWnd = (ctl == NULL ? NULL : ctl->m_hWnd);
		}
		*/
		//20140721 next cannot be hWnd, otherwise if has top win, then cannot click OK button!
		//hWnd = NULL;
		//::MessageBox(hWnd,
		//	msg == NULL ? CC_APP_GetLang()->Get(_T("Lang.S_AP_ALREADYRUN"), cc::Str(_T("this application is already running."))) : msg,
		//	NULL, MB_ICONERROR|MB_TOPMOST);
		Dialog *dlg = new Dialog();
		dlg->SetText(App::GetNameVersion());
		dlg->SetSize(300, 60);
		dlg->SetCenter();
		dlg->SetAlwaysOnTop(true);

		Label *label = new Label();
		label->SetLocation(5, 10);
		label->SetSize(290, 17);
		label->SetText(msg == NULL ? CC_APP_GetLang()->Get(_T("Lang.S_AP_ALREADYRUN"), cc::Str(_T("this application is already running."))) : msg);
		dlg->AddControl(label);

		dlg->ShowDialog();

		dlg->Dispose();
		delete dlg;
	}
	//return _bIsCreatedShareMemory;
	return CC_BIT_HAS(_dwAppFlag, CC_FLAG_APP_SHAREDMEM);
}

_APP_THREAD_STATE* App::GetThreadStateData()
{
	//Retrieve the _APP_THREAD_STATE pointer from TLS
	_APP_THREAD_STATE* appTHREAD_STATE = (_APP_THREAD_STATE*)::TlsGetValue(_dwTlsIndex);
	return appTHREAD_STATE;
}

//void App::SetThreadStateData(_APP_THREAD_STATE* appTHREAD_STATE)
//{
//	::TlsSetValue(_dwTlsIndex, appTHREAD_STATE);
//}

bool App::GetMessageLoop()
{
	return (_nMessageLoopStarted > 0);
}

void App::ThreadSafeEnter()
{
	::EnterCriticalSection(&_CRITICAL_SECTION);
}

void App::ThreadSafeLeave()
{
	::LeaveCriticalSection(&_CRITICAL_SECTION);
}

void App::SetNameVersion(const TCHAR* tcName, const TCHAR* tcVersion)
{
	_tcAppName = tcName;
	_tcAppVersion = tcVersion;
}

cc::Str App::GetNameVersion()
{
	return cc::Str(_tcAppName).Append(_T(" Ver ")).Append(_tcAppVersion);
}

const TCHAR* App::GetVersion()
{
	return _tcAppVersion;
}

const TCHAR* App::GetName()
{
	return _tcAppName;
}

void App::Exit()
{
	//to help close Log window, if no this,wait long time for close
	HWND logHWND = Log::GetHWND();
	if(logHWND != NULL && ::IsWindow(logHWND))
	{
		::ShowWindow(logHWND, SW_HIDE);
	}
	_bMessageLoopStopRequest = true;
	PostQuitMessage(0);
}

bool __cdecl CC_PreProcessMessage(MSG* pMsg)
{
#ifdef _DEBUG_
	//for debug to set breakpoint
	int x = 0;
	if(pMsg->message == WM_KEYDOWN)
	{
		x = 1;
	}
	if(pMsg->message == WM_RBUTTONUP)
	{
		x = 2;
	}
#endif
	//#ifdef _DEBUG_
	//UINT_PTR keyData = 0;
	//if((pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST)
	//	||(pMsg->message >= WM_MOUSEFIRST && pMsg->message <= WM_MOUSELAST && pMsg->message != WM_MOUSEMOVE))
	//{
	//}
	//if want eat some msg,do it here!
	if(AppConfig::GlobalPreProcessMessage != NULL)
	{
		if(AppConfig::GlobalPreProcessMessage(pMsg->hwnd, pMsg->message, pMsg->wParam, pMsg->lParam) == (LRESULT)TRUE)
		{
			return true;
		}
	}

	UINT_PTR keyData = pMsg->wParam;
	Control* pMainWnd = NULL;
	HWND hWnd = NULL;
	if(pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST)
	{
		//save time for only that need KeyInfo
		if((HIWORD(pMsg->lParam) & KF_ALTDOWN) != 0)
		{
			keyData |= SHORTCUT_ALT;
		}
		if((::GetKeyState(VK_CONTROL) & 0x8000) != 0)
		{
			keyData |= SHORTCUT_CTRL;
		}
		if((::GetKeyState(VK_SHIFT) & 0x8000) != 0)
		{
			keyData |= SHORTCUT_SHIFT;
		}

		pMainWnd = App::GetActiveForm();
		//first treate key event throgh PreFormProcessMessage
		if(pMainWnd != NULL)
		{
			hWnd = pMainWnd->m_hWnd;
			if(pMainWnd->PreFormProcessMessage(pMsg, keyData))
			{
				return true;
			}
		}

		pMainWnd = App::GetMainForm();
		//first treate key event throgh PreFormProcessMessage
		if(pMainWnd != NULL && pMainWnd->m_hWnd != hWnd && pMainWnd->PreFormProcessMessage(pMsg, keyData))
		{
			return true;
		}
	}
	// walk from target to main window
	//HWND hWndStop = (pMainWnd == NULL ? ::GetDesktopWindow() : pMainWnd->m_hWnd);
	//if(Control::WalkPreTranslateTree(hWndStop, pMsg, keyData))
	//{
	//	return true;
	//}
	Control* ctlTarget = NULL;
	for(hWnd = pMsg->hwnd; hWnd != NULL; hWnd = ::GetParent(hWnd))
	{
		ctlTarget = Control::FromHandle(hWnd);
		// if found or goto topleveal
		if(ctlTarget != NULL)
		{
			// target window is a C++ window
			if(ctlTarget->PreProcessMessage(pMsg, keyData))
			{
				return true; // trapped by target window (eg: accelerators)
			}
			break;
		}
	}

	//this comment is not valid after 20070323:
	//    even "ctlTarget->GetTopLevelParent() = pMainWnd", the pMainWnd'PreProcessMessage is not called  by child,
	//    but ProcessCmdKey&ProcessDialogKey is called whild (pMsg->message == WM_KEYDOWN || pMsg->message == WM_SYSKEYDOWN)
	//    and ProcessDialogChar is called while (pMsg->message == WM_CHAR) by child
	// in case of modeless dialogs, last chance route through main
	//   window's accelerator table
	//if(pMainWnd != NULL && (ctlTarget == NULL || ctlTarget->GetTopLevelParent() != pMainWnd))
	//modify 20070323,to give topform a chance to treate all message,so ToolTip can do sth. there
	//20070427,if keyData == 0,then IsInputKey
	//if not WM_KEYFIRST&WM_KEYLAST,then not need go here(not need GetActiveForm for gone at above by ::GetParent(hWnd))
	if(keyData != 0 && pMainWnd != NULL && ctlTarget != pMainWnd)
	{
		return (pMainWnd->PreProcessMessage(pMsg, keyData));
	}
	return false;   // no special processing
}

bool __stdcall CC_PumpMessage()
{
	_APP_THREAD_STATE* pState = App::GetThreadStateData();
	if(!::GetMessage(&(pState->m_msgCur), NULL, NULL, NULL))
	{
		//Note: prevents calling message loop things in 'ExitInstance'
		//will never be decremented
		return false;
	}

	//process this message
	//#define WM_KICKIDLE         0x036A  // (params unused) causes idles to kick in
	//if(pState->m_msgCur.message != 0x036A && !CC_PreProcessMessage(&(pState->m_msgCur)))
	if(!CC_PreProcessMessage(&(pState->m_msgCur)))
	{
		::TranslateMessage(&(pState->m_msgCur));
		::DispatchMessage(&(pState->m_msgCur));
	}
	return true;
}

HWND App::GetActiveFormHwnd()
{
	//invoid popwindow or others,should do this first
	_APP_THREAD_STATE* pState = App::GetThreadStateData();
	//this must be first for show status
	if(pState->m_pActiveWnd != NULL && ::IsWindow(pState->m_pActiveWnd))
	{
		return pState->m_pActiveWnd;    // probably in-place active
	}
	if(pState->m_pMainWnd != NULL && ::IsWindow(pState->m_pMainWnd))
	{
		return pState->m_pMainWnd;    // when not inplace active, just return main window
	}
	return ::GetActiveWindow();
}

Control* App::GetActiveForm()
{
	return Control::FromHandle(GetActiveFormHwnd());
}

HWND App::GetMainFormHwnd()
{
	_APP_THREAD_STATE* pState = App::GetThreadStateData();
	if(pState->m_pMainWnd != NULL && ::IsWindow(pState->m_pMainWnd))
	{
		return pState->m_pMainWnd;    // when not inplace active, just return main window
	}
	if(pState->m_pActiveWnd != NULL && ::IsWindow(pState->m_pActiveWnd))
	{
		return pState->m_pActiveWnd;    // probably in-place active
	}

	return ::GetActiveWindow();
}

Control* App::GetMainForm()
{
	return Control::FromHandle(GetMainFormHwnd());
}

void App::DoEvents(HWND hWnd)
{
	_APP_THREAD_STATE* pState = App::GetThreadStateData();
	MSG *pMsg = &(pState->m_msgCur);
	while(::PeekMessage(pMsg, hWnd, 0, 0, PM_NOREMOVE))
	{
		// pump message, but quit on WM_QUIT
		if(!CC_PumpMessage())
		{
			_nMessageLoopStarted = 0;
			::PostQuitMessage(0);
			return;
		}
	}
	//do
	//{
	//	// pump message, but quit on WM_QUIT
	//	if(!CC_PumpMessage())
	//	{
	//		_nMessageLoopStarted = 0;
	//		::PostQuitMessage(0);
	//		return;
	//	}
	//} while(::PeekMessage(pMsg, NULL, NULL, NULL, PM_NOREMOVE));
	//return;
	//MSG msg;

	//while(::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
	//{  
	//	if(::GetMessage(&msg, NULL, 0, 0))
	//	{
	//		::TranslateMessage(&msg);
	//		::DispatchMessage(&msg);
	//	}
	//	else
	//	{
	//		break;
	//	}
	//}
}

// main running routine until thread exits
void App::StartMessageLoop(int *messageLoopStopRequest)
{
	_APP_THREAD_STATE* pState = App::GetThreadStateData();
	MSG *pMsg = &(pState->m_msgCur);
	_nMessageLoopStarted++;
	_bMessageLoopStopRequest = false;

	// acquire and dispatch messages until the modal state is done
	for (;;)
	{
		// phase1: check to see if we can do idle work
		//...
		// phase2: pump messages while available
		do
		{
			// pump message, but quit on WM_QUIT
			if(!CC_PumpMessage())
			{
				_nMessageLoopStarted = 0;
				::PostQuitMessage(0);
				return;
			}
			if(*messageLoopStopRequest || _bMessageLoopStopRequest)
			{
				_nMessageLoopStarted--;
				_bMessageLoopStopRequest = false;
				return;
			}

		} while(::PeekMessage(pMsg, NULL, NULL, NULL, PM_NOREMOVE));
	}

	_nMessageLoopStarted--;
}

void App::Run(Form* form)
{
	//only run this once,but u can use ShowDialog
	if(GetMessageLoop())
	{
		CC_LOGSTEP();
		TCHAR* msg = _T("[App::Run(Form* form)]Window is running.");
		cc::Log::fatal(msg);
		throw UserException(msg);
	}

	form->CreateControl();

	_APP_THREAD_STATE* pState = App::GetThreadStateData();
	pState->m_pMainWnd = form->m_hWnd;
	_APP_SHARE_MEMORY* appMemory = (_APP_SHARE_MEMORY*)GetShareMemory();
	appMemory->m_pMainWnd = form->m_hWnd;

	if(form->GetVisible())
	{
		form->Show();
	}
	form->Update();
	//let msg loop
	int *flag = form->GetMessageLoopFlag();
	*flag = false;
	StartMessageLoop(flag);
	//form->_bMessageLoopStopRequest = false;
	//StartMessageLoop(&form->_bMessageLoopStopRequest);
}

void App::ExitMessageLoop()
{
	_bMessageLoopStopRequest = true;
	PostMessage(NULL, 0, 0, 0 ); 
}

//===========================================================cc::win::Base
//this is defined at cc::Util or cc::win::Base
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//===========================================================cc::win::Base(need App)
void CC_APP_DoEvents()
{
	App::DoEvents();
}

_APP_THREAD_STATE* CC_APP_GetThreadStateData()
{
	return App::GetThreadStateData();
}

HWND CC_APP_GetMainFormHwnd()
{
	return App::GetMainFormHwnd();
}

Control* CC_APP_GetActiveForm()
{
	return App::GetActiveForm();
}

HWND CC_APP_GetActiveFormHwnd()
{
	return App::GetActiveFormHwnd();
}

Control* CC_APP_GetMainForm()
{
	return App::GetMainForm();
}

Map<HMENU, Menu*>* CC_APP_GetMapHMENU()
{
	return App::GetMapHMENU();
}

Map<HWND, Control*>* CC_APP_GetMapHWND()
{
	return App::GetMapHWND();
}

Map<int, Timer*>* CC_APP_GetMapTimer()
{
	return App::GetMapTimer();
}

HINSTANCE CC_APP_GetInstance()
{
	return App::GetInstance();
}

DrawActor* CC_APP_GetDrawActor()
{
	return App::GetDrawActor();
}

cc::Config* CC_APP_GetConfig()
{
	return App::GetConfig();
}

cc::KeyArr* CC_APP_GetIni()
{
	return App::GetConfig()->GetIni();
}

cc::KeyArr* CC_APP_GetLang()
{
	return App::GetConfig()->GetLang();
}

void CC_APP_SetVarious(const TCHAR* key, void* value)
{
	return App::SetVarious(key, value);
}

void* CC_APP_GetVarious(const TCHAR* key)
{
	return App::GetVarious(key);
}
//===========================================================cc::win::Base(need App)

int DrawActorConfig::nMenuMarginLeft = 21;
int DrawActorConfig::nMenuMarginRight = 12;
int DrawActorConfig::nMenuMarginShortcut = 33;
int DrawActorConfig::nMenuIconWH = 16;
int DrawActorConfig::nMenuMinWidth = 33;
int DrawActorConfig::nMenuMaxWidth = 410;
int DrawActorConfig::nMenuBreakHeight = 2;
double DrawActorConfig::nDisabledIconRScale = 0.299; //0.11,0.299
double DrawActorConfig::nDisabledIconGScale = 0.587; //0.59,0.587
double DrawActorConfig::nDisabledIconBScale = 0.114; //0.30,0.114
//also need set at OnSysColorChanged
COLORREF DrawActorConfig::corMenuBackground = ::GetSysColor(COLOR_MENU);
COLORREF DrawActorConfig::corMenuTextground = ::GetSysColor(COLOR_MENUTEXT);
COLORREF DrawActorConfig::corMenuHotBackColor = ::GetSysColor(COLOR_HIGHLIGHT);
COLORREF DrawActorConfig::corMenuHotTextColor = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
COLORREF DrawActorConfig::corMenuDisabledTextground = ::GetSysColor(COLOR_GRAYTEXT);
COLORREF DrawActorConfig::corFrameColor = RGB(208, 208, 191);
//===========================================================cc::win::DrawActor
///////////////////////////////////////////////////////////////////////////////
// constructors/destructor
DrawActor::DrawActor()
{
	hFontMenu = NULL;
	for(int i = 0; i < DRAWACTOR_THEMEBRUSHBACK_COUNT; i++)
	{
		hbrThemeBack[i] = NULL;
	}
	//hbrGradientFill[0] = NULL;
}

DrawActor::~DrawActor()
{
	if(hFontMenu != NULL)
	{
		::DeleteObject(hFontMenu);
		hFontMenu = NULL;
	}
	for(int i = 0; i < DRAWACTOR_THEMEBRUSHBACK_COUNT; i++)
	{
		::DeleteObject(hbrThemeBack[i]);
		hbrThemeBack[i] = NULL;
	}
	//if(hbrGradientFill[0] != NULL)
	//{
	//	for(i = 0; i < DRAWACTOR_GRADIBRUSHBACK_COUNT; i++)
	//	{
	//		::DeleteObject(hbrGradientFill[i]);
	//	}
	//	hbrGradientFill[i] = NULL;
	//}
}
// constructors/destructor

void DrawActor::OnThemeChanged()
{
	for(int i = 0; i < DRAWACTOR_THEMEBRUSHBACK_COUNT; i++)
	{
		::DeleteObject(hbrThemeBack[i]);
		hbrThemeBack[i] = NULL;
	}
}

HFONT DrawActor::GetMenuFont(bool isFourceRefresh)
{
	if(isFourceRefresh || hFontMenu == NULL)
	{
		//NONCLIENTMETRICS ncm;
		//ncm.cbSize = sizeof(NONCLIENTMETRICS);
		//SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(ncm), &ncm, 0);
		if(hFontMenu != NULL)
		{
			DeleteObject(hFontMenu);
		}
		//hFontMenu = ::CreateFontIndirect(&ncm.lfMenuFont);
		hFontMenu = ::CreateFontIndirect(&FontConfig::lfMenu);
	}
	return hFontMenu;
}

//   9 V
//  48 Min
//  49 Max
//  50 Restore
//  98 Checkmark
// 105 Bullet
// 114 Close
void DrawActor::DrawSpecialChar(HDC hdc, RECT rc, TCHAR tc, bool bBold, COLORREF color, long fontHeight, UINT nFlag)
{
	HFONT m_hFont;
	LOGFONT logfont;
	if(fontHeight != -1)
	{
		logfont.lfHeight = -(fontHeight);
	}
	else
	{
		logfont.lfHeight = -(rc.bottom - rc.top);
	}
	logfont.lfWidth = 0;
	logfont.lfEscapement = 0;
	logfont.lfOrientation = 0;
	logfont.lfWeight = (bBold) ? FW_BOLD:FW_NORMAL;
	logfont.lfItalic = FALSE;
	logfont.lfUnderline = FALSE;
	logfont.lfStrikeOut = FALSE;
	logfont.lfCharSet = DEFAULT_CHARSET;
	logfont.lfOutPrecision = OUT_DEFAULT_PRECIS;
	logfont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	logfont.lfQuality = DEFAULT_QUALITY;
	logfont.lfPitchAndFamily = DEFAULT_PITCH|FF_MODERN;
	cc::Buf::Cpy(logfont.lfFaceName, 32, _T("Marlett"));

	m_hFont = ::CreateFontIndirect(&logfont);
	HFONT hOldFont = (HFONT)::SelectObject(hdc, m_hFont);
	if(color != -1)
	{
		::SetTextColor(hdc, color);
	}
	::DrawText(hdc, &tc, 1, &rc, nFlag);
	SelectObject(hdc, hOldFont);
	DeleteObject(m_hFont);
}

//Draw the Gradient
void DrawActor::DrawGradient(HDC hDC, RECT rect, COLORREF corFrom, COLORREF corTo, bool isVertical)
{
	DoDrawGradient(hDC, rect, corFrom, corTo, isVertical);
}
void DrawActor::DoDrawGradient(HDC hDC, RECT rect, COLORREF corFrom, COLORREF corTo, bool isVertical)
{
	HBITMAP hBitmap = GetGradientBmp(corFrom, corTo, rect.right - rect.left, rect.bottom - rect.top, isVertical, hDC, hDC, rect.left, rect.top);
	DeleteObject(hBitmap);
}

HICON DrawActor::CreateGloomIcon(HICON hIcon, int nScale)
{
	return DoCreateGloomIcon(hIcon, nScale);
}

HICON DrawActor::DoCreateGloomIcon(HICON hIcon, int nScale)
{
	ICONINFO iconInfo = {0};
	// get the icon information
	if(hIcon == NULL || !::GetIconInfo(hIcon, &iconInfo))
	{
		return NULL;
	}

	// create a DC used to convert from an icon to a bitmap
	HDC hDC = ::CreateCompatibleDC(0);
	// select the icon into the DC as it is now
	HBITMAP bmOld = (HBITMAP)::SelectObject(hDC, iconInfo.hbmColor);
	// do the width
	int nIconX = iconInfo.xHotspot * 2;
	int nIconY = iconInfo.yHotspot * 2;

	COLORREF crPixel;
	for (int nX = 0; nX < nIconX; nX++)
	{
		for (int nY = 0; nY < nIconY; nY++)
		{
			crPixel = ::GetPixel(hDC, nX, nY);
			long red   = MulDiv(GetRValue(crPixel), (255 - nScale), 255);
			long green = MulDiv(GetGValue(crPixel), (255 - nScale), 255);
			long blue  = MulDiv(GetBValue(crPixel), (255 - nScale), 255);
			::SetPixel(hDC, nX, nY, RGB(red, green, blue));
		}
	}

	// reselect the previous bitmap
	HICON newIcon = ::CreateIconIndirect(&iconInfo);
	::SelectObject(hDC, bmOld);
	::DeleteObject(iconInfo.hbmColor);
	::DeleteObject(iconInfo.hbmMask);
	::ReleaseDC(NULL, hDC);
	return newIcon;
}

HICON DrawActor::CreateDisabledIcon(HICON hIcon)
{
	return DoCreateDisabledIcon(hIcon);
}

HICON DrawActor::DoCreateDisabledIcon(HICON hIcon)
{
	ICONINFO iconInfo = {0};
	// get the icon information
	if(hIcon == NULL || !::GetIconInfo(hIcon, &iconInfo))
	{
		return NULL;
	}

	// create a DC used to convert from an icon to a bitmap
	HDC hDC = ::CreateCompatibleDC(0);
	// select the icon into the DC as it is now
	HBITMAP bmOld = (HBITMAP)::SelectObject(hDC, iconInfo.hbmColor);
	// do the all dot
	int nIconX = iconInfo.xHotspot * 2;
	int nIconY = iconInfo.yHotspot * 2;
	COLORREF crPixel;
	BYTE byNewPixel;
	for (int nX = 0; nX < nIconX; nX++)
	{
		for (int nY = 0; nY < nIconY; nY++)
		{
			crPixel = ::GetPixel(hDC, nX, nY);
			byNewPixel = (BYTE)((GetRValue(crPixel) * DrawActorConfig::nDisabledIconRScale) + 
									(GetGValue(crPixel) * DrawActorConfig::nDisabledIconGScale) + 
									(GetBValue(crPixel) * DrawActorConfig::nDisabledIconBScale));
			::SetPixel(hDC, nX, nY, RGB(byNewPixel, byNewPixel, byNewPixel));
		}
	}

	// reselect the previous bitmap
	HICON newIcon = ::CreateIconIndirect(&iconInfo);
	::SelectObject(hDC, bmOld);
	::DeleteObject(iconInfo.hbmColor);
	::DeleteObject(iconInfo.hbmMask);
	::ReleaseDC(NULL, hDC);
	return newIcon;
}

HICON DrawActor::ResizeIcon(HICON hIconOld, int cx, int cy)
{
	return DoResizeIcon(hIconOld, cx, cy);
}

HICON DrawActor::DoResizeIcon(HICON hIconOld, int cx, int cy)
{
	HDC hMemDC1;
	HDC hMemDC2;
	HICON hIconNew;
	BITMAP bmp;
	HBITMAP hOldBmp1;
	HBITMAP hOldBmp2;
	ICONINFO infoOld;
	ICONINFO infoNew;

	// get the icon information
	if(hIconOld == NULL || !::GetIconInfo(hIconOld, &infoOld))
	{
		return NULL;
	}
	int nIconX = infoOld.xHotspot * 2;
	int nIconY = infoOld.yHotspot * 2;

	HDC hScreenDC = ::GetDC(NULL);
	hMemDC1 = CreateCompatibleDC(hScreenDC);
	hMemDC2 = CreateCompatibleDC(hScreenDC);

	GetObject(infoOld.hbmColor, sizeof(BITMAP), &bmp);
	infoNew.hbmColor = CreateBitmap(cx, cy, bmp.bmPlanes, bmp.bmBitsPixel, NULL);

	hOldBmp1 = (HBITMAP) SelectObject(hMemDC1, infoOld.hbmColor);
	hOldBmp2 = (HBITMAP) SelectObject(hMemDC2, infoNew.hbmColor);

	SetStretchBltMode(hMemDC2, COLORONCOLOR);
	StretchBlt(hMemDC2, 0, 0, cx, cy, hMemDC1, 0, 0, nIconX, nIconY, SRCCOPY);
	GetObject(infoOld.hbmMask, sizeof(BITMAP), &bmp);
	infoNew.hbmMask = CreateBitmap(cx, cy, bmp.bmPlanes, bmp.bmBitsPixel, NULL);
	SelectObject(hMemDC1, infoOld.hbmMask);
	SelectObject(hMemDC2, infoNew.hbmMask);

	StretchBlt(hMemDC2, 0, 0, cx, cy, hMemDC1, 0, 0, nIconX, nIconY, SRCCOPY);
	SelectObject(hMemDC1, hOldBmp1);
	SelectObject(hMemDC2, hOldBmp2);

	infoNew.fIcon = TRUE;
	hIconNew = CreateIconIndirect(&infoNew);

	DeleteObject(infoOld.hbmColor);
	DeleteObject(infoNew.hbmColor);
	DeleteObject(infoOld.hbmMask);
	DeleteObject(infoNew.hbmMask);
	DeleteDC(hMemDC1);
	DeleteDC(hMemDC2);
	ReleaseDC(NULL, hScreenDC);

	return hIconNew;
}

HBITMAP DrawActor::ResizeBmp(HBITMAP hBmpSource, int nDestW, int nDestH, bool isBitsFromScreenDC)
{
	return DoResizeBmp(hBmpSource, nDestW, nDestH, isBitsFromScreenDC);
}

HBITMAP DrawActor::DoResizeBmp(HBITMAP hBmpSource, int nDestW, int nDestH, bool isBitsFromScreenDC)
{
	return cc::Util::ResizeBmp(hBmpSource, nDestW, nDestH, isBitsFromScreenDC);
}

void DrawActor::DrawIcon(HDC hDC, int xLeft, int yTop, HICON hIcon, int cxWidth, int cyWidth, HBRUSH hBrush, UINT uFlags)
{
	DoDrawIcon(hDC, xLeft, yTop, hIcon, cxWidth, cyWidth, hBrush, uFlags);
}

void DrawActor::DoDrawIcon(HDC hDC, int xLeft, int yTop, HICON hIcon, int cxWidth, int cyWidth, HBRUSH hBrush, UINT uFlags)
{
	DrawIconEx(hDC, xLeft, yTop, hIcon, cxWidth, cyWidth, 0, hBrush, uFlags);
	//return;
	////CC_DrawIcon(hdc, xLeft, yTop, hIcon, cxWidth, cyWidth, hBrush, uFlags);
	//ICONINFO iconinfo;
	//if(hIcon == NULL || !::GetIconInfo(hIcon, &iconinfo))
	//{
	//	return;
	//}
	//int nIconX = iconinfo.xHotspot * 2;
	//int nIconY = iconinfo.yHotspot * 2;
	//HDC dcMem = ::CreateCompatibleDC(hDC);
	//HBITMAP bmp = ::CreateCompatibleBitmap(hDC, nIconX, nIconY);
	//HBITMAP OldBmp = (HBITMAP)::SelectObject(dcMem, bmp);
	////RECT rcRect = {0, 0, nIconX, nIconY};
	////if(hBrush != NULL)
	////{
	////	::FillRect(dcMem, &rcRect, hBrush);
	////}
	////::DrawIcon(dcMem, 0, 0, hIcon);
	//::DrawIconEx(dcMem, 0, 0, hIcon, nIconX, nIconY, 0, hBrush, DI_NORMAL);
	////int iOldMode = ::SetStretchBltMode(dcMem, COLORONCOLOR);
	//::StretchBlt(hDC, xLeft, yTop, cxWidth, cyWidth, dcMem, 0, 0, nIconX, nIconY, SRCCOPY);
	////if(iOldMode != 0)
	////{
	////	::SetStretchBltMode(dcMem, iOldMode);
	////}
	//::SelectObject(dcMem, OldBmp);
}

HBITMAP DrawActor::GetBitmap(const TCHAR* szFile, COLORREF corBack)
{
	return DoGetBitmap(szFile, corBack);
}
HBITMAP DrawActor::DoGetBitmap(const TCHAR* szFile, COLORREF corBack)
{
	HBITMAP hBitmap = NULL;
	int slen = cc::Str::Len(szFile);
	if(slen > 4)
	{
		DWORD nResType = -1;
		if(cc::Str::NCmp(szFile + (slen - 4), _T(".bmp"), 4, true) == 0)
		{
			nResType = IMAGE_BITMAP;
		}
		else if(cc::Str::NCmp(szFile + (slen - 4), _T(".ico"), 4, true) == 0)
		{
			nResType = IMAGE_ICON;
		}
		else if(cc::Str::NCmp(szFile + (slen - 4), _T(".cur"), 4, true) == 0)
		{
			nResType = IMAGE_CURSOR;
		}
		//ico,cur,bmp
		//IMAGE_BITMAP,IMAGE_CURSOR,IMAGE_ICON
		if(nResType >= 0)
		{
			HANDLE handle = ::LoadImage(CC_APP_GetInstance(),
				szFile,
				nResType, // type
				0, // actual width
				0, // actual height
				LR_LOADFROMFILE); // no flags
			if(handle != NULL)
			{
				if(nResType != IMAGE_BITMAP)
				{
					return Icon2Bmp((HICON)handle, corBack);
				}
				return (HBITMAP)handle;
			}
		}
	}

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
static const TCHAR * gDrawActor_RCDATA = _T("RT_RCDATA");

HBITMAP DrawActor::GetBitmap(UINT nResID, const TCHAR *resType, COLORREF corBack)
{
	return DoGetBitmap(nResID, resType, corBack);
}
HBITMAP DrawActor::DoGetBitmap(UINT nResID, const TCHAR *resType, COLORREF corBack)
{
	HBITMAP hBitmap = NULL;
	int nResType = -1;
	if(resType == RT_RCDATA)
	{
		resType = gDrawActor_RCDATA;
	}
	if(resType == RT_BITMAP)
	{
		nResType = IMAGE_BITMAP;
	}
	else if(resType == RT_GROUP_ICON || resType == RT_ICON)
	{
		nResType = IMAGE_ICON;
	}
	else if(resType == RT_GROUP_CURSOR || resType == RT_CURSOR)
	{
		nResType = IMAGE_CURSOR;
	}
	else if(!IS_INTRESOURCE(resType))
	{
		if(_tcscmp(resType, _T("#2")) == 0)
		{
			nResType = IMAGE_BITMAP;
		}
		else if(_tcscmp(resType, _T("#3")) == 0 || _tcscmp(resType, _T("#14")) == 0)
		{
			nResType = IMAGE_ICON;
		}
		else if(_tcscmp(resType, _T("#1")) == 0 || _tcscmp(resType, _T("#12")) == 0)
		{
			nResType = IMAGE_CURSOR;
		}
		else if(_tcscmp(resType, _T("#10")) == 0)
		{
			resType = gDrawActor_RCDATA;
		}
	}
	if(nResType >= 0)
	{
		HANDLE handle = ::LoadImage(CC_APP_GetInstance(), MAKEINTRESOURCE(nResID), nResType, // type
			0, // actual width
			0, // actual height
			0); // no flags
		//auto set to CC_BACK_TYPE_BMP
		if(handle != NULL)
		{
			if(nResType != IMAGE_BITMAP)
			{
				return Icon2Bmp((HICON)handle, corBack);
			}
			return (HBITMAP)handle;
		}
	}

	IPicture* pPicture = cc::Util::readPicture(nResID, resType);
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

HBITMAP DrawActor::GetGradientBmp(COLORREF corFrom, COLORREF corTo, int sizeW, int sizeH, bool isVertical, HDC hDC, HDC hDCDes, int nFromX, int nFromY)
{
	HDC hDC2 = hDC;
	if(hDC == NULL)
	{
		hDC2 = GetDC(NULL);
	}

	long redFrom   = GetRValue(corFrom);
	long greenFrom = GetGValue(corFrom);
	long blueFrom  = GetBValue(corFrom);
	long redDiff   = GetRValue(corTo) - redFrom;
	long greenDiff = GetGValue(corTo) - greenFrom;
	long blueDiff  = GetBValue(corTo) - blueFrom;
	long maxDiff = CC_ABS(redDiff) > CC_ABS(greenDiff) ? CC_ABS(redDiff) : CC_ABS(greenDiff);
	if(maxDiff < CC_ABS(blueDiff))
	{
		maxDiff = CC_ABS(blueDiff);
	}
	if(maxDiff <= 0)
	{
		maxDiff = 1;
	}

	int nWorH = 0;
	if(isVertical)
	{
		if(sizeW <= 0 || sizeH <= 0)
		{
			sizeW = 1;
			sizeH = maxDiff;
		}
		nWorH = sizeH;
	}
	else
	{
		if(sizeW <= 0 || sizeH <= 0)
		{
			sizeW = maxDiff;
			sizeH = 1;
		}
		nWorH = sizeW;
	}
	if(nWorH > maxDiff)
	{
		nWorH = maxDiff;
	}

	HBITMAP hBitmap;
	HDC memdc;
	COLORREF colour;
	if(isVertical)
	{
		hBitmap = ::CreateCompatibleBitmap(hDC2, 1, nWorH);
	}
	else
	{
		hBitmap = ::CreateCompatibleBitmap(hDC2, nWorH, 1);
	}
	memdc = ::CreateCompatibleDC(hDC2);
	HBITMAP oldbmp = (HBITMAP)::SelectObject(memdc, hBitmap);
	if(isVertical)
	{
		for(int i = 0; i < nWorH; i++)
		{
			colour = RGB(redFrom + (redDiff * i)/nWorH,
				greenFrom + (greenDiff * i)/nWorH, blueFrom + (blueDiff * i)/nWorH);
			SetPixelV(memdc, 0, i, colour);
		}
		if(hDCDes != NULL)
		{
			StretchBlt(hDCDes, nFromX, nFromY, sizeW, sizeH, memdc, 0, 0, 1, nWorH, SRCCOPY);
		}
	}
	else
	{
		for(int i = 0; i < nWorH; i++)
		{
			colour = RGB(redFrom + (redDiff * i)/nWorH,
				greenFrom + (greenDiff * i)/nWorH, blueFrom + (blueDiff * i)/nWorH);
			SetPixelV(memdc, i, 0, colour);
		}
		if(hDCDes != NULL)
		{
			StretchBlt(hDCDes, nFromX, nFromY, sizeW, sizeH, memdc, 0, 0, nWorH, 1, SRCCOPY);
		}
	}
	SelectObject(memdc, oldbmp);

	if(hDC == NULL)
	{
		ReleaseDC(NULL, hDC2);
	}
	return hBitmap;
}

HBITMAP DrawActor::CopyBitmap(HBITMAP hSrcBitmap)
{
	return cc::Util::CopyBitmap(hSrcBitmap);
}

HBITMAP DrawActor::GetBmpFromHDC(int nFromX, int nFromY, int nFromW, int nFromH, int sizeW, int sizeH, HDC hDC, int nBitCount)
{
	HDC hDC2 = hDC;
	BITMAPINFO bi;
	VOID *pvBits = NULL;
	if(hDC == NULL)
	{
		hDC2 = GetDC(NULL);
	}
	HBITMAP hBitmap;
	//HBITMAP hBitmap = CreateBitmap(sizeW, sizeH, bmPlanes, bmBitsPixel, NULL);
	if(nBitCount <= 0)
	{
		hBitmap = CreateCompatibleBitmap(hDC2, sizeW, sizeH);
	}
	else
	{
		//this time cannot save to SetClipboardData(CF_DIB, bmp)
		ZeroMemory(&bi.bmiHeader, sizeof(BITMAPINFOHEADER));
		bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bi.bmiHeader.biWidth        = sizeW;
		bi.bmiHeader.biHeight       = sizeH;
		bi.bmiHeader.biPlanes       = 1;
		bi.bmiHeader.biBitCount     = nBitCount;
		bi.bmiHeader.biCompression  = BI_RGB; //0,1,2,3
		hBitmap = CreateDIBSection(hDC, &bi, DIB_RGB_COLORS, &pvBits, NULL, 0);
	}

	HDC hTargetDC = CreateCompatibleDC(hDC2);
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hTargetDC, hBitmap);

	int mode = SetStretchBltMode(hTargetDC, COLORONCOLOR);
	StretchBlt(hTargetDC, 0, 0, sizeW, sizeH, hDC2, nFromX, nFromY, nFromW, nFromH, SRCCOPY);
	SetStretchBltMode(hTargetDC, mode);

	SelectObject(hTargetDC, hOldBitmap);
	DeleteDC(hTargetDC);
	if(hDC == NULL)
	{
		ReleaseDC(NULL, hDC2);
	}
	return hBitmap;
}

HBITMAP DrawActor::Icon2Bmp(HICON hicon, COLORREF corBack, int sizeW, int sizeH, HDC hDC)
{
	if(hicon == NULL)
	{
		return NULL;
	}
	HDC hDC2 = hDC;
	if(hDC == NULL)
	{
		hDC2 = GetDC(NULL);
	}
	if(sizeW <= 0 || sizeH <= 0)
	{
		ICONINFO infoIcon;
		if(!::GetIconInfo(hicon, &infoIcon))
		{
			cc::Log::error(_T("Util::Icon2Bmp, Error GetIconInfo."));
			return NULL;
		}
		if(sizeW <= 0)
		{
			sizeW = infoIcon.xHotspot * 2;
		}
		if(sizeH <= 0)
		{
			sizeH = infoIcon.yHotspot * 2;
		}
		DeleteObject(infoIcon.hbmColor);
		DeleteObject(infoIcon.hbmMask);
	}
	HBRUSH hBrush = NULL;
	if(corBack != (COLORREF)CLR_INVALID)
	{
		hBrush = ::CreateSolidBrush(corBack);
	}
	HDC dcMem = ::CreateCompatibleDC(hDC2);
	HBITMAP hBitmap = ::CreateCompatibleBitmap(hDC2, sizeW, sizeH);
	HBITMAP OldBmp = (HBITMAP)::SelectObject(dcMem, hBitmap);

	::DrawIconEx(dcMem, 0, 0, hicon, sizeW, sizeH, 0, hBrush, DI_NORMAL);

	::SelectObject(dcMem, OldBmp);
	if(hBrush != NULL)
	{
		::DeleteObject(hBrush);
	}
	DeleteDC(dcMem);
	if(hDC == NULL)
	{
		ReleaseDC(NULL, hDC2);
	}
	return hBitmap;
}

void DrawActor::DotLine(HDC hDC, int fromX, int fromY, int toX, int toY, float jumpX, float jumpY, COLORREF color, bool isTurnColor)
{
	float x = (float)fromX;
	float y = (float)fromY;
	COLORREF realcolor = color;
	while((int)x <= toX && (int)y <= toY)
	{
		if(isTurnColor)
		{
			COLORREF getcolor = GetPixel(hDC, (int)x, (int)y);
			realcolor = RGB(255 - GetRValue(getcolor), 255 - GetGValue(getcolor), 255 - GetBValue(getcolor));
			if(realcolor > RGB(110, 110, 110) && realcolor < RGB(140, 140, 140))
			{
				realcolor = RGB(255, 255, 255);
			}
		}
		SetPixelV(hDC, (int)x, (int)y, realcolor);
		x += jumpX;
		y += jumpY;
	}
}

void DrawActor::InvertBorder(int x, int y, int cx, int cy, HDC hDC, int nPenWidth)
{
	HDC hDC2 = hDC;
	if(hDC == NULL)
	{
		hDC2 = ::GetDC(NULL);
	}
	//int oldrop2 = GetROP2(hDC2);
	int oldrop2 = ::SetROP2(hDC2, R2_NOT);
	HPEN hPen = CreatePen(PS_INSIDEFRAME, nPenWidth, RGB(0,0,0));
	HPEN hOldPen = (HPEN)SelectObject(hDC2, hPen);
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC2, GetStockObject(NULL_BRUSH));
	// Draw the rectangle
	Rectangle(hDC2, x, y, x + cx, y + cy);
	SelectObject(hDC2, hOldBrush);
	SelectObject(hDC2, hOldPen);
	SetROP2(hDC2, oldrop2);
	if(hDC == NULL)
	{
		::ReleaseDC(NULL, hDC2);
	}
	DeleteObject(hPen);
}

void DrawActor::TransparentBlt(
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
)
{
	HBITMAP hOldImageBMP, hImageBMP = CreateCompatibleBitmap(hdcDest, nWidthDest, nHeightDest);
	HBITMAP hOldMaskBMP, hMaskBMP = CreateBitmap(nWidthDest, nHeightDest, 1, 1, NULL);
	HDC		hImageDC = CreateCompatibleDC(hdcDest);
	HDC		hMaskDC = CreateCompatibleDC(hdcDest);
	hOldImageBMP = (HBITMAP)SelectObject(hImageDC, hImageBMP);
	hOldMaskBMP = (HBITMAP)SelectObject(hMaskDC, hMaskBMP);
	if(nWidthDest == nWidthSrc && nHeightDest == nHeightSrc)
	{
		BitBlt(hImageDC, 0, 0, nWidthDest, nHeightDest, hdcSrc, nXOriginSrc, nYOriginSrc, SRCCOPY);
	}
	else
	{
		SetStretchBltMode(hImageDC, COLORONCOLOR);
		StretchBlt(hImageDC, 0, 0, nWidthDest, nHeightDest, hdcSrc, nXOriginSrc, nYOriginSrc, nWidthSrc, nHeightSrc, SRCCOPY);
	}
	SetBkColor(hImageDC, crTransparent);
	BitBlt(hMaskDC, 0, 0, nWidthDest, nHeightDest, hImageDC, 0, 0, SRCCOPY);
	SetBkColor(hImageDC, RGB(0,0,0));
	SetTextColor(hImageDC, RGB(255,255,255));
	BitBlt(hImageDC, 0, 0, nWidthDest, nHeightDest, hMaskDC, 0, 0, SRCAND);
	SetBkColor(hdcDest, RGB(255,255,255));
	SetTextColor(hdcDest, RGB(0,0,0));
	BitBlt(hdcDest, nXOriginDest, nYOriginDest, nWidthDest, nHeightDest, hMaskDC, 0, 0, SRCAND);
	BitBlt(hdcDest, nXOriginDest, nYOriginDest, nWidthDest, nHeightDest, hImageDC, 0, 0, SRCPAINT);
	SelectObject(hImageDC, hOldImageBMP);
	DeleteDC(hImageDC);
	SelectObject(hMaskDC, hOldMaskBMP);
	DeleteDC(hMaskDC);
	DeleteObject(hImageBMP);
	DeleteObject(hMaskBMP);
}

void DrawActor::DrawFocusFrame(HDC hDC, int nLeft, int nTop, int nWidth, int nHeight)
{
	DoDrawFocusFrame(hDC, nLeft, nTop, nWidth, nHeight);
}

void DrawActor::DrawFocusFrame(HDC hDC, RECT rect)
{
	DoDrawFocusFrame(hDC, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);
}

void DrawActor::DoDrawFocusFrame(HDC hDC, int nLeft, int nTop, int nWidth, int nHeight)
{
	POINT oldPoint;
	HPEN pLeft = CreatePen(PS_INSIDEFRAME | PS_SOLID, 3, RGB(250, 196, 88)); 
	HPEN pRight = CreatePen(PS_INSIDEFRAME | PS_SOLID, 3, RGB(251, 202, 106));
	HPEN pTop = CreatePen(PS_INSIDEFRAME | PS_SOLID, 2, RGB(252, 210, 121));
	HPEN pBottom = CreatePen(PS_INSIDEFRAME | PS_SOLID, 2, RGB(229, 151, 0));
	::MoveToEx(hDC, nLeft, nTop + nHeight - 1, &oldPoint);
	HPEN pOldPen = (HPEN)SelectObject(hDC, pLeft);
	::LineTo(hDC, nLeft, nTop + 1);
	::SelectObject(hDC, pRight);
	::MoveToEx(hDC, nLeft + nWidth - 1, nTop + nHeight - 1, NULL);
	LineTo(hDC, nLeft + nWidth - 1, nTop);
	SelectObject(hDC, pTop);
	::MoveToEx(hDC, nLeft - 1, nTop, NULL);
	LineTo(hDC, nLeft + nWidth - 1, nTop);
	SelectObject(hDC, pBottom);
	::MoveToEx(hDC, nLeft, nTop + nHeight, NULL);
	LineTo(hDC, nLeft + nWidth - 1, nTop + nHeight);
	SelectObject(hDC, pOldPen);
	::MoveToEx(hDC, oldPoint.x, oldPoint.y, NULL);
	::DeleteObject(pLeft);
	::DeleteObject(pRight);
	::DeleteObject(pTop);
	::DeleteObject(pBottom);
}

void DrawActor::MeasureMenu(MenuItem* item, MeasureItemEventArgs *e)
{
	DoMeasureMenu(item, e);
}

void DrawActor::DoMeasureMenu(MenuItem* item, MeasureItemEventArgs *e)
{
	if(item->GetBreak())
	{
		e->data->itemHeight = DrawActorConfig::nMenuBreakHeight;
	}
	else
	{
		RECT rc = {0, 0, 0, 0};
		const TCHAR* tc = item->GetText();
		if(tc != NULL)
		{
			HFONT holdFont = (HFONT)::SelectObject(e->hDC, GetMenuFont());
			::DrawText(e->hDC, tc, -1, &rc, DT_CALCRECT);
			rc.right += DrawActorConfig::nMenuMarginLeft;
			if(item->GetShowShortcut() && item->GetShortcutText() != NULL)
			{
				RECT rc2 = {0, 0, 0, 0};
				::DrawText(e->hDC, item->GetShortcutText(), -1, &rc2, DT_CALCRECT);
				rc.right += rc2.right + DrawActorConfig::nMenuMarginShortcut;
				rc.bottom = CC_MAX(rc.bottom, rc2.bottom);
			}
			::SelectObject(e->hDC, holdFont);
		}
		e->data->itemHeight = CC_MAX(rc.bottom, DrawActorConfig::nMenuIconWH + 4);
		//the min length of menu
		e->data->itemWidth = CC_MAX(rc.right, DrawActorConfig::nMenuMinWidth);
		if(e->data->itemWidth > (UINT)DrawActorConfig::nMenuMaxWidth)
		{
			e->data->itemWidth = DrawActorConfig::nMenuMaxWidth;
		}
	}
}

void DrawActor::DrawMenuTitle(MenuItem* item, DrawItemEventArgs *e)
{
	DoDrawMenuTitle(item, e);
}

void DrawActor::DoDrawMenuTitle(MenuItem* item, DrawItemEventArgs *e)
{
	RECT rect = e->data->rcItem;
	bool isOn = ((e->data->itemState & ODS_SELECTED) != 0);
	//selected or selection state changed: paint text background
	if(isOn)
	{
		HBRUSH brush = ::CreateSolidBrush(isOn ? MainMenuBarConfig::corHotBackColor : MainMenuBarConfig::corBackground);
		HBRUSH oldBrush = (HBRUSH)::SelectObject(e->data->hDC, brush);
		::PatBlt(e->data->hDC, rect.left + 1, rect.top, rect.right - rect.left - 2, rect.bottom - rect.top, PATCOPY);
		::SelectObject(e->data->hDC, oldBrush);
		::DeleteObject(brush);
	}
	else
	{
		HWND hWnd = ::WindowFromDC(e->data->hDC);
		HWND parent = ::GetParent(hWnd);
		CC_ASSERT(parent != NULL);

		//let parent to EraseBackground
		HRGN hRgn = CreateRectRgn(rect.left, rect.top, rect.right, rect.bottom);
		ExtSelectClipRgn(e->data->hDC, hRgn, RGN_COPY);

		POINT pt = {0, 0};
		::MapWindowPoints(hWnd, parent, (LPPOINT)&pt, 1);
		OffsetWindowOrgEx(e->data->hDC, pt.x, pt.y, &pt);
		::SendMessage(parent, WM_ERASEBKGND, (WPARAM)e->data->hDC, (LPARAM)this);
		SetWindowOrgEx(e->data->hDC, pt.x, pt.y, NULL);

		SelectClipRgn(e->data->hDC, NULL);
		DeleteObject(hRgn);
	}

	::SetBkMode(e->data->hDC, TRANSPARENT); // paint transparent text
	::SetTextColor(e->data->hDC, isOn ? MainMenuBarConfig::corHotTextColor : MainMenuBarConfig::corTextground);
	::DrawText(e->data->hDC, item->GetText(), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
}

void DrawActor::DrawMenu(MenuItem* item, DrawItemEventArgs *e)
{
	DoDrawMenu(item, e);
}

void DrawActor::DoDrawMenu(MenuItem* item, DrawItemEventArgs *e)
{
	HDC hDC = e->data->hDC;
	RECT rect = e->data->rcItem;
	RECT rect2;
	if(item->GetBreak())
	{
		//draw separator
		rect.top += (rect.bottom - rect.top) >>1; //vertical center
		::DrawEdge(hDC, &rect, EDGE_ETCHED, BF_TOP); //draw separator line
		return;
	}

	HFONT holdFont = (HFONT)::SelectObject(hDC, GetMenuFont());
	//not a separator
	//bool menuDisabled = (e->data->itemState & ODS_GRAYED) != 0;
	bool menuDisabled = !item->GetEnabled();
	bool menuSelected = (e->data->itemState & ODS_SELECTED) != 0;
	//bool menuSelected = item->GetSelected();
	//bool menuChecked  = (e->data->itemState & ODS_CHECKED) != 0;
	bool menuChecked  = item->GetChecked();

	//compute text rectangle and colors
	HICON hIcon = item->GetIcon(IconType::Hot);
	COLORREF colorText = (menuDisabled ?  DrawActorConfig::corMenuDisabledTextground : 
						(menuSelected ? DrawActorConfig::corMenuHotTextColor : DrawActorConfig::corMenuTextground));
	::SetTextColor(hDC, colorText);
	HBRUSH brush = NULL;
	::SetBkMode(hDC, TRANSPARENT); // paint transparent text
	//if(menuSelected || (e->data->itemAction == ODA_SELECT))
	//{
		//selected or selection state changed: paint text background
		brush = ::CreateSolidBrush(menuSelected ? DrawActorConfig::corMenuHotBackColor : DrawActorConfig::corMenuBackground);
		HBRUSH oldBrush = (HBRUSH)::SelectObject(hDC, brush);
		::PatBlt(hDC, rect.left, rect.top + 2, rect.right - rect.left, rect.bottom - rect.top - 2, PATCOPY);
		::SelectObject(hDC, oldBrush);
	//}

	// draw icon,only exist normal icon,then others
	if(hIcon != NULL)
	{
		if(menuDisabled)
		{
			hIcon = item->GetIcon(IconType::Disabled);
		}
		else if(menuSelected)
		{
		}
		else
		{
			hIcon = item->GetIcon(IconType::Gloom);
		}
	}
	if(hIcon != NULL)
	{
		if(!menuDisabled || !menuSelected)
		{
			if(brush != NULL)
			{
				::DeleteObject(brush);
				brush = NULL;
			}
		}
		int nY = rect.top + (rect.bottom - rect.top - DrawActorConfig::nMenuIconWH)/2 + 1;
		//brush is setter before
		int nJump = 0;
		if(menuSelected && !menuDisabled)
		{
		//	DrawIconEx(hDC, rect.left + 2, nY, hIcon, DrawActorConfig::nMenuIconWH, DrawActorConfig::nMenuIconWH, 0, brush, DSS_NORMAL | DSS_MONO);
			nJump = -1;
		}
		//DrawIconEx(hDC, rect.left + 2 + nJump, nY + nJump, hIcon, DrawActorConfig::nMenuIconWH, DrawActorConfig::nMenuIconWH, 0, brush, DI_NORMAL);
		//brush = ::CreateSolidBrush(::GetSysColor(menuSelected ? COLOR_HIGHLIGHT : COLOR_MENU));
		DoDrawIcon(hDC, rect.left + 2 + nJump, nY + nJump, hIcon, DrawActorConfig::nMenuIconWH, DrawActorConfig::nMenuIconWH, brush, DI_NORMAL);
		//if(!menuDisabled && e->data->itemAction == ODA_SELECT)
		//{
		//	RECT rectEdge = {rect.left + 1, nY - 1, rect.left + 3 + DrawActorConfig::nMenuIconWH, nY + 1 + DrawActorConfig::nMenuIconWH};
		//	::DrawEdge(hDC, &rectEdge, BDR_RAISEDINNER, BF_RECT);
		//}
	}
	else if(menuChecked)
	{
		::SetRect(&rect2, rect.left + 2, rect.top + 3,
			rect.left + 2 + DrawActorConfig::nMenuIconWH, rect.top + 3 + DrawActorConfig::nMenuIconWH);
		::DrawEdge(hDC, &rect2, BDR_SUNKENOUTER, BF_RECT);
		OffsetRect(&rect2, 1, 1);
		DrawSpecialChar(hDC, rect2, 98, false, colorText);
	}
	if(brush != NULL)
	{
		::DeleteObject(brush);
		brush = NULL;
	}

	// Now paint menu item text.the font had been set before sending WM_DRAWITEM
	rect.left += DrawActorConfig::nMenuMarginLeft;
	const TCHAR* tcText = item->GetText();
	if(tcText != NULL)
	{
		RECT recttext;
		::CopyRect(&recttext, &rect);
		//right margin
		recttext.right -= DrawActorConfig::nMenuMarginRight;
		const TCHAR* tcShortcut = NULL;
		if(item->GetShowShortcut() && (tcShortcut = item->GetShortcutText()) != NULL)
		{
			SetRectEmpty(&rect2);
			::DrawText(hDC, tcShortcut, -1, &rect2, DT_CALCRECT);
			recttext.right -= rect2.right;
		}

		//DT_LEFT|DT_VCENTER|DT_EDITCONTROL|DT_END_ELLIPSIS|DT_WORDBREAK
		if(menuDisabled && !menuSelected)
		{
			OffsetRect(&recttext, 1, 1);
			::SetTextColor(hDC, ::GetSysColor(COLOR_BTNHIGHLIGHT));
			::DrawText(hDC, tcText, -1, &recttext, DT_LEFT|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS|DT_WORDBREAK);
			OffsetRect(&recttext, -1, -1);
			::SetTextColor(hDC, ::GetSysColor(COLOR_BTNSHADOW));
			::DrawText(hDC, tcText, -1, &recttext, DT_LEFT|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS|DT_WORDBREAK);
		}
		else
		{
			::DrawText(hDC, tcText, -1, &recttext, DT_LEFT|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS|DT_WORDBREAK);
		}
		//tcText = item->GetShortcutText();
		//if(item->GetShowShortcut() && tcText != NULL)
		if(tcShortcut != NULL)
		{
			if(menuDisabled && !menuSelected)
			{
				rect.left = rect.right - rect2.right - DrawActorConfig::nMenuMarginShortcut;
				OffsetRect(&rect, 1, 1);
				::SetTextColor(hDC, ::GetSysColor(COLOR_BTNHIGHLIGHT));
				::DrawText(hDC, tcShortcut, -1, &rect, DT_LEFT|DT_SINGLELINE|DT_VCENTER);
				OffsetRect(&rect, -1, -1);
				::SetTextColor(hDC, ::GetSysColor(COLOR_BTNSHADOW));
				::DrawText(hDC, tcShortcut, -1, &rect, DT_LEFT|DT_SINGLELINE|DT_VCENTER);
			}
			else
			{
				rect.left = rect.right - rect2.right - DrawActorConfig::nMenuMarginShortcut;
				::DrawText(hDC, tcShortcut, -1, &rect, DT_LEFT|DT_SINGLELINE|DT_VCENTER);
			}
		}
	}
	::SelectObject(hDC, holdFont);
}

SIZE DrawActor::MeasureControl(Control* control, int ctlHeight, DWORD dwStyle)
{
	return DoMeasureControl(control, ctlHeight, dwStyle);
}

SIZE DrawActor::DoMeasureControl(Control* control, int ctlHeight, DWORD dwStyle)
{
	SIZE size;
	if(ctlHeight == -1 || control->HasStyle(StyleType::Outer, OuterStyle::Blank))
	{
		RECT rect = control->GetBounds();
		ctlHeight = rect.bottom - rect.top;
		if(control->HasStyle(StyleType::Outer, OuterStyle::Blank))
		{
			size.cx = rect.right - rect.left;
			size.cy = ctlHeight;
			return size;
		}
	}
	if(control->HasStyle(StyleType::Outer, OuterStyle::Break))
	{
		size.cx = 3;
		size.cy = ctlHeight;
	}	
	else
	{
		int nTextHeight = 0;
		RECT rc = {0, 0, 0, 0};
		RECT margin = control->GetMargin();
		int len = control->HasStyle(StyleType::Outer, OuterStyle::DrawNoText) ? 0 : control->GetTextLen();
		if(len > 0)
		{
			//if m_hWnd is NULL,then get screen HDC
			HDC hDC = ::GetDC(control->m_hWnd);
			HFONT holdFont = (HFONT)::SelectObject(hDC, control->GetFont());
			const TCHAR* tc = control->GetText();
			::DrawText(hDC, tc, -1, &rc, DT_CALCRECT);
			::SelectObject(hDC, holdFont);
			::DeleteObject(holdFont);
			::ReleaseDC(control->m_hWnd, hDC);
			//space before and after text
			rc.right += margin.left + margin.right;
			rc.bottom += margin.top + margin.bottom;
			nTextHeight = rc.bottom;
		}
		rc.bottom = CC_MAX(rc.bottom, ctlHeight);
		if(!control->HasStyle(StyleType::Outer, OuterStyle::DrawNoIcon) && control->GetIcon() != NULL)
		{
			if(len <= 0)
			{
				rc.right = margin.left + margin.right + rc.bottom - margin.top - margin.bottom;
			}
			else
			{
				if(control->HasStyle(StyleType::Outer, OuterStyle::DrawIconStretch))
				{
				}
				else if(control->HasStyle(StyleType::Outer, OuterStyle::DrawIconTop))
				{
					int nWH = rc.bottom - rc.top - nTextHeight;
					if(nWH > rc.right - rc.left - margin.left - margin.right)
					{
						rc.right = rc.left + nWH + margin.left + margin.right;
					}
				}
				else
				{
					rc.right += rc.bottom + CONTROL_SPACE_ICONTEXT - margin.top - margin.bottom;
				}
			}
		}
		size.cy = rc.bottom;
		size.cx = rc.right;
		//if(control->HasStyle(StyleType::Outer, OuterStyle::ContextMenuButton) && control->GetContextMenu() != NULL && !control->HasStyle(StyleType::Outer, OuterStyle::PushButton))
		if((dwStyle & CC_DRAWITEM_RIGHTARROW) != 0)
		{
			size.cx += CONTROL_CONTEXTMENU_BTN_W;
		}
		if(control->HasStyle(StyleType::Outer, OuterStyle::OuterEdge))
		{
			size.cx += 6;
		}
	}
	return size;
}

void DrawActor::DrawControl(Control* control, DrawItemEventArgs *e)
{
	DoDrawControl(control, e);
}

void DrawActor::DoDrawControl(Control* control, DrawItemEventArgs *e)
{
	HDC hDC = e->data->hDC;
	RECT rcItem = e->data->rcItem;
	RECT margin = control->GetMargin();
	UINT uState = e->data->itemState;
	bool isDisabled = !control->GetEnabled();
	bool isSele = ((uState & ODS_SELECTED) != 0);
	bool isPushedBtn = control->HasStyle(StyleType::Outer, OuterStyle::PushButton);
	bool isMenu = ((e->dwStyle & CC_DRAWITEM_POPMENU) != 0);
	bool isMouseOn = isMenu || ((uState & ODS_FOCUS) || control->GetIsMouseOn());
	//bool isRightPushedMenu = (control->HasStyle(StyleType::Outer, OuterStyle::ContextMenuButton) && control->GetContextMenu() != NULL && !control->HasStyle(StyleType::Outer, OuterStyle::PushButton));
	bool isRightPushedMenu = ((e->dwStyle & CC_DRAWITEM_RIGHTARROW) != 0);

	//cc::Log::debug(_T("OldL:%d, OldT:%d, OldW:%d, OldH:%d"), rcItem.left, rcItem.top, rcItem.right - rcItem.left, rcItem.bottom - rcItem.top);

	// set bkcolor
	if(!control->HasStyle(StyleType::Inner, InnerStyle::BackgroundGradient|InnerStyle::BackgroundBrush|InnerStyle::BackgroundTheme|InnerStyle::BackgroundBmp)
		&& (!control->HasStyle(StyleType::Outer, OuterStyle::TransparentParentErase)
		|| (isMouseOn && control->HasStyle(StyleType::Outer, OuterStyle::TransparentOffMouseOn))))
	{
		if(isMouseOn)
		{
			//COLORREF back = isMouseOn ? control->GetHotBackColor() : control->GetBackground();
			::SetBkColor(hDC, control->GetHotBackColor());
			::ExtTextOut(hDC, 0, 0, ETO_OPAQUE, &rcItem, NULL, 0, NULL);
		}
		else
		{
			HBRUSH hbr = control->GetBackgroundBrush();
			::FillRect(hDC, &rcItem, hbr);
		}
	}
	else
	{
		::SendMessage(control->m_hWnd, WM_ERASEBKGND, (WPARAM)hDC, (LPARAM)this);
	}

	PaintEventArgs ee(e->baseArgs);
	ee.data.hdc = e->data->hDC;
	CopyRect(&ee.data.rcPaint, &rcItem);
	control->PaintBegin.Invoke((Object*)control, &ee);
	if(control->HasStyle(StyleType::Outer, OuterStyle::Blank))
	{
		control->PaintEnd.Invoke((Object*)control, &ee);
		return;
	}

	::SetBkMode(hDC, TRANSPARENT);
	//for toolitem while 1 then not show Edge always(even with mouse on)
	bool isAlwaysNoEdge = control->HasStyle(StyleType::Outer, OuterStyle::AlwaysNoEdge);
	if(control->HasStyle(StyleType::Outer, OuterStyle::Break))
	{
		if(isAlwaysNoEdge)
		{
			//for statusitem
			::OffsetRect(&rcItem, -1, 0);
			::DrawEdge(hDC, &rcItem, BDR_RAISEDINNER, BF_LEFT);
			::OffsetRect(&rcItem, 1, 0);
			::DrawEdge(hDC, &rcItem, BDR_RAISEDOUTER, BF_RIGHT);
		}
		else
		{
			//for toolitem or others
			::DrawEdge(hDC, &rcItem, EDGE_ETCHED, rcItem.right - rcItem.left < 5 ? BF_LEFT : BF_TOP);
		}
		control->PaintEnd.Invoke((Object*)control, &ee);
		return;
	}

	if(control->HasStyle(StyleType::Outer, OuterStyle::OuterEdge))
	{
		DrawEdge(hDC, &rcItem, EDGE_SUNKEN, BF_RECT);
		rcItem.top += 3;
		rcItem.left += 4;
		rcItem.right -= 2;
		rcItem.bottom -= 3;
	}

	bool isAlwaysEdge = control->HasStyle(StyleType::Outer, OuterStyle::AlwaysEdge);
	//a copy of rect
	RECT rect;
	CopyRect(&rect, &rcItem);
	rect.left += margin.left;
	rect.top += margin.top;
	rect.right -= margin.right;
	rect.bottom -= margin.bottom;
	//::SetBkMode(hDC, TRANSPARENT);
	HFONT holdFont = (HFONT)::SelectObject(hDC, control->GetFont());

	// draw icon,only exist normal icon,then others
	HICON hIcon = control->HasStyle(StyleType::Outer, OuterStyle::DrawNoIcon) ? NULL : control->GetIcon(IconType::Hot);
	if(hIcon != NULL)
	{
		if(isDisabled)
		{
			hIcon = control->GetIcon(IconType::Disabled);
		}
		else if(isSele && isMouseOn)
		{
			hIcon = control->GetIcon(IconType::Pushed);
		}
		else if(isMouseOn || isAlwaysNoEdge)
		{
		}
		else
		{
			hIcon = control->GetIcon(IconType::Gloom);
		}
		int nJump = 1;
		if(control->HasStyle(StyleType::Outer, OuterStyle::DrawIconStretch))
		{
			if(isRightPushedMenu)
			{
				DoDrawIcon(hDC, rcItem.left + 2, rcItem.top + 2, hIcon, rcItem.right - CONTROL_CONTEXTMENU_BTN_W + 1 - 4, rcItem.bottom - 4, NULL, DI_NORMAL);
			}
			else
			{
				DoDrawIcon(hDC, rcItem.left + 2, rcItem.top + 2, hIcon, rcItem.right - 4, rcItem.bottom - 4, NULL, DI_NORMAL);
			}
		}
		else if(control->HasStyle(StyleType::Outer, OuterStyle::DrawIconTop))
		{
			RECT rc = {0, 0, 0, 0};
			const TCHAR* tc = control->GetText();
			if(tc != NULL)
			{
				::DrawText(hDC, tc, -1, &rc, DT_CALCRECT);
				rc.bottom += margin.bottom;
			}
			int nHW = 0;
			int nLeft = 0;
			if(isRightPushedMenu)
			{
				nHW = CC_MIN(rect.bottom - rect.top - CONTROL_SPACE_ICONTEXT_VERT - rc.bottom, rect.right - rect.left - CONTROL_CONTEXTMENU_BTN_W + 1);
				nLeft = (rect.right - rect.left - CONTROL_CONTEXTMENU_BTN_W + 1 - nHW)/2 - 1;
			}
			else
			{
				nHW = CC_MIN(rect.bottom - rect.top - CONTROL_SPACE_ICONTEXT_VERT - rc.bottom, rect.right - rect.left);
				nLeft = (rect.right - rect.left - nHW)/2 - 1;
			}
			if((isMouseOn || isSele) && !isAlwaysNoEdge)
			{
				nJump = 0;
			}
			DoDrawIcon(hDC, rect.left + nLeft + nJump, rect.top + nJump, hIcon, rect.left + nHW, rect.top + nHW, NULL, DI_NORMAL);
			rect.top = rect.bottom - rc.bottom;
		}
		else
		{
			int nHW = rect.bottom - rect.top;
			if(rect.right - rect.left >= nHW)
			{
				if((isMouseOn || isSele) && !isAlwaysNoEdge)
				{
					nJump = 0;
				}
				DoDrawIcon(hDC, rect.left + nJump, rect.top + nJump, hIcon, nHW, nHW, NULL, DI_NORMAL);
			}
			rect.left += nHW + CONTROL_SPACE_ICONTEXT;
		}
	}

	int len = control->HasStyle(StyleType::Outer, OuterStyle::DrawNoText) ? 0 : control->GetTextLen();
	if(len > 0)
	{
		const TCHAR* tcText = control->GetText();
		DWORD dwTextStyle = control->GetStyle(StyleType::OwnerDrawText);
		//DT_LEFT|DT_VCENTER|DT_END_ELLIPSIS|DT_EDITCONTROL|DT_WORDBREAK
		if(isRightPushedMenu)
		{
			rect.right -= CONTROL_CONTEXTMENU_BTN_W - 1;
		}
		if(isDisabled)
		{
			//::DrawState(hDC, (HBRUSH)NULL, NULL, (LPARAM)tcText, (WPARAM)len, 
			//	rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top, DSS_DISABLED|DST_PREFIXTEXT);
			OffsetRect(&rect, 1, 1);
			::SetTextColor(hDC, ::GetSysColor(COLOR_BTNHIGHLIGHT));
			::DrawText(hDC, tcText, -1, &rect, dwTextStyle);
			OffsetRect(&rect, -1, -1);
			::SetTextColor(hDC, ::GetSysColor(COLOR_BTNSHADOW));
			::DrawText(hDC, tcText, -1, &rect, dwTextStyle);
		}
		else
		{
			::SetTextColor(hDC, isMouseOn ? control->GetHotTextColor() : control->GetTextColor());
			::DrawText(hDC, tcText, -1, &rect, dwTextStyle);
		}
	}

	if(isRightPushedMenu)
	{
		::SetRect(&rect, rcItem.right - CONTROL_CONTEXTMENU_BTN_W + 1, rcItem.top + 1, rcItem.right, rcItem.bottom);
		if(isDisabled)
		{
			OffsetRect(&rect, 1, 1);
			DrawSpecialChar(hDC, rect, _T('9'), false, ::GetSysColor(COLOR_BTNHIGHLIGHT), 18);
			OffsetRect(&rect, -1, -1);
			DrawSpecialChar(hDC, rect, _T('9'), false, ::GetSysColor(COLOR_BTNSHADOW), 18);
		}
		else
		{
			DrawSpecialChar(hDC, rect, _T('9'), false, isMouseOn ? control->GetHotTextColor() : control->GetTextColor(), 18);
		}
		rect.top--;
		rect.left--;
		if(!isAlwaysNoEdge)
		{
			if(isMouseOn)
			{
				if(isSele || isMenu)
				{
					if(isAlwaysEdge)
					{
						::DrawEdge(hDC, &rect, BDR_SUNKENOUTER|BDR_SUNKENINNER, BF_SOFT|BF_RECT);
					}
					else
					{
						::DrawEdge(hDC, &rect, BDR_SUNKENOUTER, BF_RECT);
					}
				}
				else
				{
					if(isAlwaysEdge)
					{
						::DrawEdge(hDC, &rect, BDR_RAISEDINNER|BDR_RAISEDOUTER, BF_SOFT|BF_RECT);
					}
					else
					{
						::DrawEdge(hDC, &rect, BDR_RAISEDINNER, BF_RECT);
					}
				}
			}
			else
			{
				if(isSele && isPushedBtn)
				{
					if(isAlwaysEdge)
					{
						::DrawEdge(hDC, &rect, BDR_SUNKENOUTER|BDR_SUNKENINNER, BF_SOFT|BF_RECT);
					}
					else
					{
						::DrawEdge(hDC, &rect, BDR_SUNKENOUTER, BF_RECT);
					}
				}
				else if(isAlwaysEdge)
				{
					::DrawEdge(hDC, &rect, BDR_RAISEDINNER|BDR_RAISEDOUTER, BF_SOFT|BF_RECT);
				}
			}
		}
		rcItem.right -= CONTROL_CONTEXTMENU_BTN_W;
	}
	if(!isAlwaysNoEdge && (len > 0 || hIcon != NULL))
	{
		if(isMouseOn)
		{
			if(isSele && (!isMenu || (isMenu && !control->HasStyle(StyleType::Outer, OuterStyle::ContextMenuButton))))
			{
				if(isAlwaysEdge)
				{
					::DrawEdge(hDC, &rcItem, BDR_SUNKENOUTER|BDR_SUNKENINNER, BF_SOFT|BF_RECT);
				}
				else
				{
					::DrawEdge(hDC, &rcItem, BDR_SUNKENOUTER, BF_RECT);
				}
			}
			else
			{
				if(isAlwaysEdge)
				{
					::DrawEdge(hDC, &rcItem, BDR_RAISEDINNER|BDR_RAISEDOUTER, BF_SOFT|BF_RECT);
				}
				else
				{
					::DrawEdge(hDC, &rcItem, BDR_RAISEDINNER, BF_RECT);
				}
			}
		}
		else
		{
			if(isSele && isPushedBtn)
			{
				if(isAlwaysEdge)
				{
					::DrawEdge(hDC, &rcItem, BDR_SUNKENOUTER|BDR_SUNKENINNER, BF_SOFT|BF_RECT);
				}
				else
				{
					::DrawEdge(hDC, &rcItem, BDR_SUNKENOUTER, BF_RECT);
				}
			}
			else if(isAlwaysEdge)
			{
				::DrawEdge(hDC, &rcItem, BDR_RAISEDINNER|BDR_RAISEDOUTER, BF_SOFT|BF_RECT);
			}
		}
	}
	if(uState & ODS_FOCUS)
	{
		// && !control->HasStyle(StyleType::Outer, OuterStyle::NoPush)
		if((len > 0 || hIcon != NULL) && (!isAlwaysNoEdge
			|| (control->HasStyle(StyleType::Style, WS_TABSTOP|WS_VISIBLE) && !isDisabled)))
		{
			//if((e->dwStyle & CC_DRAWITEM_CHECKBOX) != 0)
			//{
			//	rcItem.left += CC_CHECKBOX_BOXWH + 2;
			//}
			//use origal rect
			//rcItem.left += 2;
			//rcItem.top += 2;
			//rcItem.right -= 2;
			//rcItem.bottom -= 2;
			::InflateRect(&rcItem, -2, -2);
			::DrawFocusRect(hDC, &rcItem);
		}
	}
	control->PaintEnd.Invoke((Object*)control, &ee);
	holdFont = (HFONT)::SelectObject(hDC, holdFont);
	if(control->GetFont() != holdFont)
	{
		::DeleteObject(holdFont);
	}
}

SIZE DrawActor::MeasureToolItem(ToolItem* control, int ctlHeight, DWORD dwStyle)
{
	return DoMeasureToolItem(control, ctlHeight, dwStyle);
}

SIZE DrawActor::DoMeasureToolItem(ToolItem* control, int ctlHeight, DWORD dwStyle)
{
	return DoMeasureControl(control, ctlHeight, dwStyle);
}

void DrawActor::DrawToolItem(ToolItem* control, DrawItemEventArgs *e)
{
	DoDrawToolItem(control, e);
}

void DrawActor::DoDrawToolItem(ToolItem* control, DrawItemEventArgs *e)
{
	DoDrawControl(control, e);
}

SIZE DrawActor::MeasureStatusItem(StatusItem* control, int ctlHeight, DWORD dwStyle)
{
	return DoMeasureStatusItem(control, ctlHeight, dwStyle);
}

SIZE DrawActor::DoMeasureStatusItem(StatusItem* control, int ctlHeight, DWORD dwStyle)
{
	return DoMeasureControl(control, ctlHeight, dwStyle);
}

void DrawActor::DrawStatusItem(StatusItem* control, DrawItemEventArgs *e)
{
	DoDrawStatusItem(control, e);
}

void DrawActor::DoDrawStatusItem(StatusItem* control, DrawItemEventArgs *e)
{
	DoDrawControl(control, e);
}

void DrawActor_DrawEtchedRectangle(HDC hDC, RECT& rect, COLORREF cor, bool isEtched)
{
	HPEN hPenDark = CreatePen(PS_SOLID, 1, cor);
	HPEN hPenOld = NULL;

	if(isEtched)
	{
		// border is 2 pixels, so deflate bottom and right sides by 1
		HPEN hPenLight = CreatePen(PS_SOLID, 2, GetSysColor(COLOR_BTNHIGHLIGHT));
		hPenOld = (HPEN)::SelectObject(hDC, &hPenLight);
		//DeflateRect(&rect, 0, 0, 1, 1);
		//OffsetRect(&rect, 1, 1);
		// white rectangle, 2 pixels wide
		Rectangle(hDC, rect.left + 1, rect.top + 1, rect.right, rect.bottom);
		//OffsetRect(&rect, -1, -1);
		::SelectObject(hDC, &hPenDark);
	}
	else
	{
		hPenOld = (HPEN)::SelectObject(hDC, &hPenDark);
	}

	// dark rectangle - overlay white rectangle
	::Rectangle(hDC, rect.left, rect.top, rect.right - 1, rect.bottom - 1);
	SelectObject(hDC, hPenOld);
}

void DrawActor::DrawPanel(Control* control, DrawItemEventArgs *e)
{
	DoDrawPanel(control, e);
}

#define CC_DRAWPANEL_TITLE_RECT 8
#define CC_DRAWPANEL_TITLE_SPACE 2
void DrawActor::DoDrawPanel(Control* control, DrawItemEventArgs *e)
{
	HDC hDC = e->data->hDC;
	RECT rcItem = e->data->rcItem;
	RECT rect = control->GetMargin();
	UINT uState = e->data->itemState;
	bool isDisabled = !control->GetEnabled();
	//cc::Log::debug(_T("OldL:%d, OldT:%d, OldW:%d, OldH:%d"), rcItem.left, rcItem.top, rcItem.right - rcItem.left, rcItem.bottom - rcItem.top);

	// set bkcolor
	if(!control->HasStyle(StyleType::Inner, InnerStyle::BackgroundGradient|InnerStyle::BackgroundBrush|InnerStyle::BackgroundTheme|InnerStyle::BackgroundBmp)
		&& !control->HasStyle(StyleType::Outer, OuterStyle::TransparentParentErase))
	{
		HBRUSH hbr = control->GetBackgroundBrush();
		::FillRect(hDC, &rcItem, hbr);
	}
	else
	{
		::SendMessage(control->m_hWnd, WM_ERASEBKGND, (WPARAM)hDC, (LPARAM)this);
	}

	if(control->HasStyle(StyleType::Outer, OuterStyle::Blank) && !control->HasStyle(StyleType::Outer, OuterStyle::AlwaysNoEdge))
	{
		return;
	}

	rcItem.left += rect.left;
	rcItem.top += rect.top;
	rcItem.right -= rect.right;
	rcItem.bottom -= rect.bottom;
	RECT rectTxt;
	CopyRect(&rectTxt, &rcItem);
	::SetBkMode(hDC, TRANSPARENT);
	HFONT holdFont = (HFONT)::SelectObject(hDC, control->GetFont());

	rectTxt.left += CC_DRAWPANEL_TITLE_RECT + CC_DRAWPANEL_TITLE_SPACE;
	rectTxt.right -= CC_DRAWPANEL_TITLE_RECT + CC_DRAWPANEL_TITLE_SPACE;
	// draw icon,only exist normal icon,then others
	int nH = rectTxt.bottom - rectTxt.top;
	int nW = rectTxt.right - rectTxt.left;

	//DT_CALCRECT text
	RECT rc = {0, 0, 0, 0};
	const TCHAR* tcText = NULL;
	int len = control->HasStyle(StyleType::Outer, OuterStyle::DrawNoText) ? 0 : control->GetTextLen();
	bool isNoTitle = true;
	if(nW >= 16 && len > 0)
	{
		tcText = control->GetText();
		::DrawText(hDC, tcText, len, &rc, DT_CALCRECT);
	}

	HICON hIcon = control->HasStyle(StyleType::Outer, OuterStyle::DrawNoIcon) ? NULL : control->GetIcon(IconType::Hot);
	if(hIcon != NULL && nW >= 12 && nH >= 12)
	{
		if(rc.bottom <= 0 || (rc.bottom <= nW && rc.bottom <= nH))
		{
			if(isDisabled)
			{
				hIcon = control->GetIcon(IconType::Disabled);
			}
			//now as width of icon
			nW = rc.bottom <= 0 ? 12 : rc.bottom;
			DoDrawIcon(hDC, rectTxt.left, rectTxt.top, hIcon, nW, nW, NULL, DI_NORMAL);
			rectTxt.left += nW + CONTROL_SPACE_ICONTEXT;
			isNoTitle = false;
		}
	}

	nW = rectTxt.right - rectTxt.left;
	if(nW > 4 && len > 0)
	{
		if(rc.bottom <= nH)
		{
			rectTxt.bottom = rectTxt.top + rc.bottom;
			if(rc.right > nW)
			{
				rc.right = nW;
			}
			rectTxt.right = rectTxt.left + rc.right;
			//DWORD dwTextStyle = control->GetStyle(StyleType::OwnerDrawText);
			//DT_LEFT|DT_VCENTER|DT_END_ELLIPSIS|DT_EDITCONTROL|DT_WORDBREAK
			if(isDisabled)
			{
				OffsetRect(&rectTxt, 1, 1);
				::SetTextColor(hDC, ::GetSysColor(COLOR_BTNHIGHLIGHT));
				::DrawText(hDC, tcText, len, &rectTxt, DT_SINGLELINE);
				OffsetRect(&rectTxt, -1, -1);
				::SetTextColor(hDC, ::GetSysColor(COLOR_BTNSHADOW));
				::DrawText(hDC, tcText, len, &rectTxt, DT_SINGLELINE);
			}
			else
			{
				::SetTextColor(hDC, control->GetTextColor());
				::DrawText(hDC, tcText, len, &rectTxt, DT_SINGLELINE);
			}
			rectTxt.left += rc.right;
			isNoTitle = false;
		}
	}

	CopyRect(&rect, &rcItem);
	if(rect.bottom - rect.top > 5 && rect.right - rect.left > 2)
	{
		rect.left += 1;
		rect.right -= 1;
		rect.top += 4;
		rect.bottom -= 1;
	}
	HRGN hRgnTxt = isNoTitle ? NULL : CreateRectRgn(rcItem.left + CC_DRAWPANEL_TITLE_RECT,
		rcItem.top, rectTxt.left + CC_DRAWPANEL_TITLE_SPACE, rectTxt.bottom);
	if(cc::Theme::IsThemeActive() && !control->HasStyle(StyleType::Outer, OuterStyle::Break))
	{
		HRGN hRgn = ::CreateRoundRectRgn(rect.left, rect.top, rect.right, rect.bottom, 4, 4);
		HRGN hRgn2 = ::CreateRoundRectRgn(rect.left + 1, rect.top + 1, rect.right - 1, rect.bottom - 1, 4, 4);
		if(hRgnTxt != NULL)
		{
			::CombineRgn(hRgn, hRgn, hRgnTxt, RGN_DIFF);
		}
		::CombineRgn(hRgn, hRgn, hRgn2, RGN_DIFF);
		HBRUSH brush = ::CreateSolidBrush(DrawActorConfig::corFrameColor);
		::FrameRgn(hDC, hRgn, brush, 1, 1);
		DeleteObject(brush);
		DeleteObject(hRgn2);
		DeleteObject(hRgn);
	}
	else
	{
		HRGN hRgn = ::CreateRoundRectRgn(rcItem.left, rcItem.top, rcItem.right, rcItem.bottom, 0, 0);
		if(hRgnTxt != NULL)
		{
			::CombineRgn(hRgn, hRgn, hRgnTxt, RGN_DIFF);
		}
		ExtSelectClipRgn(hDC, hRgn, RGN_COPY);
		if(control->HasStyle(StyleType::Outer, OuterStyle::Break))
		{
			::DrawEdge(hDC, &rect, EDGE_ETCHED|BDR_SUNKENOUTER|BDR_RAISEDINNER, BF_TOP);
		}
		else
		{
			::DrawEdge(hDC, &rect, EDGE_ETCHED|BDR_SUNKENOUTER|BDR_RAISEDINNER, BF_RECT);
		}
		SelectClipRgn(hDC , NULL);
		DeleteObject(hRgn);
	}
	if(hRgnTxt != NULL)
	{
		DeleteObject(hRgnTxt);
	}
	::SelectObject(hDC, holdFont);
}

void DrawActor::DrawThemeBackground(HDC hDC, const RECT *pRect, const RECT *pClipRect)
{
	DoDrawThemeBackground(hDC, pRect, pClipRect);
}

void DrawActor::DoDrawThemeBackground(HDC hDC, const RECT *pRect, const RECT *pClipRect)
{
	//http://blog.csdn.net/nicholasmaxwell/archive/2006/09/04/1176583.aspx
	//http://msdn.microsoft.com/en-us/library/bb773210(VS.85).aspx
	//TABP_BODY:background, TABP_PANE:client
	HTHEME hTheme = cc::Theme::OpenThemeData(NULL, L"TAB");
	cc::Theme::DrawThemeBackground(hTheme, hDC, TABP_BODY, 0, pRect, pClipRect);
	cc::Theme::CloseThemeData(hTheme);
}

//cannot delete Brush out this class
HBRUSH DrawActor::ThemeBackgroundBrush(Control* control)
{
	return DoThemeBackgroundBrush(control);
}

HBRUSH DrawActor::DoThemeBackgroundBrush(Control* control)
{
	CC_ASSERT(control != NULL && control->m_hWnd != NULL);
	if(!cc::Theme::IsThemeActive())
	{
		return NULL;
	}
	//300X250; 300X450; 300X650; 300X900; MAX
	static int nMaxSize = 0;
	RECT rect;
	::GetClientRect(control->m_hWnd, &rect);
	int nInd = 0;
	if(rect.bottom <= 250)
	{
		rect.bottom = 250;
		nInd = 0;
	}
	else if(rect.bottom <= 450)
	{
		rect.bottom = 450;
		nInd = 1;
	}
	else if(rect.bottom <= 650)
	{
		rect.bottom = 650;
		nInd = 2;
	}
	else if(rect.bottom <= 900)
	{
		rect.bottom = 900;
		nInd = 3;
	}
	else
	{
		if(nMaxSize < rect.bottom)
		{
			nMaxSize = rect.bottom;
			if(hbrThemeBack[4] != NULL)
			{
				::DeleteObject(hbrThemeBack[4]);
				hbrThemeBack[4] = NULL;
			}
		}
		rect.bottom = nMaxSize;
		nInd = 4;
	}
	if(hbrThemeBack[nInd] != NULL)
	{
		//can not do next for dead loop
		//control->SetBackgroundBrush(hbrThemeBack, false);
		return hbrThemeBack[nInd];
	}
	rect.right = 300;

	HDC hDCSelf = ::GetDC(NULL);
	HDC hChaceDC = ::CreateCompatibleDC(hDCSelf);
	HBITMAP hChaceBmp = ::CreateCompatibleBitmap(hDCSelf, rect.right, rect.bottom);
	HBITMAP hChaceBmpOld = (HBITMAP)::SelectObject(hChaceDC, hChaceBmp);

	DoDrawThemeBackground(hChaceDC, &rect, NULL);

	hbrThemeBack[nInd] = ::CreatePatternBrush(hChaceBmp);
	::SelectObject(hChaceDC, hChaceBmpOld);
	::DeleteDC(hChaceDC);
	::DeleteDC(hDCSelf);
	::DeleteObject(hChaceBmp);
	return hbrThemeBack[nInd];
}
//cc::win::DrawActor
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//this is defined at cc::win::Base
//===========================================================cc::win::DrawActor
