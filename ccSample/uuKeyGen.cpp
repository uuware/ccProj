#include "uuKeyGen.h"
#include "..\ccExt\AES.h"
#include "..\ccExt\SHA1.h"
using namespace cc;
using namespace cc::win;

#define REG_KEYS "[#KEYSTART#]"
#define REG_KEYE "[#KEYEND#]"

///////////////////////////////////////////////////////////////////////////////
//WinMain
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//when add manifest.xml for xp theme, also need set controls to have default teansparent
	//so while some controls need backcolor, need remove  (OuterStyle::TransparentParentBrush|OuterStyle::TransparentParentErase)
	cc::win::App::SetDefaultTransparent(true);
	cc::win::App::SetInstance(hInstance);

	cc::win::FormConfig::nInnerStyleClient = InnerStyle::BackgroundTheme|FormClientConfig::nInnerStyle;
	cc::win::TabControlExConfig::nOuterStyle = OuterStyle::TransparentParentErase;
	cc::win::TabPageConfig::corBackground = ::GetSysColor(COLOR_MENU);
	cc::win::TabPageConfig::corHotBackColor = ::GetSysColor(COLOR_MENU);
	cc::win::TabPageConfig::corTextground = RGB(255, 0, 0);
	cc::win::TextBoxConfig::nOuterStyle |= OuterStyle::TransparentParentBrush;

	//should set this before cc::Config::GetLang();
	cc::win::App::GetConfig()->LoadLang(CC_CONFIG_LANGENG);
	//the name is used for CreateMutex"s flages,cannot change to other lang
	cc::win::App::SetNameVersion(_T("uuKey Maker[uuware.com]"), _T("Ver 1.00 build 20161002"));

	MainForm* form = new MainForm();
	cc::win::App::Run(form);
	form->Dispose();
	delete form;
	return 0;
}

cc::Str GenPassWord(cc::Str& sChars, int nMinLen, int nMaxLen)
{
	//// initialize random numbers generator
	//srand((unsigned int)GetTickCount());
	cc::Util::GetRandom(0, 10000, true);
	cc::Str strPass(sChars.GetLangCode());
	int nPosLast = -1;;
	for(int i = 0; i < nMaxLen; i++)
	{
		int nPos = cc::Util::GetRandom(0, sChars.GetLength() - 1);
		if(nPos == nPosLast)
		{
			nPos = cc::Util::GetRandom(0, sChars.GetLength() - 1);
		}
		nPosLast = nPos;
		strPass.Append(sChars.Substring(nPos, nPos + 1));
		if(i >= nMinLen && nPos%2 == 0)
		{
			nPos = cc::Util::GetRandom(0, sChars.GetLength() - 1);
			if(nPos%2 == 0)
			{
				break;
			}
		}
	}
	return strPass;
}

///////////////////////////////////////////////////////////////////////////////
//MainForm
MainForm::MainForm()
{
	//memset(buf, 0, BUF_LEN * sizeof(TCHAR));
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
	imageList = new ImageList();
	imageList->SetIconSize(-1, -1);
	imageList->SetIcon(IDI_ICON_MAIN, IDI_ICON_MAIN);
	this->DisposeItemAdd(imageList);

	int nLeft = 5;
	toolOnTop = new CheckBox();
	toolOnTop->SetName(_T("toolOnTop"));
	toolOnTop->SetText(_T("Always OnTop"));
	toolOnTop->SetLocation(nLeft, nTop);
	toolOnTop->SetSize(200, 17);
	toolOnTop->Click += EventHandler((Object*)this, (EventFun)&MainForm::toolOnTop_Click);
	//toolOnTop->SetOuterStyle(OuterStyle::AutoConfig, true);
	this->AddControl(toolOnTop);

	nTop += 21;
	nLeft = 5;
	btnGenPass = new Button();
	btnGenPass->SetName(_T("btnGenPass"));
	btnGenPass->SetText(_T("GenPassWord"));
	btnGenPass->SetLocation(nLeft, nTop);
	btnGenPass->SetSize(100, 30);
	btnGenPass->Click += EventHandler((Object*)this, (EventFun)&MainForm::Control_Click);
	this->AddControl(btnGenPass);

	nTop += 10;
	nLeft += 105;
	label = new Label();
	label->SetName(_T("labPMin"));
	label->SetText(_T("MinLen:"));
	label->SetLocation(nLeft, nTop + 2);
	label->SetSize(50, 17);
	this->AddControl(label);
	nLeft += 50;
	editPMin = new TextBox();
	editPMin->SetName(_T("editPMin"));
	editPMin->SetText(_T("10"));
	editPMin->SetStyleNumber(true);
	editPMin->SetLocation(nLeft, nTop);
	editPMin->SetSize(25, 17);
	editPMin->SetOuterStyle(OuterStyle::AutoConfig, true);
	this->AddControl(editPMin);

	spanPMin = new SpinButton();
	spanPMin->SetName(_T("spin1"));
	spanPMin->SetText(_T("spin1"));
	spanPMin->SetLocationOffset(editPMin, false, 1, 0);
	spanPMin->SetBuddy(editPMin);
	spanPMin->SetRange(1, 100);
	spanPMin->SetBase(1);
	spanPMin->SetPos(30);
	this->AddControl(spanPMin);

	nLeft += 50;
	label = new Label();
	label->SetName(_T("labPMax"));
	label->SetText(_T("MaxLen:"));
	label->SetLocation(nLeft, nTop + 2);
	label->SetSize(50, 17);
	this->AddControl(label);
	nLeft += 50;
	editPMax = new TextBox();
	editPMax->SetName(_T("editPMax"));
	editPMax->SetText(_T("20"));
	editPMax->SetStyleNumber(true);
	editPMax->SetLocation(nLeft, nTop);
	editPMax->SetSize(25, 17);
	editPMax->SetOuterStyle(OuterStyle::AutoConfig, true);
	this->AddControl(editPMax);

	spanPMax = new SpinButton();
	spanPMax->SetName(_T("spin1"));
	spanPMax->SetText(_T("spin1"));
	spanPMax->SetLocationOffset(editPMax, false, 1, 0);
	spanPMax->SetBuddy(editPMax);
	spanPMax->SetRange(2, 100);
	spanPMax->SetBase(1);
	spanPMax->SetPos(30);
	this->AddControl(spanPMax);

	nTop += 21;
	nLeft = 5;
	chkPUpper = new CheckBox();
	chkPUpper->SetName(_T("chkPUpper"));
	chkPUpper->SetText(_T("UpperLeters"));
	chkPUpper->SetChecked(true);
	chkPUpper->SetLocation(nLeft, nTop);
	chkPUpper->SetSize(90, 17);
	chkPUpper->SetOuterStyle(OuterStyle::AutoConfig, true);
	this->AddControl(chkPUpper);

	nLeft += 90;
	chkPLower = new CheckBox();
	chkPLower->SetName(_T("chkPLower"));
	chkPLower->SetText(_T("LowerLeters"));
	chkPLower->SetChecked(true);
	chkPLower->SetLocation(nLeft, nTop);
	chkPLower->SetSize(90, 17);
	chkPLower->SetOuterStyle(OuterStyle::AutoConfig, true);
	this->AddControl(chkPLower);

	nLeft += 90;
	chkPNumber = new CheckBox();
	chkPNumber->SetName(_T("chkPNumber"));
	chkPNumber->SetText(_T("Numbers"));
	chkPNumber->SetChecked(true);
	chkPNumber->SetLocation(nLeft, nTop);
	chkPNumber->SetSize(80, 17);
	chkPNumber->SetOuterStyle(OuterStyle::AutoConfig, true);
	this->AddControl(chkPNumber);

	nTop += 21;
	nLeft = 5;
	chkPSymbol1 = new CheckBox();
	chkPSymbol1->SetName(_T("chkPSymbol"));
	chkPSymbol1->SetText(_T("Symbols1:"));
	chkPSymbol1->SetChecked(true);
	chkPSymbol1->SetLocation(nLeft, nTop);
	chkPSymbol1->SetSize(75, 17);
	chkPSymbol1->SetOuterStyle(OuterStyle::AutoConfig, true);
	this->AddControl(chkPSymbol1);

	nLeft += 75;
	editPSymbol1 = new TextBox();
	editPSymbol1->SetName(_T("editPSymbol1"));
	editPSymbol1->SetText(_T("!#$%&()=@{}[]*:+;_?/.<>,"));
	editPSymbol1->SetLocation(nLeft, nTop);
	editPSymbol1->SetSize(100, 17);
	editPSymbol1->SetOuterStyle(OuterStyle::AutoConfig, true);
	this->AddControl(editPSymbol1);

	nLeft += 105;
	chkPSymbol2 = new CheckBox();
	chkPSymbol2->SetName(_T("chkPSymbol2"));
	chkPSymbol2->SetText(_T("Symbols2:"));
	chkPSymbol2->SetLocation(nLeft, nTop);
	chkPSymbol2->SetSize(75, 17);
	chkPSymbol2->SetOuterStyle(OuterStyle::AutoConfig, true);
	this->AddControl(chkPSymbol2);

	nLeft += 75;
	editPSymbol2 = new TextBox();
	editPSymbol2->SetName(_T("editPSymbol2"));
	editPSymbol2->SetText(_T("\"\\`'~|"));
	editPSymbol2->SetLocation(nLeft, nTop);
	editPSymbol2->SetSize(100, 17);
	editPSymbol2->SetOuterStyle(OuterStyle::AutoConfig, true);
	this->AddControl(editPSymbol2);

	nTop += 20;
	label = new Label();
	label->SetName(_T("labPass"));
	label->SetText(_T("PassWord:"));
	label->SetLocation(5, nTop + 2);
	label->SetSize(60, 17);
	this->AddControl(label);
	editPass = new TextBox();
	editPass->SetName(_T("editPass"));
	editPass->SetText(_T(""));
	editPass->SetLocation(5 + 60, nTop);
	editPass->SetSize(400, 17);
	this->AddControl(editPass);

	this->SetOuterStyle(OuterStyle::AutoConfig, true);
	this->SetIcon(imageList, IDI_ICON_MAIN);
	this->SetName(_T("uuKeyGen"));
	this->SetText(App::GetNameVersion());
	this->SetConfigName(_T(""));
	this->SetLocation(0, 0);
	this->SetClientSize(600, 600);
	this->SetSize(480, 165);
	this->SetMaxButtonEnable(false);
	this->SetMoveFormOnClick(true);
	this->SetCenter();
	//this->SetMinSize(660, 520);
	this->Load += EventHandler((Object*)this, (EventFun)&MainForm::Form_Load);
	this->Closed += EventHandler((Object*)this, (EventFun)&MainForm::Form_Closed);

	SetLang(*CC_APP_GetLang(), cc::Str(CC_CONFIG_LANGSECTION));
}

void MainForm::Form_Load(Object* sender, EventArgs* e)
{
	//load config
	this->Config(true, *CC_APP_GetIni(), cc::Str(_T("Main")));
	toolOnTop_Click(this, NULL);
	//show it until Form_Load
	this->SetVisible(true);
}

void MainForm::Form_Closed(Object* sender, EventArgs* e)
{
	//save config
	this->Config(false, *CC_APP_GetIni(), cc::Str(_T("Main")));
}

void MainForm::Control_Click(Object* sender, EventArgs* e)
{
	cc::Str sKey;
	cc::Str sVal;

	//btnGenPass
	if(sender == btnGenPass)
	{
		int nMin = editPMin->GetTextInt();
		int nMax = editPMax->GetTextInt();
		if(nMin < 1 || nMax < 1 || nMin > nMax)
		{
			::MessageBox(m_hWnd, _T("Not right for min&max length."), _T("Question"), MB_OK);
			return;
		}
		if(editPSymbol1->GetTextLen() < 1)
		{
			editPSymbol1->SetText(_T("!#$%&()=@{}[]*:+;_?/.<>,"));
		}
		if(editPSymbol2->GetTextLen() < 1)
		{
			editPSymbol2->SetText(_T("\"\\`'~|"));
		}
		sVal.Clear();
		if(chkPUpper->GetChecked())
		{
			sVal.Append(_T("ABCDEFGHIJKLMNOPQRSTUVWXYZ"));
		}
		if(chkPLower->GetChecked())
		{
			sVal.Append(_T("abcdefghijklmnopqrstuvwxyz"));
		}
		if(chkPNumber->GetChecked())
		{
			sVal.Append(_T("1234567890"));
		}
		if(chkPSymbol1->GetChecked())
		{
			sVal.Append(editPSymbol1->GetText());
		}
		if(chkPSymbol2->GetChecked())
		{
			sVal.Append(editPSymbol2->GetText());
		}
		if(sVal.GetLength() < 1)
		{
			::MessageBox(m_hWnd, _T("Need check on at least one option."), _T("Question"), MB_OK);
			return;
		}

		cc::Str strPass = GenPassWord(sVal, nMin, nMax);
		editPass->SetText(strPass);
	}
}

void MainForm::toolOnTop_Click(Object* sender, EventArgs* e)
{
	bool chk = toolOnTop->GetChecked();
	::SetWindowPos(m_hWnd, (chk) ? HWND_TOPMOST : HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}

void MainForm::menuExit_Click(Object* sender, EventArgs* e)
{
	App::Exit();
}
