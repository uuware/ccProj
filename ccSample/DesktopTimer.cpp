#include "DesktopTimer.h"
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
	cc::win::App::SetNameVersion(NAMEVER, _T("1.00"));

	MainForm* form = new MainForm();
	cc::win::App::Run(form);
	form->Dispose();
	delete form;
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
//MainForm
SetDialog::SetDialog()
{
	int nTop = 10;
	Label* label;

	label = new Label();
	label->SetName(_T("labTime"));
	label->SetText(_T("Time for timer:"));
	label->SetLocation(3, nTop);
	label->SetSize(200, 17);
	this->AddControl(label);

	nTop += 20;
	radio1 = new RadioButton();
	radio1->SetName(_T("radio1"));
	radio1->SetText(_T("Countdown time"));
	radio1->SetLocation(20, nTop);
	radio1->SetSize(110, 17);
	radio1->SetStyle(StyleType::Outer, 0, OuterStyle::AutoConfig);
	radio1->SetStyle(StyleType::Outer, 0, OuterStyle::AutoSize);
	radio1->Click += EventHandler((Object*)this, (EventFun)&SetDialog::Control_Click);
	radio1->SetChecked(true);
	this->AddControl(radio1);

	editM0 = new TextBox();
	editM0->SetName(_T("editM0"));
	editM0->SetText(_T("0"));
	editM0->SetLocationOffset(radio1, false, 0, 0);
	editM0->SetSize(30, 17);
	this->AddControl(editM0);
	spanM0 = new SpinButton();
	spanM0->SetName(_T("spanM0"));
	spanM0->SetLocationOffset(editM0, false, 0, 0);
	spanM0->SetBuddy(editM0);
	spanM0->SetRange(0, 999);
	spanM0->SetBase(1);
	spanM0->SetPos(0);
	spanM0->SetOuterStyle(OuterStyle::AutoConfig, true);
	this->AddControl(spanM0);
	label = new Label();
	label->SetName(_T("labelM"));
	label->SetText(_T("m"));
	label->SetLocationOffset(spanM0, false, 0, 0);
	label->SetSize(10, 17);
	this->AddControl(label);

	editS0 = new TextBox();
	editS0->SetName(_T("editS0"));
	editS0->SetText(_T("0"));
	editS0->SetLocationOffset(label, false, 3, 0);
	editS0->SetSize(30, 17);
	this->AddControl(editS0);
	spanS0 = new SpinButton();
	spanS0->SetName(_T("spanS0"));
	spanS0->SetLocationOffset(editS0, false, 0, 0);
	spanS0->SetBuddy(editS0);
	spanS0->SetRange(0, 60);
	spanS0->SetBase(1);
	spanS0->SetPos(0);
	spanS0->SetOuterStyle(OuterStyle::AutoConfig, true);
	this->AddControl(spanS0);
	label = new Label();
	label->SetName(_T("labelS"));
	label->SetText(_T("s"));
	label->SetLocationOffset(spanS0, false, 0, 0);
	label->SetSize(10, 17);
	this->AddControl(label);

	nTop += 20;
	radio2 = new RadioButton();
	radio2->SetName(_T("radio2"));
	radio2->SetText(_T("at time"));
	radio2->SetLocation(20, nTop);
	radio2->SetSize(110, 17);
	radio2->SetStyle(StyleType::Outer, 0, OuterStyle::AutoConfig);
	radio2->SetStyle(StyleType::Outer, 0, OuterStyle::AutoSize);
	radio2->Click += EventHandler((Object*)this, (EventFun)&SetDialog::Control_Click);
	this->AddControl(radio2);

	editH = new TextBox();
	editH->SetName(_T("editH"));
	editH->SetText(_T("0"));
	editH->SetLocationOffset(radio2, false, 0, 0);
	editH->SetSize(30, 17);
	this->AddControl(editH);
	spanH = new SpinButton();
	spanH->SetName(_T("spanH"));
	spanH->SetLocationOffset(editH, false, 0, 0);
	spanH->SetBuddy(editH);
	spanH->SetRange(0, 23);
	spanH->SetBase(1);
	spanH->SetPos(0);
	spanH->SetOuterStyle(OuterStyle::AutoConfig, true);
	this->AddControl(spanH);
	label = new Label();
	label->SetName(_T("labelM"));
	label->SetText(_T(":"));
	label->SetLocationOffset(spanH, false, 5, 0);
	label->SetSize(5, 17);
	this->AddControl(label);

	editM = new TextBox();
	editM->SetName(_T("editM"));
	editM->SetText(_T("0"));
	editM->SetLocationOffset(label, false, 3, 0);
	editM->SetSize(30, 17);
	this->AddControl(editM);
	spanM = new SpinButton();
	spanM->SetName(_T("spanM"));
	spanM->SetLocationOffset(editM, false, 0, 0);
	spanM->SetBuddy(editM);
	spanM->SetRange(0, 59);
	spanM->SetBase(1);
	spanM->SetPos(0);
	spanM->SetOuterStyle(OuterStyle::AutoConfig, true);
	this->AddControl(spanM);
	label = new Label();
	label->SetName(_T("labelM"));
	label->SetText(_T(":"));
	label->SetLocationOffset(spanM, false, 5, 0);
	label->SetSize(5, 17);
	this->AddControl(label);

	editS = new TextBox();
	editS->SetName(_T("editS"));
	editS->SetText(_T("0"));
	editS->SetLocationOffset(label, false, 3, 0);
	editS->SetSize(30, 17);
	this->AddControl(editS);
	spanS = new SpinButton();
	spanS->SetName(_T("spanS"));
	spanS->SetLocationOffset(editS, false, 0, 0);
	spanS->SetBuddy(editS);
	spanS->SetRange(0, 100);
	spanS->SetBase(1);
	spanS->SetPos(0);
	spanS->SetOuterStyle(OuterStyle::AutoConfig, true);
	this->AddControl(spanS);

	nTop += 20;
	chkBeep = new CheckBox();
	chkBeep->SetName(_T("chkBeep"));
	chkBeep->SetText(_T("Beep"));
	chkBeep->SetLocation(5, nTop);
	chkBeep->SetSize(80, 19);
	chkBeep->SetOuterStyle(OuterStyle::AutoSize, true);
	chkBeep->SetOuterStyle(OuterStyle::AutoConfig, true);
	chkBeep->SetStyle(StyleType::Outer, 0, OuterStyle::AutoConfig);
	chkBeep->Click += EventHandler((Object*)this, (EventFun)&SetDialog::Control_Click);
	this->AddControl(chkBeep);
	
	btnPlayBeep = new Button();
	btnPlayBeep->SetName(_T("btnPlayBeep"));
	btnPlayBeep->SetText(_T("Play"));
	btnPlayBeep->SetLocationOffset(chkBeep, false, 2, 0);
	btnPlayBeep->SetSize(40, 19);
	btnPlayBeep->Click += EventHandler((Object*)this, (EventFun)&SetDialog::Control_Click);
	this->AddControl(btnPlayBeep);

	nTop += 20;
	chkSound = new CheckBox();
	chkSound->SetName(_T("chkSound"));
	chkSound->SetText(_T("Play Sound"));
	chkSound->SetLocation(5, nTop);
	chkSound->SetSize(80, 19);
	chkSound->SetOuterStyle(OuterStyle::AutoSize, true);
	chkSound->SetOuterStyle(OuterStyle::AutoConfig, true);
	chkSound->SetStyle(StyleType::Outer, 0, OuterStyle::AutoConfig);
	chkSound->Click += EventHandler((Object*)this, (EventFun)&SetDialog::Control_Click);
	this->AddControl(chkSound);

	editSound = new TextBox();
	editSound->SetName(_T("editSound"));
	editSound->SetText(_T(""));
	editSound->SetLocationOffset(chkSound, false, 5, 0);
	editSound->SetSize(180-15, 17);
	editSound->SetOuterStyle(OuterStyle::AutoConfig, true);
	this->AddControl(editSound);
	btnSelFile = new PathButton();
	btnSelFile->SetLocationOffset(editSound, false, 0, 1);
	btnSelFile->SetSize(15, 17);
	btnSelFile->IsFile = true;
	btnSelFile->PathControl = editSound;
	btnSelFile->tcCaption = _T("select sound file:");
	btnSelFile->tcFilter = _T("Sound Files (*.wav)\0*.wav\0\0");
	btnSelFile->tcDefaultExt = _T("wav");
	this->AddControl(btnSelFile);
	
	btnPlaySound = new Button();
	btnPlaySound->SetName(_T("btnPlaySound"));
	btnPlaySound->SetText(_T("Play"));
	btnPlaySound->SetLocationOffset(btnSelFile, false, 2, 0);
	btnPlaySound->SetSize(40, 19);
	btnPlaySound->Click += EventHandler((Object*)this, (EventFun)&SetDialog::Control_Click);
	this->AddControl(btnPlaySound);
	
	nTop += 20;
	chkText = new CheckBox();
	chkText->SetName(_T("chkText"));
	chkText->SetText(_T("Show Text"));
	chkText->SetLocation(5, nTop);
	chkText->SetSize(80, 19);
	chkText->SetOuterStyle(OuterStyle::AutoSize, true);
	chkText->SetOuterStyle(OuterStyle::AutoConfig, true);
	chkText->Click += EventHandler((Object*)this, (EventFun)&SetDialog::Control_Click);
	this->AddControl(chkText);

	editText = new TextBox();
	editText->SetName(_T("editText"));
	editText->SetText(_T("Show This Text..."));
	editText->SetLocationOffset(chkText, false, 5, 0);
	editText->SetSize(180, 17);
	editText->SetOuterStyle(OuterStyle::AutoConfig, true);
	this->AddControl(editText);
	
	nTop += 25;
	btnOK = new Button();
	btnOK->SetName(_T("btnOK"));
	btnOK->SetText(_T("OK"));
	btnOK->SetLocation(3, nTop);
	btnOK->SetSize(70, 19);
	btnOK->Click += EventHandler((Object*)this, (EventFun)&SetDialog::Control_Click);
	this->AddControl(btnOK);
	this->SetOKButton(btnOK);

	btnCancel = new Button();
	btnCancel->SetName(_T("btnCancel"));
	btnCancel->SetText(_T("Cancel"));
	btnCancel->SetLocation(75, nTop);
	btnCancel->SetSize(70, 19);
	btnCancel->Click += EventHandler((Object*)this, (EventFun)&SetDialog::Control_Click);
	this->AddControl(btnCancel);
	this->SetCancelButton(btnCancel);

	this->SetName(_T("SetDlg"));
	this->SetText(_T("Setting Timer"));
	//this->SetStyle(StyleType::Style, 0, WS_THICKFRAME);
	//this->SetSize(300, nTop + 19 + 10);
	this->SetSizeBaseOnClient(320, nTop + 17 + 5);
	SetLang(*CC_APP_GetLang(), cc::Str(_T("SetDialog")));

	this->Load += EventHandler((Object*)this, (EventFun)&SetDialog::Form_Load);
	this->Closed += EventHandler((Object*)this, (EventFun)&SetDialog::Form_Closed);
}

void SetDialog::Form_Load(Object* sender, EventArgs* e)
{
	//load config
	this->Config(true, *CC_APP_GetIni(), cc::Str(_T("")));
}

void SetDialog::Form_Closed(Object* sender, EventArgs* e)
{
}

unsigned long _stdcall BeepThread(void *entity)
{
	::Beep(850, 200);
	::Beep(750, 800);
	::Beep(850, 200);
	::Beep(750, 800);
	::Beep(850, 200);
	::Beep(750, 800);
	return 0;
}

void SetDialog::Control_Click(Object* sender, EventArgs* e)
{
	if(sender == btnOK)
	{
		//save config
		this->Config(false, *CC_APP_GetIni(), cc::Str(_T("")));
		EndDialog(DialogResult::OK);
	}
	else if(sender == btnCancel)
	{
		EndDialog(DialogResult::Cancel);
	}
	else if(sender == btnPlaySound)
	{
		cc::Str str = editSound->GetText();
		if(cc::Util::fExist(str) && str.EndsWith(_T(".wav"), true)) {
			cc::Util::PlaySoundW(editSound->GetTextW(), NULL, SND_FILENAME|SND_ASYNC|SND_NOWAIT|SND_NODEFAULT);
		}
		else {
			::MessageBox(m_hWnd, _T("File not exist or not is wav file."), _T("Critical"), MB_OK | MB_ICONSTOP);
		}
	}
	else if(sender == btnPlayBeep)
	{
		CreateThread(NULL, 0, BeepThread, NULL, 0, NULL);
	}
}

///////////////////////////////////////////////////////////////////////////////
//MainForm
MainForm::MainForm()
{
	imageList = new ImageList();
	imageList->SetIconSize(-1, -1);
	imageList->SetIcon(IDI_ICON_MAIN, IDI_ICON_MAIN);
	this->DisposeItemAdd(imageList);
	nResetSec = 0;

	contextMenu = new ContextMenu();
	this->DisposeItemAdd(contextMenu);

	menuOption = new MenuItem();
	menuOption->SetText(_T("Timer Option"));
	menuOption->Click += EventHandler((Object*)this, (EventFun)&MainForm::Control_Click);
	contextMenu->ItemAdd(menuOption);

	menuQuick = new MenuItem();
	menuQuick->SetName(_T("menuQuick"));
	menuQuick->SetText(_T("Quick Timer"));
	contextMenu->ItemAdd(menuQuick);

		menu1M = new MenuItem();
		menu1M->SetName(_T("menu1M"));
		menu1M->SetText(_T("1 minute"));
		menu1M->Click += EventHandler((Object*)this, (EventFun)&MainForm::Control_Click);
		menuQuick->ItemAdd(menu1M);
		menu5M = new MenuItem();
		menu5M->SetName(_T("menu5M"));
		menu5M->SetText(_T("5 minutes"));
		menu5M->Click += EventHandler((Object*)this, (EventFun)&MainForm::Control_Click);
		menuQuick->ItemAdd(menu5M);
		menu10M = new MenuItem();
		menu10M->SetName(_T("menu10M"));
		menu10M->SetText(_T("10 minutes"));
		menu10M->Click += EventHandler((Object*)this, (EventFun)&MainForm::Control_Click);
		menuQuick->ItemAdd(menu10M);
		menu15M = new MenuItem();
		menu15M->SetName(_T("menu15M"));
		menu15M->SetText(_T("15 minutes"));
		menu15M->Click += EventHandler((Object*)this, (EventFun)&MainForm::Control_Click);
		menuQuick->ItemAdd(menu15M);
		menu30M = new MenuItem();
		menu30M->SetName(_T("menu30M"));
		menu30M->SetText(_T("30 minutes"));
		menu30M->Click += EventHandler((Object*)this, (EventFun)&MainForm::Control_Click);
		menuQuick->ItemAdd(menu30M);
		menu1H = new MenuItem();
		menu1H->SetName(_T("menu1H"));
		menu1H->SetText(_T("1 hour"));
		menu1H->Click += EventHandler((Object*)this, (EventFun)&MainForm::Control_Click);
		menuQuick->ItemAdd(menu1H);
	
	menuTopmost = new MenuItem();
	menuTopmost->SetText(_T("Always Top"));
	menuTopmost->Click += EventHandler((Object*)this, (EventFun)&MainForm::Control_Click);
	contextMenu->ItemAdd(menuTopmost);

	menuHhmm = new MenuItem();
	menuHhmm->SetText(_T("Show HH:MM only"));
	menuHhmm->Click += EventHandler((Object*)this, (EventFun)&MainForm::Control_Click);
	contextMenu->ItemAdd(menuHhmm);
	
	menuNoCaption = new MenuItem();
	menuNoCaption->SetText(_T("No Window Title"));
	menuNoCaption->Click += EventHandler((Object*)this, (EventFun)&MainForm::Control_Click);
	contextMenu->ItemAdd(menuNoCaption);

	menuNolink = new MenuItem();
	menuNolink->SetText(_T("Not Show Link"));
	menuNolink->Click += EventHandler((Object*)this, (EventFun)&MainForm::Control_Click);
	contextMenu->ItemAdd(menuNolink);

	menuAbout = new MenuItem();
	menuAbout->SetText(_T("About"));
	menuAbout->Click += EventHandler((Object*)this, (EventFun)&MainForm::Control_Click);
	contextMenu->ItemAdd(menuAbout);

	menuExit = new MenuItem();
	menuExit->SetText(_T("Exit"));
	menuExit->Click += EventHandler((Object*)this, (EventFun)&MainForm::Control_Click);
	contextMenu->ItemAdd(menuExit);

	int nTop = 8;
	chkTimer = new ToolItem();
	chkTimer->SetName(_T("chkTimer"));
	chkTimer->SetText(_T("Timer"));
	chkTimer->SetLocation(5, 3);
	chkTimer->SetSize(40, 19);
	chkTimer->SetIsPushButton(true);
	chkTimer->SetOuterStyle(OuterStyle::AutoSize, true);
	chkTimer->Click += EventHandler((Object*)this, (EventFun)&MainForm::Control_Click);
	this->AddControl(chkTimer);

	btnStart = new ToolItem();
	btnStart->SetName(_T("btnStart"));
	btnStart->SetText(_T("Start"));
	btnStart->SetLocationOffset(chkTimer, false, 1, 0);
	btnStart->SetSize(40, 19);
	btnStart->Click += EventHandler((Object*)this, (EventFun)&MainForm::Control_Click);
	btnStart->SetIsPushButton(true);
	this->AddControl(btnStart);
	btnClear = new ToolItem();
	btnClear->SetName(_T("btnStart"));
	btnClear->SetText(_T("Reset"));
	btnClear->SetLocationOffset(btnStart, false, 1, 0);
	btnClear->SetSize(40, 19);
	btnClear->Click += EventHandler((Object*)this, (EventFun)&MainForm::Control_Click);
	this->AddControl(btnClear);
	btnSet = new ToolItem();
	btnSet->SetName(_T("btnStart"));
	btnSet->SetText(_T("Set"));
	btnSet->SetLocationOffset(btnClear, false, 1, 0);
	btnSet->SetSize(40, 19);
	btnSet->Click += EventHandler((Object*)this, (EventFun)&MainForm::Control_Click);
	btnSet->SetContextMenu(contextMenu);
	this->AddControl(btnSet);

	led = new LEDControl();
	led->SetName(_T("led"));
	led->SetText(_T(""));
	led->SetTextColor(RGB(0, 0, 255));
	//led->SetBackground(RGB(230, 245, 235));
	led->SetTextBackground(RGB(0xe8, 0xe8, 0xe8));
	led->SetLocation(5, 7 + 20);
	led->SetSize(600 - 15, 600 - 30 - 20 + 7);
	led->SetOuterStyle(OuterStyle::TransparentParentErase, true);
	led->Anchor = (AnchorStyles::Type)(AnchorStyles::Top | AnchorStyles::Bottom | AnchorStyles::Left | AnchorStyles::Right);
	led->TimerEnd += EventHandler((Object*)this, (EventFun)&MainForm::Control_Click);
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
	this->SetMinSize(110, 100);
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

	//show it until Form_Load
	this->SetVisible(false);
}

void MainForm::Form_Load(Object* sender, EventArgs* e)
{
	//load config
	this->Config(true, *CC_APP_GetIni(), cc::Str(_T("Main")));
	if(CC_APP_GetIni()->GetBool(_T("Main.Timer"), true))
	{
		chkTimer->SetPushed(true);
	}
	Control_Click(chkTimer, NULL);
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
	CC_APP_GetIni()->SetBool(_T("Main.Timer"), chkTimer->GetPushed());
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
	if(sender == chkTimer)
	{
		bool chk = chkTimer->GetPushed();
		if(chk) {
			led->SetTimerSec(0);
			led->SetStart(true);
		}
		else {
			led->SetTimerSec(-1);
		}
		btnStart->SetEnabled(chk);
		btnClear->SetEnabled(chk);
		btnSet->SetEnabled(chk);
	}
	else if(sender == btnStart)
	{
		bool chk = btnStart->GetPushed();
		led->SetStart(chk);
	}
	else if(sender == btnClear)
	{
		led->SetTimerSec(nResetSec);
	}
	else if(sender == menuHhmm)
	{
		bool chk = !menuHhmm->GetChecked();
		menuHhmm->SetChecked(chk);
		led->SetCharWidth(-1);
		led->SetHhmm(chk);
	}
	else if(sender == menuOption)
	{
		SetDialog* dialog = new SetDialog();
		dialog->SetParent(this, true);
		dialog->SetCenter();
		DialogResult::Type type = dialog->ShowDialog();
		dialog->Dispose();
		delete dialog;

		if(type == DialogResult::OK) {
			int chk = CC_APP_GetIni()->Get(_T("SetDlg.radio1"), 0);
			if(chk) {
				int m = CC_APP_GetIni()->Get(_T("SetDlg.spanM0"), 0);
				int s = CC_APP_GetIni()->Get(_T("SetDlg.spanS0"), 0);
				nResetSec = m * 60 + s;
			}
			else {
				struct tm tm;
				time_t m_time = ::time(NULL);
				localtime_s(&tm, &m_time);
				int nNowSec = tm.tm_hour * 60 * 60 + tm.tm_min * 60 + tm.tm_sec;

				int h = CC_APP_GetIni()->Get(_T("SetDlg.spanH"), 0);
				int m = CC_APP_GetIni()->Get(_T("SetDlg.spanM"), 0);
				int s = CC_APP_GetIni()->Get(_T("SetDlg.spanS"), 0);
				int nSetSec = h * 60 * 60 + m * 60 + s;
				if(nSetSec < nNowSec) {
					nSetSec += 24 * 60 * 60;
				}
				nResetSec = nSetSec - nNowSec;
			}
			btnStart->SetPushed(true);
			led->SetStart(true);
			led->SetTimerSec(nResetSec);
		}
	}
	else if(sender == led)
	{
		bool chkBeep = CC_APP_GetIni()->GetBool(_T("SetDlg.chkBeep"), false);
		if(chkBeep) {
			CreateThread(NULL, 0, BeepThread, NULL, 0, NULL);
		}
		bool chkSound = CC_APP_GetIni()->GetBool(_T("SetDlg.chkSound"), false);
		if(chkSound) {
			cc::Str str = CC_APP_GetIni()->Get(_T("SetDlg.editSound"));
			if(cc::Util::fExist(str) && str.EndsWith(_T(".wav"), true)) {
				str.ToWChar();
				cc::Util::PlaySoundW((wchar_t*)str.GetBuf(), NULL, SND_FILENAME|SND_ASYNC|SND_NOWAIT|SND_NODEFAULT);
			}
		}
		bool chkText = CC_APP_GetIni()->GetBool(_T("SetDlg.chkText"), false);
		if(chkText) {
			cc::Str str = CC_APP_GetIni()->Get(_T("SetDlg.editText"));
			MsgDlg *dialog = new MsgDlg(_T("On Timer:\r\n") + str);
			dialog->SetParent(this, true);
			dialog->SetCenter();
			dialog->ShowDialog();
			delete dialog;

		}

		btnStart->SetPushed(false);
		led->SetStart(false);
	}
	else if(sender == menu1M)
	{
		nResetSec = 60;
		btnStart->SetPushed(false);
		led->SetStart(false);
		led->SetTimerSec(nResetSec);
	}
	else if(sender == menu5M)
	{
		nResetSec = 60 * 5;
		btnStart->SetPushed(false);
		led->SetStart(false);
		led->SetTimerSec(nResetSec);
	}
	else if(sender == menu10M)
	{
		nResetSec = 60 * 10;
		btnStart->SetPushed(false);
		led->SetStart(false);
		led->SetTimerSec(nResetSec);
	}
	else if(sender == menu15M)
	{
		nResetSec = 60 * 15;
		btnStart->SetPushed(false);
		led->SetStart(false);
		led->SetTimerSec(nResetSec);
	}
	else if(sender == menu30M)
	{
		nResetSec = 60 * 30;
		btnStart->SetPushed(false);
		led->SetStart(false);
		led->SetTimerSec(nResetSec);
	}
	else if(sender == menu1H)
	{
		nResetSec = 60 * 60;
		btnStart->SetPushed(false);
		led->SetStart(false);
		led->SetTimerSec(nResetSec);
	}
	else if(sender == menuTopmost)
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
	else if(sender == menuAbout)
	{
		CC_APP_GetLang()->Set(_T("Lang.U_Link"), _T("http://uuware.com/"));
		CC_APP_GetLang()->Set(_T("Lang.U_LinkURL"), _T("http://uuware.com/"));
		cc::Str sAbout = _T("Desktop Timer Ver 1.00 - uuware.com");
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
