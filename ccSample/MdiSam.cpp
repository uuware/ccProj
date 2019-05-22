#include "MdiSam.h"
using namespace cc;
using namespace cc::win;

MdiSam::MdiSam()
{
	InitializeComponent();
}

void MdiSam::InitializeComponent()
{
	int nTop = 1;
	imageList = (ImageList*)CC_APP_GetVarious(_T("ImageList"));
	contextMenu1 = (ContextMenu*)CC_APP_GetVarious(_T("ContextMenu1"));
	contextMenu2 = (ContextMenu*)CC_APP_GetVarious(_T("ContextMenu2"));
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
	editText->SetSize(380, 17);
	editText->Anchor = (AnchorStyles::Type)(AnchorStyles::Left | AnchorStyles::Top | AnchorStyles::Right);
	this->AddControl(editText);

	chkIcon = new CheckBox();
	chkIcon->SetName(_T("chkIcon"));
	chkIcon->SetText(_T("Icon"));
	chkIcon->SetLocation(410, nTop - 2);
	chkIcon->SetSize(70, 20);
	chkIcon->Anchor = (AnchorStyles::Type)(AnchorStyles::Right | AnchorStyles::Top);
	this->AddControl(chkIcon);

	nTop += 19;
	btnAdd = new Button();
	btnAdd->SetName(_T("btnAdd"));
	btnAdd->SetText(_T("Add"));
	btnAdd->Click += EventHandler((Object*)this, (EventFun)&MdiSam::btnAdd_Click);
	btnAdd->SetLocation(2, nTop);
	btnAdd->SetSize(50, 21);
	this->AddControl(btnAdd);

	btnMod = new Button();
	btnMod->SetName(_T("btnMod"));
	btnMod->SetText(_T("Modify"));
	btnMod->Click += EventHandler((Object*)this, (EventFun)&MdiSam::btnMod_Click);
	btnMod->SetLocation(54, nTop);
	btnMod->SetSize(50, 21);
	this->AddControl(btnMod);

	btnDel = new Button();
	btnDel->SetName(_T("btnDel"));
	btnDel->SetText(_T("Del"));
	btnDel->Click += EventHandler((Object*)this, (EventFun)&MdiSam::btnDel_Click);
	btnDel->SetLocation(106, nTop);
	btnDel->SetSize(50, 21);
	this->AddControl(btnDel);

	btnDeEnable = new Button();
	btnDeEnable->SetName(_T("btnDeEnable"));
	btnDeEnable->SetText(_T("!Enable"));
	btnDeEnable->Click += EventHandler((Object*)this, (EventFun)&MdiSam::btnDeEnable_Click);
	btnDeEnable->SetLocation(160, nTop);
	btnDeEnable->SetSize(60, 21);
	this->AddControl(btnDeEnable);

	btnAlign = new Button();
	btnAlign->SetName(_T("btnAlign"));
	btnAlign->SetText(_T("ChgAlign"));
	btnAlign->Click += EventHandler((Object*)this, (EventFun)&MdiSam::btnAlign_Click);
	btnAlign->SetLocation(222, nTop);
	btnAlign->SetSize(60, 21);
	this->AddControl(btnAlign);

	btnAppearance = new Button();
	btnAppearance->SetName(_T("btnAppearance"));
	btnAppearance->SetText(_T("ChgAppearance"));
	btnAppearance->Click += EventHandler((Object*)this, (EventFun)&MdiSam::btnAppearance_Click);
	btnAppearance->SetLocation(284, nTop);
	btnAppearance->SetSize(88, 21);
	this->AddControl(btnAppearance);

	nTop += 22;
	tab1 = new TabControl();
	tab1->SetLocation(2, nTop);
	tab1->SetSize(489, 145);
	tab1->HeadDoubleClick += EventHandler((Object*)this, (EventFun)&MdiSam::tabHead_DoubleClick);
	tab1->Anchor = (AnchorStyles::Type)(AnchorStyles::Top | AnchorStyles::Left | AnchorStyles::Right | AnchorStyles::Bottom);
	this->AddControl(tab1);

	nTop += 148;
	btnSetRMenu = new Button();
	btnSetRMenu->SetName(_T("btnSetRMenu"));
	btnSetRMenu->SetText(_T("SetRightMenu"));
	btnSetRMenu->Click += EventHandler((Object*)this, (EventFun)&MdiSam::btnSetRMenu_Click);
	btnSetRMenu->SetLocation(2, nTop);
	btnSetRMenu->SetSize(85, 21);
	btnSetRMenu->Anchor = (AnchorStyles::Type)(AnchorStyles::Left | AnchorStyles::Bottom);
	this->AddControl(btnSetRMenu);

	btnUnSetRMenu = new Button();
	btnUnSetRMenu->SetName(_T("btnUnSetRMenu"));
	btnUnSetRMenu->SetText(_T("UnSetRightMenu"));
	btnUnSetRMenu->Click += EventHandler((Object*)this, (EventFun)&MdiSam::btnUnSetRMenu_Click);
	btnUnSetRMenu->SetLocation(87, nTop);
	btnUnSetRMenu->SetSize(100, 21);
	btnUnSetRMenu->Anchor = (AnchorStyles::Type)(AnchorStyles::Left | AnchorStyles::Bottom);
	this->AddControl(btnUnSetRMenu);

	btnExit = new Button();
	btnExit->SetName(_T("btnExit"));
	btnExit->SetText(_T("Exit"));
	btnExit->Click += EventHandler((Object*)this, (EventFun)&MdiSam::btnExit_Click);
	btnExit->SetLocation(189, nTop);
	btnExit->SetSize(50, 21);
	btnExit->Anchor = (AnchorStyles::Type)(AnchorStyles::Left | AnchorStyles::Bottom);
	this->AddControl(btnExit);

	btnReCreate = new Button();
	btnReCreate->SetName(_T("btnReCreate"));
	btnReCreate->SetText(_T("ReCreateTab"));
	btnReCreate->Click += EventHandler((Object*)this, (EventFun)&MdiSam::btnReCreate_Click);
	btnReCreate->SetLocation(250, nTop);
	btnReCreate->SetSize(80, 21);
	btnReCreate->Anchor = (AnchorStyles::Type)(AnchorStyles::Left | AnchorStyles::Bottom);
	this->AddControl(btnReCreate);

	chkOwnDraw = new CheckBox();
	chkOwnDraw->SetName(_T("chkOwnDraw"));
	chkOwnDraw->SetText(_T("OwnDraw"));
	chkOwnDraw->Click += EventHandler((Object*)this, (EventFun)&MdiSam::chkOwnDraw_Click);
	chkOwnDraw->SetLocation(335, nTop);
	chkOwnDraw->SetSize(80, 21);
	chkOwnDraw->Anchor = (AnchorStyles::Type)(AnchorStyles::Left | AnchorStyles::Bottom);
	this->AddControl(chkOwnDraw);

	nTop += 22;
	this->SetName(_T("MdiSam"));
	this->SetText(_T("TabPage&TabControl Sample"));
	this->SetLocation(466, 366);
	this->SetSizeBaseOnClient(500, nTop + 35);
	//this->SetCenter();

	this->Load += EventHandler((Object*)this, (EventFun)&MdiSam::Form_Load);
}

void MdiSam::Form_Load(Object* sender, EventArgs* e)
{
	AddPage();
	AddPage();
}

void MdiSam::btnExit_Click(Object* sender, EventArgs* e)
{
	this->EndDialog(DialogResult::OK);
}

void MdiSam::menuCnt1_Click(Object* sender, EventArgs* e)
{
	cc::Log::fatal(_T("menuCnt1_Click"));
}

void MdiSam::AddPage()
{
	const TCHAR* tc = editText->GetText();
	TabPage* tp = new TabPage();
	//tp->SetStyle(0, WS_THICKFRAME);
	tp->SetClientSize(100, 100);
	tp->SetText(tc);
	if(chkIcon->GetChecked())
	{
		tp->SetIcon(imageList, IDI_ICON2);
	}

	TextBox* edit1 = new TextBox();
	edit1->SetText(tc);
	edit1->Click += EventHandler((Object*)this, (EventFun)&MdiSam::btnReCreate_Click);
	edit1->SetLocation(1, 2);
	edit1->SetSize(98, 21);
	edit1->Anchor = (AnchorStyles::Type)(AnchorStyles::Top | AnchorStyles::Left | AnchorStyles::Right);
	tp->AddControl(edit1);

	tab1->AddPage(tp);
}

void MdiSam::btnDeEnable_Click(Object* sender, EventArgs* e)
{
	int nInd = tab1->GetSelectedIndex();
	if(nInd < 0)
	{
		::MessageBox(m_hWnd, _T("need selected one page."), NULL, MB_OK);
		return;
	}
	tab1->GetPage(nInd)->SetEnabled( !tab1->GetPage(nInd)->GetEnabled() );
}

void MdiSam::btnAdd_Click(Object* sender, EventArgs* e)
{
	AddPage();
}

void MdiSam::btnMod_Click(Object* sender, EventArgs* e)
{
	int nInd = tab1->GetSelectedIndex();
	if(nInd < 0)
	{
		::MessageBox(m_hWnd, _T("need selected one page."), NULL, MB_OK);
		return;
	}
	TabPage* tabPage = tab1->GetPage(nInd);
	TextBox* edit1 = (TextBox*)tabPage->GetControl(0);

	const TCHAR* tc = editText->GetText();
	if(chkIcon->GetChecked())
	{
		tabPage->SetIcon(imageList, IDI_ICON2);
	}
	else
	{
		tabPage->SetIcon(NULL, -1);
	}
	tabPage->SetText(tc);
	edit1->SetText(tc);
}

void MdiSam::btnDel_Click(Object* sender, EventArgs* e)
{
	int nInd = tab1->GetSelectedIndex();
	if(nInd < 0)
	{
		::MessageBox(m_hWnd, _T("need selected one page."), NULL, MB_OK);
		return;
	}
	TabPage* tabPage = tab1->RemovePage(nInd);
	delete tabPage;
}

void MdiSam::btnReCreate_Click(Object* sender, EventArgs* e)
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
	this->AddControl(tab1);

}


void MdiSam::chkOwnDraw_Click(Object* sender, EventArgs* e)
{
	if(tab1->HasStyle(TCS_OWNERDRAWFIXED))
	{
		tab1->SetStyle(TCS_OWNERDRAWFIXED, 0);
	}
	else
	{
		tab1->SetStyle(0, TCS_OWNERDRAWFIXED);
	}

}
void MdiSam::btnSetRMenu_Click(Object* sender, EventArgs* e)
{
	tab1->SetHeadContextMenu(contextMenu3);
	tab1->SetContextMenu(contextMenu1);
}

void MdiSam::btnUnSetRMenu_Click(Object* sender, EventArgs* e)
{
	tab1->SetHeadContextMenu(NULL);
	tab1->SetContextMenu(NULL);
}

void MdiSam::btnAlign_Click(Object* sender, EventArgs* e)
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

void MdiSam::btnAppearance_Click(Object* sender, EventArgs* e)
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

void MdiSam::tabHead_DoubleClick(Object* sender, EventArgs* e)
{
	::MessageBox(m_hWnd, _T("tab Head DoubleClick"), NULL, MB_OK);
}
