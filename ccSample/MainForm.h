// cc.win.Form.h
// The C# like Form class
// Author: Shu.KK
// 2006-01-01
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef CC_WIN_MAINFORM_H_
#define CC_WIN_MAINFORM_H_

#include "..\cc\cc.All.h"
#include "resource.h"

using namespace cc;
using namespace cc::win;

#include "ControlSam.h"
#include "ListBoxSam.h"
#include "TabSam.h"
#include "TabExSam.h"


class TestControls : public Object
{
public:
	TestControls(Form* formClickEvent, Control* parent, ImageList* imageList, ContextMenu* coneMenuConfig, int nLeft, int nTop);
	~TestControls();

	void SetEnabled(bool value);
	void SetVisible(bool value);

	//childrens for test layout
	Label* label;
	PickColorCtl* pickColorCtl;
	PickColorCtl* pickColorCtl2;
	Button* btn1;
	ToolItem* tool1;
	ToolItem* tool2;
	RadioButton* radio1;
	RadioButton* radio2;
	CheckBox* chk1;
	PictureBox* pic1;
	TextBox* txtSpin1;
	PickColorCtl* pickColorCtl3;
	PickColorCtl* pickColorCtl4;
	SpinButton* spin1;
	ComboBox* com1;
	ComboBoxEx* comEx1;
	DateTimePicker* dtPicker;
	LabelLink* link1;
	GradientLabel* glabel;
	LEDControl* led;

protected:
};

class MainForm : public Form
{
public:
	MainForm();
	~MainForm();
	void Control_Click(Object* sender, EventArgs* e);

protected:
	ImageList* imageList;
	MenuItem* menuBreak;
	MenuItem* menu1_1;
	MenuItem* menu1_2;
	MenuItem* menu1_2_1;
	MenuItem* menu1_2_2;
	MenuItem* menu1_3;
	MenuItem* menu1_3_1;
	ContextMenu* contextMenu1;

	MenuItem* menuFClientBack;
	MenuItem* menuFClientBack1;
	MenuItem* menuFClientBack2;
	MenuItem* menuFClientBack3;
	MenuItem* menuFClientBack4;
	MenuItem* menuFClientBack5;
	MenuItem* menuFClientBack6;
	MenuItem* menu2_2;
	MenuItem* menuToolEdge;
	MenuItem* menuPictureBack1;
	MenuItem* menuPictureBack2;
	MenuItem* menuTestMsgDlg;
	MenuItem* menuTestAboutDlg;
	MenuItem* menuGroupBreak;
	MenuItem* menuGroupGradient;
	ContextMenu* coneMenuConfig;

	MenuItem* menu3_1;
	MenuItem* menu3_2;
	ContextMenu* contextMenu3;

	MenuItem* menu4_1;
	MenuItem* menu4_2;
	ContextMenu* contextMenu4;

	TabSam* tabDialog;
	TabExSam* tabExDialog;

	MenuItem* mainMenu_1;
	MenuItem* mainMenu_1_1;
	MenuItem* mainMenu_1_2;
	MenuItem* mainMenu_1_2_1;
	MenuItem* mainMenu_2;
	MenuItem* mainMenu_2_1;
	MenuItem* mainMenu_2_2;
	MenuItem* mainMenu_2_3;
	MenuItem* mainMenu_2_4;
	MenuItem* mainMenu_2_4_1;
	MenuItem* mainMenu_2_4_1_1;
	MenuItem* mainMenu_2_4_2;
	MenuItem* mainMenu_2_5;
	MenuItem* mainMenu_2_6;
	MenuItem* mainMenu_2_7;
	MenuItem* mainMenu_2_8;
	MenuItem* mainMenu_2_9;
	MenuItem* mainMenu_2_10;
	MenuItem* mainMenu_3;
	MenuItem* mainMenu_3_0;
	MenuItem* mainMenu_3_1;
	MenuItem* mainMenu_3_2;
	MenuItem* mainMenu_3_3;
	MenuItem* mainMenu_3_4;
	MenuItem* mainMenu_3_5;
	MainMenu* mainMenu1;
	MainMenuBar* mainMenuBar;

	HIMAGELIST hImageList;

	ReBar* rebar1;
	Label* toollabel2;
	ToolItem* toolbutton1;
	ToolItem* toolbutton2;
	ToolItem* toolbutton3;
	ToolItem* toolConfig;
	ToolItem* toolbutton5;
	ToolItem* toolbutton6;
	ToolItem* toolbutton7;
	ToolBar* toolbar1;
	ToolItem* toolbutton2_1;
	ToolItem* toolbutton2_2;
	ToolItem* toolbutton2_3;
	ToolItem* toolbutton2_4;
	ToolBar* toolbar2;

	ToolItem* toolitem81;
	ToolItem* titemStretchIcon;

	ToolItem* toolitem9;
	ToolItem* toolitem10;
	ToolItem* toolitem11;

	MenuItem* menuTopIcon1;
	MenuItem* menuTopIcon2;
	MenuItem* menuTopIcon3;
	MenuItem* menuTopIcon4;
	MenuItem* menuTopIcon5;
	ContextMenu* contextMenuTopIcon;

	ToolItem* tbtnTopIcon1;
	ToolItem* tbtnTopIcon2;
	ToolItem* tbtnTopIcon3;
	ToolItem* tbtnTopIcon4;
	ToolItem* tbtnTopIcon5;
	ToolBar* tbarTopIcon;

	ToolItem* toolOnTop;
	ToolItem* toolSetBack;
	ToolBar* commTool;

	//control on form
	Label* label;
	GroupPanel* groupForm1;
	GroupPanel* groupForm2;
	TestControls* testCtlsForm;
	TestControls* testCtlsGroup1;
	TestControls* testCtlsGroup2;
	TestControls* testCtlsTab;

	TabControl* tab1;
	TabPage* tabPage1;
	GroupPanel* groupPage1;
	GroupPanel* groupPage2;
	TestControls* testCtlsTabGroup1;
	TestControls* testCtlsTabGroup2;

	TabPage* tabPage2;
	TextBox* edit1;
	ListView* listView1;

	TabPage* tabPage3;
	ComboMenu* comboMenu;
	ComboList* comboList;
	PickPercentCtl* pickPercent;
	TextBox* edit3_1;
	PickPercentCtl* pickPercent2;
	PickPercentCtl* pickPercent3;
	PickPercentCtl* pickPercent4;

	TabPage* tabPage4;

	TabPage* tabPage5;
	Panel* panel1;
	Label* label1;
	Button* button1;
	Button* button2;
	ComboBoxEx* comboBoxEx1;
	ComboBox* comboBoxEx1_In;
	IEControl* ie1;

	//pageOption
	TabPage* pageOption;
	ComboBox* comboLang;
	Button* btnOK;
	VersionInfo* verInfo;

	StatusItem* statusitem1;
	StatusItem* statusbreak1;
	StatusItem* statusitem2;
	StatusItem* statusbreak2;
	StatusItem* statusitem3;
	StatusItem* statusbreak3;
	StatusItem* statusitem4;
	StatusItem* statusitem5;
	StatusItem* statusitem6;
	StatusItem* statusitem7;
	StatusBar* statusbar;

	NotifyIcon* notify1;
	NotifyIcon* notify2;

	void InitializeComponent();
	virtual void OnThemeChanged();

	void Form_Load(Object* sender, EventArgs* e);
	void Form_Closed(Object* sender, EventArgs* e);
	void Form_Closing(Object* sender, CancelEventArgs* e);
	void Form_Shutdown(Object* sender, EventArgs* e);
	void DoSetLang(KeyArr& keyArr, cc::Str sThisHead);

	void OptionOK_Click(Object* sender, EventArgs* e);
	void toolOnTop_Click(Object* sender, EventArgs* e);
	void toolSetBack_Click(Object* sender, EventArgs* e);
	void btnControlSam_Click(Object* sender, EventArgs* e);
	void btnListBoxSam_Click(Object* sender, EventArgs* e);
	void btnTabSam_Click(Object* sender, EventArgs* e);
	void btnTabExSam_Click(Object* sender, EventArgs* e);
	void menuShowToolBar_Click(Object* sender, EventArgs* e);

	void exit_Click(Object* sender, EventArgs* e);
	void menu5event_MeasureItem(Object* sender, MeasureItemEventArgs* e);
	void menu5event_DrawItem(Object* sender, DrawItemEventArgs* e);

	void menuitem_Click(Object* sender, EventArgs* e);
	void menu1_Click(Object* sender, EventArgs* e);
	void menu1_DrawItem(Object* sender, DrawItemEventArgs* e);
	void menu1_MeasureItem(Object* sender, MeasureItemEventArgs* e);
	void menu1_Popup(Object* sender, EventArgs* e);
	void menu1_Select(Object* sender, EventArgs* e);

	void toolbutton1_Click(Object* sender, EventArgs* e);
	void toolbutton1_Popup(Object* sender, EventArgs* e);
	void toolbutton3_Click(Object* sender, EventArgs* e);

	void setTransparent_Click(Object* sender, EventArgs* e);
	void setclosebutton_Click(Object* sender, EventArgs* e);
	void settoolbarfloat_Click(Object* sender, EventArgs* e);
	void ChangeMenuItem_Click(Object* sender, EventArgs* e);
	void ChangeStatus_Click(Object* sender, EventArgs* e);

	void Notify_Click(Object* sender, MouseEventArgs* e);
	void Notify_DoubleClick(Object* sender, MouseEventArgs* e);
	void Notify_MouseMove(Object* sender, MouseEventArgs* e);
	void Notify_MouseUp(Object* sender, MouseEventArgs* e);
	void Notify_MouseDown(Object* sender, MouseEventArgs* e);

	void forie_Click(Object* sender, EventArgs* e)
	{
		cc::Log::debug(_T("[forie_Click]S"));
		ie1->Navigate((wchar_t*)cc::Str(cc::LangCode::UNICODE16, comboBoxEx1->GetText()));
		cc::Log::debug(_T("[forie_Click]E"));
	}

	void iepreview_Click(Object* sender, EventArgs* e)
	{
		//ie1->SetVisible(false);
		cc::Log::debug(_T("[iepreview_Click]OLECMDID_PAGESETUP"));
		ie1->GetWebBrowser()->ExecWB((ie_OLECMDID)8, (ie_OLECMDEXECOPT)1, NULL, NULL);
		cc::Log::debug(_T("[iepreview_Click]OLECMDID_PRINTPREVIEW"));
		ie1->GetWebBrowser()->ExecWB((ie_OLECMDID)7, (ie_OLECMDEXECOPT)1, NULL, NULL);
	}

	void button1_Click(Object* sender, EventArgs* e)
	{
		cc::Log::debug(_T("[button1_Click]S"));
	}
};

#endif //#ifndef CC_WIN_MAINFORM_H_
