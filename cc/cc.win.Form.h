// cc.win.Form.h
// The C# like Form class
// Author: Shu.KK
// 2006-01-01
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef CC_WIN_FORM_H_
#define CC_WIN_FORM_H_

#include "cc.win.Control.h"

namespace cc
{
namespace win
{

class MainMenuBarConfig : public ObjectConfig
{
public:
	BASE_CONTROL_CONFIG_DEF;
	static int nMarginLeft;
	static int nMarginTop;
	static int nMarginRight;
	static int nMarginBottom;
};

#if !defined(_CCTHINFORM)
//CC_FLAG_TOOLBAR_END is used 2bit
#define CC_MAINMENUBAR_MEARGIN -2
#define CC_FLAG_MAINMENUBAR_IGNOREALTKEY			CC_FLAG_TOOLBAR_END
#define CC_FLAG_MAINMENUBAR_PROCESSRIGHTARROW		CC_FLAG_TOOLBAR_END <<1
#define CC_FLAG_MAINMENUBAR_PROCESSLEFTARROW		CC_FLAG_TOOLBAR_END <<2
#define CC_FLAG_MAINMENUBAR_ISCALCELBYESC			CC_FLAG_TOOLBAR_END <<3
#define CC_FLAG_MAINMENUBAR_ISCALCELBYMCLK			CC_FLAG_TOOLBAR_END <<4
#define CC_FLAG_MAINMENUBAR_END						CC_FLAG_TOOLBAR_END <<5
class MainMenuBar : public ToolBar
{
public:
	MainMenuBar();
	virtual ~MainMenuBar();

	//virtual SIZE BestSize(bool isUpdate = false);
	virtual void SetMainMenu(MainMenu* mainMenu);
	virtual MainMenu* GetMainMenu();
	int GetItemCount() const;

protected:
	typedef enum
	{
		TR_NONE,
		TR_POPUP,
		TR_MENU
	} ETrackingState;

	// WndProc - calls appriate On... function for the give
	virtual LRESULT WndProc(EventArgs *e);
	virtual void OnCreateControl();
	virtual void OnMouseLeave(EventArgs *e);
	virtual void OnMouseMove(MouseEventArgs *e);
	virtual bool PreFormProcessMessage(MSG* pMsg, UINT_PTR& keyData);
	static LRESULT CALLBACK MenuInputFilter(int code, WPARAM wParam, LPARAM lParam);
	bool MenuMessageCenter(UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR keyData, int nType);
	bool MenuMessageOnKey(UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR& keyData, int nType);
	int PointToIndex(POINT point) const;
	void SetHit(int index, HDC hDC, bool isOn);
	void SetHotHit(int index);
	void ShowMenu(int index);
	virtual void DoUpdate();
	void UpdateMainMenu();
	HFONT GetMenuFont();

protected:
	ETrackingState m_eTrackingState;
	MainMenu* _pMainMenu;
	RECT* _ItemsRect;
	static MainMenuBar* MainMenuBar::cc_win_pMenuBar;
	int m_nHotIndex;
	int _nNewIndex;
	Control* _pTopLevelForm;

	//keyArr is param address,but sThisHead is by value
	virtual void DoSetLang(KeyArr& keyArr, cc::Str sThisHead);
};
#endif //#if !defined(_CCTHINFORM)

class UserControlConfig : public ObjectConfig
{
public:
	BASE_CONTROL_CONFIG_DEF;
};
#define CC_FLAG_USERCONTROL_END				CC_FLAG_CONTAINERCONTROL_END
class UserControl : public Container
{
public:
	//Constructors
	UserControl();
	//destructor
	virtual ~UserControl();

protected:
};

//System.Windows.Forms.TabAlignment
typedef struct tagTabAlignment
{
	enum Type
	{
		Bottom = 1,
		Left = 2,
		Right = 3,
		Top = 0,
		None = 4
	};
}TabAlignment;

//System.Windows.Forms.TabAppearance
typedef struct tagTabAppearance
{
	enum Type
	{
		Buttons = 1,
		FlatButtons = 2,
		Normal = 0
	};
}TabAppearance;

class TabPageConfig : public ObjectConfig
{
public:
	BASE_CONTROL_CONFIG_DEF;
	static int nMarginLeft;
	static int nMarginTop;
	static int nMarginRight;
	static int nMarginBottom;
};
#define CC_FLAG_TABPAGE_END				CC_FLAG_CONTAINERCONTROL_END
class TabPage : public Container
{
public:
	//Constructors
	TabPage();
	//destructor
	virtual ~TabPage();
	//this is accessed by parent of tab or others
	void* PageData;
	virtual bool SetActivateFocus(HWND _hFocus = NULL);

protected:
	HWND _hFocusBeforeDeactivate;

	virtual void OnVisibleChanged(EventArgs *e);
	virtual void DoUpdate();

private:
	//hide this
	bool GetVisible() {return Control::GetVisible();};
	virtual void SetVisible(bool value) {Control::SetVisible(value);};
};

class TabControlConfig : public ObjectConfig
{
public:
	BASE_CONTROL_CONFIG_DEF;
};
#define CC_TABCONTROL_ICONSIZE 14
#define CC_FLAG_TABCONTROL_END				CC_FLAG_CONTAINERCONTROL_END
class TabControl : public Control
{
public:
	EventHandler HeadDoubleClick;
	EventHandler SelectedIndexChanged;
	EventHandler SelectedIndexChanging;

public:
	//Constructors
	TabControl();
	//destructor
	virtual ~TabControl();

	virtual bool Focus(bool setLastControl = false);
	void AddPage(TabPage *control, int nIndex = -1);
	//show delete TabPage out this scope
	TabPage* RemovePage(int nIndex);
	TabPage *GetPage(TCHAR* const ctlName) const;
	TabPage *GetPage(int nIndex) const;
	int GetTabCount() const;
	void SetSelectedIndex(int selectedIndex);
	int GetSelectedIndex();
	TabPage* GetSelectedTab();
	void SetSelectedTab(TabPage* selectedTab);
	void SetHeadContextMenu(ContextMenu* menu);
	ContextMenu* GetHeadContextMenu() const;

	TabAlignment::Type GetTabAlignment() const;
	void SetTabAlignment(TabAlignment::Type alignment);
	TabAppearance::Type GetTabAppearance() const;
	void SetTabAppearance(TabAppearance::Type appearance);

	virtual void GetBodyRect(RECT &rect);

	virtual Control* RemoveControl(int nIndex);
	virtual void RemoveControl(Control* control);

protected:
	int _nSelectedIndex;
	int _nLastSelectedIndex;
	TabAlignment::Type _eAlignment;
	TabAppearance::Type _eAppearance;
	ContextMenu* _pHeadContextMenu;
	RECT _rectPage;

	//keyArr is param addres,but sHead is by value
	virtual void DoConfig(bool isFromIni, KeyArr& keyArr, cc::Str sThisHead);
	virtual bool ProcessCmdKey(MSG* pMsg, UINT_PTR& keyData);
	virtual void OnChildNotify(EventArgs *e);
	virtual void OnCreateControl();
	virtual void DoUpdate();
	//virtual void OnPaint(PaintEventArgs *e);
	virtual void OnResize(EventArgs *e);
	virtual void OnSelectedIndexChanged(EventArgs *e);
	virtual void OnSelectedIndexChanging(EventArgs *e);
// Overridables (for owner draw only)
	virtual void DrawItem(DrawItemEventArgs* e);

	virtual void OnDoubleClick(EventArgs *e);
	virtual void OnHeadRClick(EventArgs *e);

	virtual void GetItemRect(int nIndex, RECT &rect);
	bool SetImageList(TabPage* page, int nIndex);
	virtual int DoInsPage(TabPage *control, int nIndex = -1);
	virtual int DoModPage(TabPage *control, int nIndex = -1);

private:
	//hide this
	void AddControl(Control* control, int nIndex = -1){};
	Control* GetControl(TCHAR* const ctlName) const{return Control::GetControl(ctlName);};
	Control* GetControl(int nIndex) const{return Control::GetControl(nIndex);};
};

//for control
typedef struct tagFormWindowState
{
	enum Type
	{
		Maximized = SW_SHOWMAXIMIZED,
		Minimized = SW_SHOWMINIMIZED,
		Normal = SW_SHOWNORMAL,
	};
}FormWindowState;

class FormClientConfig : public ObjectConfig
{
public:
	BASE_CONTROL_CONFIG_DEF;
};
#define CC_FLAG_FORMCLIENT_END				CC_FLAG_CONTAINERCONTROL_END
class FormClient : public Container
{
public:
	// constructors
	FormClient();
	// destructor
	virtual ~FormClient();

	void SetIsMdiContainer();

protected:
	virtual bool PreCreateWindow(CREATESTRUCT& cs);
	//virtual void OnGotFocus(EventArgs *e);
};

class MDIChildConfig : public ObjectConfig
{
public:
	BASE_CONTROL_CONFIG_DEF;
};
//start at:200709,...
#define CC_FLAG_MDICHILD_END				CC_FLAG_CONTAINERCONTROL_END
class MDIChild : public Container
{
public:
	// constructors
	MDIChild();
	// destructor
	virtual ~MDIChild();

protected:
	//virtual bool PreCreateWindow(CREATESTRUCT& cs);
	//virtual void OnGotFocus(EventArgs *e);
};

class FormFrameConfig : public ObjectConfig
{
public:
	//BASE_CONTROL_CONFIG_DEF;
};
#define CC_FLAG_FORMFRAME_END				CC_FLAG_CONTAINERCONTROL_END
class FormFrame : public Container
{
public:
	// constructors
	FormFrame();
#if !defined(_CCTHINFORM)
	// destructor
	virtual ~FormFrame();
	virtual void Dispose();
#endif //#if !defined(_CCTHINFORM)

	//only can do what you know to client
	FormClient* GetClient();
#if !defined(_CCTHINFORM)
	virtual void AddDockBar(DockBar* control, int nIndex = -1);
	virtual DockBar* RemoveDockBar(int nIndex);
	virtual void RemoveDockBar(DockBar* control);
	virtual DockBar* GetDockBar(TCHAR* const ctlName) const;
	virtual DockBar* GetDockBar(int nIndex) const;
	//use Control* but not ToolBar*
	virtual bool ContainsDockBar(Control* control) const;
	int GetDockBarCount() const;

	//ReBarStyle::Type is defined at ReBar
	ReBarStyle::Type GetReBarStyle();
	void SetReBarStyle(ReBarStyle::Type type);
	//add toolbar dock to rebar
	virtual void AddToolBar(ToolBar* control, int nIndex = -1);
	virtual ToolBar* RemoveToolBar(int nIndex);
	virtual void RemoveToolBar(ToolBar* control);
	virtual ToolBar* GetToolBar(TCHAR* const ctlName) const;
	virtual ToolBar* GetToolBar(int nIndex) const;
	//use Control* but not ToolBar*
	virtual bool ContainsToolBar(Control* control) const;
	int GetToolBarCount() const;
#endif //#if !defined(_CCTHINFORM)

protected:
#if !defined(_CCTHINFORM)
	//for DockBar
	List<Control*>* _lstBars;
	//for toolbar
	List<Control*>* _lstToolBars;
	ReBar* _arrBarCont[4];
	ReBarStyle::Type _eRebarStyle;
#if defined(_DEBUG_) || defined(_DESIGN)
	//for design while debug
	FloatContainer* _designBar;
#endif
#endif //#if !defined(_CCTHINFORM)

#if !defined(_CCTHINFORM)
	virtual LRESULT WndProc(EventArgs *e);
	//here to create window
	virtual void OnCreateControl();
	virtual bool PreFormProcessMessage(MSG* pMsg, UINT_PTR& keyData);
	virtual bool ProcessMnemonic(TCHAR tchar);
	virtual void OnResizeFrame(HDWP hDWP, RECT& frameRect);
	//keyArr is param addres,but sHead is by value
	virtual void DoConfig(bool isFromIni, KeyArr& keyArr, cc::Str sThisHead);
	//keyArr is param address,but sThisHead is by value
	virtual void DoSetLang(KeyArr& keyArr, cc::Str sThisHead);
	//0:left;1:top;2:right;3:bottom;-1 for float,-2 for hide
	virtual int GetToolBarDockSide(HWND hWnd) const;
	virtual void OnNcHittest(EventArgs *e);
#endif
};

class FormConfig : public ObjectConfig
{
public:
	BASE_CONTROL_CONFIG_DEF;
	static DWORD nOuterStyleClient;
	static DWORD nInnerStyleClient;
	static COLORREF corBackgroundClient;
	//form:timer's timeout for update tips
	static UINT nWaitTimerToUpdateTips;
};
#define CC_FLAG_FORM_ISALWAYSONTOP			CC_FLAG_FORMFRAME_END
#define CC_FLAG_FORM_ISCALLEDCLOSED			CC_FLAG_FORMFRAME_END <<1
#define CC_FLAG_FORM_ISMOVEFORMONCLICK		CC_FLAG_FORMFRAME_END <<2
#define CC_FLAG_FORM_ISACTIVATED			CC_FLAG_FORMFRAME_END <<3
#define CC_FLAG_FORM_ISKEYPREVIEW			CC_FLAG_FORMFRAME_END <<4
#define CC_FLAG_FORM_ENABLEPARENT			CC_FLAG_FORMFRAME_END <<5
#define CC_FLAG_FORM_END					CC_FLAG_FORMFRAME_END <<6
//if SetOKButton or SetCancelButton but some control like textbox(multiline) want VK_RETURN,
//then should set keyData=0 at TextBox::IsInputKey(UINT_PTR& keyData)(as sample).
//if set sth. for client should use Form->GetClient()
class Form : public FormFrame
{
public:
	//Event
	EventHandler Activated;
	CancelEventHandler Closing;
	EventHandler Closed;
	EventHandler Deactivate;
	//EventHandler Load;
	EventHandler Shutdowning;
	EventHandler Shutdown;

	EventHandler ActivateApp;
	//EventHandler GetMinMaxInfo;
	//e->LParam is focused control
	EventHandler FocusChanged;

public:
	// constructors
	Form();
	// destructor
	virtual ~Form();
	virtual void Dispose();

	void Show();
	//void Update();
	virtual RECT GetNormalBounds();
	DialogResult::Type ShowDialog();
	void EndDialog(DialogResult::Type dialogResult = DialogResult::Empty);

	virtual void SetMainMenu(int nIDMenu);
#if !defined(_CCTHINFORM)
	virtual void SetMainMenu(MainMenu* mainMenu);
	MainMenu* GetMainMenu();
	void SetStatusBar(StatusBar* statusBar);
	StatusBar* GetStatusBar();
	//if nIndex = -1, use nDefaultIndex,if isSetDefault then set for default text,also nIndex is for defaultindex
	virtual void SetStatus(const TCHAR* status, ImageList* imageList = NULL, int nIconID = -1, int nIndex = -1, bool isSetDefault = false);
	virtual void SetTips(const TCHAR* status, ImageList* imageList = NULL, int nIconID = -1);
	virtual void SetTips(const TCHAR* status, ImageList* imageList, int nIconID, int x, int y);
#endif

	virtual void SetVisible(bool value);
	void SetKeyPreview(bool value);
	bool GetKeyPreview();
	void SetCloseButtonEnable(bool value);
	bool GetCloseButtonEnable();
	void SetMinButtonEnable(bool value);
	bool GetMinButtonEnable();
	void SetMaxButtonEnable(bool value);
	bool GetMaxButtonEnable();
	void SetAlwaysOnTop(bool value);
	bool GetAlwaysOnTop();
	void SetWindowState(FormWindowState::Type value, bool notSetWhileHide = false);
	FormWindowState::Type GetWindowState();
	void SetMoveFormOnClick(bool value);
	bool GetMoveFormOnClick();
	void SetCancelButton(Button* btn);
	Button* GetCancelButton();
	void SetOKButton(Button* btn);
	Button* GetOKButton();

	//SetStype&SetExStyle should be called before this,also ignore toolbars
	//while Controls's positon is fixed,then recommend use SetSizeBaseOnClient(alse call SetClientSize).
	//while form'size is fixed,recommend to use this
	void SetSizeBaseOnClient(int cxWidth, int cyHeight);
	virtual RECT GetBounds();
	virtual bool Focused();
	int* GetMessageLoopFlag();

	void SetIsMdiContainer();
	void SetIsMdiContainer(bool value);
	bool GetIsMdiContainer();
	//need after SetSize
	void SetCenter(bool isOnScreen = false);
	//if maxSize is small than minSize, then auto use minSize as maxSize
	void SetMaxSize(int maxWidth, int maxHeight);
	//do nothing while form
	virtual RECT CalcuteRect(bool updateRect = false, int nExpectX = -1, int nExpectY = -1, int nExpectCX = -1, int nExpectCY = -1, bool isFourceCalcute = false);
#if defined(_DEBUG_) || defined(_DESIGN)
	virtual FloatContainer* CreateDesignBar();
#endif

protected:
	//add this to invoid some unkonw error!
	//DWORD _FormFlags;
#if !defined(_CCTHINFORM)
	StatusBar* _pStatusBar;
	MainMenu* _pMainMenu;
#endif //#if !defined(_CCTHINFORM)
	SIZE _maxSize;
	HWND _hTipsHwnd;
	LPARAM _pLastToolTip;
	//bool _bIsCloseButtonEnable;
	//bool _bIsAlwaysOnTop;
	//bool _bIsCalledClosed;
	//bool _bIsMoveFormOnClick;
	//bool _bIsActivated;
	DialogResult::Type _eDialogResult;
	FormWindowState::Type _eInitWindowState;
	HWND _hFocusBeforeDeactivate;
	int _bMessageLoopStopRequest;
	Button* _btnOK;
	Button* _btnCancel;

	//keyArr is param addres,but sHead is by value
	virtual void DoUpdate();
	virtual void DoConfig(bool isFromIni, KeyArr& keyArr, cc::Str sThisHead);
	//keyArr is param address,but sThisHead is by value
	virtual void DoSetLang(KeyArr& keyArr, cc::Str sThisHead);
	virtual void OnThemeChanged();
	virtual void OnSysColorChanged();

	virtual LRESULT DefWndProc(EventArgs *e);
	virtual LRESULT DefWndProc(int msg, WPARAM wParam, LPARAM lParam);
#if !defined(_CCTHINFORM)
	virtual void Menu_OnEnterLoop(EventArgs *e);
	virtual void UpdateMainMenu();
	virtual void OnEnabledChanged(EventArgs *e);
#endif //#if !defined(_CCTHINFORM)
	//virtual bool PreProcessMessage(MSG* pMsg, UINT_PTR keyData);
	virtual bool PreFormProcessMessage(MSG* pMsg, UINT_PTR& keyData);
	virtual bool ProcessCmdKey(MSG* pMsg, UINT_PTR& keyData);
	virtual bool ProcessDialogKey(UINT_PTR& keyData);
	virtual bool ProcessKeyPreview(EventArgs *e);
	virtual LRESULT WndProc(EventArgs *e);
	virtual bool PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInnerTimer(EventArgs *e);

///////////////////////////////////////////////////////////////////////////////
// On...
protected:
	virtual void OnActivated(EventArgs* e);
	virtual void OnClosing(CancelEventArgs* e);
	virtual void OnClosed(EventArgs* e);
	virtual void OnShutdowning(EventArgs* e);
	virtual void OnShutdown(EventArgs* e);
	//here to create window
	virtual void OnCreateControl();
	virtual void OnDeactivate(EventArgs *e);
	virtual void OnHandleDestroyed(EventArgs *e);
	virtual void OnLoad(EventArgs *e);
	virtual void OnResize(EventArgs *e);
	virtual void OnMouseDown(MouseEventArgs *e);
	virtual void SetBoundsCore(int x, int y, int width, int height, BoundsSpecified::Type specified, bool isSetSameOriginal);

	virtual void OnActivateApp(EventArgs *e);
	virtual void OnGetMinMaxInfo(EventArgs *e);
	virtual void OnToolBarActivated(EventArgs* e);
	virtual void OnFocusChanged(EventArgs *e);

	//for client
	virtual void Client_OnMouseDown(Object* sender, MouseEventArgs* e);
// On...
///////////////////////////////////////////////////////////////////////////////
};

class DialogConfig : public ObjectConfig
{
public:
	BASE_CONTROL_CONFIG_DEF;
	static DWORD nOuterStyleClient;
	static DWORD nInnerStyleClient;
	static COLORREF corBackgroundClient;
};
//DrawThemeBackground with OuterStyle::TransparentTop
#define CC_FLAG_DIALOG_END				CC_FLAG_FORM_END
//if set sth. for client should use Form->GetClient()
class Dialog : public Form
{
public:
	Dialog();
	virtual ~Dialog();

protected:
};

//if share memory in your app.you should rewrite _APP_SHARE_MEMORY and set App::SetShareMemorySize(int nShareMemorySize) at begin of run app
//now sizeof(_APP_SHARE_MEMORY)=48
struct _APP_SHARE_MEMORY
{
	HWND m_pMainWnd;//(size4)
	HINSTANCE m_hMainInstance;//(size4)
	DWORD nFlag1;
	DWORD nFlag2;
	DWORD nFlag3;
	DWORD nFlag4;
	DWORD nFlag5;
	char reverse2[10];
	char reverse1[10];
};

#define CC_FLAG_APP_SHAREDMEM				1<<0
#define CC_FLAG_APP_ALWAYSTIPS				1<<1
class AppConfig : public ObjectConfig
{
public:
	//all message is first go here, if return 1(TRUE) then stop here.
	static WNDPROC GlobalPreProcessMessage;
};
class App : public Object
{
protected:
	//flage check for first instance of App
	static bool _bIsConstructored;
	static CRITICAL_SECTION _CRITICAL_SECTION;
	//static HANDLE _hMutexRunFlage;
	//notice that must be const various
	static const TCHAR* _tcAppName;
	static const TCHAR* _tcAppVersion;

	static Map<HWND, Control*>* _mapHWND;
	static Map<HMENU, Menu*>* _mapMENU;
	static Map<int, Timer*>* _mapTimer;
	static HINSTANCE _appHINSTANCE;
	static Map<const TCHAR*, void*>* _mapVarious;
#if defined(_DEBUG_) || defined(_DESIGN)
	//only for design
	static FloatContainer* _designBar;
#endif

	//Static variable for Thread Local Storage Index
	static DWORD _dwTlsIndex;
	static int _nMessageLoopStarted;
	static bool _bMessageLoopStopRequest;
	static ToolTip* _pToolTip;
	static DrawActor* _pDrawActor;
	static cc::Config* _pConfig;
	static int _nShareMemorySize;
	static void* _pShareMemory;
	static HANDLE _pShareMemoryMap;
	//if _bIsCreatedShareMemory,then IsFirstInstance
	//static bool _bIsCreatedShareMemory;
	static DWORD _dwAppFlag;

public:
	// constructors
	App();
	// destructor
	virtual ~App();

	//for check should be same of _WIN32_WINNT for lib and app.
	static const DWORD _dWIN32_WINNT;
	//for check should be same of _DEBUG_ for lib and app.
	static const DWORD _dDEBUG;
	//for check should be same of _UNICODE for lib and app.
	static const DWORD _dUNICODE;

	static void SetInstance(HINSTANCE appHINSTANCE);
	static HINSTANCE GetInstance();
	static HWND GetActiveFormHwnd();
	static Control* GetActiveForm();
	static HWND GetMainFormHwnd();
	static Control* GetMainForm();
	static void Run(Form *form);
	static void StartMessageLoop(int* messageLoopStopRequest);
	static void ExitMessageLoop();
	static void Exit();
	static void DoEvents(HWND hWnd = NULL);

	static bool GetMessageLoop();
	static Map<HWND, Control*>* GetMapHWND();
	static Map<HMENU, Menu*>* GetMapHMENU();
	static Map<int, Timer*>* GetMapTimer();

	//when add manifest.xml for xp theme, also need set controls to have default teansparent
	static void SetDefaultTransparent(bool isTransparent);

#if defined(_DEBUG_) || defined(_DESIGN)
	//for design
	static FloatContainer* ShowDesignBar(Form* parent);
	static bool GetDesignBarActive();
	static HWND GetDesignBarHwnd();
#endif

	//for ToolTip
	static void SetToolTip(ToolTip* toolTip);
	static ToolTip* GetToolTip();
	static void SetToolTipActive(bool isActive);
	static bool GetToolTipActive();
	static void SetAlwaysTips(bool value);
	static bool GetAlwaysTips();

	//for DrawActor
	static void SetDrawActor(DrawActor* drawActor);
	static DrawActor* GetDrawActor();

	//for config
	static void SetConfig(cc::Config* keyArr);
	static cc::Config* GetConfig();

	//for global(public) point
	static void SetVarious(const TCHAR* key, void* value);
	static void* GetVarious(const TCHAR* key);

	//for share memory at multi app
	static void SetShareMemorySize(int nShareMemorySize);
	//defult is sizeof(_APP_SHARE_MEMORY)
	static int GetShareMemorySize();
	//default is point of _APP_SHARE_MEMORY
	static void* GetShareMemory();

	//no message,run this must after set for SetShareMemorySize(int nShareMemorySize)
	static bool IsFirstInstance();
	//if already run then show message, and if NULL then inner msg.
	static bool IsFirstInstance(const TCHAR* msg);

	static _APP_THREAD_STATE* GetThreadStateData();
	//static void SetThreadStateData(_APP_THREAD_STATE* pTlsData);
	static void ThreadSafeEnter();
	static void ThreadSafeLeave();
	static void SetNameVersion(const TCHAR* tcName, const TCHAR* tcVersion);
	static cc::Str GetNameVersion();
	static const TCHAR* GetVersion();
	static const TCHAR* GetName();
};

} //namespace win
} //namespace cc

#endif //#ifndef CC_WIN_FORM_H_
