// TabSam.h
// The C# like Form class
// Author: Shu.KK
// 2006-01-01
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef CC_WIN_TabSam_H_
#define CC_WIN_TabSam_H_

#include "..\cc\cc.All.h"
#include "resource.h"

using namespace cc;
using namespace cc::win;

class TabSam : public Form
{
protected:
	ImageList* imageList;
	ContextMenu* contextMenu1;
	ContextMenu* coneMenuConfig;
	ContextMenu* contextMenu3;
	ContextMenu* contextMenu4;

	Label* labText;
	TextBox* editText;
	CheckBox* chkIcon;
	CheckBox* chkTransparent;
	CheckBox* chkIE;
	PickColorCtl* toolPageColor;

	Button* btnAdd;
	Button* btnDel;
	Button* btnMod;

	Button* btnDeEnable;
	Button* btnAlign;
	Button* btnAppearance;

	TabControl* tab1;

	Button* btnSetRMenu;
	Button* btnUnSetRMenu;
	Button* btnExit;

	Button* btnReCreate;
	CheckBox* chkOwnDraw;

protected:
	void InitializeComponent();
	void Form_Load(Object* sender, EventArgs* e);
	void Form_Closed(Object* sender, EventArgs* e);
	void Control_Click(Object* sender, EventArgs* e);
	void menuCnt1_Click(Object* sender, EventArgs* e);

	void AddPage();
	void btnAdd_Click(Object* sender, EventArgs* e);
	void btnMod_Click(Object* sender, EventArgs* e);
	void btnDel_Click(Object* sender, EventArgs* e);

	void btnDeEnable_Click(Object* sender, EventArgs* e);
	void btnChgAlign_Click(Object* sender, EventArgs* e);
	void btnChgMode_Click(Object* sender, EventArgs* e);

	void btnAlign_Click(Object* sender, EventArgs* e);
	void btnAppearance_Click(Object* sender, EventArgs* e);
	void btnSetRMenu_Click(Object* sender, EventArgs* e);
	void btnUnSetRMenu_Click(Object* sender, EventArgs* e);
	void btnExit_Click(Object* sender, EventArgs* e);

	void btnReCreate_Click(Object* sender, EventArgs* e);
	void chkOwnDraw_Click(Object* sender, EventArgs* e);

	void tabHead_DoubleClick(Object* sender, EventArgs* e);
public:
	TabSam();
};

#endif //#ifndef CC_WIN_TabSam_H_
