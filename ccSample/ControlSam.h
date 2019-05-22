// TabSam.h
// The C# like Form class
// Author: Shu.KK
// 2006-01-01
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef CC_WIN_ControlSam_H_
#define CC_WIN_ControlSam_H_

#include "..\cc\cc.All.h"
#include "resource.h"

using namespace cc;
using namespace cc::win;

class ControlSam : public Form
{
protected:
	ImageList* imageList;
	ContextMenu* contextMenu1;
	ContextMenu* coneMenuConfig;
	ContextMenu* contextMenu3;
	ContextMenu* contextMenu4;

	ToolBar* toolbar1;

	Label* labIndex;
	TextBox* editIndex;
	Label* labText;
	TextBox* editText;
	CheckBox* chkIcon;
	CheckBox* chkRMenu;
	ComboBox* lstType;

	Button* btnAdd;
	Button* btnDel;
	Button* btnMod;
	Button* btnDeEnable;

	Button* btnExit;
	Button* btnReCreate;

	CheckBox* chkBarRMenu;
	CheckBox* chkBarBorder;
	CheckBox* chkBarTraceRMenu;
	CheckBox* chkBigSize;
	CheckBox* chkDisabled;

	ToolItem* toolSample;
	Label* labStyle;
	ComboBox* lstStyle;
	Button* btnStyleAdd;
	Button* btnStyleRemove;

	Label* labSample1;
	Label* labSample2;
	Button* btnSample1;
	Button* btnSample2;
	ToolItem* toolSample1;
	ToolItem* toolSample2;
	ToolItem* toolSample3;
	ToolItem* toolSample4;

protected:
	void InitializeComponent();
	void Form_Load(Object* sender, EventArgs* e);

	void test1_Click(Object* sender, EventArgs* e);
	void test1_Pupup(Object* sender, EventArgs* e);
	void btnAdd_Click(Object* sender, EventArgs* e);
	void btnMod_Click(Object* sender, EventArgs* e);
	void btnDel_Click(Object* sender, EventArgs* e);

	void btnDeEnable_Click(Object* sender, EventArgs* e);
	void btnChgAlign_Click(Object* sender, EventArgs* e);
	void btnChgMode_Click(Object* sender, EventArgs* e);

	void chkBarRMenu_Click(Object* sender, EventArgs* e);
	void chkBarBorder_Click(Object* sender, EventArgs* e);
	void chkTraceMParent_Click(Object* sender, EventArgs* e);
	void chkBigSize_Click(Object* sender, EventArgs* e);
	void chkDisabled_Click(Object* sender, EventArgs* e);

	void btnStyleAdd2_Click(Object* sender, EventArgs* e);
	void btnStyleRemove2_Click(Object* sender, EventArgs* e);

	void btnExit_Click(Object* sender, EventArgs* e);
	void btnReCreate_Click(Object* sender, EventArgs* e);
public:
	ControlSam();
};

#endif //#ifndef CC_WIN_ControlSam_H_
