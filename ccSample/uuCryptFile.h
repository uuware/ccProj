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
#include "../ccExt/AES.h"

#define CC_TIPSDLG_REMOVEORIG 1
#define CC_TIPSDLG_OVERORIG 2

using namespace cc;
using namespace cc::win;

class MainForm : public Dialog
{
public:
	MainForm();
	virtual ~MainForm();

protected:
	ImageList* imageList;
	Label* label;
	TextBox* editPass;
	CheckBox* chkShowPW;
	CheckBox* chkDelOrg;
	CheckBox* chkConfirm;
	Button* btnCrypt;
	//Button* btnReset;

	void InitializeComponent();
	LRESULT WndProc(EventArgs *e);
	void Form_Load(Object* sender, EventArgs* e);
	void Form_Closed(Object* sender, EventArgs* e);

	bool IsConfigOK(bool isMsg);
	bool TreateOne(cc::Str& sFile, const char* pwd, int klen, bool isDelOrg);
	bool EnCryptOne(cc::Str& sFile, const char* pwd, int klen, bool isDelOrg);
	bool DeCryptOne(cc::Str& sFile, const char* pwd, int klen);
	void Control_Click(Object* sender, EventArgs* e);
	void menuExit_Click(Object* sender, EventArgs* e);
};

#endif //#ifndef CC_WIN_MAINFORM_H_
