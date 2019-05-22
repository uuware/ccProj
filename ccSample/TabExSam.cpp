#include "TabExSam.h"
using namespace cc;
using namespace cc::win;

TabExSam::TabExSam()
{
	InitializeComponent();
}

void TabExSam::InitializeComponent()
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
	toolPageColor->Click += EventHandler((Object*)this, (EventFun)&TabExSam::Control_Click);
	this->AddControl(toolPageColor);

	nTop += 19;
	btnAdd = new Button();
	btnAdd->SetName(_T("btnAdd"));
	btnAdd->SetText(_T("Add"));
	btnAdd->Click += EventHandler((Object*)this, (EventFun)&TabExSam::btnAdd_Click);
	btnAdd->SetLocation(2, nTop);
	btnAdd->SetSize(50, 21);
	this->AddControl(btnAdd);

	btnMod = new Button();
	btnMod->SetName(_T("btnMod"));
	btnMod->SetText(_T("Modify"));
	btnMod->Click += EventHandler((Object*)this, (EventFun)&TabExSam::btnMod_Click);
	btnMod->SetLocation(54, nTop);
	btnMod->SetSize(50, 21);
	this->AddControl(btnMod);

	btnDel = new Button();
	btnDel->SetName(_T("btnDel"));
	btnDel->SetText(_T("Del"));
	btnDel->Click += EventHandler((Object*)this, (EventFun)&TabExSam::btnDel_Click);
	btnDel->SetLocation(106, nTop);
	btnDel->SetSize(50, 21);
	this->AddControl(btnDel);

	btnDeEnable = new Button();
	btnDeEnable->SetName(_T("btnDeEnable"));
	btnDeEnable->SetText(_T("!Enable"));
	btnDeEnable->Click += EventHandler((Object*)this, (EventFun)&TabExSam::btnDeEnable_Click);
	btnDeEnable->SetLocation(160, nTop);
	btnDeEnable->SetSize(60, 21);
	this->AddControl(btnDeEnable);

	btnAlign = new Button();
	btnAlign->SetName(_T("btnAlign"));
	btnAlign->SetText(_T("ChgAlign"));
	btnAlign->Click += EventHandler((Object*)this, (EventFun)&TabExSam::btnAlign_Click);
	btnAlign->SetLocation(222, nTop);
	btnAlign->SetSize(60, 21);
	this->AddControl(btnAlign);

	btnAppearance = new Button();
	btnAppearance->SetName(_T("btnAppearance"));
	btnAppearance->SetText(_T("ChgAppearance"));
	btnAppearance->Click += EventHandler((Object*)this, (EventFun)&TabExSam::btnAppearance_Click);
	btnAppearance->SetLocation(284, nTop);
	btnAppearance->SetSize(88, 21);
	this->AddControl(btnAppearance);

	nTop += 22;
	tab1 = new TabControlEx();
	tab1->SetLocation(4, nTop);
	tab1->SetSize(490, 145);
	tab1->HeadDoubleClick += EventHandler((Object*)this, (EventFun)&TabExSam::tabHead_DoubleClick);
	tab1->HeadIconClick += EventHandler((Object*)this, (EventFun)&TabExSam::tabHeadIcon_Click);
	tab1->Anchor = (AnchorStyles::Type)(AnchorStyles::Top | AnchorStyles::Left | AnchorStyles::Right | AnchorStyles::Bottom);
	this->AddControl(tab1);

	nTop += 148;
	btnExit = new Button();
	btnExit->SetName(_T("btnExit"));
	btnExit->SetText(_T("Exit"));
	btnExit->Click += EventHandler((Object*)this, (EventFun)&TabExSam::btnExit_Click);
	btnExit->SetLocation(2, nTop);
	btnExit->SetSize(50, 21);
	btnExit->Anchor = (AnchorStyles::Type)(AnchorStyles::Left | AnchorStyles::Bottom);
	this->AddControl(btnExit);

	btnReCreate = new Button();
	btnReCreate->SetName(_T("btnReCreate"));
	btnReCreate->SetText(_T("ReCreateTab"));
	btnReCreate->Click += EventHandler((Object*)this, (EventFun)&TabExSam::btnReCreate_Click);
	btnReCreate->SetLocation(54, nTop);
	btnReCreate->SetSize(80, 21);
	btnReCreate->Anchor = (AnchorStyles::Type)(AnchorStyles::Left | AnchorStyles::Bottom);
	this->AddControl(btnReCreate);

	chkRMenu = new CheckBox();
	chkRMenu->SetName(_T("chkRMenu"));
	chkRMenu->SetText(_T("HasRightMenu"));
	chkRMenu->Click += EventHandler((Object*)this, (EventFun)&TabExSam::chkRMenu_Click);
	chkRMenu->SetLocation(140, nTop);
	chkRMenu->SetSize(96, 21);
	chkRMenu->Anchor = (AnchorStyles::Type)(AnchorStyles::Left | AnchorStyles::Bottom);
	this->AddControl(chkRMenu);

	chkRTool = new CheckBox();
	chkRTool->SetName(_T("chkRTool"));
	chkRTool->SetText(_T("HasRightTool"));
	chkRTool->Click += EventHandler((Object*)this, (EventFun)&TabExSam::chkRTool_Click);
	chkRTool->SetLocation(240, nTop);
	chkRTool->SetSize(96, 21);
	chkRTool->Anchor = (AnchorStyles::Type)(AnchorStyles::Left | AnchorStyles::Bottom);
	this->AddControl(chkRTool);

	labVWidth = new Label();
	labVWidth->SetName(_T("labVWidth"));
	labVWidth->SetText(_T("VHeadWidth:"));
	labVWidth->SetLocation(340, nTop + 2);
	labVWidth->SetSize(80, 17);
	labVWidth->Anchor = (AnchorStyles::Type)(AnchorStyles::Left | AnchorStyles::Bottom);
	this->AddControl(labVWidth);

	editVWidth = new TextBox();
	editVWidth->SetName(_T("editVWidth"));
	editVWidth->SetText(_T("80"));
	editVWidth->SetStyleNumber(true);
	editVWidth->SetLocation(420, nTop);
	editVWidth->SetSize(30, 17);
	editVWidth->Anchor = (AnchorStyles::Type)(AnchorStyles::Left | AnchorStyles::Bottom);
	this->AddControl(editVWidth);

	btnVWidth = new Button();
	btnVWidth->SetName(_T("btnVWidth"));
	btnVWidth->SetText(_T("Set"));
	btnVWidth->Click += EventHandler((Object*)this, (EventFun)&TabExSam::btnVWidth_Click);
	btnVWidth->SetLocation(452, nTop);
	btnVWidth->SetSize(30, 21);
	btnVWidth->Anchor = (AnchorStyles::Type)(AnchorStyles::Left | AnchorStyles::Bottom);
	this->AddControl(btnVWidth);

	nTop += 22;
	labStyle = new Label();
	labStyle->SetName(_T("labStyle"));
	labStyle->SetText(_T("Style:"));
	labStyle->SetLocation(1, nTop + 2);
	labStyle->SetSize(35, 17);
	labStyle->Anchor = (AnchorStyles::Type)(AnchorStyles::Left | AnchorStyles::Bottom);
	this->AddControl(labStyle);

	lstStyle = new ComboBox();
	lstStyle->SetName(_T("lstStyle"));
	lstStyle->SetLocation(36, nTop);
	lstStyle->SetSize(116, 200);
	lstStyle->Anchor = (AnchorStyles::Type)(AnchorStyles::Left | AnchorStyles::Bottom);
	this->AddControl(lstStyle);

	btnStyleAdd = new Button();
	btnStyleAdd->SetName(_T("btnStyleAdd"));
	btnStyleAdd->SetText(_T("Add"));
	btnStyleAdd->Click += EventHandler((Object*)this, (EventFun)&TabExSam::btnStyleAdd_Click);
	btnStyleAdd->SetLocation(154, nTop);
	btnStyleAdd->SetSize(30, 21);
	btnStyleAdd->Anchor = (AnchorStyles::Type)(AnchorStyles::Left | AnchorStyles::Bottom);
	this->AddControl(btnStyleAdd);

	btnStyleRemove = new Button();
	btnStyleRemove->SetName(_T("btnStyleRemove"));
	btnStyleRemove->SetText(_T("Remove"));
	btnStyleRemove->Click += EventHandler((Object*)this, (EventFun)&TabExSam::btnStyleRemove_Click);
	btnStyleRemove->SetLocation(186, nTop);
	btnStyleRemove->SetSize(50, 21);
	btnStyleRemove->Anchor = (AnchorStyles::Type)(AnchorStyles::Left | AnchorStyles::Bottom);
	this->AddControl(btnStyleRemove);

	chkHeadIcon = new CheckBox();
	chkHeadIcon->SetName(_T("chkHeadIcon"));
	chkHeadIcon->SetText(_T("HasHeadIcon"));
	chkHeadIcon->Click += EventHandler((Object*)this, (EventFun)&TabExSam::chkHeadIcon_Click);
	chkHeadIcon->SetLocation(240, nTop);
	chkHeadIcon->SetSize(90, 21);
	chkHeadIcon->Anchor = (AnchorStyles::Type)(AnchorStyles::Left | AnchorStyles::Bottom);
	this->AddControl(chkHeadIcon);

	labBestWidth = new Label();
	labBestWidth->SetName(_T("labBestWidth"));
	labBestWidth->SetText(_T("BestHeadWidth:"));
	labBestWidth->SetLocation(340, nTop + 2);
	labBestWidth->SetSize(80, 17);
	labBestWidth->Anchor = (AnchorStyles::Type)(AnchorStyles::Left | AnchorStyles::Bottom);
	this->AddControl(labBestWidth);

	editBestWidth = new TextBox();
	editBestWidth->SetName(_T("editBestWidth"));
	editBestWidth->SetText(_T("60"));
	editBestWidth->SetStyleNumber(true);
	editBestWidth->SetLocation(420, nTop);
	editBestWidth->SetSize(30, 17);
	editBestWidth->Anchor = (AnchorStyles::Type)(AnchorStyles::Left | AnchorStyles::Bottom);
	this->AddControl(editBestWidth);

	btnBestWidth = new Button();
	btnBestWidth->SetName(_T("btnBestWidth"));
	btnBestWidth->SetText(_T("Set"));
	btnBestWidth->Click += EventHandler((Object*)this, (EventFun)&TabExSam::btnBestWidth_Click);
	btnBestWidth->SetLocation(452, nTop);
	btnBestWidth->SetSize(30, 21);
	btnBestWidth->Anchor = (AnchorStyles::Type)(AnchorStyles::Left | AnchorStyles::Bottom);
	this->AddControl(btnBestWidth);

	nTop += 22;
	this->SetName(_T("TabExSam"));
	this->SetText(_T("TabPage&TabControl Sample"));
	this->SetContextMenu(coneMenuConfig);
	this->SetLocation(466, 666);
	this->SetSizeBaseOnClient(505, nTop);
	//this->SetCenter();

	this->Load += EventHandler((Object*)this, (EventFun)&TabExSam::Form_Load);
	this->Closed += EventHandler((Object*)this, (EventFun)&TabExSam::Form_Closed);

	editText->SetText(_T("BeforeLoad1"));
	AddPage();
	editText->SetText(_T("BeforeLoad2"));
	AddPage();
}

void TabExSam::Form_Load(Object* sender, EventArgs* e)
{
	//load config
	this->Config(true, *CC_APP_GetIni(), cc::Str(_T("Main")));

	lstStyle->ItemAdd(_T("PrevShowLeft=0x0001"));
	lstStyle->ItemAdd(_T("PrevShowRight=0x0002"));
	lstStyle->ItemAdd(_T("PrevShowMenu=0x0004"));
	lstStyle->ItemAdd(_T("NextShowLeft=0x0008"));
	lstStyle->ItemAdd(_T("NextShowRight=0x0010"));
	lstStyle->ItemAdd(_T("NextShowMenu=0x0020"));
	lstStyle->ItemAdd(_T("TopEndShow=0x0040"));
	lstStyle->ItemAdd(_T("IsSwapSelectedIndex=0x0800"));
	lstStyle->ItemAdd(_T("NoBodyRect=0x0080"));
	lstStyle->ItemAdd(_T("FlatTitle=0x0100"));
	editText->SetText(_T("1111111111111111111111111111"));
	AddPage();
	editText->SetText(_T("2222222222222222222"));
	AddPage();
	editText->SetText(_T("333333333333333333333333333333333"));
	AddPage();
	editText->SetText(_T("444444444444444444444"));
	AddPage();
	editText->SetText(_T("555555555555555555555555555555"));
	AddPage();
	editText->SetText(_T("6666"));
}

void TabExSam::Form_Closed(Object* sender, EventArgs* e)
{
	//save config
	this->Config(false, *CC_APP_GetIni(), cc::Str(_T("Main")));
}

void TabExSam::Control_Click(Object* sender, EventArgs* e)
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

DWORD TabExSam::GetSelStyle()
{
	DWORD nRet = 0;
	int nInd = lstStyle->GetSelectedIndex();
	if(nInd < 0)
	{
		::MessageBox(m_hWnd, _T("need selected one item."), NULL, MB_OK);
		return nRet;
	}
	if(nInd == 0)
	{
		nRet = 0x0001;
	}
	else if(nInd == 1)
	{
		nRet = 0x0002;
	}
	else if(nInd == 2)
	{
		nRet = 0x0004;
	}
	else if(nInd == 3)
	{
		nRet = 0x0008;
	}
	else if(nInd == 4)
	{
		nRet = 0x0010;
	}
	else if(nInd == 5)
	{
		nRet = 0x0020;
	}
	else if(nInd == 6)
	{
		nRet = 0x0040;
	}
	else if(nInd == 7)
	{
		nRet = 0x0800;
	}
	else if(nInd == 8)
	{
		nRet = 0x0080;
	}
	else if(nInd == 9)
	{
		nRet = 0x0100;
	}
	return nRet;
}

void TabExSam::btnStyleAdd_Click(Object* sender, EventArgs* e)
{
	DWORD nRet = GetSelStyle();
	tab1->SetTabStyle(0, nRet);
}

void TabExSam::btnStyleRemove_Click(Object* sender, EventArgs* e)
{
	DWORD nRet = GetSelStyle();
	tab1->SetTabStyle(nRet, 0);
}

void TabExSam::btnExit_Click(Object* sender, EventArgs* e)
{
	this->EndDialog(DialogResult::OK);
}

void TabExSam::menuCnt1_Click(Object* sender, EventArgs* e)
{
	cc::Log::fatal(_T("menuCnt1_Click"));
}

void TabExSam::AddPage()
{
	const TCHAR* tc = editText->GetText();
	TabPage* page = new TabPage();
	//page->SetStyle(0, WS_THICKFRAME);
	page->SetClientSize(100, 100);
	page->SetText(tc);
	if(chkIcon->GetChecked())
	{
		page->SetIcon(imageList, IDI_ICON2);
	}
	page->SetOuterStyle(OuterStyle::TransparentTop, !chkTransparent->GetChecked());
	page->SetOuterStyle(OuterStyle::TransparentParentErase, chkTransparent->GetChecked());
	page->SetTextColor(toolPageColor->GetBackground());
	tab1->AddPage(page);

	TextBox* edit1 = new TextBox();
	edit1->SetName(_T("edit1"));
	edit1->SetText(tc);
	edit1->SetLocation(11, 2);
	edit1->SetSize(88, 21);
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

void TabExSam::btnDeEnable_Click(Object* sender, EventArgs* e)
{
	int nInd = tab1->GetSelectedIndex();
	if(nInd < 0)
	{
		::MessageBox(m_hWnd, _T("need selected one page."), NULL, MB_OK);
		return;
	}
	tab1->GetPage(nInd)->SetEnabled( !tab1->GetPage(nInd)->GetEnabled() );
}

void TabExSam::btnAdd_Click(Object* sender, EventArgs* e)
{
	AddPage();
}

void TabExSam::btnMod_Click(Object* sender, EventArgs* e)
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
	IEControl* ie1 = (IEControl*)page->GetControl(_T("ie1"));

	const TCHAR* tc = editText->GetText();
	page->SetText(tc);
	if(chkIcon->GetChecked())
	{
		page->SetIcon(imageList, 9999);
	}
	else
	{
		page->SetIcon(NULL, -1);
	}
	page->SetOuterStyle(OuterStyle::TransparentTop, !chkTransparent->GetChecked());
	page->SetOuterStyle(OuterStyle::TransparentParentErase, chkTransparent->GetChecked());
	page->SetTextColor(toolPageColor->GetBackground());
	edit1->SetText(tc);
	edit1->SetOuterStyle(OuterStyle::TransparentParentBrush, chkTransparent->GetChecked());
	if(edit2 != NULL)
	{
		edit2->SetOuterStyle(OuterStyle::TransparentParentBrush, chkTransparent->GetChecked());
	}
	cc::Str str = cc::Str(cc::LangCode::UNICODE16, tc);
	if(ie1 != NULL && str.IndexOf(L".") > 0)
	{
		ie1->Navigate((wchar_t*)str.GetBuf());
	}
}

void TabExSam::btnDel_Click(Object* sender, EventArgs* e)
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

void TabExSam::btnReCreate_Click(Object* sender, EventArgs* e)
{
	int n1 = tab1->GetTabAlignment();
	int n2 = tab1->GetTabAppearance();
	RECT rect = tab1->GetBounds();

	this->RemoveControl(tab1);
	tab1->Dispose();
	delete tab1;
	tab1 = new TabControlEx();
	tab1->SetBounds(rect);
	tab1->Anchor = (AnchorStyles::Type)(AnchorStyles::Top | AnchorStyles::Left | AnchorStyles::Right | AnchorStyles::Bottom);
	tab1->SetTabAlignment((TabAlignment::Type)n1);
	tab1->SetTabAppearance((TabAppearance::Type)n2);
	this->AddControl(tab1);
}

void TabExSam::chkHeadIcon_Click(Object* sender, EventArgs* e)
{
	if(chkHeadIcon->GetChecked())
	{
		//tab1->AddHeadIcon(::LoadIcon(CC_APP_GetInstance(), MAKEINTRESOURCE(IDI_ICON3)), true);
		//tab1->AddHeadIcon(::LoadIcon(CC_APP_GetInstance(), MAKEINTRESOURCE(IDI_ICON4)), true);
		//tab1->AddHeadIcon(::LoadIcon(CC_APP_GetInstance(), MAKEINTRESOURCE(IDI_ICON5)), true);
		//tab1->AddHeadIcon(imageList, IDI_ICON2);
		//tab1->AddHeadIcon(imageList, IDI_ICON3);
		tab1->AddHeadIcon(imageList, IDI_ICON4);
	}
	else
	{
		tab1->ClearHeadIcon();
	}
}

void TabExSam::chkRMenu_Click(Object* sender, EventArgs* e)
{
	if(chkRMenu->GetChecked())
	{
		tab1->SetHeadContextMenu(contextMenu3);
		tab1->SetContextMenu(contextMenu1);
	}
	else
	{
		tab1->SetHeadContextMenu(NULL);
		tab1->SetContextMenu(NULL);
	}
}

void TabExSam::chkRTool_Click(Object* sender, EventArgs* e)
{
	if(chkRTool->GetChecked())
	{
		ToolItem* tool1 = new ToolItem();
		tool1->SetText(_T(""));
		tool1->SetSize(3, 17);
		tool1->SetOuterStyle(OuterStyle::Blank, true);
		tab1->AddRightButton(tool1);

		tool1 = new ToolItem();
		tool1->SetText(_T("tool1"));
		tool1->SetOuterStyle(OuterStyle::AlwaysEdge, true);
		tool1->SetContextMenu(contextMenu3);
		tab1->AddRightButton(tool1);

		tool1 = new ToolItem();
		tool1->SetHotTextColor(RGB(255, 0, 0));
		tool1->SetText(_T("X"));
		tab1->AddRightButton(tool1);

		tool1 = new ToolItem();
		tool1->SetText(NULL);
		tool1->SetContextMenu(contextMenu1);
		tab1->AddRightButton(tool1);
	}
	else
	{
		tab1->ClearRightButton();
	}
}

void TabExSam::btnVWidth_Click(Object* sender, EventArgs* e)
{
	int nW = editVWidth->GetTextInt();
	tab1->SetVHeadWidth(nW);
}

void TabExSam::btnBestWidth_Click(Object* sender, EventArgs* e)
{
	int nW = editBestWidth->GetTextInt();
	tab1->SetBestHeadWidth(nW);
}

void TabExSam::btnAlign_Click(Object* sender, EventArgs* e)
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

void TabExSam::btnAppearance_Click(Object* sender, EventArgs* e)
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

void TabExSam::tabHead_DoubleClick(Object* sender, EventArgs* e)
{
	::MessageBox(m_hWnd, _T("tab Head DoubleClick"), NULL, MB_OK);
}

void TabExSam::tabHeadIcon_Click(Object* sender, EventArgs* e)
{
	cc::Str s;
	s.Format(_T("tabHeadIcon_Click Index:%d"), (int)e->WParam);
	::MessageBox(m_hWnd, (TCHAR*)s.GetBuf(), NULL, MB_OK);
}
