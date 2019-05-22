#include "LedTime.h"
using namespace cc;
using namespace cc::win;

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
	cc::win::App::SetNameVersion(NAMEVER, _T("1.10"));

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

void MainForm::InitializeComponent()
{
	imageList = new ImageList();
	imageList->SetIconSize(-1, -1);
	imageList->SetIcon(IDI_ICON_MAIN, IDI_ICON_MAIN);
	this->DisposeItemAdd(imageList);

	contextMenu = new ContextMenu();
	this->DisposeItemAdd(contextMenu);
	menuTopmost = new MenuItem();
	menuTopmost->SetText(_T("Always Top"));
	menuTopmost->Click += EventHandler((Object*)this, (EventFun)&MainForm::Control_Click);
	contextMenu->ItemAdd(menuTopmost);
	
	menuNoCaption = new MenuItem();
	menuNoCaption->SetText(_T("No Window Title"));
	menuNoCaption->Click += EventHandler((Object*)this, (EventFun)&MainForm::Control_Click);
	contextMenu->ItemAdd(menuNoCaption);

	menuNolink = new MenuItem();
	menuNolink->SetText(_T("Not Show Link"));
	menuNolink->Click += EventHandler((Object*)this, (EventFun)&MainForm::Control_Click);
	contextMenu->ItemAdd(menuNolink);

	menuHhmm = new MenuItem();
	menuHhmm->SetText(_T("Show HH:MM only"));
	menuHhmm->Click += EventHandler((Object*)this, (EventFun)&MainForm::Control_Click);
	contextMenu->ItemAdd(menuHhmm);

	menuAbout = new MenuItem();
	menuAbout->SetText(_T("About"));
	menuAbout->Click += EventHandler((Object*)this, (EventFun)&MainForm::Control_Click);
	contextMenu->ItemAdd(menuAbout);

	menuExit = new MenuItem();
	menuExit->SetText(_T("Exit"));
	menuExit->Click += EventHandler((Object*)this, (EventFun)&MainForm::Control_Click);
	contextMenu->ItemAdd(menuExit);

	int nTop = 8;
	led = new LEDControl();
	led->SetName(_T("led"));
	led->SetText(_T(""));
	led->SetTextColor(RGB(0, 0, 255));
	//led->SetBackground(RGB(230, 245, 235));
	led->SetTextBackground(RGB(0xe8, 0xe8, 0xe8));
	led->SetLocation(5, 7);
	led->SetSize(600 - 15, 600 - 30 + 7);
	led->SetOuterStyle(OuterStyle::TransparentParentErase, true);
	led->Anchor = (AnchorStyles::Type)(AnchorStyles::Top | AnchorStyles::Bottom | AnchorStyles::Left | AnchorStyles::Right);
	led->MouseDown += MouseEventHandler((Object*)this, (MouseEventFun)&MainForm::OnMouseDown);
	this->AddControl(led);

	link = new LabelLink();
	link->SetName(_T("link"));
	link->SetText(WWWLINK);
	link->SetLocation(5, 600 - 25 + 9);
	link->SetSize(100, 17);
	link->SetOuterStyle(OuterStyle::AutoSize, true);
	//link->SetURL((const TCHAR*)CC_APP_GetLang()->Get(_T("Lang.U_LinkURL")));
	link->Anchor = (AnchorStyles::Type)(AnchorStyles::Bottom | AnchorStyles::Left);
	this->AddControl(link);

	this->SetContextMenu(contextMenu);
	this->SetOuterStyle(OuterStyle::AutoConfig, true);
	this->SetName(_T("LedTime"));
	this->SetText(App::GetNameVersion());
	this->SetConfigName(_T(""));
	this->SetCenter();
	this->SetClientSize(600, 600);
	this->SetSize(390, 100);
	//here need be 32X32
	this->SetIcon(imageList, IDI_ICON_MAIN);
	//this->SetMinSize(110, 70);
	//this->SetMaxSize(900, 600);
	if(CC_APP_GetIni()->GetBool(_T("Main.NoCaption"), true))
	{
		this->SetStyle(StyleType::Style, WS_CAPTION, 0);
	}
	this->SetMoveFormOnClick(true);
	this->SetStyle(StyleType::ExStyle, WS_EX_APPWINDOW, WS_EX_TOOLWINDOW);
	if(!CC_APP_GetIni()->GetBool(_T("Main.Topmost"), true))
	{
		this->SetStyle(StyleType::ExStyle, 0, WS_EX_APPWINDOW);
	}
	this->Load += EventHandler((Object*)this, (EventFun)&MainForm::Form_Load);
	this->Closed += EventHandler((Object*)this, (EventFun)&MainForm::Form_Closed);

	SetLang(*CC_APP_GetLang(), cc::Str(CC_CONFIG_LANGSECTION));
}

void MainForm::Form_Load(Object* sender, EventArgs* e)
{
	//load config
	this->Config(true, *CC_APP_GetIni(), cc::Str(_T("Main")));
	if(CC_APP_GetIni()->GetBool(_T("Main.Topmost"), true))
	{
		Control_Click(menuTopmost, NULL);
	}
	if(CC_APP_GetIni()->GetBool(_T("Main.NoCaption"), true))
	{
		menuNoCaption->SetChecked(true);
	}
	if(CC_APP_GetIni()->GetBool(_T("Main.Nolink"), false))
	{
		Control_Click(menuNolink, NULL);
	}
	if(CC_APP_GetIni()->GetBool(_T("Main.Hhmm"), false))
	{
		Control_Click(menuHhmm, NULL);
	}

	//show it until Form_Load
	this->SetVisible(true);
}

void MainForm::Form_Closed(Object* sender, EventArgs* e)
{
	//save config
	CC_APP_GetIni()->SetBool(_T("Main.Topmost"), menuTopmost->GetChecked());
	CC_APP_GetIni()->SetBool(_T("Main.NoCaption"), menuNoCaption->GetChecked());
	CC_APP_GetIni()->SetBool(_T("Main.Nolink"), menuNolink->GetChecked());
	CC_APP_GetIni()->SetBool(_T("Main.Hhmm"), menuHhmm->GetChecked());
	this->Config(false, *CC_APP_GetIni(), cc::Str(_T("Main")));
}

void MainForm::OnMouseDown(Object* sender, MouseEventArgs* e)
{
	if(e->Button == MouseButtons::Left)
	{
		::SetCursor(::LoadCursor(NULL, IDC_SIZEALL));
		::ReleaseCapture();
		::SendMessage(m_hWnd, WM_NCLBUTTONDOWN, HTCAPTION, NULL);
	}
}

void MainForm::Control_Click(Object* sender, EventArgs* e)
{
	if(sender == menuTopmost)
	{
		bool chk = !menuTopmost->GetChecked();
		menuTopmost->SetChecked(chk);
		this->SetVisible(false);
		if(chk)
		{
			::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
			this->SetStyle(StyleType::ExStyle, WS_EX_APPWINDOW, 0);
		}
		else
		{
			::SetWindowPos(m_hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
			this->SetStyle(StyleType::ExStyle, 0, WS_EX_APPWINDOW);
		}
		this->SetVisible(true);
	}
	else if(sender == menuNoCaption)
	{
		bool chk = !menuNoCaption->GetChecked();
		menuNoCaption->SetChecked(chk);
		if(!chk) {
			this->SetStyle(StyleType::Style, 0, WS_CAPTION);
		}
		else {
			this->SetStyle(StyleType::Style, WS_CAPTION, 0);
		}
		SetWindowPos(m_hWnd, NULL, 0,0,0,0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER);
	}
	else if(sender == menuNolink)
	{
		bool chk = !menuNolink->GetChecked();
		menuNolink->SetChecked(chk);
		link->SetVisible(!chk);
		RECT rect = led->GetBounds();
		if(chk)
		{
			rect.bottom += 12;
		}
		else
		{
			rect.bottom -= 12;
		}
		led->SetBoundsCalcuteOriginal(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);
	}
	else if(sender == menuHhmm)
	{
		bool chk = !menuHhmm->GetChecked();
		menuHhmm->SetChecked(chk);
		led->SetCharWidth(-1);
		led->SetHhmm(chk);
	}
	else if(sender == menuAbout)
	{
		CC_APP_GetLang()->Set(_T("Lang.U_Link"), WWWLINK);
		CC_APP_GetLang()->Set(_T("Lang.U_LinkURL"), WWWLINK);
		cc::Str sAbout = NAMEVER;
		AboutDlg* about = new AboutDlg(sAbout, imageList, IDI_ICON_MAIN, _T("CopyRight"), 
			280, 36, 36, NULL, true, false);
		about->SetParent(this, true);
		about->SetCenter();
		about->ShowDialog();

		about->Dispose();
		delete about;
	}
	else if(sender == menuExit)
	{
		App::Exit();
	}
}
