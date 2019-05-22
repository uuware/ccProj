// cc.win.Design.hpp
// class for design,should include in cc.win.Module.cpp
// Author: Shu.KK
// 2009-01-01

#if defined(_DEBUG_) || defined(_DESIGN)
const TCHAR* ControlNameFromID(int nClassID);

#define CC_FLAG_DESIGN_ISCALLEDCLOSED			CC_FLAG_FLOATCONTAINER_END
//#define CC_FLAG_DESIGN_ISACTIVATED				CC_FLAG_FLOATCONTAINER_END <<1
#define CC_FLAG_DESIGN_ISRECTED					CC_FLAG_FLOATCONTAINER_END <<2
#define CC_FLAG_DESIGN_END						CC_FLAG_FLOATCONTAINER_END <<3

#if defined(_DEBUG_) || (defined(_DESIGN) && !defined(_DESIGNEX))
class CC_DesignBar : public FloatContainer
{
public:
	CC_DesignBar()
	{
		//CC_ASSERT(parent != NULL && ::IsWindow(parent->m_hWnd));
		nLastVarIndex = 0;
		nActionType = 0; //1:Text;2:Tips;3:TipsExt;4:Color;5:Left;6:XXX;7:Anchor;8:URL;9:Style;
		nDoEnd = 0;
		isRestoreRect = 0;
		nSaveX = 0;
		nSaveY = 0;
		nSaveW = 0;
		nSaveH = 0;
		crSaveText = 0;
		crSaveHotText = 0;
		crSaveBack = 0;
		crSaveHotback = 0;
		dwSaveAnchor = 0;
		pLastCtl = NULL;

		int nTop = 1;
		comboCtl = new ComboBox();
		comboCtl->SetName(_T("comboCtl"));
		comboCtl->SetComboBoxStyle(ComboBoxStyle::DropDownList);
		comboCtl->SetLocation(2, nTop);
		comboCtl->SetSize(600 - 35, 145);
		comboCtl->SelectedIndexChanged += EventHandler((Object*)this, (EventFun)&CC_DesignBar::comboCtl_SelectedIndexChanged);
		comboCtl->Anchor = (AnchorStyles::Type)(AnchorStyles::Top | AnchorStyles::Left | AnchorStyles::Right);
		this->AddControl(comboCtl);

		btnRunRef = new ToolItem();
		btnRunRef->SetName(_T("btnRunRef"));
		btnRunRef->SetText(_T("="));
		btnRunRef->SetLocationOffset(comboCtl, false, 2, 0);
		btnRunRef->SetSize(12, 10);
		btnRunRef->SetOuterStyle(OuterStyle::AutoSize, false);
		btnRunRef->Anchor = (AnchorStyles::Type)(AnchorStyles::Top | AnchorStyles::Right);
		btnRunRef->SetTips(_T("Refresh Control's info."));
		btnRunRef->Click += EventHandler((Object*)this, (EventFun)&CC_DesignBar::Control_Click);
		this->AddControl(btnRunRef);
		btnRunDel = new ToolItem();
		btnRunDel->SetName(_T("btnRunDel"));
		btnRunDel->SetText(_T("-"));
		btnRunDel->SetLocationOffset(btnRunRef, true, 0, 0);
		btnRunDel->SetSize(12, 10);
		btnRunDel->Click += EventHandler((Object*)this, (EventFun)&CC_DesignBar::Control_Click);
		btnRunDel->SetOuterStyle(OuterStyle::AutoSize, false);
		btnRunDel->Anchor = (AnchorStyles::Type)(AnchorStyles::Top | AnchorStyles::Right);
		btnRunDel->SetTips(_T("Remove Control that created dynamically."));
		this->AddControl(btnRunDel);

		tab1 = new TabControl();
		tab1->SetName(_T("tab1"));
		tab1->SetLocation(1, 23);
		tab1->SetSize(600 - 9, 600 - 26 - 23 - 3);
		tab1->Anchor = (AnchorStyles::Type)(AnchorStyles::Top | AnchorStyles::Left | AnchorStyles::Right | AnchorStyles::Bottom);
		//tab1->SetOuterStyle(OuterStyle::AutoConfig, true);
		this->AddControl(tab1);

		//pageCfg1
		pageCfg1 = new TabPage();
		pageCfg1->SetName(_T("page1"));
		pageCfg1->SetText(_T("Controls"));
		pageCfg1->SetClientSize(600, 600);
		pageCfg1->SetOuterStyle(OuterStyle::TransparentTop, true);
		tab1->AddPage(pageCfg1);

		nTop = 1;
		lstVar = new ListBox();
		lstVar->SetLocation(2, 1);
		lstVar->SetSize(600 - 5, 600 - 50);
		lstVar->SelectedIndexChanged += EventHandler((Object*)this, (EventFun)&CC_DesignBar::lstVar_SelectedIndexChanged);
		lstVar->Anchor = (AnchorStyles::Type)(AnchorStyles::Top | AnchorStyles::Left | AnchorStyles::Right | AnchorStyles::Bottom);
		//lstVar->MouseEnter += EventHandler((Object*)this, (EventFun)&CC_DesignBar::HotHwnd_MouseEnter);
		//lstVar->MouseLeave += EventHandler((Object*)this, (EventFun)&CC_DesignBar::HotHwnd_MouseLeave);
		lstVar->SetTrackMouse(true);
		pageCfg1->AddControl(lstVar);

		btnSet = new Button();
		btnSet->SetName(_T("btnSet"));
		btnSet->SetText(_T("Set/Add"));
		btnSet->SetLocation(2, 600 - 42);
		btnSet->SetSize(60, 17);
		btnSet->Click += EventHandler((Object*)this, (EventFun)&CC_DesignBar::Control_Click);
		btnSet->SetTips(_T("After changed something, must click Set."));
		btnSet->SetEnabled(false);
		btnSet->Anchor = (AnchorStyles::Type)(AnchorStyles::Bottom | AnchorStyles::Left);
		pageCfg1->AddControl(btnSet);
		btnRunCls = new ToolItem();
		btnRunCls->SetName(_T("btnRunCls"));
		btnRunCls->SetText(_T("-"));
		btnRunCls->SetLocationOffset(btnSet, false, 1, 0);
		btnRunCls->SetSize(12, 17);
		btnRunCls->Click += EventHandler((Object*)this, (EventFun)&CC_DesignBar::Control_Click);
		btnRunCls->SetOuterStyle(OuterStyle::AutoSize, false);
		btnRunCls->SetTips(_T("Clear set.\r\nBut cann't restore control's setting(need restart application to restore orignal setting)."));
		btnRunCls->SetEnabled(false);
		btnRunCls->Anchor = (AnchorStyles::Type)(AnchorStyles::Bottom | AnchorStyles::Left);
		pageCfg1->AddControl(btnRunCls);

		btnCopyName = new ToolItem();
		btnCopyName->SetName(_T("btnCopyName"));
		btnCopyName->SetText(_T("N"));
		btnCopyName->SetTips(_T("Copy selected control's name."));
		btnCopyName->SetLocationOffset(btnSet, true, 0, 3);
		btnCopyName->SetSize(15, 17);
		btnCopyName->SetOuterStyle(OuterStyle::AutoSize, false);
		btnCopyName->Anchor = (AnchorStyles::Type)(AnchorStyles::Bottom | AnchorStyles::Left);
		btnCopyName->Click += EventHandler((Object*)this, (EventFun)&CC_DesignBar::Control_Click);
		pageCfg1->AddControl(btnCopyName);

		btnPrintCtl = new ToolItem();
		btnPrintCtl->SetName(_T("btnPrintCtl"));
		btnPrintCtl->SetText(_T("C"));
		btnPrintCtl->SetTips(_T("Copy selected control's config."));
		btnPrintCtl->SetLocationOffset(btnCopyName, false, 2, 0);
		btnPrintCtl->SetSize(15, 17);
		btnPrintCtl->SetOuterStyle(OuterStyle::AutoSize, false);
		btnPrintCtl->Anchor = (AnchorStyles::Type)(AnchorStyles::Bottom | AnchorStyles::Left);
		btnPrintCtl->Click += EventHandler((Object*)this, (EventFun)&CC_DesignBar::Control_Click);
		pageCfg1->AddControl(btnPrintCtl);

		btnCopyLang = new ToolItem();
		btnCopyLang->SetName(_T("btnCopyLang"));
		btnCopyLang->SetText(_T("L"));
		btnCopyLang->SetTips(_T("Copy current lang's config."));
		btnCopyLang->SetLocationOffset(btnPrintCtl, false, 2, 0);
		btnCopyLang->SetSize(15, 17);
		btnCopyLang->SetOuterStyle(OuterStyle::AutoSize, false);
		btnCopyLang->Anchor = (AnchorStyles::Type)(AnchorStyles::Bottom | AnchorStyles::Left);
		btnCopyLang->Click += EventHandler((Object*)this, (EventFun)&CC_DesignBar::Control_Click);
		pageCfg1->AddControl(btnCopyLang);

		btnClearDesign = new ToolItem();
		btnClearDesign->SetName(_T("btnClearDesign"));
		btnClearDesign->SetText(_T("X"));
		btnClearDesign->SetTips(_T("Clear current lang's config and all design's config."));
		btnClearDesign->SetLocationOffset(btnCopyLang, false, 2, 0);
		btnClearDesign->SetSize(15, 17);
		btnClearDesign->SetOuterStyle(OuterStyle::AutoSize, false);
		btnClearDesign->Anchor = (AnchorStyles::Type)(AnchorStyles::Bottom | AnchorStyles::Left);
		btnClearDesign->Click += EventHandler((Object*)this, (EventFun)&CC_DesignBar::Control_Click);
		pageCfg1->AddControl(btnClearDesign);

		comboStyle = new ComboBox();
		comboStyle->SetName(_T("comboStyle"));
		comboStyle->SetText(_T("AutoSize:On;	AutoSize:Off;	Color Back;	Gradient Back(Left-Right);	Gradient Back(Up-Down);	Image Back;	"));
		comboStyle->SetComboBoxStyle(ComboBoxStyle::DropDownList);
		comboStyle->SetLocationOffset(btnRunCls, false, 4, 20);
		comboStyle->SetSize(600 - 5 - 20 - 60 - 15, 145);
		comboStyle->SelectedIndexChanged += EventHandler((Object*)this, (EventFun)&CC_DesignBar::comboStyle_SelectedIndexChanged);
		comboStyle->Anchor = (AnchorStyles::Type)(AnchorStyles::Bottom | AnchorStyles::Left | AnchorStyles::Right);
		pageCfg1->AddControl(comboStyle);
		comboStyle->SetVisible(false);

		editX = new TextBox();
		editX->SetName(_T("editX"));
		editX->SetLocationOffset(btnRunCls, false, 4, 0);
		editX->SetSize(28, 17);
		editX->SetStyleNumber(true);
		editX->Anchor = (AnchorStyles::Type)(AnchorStyles::Bottom | AnchorStyles::Left);
		pageCfg1->AddControl(editX);
		pickX = new PickPercentCtl();
		pickX->SetName(_T("pickX"));
		pickX->SetLocationOffset(editX, false, 2, 0);
		pickX->SetSize(15, 17);
		pickX->SetResultControl(editX, cc::Str(), true);
		pickX->SetMinMaxValue(0, 999);
		pickX->SetValue(0);
		pickX->SetStyle(StyleType::Outer, 0, OuterStyle::DrawNoText);
		pickX->Anchor = (AnchorStyles::Type)(AnchorStyles::Bottom | AnchorStyles::Left);
		pageCfg1->AddControl(pickX);
		editY = new TextBox();
		editY->SetName(_T("editY"));
		editY->SetLocationOffset(pickX, false, 4, 0);
		editY->SetSize(28, 17);
		editY->SetStyleNumber(true);
		editY->Anchor = (AnchorStyles::Type)(AnchorStyles::Bottom | AnchorStyles::Left);
		pageCfg1->AddControl(editY);
		pickY = new PickPercentCtl();
		pickY->SetName(_T("pickY"));
		pickY->SetLocationOffset(editY, false, 2, 0);
		pickY->SetSize(15, 17);
		pickY->SetResultControl(editY, cc::Str(), true);
		pickY->SetMinMaxValue(0, 999);
		pickY->SetValue(0);
		pickY->SetStyle(StyleType::Outer, 0, OuterStyle::DrawNoText);
		pickY->Anchor = (AnchorStyles::Type)(AnchorStyles::Bottom | AnchorStyles::Left);
		pageCfg1->AddControl(pickY);
		editW = new TextBox();
		editW->SetName(_T("editW"));
		editW->SetLocationOffset(pickY, false, 4, 0);
		editW->SetSize(28, 17);
		editW->SetStyleNumber(true);
		editW->Anchor = (AnchorStyles::Type)(AnchorStyles::Bottom | AnchorStyles::Left);
		pageCfg1->AddControl(editW);
		pickW = new PickPercentCtl();
		pickW->SetName(_T("pickW"));
		pickW->SetLocationOffset(editW, false, 2, 0);
		pickW->SetSize(15, 17);
		pickW->SetResultControl(editW, cc::Str(), true);
		pickW->SetMinMaxValue(0, 999);
		pickW->SetValue(0);
		pickW->SetStyle(StyleType::Outer, 0, OuterStyle::DrawNoText);
		pickW->Anchor = (AnchorStyles::Type)(AnchorStyles::Bottom | AnchorStyles::Left);
		pageCfg1->AddControl(pickW);
		editH = new TextBox();
		editH->SetName(_T("editH"));
		editH->SetLocationOffset(pickW, false, 4, 0);
		editH->SetSize(28, 17);
		editH->SetStyleNumber(true);
		editH->Anchor = (AnchorStyles::Type)(AnchorStyles::Bottom | AnchorStyles::Left);
		pageCfg1->AddControl(editH);
		pickH = new PickPercentCtl();
		pickH->SetName(_T("pickH"));
		pickH->SetLocationOffset(editH, false, 2, 0);
		pickH->SetSize(15, 17);
		pickH->SetResultControl(editH, cc::Str(), true);
		pickH->SetMinMaxValue(0, 999);
		pickH->SetValue(0);
		pickH->SetStyle(StyleType::Outer, 0, OuterStyle::DrawNoText);
		pickH->Anchor = (AnchorStyles::Type)(AnchorStyles::Bottom | AnchorStyles::Left);
		pageCfg1->AddControl(pickH);
		pickX->ValueChanged += EventHandler((Object*)this, (EventFun)&CC_DesignBar::Control_Click);
		pickY->ValueChanged += EventHandler((Object*)this, (EventFun)&CC_DesignBar::Control_Click);
		pickW->ValueChanged += EventHandler((Object*)this, (EventFun)&CC_DesignBar::Control_Click);
		pickH->ValueChanged += EventHandler((Object*)this, (EventFun)&CC_DesignBar::Control_Click);
		editX->SetVisible(false);
		editY->SetVisible(false);
		editW->SetVisible(false);
		editH->SetVisible(false);
		pickX->SetVisible(false);
		pickY->SetVisible(false);
		pickW->SetVisible(false);
		pickH->SetVisible(false);

		pcColor = new PickColorCtl();
		pcColor->SetName(_T("pcColor"));
		pcColor->SetText(_T("Color"));
		//pcColor->SetBackground(label->GetBackground());
		pcColor->SetLocationOffset(btnRunCls, false, 4, 0);
		pcColor->SetSize(15, 19);
		pcColor->SetTextColor(RGB(255, 255, 255));
		pcColor->SetHotTextColor(RGB(255, 255, 255));
		pcColor->SetOuterStyle(OuterStyle::AutoSize, true);
		pcColor->Anchor = (AnchorStyles::Type)(AnchorStyles::Bottom | AnchorStyles::Left);
		pcColor->BestSize(true);
		pageCfg1->AddControl(pcColor);
		pcBack = new PickColorCtl();
		pcBack->SetName(_T("pcBack"));
		pcBack->SetText(_T("BackColor"));
		//pcBack->SetBackground(label->GetBackground());
		pcBack->SetLocationOffset(pcColor, false, 2, 0);
		pcBack->SetSize(15, 19);
		pcBack->SetTextColor(RGB(255, 255, 255));
		pcBack->SetHotTextColor(RGB(255, 255, 255));
		pcBack->SetOuterStyle(OuterStyle::AutoSize, true);
		pcBack->Anchor = (AnchorStyles::Type)(AnchorStyles::Bottom | AnchorStyles::Left);
		pcBack->BestSize(true);
		pageCfg1->AddControl(pcBack);

		pcHotColor = new PickColorCtl();
		pcHotColor->SetName(_T("pcHotColor"));
		pcHotColor->SetText(_T("HotColor"));
		//pcHotColor->SetBackground(label->GetBackground());
		pcHotColor->SetLocationOffset(pcColor, true, 0, 2);
		pcHotColor->SetSize(15, 19);
		pcHotColor->SetTextColor(RGB(255, 255, 255));
		pcHotColor->SetHotTextColor(RGB(255, 255, 255));
		pcHotColor->SetOuterStyle(OuterStyle::AutoSize, true);
		pcHotColor->Anchor = (AnchorStyles::Type)(AnchorStyles::Bottom | AnchorStyles::Left);
		pcHotColor->BestSize(true);
		pageCfg1->AddControl(pcHotColor);
		pcHotBack = new PickColorCtl();
		pcHotBack->SetName(_T("pcHotBack"));
		pcHotBack->SetText(_T("HotBackColor"));
		//pcHotBack->SetBackground(label->GetBackground());
		pcHotBack->SetLocationOffset(pcHotColor, false, 2, 0);
		pcHotBack->SetSize(15, 19);
		pcHotBack->SetTextColor(RGB(255, 255, 255));
		pcHotBack->SetHotTextColor(RGB(255, 255, 255));
		pcHotBack->SetOuterStyle(OuterStyle::AutoSize, true);
		pcHotBack->Anchor = (AnchorStyles::Type)(AnchorStyles::Bottom | AnchorStyles::Left);
		pcHotBack->BestSize(true);
		pageCfg1->AddControl(pcHotBack);
		pcColor->SetVisible(false);
		pcHotColor->SetVisible(false);
		pcBack->SetVisible(false);
		pcHotBack->SetVisible(false);
		pcColor->ValueChanged += EventHandler((Object*)this, (EventFun)&CC_DesignBar::Control_Click);
		pcHotColor->ValueChanged += EventHandler((Object*)this, (EventFun)&CC_DesignBar::Control_Click);
		pcBack->ValueChanged += EventHandler((Object*)this, (EventFun)&CC_DesignBar::Control_Click);
		pcHotBack->ValueChanged += EventHandler((Object*)this, (EventFun)&CC_DesignBar::Control_Click);

		//anche
		wchar_t wchLeft[] = {0x2190,0x0000}; //<--
		wchar_t wchTop[] = {0x2191,0x0000}; //^
		wchar_t wchRight[] = {0x2192,0x0000}; //-->
		wchar_t wchBottom[] = {0x2193,0x0000}; //V
		pbtnLeft = new PushButton();
		pbtnLeft->SetName(_T("pbtnLeft"));
		pbtnLeft->SetText(cc::Str(wchLeft));
		//pbtnLeft->SetBackground(label->GetBackground());
		pbtnLeft->SetLocationOffset(btnRunCls, false, 4, 10);
		pbtnLeft->SetSize(20, 19);
		pbtnLeft->SetOuterStyle(OuterStyle::AutoSize, false);
		pbtnLeft->Anchor = (AnchorStyles::Type)(AnchorStyles::Bottom | AnchorStyles::Left);
		pageCfg1->AddControl(pbtnLeft);
		pbtnTop = new PushButton();
		pbtnTop->SetName(_T("pbtnTop"));
		pbtnTop->SetText(cc::Str(wchTop));
		//pbtnTop->SetBackground(label->GetBackground());
		pbtnTop->SetLocationOffset(pbtnLeft, false, 3, -10);
		pbtnTop->SetSize(20, 19);
		pbtnTop->SetOuterStyle(OuterStyle::AutoSize, false);
		pbtnTop->Anchor = (AnchorStyles::Type)(AnchorStyles::Bottom | AnchorStyles::Left);
		pageCfg1->AddControl(pbtnTop);

		pbtnBottom = new PushButton();
		pbtnBottom->SetName(_T("pbtnBottom"));
		pbtnBottom->SetText(cc::Str(wchBottom));
		//pbtnBottom->SetBackground(label->GetBackground());
		pbtnBottom->SetLocationOffset(pbtnTop, true, 0, 2);
		pbtnBottom->SetSize(20, 19);
		pbtnBottom->SetOuterStyle(OuterStyle::AutoSize, false);
		pbtnBottom->Anchor = (AnchorStyles::Type)(AnchorStyles::Bottom | AnchorStyles::Left);
		pageCfg1->AddControl(pbtnBottom);
		pbtnRight = new PushButton();
		pbtnRight->SetName(_T("pbtnRight"));
		pbtnRight->SetText(cc::Str(wchRight));
		//pbtnRight->SetBackground(label->GetBackground());
		pbtnRight->SetLocationOffset(pbtnBottom, false, 3, -10);
		pbtnRight->SetSize(20, 19);
		pbtnRight->SetOuterStyle(OuterStyle::AutoSize, false);
		pbtnRight->Anchor = (AnchorStyles::Type)(AnchorStyles::Bottom | AnchorStyles::Left);
		pageCfg1->AddControl(pbtnRight);
		pbtnLeft->SetVisible(false);
		pbtnTop->SetVisible(false);
		pbtnRight->SetVisible(false);
		pbtnBottom->SetVisible(false);

		editVItem = new TextBox();
		editVItem->SetName(_T("editVItem"));
		editVItem->SetText(_T(""));
		//editVItem->SetMaxLength(ITEM_MAX_LENGTH);
		editVItem->SetLocationOffset(btnRunCls, false, 4, 0);
		editVItem->SetSize(600 - 5 - 20 - 60 - 15, 17);
		editVItem->Anchor = (AnchorStyles::Type)(AnchorStyles::Bottom | AnchorStyles::Left | AnchorStyles::Right);
		editVItem->SetVisible(false);
		pageCfg1->AddControl(editVItem);
		btnSelFrom = new PathButton();
		btnSelFrom->SetLocationOffset(editVItem, false, 0, 0);
		btnSelFrom->SetSize(15, 17);
		btnSelFrom->IsFile = true;
		btnSelFrom->PathControl = editVItem;
		btnSelFrom->Anchor = (AnchorStyles::Type)(AnchorStyles::Bottom | AnchorStyles::Right);
		btnSelFrom->MinPosition.x = 80;
		btnSelFrom->tcCaption = _T("select Image file:");
		btnSelFrom->tcFilter = _T("Image Files (*.jpg;*.gif;*.tif;*.bmp;*.ico)\0*.jpg;*.gif;*.tif;*.bmp;*.ico\0\0");
		btnSelFrom->tcDefaultExt = _T("gif");
		//btnSelFrom->Click += EventHandler((Object*)this, (EventFun)&CC_DesignBar::Control_Click);
		pageCfg1->AddControl(btnSelFrom);
		btnSelFrom->SetVisible(false);

		editVItemM = new TextBox();
		editVItemM->SetName(_T("editVItemM"));
		editVItemM->SetText(_T(""));
		//editVItemM->SetMaxLength(ITEM_MAX_LENGTH);
		editVItemM->SetLocationOffset(btnRunCls, false, 4, 0);
		editVItemM->SetSize(600 - 5 - 20 - 60, 37);
		editVItemM->SetMultiline(true);
		editVItemM->Anchor = (AnchorStyles::Type)(AnchorStyles::Bottom | AnchorStyles::Left | AnchorStyles::Right);
		editVItemM->SetVisible(false);
		pageCfg1->AddControl(editVItemM);

		//pageCfg2
		pageCfg2 = new TabPage();
		pageCfg2->SetName(_T("page2"));
		pageCfg2->SetText(_T("CMD"));
		pageCfg2->SetClientSize(600, 600);
		pageCfg2->SetOuterStyle(OuterStyle::TransparentTop, true);
		tab1->AddPage(pageCfg2);

		nTop = 1;
		chkDebug = new CheckBox();
		chkDebug->SetText(_T("IsDebug"));
		chkDebug->SetLocation(2, nTop);
		chkDebug->SetSize(100, 17);
		chkDebug->SetOuterStyle(OuterStyle::AutoSize, true);
		if(CC_APP_GetConfig()->GetIsDebug())
		{
			chkDebug->SetChecked(true);
		}
		chkDebug->Click += EventHandler((Object*)this, (EventFun)&CC_DesignBar::Control_Click);
		pageCfg2->AddControl(chkDebug);

		nTop += 20;
		chkDesign = new CheckBox();
		chkDesign->SetText(_T("IsDesign"));
		chkDesign->SetLocation(2, nTop);
		chkDesign->SetSize(100, 17);
		chkDesign->SetOuterStyle(OuterStyle::AutoSize, true);
		chkDesign->Click += EventHandler((Object*)this, (EventFun)&CC_DesignBar::Control_Click);
		pageCfg2->AddControl(chkDesign);

		nTop += 25;
		btnOpenLogWin = new Button();
		btnOpenLogWin->SetText(_T("Open LogWindow"));
		btnOpenLogWin->SetLocation(2, nTop);
		btnOpenLogWin->SetSize(160, 19);
		btnOpenLogWin->Click += EventHandler((Object*)this, (EventFun)&CC_DesignBar::Control_Click);
		pageCfg2->AddControl(btnOpenLogWin);

		nTop += 20;
		btnOutLang = new Button();
		btnOutLang->SetText(_T("Write Config to Lang"));
		btnOutLang->SetLocation(2, nTop);
		btnOutLang->SetSize(160, 19);
		btnOutLang->Click += EventHandler((Object*)this, (EventFun)&CC_DesignBar::Control_Click);
		pageCfg2->AddControl(btnOutLang);

		nTop += 20;
		btnDumpConfig = new Button();
		btnDumpConfig->SetText(_T("Dump Config(Lang&Ini)"));
		btnDumpConfig->SetLocation(2, nTop);
		btnDumpConfig->SetSize(160, 19);
		btnDumpConfig->Click += EventHandler((Object*)this, (EventFun)&CC_DesignBar::Control_Click);
		pageCfg2->AddControl(btnDumpConfig);

		nTop += 20;
		btnCreateSource = new Button();
		btnCreateSource->SetText(_T("Output Source of Current Form"));
		btnCreateSource->SetLocation(2, nTop);
		btnCreateSource->SetSize(200, 19);
		btnCreateSource->Click += EventHandler((Object*)this, (EventFun)&CC_DesignBar::Control_Click);
		pageCfg2->AddControl(btnCreateSource);

		//pageCfg3
		pageCfg3 = new TabPage();
		pageCfg3->SetName(_T("page3"));
		pageCfg3->SetText(_T("New Control"));
		pageCfg3->SetClientSize(600, 600);
		pageCfg3->SetOuterStyle(OuterStyle::TransparentTop, true);
		tab1->AddPage(pageCfg3);

		nTop = 1;
		label = new Label();
		label->SetName(_T("labType"));
		label->SetText(_T("Type:"));
		label->SetLocation(2, nTop + 2);
		label->SetSize(80, 17);
		pageCfg3->AddControl(label);
		comboType = new ComboBox();
		comboType->SetName(_T("comboType"));
		comboType->SetText(_T("Label;	Button;	CheckBox;	RadioButton;	ToolItem;	LabelLink;	TextBox;	GradientLabel;	"));
		comboType->SetComboBoxStyle(ComboBoxStyle::DropDownList);
		comboType->SetLocationOffset(label, false, 0, -2);
		comboType->SetSize(600 - 5 - 80, 145);
		//comboType->SelectedIndexChanged += EventHandler((Object*)this, (EventFun)&CC_DesignBar::comboType_SelectedIndexChanged);
		comboType->Anchor = (AnchorStyles::Type)(AnchorStyles::Top | AnchorStyles::Left | AnchorStyles::Right);
		pageCfg3->AddControl(comboType);

		nTop += 22;
		label = new Label();
		label->SetName(_T("labName"));
		label->SetText(_T("Name:"));
		label->SetLocation(2, nTop + 2);
		label->SetSize(80, 17);
		pageCfg3->AddControl(label);
		editName = new TextBox();
		editName->SetName(_T("editName"));
		editName->SetText(_T(""));
		editName->SetTips(_T("name is no change after cteated."));
		editName->SetLocationOffset(label, false, 0, -2);
		editName->SetSize(600 - 5 - 80, 17);
		editName->Anchor = (AnchorStyles::Type)(AnchorStyles::Top | AnchorStyles::Left | AnchorStyles::Right);
		pageCfg3->AddControl(editName);

		nTop += 20;
		label = new Label();
		label->SetName(_T("labName"));
		label->SetText(_T("Cfg Name:"));
		label->SetLocation(2, nTop + 2);
		label->SetSize(80, 17);
		pageCfg3->AddControl(label);
		editNameCfg = new TextBox();
		editNameCfg->SetName(_T("editNameCfg"));
		editNameCfg->SetText(_T(""));
		editNameCfg->SetTips(_T("config name is no change after cteated,and if no input,then is same as name."));
		editNameCfg->SetLocationOffset(label, false, 0, -2);
		editNameCfg->SetSize(600 - 5 - 80, 17);
		editNameCfg->Anchor = (AnchorStyles::Type)(AnchorStyles::Top | AnchorStyles::Left | AnchorStyles::Right);
		pageCfg3->AddControl(editNameCfg);

		nTop += 20;
		label = new Label();
		label->SetName(_T("labRect"));
		label->SetText(_T("Rect(X,Y,W,H):"));
		label->SetLocation(2, nTop + 2);
		label->SetSize(80, 17);
		pageCfg3->AddControl(label);
		editCtlX = new TextBox();
		editCtlX->SetName(_T("editCtlX"));
		editCtlX->SetLocationOffset(label, false, 0, -2);
		editCtlX->SetSize(28, 17);
		editCtlX->SetStyleNumber(true);
		pageCfg3->AddControl(editCtlX);
		editCtlY = new TextBox();
		editCtlY->SetName(_T("editCtlY"));
		editCtlY->SetLocationOffset(editCtlX, false, 4, 0);
		editCtlY->SetSize(28, 17);
		editCtlY->SetStyleNumber(true);
		pageCfg3->AddControl(editCtlY);
		editCtlW = new TextBox();
		editCtlW->SetName(_T("editCtlW"));
		editCtlW->SetLocationOffset(editCtlY, false, 4, 0);
		editCtlW->SetSize(28, 17);
		editCtlW->SetStyleNumber(true);
		pageCfg3->AddControl(editCtlW);
		editCtlH = new TextBox();
		editCtlH->SetName(_T("editCtlH"));
		editCtlH->SetLocationOffset(editCtlW, false, 4, 0);
		editCtlH->SetSize(28, 17);
		editCtlH->SetStyleNumber(true);
		pageCfg3->AddControl(editCtlH);
		editCtlX->SetText(_T("0"));
		editCtlY->SetText(_T("0"));
		editCtlW->SetText(_T("100"));
		editCtlH->SetText(_T("17"));

		nTop += 20;
		btnNew = new Button();
		btnNew->SetName(_T("btnNew"));
		btnNew->SetText(_T("New"));
		btnNew->SetLocation(2, nTop);
		btnNew->SetSize(80, 17);
		btnNew->Click += EventHandler((Object*)this, (EventFun)&CC_DesignBar::Control_Click);
		btnNew->SetTips(_T("After changed something, must click Set."));
		pageCfg3->AddControl(btnNew);


		KeyArr* keyArr = CC_APP_GetIni();
		int nConsoleX = keyArr->Get(_T("Main.Design.Left"), 0);
		int nConsoleY = keyArr->Get(_T("Main.Design.Top"), 0);
		int nConsoleW = keyArr->Get(_T("Main.Design.Width"), 300);
		int nConsoleH = keyArr->Get(_T("Main.Design.Heigh"), 200);
		cc::Util::AdjustWindowRect(nConsoleX, nConsoleY, nConsoleW, nConsoleH, _minSize.cx, _minSize.cy, _maxSize.cx, _maxSize.cy);
		this->SetClientSize(600, 600);
		this->SetBounds(nConsoleX, nConsoleY, nConsoleW, nConsoleH);
		//this->SetMinSize(CC_DESIGNTB_WIDTH + 10, nTop + 20);
		this->SetName(_T("tbDesign"));
		this->SetText(_T("Design Toolbar"));
		//this->SetInitDockPosition(DockStyles::None);
		//this->SetDockable(DockStyles::None);

		SetLang(*CC_APP_GetLang(), cc::Str(CC_CONFIG_LANGSECTION));
	}

	virtual void SetVisible(bool value)
	{
		Container::SetVisible(value);
		if(value)
		{
			if(comboCtl->m_hWnd != NULL)
			{
				comboCtl->SetSelectedIndex(-1);
				comboCtl_SelectedIndexChanged(NULL, NULL);
			}
			CC_APP_GetConfig()->SetIsDesign(true);
			chkDesign->SetChecked(true);
			CC_BIT_OFF(_Flags, CC_FLAG_DESIGN_ISCALLEDCLOSED);
		}
	}

	void Control_Click(Object* sender, EventArgs* e)
	{
		if(sender == pickX || sender == pickY || sender == pickW || sender == pickH)
		{
			//nActionType = 5; //1:Text;2:Tips;3:TipsExt;4:Color;5:Left;6:XXX;7:Anchor;8:URL;9:Style;
			if(pLastCtl == NULL || nDoEnd != 1 || nActionType != 5)
			{
				return;
			}
			int nX = pickX->GetValue();
			int nY = pickY->GetValue();
			int nW = pickW->GetValue();
			int nH = pickH->GetValue();
			pLastCtl->SetBoundsNotOriginal(nX, nY, nW, nH);
		}
		else if(sender == pcColor || sender == pcHotColor || sender == pcBack || sender == pcHotBack)
		{
			if(pLastCtl == NULL || nDoEnd != 1)
			{
				return;
			}
			COLORREF crText = pcColor->GetBackground();
			COLORREF crHotText = pcHotColor->GetBackground();
			COLORREF crBack = pcBack->GetBackground();
			COLORREF crHotback = pcHotBack->GetBackground();
			if(crText != pLastCtl->GetTextColor())
			{
				pLastCtl->SetTextColor(crText);
			}
			if(crHotText != pLastCtl->GetHotTextColor())
			{
				pLastCtl->SetHotTextColor(crHotText);
			}
			if(crBack != pLastCtl->GetBackground())
			{
				pLastCtl->SetBackground(crBack);
			}
			if(crHotback != pLastCtl->GetHotBackColor())
			{
				pLastCtl->SetHotBackColor(crHotback);
			}
		}
		else if(sender == btnCopyName)
		{
			if(pLastCtl != NULL)
			{
				cc::Util::SetClipboardText(cc::Str(pLastCtl->GetName()));
			}
			return;
		}
		else if(sender == btnPrintCtl)
		{
			if(pLastCtl != NULL)
			{
				cc::Str sTxt;
				cc::Str sVarlist;
				pLastCtl->CreateSource(sTxt, sVarlist, cc::Str(_T("Main")));
				sVarlist.Append(_T("\r\n")).Append(sTxt);
				cc::Util::SetClipboardText(sVarlist);
			}
			return;
		}
		else if(sender == btnCopyLang)
		{
			cc::Str sKeyLang = CC_APP_GetIni()->Get(CC_CONFIG_LANGFILE);
			sKeyLang.Append(_T("."));
			KeyArr kaSave;
			kaSave.Add(*CC_APP_GetIni(), (TCHAR*)sKeyLang, (TCHAR*)NULL);
			cc::Str sTxt;
			kaSave.SaveToString(sTxt);
			cc::Util::SetClipboardText(sTxt);
			return;
		}
		else if(sender == btnClearDesign)
		{
			if(IDYES != (int)::MessageBox(m_hWnd, TCHAR_LANGDEF("Lang.SD_ClearDesign", _T("Clear current lang's config and all design config?")), TCHAR_LANG("Lang.S_MsgBox_Title_Question"), MB_YESNO | MB_ICONQUESTION))
			{
				return;
			}
			cc::Str sKeyLang = CC_APP_GetIni()->Get(CC_CONFIG_LANGFILE);
			sKeyLang.Append(_T("."));
			CC_APP_GetIni()->Clear(sKeyLang);
			CC_APP_GetIni()->Clear(_T("Design."));
			return;
		}
		else if(sender == btnSet || sender == btnRunCls)
		{
			comboCtl_SelectedIndexChanged(NULL, NULL);
			if(pLastCtl == NULL || sSelKey.GetLength() <= 0)
			{
				return;
			}
			//get ConfigName
			cc::Str sKey;
			cc::Str sKeyFlag;
			cc::Str sKeyLang;
			cc::Str sKeyLangFlag;
			GetCfgKey(pLastCtl, sSelKey, sKey, sKeyFlag, sKeyLang, sKeyLangFlag);
			DWORD nDFlag = CC_APP_GetIni()->Get(sKeyFlag, (DWORD)0);
			if(sender == btnRunCls)
			{
				if(sSelKey.Equals(_T("Left:")))
				{
					nDFlag &= ~CC_DESIGN_DS_RECT;
					//if((nDFlag & CC_DESIGN_DS_AUTOSIZE) != 0)
					//{
					//	pLastCtl->SetOuterStyle(OuterStyle::AutoSize, true);
					//}
				}
				else if(sSelKey.Equals(_T("Color:")))
				{
					nDFlag &= ~CC_DESIGN_DS_COLOR;
				}
				else if(sSelKey.Equals(_T("Style:")))
				{
					int nIndStyle = comboStyle->GetSelectedIndex();
					if(nIndStyle >= 0)
					{
						cc::Str sStyle = comboStyle->ItemGet(nIndStyle);
						if(sStyle.StartsWith(_T("AutoSize:")))
						{
							nDFlag &= ~CC_DESIGN_DS_ASIZE;
						}
						else
						{
							nDFlag &= ~CC_DESIGN_DS_BACK;
							CC_APP_GetIni()->Set(sKey + _T("_Img"), (const TCHAR*)NULL);
							CC_APP_GetLang()->Set(sKeyLang + _T("_Img"), (const TCHAR*)NULL);
						}
					}
				}
				//else if(sSelKey.Equals(_T("BackImage:")))
				//{
				//	nDFlag &= ~CC_DESIGN_DS_BIMG;
				//	if((nDFlag & CC_DESIGN_DS_NOTRANSPARENTTOP) == 0)
				//	{
				//		pLastCtl->SetOuterStyle(OuterStyle::TransparentTop, false);
				//	}
				//	pLastCtl->SetStyle(StyleType::Inner,
				//		InnerStyle::BackgroundBrush|InnerStyle::BackgroundBmp|InnerStyle::BackgroundTheme, InnerStyle::BackgroundColor);
				//}
				else if(sSelKey.Equals(_T("Anchor:")))
				{
					nDFlag &= ~CC_DESIGN_DS_ANCHOR;
				}
				if((nDFlag & (CC_DESIGN_DS_RECT|CC_DESIGN_DS_COLOR|CC_DESIGN_DS_ANCHOR|CC_DESIGN_DS_CTLS|CC_DESIGN_DS_ASIZE|CC_DESIGN_DS_BACK)) == 0)
				{
					CC_APP_GetIni()->Set(sKeyFlag, (const TCHAR*)NULL);
					CC_APP_GetLang()->Set(sKeyLangFlag, (const TCHAR*)NULL);
				}
				else
				{
					CC_APP_GetIni()->Set(sKeyFlag, nDFlag);
					CC_APP_GetLang()->Set(sKeyLangFlag, nDFlag);
				}
				//clear
				CC_APP_GetIni()->Set(sKey, (const TCHAR*)NULL);
				CC_APP_GetLang()->Set(sKeyLang, (const TCHAR*)NULL);
				//this time not restore nSaveW or nSaveColor
				comboCtl_SelectedIndexChanged(sender, NULL);
				return;
			}

			cc::Str sVal;
			const TCHAR* tc = NULL;
			if(sSelKey.Equals(_T("Text:")))
			{
				tc = editVItemM->GetText();
				pLastCtl->SetText(tc);
				sVal.ValueOf(tc);
				if(pLastCtl->HasStyle(StyleType::Outer, OuterStyle::AutoSize))
				{
					SIZE size = pLastCtl->BestSize(true);
					nSaveW = size.cx;
					nSaveH = size.cy;
				}
			}
			else if(sSelKey.Equals(_T("Tips:")))
			{
				tc = editVItem->GetText();
				pLastCtl->SetTips(tc);
				sVal.ValueOf(tc);
			}
			else if(sSelKey.Equals(_T("TipsExt:")))
			{
				tc = editVItemM->GetText();
				pLastCtl->SetTipsExt(tc);
				sVal.ValueOf(tc);
			}
			else if(sSelKey.Equals(_T("URL:")))
			{
				tc = editVItem->GetText();
				((LabelLink*)pLastCtl)->SetURL(tc);
				sVal.ValueOf(tc);
			}
			//else if(sSelKey.Equals(_T("BackImage:")))
			//{
			//	tc = editVItem->GetText();
			//	if(cc::Util::fExist(tc))
			//	{
			//		nDFlag |= CC_DESIGN_DS_BIMG;
			//		if(!pLastCtl->HasStyle(StyleType::Outer, OuterStyle::TransparentTop))
			//		{
			//			nDFlag |= CC_DESIGN_DS_NOTRANSPARENTTOP;
			//			pLastCtl->SetOuterStyle(OuterStyle::TransparentTop, true);
			//		}
			//		pLastCtl->SetBackgroundImage(tc, true);
			//		sVal.ValueOf(tc);
			//	}
			//}
			else if(sSelKey.Equals(_T("Anchor:")))
			{
				nDFlag |= CC_DESIGN_DS_ANCHOR;
				DWORD dwAnchor = 0;
				if(pbtnLeft->GetPushed())
				{
					dwAnchor |= AnchorStyles::Left;
				}
				if(pbtnTop->GetPushed())
				{
					dwAnchor |= AnchorStyles::Top;
				}
				if(pbtnRight->GetPushed())
				{
					dwAnchor |= AnchorStyles::Right;
				}
				if(pbtnBottom->GetPushed())
				{
					dwAnchor |= AnchorStyles::Bottom;
				}
				if(dwAnchor == 0)
				{
					dwAnchor = (AnchorStyles::Top | AnchorStyles::Left);
				}
				pLastCtl->Anchor = (AnchorStyles::Type)dwAnchor;
				//sVal.ValueOf((int)dwAnchor);
				dwSaveAnchor = dwAnchor;
				pLastCtl->MinPosition.x = pickW->GetValue();
				pLastCtl->MinPosition.y = pickH->GetValue();
				sVal.Format(_T("%d,%d,%d,"), (int)dwAnchor, pickW->GetValue(), pickH->GetValue());
				//RECT rect = pLastCtl->GetBounds();
				//pLastCtl->SetBoundsCalcuteOriginal(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);
			}
			else if(sSelKey.Equals(_T("Style:")))
			{
				int nIndStyle = comboStyle->GetSelectedIndex();
				if(nIndStyle >= 0)
				{
					cc::Str sStyle = comboStyle->ItemGet(nIndStyle);
					//AutoSize;	Color Back;	Gradient Back(Left-Right);	Gradient Back(Up-Down);	Image Back;	"));
					if(sStyle.Equals(_T("AutoSize:On")))
					{
						pLastCtl->SetOuterStyle(OuterStyle::AutoSize, true);
						pLastCtl->BestSize(true);
						nDFlag = CC_DESIGN_DS_ASIZE | nDFlag | CC_DESIGN_DS_ASIZEON;
						::MessageBox(m_hWnd, TCHAR_LANG("Lang.SD_NeedClearRect"), TCHAR_LANG("Lang.S_MsgBox_Title_Critical"), MB_OK | MB_ICONSTOP);
					}
					else if(sStyle.Equals(_T("AutoSize:Off")))
					{
						pLastCtl->SetOuterStyle(OuterStyle::AutoSize, false);
						nDFlag = CC_DESIGN_DS_ASIZE | (nDFlag & ~CC_DESIGN_DS_ASIZEON);
					}
					else if(sStyle.Equals(_T("Transparent")))
					{
						pLastCtl->SetOuterStyle(OuterStyle::TransparentTop, false);
						nDFlag = CC_DESIGN_DS_BACK | (nDFlag & ~(CC_DESIGN_DS_BACK1|CC_DESIGN_DS_BACK2|CC_DESIGN_DS_BACK3|CC_DESIGN_DS_BACK4));
						::InvalidateRect(pLastCtl->m_hWnd, NULL, TRUE);
					}
					else if(sStyle.Equals(_T("Color Back")))
					{
						pLastCtl->SetOuterStyle(OuterStyle::TransparentTop, true);
						pLastCtl->SetStyle(StyleType::Inner,
							InnerStyle::BackgroundBrush|InnerStyle::BackgroundGradient|InnerStyle::BackgroundBmp|InnerStyle::BackgroundTheme,
							InnerStyle::BackgroundColor);
						nDFlag = CC_DESIGN_DS_BACK | CC_DESIGN_DS_BACK1 | (nDFlag & ~(CC_DESIGN_DS_BACK2|CC_DESIGN_DS_BACK3|CC_DESIGN_DS_BACK4));
					}
					else if(sStyle.Equals(_T("Gradient Back(Left-Right)")))
					{
						pLastCtl->SetOuterStyle(OuterStyle::TransparentTop, true);
						pLastCtl->SetStyleBool(StyleType::Inner2, Inner2Style::GradientVertical, false);
						pLastCtl->SetStyle(StyleType::Inner,
							InnerStyle::BackgroundBrush|InnerStyle::BackgroundColor|InnerStyle::BackgroundBmp|InnerStyle::BackgroundTheme,
							InnerStyle::BackgroundGradient);
						nDFlag = CC_DESIGN_DS_BACK | CC_DESIGN_DS_BACK2 | (nDFlag & ~(CC_DESIGN_DS_BACK1|CC_DESIGN_DS_BACK3|CC_DESIGN_DS_BACK4));
					}
					else if(sStyle.Equals(_T("Gradient Back(Up-Down)")))
					{
						pLastCtl->SetOuterStyle(OuterStyle::TransparentTop, true);
						pLastCtl->SetStyleBool(StyleType::Inner2, Inner2Style::GradientVertical, true);
						pLastCtl->SetStyle(StyleType::Inner,
							InnerStyle::BackgroundBrush|InnerStyle::BackgroundColor|InnerStyle::BackgroundBmp|InnerStyle::BackgroundTheme,
							InnerStyle::BackgroundGradient);
						nDFlag = CC_DESIGN_DS_BACK | CC_DESIGN_DS_BACK3 | (nDFlag & ~(CC_DESIGN_DS_BACK1|CC_DESIGN_DS_BACK2|CC_DESIGN_DS_BACK4));
					}
					else if(sStyle.Equals(_T("Image Back")))
					{
						tc = editVItem->GetText();
						if(!cc::Util::fExist(tc))
						{
							::MessageBox(m_hWnd, TCHAR_LANG("Lang.SD_NeedImgFile"), TCHAR_LANG("Lang.S_MsgBox_Title_Critical"), MB_OK | MB_ICONSTOP);
						}
						else
						{
							pLastCtl->SetOuterStyle(OuterStyle::TransparentTop, true);
							if(pLastCtl->HasStyle(StyleType::Inner, InnerStyle::BackgroundGradient))
							{
								pLastCtl->SetStyle(StyleType::Inner, InnerStyle::BackgroundGradient, 0);
							}
							nDFlag = CC_DESIGN_DS_BACK | CC_DESIGN_DS_BACK4 | (nDFlag & ~(CC_DESIGN_DS_BACK1|CC_DESIGN_DS_BACK2|CC_DESIGN_DS_BACK3));
							pLastCtl->SetBackgroundImage(tc, true);
							CC_APP_GetIni()->Set(sKey + _T("_Img"), tc);
							CC_APP_GetLang()->Set(sKeyLang + _T("_Img"), tc);
						}
					}
				}
			}
			else if(sSelKey.Equals(_T("Left:")))
			{
				nSaveX = pickX->GetValue();
				nSaveY = pickY->GetValue();
				nSaveW = pickW->GetValue();
				nSaveH = pickH->GetValue();
				isRestoreRect = 1;
				if(pLastCtl->HasStyle(StyleType::Outer, OuterStyle::AutoSize))
				{
					pLastCtl->SetOuterStyle(OuterStyle::AutoSize, false);
				}
				pLastCtl->SetBoundsCalcuteOriginal(nSaveX, nSaveY, nSaveW, nSaveH);

				//cannot get orignal rect,so only set every time
				RECT rectOrg = pLastCtl->GetOriginalBounds();
				sVal.Format(_T("%d,%d,%d,%d,"), rectOrg.left, rectOrg.top, rectOrg.right - rectOrg.left, rectOrg.bottom - rectOrg.top);
				nDFlag |= CC_DESIGN_DS_RECT;
			}
			else if(sSelKey.Equals(_T("Color:")))
			{
				crSaveText = pcColor->GetBackground();
				crSaveHotText = pcHotColor->GetBackground();
				crSaveBack = pcBack->GetBackground();
				crSaveHotback = pcHotBack->GetBackground();
				pLastCtl->SetTextColor(crSaveText);
				pLastCtl->SetHotTextColor(crSaveHotText);
				pLastCtl->SetBackground(crSaveBack);
				pLastCtl->SetHotBackColor(crSaveHotback);
				sVal.Format(_T("%d,%d,%d,%d,"), crSaveText, crSaveHotText, crSaveBack, crSaveHotback);
				nDFlag |= CC_DESIGN_DS_COLOR;
				if(pLastCtl->HasStyle(StyleType::Inner, InnerStyle::BackgroundGradient))
				{
					pLastCtl->SetStyle(StyleType::Inner, 0, InnerStyle::BackgroundGradient);
				}
			}
			if((nDFlag & (CC_DESIGN_DS_RECT|CC_DESIGN_DS_COLOR|CC_DESIGN_DS_ANCHOR|CC_DESIGN_DS_CTLS|CC_DESIGN_DS_ASIZE|CC_DESIGN_DS_BACK)) != 0)
			{
				CC_APP_GetIni()->Set(sKeyFlag, nDFlag);
				CC_APP_GetLang()->Set(sKeyLangFlag, nDFlag);
			}
			if(!sVal.IsNULL())
			{
				CC_APP_GetIni()->Set(sKey, sVal);
				CC_APP_GetLang()->Set(sKeyLang, sVal);
			}
			//this time not restore nSaveW or nSaveColor
			comboCtl_SelectedIndexChanged(sender, NULL);
		}
		else if(sender == btnRunRef)
		{
			comboCtl_SelectedIndexChanged(NULL, NULL);
		}
		else if(sender == btnNew)
		{
			int nInd = comboCtl->GetSelectedIndex();
			Control* parent = NULL;
			if(nInd >= 0)
			{
				parent = Control::FromHandle((HWND)comboCtl->GetItemData(nInd));
			}
			if(parent == NULL)
			{
				::MessageBox(m_hWnd, TCHAR_LANG("Lang.SD_NeedParent"), TCHAR_LANG("Lang.S_MsgBox_Title_Critical"), MB_OK | MB_ICONSTOP);
				return;
			}
			if((parent->Design & DesignStyle::CanAddControl) == 0)
			{
				::MessageBox(m_hWnd, TCHAR_LANG("Lang.SD_NoAddControl"), TCHAR_LANG("Lang.S_MsgBox_Title_Critical"), MB_OK | MB_ICONSTOP);
				return;
			}

			nInd = comboType->GetSelectedIndex();
			cc::Str sName = cc::Str(editName->GetText()).Trim();
			if(nInd < 0 || sName.GetLength() < 1 || sName.IndexOf(_T(" ")) >= 0)
			{
				::MessageBox(m_hWnd, TCHAR_LANG("Lang.SD_NeedTyNm"), TCHAR_LANG("Lang.S_MsgBox_Title_Critical"), MB_OK | MB_ICONSTOP);
				return;
			}
			if(!sName.IsAlpha(0))
			{
				::MessageBox(m_hWnd, TCHAR_LANG("Lang.SD_NeedFirstAlpha"), TCHAR_LANG("Lang.S_MsgBox_Title_Critical"), MB_OK | MB_ICONSTOP);
				return;
			}
			//check from global,avoid same name
			if(CheckSameName(_pParent, sName))
			{
				::MessageBox(m_hWnd, TCHAR_LANG("Lang.SD_SameNm"), TCHAR_LANG("Lang.S_MsgBox_Title_Critical"), MB_OK | MB_ICONSTOP);
				return;
			}
			if(CheckSameName(parent, sName))
			{
				::MessageBox(m_hWnd, TCHAR_LANG("Lang.SD_SameNm"), TCHAR_LANG("Lang.S_MsgBox_Title_Critical"), MB_OK | MB_ICONSTOP);
				return;
			}

			cc::Str sClassType = cc::Str(comboType->ItemGet(nInd));
			Control* control = CC_APP_CreateControl(sClassType);
			if(control == NULL)
			{
				return;
			}
			control->SetName(sName);
			cc::Str sCfgName = cc::Str(editNameCfg->GetText());
			if(sCfgName.GetLength() > 0)
			{
				control->SetConfigName(sCfgName);
			}
			control->Design = (DesignStyle::Type)((DWORD)control->Design | DesignStyle::DynamicCreated);
			control->SetText(_T("Dynamic Control"));
			int nX = editCtlX->GetTextInt();
			int nY = editCtlY->GetTextInt();
			control->SetBounds(nX, nY, editCtlW->GetTextInt(), editCtlH->GetTextInt());
			parent->AddControl(control);
			AddOneWindow(control->m_hWnd, _T("[New]"));

			//get ConfigName
			cc::Str sPre = _T("NewControl:");
			cc::Str sKey;
			cc::Str sKeyFlag;
			cc::Str sKeyLang;
			cc::Str sKeyLangFlag;
			GetCfgKey(parent, sPre, sKey, sKeyFlag, sKeyLang, sKeyLangFlag);
			DWORD nDFlag = CC_APP_GetIni()->Get(sKeyFlag, (DWORD)0);
			nDFlag |= CC_DESIGN_DS_CTLS;
			CC_APP_GetIni()->Set(sKeyFlag, nDFlag);

			//save to parent
			DWORD nCount = CC_APP_GetIni()->Get(sKey + _T("._DCtlCount"), (DWORD)0);
			nCount++;
			CC_APP_GetIni()->Set(sKey + _T("._DCtlCount"), nCount);
			CC_APP_GetLang()->Set(sKeyLang + _T("._DCtlCount"), nCount);
			//Name
			sPre.Format(_T("%s._DCtl%d.Name"), (TCHAR*)sKey.GetBuf(), nCount);
			CC_APP_GetIni()->Set(sPre, sName);
			sPre.Format(_T("%s._DCtl%d.Name"), (TCHAR*)sKeyLang.GetBuf(), nCount);
			CC_APP_GetLang()->Set(sPre, sName);
			//Type
			sPre.Format(_T("%s._DCtl%d.Type"), (TCHAR*)sKey.GetBuf(), nCount);
			CC_APP_GetIni()->Set(sPre, sClassType);
			sPre.Format(_T("%s._DCtl%d.Type"), (TCHAR*)sKeyLang.GetBuf(), nCount);
			CC_APP_GetLang()->Set(sPre, sClassType);
			//CfgName
			if(sCfgName.GetLength() > 0)
			{
				sPre.Format(_T("%s._DCtl%d.CfgName"), (TCHAR*)sKey.GetBuf(), nCount);
				CC_APP_GetIni()->Set(sPre, sCfgName);
				sPre.Format(_T("%s._DCtl%d.CfgName"), (TCHAR*)sKeyLang.GetBuf(), nCount);
				CC_APP_GetLang()->Set(sPre, sCfgName);
			}
		}
		else if(sender == btnRunDel)
		{
			int nIndCtl = comboCtl->GetSelectedIndex();
			Control* control = NULL;
			if(nIndCtl >= 0)
			{
				control = Control::FromHandle((HWND)comboCtl->GetItemData(nIndCtl));
			}
			if(control == NULL)
			{
				::MessageBox(m_hWnd, TCHAR_LANG("Lang.SD_NoControl"), TCHAR_LANG("Lang.S_MsgBox_Title_Critical"), MB_OK | MB_ICONSTOP);
				return;
			}
			if((control->Design & DesignStyle::DynamicCreated) == 0)
			{
				::MessageBox(m_hWnd, TCHAR_LANG("Lang.SD_NoDynamicCreated"), TCHAR_LANG("Lang.S_MsgBox_Title_Critical"), MB_OK | MB_ICONSTOP);
				return;
			}
			//need no child
			if(control->GetControlCount() > 0)
			{
				::MessageBox(m_hWnd, TCHAR_LANG("Lang.SD_HasChild"), TCHAR_LANG("Lang.S_MsgBox_Title_Critical"), MB_OK | MB_ICONSTOP);
				return;
			}
			if(IDYES != (int)::MessageBox(m_hWnd, TCHAR_LANG("Lang.SD_DelCtl"), TCHAR_LANG("Lang.S_MsgBox_Title_Question"), MB_YESNO | MB_ICONQUESTION))
			{
				return;
			}
			Control* parent = control->GetParent();
			cc::Str sName = cc::Str(control->GetName());
			//get ConfigName
			cc::Str sPre = _T("NewControl:");
			cc::Str sPreG;
			cc::Str sKey;
			cc::Str sKeyFlag;
			cc::Str sKeyLang;
			cc::Str sKeyLangFlag;
			GetCfgKey(parent, sPre, sKey, sKeyFlag, sKeyLang, sKeyLangFlag);

			//clear from parent
			int nCount = CC_APP_GetIni()->Get(sKey + _T("._DCtlCount"), (DWORD)0);
			nCount--;
			if(nCount <= 0)
			{
				DWORD nDFlag = CC_APP_GetIni()->Get(sKeyFlag, (DWORD)0);
				nDFlag &= ~CC_DESIGN_DS_CTLS;
				if((nDFlag & (CC_DESIGN_DS_RECT|CC_DESIGN_DS_COLOR|CC_DESIGN_DS_ANCHOR|CC_DESIGN_DS_CTLS|CC_DESIGN_DS_ASIZE|CC_DESIGN_DS_BACK)) == 0)
				{
					CC_APP_GetIni()->Set(sKeyFlag, (const TCHAR*)NULL);
					CC_APP_GetLang()->Set(sKeyLangFlag, (const TCHAR*)NULL);
				}
				else
				{
					CC_APP_GetIni()->Set(sKeyFlag, nDFlag);
					CC_APP_GetLang()->Set(sKeyLangFlag, nDFlag);
				}
				CC_APP_GetIni()->Set(sKey + _T("._DCtlCount"), (const TCHAR*)NULL);
				CC_APP_GetLang()->Set(sKeyLang + _T("._DCtlCount"), (const TCHAR*)NULL);
			}
			else
			{
				CC_APP_GetIni()->Set(sKey + _T("._DCtlCount"), nCount);
				CC_APP_GetLang()->Set(sKeyLang + _T("._DCtlCount"), nCount);
			}
			int nFrom = -1;
			for(int n = 0; n <= nCount; n++)
			{
				//name
				sPre.Format(_T("%s._DCtl%d.Name"), (TCHAR*)sKey.GetBuf(), n);
				cc::Str sName2 = CC_APP_GetIni()->Get(sPre);
				if(nFrom < 0 && sName2.Equals(sName))
				{
					nFrom = n;
				}
				if(nFrom >= 0 && n <= nFrom)
				{
					//move from (n+1) -> n
					//Name
					sPre.Format(_T("%s._DCtl%d.Name"), (TCHAR*)sKey.GetBuf(), n+1);
					sName2 = CC_APP_GetIni()->Get(sPre);
					sPre.Format(_T("%s._DCtl%d.Name"), (TCHAR*)sKey.GetBuf(), n);
					CC_APP_GetIni()->Set(sPre, sName2);
					sPre.Format(_T("%s._DCtl%d.Name"), (TCHAR*)sKeyLang.GetBuf(), n);
					CC_APP_GetLang()->Set(sPre, sName2);
					//Type
					sPre.Format(_T("%s._DCtl%d.Type"), (TCHAR*)sKey.GetBuf(), n+1);
					sName2 = CC_APP_GetIni()->Get(sPre);
					sPre.Format(_T("%s._DCtl%d.Type"), (TCHAR*)sKey.GetBuf(), n);
					CC_APP_GetIni()->Set(sPre, sName2);
					sPre.Format(_T("%s._DCtl%d.Type"), (TCHAR*)sKeyLang.GetBuf(), n);
					CC_APP_GetLang()->Set(sPre, sName2);
					//CfgName
					sPre.Format(_T("%s._DCtl%d.CfgName"), (TCHAR*)sKey.GetBuf(), n+1);
					sName2 = CC_APP_GetIni()->Get(sPre);
					sPre.Format(_T("%s._DCtl%d.CfgName"), (TCHAR*)sKey.GetBuf(), n);
					CC_APP_GetIni()->Set(sPre, sName2);
					sPre.Format(_T("%s._DCtl%d.CfgName"), (TCHAR*)sKeyLang.GetBuf(), n);
					CC_APP_GetLang()->Set(sPre, sName2);
				}
			}

			//clear (nCount)
			//Name
			nCount++;
			sPre.Format(_T("%s._DCtl%d.Name"), (TCHAR*)sKey.GetBuf(), nCount);
			CC_APP_GetIni()->Set(sPre, (const TCHAR*)NULL);
			sPre.Format(_T("%s._DCtl%d.Name"), (TCHAR*)sKeyLang.GetBuf(), nCount);
			CC_APP_GetLang()->Set(sPre, (const TCHAR*)NULL);
			//Type
			sPre.Format(_T("%s._DCtl%d.Type"), (TCHAR*)sKey.GetBuf(), nCount);
			CC_APP_GetIni()->Set(sPre, (const TCHAR*)NULL);
			sPre.Format(_T("%s._DCtl%d.Type"), (TCHAR*)sKeyLang.GetBuf(), nCount);
			CC_APP_GetLang()->Set(sPre, (const TCHAR*)NULL);
			//CfgName
			sPre.Format(_T("%s._DCtl%d.CfgName"), (TCHAR*)sKey.GetBuf(), nCount);
			CC_APP_GetIni()->Set(sPre, (const TCHAR*)NULL);
			sPre.Format(_T("%s._DCtl%d.CfgName"), (TCHAR*)sKeyLang.GetBuf(), nCount);
			CC_APP_GetLang()->Set(sPre, (const TCHAR*)NULL);

			//clear control self's config
			sKey.Append(_T("."));
			sKeyLang.Append(_T("."));
			sKey.Append(control->GetConfigName() == NULL ? control->GetName() : control->GetConfigName());
			sKeyLang.Append(control->GetConfigName() == NULL ? control->GetName() : control->GetConfigName());
			CC_APP_GetIni()->Clear(sKey);
			CC_APP_GetLang()->Clear(sKeyLang);

			//remove control
			parent->RemoveControl(control);
			control->Dispose();
			delete control;
			//remove from list
			comboCtl->ItemDel(nIndCtl);
			pLastCtl = NULL;
			comboCtl_SelectedIndexChanged(NULL, NULL);
		}
		else if(sender == btnCreateSource)
		{
			if(IDYES != (int)::MessageBox(m_hWnd, TCHAR_LANG("Lang.SD_CreateSource"), TCHAR_LANG("Lang.S_MsgBox_Title_Question"), MB_YESNO | MB_ICONQUESTION))
			{
				return;
			}
			cc::Log::trace(_T("==============================CreateSource S:"));
			cc::Str sTxt;
			cc::Str sVarlist;
			sVarlist.Append(_T("\r\n==============================[Source AutoCreated]\r\n"));
			sTxt.Append(_T("\r\n==============================[Source AutoCreated]\r\n"));
			_pParent->CreateSource(sTxt, sVarlist, cc::Str(_T("Main")));
			cc::Log::trace((TCHAR*)sVarlist.GetBuf());
			cc::Log::trace((TCHAR*)sTxt.GetBuf());
			cc::Log::trace(_T("==============================CreateSource E:"));
			::MessageBox(m_hWnd, CC_APP_GetLang()->Get(_T("Lang.SD_CreateSourceOK"), cc::Str(_T("Source is printer to log(trace), you should sure [Log trace to File] is on."))),
					CC_APP_GetLang()->Get(_T("Lang.S_MsgBox_Title_Info")), MB_OK);
		}
		else if(sender == btnOpenLogWin)
		{
			cc::Log::CreateConsole();
		}
		else if(sender == btnOutLang)
		{
			cc::Str sPath = cc::Util::getStartupPath(true);
			cc::Str sPath2 = sPath + _T("Lang\\");
			if(!cc::Util::dExist(sPath2))
			{
				sPath2.Clear();
				int nPos = sPath.LastIndexOf(_T("\\"), sPath.GetLength() - 2);
				if(nPos > 0)
				{
					sPath2 = sPath.Remove(nPos + 1) + _T("Lang\\");
					if(!cc::Util::dExist(sPath2))
					{
						sPath2.Clear();
					}
				}
			}
			if(sPath2.GetLength() <= 0)
			{
				::MessageBox(m_hWnd, CC_APP_GetLang()->Get(_T("Lang.SD_NoLangDir"), cc::Str(_T("No \\Lang folder."))),
					CC_APP_GetLang()->Get(_T("Lang.S_MsgBox_Title_Critical")), MB_OK | MB_ICONSTOP);
				return;
			}
			if(IDYES != (int)::MessageBox(m_hWnd, TCHAR_LANG("Lang.SD_SaveToLang"), TCHAR_LANG("Lang.S_MsgBox_Title_Question"), MB_YESNO | MB_ICONQUESTION))
			{
				return;
			}
			cc::KeyArr& arrIni = *CC_APP_GetIni();
			int nCapacity = arrIni.GetSize();
			cc::Map<cc::Str, cc::Str> arrRes;
			cc::Util::readResNames(arrRes, CC_CONFIG_LANGTYPE);
			for(int n = 0; n < arrRes.GetSize(); n++)
			{
				cc::Str langName = arrRes.GetKey(n);
				langName.Append(_T("."));
				sPath = sPath2 + langName + _T("ini");
				//Map(bool isSort, bool bIsDESC = false, bool isDistinct = false, int nMinCapacity = 0, double nIncrement = 1.3);
				cc::Map<cc::Str, cc::Str> arrFile(false, false, true);
				int nSize = cc::Util::readIni(arrFile, sPath, LangCode::UTF8, true, true);
				//arrFile.Add(arrIni, (TCHAR*)langName.GetBuf(), CC_CONFIG_LANGHEADDOT);
				//arrFile.Add(arrIni, CC_CONFIG_DESIGNHEADDOT, CC_CONFIG_LANGHEADDOT);
				for(int i = 0; i < nCapacity; i++)
				{
					cc::Str sKey = arrIni.GetArr()->GetKey(i);
					if(sKey.StartsWith(CC_CONFIG_DESIGNHEADDOT, true) || sKey.StartsWith(langName, true))
					{
						int nFrom = sKey.IndexOf(_T("."));
						sKey.Remove(0, nFrom);
						sKey.Insert(0, CC_CONFIG_LANGSECTION);
						arrFile.Add(sKey, arrIni.GetArr()->GetAt(i));
					}
				}
				cc::Util::writeIni(arrFile, sPath, LangCode::UTF8);
			}
		}
		else if(sender == btnDumpConfig)
		{
			CC_APP_GetConfig()->Dump();
			::MessageBox(m_hWnd, CC_APP_GetLang()->Get(_T("Lang.SD_DumpLang"), cc::Str(_T("Config(Lang&Ini) is dumped to log(trace), you should sure [Log trace to File] is on."))),
					CC_APP_GetLang()->Get(_T("Lang.S_MsgBox_Title_Info")), MB_OK);
		}
		else if(sender == chkDebug)
		{
			CC_APP_GetConfig()->SetIsDebug(chkDebug->GetChecked());
		}
		else if(sender == chkDesign)
		{
			CC_APP_GetConfig()->SetIsDesign(chkDesign->GetChecked());
			if(!chkDesign->GetChecked())
			{
				comboCtl->SetSelectedIndex(-1);
				comboCtl_SelectedIndexChanged(NULL, NULL);
			}
		}
	}
protected:
	Control* pLastCtl;
	cc::Str sSelKey;
	TabControl* tab1;
	TabPage* pageCfg1;
	TabPage* pageCfg2;
	ComboBox* comboCtl;
	ToolItem* btnRunDel;
	ToolItem* btnRunRef;
	ListBox* lstVar;
	Button* btnSet;
	ToolItem* btnRunCls;
	ToolItem* btnCopyName;
	ToolItem* btnPrintCtl;
	ToolItem* btnCopyLang;
	ToolItem* btnClearDesign;
	TextBox* editVItem;
	TextBox* editVItemM;
	CheckBox* chkDebug;
	CheckBox* chkDesign;
	Button* btnOpenLogWin;
	Button* btnOutLang;
	Button* btnDumpConfig;
	TextBox* editX;
	TextBox* editY;
	TextBox* editW;
	TextBox* editH;
	PickPercentCtl* pickX;
	PickPercentCtl* pickY;
	PickPercentCtl* pickW;
	PickPercentCtl* pickH;
	PickColorCtl* pcColor;
	PickColorCtl* pcHotColor;
	PickColorCtl* pcBack;
	PickColorCtl* pcHotBack;
	int nDoEnd;
	//while parent resize,if isRestoreRect
	int isRestoreRect;
	int nSaveX;
	int nSaveY;
	int nSaveW;
	int nSaveH;
	COLORREF crSaveText;
	COLORREF crSaveHotText;
	COLORREF crSaveBack;
	COLORREF crSaveHotback;
	DWORD dwSaveAnchor;
	PathButton* btnSelFrom;
	ComboBox* comboStyle;
	TabPage* pageCfg3;
	Label* label;
	TextBox* editName;
	TextBox* editNameCfg;
	ComboBox* comboType;
	TextBox* editCtlX;
	TextBox* editCtlY;
	TextBox* editCtlW;
	TextBox* editCtlH;
	Button* btnNew;
	int nLastVarIndex;
	int nActionType; //1:Text;2:Tips;3:TipsExt;4:Color;5:Left;6:XXX;7:Anchor;8:URL;9:Style;
	Button* btnCreateSource;
	PushButton* pbtnLeft;
	PushButton* pbtnTop;
	PushButton* pbtnRight;
	PushButton* pbtnBottom;

	void OnInnerTimer(EventArgs *e)
	{
		if(e->WParam == CC_TIMERID_DESIGN_SETHOT)
		{
			::KillTimer(m_hWnd, CC_TIMERID_DESIGN_SETHOT);

			Control* newHot = CC_Design_Hotcontrol_new;
			CC_Design_Hotcontrol_new = NULL;
			if(pLastCtl == newHot || newHot == NULL)
			{
				return;
			}

			int nCnt = comboCtl->ItemCount();
			if(nCnt > 0 && comboCtl->GetSelectedIndex() >= 0)
			{
				comboCtl->SetSelectedIndex(-1);
				comboCtl_SelectedIndexChanged(NULL, NULL);
			}
			for(int n = 0; n < nCnt; n++)
			{
				if(newHot->m_hWnd == (HWND)comboCtl->GetItemData(n))
				{
					comboCtl->SetSelectedIndex(n);
					break;
				}
			}
			comboCtl_SelectedIndexChanged(NULL, NULL);
			return;
		}
		FloatContainer::OnInnerTimer(e);
	}

	// WndProc - calls appriate On... function for the give
	// message
	LRESULT WndProc(EventArgs *e)
	{
		switch(e->Msg)
		{
		case CC_WM_MAINMSG:
			if(e->WParam == CC_WM_MAINMSG_WPARAM_DS_LOCCHANGED)
			{
				if(pLastCtl == (Control*)e->LParam)
				{
					::InvalidateRect(pLastCtl->m_hWnd, NULL, TRUE);
				}
			}
			else if(e->WParam == CC_WM_MAINMSG_WPARAM_DS_GETHOT)
			{
				return (LRESULT)pLastCtl;
			}
			else if(e->WParam == CC_WM_MAINMSG_WPARAM_DS_RESIZE)
			{
				if(isRestoreRect == 1 && pLastCtl != NULL && pLastCtl != (Control*)e->LParam && ::IsChild(((Control*)e->LParam)->m_hWnd, pLastCtl->m_hWnd))
				{
					isRestoreRect = 0;
					RECT rect = pLastCtl->GetBounds();
					if(rect.left != nSaveX || rect.top != nSaveY)
					{
						pLastCtl->SetBoundsNotOriginal(nSaveX, nSaveY, 0, 0, BoundsSpecified::Location);
					}
					if(rect.right - rect.left != nSaveW || rect.bottom - rect.top != nSaveH)
					{
						pLastCtl->SetBoundsNotOriginal(0, 0, nSaveW, nSaveH, BoundsSpecified::Size);
					}
				}
			}
			else if(e->WParam == CC_WM_MAINMSG_WPARAM_TEXTTIPSONLY)
			{
				this->SetText((const TCHAR*)e->LParam);
			}
			break;
		case WM_SETTEXT:
			return DefWndProc(e);
		//case WM_ACTIVATE:
		//	if(LOWORD(e->WParam) != WA_INACTIVE)
		//	{
		//		CC_BIT_ON(_Flags, CC_FLAG_DESIGN_ISACTIVATED);
		//		bool isChild = false;
		//		Control* ctl = Control::FromHandle((HWND)e->LParam);
		//		while(ctl != NULL)
		//		{
		//			if(m_hWnd == ctl->m_hWnd || ::IsChild(m_hWnd, ctl->m_hWnd))
		//			{
		//				isChild = true;
		//				break;
		//			}
		//			ctl = ctl->GetParent();
		//		}
		//		if(!isChild)
		//		{
		//			comboCtl_SelectedIndexChanged(btnSet, NULL);
		//		}
		//	}
		//	else
		//	{
		//		CC_BIT_OFF(_Flags, CC_FLAG_DESIGN_ISACTIVATED);
		//	}
		//	break;
		case WM_CLOSE:
		case WM_QUIT:
		case WM_DESTROY:
		case WM_NCDESTROY:
		case WM_ENDSESSION:
			if(!CC_BIT_HAS(_Flags, CC_FLAG_DESIGN_ISCALLEDCLOSED))
			{
				CC_BIT_ON(_Flags, CC_FLAG_DESIGN_ISCALLEDCLOSED);
				ReleaseDesignControl(pLastCtl);
				RestoreLastSet(pLastCtl);
				RECT rect;
				::GetWindowRect(m_hWnd, &rect);
				KeyArr* keyArr = CC_APP_GetIni();
				keyArr->Set(_T("Main.Design.Left"), rect.left);
				keyArr->Set(_T("Main.Design.Top"), rect.top);
				keyArr->Set(_T("Main.Design.Width"), rect.right - rect.left);
				keyArr->Set(_T("Main.Design.Heigh"), rect.bottom - rect.top);
				CC_APP_GetConfig()->SetIsDesign(false);
			}
			//OnShutdown(e);
			break;
		}
		return FloatContainer::WndProc(e);
	}

	void RestoreLastSet(Control* pSaveLastCtl)
	{
		if(pSaveLastCtl == NULL || !::IsWindow(pSaveLastCtl->m_hWnd))
		{
			return;
		}
		//pSaveLastCtl->Design = (DesignStyle::Type)((DWORD)pSaveLastCtl->Design & ~DesignStyle::Selected);
		//::InvalidateRect(pSaveLastCtl->m_hWnd, NULL, FALSE);
		if(crSaveText != pSaveLastCtl->GetTextColor())
		{
			pSaveLastCtl->SetTextColor(crSaveText);
		}
		if(crSaveHotText != pSaveLastCtl->GetHotTextColor())
		{
			pSaveLastCtl->SetHotTextColor(crSaveHotText);
		}
		if(crSaveBack != pSaveLastCtl->GetBackground())
		{
			pSaveLastCtl->SetBackground(crSaveBack);
		}
		if(crSaveHotback != pSaveLastCtl->GetHotBackColor())
		{
			pSaveLastCtl->SetHotBackColor(crSaveHotback);
		}
		if(dwSaveAnchor != (DWORD)pSaveLastCtl->Anchor)
		{
			pSaveLastCtl->Anchor = (AnchorStyles::Type)dwSaveAnchor;
		}
		if(isRestoreRect == 1)
		{
			RECT rect = pSaveLastCtl->GetBounds();
			if(rect.left != nSaveX || rect.top != nSaveY)
			{
				pSaveLastCtl->SetBoundsNotOriginal(nSaveX, nSaveY, 0, 0, BoundsSpecified::Location);
			}
			if(rect.right - rect.left != nSaveW || rect.bottom - rect.top != nSaveH)
			{
				pSaveLastCtl->SetBoundsNotOriginal(0, 0, nSaveW, nSaveH, BoundsSpecified::Size);
			}
		}
		if(pSaveLastCtl == pLastCtl)
		{
			ReleaseDesignControl(pLastCtl);
			pLastCtl = NULL;
			if(lstVar->m_hWnd != NULL)
			{
				lstVar->ItemClear();
			}
			btnSet->SetEnabled(false);
			btnRunCls->SetEnabled(false);
			btnPrintCtl->SetEnabled(false);
			btnCopyName->SetEnabled(false);
		}
	}

	void OnCreateControl()
	{
		FloatContainer::OnCreateControl();
		if(_pParent != NULL)
		{
			AddOneWindow(_pParent->m_hWnd, _T(""));
		}

		//load config
		this->Config(true, *CC_APP_GetIni(), cc::Str(_T("Main")));
	}

	void GetCfgKey(Control* ctl, cc::Str& sPre, cc::Str& sKey, cc::Str& sKeyFlag, cc::Str& sKeyLang, cc::Str& sKeyLangFlag)
	{
		sKey.Clear();
		sKeyFlag.Clear();
		while(ctl != NULL)
		{
			const TCHAR* tc = ctl->GetConfigName();
			if(tc == NULL || _tcscmp(_T(""), tc) != 0)
			{
				if(sKey.GetLength() > 0)
				{
					sKey.Insert(0, _T("."));
				}
				sKey.Insert(0, tc == NULL ? ctl->GetName() : tc);
			}
			ctl = ctl->GetParent();
		}
		if(sKey.GetLength() <= 0)
		{
			//NewControl may come here
			sKey.ValueOf(CC_CONFIG_DESIGNSECTION);
			sKeyFlag.ValueOf(CC_CONFIG_DESIGNSECTION).Append(_T("._DFlag"));
			sKeyLang.ValueOf(CC_CONFIG_LANGSECTION);
			sKeyLangFlag.ValueOf(CC_CONFIG_LANGSECTION).Append(_T("._DFlag"));
			return;
		}

		sKeyFlag = CC_CONFIG_DESIGNHEADDOT + sKey + _T("._DFlag");
		sKeyLangFlag = CC_CONFIG_LANGHEADDOT + sKey + _T("._DFlag");
		bool isLang = false;
		if(sPre.Equals(_T("Text:")))
		{
			isLang = true;
		}
		else if(sPre.Equals(_T("Color:")))
		{
			sKey.Append(_T("._DColor"));
		}
		else if(sPre.Equals(_T("Left:")))
		{
			sKey.Append(_T("._DRect"));
		}
		else if(sPre.Equals(_T("URL:")))
		{
			sKey.Append(_T(".URL"));
		}
		//else if(sPre.Equals(_T("BackImage:")))
		//{
		//	sKey.Append(_T("._DImg"));
		//}
		else if(sPre.Equals(_T("Anchor:")))
		{
			sKey.Append(_T("._DAnchor"));
		}
		else if(sPre.Equals(_T("Style:")))
		{
			sKey.Append(_T("._DStyle"));
		}
		else if(sPre.StartsWith(_T("Tips")))
		{
			//Tips,TipsExt
			sKey.Append(sPre, 0, sPre.GetLength() - 1);
			isLang = true;
		}
		else if(sPre.Equals(_T("NewControl:")))
		{
		}
		else
		{
			sKey.Append(_T(".")).Append(sPre, 0, sPre.GetLength() - 1);
		}
		//should be here
		sKeyLang = CC_CONFIG_LANGHEADDOT + sKey;
		if(isLang)
		{
			sKey.Insert(0, CC_APP_GetIni()->Get(CC_CONFIG_LANGFILE) + _T("."));
		}
		else
		{
			sKey.Insert(0, CC_CONFIG_DESIGNHEADDOT);
		}
	}

	bool CheckSameName(Control* ctl, const TCHAR* sName)
	{
		if(ctl->GetControl(sName) != NULL)
		{
			return true;
		}
		int nCnt;
		if((nCnt = ctl->GetControlCount()) > 0)
		{
			for(int n = nCnt - 1; n >= 0; n--)
			{
				Control* ctl2 = ctl->GetControl(n);
				if(CheckSameName(ctl2, sName))
				{
					return true;
				}
			}
		}
		return false;
	}
	void AddOneWindow(HWND hWnd, cc::Str strPre)
	{
		Control* control = Control::FromHandle(hWnd);
		if(control != NULL)
		{
			TCHAR buf[256];
			memset(buf, 0, 256 * sizeof(TCHAR));
			::GetClassName(hWnd, (LPTSTR)&buf, 256 - 1);
			cc::Str strBuf;
			strBuf.Format(_T("%s[%s, %s]"), (TCHAR*)strPre.GetBuf(), buf, control->GetName());
			::GetWindowText(hWnd, (LPTSTR)&buf, 256 - 1);
			strBuf.Append(buf);
			int nInd = comboCtl->ItemAdd((const TCHAR *)strBuf.GetBuf());
			comboCtl->SetItemData(nInd, (DWORD_PTR)hWnd);
		}

		HWND hwndtmp = ::GetNextWindow(hWnd, GW_CHILD);
		HWND hwndend = hwndtmp;
		while(hwndtmp != NULL)
		{
			//for show child with space ahead
			AddOneWindow(hwndtmp, strPre + _T("  "));

			hwndtmp = GetNextWindow(hwndtmp, GW_HWNDNEXT);
			if(hwndtmp == hwndend)
			{
				break;
			}
		}
	}

	/*
	void HotHwnd_MouseEnter(Object* sender, EventArgs* e)
	{
		if(pLastCtl == NULL || !::IsWindow(pLastCtl->m_hWnd) || !CC_APP_GetConfig()->GetIsDesign() || !CC_BIT_HAS(_Flags, CC_FLAG_DESIGN_ISACTIVATED))
		{
			return;
		}
		RECT rect;
		::GetWindowRect(pLastCtl->m_hWnd, &rect);
		HDC hDC = ::GetWindowDC(pLastCtl->m_hWnd);
		CC_APP_GetDrawActor()->InvertBorder(0, 0, rect.right - rect.left, rect.bottom - rect.top, hDC, 3 * GetSystemMetrics(SM_CXBORDER));
		::ReleaseDC(pLastCtl->m_hWnd, hDC);
		CC_BIT_ON(_Flags, CC_FLAG_DESIGN_ISRECTED);
	}

	void HotHwnd_MouseLeave(Object* sender, EventArgs* e)
	{
		if(pLastCtl == NULL || !::IsWindow(pLastCtl->m_hWnd) || !CC_APP_GetConfig()->GetIsDesign() || !CC_BIT_HAS(_Flags, CC_FLAG_DESIGN_ISACTIVATED) || !CC_BIT_HAS(_Flags, CC_FLAG_DESIGN_ISRECTED))
		{
			return;
		}
		RECT rect;
		::GetWindowRect(pLastCtl->m_hWnd, &rect);
		HDC hDC = ::GetWindowDC(pLastCtl->m_hWnd);
		CC_APP_GetDrawActor()->InvertBorder(0, 0, rect.right - rect.left, rect.bottom - rect.top, hDC, 3 * GetSystemMetrics(SM_CXBORDER));
		::ReleaseDC(pLastCtl->m_hWnd, hDC);
		CC_BIT_OFF(_Flags, CC_FLAG_DESIGN_ISRECTED);
	}
	*/

	void ReleaseDesignControl(Control* control)
	{
		if(control != NULL && ((DWORD)control->Design & DesignStyle::Selected) != 0)
		{
			control->Design = (DesignStyle::Type)((DWORD)control->Design & ~DesignStyle::Selected);
			//here need be Parent's m_hWnd
			if(control->GetParent() != NULL && control->GetParent()->m_hWnd != NULL)
			{
				RECT rect;
				::GetWindowRect(control->m_hWnd, &rect);
				::ScreenToClient(control->GetParent()->m_hWnd, (LPPOINT)&rect);
				::ScreenToClient(control->GetParent()->m_hWnd, ((LPPOINT)&rect)+1);
				::InvalidateRect(control->GetParent()->m_hWnd, &rect, TRUE);
				::InvalidateRect(control->m_hWnd, NULL, TRUE);
			}
		}
	}
	void comboStyle_SelectedIndexChanged(Object* sender, EventArgs* e)
	{
		int nInd = comboStyle->GetSelectedIndex();
		if(nInd >= 0)
		{
			cc::Str sStyle = comboStyle->ItemGet(nInd);
			if(sStyle.Equals(_T("Image Back")))
			{
				nInd = -2;
			}
		}
		editVItem->SetEnabled(nInd == -2);
		btnSelFrom->SetEnabled(nInd == -2);
	}

	void comboCtl_SelectedIndexChanged(Object* sender, EventArgs* e)
	{
		nDoEnd = 0;
		Control* pSaveLastCtl = pLastCtl;
		pLastCtl = NULL;
		int nIndOld = lstVar->GetSelectedIndex();
		lstVar->ItemClear();
		btnSet->SetEnabled(false);
		btnPrintCtl->SetEnabled(false);
		btnCopyName->SetEnabled(false);
		btnRunCls->SetEnabled(false);
		int nInd = comboCtl->GetSelectedIndex();
		if(nInd >= 0)
		{
			HWND hWnd = (HWND)comboCtl->GetItemData(nInd);
			if(::IsWindow(hWnd))
			{
				pLastCtl = Control::FromHandle(hWnd);
			}
		}
		if(pSaveLastCtl != pLastCtl)
		{
			ReleaseDesignControl(pSaveLastCtl);
		}
		if(pSaveLastCtl != NULL && pSaveLastCtl != pLastCtl && sender != btnSet)
		{
			const TCHAR* tcName = pSaveLastCtl->GetName();
			const TCHAR* tcCfg = pSaveLastCtl->GetConfigName();
			if((tcCfg == NULL && tcName == NULL) || (tcCfg != NULL && _tcscmp(_T(""), tcCfg) == 0))
			{
			}
			else
			{
				//now restore
				RestoreLastSet(pSaveLastCtl);
			}
		}
		if(pLastCtl == NULL || (((DWORD)pLastCtl->Design & DesignStyle::NoDesign) != 0))
		{
			//if refresh need pLastCtl=NULL;
			lstVar->ItemAdd(_T("NoDesign,need do nothing."));
			pLastCtl = NULL;
			return;
		}
		const TCHAR* tcName = pLastCtl->GetName();
		const TCHAR* tcCfg = pLastCtl->GetConfigName();
		if((tcCfg == NULL && tcName == NULL) || (tcCfg != NULL && _tcscmp(_T(""), tcCfg) == 0))
		{
			lstVar->ItemAdd(_T("No name&config name,do nothing."));
			//this->SetText(_T("No name&config name,do nothing."));
			pLastCtl = NULL;
			return;
		}

		if(pSaveLastCtl != pLastCtl)
		{
			btnSelFrom->SelectedPath.Clear();
			pLastCtl->Design = (DesignStyle::Type)((DWORD)pLastCtl->Design | DesignStyle::Selected);
			CC_APP_DrawFocusFrame(pLastCtl);
		}

		if(sender != btnSet && pSaveLastCtl != pLastCtl)
		{
			//when btnSet not reget
			pSaveLastCtl = pLastCtl;

			RECT rect = pLastCtl->GetBounds();
			nSaveX = rect.left;
			nSaveY = rect.top;
			nSaveW = rect.right - rect.left;
			nSaveH = rect.bottom - rect.top;
			isRestoreRect = 1;
			crSaveText = pLastCtl->GetTextColor();
			crSaveHotText = pLastCtl->GetHotTextColor();
			crSaveBack = pLastCtl->GetBackground();
			crSaveHotback = pLastCtl->GetHotBackColor();
			dwSaveAnchor = (DWORD)pLastCtl->Anchor;
		}
		cc::Str strBuf;
		if(((DWORD)pLastCtl->Design & DesignStyle::AutoSize) != 0 || ((DWORD)pLastCtl->Design & DesignStyle::Background) != 0)
		{
			strBuf.Append(_T("Style:"));
			if(((DWORD)pLastCtl->Design & DesignStyle::AutoSize) != 0)
			{
				strBuf.Append(_T("AutoSize:"));
				if(pLastCtl->HasStyle(StyleType::Outer, OuterStyle::AutoSize))
				{
					strBuf.Append(_T("On; "));
				}
				else
				{
					strBuf.Append(_T("Off; "));
				}
			}
			if(((DWORD)pLastCtl->Design & DesignStyle::Background) != 0)
			{
				//Color Back;	Gradient Back(Left-Right);	Gradient Back(Up-Down);	Image Back;
				strBuf.Append(_T("Background:"));
				if(pLastCtl->HasStyle(StyleType::Outer, OuterStyle::TransparentTop))
				{
					if(pLastCtl->HasStyle(StyleType::Inner, InnerStyle::BackgroundColor))
					{
						strBuf.Append(_T("Color; "));
					}
					else if(pLastCtl->HasStyle(StyleType::Inner, InnerStyle::BackgroundBmp))
					{
						strBuf.Append(_T("Image; "));
					}
					else if(pLastCtl->HasStyle(StyleType::Inner, InnerStyle::BackgroundGradient))
					{
						if(!pLastCtl->HasStyle(StyleType::Inner2, Inner2Style::GradientVertical))
						{
							strBuf.Append(_T("Gradient(Left-Right); "));
						}
						else
						{
							strBuf.Append(_T("Gradient(Up-Down); "));
						}
					}
				}
				else
				{
					strBuf.Append(_T("Transparent; "));
				}
			}
			//strBuf.Append(pLastCtl->GetText());
			lstVar->ItemAdd((const TCHAR *)strBuf.GetBuf());
		}

		if(((DWORD)pLastCtl->Design & DesignStyle::NoEditText) == 0)
		{
			strBuf.Clear();
			strBuf.Append(_T("Text:"));
			strBuf.Append(pLastCtl->GetText());
			lstVar->ItemAdd((const TCHAR *)strBuf.GetBuf());
		}

		if(((DWORD)pLastCtl->Design & DesignStyle::NoEditTips) == 0)
		{
			strBuf.Clear();
			strBuf.Append(_T("Tips:"));
			strBuf.Append(pLastCtl->GetTips());
			lstVar->ItemAdd((const TCHAR *)strBuf.GetBuf());
		}

		if(((DWORD)pLastCtl->Design & DesignStyle::NoEditTipsExt) == 0)
		{
			strBuf.Clear();
			strBuf.Append(_T("TipsExt:"));
			strBuf.Append(pLastCtl->GetTipsExt());
			lstVar->ItemAdd((const TCHAR *)strBuf.GetBuf());
		}

		if(((DWORD)pLastCtl->Design & CC_DESIGN_NO_COLOR) != CC_DESIGN_NO_COLOR)
		{
			strBuf.Format(_T("Color:%d, HotColor:%d, BackColor:%d, HotbackColor:%d"), pLastCtl->GetTextColor(), pLastCtl->GetBackground(), pLastCtl->GetHotTextColor(), pLastCtl->GetHotBackColor());
			lstVar->ItemAdd((const TCHAR *)strBuf.GetBuf());
		}

		if(((DWORD)pLastCtl->Design & CC_DESIGN_NO_MOVE) != CC_DESIGN_NO_MOVE)
		{
			RECT rect = pLastCtl->GetBounds();
			strBuf.Format(_T("Left:%d, Top:%d, Width:%d, Height:%d"), rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);
			lstVar->ItemAdd((const TCHAR *)strBuf.GetBuf());
		}

		if(((DWORD)pLastCtl->Design & DesignStyle::NoAnchor) == 0)
		{
			strBuf.Clear();
			strBuf.Append(_T("Anchor:"));
			if((pLastCtl->Anchor & AnchorStyles::Left) != 0)
			{
				strBuf.Append(_T("Left; "));
			}
			if((pLastCtl->Anchor & AnchorStyles::Top) != 0)
			{
				strBuf.Append(_T("Top; "));
			}
			if((pLastCtl->Anchor & AnchorStyles::Right) != 0)
			{
				strBuf.Append(_T("Right; "));
			}
			if((pLastCtl->Anchor & AnchorStyles::Bottom) != 0)
			{
				strBuf.Append(_T("Bottom:; "));
			}
			lstVar->ItemAdd((const TCHAR *)strBuf.GetBuf());
		}

		if(((DWORD)pLastCtl->Design & DesignStyle::EditURL) == DesignStyle::EditURL)
		{
			strBuf.Clear();
			strBuf.Append(_T("URL:"));
			strBuf.Append(((LabelLink*)pLastCtl)->GetURL());
			lstVar->ItemAdd((const TCHAR *)strBuf.GetBuf());
		}

		//strBuf.Clear();
		//strBuf.Append(_T("Name:"));
		//strBuf.Append(sName);
		//lstVar->ItemAdd((const TCHAR *)strBuf.GetBuf());

		//strBuf.Clear();
		//strBuf.Append(_T("ConfigName:"));
		//strBuf.Append(sCfgName);
		//lstVar->ItemAdd((const TCHAR *)strBuf.GetBuf());

		lstVar->SetSelectedIndex(nIndOld);
		lstVar_SelectedIndexChanged(NULL, NULL);
	}

	void lstVar_SelectedIndexChanged(Object* sender, EventArgs* e)
	{
		nDoEnd = 0;
		nActionType = 0; //1:Text;2:Tips;3:TipsExt;4:Color;5:Left;6:XXX;7:Anchor;8:URL;9:Style;
		btnSet->SetEnabled(false);
		btnPrintCtl->SetEnabled(false);
		btnCopyName->SetEnabled(false);
		btnRunCls->SetEnabled(false);
		btnSelFrom->SetEnabled(false);
		comboStyle->SetVisible(false);
		int nInd = lstVar->GetSelectedIndex();
		if(nInd < 0 && nLastVarIndex < lstVar->ItemCount())
		{
			nInd = nLastVarIndex;
			lstVar->SetSelectedIndex(nLastVarIndex);
		}
		if(pLastCtl == NULL || nInd < 0)
		{
			sSelKey.Clear();
			editVItem->SetEnabled(false);
			editVItemM->SetEnabled(false);
			editVItem->SetTips(NULL);
			editVItemM->SetTips(NULL);
			btnSet->SetTips(NULL);
			return;
		}
		nLastVarIndex = nInd;
		cc::Str strBuf;
		cc::Str sVal;
		strBuf = lstVar->ItemGet(nInd);
		nInd = strBuf.IndexOf(_T(":"));
		sVal = strBuf.Substring(nInd + 1);
		sSelKey = strBuf.Substring(0, nInd + 1);
		bool isRect = (cc::Str::Search(_T("Left:"), sSelKey) >= 0);
		bool isMulti = false;
		bool isColor = false;
		bool isAnchor = false;
		bool isStyle = false;
		if(!isRect)
		{
			isMulti = (cc::Str::Search(_T("Text:TipsExt:"), sSelKey) >= 0);
		}

		if(isRect)
		{
			nActionType = 5; //1:Text;2:Tips;3:TipsExt;4:Color;5:Left;6:XXX;7:Anchor;8:URL;9:Style;
			RECT rect = pLastCtl->GetBounds();
			pickX->SetValue(rect.left);
			pickY->SetValue(rect.top);
			pickW->SetValue(rect.right - rect.left);
			pickH->SetValue(rect.bottom - rect.top);
			pickX->SetEnabled(((DWORD)pLastCtl->Design & DesignStyle::NoMoveLeft) == 0);
			pickY->SetEnabled(((DWORD)pLastCtl->Design & DesignStyle::NoMoveTop) == 0);
			pickW->SetEnabled(((DWORD)pLastCtl->Design & DesignStyle::NoMoveRight) == 0);
			pickH->SetEnabled(((DWORD)pLastCtl->Design & DesignStyle::NoMoveBottom) == 0);
			editX->SetEnabled(((DWORD)pLastCtl->Design & DesignStyle::NoMoveLeft) == 0);
			editY->SetEnabled(((DWORD)pLastCtl->Design & DesignStyle::NoMoveTop) == 0);
			editW->SetEnabled(((DWORD)pLastCtl->Design & DesignStyle::NoMoveRight) == 0);
			editH->SetEnabled(((DWORD)pLastCtl->Design & DesignStyle::NoMoveBottom) == 0);
			pickX->SetTips(_T("Left."));
			pickY->SetTips(_T("Top."));
			pickW->SetTips(_T("Width."));
			pickH->SetTips(_T("Height."));
		}
		else if(sSelKey.Equals(_T("Color:")))
		{
			nActionType = 4; //1:Text;2:Tips;3:TipsExt;4:Color;5:Left;6:XXX;7:Anchor;8:URL;9:Style;
			isColor = true;
			pcColor->SetBackground(pLastCtl->GetTextColor());
			pcHotColor->SetBackground(pLastCtl->GetHotTextColor());
			pcBack->SetBackground(pLastCtl->GetBackground());
			pcHotBack->SetBackground(pLastCtl->GetHotBackColor());
			pcColor->SetEnabled(((DWORD)pLastCtl->Design & DesignStyle::NoEditColor) == 0);
			pcHotColor->SetEnabled(((DWORD)pLastCtl->Design & DesignStyle::NoEditHColor) == 0);
			pcBack->SetEnabled(((DWORD)pLastCtl->Design & DesignStyle::NoEditBColor) == 0);
			pcHotBack->SetEnabled(((DWORD)pLastCtl->Design & DesignStyle::NoEditHBColor) == 0);
		}
		else if(sSelKey.Equals(_T("Style:")))
		{
			nActionType = 9; //1:Text;2:Tips;3:TipsExt;4:Color;5:Left;6:XXX;7:Anchor;8:URL;9:Style;
			btnSelFrom->SetEnabled(true);
			editVItem->SetText(_T("<Select Image File>"));
			editVItem->SetEnabled(true);
			isStyle = true;
			int nInd2 = comboStyle->GetSelectedIndex();
			comboStyle->ItemClear();
			if(((DWORD)pLastCtl->Design & DesignStyle::AutoSize) != 0)
			{
				comboStyle->ItemAdd(_T("AutoSize:On"));
				comboStyle->ItemAdd(_T("AutoSize:Off"));
			}
			if(((DWORD)pLastCtl->Design & DesignStyle::Background) != 0)
			{
				comboStyle->ItemAdd(_T("Transparent"));
				comboStyle->ItemAdd(_T("Color Back"));
				comboStyle->ItemAdd(_T("Gradient Back(Left-Right)"));
				comboStyle->ItemAdd(_T("Gradient Back(Up-Down)"));
				comboStyle->ItemAdd(_T("Image Back"));
			}
			comboStyle->SetSelectedIndex(nInd2);
			comboStyle->SetVisible(true);
			comboStyle_SelectedIndexChanged(NULL, NULL);
		}
		else if(sSelKey.Equals(_T("Anchor:")))
		{
			nActionType = 7; //1:Text;2:Tips;3:TipsExt;4:Color;5:Left;6:XXX;7:Anchor;8:URL;9:Style;
			isAnchor = true;
			pbtnLeft->SetPushed(((DWORD)pLastCtl->Anchor & AnchorStyles::Left) != 0);
			pbtnTop->SetPushed(((DWORD)pLastCtl->Anchor & AnchorStyles::Top) != 0);
			pbtnRight->SetPushed(((DWORD)pLastCtl->Anchor & AnchorStyles::Right) != 0);
			pbtnBottom->SetPushed(((DWORD)pLastCtl->Anchor & AnchorStyles::Bottom) != 0);
			pickW->SetValue(pLastCtl->MinPosition.x);
			pickH->SetValue(pLastCtl->MinPosition.y);
			pickW->SetTips(_T("MinPosition of Left."));
			pickH->SetTips(_T("MinPosition of Top."));
		}
		else
		{
			//if(sSelKey.Equals(_T("BackImage:")))
			//{
			//	nActionType = 6; //1:Text;2:Tips;3:TipsExt;4:Color;5:Left;6:XXX;7:Anchor;8:URL;9:Style;
			//	btnSelFrom->SetEnabled(true);
			//}
			//else
			//{
				//notice that 1,2,3 is not used
				nActionType = 1; //1:Text;2:Tips;3:TipsExt;4:Color;5:Left;6:XXX;7:Anchor;8:URL;9:Style;
			//}
			editVItem->SetText(sVal);
			editVItemM->SetText(sVal);
			editVItem->SetEnabled(true);
			editVItemM->SetEnabled(true);
		}

		editX->SetVisible(isRect);
		editY->SetVisible(isRect);
		editW->SetVisible(isRect || isAnchor);
		editH->SetVisible(isRect || isAnchor);
		pickX->SetVisible(isRect);
		pickY->SetVisible(isRect);
		pickW->SetVisible(isRect || isAnchor);
		pickH->SetVisible(isRect || isAnchor);
		pcColor->SetVisible(isColor);
		pcHotColor->SetVisible(isColor);
		pcBack->SetVisible(isColor);
		pcHotBack->SetVisible(isColor);
		btnSelFrom->SetVisible((!isMulti && !isRect && !isColor && !isAnchor) || isStyle);
		editVItem->SetVisible((!isMulti && !isRect && !isColor && !isAnchor) || isStyle);
		editVItemM->SetVisible(isMulti && !isRect && !isColor && !isAnchor);
		pbtnLeft->SetVisible(isAnchor);
		pbtnTop->SetVisible(isAnchor);
		pbtnRight->SetVisible(isAnchor);
		pbtnBottom->SetVisible(isAnchor);
		//::SendMessage(pageCfg1->m_hWnd, WM_SIZE, 0, 0);

		nDoEnd = 1;
		cc::Str sKey;
		cc::Str sKeyFlag;
		cc::Str sKeyLang;
		cc::Str sKeyLangFlag;
		GetCfgKey(pLastCtl, sSelKey, sKey, sKeyFlag, sKeyLang, sKeyLangFlag);
		if(sKey.GetLength() <= 0)
		{
			return;
		}
		strBuf.Clear();
		strBuf.Append(_T("Current Lang:")).Append(CC_APP_GetIni()->Get(CC_CONFIG_LANGFILE));
		strBuf.Append(_T("\r\nName:")).Append(pLastCtl->GetName()).Append(_T("\r\nConfigName:")).Append(pLastCtl->GetConfigName()).Append(_T("\r\nIniKey:")).Append(sKey);
		editVItem->SetTips(strBuf);
		editVItemM->SetTips(strBuf);
		btnSet->SetTips(strBuf);

		btnSet->SetEnabled(true);
		btnPrintCtl->SetEnabled(true);
		btnCopyName->SetEnabled(true);
		if(CC_APP_GetIni()->Exist(sKey))
		{
			btnRunCls->SetEnabled(true);
		}
	}
};
#endif //#if defined(_DEBUG_) || (defined(_DESIGN) && !defined(_DESIGNEX))

FloatContainer* CC_APP_CreateDesignBar()
{
#if defined(_DEBUG_) || (defined(_DESIGN) && !defined(_DESIGNEX))
	return new CC_DesignBar();
#else
	return NULL;
#endif //#if defined(_DEBUG_) || (defined(_DESIGN) && !defined(_DESIGNEX))
}

bool CC_APP_CreateSource(Control* control, cc::Str& sTxt, cc::Str& sVarList, cc::Str sThisHead)
{
	//editName = new TextBox();
	//editName->SetName(_T("editName"));
	//editName->SetText(_T(""));
	//editName->SetTips(_T("name is no change after cteated."));
	//editName->SetLocationOffset(label, false, 0, -2);
	//editName->SetSize(600 - 5 - 80, 17);
	//editName->Anchor = (AnchorStyles::Type)(AnchorStyles::Top | AnchorStyles::Left | AnchorStyles::Right);
	//pageCfg3->AddControl(editName);
	sTxt.Append(_T("	//")).Append(sThisHead).Append(_T("\r\n"));

	const TCHAR* _Name = control->GetName();
	const TCHAR* sClsName = ControlNameFromID(control->GetClassID());
	sVarList.Append(_T("	")).Append(sClsName).Append(_T("* ")).Append(_Name).Append(_T(";\r\n"));

	cc::Str buf;
	sTxt.Append(_T("	")).Append(_Name).Append(_T(" = new ")).Append(sClsName).Append(_T("();\r\n"));
	sTxt.Append(_T("	")).Append(_Name).Append(_T("->SetName(_T(\"")).Append(_Name).Append(_T("\"));\r\n"));
	sTxt.Append(_T("	")).Append(_Name).Append(_T("->SetText(_T(\"")).Append(_T("")).Append(_T("\"));\r\n"));
	if(control->GetImageList() != NULL && control->GetIconID() > 0)
	{
		buf.Format(_T("->SetIcon(imageList, %d);\r\n"), (int)control->GetIconID());
		sTxt.Append(_T("	")).Append(_Name).Append(buf);
	}
	RECT rect = control->GetBounds();
	buf.Format(_T("->SetLocation(%d, %d);\r\n"), rect.left, rect.top);
	sTxt.Append(_T("	")).Append(_Name).Append(buf);
	buf.Format(_T("->SetSize(%d, %d);\r\n"), rect.right - rect.left, rect.bottom - rect.top);
	sTxt.Append(_T("	")).Append(_Name).Append(buf);
	if(control->Anchor != (AnchorStyles::Type)(AnchorStyles::Top | AnchorStyles::Left) && control->Anchor != AnchorStyles::None)
	{
		sTxt.Append(_T("	")).Append(_Name).Append(_T("->Anchor = (AnchorStyles::Type)("));
		buf.Clear();
		if((control->Anchor & AnchorStyles::Left) != 0)
		{
			buf.Append(_T(" | AnchorStyles::Left"));
		}
		if((control->Anchor & AnchorStyles::Top) != 0)
		{
			buf.Append(_T(" | AnchorStyles::Top"));
		}
		if((control->Anchor & AnchorStyles::Right) != 0)
		{
			buf.Append(_T(" | AnchorStyles::Right"));
		}
		if((control->Anchor & AnchorStyles::Bottom) != 0)
		{
			buf.Append(_T(" | AnchorStyles::Bottom"));
		}
		sTxt.Append(buf.Substring(3)).Append(_T(");\r\n"));
	}
	if(control->MinPosition.x > 0)
	{
		buf.Format(_T("->MinPosition.x = %d;\r\n"), (int)control->MinPosition.x);
		sTxt.Append(_T("	")).Append(_Name).Append(buf);
	}
	if(control->MinPosition.y > 0)
	{
		buf.Format(_T("->MinPosition.y = %d;\r\n"), (int)control->MinPosition.y);
		sTxt.Append(_T("	")).Append(_Name).Append(buf);
	}
	if(control->GetMinSize().cx > 0 || control->GetMinSize().cy > 0)
	{
		buf.Format(_T("->SetMinSize(%d, %d);\r\n"), (int)control->GetMinSize().cx, (int)control->GetMinSize().cy);
		sTxt.Append(_T("	")).Append(_Name).Append(buf);
	}
	if(control->HasStyle(StyleType::Outer, OuterStyle::AutoSize))
	{
		sTxt.Append(_T("	")).Append(_Name).Append(_T("->SetOuterStyle(OuterStyle::AutoSize, true);\r\n"));
	}
	if(control->HasStyle(StyleType::Outer, OuterStyle::AutoConfig))
	{
		sTxt.Append(_T("	")).Append(_Name).Append(_T("->SetOuterStyle(OuterStyle::AutoConfig, true);\r\n"));
	}
	if(control->GetClassID() == CC_CLASSID_TextBox)
	{
		if(((TextBox*)control)->GetMultiline())
		{
			sTxt.Append(_T("	")).Append(_Name).Append(_T("->SetMultiline(true);\r\n"));
		}
		if(control->HasStyle(StyleType::Style, ES_NUMBER))
		{
			sTxt.Append(_T("	")).Append(_Name).Append(_T("->SetStyleNumber(true);\r\n"));
		}
	;
	}
	Control* ctl2 = control->GetParent();
	while(ctl2 != NULL && ctl2->GetParent() != NULL && ctl2->GetParent()->GetClient() == ctl2)
	{
		ctl2 = ctl2->GetParent();;
	}
	if(ctl2 != NULL)
	{
		sTxt.Append(_T("	")).Append(ctl2->GetName());
		if(control->GetClassID() == CC_CLASSID_TabPage)
		{
			sTxt.Append(_T("->AddPage("));
		}
		else
		{
			sTxt.Append(_T("->AddControl("));
		}
		sTxt.Append(_Name).Append(_T(");\r\n"));
	}
	sTxt.Append(_T("\r\n"));
	return true;
}

const TCHAR* ControlNameFromID(int nClassID)
{
	switch(nClassID)
	{
	case CC_CLASSID_Control:
		return _T("Control");
	case CC_CLASSID_ToolItem:
		return _T("ToolItem");
	case CC_CLASSID_StatusItem:
		return _T("StatusItem");
	case CC_CLASSID_LabelLink:
		return _T("LabelLink");
	case CC_CLASSID_Label:
		return _T("Label");
	case CC_CLASSID_PictureBox:
		return _T("PictureBox");
	case CC_CLASSID_Button:
		return _T("Button");
	case CC_CLASSID_CheckBox:
		return _T("CheckBox");
	case CC_CLASSID_RadioButton:
		return _T("RadioButton");
	case CC_CLASSID_Group:
		return _T("Group");
	case CC_CLASSID_SpinButton:
		return _T("SpinButton");
	case CC_CLASSID_ListBox:
		return _T("ListBox");
	case CC_CLASSID_CheckListBox:
		return _T("CheckListBox");
	case CC_CLASSID_ComboBox:
		return _T("ComboBox");
	case CC_CLASSID_ComboBoxEx:
		return _T("ComboBoxEx");
	case CC_CLASSID_TextBox:
		return _T("TextBox");
	case CC_CLASSID_RichTextBox:
		return _T("RichTextBox");
	case CC_CLASSID_ListView:
		return _T("ListView");
	case CC_CLASSID_TreeView:
		return _T("TreeView");
	case CC_CLASSID_DateTimePicker:
		return _T("DateTimePicker");
	case CC_CLASSID_MonthCalendar:
		return _T("MonthCalendar");
	case CC_CLASSID_Splitter:
		return _T("Splitter");
	case CC_CLASSID_Panel:
		return _T("Panel");
	case CC_CLASSID_ScrollContainer:
		return _T("ScrollContainer");
	case CC_CLASSID_TabPage:
		return _T("TabPage");
	case CC_CLASSID_TabControl:
		return _T("TabControl");
	case CC_CLASSID_TabControlEx:
		return _T("TabControlEx");
	case CC_CLASSID_TreeViewEx:
		return _T("TreeViewEx");
	case CC_CLASSID_GroupPanel:
		return _T("GroupPanel");
	case CC_CLASSID_GradientLabel:
		return _T("GradientLabel");
	case CC_CLASSID_Form:
		return _T("Form");
	case CC_CLASSID_Dialog:
		return _T("Dialog");
	case CC_CLASSID_IEControl:
		return _T("IEControl");
	case CC_CLASSID_MainMenu:
		return _T("MainMenu");
	case CC_CLASSID_ScrollBar:
		return _T("ScrollBar");
	case CC_CLASSID_HScrollBar:
		return _T("HScrollBar");
	case CC_CLASSID_VScrollBar:
		return _T("VScrollBar");
	case CC_CLASSID_ProgressBar:
		return _T("ProgressBar");
	case CC_CLASSID_ReBar:
		return _T("ReBar");
	case CC_CLASSID_DockBar:
		return _T("DockBar");
	case CC_CLASSID_ToolBar:
		return _T("ToolBar");
	case CC_CLASSID_StatusBar:
		return _T("StatusBar");
	case CC_CLASSID_MainMenuBar:
		return _T("MainMenuBar");
	case CC_CLASSID_UserControl:
		return _T("UserControl");
	}
	return _T("Control");
}

//!!!import,if use this,then many class is included(compiled) into your exe
Control* CC_APP_CreateControl(int nClassID)
{
	switch(nClassID)
	{
	case CC_CLASSID_Control:
		return new Control();
	case CC_CLASSID_ToolItem:
		return new ToolItem();
	case CC_CLASSID_StatusItem:
		return new StatusItem();
	case CC_CLASSID_LabelLink:
		return new LabelLink();
	case CC_CLASSID_Label:
		return new Label();
	case CC_CLASSID_PictureBox:
		return new PictureBox();
	case CC_CLASSID_Button:
		return new Button();
	case CC_CLASSID_CheckBox:
		return new CheckBox();
	case CC_CLASSID_RadioButton:
		return new RadioButton();
	case CC_CLASSID_Group:
		return new Group();
	case CC_CLASSID_SpinButton:
		return new SpinButton();
	case CC_CLASSID_ListBox:
		return new ListBox();
	case CC_CLASSID_CheckListBox:
		return new CheckListBox();
	case CC_CLASSID_ComboBox:
		return new ComboBox();
	case CC_CLASSID_ComboBoxEx:
		return new ComboBoxEx();
	case CC_CLASSID_TextBox:
		return new TextBox();
	case CC_CLASSID_RichTextBox:
		return new RichTextBox();
	case CC_CLASSID_ListView:
		return new ListView();
	case CC_CLASSID_TreeView:
		return new TreeView();
	case CC_CLASSID_DateTimePicker:
		return new DateTimePicker();
	case CC_CLASSID_MonthCalendar:
		return new MonthCalendar();
	case CC_CLASSID_Splitter:
		return new Splitter();
	case CC_CLASSID_Panel:
		return new Panel();
	case CC_CLASSID_ScrollContainer:
		return new ScrollContainer();
	case CC_CLASSID_TabPage:
		return new TabPage();
	case CC_CLASSID_TabControl:
		return new TabControl();
	case CC_CLASSID_TabControlEx:
		return new TabControlEx();
	case CC_CLASSID_TreeViewEx:
		return new TreeViewEx();
	case CC_CLASSID_GroupPanel:
		return new GroupPanel();
	case CC_CLASSID_GradientLabel:
		return new GradientLabel();
	}
	return NULL;
}

void CC_APP_DrawFocusFrame(cc::win::Control* control)
{
	if(((DWORD)control->Design & DesignStyle::Selected) != 0)
	{
		RECT rect;
		HDC hDC = ::GetDC(control->m_hWnd);
		HBRUSH brush = ::CreateSolidBrush(RGB(252, 110, 021));
		::GetWindowRect(control->m_hWnd, &rect);
		::ScreenToClient(control->m_hWnd, (LPPOINT)&rect);
		::ScreenToClient(control->m_hWnd, ((LPPOINT)&rect)+1);
		::InflateRect(&rect, -1, -1);
		::FrameRect(hDC, &rect, brush);
		::InflateRect(&rect, -1, -1);
		::FrameRect(hDC, &rect, brush);
		DeleteObject(brush);
		::ReleaseDC(control->m_hWnd, hDC);
	}
}
#endif

#if defined(_DEBUG_) || defined(_DESIGN) || defined(_DESIGNRUN)
Control* CC_APP_CreateControl(cc::Str sClassName)
{
	Control* control = NULL;
	if(sClassName.Equals(_T("Label")))
	{
		control = new Label();
	}
	else if(sClassName.Equals(_T("Button")))
	{
		control = new Button();
	}
	else if(sClassName.Equals(_T("CheckBox")))
	{
		control = new CheckBox();
	}
	else if(sClassName.Equals(_T("RadioButton")))
	{
		control = new RadioButton();
	}
	else if(sClassName.Equals(_T("ToolItem")))
	{
		control = new ToolItem();
	}
	else if(sClassName.Equals(_T("LabelLink")))
	{
		control = new LabelLink();
	}
	else if(sClassName.Equals(_T("TextBox")))
	{
		control = new TextBox();
	}
	else if(sClassName.Equals(_T("GradientLabel")))
	{
		control = new GradientLabel();
	}
	return control;
}
#endif
