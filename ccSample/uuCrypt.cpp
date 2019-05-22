#include "uuCrypt.h"
using namespace cc;
using namespace cc::win;

#define REG_KEYS "[#KEYSTART#]"
#define REG_KEYE "[#KEYEND#]"
#define APPINFO_SPLIT _T(";\r")
#ifdef _UNICODE
	#define REG_REGKEY _T("SOFTWARE\\uuware\\uuCrypt")
#else
	#define REG_REGKEY _T("SOFTWARE\\uuware\\uuCrypt_ansi")
#endif

///////////////////////////////////////////////////////////////////////////////
//WinMain
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	cc::win::App::SetDefaultTransparent(true);
	cc::win::App::SetInstance(hInstance);
	//should set this before cc::Config::GetLang(),and SetIsAutoSave is before LoadLang
	cc::win::App::GetConfig()->SetIsAutoSave(true, true, REG_REGKEY);
	cc::win::App::GetConfig()->LoadLang(CC_CONFIG_LANGENG);
	//the name is used for CreateMutex"s flages,cannot change to other lang
	cc::win::App::SetNameVersion(_T("uuCrypt[uuware.com]"), _T("1.00"));
	//cc::win::App::SetNameVersion(_T(APPNAME), _T(APPVERSION));

	MainForm* form = new MainForm();
	cc::win::App::Run(form);
	form->Dispose();
	delete form;
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
//MainForm
MainForm::MainForm()
{
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
	label = new Label();
	label->SetText(_T("PassWord:"));
	label->SetLocation(5, nTop + 2);
	label->SetSize(80, 17);
	this->AddControl(label);
	editPass = new TextBox();
	editPass->SetName(_T("editPass"));
	editPass->SetText(_T(""));
	editPass->SetLocationOffset(label, false, 0, -3);
	editPass->SetSize(130, 17);
	editPass->SetStyle(StyleType::Style, 0, ES_PASSWORD);
	editPass->SetOuterStyle(OuterStyle::AutoConfig, true);
	this->AddControl(editPass);

	nTop += 20;
	label = new Label();
	label->SetText(_T("Tips for PWD:"));
	label->SetLocation(5, nTop + 2);
	label->SetSize(80, 17);
	this->AddControl(label);
	TextBox* editTips = new TextBox();
	editTips->SetName(_T("editTips"));
	editTips->SetText(_T(""));
	editTips->SetLocationOffset(label, false, 0, -3);
	editTips->SetSize(130, 17);
	editTips->SetOuterStyle(OuterStyle::AutoConfig, true);
	this->AddControl(editTips);

	chkSavePW = new CheckBox();
	chkSavePW->SetName(_T("chkSavePW"));
	chkSavePW->SetText(_T("Save PassWord"));
	chkSavePW->SetLocationOffset(label, true, 0, 3);
	chkSavePW->SetSize(140, 17);
	chkSavePW->SetOuterStyle(OuterStyle::AutoConfig, true);
	this->AddControl(chkSavePW);
	chkDelOrg = new CheckBox();
	chkDelOrg->SetName(_T("chkDelOrg"));
	chkDelOrg->SetText(_T("Del Orignal Files After Crypted"));
	chkDelOrg->SetLocationOffset(chkSavePW, true, 0, 1);
	chkDelOrg->SetSize(200, 17);
	chkDelOrg->SetOuterStyle(OuterStyle::AutoConfig, true);
	this->AddControl(chkDelOrg);

	btnCrypt = new Button();
	btnCrypt->SetName(_T("btnCrypt"));
	btnCrypt->SetText(_T("Crypt"));
	btnCrypt->SetLocationOffset(editPass, false, 5, 0);
	btnCrypt->SetSize(70, 27);
	btnCrypt->Click += EventHandler((Object*)this, (EventFun)&MainForm::Control_Click);
	this->AddControl(btnCrypt);
	btnDeCrypt = new Button();
	btnDeCrypt->SetName(_T("btnDeCrypt"));
	btnDeCrypt->SetText(_T("DeCrypt"));
	btnDeCrypt->SetLocationOffset(btnCrypt, false, 1, 0);
	btnDeCrypt->SetSize(70, 27);
	btnDeCrypt->Click += EventHandler((Object*)this, (EventFun)&MainForm::Control_Click);
	this->AddControl(btnDeCrypt);

	nTop += 60;
	label = new Label();
	label->SetText(_T("Files List:"));
	label->SetLocation(5, nTop + 3);
	label->SetSize(100, 17);
	this->AddControl(label);

	nTop += 20;
	lstFiles = new ListBox();
	lstFiles->SetName(_T("lstFiles"));
	lstFiles->SetLocation(5, nTop);
	lstFiles->SetSize(600 - 4, 600 - nTop - 3);
	lstFiles->Anchor = (AnchorStyles::Type)(AnchorStyles::Top | AnchorStyles::Left | AnchorStyles::Right | AnchorStyles::Bottom);
	this->AddControl(lstFiles);

	btnFAdd = new Button();
	btnFAdd->SetText(_T("Add File"));
	btnFAdd->SetLocationOffset(label, false, 10, -2);
	btnFAdd->SetSize(80, 18);
	btnFAdd->Click += EventHandler((Object*)this, (EventFun)&MainForm::Control_Click);
	this->AddControl(btnFAdd);
	btnFDel = new Button();
	btnFDel->SetText(_T("Remove File"));
	btnFDel->SetLocationOffset(btnFAdd, false, 1, 0);
	btnFDel->SetSize(80, 18);
	btnFDel->Click += EventHandler((Object*)this, (EventFun)&MainForm::Control_Click);
	this->AddControl(btnFDel);
	btnFDelAll = new Button();
	btnFDelAll->SetText(_T("Remove All"));
	btnFDelAll->SetLocationOffset(btnFDel, false, 1, 0);
	btnFDelAll->SetSize(80, 18);
	btnFDelAll->Click += EventHandler((Object*)this, (EventFun)&MainForm::Control_Click);
	this->AddControl(btnFDelAll);

	this->SetOuterStyle(OuterStyle::AutoConfig, true);
	this->SetName(_T("uuCrypt"));
	this->SetText(App::GetNameVersion());
	this->SetConfigName(_T(""));
	this->SetLocation(0, 0);
	this->SetClientSize(600, 600);
	this->SetSize(390, 190);
	//this->SetMinSize(660, 520);
	this->SetCenter();
	this->Load += EventHandler((Object*)this, (EventFun)&MainForm::Form_Load);
	this->Closed += EventHandler((Object*)this, (EventFun)&MainForm::Form_Closed);

	SetLang(*CC_APP_GetLang(), cc::Str(CC_CONFIG_LANGSECTION));
}

void MainForm::Form_Load(Object* sender, EventArgs* e)
{
	//load config
	this->Config(true, *CC_APP_GetIni(), cc::Str(_T("Main")));
	Config_Load();

	//show it until Form_Load
	this->SetVisible(true);
}

void MainForm::Form_Closed(Object* sender, EventArgs* e)
{
	if(!chkSavePW->GetChecked())
	{
		editPass->SetText(_T(""));
	}
	//save config
	this->Config(false, *CC_APP_GetIni(), cc::Str(_T("Main")));

	Config_Save();
}

void MainForm::Config_Save()
{
	cc::KeyArr* keyArr = CC_APP_GetIni();
	cc::Str sKey;
	int nCnt = lstFiles->ItemCount();
	keyArr->Set(_T("Main.F.Count"), (DWORD)nCnt);
	keyArr->Set(_T("Main.F.Index"), (DWORD)lstFiles->GetSelectedIndex());
	for(int n = 0; n < nCnt; n++)
	{
		sKey.Format(_T("Main.F.Item%d"), n);
		keyArr->Set((TCHAR*)sKey.GetBuf(), lstFiles->ItemGet(n));
	}
}
void MainForm::Config_Load()
{
	cc::KeyArr* keyArr = CC_APP_GetIni();
	cc::Str sKey;
	lstFiles->ItemClear();
	int nCnt = (int)keyArr->Get(_T("Main.F.Count"), 0);
	for(int n = 0; n < nCnt; n++)
	{
		sKey.Format(_T("Main.F.Item%d"), n);
		if(keyArr->Exist(sKey))
		{
			lstFiles->ItemSet(n, keyArr->Get(sKey));
		}
	}
	int nInd = (int)keyArr->Get(_T("Main.F.Index"), 0);
	if(nInd < 0 || nInd >= nCnt)
	{
		nInd = 0;
	}
	lstFiles->SetSelectedIndex(nInd);
}

bool MainForm::EnCryptOne(cc::Str& sFile, const char* pwd, int klen, bool isDelOrg)
{
	void* buf = NULL;
	int nLen = cc::Util::fRead(&buf, sFile);
	if(nLen < 0)
	{
		return false;
	}
	if(nLen > 0)
	{
		cc::Util::teaEnCrypt((char*)buf, nLen, pwd, klen, 5);
	}
	cc::Util::fWrite(buf, nLen, LangCode::SYSTEM, sFile + _T(".crypt"));
	if(isDelOrg)
	{
		cc::Util::fDel(sFile);
	}
	return true;
}
bool MainForm::DeCryptOne(cc::Str& sFile, const char* pwd, int klen)
{
	cc::Str sFileCry = sFile + _T(".crypt");
	if(!cc::Util::fExist(sFileCry))
	{
		return false;
	}
	void* buf = NULL;
	int nLen = cc::Util::fRead(&buf, sFileCry);
	if(nLen < 0)
	{
		return false;
	}
	if(nLen > 0)
	{
		cc::Util::teaDeCrypt((char*)buf, nLen, pwd, klen, 5);
	}
	cc::Util::fWrite(buf, nLen, LangCode::SYSTEM, sFile);
	return true;
}

void MainForm::Control_Click(Object* sender, EventArgs* e)
{
	cc::Str sKey;
	cc::Str sVal;
	if(sender == btnCrypt)
	{
		if(IDYES != (int)::MessageBox(m_hWnd, _T("Crypt it?\r\n(While Submit Files)"), _T("Question Message"), MB_YESNO | MB_ICONQUESTION))
		{
			return;
		}
		int nCnt = lstFiles->ItemCount();
		if(nCnt <= 0)
		{
			::MessageBox(m_hWnd, _T("No Files."), _T("Info Message"), MB_OK);
			return;
		}
		int klen = editPass->GetTextLenA();
		if(klen <= 0)
		{
			::MessageBox(m_hWnd, _T("No PassWord."), _T("Info Message"), MB_OK);
			return;
		}
		const char*pwd = editPass->GetTextA();
		bool isDelOrg = chkDelOrg->GetChecked();

		for(int n = 0; n < nCnt; n++)
		{
			sVal = lstFiles->ItemGet(n);
			if(!cc::Util::fExist(sVal))
			{
				cc::Str smsg;
				smsg.Format(_T("Not Exist, Index:%d, File:%s"), n, (TCHAR*)sVal.GetBuf());
				::MessageBox(m_hWnd, smsg, _T("Info Message"), MB_OK);
				return;
			}
			EnCryptOne(sVal, pwd, klen, isDelOrg);
		}
		::MessageBox(m_hWnd, _T("Crypt Over."), _T("Info Message"), MB_OK);
	}
	else if(sender == btnDeCrypt)
	{
		if(IDYES != (int)::MessageBox(m_hWnd, _T("DeCrypt it?\r\n(While Downloaded, and Restore Files)"), _T("Question Message"), MB_YESNO | MB_ICONQUESTION))
		{
			return;
		}
		int nCnt = lstFiles->ItemCount();
		if(nCnt <= 0)
		{
			::MessageBox(m_hWnd, _T("No Files."), _T("Info Message"), MB_OK);
			return;
		}
		int klen = editPass->GetTextLenA();
		if(klen <= 0)
		{
			::MessageBox(m_hWnd, _T("No PassWord."), _T("Info Message"), MB_OK);
			return;
		}
		const char*pwd = editPass->GetTextA();

		for(int n = 0; n < nCnt; n++)
		{
			sVal = lstFiles->ItemGet(n);
			DeCryptOne(sVal, pwd, klen);
		}
		::MessageBox(m_hWnd, _T("DeCrypt Over."), _T("Info Message"), MB_OK);
	}
	else if(sender == btnFAdd)
	{
		cc::List<cc::Str> arrFiles;
		sVal = cc::Util::selectOpenFile(_T(""), _T("All Files (*.*)\0*.*\0\0"), _T(".*"), m_hWnd, _T("Open file for crypt"), 0, true, &arrFiles);
		if(sVal.GetLength() < 1)
		{
			return;
		}
		for(int n = arrFiles.GetSize() - 1; n >= 0; n--)
		{
			sVal = arrFiles.GetKey(n);
			if(cc::Util::fExist(sVal))
			{
				if(sVal.EndsWith(_T(".crypt"), true))
				{
					sVal.Remove(sVal.GetLength() - 6);
				}
				int nInd = lstFiles->FindStringExact(0, sVal);
				if(nInd < 0)
				{
					lstFiles->ItemAdd(sVal);
					//::MessageBox(m_hWnd, _T("Already Exist."), _T("Info Message"), MB_OK);
					//return;
				}
			}
		}
	}
	else if(sender == btnFDel)
	{
		int nInd = lstFiles->GetSelectedIndex();
		if(nInd < 0)
		{
			::MessageBox(m_hWnd, _T("No Select."), _T("Info Message"), MB_OK);
			return;
		}
		lstFiles->ItemDel(nInd);
	}
	else if(sender == btnFDelAll)
	{
		lstFiles->ItemClear();
	}
}

void MainForm::menuExit_Click(Object* sender, EventArgs* e)
{
	App::Exit();
}
