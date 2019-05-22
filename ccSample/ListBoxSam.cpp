#include "ListBoxSam.h"
using namespace cc;
using namespace cc::win;

ListBoxSam::ListBoxSam()
{
	InitializeComponent();
}

void ListBoxSam::InitializeComponent()
{
	int nTop = 1;
	imageList = (ImageList*)CC_APP_GetVarious(_T("ImageList"));
	contextMenu1 = (ContextMenu*)CC_APP_GetVarious(_T("ContextMenu1"));
	coneMenuConfig = (ContextMenu*)CC_APP_GetVarious(_T("coneMenuConfig"));
	contextMenu3 = (ContextMenu*)CC_APP_GetVarious(_T("ContextMenu3"));
	contextMenu4 = (ContextMenu*)CC_APP_GetVarious(_T("ContextMenu4"));

	labIndex = new Label();
	labIndex->SetName(_T("labIndex"));
	labIndex->SetText(_T("Index:"));
	labIndex->SetLocation(2, nTop + 2);
	labIndex->SetSize(30, 17);
	this->AddControl(labIndex);

	editLstIndex = new TextBox();
	editLstIndex->SetName(_T("editLstIndex"));
	editLstIndex->SetText(_T("0"));
	editLstIndex->SetStyleNumber(true);
	editLstIndex->SetLocation(33, nTop);
	editLstIndex->SetSize(30, 17);
	this->AddControl(editLstIndex);

	labText = new Label();
	labText->SetName(_T("labText"));
	labText->SetText(_T("Text:"));
	labText->SetLocation(67, nTop + 2);
	labText->SetSize(25, 17);
	this->AddControl(labText);

	editText = new TextBox();
	editText->SetName(_T("editText"));
	editText->SetLocation(93, nTop);
	editText->SetSize(387, 17);
	this->AddControl(editText);

	nTop += 20;
	btnLstGetText = new Button();
	btnLstGetText->SetName(_T("btnLstGetText"));
	btnLstGetText->SetText(_T("GetListText"));
	btnLstGetText->Click += EventHandler((Object*)this, (EventFun)&ListBoxSam::btnLstGetText_Click);
	btnLstGetText->SetLocation(2, nTop);
	btnLstGetText->SetSize(70, 19);
	this->AddControl(btnLstGetText);

	btnCLstGetText = new Button();
	btnCLstGetText->SetName(_T("btnCLstGetText"));
	btnCLstGetText->SetText(_T("GetCheckListText"));
	btnCLstGetText->Click += EventHandler((Object*)this, (EventFun)&ListBoxSam::btnCLstGetText_Click);
	btnCLstGetText->SetLocation(74, nTop);
	btnCLstGetText->SetSize(100, 19);
	this->AddControl(btnCLstGetText);

	btnDeEnable = new Button();
	btnDeEnable->SetName(_T("btnDeEnable"));
	btnDeEnable->SetText(_T("!Enable(CheckList)"));
	btnDeEnable->Click += EventHandler((Object*)this, (EventFun)&ListBoxSam::btnDeEnable_Click);
	btnDeEnable->SetLocation(176, nTop);
	btnDeEnable->SetSize(100, 19);
	this->AddControl(btnDeEnable);

	chkTransparent = new Button();
	chkTransparent->SetName(_T("chkTransparent"));
	chkTransparent->SetText(_T("Transparent"));
	chkTransparent->SetLocation(278, nTop);
	chkTransparent->SetSize(50, 20);
	chkTransparent->Click += EventHandler((Object*)this, (EventFun)&ListBoxSam::Control_Click);
	this->AddControl(chkTransparent);

	nTop += 20;
	listBox1 = new ListBox();
	listBox1->SetName(_T("listBox1"));
	listBox1->SetLocation(2, nTop);
	listBox1->SetSize(150, 140);
	this->AddControl(listBox1);

	btnLstAdd = new Button();
	btnLstAdd->SetName(_T("btnLstAdd"));
	btnLstAdd->SetText(_T("Add"));
	btnLstAdd->Click += EventHandler((Object*)this, (EventFun)&ListBoxSam::btnLstAdd_Click);
	btnLstAdd->SetLocation(160, nTop);
	btnLstAdd->SetSize(50, 21);
	this->AddControl(btnLstAdd);

	btnLstMod = new Button();
	btnLstMod->SetName(_T("btnLstMod"));
	btnLstMod->SetText(_T("Modify"));
	btnLstMod->Click += EventHandler((Object*)this, (EventFun)&ListBoxSam::btnLstMod_Click);
	btnLstMod->SetLocation(160, nTop + 22);
	btnLstMod->SetSize(50, 21);
	this->AddControl(btnLstMod);

	btnLstDel = new Button();
	btnLstDel->SetName(_T("btnLstDel"));
	btnLstDel->SetText(_T("Del"));
	btnLstDel->Click += EventHandler((Object*)this, (EventFun)&ListBoxSam::btnLstDel_Click);
	btnLstDel->SetLocation(160, nTop + 44);
	btnLstDel->SetSize(50, 21);
	this->AddControl(btnLstDel);

	btnLstClr = new Button();
	btnLstClr->SetName(_T("btnLstClr"));
	btnLstClr->SetText(_T("Clear"));
	btnLstClr->Click += EventHandler((Object*)this, (EventFun)&ListBoxSam::btnLstClr_Click);
	btnLstClr->SetLocation(160, nTop + 66);
	btnLstClr->SetSize(50, 21);
	this->AddControl(btnLstClr);

	btnLstUp = new Button();
	btnLstUp->SetName(_T("btnLstUp"));
	btnLstUp->SetText(_T("Up"));
	btnLstUp->Click += EventHandler((Object*)this, (EventFun)&ListBoxSam::btnLstUp_Click);
	btnLstUp->SetLocation(160, nTop + 90);
	btnLstUp->SetSize(50, 21);
	this->AddControl(btnLstUp);

	btnLstDown = new Button();
	btnLstDown->SetName(_T("btnLstDown"));
	btnLstDown->SetText(_T("Down"));
	btnLstDown->Click += EventHandler((Object*)this, (EventFun)&ListBoxSam::btnLstDown_Click);
	btnLstDown->SetLocation(160, nTop + 112);
	btnLstDown->SetSize(50, 21);
	this->AddControl(btnLstDown);

	checkListBox1 = new CheckListBox();
	checkListBox1->SetName(_T("checkListBox1"));
	checkListBox1->SetText(_T("checkListBox1"));
	checkListBox1->SetLocation(220, nTop);
	checkListBox1->SetSize(200, 140);
	this->AddControl(checkListBox1);
	
	btnCLstAdd = new Button();
	btnCLstAdd->SetName(_T("btnCLstAdd"));
	btnCLstAdd->SetText(_T("Add"));
	btnCLstAdd->Click += EventHandler((Object*)this, (EventFun)&ListBoxSam::btnCLstAdd_Click);
	btnCLstAdd->SetLocation(428, nTop);
	btnCLstAdd->SetSize(50, 21);
	this->AddControl(btnCLstAdd);

	btnCLstMod = new Button();
	btnCLstMod->SetName(_T("btnCLstMod"));
	btnCLstMod->SetText(_T("Modify"));
	btnCLstMod->Click += EventHandler((Object*)this, (EventFun)&ListBoxSam::btnCLstMod_Click);
	btnCLstMod->SetLocation(428, nTop + 22);
	btnCLstMod->SetSize(50, 21);
	this->AddControl(btnCLstMod);

	btnCLstDel = new Button();
	btnCLstDel->SetName(_T("btnCLstDel"));
	btnCLstDel->SetText(_T("Del"));
	btnCLstDel->Click += EventHandler((Object*)this, (EventFun)&ListBoxSam::btnCLstDel_Click);
	btnCLstDel->SetLocation(428, nTop + 44);
	btnCLstDel->SetSize(50, 21);
	this->AddControl(btnCLstDel);

	btnCLstClr = new Button();
	btnCLstClr->SetName(_T("btnCLstClr"));
	btnCLstClr->SetText(_T("Clear"));
	btnCLstClr->Click += EventHandler((Object*)this, (EventFun)&ListBoxSam::btnCLstClr_Click);
	btnCLstClr->SetLocation(428, nTop + 66);
	btnCLstClr->SetSize(50, 21);
	this->AddControl(btnCLstClr);

	btnCLstUp = new Button();
	btnCLstUp->SetName(_T("btnCLstUp"));
	btnCLstUp->SetText(_T("Up"));
	btnCLstUp->Click += EventHandler((Object*)this, (EventFun)&ListBoxSam::btnCLstUp_Click);
	btnCLstUp->SetLocation(428, nTop + 90);
	btnCLstUp->SetSize(50, 21);
	this->AddControl(btnCLstUp);

	btnCLstDown = new Button();
	btnCLstDown->SetName(_T("btnCLstDown"));
	btnCLstDown->SetText(_T("Down"));
	btnCLstDown->Click += EventHandler((Object*)this, (EventFun)&ListBoxSam::btnCLstDown_Click);
	btnCLstDown->SetLocation(428, nTop + 112);
	btnCLstDown->SetSize(50, 21);
	this->AddControl(btnCLstDown);

	nTop += 144;
	labWidth = new Label();
	labWidth->SetName(_T("labWidth"));
	labWidth->SetText(_T("InnerWidth:"));
	labWidth->SetLocation(2, nTop + 2);
	labWidth->SetSize(57, 17);
	this->AddControl(labWidth);

	editWidth = new TextBox();
	editWidth->SetName(_T("editWidth"));
	editWidth->SetText(_T("300"));
	editWidth->SetStyleNumber(true);
	editWidth->SetLocation(58, nTop);
	editWidth->SetSize(30, 17);
	this->AddControl(editWidth);

	btnLstSetWidth = new Button();
	btnLstSetWidth->SetName(_T("btnLstSetWidth"));
	btnLstSetWidth->SetText(_T("SetListWidth"));
	btnLstSetWidth->Click += EventHandler((Object*)this, (EventFun)&ListBoxSam::btnLstSetWidth_Click);
	btnLstSetWidth->SetLocation(92, nTop);
	btnLstSetWidth->SetSize(75, 21);
	this->AddControl(btnLstSetWidth);

	btnCLstSetWidth = new Button();
	btnCLstSetWidth->SetName(_T("btnCLstSetWidth"));
	btnCLstSetWidth->SetText(_T("SetCheckBoxListWidth"));
	btnCLstSetWidth->Click += EventHandler((Object*)this, (EventFun)&ListBoxSam::btnCLstSetWidth_Click);
	btnCLstSetWidth->SetLocation(168, nTop);
	btnCLstSetWidth->SetSize(125, 21);
	this->AddControl(btnCLstSetWidth);

	chkSet3State = new CheckBox();
	chkSet3State->SetName(_T("chkSet3State"));
	chkSet3State->SetText(_T("Set3STATE for CheckBoxList"));
	chkSet3State->Click += EventHandler((Object*)this, (EventFun)&ListBoxSam::chkSet3State_Click);
	chkSet3State->SetLocation(295, nTop);
	chkSet3State->SetSize(174, 21);
	this->AddControl(chkSet3State);

	nTop += 25;
	labMultiCheck = new Label();
	labMultiCheck->SetName(_T("labMultiCheck"));
	labMultiCheck->SetText(_T("MulitCheckBox Count(1~16):"));
	labMultiCheck->SetLocation(2, nTop + 2);
	labMultiCheck->SetSize(146, 17);
	this->AddControl(labMultiCheck);

	editMultiCheck = new TextBox();
	editMultiCheck->SetName(_T("editMultiCheck"));
	editMultiCheck->SetText(_T("2"));
	editMultiCheck->SetStyleNumber(true);
	editMultiCheck->SetLocation(149, nTop);
	editMultiCheck->SetSize(26, 17);
	this->AddControl(editMultiCheck);

	btnMultiCheck = new Button();
	btnMultiCheck->SetName(_T("btnMultiCheck"));
	btnMultiCheck->SetText(_T("SetCount"));
	btnMultiCheck->Click += EventHandler((Object*)this, (EventFun)&ListBoxSam::btnSetMultiCheckCount_Click);
	btnMultiCheck->SetLocation(174, nTop);
	btnMultiCheck->SetSize(60, 21);
	this->AddControl(btnMultiCheck);

	btnSetRMenu = new Button();
	btnSetRMenu->SetName(_T("btnSetRMenu"));
	btnSetRMenu->SetText(_T("SetRightMenu"));
	btnSetRMenu->Click += EventHandler((Object*)this, (EventFun)&ListBoxSam::btnSetRMenu_Click);
	btnSetRMenu->SetLocation(240, nTop);
	btnSetRMenu->SetSize(85, 21);
	this->AddControl(btnSetRMenu);

	btnUnSetRMenu = new Button();
	btnUnSetRMenu->SetName(_T("btnUnSetRMenu"));
	btnUnSetRMenu->SetText(_T("UnSetRightMenu"));
	btnUnSetRMenu->Click += EventHandler((Object*)this, (EventFun)&ListBoxSam::btnUnSetRMenu_Click);
	btnUnSetRMenu->SetLocation(327, nTop);
	btnUnSetRMenu->SetSize(100, 21);
	this->AddControl(btnUnSetRMenu);

	btnExit = new Button();
	btnExit->SetName(_T("btnExit"));
	btnExit->SetText(_T("Exit"));
	btnExit->Click += EventHandler((Object*)this, (EventFun)&ListBoxSam::btnExit_Click);
	btnExit->SetLocation(429, nTop);
	btnExit->SetSize(50, 21);
	this->AddControl(btnExit);

	this->SetName(_T("ListBoxSam"));
	this->SetText(_T("ListBox&CheckBoxList Sample"));
	this->SetSize(500, 268);
	this->SetContextMenu(coneMenuConfig);
	//this->SetCenter();

	this->Load += EventHandler((Object*)this, (EventFun)&ListBoxSam::Form_Load);
}

void ListBoxSam::Form_Load(Object* sender, EventArgs* e)
{
}

void ListBoxSam::Control_Click(Object* sender, EventArgs* e)
{
	if(sender == chkTransparent)
	{
		bool isOn = listBox1->HasStyle(StyleType::Outer, OuterStyle::TransparentParentErase);
		if(isOn)
		{
			listBox1->SetStyle(StyleType::Outer, OuterStyle::TransparentParentErase, 0);
			listBox1->SetStyle(StyleType::ExStyle, WS_EX_TRANSPARENT, 0);
			checkListBox1->SetStyle(StyleType::Outer, OuterStyle::TransparentParentErase, 0);
		}
		else
		{
			listBox1->SetStyle(StyleType::Outer, 0, OuterStyle::TransparentParentErase);
			listBox1->SetStyle(StyleType::ExStyle, 0, WS_EX_TRANSPARENT);
			checkListBox1->SetStyle(StyleType::Outer, 0, OuterStyle::TransparentParentErase);
		}
	}
}

void ListBoxSam::btnExit_Click(Object* sender, EventArgs* e)
{
	this->EndDialog(DialogResult::OK);
}

void ListBoxSam::menuCnt1_Click(Object* sender, EventArgs* e)
{
	cc::Log::fatal(_T("menuCnt1_Click"));
}

void ListBoxSam::btnLstGetText_Click(Object* sender, EventArgs* e)
{
	int nInd = editLstIndex->GetTextInt();
	int nCnt = listBox1->ItemCount();
	if(nInd < 0 || nInd >= nCnt)
	{
		::MessageBox(m_hWnd, _T("Index must >= 0 and < the count of listbox's items."), NULL, MB_OK);
		return;
	}
	editText->SetText(listBox1->ItemGet(nInd));
}

void ListBoxSam::btnCLstGetText_Click(Object* sender, EventArgs* e)
{
	int nInd = editLstIndex->GetTextInt();
	int nCnt = checkListBox1->ItemCount();
	if(nInd < 0 || nInd >= nCnt)
	{
		::MessageBox(m_hWnd, _T("Index must >= 0 and < the count of checkboxlist's items."), NULL, MB_OK);
		return;
	}
	editText->SetText(checkListBox1->ItemGet(nInd));
}

void ListBoxSam::btnDeEnable_Click(Object* sender, EventArgs* e)
{
	int nInd = editLstIndex->GetTextInt();
	int nCnt = checkListBox1->ItemCount();
	if(nInd < 0 || nInd >= nCnt)
	{
		::MessageBox(m_hWnd, _T("Index must >= 0 and < the count of checkboxlist's items."), NULL, MB_OK);
		return;
	}
	checkListBox1->SetItemEnabled(nInd, !checkListBox1->GetItemEnabled(nInd));
}

void ListBoxSam::btnLstAdd_Click(Object* sender, EventArgs* e)
{
	listBox1->ItemAdd(editText->GetText());
}

void ListBoxSam::btnLstMod_Click(Object* sender, EventArgs* e)
{
	int nInd = listBox1->GetSelectedIndex();
	if(nInd < 0)
	{
		::MessageBox(m_hWnd, _T("must select one item."), NULL, MB_OK);
		return;
	}
	const TCHAR* tc = editText->GetText();
	listBox1->ItemSet(nInd, tc);
}

void ListBoxSam::btnLstDel_Click(Object* sender, EventArgs* e)
{
	int nInd = listBox1->GetSelectedIndex();
	if(nInd < 0)
	{
		::MessageBox(m_hWnd, _T("must select one item."), NULL, MB_OK);
		return;
	}
	listBox1->ItemDel(nInd);
	int nCnt = listBox1->ItemCount();
	if(nCnt > 0)
	{
		listBox1->SetSelectedIndex(nInd >= nCnt ? nCnt - 1 : nInd);
	}
}

void ListBoxSam::btnLstClr_Click(Object* sender, EventArgs* e)
{
	listBox1->ItemClear();
}

void ListBoxSam::btnLstUp_Click(Object* sender, EventArgs* e)
{
	int nInd = listBox1->GetSelectedIndex();
	if(nInd < 0)
	{
		::MessageBox(m_hWnd, _T("must select one item."), NULL, MB_OK);
		return;
	}
	listBox1->ItemUp(nInd);
}

void ListBoxSam::btnLstDown_Click(Object* sender, EventArgs* e)
{
	int nInd = listBox1->GetSelectedIndex();
	if(nInd < 0)
	{
		::MessageBox(m_hWnd, _T("must select one item."), NULL, MB_OK);
		return;
	}
	listBox1->ItemDown(nInd);
}

void ListBoxSam::btnCLstAdd_Click(Object* sender, EventArgs* e)
{
	checkListBox1->ItemAdd(editText->GetText());
}

void ListBoxSam::btnCLstMod_Click(Object* sender, EventArgs* e)
{
	int nInd = checkListBox1->GetSelectedIndex();
	if(nInd < 0)
	{
		::MessageBox(m_hWnd, _T("must select one item."), NULL, MB_OK);
		return;
	}
	const TCHAR* tc = editText->GetText();
	checkListBox1->ItemSet(nInd, tc);
}

void ListBoxSam::btnCLstDel_Click(Object* sender, EventArgs* e)
{
	int nInd = checkListBox1->GetSelectedIndex();
	if(nInd < 0)
	{
		::MessageBox(m_hWnd, _T("must select one item."), NULL, MB_OK);
		return;
	}
	checkListBox1->ItemDel(nInd);
	int nCnt = checkListBox1->ItemCount();
	if(nCnt > 0)
	{
		checkListBox1->SetSelectedIndex(nInd >= nCnt ? nCnt - 1 : nInd);
	}
}

void ListBoxSam::btnCLstClr_Click(Object* sender, EventArgs* e)
{
	checkListBox1->ItemClear();
}

void ListBoxSam::btnCLstUp_Click(Object* sender, EventArgs* e)
{
	int nInd = checkListBox1->GetSelectedIndex();
	if(nInd < 0)
	{
		::MessageBox(m_hWnd, _T("must select one item."), NULL, MB_OK);
		return;
	}
	checkListBox1->ItemUp(nInd);
}

void ListBoxSam::btnCLstDown_Click(Object* sender, EventArgs* e)
{
	int nInd = checkListBox1->GetSelectedIndex();
	if(nInd < 0)
	{
		::MessageBox(m_hWnd, _T("must select one item."), NULL, MB_OK);
		return;
	}
	checkListBox1->ItemDown(nInd);
}

void ListBoxSam::btnLstSetWidth_Click(Object* sender, EventArgs* e)
{
	int nWidth = editWidth->GetTextInt();
	listBox1->SetHExtent(nWidth);
}

void ListBoxSam::btnCLstSetWidth_Click(Object* sender, EventArgs* e)
{
	int nWidth = editWidth->GetTextInt();
	checkListBox1->SetHExtent(nWidth);
}

void ListBoxSam::chkSet3State_Click(Object* sender, EventArgs* e)
{
	CheckStyle::Type nStyle = checkListBox1->GetCheckStyle();
	if(nStyle == CheckStyle::AutoCheckBox)
	{
		checkListBox1->SetCheckStyle(CheckStyle::Auto3State);
	}
	else
	{
		checkListBox1->SetCheckStyle(CheckStyle::AutoCheckBox);
	}
}

void ListBoxSam::btnSetMultiCheckCount_Click(Object* sender, EventArgs* e)
{
	int nCnt = editMultiCheck->GetTextInt();
	checkListBox1->SetMultiBoxCount(nCnt);
}

void ListBoxSam::btnSetRMenu_Click(Object* sender, EventArgs* e)
{
	listBox1->SetContextMenu(contextMenu1);
	checkListBox1->SetContextMenu(contextMenu1);
}

void ListBoxSam::btnUnSetRMenu_Click(Object* sender, EventArgs* e)
{
	listBox1->SetContextMenu(NULL);
	checkListBox1->SetContextMenu(NULL);
}
