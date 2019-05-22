#include "MainForm.h"
using namespace cc;
using namespace cc::win;

//int main(int argc, char *argv[])
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// the defined of _WIN32_WINNT compile for this app should same to Lib!
	CC_ASSERT(App::_dWIN32_WINNT == _WIN32_WINNT);
#if defined(UNICODE) || defined(_UNICODE)
	CC_ASSERT(App::_dUNICODE == 1);
#else
	CC_ASSERT(App::_dUNICODE == 0);
#endif
#if defined(_DEBUG_)
	CC_ASSERT(App::_dDEBUG == 1);
#else
	CC_ASSERT(App::_dDEBUG == 0);
#endif

	// Compute pseudo-highlight color for background...
	COLORREF btnface = ::GetSysColor(COLOR_BTNFACE);
	COLORREF highlight = ::GetSysColor(COLOR_WINDOW);
	COLORREF whiteBackground = RGB(
		(BYTE)(0.7*GetRValue(highlight) + 0.3*GetRValue(btnface)),
		(BYTE)(0.7*GetGValue(highlight) + 0.3*GetGValue(btnface)),
		(BYTE)(0.7*GetBValue(highlight) + 0.3*GetBValue(btnface)));
	PanelConfig::corBackground = whiteBackground;
	//LabelConfig::corTextground = RGB(255, 0, 255);
	//ButtonConfig::nStyle |= BS_OWNERDRAW;
	//ButtonConfig::corBackground = RGB(80, 100, 100);
	//ButtonConfig::corHotBackColor = RGB(200, 200, 100);
	//ButtonConfig::corTextground = RGB(0, 0, 255);
	//ButtonConfig::corHotTextColor = RGB(0, 0, 255);
	//TextBoxConfig::corBackground = RGB(80, 175, 123);
	//TextBoxConfig::corTextground = RGB(168, 2, 15);
	//TextBoxConfig::corBackgroundReadOnly = RGB(255, 0, 0);
	//TextBoxConfig::corBackgroundDisabled = RGB(0, 255, 0);

	//CheckListBoxConfig::corBackground = RGB(128, 128, 128);
	//CheckListBoxConfig::corTextground = RGB(0, 0, 255);
	//CheckListBoxConfig::corHotBackColor = RGB(0, 100, 100);
	//CheckListBoxConfig::corHotTextColor = RGB(255, 0, 0);

	//when add manifest.xml for xp theme, also need set controls to have default teansparent
	//so while some controls need backcolor, need remove  (OuterStyle::TransparentParentBrush|OuterStyle::TransparentParentErase)
	cc::win::App::SetDefaultTransparent(true);
	cc::win::App::SetInstance(hInstance);
	//should set this before cc::Config::GetLang();
	cc::win::App::GetConfig()->LoadLang(CC_CONFIG_LANGENG);
	//the name is used for CreateMutex"s flages,cannot change to other lang
	cc::win::App::SetNameVersion(_T(APPNAME), _T(APPVERSION));

	//set Transparent for TabControlEx
	//cc::win::TabPageConfig::nOuterStyle = OuterStyle::TransparentParentErase;
	cc::win::FormConfig::nInnerStyleClient = InnerStyle::BackgroundTheme|FormClientConfig::nInnerStyle;
	cc::win::TabControlExConfig::nOuterStyle = OuterStyle::TransparentParentErase;
	cc::win::TabPageConfig::corBackground = ::GetSysColor(COLOR_MENU);
	cc::win::TabPageConfig::corHotBackColor = ::GetSysColor(COLOR_MENU);
	cc::win::TabPageConfig::corTextground = RGB(255, 0, 0);
	cc::win::TextBoxConfig::nOuterStyle |= OuterStyle::TransparentParentBrush;
	//should set next for RichTextBox
	//rich->SetStyle(StyleType::ExStyle, 0, WS_EX_TRANSPARENT);
	//rich->SetOuterStyle(OuterStyle::TransparentHollowBrush, true);

	MainForm* form = new MainForm();
	cc::win::App::Run(form);
	form->Dispose();
	delete form;
	return 0;
}


TestControls::TestControls(Form* formClickEvent, Control* parent, ImageList* imageList, ContextMenu* coneMenuConfig, int nLeft, int nTop)
{
	label = new Label();
	label->SetName(_T("label"));
	label->SetText(_T("Label:"));
	label->SetTextColor(RGB(0, 0, 255));
	label->SetLocation(nLeft, nTop + 2);
	label->SetSize(30, 17);
	label->SetOuterStyle(OuterStyle::TransparentParentBrush, false);
	label->SetOuterStyle(OuterStyle::TransparentHollowBrush, false);
	parent->AddControl(label);

	pickColorCtl = new PickColorCtl();
	pickColorCtl->SetName(_T("pickColorCtl"));
	pickColorCtl->SetText(_T(""));
	//pickColorCtl->SetBackground(label->GetBackground());
	pickColorCtl->SetLocation(nLeft + 30, nTop);
	pickColorCtl->SetSize(15, 20);
	//pickColorCtl->SetOuterStyle(OuterStyle::AutoSize, true);
	//pickColorCtl->SetOuterStyle(OuterStyle::OuterEdge, false);
	pickColorCtl->SetResultControl(label, CC_PICKCOLOR_BACKGROUND);
	pickColorCtl->SetOuterStyle(OuterStyle::AutoConfig, true);
	parent->AddControl(pickColorCtl);

	pickColorCtl2 = new PickColorCtl();
	pickColorCtl2->SetName(_T("pickColorCtl2"));
	pickColorCtl2->SetText(_T(""));
	//pickColorCtl2->SetBackground(label->GetTextColor());
	pickColorCtl2->SetLocation(nLeft + 30 + 17, nTop);
	pickColorCtl2->SetSize(15, 20);
	//pickColorCtl2->SetOuterStyle(OuterStyle::AutoSize, true);
	//pickColorCtl2->SetOuterStyle(OuterStyle::OuterEdge, false);
	pickColorCtl2->SetResultControl(label, CC_PICKCOLOR_TEXTCOLOR);
	pickColorCtl2->SetOuterStyle(OuterStyle::AutoConfig, true);
	parent->AddControl(pickColorCtl2);

	btn1 = new Button();
	btn1->SetName(_T("btn1"));
	btn1->SetText(_T("Button"));
	btn1->SetTextColor(RGB(255, 0, 0));
	btn1->SetLocation(nLeft + 75, nTop);
	btn1->SetSize(70, 17);
	btn1->Click += EventHandler((Object*)formClickEvent, (EventFun)&MainForm::Control_Click);
	btn1->SetOuterStyle(OuterStyle::AutoConfig, true);
	parent->AddControl(btn1);

	tool1 = new ToolItem();
	tool1->SetName(_T("tool1"));
	tool1->SetText(_T("tool1"));
	tool1->SetTextColor(RGB(0, 255, 255));
	tool1->SetLocation(nLeft + 155, nTop);
	tool1->SetSize(55, 17);
	tool1->SetTips(_T("tips of toolbutton1.\r\ntest for click and popup.\r\nonly so."));
	tool1->SetIcon(imageList, IDI_ICON_REFRESH);
	tool1->Click += EventHandler((Object*)formClickEvent, (EventFun)&MainForm::Control_Click);
	tool1->SetContextMenu(coneMenuConfig);
	parent->AddControl(tool1);

	tool2 = new ToolItem();
	tool2->SetName(_T("tool2"));
	tool2->SetText(_T("tool2"));
	tool2->SetTextColor(RGB(150, 0, 255));
	tool2->SetLocation(nLeft + 215, nTop);
	tool2->SetSize(55, 17);
	tool2->SetTips(_T("tips of toolbutton1.\r\ntest for click and popup.\r\nonly so."));
	tool2->SetIcon(imageList, IDI_ICON_FONT);
	tool2->Click += EventHandler((Object*)formClickEvent, (EventFun)&MainForm::Control_Click);
	tool2->SetContextMenu(coneMenuConfig);
	tool2->SetIsPushButton(true);
	parent->AddControl(tool2);

	nTop += 20;
	radio1 = new RadioButton();
	radio1->SetName(_T("radio1"));
	radio1->SetText(_T("radio1"));
	radio1->SetTextColor(RGB(120, 0, 80));
	radio1->SetLocation(nLeft, nTop);
	radio1->SetSize(70, 17);
	radio1->SetChecked(true);
	radio1->SetOuterStyle(OuterStyle::AutoConfig, true);
	radio1->SetOuterStyle(OuterStyle::AutoSize, true);
	radio1->Click += EventHandler((Object*)formClickEvent, (EventFun)&MainForm::Control_Click);
	parent->AddControl(radio1);

	radio2 = new RadioButton();
	radio2->SetName(_T("radio2"));
	radio2->SetText(_T("radio2"));
	radio2->SetTextColor(RGB(255, 0, 100));
	radio2->SetLocation(nLeft + 75, nTop);
	radio2->SetSize(70, 17);
	radio2->SetChecked(false);
	radio2->SetOuterStyle(OuterStyle::AutoConfig, true);
	radio2->SetOuterStyle(OuterStyle::AutoSize, true);
	radio2->Click += EventHandler((Object*)formClickEvent, (EventFun)&MainForm::Control_Click);
	parent->AddControl(radio2);

	chk1 = new CheckBox();
	chk1->SetName(_T("chk1"));
	chk1->SetText(_T("chk1"));
	chk1->SetTextColor(RGB(0, 100, 100));
	chk1->SetLocation(nLeft + 160, nTop);
	chk1->SetSize(80, 17);
	chk1->SetOuterStyle(OuterStyle::AutoSize, true);
	chk1->Click += EventHandler((Object*)formClickEvent, (EventFun)&MainForm::Control_Click);
	chk1->SetOuterStyle(OuterStyle::AutoConfig, true);
	parent->AddControl(chk1);

	pic1 = new PictureBox();
	pic1->SetName(_T("pic1"));
	pic1->SetText(_T("pic1"));
	pic1->SetTextColor(RGB(200, 0, 100));
	pic1->SetBackground(RGB(0, 200, 200));
	pic1->SetLocation(nLeft + 220, nTop + 2);
	//SetRealSizeImage is seem not work at w2k
	pic1->SetSize(40, 40);
	pic1->SetIcon(imageList, IDI_ICON_COLOR);
	//pic1->SetImage(IDI_BMP_BACK16, IMAGE_BITMAP, true);
	//pic1->SetRealSizeImage(true);
	parent->AddControl(pic1);

	nTop += 20;
	txtSpin1 = new TextBox();
	txtSpin1->SetTextColor(RGB(100, 0, 0));
	txtSpin1->SetBackground(RGB(100, 200, 200));
	txtSpin1->SetStyleNumber(true);
	txtSpin1->SetLocation(nLeft, nTop);
	txtSpin1->SetSize(40, 17);
	//txtSpin1->SetTextInt(50);
	parent->AddControl(txtSpin1);
	spin1 = new SpinButton();
	spin1->SetName(_T("spin1"));
	spin1->SetText(_T("spin1"));
	spin1->SetLocation(nLeft + 40, nTop);
	spin1->SetBuddy(txtSpin1);
	spin1->SetRange(-100, 100);
	spin1->SetBase(10);
	spin1->SetPos(30);
	parent->AddControl(spin1);

	pickColorCtl3 = new PickColorCtl();
	pickColorCtl3->SetName(_T("pickColorCtl3"));
	pickColorCtl3->SetText(_T("Back"));
	//pickColorCtl3->SetBackground(txtSpin1->GetBackground());
	pickColorCtl3->SetLocation(nLeft + 60, nTop);
	pickColorCtl3->SetSize(15, 17);
	pickColorCtl3->SetOuterStyle(OuterStyle::AutoSize, true);
	pickColorCtl3->SetOuterStyle(OuterStyle::OuterEdge, false);
	pickColorCtl3->SetResultControl(txtSpin1, CC_PICKCOLOR_BACKGROUND);
	pickColorCtl3->SetOuterStyle(OuterStyle::AutoConfig, true);
	parent->AddControl(pickColorCtl3);

	pickColorCtl4 = new PickColorCtl();
	pickColorCtl4->SetName(_T("pickColorCtl4"));
	pickColorCtl4->SetText(_T("Text"));
	//pickColorCtl4->SetBackground(txtSpin1->GetTextColor());
	pickColorCtl4->SetLocation(nLeft + 60 + 50, nTop);
	pickColorCtl4->SetSize(15, 17);
	pickColorCtl4->SetOuterStyle(OuterStyle::AutoSize, true);
	pickColorCtl4->SetOuterStyle(OuterStyle::OuterEdge, false);
	pickColorCtl4->SetResultControl(txtSpin1, CC_PICKCOLOR_TEXTCOLOR);
	pickColorCtl4->SetOuterStyle(OuterStyle::AutoConfig, true);
	parent->AddControl(pickColorCtl4);

	nTop += 20;
	com1 = new ComboBox();
	com1->SetName(_T("com1"));
	com1->SetText(_T("Text1;	Text2;	Text3;	Text4asdfdsafsadfv sadfas;	Text5;	Text6;	"));
	com1->SetTips(_T("click chk1 to change style"));
	com1->SetComboBoxStyle(ComboBoxStyle::DropDownList);
	com1->SetLocation(nLeft, nTop);
	com1->SetSize(70, 145);
	parent->AddControl(com1);

	comEx1 = new ComboBoxEx();
	comEx1->SetName(_T("comEx1"));
	comEx1->SetText(_T("Text1;	Text2;	Text3 sad fsadf adsf asdfas;	Text4;	Text5;	Text6;	"));
	//comEx1->SetComboBoxStyle(ComboBoxStyle::DropDownList);
	comEx1->SetLocation(nLeft + 75, nTop);
	comEx1->SetSize(70, 145);
	//parent->AddControl(comEx1);

	led = new LEDControl();
	led->SetName(_T("led"));
	led->SetText(_T(""));
	led->SetTextColor(RGB(0, 0, 255));
	//led->SetBackground(RGB(230, 245, 235));
	//led->SetTextBackground(RGB(230, 245, 235));
	led->SetLocation(nLeft + 150, nTop);
	led->SetSize(125, 40);
	led->SetOuterStyle(OuterStyle::TransparentParentErase, true);
	parent->AddControl(led);

	nTop += 22;
	dtPicker = new DateTimePicker();
	dtPicker->SetName(_T("dtPicker"));
	dtPicker->SetLocation(nLeft, nTop);
	dtPicker->SetSize(86, 18);
	parent->AddControl(dtPicker);

	nTop += 20;
	link1 = new LabelLink();
	link1->SetName(_T("link1"));
	link1->SetText(_T("http://www.uuware.com/"));
	link1->SetLocation(nLeft, nTop);
	link1->SetSize(100, 17);
	link1->SetOuterStyle(OuterStyle::AutoSize, true);
	//link1->SetURL((const TCHAR*)CC_APP_GetLang()->Get(_T("Lang.U_LinkURL")));
	parent->AddControl(link1);

	nTop += 20;
	glabel = new GradientLabel();
	glabel->SetName(_T("glabel"));
	glabel->SetText(_T("GradientLabel"));
	glabel->SetTextColor(RGB(0, 0, 155));
	glabel->SetHotTextColor(RGB(0, 0, 255));
	glabel->SetHotBackColor(RGB(70, 170, 100));
	glabel->SetBackground(RGB(230, 245, 235));
	glabel->SetLocation(nLeft, nTop);
	glabel->SetSize(200, 17);
	parent->AddControl(glabel);
}

TestControls::~TestControls()
{
}

void TestControls::SetEnabled(bool value)
{
	label->SetEnabled(value);
	btn1->SetEnabled(value);
	tool1->SetEnabled(value);
	tool2->SetEnabled(value);
	radio1->SetEnabled(value);
	radio2->SetEnabled(value);
	chk1->SetEnabled(value);
	pic1->SetEnabled(value);
	txtSpin1->SetEnabled(value);
	spin1->SetEnabled(value);
	com1->SetEnabled(value);
	comEx1->SetEnabled(value);
	dtPicker->SetEnabled(value);
	link1->SetEnabled(value);
	glabel->SetEnabled(value);
	pickColorCtl->SetEnabled(value);
	pickColorCtl2->SetEnabled(value);
	pickColorCtl3->SetEnabled(value);
	pickColorCtl4->SetEnabled(value);
	led->SetEnabled(value);
}

void TestControls::SetVisible(bool value)
{
	label->SetVisible(value);
	btn1->SetVisible(value);
	tool1->SetVisible(value);
	tool2->SetVisible(value);
	radio1->SetVisible(value);
	radio2->SetVisible(value);
	chk1->SetVisible(value);
	pic1->SetVisible(value);
	txtSpin1->SetVisible(value);
	spin1->SetVisible(value);
	com1->SetVisible(value);
	comEx1->SetVisible(value);
	dtPicker->SetVisible(value);
	link1->SetVisible(value);
	glabel->SetVisible(value);
	pickColorCtl->SetVisible(value);
	pickColorCtl2->SetVisible(value);
	pickColorCtl3->SetVisible(value);
	pickColorCtl4->SetVisible(value);
	led->SetVisible(value);
}

MainForm::MainForm()
{
	tabDialog = NULL;
	tabExDialog = NULL;
	hImageList = NULL;
	InitializeComponent();

	CC_APP_SetVarious(_T("ImageList"), imageList);
	CC_APP_SetVarious(_T("ContextMenu1"), contextMenu1);
	CC_APP_SetVarious(_T("coneMenuConfig"), coneMenuConfig);
	CC_APP_SetVarious(_T("ContextMenu3"), contextMenu3);
	CC_APP_SetVarious(_T("ContextMenu4"), contextMenu4);
	CC_APP_SetVarious(_T("NotifyIcon"), notify1);
	CC_APP_SetVarious(_T("MainForm"), this);

	testCtlsForm = new TestControls(this, this, imageList, coneMenuConfig, 5, 5);
	testCtlsGroup1 = new TestControls(this, groupForm1, imageList, coneMenuConfig, 5, 5);
	testCtlsGroup2 = new TestControls(this, groupForm2, imageList, coneMenuConfig, 5, 5);
	testCtlsTab = new TestControls(this, tabPage1, imageList, coneMenuConfig, 5, 5);
	testCtlsTabGroup1 = new TestControls(this, groupPage1, imageList, coneMenuConfig, 5, 5);
	testCtlsTabGroup2 = new TestControls(this, groupPage2, imageList, coneMenuConfig, 5, 5);
}

MainForm::~MainForm()
{
	if(hImageList != NULL)
	{
		ImageList_Destroy(hImageList);
		hImageList = NULL;
	}
	delete testCtlsForm;
	delete testCtlsGroup1;
	delete testCtlsGroup2;
	delete testCtlsTab;
	delete testCtlsTabGroup1;
	delete testCtlsTabGroup2;
}

void MainForm::OnThemeChanged()
{
	Form::OnThemeChanged();
	bool isThemed = (cc::Theme::IsAppThemed() == TRUE);
	for(int i = statusbar->GetControlCount() - 1; i >= 0; i--)
	{
		statusbar->GetControl(i)->SetOuterStyle(OuterStyle::AlwaysNoEdge, !isThemed);
	}
	if(m_hWnd != NULL)
	{
		//reshow rebar
		RECT rect;
		::GetWindowRect(m_hWnd, &rect);
		SetBoundsNotOriginal(0, 0, rect.right - rect.left + 1, rect.bottom - rect.top + 1, BoundsSpecified::Size);
		SetBoundsNotOriginal(0, 0, rect.right - rect.left, rect.bottom - rect.top, BoundsSpecified::Size);
	}
}

void MainForm::Form_Load(Object* sender, EventArgs* e)
{
	//load config
	this->Config(true, *CC_APP_GetIni(), cc::Str(_T("Main")));
	//show it until Form_Load
	this->SetVisible(true);

	ConfigBackDlg::ConfigResult(this->GetClient(), 0);
	toolOnTop_Click(NULL, NULL);
	hImageList = imageList->CreateHIMAGELIST(16, 16);
	comboBoxEx1->SetImageList(hImageList);
	comboBoxEx1->ItemAdd(_T("http://www.google.com/"), 0, 0, 1, 0);
	comboBoxEx1->ItemAdd(_T("http://www.msn.com/"), 1, 2, 3, 0);
	comboBoxEx1->SetSelectedIndex(0);

	//get all lang(res and files) and set to item
	cc::win::WinUtil::AddLangItems(comboLang, true, true);
}

void MainForm::Form_Closing(Object* sender, CancelEventArgs* e)
{
}

void MainForm::Form_Closed(Object* sender, EventArgs* e)
{
	if(tabDialog != NULL)
	{
		tabDialog->Dispose();
		delete tabDialog;
		tabDialog = NULL;
	}
	if(tabExDialog != NULL)
	{
		tabExDialog->Dispose();
		delete tabExDialog;
		tabExDialog = NULL;
	}

	//save config
	this->Config(false, *CC_APP_GetIni(), cc::Str(_T("Main")));
}

void MainForm::Form_Shutdown(Object* sender, EventArgs* e)
{
	//save config while window down
	CC_APP_GetConfig()->SaveIni();
}

void MainForm::OptionOK_Click(Object* sender, EventArgs* e)
{
	//get selected lang and set to ini
	cc::win::WinUtil::SetLangToConfig(comboLang);
	SetStatus(NULL);
	SetTips(NULL);
	cc::win::App::GetConfig()->LoadLang(CC_CONFIG_LANGENG);
	SetLang(*CC_APP_GetLang(), cc::Str(CC_CONFIG_LANGSECTION));
}

void MainForm::Control_Click(Object* sender, EventArgs* e)
{
	Form* actForm = (Form*)App::GetActiveForm();
	if(actForm == NULL)
	{
		actForm = this;
	}
	Control* control = (Control*)sender;
	cc::Log::debug(_T("Control_Click S, Name:%s, Text:%s"), control->GetName(), control->GetText());
	bool isPushed = false;
	if(sender == toolbutton6)
	{
		//set some disable and some to be enabled
		isPushed = toolbutton6->GetPushed();
		if(isPushed)
		{
			toolbutton6->SetText(_T("Enable"));
		}
		else
		{
			toolbutton6->SetText(_T("Disable"));
		}
		toolitem9->SetEnabled(!isPushed);
		toolitem10->SetEnabled(!isPushed);
		toolitem11->SetEnabled(!isPushed);

		//tab1->SetEnabled(!isPushed);
		tabPage1->SetEnabled(!isPushed);
		tabPage2->SetEnabled(!isPushed);
		tabPage3->SetEnabled(isPushed);
		tabPage4->SetEnabled(isPushed);
		tabPage5->SetEnabled(isPushed);

		testCtlsForm->SetEnabled(!isPushed);
		groupForm1->SetEnabled(isPushed);
		testCtlsGroup1->SetEnabled(isPushed);
		groupForm2->SetEnabled(isPushed);
		testCtlsGroup2->SetEnabled(isPushed);
	}
	else if(sender == toolbutton7)
	{
		//add ICON or remove ICON from some controls
		isPushed = toolbutton7->GetPushed();
		if(isPushed)
		{
			toolbutton7->SetText(_T("Del ICON"));
			toolbutton7->SetIcon(imageList, IDI_ICON2);
			toolbutton6->SetIcon(imageList, 0);
		}
		else
		{
			toolbutton7->SetText(_T("Add ICON"));
			toolbutton7->SetIcon(imageList, 0);
			toolbutton6->SetIcon(imageList, IDI_ICON2);
		}
	}
	else if(sender == menuFClientBack1)
	{
		menuFClientBack1->SetChecked(true);
		menuFClientBack2->SetChecked(false);
		menuFClientBack3->SetChecked(false);
		menuFClientBack4->SetChecked(false);
		menuFClientBack5->SetChecked(false);

		COLORREF cor = actForm->GetClient()->GetBackground();
		if(cc::Util::selectColor(cor, true, NULL, -1, m_hWnd))
		{
		}
		//auto set to CC_BACK_TYPE_COLOR
		actForm->GetClient()->SetBackground(cor);
		tabPage1->SetBackground(cor);
	}
	else if(sender == menuFClientBack2)
	{
		menuFClientBack1->SetChecked(false);
		menuFClientBack2->SetChecked(true);
		menuFClientBack3->SetChecked(false);
		menuFClientBack4->SetChecked(false);
		menuFClientBack5->SetChecked(false);
		actForm->GetClient()->SetStyle(StyleType::Inner,
			InnerStyle::BackgroundBrush|InnerStyle::BackgroundColor|InnerStyle::BackgroundBmp,
			InnerStyle::BackgroundTheme);
		tabPage1->SetStyle(StyleType::Inner,
			InnerStyle::BackgroundBrush|InnerStyle::BackgroundColor|InnerStyle::BackgroundBmp,
			InnerStyle::BackgroundTheme);
	}
	else if(sender == menuFClientBack3)
	{
		menuFClientBack1->SetChecked(false);
		menuFClientBack2->SetChecked(false);
		menuFClientBack3->SetChecked(true);
		menuFClientBack4->SetChecked(false);
		menuFClientBack5->SetChecked(false);

		bool isStretch = (IDYES == (int)::MessageBox(m_hWnd, _T("Stretch it?"), TCHAR_LANG("Lang.S_MsgBox_Title_Question"), MB_YESNOCANCEL | MB_ICONQUESTION));
		actForm->GetClient()->SetBackgroundImage(IDI_IMG_BACK2, _T("IMAGE"), isStretch);//IMAGE_BITMAP
		tabPage1->SetBackgroundImage(IDI_IMG_BACK1, _T("IMAGE"), isStretch);
	}
	else if(sender == menuFClientBack4)
	{
		cc::Str selectedPath = cc::Util::selectOpenFile(_T(""), _T("Image Files (*.jpg;*.gif;*.tif;*.bmp;*.ico)\0*.jpg;*.gif;*.tif;*.bmp;*.ico\0\0"), _T(".bmp"), m_hWnd, _T("bmp"), false);
		if(selectedPath.GetLength() > 0)
		{
			menuFClientBack1->SetChecked(false);
			menuFClientBack2->SetChecked(false);
			menuFClientBack3->SetChecked(false);
			menuFClientBack4->SetChecked(true);
			menuFClientBack5->SetChecked(false);
			bool isStretch = (IDYES == (int)::MessageBox(m_hWnd, _T("Stretch it?"), TCHAR_LANG("Lang.S_MsgBox_Title_Question"), MB_YESNOCANCEL | MB_ICONQUESTION));
			actForm->GetClient()->SetBackgroundImage(selectedPath, isStretch);
			tabPage1->SetBackgroundImage(selectedPath, isStretch);
			//bool isIcon = selectedPath.EndsWith(_T(".ico"), true);
			//HANDLE handle = ::LoadImage(CC_APP_GetInstance(),
			//	selectedPath,
			//	isIcon ? IMAGE_ICON : IMAGE_BITMAP, // type
			//	0, // actual width
			//	0, // actual height
			//	LR_LOADFROMFILE); // no flags
			////auto set to CC_BACK_TYPE_BMP
			//if(isIcon)
			//{
			//	actForm->GetClient()->SetBackgroundImage((HICON)handle, isStretch, true);
			//}
			//else
			//{
			//	actForm->GetClient()->SetBackgroundImage((HBITMAP)handle, isStretch, true);
			//}
		}
	}
	else if(sender == menuFClientBack5)
	{
		menuFClientBack1->SetChecked(false);
		menuFClientBack2->SetChecked(false);
		menuFClientBack3->SetChecked(false);
		menuFClientBack4->SetChecked(false);
		menuFClientBack5->SetChecked(true);
		static int isFirst = 0;
		////HS_DIAGCROSS,HS_CROSS,
		HBRUSH hbr = ::CreateHatchBrush(isFirst == 0 ? HS_DIAGCROSS : HS_CROSS, actForm->GetClient()->GetBackground());
		actForm->GetClient()->SetBackgroundBrush(hbr);
		tabPage1->SetBackgroundBrush(hbr);
		isFirst = (isFirst == 0 ? 1 : 0);
	}
	else if(sender == menuFClientBack6)
	{
		bool isChk = !menuFClientBack6->GetChecked();
		menuFClientBack6->SetChecked(isChk);
		if(isChk)
		{
			bool isOK = (IDYES == (int)::MessageBox(m_hWnd, _T("is Vertical?"), TCHAR_LANG("Lang.S_MsgBox_Title_Question"), MB_YESNOCANCEL | MB_ICONQUESTION));
			actForm->GetClient()->SetStyleBool(StyleType::Inner2, Inner2Style::GradientVertical, isOK);
			COLORREF cor = actForm->GetClient()->GetHotBackColor();
			if(cc::Util::selectColor(cor, true, NULL, -1, m_hWnd))
			{
			}
			actForm->GetClient()->SetHotBackColor(cor);
			actForm->GetClient()->SetStyle(StyleType::Inner, 0, InnerStyle::BackgroundGradient, true);
			tabPage1->SetStyleBool(StyleType::Inner2, Inner2Style::GradientVertical, isOK);
			tabPage1->SetHotBackColor(cor);
			tabPage1->SetStyle(StyleType::Inner, 0, InnerStyle::BackgroundGradient, true);
		}
		else
		{
			actForm->GetClient()->SetStyle(StyleType::Inner, InnerStyle::BackgroundGradient, 0, true);
			menuFClientBack1->SetChecked(true);
			menuFClientBack2->SetChecked(false);
			menuFClientBack3->SetChecked(false);
			menuFClientBack4->SetChecked(false);
			menuFClientBack5->SetChecked(false);
			actForm->GetClient()->SetBackground(actForm->GetClient()->GetBackground());
			tabPage1->SetStyle(StyleType::Inner, InnerStyle::BackgroundGradient, 0, true);
			tabPage1->SetBackground(actForm->GetClient()->GetBackground());
		}
	}
	else if(sender == menuToolEdge)
	{
		bool isOn = testCtlsForm->tool1->HasStyle(StyleType::Outer, OuterStyle::AlwaysEdge);
		testCtlsForm->tool1->SetOuterStyle(OuterStyle::AlwaysEdge, !isOn, true);
		testCtlsForm->tool2->SetOuterStyle(OuterStyle::AlwaysEdge, !isOn, true);
		testCtlsGroup1->tool1->SetOuterStyle(OuterStyle::AlwaysEdge, !isOn, true);
		testCtlsGroup1->tool2->SetOuterStyle(OuterStyle::AlwaysEdge, !isOn, true);
		testCtlsGroup2->tool1->SetOuterStyle(OuterStyle::AlwaysEdge, !isOn, true);
		testCtlsGroup2->tool2->SetOuterStyle(OuterStyle::AlwaysEdge, !isOn, true);
		testCtlsTab->tool1->SetOuterStyle(OuterStyle::AlwaysEdge, !isOn, true);
		testCtlsTab->tool2->SetOuterStyle(OuterStyle::AlwaysEdge, !isOn, true);
		testCtlsTabGroup1->tool1->SetOuterStyle(OuterStyle::AlwaysEdge, !isOn, true);
		testCtlsTabGroup1->tool2->SetOuterStyle(OuterStyle::AlwaysEdge, !isOn, true);
		testCtlsTabGroup2->tool1->SetOuterStyle(OuterStyle::AlwaysEdge, !isOn, true);
		testCtlsTabGroup2->tool2->SetOuterStyle(OuterStyle::AlwaysEdge, !isOn, true);
	}
	else if(sender == menuPictureBack1)
	{
		cc::Str selectedPath = cc::Util::selectOpenFile(_T(""), _T("Image Files (*.jpg;*.gif;*.tif;*.bmp;*.ico)\0*.jpg;*.gif;*.tif;*.bmp;*.ico\0\0"), _T(".bmp"), m_hWnd, _T("bmp"), false);
		if(selectedPath.GetLength() > 0)
		{
			testCtlsForm->pic1->SetImage(selectedPath);
			testCtlsGroup1->pic1->SetImage(selectedPath);
			testCtlsGroup2->pic1->SetImage(selectedPath);
			testCtlsTab->pic1->SetImage(selectedPath);
			testCtlsTabGroup1->pic1->SetImage(selectedPath);
			testCtlsTabGroup2->pic1->SetImage(selectedPath);
		}
	}
	else if(sender == menuPictureBack2)
	{
		testCtlsForm->pic1->SetImage(IDI_BMP_BACK16, IMAGE_BITMAP);
		testCtlsGroup1->pic1->SetImage(IDI_BMP_BACK16, IMAGE_BITMAP);
		testCtlsGroup2->pic1->SetImage(IDI_BMP_BACK16, IMAGE_BITMAP);
		testCtlsTab->pic1->SetImage(IDI_BMP_BACK16, IMAGE_BITMAP);
		testCtlsTabGroup1->pic1->SetImage(IDI_BMP_BACK16, IMAGE_BITMAP);
		testCtlsTabGroup2->pic1->SetImage(IDI_BMP_BACK16, IMAGE_BITMAP);
	}
	else if(sender == mainMenu_3_0)
	{
		cc::Str sAbout = App::GetNameVersion();
		sAbout.Append(_T(" ")).Append(APPBUILD);
		AboutDlg* input = new AboutDlg(sAbout, imageList, IDI_ICON_MAIN, CC_APP_GetLang()->Get(_T("Lang.U_CopyRightTitle")), 
			420, 36, 36, CC_APP_GetLang()->Get(_T("Lang.U_CopyRight")), true, true);
		input->SetParent(this, true);
		input->SetCenter();
		//input->GetClient()->SetStyle(StyleType::Inner, 0, InnerStyle::BackgroundTheme);
		input->ShowDialog();

		input->Dispose();
		delete input;
	}
#ifdef _DEBUG_
	else if(sender == menuTestMsgDlg)
	{
		cc::win::MsgDlg::test(actForm);
	}
	else if(sender == menuTestAboutDlg)
	{
		cc::win::AboutDlg::test(actForm);
	}
#endif
	else if(sender == menuGroupBreak)
	{
		bool isChk = !menuGroupBreak->GetChecked();
		menuGroupBreak->SetChecked(isChk);
		groupForm1->SetOuterStyle(OuterStyle::Break, isChk, true);
		groupForm2->SetOuterStyle(OuterStyle::Break, isChk, true);
		groupPage1->SetOuterStyle(OuterStyle::Break, isChk, true);
		groupPage2->SetOuterStyle(OuterStyle::Break, isChk, true);
	}
	else if(sender == menuGroupGradient)
	{
		bool isChk = !menuGroupGradient->GetChecked();
		menuGroupGradient->SetChecked(isChk);
		if(isChk)
		{
			groupForm1->SetHotBackColor(RGB(255, 0, 0));
			groupForm1->SetBackground(RGB(255, 255, 255));
			groupForm2->SetHotBackColor(RGB(255, 255, 255));
			groupForm2->SetBackground(RGB(255, 255, 0));
			groupPage1->SetHotBackColor(RGB(255, 255, 255));
			groupPage1->SetBackground(RGB(0, 0, 0));
			groupPage2->SetHotBackColor(RGB(0, 0, 255));
			groupPage2->SetBackground(RGB(255, 255, 0));

			groupForm1->SetStyle(StyleType::Outer, OuterStyle::TransparentParentErase, OuterStyle::TransparentTop);
			groupForm1->SetStyle(StyleType::Inner, 0, InnerStyle::BackgroundGradient);
			groupForm2->SetStyle(StyleType::Outer, OuterStyle::TransparentParentErase, OuterStyle::TransparentTop);
			groupForm2->SetStyle(StyleType::Inner, 0, InnerStyle::BackgroundGradient);
			groupPage1->SetStyle(StyleType::Outer, OuterStyle::TransparentParentErase, OuterStyle::TransparentTop);
			groupPage1->SetStyle(StyleType::Inner, 0, InnerStyle::BackgroundGradient);
			groupPage2->SetStyle(StyleType::Outer, OuterStyle::TransparentParentErase, OuterStyle::TransparentTop);
			groupPage2->SetStyle(StyleType::Inner, 0, InnerStyle::BackgroundGradient);
		}
		else
		{
			groupForm1->SetStyle(StyleType::Outer, OuterStyle::TransparentTop, OuterStyle::TransparentParentErase);
			groupForm1->SetStyle(StyleType::Inner, InnerStyle::BackgroundGradient, 0);
			groupForm2->SetStyle(StyleType::Outer, OuterStyle::TransparentTop, OuterStyle::TransparentParentErase);
			groupForm2->SetStyle(StyleType::Inner, InnerStyle::BackgroundGradient, 0);
			groupPage1->SetStyle(StyleType::Outer, OuterStyle::TransparentTop, OuterStyle::TransparentParentErase);
			groupPage1->SetStyle(StyleType::Inner, InnerStyle::BackgroundGradient, 0);
			groupPage2->SetStyle(StyleType::Outer, OuterStyle::TransparentTop, OuterStyle::TransparentParentErase);
			groupPage2->SetStyle(StyleType::Inner, InnerStyle::BackgroundGradient, 0);
		}
	}


	//contextMenuTopIcon
	else if(sender == menuTopIcon1)
	{
		//Icon(Up)&&Text
		tbtnTopIcon1->SetStyle(StyleType::Outer, OuterStyle::DrawNoIcon|OuterStyle::DrawNoText, OuterStyle::DrawIconTop);
		tbtnTopIcon2->SetStyle(StyleType::Outer, OuterStyle::DrawNoIcon|OuterStyle::DrawNoText, OuterStyle::DrawIconTop);
		tbtnTopIcon4->SetStyle(StyleType::Outer, OuterStyle::DrawNoIcon|OuterStyle::DrawNoText, OuterStyle::DrawIconTop);
		tbtnTopIcon5->SetStyle(StyleType::Outer, OuterStyle::DrawNoIcon|OuterStyle::DrawNoText, OuterStyle::DrawIconTop);
		tbarTopIcon->SetMinSize(0, 32 + 24);
	}
	else if(sender == menuTopIcon2)
	{
		//Icon(Left)&&Text
		tbtnTopIcon1->SetStyle(StyleType::Outer, OuterStyle::DrawNoIcon|OuterStyle::DrawNoText|OuterStyle::DrawIconTop, 0);
		tbtnTopIcon2->SetStyle(StyleType::Outer, OuterStyle::DrawNoIcon|OuterStyle::DrawNoText|OuterStyle::DrawIconTop, 0);
		tbtnTopIcon4->SetStyle(StyleType::Outer, OuterStyle::DrawNoIcon|OuterStyle::DrawNoText|OuterStyle::DrawIconTop, 0);
		tbtnTopIcon5->SetStyle(StyleType::Outer, OuterStyle::DrawNoIcon|OuterStyle::DrawNoText|OuterStyle::DrawIconTop, 0);
		tbarTopIcon->SetMinSize(0, 32 + 0);
	}
	else if(sender == menuTopIcon3)
	{
		//Icon Only
		tbtnTopIcon1->SetStyle(StyleType::Outer, OuterStyle::DrawNoIcon|OuterStyle::DrawIconTop, OuterStyle::DrawNoText);
		tbtnTopIcon2->SetStyle(StyleType::Outer, OuterStyle::DrawNoIcon|OuterStyle::DrawIconTop, OuterStyle::DrawNoText);
		tbtnTopIcon4->SetStyle(StyleType::Outer, OuterStyle::DrawNoIcon|OuterStyle::DrawIconTop, OuterStyle::DrawNoText);
		tbtnTopIcon5->SetStyle(StyleType::Outer, OuterStyle::DrawNoIcon|OuterStyle::DrawIconTop, OuterStyle::DrawNoText);
		tbarTopIcon->SetMinSize(0, 32 + 0);
	}
	else if(sender == menuTopIcon4)
	{
		//Text Only
		tbtnTopIcon1->SetStyle(StyleType::Outer, OuterStyle::DrawNoText|OuterStyle::DrawIconTop, OuterStyle::DrawNoIcon);
		tbtnTopIcon2->SetStyle(StyleType::Outer, OuterStyle::DrawNoText|OuterStyle::DrawIconTop, OuterStyle::DrawNoIcon);
		tbtnTopIcon4->SetStyle(StyleType::Outer, OuterStyle::DrawNoText|OuterStyle::DrawIconTop, OuterStyle::DrawNoIcon);
		tbtnTopIcon5->SetStyle(StyleType::Outer, OuterStyle::DrawNoText|OuterStyle::DrawIconTop, OuterStyle::DrawNoIcon);
		tbarTopIcon->SetMinSize(0, 0 + 24);
	}
	else if(sender == menuTopIcon5)
	{
		bool chk = menuTopIcon5->GetChecked();
		menuTopIcon5->SetChecked(!chk);
		tbtnTopIcon1->SetEnabled(chk);
		tbtnTopIcon2->SetEnabled(chk);
		tbtnTopIcon4->SetEnabled(chk);
		//tbtnTopIcon5->SetEnabled(chk);
	}

	else if(sender == testCtlsForm->chk1)
	{
		bool chk = testCtlsForm->chk1->GetChecked();
		if(chk)
		{
			testCtlsForm->com1->SetComboBoxStyle(ComboBoxStyle::DropDownList);
			testCtlsForm->comEx1->SetComboBoxStyle(ComboBoxStyle::DropDownList);
		}
		else
		{
			testCtlsForm->com1->SetComboBoxStyle(ComboBoxStyle::DropDown);
			testCtlsForm->comEx1->SetComboBoxStyle(ComboBoxStyle::DropDown);
		}
	}
	else
	{
	}

	//BSTR bstrCode = ::SysAllocString(L"text<br>text2<br>");
	//ie1->Write(bstrCode);
	//::SysFreeString(bstrCode);
	//ie1->LoadText(_T("abc<br><a href='adc'>aaa</a>"));

	//TCHAR* tc = ie1->SaveText();
	//edit1->SetText(tc);
	//int len = (int)_tcslen(tc) * sizeof(TCHAR);
	//cc::Util::fWrite(tc, len, LangCode::SYSTEM, _T("c:\\tmp.txt"));
	//free(tc);

	//void CC_APP_GetDrawActor()->DrawIcon(HDC hdc, int xLeft, int yTop, HICON hIcon, int cxWidth, int cyWidth, HBRUSH hBrush = NULL, UINT uFlags = CC_DI_NORMAL);
	HICON icon = imageList->GetIcon(9999);
	HDC hDC = ::GetDC(m_hWnd);
	CC_APP_GetDrawActor()->DrawIcon(hDC, 180, 60, icon, 16, 16, NULL, DI_NORMAL);
	icon = CC_APP_GetDrawActor()->CreateGloomIcon(icon);
	CC_APP_GetDrawActor()->DrawIcon(hDC, 220, 60, icon, 32, 32, NULL, DI_NORMAL);

	//BSTR bstrCode = ::SysAllocString(L"alert(\"sss\");document.write(\"dddddd\");");
	//ie1->ExecScript(bstrCode);
	//::SysFreeString(bstrCode);
	icon = imageList->GetIcon(9999, IconType::Hot);
	DrawIconEx(hDC, 260, 60, icon, 16, 16, 0, NULL, DI_NORMAL);
	icon = imageList->GetIcon(9999, IconType::Gloom);
	DrawIconEx(hDC, 260, 80, icon, 32, 32, 1, NULL, DI_NORMAL);
	icon = imageList->GetIcon(9999, IconType::Disabled);
	DrawIconEx(hDC, 300, 60, icon, 48, 48, 0, NULL, DI_NORMAL);
	::ReleaseDC(m_hWnd, hDC);

	cc::Log::debug(_T("Control_Click E"));
}

void MainForm::btnControlSam_Click(Object* sender, EventArgs* e)
{
	cc::Log::debug(_T("btnControlSam_Click S"));
	ControlSam dialog;
	dialog.SetParent(this, true);
	dialog.SetCenter();
	dialog.ShowDialog();
	dialog.Dispose();
	cc::Log::debug(_T("btnControlSam_Click E"));
}

void MainForm::btnListBoxSam_Click(Object* sender, EventArgs* e)
{
	cc::Log::debug(_T("btnListBoxSam_Click S"));
	ListBoxSam dialog;
	dialog.SetParent(this, true);
	dialog.SetCenter();
	dialog.ShowDialog();
	dialog.Dispose();
	cc::Log::debug(_T("btnListBoxSam_Click E"));
}

void MainForm::btnTabSam_Click(Object* sender, EventArgs* e)
{
	if(tabDialog != NULL)
	{
		tabDialog->Dispose();
		delete tabDialog;
	}
	tabDialog = new TabSam();
	tabDialog->SetParent(this, true);
	//tabDialog->SetCenter();
	tabDialog->SetLocation(466, 60);
	tabDialog->Show();
}

void MainForm::btnTabExSam_Click(Object* sender, EventArgs* e)
{
	cc::Log::debug(_T("btnTabExSam_Click S"));
	if(tabExDialog != NULL)
	{
		tabExDialog->Dispose();
		delete tabExDialog;
	}
	tabExDialog = new TabExSam();
	tabExDialog->SetParent(this, true);
	//tabExDialog->SetCenter();
	tabExDialog->SetLocation(466, 360);
	tabExDialog->Show();
	cc::Log::debug(_T("btnTabExSam_Click E"));
}

void MainForm::menuShowToolBar_Click(Object* sender, EventArgs* e)
{
	cc::Log::debug(_T("menuShowToolBar_Click S"));
	if(_lstToolBars != NULL)
	{
		for(int n = _lstToolBars->GetSize() - 1; n >= 0; n--)
		{
			ToolBar* ctl = (ToolBar*)_lstToolBars->GetKey(n);
			if(ctl->GetFloating())
			{
				ctl->SetFloating(true, true);
			}
		}
	}
	cc::Log::debug(_T("menuShowToolBar_Click E"));
}

void MainForm::exit_Click(Object* sender, EventArgs* e)
{
	cc::Log::debug(_T("exit_Click"));
	App::Exit();
}

void MainForm::menuitem_Click(Object* sender, EventArgs* e)
{
	cc::Log::debug(_T("menuitem_Click, also traceConsole with red text."));
	cc::Log::traceConsole(RGB(255, 0, 0), _T("menuitem_Click"));
}
void MainForm::menu1_Click(Object* sender, EventArgs* e)
{
	MenuItem* menu = (MenuItem*)sender;
	cc::Log::debug(_T("menu1_Click:%s"), menu->GetText());
}
void MainForm::menu1_DrawItem(Object* sender, DrawItemEventArgs* e)
{
	MenuItem* menu = (MenuItem*)sender;
	cc::Log::debug(_T("menu1_DrawItem:%s, also traceConsole with color text."), menu->GetText());
	cc::Log::traceConsole(RGB(255, 0, 255), _T("menu1_DrawItem:%s"), menu->GetText());
	CC_APP_GetDrawActor()->DrawMenu((MenuItem*)sender, e);
}
void MainForm::menu1_MeasureItem(Object* sender, MeasureItemEventArgs* e)
{
	MenuItem* menu = (MenuItem*)sender;
	cc::Log::debug(_T("menu1_MeasureItem:%s"), menu->GetText());
	CC_APP_GetDrawActor()->MeasureMenu((MenuItem*)sender, e);
}
void MainForm::menu1_Popup(Object* sender, EventArgs* e)
{
	MenuItem* menu = (MenuItem*)sender;
	cc::Log::warn(_T("menu1_Popup:%s, warn for color text"), menu->GetText());
}
void MainForm::menu1_Select(Object* sender, EventArgs* e)
{
	MenuItem* menu = (MenuItem*)sender;
	cc::Log::error(_T("menu1_Select:%s, error for color text"), menu->GetText());
}

void MainForm::Notify_Click(Object* sender, MouseEventArgs* e)
{
	cc::Log::debug(_T("Notify_Click"));
}
void MainForm::Notify_DoubleClick(Object* sender, MouseEventArgs* e)
{
	cc::Log::debug(_T("Notify_DoubleClick"));
	if(GetVisible())
	{
		//first time go here
		notify2->PlayStart();
	}
	else
	{
		notify2->PlayStop();
	}
	SetVisible(!GetVisible());
}
void MainForm::Notify_MouseMove(Object* sender, MouseEventArgs* e)
{
	cc::Log::traceConsole(RGB(0, 0, 255), _T("Notify_MouseMove"));
}
void MainForm::Notify_MouseUp(Object* sender, MouseEventArgs* e)
{
	cc::Log::debug(_T("Notify_MouseUp"));
}
void MainForm::Notify_MouseDown(Object* sender, MouseEventArgs* e)
{
	cc::Log::debug(_T("Notify_MouseDown"));
}

void MainForm::toolbutton1_Click(Object* sender, EventArgs* e)
{
	cc::Log::debug(_T("toolbutton1_Click S"));
	cc::Log::debug(_T("toolbutton1_Click E"));
}

void MainForm::toolbutton1_Popup(Object* sender, EventArgs* e)
{
	cc::Log::debug(_T("toolbutton1_Popup S"));
	cc::Log::debug(_T("toolbutton1_Popup E"));
}

void MainForm::toolbutton3_Click(Object* sender, EventArgs* e)
{
	cc::Log::debug(_T("toolbutton3_Click S"));
	cc::Log::debug(_T("toolbutton3_Click E"));
}

void MainForm::toolOnTop_Click(Object* sender, EventArgs* e)
{
	bool chk = toolOnTop->GetPushed();
	::SetWindowPos(m_hWnd, (chk) ? HWND_TOPMOST : HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}

void MainForm::toolSetBack_Click(Object* sender, EventArgs* e)
{
	ConfigBackDlg cfgBack;
	cfgBack.SetParent(this, true);
	cfgBack.SetResultControl(this->GetClient(), 0);
	cfgBack.SetCenter();
	//AddInnerImage(UINT nResourceID, UINT nResourceType, bool isStretch, const TCHAR* info);
	cfgBack.AddInnerImage(IDI_BMP_BACK16, RT_BITMAP, true, _T("back cat"));
	cfgBack.AddInnerImage(IDI_ICON_SETBACK, RT_ICON, false, _T("icon"));
	if(cfgBack.ShowDialog() == DialogResult::OK)
	{
	}
}

void MainForm::setTransparent_Click(Object* sender, EventArgs* e)
{
	cc::Log::debug(_T("setontop_Click S"));
	mainMenu_2_6->SetChecked(!mainMenu_2_6->GetChecked());
	if(mainMenu_2_6->GetChecked())
	{
		cc::Util::SetTransparent(m_hWnd, 150);
	}
	else
	{
		cc::Util::SetTransparent(m_hWnd, 250);
	}
	cc::Log::debug(_T("setontop_Click E"));
}

void MainForm::setclosebutton_Click(Object* sender, EventArgs* e)
{
	cc::Log::debug(_T("setclosebutton_Click S"));
	mainMenu_2_7->SetChecked(!mainMenu_2_7->GetChecked());
	//toolbar1->SetCloseButtonEnable(!toolbar1->GetCloseButtonEnable());
	this->SetCloseButtonEnable(!this->GetCloseButtonEnable());
	cc::Log::debug(_T("setclosebutton_Click E"));
}

#ifndef LOCALE_SNAME
#define LOCALE_SNAME                0x005C
#endif
#define LOCALE_NAME_MAX_LENGTH 85
void MainForm::settoolbarfloat_Click(Object* sender, EventArgs* e)
{
	cc::Log::debug(_T("settoolbarfloat_Click S"));
	mainMenu_2_8->SetChecked(!mainMenu_2_8->GetChecked());

	TCHAR szLocalCodePage[250 + 1];
	//932
	int nLocaleChars = ::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_IDEFAULTANSICODEPAGE, szLocalCodePage, 250);
	cc::Log::debug(_T("LOCALE_IDEFAULTANSICODEPAGE:%s, nLocaleChars:%d"), szLocalCodePage, nLocaleChars);

	//JP
	nLocaleChars = ::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SISO3166CTRYNAME, szLocalCodePage, 250);
	cc::Log::debug(_T("LOCALE_SISO3166CTRYNAME:%s, nLocaleChars:%d"), szLocalCodePage, nLocaleChars);

	//ja
	nLocaleChars = ::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SISO639LANGNAME, szLocalCodePage, 250);
	cc::Log::debug(_T("LOCALE_SISO639LANGNAME:%s, nLocaleChars:%d"), szLocalCodePage, nLocaleChars);

	//0411
	//Returns the language identifier for the current user locale.
	nLocaleChars = ::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_ILANGUAGE, szLocalCodePage, 250);
	cc::Log::debug(_T("LOCALE_ILANGUAGE:%s, nLocaleChars:%d"), szLocalCodePage, nLocaleChars);

	cc::Log::debug(_T("settoolbarfloat_Click E"));
}

void MainForm::ChangeMenuItem_Click(Object* sender, EventArgs* e)
{
	cc::Log::debug(_T("ChangeMenuItem_Click S"));
	mainMenu_2_9->SetChecked(!mainMenu_2_9->GetChecked());
	if(mainMenu_2_9->GetChecked())
	{
		mainMenu_2_3->SetText(_T("mainMenu_2_3 new value(&P)"));
		mainMenu_2_1->SetEnabled(true);
	}
	else
	{
		mainMenu_2_3->SetText(_T("-"));
		mainMenu_2_1->SetEnabled(false);
	}
	cc::Log::debug(_T("ChangeMenuItem_Click E"));
}

void MainForm::ChangeStatus_Click(Object* sender, EventArgs* e)
{
	cc::Log::debug(_T("ChangeStatus_Click S"));
	mainMenu_2_10->SetChecked(!mainMenu_2_10->GetChecked());
	if(mainMenu_2_10->GetChecked())
	{
		statusitem1->SetText(_T("123456"));
		statusitem2->SetIcon(imageList, IDI_ICON2);
		statusitem3->SetEnabled(true);
	}
	else
	{
		statusitem1->SetText(_T("this is a long text."));
		statusitem2->SetIcon(NULL, -1);
		statusitem3->SetEnabled(false);
	}
	cc::Log::debug(_T("ChangeStatus_Click E"));
}
