#include "uuLang.h"
using namespace cc;
using namespace cc::win;

#define REG_KEYS "[#KEYSTART#]"
#define REG_KEYE "[#KEYEND#]"
#define APPINFO_SPLIT _T(";\r")

///////////////////////////////////////////////////////////////////////////////
//WinMain
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	cc::win::App::SetInstance(hInstance);
	//should set this before cc::Config::GetLang();
	cc::win::App::GetConfig()->LoadLang(CC_CONFIG_LANGENG);
	//the name is used for CreateMutex"s flages,cannot change to other lang
	cc::win::App::SetNameVersion(_T("uuLanguage Manager[uuware.com]"), _T("1.00"));
	//cc::win::App::SetNameVersion(_T(APPNAME), _T(APPVERSION));

	MainForm* form = new MainForm();
	cc::win::App::Run(form);
	form->Dispose();
	delete form;
	return 0;
}
int FindStartsWith(ComboBox* combo, const TCHAR* sPre)
{
	int nLen;
	int nCnt = combo->ItemCount();
	if(nCnt > 0 && sPre != NULL && (nLen = (int)_tcslen(sPre)) > 0)
	{
		for(int n = 0; n < nCnt; n++)
		{
			const TCHAR* sTxt = (const TCHAR*)combo->ItemGet(n).GetBuf();
			if(_tcsncmp(sPre, sTxt, nLen) == 0)
			{
				return n;
			}
		}
	}
	return -1;
}
int FindStartsWith(ListBox* list, const TCHAR* sPre)
{
	int nLen;
	int nCnt = list->ItemCount();
	if(nCnt > 0 && sPre != NULL && (nLen = (int)_tcslen(sPre)) > 0)
	{
		for(int n = 0; n < nCnt; n++)
		{
			const TCHAR* sTxt = (const TCHAR*)list->ItemGet(n).GetBuf();
			if(_tcsncmp(sPre, sTxt, nLen) == 0)
			{
				return n;
			}
		}
	}
	return -1;
}

///////////////////////////////////////////////////////////////////////////////
//MainForm
MainForm::MainForm()
{
	memset(buf, 0, BUF_LEN * sizeof(TCHAR));
	InitializeComponent();

	//show it until Form_Load
	this->SetVisible(false);
}

MainForm::~MainForm()
{
}

void MainForm::InitializeComponent()
{
	int nTop = 5;
	int nLeft = 5;
	btnMNew = new Button();
	btnMNew->SetName(_T("btnMNew"));
	btnMNew->SetText(_T("New Message"));
	btnMNew->SetLocation(nLeft, nTop);
	btnMNew->SetSize(80, 18);
	btnMNew->Click += EventHandler((Object*)this, (EventFun)&MainForm::Control_Click);
	this->AddControl(btnMNew);

	nLeft += 82;
	btnMUpdate = new Button();
	btnMUpdate->SetName(_T("btnMUpdate"));
	btnMUpdate->SetText(_T("Update"));
	btnMUpdate->SetLocation(nLeft, nTop);
	btnMUpdate->SetSize(80, 18);
	btnMUpdate->Click += EventHandler((Object*)this, (EventFun)&MainForm::Control_Click);
	this->AddControl(btnMUpdate);

	nLeft += 82;
	btnMDel = new Button();
	btnMDel->SetName(_T("btnMDel"));
	btnMDel->SetText(_T("Del"));
	btnMDel->SetLocation(nLeft, nTop);
	btnMDel->SetSize(80, 18);
	btnMDel->Click += EventHandler((Object*)this, (EventFun)&MainForm::Control_Click);
	this->AddControl(btnMDel);

	nLeft += 87;
	btnMSaveAs = new Button();
	btnMSaveAs->SetName(_T("btnMSaveAs"));
	btnMSaveAs->SetText(_T("Save As"));
	btnMSaveAs->SetLocation(nLeft, nTop);
	btnMSaveAs->SetSize(80, 18);
	btnMSaveAs->Click += EventHandler((Object*)this, (EventFun)&MainForm::Control_Click);
	this->AddControl(btnMSaveAs);

	nLeft += 82;
	btnMLoad = new Button();
	btnMLoad->SetName(_T("btnMLoad"));
	btnMLoad->SetText(_T("Load"));
	btnMLoad->SetLocation(nLeft, nTop);
	btnMLoad->SetSize(80, 18);
	btnMLoad->Click += EventHandler((Object*)this, (EventFun)&MainForm::Control_Click);
	//btnMLoad->Anchor = (AnchorStyles::Type)(AnchorStyles::Top | AnchorStyles::Right);
	this->AddControl(btnMLoad);

	nLeft += 82;
	toolOnTop = new CheckBox();
	toolOnTop->SetName(_T("toolOnTop"));
	toolOnTop->SetText(_T("Always OnTop"));
	toolOnTop->SetLocation(nLeft, nTop);
	toolOnTop->SetSize(200, 17);
	toolOnTop->Click += EventHandler((Object*)this, (EventFun)&MainForm::toolOnTop_Click);
	//toolOnTop->SetOuterStyle(OuterStyle::AutoConfig, true);
	this->AddControl(toolOnTop);

	nTop += 20;
	label = new Label();
	label->SetName(_T("labKey"));
	label->SetText(_T("Language Manager,"));
	label->SetLocation(5, nTop + 2);
	label->SetSize(110, 17);
	this->AddControl(label);

	labFile = new Label();
	labFile->SetName(_T("labFile"));
	labFile->SetText(_T(""));
	labFile->SetLocation(115, nTop + 2);
	labFile->SetSize(600, 17);
	this->AddControl(labFile);

#define LIST_KEY_WIDTH 240
	nTop += 20;
	lstMsgs = new ListBox();
	lstMsgs->SetName(_T("lstMsgs"));
	lstMsgs->SetLocation(5, nTop);
	lstMsgs->SetSize(LIST_KEY_WIDTH, 600 - nTop - 3);
	lstMsgs->SelectedIndexChanged += EventHandler((Object*)this, (EventFun)&MainForm::lstMsgs_SelectedIndexChanged);
	lstMsgs->Anchor = (AnchorStyles::Type)(AnchorStyles::Top | AnchorStyles::Left | AnchorStyles::Bottom);
	this->AddControl(lstMsgs);

	label = new Label();
	label->SetName(_T("labAName"));
	label->SetText(_T("AppName:"));
	label->SetLocation(LIST_KEY_WIDTH + 9, nTop + 2);
	label->SetSize(60, 17);
	this->AddControl(label);
	editAName = new TextBox();
	editAName->SetName(_T("editAName"));
	editAName->SetText(_T(""));
	editAName->SetLocation(LIST_KEY_WIDTH + 9 + 60, nTop);
	editAName->SetSize(200, 17);
	this->AddControl(editAName);

	label = new Label();
	label->SetName(_T("labAKey1"));
	label->SetText(_T("AppKey1:"));
	label->SetLocation(LIST_KEY_WIDTH + 9 + 70 + 200, nTop + 2);
	label->SetSize(50, 17);
	this->AddControl(label);
	editAKey1 = new TextBox();
	editAKey1->SetName(_T("editAKey1"));
	editAKey1->SetText(_T(""));
	editAKey1->SetLocation(LIST_KEY_WIDTH + 9 + 70 + 200 + 50, nTop);
	editAKey1->SetSize(120, 17);
	this->AddControl(editAKey1);

	nTop += 25;
	label = new Label();
	label->SetName(_T("labLName"));
	label->SetText(_T("LangName:"));
	label->SetLocation(LIST_KEY_WIDTH + 9, nTop + 2);
	label->SetSize(60, 17);
	this->AddControl(label);
	editLName = new TextBox();
	editLName->SetName(_T("editLName"));
	editLName->SetText(_T(""));
	editLName->SetLocation(LIST_KEY_WIDTH + 9 + 60, nTop);
	editLName->SetSize(200, 17);
	this->AddControl(editLName);

	label = new Label();
	label->SetName(_T("labLFile"));
	label->SetText(_T("LangFile:"));
	label->SetLocation(LIST_KEY_WIDTH + 9 + 70 + 200, nTop + 2);
	label->SetSize(50, 17);
	this->AddControl(label);
	cbLangFile = new ComboBox();
	cbLangFile->SetName(_T("cbLangFile"));
	cbLangFile->SetComboBoxStyle(ComboBoxStyle::DropDownList);
	cbLangFile->SetLocation(LIST_KEY_WIDTH + 9 + 70 + 200 + 50, nTop);
	cbLangFile->SetSize(120, 145);
	this->AddControl(cbLangFile);

	nTop += 22;
	label = new Label();
	label->SetName(_T("labLangs"));
	label->SetText(_T("Language:"));
	label->SetLocation(LIST_KEY_WIDTH + 9, nTop + 2);
	label->SetSize(50, 17);
	this->AddControl(label);

	nLeft = LIST_KEY_WIDTH + 9 + 53;
	btnLNew = new Button();
	btnLNew->SetName(_T("btnLNew"));
	btnLNew->SetText(_T("New Language"));
	btnLNew->SetLocation(nLeft, nTop);
	btnLNew->SetSize(80, 18);
	btnLNew->Click += EventHandler((Object*)this, (EventFun)&MainForm::Control_Click);
	this->AddControl(btnLNew);
	nLeft += 82;
	btnLUpdate = new Button();
	btnLUpdate->SetName(_T("btnLUpdate"));
	btnLUpdate->SetText(_T("Update"));
	btnLUpdate->SetLocation(nLeft, nTop);
	btnLUpdate->SetSize(80, 18);
	btnLUpdate->Click += EventHandler((Object*)this, (EventFun)&MainForm::Control_Click);
	this->AddControl(btnLUpdate);
	nLeft += 82;
	btnLDel = new Button();
	btnLDel->SetName(_T("btnLDel"));
	btnLDel->SetText(_T("Del"));
	btnLDel->SetLocation(nLeft, nTop);
	btnLDel->SetSize(80, 18);
	btnLDel->Click += EventHandler((Object*)this, (EventFun)&MainForm::Control_Click);
	this->AddControl(btnLDel);

	nTop += 20;
	lstLangs = new ListBox();
	lstLangs->SetName(_T("lstLangs"));
	lstLangs->SetLocation(LIST_KEY_WIDTH + 9, nTop);
	lstLangs->SetSize(600 - LIST_KEY_WIDTH - 12, 200);
	lstLangs->SelectedIndexChanged += EventHandler((Object*)this, (EventFun)&MainForm::lstLangs_SelectedIndexChanged);
	lstLangs->Anchor = (AnchorStyles::Type)(AnchorStyles::Top | AnchorStyles::Left | AnchorStyles::Right);
	this->AddControl(lstLangs);

	nTop += 200;
	label = new Label();
	label->SetName(_T("labMID"));
	label->SetText(_T("Msg ID:"));
	label->SetLocation(LIST_KEY_WIDTH + 9, nTop + 2);
	label->SetSize(80, 17);
	this->AddControl(label);
	nTop += 19;
	editMsgID = new TextBox();
	editMsgID->SetName(_T("editMsg"));
	editMsgID->SetText(_T(""));
	editMsgID->SetLocation(LIST_KEY_WIDTH + 9, nTop);
	editMsgID->SetSize(600 - LIST_KEY_WIDTH - 12, 17);
	editMsgID->Anchor = (AnchorStyles::Type)(AnchorStyles::Top | AnchorStyles::Left | AnchorStyles::Right);
	this->AddControl(editMsgID);

	nTop += 20;
	label = new Label();
	label->SetName(_T("labMsg"));
	label->SetText(_T("Msg:"));
	label->SetLocation(LIST_KEY_WIDTH + 9, nTop + 2);
	label->SetSize(80, 17);
	this->AddControl(label);
	nTop += 19;
	editMsg = new TextBox();
	editMsg->SetName(_T("editMsg"));
	editMsg->SetText(_T(""));
	editMsg->SetMultiline(true);
	editMsg->SetLocation(LIST_KEY_WIDTH + 9, nTop);
	editMsg->SetSize(600 - LIST_KEY_WIDTH - 12, 57);
	editMsg->Anchor = (AnchorStyles::Type)(AnchorStyles::Top | AnchorStyles::Left | AnchorStyles::Right);
	this->AddControl(editMsg);

	nTop = 12;
	nLeft = 5;


	this->SetName(_T("uuLang"));
	this->SetText(App::GetNameVersion());
	this->SetConfigName(_T(""));
	this->SetLocation(0, 0);
	this->SetClientSize(600, 600);
	this->SetSize(700, 560);
	//this->SetMinSize(660, 520);
	this->SetCenter();
	this->Load += EventHandler((Object*)this, (EventFun)&MainForm::Form_Load);
	this->Closed += EventHandler((Object*)this, (EventFun)&MainForm::Form_Closed);
	this->SetOuterStyle(OuterStyle::AutoConfig, true);

	SetLang(*CC_APP_GetLang(), cc::Str(CC_CONFIG_LANGSECTION));
}

void MainForm::Form_Load(Object* sender, EventArgs* e)
{
	//load config
	this->Config(true, *CC_APP_GetIni(), cc::Str(_T("Main")));
	sConfigPath = CC_APP_GetIni()->Get(_T("Main.CfgPath"));
	if(sConfigPath.IsNULL())
	{
		sConfigPath.ValueOf(_T(""));
	}
	Config_Load(sConfigPath);
	labFile->SetText(_T("Auto Save to: ") + sConfigPath);

	//show it until Form_Load
	this->SetVisible(true);
}

void MainForm::Form_Closed(Object* sender, EventArgs* e)
{
	//save config
	this->Config(false, *CC_APP_GetIni(), cc::Str(_T("Main")));

	if(!cc::Util::fExist((TCHAR*)sConfigPath.GetBuf()))
	{
		if(lstMsgs->ItemCount() > 0)
		{
			Control_Click(btnMSaveAs, NULL);
		}
	}
	else
	{
		Config_Save(sConfigPath);
	}
	CC_APP_GetIni()->Set(_T("Main.CfgPath"), (TCHAR*)sConfigPath.GetBuf());
}

void MainForm::Config_Save(cc::Str& sPath)
{
	cc::KeyArr keyArr;
	cc::Str sKey;
	int nCnt = lstLangs->ItemCount();
	keyArr.Set(_T("Main.Data.Lang.Count"), (DWORD)nCnt);
	for(int n = 0; n < nCnt; n++)
	{
		sKey.Format(_T("Main.Data.Lang.Item%d"), n);
		keyArr.Set((TCHAR*)sKey.GetBuf(), cbLangFile->ItemGet(n));
	}

	nCnt = lstMsgs->ItemCount();
	keyArr.Set(_T("Main.Data.Msg.Count"), (DWORD)nCnt);
	keyArr.Set(_T("Main.Data.Msg.Index"), (DWORD)lstMsgs->GetSelectedIndex());
	for(int n = 0; n < nCnt; n++)
	{
		sKey.Format(_T("Main.Data.Msg.Item%d"), n);
		keyArr.Set((TCHAR*)sKey.GetBuf(), lstMsgs->ItemGet(n));
	}
	keyArr.Save(sPath, LangCode::TCHAR, true);
}
void MainForm::Config_Load(cc::Str& sPath)
{
	cc::KeyArr keyArr;
	keyArr.Add(sPath, LangCode::TCHAR, true);
	cc::Str sKey;
	lstLangs->ItemClear();
	int nCnt = (int)keyArr.Get(_T("Main.Data.Lang.Count"), 0);
	for(int n = 0; n < nCnt; n++)
	{
		sKey.Format(_T("Main.Data.App.Item%d"), n);
		if(keyArr.Exist(sKey))
		{
			lstLangs->ItemSet(n, keyArr.Get(sKey));
		}
	}

	lstMsgs->ItemClear();
	nCnt = (int)keyArr.Get(_T("Main.Data.Msg.Count"), 0);
	for(int n = 0; n < nCnt; n++)
	{
		sKey.Format(_T("Main.Data.Msg.Item%d"), n);
		if(keyArr.Exist(sKey))
		{
			lstMsgs->ItemSet(n, keyArr.Get(sKey));
		}
	}
	int nInd = (int)keyArr.Get(_T("Main.Data.Msg.Index"), 0);
	if(nInd < 0 || nInd >= nCnt)
	{
		nInd = 0;
	}
	lstMsgs->SetSelectedIndex(nInd);
	lstMsgs_SelectedIndexChanged(NULL, NULL);
}

void MainForm::lstMsgs_SelectedIndexChanged(Object* sender, EventArgs* e)
{
	int nInd = lstMsgs->GetSelectedIndex();
	if(nInd < 0)
	{
		return;
	}

	editMsg->SetText(_T(""));
}
void MainForm::lstLangs_SelectedIndexChanged(Object* sender, EventArgs* e)
{
	int nInd = lstLangs->GetSelectedIndex();
	if(nInd < 0)
	{
		return;
	}

}


void MainForm::Control_Click(Object* sender, EventArgs* e)
{
	cc::Str sKey;
	cc::Str sVal;
	if(sender == btnMNew)
	{
		sVal = cc::Str(editMsgID->GetText()).Trim();
		if(sVal.GetLength() < 1)
		{
			::MessageBox(m_hWnd, _T("No Message ID."), TCHAR_LANG("Lang.S_MsgBox_Title_Info"), MB_OK);
			return;
		}
		sVal.Append(APPINFO_SPLIT);
		int nAppInd = FindStartsWith(lstMsgs, sVal);
		if(nAppInd >= 0)
		{
			::MessageBox(m_hWnd, _T("Message ID Already Exit."), TCHAR_LANG("Lang.S_MsgBox_Title_Info"), MB_OK);
			return;
		}

		int nInd = lstMsgs->ItemAdd(sVal);
		lstMsgs->SetSelectedIndex(nInd);
		//set value
		Control_Click(btnMUpdate, e);
	}
	else if(sender == btnMUpdate)
	{
		int nInd = lstMsgs->GetSelectedIndex();
		if(nInd < 0)
		{
			::MessageBox(m_hWnd, _T("No Select."), TCHAR_LANG("Lang.S_MsgBox_Title_Info"), MB_OK);
			return;
		}
		sVal = cc::Str(editMsgID->GetText()).Trim();
		if(sVal.GetLength() < 1)
		{
			::MessageBox(m_hWnd, _T("No Message ID."), TCHAR_LANG("Lang.S_MsgBox_Title_Info"), MB_OK);
			return;
		}
		sVal.Append(APPINFO_SPLIT);
		int nAppInd = FindStartsWith(lstMsgs, sVal);
		if(nAppInd >= 0 && nAppInd != nInd)
		{
			::MessageBox(m_hWnd, _T("Message Already Exit."), TCHAR_LANG("Lang.S_MsgBox_Title_Info"), MB_OK);
			return;
		}
		lstMsgs->ItemSet(nInd, sVal);
	}
	else if(sender == btnMDel)
	{
		int nInd = lstMsgs->GetSelectedIndex();
		if(nInd < 0)
		{
			::MessageBox(m_hWnd, _T("No Select."), TCHAR_LANG("Lang.S_MsgBox_Title_Info"), MB_OK);
			return;
		}
		if(IDYES != (int)::MessageBox(m_hWnd, _T("Delete it?"), TCHAR_LANG("Lang.S_MsgBox_Title_Question"), MB_YESNO | MB_ICONQUESTION))
		{
			return;
		}
		lstMsgs->ItemDel(nInd);
		if(lstMsgs->ItemCount() > 0)
		{
			lstMsgs->SetSelectedIndex(nInd > 0 ? nInd - 1 : 0);
			lstMsgs_SelectedIndexChanged(NULL, NULL);
		}
	}


	//save&load
	else if(sender == btnMSaveAs)
	{
		sVal = cc::Util::selectSaveAs((TCHAR*)sConfigPath.GetBuf(), _T("Config File (*.ini)\0*.ini\0All Files (*.*)\0*.*\0"), _T(".ini"), m_hWnd, _T("Config file Save As..."));
		if(sVal.GetLength() < 1)
		{
			return;
		}
		Config_Save(sVal);
		sConfigPath = sVal;
		labFile->SetText(_T("Auto Save to: ") + sConfigPath);
	}
	else if(sender == btnMLoad)
	{
		sVal = cc::Util::selectOpenFile((TCHAR*)sConfigPath.GetBuf(), _T("Config File (*.ini)\0*.ini\0All Files (*.*)\0*.*\0"), _T(".ini"), m_hWnd, _T("Open Config file"));
		if(sVal.GetLength() < 1)
		{
			return;
		}
		Config_Load(sVal);
		sConfigPath = sVal;
		labFile->SetText(_T("Auto Save to: ") + sConfigPath);
	}
}

void MainForm::toolOnTop_Click(Object* sender, EventArgs* e)
{
	bool chk = toolOnTop->GetChecked();
	toolOnTop->SetChecked(chk);
	::SetWindowPos(m_hWnd, (chk) ? HWND_TOPMOST : HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}

void MainForm::menuExit_Click(Object* sender, EventArgs* e)
{
	App::Exit();
}
