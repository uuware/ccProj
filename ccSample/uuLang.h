// MainForm.h
// The C# like Form class
// Author: Shu.KK
// 2008-06-11
//
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef CC_WIN_MAINFORM_H_
#define CC_WIN_MAINFORM_H_

#include "..\cc\cc.All.h"
#include "resource.h"

using namespace cc;
using namespace cc::win;

#define BUF_LEN 256
class MainForm : public Form
{
public:
	MainForm();
	virtual ~MainForm();

protected:
	TCHAR buf[BUF_LEN];
	cc::Str sConfigPath;

	Label* label;
	CheckBox* toolOnTop;
	Button* btnMNew;
	Button* btnMUpdate;
	Button* btnMDel;
	Button* btnMSaveAs;
	Button* btnMLoad;
	Label* labFile;
	ListBox* lstMsgs;

	TextBox* editAName;
	TextBox* editAKey1;

	ComboBox* cbLangFile;
	TextBox* editLName;
	Button* btnLNew;
	Button* btnLUpdate;
	Button* btnLDel;
	ListBox* lstLangs;
	TextBox* editMsgID;
	TextBox* editMsg;

	void InitializeComponent();
	void Form_Load(Object* sender, EventArgs* e);
	void Form_Closed(Object* sender, EventArgs* e);

	void Config_Save(cc::Str& sPath);
	void Config_Load(cc::Str& sPath);
	void Control_Click(Object* sender, EventArgs* e);
	void lstMsgs_SelectedIndexChanged(Object* sender, EventArgs* e);
	void lstLangs_SelectedIndexChanged(Object* sender, EventArgs* e);

	void menuExit_Click(Object* sender, EventArgs* e);
	void toolOnTop_Click(Object* sender, EventArgs* e);
};

#endif //#ifndef CC_WIN_MAINFORM_H_
