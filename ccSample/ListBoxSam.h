// ListBoxSam.h
// The C# like Form class
// Author: Shu.KK
// 2006-01-01
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef CC_WIN_ListBoxSam_H_
#define CC_WIN_ListBoxSam_H_

#include "..\cc\cc.All.h"
#include "resource.h"

using namespace cc;
using namespace cc::win;

class ListBoxSam : public Form
{
protected:
	ImageList* imageList;
	ContextMenu* contextMenu1;
	ContextMenu* coneMenuConfig;
	ContextMenu* contextMenu3;
	ContextMenu* contextMenu4;

	Label* labIndex;
	TextBox* editLstIndex;
	Label* labText;
	TextBox* editText;
	Button* btnLstGetText;
	Button* btnCLstGetText;
	Button* btnDeEnable;

	ListBox* listBox1;
	Button* btnLstAdd;
	Button* btnLstMod;
	Button* btnLstDel;
	Button* btnLstClr;
	Button* btnLstUp;
	Button* btnLstDown;

	CheckListBox* checkListBox1;
	Button* btnCLstAdd;
	Button* btnCLstMod;
	Button* btnCLstDel;
	Button* btnCLstClr;
	Button* btnCLstUp;
	Button* btnCLstDown;

	Label* labWidth;
	TextBox* editWidth;
	Button* btnLstSetWidth;
	Button* btnCLstSetWidth;
	Button* chkTransparent;
	CheckBox* chkSet3State;

	Label* labMultiCheck;
	TextBox* editMultiCheck;
	Button* btnMultiCheck;

	Button* btnSetRMenu;
	Button* btnUnSetRMenu;
	Button* btnExit;

protected:
	void InitializeComponent();
	void Form_Load(Object* sender, EventArgs* e);
	void Control_Click(Object* sender, EventArgs* e);
	void menuCnt1_Click(Object* sender, EventArgs* e);

	void btnLstGetText_Click(Object* sender, EventArgs* e);
	void btnCLstGetText_Click(Object* sender, EventArgs* e);
	void btnDeEnable_Click(Object* sender, EventArgs* e);

	void btnLstAdd_Click(Object* sender, EventArgs* e);
	void btnLstMod_Click(Object* sender, EventArgs* e);
	void btnLstDel_Click(Object* sender, EventArgs* e);
	void btnLstClr_Click(Object* sender, EventArgs* e);
	void btnLstUp_Click(Object* sender, EventArgs* e);
	void btnLstDown_Click(Object* sender, EventArgs* e);

	void btnCLstAdd_Click(Object* sender, EventArgs* e);
	void btnCLstMod_Click(Object* sender, EventArgs* e);
	void btnCLstDel_Click(Object* sender, EventArgs* e);
	void btnCLstClr_Click(Object* sender, EventArgs* e);
	void btnCLstUp_Click(Object* sender, EventArgs* e);
	void btnCLstDown_Click(Object* sender, EventArgs* e);

	void btnLstSetWidth_Click(Object* sender, EventArgs* e);
	void btnCLstSetWidth_Click(Object* sender, EventArgs* e);
	void chkSet3State_Click(Object* sender, EventArgs* e);
	void btnSetMultiCheckCount_Click(Object* sender, EventArgs* e);
	void btnSetRMenu_Click(Object* sender, EventArgs* e);
	void btnUnSetRMenu_Click(Object* sender, EventArgs* e);
	void btnExit_Click(Object* sender, EventArgs* e);
public:
	ListBoxSam();
};

#endif //#ifndef CC_WIN_ListBoxSam_H_
