#include "uuCryptFile.h"
using namespace cc;
using namespace cc::win;

#ifdef _UNICODE
	#define REG_REGKEY _T("SOFTWARE\\uuware\\uuCryptFile")
#else
	#define REG_REGKEY _T("SOFTWARE\\uuware\\uuCryptFile_ansi")
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
	cc::win::App::SetNameVersion(_T("uuCryptFile[uuware.com]"), _T("1.00"));

	cc::List<cc::Str>* arrCommands = new cc::List<cc::Str>();
	cc::Util::GetCommandLine(*arrCommands);
	CC_ASSERT(arrCommands->GetSize() > 0);
	//remove self
	arrCommands->RemoveAt(0);
	//be deleted at Form_Load
	CC_APP_SetVarious(_T("CommandLine"), arrCommands);

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
	imageList = new ImageList();
	imageList->SetIconSize(-1, -1);
	imageList->SetIcon(IDI_ICON_MAIN, IDI_ICON_MAIN);
	this->DisposeItemAdd(imageList);

	int nTop = 5;
	label = new Label();
	label->SetText(_T("PassWord:"));
	label->SetLocation(5, nTop + 2);
	label->SetSize(70, 17);
	this->AddControl(label);
	editPass = new TextBox();
	editPass->SetName(_T("editPass"));
	editPass->SetText(_T(""));
	editPass->SetLocationOffset(label, false, 0, -1);
	editPass->SetSize(140, 17);
	editPass->SetStyle(StyleType::Style, 0, ES_PASSWORD);
	editPass->SetOuterStyle(OuterStyle::AutoConfig, true);
	this->AddControl(editPass);

	chkShowPW = new CheckBox();
	chkShowPW->SetName(_T("chkShowPW"));
	chkShowPW->SetText(_T("Show PassWord"));
	chkShowPW->SetLocationOffset(label, true, 0, 3);
	chkShowPW->SetSize(140, 17);
	chkShowPW->SetOuterStyle(OuterStyle::AutoConfig, true);
	chkShowPW->Click += EventHandler((Object*)this, (EventFun)&MainForm::Control_Click);
	this->AddControl(chkShowPW);
	chkDelOrg = new CheckBox();
	chkDelOrg->SetName(_T("chkDelOrg"));
	chkDelOrg->SetText(_T("Del Orignal Files After Crypted"));
	chkDelOrg->SetLocationOffset(chkShowPW, true, 0, 1);
	chkDelOrg->SetSize(200, 17);
	chkDelOrg->SetOuterStyle(OuterStyle::AutoConfig, true);
	this->AddControl(chkDelOrg);
	chkConfirm = new CheckBox();
	chkConfirm->SetName(_T("chkConfirm"));
	chkConfirm->SetText(_T("NOT show confirm message!"));
	chkConfirm->SetLocationOffset(chkDelOrg, true, 0, 8);
	chkConfirm->SetSize(200, 17);
	chkConfirm->SetOuterStyle(OuterStyle::AutoConfig, true);
	this->AddControl(chkConfirm);

	btnCrypt = new Button();
	btnCrypt->SetName(_T("btnCrypt"));
	btnCrypt->SetText(_T("Select or Drag File(s)..."));
	btnCrypt->SetLocationOffset(editPass, false, 1, 0);
	btnCrypt->SetSize(160, 27);
	btnCrypt->Click += EventHandler((Object*)this, (EventFun)&MainForm::Control_Click);
	this->AddControl(btnCrypt);

	//btnReset = new Button();
	//btnReset->SetName(_T("btnReset"));
	//btnReset->SetText(_T("Reset Setting"));
	//btnReset->SetLocationOffset(btnCrypt, true, 0, 2);
	//btnReset->SetSize(130, 27);
	//btnReset->Click += EventHandler((Object*)this, (EventFun)&MainForm::Control_Click);
	//this->AddControl(btnReset);

	this->SetStyle(StyleType::ExStyle, 0, WS_EX_ACCEPTFILES);
	this->SetOuterStyle(OuterStyle::AutoConfig, true);
	this->SetName(_T("uuCryptFile"));
	this->SetText(App::GetNameVersion());
	this->SetConfigName(_T(""));
	this->SetCenter();
	this->SetClientSize(600, 600);
	this->SetSize(390, 122);
	//here need be 32X32
	this->SetIcon(imageList, IDI_ICON_MAIN);
	//this->SetMinSize(390, 122);
	this->Load += EventHandler((Object*)this, (EventFun)&MainForm::Form_Load);
	this->Closed += EventHandler((Object*)this, (EventFun)&MainForm::Form_Closed);

	SetLang(*CC_APP_GetLang(), cc::Str(CC_CONFIG_LANGSECTION));
}

LRESULT MainForm::WndProc(EventArgs *e)
{
	if(e->Msg == WM_DROPFILES)
	{
		::SetForegroundWindow(m_hWnd);
		::BringWindowToTop(m_hWnd);
		if(IsConfigOK(true))
		{
			HDROP hdrop = (HDROP)e->WParam;
			cc::List<cc::Str>* arrFiles = new cc::List<cc::Str>();
			cc::Util::GetDragFiles(hdrop, *arrFiles);

			bool isDelOrg = chkDelOrg->GetChecked();
			cc::Str sPass = cc::Str(LangCode::SYSTEM, editPass->GetTextA()).Trim();
			int nSize = arrFiles->GetSize();
			for(int n = 0; n < nSize; n++)
			{
				cc::Str sVal = arrFiles->GetKey(n);
				TreateOne(sVal, (char*)sPass, sPass.GetLength(), isDelOrg);
			}

			//delete it
			delete arrFiles;
		}
		return 0;
	}
	return Form::WndProc(e);
}

bool MainForm::IsConfigOK(bool isMsg)
{
	cc::Str sPass = cc::Str(LangCode::SYSTEM, editPass->GetTextA()).Trim();
	if(sPass.GetLength() < 1)
	{
		::MessageBox(m_hWnd, _T("Password is empty."), _T("Information Message"), MB_OK);
		return false;
	}
	return true;
}

void MainForm::Form_Load(Object* sender, EventArgs* e)
{
	//load config
	this->Config(true, *CC_APP_GetIni(), cc::Str(_T("Main")));
	Control_Click(chkShowPW, NULL);

	//show it until Form_Load
	this->SetVisible(true);

	//if only one file,do it before show form
	cc::List<cc::Str>* arrCommands = (cc::List<cc::Str>*)CC_APP_GetVarious(_T("CommandLine"));
	int nSize = (arrCommands == NULL ? 0 : arrCommands->GetSize());
	if(nSize > 0)
	{
		if(IsConfigOK(true))
		{
			bool isDelOrg = chkDelOrg->GetChecked();
			cc::Str sPass = cc::Str(LangCode::SYSTEM, editPass->GetTextA()).Trim();
			for(int n = 0; n < nSize; n++)
			{
				cc::Str sVal = arrCommands->GetKey(n);
				TreateOne(sVal, (char*)sPass, sPass.GetLength(), isDelOrg);
			}
			nSize = -1;
		}
	}
	if(arrCommands != NULL)
	{
		//delete it
		delete arrCommands;
		//remove it
		CC_APP_SetVarious(_T("CommandLine"), NULL);
		if(nSize == -1)
		{
			App::Exit();
		}
	}
}

void MainForm::Form_Closed(Object* sender, EventArgs* e)
{
	//save config
	this->Config(false, *CC_APP_GetIni(), cc::Str(_T("Main")));
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
		//cc::Util::ndesCrype((char*)buf, nLen, pwd, klen, true);
		AES aes(AES::BIT128, (unsigned char *)pwd, klen);
		aes.Cipher(buf, nLen);
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
		//cc::Util::ndesCrype((char*)buf, nLen, pwd, klen, false);
		AES aes(AES::BIT128, (unsigned char *)pwd, klen);
		aes.InvCipher(buf, nLen);
		cc::Util::teaDeCrypt((char*)buf, nLen, pwd, klen, 5);
	}
	cc::Util::fWrite(buf, nLen, LangCode::SYSTEM, sFile);
	return true;
}

bool MainForm::TreateOne(cc::Str& sFile, const char* pwd, int klen, bool isDelOrg)
{
	if(sFile.GetLength() < 1)
	{
		return true;
	}

	if(sFile.EndsWith(_T(".crypt"), true))
	{
		//decrypt
		cc::Str sOrg = sFile.Substring(0, sFile.GetLength() - 6);
		//if(!TipsDlg::GetIsNotShowAgain(CC_TIPSDLG_OVERORIG))
		if(!chkConfirm->GetChecked())
		{
			if(cc::Util::fExist(sOrg))
			{
				TipsDlg tipsDlg(CC_TIPSDLG_OVERORIG, _T("Orignal file is exist, DeCrypt and overwrite it?"), NULL, 0, _T("Question Message"),
					380, 32, 32, NULL, NULL, MB_OKCANCEL|MB_ICONQUESTION);
				tipsDlg.SetParent(this, true);
				tipsDlg.SetCenter();
				DialogResult::Type type = tipsDlg.ShowDialog();
				if(TipsDlg::GetIsNotShowAgain(CC_TIPSDLG_OVERORIG))
				{
					chkConfirm->SetChecked(true);
				}

				if(type != DialogResult::OK)
				{
					return false;
				}
			}
		}
		DeCryptOne(sOrg, pwd, klen);
	}
	else
	{
		//crypt
		//if(!TipsDlg::GetIsNotShowAgain(CC_TIPSDLG_REMOVEORIG))
		if(!chkConfirm->GetChecked())
		{
			if(isDelOrg)
			{
				TipsDlg tipsDlg(CC_TIPSDLG_REMOVEORIG, _T("Crypt it and delete orignal file?"), NULL, 0, _T("Question Message"),
					380, 32, 32, NULL, NULL, MB_OKCANCEL|MB_ICONQUESTION);
				tipsDlg.SetParent(this, true);
				tipsDlg.SetCenter();
				DialogResult::Type type = tipsDlg.ShowDialog();
				if(TipsDlg::GetIsNotShowAgain(CC_TIPSDLG_REMOVEORIG))
				{
					chkConfirm->SetChecked(true);
				}

				if(type != DialogResult::OK)
				{
					return false;
				}
			}
		}
		EnCryptOne(sFile, pwd, klen, isDelOrg);
	}
	return true;
}

void MainForm::Control_Click(Object* sender, EventArgs* e)
{
	cc::Str sKey;
	cc::Str sVal;
	if(sender == btnCrypt)
	{
		if(!IsConfigOK(true))
		{
			return;
		}
		cc::Str sPass = cc::Str(LangCode::SYSTEM, editPass->GetTextA()).Trim();
		cc::List<cc::Str> arrFiles;
		sVal = cc::Util::selectOpenFile(_T(""), _T("All Files (*.*)\0*.*\0\0"), _T(".*"), m_hWnd, _T("Open file for Crypt or DeCrypt"), 0, true, &arrFiles);
		TreateOne(sVal, (char*)sPass, sPass.GetLength(), chkDelOrg->GetChecked());
	}
	else if(sender == chkShowPW)
	{
		cc::Str sPass = editPass->GetText();
		if(chkShowPW->GetChecked())
		{
			editPass->SetStyle(StyleType::Style, ES_PASSWORD, 0);
			editPass->SetPasswordChar(0);
		}
		else
		{
			editPass->SetStyle(StyleType::Style, 0, ES_PASSWORD);
			editPass->SetPasswordChar('*');
		}
		editPass->Focus(false);
		editPass->SetText(sPass);
	}
	//else if(sender == btnReset)
	//{
	//	TipsDlg::ShowAllAgain();
	//	::MessageBox(m_hWnd, _T("Show confirm dialog again."), _T("Message"), MB_OK);
	//}
}

void MainForm::menuExit_Click(Object* sender, EventArgs* e)
{
	App::Exit();
}
