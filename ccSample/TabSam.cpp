#include "TabSam.h"
using namespace cc;
using namespace cc::win;

TabSam::TabSam()
{
	InitializeComponent();
}

void TabSam::InitializeComponent()
{
	int nTop = 1;
	imageList = (ImageList*)CC_APP_GetVarious(_T("ImageList"));
	contextMenu1 = (ContextMenu*)CC_APP_GetVarious(_T("ContextMenu1"));
	coneMenuConfig = (ContextMenu*)CC_APP_GetVarious(_T("coneMenuConfig"));
	contextMenu3 = (ContextMenu*)CC_APP_GetVarious(_T("ContextMenu3"));
	contextMenu4 = (ContextMenu*)CC_APP_GetVarious(_T("ContextMenu4"));

	labText = new Label();
	labText->SetName(_T("labText"));
	labText->SetText(_T("Text:"));
	labText->SetLocation(2, nTop + 2);
	labText->SetSize(25, 17);
	this->AddControl(labText);

	editText = new TextBox();
	editText->SetName(_T("editText"));
	editText->SetLocation(28, nTop);
	editText->SetSize(370, 17);
	editText->Anchor = (AnchorStyles::Type)(AnchorStyles::Left | AnchorStyles::Top | AnchorStyles::Right);
	this->AddControl(editText);

	chkIcon = new CheckBox();
	chkIcon->SetName(_T("chkIcon"));
	chkIcon->SetText(_T("Icon"));
	chkIcon->SetLocation(400, nTop - 2);
	chkIcon->SetSize(50, 20);
	chkIcon->SetOuterStyle(OuterStyle::AutoConfig, true);
	chkIcon->Anchor = (AnchorStyles::Type)(AnchorStyles::Right | AnchorStyles::Top);
	this->AddControl(chkIcon);

	chkTransparent = new CheckBox();
	chkTransparent->SetName(_T("chkTransparent"));
	chkTransparent->SetText(_T("Transparent"));
	chkTransparent->SetLocation(400, nTop - 2 + 20);
	chkTransparent->SetSize(50, 20);
	chkTransparent->SetOuterStyle(OuterStyle::AutoConfig, true);
	chkTransparent->Anchor = (AnchorStyles::Type)(AnchorStyles::Right | AnchorStyles::Top);
	this->AddControl(chkTransparent);

	chkIE = new CheckBox();
	chkIE->SetName(_T("chkIE"));
	chkIE->SetText(_T("IE"));
	chkIE->SetLocation(463, nTop - 2 + 20);
	chkIE->SetSize(40, 20);
	chkIE->SetOuterStyle(OuterStyle::AutoConfig, true);
	chkIE->Anchor = (AnchorStyles::Type)(AnchorStyles::Right | AnchorStyles::Top);
	this->AddControl(chkIE);

	toolPageColor = new PickColorCtl();
	toolPageColor->SetName(_T("toolPageColor"));
	toolPageColor->SetText(_T("TitleColor"));
	//toolPageColor->SetOuterStyle(OuterStyle::TransparentParentErase, false);
	//toolPageColor->SetOuterStyle(OuterStyle::AutoSize, false);
	//toolPageColor->SetOuterStyle(OuterStyle::AlwaysNoEdge, true);
	toolPageColor->SetLocation(463, nTop - 2);
	toolPageColor->SetSize(40, 18);
	toolPageColor->SetBackground(RGB(255, 0, 0));
	//toolPageColor->SetHotBackColor(RGB(255, 0, 0));
	toolPageColor->Anchor = (AnchorStyles::Type)(AnchorStyles::Right | AnchorStyles::Top);
	toolPageColor->Click += EventHandler((Object*)this, (EventFun)&TabSam::Control_Click);
	this->AddControl(toolPageColor);

	nTop += 19;
	btnAdd = new Button();
	btnAdd->SetName(_T("btnAdd"));
	btnAdd->SetText(_T("Add"));
	btnAdd->Click += EventHandler((Object*)this, (EventFun)&TabSam::btnAdd_Click);
	btnAdd->SetLocation(2, nTop);
	btnAdd->SetSize(50, 21);
	this->AddControl(btnAdd);

	btnMod = new Button();
	btnMod->SetName(_T("btnMod"));
	btnMod->SetText(_T("Modify"));
	btnMod->Click += EventHandler((Object*)this, (EventFun)&TabSam::btnMod_Click);
	btnMod->SetLocation(54, nTop);
	btnMod->SetSize(50, 21);
	this->AddControl(btnMod);

	btnDel = new Button();
	btnDel->SetName(_T("btnDel"));
	btnDel->SetText(_T("Del"));
	btnDel->Click += EventHandler((Object*)this, (EventFun)&TabSam::btnDel_Click);
	btnDel->SetLocation(106, nTop);
	btnDel->SetSize(50, 21);
	this->AddControl(btnDel);

	btnDeEnable = new Button();
	btnDeEnable->SetName(_T("btnDeEnable"));
	btnDeEnable->SetText(_T("!Enable"));
	btnDeEnable->Click += EventHandler((Object*)this, (EventFun)&TabSam::btnDeEnable_Click);
	btnDeEnable->SetLocation(160, nTop);
	btnDeEnable->SetSize(60, 21);
	this->AddControl(btnDeEnable);

	btnAlign = new Button();
	btnAlign->SetName(_T("btnAlign"));
	btnAlign->SetText(_T("ChgAlign"));
	btnAlign->Click += EventHandler((Object*)this, (EventFun)&TabSam::btnAlign_Click);
	btnAlign->SetLocation(222, nTop);
	btnAlign->SetSize(60, 21);
	this->AddControl(btnAlign);

	btnAppearance = new Button();
	btnAppearance->SetName(_T("btnAppearance"));
	btnAppearance->SetText(_T("ChgAppearance"));
	btnAppearance->Click += EventHandler((Object*)this, (EventFun)&TabSam::btnAppearance_Click);
	btnAppearance->SetLocation(284, nTop);
	btnAppearance->SetSize(88, 21);
	this->AddControl(btnAppearance);

	nTop += 22;
	tab1 = new TabControl();
	tab1->SetLocation(2, nTop);
	tab1->SetSize(489, 145);
	tab1->HeadDoubleClick += EventHandler((Object*)this, (EventFun)&TabSam::tabHead_DoubleClick);
	tab1->Anchor = (AnchorStyles::Type)(AnchorStyles::Top | AnchorStyles::Left | AnchorStyles::Right | AnchorStyles::Bottom);
	this->AddControl(tab1);

	nTop += 148;
	btnSetRMenu = new Button();
	btnSetRMenu->SetName(_T("btnSetRMenu"));
	btnSetRMenu->SetText(_T("SetRightMenu"));
	btnSetRMenu->Click += EventHandler((Object*)this, (EventFun)&TabSam::btnSetRMenu_Click);
	btnSetRMenu->SetLocation(2, nTop);
	btnSetRMenu->SetSize(85, 21);
	btnSetRMenu->Anchor = (AnchorStyles::Type)(AnchorStyles::Left | AnchorStyles::Bottom);
	this->AddControl(btnSetRMenu);

	btnUnSetRMenu = new Button();
	btnUnSetRMenu->SetName(_T("btnUnSetRMenu"));
	btnUnSetRMenu->SetText(_T("UnSetRightMenu"));
	btnUnSetRMenu->Click += EventHandler((Object*)this, (EventFun)&TabSam::btnUnSetRMenu_Click);
	btnUnSetRMenu->SetLocation(87, nTop);
	btnUnSetRMenu->SetSize(100, 21);
	btnUnSetRMenu->Anchor = (AnchorStyles::Type)(AnchorStyles::Left | AnchorStyles::Bottom);
	this->AddControl(btnUnSetRMenu);

	btnExit = new Button();
	btnExit->SetName(_T("btnExit"));
	btnExit->SetText(_T("Exit"));
	btnExit->Click += EventHandler((Object*)this, (EventFun)&TabSam::btnExit_Click);
	btnExit->SetLocation(189, nTop);
	btnExit->SetSize(50, 21);
	btnExit->Anchor = (AnchorStyles::Type)(AnchorStyles::Left | AnchorStyles::Bottom);
	this->AddControl(btnExit);

	btnReCreate = new Button();
	btnReCreate->SetName(_T("btnReCreate"));
	btnReCreate->SetText(_T("ReCreateTab"));
	btnReCreate->Click += EventHandler((Object*)this, (EventFun)&TabSam::btnReCreate_Click);
	btnReCreate->SetLocation(250, nTop);
	btnReCreate->SetSize(80, 21);
	btnReCreate->Anchor = (AnchorStyles::Type)(AnchorStyles::Left | AnchorStyles::Bottom);
	this->AddControl(btnReCreate);

	chkOwnDraw = new CheckBox();
	chkOwnDraw->SetName(_T("chkOwnDraw"));
	chkOwnDraw->SetText(_T("OwnDraw"));
	chkOwnDraw->Click += EventHandler((Object*)this, (EventFun)&TabSam::chkOwnDraw_Click);
	chkOwnDraw->SetLocation(335, nTop);
	chkOwnDraw->SetSize(80, 21);
	chkOwnDraw->Anchor = (AnchorStyles::Type)(AnchorStyles::Left | AnchorStyles::Bottom);
	this->AddControl(chkOwnDraw);

	nTop += 22;
	this->SetName(_T("TabSam"));
	this->SetText(_T("TabPage&TabControl Sample"));
	this->SetContextMenu(coneMenuConfig);
	this->SetLocation(466, 366);
	this->SetSizeBaseOnClient(505, nTop);
	//this->SetCenter();

	this->Load += EventHandler((Object*)this, (EventFun)&TabSam::Form_Load);
	this->Closed += EventHandler((Object*)this, (EventFun)&TabSam::Form_Closed);
}

void TabSam::Form_Load(Object* sender, EventArgs* e)
{
	//load config
	this->Config(true, *CC_APP_GetIni(), cc::Str(_T("Main")));

	AddPage();
	AddPage();
}

void TabSam::Form_Closed(Object* sender, EventArgs* e)
{
	//save config
	this->Config(false, *CC_APP_GetIni(), cc::Str(_T("Main")));
}

void TabSam::Control_Click(Object* sender, EventArgs* e)
{
	if(sender == toolPageColor)
	{
		COLORREF cor = toolPageColor->GetBackground();
		if(cc::Util::selectColor(cor, true, NULL, -1, m_hWnd))
		{
			toolPageColor->SetBackground(cor);
			//toolPageColor->SetHotBackColor(cor);
		}
	}
}

void TabSam::btnExit_Click(Object* sender, EventArgs* e)
{
	this->EndDialog(DialogResult::OK);
}

void TabSam::menuCnt1_Click(Object* sender, EventArgs* e)
{
	cc::Log::fatal(_T("menuCnt1_Click"));
}

void TabSam::AddPage()
{
	const TCHAR* tc = editText->GetText();
	TabPage* page = new TabPage();
	//page->SetStyle(0, WS_THICKFRAME);
	page->SetClientSize(100, 100);
	page->SetText(tc);
	page->SetTextColor(toolPageColor->GetBackground());
	if(chkIcon->GetChecked())
	{
		page->SetIcon(imageList, IDI_ICON2);
	}
	//page->SetOuterStyle(OuterStyle::TransparentTop, !chkTransparent->GetChecked());
	page->SetOuterStyle(OuterStyle::TransparentParentErase, chkTransparent->GetChecked());
	tab1->AddPage(page);

	TextBox* edit1 = new TextBox();
	edit1->SetName(_T("edit1"));
	edit1->SetText(tc);
	edit1->Click += EventHandler((Object*)this, (EventFun)&TabSam::btnReCreate_Click);
	edit1->SetLocation(1, 2);
	edit1->SetSize(98, 21);
	edit1->Anchor = (AnchorStyles::Type)(AnchorStyles::Top | AnchorStyles::Left | AnchorStyles::Right);
	edit1->SetOuterStyle(OuterStyle::TransparentParentBrush, chkTransparent->GetChecked());
	page->AddControl(edit1);

	if(chkIE->GetChecked())
	{
		IEControl* ie1;
		ie1 = new IEControl();
		ie1->SetName(_T("ie1"));
		ie1->SetLocation(0, 26);
		ie1->SetSize(98, 73);
		ie1->Anchor = (AnchorStyles::Type)(AnchorStyles::Top | AnchorStyles::Left | AnchorStyles::Right | AnchorStyles::Bottom);
		page->AddControl(ie1);
		cc::Str str = cc::Str(cc::LangCode::UNICODE16, tc);
		if(str.IndexOf(L".") > 0)
		{
			ie1->Navigate((wchar_t*)str.GetBuf());
		}
	}
	else
	{
		TextBox* edit2 = new TextBox();
		edit2->SetName(_T("edit2"));
		edit2->SetText(tc);
		edit2->SetLocation(0, 26);
		edit2->SetSize(98, 73);
		edit2->SetMultiline(true);
		edit2->Anchor = (AnchorStyles::Type)(AnchorStyles::Top | AnchorStyles::Left | AnchorStyles::Right | AnchorStyles::Bottom);
		page->AddControl(edit2);
		edit2->SetOuterStyle(OuterStyle::TransparentParentBrush, chkTransparent->GetChecked());
	}
}

void TabSam::btnDeEnable_Click(Object* sender, EventArgs* e)
{
	int nInd = tab1->GetSelectedIndex();
	if(nInd < 0)
	{
		::MessageBox(m_hWnd, _T("need selected one page."), NULL, MB_OK);
		return;
	}
	tab1->GetPage(nInd)->SetEnabled( !tab1->GetPage(nInd)->GetEnabled() );
}

void TabSam::btnAdd_Click(Object* sender, EventArgs* e)
{
	AddPage();
}

void TabSam::btnMod_Click(Object* sender, EventArgs* e)
{
	int nInd = tab1->GetSelectedIndex();
	if(nInd < 0)
	{
		::MessageBox(m_hWnd, _T("need selected one page."), NULL, MB_OK);
		return;
	}
	TabPage* page = tab1->GetPage(nInd);
	TextBox* edit1 = (TextBox*)page->GetControl(_T("edit1"));
	TextBox* edit2 = (TextBox*)page->GetControl(_T("edit2"));

	const TCHAR* tc = editText->GetText();
	page->SetText(tc);
	if(chkIcon->GetChecked())
	{
		page->SetIcon(imageList, IDI_ICON2);
	}
	else
	{
		page->SetIcon(NULL, -1);
	}
	//page->SetOuterStyle(OuterStyle::TransparentTop, !chkTransparent->GetChecked());
	page->SetOuterStyle(OuterStyle::TransparentParentErase, chkTransparent->GetChecked());
	page->SetTextColor(toolPageColor->GetBackground());
	edit1->SetText(tc);
	edit1->SetOuterStyle(OuterStyle::TransparentParentBrush, chkTransparent->GetChecked());
	if(edit2 != NULL)
	{
		edit2->SetOuterStyle(OuterStyle::TransparentParentBrush, chkTransparent->GetChecked());
	}
}

void TabSam::btnDel_Click(Object* sender, EventArgs* e)
{
	int nInd = tab1->GetSelectedIndex();
	if(nInd < 0)
	{
		::MessageBox(m_hWnd, _T("need selected one page."), NULL, MB_OK);
		return;
	}
	TabPage* page = tab1->RemovePage(nInd);
	delete page;
}

void TabSam::btnReCreate_Click(Object* sender, EventArgs* e)
{
	int n1 = tab1->GetTabAlignment();
	int n2 = tab1->GetTabAppearance();
	RECT rect = tab1->GetBounds();

	this->RemoveControl(tab1);
	tab1->Dispose();
	delete tab1;
	tab1 = new TabControl();
	tab1->SetBounds(rect);
	tab1->Anchor = (AnchorStyles::Type)(AnchorStyles::Top | AnchorStyles::Left | AnchorStyles::Right | AnchorStyles::Bottom);
	tab1->SetTabAlignment((TabAlignment::Type)n1);
	tab1->SetTabAppearance((TabAppearance::Type)n2);
	chkOwnDraw_Click(NULL, NULL);
	this->AddControl(tab1);
}

void TabSam::chkOwnDraw_Click(Object* sender, EventArgs* e)
{
	bool isOn = chkOwnDraw->GetChecked();
	if(!isOn)
	{
		tab1->SetStyle(StyleType::Style, TCS_OWNERDRAWFIXED, 0);
	}
	else
	{
		tab1->SetStyle(StyleType::Style, 0, TCS_OWNERDRAWFIXED);
	}

}
void TabSam::btnSetRMenu_Click(Object* sender, EventArgs* e)
{
	tab1->SetHeadContextMenu(contextMenu3);
	tab1->SetContextMenu(contextMenu1);
}

void TabSam::btnUnSetRMenu_Click(Object* sender, EventArgs* e)
{
	tab1->SetHeadContextMenu(NULL);
	tab1->SetContextMenu(NULL);
}

void TabSam::btnAlign_Click(Object* sender, EventArgs* e)
{
	if((int)tab1->GetTabAlignment() == 4)
	{
		tab1->SetTabAlignment((TabAlignment::Type)0);
	}
	else
	{
		tab1->SetTabAlignment((TabAlignment::Type)((int)tab1->GetTabAlignment() + 1));
	}
}

void TabSam::btnAppearance_Click(Object* sender, EventArgs* e)
{
	if((int)tab1->GetTabAppearance() == 2)
	{
		tab1->SetTabAppearance((TabAppearance::Type)0);
	}
	else
	{
		tab1->SetTabAppearance((TabAppearance::Type)((int)tab1->GetTabAppearance() + 1));
	}
}

void TabSam::tabHead_DoubleClick(Object* sender, EventArgs* e)
{
	::MessageBox(m_hWnd, _T("tab Head DoubleClick"), NULL, MB_OK);
}
