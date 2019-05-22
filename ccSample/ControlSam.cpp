#include "ControlSam.h"
using namespace cc;
using namespace cc::win;



#define MAX_LOADSTRING 100
#define _ArrCount(array) (sizeof(array)/sizeof(array[0]))

// Global Variables:
HINSTANCE g_hInst;								// current instance
HWND g_hwndPropSheet;
HWND g_hwndMain;
UINT g_uiDefaultID = 0xe000;
	
TCHAR g_szTitle[MAX_LOADSTRING];								// The title bar text
TCHAR g_szWindowClass[MAX_LOADSTRING];								// The title bar text

// Foward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

HWND CreatePropSheet();
void InitPPSheetSize();
LRESULT CALLBACK  Page1DlgProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK  Page2DlgProc(HWND, UINT, WPARAM, LPARAM);

void CALLBACK  PSheetCallback(HWND, UINT, LPARAM);
void CenterPPSheet(DWORD);

//int PropSheetButtons[] = {IDOK, IDCANCEL, ID_APPLY_NOW, IDHELP};
int PropSheetButtons[] = {IDOK, IDCANCEL};


HWND CreatePropSheet()
{
	PROPSHEETPAGE psp[2];
	PROPSHEETHEADER psh;
	
	memset(psp, 0, sizeof(PROPSHEETPAGE) * 2);
	memset(&psh, 0, sizeof(PROPSHEETHEADER));
	
	//Fill out the PROPSHEETPAGE data structure page 1
	psp[0].dwSize		 = sizeof(PROPSHEETPAGE);
	psp[0].dwFlags		 = PSP_USETITLE;
	psp[0].hInstance	 = g_hInst;
	psp[0].pszTemplate	 = MAKEINTRESOURCE(IDD_PROPPAGE1);
	psp[0].pszIcon		 = NULL;
	psp[0].pfnDlgProc	 = (DLGPROC)Page1DlgProc;
	psp[0].pszTitle 	 = TEXT("Page 1");
	//	psp[0].lParam		 = 0;

	//need space(sizeof(WORD) * 3) for Menu,Class,Title Str of end of ppDlgTemp
	DLGTEMPLATE* pDlgTemp = (DLGTEMPLATE*)malloc(sizeof(DLGTEMPLATE)*5 + sizeof(WORD) * 3*5);
	memset(pDlgTemp, 0, sizeof(DLGTEMPLATE)*5 + sizeof(WORD) * 3*5);
	pDlgTemp->style = WS_CHILD | WS_VISIBLE | WS_BORDER;
	pDlgTemp->dwExtendedStyle = 0;
	pDlgTemp->cdit = 1;
	pDlgTemp->x = 0;
	pDlgTemp->y = 0;
	pDlgTemp->cx = 200;
	pDlgTemp->cy = 200;
	WORD* pWordPtr = (WORD*)(pDlgTemp + 1);
	// no menu
	*pWordPtr++ = 0;
	// default dialog box class
	*pWordPtr++ = 0;
	// NUL termination for string
	*pWordPtr++ = 0;

	// Get a pointer to the "end"
	BYTE* pOffset = (BYTE*)pWordPtr;

	// Fill out the structure and following bytes for the control.
	// DLGITEMTEMPLATE structures should be aligned on DWORD boundaries.
	DLGITEMTEMPLATE* pDialogItem = (DLGITEMTEMPLATE*) (((DWORD_PTR)pOffset + 3) & ~3);
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



	//Fill out the PROPSHEETPAGE data structure for page 2
	psp[1].dwSize		 = sizeof(PROPSHEETPAGE);
	psp[1].dwFlags		 = PSP_DLGINDIRECT | PSP_USETITLE;
	psp[1].hInstance	 = g_hInst;
	//psp[1].pszTemplate	 = MAKEINTRESOURCE(IDD_PROPPAGE2);
	psp[1].pResource	 = pDlgTemp;
	psp[1].pszIcon		 = NULL;
	psp[1].pfnDlgProc	 = (DLGPROC)Page2DlgProc;
	psp[1].pszTitle 	 = TEXT("Page 2");
	//	psp[1].lParam		 = 0;
	
	//Fill out the PROPSHEETHEADER
	psh.dwSize			 = sizeof(PROPSHEETHEADER);
	psh.dwFlags 		 = PSH_PROPSHEETPAGE | PSH_USECALLBACK| PSH_MODELESS;
	psh.hwndParent		 = g_hwndMain;
	psh.hInstance        = /*g_hInst*/NULL;
	psh.pszIcon 		 = NULL;
	psh.pszCaption		 = NULL;
	psh.nPages			 = sizeof(psp) / sizeof(PROPSHEETPAGE);
	psh.nStartPage		 = 0;
	psh.ppsp			 = (LPCPROPSHEETPAGE) &psp;
	psh.pfnCallback 	 = (PFNPROPSHEETCALLBACK)PSheetCallback;

	psh.dwFlags &= ~PSP_HASHELP;
	psh.dwFlags |= PSH_NOAPPLYNOW;

	HWND hwnd = (HWND)PropertySheet(&psh);

	HPROPSHEETPAGE hpage;
	PROPSHEETPAGE page;
	memset(&page, 0, sizeof(page));
	page.dwSize = sizeof(page);
	page.dwFlags = PSP_DLGINDIRECT | PSP_USETITLE;
	page.hInstance = g_hInst;
	page.pResource = pDlgTemp;

	page.pszTitle = TEXT("Page 3xxx");
	page.pfnDlgProc = (DLGPROC)Page2DlgProc;
	page.pfnCallback = NULL;
	page.lParam = (LPARAM)NULL;
	hpage = CreatePropertySheetPage(&page);

	cc::Log::debug(_T("hp:%d"), hpage);
	SendMessage(hwnd, PSM_ADDPAGE, 0, (LPARAM)hpage);

	memset(&page, 0, sizeof(page));
	page.dwSize = sizeof(page);
	page.dwFlags = PSP_USETITLE;
	page.hInstance = g_hInst;
	page.pszTemplate = MAKEINTRESOURCE(IDD_PROPPAGE2);
	//page.pResource = pDlgTemp;

	page.pszTitle = TEXT("Page 4xxx");
	page.pfnDlgProc = (DLGPROC)Page2DlgProc;
	page.pfnCallback = NULL;
	page.lParam = (LPARAM)NULL;
	hpage = CreatePropertySheetPage(&page);

	cc::Log::debug(_T("hp:%d"), hpage);
	SendMessage(hwnd, PSM_ADDPAGE, 0, (LPARAM)hpage);

	//free(pDlgTemp);
	return hwnd;
}

void InitPPSheetSize()
{
	RECT rectWnd;
	RECT rectButton;

	GetWindowRect(g_hwndPropSheet, &rectWnd);
	HWND hWnd = ::GetDlgItem(g_hwndPropSheet, IDOK);
	
	if(!hWnd)
	{
		DebugBreak();
	}

	::GetWindowRect(hWnd, &rectButton);
	
	SetWindowPos(g_hwndPropSheet, NULL, 30, 30,
		rectWnd.right - rectWnd.left, rectButton.top - rectWnd.top,
		SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_SHOWWINDOW);
	
	for (int i = 0; i < _ArrCount(PropSheetButtons); i++)
	{
		hWnd = ::GetDlgItem(g_hwndPropSheet, PropSheetButtons[i]);
		if (hWnd != NULL)
		{
			::ShowWindow(hWnd, SW_HIDE);
			::EnableWindow(hWnd, FALSE);
		}
	}
}


LRESULT CALLBACK Page1DlgProc(HWND hdlg, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	return FALSE;
}

LRESULT CALLBACK Page2DlgProc(HWND hdlg, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	return FALSE;
}


void CALLBACK PSheetCallback(HWND hwndPropSheet, UINT uMsg, LPARAM lParam)
{
	switch(uMsg)
	{
		//called before the dialog is created, hwndPropSheet = NULL, lParam points to dialog resource
	case PSCB_PRECREATE:
		{
			LPDLGTEMPLATE lpTemplate = (LPDLGTEMPLATE)lParam;
			lpTemplate->style &= ~WS_VISIBLE;
		}
		break;
		
		//called after the dialog is created
	case PSCB_INITIALIZED:
		break;
		
	}
}









ControlSam::ControlSam()
{
	InitializeComponent();
}

void ControlSam::InitializeComponent()
{
	int nTop = 1;
	imageList = (ImageList*)CC_APP_GetVarious(_T("ImageList"));
	contextMenu1 = (ContextMenu*)CC_APP_GetVarious(_T("ContextMenu1"));
	coneMenuConfig = (ContextMenu*)CC_APP_GetVarious(_T("coneMenuConfig"));
	contextMenu3 = (ContextMenu*)CC_APP_GetVarious(_T("ContextMenu3"));
	contextMenu4 = (ContextMenu*)CC_APP_GetVarious(_T("ContextMenu4"));

	toolbar1 = new ToolBar();
	toolbar1->SetLocation(0, nTop);
	toolbar1->SetSize(500, 30);
	this->AddControl(toolbar1);

	nTop += 60;
	labIndex = new Label();
	labIndex->SetName(_T("labIndex"));
	labIndex->SetText(_T("Index:"));
	labIndex->SetLocation(2, nTop + 2);
	labIndex->SetSize(25, 17);
	this->AddControl(labIndex);

	editIndex = new TextBox();
	editIndex->SetName(_T("editIndex"));
	editIndex->SetText(_T("9999"));
	editIndex->SetStyleNumber(true);
	editIndex->SetLocation(28, nTop);
	editIndex->SetSize(25, 17);
	this->AddControl(editIndex);

	labText = new Label();
	labText->SetName(_T("labText"));
	labText->SetText(_T("Text:"));
	labText->SetLocation(55, nTop + 2);
	labText->SetSize(25, 17);
	this->AddControl(labText);

	editText = new TextBox();
	editText->SetName(_T("editText"));
	editText->SetLocation(88, nTop);
	editText->SetSize(170, 17);
	editText->Anchor = (AnchorStyles::Type)(AnchorStyles::Left | AnchorStyles::Top | AnchorStyles::Right);
	this->AddControl(editText);

	chkIcon = new CheckBox();
	chkIcon->SetName(_T("chkIcon"));
	chkIcon->SetText(_T("Icon"));
	chkIcon->SetLocation(260, nTop);
	chkIcon->SetSize(50, 18);
	chkIcon->Anchor = (AnchorStyles::Type)(AnchorStyles::Right | AnchorStyles::Top);
	this->AddControl(chkIcon);

	chkRMenu = new CheckBox();
	chkRMenu->SetName(_T("chkRMenu"));
	chkRMenu->SetText(_T("RMenu"));
	chkRMenu->SetLocation(310, nTop);
	chkRMenu->SetSize(70, 18);
	chkRMenu->Anchor = (AnchorStyles::Type)(AnchorStyles::Right | AnchorStyles::Top);
	this->AddControl(chkRMenu);

	lstType = new ComboBox();
	lstType->SetName(_T("lstType"));
	lstType->SetLocation(382, nTop);
	lstType->SetSize(100, 200);
	lstType->Anchor = (AnchorStyles::Type)(AnchorStyles::Right | AnchorStyles::Top);
	this->AddControl(lstType);

	nTop += 20;
	btnAdd = new Button();
	btnAdd->SetName(_T("btnAdd"));
	btnAdd->SetText(_T("Add/Ins"));
	btnAdd->Click += EventHandler((Object*)this, (EventFun)&ControlSam::btnAdd_Click);
	btnAdd->SetLocation(2, nTop);
	btnAdd->SetSize(50, 21);
	this->AddControl(btnAdd);

	btnMod = new Button();
	btnMod->SetName(_T("btnMod"));
	btnMod->SetText(_T("Modify"));
	btnMod->Click += EventHandler((Object*)this, (EventFun)&ControlSam::btnMod_Click);
	btnMod->SetLocation(54, nTop);
	btnMod->SetSize(50, 21);
	this->AddControl(btnMod);

	btnDel = new Button();
	btnDel->SetName(_T("btnDel"));
	btnDel->SetText(_T("Del"));
	btnDel->Click += EventHandler((Object*)this, (EventFun)&ControlSam::btnDel_Click);
	btnDel->SetLocation(106, nTop);
	btnDel->SetSize(50, 21);
	this->AddControl(btnDel);

	btnDeEnable = new Button();
	btnDeEnable->SetName(_T("btnDeEnable"));
	btnDeEnable->SetText(_T("!Enable"));
	btnDeEnable->Click += EventHandler((Object*)this, (EventFun)&ControlSam::btnDeEnable_Click);
	btnDeEnable->SetLocation(160, nTop);
	btnDeEnable->SetSize(60, 21);
	this->AddControl(btnDeEnable);

	btnReCreate = new Button();
	btnReCreate->SetName(_T("btnReCreate"));
	btnReCreate->SetText(_T("ReCreateTab"));
	btnReCreate->Click += EventHandler((Object*)this, (EventFun)&ControlSam::btnReCreate_Click);
	btnReCreate->SetLocation(224, nTop);
	btnReCreate->SetSize(80, 21);
	this->AddControl(btnReCreate);

	btnExit = new Button();
	btnExit->SetName(_T("btnExit"));
	btnExit->SetText(_T("Exit"));
	btnExit->Click += EventHandler((Object*)this, (EventFun)&ControlSam::btnExit_Click);
	btnExit->SetLocation(306, nTop);
	btnExit->SetSize(50, 21);
	this->AddControl(btnExit);

	nTop += 22;
	chkBarRMenu = new CheckBox();
	chkBarRMenu->SetName(_T("chkRMenu"));
	chkBarRMenu->SetText(_T("HasRightMenu"));
	chkBarRMenu->Click += EventHandler((Object*)this, (EventFun)&ControlSam::chkBarRMenu_Click);
	chkBarRMenu->SetLocation(0, nTop);
	chkBarRMenu->SetSize(93, 21);
	this->AddControl(chkBarRMenu);

	chkBarBorder = new CheckBox();
	chkBarBorder->SetName(_T("chkBarBorder"));
	chkBarBorder->SetText(_T("HasBorder"));
	chkBarBorder->SetChecked(true);
	chkBarBorder->Click += EventHandler((Object*)this, (EventFun)&ControlSam::chkBarBorder_Click);
	chkBarBorder->SetLocation(100, nTop);
	chkBarBorder->SetSize(73, 21);
	this->AddControl(chkBarBorder);

	chkBarTraceRMenu = new CheckBox();
	chkBarTraceRMenu->SetName(_T("chkBarTraceRMenu"));
	chkBarTraceRMenu->SetText(_T("xxxxxxxxxxxxxxxxxxxxx"));
	chkBarTraceRMenu->Click += EventHandler((Object*)this, (EventFun)&ControlSam::chkTraceMParent_Click);
	chkBarTraceRMenu->SetLocation(200, nTop);
	chkBarTraceRMenu->SetSize(153, 21);
	this->AddControl(chkBarTraceRMenu);

	chkBigSize = new CheckBox();
	chkBigSize->SetName(_T("chkBigSize"));
	chkBigSize->SetText(_T("BigSize"));
	chkBigSize->Click += EventHandler((Object*)this, (EventFun)&ControlSam::chkBigSize_Click);
	chkBigSize->SetLocation(360, nTop);
	chkBigSize->SetSize(60, 21);
	this->AddControl(chkBigSize);

	chkDisabled = new CheckBox();
	chkDisabled->SetName(_T("chkDisabled"));
	chkDisabled->SetText(_T("Disabled"));
	chkDisabled->Click += EventHandler((Object*)this, (EventFun)&ControlSam::chkDisabled_Click);
	chkDisabled->SetLocation(420, nTop);
	chkDisabled->SetSize(66, 21);
	this->AddControl(chkDisabled);

	nTop += 22;
	toolSample = new ToolItem();
	toolSample->SetName(_T("toolButtonSample"));
	toolSample->SetText(_T("toolButtonSample"));
	toolSample->SetLocation(0, nTop);
	toolSample->SetSize(100, 17);
	this->AddControl(toolSample);

	labStyle = new Label();
	labStyle->SetName(_T("labStyle"));
	labStyle->SetText(_T("Style:"));
	labStyle->SetLocation(100, nTop + 2);
	labStyle->SetSize(35, 17);
	this->AddControl(labStyle);

	lstStyle = new ComboBox();
	lstStyle->SetName(_T("lstStyle"));
	lstStyle->SetLocation(135, nTop);
	lstStyle->SetSize(140, 200);
	this->AddControl(lstStyle);

	btnStyleAdd = new Button();
	btnStyleAdd->SetName(_T("btnStyleAdd"));
	btnStyleAdd->SetText(_T("Add"));
	btnStyleAdd->Click += EventHandler((Object*)this, (EventFun)&ControlSam::btnStyleAdd2_Click);
	btnStyleAdd->SetLocation(277, nTop);
	btnStyleAdd->SetSize(30, 21);
	this->AddControl(btnStyleAdd);

	btnStyleRemove = new Button();
	btnStyleRemove->SetName(_T("btnStyleRemove"));
	btnStyleRemove->SetText(_T("Remove"));
	btnStyleRemove->Click += EventHandler((Object*)this, (EventFun)&ControlSam::btnStyleRemove2_Click);
	btnStyleRemove->SetLocation(309, nTop);
	btnStyleRemove->SetSize(50, 21);
	this->AddControl(btnStyleRemove);

	nTop += 22;
	labSample1 = new Label();
	labSample1->SetName(_T("labSample1"));
	labSample1->SetText(_T("LabelNormal"));
	labSample1->SetIcon(imageList, IDI_ICON5);
	labSample1->SetLocation(0, nTop);
	labSample1->SetSize(100, 17);
	this->AddControl(labSample1);

	labSample2 = new Label();
	labSample2->SetName(_T("labSample2"));
	labSample2->SetText(_T("LabelOwnerDraw"));
	labSample2->SetIcon(imageList, IDI_ICON5);
	labSample2->SetLocation(130, nTop);
	labSample2->SetSize(100, 17);
	labSample2->SetStyle(StyleType::Style, 0, SS_OWNERDRAW);
	this->AddControl(labSample2);

	btnSample1 = new Button();
	btnSample1->SetName(_T("btnSample1"));
	btnSample1->SetText(_T("ButtonNormal"));
	btnSample1->SetIcon(imageList, IDI_ICON5);
	btnSample1->SetLocation(260, nTop);
	btnSample1->SetSize(100, 17);
	this->AddControl(btnSample1);

	btnSample2 = new Button();
	btnSample2->SetName(_T("btnSample2"));
	btnSample2->SetText(_T("ButtonOwnerDraw"));
	btnSample2->SetIcon(imageList, IDI_ICON5);
	btnSample2->SetLocation(370, nTop);
	btnSample2->SetSize(100, 17);
	btnSample2->SetStyle(StyleType::Style, 0, BS_OWNERDRAW);
	this->AddControl(btnSample2);

	nTop += 22;
	toolSample1 = new ToolItem();
	toolSample1->SetName(_T("toolButtonSample1"));
	toolSample1->SetText(_T("toolButtonSample1"));
	toolSample1->SetLocation(0, nTop);
	toolSample1->SetSize(100, 17);
	toolSample1->SetOuterStyle(OuterStyle::AlwaysNoEdge, true);
	toolSample1->SetHotTextColor(::GetSysColor(COLOR_BTNTEXT));
	toolSample1->SetHotBackColor(::GetSysColor(COLOR_BTNFACE));
	this->AddControl(toolSample1);

	toolSample2 = new ToolItem();
	toolSample2->SetName(_T("toolButtonSample2"));
	toolSample2->SetText(_T("toolButtonSample2"));
	toolSample2->SetLocation(130, nTop);
	toolSample2->SetSize(100, 17);
	toolSample2->SetOuterStyle(OuterStyle::AlwaysNoEdge, true);
	toolSample2->SetHotTextColor(RGB(0, 0, 255));
	toolSample2->SetHotBackColor(RGB(220, 220, 220));
	this->AddControl(toolSample2);

	toolSample3 = new ToolItem();
	toolSample3->SetName(_T("toolButtonSample3"));
	toolSample3->SetText(_T("toolButtonSample3"));
	toolSample3->SetLocation(260, nTop);
	toolSample3->SetSize(100, 17);
	toolSample3->SetStyle(StyleType::Style, 0, WS_TABSTOP);
	toolSample3->SetOuterStyle(OuterStyle::AlwaysEdge, true);
	toolSample3->SetHotTextColor(::GetSysColor(COLOR_BTNTEXT));
	toolSample3->SetHotBackColor(::GetSysColor(COLOR_BTNFACE));
	this->AddControl(toolSample3);

	toolSample4 = new ToolItem();
	toolSample4->SetName(_T("toolButtonSample4"));
	toolSample4->SetText(_T("toolButtonSample4"));
	toolSample4->SetLocation(370, nTop);
	toolSample4->SetSize(100, 22);
	toolSample4->SetStyle(StyleType::Style, 0, WS_TABSTOP);
	//toolSample4->SetOuterStyle(OuterStyle::AutoSize, false);
	toolSample4->SetStyle(StyleType::Outer, OuterStyle::AlwaysEdge, true);
	toolSample4->SetHotTextColor(RGB(255, 0, 0));
	toolSample4->SetHotBackColor(RGB(220, 220, 220));
	this->AddControl(toolSample4);

	nTop += 22;
	this->SetName(_T("ControlSam"));
	this->SetText(_T("Control Sample"));
	this->SetContextMenu(coneMenuConfig);
	this->SetLocation(466, 666);
	this->SetSize(510, nTop + 35);
	//this->SetCenter();

	this->Load += EventHandler((Object*)this, (EventFun)&ControlSam::Form_Load);
}

void ControlSam::Form_Load(Object* sender, EventArgs* e)
{
	//g_hInst = CC_APP_GetInstance(); // Store instance handle in our global variable
	//g_hwndMain = _pClient->m_hWnd;

	//g_hwndPropSheet = CreatePropSheet();
	//if(-1 == (int)g_hwndPropSheet)
	//{
	//	return;
	//}

	//SetWindowLong(g_hwndPropSheet, GWL_STYLE, WS_CHILD);
	//::SetParent(g_hwndPropSheet, g_hwndMain);
	//SetWindowLong(g_hwndPropSheet, GWL_EXSTYLE, WS_EX_CONTROLPARENT);

	//InitPPSheetSize();

	//ShowWindow(g_hwndMain, SW_SHOW);
	//UpdateWindow(g_hwndMain);
	////SetWindowPos(g_hwndPropSheet, NULL, 0, 0, 700, 700, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_SHOWWINDOW);
	////MoveWindow(g_hwndPropSheet, 20, 30, 400, 600, TRUE);


	ToolItem* toolItem1 = new ToolItem();
	toolItem1->SetName(_T("toolItem1"));
	toolItem1->SetText(_T("tool1"));
	//toolItem1->SetIcon(imageList, IDI_ICON5);
	toolItem1->Click += EventHandler((Object*)this, (EventFun)&ControlSam::test1_Click);
	toolItem1->Popup += EventHandler((Object*)this, (EventFun)&ControlSam::test1_Pupup);
	toolItem1->SetContextMenu(coneMenuConfig);
	toolbar1->AddControl(toolItem1);

	lstStyle->ItemAdd(_T("Set ICON"));
	lstStyle->ItemAdd(_T("Set TEXT NULL"));
	lstStyle->ItemAdd(_T("Set Menu"));
	lstStyle->ItemAdd(_T("Set Disabled"));
	lstStyle->ItemAdd(_T("Break"));
	lstStyle->ItemAdd(_T("Blank"));
	lstStyle->ItemAdd(_T("ContextMenuButton"));
	lstStyle->ItemAdd(_T("PushButton"));
	lstStyle->ItemAdd(_T("AlwaysEdge"));
	lstStyle->ItemAdd(_T("Always No Edge"));

	lstType->ItemAdd(_T("ToolItem"));
	lstType->ItemAdd(_T("Break"));
	lstType->ItemAdd(_T("Blank"));
	lstType->ItemAdd(_T("ContextMenuButton"));
	lstType->ItemAdd(_T("PushButton"));
	lstType->ItemAdd(_T("TextBox")); //5
	lstType->ItemAdd(_T("ComboBox"));
	lstType->ItemAdd(_T("CheckBox"));
}

void ControlSam::test1_Click(Object* sender, EventArgs* e)
{
}

void ControlSam::test1_Pupup(Object* sender, EventArgs* e)
{
}

void ControlSam::btnExit_Click(Object* sender, EventArgs* e)
{
	this->EndDialog(DialogResult::OK);
}

void ControlSam::btnDeEnable_Click(Object* sender, EventArgs* e)
{
	int nInd = editIndex->GetTextInt();
	if(nInd < 0 || nInd > toolbar1->GetControlCount() - 1)
	{
		::MessageBox(m_hWnd, _T("index is not right."), NULL, MB_OK);
		return;
	}
	toolbar1->GetControl(nInd)->SetEnabled( !toolbar1->GetControl(nInd)->GetEnabled() );
}

void ControlSam::btnAdd_Click(Object* sender, EventArgs* e)
{
	int nInd = lstType->GetSelectedIndex();
	const TCHAR* tc = editText->GetText();
	if(nInd <= 4)
	{
		ToolItem* ctl = new ToolItem();
		if(nInd <= 0)
		{
			ctl->SetText(tc);
			if(chkIcon->GetChecked())
			{
				//ctl->SetIcon(IDI_ICON2);
				//ctl->SetIcon(IDI_ICON3, IconType::Gray);
				//ctl->SetIcon(IDI_ICON4, IconType::Hot);
				ctl->SetIcon(imageList, IDI_ICON4);
			}
			if(chkRMenu->GetChecked())
			{
				ctl->SetContextMenu(coneMenuConfig);
			}
		}
		else if(nInd == 1)
		{
			ctl->SetOuterStyle(OuterStyle::Break, true, true);
		}
		else if(nInd == 2)
		{
			ctl->SetOuterStyle(OuterStyle::Blank, true, true);
		}
		else if(nInd == 3)
		{
			ctl->SetText(tc);
			if(chkIcon->GetChecked())
			{
				//ctl->SetIcon(IDI_ICON2);
				//ctl->SetIcon(IDI_ICON3, IconType::Gray);
				//ctl->SetIcon(IDI_ICON4, IconType::Hot);
				ctl->SetIcon(imageList, IDI_ICON2);
			}
			if(chkRMenu->GetChecked())
			{
				ctl->SetContextMenu(coneMenuConfig);
			}
			ctl->SetOuterStyle(OuterStyle::ContextMenuButton, true, true);
		}
		else if(nInd == 4)
		{
			ctl->SetText(tc);
			if(chkIcon->GetChecked())
			{
				//ctl->SetIcon(IDI_ICON2);
				//ctl->SetIcon(IDI_ICON3, IconType::Gray);
				//ctl->SetIcon(IDI_ICON4, IconType::Hot);
				ctl->SetIcon(imageList, IDI_ICON2);
			}
			ctl->SetOuterStyle(OuterStyle::PushButton, true, true);
		}
		toolbar1->AddControl(ctl);
	}
	else if(nInd == 5)
	{
		TextBox* ctl = new TextBox();
		ctl->SetText(tc);
		if(chkRMenu->GetChecked())
		{
			ctl->SetContextMenu(coneMenuConfig);
		}
		toolbar1->AddControl(ctl);
	}
	else if(nInd == 6)
	{
		ComboBox* ctl = new ComboBox();
		ctl->SetText(tc);
		if(chkRMenu->GetChecked())
		{
			ctl->SetContextMenu(coneMenuConfig);
		}
		toolbar1->AddControl(ctl);
	}
	else if(nInd == 7)
	{
		CheckBox* ctl = new CheckBox();
		ctl->SetText(tc);
		if(chkRMenu->GetChecked())
		{
			ctl->SetContextMenu(coneMenuConfig);
		}
		toolbar1->AddControl(ctl);
	}
}

void ControlSam::btnMod_Click(Object* sender, EventArgs* e)
{
	int nInd = editIndex->GetTextInt();
	if(nInd < 0 || nInd > toolbar1->GetControlCount() - 1)
	{
		::MessageBox(m_hWnd, _T("index is not right."), NULL, MB_OK);
		return;
	}
	Control* ctl = toolbar1->GetControl(nInd);

	const TCHAR* tc = editText->GetText();
	ctl->SetText(tc);
	if(chkIcon->GetChecked())
	{
		//ctl->SetIcon(IDI_ICON2);
		//ctl->SetIcon(IDI_ICON3, IconType::Gray);
		//ctl->SetIcon(IDI_ICON4, IconType::Hot);
		ctl->SetIcon(imageList, IDI_ICON4);
	}
	else
	{
		ctl->SetIcon(NULL, -1);
	}
	if(chkRMenu->GetChecked())
	{
		ctl->SetContextMenu(coneMenuConfig);
	}
	else
	{
		ctl->SetContextMenu(NULL);
	}
}

void ControlSam::btnDel_Click(Object* sender, EventArgs* e)
{
	int nInd = editIndex->GetTextInt();
	if(nInd < 0 || nInd > toolbar1->GetControlCount() - 1)
	{
		::MessageBox(m_hWnd, _T("index is not right."), NULL, MB_OK);
		return;
	}
	Control* ctl = toolbar1->RemoveControl(nInd);
	delete ctl;
}

void ControlSam::btnReCreate_Click(Object* sender, EventArgs* e)
{
}


void ControlSam::chkBarRMenu_Click(Object* sender, EventArgs* e)
{
	if(chkBarRMenu->GetChecked())
	{
		toolbar1->SetContextMenu(coneMenuConfig);
	}
	else
	{
		toolbar1->SetContextMenu(NULL);
	}
}

void ControlSam::chkBarBorder_Click(Object* sender, EventArgs* e)
{
	if(chkBarBorder->GetChecked())
	{
	//	toolbar1->SetFloatable(true);
	}
	else
	{
	//	toolbar1->SetFloatable(false);
	}
}

void ControlSam::chkTraceMParent_Click(Object* sender, EventArgs* e)
{
}

void ControlSam::chkBigSize_Click(Object* sender, EventArgs* e)
{
	if(chkBigSize->GetChecked())
	{
		toolbar1->SetSize(500, 60);
	}
	else
	{
		toolbar1->SetSize(500, 30);
	}
}

void ControlSam::chkDisabled_Click(Object* sender, EventArgs* e)
{
	if(chkDisabled->GetChecked())
	{
		toolbar1->SetEnabled(false);
	}
	else
	{
		toolbar1->SetEnabled(true);
	}
}

void ControlSam::btnStyleAdd2_Click(Object* sender, EventArgs* e)
{
	int nInd = lstStyle->GetSelectedIndex();
	if(nInd < 0)
	{
		::MessageBox(m_hWnd, _T("need selected one item."), NULL, MB_OK);
		return;
	}
	//lstStyle->ItemAdd(_T("Set ICON"));
	//lstStyle->ItemAdd(_T("Set TEXT"));
	//lstStyle->ItemAdd(_T("Set Menu"));
	//lstStyle->ItemAdd(_T("Set Disabled"));
	//lstStyle->ItemAdd(_T("Break")); 4
	//lstStyle->ItemAdd(_T("Blank")); 5
	//lstStyle->ItemAdd(_T("ContextMenuButton")); 6
	//lstStyle->ItemAdd(_T("PushButton"));
	//lstStyle->ItemAdd(_T("AlwaysRECT"));
	if(nInd == 0)
	{
		//toolSample->SetIcon(IDI_ICON2);
		//toolSample->SetIcon(IDI_ICON3, IconType::Gray);
		//toolSample->SetIcon(IDI_ICON4, IconType::Hot);
		toolSample->SetIcon(imageList, IDI_ICON4);
	}
	else if(nInd == 1)
	{
		toolSample->SetText(_T("ToolSample"));
	}
	else if(nInd == 2)
	{
		toolSample->SetContextMenu(coneMenuConfig);
	}
	else if(nInd == 3)
	{
		toolSample->SetEnabled(false);
	}
	else if(nInd == 4)
	{
		toolSample->SetOuterStyle(OuterStyle::Break, true, true);
	}
	else if(nInd == 5)
	{
		toolSample->SetOuterStyle(OuterStyle::Blank, true, true);
	}
	else if(nInd == 6)
	{
		toolSample->SetOuterStyle(OuterStyle::ContextMenuButton, true, true);
	}
	else if(nInd == 7)
	{
		toolSample->SetOuterStyle(OuterStyle::PushButton, true, true);
	}
	else if(nInd == 8)
	{
		toolSample->SetOuterStyle(OuterStyle::AlwaysEdge, true, true);
	}
	else if(nInd == 9)
	{
		toolSample->SetOuterStyle(OuterStyle::AlwaysNoEdge, true, true);
	}
}

void ControlSam::btnStyleRemove2_Click(Object* sender, EventArgs* e)
{
	int nInd = lstStyle->GetSelectedIndex();
	if(nInd < 0)
	{
		::MessageBox(m_hWnd, _T("need selected one item."), NULL, MB_OK);
		return;
	}
	if(nInd == 0)
	{
		toolSample->SetIcon(NULL, -1);
	}
	else if(nInd == 1)
	{
		toolSample->SetText(NULL);
	}
	else if(nInd == 2)
	{
		toolSample->SetContextMenu(NULL);
	}
	else if(nInd == 3)
	{
		toolSample->SetEnabled(true);
	}
	else if(nInd == 4)
	{
		toolSample->SetOuterStyle(OuterStyle::Break, false, true);
	}
	else if(nInd == 5)
	{
		toolSample->SetOuterStyle(OuterStyle::Blank, false, true);
	}
	else if(nInd == 6)
	{
		toolSample->SetOuterStyle(OuterStyle::ContextMenuButton, false, true);
	}
	else if(nInd == 7)
	{
		toolSample->SetOuterStyle(OuterStyle::PushButton, false, true);
	}
	else if(nInd == 8)
	{
		toolSample->SetOuterStyle(OuterStyle::AlwaysEdge, false, true);
	}
	else if(nInd == 9)
	{
		toolSample->SetOuterStyle(OuterStyle::AlwaysNoEdge, false, true);
	}
}
