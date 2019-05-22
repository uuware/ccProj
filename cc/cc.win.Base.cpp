// cc.win.Base.cpp
// The C# like Controls class
// Author: Shu.KK
// 2006-01-01

#include "cc.win.Base.h"
using namespace cc;
using namespace cc::win;

//===========================================================cc::win::base static function
///////////////////////////////////////////////////////////////////////////////
// for base of control

//TranstrateWindowMessage
#ifdef _DEBUG_
#include "cc.win.Message.hpp"
#endif
const TCHAR* __stdcall CC_TurnWndMsg(const TCHAR* ctlClass, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
#ifdef _DEBUG_
	return CC_TurnWndMsg2(ctlClass, uMsg, wParam, lParam);
#endif
	return _T("");
}

//LogWindowMessage
void __stdcall CC_LogWndMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, const TCHAR* ctlClass, const TCHAR* ctlName)
{
#ifdef _DEBUG_
	//if(uMsg == WM_MOUSEMOVE || uMsg == WM_NCMOUSEMOVE)
	//{
	//	RECT rc;
	//	::GetWindowRect(hWnd, &rc);
	//	HWND hWndP = ::GetParent(hWnd);
	//	if(hWndP != NULL)
	//	{
	//		::ScreenToClient(hWndP, (LPPOINT)&rc);
	//		::ScreenToClient(hWndP, ((LPPOINT)&rc)+1);
	//	}
	//	cc::Log::SetTitle(_T("MOUSEMOVE[Class:%s, Name:%s, X:%d, Y:%d, [Ctl]L:%d, T:%d, W:%d, H:%d]"),
	//		ctlClass, ctlName, LOWORD(lParam), HIWORD(lParam), rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top);
	//}
	if(uMsg != WM_MOUSEMOVE && uMsg != WM_NCHITTEST && uMsg != WM_SETCURSOR && uMsg != WM_ENTERIDLE && uMsg != WM_TIMER && uMsg != WM_NCMOUSEMOVE)
	{
		const TCHAR* tcMsg = CC_TurnWndMsg(ctlClass, uMsg, wParam, lParam);
		CC_TRACE((_T("[%s,%s]msg:%s[%d, 0x%08x]"), ctlClass, ctlName, tcMsg, uMsg, uMsg));
	}
#endif
}

const TCHAR* CC_GetShortcutText(Shortcut::Type shortcut)
{
	switch(shortcut)
	{
	case Shortcut::Alt0:
		return _T("Alt+0");
	case Shortcut::Alt1:
		return _T("Alt+1");
	case Shortcut::Alt2:
		return _T("Alt+2");
	case Shortcut::Alt3:
		return _T("Alt+3");
	case Shortcut::Alt4:
		return _T("Alt+4");
	case Shortcut::Alt5:
		return _T("Alt+5");
	case Shortcut::Alt6:
		return _T("Alt+6");
	case Shortcut::Alt7:
		return _T("Alt+7");
	case Shortcut::Alt8:
		return _T("Alt+8");
	case Shortcut::Alt9:
		return _T("Alt+9");
	case Shortcut::AltBksp:
		return _T("Alt+Bksp");
	case Shortcut::AltF1:
		return _T("Alt+F1");
	case Shortcut::AltF2:
		return _T("Alt+F2");
	case Shortcut::AltF3:
		return _T("Alt+F3");
	case Shortcut::AltF4:
		return _T("Alt+F4");
	case Shortcut::AltF5:
		return _T("Alt+F5");
	case Shortcut::AltF6:
		return _T("Alt+F6");
	case Shortcut::AltF7:
		return _T("Alt+F7");
	case Shortcut::AltF8:
		return _T("Alt+F8");
	case Shortcut::AltF9:
		return _T("Alt+F9");
	case Shortcut::AltF10:
		return _T("Alt+F10");
	case Shortcut::AltF11:
		return _T("Alt+F11");
	case Shortcut::AltF12:
		return _T("Alt+F12");
	case Shortcut::Ctrl0:
		return _T("Ctrl+0");
	case Shortcut::Ctrl1:
		return _T("Ctrl+1");
	case Shortcut::Ctrl2:
		return _T("Ctrl+2");
	case Shortcut::Ctrl3:
		return _T("Ctrl+3");
	case Shortcut::Ctrl4:
		return _T("Ctrl+4");
	case Shortcut::Ctrl5:
		return _T("Ctrl+5");
	case Shortcut::Ctrl6:
		return _T("Ctrl+6");
	case Shortcut::Ctrl7:
		return _T("Ctrl+7");
	case Shortcut::Ctrl8:
		return _T("Ctrl+8");
	case Shortcut::Ctrl9:
		return _T("Ctrl+9");
	case Shortcut::CtrlA:
		return _T("Ctrl+A");
	case Shortcut::CtrlB:
		return _T("Ctrl+B");
	case Shortcut::CtrlC:
		return _T("Ctrl+C");
	case Shortcut::CtrlD:
		return _T("Ctrl+D");
	case Shortcut::CtrlDel:
		return _T("Ctrl+Del");
	case Shortcut::CtrlE:
		return _T("Ctrl+E");
	case Shortcut::CtrlF:
		return _T("Ctrl+F");
	case Shortcut::CtrlF1:
		return _T("Ctrl+F1");
	case Shortcut::CtrlF2:
		return _T("Ctrl+F2");
	case Shortcut::CtrlF3:
		return _T("Ctrl+F3");
	case Shortcut::CtrlF4:
		return _T("Ctrl+F4");
	case Shortcut::CtrlF5:
		return _T("Ctrl+F5");
	case Shortcut::CtrlF6:
		return _T("Ctrl+F6");
	case Shortcut::CtrlF7:
		return _T("Ctrl+F7");
	case Shortcut::CtrlF8:
		return _T("Ctrl+F8");
	case Shortcut::CtrlF9:
		return _T("Ctrl+F9");
	case Shortcut::CtrlF10:
		return _T("Ctrl+F10");
	case Shortcut::CtrlF11:
		return _T("Ctrl+F11");
	case Shortcut::CtrlF12:
		return _T("Ctrl+F12");
	case Shortcut::CtrlG:
		return _T("Ctrl+G");
	case Shortcut::CtrlH:
		return _T("Ctrl+H");
	case Shortcut::CtrlI:
		return _T("Ctrl+I");
	case Shortcut::CtrlIns:
		return _T("Ctrl+Ins");
	case Shortcut::CtrlJ:
		return _T("Ctrl+J");
	case Shortcut::CtrlK:
		return _T("Ctrl+K");
	case Shortcut::CtrlL:
		return _T("Ctrl+L");
	case Shortcut::CtrlM:
		return _T("Ctrl+M");
	case Shortcut::CtrlN:
		return _T("Ctrl+N");
	case Shortcut::CtrlO:
		return _T("Ctrl+O");
	case Shortcut::CtrlP:
		return _T("Ctrl+P");
	case Shortcut::CtrlQ:
		return _T("Ctrl+Q");
	case Shortcut::CtrlR:
		return _T("Ctrl+R");
	case Shortcut::CtrlS:
		return _T("Ctrl+S");
	case Shortcut::CtrlT:
		return _T("Ctrl+T");
	case Shortcut::CtrlU:
		return _T("Ctrl+U");
	case Shortcut::CtrlV:
		return _T("Ctrl+V");
	case Shortcut::CtrlW:
		return _T("Ctrl+W");
	case Shortcut::CtrlX:
		return _T("Ctrl+X");
	case Shortcut::CtrlY:
		return _T("Ctrl+Y");
	case Shortcut::CtrlZ:
		return _T("Ctrl+Z");
	case Shortcut::CtrlShift0:
		return _T("Ctrl+Shift+0");
	case Shortcut::CtrlShift1:
		return _T("Ctrl+Shift+1");
	case Shortcut::CtrlShift2:
		return _T("Ctrl+Shift+2");
	case Shortcut::CtrlShift3:
		return _T("Ctrl+Shift+3");
	case Shortcut::CtrlShift4:
		return _T("Ctrl+Shift+4");
	case Shortcut::CtrlShift5:
		return _T("Ctrl+Shift+5");
	case Shortcut::CtrlShift6:
		return _T("Ctrl+Shift+6");
	case Shortcut::CtrlShift7:
		return _T("Ctrl+Shift+7");
	case Shortcut::CtrlShift8:
		return _T("Ctrl+Shift+8");
	case Shortcut::CtrlShift9:
		return _T("Ctrl+Shift+9");
	case Shortcut::CtrlShiftA:
		return _T("Ctrl+Shift+A");
	case Shortcut::CtrlShiftB:
		return _T("Ctrl+Shift+B");
	case Shortcut::CtrlShiftC:
		return _T("Ctrl+Shift+C");
	case Shortcut::CtrlShiftD:
		return _T("Ctrl+Shift+D");
	case Shortcut::CtrlShiftE:
		return _T("Ctrl+Shift+E");
	case Shortcut::CtrlShiftF:
		return _T("Ctrl+Shift+F");
	case Shortcut::CtrlShiftF1:
		return _T("Ctrl+Shift+F1");
	case Shortcut::CtrlShiftF2:
		return _T("Ctrl+Shift+F2");
	case Shortcut::CtrlShiftF3:
		return _T("Ctrl+Shift+F3");
	case Shortcut::CtrlShiftF4:
		return _T("Ctrl+Shift+F4");
	case Shortcut::CtrlShiftF5:
		return _T("Ctrl+Shift+F5");
	case Shortcut::CtrlShiftF6:
		return _T("Ctrl+Shift+F6");
	case Shortcut::CtrlShiftF7:
		return _T("Ctrl+Shift+F7");
	case Shortcut::CtrlShiftF8:
		return _T("Ctrl+Shift+F8");
	case Shortcut::CtrlShiftF9:
		return _T("Ctrl+Shift+F9");
	case Shortcut::CtrlShiftF10:
		return _T("Ctrl+Shift+F10");
	case Shortcut::CtrlShiftF11:
		return _T("Ctrl+Shift+F11");
	case Shortcut::CtrlShiftF12:
		return _T("Ctrl+Shift+F12");
	case Shortcut::CtrlShiftG:
		return _T("Ctrl+Shift+G");
	case Shortcut::CtrlShiftH:
		return _T("Ctrl+Shift+H");
	case Shortcut::CtrlShiftI:
		return _T("Ctrl+Shift+I");
	case Shortcut::CtrlShiftJ:
		return _T("Ctrl+Shift+J");
	case Shortcut::CtrlShiftK:
		return _T("Ctrl+Shift+K");
	case Shortcut::CtrlShiftL:
		return _T("Ctrl+Shift+L");
	case Shortcut::CtrlShiftM:
		return _T("Ctrl+Shift+M");
	case Shortcut::CtrlShiftN:
		return _T("Ctrl+Shift+N");
	case Shortcut::CtrlShiftO:
		return _T("Ctrl+Shift+O");
	case Shortcut::CtrlShiftP:
		return _T("Ctrl+Shift+P");
	case Shortcut::CtrlShiftQ:
		return _T("Ctrl+Shift+Q");
	case Shortcut::CtrlShiftR:
		return _T("Ctrl+Shift+R");
	case Shortcut::CtrlShiftS:
		return _T("Ctrl+Shift+S");
	case Shortcut::CtrlShiftT:
		return _T("Ctrl+Shift+T");
	case Shortcut::CtrlShiftU:
		return _T("Ctrl+Shift+U");
	case Shortcut::CtrlShiftV:
		return _T("Ctrl+Shift+V");
	case Shortcut::CtrlShiftW:
		return _T("Ctrl+Shift+W");
	case Shortcut::CtrlShiftX:
		return _T("Ctrl+Shift+X");
	case Shortcut::CtrlShiftY:
		return _T("Ctrl+Shift+Y");
	case Shortcut::CtrlShiftZ:
		return _T("Ctrl+Shift+Z");
	case Shortcut::Del:
		return _T("Del");
	case Shortcut::F1:
		return _T("F1");
	case Shortcut::F2:
		return _T("F2");
	case Shortcut::F3:
		return _T("F3");
	case Shortcut::F4:
		return _T("F4");
	case Shortcut::F5:
		return _T("F5");
	case Shortcut::F6:
		return _T("F6");
	case Shortcut::F7:
		return _T("F7");
	case Shortcut::F8:
		return _T("F8");
	case Shortcut::F9:
		return _T("F9");
	case Shortcut::F10:
		return _T("F10");
	case Shortcut::F11:
		return _T("F11");
	case Shortcut::F12:
		return _T("F12");
	case Shortcut::Ins:
		return _T("Ins");
	case Shortcut::None:
		return NULL;
	case Shortcut::ShiftDel:
		return _T("Shift+Del");
	case Shortcut::ShiftF1:
		return _T("Shift+F1");
	case Shortcut::ShiftF2:
		return _T("Shift+F2");
	case Shortcut::ShiftF3:
		return _T("Shift+F3");
	case Shortcut::ShiftF4:
		return _T("Shift+F4");
	case Shortcut::ShiftF5:
		return _T("Shift+F5");
	case Shortcut::ShiftF6:
		return _T("Shift+F6");
	case Shortcut::ShiftF7:
		return _T("Shift+F7");
	case Shortcut::ShiftF8:
		return _T("Shift+F8");
	case Shortcut::ShiftF9:
		return _T("Shift+F9");
	case Shortcut::ShiftF10:
		return _T("Shift+F10");
	case Shortcut::ShiftF11:
		return _T("Shift+F11");
	case Shortcut::ShiftF12:
		return _T("Shift+F12");
	case Shortcut::ShiftIns:
		return _T("Shift+Ins");
	}
	return NULL;
}

long __stdcall CC_InitCommonControls(HINSTANCE hInst, LPINITCOMMONCONTROLSEX lpInitCtrls, LONG fToRegister)
{
	LONG lResult = 0;
	// attempt to get/call InitCommonControlsEx
	BOOL (STDAPICALLTYPE* pfnInit)(LPINITCOMMONCONTROLSEX lpInitCtrls) = NULL;
	(FARPROC&)pfnInit = ::GetProcAddress(hInst, "InitCommonControlsEx");
	if(pfnInit == NULL)
	{
		// not there, so call InitCommonControls if possible
		if((fToRegister & CC_WIN95CTLS_MASK) == fToRegister)
		{
			InitCommonControls();
			lResult = CC_WIN95CTLS_MASK;
		}
	}
	else if(InitCommonControlsEx(lpInitCtrls))
	{
		// InitCommonControlsEx was successful so return the full mask
		lResult = fToRegister;
	}
	return lResult;
}

bool __stdcall CC_DeferRegisterClass(long fToRegister)
{
	// mask off all classes that are already registered
	//_APP_THREAD_STATE* pModuleState = App::GetThreadStateData();
	_APP_THREAD_STATE* pModuleState = CC_APP_GetThreadStateData();
	fToRegister &= ~pModuleState->m_fRegisteredClasses;
	if(fToRegister == 0)
	{
		return true;
	}

	HINSTANCE hInst = pModuleState->m_hInstComCtl;
	if(hInst == NULL)
	{
		//load the COMCTL32.DLL library because it may not be loaded yet (delayload)
		hInst = ::LoadLibraryA("COMCTL32.DLL");
		//will be freed the library reference at App
		if(hInst == NULL)
		{
			return 0;
		}
	}
	pModuleState->m_hInstComCtl = hInst;

	LONG fRegisteredClasses = 0;
	// common initialization
	INITCOMMONCONTROLSEX init;
	init.dwSize = sizeof(init);

	// work to register classes as specified by fToRegister, populate fRegisteredClasses as we go
	if(fToRegister & CC_WNDCOMMCTLS_REG)
	{
		// this flag is compatible with the old InitCommonControls() API
		init.dwICC = ICC_WIN95_CLASSES;
		fRegisteredClasses |= CC_InitCommonControls(hInst, &init, CC_WIN95CTLS_MASK);
		fToRegister &= ~CC_WIN95CTLS_MASK;
	}
	if(fToRegister & CC_WNDCOMMCTL_UPDOWN_REG)
	{
		init.dwICC = ICC_UPDOWN_CLASS;
		fRegisteredClasses |= CC_InitCommonControls(hInst, &init, CC_WNDCOMMCTL_UPDOWN_REG);
	}
	if(fToRegister & CC_WNDCOMMCTL_TREEVIEW_REG)
	{
		init.dwICC = ICC_TREEVIEW_CLASSES;
		fRegisteredClasses |= CC_InitCommonControls(hInst, &init, CC_WNDCOMMCTL_TREEVIEW_REG);
	}
	if(fToRegister & CC_WNDCOMMCTL_TAB_REG)
	{
		init.dwICC = ICC_TAB_CLASSES;
		fRegisteredClasses |= CC_InitCommonControls(hInst, &init, CC_WNDCOMMCTL_TAB_REG);
	}
	if(fToRegister & CC_WNDCOMMCTL_PROGRESS_REG)
	{
		init.dwICC = ICC_PROGRESS_CLASS;
		fRegisteredClasses |= CC_InitCommonControls(hInst, &init, CC_WNDCOMMCTL_PROGRESS_REG);
	}
	if(fToRegister & CC_WNDCOMMCTL_LISTVIEW_REG)
	{
		init.dwICC = ICC_LISTVIEW_CLASSES;
		fRegisteredClasses |= CC_InitCommonControls(hInst, &init, CC_WNDCOMMCTL_LISTVIEW_REG);
	}
	if(fToRegister & CC_WNDCOMMCTL_HOTKEY_REG)
	{
		init.dwICC = ICC_HOTKEY_CLASS;
		fRegisteredClasses |= CC_InitCommonControls(hInst, &init, CC_WNDCOMMCTL_HOTKEY_REG);
	}
	if (fToRegister & CC_WNDCOMMCTL_BAR_REG)
	{
		init.dwICC = ICC_BAR_CLASSES;
		fRegisteredClasses |= CC_InitCommonControls(hInst, &init, CC_WNDCOMMCTL_BAR_REG);
	}
	if(fToRegister & CC_WNDCOMMCTL_ANIMATE_REG)
	{
		init.dwICC = ICC_ANIMATE_CLASS;
		fRegisteredClasses |= CC_InitCommonControls(hInst, &init, CC_WNDCOMMCTL_ANIMATE_REG);
	}
	if(fToRegister & CC_WNDCOMMCTL_INTERNET_REG)
	{
		init.dwICC = ICC_INTERNET_CLASSES;
		fRegisteredClasses |= CC_InitCommonControls(hInst, &init, CC_WNDCOMMCTL_INTERNET_REG);
	}
	if(fToRegister & CC_WNDCOMMCTL_COOL_REG)
	{
		init.dwICC = ICC_COOL_CLASSES;
		fRegisteredClasses |= CC_InitCommonControls(hInst, &init, CC_WNDCOMMCTL_COOL_REG);
	}
	if(fToRegister & CC_WNDCOMMCTL_USEREX_REG)
	{
		init.dwICC = ICC_USEREX_CLASSES;
		fRegisteredClasses |= CC_InitCommonControls(hInst, &init, CC_WNDCOMMCTL_USEREX_REG);
	}
	if(fToRegister & CC_WNDCOMMCTL_DATE_REG)
	{
		init.dwICC = ICC_DATE_CLASSES;
		fRegisteredClasses |= CC_InitCommonControls(hInst, &init, CC_WNDCOMMCTL_DATE_REG);
	}
	if(fToRegister & CC_WNDCOMMCTL_LINK_REG)
	{
		//#if (_WIN32_WINNT >= 0x501)
		//#define ICC_LINK_CLASS         0x00008000
		//#endif
		init.dwICC = 0x00008000;
		fRegisteredClasses |= CC_InitCommonControls(hInst, &init, CC_WNDCOMMCTL_LINK_REG);
	}

	// save new state of registered controls
	pModuleState->m_fRegisteredClasses |= fRegisteredClasses;

	// special case for all common controls registered, turn on CC_WNDCOMMCTLS_REG
	if((pModuleState->m_fRegisteredClasses & CC_WIN95CTLS_MASK) == CC_WIN95CTLS_MASK)
	{
		pModuleState->m_fRegisteredClasses |= CC_WNDCOMMCTLS_REG;
		fRegisteredClasses |= CC_WNDCOMMCTLS_REG;
	}

	//if first registered error,then try it again,to invoid it,save doing state(include those that fatal registered)
	pModuleState->m_fRegisteredClasses |= fToRegister;

	// must have registered at least as mamy classes as requested
	return (fToRegister & fRegisteredClasses) == fToRegister;
}

int _cc_DropDownWidth = -1;
int __stdcall CC_GetDropDownWidth()
{
	// return cached version if already determined...
	if (_cc_DropDownWidth != -1)
		return _cc_DropDownWidth;

	// otherwise calculate it...
	HDC hDC = ::GetDC(NULL);
	HFONT hFont = NULL, hFontOld = NULL;
	if ((hFont = ::CreateFont(::GetSystemMetrics(SM_CYMENUCHECK), 0, 0, 0, 
		FW_NORMAL, 0, 0, 0, SYMBOL_CHARSET, 0, 0, 0, 0, _T("Marlett"))) != NULL)
		hFontOld = (HFONT)SelectObject(hDC, hFont);
	::GetCharWidth(hDC, '6', '6', &_cc_DropDownWidth);
	if (hFont != NULL)
	{
		SelectObject(hDC, hFontOld);
		DeleteObject(hFont);
	}
	ReleaseDC(NULL, hDC);
	return _cc_DropDownWidth;
}
// 
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//===========================================================cc::win::ImageList
ImageList::ImageList()
{
	_mapIcon = NULL;
	_sizeIcon.cx = -1;
	_sizeIcon.cy = -1;
}

ImageList::~ImageList()
{
	Clear();
}

SIZE ImageList::GetIconSize()
{
	return _sizeIcon;
}

//if set any of to -1,then no resize
void ImageList::SetIconSize(int cx, int cy)
{
	if(_mapIcon != NULL && cx > 0 && cy > 0 && (_sizeIcon.cx != cx || _sizeIcon.cy != cy))
	{
		for(int i = _mapIcon->GetSize() - 1; i >= 0; i--)
		{
			//change size
			IconInfo* iconInfo = (IconInfo*)_mapIcon->GetAt(i);
			HICON icon = CC_APP_GetDrawActor()->ResizeIcon(iconInfo->Hot, cx, cy);
			::DestroyIcon(iconInfo->Hot);
			iconInfo->Hot = icon;
			if(iconInfo->Gloom != NULL)
			{
				icon = CC_APP_GetDrawActor()->ResizeIcon(iconInfo->Gloom, cx, cy);
				::DestroyIcon(iconInfo->Gloom);
				iconInfo->Gloom = icon;
			}
			if(iconInfo->Disabled != NULL)
			{
				icon = CC_APP_GetDrawActor()->ResizeIcon(iconInfo->Disabled, cx, cy);
				::DestroyIcon(iconInfo->Disabled);
				iconInfo->Disabled = icon;
			}
			if(iconInfo->Pushed != NULL)
			{
				icon = CC_APP_GetDrawActor()->ResizeIcon(iconInfo->Pushed, cx, cy);
				::DestroyIcon(iconInfo->Pushed);
				iconInfo->Pushed = icon;
			}
			_mapIcon->SetValueAt(i, (void*)iconInfo);
		}
	}
	_sizeIcon.cx = cx;
	_sizeIcon.cy = cy;
}

int ImageList::SetIcon(int nID, UINT nResourceID, IconType::Type iconType)
{
	//hIcon = (HICON)LoadImage(NULL, "1.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
	HICON hIcon = ::LoadIcon(CC_APP_GetInstance(), MAKEINTRESOURCE(nResourceID));
	return SetIcon(nID, hIcon, iconType);
}

//return index of add
//for icon of Grey&Disabled can be created by self from Hot,so if set Hot,then need set NULL to Grey&Disabled
int ImageList::SetIcon(int nID, HICON hIcon, IconType::Type iconType)
{
	if(_mapIcon == NULL)
	{
		//Map(bool isSort, bool bIsDESC = false, bool isDistinct = false, int nMinCapacity = 0, double nIncrement = 1.3);
		_mapIcon = new Map<int, void*>(true, false, true);
	}
	if(_sizeIcon.cx > 0 && _sizeIcon.cy > 0)
	{
		//change size and destroy orgial icon
		HICON hIcon2 = hIcon;
		hIcon = CC_APP_GetDrawActor()->ResizeIcon(hIcon2, _sizeIcon.cx, _sizeIcon.cy);
		::DestroyIcon(hIcon2);
	}
	IconInfo* iconInfo = NULL;
	int nIndex = _mapIcon->GetIndex(nID);
	if(nIndex < 0)
	{
		iconInfo = new IconInfo;
		//for newly,set hIcon to Hot,if iconType is Not Hot
		if(iconType != IconType::Hot)
		{
			iconInfo->Hot = hIcon;
		}
		nIndex = _mapIcon->Add(nID, iconInfo);
	}
	else
	{
		iconInfo = (IconInfo*)_mapIcon->GetAt(nIndex);
	}
	switch(iconType)
	{
	case IconType::Hot:
		if(iconInfo->Hot != NULL)
		{
			::DestroyIcon(iconInfo->Hot);
		}
		iconInfo->Hot = hIcon;
		break;
	case IconType::Gloom:
		if(iconInfo->Gloom != NULL)
		{
			::DestroyIcon(iconInfo->Gloom);
		}
		iconInfo->Gloom = hIcon;
		break;
	case IconType::Disabled:
		if(iconInfo->Disabled != NULL)
		{
			::DestroyIcon(iconInfo->Disabled);
		}
		iconInfo->Disabled = hIcon;
		break;
	case IconType::Pushed:
		if(iconInfo->Pushed != NULL)
		{
			::DestroyIcon(iconInfo->Pushed);
		}
		iconInfo->Pushed = hIcon;
		break;
	}
	return nIndex;
}

HICON ImageList::GetIcon(int nID, IconType::Type iconType)
{
	int nIndex;
	if(_mapIcon != NULL && (nIndex = _mapIcon->GetIndex(nID)) >= 0)
	{
		IconInfo* iconInfo = (IconInfo*)_mapIcon->GetAt(nIndex);
		switch(iconType)
		{
		case IconType::Hot:
			return iconInfo->Hot;
		case IconType::Gloom:
			if(iconInfo->Gloom == NULL && iconInfo->Hot != NULL)
			{
				iconInfo->Gloom = CC_APP_GetDrawActor()->CreateGloomIcon(iconInfo->Hot);
			}
			return iconInfo->Gloom;
		case IconType::Disabled:
			if(iconInfo->Disabled == NULL && iconInfo->Hot != NULL)
			{
				iconInfo->Disabled = CC_APP_GetDrawActor()->CreateDisabledIcon(iconInfo->Hot);
			}
			return iconInfo->Disabled;
		case IconType::Pushed:
			if(iconInfo->Pushed == NULL)
			{
				return iconInfo->Hot;
			}
			return iconInfo->Pushed;
		}
	}
	return NULL;
}

// need destroy it out this class
HICON ImageList::GetIconCopy(int nID, IconType::Type iconType)
{
	HICON icon = GetIcon(nID, iconType);
	if(icon != NULL)
	{
		return ::CopyIcon(icon);
	}
	return NULL;
}

HICON ImageList::GetIconAt(int nIndex, IconType::Type iconType)
{
	if(_mapIcon != NULL && nIndex >= 0 && nIndex < _mapIcon->GetSize())
	{
		IconInfo* iconInfo = (IconInfo*)_mapIcon->GetAt(nIndex);
		switch(iconType)
		{
		case IconType::Hot:
			return iconInfo->Hot;
		case IconType::Gloom:
			if(iconInfo->Gloom == NULL && iconInfo->Hot != NULL)
			{
				iconInfo->Gloom = CC_APP_GetDrawActor()->CreateGloomIcon(iconInfo->Hot);
			}
			return iconInfo->Gloom;
		case IconType::Disabled:
			if(iconInfo->Disabled == NULL && iconInfo->Hot != NULL)
			{
				iconInfo->Disabled = CC_APP_GetDrawActor()->CreateDisabledIcon(iconInfo->Hot);
			}
			return iconInfo->Disabled;
		case IconType::Pushed:
			if(iconInfo->Pushed == NULL)
			{
				return iconInfo->Hot;
			}
			return iconInfo->Pushed;
		}
	}
	return NULL;
}

// need destroy it out this class
HICON ImageList::GetIconCopyAt(int nIndex, IconType::Type iconType)
{
	HICON icon = GetIconAt(nIndex, iconType);
	if(icon != NULL)
	{
		return ::CopyIcon(icon);
	}
	return NULL;
}

int ImageList::Count()
{
	return (_mapIcon == NULL ? 0 : _mapIcon->GetSize());
}

void ImageList::Remove(int nID)
{
	if(_mapIcon != NULL && _mapIcon->ContainsKey(nID))
	{
		IconInfo* iconInfo = (IconInfo*)_mapIcon->Get(nID);
		//destroyicon at ~IconInfo
		delete iconInfo;
		//::DestroyIcon((HICON)_mapIcon->Get(nID));
		_mapIcon->Remove(nID);
	}
}

void ImageList::Clear()
{
	if(_mapIcon != NULL)
	{
		for(int i = _mapIcon->GetSize() - 1; i >= 0; i--)
		{
			IconInfo* iconInfo = (IconInfo*)_mapIcon->GetAt(i);
			//destroyicon at ~IconInfo
			delete iconInfo;
		}
		_mapIcon->Clear();
		delete _mapIcon;
		_mapIcon = NULL;
	}
}

//need release point!
//ImageList_Destroy(hImageList);
HIMAGELIST ImageList::CreateHIMAGELIST(int cx, int cy, IconType::Type iconType, DWORD mask)
{
	HIMAGELIST handle = ImageList_Create(cx, cy, mask, 0, 1);
	if(_mapIcon != NULL)
	{
		int nCnt = _mapIcon->GetSize();
		for(int i = 0; i < nCnt; i++)
		{
			ImageList_ReplaceIcon(handle, -1, GetIconAt(i, iconType));
		}
	}
	return handle;
}

//need release point!
ImageList* ImageList::FromHandle(HIMAGELIST hImage)
{
	if(hImage == NULL)
	{
		return NULL;
	}
	int cx;
	int cy;
	ImageList_GetIconSize(hImage, &cx, &cy);
	ImageList* hImageList = new ImageList();
	hImageList->SetIconSize(cx, cy);
	int nSize = ImageList_GetImageCount(hImage);
	for(int i = 0; i < nSize; i++)
	{
		HICON hIcon = ImageList_ExtractIcon(NULL, hImage, i);
		hImageList->SetIcon(i, hIcon);
	}
	return hImageList;
}

int ImageList::GetIndex(int nID)
{
	return (_mapIcon == NULL ? -1 : _mapIcon->GetIndex(nID));
}
//===========================================================cc::win::ImageList
///////////////////////////////////////////////////////////////////////////////

//===========================================================cc::win::ITextIcon
///////////////////////////////////////////////////////////////////////////////
// constructors/destructor
ITextIcon::ITextIcon()
{
	_Name = NULL;
	_Text = NULL;
	_Tips = NULL;
	_TipsExt = NULL;
	_nIconID = 0;
	_nTipsIconID = 0;
	_nTipsExtIconID = 0;
	_pImageList = NULL;
	_nUpdateFlag = 0;
	_ConfigName = NULL;
	_Flags = 0;
}

ITextIcon::~ITextIcon()
{
	if(_Name != NULL)
	{
		free(_Name);
		_Name = NULL;
	}
	if(_Text != NULL)
	{
		free(_Text);
		_Text = NULL;
	}
	if(_Tips != NULL)
	{
		free(_Tips);
		_Tips = NULL;
	}
	if(_TipsExt != NULL)
	{
		free(_TipsExt);
		_TipsExt = NULL;
	}
	if(_ConfigName != NULL)
	{
		free(_ConfigName);
		_ConfigName = NULL;
	}
}

const TCHAR* ITextIcon::GetName() const
{
	return _Name;
}

void ITextIcon::SetName(const TCHAR* sName)
{
	_Name = CC_StrCopy(&_Name, sName);
}

const TCHAR* ITextIcon::GetText()
{
	return _Text;
}

//notice,control is not call this
void ITextIcon::SetText(const TCHAR* sText)
{
	_Text = CC_StrCopy(&_Text, sText);
	_nUpdateFlag |= UpdateFlag::UpdateText;
	Update();
}

ImageList* ITextIcon::GetImageList()
{
	return _pImageList;
}

int ITextIcon::GetIconID()
{
	return _nIconID;
}

HICON ITextIcon::GetIcon(IconType::Type iconType)
{
	if(_pImageList != NULL && _nIconID > 0)
	{
		return _pImageList->GetIcon(_nIconID, iconType);
	}
	return NULL;
}

void ITextIcon::SetIcon(ImageList* imageList, int nIconID)
{
	if(imageList != NULL)
	{
		_pImageList = imageList;
	}
	_nIconID = nIconID;
	_nUpdateFlag |= UpdateFlag::UpdateIcon;
	Update();
}

const TCHAR* ITextIcon::GetTips()
{
	return _Tips;
}

int ITextIcon::GetTipsIconID()
{
	return _nTipsIconID;
}

//the ImageList is share with SetIcon
void ITextIcon::SetTips(const TCHAR* sTips, ImageList* imageList, int nIconID)
{
	if(sTips != NULL && _tcscmp(sTips, _T("")) == 0)
	{
		sTips = NULL;
	}
	_Tips = CC_StrCopy(&_Tips, sTips);
	if(imageList != NULL)
	{
		_pImageList = imageList;
	}
	_nTipsIconID = nIconID;
	_nUpdateFlag |= UpdateFlag::UpdateTips;
	Update();
}

//if _TipsExt is NULL then return _Tips
const TCHAR* ITextIcon::GetTipsExt()
{
	if(_TipsExt == NULL)
	{
		return GetTips();
	}
	return _TipsExt;
}

//if _nTipsExtIconID is <1 then return _nTipsIconID
int ITextIcon::GetTipsExtIconID()
{
	if(_nTipsExtIconID < 1)
	{
		return _nTipsIconID;
	}
	return _nTipsExtIconID;
}

//the ImageList is share with SetIcon
void ITextIcon::SetTipsExt(const TCHAR* sTips, ImageList* imageList, int nIconID)
{
	if(sTips != NULL && _tcscmp(sTips, _T("")) == 0)
	{
		sTips = NULL;
	}
	_TipsExt = CC_StrCopy(&_TipsExt, sTips);
	if(imageList != NULL)
	{
		_pImageList = imageList;
	}
	_nTipsExtIconID = nIconID;
	_nUpdateFlag |= UpdateFlag::UpdateTipsExt;
	Update();
}

const TCHAR* ITextIcon::GetConfigName() const
{
	return _ConfigName;
}

void ITextIcon::SetConfigName(const TCHAR* sName)
{
	_ConfigName = CC_StrCopy(&_ConfigName, sName);
}

bool ITextIcon::IsUpdate(UpdateFlag::Type updateFlag)
{
	//must is != 0
	return ((_nUpdateFlag & updateFlag) != 0);
}

TCHAR ITextIcon::GetMnemonic()
{
	if(_Text != NULL)
	{
		TCHAR* tc = _tcschr(_Text, '&');
		if(tc != NULL)
		{
			tc++;
			if(*tc != '\0' && *tc >= 'a' && *tc <= 'z' )
			{
				return (*tc - ('a' - 'A'));
			}
			else
			{
				return *tc;
			}
		}
	}
	return _T('\0');
}

void ITextIcon::Update(UpdateFlag::Type updateFlag)
{
	_nUpdateFlag |= updateFlag;
	DoUpdate();
}

void ITextIcon::DoUpdate()
{
	_nUpdateFlag = 0;
}

//for can not do to child,need inherit this,also need notice that sParentHead is param by value
//keyArr is param address,but sHead is by value
void ITextIcon::DoSetLang(KeyArr& keyArr, cc::Str sThisHead)
{
	//if not exist,as usually
	int nInd;
	if((nInd = keyArr.GetIndex(sThisHead)) >= 0)
	{
		SetText(keyArr.GetAt(nInd));
	}
	if((nInd = keyArr.GetIndex(sThisHead.Append(_T("Tips")))) >= 0)
	{
		_Tips = CC_StrCopy(&_Tips, (TCHAR*)keyArr.GetAt(nInd).GetBuf());
		_nUpdateFlag |= UpdateFlag::UpdateTips;
		if((nInd = keyArr.GetIndex(sThisHead.Append(_T("Ext")))) >= 0)
		{
			_TipsExt = CC_StrCopy(&_TipsExt, (TCHAR*)keyArr.GetAt(nInd).GetBuf());
			_nUpdateFlag |= UpdateFlag::UpdateTipsExt;
		}
	}
	if(_nUpdateFlag != 0)
	{
		Update();
	}
}

void ITextIcon::SetLang(KeyArr& keyArr, cc::Str sParentHead)
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
	DoSetLang(keyArr, sParentHead);
}
// constructors/destructor
///////////////////////////////////////////////////////////////////////////////

//===========================================================cc::win::Menu
///////////////////////////////////////////////////////////////////////////////
// constructors/destructor
//menu of c#:
//if set contextmenu,then right click to show it,and if this control is activating
//  then ShortCut of this contextmenu is valid
//  mainmenu's shortcut is valid all time while without showing popup menu
//if one popup menu(also notifyIcon) is showing,then AltKey is only valid in this popup menu
//ShortCut is not valid at notifyIcon
//
Menu::Menu()
{
	m_hMenu = NULL;
	_lstMenus = NULL;
	//_bNeedCreate = true;
	CC_BIT_ON(_Flags, CC_FLAG_MENU_NEEDCREATED);
	_eMenuType = MenuType::MENUITEM;
	_uMenuStyle = MF_STRING | MF_ENABLED;
	_InnerStyle = 0;
	_eShortcut = Shortcut::None;
	_Parent = NULL;
	SetInnerStyle(MenuInnerStyle::ShowShortcut, true);
#ifdef _DEBUG_
	SetName(_T("Menu"));
#endif
}

Menu::~Menu()
{
	DestroyMenu();

	//now release memory
	if(_lstMenus != NULL)
	{
		for(int n = _lstMenus->GetSize() - 1; n >= 0; n--)
		{
			Menu* menu = _lstMenus->GetKey(n);
			delete menu;
		}
		_lstMenus->Clear();
		delete _lstMenus;
		_lstMenus = NULL;
	}
}

bool Menu::GetShowShortcut()
{
	return IsInnerStyle(MenuInnerStyle::ShowShortcut);
}

void Menu::SetShowShortcut(bool value)
{
	SetInnerStyle(MenuInnerStyle::ShowShortcut, value, true);
	_nUpdateFlag |= UpdateFlag::UpdateShortcut;
	Update();
}

const TCHAR* Menu::GetShortcutText()
{
	return CC_GetShortcutText(_eShortcut);
}

Shortcut::Type Menu::GetShortcut()
{
	return _eShortcut;
}

void Menu::SetShortcut(Shortcut::Type shortcut)
{
	_eShortcut = shortcut;
	_nUpdateFlag |= UpdateFlag::UpdateShortcut;
	Update();
}

MenuType::Type Menu::GetMenuType()
{
	return _eMenuType;
}

void Menu::DoUpdate()
{
	if(IsUpdate((UpdateFlag::Type)(UpdateFlag::UpdateText|UpdateFlag::UpdateIcon|UpdateFlag::UpdateShortcut)))
	{
		CC_BIT_ON(_Flags, CC_FLAG_MENU_NEEDCREATED);
		//_bNeedCreate = true;
		if(IsUpdate(UpdateFlag::UpdateText))
		{
			if(_Text != NULL && _tcscmp(_Text, _T("-")) == 0)
			{
				this->_uMenuStyle |= MF_SEPARATOR;
			}
			else
			{
				this->_uMenuStyle &= ~MF_SEPARATOR;
			}
		}
	}
	ITextIcon::DoUpdate();
	/*
	if(_Parent != NULL && _Parent->m_hMenu != NULL)
	{
		//had created
		MENUITEMINFO info = {0};
		info.cbSize = sizeof(MENUITEMINFO);
		info.fMask = MIIM_TYPE | MIIM_CHECKMARKS;
		if(!::GetMenuItemInfo(_Parent->m_hMenu, _Parent->_lstMenus->GetIndex(this), FALSE, &info))
		{
			return;
		}
		info.fState
		::SetMenuItemInfo(_Parent->m_hMenu, _Parent->_lstMenus->GetIndex(this), FALSE, &info);
	}
	*/
}

bool Menu::GetIsNeedUpdate()
{
	if(CC_BIT_HAS(_Flags, CC_FLAG_MENU_NEEDCREATED))
	{
		return true;
	}

	if(_lstMenus != NULL && _lstMenus->GetSize() > 0)
	{
		for(int n = _lstMenus->GetSize() - 1; n >= 0; n--)
		{
			Menu* menu = _lstMenus->GetKey(n);
			if(menu->GetIsNeedUpdate())
			{
				return true;
			}
		}
	}
	return false;
}

Menu* PASCAL Menu::FromHandle(HMENU hMenu)
{
	Map<HMENU, Menu*>* mapMenus = CC_APP_GetMapHMENU();
	if(mapMenus->ContainsKey(hMenu))
	{
		return mapMenus->Get(hMenu);
	}
	else
	{
		return NULL;
	}
}

ContextMenu* Menu::GetContextMenu() const
{
	const Menu* menu = this;
	while(menu != NULL && !(menu->_eMenuType == MenuType::CONTEXTMENU))
	{
		if(menu->_eMenuType == MenuType::MENUITEM)
		{
			menu = menu->_Parent;
		}
		else
		{
			return NULL;
		}
	}
	return (ContextMenu*)menu;
}

MainMenu* Menu::GetMainMenu() const
{
	const Menu* menu = this;
	while(menu != NULL && !(menu->_eMenuType == MenuType::MAINMENU))
	{
		if(menu->_eMenuType == MenuType::MENUITEM)
		{
			menu = menu->_Parent;
		}
		else
		{
			return NULL;
		}
	}
	return (MainMenu*)menu;
}

void Menu::ItemSwap(int fromIndex, int toIndex)
{
	int nCapacity = ItemCount();
	if(fromIndex < 0 || fromIndex > nCapacity || toIndex < 0 || toIndex > nCapacity)
	{
 		throw ArgumentException(_T("Menu::Swap(int fromIndex, int toIndex), ERROR for out of Capacity."));
	}
	if(fromIndex != toIndex)
	{
		_lstMenus->Swap(fromIndex, toIndex);
		//_bNeedCreate = true;
		CC_BIT_ON(_Flags, CC_FLAG_MENU_NEEDCREATED);
	}
}

int Menu::ItemCount() const
{
	if(_lstMenus != NULL)
	{
		return _lstMenus->GetSize();
	}
	else
	{
		return 0;
	}
}

void Menu::ItemClear()
{
	if(_lstMenus != NULL)
	{
		for(int i = _lstMenus->GetSize() - 1; i >= 0; i--)
		{
			delete _lstMenus->GetKey(i);
		}
		_lstMenus->Clear();
		_lstMenus = NULL;
		//_bNeedCreate = true;
		CC_BIT_ON(_Flags, CC_FLAG_MENU_NEEDCREATED);
	}
}

int Menu::ItemAdd(Menu* menu, int nIndex)
{
	if(_lstMenus == NULL)
	{
		//List(bool isSort, bool bIsDESC = false, bool isDistinct = false, int nMinCapacity = 0, double nIncrement = 1.3);
		_lstMenus = new List<Menu*>(false, false, true);
	}
	int nReal = _lstMenus->GetIndex(menu);
	if(nReal >= 0)
	{
		if(nReal == nIndex)
		{
			return nReal;
		}
		_lstMenus->Remove(menu);
	}
	if(nIndex < 0)
	{
		nIndex = _lstMenus->GetSize();
	}
	menu->_Parent = this;
	_lstMenus->Insert(nIndex, menu);
	//_bNeedCreate = true;
	CC_BIT_ON(_Flags, CC_FLAG_MENU_NEEDCREATED);
	return nIndex;
}

Menu* Menu::ItemRemove(int nIndex)
{
	//if(_lstMenus == NULL)
	//{
	//	throw;
	//}
	Menu* menu = ItemGet(nIndex);
	if(menu != NULL)
	{
		if(m_hMenu != NULL)
		{
			::DeleteMenu(m_hMenu, nIndex, MF_BYPOSITION);
			menu->DestroyMenu();
		}
		_lstMenus->RemoveAt(nIndex);
		//_bNeedCreate = true;
		CC_BIT_ON(_Flags, CC_FLAG_MENU_NEEDCREATED);
	}
	return menu;
}

Menu* Menu::ItemGet(int nIndex) const
{
	if(_lstMenus == NULL || nIndex < 0 || nIndex >= _lstMenus->GetSize())
	{
		return NULL;
	}
	return _lstMenus->GetKey(nIndex);
}

int Menu::ItemGetIndex(Menu* menu) const
{
	if(_lstMenus != NULL)
	{
		return _lstMenus->GetIndex(menu);
	}
	return -1;
}

void Menu::ItemSetIndex(Menu* menu, int index)
{
	if(_lstMenus == NULL || index < 0 || index >= _lstMenus->GetSize() && !_lstMenus->ContainsKey(menu))
	{
		return;
	}
	_lstMenus->Remove(menu);
	_lstMenus->Insert(index, menu);
	//_bNeedCreate = true;
	CC_BIT_ON(_Flags, CC_FLAG_MENU_NEEDCREATED);
}

bool Menu::GetIsParent() const
{
	return (_lstMenus != NULL && _lstMenus->GetSize() > 0);
}

void Menu::CreateMenuHandle()
{
	//see if recreate menu
	if((_eMenuType == MenuType::MAINMENU || _eMenuType == MenuType::CONTEXTMENU) && !GetIsNeedUpdate())
	{
		return;
	}

	if(m_hMenu != NULL)
	{
		//only destory menu but not delete include item!
		DestroyMenu();
		m_hMenu = NULL;
	}

	//creat menu
	if(_lstMenus != NULL && _lstMenus->GetSize() > 0)
	{
		//treate AllOwnerDraw
		bool isOwnerDraw = false;
		if(_Parent != NULL)
		{
			SetInnerStyle(MenuInnerStyle::AllOwnerDraw, false);
			if(_Parent->IsInnerStyle(MenuInnerStyle::AllOwnerDraw))
			{
				SetInnerStyle(MenuInnerStyle::AllOwnerDraw, true);
				isOwnerDraw = true;
			}
		}
		else
		{
			isOwnerDraw = IsInnerStyle(MenuInnerStyle::AllOwnerDraw);
		}

		//here creat HMENU
		if(_eMenuType == MenuType::MAINMENU)
		{
			//for form's mainmenu
			m_hMenu = ::CreateMenu();
			isOwnerDraw = false;
		}
		else
		{
			//for context menu
			m_hMenu = ::CreatePopupMenu();
		}
		//save handel
		Map<HMENU, Menu*>* mapMenus = CC_APP_GetMapHMENU();
		mapMenus->Add(m_hMenu, this);

		int nMenuCnt = _lstMenus->GetSize();
		for(int n = 0; n < nMenuCnt; n++)
		{
			//here must be menuitem
			MenuItem* item = (MenuItem*)_lstMenus->GetKey(n);
			if(!item->GetVisible())
			{
				//if not visible,not do it
				continue;
			}

			item->CreateMenuHandle();
			if(item->m_hMenu != NULL)
			{
				//here is submenu
				item->_uMenuStyle = MF_POPUP | (isOwnerDraw ? MenuStyles::OwnerDraw : 0);
				::AppendMenu(m_hMenu, item->_uMenuStyle, (UINT_PTR)item->m_hMenu, item->GetText());
			}
			else
			{
				//just menuitem
				const TCHAR* tc = item->GetText();
				if(tc != NULL && _tcscmp(tc, _T("-")) == 0)
				{
					item->_uMenuStyle = MF_SEPARATOR | (isOwnerDraw ? MenuStyles::OwnerDraw : 0);
					::AppendMenu(m_hMenu, item->_uMenuStyle, n, tc);
				}
				else
				{
					item->_uMenuStyle |= isOwnerDraw ? MenuStyles::OwnerDraw : 0;
					::AppendMenu(m_hMenu, item->_uMenuStyle, n, tc);
				}
			}
		}
	}
	//_bNeedCreate = false;
	CC_BIT_OFF(_Flags, CC_FLAG_MENU_NEEDCREATED);
	SetInnerStyle(MenuInnerStyle::HandleCreated, true);
}

bool Menu::ProcessCmdKey(MSG* pMsg, UINT_PTR keyData)
{
	if(_lstMenus != NULL)
	{
		for(int n = _lstMenus->GetSize() - 1; n >= 0; n--)
		{
			Menu* menu = _lstMenus->GetKey(n);
			if(menu->ProcessCmdKey(pMsg, keyData))
			{
				return true;
			}
		}
	}
	else
	{
		if(_eMenuType == MenuType::MENUITEM)
		{
			MenuItem* item = (MenuItem*)this;
			if(_eShortcut != Shortcut::None && item->GetEnabled() && (UINT_PTR)_eShortcut == keyData)
			{
				EventArgs ee(pMsg->hwnd, pMsg->message, pMsg->wParam, pMsg->lParam);
				item->PerformClick(&ee);
				return true;
			}
		}
	}
	return false;
}

LRESULT Menu::FindHotKey(UINT nChar)
{
	//turn to upcase
	if(nChar >= _T('a') && nChar <= _T('z'))
	{
		nChar = nChar - (_T('a') - _T('A'));
	}
	int menusize = ItemCount();
	for(int i = 0; i < menusize; i++)
	{
		MenuItem* item = (MenuItem*)ItemGet(i);
		if(item->GetMnemonic() == (TCHAR)nChar)
		{
			return(MAKELRESULT(i, 2));
		}
	}
	return NULL;
}

//here only DestroyMenu,but not release memory
void Menu::DestroyMenu()
{
	if(_lstMenus != NULL)
	{
		for(int n = _lstMenus->GetSize() - 1; n >= 0; n--)
		{
			Menu* menu = _lstMenus->GetKey(n);
			menu->DestroyMenu();
		}
	}

	if(m_hMenu != NULL)
	{
		//remove handel
		Map<HMENU, Menu*>* mapMenus = CC_APP_GetMapHMENU();
		mapMenus->Remove(m_hMenu);

		::DestroyMenu(m_hMenu);
		m_hMenu = NULL;
	}
}

// Get the value of a menu item flag.
bool Menu::HasStyle(MenuStyles::Type style)
{
	return ((_uMenuStyle & style) != 0);
}

// Set the value of a menu item flag.
void Menu::SetStyle(MenuStyles::Type style, bool value, bool needUpdate)
{
	if(value)
	{
		_uMenuStyle |= style;
	}
	else
	{
		_uMenuStyle &= ~style;
	}
	if(needUpdate)
	{
		//_bNeedCreate = true;
		CC_BIT_ON(_Flags, CC_FLAG_MENU_NEEDCREATED);
	}
}

bool Menu::IsInnerStyle(MenuInnerStyle::Type innerStyle) const
{
	return ((_InnerStyle & innerStyle) != 0);
}

void Menu::SetInnerStyle(MenuInnerStyle::Type innerStyle, bool value, bool needUpdate)
{
	if(value)
	{
		_InnerStyle |= innerStyle;
	}
	else
	{
		_InnerStyle &= ~innerStyle;
	}
	if(needUpdate)
	{
		//_bNeedCreate = true;
		CC_BIT_ON(_Flags, CC_FLAG_MENU_NEEDCREATED);
	}
}

void Menu::DoSetLang(KeyArr& keyArr, cc::Str sThisHead)
{
	ITextIcon::DoSetLang(keyArr, sThisHead);
	if(_lstMenus != NULL)
	{
		for(int n = _lstMenus->GetSize() - 1; n >= 0; n--)
		{
			_lstMenus->GetKey(n)->SetLang(keyArr, sThisHead);
		}
	}
}
// constructors/destructor
///////////////////////////////////////////////////////////////////////////////

//===========================================================cc::win::MenuItem
///////////////////////////////////////////////////////////////////////////////
// constructors/destructor
MenuItem::MenuItem()
{
	SetText(_T("MenuItem"));

	SetInnerStyle(MenuInnerStyle::MdiList, true);
	SetInnerStyle(MenuInnerStyle::Visible, true);
#ifdef _DEBUG_
	SetName(_T("MenuItem"));
#endif
}

MenuItem::~MenuItem()
{
}

// Get or set this item's properties.
bool MenuItem::GetBreak()
{
	return HasStyle(MenuStyles::Break);
}

void MenuItem::SetBreak(bool value)
{
	SetStyle(MenuStyles::Break, value, true);
}

bool MenuItem::GetChecked()
{
	return HasStyle(MenuStyles::Checked);
}

//Checked is only valid for ownerdraw,not need recreate menu
void MenuItem::SetChecked(bool value)
{
	SetStyle(MenuStyles::Checked, value, true);
}

bool MenuItem::GetEnabled()
{
	return !HasStyle(MenuStyles::Disabled);
}

void MenuItem::SetEnabled(bool value)
{
	if(!CC_BIT_HAS(_Flags, CC_FLAG_MENU_NEEDCREATED) && _Parent != NULL && _Parent->m_hMenu != NULL)
	{
		SetStyle(MenuStyles::Disabled, !value, false);
		//directly undate menu
		::EnableMenuItem(_Parent->m_hMenu, GetIndex(), value ? MF_BYCOMMAND | MF_ENABLED : MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	}
	else
	{
		SetStyle(MenuStyles::Disabled, !value, true);
	}
}

void MenuItem::SetIndex(int index)
{
	if(_Parent != NULL)
	{
		_Parent->ItemSetIndex(this, index);
		//_bNeedCreate = true;
		CC_BIT_ON(_Flags, CC_FLAG_MENU_NEEDCREATED);
	}
}

int MenuItem::GetIndex()
{
	if(_Parent != NULL)
	{
		return _Parent->ItemGetIndex(this);
	}
	else
	{
		return -1;
	}
}

bool MenuItem::GetSelected()
{
	return HasStyle(MenuStyles::Selected);
}

void MenuItem::SetSelected(bool value)
{
	SetStyle(MenuStyles::Selected, value, true);
}

bool MenuItem::GetMdiList()
{
	return IsInnerStyle(MenuInnerStyle::MdiList);
}

void MenuItem::SetMdiList(bool value)
{
	SetInnerStyle(MenuInnerStyle::MdiList, value, true);
}

bool MenuItem::GetOwnerDraw()
{
	return HasStyle(MenuStyles::OwnerDraw);
}

void MenuItem::SetOwnerDraw(bool value)
{
	SetStyle(MenuStyles::OwnerDraw, value, true);
}

Menu* MenuItem::GetParent()
{
	return _Parent;
}

bool MenuItem::GetRadioCheck()
{
	return HasStyle(MenuStyles::RadioCheck);
}

void MenuItem::SetRadioCheck(bool value)
{
	SetStyle(MenuStyles::RadioCheck, value, true);
}

bool MenuItem::GetVisible()
{
	return IsInnerStyle(MenuInnerStyle::Visible);
}

void MenuItem::SetVisible(bool value)
{
	SetInnerStyle(MenuInnerStyle::Visible, value, true);
}

int MenuItem::GetMenuID()
{
	return GetIndex();
}

void MenuItem::PerformClick(EventArgs *e)
{
	OnClick(e);
}

void MenuItem::PerformDrawItem(DrawItemEventArgs *e)
{
	OnDrawItem(e);
}

void MenuItem::PerformMeasureItem(MeasureItemEventArgs *e)
{
	OnMeasureItem(e);
}

void MenuItem::PerformPopup(EventArgs *e)
{
	OnPopup(e);
	/*
	//if need,recreate(only check self)
	if(CC_BIT_HAS(_Flags, CC_FLAG_MENU_NEEDCREATED))
	{
		CC_ASSERT(m_hMenu != NULL);

		//creat menu
		int nMenuCnt;
		if(_lstMenus != NULL && (nMenuCnt = _lstMenus->GetSize()) > 0)
		{
			//treate AllOwnerDraw
			bool isOwnerDraw = false;
			if(_Parent != NULL)
			{
				SetInnerStyle(MenuInnerStyle::AllOwnerDraw, false);
				if(_Parent->IsInnerStyle(MenuInnerStyle::AllOwnerDraw))
				{
					SetInnerStyle(MenuInnerStyle::AllOwnerDraw, true);
					isOwnerDraw = true;
				}
			}
			else
			{
				isOwnerDraw = IsInnerStyle(MenuInnerStyle::AllOwnerDraw);
			}

			for(int n = 0; n < nMenuCnt; n++)
			{
				//here must be menuitem
				MenuItem* item = (MenuItem*)_lstMenus->GetKey(n);
				item->DestroyMenu();
				::DeleteMenu(m_hMenu, n, MF_BYPOSITION);
				if(!item->GetVisible())
				{
					//if not visible,not do it
					continue;
				}

				item->CreateMenuHandle();
				if(item->m_hMenu != NULL)
				{
					//here is submenu
					item->_uMenuStyle = MF_POPUP | (isOwnerDraw ? MenuStyles::OwnerDraw : 0);
					::AppendMenu(m_hMenu, item->_uMenuStyle, (UINT_PTR)item->m_hMenu, item->GetText());
				}
				else
				{
					//just menuitem
					const TCHAR* tc = item->GetText();
					if(tc != NULL && _tcscmp(tc, _T("-")) == 0)
					{
						item->_uMenuStyle = MF_SEPARATOR | (isOwnerDraw ? MenuStyles::OwnerDraw : 0);
						::AppendMenu(m_hMenu, item->_uMenuStyle, n, tc);
					}
					else
					{
						item->_uMenuStyle |= isOwnerDraw ? MenuStyles::OwnerDraw : 0;
						::AppendMenu(m_hMenu, item->_uMenuStyle, n, tc);
					}
				}
			}
		}
		//_bNeedCreate = false;
		CC_BIT_OFF(_Flags, CC_FLAG_MENU_NEEDCREATED);
		SetInnerStyle(MenuInnerStyle::HandleCreated, true);
	}
	*/
}

void MenuItem::PerformSelect(EventArgs *e)
{
	OnSelect(e);
}

void MenuItem::OnClick(EventArgs *e)
{
	Click.Invoke((Object*)this, e);
}

void MenuItem::OnMeasureItem(MeasureItemEventArgs *e)
{
	if(MeasureItem.IsEmpty())
	{
		CC_APP_GetDrawActor()->MeasureMenu(this, e);
	}
	else
	{
		MeasureItem.Invoke((Object*)this, e);
	}
}

void MenuItem::OnDrawItem(DrawItemEventArgs *e)
{
	if(DrawItem.IsEmpty())
	{
		CC_APP_GetDrawActor()->DrawMenu(this, e);
	}
	else
	{
		DrawItem.Invoke((Object*)this, e);
	}
}

void MenuItem::OnPopup(EventArgs* e)
{
	Popup.Invoke((Object*)this, e);
}

void MenuItem::OnSelect(EventArgs* e)
{
	Select.Invoke((Object*)this, e);
}
// constructors/destructor
///////////////////////////////////////////////////////////////////////////////

//===========================================================cc::win::Timer
///////////////////////////////////////////////////////////////////////////////
// constructors/destructor
Timer::Timer()
{
	_nID = -1;
	_isEnabled = 0;
	_nInterval = 1000;
}

Timer::~Timer()
{
	if(_isEnabled == 1)
	{
		Stop();
	}
}

void Timer::StaticProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	Map<int, Timer*>* mapTimer = CC_APP_GetMapTimer();
	int nInd;
	if((nInd = mapTimer->GetIndex((int)idEvent)) >= 0)
	{
		Timer* timer = mapTimer->GetAt(nInd);
		EventArgs e(NULL, uMsg, idEvent, dwTime);
#ifndef _DEBUG_
		try
		{
#endif //!_DEBUG_
			// timer's ontick
			timer->OnTick(&e);
#ifndef _DEBUG_
		}
		catch(...)
		{
			CC_LOGSTEP();
			cc::Log::fatal(_T("[Timer::StaticProc]Exception Error!"));
			cc::Log::printStackTrace();
			CC_ASSERT(0);
		}
#endif //!_DEBUG_
	}
}

void Timer::OnTick(EventArgs *e)
{
	Tick.Invoke((Object*)this, e);
}

void Timer::SetEnabled(bool isEnabled)
{
	if(_isEnabled != (isEnabled ? 1 : 0))
	{
		if(_isEnabled == 1)
		{
			Stop();
		}
		else
		{
			Start();
		}
	}
}

bool Timer::GetEnabled() const
{
	return (_isEnabled == 1);
}

void Timer::SetInterval(int nInterval)
{
	if(nInterval > 0)
	{
		_nInterval = nInterval;
		if(_isEnabled == 1)
		{
			Stop();
			Start();
		}
	}
}

int Timer::GetInterval() const
{
	return _nInterval;
}

void Timer::Start()
{
	if(_isEnabled == 1)
	{
		Stop();
	}
	_nID = (UINT)(WORD)::SetTimer(NULL, (UINT)(WORD)0, (UINT)(WORD)_nInterval, StaticProc);
	CC_APP_GetMapTimer()->Add(_nID, this);
	_isEnabled = 1;
}

void Timer::Stop()
{
	if(_isEnabled == 1)
	{
		::KillTimer(NULL, _nID);
		CC_APP_GetMapTimer()->Remove(_nID);
		_nID = -1;
		_isEnabled = 0;
	}
}
// constructors/destructor
///////////////////////////////////////////////////////////////////////////////

//===========================================================cc::win::Thread
//#ifdef _MT

struct _CC_THREAD_STARTUP
{
	// following are "in" parameters to thread startup
	Thread* pThread;    // Thread for new thread
	HANDLE hEvent;          // event triggered after success/non-success
	HANDLE hEvent2;         // event triggered after thread is resumed
	WPARAM wParam;
	LPARAM lParam;

	//for direct start new thread
	Object* pObj;
	void (Object::*pMethod)(Object*, EventArgs*);
};

//#endif //_MT

///////////////////////////////////////////////////////////////////////////////
// constructors/destructor
Thread::Thread()
{
	m_hThread = NULL;
	m_nThreadID = 0;
	_nThreadState = ThreadState::Unstarted;
}

///////////////////// destructor //////////////////////////
Thread::~Thread()
{
	// free thread object
	if(m_hThread != NULL)
	{
		::TerminateThread(m_hThread, 0);
		::CloseHandle(m_hThread);
		m_hThread = NULL;
		//::Terminate(0);
	}
}
// constructors/destructor

// Thread
Thread::operator HANDLE() const
	{ return this == NULL ? NULL : m_hThread; }
BOOL Thread::SetThreadPriority(int nPriority)
	{ CC_ASSERT(m_hThread != NULL); return ::SetThreadPriority(m_hThread, nPriority); }
int Thread::GetThreadPriority()
	{ CC_ASSERT(m_hThread != NULL); return ::GetThreadPriority(m_hThread); }
DWORD Thread::ResumeThread()
	{ CC_ASSERT(m_hThread != NULL); return ::ResumeThread(m_hThread); }
DWORD Thread::SuspendThread()
	{ CC_ASSERT(m_hThread != NULL); return ::SuspendThread(m_hThread); }
BOOL Thread::PostThreadMessage(UINT message, WPARAM wParam, LPARAM lParam)
	{ CC_ASSERT(m_hThread != NULL); return ::PostThreadMessage(m_nThreadID, message, wParam, lParam); }

unsigned long _stdcall Thread::ThreadEntry(void *pParam)
{
//#ifndef _MT
//#else
	_CC_THREAD_STARTUP* pStartup = (_CC_THREAD_STARTUP*)pParam;
	CC_ASSERT(pStartup != NULL);
	CC_ASSERT(pStartup->pThread != NULL);
	CC_ASSERT(pStartup->hEvent != NULL);

	// pStartup is invlaid after the following
	_CC_THREAD_STARTUP startup;
	memcpy(&startup, (_CC_THREAD_STARTUP*)pParam, sizeof(startup));
	Thread* pThread = pStartup->pThread;

	// allow the creating thread to return from Thread::Start
	CC_VERIFY(::SetEvent(startup.hEvent));
	// wait for thread to be resumed
	CC_VERIFY(::WaitForSingleObject(startup.hEvent2, INFINITE) == WAIT_OBJECT_0);
	::CloseHandle(startup.hEvent2);
	pThread->_nThreadState = ThreadState::Running;
	//pParam not valid after this

#ifndef _DEBUG_
	try
	{
#endif //!_DEBUG_
		// call user process thread
		EventArgs e(NULL, 0, startup.wParam, startup.lParam);
		pThread->CallThreadProc(&e);
#ifndef _DEBUG_
	}
	catch(const Exception exp)
	{
		CC_LOGSTEP();
		cc::Log::fatal(_T("[Thread::ThreadEntry]Exception Error:%s"), exp.GetMessage());
		cc::Log::printStackTrace();
		CC_ASSERT(0);
		throw exp;
	}
	catch(...)
	{
		CC_LOGSTEP();
		cc::Log::fatal(_T("[Thread::ThreadEntry]Exception Error!"));
		cc::Log::printStackTrace();
		//CC_ASSERT(0);
	}
#endif //!_DEBUG_

	pThread->_nThreadState = ThreadState::Stopped;
	pThread->m_hThread = NULL;
	pThread->m_nThreadID = 0;
//#endif //!_MT

	// allow C-runtime to cleanup, and exit the thread
	//_endthreadex(0);
	//ExitThread(0);
	// Not reached
	return 0;
}

unsigned long _stdcall Thread::ThreadEntry_static(void *pParam)
{
//#ifndef _MT
//#else
	_CC_THREAD_STARTUP* pStartup = (_CC_THREAD_STARTUP*)pParam;
	CC_ASSERT(pStartup != NULL);
	CC_ASSERT(pStartup->hEvent != NULL);

	// pStartup is invlaid after the following
	_CC_THREAD_STARTUP startup;
	memcpy(&startup, (_CC_THREAD_STARTUP*)pParam, sizeof(startup));

	// allow the creating thread to return from Thread::Start
	CC_VERIFY(::SetEvent(startup.hEvent));
	// wait for thread to be resumed
	CC_VERIFY(::WaitForSingleObject(startup.hEvent2, INFINITE) == WAIT_OBJECT_0);
	::CloseHandle(startup.hEvent2);

#ifndef _DEBUG_
	try
	{
#endif //!_DEBUG_
		// call user process thread
		EventArgs e(NULL, 0, startup.wParam, startup.lParam);
		((startup.pObj)->*(startup.pMethod))(startup.pObj, &e);
#ifndef _DEBUG_
	}
	catch(const Exception exp)
	{
		CC_LOGSTEP();
		cc::Log::fatal(_T("[Thread::ThreadEntry]Exception Error:%s"), exp.GetMessage());
		cc::Log::printStackTrace();
		CC_ASSERT(0);
		throw exp;
	}
	catch(...)
	{
		CC_LOGSTEP();
		cc::Log::fatal(_T("[Thread::ThreadEntry]Exception Error!"));
		cc::Log::printStackTrace();
		CC_ASSERT(0);
	}
#endif //!_DEBUG_

//#endif //!_MT

	// allow C-runtime to cleanup, and exit the thread
	//_endthreadex(0);
	//ExitThread(0);
	// Not reached
	return 0;
}

bool Thread::Start(DWORD dwCreateFlags, UINT nStackSize, LPSECURITY_ATTRIBUTES lpSecurityAttrs)
{
//#ifndef _MT
//	return false;
//#else
	CC_ASSERT(m_hThread == NULL);  // already created?

	// setup startup structure for thread initialization
	_CC_THREAD_STARTUP startup;
	memset(&startup, 0, sizeof(startup));
	startup.pThread = this;
	startup.hEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
	startup.hEvent2 = ::CreateEvent(NULL, TRUE, FALSE, NULL);
	if(startup.hEvent == NULL || startup.hEvent2 == NULL)
	{
		CC_TRACE((_T("Warning: CreateEvent failed in Thread::Start.")));
		if(startup.hEvent != NULL)
		{
			::CloseHandle(startup.hEvent);
		}
		if(startup.hEvent2 != NULL)
		{
			::CloseHandle(startup.hEvent2);
		}
		return false;
	}

	//why not use _beginthreadex?see head of "cc.File.h"
	m_hThread = ::CreateThread(lpSecurityAttrs, nStackSize, ThreadEntry, &startup, dwCreateFlags | CREATE_SUSPENDED, &m_nThreadID);
	// create the thread (it may or may not start to run)
	//m_hThread = (HANDLE)(ULONG_PTR)_beginthreadex(lpSecurityAttrs, nStackSize,  //REVIEW
	//	&ThreadEntry, &startup, dwCreateFlags | CREATE_SUSPENDED, (UINT*)&m_nThreadID);
	if(m_hThread == NULL)
	{
		::CloseHandle(startup.hEvent);
		::CloseHandle(startup.hEvent2);
		return false;
	}

	// start the thread just for initialization
	CC_VERIFY(::ResumeThread(m_hThread) != (DWORD)-1);
	CC_VERIFY(::WaitForSingleObject(startup.hEvent, INFINITE) == WAIT_OBJECT_0);
	::CloseHandle(startup.hEvent);

	// if created suspended, suspend it until resume thread wakes it up
	if(dwCreateFlags & CREATE_SUSPENDED)
	{
		CC_VERIFY(::SuspendThread(m_hThread) != (DWORD)-1);
	}

	// allow thread to continue, once resumed (it may already be resumed)
	::SetEvent(startup.hEvent2);
	return true;
//#endif //!_MT
}

void Thread::SendStopRequested()
{
	_nThreadState |= ThreadState::StopRequested;
}

bool Thread::IsStopRequested()
{
	return ((_nThreadState & ThreadState::StopRequested) != 0);
}

void Thread::Terminate(int millisecondsTimeout)
{
	if(m_hThread == NULL)
	{
		return;
	}

	_nThreadState |= ThreadState::StopRequested;
	switch(::WaitForSingleObject(m_hThread, millisecondsTimeout))
	{
	case WAIT_OBJECT_0:
	    //  The thread has terminated - do something
	    break;
	    
	case WAIT_TIMEOUT:
	    //  The timeout has elapsed but the thread is still running
	    //  do something appropriate for a timeout
		::TerminateThread(m_hThread, 0);
	    break;
	}

	if(m_hThread != NULL)
	{
		::CloseHandle(m_hThread);
		m_hThread = NULL;
	}
	m_nThreadID = 0;
	_nThreadState = ThreadState::Stopped;
}

bool Thread::IsAlive()
{
	return ((_nThreadState & ThreadState::Running) != 0);
}

void Thread::CallThreadProc(EventArgs* e)
{
	ThreadProc.Invoke((Object*)this, e);
}

bool Thread::StartThread(Object* obj, void (Object::*method)(Object*, EventArgs*), WPARAM wParam, LPARAM lParam)
{
//#ifndef _MT
//	return false;
//#else
	if(obj == NULL)
	{
		return false;
	}

	// setup startup structure for thread initialization
	_CC_THREAD_STARTUP startup;
	memset(&startup, 0, sizeof(startup));
	startup.pThread = NULL;
	startup.pObj = obj;
	startup.pMethod = method;
	startup.hEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
	startup.hEvent2 = ::CreateEvent(NULL, TRUE, FALSE, NULL);
	startup.wParam = wParam;
	startup.lParam = lParam;
	if(startup.hEvent == NULL || startup.hEvent2 == NULL)
	{
		CC_TRACE((_T("Warning: CreateEvent failed in Thread::Start.")));
		if(startup.hEvent != NULL)
		{
			::CloseHandle(startup.hEvent);
		}
		if(startup.hEvent2 != NULL)
		{
			::CloseHandle(startup.hEvent2);
		}
		return false;
	}

	//why not use _beginthreadex?see head of "cc.File.h"
	HANDLE m_hThread = ::CreateThread(NULL, 0, ThreadEntry_static, &startup, 0 | CREATE_SUSPENDED, NULL);
	// create the thread (it may or may not start to run)
	//m_hThread = (HANDLE)(ULONG_PTR)_beginthreadex(lpSecurityAttrs, nStackSize,  //REVIEW
	//	&ThreadEntry, &startup, dwCreateFlags | CREATE_SUSPENDED, (UINT*)&m_nThreadID);
	if(m_hThread == NULL)
	{
		::CloseHandle(startup.hEvent);
		::CloseHandle(startup.hEvent2);
		return false;
	}

	// start the thread just for initialization
	CC_VERIFY(::ResumeThread(m_hThread) != (DWORD)-1);
	CC_VERIFY(::WaitForSingleObject(startup.hEvent, INFINITE) == WAIT_OBJECT_0);
	::CloseHandle(startup.hEvent);

	// allow thread to continue, once resumed (it may already be resumed)
	::SetEvent(startup.hEvent2);
	return true;
//#endif //!_MT
}

#if defined(_DEBUG_) || defined(_DESIGN)
cc::win::Control* CC_Design_Hotcontrol_new = NULL;
#endif
