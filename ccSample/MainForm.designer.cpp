#include "MainForm.h"
using namespace cc;
using namespace cc::win;

void MainForm::InitializeComponent()
{
	imageList = new ImageList();
	imageList->SetIconSize(-1, -1);
	imageList->SetIcon(IDI_ICON_MAIN, IDI_ICON_MAIN);
	imageList->SetIcon(IDI_ICON_COPY, IDI_ICON_COPY);
	imageList->SetIcon(IDI_ICON_CUT, IDI_ICON_CUT);
	imageList->SetIcon(IDI_ICON_PASTE, IDI_ICON_PASTE);
	imageList->SetIcon(IDI_ICON_ATTACH, IDI_ICON_ATTACH);
	imageList->SetIcon(IDI_ICON_MOVE1, IDI_ICON_MOVE1);
	imageList->SetIcon(IDI_ICON_MOVE2, IDI_ICON_MOVE2);
	imageList->SetIcon(IDI_ICON_EXIT, IDI_ICON_EXIT);
	imageList->SetIcon(IDI_ICON_LOCK, IDI_ICON_LOCK);
	imageList->SetIcon(IDI_ICON_SAVE, IDI_ICON_SAVE);
	imageList->SetIcon(IDI_ICON_FONT, IDI_ICON_FONT);
	imageList->SetIcon(IDI_ICON_REFRESH, IDI_ICON_REFRESH);
	imageList->SetIcon(IDI_ICON_PRINT, IDI_ICON_PRINT);
	imageList->SetIcon(IDI_ICON_NEW, IDI_ICON_NEW);
	imageList->SetIcon(IDI_ICON_NEWWIN, IDI_ICON_NEWWIN);
	imageList->SetIcon(IDI_ICON_COLOR, IDI_ICON_COLOR);
	imageList->SetIcon(IDI_ICON_NOTE, IDI_ICON_NOTE);
	imageList->SetIcon(IDI_ICON5, IDI_ICON5);
	imageList->SetIcon(IDI_ICON2, IDI_ICON2);
	imageList->SetIcon(IDI_ICON3, IDI_ICON3);
	imageList->SetIcon(IDI_ICON4, IDI_ICON4);
	imageList->SetIcon(IDI_ICON_RED16, IDI_ICON_RED16);
	imageList->SetIcon(IDI_ICON_BLUE32, IDI_ICON_BLUE32);
	imageList->SetIcon(IDI_ICON_R16B32, IDI_ICON_R16B32);
	imageList->SetIcon(IDI_ICON_TESTH, IDI_ICON_TESTH);
	imageList->SetIcon(IDI_ICON_TESTG, IDI_ICON_TESTG);
	imageList->SetIcon(IDI_ICON_TESTD, IDI_ICON_TESTD);
	imageList->SetIcon(9999, IDI_ICON_TESTH, IconType::Hot);
	imageList->SetIcon(9999, IDI_ICON_TESTG, IconType::Gloom);
	imageList->SetIcon(9999, IDI_ICON_TESTD, IconType::Disabled);
	imageList->SetIcon(9999, IDI_ICON_RED16, IconType::Pushed);
	imageList->SetIcon(IDI_ICON_BTN_HOT, IDI_ICON_BTN_HOT, IconType::Hot);
	imageList->SetIcon(IDI_ICON_BTN_HOT, IDI_ICON_BTN_DIS, IconType::Disabled);
	imageList->SetIcon(IDI_ICON_BTNAERO_HOT, IDI_ICON_BTNAERO_HOT, IconType::Hot);
	imageList->SetIcon(IDI_ICON_BTNAERO_HOT, IDI_ICON_BTNAERO_GLOOM, IconType::Gloom);
	imageList->SetIcon(IDI_ICON_BTNAERO_HOT, IDI_ICON_BTNAERO_PUSHED, IconType::Pushed);
	imageList->SetIcon(IDI_ICON_SETBACK, IDI_ICON_SETBACK);

	menu1_1 = new MenuItem();
	menu1_1->SetText(_T("exit(&E)"));
	menu1_1->SetTips(_T("tips of menuitem3"), imageList, IDI_ICON_EXIT);
	menu1_1->SetIcon(imageList, IDI_ICON_EXIT);
	menu1_1->Click += EventHandler((Object*)this, (EventFun)&MainForm::exit_Click);

	menu1_2 = new MenuItem();
	menu1_2->SetText(_T("menu1_2"));
	menu1_2->SetTips(_T("tips of menu1_2"));
	menu1_2->Click += EventHandler((Object*)this, (EventFun)&MainForm::menuitem_Click);

	menu1_2_1 = new MenuItem();
	menu1_2_1->SetText(_T("menu1_2_1"));
	menu1_2_1->SetTips(_T("tips of menu1_2_1"));
	menu1_2_1->Click += EventHandler((Object*)this, (EventFun)&MainForm::menuitem_Click);
	menu1_2_2 = new MenuItem();
	menu1_2_2->SetText(_T("menu1_2_2"));
	menu1_2_2->SetTips(_T("tips of menu1_2_2"));
	menu1_2_2->Click += EventHandler((Object*)this, (EventFun)&MainForm::menuitem_Click);
	menu1_2->ItemAdd(menu1_2_1);
	menu1_2->ItemAdd(menu1_2_2);

	menu1_3 = new MenuItem();
	menu1_3->SetText(_T("menu1_3 5event(&1)"));
	menu1_3->SetTips(_T("tips of menu1_3"));
	menu1_3->DrawItem = DrawItemEventHandler((Object*)this, (DrawItemEventFun)&MainForm::menu1_DrawItem);
	menu1_3->MeasureItem = MeasureItemEventHandler((Object*)this, (MeasureItemEventFun)&MainForm::menu1_MeasureItem);
	menu1_3->Select += EventHandler((Object*)this, (EventFun)&MainForm::menu1_Select);
	menu1_3->Popup += EventHandler((Object*)this, (EventFun)&MainForm::menu1_Popup);
	menu1_3->Click += EventHandler((Object*)this, (EventFun)&MainForm::menu1_Click);

	menu1_3_1 = new MenuItem();
	menu1_3_1->SetText(_T("menu1_3_1 5event(sub)(&2)"));
	menu1_3_1->SetTips(_T("tips of menu1_3_1"));
	menu1_3_1->DrawItem = DrawItemEventHandler((Object*)this, (DrawItemEventFun)&MainForm::menu1_DrawItem);
	menu1_3_1->MeasureItem = MeasureItemEventHandler((Object*)this, (MeasureItemEventFun)&MainForm::menu1_MeasureItem);
	menu1_3_1->Select += EventHandler((Object*)this, (EventFun)&MainForm::menu1_Select);
	menu1_3_1->Popup += EventHandler((Object*)this, (EventFun)&MainForm::menu1_Popup);
	menu1_3_1->Click += EventHandler((Object*)this, (EventFun)&MainForm::menu1_Click);
	menu1_3->ItemAdd(menu1_3_1);

	contextMenu1 = new ContextMenu();
	contextMenu1->ItemAdd(menu1_1);
	contextMenu1->ItemAdd(menu1_2);
	contextMenu1->ItemAdd(menu1_3);
	contextMenu1->SetAllOwnerDraw(true);
	this->DisposeItemAdd(contextMenu1);

	menuFClientBack = new MenuItem();
	menuFClientBack->SetText(_T("Config FormClientBack"));
	menuFClientBack->SetTips(_T("set FormClient to different Back."), imageList, IDI_ICON_REFRESH);
	menuFClientBack->Click += EventHandler((Object*)this, (EventFun)&MainForm::Control_Click);

	menuFClientBack1 = new MenuItem();
	menuFClientBack1->SetText(_T("WindowBack"));
	menuFClientBack1->Click += EventHandler((Object*)this, (EventFun)&MainForm::Control_Click);
	menuFClientBack->ItemAdd(menuFClientBack1);
	menuFClientBack2 = new MenuItem();
	menuFClientBack2->SetText(_T("ThemeBack"));
	menuFClientBack2->Click += EventHandler((Object*)this, (EventFun)&MainForm::Control_Click);
	menuFClientBack->ItemAdd(menuFClientBack2);
	menuFClientBack3 = new MenuItem();
	menuFClientBack3->SetText(_T("Inner Image Back"));
	menuFClientBack3->Click += EventHandler((Object*)this, (EventFun)&MainForm::Control_Click);
	menuFClientBack->ItemAdd(menuFClientBack3);
	menuFClientBack4 = new MenuItem();
	menuFClientBack4->SetText(_T("Image File Back"));
	menuFClientBack4->SetTips(_T("select icon or image file for back of form's client ."), imageList, IDI_ICON_REFRESH);
	menuFClientBack4->Click += EventHandler((Object*)this, (EventFun)&MainForm::Control_Click);
	menuFClientBack->ItemAdd(menuFClientBack4);
	menuFClientBack5 = new MenuItem();
	menuFClientBack5->SetText(_T("OutBrush(Hatch)"));
	menuFClientBack5->SetTips(_T("create brush and set to form's client."), imageList, IDI_ICON_REFRESH);
	menuFClientBack5->Click += EventHandler((Object*)this, (EventFun)&MainForm::Control_Click);
	menuFClientBack->ItemAdd(menuFClientBack5);
	menuBreak = new MenuItem();
	menuBreak->SetText(_T("-"));
	menuFClientBack->ItemAdd(menuBreak);
	menuFClientBack6 = new MenuItem();
	menuFClientBack6->SetText(_T("Gradient for Form's Client"));
	menuFClientBack6->SetTips(_T("select \"From color\",then Gradient to Back Color(set at WindowBack).\r\nif set Gradient, will ignore Backgrond of about."), imageList, IDI_ICON_REFRESH);
	menuFClientBack6->Click += EventHandler((Object*)this, (EventFun)&MainForm::Control_Click);
	menuFClientBack->ItemAdd(menuFClientBack6);

	menu2_2 = new MenuItem();
	menu2_2->SetText(_T("menu2_2"));
	menu2_2->SetTips(_T("tips of menu2_2"));
	menu2_2->Click += EventHandler((Object*)this, (EventFun)&MainForm::Control_Click);

	menuToolEdge = new MenuItem();
	menuToolEdge->SetText(_T("menuToolEdge"));
	menuToolEdge->SetTips(_T("Add Edge to some ToolItem"));
	menuToolEdge->Click += EventHandler((Object*)this, (EventFun)&MainForm::Control_Click);
	menu2_2->ItemAdd(menuToolEdge);
	menuPictureBack1 = new MenuItem();
	menuPictureBack1->SetText(_T("File for Picture Back"));
	menuPictureBack1->SetTips(_T("can select ico or image file. but default TransparentBackColor is RGB(0, 0, 0)."));
	menuPictureBack1->Click += EventHandler((Object*)this, (EventFun)&MainForm::Control_Click);
	menu2_2->ItemAdd(menuPictureBack1);
	menuPictureBack2 = new MenuItem();
	menuPictureBack2->SetText(_T("Inner Image for Picture Back"));
	menuPictureBack2->SetTips(_T("default TransparentBackColor is RGB(0, 0, 0)."));
	menuPictureBack2->Click += EventHandler((Object*)this, (EventFun)&MainForm::Control_Click);
	menu2_2->ItemAdd(menuPictureBack2);
#ifdef _DEBUG_
	menuTestMsgDlg = new MenuItem();
	menuTestMsgDlg->SetText(_T("Test MsgDlg"));
	menuTestMsgDlg->SetTips(_T("cc::win::MsgDlg::test(this)."));
	menuTestMsgDlg->Click += EventHandler((Object*)this, (EventFun)&MainForm::Control_Click);
	menu2_2->ItemAdd(menuTestMsgDlg);
	menuTestAboutDlg = new MenuItem();
	menuTestAboutDlg->SetText(_T("Test AboutDlg"));
	menuTestAboutDlg->SetTips(_T("cc::win::AboutDlg::test(this)."));
	menuTestAboutDlg->Click += EventHandler((Object*)this, (EventFun)&MainForm::Control_Click);
	menu2_2->ItemAdd(menuTestAboutDlg);
#endif
	menuGroupBreak = new MenuItem();
	menuGroupBreak->SetText(_T("Group's Line Title"));
	menuGroupBreak->SetTips(_T("change to Line for Groups's Title."));
	menuGroupBreak->Click += EventHandler((Object*)this, (EventFun)&MainForm::Control_Click);
	menu2_2->ItemAdd(menuGroupBreak);
	menuGroupGradient = new MenuItem();
	menuGroupGradient->SetText(_T("Gradient for Groups"));
	menuGroupGradient->SetTips(_T("add Gradient for Groups."));
	menuGroupGradient->Click += EventHandler((Object*)this, (EventFun)&MainForm::Control_Click);
	menu2_2->ItemAdd(menuGroupGradient);

	coneMenuConfig = new ContextMenu();
	coneMenuConfig->ItemAdd(menuFClientBack);
	coneMenuConfig->ItemAdd(menu2_2);
	this->DisposeItemAdd(coneMenuConfig);

	menu3_1 = new MenuItem();
	menu3_1->SetText(_T("form context menu1"));
	menu3_1->Click += EventHandler((Object*)this, (EventFun)&MainForm::menuitem_Click);

	menu3_2 = new MenuItem();
	menu3_2->SetText(_T("form context menu2"));
	menu3_2->Click += EventHandler((Object*)this, (EventFun)&MainForm::menuitem_Click);

	contextMenu3 = new ContextMenu();
	contextMenu3->ItemAdd(menu3_1);
	contextMenu3->ItemAdd(menu3_2);
	this->DisposeItemAdd(contextMenu3);

	menu4_1 = new MenuItem();
	menu4_1->SetText(_T("form context menu4"));
	menu4_1->Click += EventHandler((Object*)this, (EventFun)&MainForm::menuitem_Click);

	menu4_2 = new MenuItem();
	menu4_2->SetText(_T("form context menu4"));
	menu4_2->Click += EventHandler((Object*)this, (EventFun)&MainForm::menuitem_Click);

	contextMenu4 = new ContextMenu();
	contextMenu4->ItemAdd(menu4_1);
	contextMenu4->ItemAdd(menu4_2);
	this->DisposeItemAdd(contextMenu4);

	//main menu
	mainMenu_1 = new MenuItem();
	mainMenu_1->SetText(_T("bar1 Event5(&Q)"));
	mainMenu_1->SetTips(_T("tips of mainmenu1"));
	mainMenu_1->SetOwnerDraw(true);
	mainMenu_1->DrawItem = DrawItemEventHandler((Object*)this, (DrawItemEventFun)&MainForm::menu1_DrawItem);
	mainMenu_1->MeasureItem = MeasureItemEventHandler((Object*)this, (MeasureItemEventFun)&MainForm::menu1_MeasureItem);
	mainMenu_1->Popup += EventHandler((Object*)this, (EventFun)&MainForm::menu1_Popup);
	mainMenu_1->Select += EventHandler((Object*)this, (EventFun)&MainForm::menu1_Select);
	mainMenu_1->Click += EventHandler((Object*)this, (EventFun)&MainForm::menu1_Click);

	mainMenu_1_1 = new MenuItem();
	mainMenu_1_1->SetIcon(imageList, IDI_ICON_REFRESH);
	mainMenu_1_1->SetText(_T("mainMenu_1_1 Event5(&W)"));
	mainMenu_1_1->SetTips(_T("tips of mainmenu11"), imageList, IDI_ICON_REFRESH);
	mainMenu_1_1->SetOwnerDraw(true);
	mainMenu_1_1->DrawItem = DrawItemEventHandler((Object*)this, (DrawItemEventFun)&MainForm::menu1_DrawItem);
	mainMenu_1_1->MeasureItem = MeasureItemEventHandler((Object*)this, (MeasureItemEventFun)&MainForm::menu1_MeasureItem);
	mainMenu_1_1->Popup += EventHandler((Object*)this, (EventFun)&MainForm::menu1_Popup);
	mainMenu_1_1->Select += EventHandler((Object*)this, (EventFun)&MainForm::menu1_Select);
	mainMenu_1_1->Click += EventHandler((Object*)this, (EventFun)&MainForm::menu1_Click);

	mainMenu_1_2 = new MenuItem();
	mainMenu_1_2->SetIcon(imageList, IDI_ICON_FONT);
	mainMenu_1_2->SetText(_T("mainMenu_1_2 Event5(&M)"));
	mainMenu_1_2->SetTips(_T("tips of mainmenu11"), imageList, IDI_ICON_FONT);
	mainMenu_1_2->SetOwnerDraw(true);
	mainMenu_1_2->DrawItem = DrawItemEventHandler((Object*)this, (DrawItemEventFun)&MainForm::menu1_DrawItem);
	mainMenu_1_2->MeasureItem = MeasureItemEventHandler((Object*)this, (MeasureItemEventFun)&MainForm::menu1_MeasureItem);
	mainMenu_1_2->Popup += EventHandler((Object*)this, (EventFun)&MainForm::menu1_Popup);
	mainMenu_1_2->Select += EventHandler((Object*)this, (EventFun)&MainForm::menu1_Select);
	mainMenu_1_2->Click += EventHandler((Object*)this, (EventFun)&MainForm::menu1_Click);

	mainMenu_1_2_1 = new MenuItem();
	mainMenu_1_2_1->SetIcon(imageList, 9999);
	mainMenu_1_2_1->SetText(_T("mainMenu_1_2_1 Event5(&S)"));
	mainMenu_1_2_1->SetTips(_T("tips of mainmenu11"), imageList, 9999);
	mainMenu_1_2_1->SetOwnerDraw(true);
	mainMenu_1_2_1->DrawItem = DrawItemEventHandler((Object*)this, (DrawItemEventFun)&MainForm::menu1_DrawItem);
	mainMenu_1_2_1->MeasureItem = MeasureItemEventHandler((Object*)this, (MeasureItemEventFun)&MainForm::menu1_MeasureItem);
	mainMenu_1_2_1->Popup += EventHandler((Object*)this, (EventFun)&MainForm::menu1_Popup);
	mainMenu_1_2_1->Select += EventHandler((Object*)this, (EventFun)&MainForm::menu1_Select);
	mainMenu_1_2_1->Click += EventHandler((Object*)this, (EventFun)&MainForm::menu1_Click);

	mainMenu_1_2->ItemAdd(mainMenu_1_2_1);

	mainMenu_1->ItemAdd(mainMenu_1_1);
	mainMenu_1->ItemAdd(mainMenu_1_2);

	mainMenu_2 = new MenuItem();
	mainMenu_2->SetText(_T("bar2(&E)"));
	mainMenu_2->SetTips(_T("tips of mainmenu2"), imageList, IDI_ICON5);
	mainMenu_2->Click += EventHandler((Object*)this, (EventFun)&MainForm::menuitem_Click);
	mainMenu_2->SetIcon(imageList, IDI_ICON5);

	mainMenu_2_1 = new MenuItem();
	mainMenu_2_1->SetText(_T("mainMenu_2_1(&D) no"));
	mainMenu_2_1->SetTips(_T("tips of mainmenu21"), imageList, IDI_ICON4);
	mainMenu_2_1->SetShortcut(Shortcut::Ctrl2);
	mainMenu_2_1->SetIcon(imageList, IDI_ICON4);
	//mainMenu_2_1->SetIcon(imageList->GetIcon(IDI_ICON2), IconType::Hot);
	mainMenu_2_1->SetEnabled(false);

	mainMenu_2_2 = new MenuItem();
	mainMenu_2_2->SetText(_T("mainMenu_2_2(&E)"));
	mainMenu_2_2->Click += EventHandler((Object*)this, (EventFun)&MainForm::menuitem_Click);
	mainMenu_2_2->SetShortcut(Shortcut::Ctrl6);
	mainMenu_2_2->SetIcon(imageList, IDI_ICON2);

	mainMenu_2_3 = new MenuItem();
	mainMenu_2_3->SetText(_T("-"));

	mainMenu_2_4 = new MenuItem();
	mainMenu_2_4->SetText(_T("mainMenu_2_4 (&F) click"));
	mainMenu_2_4->SetTips(_T("tips of mainmenu24"));
	mainMenu_2_4->SetChecked(true);
	mainMenu_2_4->Click += EventHandler((Object*)this, (EventFun)&MainForm::menuitem_Click);
	mainMenu_2_4->SetShortcut(Shortcut::CtrlShiftN);

	mainMenu_2_4_1 = new MenuItem();
	mainMenu_2_4_1->SetText(_T("mainMenu_2_4_1(&R)"));
	mainMenu_2_4_1->SetTips(_T("tips of mainmenu24_1"));
	mainMenu_2_4_1->SetChecked(true);
	mainMenu_2_4_1->Click += EventHandler((Object*)this, (EventFun)&MainForm::menuitem_Click);

	mainMenu_2_4_1_1 = new MenuItem();
	mainMenu_2_4_1_1->SetText(_T("mainmenu24_1_1(&F)"));
	mainMenu_2_4_1_1->SetTips(_T("tips of mainmenu24_1_1"));
	mainMenu_2_4_1_1->SetChecked(true);
	mainMenu_2_4_1_1->Click += EventHandler((Object*)this, (EventFun)&MainForm::menuitem_Click);
	mainMenu_2_4_1_1->SetOwnerDraw(true);

	mainMenu_2_4_1->ItemAdd(mainMenu_2_4_1_1);

	mainMenu_2_4_2 = new MenuItem();
	mainMenu_2_4_2->SetText(_T("mainmenu24_2,clk(&F)"));
	mainMenu_2_4_2->SetTips(_T("tips of mainmenu24_2"));
	mainMenu_2_4_2->SetChecked(true);
	mainMenu_2_4_2->Click += EventHandler((Object*)this, (EventFun)&MainForm::menuitem_Click);
	mainMenu_2_4_2->SetShortcut(Shortcut::Ctrl1);

	mainMenu_2_4->ItemAdd(mainMenu_2_4_1);
	mainMenu_2_4->ItemAdd(mainMenu_2_4_2);

	mainMenu_2_5 = new MenuItem();
	mainMenu_2_5->SetText(_T("-"));

	mainMenu_2_6 = new MenuItem();
	mainMenu_2_6->SetText(_T("SetTransparent 150/250"));
	mainMenu_2_6->Click += EventHandler((Object*)this, (EventFun)&MainForm::setTransparent_Click);

	mainMenu_2_7 = new MenuItem();
	mainMenu_2_7->SetText(_T("SetCloseButton On/Off"));
	mainMenu_2_7->Click += EventHandler((Object*)this, (EventFun)&MainForm::setclosebutton_Click);

	mainMenu_2_8 = new MenuItem();
	mainMenu_2_8->SetText(_T("SetToolBar Float On/Off"));
	mainMenu_2_8->Click += EventHandler((Object*)this, (EventFun)&MainForm::settoolbarfloat_Click);

	mainMenu_2_9 = new MenuItem();
	mainMenu_2_9->SetText(_T("change menuitem"));
	mainMenu_2_9->Click += EventHandler((Object*)this, (EventFun)&MainForm::ChangeMenuItem_Click);

	mainMenu_2_10 = new MenuItem();
	mainMenu_2_10->SetText(_T("change status"));
	mainMenu_2_10->Click += EventHandler((Object*)this, (EventFun)&MainForm::ChangeStatus_Click);

	mainMenu_2->ItemAdd(mainMenu_2_1);
	mainMenu_2->ItemAdd(mainMenu_2_2);
	mainMenu_2->ItemAdd(mainMenu_2_3);
	mainMenu_2->ItemAdd(mainMenu_2_4);
	mainMenu_2->ItemAdd(mainMenu_2_5);
	mainMenu_2->ItemAdd(mainMenu_2_6);
	mainMenu_2->ItemAdd(mainMenu_2_7);
	mainMenu_2->ItemAdd(mainMenu_2_8);
	mainMenu_2->ItemAdd(mainMenu_2_9);
	mainMenu_2->ItemAdd(mainMenu_2_10);

	mainMenu_3 = new MenuItem();
	mainMenu_3->SetText(_T("others test(&t)"));
	mainMenu_3->SetTips(_T("tips of mainmenu3"));

	mainMenu_3_0 = new MenuItem();
	mainMenu_3_0->SetIcon(imageList, IDI_ICON_LOCK);
	mainMenu_3_0->SetText(_T("About"));
	mainMenu_3_0->SetTips(_T("tips of mainmenu31"), imageList, IDI_ICON_LOCK);
	mainMenu_3_0->Click += EventHandler((Object*)this, (EventFun)&MainForm::Control_Click);
	mainMenu_3_1 = new MenuItem();
	mainMenu_3_1->SetIcon(imageList, IDI_ICON_LOCK);
	mainMenu_3_1->SetText(_T("Show MainFormple"));
	mainMenu_3_1->SetTips(_T("tips of mainmenu31"), imageList, IDI_ICON_LOCK);
	mainMenu_3_1->Click += EventHandler((Object*)this, (EventFun)&MainForm::btnControlSam_Click);
	mainMenu_3_2 = new MenuItem();
	mainMenu_3_2->SetIcon(imageList, IDI_ICON_SAVE);
	mainMenu_3_2->SetText(_T("Show ListBoxSample"));
	mainMenu_3_2->SetTips(_T("tips of mainmenu31"), imageList, IDI_ICON_SAVE);
	mainMenu_3_2->Click += EventHandler((Object*)this, (EventFun)&MainForm::btnListBoxSam_Click);
	mainMenu_3_3 = new MenuItem();
	mainMenu_3_3->SetIcon(imageList, IDI_ICON_REFRESH);
	mainMenu_3_3->SetText(_T("Show TabSample(Modeless)"));
	mainMenu_3_3->SetTips(_T("tips of mainmenu31"));
	mainMenu_3_3->Click += EventHandler((Object*)this, (EventFun)&MainForm::btnTabSam_Click);
	mainMenu_3_4 = new MenuItem();
	mainMenu_3_4->SetIcon(imageList, IDI_ICON_PRINT);
	mainMenu_3_4->SetText(_T("Show TabExSample(Modeless)"));
	mainMenu_3_4->SetTips(_T("tips of mainmenu31"));
	mainMenu_3_4->Click += EventHandler((Object*)this, (EventFun)&MainForm::btnTabExSam_Click);
	mainMenu_3_5 = new MenuItem();
	mainMenu_3_5->SetIcon(imageList, IDI_ICON_PRINT);
	mainMenu_3_5->SetText(_T("Show ToolBar"));
	mainMenu_3_5->SetTips(_T("if toolbar is closed,show it"));
	mainMenu_3_5->Click += EventHandler((Object*)this, (EventFun)&MainForm::menuShowToolBar_Click);
	mainMenu_3->ItemAdd(mainMenu_3_0);
	mainMenu_3->ItemAdd(mainMenu_3_1);
	mainMenu_3->ItemAdd(mainMenu_3_2);
	mainMenu_3->ItemAdd(mainMenu_3_3);
	mainMenu_3->ItemAdd(mainMenu_3_4);
	mainMenu_3->ItemAdd(mainMenu_3_5);

	mainMenu1 = new MainMenu();
	mainMenu1->SetText(_T("MainMenu(&C)"));
	mainMenu1->ItemAdd(mainMenu_1);
	mainMenu1->ItemAdd(mainMenu_2);
	mainMenu1->ItemAdd(mainMenu_3);
	mainMenu1->SetAllOwnerDraw(true);

	mainMenuBar = new MainMenuBar();
	mainMenuBar->SetName(_T("mainMenuBar"));
	mainMenuBar->SetText(_T("mainMenuBar"));
	//mainMenuBar->SetLocation(0, 0);
	mainMenuBar->SetMinSize(0, 23);
	mainMenuBar->SetCloseButtonEnable(false);
	mainMenuBar->SetMainMenu(mainMenu1);
	mainMenuBar->SetDockable((DockStyles::Type)(DockStyles::Top|DockStyles::Bottom|DockStyles::None));
	mainMenuBar->SetBreak(true);
	//mainMenuBar->SetFloatable(false);
	//this->AddControl(mainMenuBar);
	//this->AddDockBar(mainMenuBar);
	//this->SetMainMenu(mainMenu1);
	this->AddToolBar(mainMenuBar);

	toolbutton1 = new ToolItem();
	toolbutton1->SetName(_T("toolbutton1"));
	toolbutton1->SetText(_T("test tool"));
	toolbutton1->SetTips(_T("tips of toolbutton1.\r\ntest for click and popup.\r\nonly so."));
	toolbutton1->SetIcon(imageList, IDI_ICON5);
	toolbutton1->Click += EventHandler((Object*)this, (EventFun)&MainForm::toolbutton1_Click);
	toolbutton1->Popup += EventHandler((Object*)this, (EventFun)&MainForm::toolbutton1_Popup);
	toolbutton1->SetContextMenu(coneMenuConfig);

	toolbutton2 = new ToolItem();
	toolbutton2->SetName(_T("button2"));
	toolbutton2->SetText(_T("Exit"));
	toolbutton2->SetTips(_T("tips of toolbutton2,this is exit toolitem.exit this application."));
	toolbutton2->SetIcon(imageList, 9999);
	toolbutton2->Click += EventHandler((Object*)this, (EventFun)&MainForm::exit_Click);
	toolbutton2->SetContextMenu(contextMenu1);

	toolbutton3 = new ToolItem();
	toolbutton3->SetName(_T("button3"));
	toolbutton3->SetText(_T("disabled"));
	toolbutton3->SetTips(_T("tips of toolbutton3,this is disabled."));
	toolbutton3->SetEnabled(false);
	toolbutton3->SetIcon(imageList, IDI_ICON2);
	toolbutton3->Click += EventHandler((Object*)this, (EventFun)&MainForm::toolbutton3_Click);

	toolConfig = new ToolItem();
	toolConfig->SetName(_T("toolPopmenu"));
	toolConfig->SetText(_T("Pop Menu"));
	toolConfig->SetIcon(imageList, IDI_ICON2);
	toolConfig->SetStyle(StyleType::Outer, 0, OuterStyle::PushButton);
	toolConfig->Click += EventHandler((Object*)this, (EventFun)&MainForm::Control_Click);
	toolConfig->SetContextMenu(coneMenuConfig);

	toolbutton5 = new ToolItem();
	toolbutton5->SetName(_T("toolbutton5"));
	toolbutton5->SetBreak(true);

	toolbutton6 = new ToolItem();
	toolbutton6->SetName(_T("toolbutton6"));
	toolbutton6->SetText(_T("Disable"));
	toolbutton6->SetTips(_T("set some disable and some to be enabled."));
	toolbutton6->SetIsPushButton(true);
	toolbutton6->Click += EventHandler((Object*)this, (EventFun)&MainForm::Control_Click);

	toolbutton7 = new ToolItem();
	toolbutton7->SetName(_T("toolbutton7"));
	toolbutton7->SetText(_T("Add ICON"));
	toolbutton7->SetTips(_T("add ICON or remove ICON from some controls."));
	toolbutton7->SetIsPushButton(true);
	toolbutton7->Click += EventHandler((Object*)this, (EventFun)&MainForm::Control_Click);

	toolbar1 = new ToolBar();
	toolbar1->SetText(_T("my first toolBar"));
	toolbar1->SetContextMenu(contextMenu1);
	//toolbar1->SetIcon(imageList, IDI_ICON5);
	//toolbar1->SetLocation(0, 23);
	toolbar1->SetMinSize(0, 25);
	toolbar1->AddControl(toolbutton1);
	toolbar1->AddControl(toolbutton2);
	toolbar1->AddControl(toolbutton3);
	toolbar1->AddControl(toolConfig);
	toolbar1->AddControl(toolbutton5);
	toolbar1->AddControl(toolbutton6);
	toolbar1->AddControl(toolbutton7);
	toolbar1->SetBreak(true);
	//toolbar1->SetFloatable(false);
	//this->AddControl(toolbar1);
	//this->AddDockBar(toolbar1);
	this->AddToolBar(toolbar1);

	toolbutton2_1 = new ToolItem();
	toolbutton2_1->SetIcon(imageList, IDI_ICON5);
	//toolbutton2_1->SetIcon(imageList->GetIcon(IDI_ICON2), IconType::Hot);
	toolbutton2_1->Click += EventHandler((Object*)this, (EventFun)&MainForm::menuitem_Click);
	toolbutton2_1->SetIsPushButton(true);
	toolbutton2_1->SetOuterStyle(OuterStyle::AutoConfig, true);
	toolbutton2_1->SetContextMenu(contextMenu3);

	toolbutton2_2 = new ToolItem();
	toolbutton2_2->SetIcon(imageList, IDI_ICON4);
	toolbutton2_2->SetTips(_T("tips of toolbutton2_2,with icon."), imageList, IDI_ICON4);
	toolbutton2_2->Click += EventHandler((Object*)this, (EventFun)&MainForm::menuitem_Click);
	toolbutton2_2->SetContextMenu(coneMenuConfig);

	toolbutton2_3 = new ToolItem();
	toolbutton2_3->SetTips(_T("tips of toolbutton2_3,this is SetBreak."));
	toolbutton2_3->SetBreak(true);

	toolbutton2_4 = new ToolItem();
	toolbutton2_4->SetTips(_T("tips of toolbutton2_4,this is disabled."));
	toolbutton2_4->SetEnabled(false);
	toolbutton2_4->SetIcon(imageList, IDI_ICON2);
	toolbutton2_4->SetContextMenu(contextMenu3);

	ToolItem* toolitem7 = new ToolItem();
	toolitem7->SetName(_T("toolitem7"));
	toolitem7->SetText(_T("this cannot float(but move)"));
	toolitem7->SetTips(_T("tips of toolitem7,with icon."), imageList, IDI_ICON4);
	toolitem7->SetIcon(imageList, IDI_ICON4);
	toolitem7->Click += EventHandler((Object*)this, (EventFun)&MainForm::button1_Click);
	ToolBar* toolbar7 = new ToolBar();
	toolbar7->SetText(_T("toolbar7"));
	//toolbar7->SetLocation(0, 55);
	toolbar7->SetMinSize(0, 35);
	toolbar7->AddControl(toolitem7);
	toolbar7->SetInitDockPosition(DockStyles::Bottom);
	toolbar7->SetDockable((DockStyles::Type)(DockStyles::Left|DockStyles::Top|DockStyles::Right|DockStyles::Bottom));
	toolbar7->SetBreak(true);
	this->AddToolBar(toolbar7);

	ToolBar* toolbar8 = new ToolBar();
	toolbar8->SetText(_T("toolbar8"));
	//toolbar8->SetLocation(100, 55);
	toolbar8->SetMinSize(0, 35);
	toolbar8->SetInitDockPosition(DockStyles::Bottom);
	this->AddToolBar(toolbar8);

	toolitem81 = new ToolItem();
	toolitem81->SetName(_T("toolitem81"));
	toolitem81->SetText(_T("no AutoSize"));
	toolitem81->SetTips(_T("tips of toolitem8,with icon."), imageList, IDI_ICON4);
	toolitem81->SetIcon(imageList, IDI_ICON4);
	toolitem81->SetAutoSize(false);
	toolitem81->SetSize(120, 30);
	toolitem81->Click += EventHandler((Object*)this, (EventFun)&MainForm::Control_Click);
	toolbar8->AddControl(toolitem81);
	titemStretchIcon = new ToolItem();
	titemStretchIcon->SetName(_T("titemStretchIcon"));
	titemStretchIcon->SetText(_T("StretchIcon"));
	titemStretchIcon->SetIcon(imageList, 9999);
	titemStretchIcon->SetTips(_T("tips of titemStretchIcon,switch icon while hot/gloom/disabled/pushed."), imageList, 9999);
	titemStretchIcon->SetStyle(StyleType::Outer, OuterStyle::TransparentParentErase, OuterStyle::TransparentTop);
	//titemStretchIcon->SetBackgroundImage(IDI_ICON_BTN2, IMAGE_ICON, true);
	titemStretchIcon->SetOuterStyle(OuterStyle::DrawIconStretch, true);
	titemStretchIcon->SetMargin(8, 4, 8, 4);
	titemStretchIcon->Click += EventHandler((Object*)this, (EventFun)&MainForm::Control_Click);
	toolbar8->AddControl(titemStretchIcon);

	toolitem9 = new ToolItem();
	toolitem9->SetName(_T("toolitem9"));
	toolitem9->SetText(_T("item 9"));
	toolitem9->SetTips(_T("tips of toolitem9."));
	toolitem9->SetAutoSize(false);
	toolitem9->SetLocation(80, 0);
	toolitem9->SetEnabled(false);
	ToolBar* toolbar9 = new ToolBar();
	toolbar9->SetText(_T("toolbar9"));
	toolbar9->SetContextMenu(coneMenuConfig);
	//toolbar9->SetLocation(100, 55);
	toolbar9->SetMinSize(0, 25);
	toolbar9->AddControl(toolitem9);
	toolbar9->SetInitDockPosition(DockStyles::None);
	this->AddToolBar(toolbar9);

	toolitem10 = new ToolItem();
	toolitem10->SetName(_T("toolitem10"));
	toolitem10->SetText(_T("only move"));
	toolitem10->SetTips(_T("tips of toolitem10,with icon."), imageList, IDI_ICON4);
	toolitem10->SetIcon(imageList, IDI_ICON4);
	toolitem10->Click += EventHandler((Object*)this, (EventFun)&MainForm::button1_Click);
	ToolBar* toolbar10 = new ToolBar();
	toolbar10->SetText(_T("toolbar10"));
	//toolbar10->SetLocation(0, 55);
	toolbar10->SetMinSize(0, 35);
	toolbar10->AddControl(toolitem10);
	toolbar10->SetInitDockPosition(DockStyles::Right);
	toolbar10->SetDockable((DockStyles::Type)(DockStyles::Left|DockStyles::Top|DockStyles::Right|DockStyles::Bottom));
	toolbar10->SetBreak(true);
	this->AddToolBar(toolbar10);

	toolitem11 = new ToolItem();
	toolitem11->SetName(_T("toolitem11"));
	toolitem11->SetText(_T("left&&right"));
	toolitem11->SetTips(_T("tips of toolitem11,with icon."), imageList, IDI_ICON4);
	toolitem11->SetIcon(imageList, IDI_ICON4);
	toolitem11->SetSize(120, 30);
	toolitem11->Click += EventHandler((Object*)this, (EventFun)&MainForm::button1_Click);
	ToolBar* toolbar11 = new ToolBar();
	toolbar11->SetText(_T("toolbar11"));
	//toolbar11->SetLocation(100, 55);
	toolbar11->SetMinSize(0, 35);
	toolbar11->AddControl(toolitem11);
	toolbar11->SetInitDockPosition(DockStyles::Right);
	toolbar11->SetDockable((DockStyles::Type)(DockStyles::Left|DockStyles::Right));
	this->AddToolBar(toolbar11);

	toolbar2 = new ToolBar();
	toolbar2->SetText(_T("my sec toolBar"));
	toolbar2->SetContextMenu(coneMenuConfig);
	//toolbar2->SetLocation(0, 55);
	toolbar2->SetMinSize(0, 41);
	toolbar2->AddControl(toolbutton2_1);
	toolbar2->AddControl(toolbutton2_2);
	toolbar2->AddControl(toolbutton2_3);
	toolbar2->AddControl(toolbutton2_4);
	toolbar2->SetBreak(true);
	//this->AddControl(toolbar2);
	//this->AddDockBar(toolbar2);
	toolbar2->SetInitDockPosition(DockStyles::Left);
	this->AddToolBar(toolbar2);

	//toolitem with DrawIconTop
	contextMenuTopIcon = new ContextMenu();
	this->DisposeItemAdd(contextMenuTopIcon);

	menuTopIcon1 = new MenuItem();
	menuTopIcon1->SetText(_T("Icon(Up)&&Text"));
	menuTopIcon1->Click += EventHandler((Object*)this, (EventFun)&MainForm::Control_Click);
	contextMenuTopIcon->ItemAdd(menuTopIcon1);

	menuTopIcon2 = new MenuItem();
	menuTopIcon2->SetText(_T("Icon(Left)&&Text"));
	menuTopIcon2->Click += EventHandler((Object*)this, (EventFun)&MainForm::Control_Click);
	contextMenuTopIcon->ItemAdd(menuTopIcon2);

	menuTopIcon3 = new MenuItem();
	menuTopIcon3->SetText(_T("Icon Only"));
	menuTopIcon3->Click += EventHandler((Object*)this, (EventFun)&MainForm::Control_Click);
	contextMenuTopIcon->ItemAdd(menuTopIcon3);

	menuTopIcon4 = new MenuItem();
	menuTopIcon4->SetText(_T("Text Only"));
	menuTopIcon4->Click += EventHandler((Object*)this, (EventFun)&MainForm::Control_Click);
	contextMenuTopIcon->ItemAdd(menuTopIcon4);

	menuBreak = new MenuItem();
	menuBreak->SetText(_T("-"));
	contextMenuTopIcon->ItemAdd(menuBreak);

	menuTopIcon5 = new MenuItem();
	menuTopIcon5->SetText(_T("Disable"));
	menuTopIcon5->Click += EventHandler((Object*)this, (EventFun)&MainForm::Control_Click);
	contextMenuTopIcon->ItemAdd(menuTopIcon5);

	tbarTopIcon = new ToolBar();
	tbarTopIcon->SetText(_T("toolitem with DrawIconTop"));
	tbarTopIcon->SetContextMenu(coneMenuConfig);
	tbarTopIcon->SetMinSize(0, 32 + 24);
	tbarTopIcon->SetBreak(true);
	tbarTopIcon->SetInitDockPosition(DockStyles::Top);
	this->AddToolBar(tbarTopIcon);

	tbtnTopIcon1 = new ToolItem();
	tbtnTopIcon1->SetText(_T("Up"));
	tbtnTopIcon1->SetIcon(imageList, IDI_ICON_NOTE);
	tbtnTopIcon1->Click += EventHandler((Object*)this, (EventFun)&MainForm::Control_Click);
	tbtnTopIcon1->SetOuterStyle(OuterStyle::AutoConfig, true);
	tbtnTopIcon1->SetContextMenu(contextMenuTopIcon);
	tbtnTopIcon1->SetOuterStyle(OuterStyle::DrawIconTop, true);
	tbarTopIcon->AddControl(tbtnTopIcon1);

	tbtnTopIcon2 = new ToolItem();
	tbtnTopIcon2->SetText(_T("This is Long Text"));
	tbtnTopIcon2->SetIcon(imageList, IDI_ICON2);
	tbtnTopIcon2->Click += EventHandler((Object*)this, (EventFun)&MainForm::Control_Click);
	tbtnTopIcon2->SetOuterStyle(OuterStyle::AutoConfig, true);
	tbtnTopIcon2->SetOuterStyle(OuterStyle::DrawIconTop, true);
	tbarTopIcon->AddControl(tbtnTopIcon2);

	tbtnTopIcon3 = new ToolItem();
	tbtnTopIcon3->SetBreak(true);
	tbarTopIcon->AddControl(tbtnTopIcon3);

	tbtnTopIcon4 = new ToolItem();
	tbtnTopIcon4->SetText(_T("Left"));
	tbtnTopIcon4->SetIcon(imageList, IDI_ICON4);
	tbtnTopIcon4->Click += EventHandler((Object*)this, (EventFun)&MainForm::Control_Click);
	tbtnTopIcon4->SetOuterStyle(OuterStyle::AutoConfig, true);
	tbtnTopIcon4->SetIsPushButton(true);
	//while IsPushButton show nothing about contextMenuTopIcon,but call first item's click
	//tbtnTopIcon4->SetContextMenu(contextMenuTopIcon);
	tbtnTopIcon4->SetOuterStyle(OuterStyle::DrawIconTop, true);
	tbarTopIcon->AddControl(tbtnTopIcon4);

	tbtnTopIcon5 = new ToolItem();
	tbtnTopIcon5->SetText(_T("Abc"));
	tbtnTopIcon5->SetIcon(imageList, IDI_ICON5);
	tbtnTopIcon5->Click += EventHandler((Object*)this, (EventFun)&MainForm::Control_Click);
	tbtnTopIcon5->SetOuterStyle(OuterStyle::AutoConfig, true);
	tbtnTopIcon5->SetContextMenu(contextMenuTopIcon);
	tbtnTopIcon5->SetOuterStyle(OuterStyle::DrawIconTop, true);
	tbarTopIcon->AddControl(tbtnTopIcon5);

	commTool = new ToolBar();
	commTool->SetName(_T("commTool"));
	commTool->SetMinSize(0, 25);
	//commTool->SetBreak(true);
	this->AddToolBar(commTool);

	toolOnTop = new ToolItem();
	toolOnTop->SetName(_T("toolOnTop"));
	//toolOnTop->SetSize(80, 17);
	toolOnTop->SetTips(_T("tips of toolOnTop, with icon."), imageList, IDI_ICON4);
	toolOnTop->SetIcon(imageList, IDI_ICON_ATTACH);
	toolOnTop->SetIsPushButton(true);
	toolOnTop->Click += EventHandler((Object*)this, (EventFun)&MainForm::toolOnTop_Click);
	toolOnTop->SetOuterStyle(OuterStyle::AutoConfig, true);
	commTool->AddControl(toolOnTop);

	toolSetBack = new ToolItem();
	toolSetBack->SetName(_T("toolSetBack"));
	toolSetBack->SetIcon(imageList, IDI_ICON_SETBACK);
	toolSetBack->Click += EventHandler((Object*)this, (EventFun)&MainForm::toolSetBack_Click);
	commTool->AddControl(toolSetBack);

	statusitem1 = new StatusItem();
	statusitem1->SetName(_T("statusitem1"));
	statusitem1->SetText(_T("Status item 1"));
	statusitem1->SetTips(_T("tips of statusitem1(self),with icon."), imageList, IDI_ICON4);
	statusitem1->SetIcon(imageList, IDI_ICON4);
	statusitem1->SetAutoSize(StatusItem::AutoSize::Sprint);
	statusitem1->DoubleClick += EventHandler((Object*)this, (EventFun)&MainForm::button1_Click);
	statusitem2 = new StatusItem();
	statusitem2->SetName(_T("statusitem2"));
	statusitem2->SetText(_T("item 2"));
	statusitem2->SetTips(_T("tips of statusitem2."));
	statusitem2->SetEnabled(false);
	statusitem3 = new StatusItem();
	statusitem3->SetName(_T("statusitem3"));
	statusitem3->SetText(_T("item 3"));
	statusitem3->SetTips(_T("tips of statusitem3,with icon."), imageList, IDI_ICON4);
	statusitem3->SetBounds(0, 3, 100, 21);
	statusitem4 = new StatusItem();
	statusitem4->SetIcon(imageList, IDI_ICON_NOTE);
	statusitem4->SetOuterStyle(OuterStyle::AlwaysNoEdge, false);
	statusitem4->SetOuterStyle(OuterStyle::PushButton, true);
	statusitem4->SetEnabled(false);
	statusitem5 = new StatusItem();
	statusitem5->SetMargin(1, 0, 0, 0);
	statusitem5->SetIcon(imageList, IDI_ICON_PRINT);
	statusitem5->SetOuterStyle(OuterStyle::AlwaysNoEdge, false);
	statusitem6 = new StatusItem();
	statusitem6->SetMargin(1, 0, 1, 0);
	statusitem6->SetText(_T("NUM"));
	statusitem6->SetOuterStyle(OuterStyle::AlwaysNoEdge, false);
	statusitem6->SetContextMenu(contextMenu3);
	statusitem6->SetHotTextColor(RGB(0, 0, 255));
	toolitem7 = new StatusItem();
	toolitem7->SetMargin(1, 0, 1, 0);
	toolitem7->SetText(_T("INS "));
	toolitem7->SetOuterStyle(OuterStyle::AlwaysNoEdge, false);
	toolitem7->SetOuterStyle(OuterStyle::PushButton, true);

	statusbreak1 = new StatusItem();
	statusbreak1->SetBreak(true);
	statusbreak2 = new StatusItem();
	statusbreak2->SetBreak(true);
	statusbreak3 = new StatusItem();
	statusbreak3->SetBreak(true);

	statusbar = new StatusBar();
	statusbar->AddControl(statusitem1);
	statusbar->AddControl(statusbreak1);
	statusbar->AddControl(statusitem2);
	statusbar->AddControl(statusbreak2);
	statusbar->AddControl(statusitem3);
	statusbar->AddControl(statusbreak3);
	statusbar->AddControl(statusitem4);
	statusbar->AddControl(statusitem5);
	statusbar->AddControl(statusitem6);
	statusbar->AddControl(toolitem7);
	this->SetStatusBar(statusbar);

	notify1 = new NotifyIcon();
	notify1->SetText(_T("my first notifyicon..."));
	notify1->SetIcon(imageList, IDI_ICON5);
	notify1->SetContextMenu(contextMenu1);
	notify1->SetVisible(true);
	notify1->Click.IsEmpty();
	notify1->Click += EventHandler((Object*)this, (EventFun)&MainForm::Notify_Click);
	notify1->DoubleClick += EventHandler((Object*)this, (EventFun)&MainForm::Notify_DoubleClick);
	notify1->MouseMove += MouseEventHandler((Object*)this, (MouseEventFun)&MainForm::Notify_MouseMove);
	notify1->MouseUp += MouseEventHandler((Object*)this, (MouseEventFun)&MainForm::Notify_MouseUp);
	notify1->MouseDown += MouseEventHandler((Object*)this, (MouseEventFun)&MainForm::Notify_MouseDown);

	notify2 = new NotifyIcon();
	notify2->SetText(_T("my sescond notifyicon..."));
	notify2->SetIcon(imageList, IDI_ICON5);
	notify2->SetContextMenu(coneMenuConfig);
	notify2->SetVisible(true);
	//notify2->SetShowMenu(NotifyIcon::ShowMenu::RightClick);
	notify2->SetShowMenu(LeftRight::Left, false);
	notify2->SetShowMenu(LeftRight::Right, true);
	notify2->AddAnimate(imageList->GetIcon(IDI_ICON2));
	notify2->AddAnimate(imageList->GetIcon(IDI_ICON3));
	notify2->AddAnimate(imageList->GetIcon(IDI_ICON4));

	int nTop = 5;
	groupForm1 = new GroupPanel();
	groupForm1->SetName(_T("groupForm1"));
	groupForm1->SetLocation(280, 1);
	groupForm1->SetSize(570, 160);
	groupForm1->SetText(_T("Group"));
	groupForm1->SetTextColor(RGB(255, 0, 0));
	this->AddControl(groupForm1);

	groupForm2 = new GroupPanel();
	groupForm2->SetName(_T("groupForm1"));
	groupForm2->SetLocation(280, 0);
	groupForm2->SetSize(280, 140);
	groupForm2->SetText(_T("Group"));
	groupForm2->SetTextColor(RGB(0, 0, 255));
	groupForm2->SetIcon(imageList, IDI_ICON4);
	groupForm1->AddControl(groupForm2);

	tab1 = new TabControl();
	tab1->SetName(_T("tab1"));
	tab1->SetConfigName(_T("tab"));
	tab1->SetLocation(2, 170);
	tab1->SetSize(597, 310);
	tab1->SetOuterStyle(OuterStyle::AutoConfig, true);
	tab1->Anchor = (AnchorStyles::Type)(AnchorStyles::Top | AnchorStyles::Left | AnchorStyles::Right | AnchorStyles::Bottom);
	this->AddControl(tab1);

	tabPage1 = new TabPage();
	tabPage1->SetName(_T("page1"));
	tabPage1->SetText(_T("Page1"));
	tabPage1->SetClientSize(600, 600);
	tab1->AddPage(tabPage1);

	groupPage1 = new GroupPanel();
	groupPage1->SetName(_T("groupPage1"));
	groupPage1->SetLocation(280, 1);
	groupPage1->SetSize(570, 160);
	groupPage1->SetText(_T("Group"));
	groupPage1->SetTextColor(RGB(0, 255, 255));
	tabPage1->AddControl(groupPage1);

	groupPage2 = new GroupPanel();
	groupPage2->SetName(_T("groupPage2"));
	groupPage2->SetLocation(280, 0);
	groupPage2->SetSize(270, 140);
	groupPage2->SetText(_T("Group(no Color)"));
	groupPage2->SetIcon(imageList, IDI_ICON4);
	groupPage1->AddControl(groupPage2);

	tabPage2 = new TabPage();
	tabPage2->SetName(_T("page2"));
	tabPage2->SetText(_T("page2"));
	tabPage2->SetClientSize(600, 600);
	tabPage2->SetIcon(imageList, IDI_ICON5);
	tab1->AddPage(tabPage2);

	listView1 = new ListView();
	listView1->SetName(_T("listView1"));
	listView1->SetText(_T("listView1"));
	listView1->Click += EventHandler((Object*)this, (EventFun)&MainForm::button1_Click);
	listView1->SetLocation(1, 1);
	listView1->SetSize(100, 100);
	listView1->SetContextMenu(coneMenuConfig);
	tabPage2->AddControl(listView1);

	edit1 = new TextBox();
	edit1->SetName(_T("edit1"));
	edit1->SetMultiline(true);
	edit1->SetText(_T("edit1"));
	edit1->Click += EventHandler((Object*)this, (EventFun)&MainForm::button1_Click);
	edit1->SetLocation(120, 1);
	edit1->SetSize(200, 100);
	edit1->SetContextMenu(coneMenuConfig);
	tabPage2->AddControl(edit1);

	tabPage3 = new TabPage();
	tabPage3->SetName(_T("page3"));
	tabPage3->SetText(_T("page3"));
	tabPage3->SetClientSize(600, 600);
	tab1->AddPage(tabPage3);

	nTop = 5;
	comboMenu = new ComboMenu();
	comboMenu->SetName(_T("comboMenu"));
	comboMenu->SetText(_T("Item1;	Item2;	Item3;	Item4;	Item5;	Item6;	Item7;	Item8;	Item9;	Item10;	"));
	comboMenu->SetTips(_T("TipsItemt1;	TipsItem3;	TipsItm4;	TipsItem5;	Item6;	Item8;	TipsItem9;	Item8;	Item9;	Item10;	self's tips...;	"));
	comboMenu->SetTipsExt(_T("TipsExtItem1;	TipsExtItem1;	TipsExtItem1;	Item1;	Item1;	TipsExtItem1;	Item1;	Item8;	Item9;	Item10;	self's tipsExt...;	"));
	comboMenu->Click += EventHandler((Object*)this, (EventFun)&MainForm::button1_Click);
	comboMenu->SetLocation(10, nTop);
	comboMenu->SetSize(100, 20);
	comboMenu->SetOuterStyle(OuterStyle::AutoConfig, true);
	tabPage3->AddControl(comboMenu);

	nTop += 22;
	comboList = new ComboList();
	comboList->SetName(_T("comboList"));
	comboList->SetText(_T("Item1;	Item2;	Item3;	Item4;	Item5;	Item6;	Item7;	Item8;	Item9;	Item10;	"));
	comboList->SetTips(_T("tips for comboList"));
	comboList->SetTipsExt(_T("tipsext for comboList\r\next."));
	comboList->Click += EventHandler((Object*)this, (EventFun)&MainForm::button1_Click);
	comboList->SetLocation(10, nTop);
	comboList->SetSize(100, 20);
	comboList->SetPopSize(100, 145, true, 20, 300, 300);
	comboList->SetOuterStyle(OuterStyle::AutoConfig, true);
	tabPage3->AddControl(comboList);

	nTop += 22;
	pickPercent = new PickPercentCtl();
	pickPercent->SetName(_T("pickPercent"));
	pickPercent->SetLocation(10, nTop);
	pickPercent->SetSize(40, 20);
	pickPercent->SetMinMaxValue(0, 255);
	pickPercent->SetValue(159);
	pickPercent->SetOuterStyle(OuterStyle::AutoConfig, true);
	tabPage3->AddControl(pickPercent);

	nTop += 22;
	edit3_1 = new TextBox();
	edit3_1->SetName(_T("edit3_1"));
	edit3_1->SetStyleNumber(true);
	edit3_1->SetText(_T("0"));
	edit3_1->SetLocation(10, nTop);
	edit3_1->SetSize(40, 18);
	edit3_1->SetContextMenu(coneMenuConfig);
	tabPage3->AddControl(edit3_1);
	pickPercent2 = new PickPercentCtl();
	pickPercent2->SetName(_T("pickPercent2"));
	pickPercent2->SetLocation(51, nTop);
	pickPercent2->SetSize(15, 20);
	pickPercent2->SetResultControl(edit3_1, cc::Str(), true);
	pickPercent2->SetMinMaxValue(-1000, 1000);
	pickPercent2->SetValue(99);
	pickPercent2->SetStyle(StyleType::Outer, 0, OuterStyle::DrawNoText|OuterStyle::AutoConfig);
	pickPercent2->GetScrollBar()->SetLargeChange(50);
	tabPage3->AddControl(pickPercent2);

	nTop += 22;
	label1 = new Label();
	label1->SetName(_T("label1"));
	label1->SetText(_T("URL(&D)"));
	label1->SetLocation(10, nTop + 2);
	label1->SetSize(100, 17);
	tabPage3->AddControl(label1);
	pickPercent3 = new PickPercentCtl();
	pickPercent3->SetName(_T("pickPercent3"));
	pickPercent3->SetLocation(111, nTop);
	pickPercent3->SetSize(15, 20);
	pickPercent3->SetResultControl(label1, cc::Str(_T("Result:[%d]")));
	pickPercent3->SetMinMaxValue(-100, 100);
	pickPercent3->SetValue(99);
	pickPercent3->SetStyle(StyleType::Outer, 0, OuterStyle::DrawNoText|OuterStyle::AutoConfig);
	tabPage3->AddControl(pickPercent3);

	nTop += 22;
	pickPercent4 = new PickPercentCtl();
	pickPercent4->SetName(_T("pickPercent4"));
	pickPercent4->SetLocation(10, nTop);
	pickPercent4->SetSize(200, 20);
	pickPercent4->SetResultControl(NULL, cc::Str(_T("Special Value:[%d]")));
	pickPercent4->SetMinMaxValue(-300, 300);
	pickPercent4->SetValue(99);
	pickPercent4->SetStyle(StyleType::Outer, 0, OuterStyle::AutoConfig);
	tabPage3->AddControl(pickPercent4);

	nTop = 5;

	tabPage4 = new TabPage();
	tabPage4->SetName(_T("page4"));
	//tabPage4->SetText(_T("page4"));
	tabPage4->SetText(_T(""));
	tabPage4->SetClientSize(600, 600);
	tabPage4->SetIcon(imageList, IDI_ICON4);
	tab1->AddPage(tabPage4);

	tabPage5 = new TabPage();
	tabPage5->SetName(_T("page5"));
	tabPage5->SetText(_T("ie"));
	tabPage5->SetClientSize(600, 600);
	tab1->AddPage(tabPage5);

	nTop = 5;
	panel1 = new Panel();
	panel1->SetName(_T("panel1"));
	panel1->Anchor = (AnchorStyles::Type)(AnchorStyles::Top | AnchorStyles::Left | AnchorStyles::Right);
	panel1->SetLocation(1, nTop);
	panel1->SetSize(410, 23);
	panel1->SetMinSize(200, 0);
	tabPage5->AddControl(panel1);

	label1 = new Label();
	label1->SetName(_T("label1"));
	label1->SetText(_T("URL(&D)"));
	label1->SetLocation(0, 0);
	panel1->AddControl(label1);

	comboBoxEx1_In = NULL;
	comboBoxEx1 = new ComboBoxEx();
	comboBoxEx1->SetName(_T("comboBoxEx1"));
	comboBoxEx1->SetLocation(30, 0);
	comboBoxEx1->SetSize(270, 60);
	comboBoxEx1->Anchor = (AnchorStyles::Type)(AnchorStyles::Top | AnchorStyles::Left | AnchorStyles::Right);
	panel1->AddControl(comboBoxEx1);

	button1 = new Button();
	button1->SetName(_T("button1"));
	button1->SetText(_T("go"));
	button1->Click += EventHandler((Object*)this, (EventFun)&MainForm::forie_Click);
	button1->SetLocation(305, 0);
	button1->SetSize(50, 19);
	button1->Anchor = (AnchorStyles::Type)(AnchorStyles::Top | AnchorStyles::Right);
	button1->SetContextMenu(coneMenuConfig);
	panel1->AddControl(button1);

	button2 = new Button();
	button2->SetName(_T("button2"));
	button2->SetText(_T("view"));
	button2->Click += EventHandler((Object*)this, (EventFun)&MainForm::iepreview_Click);
	button2->SetLocation(357, 0);
	button2->SetSize(48, 19);
	button2->Anchor = (AnchorStyles::Type)(AnchorStyles::Top | AnchorStyles::Right);
	panel1->AddControl(button2);

	nTop += 24;
	ie1 = new IEControl();
	ie1->SetName(_T("ie1"));
	ie1->SetText(_T("ie1"));
	ie1->SetLocation(3, nTop);
	ie1->SetSize(600 - 5, 600 - nTop - 3);
	ie1->Anchor = (AnchorStyles::Type)(AnchorStyles::Top | AnchorStyles::Left | AnchorStyles::Right | AnchorStyles::Bottom);
	ie1->SetContextMenu(coneMenuConfig);
	tabPage5->AddControl(ie1);

	//pageOption
	pageOption = new TabPage();
	pageOption->SetName(_T("pageOption"));
	pageOption->SetText(_T("Option"));
	pageOption->SetClientSize(600, 600);
	tab1->AddPage(pageOption);

	nTop = 5;
	label = new Label();
	label->SetName(_T("labLang"));
	label->SetLocation(3, nTop + 2);
	label->SetSize(70, 17);
	pageOption->AddControl(label);

	comboLang = new ComboBox();
	comboLang->SetName(_T("comboLang"));
	comboLang->SetComboBoxStyle(ComboBoxStyle::DropDownList);
	comboLang->SetLocation(73, nTop);
	comboLang->SetSize(200, 145);
	//comboLang->Anchor = (AnchorStyles::Type)(AnchorStyles::Top | AnchorStyles::Left | AnchorStyles::Right);
	pageOption->AddControl(comboLang);

	btnOK = new Button();
	btnOK->SetName(_T("btnOK"));
	btnOK->SetLocation(276, nTop);
	btnOK->SetSize(100, 19);
	btnOK->Click += EventHandler((Object*)this, (EventFun)&MainForm::OptionOK_Click);
	pageOption->AddControl(btnOK);

	nTop += 24;
	VersionInfo* verInfo = new VersionInfo();
	verInfo->SetLocation(3, nTop);
	verInfo->SetSize(600 - 5, 600 - nTop - 3);
	verInfo->Anchor = (AnchorStyles::Type)(AnchorStyles::Top | AnchorStyles::Left | AnchorStyles::Right | AnchorStyles::Bottom);
	pageOption->AddControl(verInfo);

	this->DisposeItemAdd(contextMenu1);
	this->DisposeItemAdd(coneMenuConfig);
	this->DisposeItemAdd(contextMenu3);
	this->DisposeItemAdd(mainMenu1);
	this->DisposeItemAdd(notify1);
	this->DisposeItemAdd(notify2);
	this->DisposeItemAdd(imageList);
	this->SetContextMenu(contextMenu1);
	this->SetName(_T("ccSample"));
	this->SetConfigName(_T("main"));
	this->SetText(_T("ccSample"));
	this->SetLocation(30, 650);
	this->SetOuterStyle(OuterStyle::AutoConfig, true);
	//this->SetMinSize(200, 200);
	//this->SetMaxSize(600, 500);
	this->SetClientSize(600, 480);
	this->SetSize(800, 600);
	this->SetCenter();
	//here need be 32X32
	this->SetIcon(imageList, IDI_ICON_MAIN);
	this->Load += EventHandler((Object*)this, (EventFun)&MainForm::Form_Load);
	this->Closed += EventHandler((Object*)this, (EventFun)&MainForm::Form_Closed);
	this->Closing += CancelEventHandler((Object*)this, (CancelEventFun)&MainForm::Form_Closing);
	this->Shutdown += EventHandler((Object*)this, (EventFun)&MainForm::Form_Shutdown);

	SetLang(*CC_APP_GetLang(), cc::Str(CC_CONFIG_LANGSECTION));

	//show it until Form_Load
	this->SetVisible(false);
}

void MainForm::DoSetLang(KeyArr& keyArr, cc::Str sThisHead)
{
	//keyArr.Dump();
	Form::DoSetLang(keyArr, sThisHead);
}
