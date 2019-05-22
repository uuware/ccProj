#include "DesktopCalendar.h"
using namespace cc;
using namespace cc::win;

/*
Australian Holidays,australian
Austrian Holidays,austrian
Brazilian Holidays,brazilian
Canadian Holidays,canadian
China Holidays,china
Christian Holidays,christian
Danish Holidays,danish
Dutch Holidays,dutch
Finnish Holidays,finnish
French Holidays,french
German Holidays,german
Greek Holidays,greek
Hong Kong Holidays,hong_kong
Indian Holidays,indian
Indonesian Holidays,indonesian
Iranian Holidays,iranian
Irish Holidays,irish
Islamic Holidays,islamic
Italian Holidays,italian
Japanese Holidays,japanese
Jewish Holidays,jewish
Malaysian Holidays,malaysia
Mexican Holidays,mexican
New Zealand Holidays,new_zealand
Norwegian Holidays,norwegian
Philippines Holidays,philippines
Polish Holidays,polish
Portuguese Holidays,portuguese
Russian Holidays,russian
Singapore Holidays,singapore
South Africa Holidays,sa
South Korean Holidays,south_korea
Spain Holidays,spain
Swedish Holidays,swedish
Taiwan Holidays,taiwan
Thai Holidays,thai
UK Holidays,uk
US Holidays,usa
Vietnamese Holidays,vietnamese
ƒAƒCƒ‹ƒ‰ƒ“ƒh‚Ìj“ú,irish
ƒAƒƒŠƒJ‚Ìj“ú,usa
ƒCƒMƒŠƒX‚Ìj“ú,uk
ƒCƒXƒ‰ƒ€‹³‚Ìj“ú,islamic
ƒCƒ^ƒŠƒA‚Ìj“ú,italian
ƒCƒ“ƒh‚Ìj“ú,indian
ƒCƒ“ƒhƒlƒVƒA‚Ìj“ú,indonesian
ƒIƒ‰ƒ“ƒ_‚Ìj“ú,dutch
ƒI[ƒXƒgƒ‰ƒŠƒA‚Ìj“ú,australian
ƒI[ƒXƒgƒŠƒA‚Ìj“ú,austrian
ƒJƒiƒ_‚Ìj“ú,canadian
ƒLƒŠƒXƒg‹³‚Ìj“ú,christian
ƒMƒŠƒVƒƒ‚Ìj“ú,greek
ƒVƒ“ƒKƒ|[ƒ‹‚Ìj“ú,singapore
ƒXƒEƒF[ƒfƒ“‚Ìj“ú,swedish
ƒXƒyƒCƒ“‚Ìj“ú,spain
ƒ^ƒC‚Ìj“ú,thai
ƒfƒ“ƒ}[ƒN‚Ìj“ú,danish
ƒhƒCƒc‚Ìj“ú,german
ƒjƒ…[ƒW[ƒ‰ƒ“ƒh‚Ìj“ú,new_zealand
ƒmƒ‹ƒEƒFƒC‚Ìj“ú,norwegian
ƒtƒBƒŠƒsƒ“‚Ìj“ú,philippines
ƒtƒBƒ“ƒ‰ƒ“ƒh‚Ìj“ú,finnish
ƒtƒ‰ƒ“ƒX‚Ìj“ú,french
ƒuƒ‰ƒWƒ‹‚Ìj“ú,brazilian
ƒxƒgƒiƒ€‚Ìj“ú,vietnamese
ƒ|ƒ‹ƒgƒKƒ‹‚Ìj“ú,portuguese
ƒ|[ƒ‰ƒ“ƒh‚Ìj“ú,polish
ƒ}ƒŒ[ƒVƒA‚Ìj“ú,malaysia
ƒƒLƒVƒR‚Ìj“ú,mexican
ƒ†ƒ_ƒ„‹³‚Ìj“ú,jewish
ƒƒVƒA‚Ìj“ú,russian
’†‘‚Ìj“ú,china
ŠØ‘‚Ìj“ú,south_korea
“ìƒAƒtƒŠƒJ‚Ìj“ú,sa
‘ä˜p‚Ìj“ú,taiwan
“ú–{‚Ìj“ú,japanese
`‚Ìj“ú,hong_kong
*/

WinInet* winInet = NULL;
HANDLE m_hEventTimer = NULL;
char sHead[] = "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\nContent-Type: application/x-www-form-urlencoded\r\n";
int nTimeOutMicrosecond = 60 * 1000;
//0:OK;-1:unknow error;-2:timeout;-3:break;-4:ErrGetResponseText;-5:NG;
int WaitReadyState(const char* url, const char* body, bool isClearSession, const TCHAR* msg, bool isGetHead, const char* end)
{
#if _DEBUG_
	cc::Log::debug("GetHTML, URL:%s, body:%s, isClearSession:%d, isGetHead:%d, nTimeOut:%d.", url, body, isClearSession, isGetHead, nTimeOutMicrosecond);
#endif
	if(winInet->BeginGetHTML(url, body, isClearSession, isGetHead, sHead))
	{
		clock_t tmStart = ::clock();
		while(winInet->GetResult() == WININET_GETHTML_BUSY && (int)(clock() - tmStart) < nTimeOutMicrosecond)
		{
			App::DoEvents();
			//Sleep(100);
			WaitForSingleObject(m_hEventTimer, 100);
		}
	}
	else
	{
		//SetStatus(_T("GetHTML, busy..."));
		return -1;
	}

	int nRet = winInet->GetResult();
#if _DEBUG_
cc::Log::debug("GetHTML, nRet:%d, Reading:%d, Total:%d.", winInet->GetResult(), winInet->GetReadingSize(), winInet->GetTotalSize());
#endif
	if(nRet == WININET_GETHTML_CANCELED)
	{
		//SetStatus(_T("GetHTML, canceled..."));
		winInet->Close(true);
		return nRet;
	}
	else if(nRet == WININET_GETHTML_READING || nRet == WININET_GETHTML_BUSY)
	{
		//SetStatus(_T("GetHTML, busy..."));
		winInet->Close(true);
		return nRet;
	}
#if _DEBUG_
	//!!!notice,cannot direct use (char*)winInet->GetResponseText().GetBuf() for cc::Log::debug
	cc::Str sTmpHtml = cc::Str(LangCode::TCHAR, (char*)winInet->GetResponseText().GetBuf(), LangCode::UTF8);
	cc::Log::debug("GetHTML, nRet:%d, URL:%s, State:%d\r\n.", nRet, url, winInet->GetResponseCode());
	cc::Log::debug("GetHTML, Head:\r\n%s\r\n.", (char*)winInet->GetResponseHeaders().GetBuf());
	cc::Log::debug(_T("GetHTML, Body:\r\n%s\r\n."), (TCHAR*)sTmpHtml.GetBuf());
#endif

	while(nRet == 0 && winInet->GetResponseCode() == 302)
	{
		cc::Str sLocation = winInet->GetResponseHeader("Location", LangCode::SYSTEM);
		if(sLocation.GetLength() < 10)
		{
			//SetStatus(_T("GetHTML, redirect error..."));
			winInet->Close(true);
			return -1;
		}
		int nPos = sLocation.IndexOf("/", 8);
		cc::Str sHost = sLocation.Substring(8, nPos);
		sLocation.Remove(0, nPos);
		sHost.ToTChar();
#if _DEBUG_
		cc::Log::debug("GetHTML, sLocation:%s.", (char*)winInet->GetResponseHeader("Location", LangCode::SYSTEM).GetBuf());
		cc::Log::debug(_T("GetHTML, sHost:%s."), (TCHAR*)sHost.GetBuf());
		cc::Log::debug("GetHTML, sLocation:%s.", (char*)sLocation.GetBuf());
#endif

		winInet->SetHost((TCHAR*)sHost.GetBuf(), 443, true);
		nRet = winInet->GetHTML((char*)sLocation.GetBuf(), NULL, false, isGetHead);
#if _DEBUG_
		cc::Log::debug("GetHTML, nRet:%d, URL:%s, Head:\r\n%s\r\nState:%d\r\n.", nRet, (char*)sLocation.GetBuf(), winInet->GetResponseHeaders(), winInet->GetResponseCode());
		cc::Log::debug("GetHTML, URL:%s, Body:\r\n[%s]\r\n.", (char*)sLocation.GetBuf(), (char*)winInet->GetResponseText().GetBuf());
#endif
	}
	return nRet;
}

int GetCalendarList(const char* country, int year, cc::Str& sTitle, cc::KeyArr& keyArr)
{
	winInet = NULL;
	m_hEventTimer = NULL;
	if(winInet == NULL)
	{
		winInet = new WinInet();
		winInet->SetAgent(_T("Mozilla/5.0 (Windows NT 6.1; WOW64; rv:22.0) Gecko/20100101 Firefox/22.0"));
		//winInet->SetTimeout(nTimeOutMicrosecond);
		//WinInetConfig::nReadBufferSize = 10240;
		WinInetConfig::nReadLength = 1024;
	}
	if(m_hEventTimer == NULL)
	{
		m_hEventTimer = ::CreateEvent(NULL, TRUE, FALSE, NULL);
	}

	//winInet->SetProxy(SOCKET_PROXY_HTTP11, _T("web-proxy.atl.hp.com"), 8080);
	TCHAR loginHOST[] = _T("www.google.com");
	winInet->SetHost(loginHOST, 80, false);

	//char sLogin[] = "/calendar/feeds/japanese__ja@holiday.calendar.google.com/public/basic?start-min=2015-01-01&start-max=2015-12-31&max-results=100&alt=json";
	char sLogin[] = "/calendar/feeds/%s@holiday.calendar.google.com/public/basic?start-min=%d-01-01&start-max=%d-12-31&max-results=100&alt=json";
	cc::Str sUrl = cc::Str(LangCode::SYSTEM);
	sUrl.Format(sLogin, country, year, year);
	//cc::Str sBody = cc::Str(LangCode::SYSTEM);
	//sBody.Append("accountType=HOSTED_OR_GOOGLE&Email=fromaboy@gmail.com&Passwd=xx&&service=writely");
	int nRet = WaitReadyState((char*)sUrl.GetBuf(), NULL, true, NULL, true, NULL);

	const cc::Str sTxt = winInet->GetResponseText();
	Json json;
	int ret = json.Parse((const char*)sTxt.GetBuf());
	//cc::Str out = json.ToJSon(LangCode::UNICODE16, "\r\n");
	//cc::Log::debug(L"%s", (wchar_t*)out.GetBuf());

	cc::Json* jEntry = json.GetDeepSub("feed.entry");
	if(jEntry != NULL)
	{
		cc::Json* jTitle = json.GetDeepSub("feed.title.$t");
		if(jTitle != NULL)
		{
			sTitle = cc::Str(LangCode::UNICODE16, jTitle->GetString());
		}

		//cc::Str out = cc::Str(LangCode::UNICODE16, jEntry->ToJSon("\r\n"));
		//cc::Log::debug(L"%s", (wchar_t*)out.GetBuf());
		int size = jEntry->GetSize();
		for(int i = 0; i < size; i++)
		{
			cc::Str id = cc::Str(LangCode::UNICODE16, jEntry->GetAt(i)->GetDeepSub("id.$t")->GetString());
			cc::Str dt = cc::Str(LangCode::UNICODE16);
			//http://.../public/basic/20150211_60o30d9l6oo30c1g60o30dr4co
			cc::Util::GetHtmlTag(id, 0, "/basic/", -1, "_", -1, dt);
			cc::Str title = cc::Str(LangCode::UNICODE16, jEntry->GetAt(i)->GetDeepSub("title.$t")->GetString());
			//cc::Log::debug(L"id:%s, date:%s", (wchar_t*)title.GetBuf(), (wchar_t*)dt.GetBuf());
			keyArr.Set(dt, title);
		}
	}
	if(winInet != NULL)
	{
		winInet->Close(true, true, true);
		delete winInet;
		winInet = NULL;
	}
	if(m_hEventTimer != NULL)
	{
		::CloseHandle(m_hEventTimer);
		m_hEventTimer = NULL;
	}
	return 0;
}

int CheckCalendarExist(cc::Str& sItem, cc::Str& sRet)
{
	cc::Str sFile = cc::Str(LangCode::SYSTEM);
	cc::Str sPath = cc::Util::getStartupPath(true);
	if(sItem.GetLength() > 0)
	{
		cc::Str sValue2;
		cc::Util::GetHtmlTag(sItem, 0, _T("["), 1, _T("]"), 1, sValue2);
		if(sValue2.GetLength() > 0)
		{
			sFile.Format(_T("%sCalendar_%s.ini"), (TCHAR*)sPath.GetBuf(), (TCHAR*)sValue2.GetBuf());
			if(cc::Util::fExist(sFile))
			{
				sRet = sValue2;
			}
			else
			{
				return -1;
			}
		}
		else
		{
			return -2;
		}
	}
	return 0;
}

int SaveCalendarList(const char* country, int year, cc::Str& path)
{
	cc::Str sTitle;
	cc::KeyArr keyArr;
	cc::KeyArr keyArrSave;
	int ret = GetCalendarList(country, year, sTitle, keyArr);
	if(ret >= 0 && keyArr.GetSize() > 0)
	{
		cc::Str sTmp;
		cc::Str sFile = cc::Str(LangCode::SYSTEM);
		cc::Str sPath = cc::Str(LangCode::SYSTEM, cc::Util::getStartupPath(true));
		sFile.Format("%sCalendar_%s.ini", (char*)sPath.GetBuf(), country);
		if(cc::Util::fExist(sFile))
		{
			keyArrSave.Add(sFile, LangCode::UNICODE16, true);
		}

		sTmp.Format(_T("main.%d"), year);
		keyArrSave.Clear(sTmp);

		keyArrSave.Set(_T("main.title"), sTitle);
		cc::Log::debug(_T("title:%s"), (TCHAR*)sTitle.GetBuf());
		for(int i = 0; i < keyArr.GetSize(); i++)
		{
			cc::Str key = keyArr.GetKey(i);
			cc::Str val = keyArr.Get(key).Replace(_T("&#39;"), _T("'"));
			sTmp.Format(_T("main.%s"), (TCHAR*)key.GetBuf());
			cc::Log::debug(_T("date:%s, info:%s"), (TCHAR*)key.GetBuf(), (TCHAR*)val.GetBuf());
			keyArrSave.Set(sTmp, val);
		}
		path = sFile;
		keyArrSave.Save(sFile, LangCode::UNICODE16, true);
		return 0;
	}
	return -1;
}

void MeasureDaySize(HWND m_hWnd, RECT* rect)
{
	//if m_hWnd is NULL,then get screen HDC
	HDC hDC = ::GetDC(m_hWnd);
	SetRectEmpty(rect);
	HFONT holdFont = (HFONT)::SelectObject(hDC, CMainForm::dayFont);
	::DrawText(hDC, _T("WW"), -1, rect, DT_CALCRECT);
	::SelectObject(hDC, holdFont);
	::ReleaseDC(m_hWnd, hDC);
}

#ifndef _CC_ASSISTANT
///////////////////////////////////////////////////////////////////////////////
//WinMain
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//when add manifest.xml for xp theme, also need set controls to have default teansparent
	//so while some controls need backcolor, need remove  (OuterStyle::TransparentParentBrush|OuterStyle::TransparentParentErase)
	cc::win::App::SetDefaultTransparent(true);
	cc::win::App::SetInstance(hInstance);
	//should set this before cc::Config::GetLang(),and SetIsAutoSave is before LoadLang
	cc::win::App::GetConfig()->SetIsAutoSave(true, true, REG_REGKEY);
	cc::win::App::GetConfig()->LoadLang(CC_CONFIG_LANGENG);
	//the name is used for CreateMutex"s flages,cannot change to other lang
	cc::win::App::SetNameVersion(NAMEVER, _T("1.00"));
	if(!cc::win::App::IsFirstInstance(NULL))
	{
		return 0;
	}

	//set Transparent for TabControlEx
	cc::win::FormConfig::nInnerStyleClient = InnerStyle::BackgroundTheme|FormClientConfig::nInnerStyle;
	cc::win::TextBoxConfig::nOuterStyle |= OuterStyle::TransparentParentBrush;
	//should set next for RichTextBox
	//rich->SetStyle(StyleType::ExStyle, 0, WS_EX_TRANSPARENT);
	//rich->SetOuterStyle(OuterStyle::TransparentHollowBrush, true);

	cc::win::App::SetAlwaysTips(true);
	CMainForm* form = new CMainForm(NULL);
	cc::win::App::Run(form);
	form->Dispose();
	delete form;
	return 0;
}
#endif

///////////////////////////////////////////////////////////////////////////////
//EditDialog
EditDialog::EditDialog(cc::KeyArr* arrMemo, HWND hWndCalendar)
{
	this->m_arrMemo = arrMemo;
	this->m_hWndCalendar = hWndCalendar;

#define CC_CMEMO_LIST_WIDTH 70
#define CC_CMEMO_LIST_HEIGHT 140
#define CC_CMEMO_TXT_WIDTH 200
	m_oldIndex = -1;
	m_oldColor = (COLORREF)-1;
	int nTop = 1;
	Label* label;

	label = new Label();
	label->SetName(_T("labList"));
	label->SetText(_T("Memo List:"));
	label->SetLocation(3, nTop);
	label->SetSize(CC_CMEMO_LIST_WIDTH, 17);
	this->AddControl(label);

	label = new Label();
	label->SetName(_T("labMemo"));
	label->SetText(_T("Memo:"));
	label->SetLocation(CC_CMEMO_LIST_WIDTH + 5, nTop);
	label->SetSize(CC_CMEMO_TXT_WIDTH, 17);
	this->AddControl(label);

	nTop += 17;
	listMemo = new ListBox();
	listMemo->SetName(_T("listMemo"));
	listMemo->SetLocation(3, nTop);
	listMemo->SetSize(CC_CMEMO_LIST_WIDTH, CC_CMEMO_LIST_HEIGHT);
	listMemo->SelectedIndexChanged += EventHandler((Object*)this, (EventFun)&EditDialog::Control_Click);
	listMemo->Anchor = (AnchorStyles::Type)(AnchorStyles::Top | AnchorStyles::Left | AnchorStyles::Bottom);
	listMemo->SetSort(true);
	this->AddControl(listMemo);

	editText = new TextBox();
	editText->SetName(_T("editText"));
	editText->SetText(_T(""));
	editText->SetMultiline(true);
	editText->SetLocation(CC_CMEMO_LIST_WIDTH + 5, nTop);
	editText->SetSize(CC_CMEMO_TXT_WIDTH, CC_CMEMO_LIST_HEIGHT);
	editText->Anchor = (AnchorStyles::Type)(AnchorStyles::Top | AnchorStyles::Left | AnchorStyles::Right | AnchorStyles::Bottom);
	this->AddControl(editText);

	nTop += 143;
	btnOK = new Button();
	btnOK->SetName(_T("btnOK"));
	btnOK->SetText(_T("Close"));
	btnOK->SetLocation(3, nTop);
	btnOK->SetSize(65, 19);
	btnOK->Click += EventHandler((Object*)this, (EventFun)&EditDialog::Control_Click);
	btnOK->Anchor = (AnchorStyles::Type)(AnchorStyles::Left | AnchorStyles::Bottom);
	this->AddControl(btnOK);
	this->SetOKButton(btnOK);

	btnRemove = new Button();
	btnRemove->SetName(_T("btnRemove"));
	btnRemove->SetText(_T("Remove"));
	btnRemove->SetLocationOffset(btnOK, false, 2, 0);
	btnRemove->SetSize(65, 19);
	btnRemove->Click += EventHandler((Object*)this, (EventFun)&EditDialog::Control_Click);
	btnRemove->Anchor = (AnchorStyles::Type)(AnchorStyles::Left | AnchorStyles::Bottom);
	this->AddControl(btnRemove);

	pickColor = new PickColorCtl();
	pickColor->SetName(_T("pickColor"));
	pickColor->SetText(_T(" "));
	pickColor->SetOuterStyle(OuterStyle::AutoSize, false);
	pickColor->SetOuterStyle(OuterStyle::OuterEdge, false);
	pickColor->SetLocationOffset(btnRemove, false, 2, 1);
	pickColor->SetSize(40, 18);
	//pickColor->SetStyle(StyleType::Outer, 0, OuterStyle::AutoConfig);
	//pickColor->SetBackground(corCalendar1);
	pickColor->Anchor = (AnchorStyles::Type)(AnchorStyles::Left | AnchorStyles::Bottom);
	this->AddControl(pickColor);

	this->SetName(_T("EditDialog"));
	this->SetText(_T("Edit Calendar Memo"));
	//this->SetStyle(StyleType::Style, 0, WS_THICKFRAME);
	//this->SetSize(300, nTop + 19 + 10);
	this->SetSizeBaseOnClient(CC_CMEMO_LIST_WIDTH + CC_CMEMO_TXT_WIDTH + 7, nTop + 17 + 5);
	SetLang(*CC_APP_GetLang(), cc::Str(_T("EditDialog")));
	this->SetAlwaysOnTop(true);
	this->SetCloseButtonEnable(false);

	this->Load += EventHandler((Object*)this, (EventFun)&EditDialog::Form_Load);
	this->Closed += EventHandler((Object*)this, (EventFun)&EditDialog::Form_Closed);
	this->Closing += CancelEventHandler((Object*)this, (CancelEventFun)&EditDialog::Form_Closing);
}

void EditDialog::Form_Load(Object* sender, EventArgs* e)
{
	//load config
	//this->Config(true, *CC_APP_GetIni(), cc::Str(_T("")));
	for(int i = 0; i < m_arrMemo->GetSize(); i++)
	{
		cc::Str key = m_arrMemo->GetKey(i);
		if(key.GetLength() == 8)
		{
			key.Insert(6, _T("/")).Insert(4, _T("/"));
		}
		listMemo->ItemAdd(key);
	}
}

void EditDialog::Form_Closing(Object* sender, CancelEventArgs* e)
{
	if(e->baseArgs->LParam == 1 && e->baseArgs->WParam == 1) {
		//exit from menu
		return;
	}
	::ShowWindow(m_hWnd, SW_MINIMIZE);
	SetVisible(false);
	//not close but goto NotifyIcon
	e->Cancel = true;
}

void EditDialog::Form_Closed(Object* sender, EventArgs* e)
{
	SaveCurrentText();
}

void EditDialog::Control_Click(Object* sender, EventArgs* e)
{
	if(sender == btnOK)
	{
		//save config
		//this->Config(false, *CC_APP_GetIni(), cc::Str(_T("")));

		//EndDialog(DialogResult::OK);
		SaveCurrentText();
		this->SetVisible(false);
		::SetTimer(m_hWndCalendar, UU_TIMERID_EDITDAY_EXIT, 100, NULL);
	}
	else if(sender == btnRemove)
	{
		m_oldIndex = -1;
		editText->SetText(_T(""));
		int ind = listMemo->GetSelectedIndex();
		if(ind >= 0)
		{
			cc::Str key = listMemo->ItemGet(ind);
			key.Replace(_T("/"), _T(""));
			m_arrMemo->Remove(key);
			listMemo->ItemDel(ind);
		}
		if(ind >= listMemo->ItemCount())
		{
			ind--;
		}
		if(ind < 0)
		{
			ind = 0;
		}
		listMemo->SetSelectedIndex(ind);
		Control_Click(listMemo, NULL);
		::SetTimer(m_hWndCalendar, UU_TIMERID_EDITDAY_FRESH, 100, NULL);
	}
	else if(sender == listMemo)
	{
		int ind = listMemo->GetSelectedIndex();
		if(m_oldIndex != ind)
		{
			SaveCurrentText();
			m_oldIndex = ind;
			cc::Str key = listMemo->ItemGet(ind);
			key.Replace(_T("/"), _T(""));
			cc::Str sMemo = m_arrMemo->Get(key);
			cc::Str sColor = sMemo.SplitBy(_T(","));
			sMemo.SplitBy(_T(","));
			sMemo.SplitBy(_T(","));

			editText->SetText(sMemo);
			editText->SetIsDirty(false);
			editText->Focus();
			if(sColor.GetLength() == 0)
			{
				m_oldColor = RGB(255, 255, 0);
			}
			else
			{
				m_oldColor = (COLORREF)sColor.ToInt();
			}
			pickColor->SetBackground(m_oldColor);
		}
	}
}

void EditDialog::EditDate(cc::Str sDate)
{
	if(sDate.GetLength() == 8)
	{
		sDate.Insert(6, _T("/")).Insert(4, _T("/"));
	}

	SaveCurrentText();
	int ind = listMemo->FindStringExact(0, sDate);
	if(ind < 0)
	{
		ind = listMemo->ItemAdd(sDate);
	}
	listMemo->SetSelectedIndex(ind);
	Control_Click(listMemo, NULL);
	editText->Focus();
}

void EditDialog::SaveCurrentText()
{
	if(m_oldIndex >= 0 && (editText->GetIsDirty() || m_oldColor != pickColor->GetBackground()))
	{
		m_oldColor = pickColor->GetBackground();
		cc::Str key = listMemo->ItemGet(m_oldIndex);
		key.Replace(_T("/"), _T(""));
		cc::Str txt = editText->GetText();
		if(txt.GetLength() == 0 && m_oldColor == RGB(255, 255, 0))
		{
			m_arrMemo->Remove(key);
		}
		else
		{
			cc::Str str;
			str.AppendFormat(_T("%d,0,0,"), (int)m_oldColor);
			str.Append(txt);
			m_arrMemo->Set(key, str);
		}
		m_oldIndex = -1;

		::SetTimer(m_hWndCalendar, UU_TIMERID_EDITDAY_FRESH, 100, NULL);
	}
}

///////////////////////////////////////////////////////////////////////////////
//SetDialog
SetDialog::SetDialog()
{
	int nTop = 10;
	Label* label;
	cc::Time time = cc::Time::GetCurrentTime();
	cc::Str str;
	str.Format(_T("%d"), time.GetYear());

	label = new Label();
	label->SetName(_T("labHoliday1"));
	label->SetText(_T("Holiday Calendar 1:"));
	label->SetLocation(3, nTop);
	label->SetSize(200, 17);
	this->AddControl(label);

	label = new Label();
	label->SetName(_T("labColor1"));
	label->SetText(_T("Show Color:"));
	label->SetLocation(220, nTop);
	label->SetSize(100, 17);
	this->AddControl(label);

	nTop += 18;
	comboC1 = new ComboBox();
	comboC1->SetName(_T("comboC1"));
	comboC1->SetComboBoxStyle(ComboBoxStyle::DropDownList);
	comboC1->SetLocation(10, nTop);
	comboC1->SetSize(200, 145);
	this->AddControl(comboC1);

	COLORREF corCalendar1 = CC_APP_GetIni()->Get(_T("Main.CalendarColor1"), RGB(255, 0, 0));
	COLORREF corCalendar2 = CC_APP_GetIni()->Get(_T("Main.CalendarColor2"), RGB(0, 255, 0));
	COLORREF corCalendar3 = CC_APP_GetIni()->Get(_T("Main.CalendarColor3"), RGB(0, 255, 255));

	pickColor1 = new PickColorCtl();
	pickColor1->SetName(_T("pickColor1"));
	pickColor1->SetText(_T(" "));
	pickColor1->SetOuterStyle(OuterStyle::AutoSize, false);
	pickColor1->SetOuterStyle(OuterStyle::OuterEdge, false);
	pickColor1->SetLocationOffset(comboC1, false, 1, 2);
	pickColor1->SetSize(48, 18);
	pickColor1->SetStyle(StyleType::Outer, 0, OuterStyle::AutoConfig);
	pickColor1->SetBackground(corCalendar1);
	this->AddControl(pickColor1);

	nTop += 21;
	label = new Label();
	label->SetName(_T("labHoliday2"));
	label->SetText(_T("Holiday Calendar 2:"));
	label->SetLocation(3, nTop);
	label->SetSize(200, 17);
	this->AddControl(label);

	label = new Label();
	label->SetName(_T("labColor2"));
	label->SetText(_T("Show Color:"));
	label->SetLocation(220, nTop);
	label->SetSize(100, 17);
	this->AddControl(label);

	nTop += 18;
	comboC2 = new ComboBox();
	comboC2->SetName(_T("comboC2"));
	comboC2->SetComboBoxStyle(ComboBoxStyle::DropDownList);
	comboC2->SetLocation(10, nTop);
	comboC2->SetSize(200, 145);
	this->AddControl(comboC2);

	pickColor2 = new PickColorCtl();
	pickColor2->SetName(_T("pickColor1"));
	pickColor2->SetText(_T(" "));
	pickColor2->SetOuterStyle(OuterStyle::AutoSize, false);
	pickColor2->SetOuterStyle(OuterStyle::OuterEdge, false);
	pickColor2->SetLocationOffset(comboC2, false, 1, 2);
	pickColor2->SetSize(48, 18);
	pickColor2->SetStyle(StyleType::Outer, 0, OuterStyle::AutoConfig);
	pickColor2->SetBackground(corCalendar2);
	this->AddControl(pickColor2);

	nTop += 21;
	label = new Label();
	label->SetName(_T("labHoliday3"));
	label->SetText(_T("Holiday Calendar 3:"));
	label->SetLocation(3, nTop);
	label->SetSize(200, 17);
	this->AddControl(label);

	label = new Label();
	label->SetName(_T("labColor3"));
	label->SetText(_T("Show Color:"));
	label->SetLocation(220, nTop);
	label->SetSize(100, 17);
	this->AddControl(label);

	nTop += 18;
	comboC3 = new ComboBox();
	comboC3->SetName(_T("comboC3"));
	comboC3->SetComboBoxStyle(ComboBoxStyle::DropDownList);
	comboC3->SetLocation(10, nTop);
	comboC3->SetSize(200, 145);
	this->AddControl(comboC3);

	pickColor3 = new PickColorCtl();
	pickColor3->SetName(_T("pickColor3"));
	pickColor3->SetText(_T(" "));
	pickColor3->SetOuterStyle(OuterStyle::AutoSize, false);
	pickColor3->SetOuterStyle(OuterStyle::OuterEdge, false);
	pickColor3->SetLocationOffset(comboC3, false, 1, 2);
	pickColor3->SetSize(48, 18);
	pickColor3->SetStyle(StyleType::Outer, 0, OuterStyle::AutoConfig);
	pickColor3->SetBackground(corCalendar3);
	this->AddControl(pickColor3);

	nTop += 21;
	label = new Label();
	label->SetName(_T("labHoliday2"));
	label->SetText(_T("Download Holid Data from Google Calendar:"));
	label->SetLocation(3, nTop);
	label->SetSize(300, 17);
	this->AddControl(label);

	nTop += 18;
	comboC0 = new ComboBox();
	comboC0->SetName(_T("comboC0"));
	comboC0->SetComboBoxStyle(ComboBoxStyle::DropDownList);
	comboC0->SetLocation(10, nTop);
	comboC0->SetSize(200, 145);
	this->AddControl(comboC0);

	editY0 = new TextBox();
	editY0->SetName(_T("editY0"));
	editY0->SetText(str);
	editY0->SetLocationOffset(comboC0, false, 1, 2);
	editY0->SetSize(40, 17);
	this->AddControl(editY0);

	spanY0 = new SpinButton();
	spanY0->SetName(_T("spanY0"));
	spanY0->SetLocationOffset(editY0, false, 0, 0);
	spanY0->SetBuddy(editY0);
	spanY0->SetRange(2000, 2099);
	spanY0->SetBase(10);
	spanY0->SetPos(time.GetYear());
	spanY0->SetOuterStyle(OuterStyle::AutoConfig, true);
	this->AddControl(spanY0);
	
	nTop += 21;
	btnDl0 = new Button();
	btnDl0->SetName(_T("btnDl0"));
	btnDl0->SetText(_T("Download"));
	btnDl0->SetLocation(10, nTop);
	btnDl0->SetSize(100, 19);
	btnDl0->Click += EventHandler((Object*)this, (EventFun)&SetDialog::Control_Click);
	this->AddControl(btnDl0);
	
	btnOpen0 = new Button();
	btnOpen0->SetName(_T("btnOpen0"));
	btnOpen0->SetText(_T("Open saved folder"));
	btnOpen0->SetLocationOffset(btnDl0, false, 2, 0);
	btnOpen0->SetSize(150, 19);
	btnOpen0->Click += EventHandler((Object*)this, (EventFun)&SetDialog::Control_Click);
	this->AddControl(btnOpen0);

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
	btnCancel->SetLocationOffset(btnOK, false, 2, 0);
	btnCancel->SetSize(70, 19);
	btnCancel->Click += EventHandler((Object*)this, (EventFun)&SetDialog::Control_Click);
	this->AddControl(btnCancel);
	this->SetCancelButton(btnCancel);

	this->SetName(_T("SetDlg"));
	this->SetText(_T("Setting Calendar"));
	//this->SetStyle(StyleType::Style, 0, WS_THICKFRAME);
	//this->SetSize(300, nTop + 19 + 10);
	this->SetSizeBaseOnClient(300, nTop + 17 + 5);
	SetLang(*CC_APP_GetLang(), cc::Str(_T("SetDialog")));

	this->Load += EventHandler((Object*)this, (EventFun)&SetDialog::Form_Load);
	this->Closed += EventHandler((Object*)this, (EventFun)&SetDialog::Form_Closed);
}

void SetDialog::Form_Load(Object* sender, EventArgs* e)
{
	//load config
	//this->Config(true, *CC_APP_GetIni(), cc::Str(_T("")));
	int nCnt = CC_APP_GetIni()->Get(_T("Lang.CalendarList.Count"), 0);

	TCHAR calendar_all[] = _T("Holidays in Japan[japanese]\t“ú–{‚Ìj“ú[japanese__ja]\tHolidays in China[china]\t’†‘˜ï“ú[china__zh]\tHolidays in Taiwan[taiwan]\t‘ä˜p˜ï“ú[taiwan__zh]\tHolidays in HongKong[hong_kong]\t`˜ï“ú[hong_kong__zh]\tHolidays in USA[usa]\tHolidays in Australian[australian]\tHolidays in Austrian[austrian]\tHolidays in Brazilian[brazilian]\tHolidays in Canadian[canadian]\tHolidays in Christian[christian]\tHolidays in Danish[danish]\tHolidays in Dutch[dutch]\tHolidays in Finnish[finnish]\tHolidays in French[french]\tHolidays in German[german]\tHolidays in Greek[greek]\tHolidays in Indian[indian]\tHolidays in Indonesian[indonesian]\tHolidays in Iranian[iranian]\tHolidays in Irish[irish]\tHolidays in Islamic[islamic]\tHolidays in Italian[italian]\tHolidays in Jewish[jewish]\tHolidays in Malaysian[malaysia]\tHolidays in Mexican[mexican]\tHolidays in New Zealand[new_zealand]\tHolidays in Norwegian[norwegian]\tHolidays in Philippines[philippines]\tHolidays in Polish[polish]\tHolidays in Portuguese[portuguese]\tHolidays in Russian[russian]\tHolidays in Singapore[singapore]\tHolidays in South Africa[sa]\tHolidays in South Korean[south_korea]\tHolidays in Spain[spain]\tHolidays in Swedish[swedish]\tHolidays in Thai[thai]\tHolidays in UK[uk]\tHolidays in Vietnamese[vietnamese]\t");
	cc::Str CalendarList = CC_APP_GetIni()->Get(_T("Lang.CalendarList"), calendar_all);
	cc::Str sItem;
	sItem = CalendarList.SplitBy(_T("\t"));
	comboC1->ItemClear();
	comboC2->ItemClear();
	comboC3->ItemClear();
	comboC0->ItemClear();
	while(sItem.GetLength() > 0)
	{
		comboC1->ItemAdd(sItem);
		comboC2->ItemAdd(sItem);
		comboC3->ItemAdd(sItem);
		comboC0->ItemAdd(sItem);
		sItem = CalendarList.SplitBy(_T("\t"));
	}

	cc::Str nameCalendar1 = CC_APP_GetIni()->Get(_T("Main.Calendar1"), _T("japanese__ja"));
	cc::Str nameCalendar2 = CC_APP_GetIni()->Get(_T("Main.Calendar2"), _T("china__zh"));
	cc::Str nameCalendar3 = CC_APP_GetIni()->Get(_T("Main.Calendar3"), _T(""));
	int ind;
	ind = comboC1->FindStringOption(0, nameCalendar1, 0, true);
	if(ind >= 0)
	{
		comboC1->SetSelectedIndex(ind);
	}

	ind = comboC2->FindStringOption(0, nameCalendar2, 0, true);
	if(ind >= 0)
	{
		comboC2->SetSelectedIndex(ind);
	}

	ind = comboC3->FindStringOption(0, nameCalendar3, 0, true);
	if(ind >= 0)
	{
		comboC3->SetSelectedIndex(ind);
	}
}

void SetDialog::Form_Closed(Object* sender, EventArgs* e)
{
}

void SetDialog::Control_Click(Object* sender, EventArgs* e)
{
	if(sender == btnOK)
	{
		//save config
		//this->Config(false, *CC_APP_GetIni(), cc::Str(_T("")));

		CC_APP_GetIni()->Set(_T("Main.CalendarColor1"), pickColor1->GetBackground());
		CC_APP_GetIni()->Set(_T("Main.CalendarColor2"), pickColor2->GetBackground());
		CC_APP_GetIni()->Set(_T("Main.CalendarColor3"), pickColor3->GetBackground());

		bool isMsg = false;
		cc::Str sItem;
		cc::Str nameCalendar;
		cc::Str sFile = cc::Str(LangCode::SYSTEM);
		cc::Str sPath = cc::Util::getStartupPath(true);
		sItem = comboC1->GetText();
		nameCalendar.Clear();
		if(CheckCalendarExist(sItem, nameCalendar) < 0)
		{
			isMsg = true;
		}
		CC_APP_GetIni()->Set(_T("Main.Calendar1"), nameCalendar);

		sItem = comboC2->GetText();
		nameCalendar.Clear();
		if(CheckCalendarExist(sItem, nameCalendar) < 0)
		{
			isMsg = true;
		}
		CC_APP_GetIni()->Set(_T("Main.Calendar2"), nameCalendar);

		sItem = comboC3->GetText();
		nameCalendar.Clear();
		if(CheckCalendarExist(sItem, nameCalendar) < 0)
		{
			isMsg = true;
		}
		CC_APP_GetIni()->Set(_T("Main.Calendar3"), nameCalendar);

		if(isMsg)
		{
			::MessageBox(m_hWnd, _T("The File for selected Google Calendar ID is not exist, download it first."), _T("Stop"), MB_OK | MB_ICONSTOP);
			return;
		}
		EndDialog(DialogResult::OK);
	}
	else if(sender == btnCancel)
	{
		EndDialog(DialogResult::Cancel);
	}
	else if(sender == btnDl0)
	{
		cc::Str coutry = comboC0->GetText();
		cc::Str str = editY0->GetText();
		if(coutry.GetLength() <= 0)
		{
			::MessageBox(m_hWnd, _T("Need select right google Calendar ID for download."), _T("Stop"), MB_OK | MB_ICONSTOP);
			return;
		}
		cc::Str sValue = cc::Str(LangCode::SYSTEM);
		cc::Util::GetHtmlTag((TCHAR*)coutry, 0, _T("["), 1, _T("]"), 1, sValue);
		if(sValue.GetLength() <= 0)
		{
			::MessageBox(m_hWnd, _T("Selected google Calendar ID is not right."), _T("Stop"), MB_OK | MB_ICONSTOP);
			return;
		}

		if(str.GetLength() != 4)
		{
			::MessageBox(m_hWnd, _T("Need input right year for download."), _T("Stop"), MB_OK | MB_ICONSTOP);
			return;
		}

		if(IDYES != (int)::MessageBox(m_hWnd, _T("Start download?\r\n* Make sure that you can access the Internet on your computer.\r\n* If uses a proxy server, set it at Internet Explorer [not Automatically detect settings!]"), _T("Question"), MB_YESNO | MB_ICONQUESTION))
		{
			return;
		}

		cc::Str filepath;
		int ret = SaveCalendarList((char*)sValue, str.ToInt(), filepath);
		if(ret < 0)
		{
			::MessageBox(m_hWnd, _T("Download NG."), _T("Stop"), MB_OK | MB_ICONSTOP);
			return;
		}
		else
		{
			if(IDYES == (int)::MessageBox(m_hWnd, _T("Download OK.\r\nOpen Explorer for the downloaded file?"), _T("Info"), MB_YESNO | MB_ICONQUESTION))
			{
				cc::Util::openExplorerSelectedFile(filepath, m_hWnd);
			}
		}
	}
	else if(sender == btnOpen0)
	{
		cc::Str filepath = cc::Util::getStartupPath();
		cc::Util::openExplorerSelectedFile(filepath, m_hWnd);
	}
}

/////////////////////////////////////////////////////////////////////////////// CalendarItem
CalendarItem::CalendarItem()
{
	this->SetOuterStyle(OuterStyle::TransparentParentBrush, true);
	colorDot = (COLORREF)-1;
	color1 = (COLORREF)-1;
	color2 = (COLORREF)-1;
	color3 = (COLORREF)-1;
	isToday = false;
}
CalendarItem::~CalendarItem()
{
}

COLORREF CalendarItem::GetColorDot()
{
	return colorDot;
}
void CalendarItem::SetColorDot(COLORREF value)
{
	colorDot = value;
}

COLORREF CalendarItem::GetColor1()
{
	return color1;
}
void CalendarItem::SetColor1(COLORREF value)
{
	color1 = value;
}
COLORREF CalendarItem::GetColor2()
{
	return color2;
}
void CalendarItem::SetColor2(COLORREF value)
{
	color2 = value;
}
COLORREF CalendarItem::GetColor3()
{
	return color3;
}
void CalendarItem::SetColor3(COLORREF value)
{
	color3 = value;
}

bool CalendarItem::GetToday()
{
	return isToday;
}
void CalendarItem::SetToday(bool value)
{
	isToday = value;
}

void CalendarItem::OnPaint(PaintEventArgs* e)
{
	bool newHdc = false;
	if(e->data.hdc == NULL)
	{
		::BeginPaint(m_hWnd, &(e->data));
		newHdc = true;
	}

	HDC hDC = e->data.hdc;
	if(e->data.rcPaint.bottom != 0 && e->data.rcPaint.right != 0)
	{
		HBRUSH hbr = NULL;
		RECT rcItem;
		RECT rcItem2;
		if(newHdc)
		{
			GetClientRect(m_hWnd, &rcItem);
		}
		else
		{
			CopyRect(&rcItem, &e->data.rcPaint);
		}

		int nW = (rcItem.right-2)/3;
		//::SetBkColor(hDC, this->GetBackground());
		//::ExtTextOut(hDC, 0, 0, ETO_OPAQUE, &rcItem, NULL, 0, NULL);
		::SendMessage(this->m_hWnd, WM_ERASEBKGND, (WPARAM)hDC, (LPARAM)this);
		::SetBkMode(hDC, TRANSPARENT);

		if(colorDot != (COLORREF)-1)
		{
			hbr = ::CreateSolidBrush(colorDot);
			SetRect(&rcItem2, rcItem.right/2-4, rcItem.bottom/2-5, rcItem.right/2+3, rcItem.bottom/2+1);
			::FillRect(hDC, &rcItem2, hbr);
			::DeleteObject(hbr);
		}

		::SetTextColor(hDC, this->GetTextColor());
		DWORD dwTextStyle = DT_CENTER|DT_VCENTER|DT_SINGLELINE;
		SetRect(&rcItem2, 1, 1, rcItem.right - 2, rcItem.bottom - 3);
		HFONT hOldFont = (HFONT)SelectObject(hDC, CMainForm::dayFont);
		::DrawText(hDC, GetText(), -1, &rcItem2, dwTextStyle);
		SelectObject(hDC, hOldFont);

		bool isHolid = false;
		if(color1 != (COLORREF)-1)
		{
			hbr = ::CreateSolidBrush(color1);
			SetRect(&rcItem2, 1, rcItem.bottom - 5, 1+nW, rcItem.bottom - 2);
			::FillRect(hDC, &rcItem2, hbr);
			::DeleteObject(hbr);
			isHolid = true;
		}

		if(color2 != (COLORREF)-1)
		{
			hbr = ::CreateSolidBrush(color2);
			SetRect(&rcItem2, 1+nW, rcItem.bottom - 5, 1+nW*2, rcItem.bottom - 2);
			::FillRect(hDC, &rcItem2, hbr);
			::DeleteObject(hbr);
			isHolid = true;
		}

		if(color3 != (COLORREF)-1)
		{
			hbr = ::CreateSolidBrush(color3);
			SetRect(&rcItem2, 1+nW*2, rcItem.bottom - 5, 1+nW*3, rcItem.bottom - 2);
			::FillRect(hDC, &rcItem2, hbr);
			::DeleteObject(hbr);
			isHolid = true;
		}

		if(isToday)
		{
			hbr = ::CreateSolidBrush(RGB(255, 0, 0));
			SetRect(&rcItem2, 0, 0, rcItem.right - 1, isHolid ? rcItem.bottom - 5 : rcItem.bottom);
			::FrameRect(hDC, &rcItem2, hbr);
			::DeleteObject(hbr);
		}
	}

	if(newHdc)
	{
		::EndPaint(m_hWnd, &(e->data));
		//memset(&(e->data), 0, sizeof(PAINTSTRUCT));
	}
}

#define CAL_DAY_WIDTHSPACE 0
#define CAL_DAY_HEIGHTSPACE 0
#define CAL_DAY_FONTSIZE 10
HFONT CMainForm::dayFont = NULL;
void CMainForm::HideMonth(int indMonth)
{
	if(labYM[indMonth] != NULL)
	{
		labYM[indMonth]->SetVisible(false);
	}
	for(int i = 0; i < 6*7; i++)
	{
		if(i < 7)
		{
			if(labTitle[indMonth][i] != NULL)
			{
				labTitle[indMonth][i]->SetVisible(false);
			}
		}
		if(labDay[indMonth][i] != NULL)
		{
			labDay[indMonth][i]->SetVisible(false);
		}
	}
}
void CMainForm::PrintMonth(bool showMonFirst, int indMonth, int nYYYY, int nMM, int nBaseLeft, int& nTop)
{
	nTop = 3;
	int nLeft = 3;

	if(labYM[indMonth] == NULL)
	{
		labYM[indMonth] = new CalendarItem();
		labYM[indMonth]->SetName(_T("ym"));
		labYM[indMonth]->SetMouseDownMoveForm(formCopy);
		this->AddControl(labYM[indMonth]);
	}
	labYM[indMonth]->SetText(cc::Str().Format(_T("%4d/%02d"), nYYYY, nMM));
	labYM[indMonth]->SetSize(dayWidth*4, dayHeight);
	//labYM[indMonth]->BestSize(true);
	labYM[indMonth]->SetVisible(true);

	nTop += dayHeight + dayHeightSpace;
	TCHAR* weekT[] = {_T("Su"), _T("Mo"), _T("Tu"), _T("We"), _T("Th"), _T("Fr"), _T("Sa")};
	int ind = 0;
	for(int i = 0; i < 7; i++)
	{
		ind = i;
		if(showMonFirst)
		{
			if(i == 6)
			{
				ind = 0;
			}
			else
			{
				ind = i + 1;
			}
		}
		if(labTitle[indMonth][ind] == NULL)
		{
			labTitle[indMonth][ind] = new CalendarItem();
			labTitle[indMonth][ind]->SetName(_T("T"));
			labTitle[indMonth][ind]->SetMouseDownMoveForm(formCopy);
			this->AddControl(labTitle[indMonth][ind]);
		}
		labTitle[indMonth][ind]->SetText(weekT[ind]);
		labTitle[indMonth][ind]->SetLocation(nBaseLeft + nLeft, nTop);
		labTitle[indMonth][ind]->SetSize(dayWidth, dayHeight);
		//labTitle[indMonth][ind]->BestSize(true);
		labTitle[indMonth][ind]->SetVisible(true);

		if(ind == 0)
		{
			labTitle[indMonth][ind]->SetTextColor(corWeek7);
		}
		else if(ind == 6)
		{
			labTitle[indMonth][ind]->SetTextColor(corWeek6);
		}
		else
		{
			labTitle[indMonth][ind]->SetTextColor(RGB(0, 0, 0));
		}
		nLeft += dayWidth + dayWidthSpace;
	}

	RECT rcYM = labYM[indMonth]->GetBounds();
	labYM[indMonth]->SetLocation(nBaseLeft + (nLeft - (rcYM.right - rcYM.left))/2, 3);

	Time today = Time::GetCurrentTime();
	Time* time = new Time(nYYYY, nMM, 1, 0, 0, 0);
	if(showMonFirst)
	{
		int nWeek = time->GetDayOfWeek();
		if(nWeek == 1)
		{
			time->AddDay(-6);
		}
		else
		{
			time->AddDay(time->GetDayOfWeek() * -1 + 2);
		}
	}
	else
	{
		time->AddDay(time->GetDayOfWeek() * -1 + 1);
	}

	nLeft = 3;
	nTop += dayHeight + dayHeightSpace;
	for(int i = 0; i < 6*7; i++)
	{
		int loopDay = time->GetDay();
		int loopMonth = time->GetMonth();
		int loopYear = time->GetYear();
		if(labDay[indMonth][i] == NULL)
		{
			labDay[indMonth][i] = new CalendarItem();
			//labDay[indMonth][i]->SetOuterStyle(OuterStyle::AlwaysNoEdge, true);
			labDay[indMonth][i]->SetMouseDownMoveForm(formCopy);
			labDay[indMonth][i]->DoubleClick += EventHandler((Object*)this, (EventFun)&CMainForm::Day_Click);
			this->AddControl(labDay[indMonth][i]);
		}
		labDay[indMonth][i]->SetName(cc::Str().Format(_T("%04d%02d%02d"), loopYear, loopMonth, loopDay));
		if(loopDay < 10)
		{
			labDay[indMonth][i]->SetText(cc::Str().Format(_T(" %d"), loopDay));
		}
		else
		{
			labDay[indMonth][i]->SetText(cc::Str().Format(_T("%d"), loopDay));
		}
		labDay[indMonth][i]->SetLocation(nBaseLeft + nLeft, nTop);
		labDay[indMonth][i]->SetSize(dayWidth, dayHeight);
		//labDay[indMonth][i]->BestSize(true);
		labDay[indMonth][i]->SetVisible(true);
		labDay[indMonth][i]->SetTips(NULL);

		labDay[indMonth][i]->SetColorDot((COLORREF)-1);
		labDay[indMonth][i]->SetColor1((COLORREF)-1);
		labDay[indMonth][i]->SetColor2((COLORREF)-1);
		labDay[indMonth][i]->SetColor3((COLORREF)-1);

		if(loopMonth == nMM)
		{
			cc::Str key;
			cc::Str val;
			int index;
			key.Format(_T("main.%s"), labDay[indMonth][i]->GetName());
			cc::Str sMemo;
			if(calendarMemo.GetSize() > 0)
			{
				if((index = calendarMemo.GetIndex(labDay[indMonth][i]->GetName())) >= 0)
				{
					sMemo = calendarMemo.GetAt(index);
					//first ,,, is not memo
					cc::Str sColor = sMemo.SplitBy(_T(","));
					sMemo.SplitBy(_T(","));
					sMemo.SplitBy(_T(","));
					if(sMemo.GetLength() > 200)
					{
						sMemo = sMemo.Substring(0, 200).Append(_T("..."));
					}
					labDay[indMonth][i]->SetColorDot((COLORREF)sColor.ToInt());
				}
			}

			if(nameCalendar1.GetLength() > 0)
			{
				if((index = calendarArr1.GetIndex(key)) >= 0)
				{
					val.Append(nameCalendar1).Append(_T(": "));
					val.Append(calendarArr1.GetAt(index));
					labDay[indMonth][i]->SetColor1(corCalendar1);
				}
			}
			if(nameCalendar2.GetLength() > 0)
			{
				if((index = calendarArr2.GetIndex(key)) >= 0)
				{
					if(val.GetLength() > 0)
					{
						val.Append(_T("\r\n"));
					}
					val.Append(nameCalendar2).Append(_T(": "));
					val.Append(calendarArr2.GetAt(index));
					labDay[indMonth][i]->SetColor2(corCalendar2);
				}
			}
			if(nameCalendar3.GetLength() > 0)
			{
				if((index = calendarArr3.GetIndex(key)) >= 0)
				{
					if(val.GetLength() > 0)
					{
						val.Append(_T("\r\n"));
					}
					val.Append(nameCalendar3).Append(_T(": "));
					val.Append(calendarArr3.GetAt(index));
					labDay[indMonth][i]->SetColor3(corCalendar3);
				}
			}
			if(sMemo.GetLength() > 0)
			{
				if(val.GetLength() > 0)
				{
					val.Append(_T("\r\n"));
				}
				val.Append(sMemo);
			}
			if(val.GetLength() > 0)
			{
				labDay[indMonth][i]->SetTips(val);
				//labDay[indMonth][i]->SetPushed(true);
			}

			if(nYYYY == today.GetYear() && nMM == today.GetMonth() && time->GetDay() == today.GetDay())
			{
				//labDay[indMonth][i]->SetBackground(RGB(100, 0, 0));
				labDay[indMonth][i]->SetToday(true);
				todayItem = labDay[indMonth][i];
			}
		}

		if(loopMonth != nMM)
		{
			//labDay[indMonth][i]->SetEnabled(false);
			labDay[indMonth][i]->SetTextColor(RGB(0xcc, 0xcc, 0xcc));
			//labDay[indMonth][i]->SetHotTextColor(RGB(0xcc, 0xcc, 0xcc));
		}
		else if((!showMonFirst && i%7 == 0) || (showMonFirst && i%7 == 6))
		{
			labDay[indMonth][i]->SetTextColor(corWeek7);
			//labDay[indMonth][i]->SetHotTextColor(corWeek7);
		}
		else if((!showMonFirst && i%7 == 6) || (showMonFirst && i%7 == 5))
		{
			labDay[indMonth][i]->SetTextColor(corWeek6);
			//labDay[indMonth][i]->SetHotTextColor(corWeek6);
		}
		else
		{
			labDay[indMonth][i]->SetTextColor(RGB(0, 0, 0));
			//labDay[indMonth][i]->SetHotTextColor(RGB(0, 0, 0));
		}
		time->AddDay(1);

		nLeft += dayWidth + dayWidthSpace;
		if((i+1)%7 == 0)
		{
			nTop += dayHeight + dayHeightSpace;
			nLeft = 5;
		}
	}

	delete time;
}

void CMainForm::PrintMonth_All(bool sizeWindow)
{
	if(dayBigMultipleSave != dayBigMultiple)
	{
		dayBigMultipleSave = dayBigMultiple;
		dayWidthSpace = CC_APP_GetIni()->Get(_T("Main.DayWidth"), CAL_DAY_WIDTHSPACE);
		dayHeightSpace = CC_APP_GetIni()->Get(_T("Main.DayHeight"), CAL_DAY_HEIGHTSPACE);
		dayFontSize = CC_APP_GetIni()->Get(_T("Main.DayFontSize"), CAL_DAY_FONTSIZE);
		dayWidthSpace += (int)(0.4*dayBigMultiple);
		dayHeightSpace += (int)(0.2*dayBigMultiple);
		dayFontSize += 2*dayBigMultiple;
		if(CMainForm::dayFont != NULL)
		{
			DeleteObject(CMainForm::dayFont);
			CMainForm::dayFont = NULL;
		}
	}

	if(CMainForm::dayFont == NULL)
	{
		HFONT hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
		LOGFONT lf;
		GetObject(hFont, sizeof(LOGFONT), &lf);

		// Now change the logfont.lfHeight member
		lf.lfHeight = dayFontSize;

		CMainForm::dayFont = CreateFontIndirect(&lf);

		RECT rect;
		MeasureDaySize(NULL, &rect);
		dayWidth = rect.right + 2;
		dayHeight = rect.bottom + 2;
	}
	bool showBefore = menuShowBefore->GetChecked();
	bool showAfter = menuShowAfter->GetChecked();
	bool showTime = menuShowTime->GetChecked();
	//bool showCaption = !menuNoCaption->GetChecked();
	bool showMonFirst = menuMonFirst->GetChecked();

	if(currentY < 1 || currentM < 1)
	{
		Time today = Time::GetCurrentTime();
		currentY = today.GetYear();
		currentM = today.GetMonth();
	}

	if(todayItem != NULL)
	{
		todayItem->SetToday(false);
		todayItem = NULL;
	}
	Time* time = new Time(currentY, currentM, 1, 0, 0, 0);
	int nTop = 3;
	int nBaseLeft = 3;
	if(showBefore)
	{
		time->AddMonth(-1);
		PrintMonth(showMonFirst, 0, time->GetYear(), time->GetMonth(), nBaseLeft, nTop);
		nBaseLeft += (dayWidth + dayWidthSpace)*7 + 5;
		time->AddMonth(1);
	}
	else
	{
		HideMonth(0);
	}

	PrintMonth(showMonFirst, 1, time->GetYear(), time->GetMonth(), nBaseLeft, nTop);
	int nBaseLeftOld = nBaseLeft;
	nBaseLeft += (dayWidth + dayWidthSpace)*7 + 5;

	if(btnYPrev == NULL)
	{
		btnYPrev = new ToolItem();
		btnYPrev->SetName(_T("btnYPrev"));
		btnYPrev->Click += EventHandler((Object*)this, (EventFun)&CMainForm::Control_Click);
		this->AddControl(btnYPrev);
	}
	btnYPrev->SetText(_T("|<"));
	//btnYPrev->SetIcon(imageList, IDI_ICON_LEFTEND);
	btnYPrev->SetTips(_T("Previous Year"));
	btnYPrev->SetLocation(nBaseLeftOld, 3);
	btnYPrev->SetSize(14, dayHeight-1);
	btnYPrev->SetOuterStyle(OuterStyle::AutoSize, false);

	if(btnMPrev == NULL)
	{
		btnMPrev = new ToolItem();
		btnMPrev->SetName(_T("btnMPrev"));
		btnMPrev->Click += EventHandler((Object*)this, (EventFun)&CMainForm::Control_Click);
		this->AddControl(btnMPrev);
	}
	btnMPrev->SetText(_T("<"));
	//btnMPrev->SetIcon(imageList, IDI_ICON_LEFT);
	btnMPrev->SetTips(_T("Previous Month"));
	btnMPrev->SetLocation(nBaseLeftOld + 15, 3);
	btnMPrev->SetSize(14, dayHeight-1);
	btnMPrev->SetOuterStyle(OuterStyle::AutoSize, false);

	if(btnMNext == NULL)
	{
		btnMNext = new ToolItem();
		btnMNext->SetName(_T("btnMNext"));
		btnMNext->Click += EventHandler((Object*)this, (EventFun)&CMainForm::Control_Click);
		this->AddControl(btnMNext);
	}
	btnMNext->SetText(_T(">"));
	//btnMNext->SetIcon(imageList, IDI_ICON_RIGHT);
	btnMNext->SetTips(_T("Next Month"));
	btnMNext->SetLocation(nBaseLeft - 30, 3);
	btnMNext->SetSize(14, dayHeight-1);
	btnMNext->SetOuterStyle(OuterStyle::AutoSize, false);

	if(btnYNext == NULL)
	{
		btnYNext = new ToolItem();
		btnYNext->SetName(_T("btnYNext"));
		btnYNext->Click += EventHandler((Object*)this, (EventFun)&CMainForm::Control_Click);
		this->AddControl(btnYNext);
	}
	btnYNext->SetText(_T(">|"));
	//btnYNext->SetIcon(imageList, IDI_ICON_RIGHTEND);
	btnYNext->SetTips(_T("Next Year"));
	btnYNext->SetLocation(nBaseLeft - 15, 3);
	btnYNext->SetSize(14, dayHeight-1);
	btnYNext->SetOuterStyle(OuterStyle::AutoSize, false);

	if(showAfter)
	{
		time->AddMonth(1);
		PrintMonth(showMonFirst, 2, time->GetYear(), time->GetMonth(), nBaseLeft, nTop);
		nBaseLeft += (dayWidth + dayWidthSpace)*7 + 5;
		time->AddMonth(-1);
	}
	else
	{
		HideMonth(2);
	}

	if(sizeWindow)
	{
		//nTop = 152;
		this->SetStyle(StyleType::Style, WS_CAPTION, 0);
		formCopy->SetStyle(StyleType::Style, WS_CAPTION, 0);
		SetWindowPos(m_hWnd, NULL, 0,0,0,0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER);
		SetWindowPos(formCopy->m_hWnd, NULL, 0,0,0,0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER);

		led->SetVisible(showTime);
		if(showTime)
		{
			int nMulti = 40 * (1 + dayBigMultiple/3);
			led->SetLocation(3, nTop);
			led->SetSize(nBaseLeft, nMulti);
			nTop += nMulti + 5;
		}

		nTop += 20;
		this->SetMaxSize(-1, -1);
		this->SetMinSize(-1, -1);
		this->SetSize(nBaseLeft + 18, nTop);
		this->SetMaxSize(nBaseLeft + 18, nTop);
		this->SetMinSize(nBaseLeft + 18, nTop);
		formCopy->SetMaxSize(-1, -1);
		formCopy->SetMinSize(-1, -1);
		formCopy->SetSize(nBaseLeft + 18, nTop);
		formCopy->SetMaxSize(nBaseLeft + 18, nTop);
		formCopy->SetMinSize(nBaseLeft + 18, nTop);
	}
	delete time;
	tShown = cc::Time::GetCurrentTime();
	::KillTimer(this->m_hWnd, UU_TIMERID_MONITOR);
	::SetTimer(this->m_hWnd, UU_TIMERID_MONITOR, 100, NULL);
}

void CMainForm::Day_Click(Object* sender, EventArgs* e)
{
	//cc::Util::MsgBox(NULL, sName);
	if(editDialog == NULL)
	{
		RECT rect;
		int nBottom = (int)::GetSystemMetrics(SM_CYFULLSCREEN);
		if(formCopy->GetVisible())
		{
			rect = formCopy->GetBounds();
		}
		else
		{
			rect = this->GetBounds();
		}

		editDialog = new EditDialog(&calendarMemo, m_hWnd);
		RECT rect2 = editDialog->GetBounds();
		if(rect.bottom + (rect2.bottom - rect2.top + 5) < nBottom)
		{
			editDialog->SetLocation(rect.left, rect.bottom + 2);
		}
		else
		{
			editDialog->SetLocation(rect.left, rect.top - (rect2.bottom - rect2.top + 5));
		}
	}
	editDialog->SetVisible(true);
	editDialog->Show();
	if(sender != NULL)
	{
		cc::Str sName = ((Control*)sender)->GetName();
		editDialog->EditDate(sName);
	}
}

void CMainForm::Load_Calendar()
{
	calendarArr1.Clear();
	calendarArr2.Clear();
	calendarArr3.Clear();
	cc::Str sFile = cc::Str(LangCode::SYSTEM);
	cc::Str sPath = cc::Util::getStartupPath(true);
	if(nameCalendar1.GetLength() > 0)
	{
		sFile.Format(_T("%sCalendar_%s.ini"), (TCHAR*)sPath.GetBuf(), (TCHAR*)nameCalendar1.GetBuf());
		if(cc::Util::fExist(sFile))
		{
			calendarArr1.Add(sFile, LangCode::UNICODE16, true);
		}
	}
	if(nameCalendar2.GetLength() > 0)
	{
		sFile.Format(_T("%sCalendar_%s.ini"), (TCHAR*)sPath.GetBuf(), (TCHAR*)nameCalendar2.GetBuf());
		if(cc::Util::fExist(sFile))
		{
			calendarArr2.Add(sFile, LangCode::UNICODE16, true);
		}
	}
	if(nameCalendar3.GetLength() > 0)
	{
		sFile.Format(_T("%sCalendar_%s.ini"), (TCHAR*)sPath.GetBuf(), (TCHAR*)nameCalendar3.GetBuf());
		if(cc::Util::fExist(sFile))
		{
			calendarArr3.Add(sFile, LangCode::UNICODE16, true);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
//CMainForm
CMainForm::CMainForm(HWND mainFormHwnd)
{
	this->mainFormHwnd = mainFormHwnd;
#ifndef _CC_ASSISTANT
	imageList = new ImageList();
	imageList->SetIconSize(-1, -1);
	imageList->SetIcon(IDI_ICON_MAIN, IDI_ICON_MAIN);
	imageList->SetIcon(IDI_ICON_LEFT, IDI_ICON_LEFT);
	imageList->SetIcon(IDI_ICON_LEFTEND, IDI_ICON_LEFTEND);
	imageList->SetIcon(IDI_ICON_RIGHT, IDI_ICON_RIGHT);
	imageList->SetIcon(IDI_ICON_RIGHTEND, IDI_ICON_RIGHTEND);
	this->DisposeItemAdd(imageList);
#endif

	calendarArr1 = cc::KeyArr();
	calendarArr2 = cc::KeyArr();
	calendarArr3 = cc::KeyArr();
	currentY = 0;
	currentM = 0;
	nStep = 0;
	nMode = -1;
	//reset by MeasureDaySize
	dayWidth = 17;
	dayHeight = 16;
	dayWidthSpace = CAL_DAY_WIDTHSPACE;
	dayHeightSpace = CAL_DAY_HEIGHTSPACE;
	dayFontSize = CAL_DAY_FONTSIZE;
	dayBigMultiple = 0;
	dayBigMultipleSave = 0;
	editDialog = NULL;

	todayItem = NULL;
	for(int j = 0; j < 3; j++)
	{
		labYM[j] = NULL;
		for(int i = 0; i < 6*7; i++)
		{
			if(i < 7)
			{
				labTitle[j][i] = NULL;
			}
			labDay[j][i] = NULL;
		}
	}

	btnYPrev = NULL;
	btnMPrev = NULL;
	btnMNext = NULL;
	btnYNext = NULL;

	formCopy = new Form();
	formCopy->SetSize(0, 0);
	formCopy->SetName(_T("calendar"));
	formCopy->SetText(_T(""));
	formCopy->SetMoveFormOnClick(true);
	//formCopy->SetStyle(StyleType::Style, WS_THICKFRAME, 0);
	formCopy->SetStyle(StyleType::ExStyle, WS_EX_APPWINDOW, WS_EX_TOOLWINDOW|WS_EX_WINDOWEDGE);
	this->DisposeItemAdd(formCopy);
	formCopy->CreateControl();
	formCopy->GetClient()->SetVisible(false);
	formCopy->SetVisible(false);
	formCopy->Deactivate += EventHandler((Object*)this, (EventFun)&CMainForm::formCopy_Deactivate);

	contMenu = new ContextMenu();
	contMenu->SetName(_T("contCalendar"));
	//formCopy->SetContextMenu(contMenu);
	formCopy->DisposeItemAdd(contMenu);
	this->GetClient()->SetContextMenu(contMenu);

	menuToday = new MenuItem();
	menuToday->SetName(_T("menuToday"));
	menuToday->SetText(_T("Goto Today"));
	menuToday->Click += EventHandler((Object*)this, (EventFun)&CMainForm::Control_Click);
	contMenu->ItemAdd(menuToday);

	menuOption = new MenuItem();
	menuOption->SetName(_T("menuOption"));
	menuOption->SetText(_T("Calendar Option"));
	menuOption->Click += EventHandler((Object*)this, (EventFun)&CMainForm::Control_Click);
	contMenu->ItemAdd(menuOption);

	menuMemoList = new MenuItem();
	menuMemoList->SetName(_T("menuMemoList"));
	menuMemoList->SetText(_T("Calendar memo list"));
	menuMemoList->Click += EventHandler((Object*)this, (EventFun)&CMainForm::Control_Click);
	contMenu->ItemAdd(menuMemoList);
	
	menuTopmost = new MenuItem();
	menuTopmost->SetName(_T("menuTopmost"));
	menuTopmost->SetText(_T("Always Top"));
	menuTopmost->Click += EventHandler((Object*)this, (EventFun)&CMainForm::Control_Click);
	contMenu->ItemAdd(menuTopmost);

	menuShowBefore = new MenuItem();
	menuShowBefore->SetName(_T("menuShowBefore"));
	menuShowBefore->SetText(_T("Show Previous Month"));
	menuShowBefore->Click += EventHandler((Object*)this, (EventFun)&CMainForm::Control_Click);
	contMenu->ItemAdd(menuShowBefore);

	menuShowAfter = new MenuItem();
	menuShowAfter->SetName(_T("menuShowAfter"));
	menuShowAfter->SetText(_T("Show Next Month"));
	menuShowAfter->Click += EventHandler((Object*)this, (EventFun)&CMainForm::Control_Click);
	contMenu->ItemAdd(menuShowAfter);

	menuShowSmall = new MenuItem();
	menuShowSmall->SetName(_T("menuShowSmall"));
	menuShowSmall->SetText(_T("Show More Smaller Size"));
	menuShowSmall->Click += EventHandler((Object*)this, (EventFun)&CMainForm::Control_Click);
	contMenu->ItemAdd(menuShowSmall);

	menuShowBig = new MenuItem();
	menuShowBig->SetName(_T("menuShowBig"));
	menuShowBig->SetText(_T("Show More Bigger Size"));
	menuShowBig->Click += EventHandler((Object*)this, (EventFun)&CMainForm::Control_Click);
	contMenu->ItemAdd(menuShowBig);

	menuMonFirst = new MenuItem();
	menuMonFirst->SetName(_T("menuMonFirst"));
	menuMonFirst->SetText(_T("Week Start From Monday"));
	menuMonFirst->Click += EventHandler((Object*)this, (EventFun)&CMainForm::Control_Click);
	contMenu->ItemAdd(menuMonFirst);

	menuShowTime = new MenuItem();
	menuShowTime->SetName(_T("menuShowTime"));
	menuShowTime->SetText(_T("Show Time"));
	menuShowTime->Click += EventHandler((Object*)this, (EventFun)&CMainForm::Control_Click);
	contMenu->ItemAdd(menuShowTime);

	menuHhmm = new MenuItem();
	menuHhmm->SetName(_T("menuHhmm"));
	menuHhmm->SetText(_T("Show Time HH:MM Format"));
	menuHhmm->Click += EventHandler((Object*)this, (EventFun)&CMainForm::Control_Click);
	menuHhmm->SetEnabled(false);
	contMenu->ItemAdd(menuHhmm);
	
	//menuNoCaption = new MenuItem();
	//menuNoCaption->SetText(_T("No Window Title"));
	//menuNoCaption->Click += EventHandler((Object*)this, (EventFun)&CMainForm::Control_Click);
	//contMenu->ItemAdd(menuNoCaption);

	menuExit = NULL;
	menuAbout = NULL;
#ifndef _CC_ASSISTANT
	menuAbout = new MenuItem();
	menuAbout->SetName(_T("menuAbout"));
	menuAbout->SetText(_T("About"));
	menuAbout->Click += EventHandler((Object*)this, (EventFun)&CMainForm::Control_Click);
	contMenu->ItemAdd(menuAbout);
#endif

	menuExit = new MenuItem();
	menuExit->SetName(_T("menuExit"));
	menuExit->SetText(_T("Close"));
	menuExit->Click += EventHandler((Object*)this, (EventFun)&CMainForm::Control_Click);
	contMenu->ItemAdd(menuExit);

	int nTop = 137;
	led = new LEDControl();
	led->SetName(_T("led"));
	led->SetText(_T(""));
	led->SetTextColor(RGB(0, 0, 255));
	led->SetTextBackground(RGB(0xe8, 0xe8, 0xe8));
	led->SetLocation(3, nTop);
	led->SetSize(50, 12);
	led->SetOuterStyle(OuterStyle::TransparentParentErase, true);
	led->SetMouseDownMoveForm(formCopy);
	led->SetVisible(false);
	this->AddControl(led);

	notify1 = NULL;

	this->SetOuterStyle(OuterStyle::AutoConfig, true);
	this->SetName(_T("Calendar"));
	this->SetText(App::GetNameVersion());
	this->SetConfigName(_T(""));
	//this->SetCenter();
	//this->SetClientSize(600, 600);
	//here need be 32X32
#ifndef _CC_ASSISTANT
	this->SetIcon(imageList, IDI_ICON_MAIN);
#endif
	//this->SetMinSize(110, 100);
	//this->SetMaxSize(900, 600);
	//if(CC_APP_GetIni()->GetBool(_T("Main.NoCaption"), true))
	//{
	//	this->SetSize(nBaseLeft + 15, 152);
	//	this->SetStyle(StyleType::Style, WS_CAPTION, 0);
	//}
	//else
	//{
	//	this->SetSize(nBaseLeft + 15, 152 + 30);
	//}
	//LONG nWTop = (LONG)CC_APP_GetIni()->Get(_T("Main.CTop"), 100);
	//LONG nWLeft = (LONG)CC_APP_GetIni()->Get(_T("Main.CLeft"), 100);
	//int nWW = 30;
	//int nWH = 30;
	//cc::Util::AdjustWindowRect(nWLeft, nWTop, nWW, nWH);
	//ShellWindow_Rang(nWLeft, nWTop);
	this->SetLocation(0, 0);
	this->SetSize(30, 30);
	//this->SetMoveFormOnClick(true);
	//this->SetStyle(StyleType::Style, WS_THICKFRAME, 0);
	this->SetStyle(StyleType::ExStyle, WS_EX_APPWINDOW, WS_EX_TOOLWINDOW);
	//if(!CC_APP_GetIni()->GetBool(_T("Main.Topmost"), true))
	//{
	//	this->SetStyle(StyleType::ExStyle, 0, WS_EX_APPWINDOW);
	//}
	this->Load += EventHandler((Object*)this, (EventFun)&CMainForm::Form_Load);
	this->Closed += EventHandler((Object*)this, (EventFun)&CMainForm::Form_Closed);
	this->Shutdown += EventHandler((Object*)this, (EventFun)&CMainForm::Form_Shutdown);

	SetLang(*CC_APP_GetLang(), cc::Str(CC_CONFIG_LANGSECTION));

	//show it until Form_Load
	this->SetVisible(false);

	this->SetConfigName(_T("Calendar"));
	this->GetClient()->SetMouseDownMoveForm(formCopy);
	formCopy->SetLocation(0, 0);
}

CMainForm::~CMainForm()
{
	if(CMainForm::dayFont != NULL)
	{
		DeleteObject(CMainForm::dayFont);
		CMainForm::dayFont = NULL;
	}
}

void ShellWindow_Rang(LONG& left, LONG& top, LONG w, LONG h, bool turnToChild)
{
	POINT pt;
	pt.x = left;
	pt.y = top;
	HMONITOR hm = ::MonitorFromPoint(pt, MONITOR_DEFAULTTONEAREST);
	if(hm != NULL)
	{
		MONITORINFO mix;
		mix.cbSize = sizeof(mix);
		if(GetMonitorInfo(hm, &mix))
		{
			if(left < mix.rcWork.left)
			{
				left = mix.rcWork.left;
			}
			else if(left > mix.rcWork.right - w)
			{
				left = mix.rcWork.right - w;
			}

			if(top < mix.rcWork.top)
			{
				top = mix.rcWork.top;
			}
			else if(top > mix.rcWork.bottom - h)
			{
				top = mix.rcWork.bottom - h;
			}
		}
	}

	HWND hwnd = ::GetShellWindow();
	RECT rectAll = {0};
	GetWindowRect(hwnd, &rectAll);
	if(top < rectAll.top)
	{
		top = rectAll.top;
	}
	else if(top > rectAll.bottom - h)
	{
		top = rectAll.bottom - h;
	}
	if(turnToChild && rectAll.top < 0)
	{
		top = top - rectAll.top;
	}

	if(left < rectAll.left)
	{
		left = rectAll.left;
	}
	else if(left > rectAll.right - w)
	{
		left = rectAll.right - w;
	}
	if(turnToChild && rectAll.left < 0)
	{
		left = left - rectAll.left;
	}
}

void CMainForm::DoSetLang(KeyArr& keyArr, cc::Str sThisHead)
{
	Form::DoSetLang(keyArr, sThisHead);

	contMenu->SetLang(*CC_APP_GetLang(), CC_CONFIG_LANGSECTION);
}

void CMainForm::Form_Load(Object* sender, EventArgs* e)
{
	//load lang
	SetLang(*CC_APP_GetLang(), CC_CONFIG_LANGSECTION);
#ifndef _CC_ASSISTANT
	notify1 = new NotifyIcon();
	notify1->SetText(App::GetNameVersion());
	notify1->SetIcon(imageList, IDI_ICON_MAIN);
	notify1->SetContextMenu(contMenu);
	notify1->SetShowMenu(LeftRight::Left, false);
	notify1->Click += EventHandler((Object*)this, (EventFun)&CMainForm::notify1_Click);
	this->DisposeItemAdd(notify1);
	notify1->SetVisible(true);
#endif

	nameCalendar1 = CC_APP_GetIni()->Get(_T("Main.Calendar1"), _T("japanese__ja"));
	nameCalendar2 = CC_APP_GetIni()->Get(_T("Main.Calendar2"), _T("china__zh"));
	nameCalendar3 = CC_APP_GetIni()->Get(_T("Main.Calendar3"), _T(""));
	corCalendar1 = CC_APP_GetIni()->Get(_T("Main.CalendarColor1"), RGB(255, 0, 0));
	corCalendar2 = CC_APP_GetIni()->Get(_T("Main.CalendarColor2"), RGB(0, 255, 0));
	corCalendar3 = CC_APP_GetIni()->Get(_T("Main.CalendarColor3"), RGB(0, 255, 255));
	corWeek7 = CC_APP_GetIni()->Get(_T("Main.CalendarColorW7"), RGB(255, 0, 0));
	corWeek6 = CC_APP_GetIni()->Get(_T("Main.CalendarColorW6"), RGB(0, 0, 255));
	dayWidthSpace = CC_APP_GetIni()->Get(_T("Main.DayWidth"), CAL_DAY_WIDTHSPACE);
	dayHeightSpace = CC_APP_GetIni()->Get(_T("Main.DayHeight"), CAL_DAY_HEIGHTSPACE);
	dayFontSize = CC_APP_GetIni()->Get(_T("Main.DayFontSize"), CAL_DAY_FONTSIZE);

	cc::Str sFile = cc::Util::getStartupPath().Append(_T(".CMemo.ini"));
	calendarMemo.Add(sFile, LangCode::UNICODE16, true);
	Load_Calendar();

	//load config
	//this->Config(true, *CC_APP_GetIni(), cc::Str(_T("Main")));
	//if(CC_APP_GetIni()->GetBool(_T("Main.NoCaption"), true))
	//{
	//	menuNoCaption->SetChecked(true);
	//}
	if(CC_APP_GetIni()->GetBool(_T("Main.ShowTime"), false))
	{
		menuShowTime->SetChecked(true);
	}
	if(CC_APP_GetIni()->GetBool(_T("Main.ShowBefore"), false))
	{
		menuShowBefore->SetChecked(true);
	}
	if(CC_APP_GetIni()->GetBool(_T("Main.ShowAfter"), true))
	{
		menuShowAfter->SetChecked(true);
	}
	//if(CC_APP_GetIni()->GetBool(_T("Main.ShowBig"), false))
	//{
	//	menuShowBig->SetChecked(true);
	//}
	dayBigMultiple = CC_APP_GetIni()->Get(_T("Main.BigMultiple"), 2);
	if(!CC_RANGE(dayBigMultiple, 0, 10))
	{
		dayBigMultiple = 0;
	}
	menuShowSmall->SetEnabled(dayBigMultiple > 0);
	menuShowBig->SetEnabled(dayBigMultiple < 10);
	if(CC_APP_GetIni()->GetBool(_T("Main.MonFirst"), false))
	{
		menuMonFirst->SetChecked(true);
	}

	PrintMonth_All(true);

	RECT rect = this->GetBounds();
	HWND hwnd = ::GetShellWindow();
	::SetParent(m_hWnd, hwnd);
	//::ShowWindow(m_hWnd, SW_SHOWNOACTIVATE);
	LONG nWTop = (LONG)CC_APP_GetIni()->Get(_T("Main.CTop"), 100);
	LONG nWLeft = (LONG)CC_APP_GetIni()->Get(_T("Main.CLeft"), 100);
	//int nWW = 30;
	//int nWH = 30;
	//cc::Util::AdjustWindowRect(nWLeft, nWTop, nWW, nWH);
	ShellWindow_Rang(nWLeft, nWTop, rect.right - rect.left, rect.bottom - rect.top, true);
	this->SetLocation(nWLeft, nWTop);
	formCopy->SetLocation(nWLeft, nWTop);

	if(CC_APP_GetIni()->GetBool(_T("Main.Topmost"), true))
	{
		Control_Click(menuTopmost, NULL);
	}
	if(CC_APP_GetIni()->GetBool(_T("Main.Hhmm"), false))
	{
		Control_Click(menuHhmm, NULL);
	}
	//show it until Form_Load
	this->SetVisible(true);
	led->SetTimerSec(-1);
	led->SetStart(true);

	nMode = 0;
	cc::Util::FourceWindowTop(formCopy->m_hWnd);
}

void CMainForm::Form_Closed(Object* sender, EventArgs* e)
{
	//save config
	RECT rect;
	if(formCopy->GetVisible())
	{
		rect = formCopy->GetBounds();
	}
	else
	{
		rect = this->GetBounds();
	}
	CC_APP_GetIni()->Set(_T("Main.CTop"), rect.top);
	CC_APP_GetIni()->Set(_T("Main.CLeft"), rect.left);
	CC_APP_GetIni()->SetBool(_T("Main.Topmost"), menuTopmost->GetChecked());
	//CC_APP_GetIni()->SetBool(_T("Main.NoCaption"), menuNoCaption->GetChecked());
	CC_APP_GetIni()->SetBool(_T("Main.ShowTime"), menuShowTime->GetChecked());
	CC_APP_GetIni()->SetBool(_T("Main.Hhmm"), menuHhmm->GetChecked());
	CC_APP_GetIni()->SetBool(_T("Main.ShowBefore"), menuShowBefore->GetChecked());
	CC_APP_GetIni()->SetBool(_T("Main.ShowAfter"), menuShowAfter->GetChecked());
	CC_APP_GetIni()->Set(_T("Main.BigMultiple"), dayBigMultiple);
	CC_APP_GetIni()->SetBool(_T("Main.MonFirst"), menuMonFirst->GetChecked());
	//this->Config(false, *CC_APP_GetIni(), cc::Str(_T("Main")));

	Form_Shutdown(sender, e);
}

void CMainForm::Form_Shutdown(Object* sender, EventArgs* e)
{
	if(editDialog != NULL)
	{
		editDialog->Dispose();
		delete editDialog;
		editDialog = NULL;

		cc::Str sFile = cc::Util::getStartupPath().Append(_T(".CMemo.ini"));
		calendarMemo.Save(sFile, LangCode::UNICODE16, true);
	}

#ifndef _CC_ASSISTANT
	//save config while window down
	CC_APP_GetConfig()->SaveIni();
#endif
}

void CMainForm::formCopy_Deactivate(Object* sender, EventArgs* e)
{
	bool chk = menuTopmost->GetChecked();
	if(!chk)
	{
		::KillTimer(this->m_hWnd, UU_TIMERID_INACTIVE2);
		::KillTimer(this->m_hWnd, UU_TIMERID_INACTIVE);

		nStep = 2000;
//cc::Log::debug("formCopy_Deactivate, nStep:%d, nMode:%d", nStep, nMode);
		int nWait = CC_APP_GetIni()->Get(_T("Main.CWait"), 3000);
		if(nWait < 1000 || nWait > 1000 * 60)
		{
			nWait = 3000;
		}
		::SetTimer(this->m_hWnd, UU_TIMERID_INACTIVE, nWait, NULL);
	}
}

void CMainForm::OnInnerTimer(EventArgs *e)
{
	if(e != NULL && e->WParam == UU_TIMERID_MONITOR)
	{
		::KillTimer(this->m_hWnd, UU_TIMERID_MONITOR);
		cc::Time time = cc::Time::GetCurrentTime();
		if(tShown.GetYear() != time.GetYear() || tShown.GetMonth() != time.GetMonth() || tShown.GetDay() != time.GetDay())
		{
			//tShown is updated at PrintMonth_All
			PrintMonth_All(false);
		}

		cc::Time time2 = cc::Time(tShown.GetYear(), tShown.GetMonth(), tShown.GetDay(), 0, 0, 0);
		time2.AddDay(1);
		LONGLONG t1 = tShown.GetTotalSeconds();
		LONGLONG t2 = time2.GetTotalSeconds();
		int nDiff = (int)(t2 - t1) - 1;
		if(nDiff < 1)
		{
			nDiff = 1;
		}
		if(nDiff > 600)
		{
			nDiff = 600;
		}

		//maybe started at PrintMonth_All
		::KillTimer(this->m_hWnd, UU_TIMERID_MONITOR);
		::SetTimer(this->m_hWnd, UU_TIMERID_MONITOR, nDiff * 1000, NULL);
		return;
	}
	if(e != NULL && e->WParam == UU_TIMERID_CHGDISP)
	{
		::KillTimer(this->m_hWnd, UU_TIMERID_CHGDISP);

		RECT rect = formCopy->GetBounds();
		if(!formCopy->GetVisible())
		{
			ShellWindow_Rang(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, true);
			::SetWindowPos(this->m_hWnd, NULL, rect.left, rect.top, 0, 0, SWP_NOSIZE|SWP_NOZORDER|SWP_NOACTIVATE|SWP_SHOWWINDOW);
		}
		else
		{
			::ShowWindow(m_hWnd, SW_HIDE);
			ShellWindow_Rang(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, false);
			::SetWindowPos(formCopy->m_hWnd, NULL, rect.left, rect.top, 0, 0, SWP_NOSIZE|SWP_NOZORDER|SWP_NOACTIVATE|SWP_SHOWWINDOW);
		}
		return;
	}
	if(e != NULL && e->WParam == UU_TIMERID_EDITDAY_FRESH)
	{
		::KillTimer(this->m_hWnd, UU_TIMERID_EDITDAY_FRESH);
		PrintMonth_All(false);
		return;
	}
	if(e != NULL && e->WParam == UU_TIMERID_EDITDAY_EXIT)
	{
		::KillTimer(this->m_hWnd, UU_TIMERID_EDITDAY_EXIT);
		cc::Util::FourceWindowTop(formCopy->m_hWnd);
		return;
	}

	//::ShowWindow(m_hWnd, SW_RESTORE);
	bool chk = menuTopmost->GetChecked();
	if(formCopy->Focused() || nMode == 2 || chk || (editDialog != NULL && editDialog->GetVisible()))
	{
//cc::Log::debug("OnInnerTimer 1:%d, nStep:%d, nMode:%d", e->WParam, nStep, nMode);
		::KillTimer(this->m_hWnd, UU_TIMERID_INACTIVE2);
		::KillTimer(this->m_hWnd, UU_TIMERID_INACTIVE);
		cc::Util::SetTransparent(formCopy->m_hWnd, 255);
		return;
	}
	if(e != NULL && e->WParam == UU_TIMERID_INACTIVE)
	{
//cc::Log::debug("OnInnerTimer 2:%d, nStep:%d, nMode:%d", e->WParam, nStep, nMode);
		::KillTimer(this->m_hWnd, UU_TIMERID_INACTIVE);
		nStep = 3000;
		::SetTimer(this->m_hWnd, UU_TIMERID_INACTIVE2, 300, NULL);
		//cc::Log::debug("OnInnerTimer, nStep:%d", nStep);
		return;
	}
	if(e != NULL && e->WParam == UU_TIMERID_INACTIVE2)
	{
		nStep++;
		if(nStep >= 3001 && nStep <= 3020)
		{
			cc::Util::SetTransparent(formCopy->m_hWnd, 255 - (nStep - 3000)*10);
		}
		if(nStep >= 3020)
		{
			nStep = 3000 + 998;
			::KillTimer(this->m_hWnd, UU_TIMERID_INACTIVE2);
			//cc::Log::debug("OnInnerTimer, nStep:%d, nMode:%d", nStep, nMode);

			if(nMode == 1)
			{
				RECT rect = formCopy->GetBounds();
				::SetParent(this->GetClient()->m_hWnd, this->m_hWnd);
				formCopy->SetVisible(false);
				//formCopy->SetBounds(-1, -1, 0, 0);
				//this->SetBounds(rect);
				nStep = 3000 + 999;
				//this->SetVisible(true);

				ShellWindow_Rang(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, true);
				::SetWindowPos(this->m_hWnd, NULL, rect.left, rect.top, 0, 0, SWP_NOSIZE|SWP_NOZORDER|SWP_NOACTIVATE|SWP_SHOWWINDOW);
				//this->SetLocation(rect.left, rect.top);
				//::ShowWindow(m_hWnd, SW_SHOWNOACTIVATE);
				::SetTimer(this->m_hWnd, UU_TIMERID_INACTIVE3, 100, NULL);
			}
			nStep = 3000 + 999;
		}
		return;
	}
	if(e != NULL && e->WParam == UU_TIMERID_INACTIVE3)
	{
		::KillTimer(this->m_hWnd, UU_TIMERID_INACTIVE3);
		nMode = 0;

		//refresh
		if(notify1 != NULL)
		{
			notify1->SetText(App::GetNameVersion());
			notify1->SetVisible(true);
		}
	}
	Form::OnInnerTimer(e);
}

/*void CMainForm::AdjustLocation()
{
	HWND hWnd = NULL;
	if(formCopy->GetVisible())
	{
		hWnd = formCopy->m_hWnd;
	}
	else
	{
		hWnd = this->m_hWnd;
	}
	if(hWnd != NULL)
	{
		RECT rect = {0, 0, 0, 0};
		HMONITOR hm = MonitorFromWindow(hWnd, MONITOR_DEFAULTTOPRIMARY);
		if(hm != NULL)
		{
			MONITORINFO mix;
			mix.cbSize = sizeof(mix);
			if(GetMonitorInfo(hm, &mix))
			{
				CopyRect(&rect, &mix.rcWork);
			}
			else
			{
				hm = NULL;
			}
		}
		if(hm == NULL)
		{
			if(!::SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, 0))
			{
				rect.right = (LONG)::GetSystemMetrics(SM_CXSCREEN);
				rect.bottom = (LONG)::GetSystemMetrics(SM_CYSCREEN);
			}
		}

		RECT rect2 = {0, 0, 0, 0};
		WINDOWPLACEMENT lpwndpl;
		::GetWindowPlacement(hWnd, &lpwndpl);
		::CopyRect(&rect2, &lpwndpl.rcNormalPosition);
		if(rect2.top < rect.top)
		{
			rect2.top = rect.top;
		}
		if(rect2.left < rect.left)
		{
			rect2.left = rect.left;
		}
		if(rect2.top > rect.bottom)
		{
			rect2.top = rect.bottom - 32;
		}
		if(rect2.left > rect.right)
		{
			rect2.left = rect.right - 32;
		}
		::SetWindowPos(hWnd, NULL, rect2.left, rect2.top, 0, 0, SWP_NOSIZE | SWP_NOACTIVATE);
	}
}*/

LRESULT CMainForm::WndProc(EventArgs *e)
{
	if(e->Msg == WM_POWERBROADCAST)
	{
		::KillTimer(this->m_hWnd, UU_TIMERID_MONITOR);
		::SetTimer(this->m_hWnd, UU_TIMERID_MONITOR, 5000, NULL);
	}
	if(e->Msg == WM_DISPLAYCHANGE)// || e->Msg == WM_ACTIVATEAPP || e->Msg == WM_ACTIVATE)
	{
		::KillTimer(this->m_hWnd, UU_TIMERID_CHGDISP);
		::SetTimer(this->m_hWnd, UU_TIMERID_CHGDISP, 1000, NULL);
	}
	if(e->Msg == WM_ACTIVATE)
	{
		//return Form::WndProc(e);
//cc::Log::debug("WA_ACTIVE, nStep:%d, nMode:%d, WParam:%d", nStep, nMode, (int)LOWORD(e->WParam));
		if(LOWORD(e->WParam) == WA_ACTIVE && nMode == 1)
		{
			nStep = 3000;
			cc::Util::SetTransparent(formCopy->m_hWnd, 255);
			cc::Util::FourceWindowTop(formCopy->m_hWnd);
			::ShowWindow(m_hWnd, SW_HIDE);
		}
		//if(LOWORD(e->WParam) == WA_ACTIVE)
		{
			nStep = 1000;
			//::KillTimer(this->m_hWnd, UU_TIMERID_INACTIVE2);
			//::KillTimer(this->m_hWnd, UU_TIMERID_INACTIVE);

			if(nMode == 0)
			{
				nMode = 1;
				RECT rect = this->GetBounds();
				formCopy->SetLocation(rect.left, rect.top);
				formCopy->SetVisible(true);
				::SetWindowPos(formCopy->m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
				::SetParent(this->GetClient()->m_hWnd, formCopy->m_hWnd);

				//this->SetVisible(false);
				::ShowWindow(m_hWnd, SW_HIDE);
				//this->SetBounds(-1, -1, 0, 0);
				cc::Util::SetTransparent(formCopy->m_hWnd, 255);
				cc::Util::FourceWindowTop(formCopy->m_hWnd);
			}
		}
	}
	return Form::WndProc(e);
}

void CMainForm::notify1_Click(Object* sender, EventArgs* e)
{
	if(formCopy->GetVisible())
	{
		//cc::Log::debug("notify1_Click1, nStep:%d, nMode:%d", nStep, nMode);
		cc::Util::FourceWindowTop(formCopy->m_hWnd);
	}
	else
	{
		//cc::Log::debug("notify1_Click2, nStep:%d, nMode:%d", nStep, nMode);
		//cc::Util::FourceWindowTop(this->m_hWnd);
		SendMessage(this->m_hWnd, WM_ACTIVATE, WA_ACTIVE, 0);
	}
}

void CMainForm::Control_Click(Object* sender, EventArgs* e)
{
	if(sender == menuToday)
	{
		currentY = 0;
		currentM = 0;
		PrintMonth_All(false);
	}
	else if(sender == btnYPrev)
	{
		currentY--;
		PrintMonth_All(false);
	}
	else if(sender == btnYNext)
	{
		currentY++;
		PrintMonth_All(false);
	}
	else if(sender == btnMPrev)
	{
		cc::Time time = cc::Time(currentY, currentM, 1, 0, 0, 0);
		time.AddMonth(-1);
		currentY = time.GetYear();
		currentM = time.GetMonth();
		PrintMonth_All(false);
	}
	else if(sender == btnMNext)
	{
		cc::Time time = cc::Time(currentY, currentM, 1, 0, 0, 0);
		time.AddMonth(1);
		currentY = time.GetYear();
		currentM = time.GetMonth();
		PrintMonth_All(false);
	}
	else if(sender == menuHhmm)
	{
		bool chk = !menuHhmm->GetChecked();
		menuHhmm->SetChecked(chk);
		led->SetHhmm(chk);
	}
	else if(sender == menuShowBefore)
	{
		bool chk = !menuShowBefore->GetChecked();
		menuShowBefore->SetChecked(chk);
		PrintMonth_All(true);
	}
	else if(sender == menuShowAfter)
	{
		bool chk = !menuShowAfter->GetChecked();
		menuShowAfter->SetChecked(chk);
		PrintMonth_All(true);
	}
	else if(sender == menuShowSmall)
	{
		dayBigMultiple--;
		if(dayBigMultiple < 0)
		{
			dayBigMultiple = 0;
		}
		menuShowSmall->SetEnabled(dayBigMultiple > 0);
		menuShowBig->SetEnabled(dayBigMultiple < 10);
		PrintMonth_All(true);
	}
	else if(sender == menuShowBig)
	{
		dayBigMultiple++;
		if(dayBigMultiple > 10)
		{
			dayBigMultiple = 10;
		}
		menuShowSmall->SetEnabled(dayBigMultiple > 0);
		menuShowBig->SetEnabled(dayBigMultiple < 10);
		PrintMonth_All(true);
	}
	else if(sender == menuMonFirst)
	{
		bool chk = !menuMonFirst->GetChecked();
		menuMonFirst->SetChecked(chk);
		PrintMonth_All(true);
	}
	else if(sender == menuShowTime)
	{
		bool chk = !menuShowTime->GetChecked();
		menuHhmm->SetEnabled(chk);
		menuShowTime->SetChecked(chk);
		PrintMonth_All(true);
	}
	//else if(sender == menuNoCaption)
	//{
	//	bool chk = !menuNoCaption->GetChecked();
	//	menuNoCaption->SetChecked(chk);
	//	PrintMonth_All(true);
	//}
	else if(sender == menuOption)
	{
		nMode = 2;
		SetDialog* dialog = new SetDialog();
		dialog->SetParent(formCopy, true);
		dialog->SetCenter();
		DialogResult::Type type = dialog->ShowDialog();
		dialog->Dispose();
		delete dialog;
		nMode = 1;

		cc::Util::FourceWindowTop(formCopy->m_hWnd);

		if(type == DialogResult::OK)
		{
			nameCalendar1 = CC_APP_GetIni()->Get(_T("Main.Calendar1"), _T(""));
			nameCalendar2 = CC_APP_GetIni()->Get(_T("Main.Calendar2"), _T(""));
			nameCalendar3 = CC_APP_GetIni()->Get(_T("Main.Calendar3"), _T(""));
			corCalendar1 = CC_APP_GetIni()->Get(_T("Main.CalendarColor1"), RGB(255, 0, 0));
			corCalendar2 = CC_APP_GetIni()->Get(_T("Main.CalendarColor2"), RGB(0, 255, 0));
			corCalendar3 = CC_APP_GetIni()->Get(_T("Main.CalendarColor3"), RGB(0, 255, 255));

			Load_Calendar();
			PrintMonth_All(true);
		}
	}
	else if(sender == menuTopmost)
	{
		bool chk = !menuTopmost->GetChecked();
		menuTopmost->SetChecked(chk);
		//::SetWindowPos(m_hWnd, chk ? HWND_TOPMOST : HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		::SetParent(_pClient->m_hWnd, formCopy->m_hWnd);
		this->SetVisible(false);
		formCopy->SetVisible(true);
	}
	else if(sender == menuMemoList)
	{
		Day_Click(NULL, NULL);
	}
	else if(sender == menuAbout)
	{
		CC_APP_GetLang()->Set(_T("Lang.U_Link"), _T("http://uuware.com/"));
		CC_APP_GetLang()->Set(_T("Lang.U_LinkURL"), _T("http://uuware.com/"));
		cc::Str sAbout = _T("Desktop Calendar Ver 1.00 - uuware.com");
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
#ifndef _CC_ASSISTANT
		App::Exit();
#else
		this->SetVisible(false);
		formCopy->SetVisible(false);
		::SetTimer(this->mainFormHwnd, UU_TIMERID_CALENDAR_EXIT, 100, NULL);
#endif
	}
}
