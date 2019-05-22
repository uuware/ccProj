// cc.win.Form.h
// The C# like Form class
// Author: Shu.KK
// 2006-01-01
#include "cc.win.Module.h"
using namespace cc;
using namespace cc::win;

#include "cc.win.Design.hpp"

//===========================================================cc::win::ControlContainer
///////////////////////////////////////////////////////////////////////////////
// constructors/destructor
ControlContainer::ControlContainer()
{
	//DWORD ControlConfig::nOwnerDrawTextStyle = DT_LEFT|DT_VCENTER|DT_SINGLELINE;
	_StyleArray[(int)StyleType::OwnerDrawText] = DT_LEFT|DT_VCENTER|DT_WORDBREAK;
}

ControlContainer::~ControlContainer()
{
}
// constructors/destructor

void ControlContainer::OnPaint(PaintEventArgs *e)
{
	DRAWITEMSTRUCT data = {0};
	DrawItemEventArgs ee(e->baseArgs);
	ee.data = &data;
	HDC hDC = BeginPaint(m_hWnd, &(e->data));
	//If GetUpdateRect returns zero, the application should not call the BeginPaint and EndPaint functions.
	if(e->data.rcPaint.bottom != 0 && e->data.rcPaint.right != 0)
	{
		data.hDC = hDC;
		GetClientRect(m_hWnd, &data.rcItem);
		DWORD nInner2 = _StyleArray[(int)StyleType::Inner2];
		_StyleArray[(int)StyleType::Inner2] &= ~Inner2Style::IsMouseEnter;
		CC_APP_GetDrawActor()->DrawControl(this, &ee);
		_StyleArray[(int)StyleType::Inner2] = nInner2;
	}
	//call PaintBegin&PaintEnd at DrawToolItem
	EndPaint(m_hWnd, &(e->data));
}
///////////////////////////////////////////////////////////////////////////////

//===========================================================cc::win::PrintDocument
///////////////////////////////////////////////////////////////////////////////
PrintDocument::PrintDocument()
{
	_pPrintController = NULL;
}

PrintDocument::~PrintDocument()
{
	if(_pPrintController != NULL)
	{
		delete _pPrintController;
		_pPrintController = NULL;
	}
}

PrintController* PrintDocument::GetPrintController()
{
	return _pPrintController;
}
void PrintDocument::SetPrintController(PrintController* printController)
{
	_pPrintController = printController;
}

void PrintDocument::Print()
{
	CC_ASSERT(_pPrintController != NULL);

	PrintController* controller = _pPrintController;
	PrintEventArgs printArgs(NULL);
	QueryPageSettingsEventArgs queryArgs(NULL);
	PrintPageEventArgs pageArgs(NULL);
	HDC graphics;

	// Begin the printing process.
	//printArgs = new PrintEventArgs();
	OnBeginPrint(&printArgs);
	if(printArgs.Cancel)
	{
		return;
	}
	controller->OnStartPrint(this, &printArgs);

	// Wrap the rest in a "try" block so that the controller
	// will be properly shut down if an exception occurs.
	try
	{
		//queryArgs = new QueryPageSettingsEventArgs((PageSettings)(DefaultPageSettings.Clone()));
		do
		{
			// Query the page settings for the next page.
			OnQueryPageSettings(&queryArgs);
			if(queryArgs.Cancel)
			{
				break;
			}

			// Create the page argument structure.
			//pageArgs = new PrintPageEventArgs(queryArgs.PageSettings);
			// Get the graphics object to use to draw the page.
			graphics = controller->OnStartPage(this, &pageArgs);
			pageArgs.hDC = graphics;

			// Print the page.
			try
			{
				OnPrintPage(&pageArgs);
				controller->OnEndPage(this, &pageArgs);
			}
			catch(...)
			{
				cc::Log::printStackTrace();
			}
			//finally
			{
			//::ReleaseDC(m_hWnd, ee.hDC);
			//	graphics.Dispose();
			}
		}
		while(!(pageArgs.Cancel) && pageArgs.HasMorePages);
	}
	catch(...)
	{
		cc::Log::printStackTrace();
	}
	//finally
	{
		try
		{
			OnEndPrint(&printArgs);
		}
		catch(...)
		{
			cc::Log::printStackTrace();
		}
		//finally
		{
			controller->OnEndPrint(this, &printArgs);
		}
	}
}

void PrintDocument::OnBeginPrint(PrintEventArgs* e)
{
	BeginPrint.Invoke((Object*)this, e);
}

void PrintDocument::OnEndPrint(PrintEventArgs* e)
{
	EndPrint.Invoke((Object*)this, e);
}

void PrintDocument::OnPrintPage(PrintPageEventArgs* e)
{
	PrintPage.Invoke((Object*)this, e);
}

void PrintDocument::OnQueryPageSettings(QueryPageSettingsEventArgs* e)
{
	QueryPageSettings.Invoke((Object*)this, e);
}
///////////////////////////////////////////////////////////////////////////////
//===========================================================cc::win::PrintDocument

////===========================================================cc::win::VersionInfoXX
/////////////////////////////////////////////////////////////////////////////////
//// constructors/destructor
//VersionInfoXX::VersionInfoXX(bool isGroup)
//{
//	if(!isGroup)
//	{
//		//_ClassName = CC_WND;
//		SetStyle(StyleType::Outer, 0, OuterStyle::AlwaysEdge);
//	}
//	nStep = 0;
//	nURLIndex = 0;
//	//::SetRect(&_Rect, 0, 0, 100, 100);
//	//::CopyRect(&_OrgRect, &_Rect);
//	this->SetName(_T("VerInfo"));
//
//	btnReShow = new Button();
//	btnReShow->SetName(_T("btnReShow"));
//	btnReShow->SetLocation(0, 0);
//	btnReShow->SetSize(80, 21);
//	btnReShow->SetEnabled(false);
//	btnReShow->Click += EventHandler((Object*)this, (EventFun)&VersionInfoXX::btnReShow_Click);
//	this->AddControl(btnReShow);
//
//	chkAutoRemove = new CheckBox();
//	chkAutoRemove->SetName(_T("chkRemove"));
//	chkAutoRemove->SetLocation(81, 2);
//	//chkAutoRemove->SetChecked(true);
//	chkAutoRemove->SetStyle(StyleType::Outer, 0, OuterStyle::AutoConfig|OuterStyle::AutoSize);
//	chkAutoRemove->Click += EventHandler((Object*)this, (EventFun)&VersionInfoXX::chkAutoRemove_Click);
//	this->AddControl(chkAutoRemove);
//	if(isGroup)
//	{
//		btnReShow->SetLocation(4, 5);
//		chkAutoRemove->SetLocation(85, 7);
//	}
//
//	SetLang(*CC_APP_GetLang(), cc::Str(CC_CONFIG_LANGSECTION));
//}
//
/////////////////////// destructor //////////////////////////
//VersionInfoXX::~VersionInfoXX()
//{
//}
//// constructors/destructor
//
//void VersionInfoXX::OnInnerTimer(EventArgs *e)
//{
//	//e->WParam is ID
//	if(e->WParam != CC_TIMERID_VERSION)
//	{
//		GroupPanel::OnInnerTimer(e);
//		return;
//	}
//	::KillTimer(m_hWnd, CC_TIMERID_VERSION);
//	if(nStep <= 0)
//	{
//		nStep = 0;
//		ie_DocHandler* docHandler; //be deleted at ie
//		docHandler = new ie_DocHandler(new info_DocEvent());
//		docHandler->m_bIsShowContextMenu = false;
//		ie = new IEControl();
//		ie->SetName(_T("ie"));
//		//this is OK? yes! not same address then is group
//		//if(_ClassName != CC_WND)
//		if(HasStyle(StyleType::Outer, OuterStyle::AlwaysEdge))
//		{
//			ie->SetLocation(4, 30);
//			ie->SetSize(_OrgRect.right - _OrgRect.left - 5, _OrgRect.bottom - _OrgRect.top - 30);
//		}
//		else
//		{
//			ie->SetLocation(0, 22);
//			ie->SetSize(_OrgRect.right - _OrgRect.left, _OrgRect.bottom - _OrgRect.top - 22);
//		}
//		ie->SetOleObject(docHandler, NULL, NULL, strURL.IsNULL() ? CC_VERINFO_URL : (BSTR)strURL.GetBuf());
//		ie->Anchor = (AnchorStyles::Type)(AnchorStyles::Top | AnchorStyles::Left | AnchorStyles::Right | AnchorStyles::Bottom);
//#ifndef _DEBUG_
//		ie->SetVisible(false);
//#endif
//		this->AddControl(ie);
//		OnResize(NULL);
//		if(GetClient() != NULL)
//		{
//			SendMessage(GetClient()->m_hWnd, WM_SIZE, 0, 0);
//		}
//		::SetTimer(m_hWnd, CC_TIMERID_VERSION, 1000 * 10, NULL);
//		if(!strURL.IsNULL())
//		{
//			nStep = 201;
//		}
//	}
//	else if(nStep <= 200) //about 10minitus
//	{
//		bool isOK = false;
//		if(ie->IsComplete())
//		{
//			void* txt = NULL;
//			DWORD nLen = 0;
//			if(ie->SaveText(&txt, nLen))
//			{
//				//see point as char*
//				char* ctxt = (char*)txt;
//				char* pos = strstr(ctxt, CC_VERINFO_SELF);
//				if(pos != NULL)
//				{
//					//this is target page
//					isOK = true;
//				}
//				else
//				{
//					//find target page url
//					pos = strstr(ctxt, CC_VERINFO_URLS);
//					if(pos != NULL)
//					{
//						ctxt = pos + strlen(CC_VERINFO_URLS);
//						pos = strstr(ctxt, CC_VERINFO_URLE);
//						if(pos != NULL)
//						{
//							pos[0] = '\0';
//							pos[1] = '\0';
//							pos[2] = '\0';
//							CC_TRACE((ctxt));
//
//							strURL = cc::Str(LangCode::UNICODE16, ctxt);
//							if(strURL.IndexOf(L"?") < 0)
//							{
//								strURL.Append(L"?");
//							}
//							else
//							{
//								strURL.Append(L"&");
//							}
//							strURL.Append(L"proj=").Append(App::GetName()).Append(L"&ver=").Append(App::GetVersion());
//							ie->Navigate((BSTR)strURL.GetBuf());
//							isOK = true;
//						}
//					}
//				}
//			}
//			free(txt);
//			if(isOK)
//			{
//				nStep = 201;
//			}
//			else
//			{
//				if(nURLIndex < 2)
//				{
//					nURLIndex++;
//					ie->Navigate(nURLIndex == 1 ? CC_VERINFO_URL2 : CC_VERINFO_URL3);
//				}
//				else
//				{
//					char* c = "<html><body><div align=center><a href=\"http://www.uuware.com/\" target=\"_blank\"><font color=\"#008080\" size=\"10\">uuware.com</font></a></div><br><br><div><font size=\"2\">(c)Copyright uuware 2007. All rights reserved.</font></div></body></html>";
//					ie->LoadText(c, (int)strlen(c));
//					nStep = 201;
//				}
//			}
//		}
//		::SetTimer(m_hWnd, CC_TIMERID_VERSION, 1000 * 3, NULL);
//	}
//	else if(nStep <= 300) //about 5minitus
//	{
//		if(ie->IsComplete())
//		{
//			ie->SetVisible(true);
//			nStep = 301;
//			::SetTimer(m_hWnd, CC_TIMERID_VERSION, 1000 * 120, NULL);
//		}
//		else
//		{
//			::SetTimer(m_hWnd, CC_TIMERID_VERSION, 1000 * 3, NULL);
//		}
//	}
//	else
//	{
//		if(chkAutoRemove->GetChecked())
//		{
//			//remove ie
//			this->SetVisible(false);
//			this->RemoveControl(ie);
//			ie->Dispose();
//			delete ie;
//			ie = NULL;
//			btnReShow->SetEnabled(true);
//			chkAutoRemove->SetEnabled(false);
//			this->SetVisible(true);
//			::InvalidateRect(m_hWnd, NULL, TRUE);
//		}
//	}
//	nStep++;
//}
//
//void VersionInfoXX::OnCreateControl()
//{
//	GroupPanel::OnCreateControl();
//
//	int tOld = CC_APP_GetIni()->Get(_T("Main.VInfo"), 0);
//	int tNow = (int)(::time(NULL) - 1190000000);
//	if(tNow > tOld && tNow - tOld < CC_VERINFO_SKIPHOUR * 3600)
//	{
//		btnReShow->SetEnabled(true);
//		CC_TRACE((_T("VersionInfoXX::OnCreateControl, tOld:[%d], tNew:[%d], not Show VersionInfoXX."), tOld, tNow));
//		return;
//	}
//	CC_TRACE((_T("VersionInfoXX::OnCreateControl, tOld:[%d], tNew:[%d], Show VersionInfoXX."), tOld, tNow));
//	CC_APP_GetIni()->Set(_T("Main.VInfo"), tNow);
//#ifdef _DEBUG_
//	btnReShow->SetEnabled(true);
//#else
//	::SetTimer(m_hWnd, CC_TIMERID_VERSION, 1000 * 45, NULL);
//#endif
//}
//
//void VersionInfoXX::DoSetLang(KeyArr& keyArr, cc::Str sThisHead)
//{
//	this->SetText((TCHAR*)keyArr.Get(_T("Lang.VerInfo"), cc::Str(_T("Version Info"))).GetBuf());
//	chkAutoRemove->SetText((TCHAR*)keyArr.Get(_T("Lang.VerInfo.Remove"), cc::Str(_T("Remove Version Info after loaded minutes."))).GetBuf());
//	btnReShow->SetText((TCHAR*)keyArr.Get(_T("Lang.VerInfo.Show"), cc::Str(_T("ReShow"))).GetBuf());
//}
//
//void VersionInfoXX::chkAutoRemove_Click(Object* sender, EventArgs* e)
//{
//	if(chkAutoRemove->GetChecked() && nStep >= 301)
//	{
//		nStep = 301;
//		::KillTimer(m_hWnd, CC_TIMERID_VERSION);
//		//start timer
//		::SetTimer(m_hWnd, CC_TIMERID_VERSION, 1000, NULL);
//	}
//}
//
//void VersionInfoXX::btnReShow_Click(Object* sender, EventArgs* e)
//{
//	btnReShow->SetEnabled(false);
//	chkAutoRemove->SetEnabled(true);
//	nStep = 0;
//	//start timer
//	::SetTimer(m_hWnd, CC_TIMERID_VERSION, 1000, NULL);
//}

//http://www.codeproject.com/KB/recipes/krhtmlviewer.aspx
//http://www.koders.com/csharp/fidD0F69DABF7950CA0B770917201E390C6D43F78FC.aspx
//http://www.pin5i.com/showtopic-15212.html
//http://msdn.microsoft.com/en-us/library/aa140283(office.10).aspx
//http://www.gkrueger.com/personal/html2rtf/index.html
//http://www.codeproject.com/KB/HTML/XHTML2RTF.aspx
//http://hi.baidu.com/xaioxain/blog/item/3e6ab5caa007b080c817682e.html
char* RTFCreaterConfig::sLinkMarkStart = "[#~RTF_LINK_%d:S#]";
char* RTFCreaterConfig::sLinkMarkUrl = "[#~RTF_LINK_:URL#]";
char* RTFCreaterConfig::sLinkMarkEnd = "[#~RTF_LINK_%d:E#]";
//===========================================================cc::win::RTFCreater
///////////////////////////////////////////////////////////////////////////////
// constructors/destructor
RTFCreater::RTFCreater()
{
	nDefaultSize = 20;
	nHtmlLen = 0;
	nCurSize = nDefaultSize;
	nCurColor = CLR_INVALID;
	nCurBColor = CLR_INVALID;
	nLastSize = nCurSize;
	nLastColor = nCurColor;
	nLastBColor = nCurBColor;
	m_sText = cc::Str(LangCode::SYSTEM);
	m_sColorTable = cc::Str(LangCode::SYSTEM);
	//isSort, bIsDESC=false, isDistinct=false, nMinCapacity=0, nIncrement=1.3)
	hListInfo = new List<void*>(false, false, true, 15);
	//hListLink = new List<void*>(false, false, true);
	lastIsRTFKey = false;
	nLinkCount = -1;
}

RTFCreater::~RTFCreater()
{
	ClearList();
	if(hListInfo != NULL)
	{
		delete hListInfo;
		hListInfo = NULL;
	}
	//if(hListLink != NULL)
	//{
	//	delete hListLink;
	//	hListLink = NULL;
	//}
}
// constructors/destructor

UINT RTFCreater::GetDefaultSize()
{
	return nDefaultSize;
}
void RTFCreater::SetDefaultSize(UINT nSize)
{
	nDefaultSize = nSize;
}

void RTFCreater::LoadHtml(const char* html, int nLen, LangCode::Type lang)
{
	if(nLen < 0)
	{
		nLen = cc::Str::Len(html);
	}
	nHtmlLen = nLen;
#if _DEBUG_
	cc::Log::trace(_T("[RTFCreater::LoadHtml]nHtmlLen:%d"), nHtmlLen);
	CC_LOGTIME_START();
#endif
	CreaterStart();
	if(nHtmlLen >= 8 && strncmp(html, "{\\rtf1\\", 7) == 0)
	{
		m_sText.ValueOf(html);
		//do nothing for next
		nHtmlLen = 0;
	}
	pInfoCur = PushInfo(HTML_TAG_BEGIN);

	int n = 0;
	int nContent = 0;
	if(nLen >= 2 && (((unsigned char)html[0] == 0xFF && (unsigned char)html[1] == 0xFE) || ((unsigned char)html[0] == 0xFE && (unsigned char)html[1] == 0xFF)))
	{
		n += 2;
	}
	else if(nLen >= 3 && (unsigned char)html[0] == 0xEF && (unsigned char)html[1] == 0xBB && (unsigned char)html[2] == 0xBF)
	{
		n += 3;
	}
	for(; n < nHtmlLen - 1; n++)
	{
		if(html[n] != '<')
		{
			continue;
		}
		n++;

		//<P> or <P/> or <P ... />
		if(HtmlTag(html, n, "p", 1, false, nContent, "\\par"))
		{
		}

		//<b> and </b>
		else if(HtmlTag(html, n, "b", 1, false, nContent, "\\b"))
		{
		}
		else if(HtmlTag(html, n, "/b>", 3, true, nContent, "\\b0"))
		{
		}

		//<i> and </i>
		else if(HtmlTag(html, n, "i", 1, false, nContent, "\\i"))
		{
		}
		else if(HtmlTag(html, n, "/i>", 3, true, nContent, "\\i0"))
		{
		}

		//<u> and </u>
		else if(HtmlTag(html, n, "u", 1, false, nContent, "\\ul"))
		{
		}
		else if(HtmlTag(html, n, "/u>", 3, true, nContent, "\\ulnone"))
		{
		}

		//<s> and </s>
		else if(HtmlTag(html, n, "s", 1, false, nContent, "\\strike"))
		{
		}
		else if(HtmlTag(html, n, "/s>", 3, true, nContent, "\\strike0"))
		{
		}

		//<BR> or <BR/> or <BR ... />
		else if(HtmlTag(html, n, "BR", 2, false, nContent, "\\par"))
		{
		}

		//<A >
		else if(n + 2 < nHtmlLen && _strnicmp(html + n, "A", 1) == 0 && 
			(*(html + n + 1) == '>' || *(html + n + 1) == ' '))
		{
			AddText(html, nContent, n - 1, nCurColor, nCurBColor, nCurSize);
			pInfoCur = PushInfo(HTML_TAG_A);
			sCurHref.Clear();
			n = HtmlProperty(html, n + 1, false, true);
			if(sCurHref.GetLength() > 0)
			{
				AddLinkStart((const char*)sCurHref.GetBuf());
			}
			//nLinkCount++;
			//pInfoTmp = new HtmlInfo;
			//hListLink->Add(pInfoTmp);
			//pInfoTmp->nTag = nLinkCount;
			//pInfoTmp->nColor = 0;
			//pInfoTmp->nBColor = 0;
			//pInfoTmp->sURL = sCurHref;
			//if(sCurHref.GetLength() > 1)
			//{
				//{\field{\*\fldinst{HYPERLINK "http://aaaa.com/"}}{\fldrslt{\cf1\ul XXX}}}\cf0\ulnone
				//m_sText.Append("{\\field{\\*\\fldinst{HYPERLINK \"");
				//m_sText.Append(sCurHref);
				//nCurColor = 0x0000FF;
				//int nInd = AddColor(nCurColor);
				//m_sText.AppendFormat("\"}}{\\fldrslt{\\cf%d\\ul", nInd);
				//m_sText.AppendFormat("\\cf%d\\ul ", nInd);
				//if(lastIsRTFKey)
				//{
				//	m_sText.Append(" ");
				//}
				//m_sText.AppendFormat(RTFCreaterConfig::sLinkMarkStart, nLinkCount);
				//lastIsRTFKey = false;
			//}
			nContent = n + 1;
			//m_sText.AppendFormat(" [A]sCurHref:%s, ", (const char*)sCurHref.GetBuf());
			continue;
		}
		else if(HtmlTag(html, n, "/A>", 3, true, nContent, NULL, HTML_TAG_A))
		{
			//int nInd = AddColor(nCurColor);
			//m_sText.AppendFormat("}}}\\cf%d\\ulnone", nInd);
			//m_sText.AppendFormat("\\cf%d\\ulnone ", nInd);
			if(sCurHref.GetLength() > 0)
			{
				AddLinkEnd();
			}
			//if(lastIsRTFKey)
			//{
			//	m_sText.Append(" ");
			//}
			//m_sText.AppendFormat(RTFCreaterConfig::sLinkMarkEnd, nLinkCount);
			//lastIsRTFKey = false;
		}

		//<DIV>
		else if(n + 4 < nHtmlLen && _strnicmp(html + n, "DIV", 3) == 0 && 
			(*(html + n + 3) == '>' || *(html + n + 3) == ' '))
		{
			AddText(html, nContent, n - 1, nCurColor, nCurBColor, nCurSize);
			pInfoCur = PushInfo(HTML_TAG_DIV);
			sCurAlign.Clear();
			n = HtmlProperty(html, n + 3, false, false);
			nContent = n + 1;
			//m_sText.Append("\\pard\\nowidctlpar");
			if(!m_sText.EndsWith("\\par") && !m_sText.EndsWith("\\par\\pard") && hListInfo->GetSize() >= 3)
			{
				m_sText.Append("\\par\\pard");
				lastIsRTFKey = true;
			}
			if(sCurAlign.GetLength() > 1)
			{
				m_sText.Append(sCurAlign);
				sCurAlign.Clear();
				lastIsRTFKey = true;
			}
			continue;
		}
		else if(HtmlTag(html, n, "/DIV>", 5, true, nContent, NULL, HTML_TAG_DIV, true))
		{
		}

		//<CENTER>
		else if(HtmlTag(html, n, "CENTER>", 7, true, nContent, "\\qc", HTML_TAG_CENTER, true))
		{
		}
		else if(HtmlTag(html, n, "/CENTER>", 8, true, nContent, NULL, HTML_TAG_CENTER, true))
		{
		}

		//<SPAN>
		else if(n + 4 < nHtmlLen && _strnicmp(html + n, "SPAN", 4) == 0 && 
			(*(html + n + 4) == '>' || *(html + n + 4) == ' '))
		{
			AddText(html, nContent, n - 1, nCurColor, nCurBColor, nCurSize);
			pInfoCur = PushInfo(HTML_TAG_SPAN);
			n = HtmlProperty(html, n + 4, false, false);
			nContent = n + 1;
			continue;
		}
		else if(HtmlTag(html, n, "/SPAN>", 6, true, nContent, NULL, HTML_TAG_SPAN))
		{
		}

		//FONT
		else if(n + 5 < nHtmlLen && _strnicmp(html + n, "FONT", 4) == 0 && 
			(*(html + n + 4) == '>' || *(html + n + 4) == ' '))
		{
			AddText(html, nContent, n - 1, nCurColor, nCurBColor, nCurSize);
			pInfoCur = PushInfo(HTML_TAG_FONT);
			n = HtmlProperty(html, n + 4, true, false);
			nContent = n + 1;
			continue;
		}
		else if(HtmlTag(html, n, "/FONT>", 6, true, nContent, NULL, HTML_TAG_FONT))
		{
		}

		//<!-- my special comment -->
		else if(HtmlSkip(html, n, nContent, "!-- [#~RTF_COMM:S#] -->", 23, "<!-- [#~RTF_COMM:E#] -->", 24))
		{
		}
		//<!-- comment -->
		else if(HtmlSkip(html, n, nContent, "!--", 3, "-->", 3))
		{
		}
		//<script...>...</script>
		else if(HtmlSkip(html, n, nContent, "script", 6, "</script>", 9))
		{
		}
		//<style...>...</style>
		else if(HtmlSkip(html, n, nContent, "style", 5, "</style>", 8))
		{
		}
		else
		{
			//unknow tag
			n = HtmlNextChar(html, n, '>');
			nContent = n + 1;
		}
	}
	if(nContent < nHtmlLen)
	{
		AddText(html, nContent, nHtmlLen - 1, nCurColor, nCurBColor, nCurSize);
	}
	CreaterEnd(true);
	CC_LOGTIME_PRINT();
}

bool RTFCreater::HtmlTag(const char* html, int& nStart, const char* tcKey, int nKeyLen, bool isTagEnd, int& nContent, const char* tcAdd, int nTag, bool isNewLine)
{
	if(_strnicmp(html + nStart, tcKey, nKeyLen) == 0 && (isTagEnd || *(html + nStart + nKeyLen) == '>'
		|| *(html + nStart + nKeyLen) == '/' || *(html + nStart + nKeyLen) == ' '))
	{
		AddText(html, nContent, nStart - 1, nCurColor, nCurBColor, nCurSize);
		if(isTagEnd)
		{
			nStart += nKeyLen - 1;
		}
		else
		{
			nStart = HtmlNextChar(html, nStart + nKeyLen, '>');
		}
		nContent = nStart + 1;
		if(isNewLine && !m_sText.EndsWith("\\par") && !m_sText.EndsWith("\\par\\pard"))
		{
			m_sText.Append("\\par\\pard");
			lastIsRTFKey = true;
		}
		if(tcAdd != NULL)
		{
			m_sText.Append(tcAdd);
			lastIsRTFKey = true;
		}
		if(nTag >= 0)
		{
			nLastSize = nCurSize;;
			nLastColor = nCurColor;
			nLastBColor = nCurBColor;
			//sLastAlign = sCurAlign;
			//pop last same tag
			PopInfo(nTag);
		}
		return true;
	}
	return false;
}

bool RTFCreater::HtmlSkip(const char* html, int& nStart, int& nContent, const char* tcKey, int nKeyLen, const char* tcKey2, int nKeyLen2)
{
	if(_strnicmp(html + nStart, tcKey, nKeyLen) == 0)
	{
		AddText(html, nContent, nStart - 1, nCurColor, nCurBColor, nCurSize);
		for(nStart += nKeyLen; nStart <= nHtmlLen - nKeyLen2 && _strnicmp(html + nStart, tcKey2, nKeyLen2) != 0; nStart++)
		{
		}
		//if not found,then end
		nStart += nKeyLen2 - 1;
		nContent = nStart + 1;
		return true;
	}
	return false;
}

int RTFCreater::AddText(const char* html, int nStart, int nEnd, int nColor, int nBColor, int nSize)
{
	int nLen = (nEnd - nStart);
	if(nStart >= 0 && nLen > 0)
	{
		int nInd = 0;
		if(nColor != nLastColor)
		{
			nLastColor = nColor;
			nInd = AddColor(nColor);
			m_sText.AppendFormat("\\cf%d", nInd);
			lastIsRTFKey = true;
		}
		if(nCurBColor != nLastBColor)
		{
			nLastBColor = nBColor;
			nInd = AddColor(nBColor);
			m_sText.AppendFormat("\\highlight%d", nInd);
			lastIsRTFKey = true;
		}
		if(nSize != nLastSize)
		{
			nLastSize = nSize;
			if(nSize > 0)
			{
				m_sText.AppendFormat("\\fs%d", nSize);
				lastIsRTFKey = true;
			}
		}
		if(lastIsRTFKey)
		{
			m_sText.Append(" ");
		}
		cc::Str sWchar = cc::Str(LangCode::UNICODE16);
		sWchar.ValueOf(html + nStart, 0, nLen, LangCode::UTF8);
		wchar_t* pwch = (wchar_t*)sWchar.GetBuf();
		for(int n = 0; n < sWchar.GetLength(); n++)
		{
			if(pwch[n] >= 255)
			{
				m_sText.AppendFormat("\\u%d ?", pwch[n]);
			}
			else
			{
				m_sText.Append(pwch + n, 0, 1);
			}
		}
		//m_sText.Append(html + nStart, 0, nLen, LangCode::UTF8);
		lastIsRTFKey = false;
		return nLen;
	}
	return -1;
}
void RTFCreater::AddLinkStart(const char* link)
{
	if(lastIsRTFKey)
	{
		m_sText.Append(" ");
	}
	nLinkCount++;
	m_sText.AppendFormat(RTFCreaterConfig::sLinkMarkStart, nLinkCount);
	m_sText.Append(link);
	m_sText.Append(RTFCreaterConfig::sLinkMarkUrl);
	lastIsRTFKey = false;
}
void RTFCreater::AddLinkEnd()
{
	if(lastIsRTFKey)
	{
		m_sText.Append(" ");
	}
	m_sText.AppendFormat(RTFCreaterConfig::sLinkMarkEnd, nLinkCount);
	lastIsRTFKey = false;
}

//const List<void*>* RTFCreater::GetLinkList()
//{
//	return hListLink;
//}

void RTFCreater::ClearList()
{
	if(hListInfo != NULL)
	{
		for(int n = hListInfo->GetSize() - 1; n >= 0; n--)
		{
			pInfoTmp = (HtmlInfo*)hListInfo->GetKey(n);
			delete pInfoTmp;
		}
		hListInfo->Clear();
	}
	//if(isClearLink && hListLink != NULL)
	//{
	//	for(int n = hListLink->GetSize() - 1; n >= 0; n--)
	//	{
	//		pInfoTmp = (HtmlInfo*)hListLink->GetKey(n);
	//		delete pInfoTmp;
	//	}
	//	hListLink->Clear();
	//}
}

void RTFCreater::CreaterStart()
{
	m_sText.SetCapacity((nHtmlLen + 1) * 2, (float)1.3);
	m_sText.Clear();
	m_sColorTable.Clear();
	ClearList();
	nCurSize = nDefaultSize;;
	nCurColor = CLR_INVALID;
	nCurBColor = CLR_INVALID;
	sCurAlign = cc::Str(LangCode::SYSTEM);
	sCurHref = cc::Str(LangCode::SYSTEM);
	nLastSize = nCurSize;;
	nLastColor = nCurColor;
	nLastBColor = nCurBColor;
	//sLastAlign = sCurAlign;

	m_sText.Append("{\\rtf1\\ansi\\deff0");
	m_sText.Append("{\\fonttbl{\\f0\\fmodern\\fprq2\\fcharset128 MS UI Gothic;}}\r\n");
	m_sText.Append("{\\colortbl ;[#COLOR_TABLE#]}\r\n");
	m_sText.Append("\\viewkind4");
	m_sText.AppendFormat("\\fs%d", nCurSize);
	lastIsRTFKey = true;
	nLinkCount = -1;
}

void RTFCreater::CreaterEnd(bool isHtml)
{
	m_sText.Append("}\r\n");
	m_sText.Replace("[#COLOR_TABLE#]", (char*)m_sColorTable.GetBuf());

	if(isHtml)
	{
		struct HTMLCODES{
			char szCode[8];
			char Code[2];
		};
		HTMLCODES arrHTMLCodes[] = {
			{"&nbsp;", " "},
			{"&quot;", "\""},
			{"&amp;", "&"},
			{"&lt;", "<"},
			{"&gt;", ">"},
			{"&#039;", "'"}
		};
		for(int n = 0; n < sizeof(arrHTMLCodes) / sizeof(HTMLCODES); n++)
		{
			m_sText.Replace(arrHTMLCodes[n].szCode, arrHTMLCodes[n].Code);
		}
	}
}

HtmlInfo* RTFCreater::PushInfo(int nTag)
{
	pInfoTmp = new HtmlInfo;
	hListInfo->Add(pInfoTmp);
	pInfoTmp->nTag = nTag;
	pInfoTmp->nSize = nCurSize;
	pInfoTmp->nColor = nCurColor;
	pInfoTmp->nBColor = nCurBColor;
	return pInfoTmp;
}

bool RTFCreater::PopInfo(int nTag)
{
	//pop last same tag
	int nSize = hListInfo->GetSize();
	if(nSize > 0)
	{
		//must have one entity
		pInfoTmp = (HtmlInfo*)hListInfo->GetKey(nSize - 1);
		if(pInfoTmp->nTag == nTag)
		{
			hListInfo->RemoveAt(nSize - 1);
			nCurSize = pInfoTmp->nSize;
			nCurColor = pInfoTmp->nColor;
			nCurBColor = pInfoTmp->nBColor;
			delete pInfoTmp;
			return true;
		}
		else
		{
			CC_TRACE((_T("PopInfo Error, want:%d, but is:%d"), nTag, pInfoTmp->nTag));
		}
	}
	return false;
}

//if exist,then index+1,other wise 0 for default color
int RTFCreater::AddColor(int nColor)
{
	//\red255\green0\blue0;\red0\green255\blue0;\red0\green0\blue0;
	if(nColor != CLR_INVALID)
	{
		int r = (((DWORD)nColor & 0x00FF0000) >> 16);
		int g = (((DWORD)nColor & 0x0000FF00) >> 8);
		int b = ((DWORD)nColor & 0x000000FF);
		cc::Str buf = cc::Str(LangCode::SYSTEM);
		buf.Format("\\red%d\\green%d\\blue%d;", r, g, b);
		int nIndCor = m_sColorTable.IndexOf(buf);
		if(nIndCor > 0)
		{
			buf = m_sColorTable.Left(nIndCor + 1);
			return (buf.FindCount(";") + 1);
		}
		else
		{
			m_sColorTable.Append(buf);
			return m_sColorTable.FindCount(";");
		}
	}
	return 0;
}

int RTFCreater::HtmlNextChar(const char* html, int nStart, char ch)
{
	for(; nStart < nHtmlLen && html[nStart] != ch; nStart++)
	{
	}
	return nStart;
}

int RTFCreater::HtmlNextWord(const char* html, int nStart, cc::Str& sRet)
{
	bool isBegin = false;
	int nPos1 = -1;
	char nChar = '\0';
	for(; nStart < nHtmlLen; nStart++)
	{
		if(html[nStart] == '>')
		{
			break;
		}
		else if(html[nStart] == '\'')
		{
			nChar = '\'';
		}
		else if(html[nStart] == '\"')
		{
			nChar = '\"';
		}
		else if(html[nStart] == '=')
		{
			isBegin = true;
		}
		else if(html[nStart] != ' ' && html[nStart] != '	')
		{
			if(isBegin)
			{
				nPos1 = nStart;
				while(nStart < nHtmlLen && html[nStart] != '\'' && html[nStart] != '"' && html[nStart] != ' ' && html[nStart] != '	' && html[nStart] != '\r' && html[nStart] != '\n')
				{
					nStart++;
				}
				if(nStart < nHtmlLen)
				{
					sRet.ValueOf(html, nPos1, nStart - nPos1, LangCode::UTF8);
				}
			}
			break;
		}
		if(nChar != '\0')
		{
			nStart++;
			nPos1 = nStart;
			while(nStart < nHtmlLen && html[nStart] != nChar && html[nStart] != '\r' && html[nStart] != '\n')
			{
				nStart++;
			}
			if(isBegin && nStart < nHtmlLen)
			{
				sRet.ValueOf(html, nPos1, nStart - nPos1, LangCode::UTF8);
			}
			break;
		}
	}
	return nStart;
}

int RTFCreater::HtmlNextStyle(const char* html, int nStart, cc::Str& sRet)
{
	int nPos1 = -1;
	int nFlag = -1;
	for(; nStart < nHtmlLen; nStart++)
	{
		if(html[nStart] == ':')
		{
			nPos1 = nStart + 1;
			for(; nStart < nHtmlLen && html[nStart] != ';'; nStart++)
			{
			}
			sRet.ValueOf(html, nPos1, nStart - nPos1, LangCode::UTF8);
			break;
		}
		else if(html[nStart] != ' ' && html[nStart] != '	')
		{
			break;
		}
	}
	return nStart;
}

int RTFCreater::HtmlStyle(const char* html, int nStart, int nEnd)
{
	cc::Str sRet = cc::Str(LangCode::SYSTEM);
	for(; nStart < nEnd; nStart++)
	{
		if(_strnicmp(html + nStart, "font-size", 9) == 0)
		{
			nStart = HtmlNextStyle(html, nStart + 9, sRet);
			sRet.Trim();
			//CC_TRACE(("font-size:%s", (char*)sRet.GetBuf()));
			if(sRet.GetLength() > 0)
			{
				nCurSize = sRet.ToInt() * 2;
			}
		}
		else if(_strnicmp(html + nStart, "color", 5) == 0)
		{
			nStart = HtmlNextStyle(html, nStart + 5, sRet);
			sRet.Trim();
			//CC_TRACE(("color:%s", (char*)sRet.GetBuf()));
			if(sRet.GetLength() == 7 && sRet.StartsWith("#"))
			{
				sRet.Remove(0, 1);
				nCurColor = (COLORREF)sRet.ToInt(16);
			}
		}
		else if(_strnicmp(html + nStart, "background", 10) == 0)
		{
			nStart = HtmlNextStyle(html, nStart + 10, sRet);
			sRet.Trim();
			//CC_TRACE(("background:%s", (char*)sRet.GetBuf()));
			if(sRet.GetLength() == 7 && sRet.StartsWith("#"))
			{
				sRet.Remove(0, 1);
				nCurBColor = (COLORREF)sRet.ToInt(16);
			}
		}
		else if(_strnicmp(html + nStart, "text-align", 10) == 0)
		{
			nStart = HtmlNextStyle(html, nStart + 10, sCurAlign);
			sCurAlign.Trim();
			//CC_TRACE(("text-align:%s", (char*)sCurAlign.GetBuf()));
		}
	}
	return nStart;
}

int RTFCreater::HtmlProperty(const char* html, int nStart, bool isFont, bool isHref)
{
	cc::Str sRet = cc::Str(LangCode::SYSTEM);
	for(; nStart < nHtmlLen; nStart++)
	{
		if(_strnicmp(html + nStart, ">", 1) == 0)
		{
			break;
		}
		else if(isFont && _strnicmp(html + nStart, "size", 4) == 0)
		{
			nStart = HtmlNextWord(html, nStart + 4, sRet);
			sRet.Trim();
			//CC_TRACE(("size:%s", (char*)sRet.GetBuf()));
			if(sRet.GetLength() > 0)
			{
				nCurSize = sRet.ToInt() * 6;
			}
		}
		else if(isFont && _strnicmp(html + nStart, "color", 5) == 0)
		{
			nStart = HtmlNextWord(html, nStart + 5, sRet);
			sRet.Trim();
			//CC_TRACE(("color:%s", (char*)sRet.GetBuf()));
			if(sRet.GetLength() == 7 && sRet.StartsWith("#"))
			{
				sRet.Remove(0, 1);
				nCurColor = (COLORREF)sRet.ToInt(16);
			}
		}
		else if(isHref && _strnicmp(html + nStart, "href", 4) == 0)
		{
			nStart = HtmlNextWord(html, nStart + 4, sCurHref);
			sCurHref.Trim();
			//CC_TRACE(("href:%s", (char*)sCurHref.GetBuf()));
		}
		else if(_strnicmp(html + nStart, "align", 5) == 0)
		{
			nStart = HtmlNextWord(html, nStart + 5, sCurAlign);
			sCurAlign.Trim();
			//CC_TRACE(("align:%s", (char*)sCurAlign.GetBuf()));
		}
		else if(_strnicmp(html + nStart, "style", 5) == 0)
		{
			nStart = HtmlNextWord(html, nStart + 5, sRet);
			sRet.Trim();
			//CC_TRACE(("style:%s", (char*)sRet.GetBuf()));
			if(sRet.GetLength() > 5)
			{
				HtmlStyle((const char*)sRet.GetBuf(), 0, sRet.GetLength());
			}
		}
	}
	if(sCurAlign.GetLength() > 1)
	{
		if(sCurAlign.Equals("left", true))
		{
			sCurAlign.ValueOf("\\ql");
		}
		else if(sCurAlign.Equals("center", true))
		{
			sCurAlign.ValueOf("\\qc");
		}
		else if(sCurAlign.Equals("right", true))
		{
			sCurAlign.ValueOf("\\qr");
		}
		else
		{
			sCurAlign.Clear();
		}
	}
	return nStart;
}
//===========================================================cc::win::RTFCreater
///////////////////////////////////////////////////////////////////////////////

char* HtmlTextBoxConfig::sSkipURL = "skip://";
//===========================================================cc::win::HtmlTextBox
///////////////////////////////////////////////////////////////////////////////
// constructors/destructor
HtmlTextBox::HtmlTextBox()
{
	//use of TextBox
	//BASE_CONTROL_CONFIG_INIT(HtmlTextBox);
	_Rect.right = _Rect.left + 70;
	_Rect.bottom = _Rect.top + 21;
	::CopyRect(&_OrgRect, &_Rect);
	hListLink = NULL;
	SetText(_T("HtmlTextBox"));
#ifdef _DEBUG_
	SetName(_T("HtmlTextBox"));
#endif
	SetTrackMouse(true);
}

HtmlTextBox::~HtmlTextBox()
{
	if(hListLink != NULL)
	{
		ClearList();
		delete hListLink;
		hListLink = NULL;
	}
}
// constructors/destructor

void HtmlTextBox::ClearList()
{
	if(hListLink != NULL)
	{
		for(int n = hListLink->GetSize() - 1; n >= 0; n--)
		{
			HtmlInfo* pInfoTmp = (HtmlInfo*)hListLink->GetKey(n);
			delete pInfoTmp;
		}
		hListLink->Clear();
	}
}

void HtmlTextBox::LoadHtml(const char* html, int nLen, LangCode::Type lang)
{
	CC_ASSERT(::IsWindow(m_hWnd));

	if(nLen <= 0)
	{
		nLen = cc::Str::Len(html);
	}
	if(nLen >= 8 && strncmp(html, "{\\rtf1\\", 7) == 0)
	{
		StreamIn(SF_RTF, html);
	}
	else
	{
		RTFCreater rtfCreater;
		rtfCreater.SetDefaultSize(20);
		rtfCreater.LoadHtml(html, nLen, lang);

		//CC_TRACE(((char*)rtfCreater.m_sText.GetBuf()));
		StreamIn(SF_RTF, (char*)rtfCreater.m_sText);
		//const List<void*>* hListLinkOrg = rtfCreater.GetLinkList();
		//if(hListLinkOrg != NULL)
		//{
		//	for(int n = 0; n < hListLinkOrg->GetSize(); n++)
		//	{
		//		HtmlInfo* pInfoTmp = (HtmlInfo*)hListLinkOrg->GetKey(n);
		//		AddLink((const char*)pInfoTmp->sURL.GetBuf());
		//	}
		//}
	}
	MarkLink();
}

void HtmlTextBox::LoadRtf(const char* rtf, int nLen)
{
	CC_ASSERT(::IsWindow(m_hWnd));

	if(nLen <= 0)
	{
		nLen = cc::Str::Len(rtf);
	}
	if(nLen >= 12 && strncmp(rtf, "{\\rtf1\\ansi\\", 12) == 0)
	{
		StreamIn(SF_RTF, rtf);
		//bool isFound = false;
		////{\field{\*\fldinst{HYPERLINK "http://www.uuware.com/"}}{\fldrslt{\cf4\ul XXXXX}}}
		//const char* p1 = strstr(rtf + 12, "{\\field{\\*\\fldinst");
		//int nPosLast = 0;
		//while(p1 != NULL)
		//{
		//	p1 += 18;
		//	if(*p1 != '{' && *(p1 + 1) != '{')
		//	{
		//		break;
		//	}
		//	const char* p2 = strstr(p1, "HYPERLINK ");
		//	if(p2 == NULL)
		//	{
		//		break;
		//	}
		//	p2 += 10;
		//	while(p2 != NULL && *p2 != '\"')
		//	{
		//		p2++;
		//	}
		//	if(p2 != NULL)
		//	{
		//		p2++;
		//	}
		//	while(p2 != NULL && *p2 != '\"')
		//	{
		//		p2++;
		//	}
		//		nPos = cc::Str::Search(rtf, "{\\field{\\*\\fldinst", nPosSave);
		//		if(nPos <= 0 || nPos > nLen - 20)
		//		{
		//			break;
		//		}
		//		if(rtf[nPos] != '{' && rtf[nPos + 1] == '{')
		//		{
		//			break;
		//		}
		//		int nPos2 = cc::Str::Search(rtf, "HYPERLINK ", nPos + 10);
		//		int nPos3 = cc::Str::Search(rtf, "}", nPos + 10);
		//		if(nPos2 > 0 &6 nPos3 > 0 && nPos2 > nPos3)
		//		{
		//			int nPos4 = cc::Str::Search(rtf, "\"", nPos2 + 10);
		//			if(nPos4 > 0 && nPos4 < nPos3)
		//			{
		//				int nPos5 = cc::Str::Search(rtf, "\"", nPos4 + 1);
		//			}
		//		}
		//		nPosSave = nPos + 10
		//	}
		//	StreamIn(SF_RTF, (char*)buf.GetBuf());
		//	MarkLink();
		//}
		//if!(isFound)
		//{
		//	StreamIn(SF_RTF, rtf);
		//}
	}
	else
	{
		SendMessageA(m_hWnd, WM_SETTEXT, 0, (LPARAM)(rtf == NULL ? "" : rtf));
	}
}

HtmlInfo* HtmlTextBox::AddLink(const char* url)
{
	if(hListLink == NULL)
	{
		hListLink = new List<void*>(false, false, true);
	}
	HtmlInfo* pInfoTmp = new HtmlInfo;
	hListLink->Add(pInfoTmp);
	pInfoTmp->nTag = (hListLink->GetSize() - 1);
	pInfoTmp->nColor = 0;
	pInfoTmp->nBColor = 0;
	pInfoTmp->sURL.ValueOf(url);
	return pInfoTmp;
}

void HtmlTextBox::MarkLink()
{
	CC_ASSERT(::IsWindow(m_hWnd));

	//DWORD dwMask = (DWORD)::SendMessage(m_hWnd, EM_GETEVENTMASK, 0L, 0L);
	//dwMask |= ENM_LINK;
	//::SendMessage(m_hWnd, EM_SETEVENTMASK, 0L, (LPARAM)dwMask);

	::SendMessage(m_hWnd, EM_HIDESELECTION, TRUE, 0);
	int nMaxLen = (int)::SendMessage(m_hWnd, WM_GETTEXTLENGTH, 0, 0);
	int nInd = 0;
	CHARFORMAT2 cf;
	ZeroMemory(&cf, sizeof(CHARFORMAT2));
	cf.cbSize = sizeof(CHARFORMAT2);
	cf.dwMask = CFM_LINK;
	cf.dwEffects |= CFE_LINK;
	FINDTEXT ft;
	ZeroMemory(&ft, sizeof(FINDTEXT));
	cc::Str buf = cc::Str();
	int n = -1;
	while(true)
	{
		n++;
		//HtmlInfo* pInfoTmp = (HtmlInfo*)hListLink->GetKey(n);
		//link mark start
		buf.Format(RTFCreaterConfig::sLinkMarkStart, n);
		ft.chrg.cpMin = nInd;
		ft.chrg.cpMax = nMaxLen;
		ft.lpstrText = (TCHAR*)buf.GetBuf();
		nInd = (int)::SendMessage(m_hWnd, EM_FINDTEXT, FR_DOWN|FR_MATCHCASE, (LPARAM)&ft);
		if(nInd < 0)
		{
			break;
		}

		//link url
		buf.ValueOf(RTFCreaterConfig::sLinkMarkUrl);
		ft.chrg.cpMin = nInd + buf.GetLength();
		ft.chrg.cpMax = nMaxLen;
		ft.lpstrText = (TCHAR*)buf.GetBuf();
		int nInd2 = (int)::SendMessage(m_hWnd, EM_FINDTEXT, FR_DOWN|FR_MATCHCASE, (LPARAM)&ft);
		if(nInd2 < 0)
		{
			break;
		}
		HtmlInfo* pInfoTmp = AddLink("");
		pInfoTmp->nColor = nInd;
		//get url
		::SendMessage(m_hWnd, EM_SETSEL, nInd + buf.GetLength() - 1, nInd2);
		pInfoTmp->sURL.ValueOf(GetTextSel());
		//clear mark
		::SendMessage(m_hWnd, EM_SETSEL, nInd, nInd2 + buf.GetLength());
		::SendMessage(m_hWnd, EM_REPLACESEL, false, (LPARAM)_T(""));

		//link mark end
		buf.Format(RTFCreaterConfig::sLinkMarkEnd, n);
		ft.chrg.cpMin = nInd;
		ft.lpstrText = (TCHAR*)buf.GetBuf();
		nInd = (int)::SendMessage(m_hWnd, EM_FINDTEXT, FR_DOWN|FR_MATCHCASE, (LPARAM)&ft);
		if(nInd < 0)
		{
			break;
		}
		pInfoTmp->nBColor = nInd;
		::SendMessage(m_hWnd, EM_SETSEL, nInd, nInd + buf.GetLength());
		::SendMessage(m_hWnd, EM_REPLACESEL, false, (LPARAM)_T(""));

		//CC_TRACE(("n:%d, start:%d, end:%d, URL:%s", n, pInfoTmp->nColor, pInfoTmp->nBColor, (const char*)pInfoTmp->sURL.GetBuf()));
		if(pInfoTmp->sURL.GetLength() > 0)
		{
			::SendMessage(m_hWnd, EM_SETSEL, pInfoTmp->nColor, pInfoTmp->nBColor);
			::SendMessage(m_hWnd, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);
		}
	}
	::SendMessage(m_hWnd, EM_HIDESELECTION, 0, 0);
	::SendMessage(m_hWnd, EM_SETSEL, 0, 0);
}

void HtmlTextBox::OnMouseLeave(EventArgs* e)
{
	Control::OnMouseLeave(e);
	HWND hWnd = CC_APP_GetActiveFormHwnd();
	if(hWnd != NULL)
	{
		::SendMessage(hWnd, CC_WM_MAINMSG, CC_WM_MAINMSG_WPARAM_CLEARSTATUS, (LPARAM)NULL);
	}
}

void HtmlTextBox::OnMouseHover(EventArgs* e)
{
	if(hListLink != NULL)
	{
		POINT pt;
		GetCursorPos(&pt);
		::ScreenToClient(m_hWnd, &pt);
		int nPos = CharFromPos(pt);
		if(nPos >= 0)
		{
			POINT pt2 = PosFromChar((UINT)nPos);
			if(pt2.x >= pt.x - 5 && pt2.x <= pt.x + 5)
			{
				for(int n = hListLink->GetSize() - 1; n >= 0; n--)
				{
					HtmlInfo* pInfoTmp = (HtmlInfo*)hListLink->GetKey(n);
					if(nPos >= pInfoTmp->nColor && nPos <= pInfoTmp->nBColor)
					{
						HWND hWnd = CC_APP_GetActiveFormHwnd();
						if(hWnd != NULL)
						{
							cc::Str sText = cc::Str(LangCode::TCHAR, pInfoTmp->sURL);
							::SendMessage(hWnd, CC_WM_MAINMSG, CC_WM_MAINMSG_WPARAM_TEXTSTATUS, (LPARAM)(const TCHAR*)sText.GetBuf());
						}
						return;
					}
				}
			}
		}
		HWND hWnd = CC_APP_GetActiveFormHwnd();
		if(hWnd != NULL)
		{
			::SendMessage(hWnd, CC_WM_MAINMSG, CC_WM_MAINMSG_WPARAM_CLEARSTATUS, (LPARAM)NULL);
		}
	}
	MouseHover.Invoke((Object*)this, e);
}

void HtmlTextBox::OnMouseUp(MouseEventArgs* e)
{
	if(hListLink != NULL)
	{
		POINT pt;
		GetCursorPos(&pt);
		::ScreenToClient(m_hWnd, &pt);
		int nPos = CharFromPos(pt);
		if(nPos >= 0)
		{
			POINT pt2 = PosFromChar((UINT)nPos);
			if(pt2.x >= pt.x - 5 && pt2.x <= pt.x + 5)
			{
				e->baseArgs->WParam = (WPARAM)NULL;
				for(int n = hListLink->GetSize() - 1; n >= 0; n--)
				{
					HtmlInfo* pInfoTmp = (HtmlInfo*)hListLink->GetKey(n);
					if(nPos >= pInfoTmp->nColor && nPos <= pInfoTmp->nBColor)
					{
						e->baseArgs->WParam = (WPARAM)pInfoTmp;
						if(!pInfoTmp->sURL.StartsWith(HtmlTextBoxConfig::sSkipURL), true)
						{
							ShellExecuteA(NULL, "open", (const char*)pInfoTmp->sURL.GetBuf(), NULL, NULL, SW_SHOWNORMAL);
							break;
						}
						ClickLink.Invoke((Object*)this, e->baseArgs);
					}
				}
			}
		}
	}
	MouseUp.Invoke((Object*)this, e);
}
//
//void HtmlTextBox::OnChildNotify(EventArgs *e)
//{
//	LPNMHDR p_pNotifyStruct = (LPNMHDR)e->LParam;
//	if(hListLink != NULL && p_pNotifyStruct->code == EN_LINK)
//	{
//		ENLINK *pLink = (ENLINK*)e->LParam;
//		//if(false && pLink->msg == WM_MOUSEMOVE)
//		//{
//		//	for(int n = hListLink->GetSize() - 1; n >= 0; n--)
//		//	{
//		//		HtmlInfo* pInfoTmp = (HtmlInfo*)hListLink->GetKey(n);
//		//		if(pInfoTmp->nColor >= pLink->chrg.cpMin - 1 && pInfoTmp->nBColor <= pLink->chrg.cpMax + 1)
//		//		{
//		//			Form* form = (Form*)CC_APP_GetMainForm();
//		//			if(form != NULL)
//		//			{
//		//				cc::Str sText = cc::Str(pInfoTmp->sURL);
//		//				form->SetStatus(sText);
//		//				form->SetTips(NULL);
//		//				form->SetTips(sText);
//		//			}
//		//			return;
//		//		}
//		//	}
//		//}
//		if(pLink->msg == WM_LBUTTONUP)
//		{
//			e->WParam = (WPARAM)NULL;
//			for(int n = hListLink->GetSize() - 1; n >= 0; n--)
//			{
//				HtmlInfo* pInfoTmp = (HtmlInfo*)hListLink->GetKey(n);
//				if(pInfoTmp->nColor >= pLink->chrg.cpMin - 1 && pInfoTmp->nBColor <= pLink->chrg.cpMax + 1)
//				{
//					e->WParam = (WPARAM)pInfoTmp;
//					if(!pInfoTmp->sURL.StartsWith(HtmlTextBoxConfig::sSkipURL), true)
//					{
//						ShellExecuteA(NULL, "open", (const char*)pInfoTmp->sURL.GetBuf(), NULL, NULL, SW_SHOWNORMAL);
//						break;
//					}
//				}
//			}
//			ClickLink.Invoke((Object*)this, e);
//		}
//	}
//	RichTextBox::OnChildNotify(e);
//}
///////////////////////////////////////////////////////////////////////////////

char* VersionInfoConfig::sURL1 = "http://www.uuware.com/";
char* VersionInfoConfig::sURL2 = "http://uuware.googlepages.com/";
char* VersionInfoConfig::sURL3 = "http://uuware.spaces.live.com/";
char* VersionInfoConfig::sDefaultHtml = "<html><body><div style='text-align:center;font-size:20px;'><a href=\"http://www.uuware.com/\">uuware.com</a></div><br><div style='text-align:right;'><font size=\"3\">(c)Copyright uuware 2007-2016. All rights reserved.</font></div></body></html>";
char* VersionInfoConfig::sURLFlag = "[#VERSION_URL:%s%s#]";
char* VersionInfoConfig::sSelfFlag = "[#VERSION_SELF#]";
int VersionInfoConfig::nSkipHour = 12;
#define CC_VERSIONINFO_WAITREMOVE  (5 * 60 * 1000)

//===========================================================cc::win::VersionInfo
///////////////////////////////////////////////////////////////////////////////
// constructors/destructor
VersionInfo::VersionInfo()
{
	winInet = NULL;
	nStep = 0;
	nReDirectTry = 0;
	strURLNext.ToMByte();
	strURLEnd.ToMByte();
	strURLCountAll.ToMByte();
	strURLCountOne.ToMByte();

	btnReShow = new Button();
	btnReShow->SetName(_T("btnReShow"));
	btnReShow->SetLocation(3, 3);
	btnReShow->SetSize(120, 19);
	btnReShow->Click += EventHandler((Object*)this, (EventFun)&VersionInfo::Control_Click);
	this->AddControl(btnReShow);

	chkAutoRemove = new CheckBox();
	chkAutoRemove->SetName(_T("chkRemove"));
	chkAutoRemove->SetLocationOffset(btnReShow, false, 2, 0);
	chkAutoRemove->SetChecked(CC_APP_GetIni()->GetBool(_T("Main.VRemove"), true));
	chkAutoRemove->SetStyle(StyleType::Outer, 0, OuterStyle::AutoSize);
	chkAutoRemove->Click += EventHandler((Object*)this, (EventFun)&VersionInfo::Control_Click);
	this->AddControl(chkAutoRemove);

	html = new HtmlTextBox();
	html->SetName(_T("CR"));
	html->SetMultiline(true);
	html->SetLocation(2, 24);
	html->SetSize(600 - 2, 600 - 24 - 2);
	html->SetReadOnly(true);
	html->SetStyle(StyleType::ExStyle, WS_EX_CLIENTEDGE, WS_EX_TRANSPARENT);
	html->SetOuterStyle(OuterStyle::TransparentHollowBrush, true);
	//html->SetText((TCHAR*)CC_APP_GetLang()->Get(_T("Lang.VerInfo.Init"), cc::Str(_T("Version info."))).GetBuf());
	html->SetText(_T(""));
	html->Anchor = (AnchorStyles::Type)(AnchorStyles::Top | AnchorStyles::Left | AnchorStyles::Right | AnchorStyles::Bottom);
	this->AddControl(html);

	this->SetName(_T("VerInfo"));
	this->SetStyle(StyleType::Outer, 0, OuterStyle::AutoConfig);
	this->SetClientSize(600, 600);
	//this->SetOuterStyle(OuterStyle::Blank, true);
	SetLang(*CC_APP_GetLang(), cc::Str(CC_CONFIG_LANGSECTION));
}

///////////////////// destructor //////////////////////////
VersionInfo::~VersionInfo()
{
	::KillTimer(m_hWnd, CC_TIMERID_VERSION);
	nStep = CC_VERINFO_STEP_END + 1;
	if(winInet != NULL)
	{
		winInet->Close(true, true, true);
		delete winInet;
		winInet = NULL;
	}
}
// constructors/destructor

void VersionInfo::OnCreateControl()
{
	//html->SetSize(_OrgRect.right - _OrgRect.left - 3, _OrgRect.bottom - _OrgRect.top - 26);
	GroupPanel::OnCreateControl();

	int tOld = CC_APP_GetIni()->Get(_T("Main.VInfo"), 0);
	int tNow = (int)(::time(NULL) - 1190000000);
	if(tNow > tOld && tNow - tOld < VersionInfoConfig::nSkipHour * 3600)
	{
		CC_TRACE((_T("VersionInfo::OnCreateControl, tOld:[%d], tNew:[%d], not Show VersionInfo."), tOld, tNow));
		return;
	}
	CC_TRACE((_T("VersionInfo::OnCreateControl, tOld:[%d], tNew:[%d], Show VersionInfo."), tOld, tNow));
	CC_APP_GetIni()->Set(_T("Main.VInfo"), tNow);
#ifdef _DEBUG_
	btnReShow->SetEnabled(true);
#else
	btnReShow->SetEnabled(false);
	::SetTimer(m_hWnd, CC_TIMERID_VERSION, 1000 * 45, NULL);
#endif
}

void VersionInfo::DoSetLang(KeyArr& keyArr, cc::Str sThisHead)
{
	this->SetText((TCHAR*)keyArr.Get(_T("Lang.VerInfo"), cc::Str(_T("Version Info"))).GetBuf());
	chkAutoRemove->SetText((TCHAR*)keyArr.Get(_T("Lang.VerInfo.Remove"), cc::Str(_T("Remove Version Info after loaded minutes."))).GetBuf());
	btnReShow->SetText((TCHAR*)keyArr.Get(_T("Lang.VerInfo.Show"), cc::Str(_T("Show"))).GetBuf());
}

void VersionInfo::OnInnerTimer(EventArgs *e)
{
	//e->WParam is ID
	if(e->WParam != CC_TIMERID_VERSION)
	{
		GroupPanel::OnInnerTimer(e);
		return;
	}
	::KillTimer(m_hWnd, CC_TIMERID_VERSION);

	if(strURLNext.GetLength() > 0)
	{
		CC_TRACE(("VersionInfo, strURLNext:%s.", (const char*)strURLNext.GetBuf()));
		winInet->BeginGetHTMLFullUrl((const char*)strURLNext.GetBuf(), NULL, true, true, NULL);
		return;
	}

	if(nStep == 1 && cc::Str::Len(VersionInfoConfig::sURL2) == 0)
	{
		nStep++;
	}
	if(nStep == 2 && cc::Str::Len(VersionInfoConfig::sURL3) == 0)
	{
		nStep++;
	}
	nReDirectTry = 0;
	CC_TRACE((_T("VersionInfo, nStep:%d."), nStep));
	if(nStep <= 0)
	{
		if(winInet == NULL)
		{
			winInet = new WinInet();
			winInet->SetAgent(_T("Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.0; .NET CLR 1.1.4322)"));
			//winInet->SetAgent(App::GetNameVersion());
			//WinInetConfig::nReadBufferSize = 10240;
			//WinInetConfig::nReadLength = 128;
			winInet->ReceiveEnd += EventHandler((Object*)this, (EventFun)&VersionInfo::Control_Click);
		}
		winInet->BeginGetHTMLFullUrl(VersionInfoConfig::sURL1, NULL, true, true, NULL);
	}
	else if(nStep == 1)
	{
		winInet->BeginGetHTMLFullUrl(VersionInfoConfig::sURL2, NULL, true, true, NULL);
	}
	else if(nStep == 2)
	{
		winInet->BeginGetHTMLFullUrl(VersionInfoConfig::sURL3, NULL, true, true, NULL);
	}
	else if(nStep < CC_VERINFO_STEP_COUNT)
	{
		html->LoadHtml(VersionInfoConfig::sDefaultHtml);
		nStep = CC_VERINFO_STEP_END;
		//start timer
		::SetTimer(m_hWnd, CC_TIMERID_VERSION, CC_VERSIONINFO_WAITREMOVE, NULL);
	}
	//char* c = "<!-- comment --><html><body>begin body<p>bbb<p/>ccc<b>dd<i>de</i>ee</b>fff<u>ggg<s>hhh</s>iii</u>jjjkkk<div align=center style='color:#F000FF;font-size:10px '>pre comment<!---->end comment<a href=\"http://www.uuware.com/\" target=\"_blank\"><font size=\"15\">uuware.com</font></a></div><div><font color=    \"#FF0FF0\"    size  =\"12px\">(c)Copyright uuware 2007. All rights reserved.</font></div><p/><span style=\"color:#00FFFF;font-size:13;font-family:arial;font-weight:bold;background: #050505;  font: 1em Georgia, Palatino, Serif;  \"><font   size   =   \"12\" color=\"#00F00F\"  >Red</font></span><a href=\"http://uuware22.com/\">uuware.com</a><div style=\"color:#FF000F;background:#0808AA;font-size:11px;font-family:arial;font-weight:bold;background: #f5f5f5;\"><span style=\"color:#00FFFF;font-size:13;font-family:arial;font-weight:bold;background: #f5f5f5;  font: ;  \">&nbsp;R&nbsp;&lt;e&gt;&nbsp;d&nbsp;</span>abc<a href=\"http://www.uuware333.com/\">uuware.com</a>de <span style=\"color:#0000FF;background:#0808AA;font-size:12px;background: #00f50a;\">before font<font size=\"8\" color=\"#FF00FF\"  >&nbsp;R&nbsp;&lt;e&gt;&nbsp;d&nbsp;</font>end font</span></div><div><span>fghend font</span></div>end <center>of</center> body<br><br><br></body></html>";
	//html->LoadHtml(c);
	//html->SetReadOnly(!html->GetReadOnly());
	//cc::Str buf = cc::Str(LangCode::SYSTEM);
	//html->StreamOut(SF_RTF, buf);
	//CC_TRACE(((char*)buf.GetBuf()));

	else if(nStep == CC_VERINFO_STEP_COUNT)
	{
		if(strURLCountOne.GetLength() > 0)
		{
			//<!-- [#VERSION_URL:ACCESSCOUNT#][#BREAK#]http://www.uuware.com/?{W:[#C:W#]}{H:[#C:H#]}{LANG:[#C:LANG#]}{L2:[#C:L2#]}{PROJ:[#C:PROJ#]}{VER:[#C:VER#]}{REF:[#C:REF#]}{URL:[#C:URL#]}[#BREAK#][#VERSION_URL:ACCESSCOUNT#] -->
			CC_TRACE(("VersionInfo, orignal count URL:[%s].", (const char*)strURLCountOne.GetBuf()));
			CC_ASSERT(strURLEnd.GetLength() > 0);
			cc::Str sBuf = cc::Str(LangCode::SYSTEM);
			sBuf.Format("%d", (LONG)::GetSystemMetrics(SM_CXSCREEN));
			strURLCountOne.Replace("[#C:W#]", (const char*)sBuf.GetBuf());
			sBuf.Format("%d", (LONG)::GetSystemMetrics(SM_CYSCREEN));
			strURLCountOne.Replace("[#C:H#]", (const char*)sBuf.GetBuf());
			sBuf.Format("%d", cc::Util::GetCurrentCodePage());
			strURLCountOne.Replace("[#C:LANG#]", (const char*)sBuf.GetBuf());

			sBuf.ValueOf(CC_APP_GetIni()->Get(CC_CONFIG_LANGFILE));
			sBuf.Replace("=", "-").Replace("&", "-").Replace("%", "-").Replace(".", "-").Replace(" ", "-");
			//sBuf.Format("%d", CC_APP_GetLang()->Get(_T("Lang.LangCode"), 0));
			strURLCountOne.Replace("[#C:L2#]", (const char*)sBuf.GetBuf());

			sBuf.ValueOf(App::GetName());
			strURLCountOne.Replace("[#C:PROJ#]", (const char*)sBuf.GetBuf());
			sBuf.ValueOf(App::GetVersion());
			strURLCountOne.Replace("[#C:VER#]", (const char*)sBuf.GetBuf());
			strURLCountOne.Replace("[#C:REF#]", (const char*)strURLEnd.GetBuf());

			CC_TRACE(("VersionInfo, strURLEnd:[%s].", (const char*)strURLEnd.GetBuf()));
			CC_TRACE(("VersionInfo, strURLCountOne:[%s].", (const char*)strURLCountOne.GetBuf()));

			char* output = NULL;
			cc::Util::codeURLEncode((const char*)strURLEnd.GetBuf(), strURLEnd.GetLength(), &output);
			//release output in sBuf
			//cc::Log::debug("encoded url:[%s]", output);
			sBuf.ValueOf(output);
			delete output;
			output = NULL;
			sBuf.Replace("?", "%3F").Replace(":", "%3A").Replace("&", "%26");
			strURLCountOne.Replace("[#C:URL#]", (const char*)sBuf.GetBuf());
#if _DEBUG_
			cc::Log::debug("replaced url:[%s]", (const char*)sBuf.GetBuf());
			cc::Log::debug("end url:[%s]", (const char*)strURLCountOne.GetBuf());
#endif
			//Host: www.colossal.jp\r\nAccept-Language: ja\r\nAccept-Encoding: gzip, deflate\r\n
			sBuf.Format("Accept: *.*\r\nReferer: %s\r\n", (const char*)strURLEnd.GetBuf());
			winInet->BeginGetHTMLFullUrl((const char*)strURLCountOne.GetBuf(), NULL, true, true, (const char*)sBuf.GetBuf());
		}
		else
		{
			nStep = CC_VERINFO_STEP_END;
			//start timer
			::SetTimer(m_hWnd, CC_TIMERID_VERSION, CC_VERSIONINFO_WAITREMOVE, NULL);
		}
	}
	else if(nStep >= CC_VERINFO_STEP_END)
	{
		if(chkAutoRemove->GetChecked())
		{
			btnReShow->SetEnabled(true);
			html->SetText(_T(""));
			if(winInet != NULL)
			{
				winInet->Close(true, true, true);
				delete winInet;
				winInet = NULL;
			}
		}
	}
}

//cannot call BeginGetHTML in this
void VersionInfo::Control_Click(Object* sender, EventArgs* e)
{
	CC_LOGTIME_START();
	if(sender == btnReShow)
	{
		btnReShow->SetEnabled(false);
		//from 0 for language changed by user
		nStep = 0;
		//start timer
		::SetTimer(m_hWnd, CC_TIMERID_VERSION, 1000, NULL);
	}
	else if(sender == chkAutoRemove)
	{
		CC_APP_GetIni()->SetBool(_T("Main.VRemove"), chkAutoRemove->GetChecked());
		if(chkAutoRemove->GetChecked() && nStep >= CC_VERINFO_STEP_END)
		{
			nStep = CC_VERINFO_STEP_END;
			//start timer
			::SetTimer(m_hWnd, CC_TIMERID_VERSION, 1000, NULL);
		}
	}
	else if(sender == winInet)
	{
		strURLNext.Clear();
		if(nStep > CC_VERINFO_STEP_END)
		{
			return;
		}

		int nRet = winInet->GetResult();
		int nRCode = winInet->GetResponseCode();
#if _DEBUG_
		//!!!notice,cannot direct use (char*)winInet->GetResponseText().GetBuf() for cc::Log::debug
		cc::Str sTmpHead = cc::Str(LangCode::UNICODE16, winInet->GetResponseHeaders());
		cc::Str sTmpHtml = cc::Str(LangCode::UNICODE16, winInet->GetResponseText());
		cc::Log::debug(_T("VersionInfo, nRet:%d, ResponseCode:%d."), nRet, nRCode);
		CC_TRACE((L"VersionInfo, Head:[%s]", (const wchar_t*)sTmpHead.GetBuf()));
		CC_TRACE((L"VersionInfo, Body:[%s]", (const wchar_t*)sTmpHtml.GetBuf()));
#endif
		cc::Str id = cc::Str(LangCode::SYSTEM);
		int nPos = -1;
		if(nRet == 0 && (nRCode >= 300 && nRCode <= 399))
		{
			cc::Str sLocation = winInet->GetResponseHeader("Location", LangCode::SYSTEM);
			nReDirectTry++;
			if(nReDirectTry <= CC_VERINFO_MAXREDIRECT)
			{
				strURLNext = sLocation;
			}
			if(strURLNext.GetLength() > 9)
			{
				//start timer
				::SetTimer(m_hWnd, CC_TIMERID_VERSION, 1000, NULL);
				return;
			}
			else
			{
				strURLNext.Clear();
			}
			if(nStep < CC_VERINFO_STEP_COUNT)
			{
				//treate next url
				nStep++;
				//start timer
				::SetTimer(m_hWnd, CC_TIMERID_VERSION, 1000, NULL);
				return;
			}
		}

		if(nStep == CC_VERINFO_STEP_COUNT)
		{
			id.ValueOf("[#BREAK#]");
			if((nPos = GetBestUrl(strURLCountOne, strURLCountAll, id, false)) > 0)
			{
				strURLCountAll.Remove(0, nPos);
				//get next url's page
				//start timer
				::SetTimer(m_hWnd, CC_TIMERID_VERSION, 1000, NULL);
			}
			else
			{
				nStep = CC_VERINFO_STEP_END;
				//now is ended
				//start timer
				::SetTimer(m_hWnd, CC_TIMERID_VERSION, CC_VERSIONINFO_WAITREMOVE, NULL);
			}
			return;
		}

		bool isOK = false;
		const cc::Str& str = winInet->GetResponseText();
		//if(nRet == 0)
		if(nStep >= 3)
		{
			//not go here?
			nStep++;
			//start timer
			::SetTimer(m_hWnd, CC_TIMERID_VERSION, 1000, NULL);
		}
		else
		{
			if((nPos = str.IndexOf(VersionInfoConfig::sSelfFlag)) >= 0)
			{
				//this is target page
				html->LoadHtml((const char*)str.GetBuf(), str.GetLength());

				//only count once
				if(strURLCountAll.GetLength() <= 0)
				{
					//"[#VERSION_URL:ACCESSCOUNT#]"
					id.Format(VersionInfoConfig::sURLFlag, "ACCESSCOUNT", "");
					if(GetBestUrl(strURLCountAll, str, id, false) > 0)
					{
						id.ValueOf("[#BREAK#]");
						if((nPos = GetBestUrl(strURLCountOne, strURLCountAll, id, false)) > 0)
						{
							strURLCountAll.Remove(0, nPos);
							nStep = CC_VERINFO_STEP_COUNT;
							//start timer
							::SetTimer(m_hWnd, CC_TIMERID_VERSION, 1000, NULL);
							return;
						}
					}
				}
				nStep = CC_VERINFO_STEP_END;
				//start timer
				::SetTimer(m_hWnd, CC_TIMERID_VERSION, CC_VERSIONINFO_WAITREMOVE, NULL);
			}
			else
			{
				//search redirect info
				cc::Str appName = cc::Str(LangCode::SYSTEM, App::GetName());
				cc::Str defLang = cc::Str(LangCode::SYSTEM, CC_APP_GetIni()->Get(CC_CONFIG_LANGFILE));
				defLang.Replace("=", "-").Replace("&", "-").Replace("%", "-").Replace(".", "-").Replace(" ", "-");
				//"[#VERSION_URL:AppNameLangName#]"
				id.Format(VersionInfoConfig::sURLFlag, (const char*)appName.GetBuf(), (const char*)defLang.GetBuf());
				if(GetBestUrl(strURLNext, str, id, true) <= 0)
				{

				//"[#VERSION_URL:AppNameLangName(sys)#]"
				cc::Str defLang2 = cc::Str(LangCode::SYSTEM, cc::Util::GetCurrentLocaleName(true));
				defLang2.Replace("=", "-").Replace("&", "-").Replace("%", "-").Replace(".", "-").Replace(" ", "-");
				id.Format(VersionInfoConfig::sURLFlag, (const char*)appName.GetBuf(), (const char*)defLang2.GetBuf());
				if(GetBestUrl(strURLNext, str, id, true) <= 0)
				{

					//"[#VERSION_URL:LangName#]"
					id.Format(VersionInfoConfig::sURLFlag, "", (const char*)defLang.GetBuf());
					if(GetBestUrl(strURLNext, str, id, true) <= 0)
					{

					//"[#VERSION_URL:LangName(sys)#]"
					id.Format(VersionInfoConfig::sURLFlag, "", (const char*)defLang2.GetBuf());
					if(GetBestUrl(strURLNext, str, id, true) <= 0)
					{
						//"[#VERSION_URL:AppNameen-US#]", en-US  English (United States)
						id.Format(VersionInfoConfig::sURLFlag, (const char*)appName.GetBuf(), "en-US");
						if(GetBestUrl(strURLNext, str, id, true) <= 0)
						{
							//"[#VERSION_URL:en-US#]", 1252  English (United States)
							id.Format(VersionInfoConfig::sURLFlag, "", "en-US");
							if(GetBestUrl(strURLNext, str, id, true) <= 0)
							{
								//try next url
								nStep++;
								//start timer
								::SetTimer(m_hWnd, CC_TIMERID_VERSION, 1000, NULL);
							}
						}
					}

					}

				}

				}

				//get target page
				strURLEnd = strURLNext;
				//start timer
				::SetTimer(m_hWnd, CC_TIMERID_VERSION, 1000, NULL);
			}
		}
	}
	CC_LOGTIME_PRINT();
	//COleDataSource src;
	//STGMEDIUM sm;
	//sm.tymed = TYMED_GDI;
	//sm.hBitmap = hbmp;
	//sm.pUnkForRelease = NULL;
	//src.CacheData(CF_BITMAP, &sm);
	//LPDATAOBJECT lpDataObject = (LPDATAOBJECT)src.GetInterface(&IID_IDataObject);
	//pRichEditOle->ImportDataObject(lpDataObject, 0, NULL);
	//lpDataObject->Release();
}

int VersionInfo::GetBestUrl(cc::Str& sLocation, const cc::Str& str, const cc::Str& id, bool isAddExt)
{
	CC_TRACE(("VersionInfo::GetBestUrl, id:%s, org url:%s", (const char*)id.GetBuf(), (const char*)str.GetBuf()));
	int nPos = str.IndexOf((const char*)id.GetBuf(), true);
	if(nPos >= 0)
	{
		nPos += id.GetLength();
		int nPos2 = str.IndexOf((const char*)id.GetBuf(), nPos, true);
		if(nPos2 > nPos)
		{
			//find target page url
			cc::Str strURL = str.Substring(nPos, nPos2);
			if(isAddExt)
			{
				if(strURL.IndexOf("?") < 0)
				{
					strURL.Append("?");
				}
				else
				{
					strURL.Append("&");
				}
				cc::Str defLang = cc::Str(LangCode::SYSTEM, CC_APP_GetIni()->Get(CC_CONFIG_LANGFILE));
				defLang.Replace("=", "-").Replace("&", "-").Replace("%", "-").Replace(".", "-").Replace(" ", "-");
				//int lang = CC_APP_GetLang()->Get(_T("Lang.LangCode"), 0);
				strURL.AppendFormat("lang=%d&l2=%s&w=%d&h=%d&proj=", cc::Util::GetCurrentCodePage(), (const char*)defLang.GetBuf(), (LONG)::GetSystemMetrics(SM_CXSCREEN), (LONG)::GetSystemMetrics(SM_CYSCREEN)).Append(App::GetName()).Append("&ver=").Append(App::GetVersion());
			}
			sLocation.TakeoverPointer(strURL);
#if _DEBUG_
			cc::Log::debug("VersionInfo::GetBestUrl, result URL:%s.", (char*)sLocation.GetBuf());
#endif
			return nPos2;
		}
	}
	return -1;
}

//===========================================================cc::win::PathButton
///////////////////////////////////////////////////////////////////////////////
// constructors/destructor
PathButton::PathButton()
{
	PathControl = NULL;
	tcCaption = NULL;
	tcFilter = NULL;
	tcDefaultExt = NULL;
	IsFile = true;
	IsSaveAs = false;
	IsOpenMuitl = false;

	this->SetText(_T("..."));
	this->SetName(_T("PathButton"));
	this->SetSize(14, 19);
}

///////////////////// destructor //////////////////////////
PathButton::~PathButton()
{
}
// constructors/destructor

void PathButton::OnClick(EventArgs* e)
{
	HWND hWnd = m_hWnd;
	cc::Str sTemp = SelectedPath;
	if(PathControl != NULL)
	{
		sTemp = PathControl->GetText();
		hWnd = PathControl->m_hWnd;
	}
	if(sTemp.GetLength() < 1)
	{
		sTemp = cc::Util::getStartupPath(true);
	}
	else
	{
		sTemp = cc::Util::getStartupRelatePath(sTemp);
	}
	SelectedPath.Clear();
	if(IsFile)
	{
		//if(!cc::Util::fExist(tcTxt))
		//{
		//	tcTxt = _T("");
		//}
		if(IsSaveAs)
		{
			SelectedPath = cc::Util::selectSaveAs(sTemp, tcFilter, tcDefaultExt, hWnd, tcCaption);
		}
		else
		{
			SelectedPath = cc::Util::selectOpenFile(sTemp, tcFilter, tcDefaultExt, hWnd, tcCaption, IsOpenMuitl);
		}
	}
	else
	{
		if(!cc::Util::dExist(sTemp))
		{
			sTemp = cc::Util::getParentDirName(sTemp);
		}
		SelectedPath = cc::Util::selectDir(sTemp, hWnd, tcCaption);
	}
	CC_TRACE((_T("PathButton::OnClick, SelectedPath:[%s]"), (TCHAR*)SelectedPath.GetBuf()));
	if(PathControl != NULL && SelectedPath.GetLength() > 0 && !SelectedPath.Equals(_T("\\")))
	{
		PathControl->SetText((TCHAR*)SelectedPath.GetBuf());
	}
	Button::OnClick(e);
}


//===========================================================cc::win::WinTool
///////////////////////////////////////////////////////////////////////////////
//add lang item(res and files) to ComboBox
int WinUtil::AddLangItems(ComboBox* ctl, bool searchRes, bool searchFile)
{
	CC_ASSERT(ctl != NULL && ::IsWindow(ctl->m_hWnd));
	ctl->ItemClear();

	int nInd = 0;
	cc::Str defLang = CC_APP_GetIni()->Get(CC_CONFIG_LANGFILE);
	//get lang from res
	if(searchRes)
	{
		cc::Map<cc::Str, cc::Str> arrRes;
		cc::Util::readResNames(arrRes, CC_CONFIG_LANGTYPE);
		for(int n = 0; n < arrRes.GetSize(); n++)
		{
			nInd = ctl->ItemAdd((const TCHAR*)arrRes.GetKey(n).GetBuf());
			if(defLang.Equals(arrRes.GetKey(n), true))
			{
				ctl->SetSelectedIndex(nInd);
			}
		}
	}

	//get lang from files
	if(searchFile)
	{
		cc::Str langPath = cc::Util::getStartupRelatePath(CC_APP_GetConfig()->GetLangPath());
#ifdef _DEBUG_
		if(!cc::Util::dExist(langPath))
		{
			langPath = cc::Util::getStartupPath(true).Append(_T("..\\")).Append(CC_APP_GetConfig()->GetLangPath());
		}
#endif
		if(langPath.EndsWith(_T("\\")) || langPath.EndsWith(_T("/")) || !langPath.EndsWith(CC_CONFIG_LANGEXT))
		{
			langPath.Append(_T("*"));
			langPath.Append(CC_CONFIG_LANGEXT);
		}
		//get all files from langdir
		cc::List<cc::Str> arrFileName;
		cc::Util::getFiles(arrFileName, (TCHAR*)langPath.GetBuf());
		for(int i = arrFileName.GetSize() - 1; i >= 0; i--)
		{
			//only read first 128byte
			cc::Str sFile = arrFileName.GetKey(i);
			cc::Str sOneIni = cc::Util::readOneIni(CC_CONFIG_LANGSECTION, CC_CONFIG_LANGNAME, (TCHAR*)sFile.GetBuf(), LangCode::UTF8, true, 128);
			if(sOneIni.GetLength() > 0)
			{
				sFile = cc::Util::getFileName((TCHAR*)sFile.GetBuf());
				sOneIni.Append(_T(":")).Append(sFile);
				nInd = ctl->ItemAdd((const TCHAR*)sOneIni.GetBuf());
				if(sFile.Equals(defLang))
				{
					ctl->SetSelectedIndex(nInd);
				}
			}
		}
	}
	return ctl->ItemCount();
}

//get lang item from ComboBox and set to CC_APP_GetIni()
bool WinUtil::SetLangToConfig(ComboBox* ctl)
{
	CC_ASSERT(ctl != NULL && ::IsWindow(ctl->m_hWnd));

	int n = ctl->GetSelectedIndex();
	if(n >= 0)
	{
		cc::Str str = cc::Str(ctl->ItemGet(n));
		int nPos = str.LastIndexOf(_T(":"));
		if(nPos > 0)
		{
			str = str.Substring(nPos + 1);
		}
		if(str.GetLength() > 0)
		{
			CC_APP_GetIni()->Set(CC_CONFIG_LANGFILE, (const TCHAR*)str.GetBuf());
			return true;
		}
	}
	return false;
}

bool WinUtil::PrintHtml(cc::Str& sHtml, Form* parent)
{
	Dialog dialog;
	dialog.SetSize(600, 480);
	IEControl* ie1 = new IEControl();
	ie1->SetName(_T("ie1"));
	ie1->SetLocation(0, 0);
	ie1->SetSize(600, 480);
	ie1->Anchor = (AnchorStyles::Type)(AnchorStyles::Top | AnchorStyles::Left | AnchorStyles::Right | AnchorStyles::Bottom);
	dialog.AddControl(ie1);
	dialog.SetParent(parent, true);
	dialog.SetCenter();
	dialog.SetText(_T("PrewView"));
	dialog.Show();

	//ie1->LoadText((wchar_t*)sHtml.GetBuf());
	BSTR bstrCode = ::SysAllocString((wchar_t*)sHtml.GetBuf());
	ie1->Write(bstrCode);
	::SysFreeString(bstrCode);
	ie1->GetWebBrowser()->ExecWB((ie_OLECMDID)7, (ie_OLECMDEXECOPT)1, NULL, NULL);

	dialog.ShowDialog();
	return true;
}

#define REG_REGDATAKEY _T("data")
#define REG_KEYS "[#KEYSTART#]"
#define REG_KEYE "[#KEYEND#]"
#define REG_EXPIRED_KEY _T("SYSTEM\\Setup\\UW")
bool WinUtil::RegistSave(const TCHAR* sPrgRegDir, const char* sREG_KEY1, const char* sREG_KEY2)
{
	bool bRet = false;
	cc::Str tkey = _T("Lang");
	tkey.Append(_T(".r."));
	cc::Str sUserData = CC_APP_GetLang()->Get(tkey + _T("d"));
	if(sUserData.GetLength() > 0 && CC_APP_GetLang()->Exist(tkey + _T("d")) && CC_APP_GetLang()->Exist(tkey + _T("k")))
	{
		//notice here is UTF8
		cc::Str sUserKey = CC_APP_GetLang()->Get(tkey + _T("k"));
		char* output1 = NULL;
		char* output2 = NULL;
		int nLen1 = cc::Util::codeB64Decode((char*)sUserKey.GetBuf(), (unsigned int)sUserKey.GetLength(), &output1);
		int nLen2 = cc::Util::codeB64Decode((char*)sUserData.GetBuf(), (unsigned int)sUserData.GetLength(), &output2);
		if(nLen1 > 0 && nLen2 > 0)
		{
			//notice,crypt it for loop at least 8 and 4
			cc::Util::teaDeCrypt(output1, nLen1, (char*)sREG_KEY2, (int)strlen(sREG_KEY2), 8);
			cc::Util::teaDeCrypt(output2, nLen2, output1, nLen1, 4);

			cc::Str sTmpData = cc::Str(LangCode::UTF8, output2, LangCode::UTF8);
			cc::Str sCode1 = cc::Util::GetCpuId();
			cc::Str sCode2 = cc::Util::GetDiskSize(0, true);
			//TODO:changed after format disk? OK for here
			cc::Str sCode3 = cc::Util::GetDriveSerialNumber();
			cc::Str sCode4 = cc::Util::GetEnv(_T("USERDNSDOMAIN"));
			cc::Str sCode5 = cc::Util::GetEnv(_T("USERDOMAIN"));
			cc::Str sCode6 = cc::Util::GetEnv(_T("USERNAME"));
			cc::Str sCode7 = cc::Util::GetEnv(_T("COMPUTERNAME"));
			sTmpData.Append("\nU1=").Append(sCode1).Append("\nU2=").Append(sCode2).Append("\nU3=").Append(sCode3);
			sTmpData.Append("\nU4=").Append(sCode4).Append("\nU5=").Append(sCode5).Append("\nU6=").Append(sCode6);
			sTmpData.Append("\nU7=").Append(sCode7);
#if _DEBUG_
			cc::Log::debug("teaDeCrypt, len:%d, key:[%s]", nLen1, (char*)output1);
			cc::Log::debug(_T("teaDeCrypt, len:%d, data:[%s]"), sTmpData.GetLength(), (char*)sTmpData.GetBuf());
#endif
			//notice the orignal data is LangCode::UTF8
			cc::Util::teaEnCrypt((char*)sTmpData.GetBuf(), sTmpData.GetLength(), (char*)sREG_KEY1, (int)strlen(sREG_KEY1), 4);
			nLen2 = cc::Util::codeB64Encode((char*)sTmpData.GetBuf(), (unsigned int)sTmpData.GetLength(), &output1);

			cc::RegistryKey* regKey = cc::Registry::GetLocalMachine();
			if(!regKey->OpenSubKey(sPrgRegDir, true))
			{
				regKey->CreateSubKey(sPrgRegDir);
			}
			cc::Str sEndData = cc::Str(LangCode::TCHAR, output1);
			//notice Save Data is TCHAR
			regKey->SetValue(REG_REGDATAKEY, (TCHAR*)sEndData.GetBuf());
			delete regKey;
			bRet = true;
		}
		if(output1 != NULL)
		{
			delete output1;
		}
		if(output2 != NULL)
		{
			delete output2;
		}
	}
	return bRet;
}
//get crypt string from regist,or value of sKey
//notice:sKey must be \nN= or \nE= or \nW= or \nK?(1-4)=
bool WinUtil::RegistLoad(const TCHAR* sPrgRegDir, const char* sREG_KEY1, cc::Str& sRegCryptData, const char* sKey, cc::Str& sKeyValue, const char* sKey2, cc::Str& sKeyValue2)
{
	CC_ASSERT(sREG_KEY1 != NULL);
	bool bRet = false;
	cc::RegistryKey* regKey = cc::Registry::GetLocalMachine();
	if(regKey->OpenSubKey(sPrgRegDir, false))
	{
		//notice Save Data is TCHAR,but need turn to char to used by codeB64Decode
		cc::Str sRegData = cc::Str(LangCode::SYSTEM, regKey->GetValue(REG_REGDATAKEY));
#if _DEBUG_
		cc::Log::debug("WinUtil::RegistLoad, len:%d, sRegData:[%s]", sRegData.GetLength(), (char*)sRegData.GetBuf());
#endif
		char* output1 = NULL;
		int nLen1 = cc::Util::codeB64Decode((char*)sRegData.GetBuf(), (unsigned int)sRegData.GetLength(), &output1);
		if(nLen1 >= 11)
		{
			cc::Util::teaDeCrypt(output1, nLen1, (char*)sREG_KEY1, (int)strlen(sREG_KEY1), 4);
			//notice the orignal data is LangCode::UTF8
			//sRegReturnData.TakeoverPointer(&output1, LangCode::UTF8);
			//cc::Util::GetHtmlTag(output1, 0, "\nN=", -1, "\n", 1, sRegReturnData, NULL, -1, LangCode::UTF8);
			//[RegKey]\nN=
			cc::Str tmp = cc::Str(LangCode::SYSTEM, "[R");
			tmp.Append("egK").Append("ey").Append("]\nN=");
			if(output1 != NULL && strncmp(output1, (char*)tmp, 11) == 0)
			{
				cc::Str sValTmp = cc::Str(LangCode::UTF8);
				int nRand = cc::Util::GetTodayInt();
				if(nRand % 3 == 0)
				{
					char k1[] = "\nU1=";
					cc::Str sCode1 = cc::Str(LangCode::UTF8, cc::Util::GetCpuId());
					cc::Util::GetHtmlTag(output1, 0, k1, (int)strlen(k1), "\n", 1, sValTmp, NULL, -1, LangCode::UTF8);
					if(!sValTmp.Equals(sCode1))
					{
						sKey = NULL;
						sKey2 = NULL;
					}
				}
				else if(nRand % 3 == 1)
				{
					char k1[] = "\nU2=";
					cc::Str sCode2 = cc::Str(LangCode::UTF8, cc::Util::GetDiskSize(0, true));
					cc::Util::GetHtmlTag(output1, 0, k1, (int)strlen(k1), "\n", 1, sValTmp, NULL, -1, LangCode::UTF8);
					if(!sValTmp.Equals(sCode2))
					{
						sKey = NULL;
						sKey2 = NULL;
					}
				}
				else if(nRand % 3 == 2)
				{
					char k1[] = "\nU3=";
					//TODO:changed after format disk? OK for here
					cc::Str sCode3 = cc::Str(LangCode::UTF8, cc::Util::GetDriveSerialNumber());
					cc::Util::GetHtmlTag(output1, 0, k1, (int)strlen(k1), "\n", 1, sValTmp, NULL, -1, LangCode::UTF8);
					if(!sValTmp.Equals(sCode3))
					{
						sKey = NULL;
						sKey2 = NULL;
					}
				}

				bRet = true;
				sRegCryptData.TakeoverPointer(sRegData);
				if(sKey != NULL && sValTmp.GetLength() > 0)
				{
					cc::Util::GetHtmlTag(output1, 0, sKey, (int)strlen(sKey), "\n", 1, sKeyValue, NULL, -1, LangCode::UTF8);
				}
				if(sKey2 != NULL && sValTmp.GetLength() > 0)
				{
					cc::Util::GetHtmlTag(output1, 0, sKey2, (int)strlen(sKey2), "\n", 1, sKeyValue2, NULL, -1, LangCode::UTF8);
				}
			}
		}
		if(output1 != NULL)
		{
			delete output1;
		}
	}
	delete regKey;
	return bRet;
}
//notice:sKey must be \nN= or \nE= or \nW= or \nK?(1-4)=
bool WinUtil::RegistGetValue(const char* sREG_KEY1, const cc::Str& sRegCryptData, const char* sKey, cc::Str& sKeyValue, const char* sKey2, cc::Str& sKeyValue2)
{
	CC_ASSERT(sREG_KEY1 != NULL);
	CC_ASSERT(sKey != NULL);
	bool bRet = false;
	char* output1 = NULL;
	int nLen1 = cc::Util::codeB64Decode((char*)sRegCryptData.GetBuf(), (unsigned int)sRegCryptData.GetLength(), &output1);
	if(nLen1 >= 11)
	{
		cc::Util::teaDeCrypt(output1, nLen1, (char*)sREG_KEY1, (int)strlen(sREG_KEY1), 4);
		//[RegKey]\nN=
		cc::Str tmp = cc::Str(LangCode::SYSTEM, "[R");
		tmp.Append("egK").Append("ey").Append("]\nN=");
		if(strncmp(output1, (char*)tmp, 11) == 0)
		{
			bRet = true;
			cc::Util::GetHtmlTag(output1, 0, sKey, (int)strlen(sKey), "\n", 1, sKeyValue, NULL, -1, LangCode::UTF8);
			if(sKey2 != NULL)
			{
				cc::Util::GetHtmlTag(output1, 0, sKey2, (int)strlen(sKey2), "\n", 1, sKeyValue2, NULL, -1, LangCode::UTF8);
			}
		}
	}
	if(output1 != NULL)
	{
		delete output1;
	}
	return bRet;
}
//notice:sInputData is LangCode::SYSTEM and is changed at here
bool WinUtil::RegistInput(cc::Str& sInputData, const char* sREG_KEY1)
{
	bool bRet = false;
	sInputData.Replace(" ", "").Replace("\r", "").Replace("\n", "").Replace("-", "").Replace(",", "").Replace(";", "").Replace("<", "").Replace(">", "");
	int nInd1 = sInputData.IndexOf(REG_KEYS);
	if(nInd1 >= 0)
	{
		sInputData.Remove(0, nInd1 + (int)strlen(REG_KEYS));
	}
	int nInd2 = sInputData.IndexOf(REG_KEYE);
	if(nInd2 > 0)
	{
		sInputData.Remove(nInd2);
	}
#if _DEBUG_
	cc::Log::debug("inputed len:%d, data:[%s]", sInputData.GetLength(), (char*)sInputData.GetBuf());
#endif
	if(sInputData.GetLength() > 0)
	{
		char* output = NULL;
		char* output2 = NULL;
		int nLen = cc::Util::codeB64Decode((char*)sInputData.GetBuf(), (unsigned int)sInputData.GetLength(), &output);
		if(nLen > 0)
		{
			//from 20081101,use ndes
			//notice,crypt it for loop at least 8
			//cc::Util::teaDeCrypt(output, nLen, (char*)sREG_KEY1, (int)strlen(sREG_KEY1), 8);
			//void  Util::ndesCrype(char* inoutbuf, int len, const char* key, int keylen, bool isCrype)

			//from 20101119, if is regkey for uuInsMaker, add araCrype
			cc::Util::araCrype(output, nLen, (char*)sREG_KEY1, (int)strlen(sREG_KEY1));
#if _DEBUG_
			cc::Log::debug("araCrype, len:%d, data:[%s]", nLen, (char*)output);
#endif

			nLen = cc::Util::codeB64Decode(output, nLen, &output2);
			//cc::Util::ndesCrype(output, nLen, (char*)sREG_KEY1, (int)strlen(sREG_KEY1), false);
			AES aes(AES::BIT256, (unsigned char *)sREG_KEY1, (int)strlen(sREG_KEY1));
			aes.InvCipher(output2, nLen);
			cc::Util::teaDeCrypt(output2, nLen, (char*)sREG_KEY1, (int)strlen(sREG_KEY1), 4); //UU_REG_CRYPT_DATA_LOOPCNT
#if _DEBUG_
			cc::Log::debug("AES Restore, len:%d, data:[%s]", nLen, (char*)output2);
#endif

			cc::Str sUserName = cc::Str(LangCode::UTF8);
			cc::Str sUserKey = cc::Str(LangCode::UTF8);
			cc::Str sUserData = cc::Str(LangCode::UTF8);
			//\nName=
			cc::Str tmp = cc::Str(LangCode::SYSTEM, "\n");
			tmp.Append("Na").Append("me=");
			int nDataPos = cc::Util::GetHtmlTag(output2, 0, (char*)tmp, -1, "\n", 1, sUserName, NULL, -1, LangCode::UTF8);
			if(nDataPos >= 0)
			{
				tmp = cc::Str(LangCode::SYSTEM, "\n");
				tmp.Append("Ke").Append("y=");
				nDataPos = cc::Util::GetHtmlTag(output2, nDataPos - 2, (char*)tmp, -1, "\n", 1, sUserKey, NULL, -1, LangCode::UTF8);
				if(nDataPos >= 0)
				{
					tmp = cc::Str(LangCode::SYSTEM, "\n");
					tmp.Append("Da").Append("ta=");
					nDataPos = cc::Util::GetHtmlTag(output2, nDataPos - 2, (char*)tmp, -1, "\n", 1, sUserData, NULL, -1, LangCode::UTF8);
				}
			}
			if(nDataPos > 0)
			{
				//notice here is UTF8
				//save to lang for temp,treate it at ~MainForm()
				bRet = true;
				cc::Str tkey = _T("Lang");
				tkey.Append(_T(".r."));
				CC_APP_GetLang()->Set(tkey + cc::Str(_T("k")), sUserKey);
				CC_APP_GetLang()->Set(tkey + cc::Str(_T("d")), sUserData);
#if _DEBUG_
				cc::Log::debug("OK, Name:%s, Key:%s, Data:[%s]", (char*)sUserName.GetBuf(), (char*)sUserKey.GetBuf(), (char*)sUserData.GetBuf());
#endif
			}
		}
		if(output2 != NULL)
		{
			delete output2;
			output2 = NULL;
		}
		if(output != NULL)
		{
			delete output;
			output = NULL;
		}
	}
	return bRet;
}
bool WinUtil::RegistHasData(cc::Str& sTime, cc::Str& sRegNow1, cc::Str& sRegNow2, const char* sREG_KEY1, int nBios, int nFile, cc::Str& sNowCrypt)
{
	int nLen = 0;
	char* output = NULL;
	if(sTime.GetLength() > 0)
	{
		nLen = cc::Util::codeB64Decode((char*)sTime.GetBuf(), (unsigned int)sTime.GetLength(), &output);
		cc::Util::teaDeCrypt((char*)output, nLen, (char*)sREG_KEY1, (int)strlen(sREG_KEY1), 9);
		cc::Util::GetHtmlTag(output, 0, "\nN1=", -1, "\n", 1, sRegNow1, NULL, -1, LangCode::SYSTEM);
		cc::Util::GetHtmlTag(output, 0, "\nN2=", -1, "\n", 1, sRegNow2, NULL, -1, LangCode::SYSTEM);
		if(output != NULL)
		{
			delete output;
		}
		return true;
	}
	else
	{
		if(sNowCrypt.GetLength() <= 0)
		{
			//crypt now for first time
			cc::Str sNow = cc::Str(LangCode::SYSTEM);
			sNow.Format("\nN1=%d\nN2=%d\n", nBios, nFile);
			//after GetBuffer, not need GetBufferSetLength() bucause not change sring length here
			cc::Util::teaEnCrypt((char*)sNow.GetBuffer(), sNow.GetLength(), (char*)sREG_KEY1, (int)strlen(sREG_KEY1), 9);
			nLen = cc::Util::codeB64Encode((char*)sNow.GetBuffer(), (unsigned int)sNow.GetLength(), &output);
			sNowCrypt = cc::Str(output);
			if(output != NULL)
			{
				delete output;
			}
		}
	}
	return false;
}
//get now form BIOS and file of system.dat,
//then get firstruntime,if not,then create it from now with nType&nTimes
//if two time of now is all between the two time of firstruntime then OK
int WinUtil::RegistExpired(const TCHAR* sPrgRegDir, const char* sREG_KEY1)
{
	CC_ASSERT(sREG_KEY1 != NULL);

	//item name of registy or file
	cc::Str sItem = cc::Str(LangCode::SYSTEM, sPrgRegDir);
	cc::Util::teaEnCrypt((char*)sItem.GetBuffer(), sItem.GetLength(), (char*)sREG_KEY1, (int)strlen(sREG_KEY1), 9);
	char* output = NULL;
	int nLen = cc::Util::codeB64Encode((char*)sItem.GetBuf(), (unsigned int)sItem.GetLength(), &output);
	cc::Str sItem2 = cc::Str(output);
	sItem2.Replace(_T("="), _T(""));
	sItem2.Insert(0, _T("T."));

	int nBios = 0;
	int nFile = 0;
	//get now
	cc::Util::GetSystemDate(nBios, nFile);
	////crypt now for first time
	//cc::Str sNow = cc::Str(LangCode::SYSTEM);
	//sNow.Format("\nN1=%d\nN2=%d\n", nBios, nFile);
	//cc::Util::teaEnCrypt((char*)sNow.GetBuffer(), sNow.GetLength(), (char*)sREG_KEY1, (int)strlen(sREG_KEY1), 9);
	//nLen = cc::Util::codeB64Encode((char*)sNow.GetBuffer(), (unsigned int)sNow.GetLength(), &output);
	//cc::Str sNowCrypt = cc::Str(output);

	//get first run time from reg
	cc::Str sRegNow1;
	cc::Str sRegNow2;
	cc::RegistryKey* regKey = cc::Registry::GetLocalMachine();
	if(!regKey->OpenSubKey(REG_EXPIRED_KEY, true))
	{
		regKey->CreateSubKey(REG_EXPIRED_KEY);
	}
	cc::Str sFTime1 = cc::Str(LangCode::SYSTEM, regKey->GetValue(sItem2));
	cc::Str sNowCrypt;
	if(RegistHasData(sFTime1, sRegNow1, sRegNow2, sREG_KEY1, nBios, nFile, sNowCrypt))
	{
	}
	//if(sFTime1.GetLength() > 0)
	//{
	//	nLen = cc::Util::codeB64Decode((char*)sFTime1.GetBuffer(), (unsigned int)sFTime1.GetLength(), &output);
	//	cc::Util::teaDeCrypt((char*)output, nLen, (char*)sREG_KEY1, (int)strlen(sREG_KEY1), 9);
	//	cc::Util::GetHtmlTag(output, 0, "\nN1=", -1, "\n", 1, sRegNow1, NULL, -1, LangCode::SYSTEM);
	//	cc::Util::GetHtmlTag(output, 0, "\nN2=", -1, "\n", 1, sRegNow2, NULL, -1, LangCode::SYSTEM);
	//}
	else
	{
		regKey->SetValue(sItem2, sNowCrypt);
	}
	regKey->Close();
	delete regKey;

	//get first run time from system.dat
	cc::Str sFileNow1;
	cc::Str sFileNow2;
	cc::Str strbuf(_MAX_FNAME * 2, 0);
	::GetWindowsDirectory((TCHAR*)strbuf.GetBuffer(), _MAX_FNAME * 2);
	strbuf.UpdateStringLength();
	strbuf.Append(_T("\\temp\\"));
	if(!cc::Util::dExist(strbuf))
	{
		cc::Util::dCreate(strbuf);
	}
	strbuf.Append(_T("uw.tmp"));
	cc::KeyArr keyArr;
	keyArr.Add(strbuf, LangCode::UTF8, true);
	cc::Str sFTime2 = cc::Str(LangCode::SYSTEM, keyArr.Get(sItem2));
	if(RegistHasData(sFTime2, sFileNow1, sFileNow2, sREG_KEY1, nBios, nFile, sNowCrypt))
	{
	}
	//if(sFTime2.GetLength() > 0)
	//{
	//	nLen = cc::Util::codeB64Decode((char*)sFTime2.GetBuffer(), (unsigned int)sFTime2.GetLength(), &output);
	//	cc::Util::teaDeCrypt((char*)output, nLen, (char*)sREG_KEY1, (int)strlen(sREG_KEY1), 9);
	//	cc::Util::GetHtmlTag(output, 0, "\nN1=", -1, "\n", 1, sFileNow1, NULL, -1, LangCode::SYSTEM);
	//	cc::Util::GetHtmlTag(output, 0, "\nN2=", -1, "\n", 1, sFileNow2, NULL, -1, LangCode::SYSTEM);
	//}
	else
	{
		keyArr.Set(sItem2, sNowCrypt);
		keyArr.Save(strbuf, LangCode::UTF8, true);
	}
	if(output != NULL)
	{
		delete output;
	}

	int n1 = sRegNow1.ToInt();
	int n = sFileNow1.ToInt();
	if(n1 <= 0 || (n > 0 && n < n1))
	{
		n1 = n;
	}
	int n2 = sRegNow2.ToInt();
	n = sFileNow2.ToInt();
	if(n2 <= 0 || (n > 0 && n < n2))
	{
		n2 = n;
	}

	int nDiff1 = (n1 > 0 ? nBios - n1 : 0);
	int nDiff2 = (n2 > 0 ? nFile - n2 : 0);
	if(nDiff1 < 0)
	{
		nDiff1 *= -1;
	}
	if(nDiff2 < 0)
	{
		nDiff2 *= -1;
	}
	if(nDiff1 < nDiff2)
	{
		nDiff1 = nDiff2;
	}
#if _DEBUG_
	cc::Log::debug(_T("WinUtil::RegistExpired, Item:%s, RegNow1:%s, RegNow2:%s, FileNow1:%s, FileNow2:%s, Return:%d"), (TCHAR*)sItem2.GetBuf(), (TCHAR*)sRegNow1.GetBuf(), (TCHAR*)sRegNow2.GetBuf(), (TCHAR*)sFileNow1.GetBuf(), (TCHAR*)sFileNow2.GetBuf(), nDiff1);
#endif
	return nDiff1;
}

cc::Str WinUtil::GetIp(cc::Str sSkipIp_Pre)
{
	//for WSAStartup&WSACleanup
	cc::win::Socket socket(SOCK_DGRAM);
	cc::Str sRet;

	char ac[80];
	if(gethostname(ac, sizeof(ac)) == SOCKET_ERROR)
	{
		return sRet;
	}
	struct hostent *phe = gethostbyname(ac);
	if(phe == 0)
	{
		return sRet;
	}

	for(int i = 0; phe->h_addr_list[i] != 0; ++i)
	{
		struct in_addr addr;
		memcpy(&addr, phe->h_addr_list[i], sizeof(struct in_addr));

		cc::Str str = inet_ntoa(addr);
		if(sSkipIp_Pre.GetLength() == 0 || !str.StartsWith(sSkipIp_Pre))
		{
			if(sRet.GetLength() > 0)
			{
				sRet.Append(_T(";"));
			}
			sRet.Append(str);
		}
	}

	return sRet;
}

//cannot find real netcard!
typedef DWORD (__stdcall *pGetAdaptersInfo)(PIP_ADAPTER_INFO,PULONG);
// Fetches the MAC address and prints it
cc::Str WinUtil::GetMAC(cc::Str sSkipIp_Pre, bool isWithIpAddress)
{
	cc::Str sRet;
	IP_ADAPTER_INFO AdapterInfo[16];       // Allocate information for up to 16 NICs
	DWORD dwBufLen = sizeof(AdapterInfo);  // Save memory size of buffer
 
	HMODULE hInst = ::LoadLibrary(_T("iphlpapi.dll"));
	if(hInst != NULL)
	{
		pGetAdaptersInfo GetAdaptersInfo = (pGetAdaptersInfo)GetProcAddress(hInst,"GetAdaptersInfo");
		if(GetAdaptersInfo != NULL)
		{
			DWORD dwStatus = GetAdaptersInfo(      // Call GetAdapterInfo
					AdapterInfo,                   // [out] buffer to receive data
					&dwBufLen);                    // [in] size of receive data buffer
			if(dwStatus != ERROR_SUCCESS)     // Verify return value is valid, no buffer overflow
			{
				return sRet;
			}
 
			PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo; // Contains pointer to current adapter info
			do
			{
				cc::Str str2 = cc::Str(pAdapterInfo->IpAddressList.IpAddress.String);
				if(sSkipIp_Pre.GetLength() == 0 || !str2.StartsWith(sSkipIp_Pre))
				{
					cc::Str str;
					str.Format(_T("%02X-%02X-%02X-%02X-%02X-%02X"), 
						pAdapterInfo->Address[0], pAdapterInfo->Address[1],
						pAdapterInfo->Address[2], pAdapterInfo->Address[3],
						pAdapterInfo->Address[4], pAdapterInfo->Address[5]);
					if(sRet.GetLength() > 0)
					{
						sRet.Append(_T(";"));
					}
					sRet.Append(str);
					if(isWithIpAddress)
					{
						sRet.Append(_T(":")).Append(str2);
					}
				}

				//pAdapterInfo->AdapterName;
				//pAdapterInfo->IpAddressList;
				pAdapterInfo = pAdapterInfo->Next;    // Progress through linked list
			}
			while(pAdapterInfo);                      // Terminate if last adapter
		}
		::FreeLibrary(hInst);
	}
	return sRet;
}

///////////////////////////////////////////////////////////////////////////////
//===========================================================cc::win::WinTool

DWORD TabControlExConfig::nStyle = WS_CHILD | WS_VISIBLE | WS_TABSTOP;
DWORD TabControlExConfig::nExStyle = 0;
//DWORD TabControlExConfig::nClassStyle = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
DWORD TabControlExConfig::nInnerStyle = InnerStyle::TrackMouse;
DWORD TabControlExConfig::nOuterStyle = OuterStyle::TransparentParentErase;
DWORD TabControlExConfig::nOwnerDrawTextStyle = DT_LEFT|DT_VCENTER|DT_SINGLELINE;
COLORREF TabControlExConfig::corBackground = ::GetSysColor(COLOR_BTNFACE);
COLORREF TabControlExConfig::corTextground = ::GetSysColor(COLOR_BTNTEXT);
COLORREF TabControlExConfig::corHotBackColor = ::GetSysColor(COLOR_MENU);
COLORREF TabControlExConfig::corHotTextColor = ::GetSysColor(COLOR_MENUTEXT);
COLORREF TabControlExConfig::corFlatButtonsHeadBackColor = RGB(80, 80, 80);
COLORREF TabControlExConfig::corNoBodyRectColor = RGB(229, 151, 0);
int TabControlExConfig::nMarginLeft = 0;
int TabControlExConfig::nMarginTop = 0;
int TabControlExConfig::nMarginRight = 0;
int TabControlExConfig::nMarginBottom = 0;

//===========================================================cc::win::TabControlEx
///////////////////////////////////////////////////////////////////////////////
// constructors/destructor
TabControlEx::TabControlEx()
{
	BASE_CONTROL_CONFIG_INIT(TabControlEx);
	::SetRect(&_Rect, 0, 0, 75, 23);
	::CopyRect(&_OrgRect, &_Rect);
	//used for contain page
	::SetRect(&_Margin, 3, 3, 3, 3);
	_ClassName = WndClassName::TabControlEx;
	//_StyleArray[(int)StyleType::Style] = WS_CHILD|WS_VISIBLE|TCS_SINGLELINE|TCS_OWNERDRAWFIXED;
	//_StyleArray[(int)StyleType::Style] = WS_CHILD | WS_VISIBLE | WS_TABSTOP;
	//_StyleArray[(int)StyleType::ExStyle] = 0; //no extended wnd style
	//_ClassStyle = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
	//_InnerStyle |= InnerStyle::TrackMouse;

	_nHeadHeight = TABEX_DEFAULT_HEADHEIGHT;
	_nSelectedIndex = -1;
	_nLastSelectedIndex = -1;
	_nBestHeadWidth = TABEX_DEFAULT_HEAD_BESTWIDTH;
	_eAlignment = TabAlignment::Top;
	_eAppearance = TabAppearance::Normal;
	//for show text in vlist
	_nVHeadWidth = -1;
	//_TabStyle = TabStyle::PrevShowRight | TabStyle::NextShowRight | TabStyle::NextShowMenu|TabStyle::TopEndShow;
	_TabStyle = 0;

	_corHeadBackColor = TabControlExConfig::corFlatButtonsHeadBackColor;
	_nHeadFirstIndex = -1;
	_nHeadLastIndex = -2;
	m_nHotIndex = -1;
	_nLastActiveIndex = 0;
	_lstRightCtls = NULL;
	_lstHeadIconsID = NULL;
	_pToolPriv = NULL;
	_pToolNext = NULL;
	_pToolTop = NULL;
	_pToolEnd = NULL;

	_pHeadContextMenu = NULL;
	_pItemListMenu = NULL;

	SetRectEmpty(&_rectBody);
	SetRectEmpty(&_rectPage);
	SetRectEmpty(&_rectHead);
	_sizeHeadIcon.cx = 16;
	_sizeHeadIcon.cy = 16;

#ifdef _DEBUG_
	SetName(_T("TabControlEx"));
#endif
#if defined(_DEBUG_) || defined(_DESIGN)
	//code for design
	//flag for moveable or others
	Design = (DesignStyle::Type)(CC_DESIGN_NO_COLOR);
#endif
}

TabControlEx::~TabControlEx()
{
	Dispose();
	if(_lstRightCtls != NULL)
	{
		for(int n = _lstRightCtls->GetSize() - 1; n >= 0; n--)
		{
			Control* ctl = _lstRightCtls->GetKey(n);
			delete ctl;
		}
		delete _lstRightCtls;
		_lstRightCtls = NULL;
	}
	if(_lstHeadIconsID != NULL)
	{
		delete _lstHeadIconsID;
		_lstHeadIconsID = NULL;
	}
	if(_lstControls != NULL)
	{
		for(int n = _lstControls->GetSize() - 1; n >= 0; n--)
		{
			TabPage* ctl = (TabPage*)_lstControls->GetKey(n);
			if(ctl->PageData != NULL)
			{
				delete ctl->PageData;
				ctl->PageData = NULL;
			}
		}
	}
}
// constructors/destructor

void TabControlEx::Dispose()
{
	if(!GetIsDispose())
	{
		if(_lstRightCtls != NULL)
		{
			for(int n = _lstRightCtls->GetSize() - 1; n >= 0; n--)
			{
				Control* ctl = _lstRightCtls->GetKey(n);
				ctl->Dispose();
			}
		}
		if(_pToolPriv != NULL)
		{
			_pToolPriv->Dispose();
		}
		if(_pToolNext != NULL)
		{
			_pToolNext->Dispose();
		}
		if(_pToolTop != NULL)
		{
			_pToolTop->Dispose();
		}
		if(_pToolEnd != NULL)
		{
			_pToolEnd->Dispose();
		}
		Container::Dispose();
	}
}

//keyArr is param addres,but sHead is by value
void TabControlEx::DoConfig(bool isFromIni, KeyArr& keyArr, cc::Str sThisHead)
{
	if(!HasStyle(StyleType::Outer, OuterStyle::AutoConfig))
	{
		return;
	}
	sThisHead.Append(_T(".Index"));
	if(isFromIni)
	{
		//if not exist,as usually
		if(keyArr.Exist(sThisHead))
		{
			int nInd = keyArr.Get(sThisHead, 0);
			if(nInd >= 0 && nInd < GetTabCount())
			{
				SetSelectedIndex(nInd);
			}
		}
	}
	else
	{
		keyArr.Set(sThisHead, GetSelectedIndex());
	}
}

void TabControlEx::OnCreateControl()
{
	//do some setting
	OnStyleChanged(false);
	//AdjustRect();
	int nCnt = (_lstControls == NULL) ? 0 : _lstControls->GetSize();
	if(nCnt > 0)
	{
		//first resize page to tab,then create it
		for(int n = 0; n < nCnt; n++)
		{
			Control* ctl = _lstControls->GetKey(n);
			//ctl->SetVisible(n ==_nSelectedIndex);
			ctl->SetVisible(false);
			DoInsPage((TabPage*)ctl, n);
		}
		SetSelectedIndex(_nSelectedIndex);
	}
	Container::OnCreateControl();
	//create HeadControls
	if(_lstRightCtls != NULL)
	{
		for(int n = _lstRightCtls->GetSize() - 1; n >= 0; n--)
		{
			Control* ctl = _lstRightCtls->GetKey(n);
			ctl->SetParent(this, true);
			ctl->SetVisible(false);
			if(ctl->m_hWnd == NULL)
			{
				ctl->CreateControl();
			}
		}
	}
}

bool TabControlEx::HasTabStyle(DWORD style)
{
	//must != 0
	return ((_TabStyle & style) != 0);
}

void TabControlEx::SetTabStyle(DWORD dwRemove, DWORD dwAdd)
{
	_TabStyle = (_TabStyle & ~dwRemove) | dwAdd;
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
	{
		//SetOuterStyle(OuterStyle::Sizing, true);
		//SetStyle(StyleType::Outer, 0, OuterStyle::Sizing);
		_StyleArray[(int)StyleType::Outer] |= OuterStyle::Sizing;
		OnStyleChanged(true);
		OnResize(NULL);
		//SetOuterStyle(OuterStyle::Sizing, false);
		SetStyle(StyleType::Outer, OuterStyle::Sizing, 0);
	}
}

//for _nHeadHeightCur&_nVHeadWidthCur is changed by OnStyleChanged,do here before AdjustRect
void TabControlEx::OnStyleChanged(bool isAddItemListMenu)
{
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
	{
		if(HasTabStyle(TabStyle::PrevShowMenu|TabStyle::NextShowMenu|TabStyle::CreateItemListMenu))
		{
			if(_pItemListMenu == NULL)
			{
				_pItemListMenu = new ContextMenu();
				DisposeItemAdd(_pItemListMenu);
				_pItemListMenu->SetAllOwnerDraw(true);
			}
			if(isAddItemListMenu && _lstControls != NULL && _pItemListMenu->ItemCount() != _lstControls->GetSize())
			{
				_pItemListMenu->ItemClear();
				int nCnt = _lstControls->GetSize();
				for(int n = 0; n < nCnt; n++)
				{
					TabPage* ctl = (TabPage*)_lstControls->GetKey(n);
					AddItemListMenu(ctl, n);
				}
			}
		}
		if(_pToolPriv != NULL)
		{
			_pToolPriv->SetVisible(false);
		}
		//need always create _pToolPriv
		//if(HasTabStyle(TabStyle::PrevShowLeft|TabStyle::PrevShowRight))
		{
			if(_pToolPriv == NULL)
			{
				_pToolPriv = new ToolItem();
				_pToolPriv->SetVisible(false);
				_pToolPriv->SetFont(10, _T("Marlett"));
				_pToolPriv->SetMargin(0, 0, 0, 0);
				_pToolPriv->SetParent(this, true);
				_pToolPriv->Click += EventHandler((Object*)this, (EventFun)&TabControlEx::PrivTool_OnClick);
				//for toolitem while 1 then Do toolitem's ClickEvent even has _pContextMenu
				//and while set e->Handled in toolitem's ClickEvent,then not call first item's ClickEvent of _pContextMenu even has _pContextMenu
				_pToolPriv->SetOuterStyle(OuterStyle::PrivateFlag1, true);
				_pToolPriv->CreateControl();
				//_pToolPriv->SetOuterStyle(OuterStyle::ContextMenuButton, false);
				DisposeItemAdd(_pToolPriv);
			}
			if(HasTabStyle(TabStyle::PrevShowMenu))
			{
				_pToolPriv->SetContextMenu(_pItemListMenu);
			}
			else
			{
				_pToolPriv->SetContextMenu(NULL);
			}
			if(_eAlignment == TabAlignment::Top || _eAlignment == TabAlignment::Bottom)
			{
				_pToolPriv->SetText(_T("3")); //<-
			}
			else
			{
				_pToolPriv->SetText(_T("5")); //A
			}
		}
		if(_pToolNext != NULL)
		{
			_pToolNext->SetVisible(false);
		}
		//need always create _pToolNext
		//if(HasTabStyle(TabStyle::NextShowLeft|TabStyle::NextShowRight))
		{
			if(_pToolNext == NULL)
			{
				_pToolNext = new ToolItem();
				_pToolNext->SetVisible(false);
				_pToolNext->SetFont(10, _T("Marlett"));
				_pToolNext->SetMargin(0, 0, 0, 0);
				_pToolNext->SetParent(this, true);
				_pToolNext->Click += EventHandler((Object*)this, (EventFun)&TabControlEx::NextTool_OnClick);
				//for toolitem while 1 then Do toolitem's ClickEvent even has _pContextMenu
				//and while set e->Handled in toolitem's ClickEvent,then not call first item's ClickEvent of _pContextMenu even has _pContextMenu
				_pToolNext->SetOuterStyle(OuterStyle::PrivateFlag1, true);
				_pToolNext->CreateControl();
				//_pToolPriv->SetOuterStyle(OuterStyle::ContextMenuButton, false);
				DisposeItemAdd(_pToolNext);
			}
			if(HasTabStyle(TabStyle::NextShowMenu))
			{
				_pToolNext->SetContextMenu(_pItemListMenu);
			}
			else
			{
				_pToolNext->SetContextMenu(NULL);
			}
			if(_eAlignment == TabAlignment::Top || _eAlignment == TabAlignment::Bottom)
			{
				_pToolNext->SetText(_T("4")); //->
			}
			else
			{
				_pToolNext->SetText(_T("6")); //V
			}
		}
		if(_pToolTop != NULL)
		{
			_pToolTop->SetVisible(false);
		}
		if(_pToolEnd != NULL)
		{
			_pToolEnd->SetVisible(false);
		}
		if(HasTabStyle(TabStyle::TopEndShow))
		{
			if(_pToolTop == NULL)
			{
				_pToolTop = new ToolItem();
				_pToolTop->SetText(_T("7")); //V
				_pToolTop->SetVisible(false);
				_pToolTop->SetMargin(0, 0, 0, 0);
				_pToolTop->SetParent(this, true);
				_pToolTop->Click += EventHandler((Object*)this, (EventFun)&TabControlEx::TopTool_OnClick);
				_pToolTop->CreateControl();
				DisposeItemAdd(_pToolTop);
			}
			if(_eAlignment == TabAlignment::Top || _eAlignment == TabAlignment::Bottom)
			{
//void Control::SetFont(int nSize, const TCHAR* sName, bool isBold, bool isItalic, bool isUnderline, bool isStrikeOut, long nEscapement)
				_pToolTop->SetFont(-12, _T("Marlett"), false, false, false, false, 500);
			}
			else
			{
				_pToolTop->SetFont(10, _T("Marlett"), false, false, false, false, 1800);
			}
			if(_pToolEnd == NULL)
			{
				_pToolEnd = new ToolItem();
				_pToolEnd->SetText(_T("7")); //V
				_pToolEnd->SetVisible(false);
				_pToolEnd->SetMargin(0, 0, 0, 0);
				_pToolEnd->SetParent(this, true);
				_pToolEnd->Click += EventHandler((Object*)this, (EventFun)&TabControlEx::EndTool_OnClick);
				_pToolEnd->CreateControl();
				DisposeItemAdd(_pToolEnd);
			}
			if(_eAlignment == TabAlignment::Top || _eAlignment == TabAlignment::Bottom)
			{
				_pToolEnd->SetFont(10, _T("Marlett"), false, false, false, false, 900);
			}
			else
			{
				_pToolEnd->SetFont(10, _T("Marlett"), false, false, false, false, 0);
			}
		}

		::InvalidateRect(m_hWnd, NULL, FALSE);
	}
}

void TabControlEx::AddPage(TabPage *control, int nIndex)
{
	//assert that not added
	CC_ASSERT(_lstControls == NULL || _lstControls->GetIndex(control) < 0);
	CC_ASSERT(control->PageData == NULL);

	//hide it first
	((Control*)control)->SetVisible(false);
	control->PageData = new HeadInfo();
	if(control->HasStyle(StyleType::Inner, InnerStyle::BackgroundTheme))
	{
		control->SetStyle(StyleType::Inner,
			InnerStyle::BackgroundBrush|InnerStyle::BackgroundBmp|InnerStyle::BackgroundTheme,
			InnerStyle::BackgroundColor, 0, true);
	}
	Container::AddControl(control, nIndex);
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0 && m_hWnd != NULL)
	{
		DoInsPage(control, nIndex);
		//if end item is selected,then show this page
		CalcuteHeadInfo();
		//::InvalidateRect(m_hWnd, &_rectHead, FALSE);
		::InvalidateRect(m_hWnd, NULL, FALSE);
	}
}

//show delete TabPage out this scope
TabPage* TabControlEx::RemovePage(int nIndex)
{
	CC_ASSERT(nIndex >= 0 && _lstControls != NULL && nIndex < _lstControls->GetSize());

	//before delete it,set selindex to others
	int nCurIndex = GetSelectedIndex();
	//if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
	//{
		if(nCurIndex >= nIndex)
		{
			nCurIndex--;
		}
		m_nHotIndex = -1;
	//}
	if(_nLastSelectedIndex == nIndex)
	{
		_nLastSelectedIndex = -1;
	}
	else if(_nLastSelectedIndex > nIndex)
	{
		_nLastSelectedIndex--;
	}
	//modi menu if need
	if(_pItemListMenu != NULL)
	{
		_pItemListMenu->ItemRemove(nIndex);
	}
	TabPage* ctl = (TabPage*)_lstControls->GetKey(nIndex);
	if(ctl->PageData != NULL)
	{
		delete ctl->PageData;
		ctl->PageData = NULL;
	}
	Container::RemoveControl(ctl);
	//if -1, then change to 0 at SetSelectedIndex
	SetSelectedIndex(nCurIndex);
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
	{
		_nUpdateFlag |= UpdateFlag::UpdateFlag;
		DoUpdate();
	}
	return ctl;
}

//the index must be index of _lstControls,or -1 for auto get real index
int TabControlEx::DoInsPage(TabPage *control, int nIndex)
{
	CC_ASSERT(::IsWindow(m_hWnd));
	CC_ASSERT(nIndex >= 0 || (nIndex < 0 && _lstControls != NULL && _lstControls->GetIndex(control) >= 0));

	if(control->PageData == NULL)
	{
		control->PageData = new HeadInfo();
	}
	if(nIndex == -1)
	{
		nIndex = _lstControls->GetIndex(control);
	}
	if(HasTabStyle(TabStyle::PrevShowMenu|TabStyle::NextShowMenu|TabStyle::CreateItemListMenu))
	{
		AddItemListMenu(control, nIndex);
	}
	CalcuteHeadWidth(control, nIndex);
	//not need CalcuteHeadInfo for do it at parent 
	if(_nLastSelectedIndex >= nIndex)
	{
		_nLastSelectedIndex++;
	}
	if(_nSelectedIndex < 0 || _nSelectedIndex >= _lstControls->GetSize())
	{
		SetSelectedIndex(0);
	}
	return nIndex;
}

//the index must be index of _lstControls,or -1 for auto get real index
int TabControlEx::DoModPage(TabPage *control, int nIndex)
{
	CC_ASSERT(::IsWindow(m_hWnd) && _lstControls != NULL && nIndex < GetTabCount());

	if(nIndex == -1)
	{
		nIndex = _lstControls->GetIndex(control);
	}
	if(nIndex == -1)
	{
		throw;
	}
	//modi menu if need
	if(HasTabStyle(TabStyle::PrevShowMenu|TabStyle::NextShowMenu|TabStyle::CreateItemListMenu))
	{
		CC_ASSERT(_pItemListMenu != NULL);

		MenuItem* menu = (MenuItem*)_pItemListMenu->ItemGet(nIndex);
		menu->SetEnabled(control->GetEnabled());
		menu->SetText(control->GetText());
		menu->SetIcon(control->GetImageList(), control->GetIconID());
		//menu->SetIcon(control->GetIcon(IconType::Hot), IconType::Hot);
		//menu->SetIcon(control->GetIcon(IconType::Gloom), IconType::Gloom);
		//menu->SetIcon(control->GetIcon(IconType::Disabled), IconType::Disabled);
	}
	CalcuteHeadWidth(control, nIndex);
	//not need CalcuteHeadInfo for do it at parent
	return nIndex;
}

void TabControlEx::CalcuteHeadWidth(TabPage *control, int nIndex)
{
	CC_ASSERT(control->PageData != NULL);

	HeadInfo* pHI = (HeadInfo*)control->PageData;
	RECT rc = {0, 0, 0, 0};
	int nLen = control->GetTextLen();
	if(nLen > 0)
	{
		const TCHAR* tc = control->GetText();
		HDC hDC = ::GetDC(m_hWnd);
		//use tab's font
		HFONT hOldFont = (HFONT)::SelectObject(hDC, GetFont());
		::DrawText(hDC, tc, -1, &rc, DT_CALCRECT);
		::SelectObject(hDC, hOldFont);
		if(GetFont() != hOldFont)
		{
			::DeleteObject(hOldFont);
		}
		::ReleaseDC(m_hWnd, hDC);
		//space before and after text
		rc.right += TABEX_PAGEHEAD_MARGINL + TABEX_PAGEHEAD_MARGINR;
		rc.bottom += TABEX_PAGEHEAD_MARGINT + TABEX_PAGEHEAD_MARGINB;
	}
	rc.bottom = CC_MAX(rc.bottom, _nHeadHeight);
	if(control->GetIcon() != NULL)
	{
		if(nLen > 0)
		{
			//the height of icon is rc.bottom - (TABEX_PAGEHEAD_MARGINT + TABEX_PAGEHEAD_MARGINB)
			rc.right += TABEX_SPLITE_ICON_TEXT + rc.bottom - (TABEX_PAGEHEAD_MARGINT + TABEX_PAGEHEAD_MARGINB);
		}
		else
		{
			//left and right margin and height of icon
			rc.right = TABEX_PAGEHEAD_MARGINL + TABEX_PAGEHEAD_MARGINR + rc.bottom - (TABEX_PAGEHEAD_MARGINT + TABEX_PAGEHEAD_MARGINB);
		}
	}
	else
	{
		//the min width while no text&icon
		if(nLen <= 0)
		{
			rc.right = TABEX_PAGEHEAD_MINWIDTH;
		}
	}
	//get headright icon width
	//for DropMenu size
	int nHeadBW = 0;
	if(_lstHeadIconsID != NULL)
	{
		nHeadBW = _lstHeadIconsID->GetSize() * (_nHeadHeight + 1);
		if(nHeadBW != 0)
		{
			nHeadBW += TABEX_SPLITE_HEAD_RICON;
		}
	}
	pHI->width = rc.right + nHeadBW;
	if(pHI->width < TABEX_PAGEHEAD_MINWIDTH)
	{
		pHI->width = TABEX_PAGEHEAD_MINWIDTH;
	}
}

void TabControlEx::CalcuteHeadActiveIndex(TabPage *control)
{
	CC_ASSERT(_lstControls->GetSize() < 0xffffff);

	_nLastActiveIndex++;
	//if reach max active index
	if(_nLastActiveIndex > 0xffffff)
	{
		// Map(bool isSort, bool bIsDESC = false, bool isDistinct = false, int nMinCapacity = 0, double nIncrement = 1.3);
		Map<int, void*>* mapCtl = new Map<int, void*>(true, false, true);;
		for(int n = _lstControls->GetSize() - 1; n >= 0; n--)
		{
			TabPage* page2 = (TabPage*)_lstControls->GetKey(n);
			mapCtl->Add(((HeadInfo*)page2->PageData)->activeIndex, page2);
		}
		for(int n = mapCtl->GetSize() - 1; n >= 0; n--)
		{
			TabPage* page2 = (TabPage*)mapCtl->GetAt(n);
			((HeadInfo*)page2->PageData)->activeIndex = n;
		}
		delete mapCtl;
		_nLastActiveIndex = _lstControls->GetSize();
	}
	((HeadInfo*)control->PageData)->activeIndex = _nLastActiveIndex;
}

TabPage* TabControlEx::GetPage(TCHAR* const ctlName) const
{
	return (TabPage*)Container::GetControl(ctlName);
}

TabPage* TabControlEx::GetPage(int nIndex) const
{
	return (TabPage*)Container::GetControl(nIndex);
}

int TabControlEx::GetTabCount() const
{
	return Container::GetControlCount();
}

void TabControlEx::SetSelectedIndex(int selectedIndex)
{
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0 && m_hWnd != NULL)
	{
		//::InvalidateRect(m_hWnd, &_rectHead, FALSE);
		::InvalidateRect(m_hWnd, NULL, FALSE);
	}
	if(_lstControls == NULL || _lstControls->GetSize() <= 0)
	{
		_nSelectedIndex = -1;
		_nLastSelectedIndex = -1;
		CalcuteHeadInfo();
		return;
	}
	if(selectedIndex < 0)
	{
		selectedIndex = 0;
	}
	if(selectedIndex >= _lstControls->GetSize())
	{
		selectedIndex = _lstControls->GetSize() - 1;
	}

	if(!_lstControls->GetKey(selectedIndex)->GetEnabled())
	{
		bool isFound = false;
		for(int n = selectedIndex + 1; n < _lstControls->GetSize(); n++)
		{
			if(_lstControls->GetKey(n)->GetEnabled())
			{
				isFound = true;
				selectedIndex = n;
				break;
			}
		}
		if(!isFound)
		{
			for(int n = 0; n < selectedIndex; n++)
			{
				if(_lstControls->GetKey(n)->GetEnabled())
				{
					selectedIndex = n;
					break;
				}
			}
		}
	}
	if(_nSelectedIndex == selectedIndex)
	{
		//need this for deleted curuent page,and reset _nSelectedIndex
		//fire OnSelectedIndexChanged
		OnSelectedIndexChanged(NULL);
		//_lstControls->GetKey(_nSelectedIndex)->SetVisible(true);
		return;
	}
	//if curent is not enabled,and no next enabled page,set current disabled page
	_nSelectedIndex = selectedIndex;
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
	{
		TabPage* page = (TabPage*)_lstControls->GetKey(_nSelectedIndex);
		HeadInfo* pHI = (HeadInfo*)page->PageData;
		if(HasTabStyle(TabStyle::IsSwapSelectedIndex))
		{
			//recalcute active index
			CalcuteHeadActiveIndex(page);
		}
		//if is _nHeadLastIndex,see it is need calcute
		if(  _nSelectedIndex >= _nHeadFirstIndex && 
			(_nSelectedIndex < _nHeadLastIndex || (_nSelectedIndex == _nHeadLastIndex && pHI->rect.right - pHI->rect.left + 6 > pHI->width))  )
		{
			//::InvalidateRect(m_hWnd, &_rectHead, FALSE);
			::InvalidateRect(m_hWnd, NULL, FALSE);
		}
		else
		{
			//now swap page of _nSelectedIndex to _nHeadFirstIndex~_nHeadLastIndex
			if(HasTabStyle(TabStyle::IsSwapSelectedIndex))
			{
				//search min activeindex
				int nMinActIndex = -1;
				int nMinSelInd = -1;
				for(int n = _nHeadFirstIndex; n <= _nHeadLastIndex; n++)
				{
					TabPage* pageAct = (TabPage*)_lstControls->GetKey(n);
					HeadInfo* pHIAct = (HeadInfo*)pageAct->PageData;
					if(nMinActIndex > pHIAct->activeIndex || nMinActIndex == -1)
					{
						nMinActIndex = pHIAct->activeIndex;
						nMinSelInd = n;
					}
				}
				if(nMinSelInd >= 0 && nMinSelInd != _nSelectedIndex)
				{
					//swap!
					_lstControls->Swap(nMinSelInd, _nSelectedIndex);
					if(_pItemListMenu != NULL)
					{
						_pItemListMenu->ItemSwap(nMinSelInd, _nSelectedIndex);
					}
					_nSelectedIndex = nMinSelInd;
				}
			}
			CalcuteHeadInfo(_nSelectedIndex);
		}
		//fire OnSelectedIndexChanged
		OnSelectedIndexChanged(NULL);
	}
}

int TabControlEx::GetSelectedIndex()
{
	return _nSelectedIndex;
}

void TabControlEx::SetSelectedTab(TabPage* selectedTab)
{
	if(_lstControls == NULL || !_lstControls->ContainsKey(selectedTab))
	{
		throw;
	}
	SetSelectedIndex(_lstControls->GetIndex(selectedTab));
}

TabPage* TabControlEx::GetSelectedTab() const
{
	if(_lstControls == NULL || _nSelectedIndex < 0)
	{
		return NULL;
	}
	return (TabPage*)_lstControls->GetKey(_nSelectedIndex);
}

ContextMenu* TabControlEx::GetHeadContextMenu() const
{
	return _pHeadContextMenu;
}

void TabControlEx::SetHeadContextMenu(ContextMenu* menu)
{
	_pHeadContextMenu = menu;
}

//for _nHeadHeightCur&_nVHeadWidthCur is changed by onstylechange,do here after onstylechange
void TabControlEx::AdjustRect()
{
	CC_ASSERT(::IsWindow(m_hWnd));

	::GetClientRect(m_hWnd, &_rectBody);
	::CopyRect(&_rectHead, &_rectBody);
	if(_eAlignment == TabAlignment::Top)
	{
		_rectBody.top += _nHeadHeight;
		_rectHead.bottom = _rectHead.top + _nHeadHeight;
	}
	else if(_eAlignment == TabAlignment::Bottom)
	{
		_rectBody.bottom -= _nHeadHeight;
		_rectHead.top = _rectHead.bottom - _nHeadHeight;
	}
	else if(_eAlignment == TabAlignment::Left)
	{
		if(_nVHeadWidth <= 0)
		{
			_rectBody.left += _nHeadHeight;
			_rectHead.right = _rectHead.left + _nHeadHeight;
		}
		else
		{
			_rectBody.left += _nVHeadWidth;
			_rectHead.right = _rectHead.left + _nVHeadWidth;
		}
	}
	else if(_eAlignment == TabAlignment::Right)
	{
		if(_nVHeadWidth <= 0)
		{
			_rectBody.right -= _nHeadHeight;
			_rectHead.left = _rectHead.right - _nHeadHeight;
		}
		else
		{
			_rectBody.right -= _nVHeadWidth;
			_rectHead.left = _rectHead.right - _nVHeadWidth;
		}
	}
	else //None
	{
		::SetRect(&_rectHead, 0, 0, 0, 0);
	}
	::SetRect(&_rectPage, _rectBody.left + _Margin.left, _rectBody.top + _Margin.top, _rectBody.right - _Margin.right, _rectBody.bottom - _Margin.bottom);
#ifdef _DEBUG_
	if(CC_APP_GetConfig()->GetIsDebug())
	{
		CC_TRACE((_T("TabControlEx::AdjustRect, Body[left:%d, top:%d, right-left:%d, bottom-top:%d]"),
			_rectBody.left, _rectBody.top, _rectBody.right - _rectBody.left, _rectBody.bottom - _rectBody.top));
		CC_TRACE((_T("TabControlEx::AdjustRect, Head[left:%d, top:%d, right-left:%d, bottom-top:%d]"),
			_rectHead.left, _rectHead.top, _rectHead.right - _rectHead.left, _rectHead.bottom - _rectHead.top));
	}
#endif
}

void TabControlEx::DoUpdate()
{
	if(_lstControls != NULL)
	{
		bool isUpdate = false;
		for(int n = _lstControls->GetSize() - 1; n >= 0; n--)
		{
			Control* ctl = _lstControls->GetKey(n);
			if(ctl->IsUpdate((UpdateFlag::Type)(UpdateFlag::UpdateFlag|UpdateFlag::UpdateText|UpdateFlag::UpdateIcon|UpdateFlag::UpdateEnabled)))
			{
				isUpdate = true;
				DoModPage((TabPage*)ctl, n);
			}
		}
		if(isUpdate || IsUpdate(UpdateFlag::UpdateFlag))
		{
			OnResize(NULL);
			_nUpdateFlag &= ~UpdateFlag::UpdateFlag;
		}
	}
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
	{
		//::InvalidateRect(m_hWnd, &_rectHead, FALSE);
		::InvalidateRect(m_hWnd, NULL, FALSE);
	}
}

int TabControlEx::CalcuteHeadInfoRButton(bool isShow, int nPosLeft, int nPosTop, int nPosRight, int nHeight)
{
//cc::Log::error("CalcuteHeadInfoRButton; isShow:%d, nPosLeft:%d, nPosTop:%d, nPosRight:%d, nHeight:%d", isShow, nPosLeft, nPosTop, nPosRight, nHeight);
	if(_lstRightCtls != NULL)
	{
		if(isShow)
		{
			//draw headbutton at right only if TabAlignment::Top or Bottom
			for(int n = _lstRightCtls->GetSize() - 1; n >= 0; n--)
			{
				Control* ctl = _lstRightCtls->GetKey(n);
				RECT ctlRect = ctl->CalcuteRect(false, 0, nPosTop, -1, nHeight);
				int nW = ctlRect.right - ctlRect.left;
//cc::Log::error("CalcuteHeadInfoRButton; nPosRight - nW:%d, ctlRect.top:%d, nW:%d, ctlRect.bottom - ctlRect.top:%d", nPosRight - nW, ctlRect.top, nW, ctlRect.bottom - ctlRect.top);
				ctl->SetBoundsNotOriginal(nPosRight - nW, ctlRect.top, nW, ctlRect.bottom - ctlRect.top, BoundsSpecified::All);
				ctl->SetVisible(true);
				nPosRight -= (nW + TABEX_SPLITE_HEADBUTTON);
				if(nPosRight < nPosLeft)
				{
					nPosRight += (nW + TABEX_SPLITE_HEADBUTTON);
					//hidden current&next
					for(int nn = n; nn >= 0; nn--)
					{
						_lstRightCtls->GetKey(nn)->SetVisible(false);
					}
					break;
				}
			}
			if(_lstRightCtls->GetSize() > 0)
			{
				nPosRight -= TABEX_SPLITE_HEAD_RBUTTON;
			}
		}
		else
		{
			//hidden this,so Visible of this button is not valid
			for(int n = _lstRightCtls->GetSize() - 1; n >= 0; n--)
			{
				_lstRightCtls->GetKey(n)->SetVisible(false);
			}
		}
	}
	return nPosRight;
}

void TabControlEx::CalcuteHeadInfo(int nMustShowIndex)
{
	CC_ASSERT(::IsWindow(m_hWnd));

	_StyleArray[(int)StyleType::Outer] |= OuterStyle::Sizing;
	int nH = _rectHead.bottom - _rectHead.top - 2;
	if(_lstControls == NULL || _lstControls->GetSize() == 0)
	{
		_nHeadFirstIndex = -1;
		_nHeadLastIndex = -2;
		if(_eAlignment == TabAlignment::Bottom || _eAlignment == TabAlignment::Top)
		{
			//show right buttons first
			CalcuteHeadInfoRButton(true, _rectHead.left, _rectHead.top + 1, _rectHead.right, nH);
		}
		_StyleArray[(int)StyleType::Outer] &= ~OuterStyle::Sizing;
		return;
	}
	if(_pToolPriv != NULL)
	{
		_pToolPriv->SetVisible(false);
	}
	if(_pToolNext != NULL)
	{
		_pToolNext->SetVisible(false);
	}
	if(_pToolTop != NULL)
	{
		_pToolTop->SetVisible(false);
	}
	if(_pToolEnd != NULL)
	{
		_pToolEnd->SetVisible(false);
	}
	if(_eAlignment == TabAlignment::None)
	{
		//not draw any head
		_nHeadLastIndex = -2;
		CalcuteHeadInfoRButton(false, 0, 0, 0, 0);
		_StyleArray[(int)StyleType::Outer] &= ~OuterStyle::Sizing;
		return;
	}
	int nCnt = _lstControls->GetSize();
	int nAllWidth = 0;
	int nAllWidthBest = 0;
	for(int n = 0; n < nCnt; n++)
	{
		TabPage* ctl = (TabPage*)_lstControls->GetKey(n);
		HeadInfo* pHI = (HeadInfo*)ctl->PageData;
		nAllWidth += pHI->width;
		if(pHI->width > _nBestHeadWidth)
		{
			nAllWidthBest += _nBestHeadWidth;
		}
		else
		{
			nAllWidthBest += pHI->width;
		}
	}
	if(_nHeadFirstIndex < 0)
	{
		_nHeadFirstIndex = 0;
	}
	if(_nHeadFirstIndex >= nCnt)
	{
		_nHeadFirstIndex = nCnt - 1;
	}
	//nStart
	int nW;
	int nStart;
	int nEnd;
	bool isCalcuteNextTool = true;
	RECT ctlRect;
	bool isUseBestWidth = false;
	bool isShowPrivNext = false;
	if(_eAlignment == TabAlignment::Bottom || _eAlignment == TabAlignment::Top)
	{
		nStart = _rectHead.left;
		nEnd = _rectHead.right;
		//show right buttons first
		nEnd = CalcuteHeadInfoRButton(true, _rectHead.left, _rectHead.top + 1, _rectHead.right, nH);

		if(!HasTabStyle(TabStyle::PrevShowLeft|TabStyle::PrevShowRight|TabStyle::NextShowLeft|TabStyle::NextShowRight))
		{
			//auto show priv&next
			if(nEnd - nStart < nAllWidthBest)
			{
				//need show
				isShowPrivNext = true;
			}
		}
		if(HasTabStyle(TabStyle::PrevShowLeft) && !HasTabStyle(TabStyle::PrevShowRight))
		{
			if(HasTabStyle(TabStyle::TopEndShow))
			{
				ctlRect = _pToolTop->CalcuteRect(false, 0, _rectHead.top + 1, -1, nH);
				nW = ctlRect.right - ctlRect.left;
				_pToolTop->SetBoundsNotOriginal(nStart, ctlRect.top, nW, ctlRect.bottom - ctlRect.top, BoundsSpecified::All);
				if(nStart + nW < nEnd)
				{
					_pToolTop->SetVisible(true);
					nStart += nW + TABEX_SPLITE_HEADBUTTON;
				}
				else
				{
					isCalcuteNextTool = false;
				}
			}
			if(isCalcuteNextTool)
			{
				ctlRect = _pToolPriv->CalcuteRect(false, 0, _rectHead.top + 1, -1, nH);
				nW = ctlRect.right - ctlRect.left;
				_pToolPriv->SetBoundsNotOriginal(nStart, ctlRect.top, nW, ctlRect.bottom - ctlRect.top, BoundsSpecified::All);
				if(nStart + nW < nEnd)
				{
					_pToolPriv->SetVisible(true);
					nStart += nW + TABEX_SPLITE_HEADBUTTON;
				}
				else
				{
					isCalcuteNextTool = false;
				}
			}
		}
		if(HasTabStyle(TabStyle::NextShowLeft) && isCalcuteNextTool && !HasTabStyle(TabStyle::NextShowRight))
		{
			ctlRect = _pToolNext->CalcuteRect(false, 0, _rectHead.top + 1, -1, nH);
			nW = ctlRect.right - ctlRect.left;
			_pToolNext->SetBoundsNotOriginal(nStart, ctlRect.top, nW, ctlRect.bottom - ctlRect.top, BoundsSpecified::All);
			if(nStart + nW < nEnd)
			{
				_pToolNext->SetVisible(true);
				nStart += nW + TABEX_SPLITE_HEADBUTTON;
			}
			else
			{
				isCalcuteNextTool = false;
			}
			if(HasTabStyle(TabStyle::TopEndShow) && isCalcuteNextTool)
			{
				ctlRect = _pToolEnd->CalcuteRect(false, 0, _rectHead.top + 1, -1, nH);
				nW = ctlRect.right - ctlRect.left;
				_pToolEnd->SetBoundsNotOriginal(nStart, ctlRect.top, nW, ctlRect.bottom - ctlRect.top, BoundsSpecified::All);
				if(nStart + nW < nEnd)
				{
					_pToolEnd->SetVisible(true);
					nStart += nW + TABEX_SPLITE_HEADBUTTON;
				}
				else
				{
					isCalcuteNextTool = false;
				}
			}
		}
		if((HasTabStyle(TabStyle::NextShowRight) || isShowPrivNext) && isCalcuteNextTool)
		{
			if(HasTabStyle(TabStyle::TopEndShow))
			{
				ctlRect = _pToolEnd->CalcuteRect(false, 0, _rectHead.top + 1, -1, nH);
				nW = ctlRect.right - ctlRect.left;
				_pToolEnd->SetBoundsNotOriginal(nEnd - nW, ctlRect.top, nW, ctlRect.bottom - ctlRect.top, BoundsSpecified::All);
				if(nEnd - nW > nStart)
				{
					_pToolEnd->SetVisible(true);
					nEnd -= nW + TABEX_SPLITE_HEADBUTTON;
				}
				else
				{
					isCalcuteNextTool = false;
				}
			}
			if(isCalcuteNextTool)
			{
				ctlRect = _pToolNext->CalcuteRect(false, 0, _rectHead.top + 1, -1, nH);
				nW = ctlRect.right - ctlRect.left;
				_pToolNext->SetBoundsNotOriginal(nEnd - nW, ctlRect.top, nW, ctlRect.bottom - ctlRect.top, BoundsSpecified::All);
				if(nEnd - nW > nStart)
				{
					_pToolNext->SetVisible(true);
					nEnd -= nW + TABEX_SPLITE_HEADBUTTON;
				}
				else
				{
					isCalcuteNextTool = false;
				}
			}
		}
		if((HasTabStyle(TabStyle::PrevShowRight) || isShowPrivNext) && isCalcuteNextTool)
		{
			ctlRect = _pToolPriv->CalcuteRect(false, 0, _rectHead.top + 1, -1, nH);
			nW = ctlRect.right - ctlRect.left;
			_pToolPriv->SetBoundsNotOriginal(nEnd - nW, ctlRect.top, nW, ctlRect.bottom - ctlRect.top, BoundsSpecified::All);
			if(nEnd - nW > nStart)
			{
				_pToolPriv->SetVisible(true);
				nEnd -= nW + TABEX_SPLITE_HEADBUTTON;
			}
			else
			{
				isCalcuteNextTool = false;
			}
			if(HasTabStyle(TabStyle::TopEndShow) && isCalcuteNextTool)
			{
				ctlRect = _pToolTop->CalcuteRect(false, 0, _rectHead.top + 1, -1, nH);
				nW = ctlRect.right - ctlRect.left;
				_pToolTop->SetBoundsNotOriginal(nEnd - nW, ctlRect.top, nW, ctlRect.bottom - ctlRect.top, BoundsSpecified::All);
				if(nEnd - nW > nStart)
				{
					_pToolTop->SetVisible(true);
					nEnd -= nW + TABEX_SPLITE_HEADBUTTON;
				}
				else
				{
					isCalcuteNextTool = false;
				}
			}
		}
		if(nEnd != _rectHead.right)
		{
			nEnd -= 3;
		}

		//
		//calcute pagehead
		//
		int nValidLen = nEnd - nStart;
		if(nValidLen <= 10)
		{
			//not draw any head
			_nHeadLastIndex = -2;
			_StyleArray[(int)StyleType::Outer] &= ~OuterStyle::Sizing;
			return;
		}
		if(nValidLen < nAllWidth)
		{
			isUseBestWidth = true;
		}
		if(nValidLen >= nAllWidthBest)
		{
			_nHeadFirstIndex = 0;
		}
		else
		{
			//seak best _nHeadFirstIndex
			if(nMustShowIndex >= 0)
			{
				if(nMustShowIndex <= _nHeadFirstIndex)
				{
					_nHeadFirstIndex = nMustShowIndex;
				}
				else
				{
					int nTmpW = 0;
					for(int n = nMustShowIndex; n >= _nHeadFirstIndex; n--)
					{
						TabPage* ctl = (TabPage*)_lstControls->GetKey(n);
						HeadInfo* pHI = (HeadInfo*)ctl->PageData;
						int nItemW = pHI->width;
						if(isUseBestWidth && nItemW > _nBestHeadWidth)
						{
							nItemW = _nBestHeadWidth;
						}
						nTmpW += nItemW;
						if(nTmpW > nValidLen)
						{
							_nHeadFirstIndex = n + 1;
							break;
						}
					}
					if(_nHeadFirstIndex >= nMustShowIndex)
					{
						_nHeadFirstIndex = nMustShowIndex;
					}
				}
			}
		}

		//determine extra space for stretchy pane
		int cxExtra = 0;
		int nStretchyCount = 0;
		if(isUseBestWidth)
		{
			cxExtra = nValidLen;
			for(int n = _nHeadFirstIndex; n < nCnt; n++)
			{
				TabPage* ctl = (TabPage*)_lstControls->GetKey(n);
				HeadInfo* pHI = (HeadInfo*)ctl->PageData;
				int nItemW = pHI->width;
				if(nItemW > _nBestHeadWidth)
				{
					++nStretchyCount;
					nItemW = _nBestHeadWidth;
				}
				cxExtra -= nItemW;
				if(cxExtra < 28)
				{
					break;
				}
			}
		}

		//headright space for righticon
		int nRIconLen = 0;
		if(_lstHeadIconsID != NULL)
		{
			nRIconLen = _sizeHeadIcon.cx * _lstHeadIconsID->GetSize();
			if(nRIconLen != 0)
			{
				//have 1 space to text
				nRIconLen += 3;
			}
		}
		if(nStretchyCount <= 0)
		{
			nStretchyCount = 1;
		}
		//first pagehead's left is +2
		int nRight = nStart + 2;
		for(int n = _nHeadFirstIndex; n < nCnt; n++)
		{
			TabPage* ctl = (TabPage*)_lstControls->GetKey(n);
			HeadInfo* pHI = (HeadInfo*)ctl->PageData;
			int nItemW = pHI->width;
			if(isUseBestWidth && nItemW > _nBestHeadWidth)
			{
				nItemW = _nBestHeadWidth;

				if(cxExtra > 0)
				{
					int cxAddExtra = cxExtra / (nStretchyCount <= 0 ? 1 : nStretchyCount);
					if(cxAddExtra + nItemW > pHI->width)
					{
						cxAddExtra = pHI->width - nItemW;
					}
					nItemW += cxAddExtra;
					--nStretchyCount;
					cxExtra -= cxAddExtra;
				}
			}
			if(nRight + 24 > nEnd)
			{
				//if short than icon,break
				break;
			}
			_nHeadLastIndex = n;
			int nRectRight = nRight + nItemW;
			if(nRectRight > nEnd - 2)
			{
				nRectRight = nEnd - 2;
			}
			::SetRect(&pHI->rect, nRight, _rectHead.top, nRectRight, _rectHead.bottom);
			int nTextL = 0;
			if(_eAlignment == TabAlignment::Top)
			{
				if(ctl->GetIcon() != NULL)
				{
					pHI->ptIcon.x =    nRight + TABEX_PAGEHEAD_MARGINL;
					pHI->ptIcon.y =    _rectHead.top + TABEX_PAGEHEAD_MARGINT + 1;
					pHI->ptIconAct.x = nRight + TABEX_PAGEHEAD_MARGINL;
					pHI->ptIconAct.y = _rectHead.top + TABEX_PAGEHEAD_MARGINT;
					nTextL = _sizeHeadIcon.cx + 3;
				}
				if(nRIconLen != 0)
				{
					::SetRect(&pHI->rectRIcon,    nRectRight - TABEX_PAGEHEAD_MARGINR - nRIconLen + 1, _rectHead.top + TABEX_PAGEHEAD_MARGINT + 1, nRectRight - TABEX_PAGEHEAD_MARGINR, _rectHead.bottom - TABEX_PAGEHEAD_MARGINB);
					::SetRect(&pHI->rectRIconAct, nRectRight - TABEX_PAGEHEAD_MARGINR - nRIconLen + 1, _rectHead.top + TABEX_PAGEHEAD_MARGINT    , nRectRight - TABEX_PAGEHEAD_MARGINR, _rectHead.bottom - TABEX_PAGEHEAD_MARGINB - 1);
					pHI->nRIconAdjustX = -_sizeHeadIcon.cx;
					pHI->nRIconAdjustY = 0;
				}
				::SetRect(&pHI->rectTextAct, nRight + TABEX_PAGEHEAD_MARGINL + nTextL, _rectHead.top + TABEX_PAGEHEAD_MARGINT + 1, nRectRight - TABEX_PAGEHEAD_MARGINR - nRIconLen, _rectHead.bottom - TABEX_PAGEHEAD_MARGINB - 1);
				//while not active,not draw icon
				::SetRect(&pHI->rectText,    nRight + TABEX_PAGEHEAD_MARGINL + nTextL, _rectHead.top + TABEX_PAGEHEAD_MARGINT + 2, nRectRight - TABEX_PAGEHEAD_MARGINR, _rectHead.bottom - TABEX_PAGEHEAD_MARGINB);

				pHI->flagEdge = BF_SOFT | BF_LEFT | BF_TOP | BF_RIGHT;
				//2008/11/20 changed
				//::SetRect(&pHI->rectEdge,    nRight + 1, _rectHead.top + 2, nRectRight + 1, _rectHead.bottom);
				//::SetRect(&pHI->rectEdge,    nRight + 1, _rectHead.top + 2, nRectRight + 1, _rectHead.bottom + 1);
				::SetRect(&pHI->rectEdge,      nRight + 2, _rectHead.top + 2, nRectRight, _rectHead.bottom);
				//::SetRect(&pHI->rectEdgeAct, nRight - 1, _rectHead.top, nRectRight + 2, _rectHead.bottom + 1);
				::SetRect(&pHI->rectEdgeAct,   nRight + 1, _rectHead.top, nRectRight + 1, _rectHead.bottom);
				//2008/11/20 changed
				//::SetRect(&pHI->rectErase,   nRight + 2, _rectHead.top + 3, nRectRight - 2, _rectHead.bottom - 1);
				//::SetRect(&pHI->rectErase,   nRight + 2, _rectHead.top + 3, nRectRight - 2, _rectHead.bottom + 1);
				::SetRect(&pHI->rectErase,     nRight + 3, _rectHead.top + 3, nRectRight - 3, _rectHead.bottom);
				::SetRect(&pHI->rectEraseAct,  nRight - 0, _rectHead.top + 1, nRectRight - 0, _rectHead.bottom + 2);
			}
			else
			{
				if(ctl->GetIcon() != NULL)
				{
					pHI->ptIcon.x = nRight + TABEX_PAGEHEAD_MARGINL;
					pHI->ptIcon.y = _rectHead.top + TABEX_PAGEHEAD_MARGINT - 2;
					pHI->ptIconAct.x = nRight + TABEX_PAGEHEAD_MARGINL;
					pHI->ptIconAct.y = _rectHead.top + TABEX_PAGEHEAD_MARGINT - 1;
					nTextL = _sizeHeadIcon.cx + 3;
				}
				if(nRIconLen != 0)
				{
					::SetRect(&pHI->rectRIcon,    nRectRight - TABEX_PAGEHEAD_MARGINR - nRIconLen + 1, _rectHead.top + TABEX_PAGEHEAD_MARGINT - 1, nRectRight - TABEX_PAGEHEAD_MARGINR, _rectHead.bottom - TABEX_PAGEHEAD_MARGINB - 2);
					::SetRect(&pHI->rectRIconAct, nRectRight - TABEX_PAGEHEAD_MARGINR - nRIconLen + 1, _rectHead.top + TABEX_PAGEHEAD_MARGINT - 2, nRectRight - TABEX_PAGEHEAD_MARGINR, _rectHead.bottom - TABEX_PAGEHEAD_MARGINB - 3);
					pHI->nRIconAdjustX = -_sizeHeadIcon.cx;
					pHI->nRIconAdjustY = 0;
				}
				::SetRect(&pHI->rectTextAct, nRight + TABEX_PAGEHEAD_MARGINL + nTextL, _rectHead.top + TABEX_PAGEHEAD_MARGINT - 3 + 2, nRectRight - TABEX_PAGEHEAD_MARGINR - nRIconLen, _rectHead.bottom - TABEX_PAGEHEAD_MARGINB);
				//while not active,not draw icon
				::SetRect(&pHI->rectText,    nRight + TABEX_PAGEHEAD_MARGINL + nTextL, _rectHead.top + TABEX_PAGEHEAD_MARGINT - 3, nRectRight - TABEX_PAGEHEAD_MARGINR, _rectHead.bottom - TABEX_PAGEHEAD_MARGINB);

				pHI->flagEdge = BF_SOFT | BF_LEFT | BF_BOTTOM | BF_RIGHT;
				//::SetRect(&pHI->rectEdge,    nRight + 1, _rectHead.top, nRectRight + 1, _rectHead.bottom - 1);
				::SetRect(&pHI->rectEdge,    nRight + 2, _rectHead.top, nRectRight + 0, _rectHead.bottom - 1);
				//::SetRect(&pHI->rectEdgeAct, nRight - 1, _rectHead.top - 2, nRectRight + 2, _rectHead.bottom);
				::SetRect(&pHI->rectEdgeAct, nRight + 1, _rectHead.top - 0, nRectRight + 1, _rectHead.bottom);
				::SetRect(&pHI->rectErase,   nRight + 2, _rectHead.top + 1, nRectRight - 2, _rectHead.bottom - 3);
				::SetRect(&pHI->rectEraseAct,nRight - 0, _rectHead.top - 3, nRectRight - 0, _rectHead.bottom - 2);
			}
			if(nRectRight >= nEnd - 2)
			{
				break;
			}
			else
			{
				nRight += nItemW;
			}
		}
	}
	else
	{
		nStart = _rectHead.top;
		nEnd = _rectHead.bottom;
		CalcuteHeadInfoRButton(false, 0, 0, 0, 0);
		if(_nVHeadWidth <= 0)
		{
			if(!HasTabStyle(TabStyle::PrevShowLeft|TabStyle::PrevShowRight|TabStyle::NextShowLeft|TabStyle::NextShowRight))
			{
				//auto show priv&next
				if(nEnd - nStart < nAllWidthBest)
				{
					//need show
					isShowPrivNext = true;
				}
			}
			int nHeadWidth = _rectHead.right - _rectHead.left - 2;
			if(isShowPrivNext || HasTabStyle(TabStyle::PrevShowLeft|TabStyle::PrevShowRight))
			{
				if(HasTabStyle(TabStyle::TopEndShow))
				{
					_pToolTop->SetBoundsNotOriginal(_rectHead.left + 1, nStart, nHeadWidth, TABEX_TOOLITEM_VHEIGHT, BoundsSpecified::All);
					_pToolTop->SetVisible(true);
					nStart += TABEX_TOOLITEM_VHEIGHT;
				}
				_pToolPriv->SetBoundsNotOriginal(_rectHead.left + 1, nStart, nHeadWidth, TABEX_TOOLITEM_VHEIGHT, BoundsSpecified::All);
				_pToolPriv->SetVisible(true);
				nStart += TABEX_TOOLITEM_VHEIGHT;
			}
			if(isShowPrivNext || HasTabStyle(TabStyle::NextShowLeft|TabStyle::NextShowRight))
			{
				_pToolNext->SetBoundsNotOriginal(_rectHead.left + 1, nStart, nHeadWidth, TABEX_TOOLITEM_VHEIGHT, BoundsSpecified::All);
				_pToolNext->SetVisible(true);
				nStart += TABEX_TOOLITEM_VHEIGHT;
				if(HasTabStyle(TabStyle::TopEndShow))
				{
					_pToolEnd->SetBoundsNotOriginal(_rectHead.left + 1, nStart, nHeadWidth, TABEX_TOOLITEM_VHEIGHT, BoundsSpecified::All);
					_pToolEnd->SetVisible(true);
					nStart += TABEX_TOOLITEM_VHEIGHT;
				}
			}

			//
			//calcute pagehead
			//
			int nValidLen = nEnd - nStart;
			if(nValidLen <= 10)
			{
				//not draw any head
				_nHeadLastIndex = -2;
				_StyleArray[(int)StyleType::Outer] &= ~OuterStyle::Sizing;
				return;
			}
			if(nValidLen >= nAllWidthBest)
			{
				_nHeadFirstIndex = 0;
			}
			if(nValidLen < nAllWidth)
			{
				isUseBestWidth = true;
			}
			int nRight = nStart;
			for(int n = _nHeadFirstIndex; n < nCnt; n++)
			{
				TabPage* ctl = (TabPage*)_lstControls->GetKey(n);
				HeadInfo* pHI = (HeadInfo*)ctl->PageData;
				int nItemW = pHI->width;
				if(isUseBestWidth && nItemW > _nBestHeadWidth)
				{
					nItemW = _nBestHeadWidth;
				}
				_nHeadLastIndex = n;
				if(nRight + nItemW < nEnd)
				{
					::SetRect(&pHI->rect, _rectHead.left + 1, nRight, _rectHead.left + nHeadWidth, nRight + nItemW);
					nRight += nItemW;
				}
				else
				{
					::SetRect(&pHI->rect, _rectHead.left + 1, nRight, _rectHead.left + nHeadWidth, nRight + nEnd - nItemW);
					nRight += nEnd - nItemW;
					break;
				}
			}
		}
		else
		{
			if(!HasTabStyle(TabStyle::PrevShowLeft|TabStyle::PrevShowRight|TabStyle::NextShowLeft|TabStyle::NextShowRight))
			{
				//auto show priv&next
				if(nEnd - nStart - _nHeadHeight - 2 < nAllWidthBest)
				{
					//need show
					isShowPrivNext = true;
				}
			}
			nH = _nHeadHeight;
			int nLineLeft = _rectHead.left;
			if(isShowPrivNext || HasTabStyle(TabStyle::PrevShowLeft|TabStyle::PrevShowRight))
			{
				if(HasTabStyle(TabStyle::TopEndShow))
				{
					ctlRect = _pToolTop->CalcuteRect(false, 0, _rectHead.top + 1, -1, nH);
					nW = ctlRect.right - ctlRect.left;
					_pToolTop->SetBoundsNotOriginal(nLineLeft, ctlRect.top, nW, ctlRect.bottom - ctlRect.top, BoundsSpecified::All);
					_pToolTop->SetVisible(true);
					nLineLeft += nW + TABEX_SPLITE_HEADBUTTON;
				}
				ctlRect = _pToolPriv->CalcuteRect(false, 0, _rectHead.top + 1, -1, nH);
				nW = ctlRect.right - ctlRect.left;
				_pToolPriv->SetBoundsNotOriginal(nLineLeft, ctlRect.top, nW, ctlRect.bottom - ctlRect.top, BoundsSpecified::All);
				_pToolPriv->SetVisible(true);
				nLineLeft += nW + TABEX_SPLITE_HEADBUTTON;
			}
			if(isShowPrivNext || HasTabStyle(TabStyle::NextShowLeft|TabStyle::NextShowRight))
			{
				ctlRect = _pToolNext->CalcuteRect(false, 0, _rectHead.top + 1, -1, nH);
				nW = ctlRect.right - ctlRect.left;
				_pToolNext->SetBoundsNotOriginal(nLineLeft, ctlRect.top, nW, ctlRect.bottom - ctlRect.top, BoundsSpecified::All);
				_pToolNext->SetVisible(true);
				nLineLeft += nW + TABEX_SPLITE_HEADBUTTON;
				if(HasTabStyle(TabStyle::TopEndShow))
				{
					ctlRect = _pToolEnd->CalcuteRect(false, 0, _rectHead.top + 1, -1, nH);
					nW = ctlRect.right - ctlRect.left;
					_pToolEnd->SetBoundsNotOriginal(nLineLeft, ctlRect.top, nW, ctlRect.bottom - ctlRect.top, BoundsSpecified::All);
					_pToolEnd->SetVisible(true);
					nLineLeft += nW + TABEX_SPLITE_HEADBUTTON;
				}
			}
			//if have space,want to show right buttons
			int newRight = CalcuteHeadInfoRButton(true, nLineLeft, _rectHead.top + 1, _rectHead.right, nH);
			if(nLineLeft != _rectHead.left || newRight != _rectHead.right)
			{
				nStart += _nHeadHeight + 2;
			}

			//calcute pagehead
			int nValidLen = nEnd - nStart;
			if(nValidLen <= 10)
			{
				//not draw any head
				_nHeadLastIndex = -2;
				_StyleArray[(int)StyleType::Outer] &= ~OuterStyle::Sizing;
				return;
			}
			int nRight = nStart;
			for(int n = _nHeadFirstIndex; n < nCnt; n++)
			{
				TabPage* ctl = (TabPage*)_lstControls->GetKey(n);
				HeadInfo* pHI = (HeadInfo*)ctl->PageData;
				if(nRight + _nHeadHeight < nEnd)
				{
					::SetRect(&pHI->rect, _rectHead.left + 1, nRight, _rectHead.left + _nVHeadWidth, nRight + _nHeadHeight);
					nRight += _nHeadHeight;
				}
				else
				{
					break;
				}
				_nHeadLastIndex = n;
			}
		}
	}
	_StyleArray[(int)StyleType::Outer] &= ~OuterStyle::Sizing;
}

void TabControlEx::OnPaint(PaintEventArgs *e)
{
	HDC hDC = BeginPaint(m_hWnd, &(e->data));
	//If GetUpdateRect returns zero, the application should not call the BeginPaint and EndPaint functions.
	if(e->data.rcPaint.bottom != 0 && e->data.rcPaint.right != 0)
	{
		//clear back
		bool isTransparent = this->HasStyle(StyleType::Outer, OuterStyle::TransparentParentErase);
		if(!isTransparent)
		{
			//clear back
			::SetBkColor(hDC, GetBackground());
			//::ExtTextOut(hDC, 0, 0, ETO_OPAQUE, &_rectBody, NULL, 0, NULL);
			//while FlatButtons:Microsoft Visual Studio's head,set head backcolor
			if(_eAppearance == TabAppearance::FlatButtons)
			{
				::SetBkColor(hDC, _corHeadBackColor);
			}
			::ExtTextOut(hDC, 0, 0, ETO_OPAQUE, &_rectHead, NULL, 0, NULL);
		}
		else
		{
			//let parent to EraseBackground
			//HRGN hRgn = CreateRectRgn(_rectHead.left, _rectHead.top, _rectHead.right, _rectHead.bottom);
			//ExtSelectClipRgn(hDC, hRgn, RGN_AND); //RGN_COPY
			::SendMessage(this->m_hWnd, WM_ERASEBKGND, (WPARAM)hDC, 0);
			//SelectClipRgn(hDC, NULL);
			//DeleteObject(hRgn);
		}
		::SetBkMode(hDC, TRANSPARENT);
		::SetBkColor(hDC, GetBackground());
		if(HasTabStyle(TabStyle::NoBodyRect))
		{
			if(!isTransparent)
			{
				if(_eAlignment == TabAlignment::Top)
				{
					::DrawEdge(hDC, &_rectBody, BDR_RAISEDINNER|BDR_RAISEDOUTER, BF_TOP);
				}
				else if(_eAlignment == TabAlignment::Bottom)
				{
					::DrawEdge(hDC, &_rectBody, BDR_RAISEDINNER|BDR_RAISEDOUTER, BF_BOTTOM);
				}
			}
			else
			{
				HPEN pLine = CreatePen(PS_INSIDEFRAME | PS_SOLID, 2, TabControlExConfig::corNoBodyRectColor);
				HPEN pOldPen = (HPEN)SelectObject(hDC, pLine);
				if(_eAlignment == TabAlignment::Top)
				{
					//::DrawEdge(hDC, &_rectBody, BDR_RAISEDINNER|BDR_RAISEDOUTER, BF_TOP);
					::MoveToEx(hDC, _rectBody.left, _rectBody.top + 1, NULL);
					::LineTo(hDC, _rectBody.right, _rectBody.top + 1);
				}
				else if(_eAlignment == TabAlignment::Bottom)
				{
					//::DrawEdge(hDC, &_rectBody, BDR_RAISEDINNER|BDR_RAISEDOUTER, BF_BOTTOM);
					::MoveToEx(hDC, _rectBody.left, _rectBody.bottom - 1, NULL);
					::LineTo(hDC, _rectBody.right, _rectBody.bottom - 1);
				}
				SelectObject(hDC, pOldPen);
				::DeleteObject(pLine);
			}
		}
		else
		{
			::DrawEdge(hDC, &_rectBody, BDR_RAISEDINNER|BDR_RAISEDOUTER, BF_RECT);
			//modify for bug,this dot is not need
			::SetPixel(hDC, _rectBody.left, _rectBody.bottom - 1, GetBackground());
		}

		HFONT holdFont = (HFONT)::SelectObject(hDC, GetFont());
		for(int n = _nHeadFirstIndex; n <= _nHeadLastIndex; n++)
		{
			if(_nSelectedIndex != n)
			{
				SetHit(n, hDC, false, true);
			}
		}
		if(_nSelectedIndex >= _nHeadFirstIndex && _nSelectedIndex <= _nHeadLastIndex)
		{
			//draw curuent page
			SetHit(_nSelectedIndex, hDC, false, true);
		}
		::SelectObject(hDC, holdFont);
		::DeleteObject(holdFont);
	}
#ifdef _DEBUG_xz
	static HFONT hFont;
  //hFont = CreateFont(100,0,900,0,500,FALSE,FALSE,FALSE,0,0,0,0,0,L"?l?r ?o?S?V?b?N");
	//define the logical parameters for the default font
	hFont = ::CreateFont(
		-12,					// height of font,size 8:-11;
		0,						// average character width
		0,						// angle of escapement
		0,						// base-line orientation angle
		FW_NORMAL,				// font weight
		FALSE,					// italic attribute option
		FALSE,					// underline attribute option
		0,						// strikeout attribute option
		DEFAULT_CHARSET,		// character set identifier
		OUT_DEFAULT_PRECIS,		// output precision
		CLIP_DEFAULT_PRECIS,	// clipping precision
		DEFAULT_QUALITY,		// output quality
		DEFAULT_PITCH,			// pitch and family
		_T("MS UI Gothic")		// typeface name
	);
		LOGFONT lf;
		::GetObject(hFont, sizeof(lf), &lf);
		//be deleted while setfont
		//::DeleteObject(hFont);
		lf.lfOutPrecision = OUT_STROKE_PRECIS;
if(_eAlignment == TabAlignment::Top)
{
		lf.lfEscapement = 0;
}
else if(_eAlignment == TabAlignment::Bottom)
{
		lf.lfEscapement = 900;
}
else if(_eAlignment == TabAlignment::Right)
{
		lf.lfEscapement = 1800;
}
else
{
		lf.lfEscapement = 2700;
}
		lf.lfOrientation = 0;
		//lf.lfClipPrecision = CLIP_STROKE_PRECIS;
		lf.lfClipPrecision = CLIP_LH_ANGLES;
		lf.lfQuality = DRAFT_QUALITY;
		lf.lfPitchAndFamily = VARIABLE_PITCH|FF_MODERN;
		lf.lfHeight = 12;
		lf.lfWeight = FW_NORMAL;
		lf.lfItalic = FALSE;
		lf.lfUnderline = FALSE;
		lf.lfStrikeOut = FALSE;
			_tcscpy(lf.lfFaceName, _T("Marlett"));
		hFont = ::CreateFontIndirect(&lf);
      SelectObject(hDC,hFont);
      //TextOut(hDC,0,100,L"?V?\??",3);
	RECT rect;
	rect.left = 50;
	rect.top = 50;
	rect.right = 100;
	rect.bottom = 100;
	::DrawText(hDC, _T("7"), -1, &rect, DT_LEFT|DT_SINGLELINE|DT_VCENTER);

      DeleteObject(SelectObject(hDC,hFont));
#endif
	EndPaint(m_hWnd, &(e->data));
}

void TabControlEx::DrawHead(TabPage *page, HDC hDC, bool isActive, bool isMouseOn, bool isEdge)
{
	HeadInfo* pHI = (HeadInfo*)page->PageData;
	bool isTransparent = this->HasStyle(StyleType::Outer, OuterStyle::TransparentParentErase);
	if(isTransparent)
	{
		//let parent to EraseBackground
		int nAdd = 0;
		if(isActive)
		{
			nAdd = 2;
		}
		HRGN hRgn = CreateRectRgn(pHI->rect.left + 2, pHI->rect.top - nAdd, pHI->rect.right - 1, pHI->rect.bottom + nAdd);
		ExtSelectClipRgn(hDC, hRgn, RGN_AND); //RGN_COPY
		::SendMessage(m_hWnd, WM_ERASEBKGND, (WPARAM)hDC, (LPARAM)this);
		SelectClipRgn(hDC, NULL);
		DeleteObject(hRgn);
	}
	if(pHI->rect.right - pHI->rect.left < 6)
	{
		return;
	}

	//set bkcolor,use page's hot back color
	::SetBkColor(hDC, (isActive || isMouseOn) ? page->GetBackground() : page->GetHotBackColor());
	if(isActive)
	{
		if(!isTransparent)
		{
			::ExtTextOut(hDC, 0, 0, ETO_OPAQUE, &pHI->rectEraseAct, NULL, 0, NULL);
		}
		//if(isEdge)
		//if(HasTabStyle(TabStyle::FlatTitle))
		//{
		//	//::DrawEdge(hDC, &pHI->rectEdgeAct, BDR_RAISEDINNER|BDR_RAISEDOUTER, BF_LEFT|BF_RIGHT|BF_SOFT);
		//}
		//else
		//{
			::DrawEdge(hDC, &pHI->rectEdgeAct, BDR_RAISEDINNER|BDR_RAISEDOUTER, pHI->flagEdge);
		//}
		if(HasTabStyle(TabStyle::NoBodyRect))
		{
			HPEN pLine = CreatePen(PS_INSIDEFRAME | PS_SOLID, 2, TabControlExConfig::corNoBodyRectColor);
			HPEN pOldPen = (HPEN)SelectObject(hDC, pLine);
			if(_eAlignment == TabAlignment::Top)
			{
				::MoveToEx(hDC, pHI->rectEdgeAct.left + 2, pHI->rectEdgeAct.bottom + 1, NULL);
				::LineTo(hDC, pHI->rectEdgeAct.left + 2, pHI->rectEdgeAct.top + 1);
				::MoveToEx(hDC, pHI->rectEdgeAct.left + 2, pHI->rectEdgeAct.top + 2, NULL);
				::LineTo(hDC, pHI->rectEdgeAct.right - 3, pHI->rectEdgeAct.top + 2);
				::LineTo(hDC, pHI->rectEdgeAct.right - 3, pHI->rectEdgeAct.bottom + 1);
			}
			else if(_eAlignment == TabAlignment::Bottom)
			{
				::MoveToEx(hDC, pHI->rectEdgeAct.left + 2, pHI->rectEdgeAct.top - 2, NULL);
				::LineTo(hDC, pHI->rectEdgeAct.left + 2, pHI->rectEdgeAct.bottom - 2);
				::LineTo(hDC, pHI->rectEdgeAct.right - 3, pHI->rectEdgeAct.bottom - 2);
				::LineTo(hDC, pHI->rectEdgeAct.right - 3, pHI->rectEdgeAct.top - 2);
			}
			SelectObject(hDC, pOldPen);
			::DeleteObject(pLine);
		}
	}
	else
	{
		if(!isTransparent)
		{
			::ExtTextOut(hDC, 0, 0, ETO_OPAQUE, &pHI->rectErase, NULL, 0, NULL);
		}
		//if(isEdge)
		if(HasTabStyle(TabStyle::FlatTitle))
		{
			if(!isMouseOn && isEdge)
			{
				HPEN pLine = CreatePen(PS_INSIDEFRAME | PS_SOLID, 1, ::GetSysColor(COLOR_BTNSHADOW));
				HPEN pOldPen = (HPEN)SelectObject(hDC, pLine);
				if(_eAlignment == TabAlignment::Top)
				{
					if(pHI->rectEdge.left > 6)
					{
						::MoveToEx(hDC, pHI->rectEdge.left - 2, pHI->rectEdge.bottom - 2, NULL);
						::LineTo(hDC, pHI->rectEdge.left - 2, pHI->rectEdge.top + 2);
					}
				}
				else if(_eAlignment == TabAlignment::Bottom)
				{
					if(pHI->rectEdge.left > 6)
					{
						::MoveToEx(hDC, pHI->rectEdge.left - 2, pHI->rectEdge.bottom - 3, NULL);
						::LineTo(hDC, pHI->rectEdge.left - 2, pHI->rectEdge.top + 1);
					}
				}
				SelectObject(hDC, pOldPen);
				::DeleteObject(pLine);
			}
		}
		else
		{
			::DrawEdge(hDC, &pHI->rectEdge, BDR_RAISEDINNER|BDR_RAISEDOUTER, pHI->flagEdge);
		}
	}

	::SetBkMode(hDC, TRANSPARENT);
	// draw icon,only exist normal icon,then others
	HICON hIcon = page->GetIcon(IconType::Hot);
	if(hIcon != NULL)
	{
		if(!page->GetEnabled())
		{
			hIcon = page->GetIcon(IconType::Disabled);
		}
		else if(isActive || isMouseOn)
		{
		}
		else
		{
			hIcon = page->GetIcon(IconType::Gloom);
		}
	}

	if(hIcon != NULL)
	{
		if(isActive)
		{
			//DrawIconEx(hDC, pHI->ptIconAct.x, pHI->ptIconAct.y, hIcon, 16, 16, 0, NULL, DI_NORMAL);
			CC_APP_GetDrawActor()->DrawIcon(hDC, pHI->ptIconAct.x, pHI->ptIconAct.y, hIcon, _sizeHeadIcon.cx, _sizeHeadIcon.cy, NULL, DI_NORMAL);
		}
		else
		{
			//DrawIconEx(hDC, pHI->ptIcon.x, pHI->ptIcon.y, hIcon, 16, 16, 0, NULL, DI_NORMAL);
			CC_APP_GetDrawActor()->DrawIcon(hDC, pHI->ptIcon.x, pHI->ptIcon.y, hIcon, _sizeHeadIcon.cx, _sizeHeadIcon.cy, NULL, DI_NORMAL);
		}
	}

	int len = page->GetTextLen();
	if(len > 0)
	{
		RECT rect;
		//OuterStyle::PrivateFlag1 is no right icons
		if(isActive || (isMouseOn && _lstHeadIconsID != NULL && page->GetEnabled() && !page->HasStyle(StyleType::Outer, OuterStyle::PrivateFlag1)))
		{
			rect = pHI->rectTextAct;
			//::CopyRect(&rect, &pHI->rectTextAct);
		}
		else
		{
			rect = pHI->rectText;
			//::CopyRect(&rect, &pHI->rectText);
		}
		//use font of tab
		const TCHAR* tcText = page->GetText();
		if(!page->GetEnabled())
		{
			OffsetRect(&rect, 1, 1);
			::SetTextColor(hDC, ::GetSysColor(COLOR_BTNHIGHLIGHT));
			::DrawText(hDC, tcText, -1, &rect, DT_SINGLELINE|DT_VCENTER);
			OffsetRect(&rect, -1, -1);
			::SetTextColor(hDC, ::GetSysColor(COLOR_BTNSHADOW));
			::DrawText(hDC, tcText, -1, &rect, DT_SINGLELINE|DT_VCENTER);
		}
		else
		{
			::SetTextColor(hDC, (isActive || isMouseOn) ? page->GetTextColor() : page->GetHotTextColor());
			::DrawText(hDC, tcText, -1, &rect, DT_SINGLELINE|DT_VCENTER);
		}
	}
	//draw right icon, and OuterStyle::PrivateFlag1 is no right icons
	if((isActive || isMouseOn) && _lstHeadIconsID != NULL && page->GetEnabled() && !page->HasStyle(StyleType::Outer, OuterStyle::PrivateFlag1))
	{
		POINT pt = {-1, -1};
		DrawHeadIcon(page, hDC, pt);
	}
}

void TabControlEx::DrawHeadIcon(TabPage *page, HDC hDC, POINT ptMouse)
{
	CC_ASSERT(_lstHeadIconsID != NULL);

	HeadInfo* pHI = (HeadInfo*)page->PageData;
	RECT rect;
	::CopyRect(&rect, &pHI->rectRIconAct);
	int nCnt = _lstHeadIconsID->GetSize() - 1;
	for(int n = nCnt; n >= 0; n--)
	{
		rect.right += pHI->nRIconAdjustX;
		if(rect.right < rect.left || rect.top >= rect.bottom)
		{
			break;
		}
		int nIconID = _lstHeadIconsID->GetKey(n);
		HICON hIcon;
		RECT rcIcon = {rect.right + 1, rect.top + 1, rect.right + 15, rect.top + 15};
		if(::PtInRect(&rcIcon, ptMouse))
		{
			hIcon = _pImageList->GetIcon(nIconID, IconType::Hot);
		}
		else
		{
			hIcon = _pImageList->GetIcon(nIconID, IconType::Gloom);
		}
		//DrawIconEx(hDC, rect.right, rect.top, hIcon, 16, 16, 0, NULL, DI_NORMAL);
		CC_APP_GetDrawActor()->DrawIcon(hDC, rect.right, rect.top, hIcon, _sizeHeadIcon.cx, _sizeHeadIcon.cy, NULL, DI_NORMAL);
		rect.top += pHI->nRIconAdjustY;
	}
}

void TabControlEx::AddItemListMenu(TabPage *control, int nIndex)
{
	CC_ASSERT(HasTabStyle(TabStyle::PrevShowMenu|TabStyle::NextShowMenu|TabStyle::CreateItemListMenu));
	CC_ASSERT(_pItemListMenu != NULL);

	MenuItem* menu = new MenuItem();
	menu->SetEnabled(control->GetEnabled());
	menu->SetText(control->GetText());
	menu->SetIcon(control->GetImageList(), control->GetIconID());
	//menu->SetIcon(control->GetIcon());
	//menu->SetIcon(control->GetIcon(IconType::Gray), IconType::Gray);
	//menu->SetIcon(control->GetIcon(IconType::Hot), IconType::Hot);
	menu->SetOwnerDraw(true);
	menu->Click += EventHandler((Object*)this, (EventFun)&TabControlEx::HeadDropMenu_OnClick);
	_pItemListMenu->ItemAdd(menu, nIndex);
}

int TabControlEx::PointToIndex(POINT pt) const
{
	if(_lstControls == NULL)
	{
		return -1;
	}
	for(int n = _nHeadFirstIndex; n <= _nHeadLastIndex; n++)
	{
		TabPage* ctl = (TabPage*)_lstControls->GetKey(n);
		HeadInfo* pHI = (HeadInfo*)ctl->PageData;
		if(::PtInRect(&pHI->rect, pt))
		{
			return n;
		}
	}
	return -1;
}

void TabControlEx::OnMouseLeave(EventArgs* e)
{
	Container::OnMouseLeave(e);
	if(!HasStyle(StyleType::Inner, InnerStyle::ContextMenuShowing))
	{
		SetHotHit(-1, false, false);
	}
}

void TabControlEx::OnMouseMove(MouseEventArgs* e)
{
	Container::OnMouseMove(e);
	if(!HasStyle(StyleType::Inner, InnerStyle::ContextMenuShowing) && e->Button == MouseButtons::None)
	{
		POINT pt = {e->X, e->Y};
		int nIndex = PointToIndex(pt);
		if(nIndex != m_nHotIndex)
		{
			SetHotHit(nIndex, nIndex >= 0, false);
		}
		if(_lstHeadIconsID != NULL && nIndex >= 0)
		{
			TabPage* page = (TabPage*)_lstControls->GetKey(nIndex);
			//draw right icon, and OuterStyle::PrivateFlag1 is no right icons
			if(page->GetEnabled() && !page->HasStyle(StyleType::Outer, OuterStyle::PrivateFlag1))
			{
				HDC hDC = ::GetDC(m_hWnd);
				DrawHeadIcon(page, hDC, pt);
				::ReleaseDC(m_hWnd, hDC);
			}
		}
	}
}

void TabControlEx::OnMouseDown(MouseEventArgs* e)
{
	Container::OnMouseDown(e);
	POINT pt = {e->X, e->Y};
	int nIndex = PointToIndex(pt);
	if(nIndex >= 0)
	{
		TabPage* ctl = (TabPage*)_lstControls->GetKey(nIndex);
		if(ctl->GetEnabled())
		{
			SetSelectedIndex(nIndex);
			//OuterStyle::PrivateFlag1 is no right icons
			if(e->Button == MouseButtons::Left && _lstHeadIconsID != NULL && !ctl->HasStyle(StyleType::Outer, OuterStyle::PrivateFlag1))
			{
				HeadInfo* pHI = (HeadInfo*)ctl->PageData;
				RECT rect;
				::CopyRect(&rect, &pHI->rectRIconAct);
				int nCnt = _lstHeadIconsID->GetSize() - 1;
				for(int n = nCnt; n >= 0; n--)
				{
					rect.right += pHI->nRIconAdjustX;
					if(rect.right < rect.left || rect.top >= rect.bottom)
					{
						break;
					}
					RECT rcIcon = {rect.right + 1, rect.top + 1, rect.right + 15, rect.top + 15};
					if(::PtInRect(&rcIcon, pt))
					{
						e->baseArgs->WParam = nCnt - n;
						OnHeadIconClick(e->baseArgs);
						break;
					}
					rect.top += pHI->nRIconAdjustY;
				}
			}
		}
	}
}

void TabControlEx::OnContextMenu(MouseEventArgs* e)
{
	CC_ASSERT(::IsWindow(m_hWnd));

	e->baseArgs->Handled = true;
	e->baseArgs->Result = TRUE;
	POINT pt = {e->X, e->Y};
	::ScreenToClient(m_hWnd, &pt);
	int nIndex = PointToIndex(pt);
	if(nIndex >= 0)
	{
		if(_pHeadContextMenu == NULL || ::GetCapture() != NULL)
		{
			return;
		}
		TabPage* ctl = (TabPage*)_lstControls->GetKey(nIndex);
		if(ctl->GetEnabled())
		{
			if(nIndex != GetSelectedIndex())
			{
				//set clicked index
				SetSelectedIndex(nIndex);
			}
			pt.x = e->X;
			pt.y = e->Y;
			SetStyle(StyleType::Inner, 0, InnerStyle::ContextMenuShowing);
			_pHeadContextMenu->Show(this, pt);
			SetStyle(StyleType::Inner, InnerStyle::ContextMenuShowing, 0);
		}
	}
	else
	{
		//if in _rectHead,do nothing
		if(::PtInRect(&_rectHead, pt))
		{
			return;
		}
		Container::OnContextMenu(e);
	}
}

void TabControlEx::OnDoubleClick(EventArgs* e)
{
	POINT pt;
	::GetCursorPos(&pt);
	::ScreenToClient(m_hWnd, &pt);
	int nIndex = PointToIndex(pt);
	if(nIndex >= 0)
	{
		//eat it
		e->Handled = true;
		e->Result = TRUE;
		Control* ctl = _lstControls->GetKey(nIndex);
		if(!ctl->GetEnabled())
		{
			return;
		}
		HeadDoubleClick.Invoke((Object*)this, e);
	}
	else
	{
		Control::OnDoubleClick(e);
	}
}

void TabControlEx::OnSelectedIndexChanged(EventArgs* e)
{
	if(_lstControls != NULL)
	{
		int nIndex = GetSelectedIndex();
		if(nIndex >= 0)
		{
			Control* page = NULL;
			if(_nLastSelectedIndex != nIndex && _nLastSelectedIndex >= 0 && _nLastSelectedIndex < _lstControls->GetSize())
			{
				page = _lstControls->GetKey(_nLastSelectedIndex);
				((TabPage*)page)->SetActivateFocus();
			}
			Control* ctl = _lstControls->GetKey(nIndex);
			if(!::EqualRect(&_rectPage, &ctl->GetBounds()))
			{
				ctl->SetBoundsNotOriginal(_rectPage, BoundsSpecified::All);
			}
			if(!ctl->GetVisible())
			{
				ctl->SetVisible(true);
			}
			//Control* pWndNext = ctl->GetNextControl(NULL);
			//if(pWndNext != NULL)
			//{
			//	pWndNext->Focus();
			//}

			//hide last
			if(page != NULL && page->GetVisible())
			{
				page->SetVisible(false);
			}
			_nLastSelectedIndex = nIndex;
			m_nHotIndex = -1;
		}
		SelectedIndexChanged.Invoke((Object*)this, e);
	}
}

void TabControlEx::OnResize(EventArgs* e)
{
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) == 0)
	{
		return;
	}
	AdjustRect();
	CalcuteHeadInfo();
	//SetOuterStyle(OuterStyle::Sizing, true);
	_StyleArray[(int)StyleType::Outer] |= OuterStyle::Sizing;
	Resize.Invoke((Object*)this, e);
	if(_lstControls != NULL && _lstControls->GetSize() > 0)
	{

#ifdef _DEBUG_
		if(CC_APP_GetConfig()->GetIsDebug())
		{
			CC_TRACE((_T("TabControl::OnResize[PageRect]left:%d, top:%d, right-left:%d, bottom-top:%d"),
				_rectPage.left, _rectPage.top, _rectPage.right - _rectPage.left, _rectPage.bottom - _rectPage.top));
		}
#endif

		//only change curpage size
		//for(int n = _lstControls->GetSize() - 1; n >= 0; n--)
		if(_nSelectedIndex != -1)
		{
			_lstControls->GetKey(_nSelectedIndex)->SetBoundsNotOriginal(_rectPage, BoundsSpecified::All);
		}
	}
	//SetOuterStyle(OuterStyle::Sizing, false);
	_StyleArray[(int)StyleType::Outer] &= ~OuterStyle::Sizing;
}

void TabControlEx::SetHit(int index, HDC hDC, bool isMouseOn, bool isEdge)
{
	if(index < 0 || index >= GetTabCount())
	{
		return;
	}
	TabPage* ctl = (TabPage*)_lstControls->GetKey(index);
	DrawHead(ctl, hDC, (index == _nSelectedIndex), isMouseOn, isEdge);
}

void TabControlEx::SetHotHit(int index, bool isMouseOn, bool isEdge)
{
	//e->WParam is the index of hotpage,-1 for mouse out of head
	EventArgs e(m_hWnd, 0, index, NULL);
	HeadHotHit.Invoke((Object*)this, &e);

	if(m_nHotIndex == index)
	{
		return;
	}

	HDC hDC = ::GetDC(m_hWnd);
	HFONT hOldFont = (HFONT)::SelectObject(hDC, GetFont());

	if(m_nHotIndex != -1)
	{
		SetHit(m_nHotIndex, hDC, false, isEdge);
	}
	if(index != -1 && _lstControls->GetKey(index)->GetEnabled())
	{
		SetHit(index, hDC, isMouseOn, isEdge);
	}

	::SelectObject(hDC, hOldFont);
	if(GetFont() != hOldFont)
	{
		::DeleteObject(hOldFont);
	}
	::ReleaseDC(m_hWnd, hDC);
	m_nHotIndex = index;
}

//e->WParam is the index of clicked icon
void TabControlEx::OnHeadIconClick(EventArgs *e)
{
	HeadIconClick.Invoke((Object*)this, e);
}

void TabControlEx::SetTabAlignment(TabAlignment::Type alignment)
{
	_eAlignment = alignment;
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
	{
		//TODO:
		//::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED | SWP_DRAWFRAME);
		//do some setting
		OnStyleChanged(true);
		AdjustRect();
		CalcuteHeadInfo();
		OnResize(NULL);
	}
}

TabAlignment::Type TabControlEx::GetTabAlignment() const
{
	return _eAlignment;
}

void TabControlEx::SetTabAppearance(TabAppearance::Type appearance)
{
	_eAppearance = appearance;
	if(_eAppearance == TabAppearance::Normal)
	{
		SetStyle(StyleType::Style, TCS_BUTTONS|TCS_FLATBUTTONS, 0);
	}
	else if(_eAppearance == TabAppearance::Buttons)
	{
		SetStyle(StyleType::Style, TCS_FLATBUTTONS, TCS_BUTTONS);
		//this is valid before create
		SetStyle(StyleType::ExStyle, 0, TCS_EX_FLATSEPARATORS);
	}
	else //FlatButtons
	{
		SetStyle(StyleType::Style, 0, TCS_BUTTONS|TCS_FLATBUTTONS);
		//this is valid before create
		SetStyle(StyleType::ExStyle, 0, TCS_EX_FLATSEPARATORS);
	}
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
	{
		//::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED | SWP_DRAWFRAME);
		DoUpdate();
		OnResize(NULL);
	}
}

TabAppearance::Type TabControlEx::GetTabAppearance() const
{
	return _eAppearance;
}

void TabControlEx::AddRightButton(Control* control)
{
	if(_lstRightCtls == NULL)
	{
		//isSort, bIsDESC=false, isDistinct=false, nMinCapacity=0, nIncrement=1.3)
		_lstRightCtls = new List<Control*>(false, false, true);
	}
	if(!_lstRightCtls->ContainsKey(control))
	{
		_lstRightCtls->Add(control);
		control->SetParent(this, true);
	}
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
	{
		if(control->m_hWnd == NULL)
		{
			control->SetVisible(false);
			control->CreateControl();
		}
		_nUpdateFlag |= UpdateFlag::UpdateFlag;
		DoUpdate();
	}
}

void TabControlEx::ClearRightButton()
{
	if(_lstRightCtls != NULL)
	{
		for(int n = _lstRightCtls->GetSize() - 1; n >= 0; n--)
		{
			Control* ctl = _lstRightCtls->GetKey(n);
			ctl->Dispose();
			delete ctl;
		}
		delete _lstRightCtls;
		_lstRightCtls = NULL;
		if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
		{
			_nUpdateFlag |= UpdateFlag::UpdateFlag;
			DoUpdate();
		}
	}
}

////the imageList is same with "SetIcon(ImageList* imageList, UINT nIconID)" 's imageList.and is only one point!
void TabControlEx::AddHeadIcon(ImageList* imageList, int nIconID)
{
	_pImageList = imageList;
	if(_lstHeadIconsID == NULL)
	{
		//isSort, bIsDESC=false, isDistinct=false, nMinCapacity=0, nIncrement=1.3)
		_lstHeadIconsID = new List<int>(false, false, true);
	}
	if(!_lstHeadIconsID->ContainsKey(nIconID))
	{
		_lstHeadIconsID->Add(nIconID);
		if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0 && _lstControls != NULL)
		{
			for(int n = _lstControls->GetSize() - 1; n >= 0; n--)
			{
				Control* ctl = _lstControls->GetKey(n);
				DoModPage((TabPage*)ctl, n);
			}
			_nUpdateFlag |= UpdateFlag::UpdateFlag;
			DoUpdate();
		}
	}
}

//void TabControlEx::AddHeadIcon(HICON hIcon, bool needDestroy)
//{
//}
//
void TabControlEx::ClearHeadIcon()
{
	if(_lstHeadIconsID != NULL)
	{
		delete _lstHeadIconsID;
		_lstHeadIconsID = NULL;
		if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0 && _lstControls != NULL)
		{
			for(int n = _lstControls->GetSize() - 1; n >= 0; n--)
			{
				Control* ctl = _lstControls->GetKey(n);
				DoModPage((TabPage*)ctl, n);
			}
			_nUpdateFlag |= UpdateFlag::UpdateFlag;
			DoUpdate();
		}
	}
}

int TabControlEx::GetVHeadWidth() const
{
	return _nVHeadWidth;
}

//if -1 for Vlist text
void TabControlEx::SetVHeadWidth(int nVHeadWidth)
{
	this->_nVHeadWidth = nVHeadWidth;
	_nUpdateFlag |= UpdateFlag::UpdateFlag;
	DoUpdate();
}

int TabControlEx::GetBestHeadWidth() const
{
	return _nBestHeadWidth;
}

//if -1 for Vlist text
void TabControlEx::SetBestHeadWidth(int nBestHeadWidth)
{
	this->_nBestHeadWidth = nBestHeadWidth;
	_nUpdateFlag |= UpdateFlag::UpdateFlag;
	DoUpdate();
}

//this size is include headicon and headrighticon.default is 16x16
void TabControlEx::SetHeadIconSize(int cx, int cy)
{
	_sizeHeadIcon.cx = cx;
	_sizeHeadIcon.cy = cy;
	_nUpdateFlag |= UpdateFlag::UpdateFlag;
	DoUpdate();
}

void TabControlEx::HeadDropMenu_OnClick(Object* sender, EventArgs* e)
{
	if(sender != NULL)
	{
		MenuItem* menu = (MenuItem*)sender;
		SetSelectedIndex(menu->GetIndex());
	}
}

void TabControlEx::PrivTool_OnClick(Object* sender, EventArgs* e)
{
	CC_ASSERT(sender != NULL && e != NULL && m_hWnd != NULL);

	e->Handled = true;
	_nHeadFirstIndex--;
	CalcuteHeadInfo();
	//::InvalidateRect(m_hWnd, &_rectHead, FALSE);
	::InvalidateRect(m_hWnd, NULL, FALSE);
}

void TabControlEx::NextTool_OnClick(Object* sender, EventArgs* e)
{
	CC_ASSERT(sender != NULL && e != NULL && m_hWnd != NULL);

	e->Handled = true;
	if(_lstControls != NULL && _nHeadLastIndex < _lstControls->GetSize() - 1)
	{
		_nHeadFirstIndex++;
		CalcuteHeadInfo();
		//::InvalidateRect(m_hWnd, &_rectHead, FALSE);
		::InvalidateRect(m_hWnd, NULL, FALSE);
	}
}

void TabControlEx::TopTool_OnClick(Object* sender, EventArgs* e)
{
	CC_ASSERT(sender != NULL && e != NULL && m_hWnd != NULL);

	e->Handled = true;
	_nHeadFirstIndex = 0;
	CalcuteHeadInfo();
	//::InvalidateRect(m_hWnd, &_rectHead, FALSE);
	::InvalidateRect(m_hWnd, NULL, FALSE);
}

void TabControlEx::EndTool_OnClick(Object* sender, EventArgs* e)
{
	CC_ASSERT(sender != NULL && e != NULL && m_hWnd != NULL);

	e->Handled = true;
	int nCnt;
	if(_lstControls != NULL && _nHeadLastIndex < (nCnt = _lstControls->GetSize() - 1))
	{
		while(_nHeadLastIndex != nCnt && _nHeadFirstIndex < nCnt)
		{
			_nHeadFirstIndex++;;
			CalcuteHeadInfo();
		}
		//_nHeadFirstIndex = nCnt - 1;;
		//CalcuteHeadInfo();
		//::InvalidateRect(m_hWnd, &_rectHead, FALSE);
		::InvalidateRect(m_hWnd, NULL, FALSE);
	}
}
///////////////////////////////////////////////////////////////////////////////
//===========================================================cc::win::TabControlEx

//===========================================================cc::win::TreeViewEx
///////////////////////////////////////////////////////////////////////////////
//http://www.pocketpcdn.com/articles/tree_color.html
//http://www.functionx.com/visualc/controls/treectrl.htm
DWORD TreeViewExConfig::nStyle = WS_CHILD|WS_VISIBLE|TVS_HASBUTTONS|TVS_HASLINES|TVS_LINESATROOT|TVS_EDITLABELS|TVS_SHOWSELALWAYS;
DWORD TreeViewExConfig::nExStyle = WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR | WS_EX_NOPARENTNOTIFY | WS_EX_CLIENTEDGE;
//DWORD TreeViewExConfig::nClassStyle = CS_DBLCLKS | CS_GLOBALCLASS | CS_HREDRAW | CS_PARENTDC | CS_VREDRAW;
DWORD TreeViewExConfig::nInnerStyle = InnerStyle::TranceContextMenuUp;
DWORD TreeViewExConfig::nOuterStyle = 0;
DWORD TreeViewExConfig::nOwnerDrawTextStyle = DT_LEFT|DT_VCENTER|DT_SINGLELINE;
COLORREF TreeViewExConfig::corBackground = ::GetSysColor(COLOR_WINDOW);
COLORREF TreeViewExConfig::corTextground = ::GetSysColor(COLOR_WINDOWTEXT);
COLORREF TreeViewExConfig::corHotBackColor = ::GetSysColor(COLOR_ACTIVECAPTION);
COLORREF TreeViewExConfig::corHotTextColor = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
COLORREF TreeViewExConfig::corTextgroundDisabled = RGB(0x80, 0x80, 0x80);
//===========================================================cc::win::TreeViewEx
///////////////////////////////////////////////////////////////////////////////
// constructors/destructor
TreeViewEx::TreeViewEx()
{
	BASE_CONTROL_CONFIG_INIT(TreeViewEx);
	_Rect.right = _Rect.left + 105;
	_Rect.bottom = _Rect.top + 75;
	::CopyRect(&_OrgRect, &_Rect);
	//_ClassName = WndClassName::TreeView;
	_StyleArray[(int)StyleType::InitControl] = CC_WNDCOMMCTL_TREEVIEW_REG;
	_hEdit = NULL;
	m_hImgList = NULL;
	m_nStyle = CheckStyle::AutoCheckBox; //nStyle == BS_CHECKBOX || nStyle == BS_AUTOCHECKBOX || nStyle == BS_AUTO3STATE || nStyle == BS_3STATE
#ifdef _DEBUG_
	SetName(_T("TreeViewEx"));
#endif
	//Map(bool isSort, bool bIsDESC = false, bool isDistinct = false, int nMinCapacity = 0, double nIncrement = 1.3);
	_mapData = NULL;
}

TreeViewEx::~TreeViewEx()
{
	if(_mapData != NULL)
	{
		for(int i = _mapData->GetSize()-1; i >= 0; i--)
		{
			CC_TREEVIEWEX_DATA* pData = (CC_TREEVIEWEX_DATA*)_mapData->GetAt(i);
			delete pData;
		}

		delete _mapData;
		_mapData = NULL;
	}
}

//
//void* TreeViewEx::GetItemDataPtr(HTREEITEM hItem)
//{
//	return (void*)GetItemData(hItem);
//}
//BOOL TreeViewEx::SetItemDataPtr(HTREEITEM hItem, void* pData)
//{
//	return SetItemData(hItem, (DWORD_PTR)(LPVOID)pData);
//}

void TreeViewEx::SetItemEnabled(HTREEITEM hItem, bool bEnabled)
{
	CC_ASSERT(::IsWindow(m_hWnd));

	//CC_TREEVIEWEX_DATA* pData = GetItemDataBase(hItem);
	int index = GetCustomDataIndex(hItem, true);
	if(index >= 0)
	{
		CC_TREEVIEWEX_DATA* pData = (CC_TREEVIEWEX_DATA*)_mapData->GetAt(index);
		CC_BIT_SET(pData->m_nFlags, CC_ACCESS_ITEM_DISIABLED, !bEnabled);
	}
	if(m_hWnd != NULL)
	{
		RECT rect = {0};
		GetItemRect(hItem, &rect, TRUE);
		::InvalidateRect(m_hWnd, &rect, TRUE);
	}
}

bool TreeViewEx::GetItemEnabled(HTREEITEM hItem)
{
	CC_ASSERT(::IsWindow(m_hWnd));

	bool ret = true;
	//CC_TREEVIEWEX_DATA* pData = GetItemDataBase(hItem);
	int index = GetCustomDataIndex(hItem, false);
	if(index >= 0)
	{
		CC_TREEVIEWEX_DATA* pData = (CC_TREEVIEWEX_DATA*)_mapData->GetAt(index);
		ret = !CC_BIT_HAS(pData->m_nFlags, CC_ACCESS_ITEM_DISIABLED);
	}
	return ret;
}

void TreeViewEx::SetCheckboxEnabled(HTREEITEM hItem, bool bEnabled)
{
	CC_ASSERT(::IsWindow(m_hWnd));

	//CC_TREEVIEWEX_DATA* pData = GetItemDataBase(hItem);
	int index = GetCustomDataIndex(hItem, true);
	if(index >= 0)
	{
		CC_TREEVIEWEX_DATA* pData = (CC_TREEVIEWEX_DATA*)_mapData->GetAt(index);
		CC_BIT_SET(pData->m_nFlags, CC_ACCESS_CHKBOX_DISIABLED, !bEnabled);
	}
	if(m_hWnd != NULL)
	{
		RECT rect = {0};
		GetItemRect(hItem, &rect, TRUE);
		::InvalidateRect(m_hWnd, &rect, TRUE);
	}
}

bool TreeViewEx::GetCheckboxEnabled(HTREEITEM hItem)
{
	CC_ASSERT(::IsWindow(m_hWnd));

	bool ret = true;
	//CC_TREEVIEWEX_DATA* pData = GetItemDataBase(hItem);
	int index = GetCustomDataIndex(hItem, false);
	if(index >= 0)
	{
		CC_TREEVIEWEX_DATA* pData = (CC_TREEVIEWEX_DATA*)_mapData->GetAt(index);
		ret = !CC_BIT_HAS(pData->m_nFlags, CC_ACCESS_CHKBOX_DISIABLED);
	}
	return ret;
}

void TreeViewEx::SetItemTextColor(HTREEITEM hItem, COLORREF clrNew)
{
	CC_ASSERT(::IsWindow(m_hWnd));

	//CC_TREEVIEWEX_DATA* pData = GetItemDataBase(hItem);
	int index = GetCustomDataIndex(hItem, true);
	if(index >= 0)
	{
		CC_TREEVIEWEX_DATA* pData = (CC_TREEVIEWEX_DATA*)_mapData->GetAt(index);
		pData->corTextground = clrNew;
	}
}

COLORREF TreeViewEx::GetItemTextColor(HTREEITEM hItem)
{
	CC_ASSERT(::IsWindow(m_hWnd));

	COLORREF ret = TreeViewExConfig::corTextground;
	//CC_TREEVIEWEX_DATA* pData = GetItemDataBase(hItem);
	int index = GetCustomDataIndex(hItem, false);
	if(index >= 0)
	{
		CC_TREEVIEWEX_DATA* pData = (CC_TREEVIEWEX_DATA*)_mapData->GetAt(index);
		ret = pData->corTextground;
	}
	return ret;
}

void TreeViewEx::SetItemHotTextColor(HTREEITEM hItem, COLORREF clrNew)
{
	CC_ASSERT(::IsWindow(m_hWnd));

	//CC_TREEVIEWEX_DATA* pData = GetItemDataBase(hItem);
	int index = GetCustomDataIndex(hItem, true);
	if(index >= 0)
	{
		CC_TREEVIEWEX_DATA* pData = (CC_TREEVIEWEX_DATA*)_mapData->GetAt(index);
		pData->corHotTextColor = clrNew;
	}
}

COLORREF TreeViewEx::GetItemHotTextColor(HTREEITEM hItem)
{
	CC_ASSERT(::IsWindow(m_hWnd));

	COLORREF ret = TreeViewExConfig::corHotTextColor;
	//CC_TREEVIEWEX_DATA* pData = GetItemDataBase(hItem);
	int index = GetCustomDataIndex(hItem, false);
	if(index >= 0)
	{
		CC_TREEVIEWEX_DATA* pData = (CC_TREEVIEWEX_DATA*)_mapData->GetAt(index);
		ret = pData->corHotTextColor;
	}
	return ret;
}

void TreeViewEx::SetCheckStyle(CheckStyle::Type eStyle)
{
	if((int)eStyle == 0 || (int)eStyle == BS_CHECKBOX || (int)eStyle == BS_AUTOCHECKBOX || (int)eStyle == BS_AUTO3STATE || (int)eStyle == BS_3STATE)
	{
		m_nStyle = eStyle;
	}
}

CheckStyle::Type TreeViewEx::GetCheckStyle()
{
	return m_nStyle;
}

void TreeViewEx::SetItemCheckBox(HTREEITEM hItem, bool bShowBox)
{
	TVITEM item;
	memset(&item, 0, sizeof(TVITEM));
	item.hItem = hItem;
	item.mask = TVIF_HANDLE | TVIF_STATE;
	item.stateMask = TVIS_STATEIMAGEMASK;
	item.state = bShowBox ? INDEXTOSTATEIMAGEMASK(1) : INDEXTOSTATEIMAGEMASK(0);
	SetItem(&item);
}

bool TreeViewEx::GetItemCheckBox(HTREEITEM hItem)
{
	return ((int)TreeView_GetCheckState(m_hWnd, hItem) != -1);
}

DWORD TreeViewEx::GetAccessFlag(HTREEITEM hItem)
{
	DWORD nAccessFlag = 0;
	if(hItem != NULL)
	{
		//CC_TREEVIEWEX_DATA* pData = GetItemDataBase(hItem);
		int index = GetCustomDataIndex(hItem, false);
		if(index >= 0)
		{
			CC_TREEVIEWEX_DATA* pData = (CC_TREEVIEWEX_DATA*)_mapData->GetAt(index);
			if(CC_BIT_HAS(pData->m_nFlags, CC_ACCESS_ITEM_DISIABLED))
			{
				nAccessFlag |= CC_ACCESS_ITEM_DISIABLED;
			}
			if(CC_BIT_HAS(pData->m_nFlags, CC_ACCESS_CHKBOX_DISIABLED))
			{
				nAccessFlag |= CC_ACCESS_CHKBOX_DISIABLED;
			}
		}
	}
	return nAccessFlag;
}
//
//void TreeViewEx::OnMouseDown(MouseEventArgs* e)
//{
//	POINTS Pos = MAKEPOINTS(e->baseArgs->LParam);
//	TVHITTESTINFO ht = {0};
//	ht.pt.x = Pos.x;
//	ht.pt.y = Pos.y;
//	HitTest(&ht);
//	if(ht.hItem != NULL)
//	{
//		CC_TREEVIEWEX_DATA* pData = GetItemDataBase(ht.hItem);
//		if(CC_BIT_HAS(pData->m_nFlags, CC_ACCESS_ITEM_DISIABLED))
//		{
//			e->baseArgs->Result = (LRESULT)0;
//			e->baseArgs->Handled = true;
//			return;
//		}
//	}
//	TreeView::OnMouseDown(e);
//}

void TreeViewEx::OnSelectedIndexChanging(EventArgs* e)
{
	CC_ASSERT(::IsWindow(m_hWnd));
	LPNMTREEVIEW pnmtv = (LPNMTREEVIEW)e->LParam;
	if(pnmtv->itemNew.lParam != NULL)
	{
		int index = GetCustomDataIndex((HTREEITEM)pnmtv->itemNew.lParam, true);
		if(index >= 0)
		{
			CC_TREEVIEWEX_DATA* pData = (CC_TREEVIEWEX_DATA*)_mapData->GetAt(index);
			if(CC_BIT_HAS(pData->m_nFlags, CC_ACCESS_ITEM_DISIABLED))
			{
				e->Result = (LRESULT)TRUE;
				e->Handled = true;
				return;
			}
		}
	}
	TreeView::OnSelectedIndexChanging(e);
}

void TreeViewEx::OnItemCheckChangedLoopChild(HTREEITEM hItem, int nChecked)
{
	HTREEITEM hItemSub = GetChildItem(hItem);
	if(hItemSub == NULL)
	{
		return;
	}
	HTREEITEM hItemNext = hItemSub;
	while(hItemNext != NULL)
	{
		//if item is no checkbox,not loop its child
		if(GetItemCheckBox(hItemNext))
		{
			TreeView_SetCheckState(m_hWnd, hItemNext, (nChecked != 0));
			//cc::Log::debug(_T("CheckChild, hItemNext:%d, nChecked:%d"), hItemNext, nChecked);
			OnItemCheckChangedLoopChild(hItemNext, nChecked);
		}
		hItemNext = GetNextItem(hItemNext, TVGN_NEXT);
	}
}

void TreeViewEx::OnItemCheckChangedLoopParent(HTREEITEM hItem, int nChecked)
{
	//cc::Log::debug(_T("CheckParent, hItem:%d, nChecked:%d"), hItem, nChecked);
	HTREEITEM hItemParent = GetParentItem(hItem);
	//if parent is no checkbox,end loop
	if(hItemParent == NULL || !GetItemCheckBox(hItemParent))
	{
		return;
	}
	HTREEITEM hItemSub = GetChildItem(hItemParent);
	CC_ASSERT(hItemSub != NULL);
	HTREEITEM hItemNext = hItemSub;
	while(hItemNext != NULL)
	{
		if(hItemNext != hItem)
		{
			int nchk = (int)TreeView_GetCheckState(m_hWnd, hItemNext);
			//if item no checkbox,not count it
			if(nchk != -1 && nchk != nChecked)
			{
				//cc::Log::debug(_T("CheckParent 2, hItemNext:%d, nchk:%d"), hItemNext, nchk);
				nChecked = 2;
				break;
			}
		}
		hItemNext = GetNextItem(hItemNext, TVGN_NEXT);
	}
	TreeView_SetItemState(m_hWnd, hItemParent, INDEXTOSTATEIMAGEMASK(nChecked >= 2 ? 3 : (nChecked == 1 ? 2 : 1)), TVIS_STATEIMAGEMASK)
	//TVITEM item;
	//memset(&item, 0, sizeof(TVITEM));
	//item.hItem = hItemParent;
	//item.mask = TVIF_HANDLE | TVIF_STATE;
	//item.stateMask = TVIS_STATEIMAGEMASK;
	////cc::Log::debug(_T("CheckParent End, hItemParent:%d, nChecked:%d"), hItemParent, nChecked);
	//item.state = (nChecked == 2 ? INDEXTOSTATEIMAGEMASK(3) : INDEXTOSTATEIMAGEMASK(nChecked == 0 ? 1 : 2));
	//SetItem(&item);
	OnItemCheckChangedLoopParent(hItemParent, nChecked);
}
//HIWORD(e->WParam) = 0, LOWORD(e->WParam) = isChecked, e->LParam = ht.hItem(SelectedItem)
void TreeViewEx::OnItemCheckChanged(EventArgs* e)
{
	//cc::Log::debug(_T("TreeViewEx::OnItemCheckChanged, LOWORD(e->WParam):%d, GetCheckState:%d"), (int)LOWORD(e->WParam), TreeView_GetCheckState(m_hWnd, GetSelectedItem()));
	OnItemCheckChangedLoopChild((HTREEITEM)e->LParam, (int)LOWORD(e->WParam));
	OnItemCheckChangedLoopParent((HTREEITEM)e->LParam, (int)LOWORD(e->WParam));
	ItemCheckChanged.Invoke((Object*)this, e);
}

int TreeViewEx::GetCustomDataIndex(HTREEITEM hItem, bool isCreate)
{
	//CC_TREEVIEWEX_DATA* pData = GetItemDataBase(hItem);
	int index = -1;
	if(_mapData != NULL)
	{
		index = _mapData->GetIndex((int)hItem);
	}
	if(isCreate && index < 0)
	{
		if(_mapData == NULL)
		{
			_mapData = new Map<int, void*>(true, false, true);
		}
		CC_TREEVIEWEX_DATA* pData = new CC_TREEVIEWEX_DATA;
		pData->corTextground = _corTextground;
		pData->corHotTextColor = _corHotTextColor;
		index = _mapData->Add((int)hItem, pData);
	}
	return index;
}

void TreeViewEx::OnDeleteItem(EventArgs *e)
{
	LPNMTREEVIEW pnmtv = (LPNMTREEVIEW)e->LParam;
	TreeView::OnDeleteItem(e);

	int index = _mapData->GetIndex((int)pnmtv->itemOld.hItem);
	if(index >= 0)
	{
		CC_TREEVIEWEX_DATA* pData = (CC_TREEVIEWEX_DATA*)_mapData->GetAt(index);
		delete pData;
		_mapData->RemoveAt(index);
	}
}

//Developing Custom Draw Controls in Visual C++
//http://msdn.microsoft.com/en-us/library/ms364048(VS.80).aspx
void TreeViewEx::OnCustomDraw(EventArgs* e)
{
	if(CC_BIT_HAS(_Flags, CC_FLAG_TREEVIEW_NOCUSTOMDRAW))
	{
		return;
	}
	int index = 0;
	CC_TREEVIEWEX_DATA* pData = NULL;
	LPNMTVCUSTOMDRAW pCustomDraw = (LPNMTVCUSTOMDRAW)e->LParam;
	HDC hDC = pCustomDraw->nmcd.hdc;
	HTREEITEM hItem = (HTREEITEM)pCustomDraw->nmcd.dwItemSpec;
	//if(this->HasStyle(StyleType::Inner, InnerStyle::BackgroundGradient) || this->HasStyle(StyleType::Outer, OuterStyle::TransparentParentErase|OuterStyle::TransparentParentBrush))
	//{
	//	::SetBkMode(hDC, TRANSPARENT);
	//}
	//HBRUSH brush = (HBRUSH)::GetStockObject(HOLLOW_BRUSH);
	//::SetBkMode(hDC, TRANSPARENT);
	//::SelectObject(hDC, &brush);
	int nState = 0;
	switch(pCustomDraw->nmcd.dwDrawStage)
	{
		case CDDS_PREPAINT:
			// Need to process this case and set pResult to CDRF_NOTIFYITEMDRAW, 
			// otherwise parent will never receive CDDS_ITEMPREPAINT notification. (GGH) 
			e->Result = CDRF_NOTIFYITEMDRAW;
			e->Handled = true;
			m_hImgList = TreeView_GetImageList(m_hWnd, TVSIL_NORMAL);
			break;
		case CDDS_ITEMPREPAINT:
		//case CDDS_ITEMPREPAINT | CDDS_SUBITEM:
			//pData = GetItemDataBase(hItem);
			e->Result = CDRF_DODEFAULT;
			e->Handled = true;
			index = GetCustomDataIndex(hItem, false);
			if(index >= 0)
			{
				pData = (CC_TREEVIEWEX_DATA*)_mapData->GetAt(index);
				//cc::Log::debug(_T("TreeView_GetCheckState(m_hWnd, hItem):%d"), (int)TreeView_GetCheckState(m_hWnd, hItem));
				//if(m_nStyle == CheckStyle::Auto3State && (_StyleArray[(int)StyleType::Style] & TVS_CHECKBOXES) == TVS_CHECKBOXES
				//	&& (int)TreeView_GetCheckState(m_hWnd, hItem) == 2)
				nState = (int)TreeView_GetCheckState(m_hWnd, hItem);
				if(HasStyle(StyleType::Outer, OuterStyle::TransparentHollowBrush) || CC_BIT_HAS(pData->m_nFlags, CC_ACCESS_CHKBOX_DISIABLED) || ((_StyleArray[(int)StyleType::Style] & TVS_CHECKBOXES) == TVS_CHECKBOXES && nState == 2))
				{
					e->Result = CDRF_DODEFAULT | CDRF_NOTIFYPOSTPAINT;
				}
			}
			break;
		case CDDS_ITEMPOSTPAINT:
			//pData = GetItemDataBase(hItem);
			RECT rcLabel;
			index = GetCustomDataIndex(hItem, false);
			if(index >= 0)
			{
				pData = (CC_TREEVIEWEX_DATA*)_mapData->GetAt(index);
				//CC_ASSERT(m_nStyle == CheckStyle::Auto3State && (_StyleArray[(int)StyleType::Style] & TVS_CHECKBOXES) == TVS_CHECKBOXES);
				CC_ASSERT((_StyleArray[(int)StyleType::Style] & TVS_CHECKBOXES) == TVS_CHECKBOXES);

				nState = (int)TreeView_GetCheckState(m_hWnd, hItem);
				if(CC_BIT_HAS(pData->m_nFlags, CC_ACCESS_CHKBOX_DISIABLED) || ((_StyleArray[(int)StyleType::Style] & TVS_CHECKBOXES) == TVS_CHECKBOXES && nState == 2))
				{
					GetItemRect(hItem, &rcLabel, TRUE);
					//cc::Log::debug(_T("DrawEdge, rcLabel, L:%d, T:%d, W:%d, H:%d"), rcLabel.left, rcLabel.top, rcLabel.right - rcLabel.left, rcLabel.bottom - rcLabel.top);
					//in fact must need m_hImgList!
					if(m_hImgList != NULL)
					{
						::SetRect(&rcLabel, rcLabel.left - 14 * 2 - 5, rcLabel.top + 2,
							rcLabel.left - 14 - 5, rcLabel.top + 2 + 14);
					}
					else
					{
						::SetRect(&rcLabel, rcLabel.left - 14, rcLabel.top + 2,
							rcLabel.left, rcLabel.top + 2 + 14);
					}
					//CC_APP_GetDrawActor()->DrawSpecialChar(hDC, rcLabel, 98, false, ::GetSysColor(COLOR_BTNSHADOW), 12);
					::DrawFrameControl(hDC, &rcLabel, DFC_BUTTON, DFCS_BUTTONCHECK | DFCS_BUTTON3STATE | (nState >= 1 ? DFCS_CHECKED : 0));
				}
			}
			if(HasStyle(StyleType::Outer, OuterStyle::TransparentHollowBrush) && (!Focused() || hItem != this->GetSelectedItem()))
			{
				RECT rect;
				HBRUSH brush = NULL;
				POINT pt = {0, 0};
				GetItemRect(hItem, &rcLabel, TRUE);
				if(_pParentBrush != NULL)
				{
					::GetWindowRect(this->m_hWnd, &rect);
					::MapWindowPoints(NULL, _pParentBrush->m_hWnd, (LPPOINT)(&rect), 2);
					brush = _pParentBrush->GetBackgroundBrush();
					::SetBrushOrgEx(hDC, -rect.left, -rect.top, &pt);
					::FillRect(hDC, &rcLabel, brush);
				}
				else
				{
					//clear back use window color
					COLORREF oldBkColor = ::SetBkColor(hDC, _corBackground);
					::ExtTextOut(hDC, rcLabel.left, rcLabel.top, ETO_OPAQUE, &rcLabel, NULL, 0, NULL);
					::SetBkColor(hDC, oldBkColor);
				}
				::SetBkMode(hDC, TRANSPARENT);
				cc::Str str = this->GetItemText(hItem, 256);
				rcLabel.left += 2;
				rcLabel.right += 2;
				//::SetTextColor(hDC, pCustomDraw->clrText);
				::SetTextColor(hDC, GetItemTextColor(hItem));
				::DrawText(hDC, str.GetLength() <= 0 ? _T("") : str, -1, &rcLabel, DT_VCENTER|DT_SINGLELINE);
				if(brush != NULL)
				{
					::SetBrushOrgEx(hDC, pt.x, pt.y, NULL);
				}
			}
			break;
	}
	//LPNMTVCUSTOMDRAW pCustomDraw = (LPNMTVCUSTOMDRAW)e->LParam;
	//CustomDraw.Invoke((Object*)this, e);
	TreeView::OnCustomDraw(e);
}
///////////////////////////////////////////////////////////////////////////////
//===========================================================cc::win::TreeViewEx

/*
//===========================================================cc::win::MoveFormCtl
///////////////////////////////////////////////////////////////////////////////
// constructors/destructor
MoveFormCtl::MoveFormCtl()
{
	_corHotBackColor = ::GetSysColor(COLOR_BTNFACE);
	_corHotTextColor = ::GetSysColor(COLOR_BTNTEXT);
	_StyleArray[(int)StyleType::Outer] |= OuterStyle::AutoSize | OuterStyle::AlwaysNoEdge;
	moveForm = NULL;
#ifdef _DEBUG_
	SetName(_T("MoveFormCtl"));
#endif
}
MoveFormCtl::~MoveFormCtl()
{
}
// constructors/destructor
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// private/protected/internal used methods
void MoveFormCtl::SetMoveForm(Form* form)
{
	this->moveForm = form;
}

void MoveFormCtl::OnMouseDown(MouseEventArgs* e)
{
	Control::OnMouseDown(e);
	if(e->Button == MouseButtons::Left && (moveForm != NULL || _pParent != NULL))
	{
		::SetCursor(::LoadCursor(NULL, IDC_SIZEALL));
		::ReleaseCapture();
		::SendMessage(moveForm == NULL ? _pParent->m_hWnd : moveForm->m_hWnd, WM_NCLBUTTONDOWN, HTCAPTION, NULL);
	}
}
// private/protected/internal used methods
///////////////////////////////////////////////////////////////////////////////
*/

// To hold the colours and their names
typedef struct {
    COLORREF crColor;
    TCHAR    *szName;
} ColorEntry;

ColorEntry g_crColors[] = 
{
    { RGB(0x00, 0x00, 0x00),    _T("Black")             },
    { RGB(0xA5, 0x2A, 0x00),    _T("Brown")             },
    { RGB(0x00, 0x40, 0x40),    _T("Dark Olive Green")  },
    { RGB(0x00, 0x55, 0x00),    _T("Dark Green")        },
    { RGB(0x00, 0x00, 0x5E),    _T("Dark Teal")         },
    { RGB(0x00, 0x00, 0x8B),    _T("Dark blue")         },
    { RGB(0x4B, 0x00, 0x82),    _T("Indigo")            },
    { RGB(0x28, 0x28, 0x28),    _T("Dark grey")         },

    { RGB(0x8B, 0x00, 0x00),    _T("Dark red")          },
    { RGB(0xFF, 0x68, 0x20),    _T("Orange")            },
    { RGB(0x8B, 0x8B, 0x00),    _T("Dark yellow")       },
    { RGB(0x00, 0x93, 0x00),    _T("Green")             },
    { RGB(0x38, 0x8E, 0x8E),    _T("Teal")              },
    { RGB(0x00, 0x00, 0xFF),    _T("Blue")              },
    { RGB(0x7B, 0x7B, 0xC0),    _T("Blue-grey")         },
    { RGB(0x66, 0x66, 0x66),    _T("Grey - 40")         },

    { RGB(0xFF, 0x00, 0x00),    _T("Red")               },
    { RGB(0xFF, 0xAD, 0x5B),    _T("Light orange")      },
    { RGB(0x32, 0xCD, 0x32),    _T("Lime")              },
    { RGB(0x3C, 0xB3, 0x71),    _T("Sea green")         },
    { RGB(0x7F, 0xFF, 0xD4),    _T("Aqua")              },
    { RGB(0x7D, 0x9E, 0xC0),    _T("Light blue")        },
    { RGB(0x80, 0x00, 0x80),    _T("Violet")            },
    { RGB(0x7F, 0x7F, 0x7F),    _T("Grey - 50")         },

    { RGB(0xFF, 0xC0, 0xCB),    _T("Pink")              },
    { RGB(0xFF, 0xD7, 0x00),    _T("Gold")              },
    { RGB(0xFF, 0xFF, 0x00),    _T("Yellow")            },
    { RGB(0x00, 0xFF, 0x00),    _T("Bright green")      },
    { RGB(0x40, 0xE0, 0xD0),    _T("Turquoise")         },
    { RGB(0xC0, 0xFF, 0xFF),    _T("Skyblue")           },
    { RGB(0x48, 0x00, 0x48),    _T("Plum")              },
    { RGB(0xC0, 0xC0, 0xC0),    _T("Light grey")        },

    { RGB(0xFF, 0xE4, 0xE1),    _T("Rose")              },
    { RGB(0xD2, 0xB4, 0x8C),    _T("Tan")               },
    { RGB(0xFF, 0xFF, 0xE0),    _T("Light yellow")      },
    { RGB(0x98, 0xFB, 0x98),    _T("Pale green ")       },
    { RGB(0xAF, 0xEE, 0xEE),    _T("Pale turquoise")    },
    { RGB(0x68, 0x83, 0x8B),    _T("Pale blue")         },
    { RGB(0xE6, 0xE6, 0xFA),    _T("Lavender")          },
    { RGB(0xFF, 0xFF, 0xFF),    _T("White")             }
};

//http://www.codeproject.com/KB/miscctrl/colour_picker.aspx
#define CC_PICKCOLORCTL_WIN_WIDTH 141
#define CC_PICKCOLOR_WH 14
#define CC_PICKCOLOR_MARGIN 3
//===========================================================cc::win::PickColorCtl
///////////////////////////////////////////////////////////////////////////////
// constructors/destructor
PickColorCtl::PickColorCtl()
{
	m_pResultCtl = NULL;
	m_dwResultStyle = 0;
	_corHotBackColor = _corBackground;
	//not need OuterStyle::AutoSize,OuterStyle::TransparentParentErase,also not need PushButton&ContextMenuButton
	//_StyleArray[(int)StyleType::Style] |= WS_TABSTOP;
	//if no OuterStyle::PushButton then click to show ColorDialog
	_StyleArray[(int)StyleType::Outer] = OuterStyle::ContextMenuButton|OuterStyle::OuterEdge;
	::SetRect(&_Rect, 0, 0, 15, 20);
	::SetRect(&_Margin, 3, 2, 3, 2);
	nIndexSave = -1;
	this->SetText(_T(" "));
#ifdef _DEBUG_
	SetName(_T("PickColorCtl"));
#endif
	_pFloater = new FloatContainer();
	_pFloater->SetStyle(StyleType::Style, WS_CAPTION|WS_THICKFRAME, WS_BORDER);
	_pFloater->SetStyle(StyleType::ExStyle, 0, WS_EX_TOPMOST);
	_pFloater->SetSize(CC_PICKCOLORCTL_WIN_WIDTH, 111);
	_pFloater->SetTrackMouse(true);
	_pFloater->PaintEnd += PaintEventHandler((Object*)this, (PaintEventFun)&PickColorCtl::Pop_PaintEnd);
	_pFloater->MouseMove += MouseEventHandler((Object*)this, (MouseEventFun)&PickColorCtl::Pop_MouseMove);
	_pFloater->MouseDown += MouseEventHandler((Object*)this, (MouseEventFun)&PickColorCtl::Pop_MouseDown);
	_pFloater->MouseLeave += EventHandler((Object*)this, (EventFun)&PickColorCtl::Pop_MouseLeave);
	_pFloater->Deactivate += EventHandler((Object*)this, (EventFun)&PickColorCtl::Pop_Deactivate);
	CC_BIT_ON(_Flags, CC_FLAG_TOOLITEM_NEEDDESTROY);
	if(CC_APP_GetLang()->Exist(_T("Lang.ColorOthers")))
	{
		sColorOthers = CC_APP_GetLang()->Get(_T("Lang.ColorOthers"), _T("Others..."));
	}
	else
	{
		sColorOthers = _T("Others...");
	}
	nValueChangedCount = 0;
}

PickColorCtl::~PickColorCtl()
{
}
// constructors/destructor
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// private/protected/internal used methods

//keyArr is param address,but sHead is by value
void PickColorCtl::DoConfig(bool isFromIni, KeyArr& keyArr, cc::Str sThisHead)
{
	if(!HasStyle(StyleType::Outer, OuterStyle::AutoConfig))
	{
		return;
	}
	if(isFromIni)
	{
		//if not exist,as usually
		if(keyArr.Exist(sThisHead))
		{
			//EventArgs ee(m_hWnd, 0, 0, (LPARAM)(COLORREF)keyArr.Get(sThisHead, (DWORD)0));
			//OnValueChanged(&ee);
			SetBackground((COLORREF)keyArr.Get(sThisHead, (DWORD)0));
		}
	}
	else
	{
		keyArr.Set(sThisHead, (DWORD)GetBackground());
	}
}

void PickColorCtl::DoSetLang(KeyArr& keyArr, cc::Str sThisHead)
{
	sColorOthers = CC_APP_GetLang()->Get(_T("Lang.ColorOthers"), _T("Others..."));
}

void PickColorCtl::SetHotBackColor(COLORREF corHotBackColor, bool isInvokeValueChanged)
{
	this->SetBackground(corHotBackColor, isInvokeValueChanged);
}

void PickColorCtl::SetBackground(COLORREF corBackColor, bool isInvokeValueChanged)
{
	EventArgs ee(m_hWnd, 0, 0, (LPARAM)corBackColor);
	OnValueChanged(&ee, isInvokeValueChanged);
}

void PickColorCtl::SetResultControl(Control* pResultCtl, DWORD dwResultStyle)
{
	m_pResultCtl = NULL;
	m_dwResultStyle = dwResultStyle;
	if(pResultCtl != NULL && m_dwResultStyle != 0)
	{
		//avoid dead loop,should first set m_pResultCtl to NULL
		if((m_dwResultStyle & CC_PICKCOLOR_BACKGROUND) != 0)
		{
			this->SetBackground(pResultCtl->GetBackground());
		}
		if((m_dwResultStyle & CC_PICKCOLOR_TEXTCOLOR) != 0)
		{
			this->SetBackground(pResultCtl->GetTextColor());
		}
		if((m_dwResultStyle & CC_PICKCOLOR_HOTBACKCOLOR) != 0)
		{
			this->SetBackground(pResultCtl->GetHotBackColor());
		}
		if((m_dwResultStyle & CC_PICKCOLOR_HOTTEXTCOLOR) != 0)
		{
			this->SetBackground(pResultCtl->GetHotTextColor());
		}
	}
	m_pResultCtl = pResultCtl;
}

void PickColorCtl::OnValueChanged(EventArgs* e, bool isInvokeValueChanged)
{
	COLORREF cor = (COLORREF)e->LParam;
	//this->SetBackground(cor);
	ToolItem::SetHotBackColor(cor);
	ToolItem::SetBackground(cor);
	if(isInvokeValueChanged)
	{
		if(nValueChangedCount <= 0)
		{
			//for dead loop
			nValueChangedCount = 1;
			ValueChanged.Invoke((Object*)this, e);
			nValueChangedCount = 0;
		}
	}
	if(m_pResultCtl != NULL && m_dwResultStyle != 0)
	{
		if((m_dwResultStyle & CC_PICKCOLOR_BACKGROUND) != 0)
		{
			m_pResultCtl->SetBackground(cor);
		}
		if((m_dwResultStyle & CC_PICKCOLOR_TEXTCOLOR) != 0)
		{
			m_pResultCtl->SetTextColor(cor);
		}
		if((m_dwResultStyle & CC_PICKCOLOR_HOTBACKCOLOR) != 0)
		{
			m_pResultCtl->SetHotBackColor(cor);
		}
		if((m_dwResultStyle & CC_PICKCOLOR_HOTTEXTCOLOR) != 0)
		{
			m_pResultCtl->SetHotTextColor(cor);
		}
	}
}

void PickColorCtl::Pop_PaintEnd(Object* sender, PaintEventArgs* e)
{
	HDC hDC = e->data.hdc;
	RECT rect;
	GetClientRect(_pFloater->m_hWnd, &rect);
	::SetBkColor(hDC, RGB(0xc4, 0xdb, 0xf9));
	::ExtTextOut(hDC, 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL);
	HFONT holdFont = (HFONT)::SelectObject(hDC, this->GetFont());

	HBRUSH brush = ::CreateSolidBrush(RGB(0x2a, 0x66, 0xc9));
	::FrameRect(hDC, &rect, brush);
	DeleteObject(brush);

	rect.bottom = 20;
	::SetBkMode(hDC, TRANSPARENT);
	::SetTextColor(hDC, this->GetTextColor());
	::DrawText(hDC, (const TCHAR*)sColorOthers.GetBuf(), -1, &rect, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	::DrawEdge(hDC, &rect, EDGE_ETCHED, BF_BOTTOM);
	if(nIndexSave == 100)
	{
		::InflateRect(&rect, -3, -2);
		::DrawEdge(hDC, &rect, EDGE_ETCHED, BF_RECT);
		//CC_APP_GetDrawActor()->DrawFocusFrame(hDC, 3, 3, rect.right - 7, 13);
	}

	for(int nIndex = 0; nIndex < 40; nIndex++)
	{
		rect.left = 3 + (nIndex%8) * (CC_PICKCOLOR_WH + CC_PICKCOLOR_MARGIN);
		rect.top = 23 + (int)((nIndex - (nIndex%8))/8) * (CC_PICKCOLOR_WH + CC_PICKCOLOR_MARGIN);
		rect.right = rect.left + CC_PICKCOLOR_WH;
		rect.bottom = rect.top + CC_PICKCOLOR_WH;

		::SetBkColor(hDC, g_crColors[nIndex].crColor);
		::ExtTextOut(hDC, 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL);
		::InflateRect(&rect, 1, 1);
		DrawEdge(hDC, &rect, BDR_SUNKENOUTER, BF_SOFT|BF_RECT);
		if(nIndexSave == nIndex)
		{
			::InflateRect(&rect, -2, -2);
			DrawEdge(hDC, &rect, EDGE_ETCHED, BF_RECT);
		}
		if(g_crColors[nIndex].crColor == _corBackground)
		{
			::InflateRect(&rect, -4, -4);
			CC_APP_GetDrawActor()->InvertBorder(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, hDC, 1);
		}
	}
	::SelectObject(hDC, holdFont);
}

void PickColorCtl::OnMouseMove(MouseEventArgs* e)
{
	ToolItem::OnMouseMove(e);
	if(!GetIsMouseOn())
	{
		if(!CC_BIT_HAS(_Flags, CC_FLAG_PICKCOLOR_MSTART)) {
			CC_BIT_SET(_Flags, CC_FLAG_PICKCOLOR_MSTART, true);
			::SetCursor(::LoadCursor(NULL, IDC_CROSS));
		}

		POINT ptScreen;
		GetCursorPos(&ptScreen);
		HDC hDC = CreateDC(_T("DISPLAY"), 0, 0, 0);
		COLORREF getcolor = GetPixel(hDC, ptScreen.x, ptScreen.y);
		DeleteDC(hDC);
		if(getcolor != _corBackground || !App::GetToolTip()->GetVisible())
		{
			EventArgs ee(m_hWnd, 0, 0, (LPARAM)getcolor);
			OnValueChanged(&ee);

			if(App::GetToolTipActive())
			{
				App::GetToolTip()->Show(NULL, NULL, -1, -1, false, true);

				cc::Str sText;
				sText.Format(_T("#%02x%02x%02x"), GetRValue(getcolor), GetGValue(getcolor), GetBValue(getcolor));
				App::GetToolTip()->Show((const TCHAR*)sText.GetBuf(), NULL, -1, -1, false, true);
			}
		}

		if(App::GetToolTip()->GetActive())
		{
			App::GetToolTip()->KeepShow(false);
		}
	}
}

void PickColorCtl::OnMouseUp(MouseEventArgs* e)
{
	ToolItem::OnMouseUp(e);
	CC_BIT_SET(_Flags, CC_FLAG_PICKCOLOR_MSTART, false);
	::SetCursor(::LoadCursor(NULL, IDC_ARROW));
	App::GetToolTip()->Show(NULL, NULL, -1, -1, false);
}

void PickColorCtl::Pop_MouseMove(Object* sender, MouseEventArgs* e)
{
	POINT pt;
	GetCursorPos(&pt);
	::ScreenToClient(_pFloater->m_hWnd, &pt);
	if(pt.x >= 3 && pt.y >= 3 && pt.x <= (CC_PICKCOLORCTL_WIN_WIDTH - 3) && pt.y <= 19)
	{
		if(nIndexSave != 100)
		{
			nIndexSave = 100;
			::InvalidateRect(_pFloater->m_hWnd, NULL, TRUE);
		}
		HWND hWnd = CC_APP_GetActiveFormHwnd();
		if(hWnd != NULL)
		{
			::SendMessage(hWnd, CC_WM_MAINMSG, CC_WM_MAINMSG_WPARAM_CLEARSTATUS, (LPARAM)NULL);
		}
	}
	else
	{
		pt.x = (pt.x - 3) / (CC_PICKCOLOR_WH + CC_PICKCOLOR_MARGIN);
		pt.y = (pt.y - 23) / (CC_PICKCOLOR_WH + CC_PICKCOLOR_MARGIN);
		int nInd = pt.y * 8 + pt.x;
		if(nInd >= 0 && nInd <= 39)
		{
			if(nIndexSave != nInd)
			{
				nIndexSave = nInd;
				::InvalidateRect(_pFloater->m_hWnd, NULL, TRUE);

				HWND hWnd = CC_APP_GetActiveFormHwnd();
				if(hWnd != NULL)
				{
					cc::Str sKey = cc::Str().Format(_T("Lang.ColorName%d"), nInd);
					cc::Str sText = cc::Str(g_crColors[nInd].szName);
					if(CC_APP_GetLang()->Exist(sKey))
					{
						sText = CC_APP_GetLang()->Get(sKey, sText);
					}
					App::GetToolTip()->Show((const TCHAR*)NULL, NULL, -1, -1, false, false);
					::SendMessage(hWnd, CC_WM_MAINMSG, CC_WM_MAINMSG_WPARAM_TEXTSTATUS, (LPARAM)(const TCHAR*)sText.GetBuf());
				}
			}
		}
	}
}

void PickColorCtl::Pop_MouseDown(Object* sender, MouseEventArgs* e)
{
	POINT pt;
	GetCursorPos(&pt);
	::ScreenToClient(_pFloater->m_hWnd, &pt);
	if(pt.x >= 3 && pt.y >= 3 && pt.x <= (CC_PICKCOLORCTL_WIN_WIDTH - 3) && pt.y <= 19)
	{
		OnClick(e->baseArgs);
		return;
	}

	pt.x = (pt.x - 3) / (CC_PICKCOLOR_WH + CC_PICKCOLOR_MARGIN);
	pt.y = (pt.y - 23) / (CC_PICKCOLOR_WH + CC_PICKCOLOR_MARGIN);
	int nInd = pt.y * 8 + pt.x;
	if(nInd >= 0 && nInd <= 39)
	{
		e->baseArgs->LParam = (LPARAM)g_crColors[nInd].crColor;
		OnValueChanged(e->baseArgs);
		_pFloater->SetVisible(false);
		e->baseArgs->Handled = true;
		e->baseArgs->Result = TRUE;
	}
}

void PickColorCtl::Pop_MouseLeave(Object* sender, EventArgs* e)
{
	nIndexSave = -1;
	::InvalidateRect(_pFloater->m_hWnd, NULL, TRUE);
	HWND hWnd = CC_APP_GetActiveFormHwnd();
	if(hWnd != NULL)
	{
		::SendMessage(hWnd, CC_WM_MAINMSG, CC_WM_MAINMSG_WPARAM_CLEARSTATUS, (LPARAM)NULL);
	}
}

void PickColorCtl::Pop_Deactivate(Object* sender, EventArgs* e)
{
	_pFloater->SetVisible(false);
	this->SetPushed(false);
}

void PickColorCtl::OnClick(EventArgs *e)
{
	_pFloater->SetVisible(false);
	COLORREF cor = this->GetBackground();
	if(cc::Util::selectColor(cor, true, NULL, -1, m_hWnd))
	{
		e->LParam = (LPARAM)cor;
		OnValueChanged(e);
	}
	this->SetPushed(false);
}
// private/protected/internal used methods
///////////////////////////////////////////////////////////////////////////////

//===========================================================cc::win::PickPercentCtl
///////////////////////////////////////////////////////////////////////////////
// constructors/destructor
PickPercentCtl::PickPercentCtl()
{
	//not need OuterStyle::AutoSize,OuterStyle::TransparentParentErase,also not need PushButton&ContextMenuButton
	//_StyleArray[(int)StyleType::Style] |= WS_TABSTOP;
	//cannot use OuterStyle::AutoSize
	_StyleArray[(int)StyleType::Outer] |= OuterStyle::OuterEdge|OuterStyle::ContextMenuButton|OuterStyle::PushButton;
	_StyleArray[(int)StyleType::Outer] &= ~(OuterStyle::AutoSize);
	::SetRect(&_Rect, 0, 0, 15, 20);
	::SetRect(&_Margin, 3, 2, 3, 2);
#ifdef _DEBUG_
	SetName(_T("PickPercentCtl"));
#endif
	SetText(_T("0"));
	_pFloater = new FloatContainer();
	_pFloater->SetStyle(StyleType::Style, WS_CAPTION|WS_THICKFRAME, 0);
	_pFloater->SetStyle(StyleType::ExStyle, 0, WS_EX_TOPMOST);
	_pFloater->SetSizeBaseOnClient(204, 25);
	_pFloater->Deactivate += EventHandler((Object*)this, (EventFun)&PickPercentCtl::Pop_Deactivate);
	CC_BIT_ON(_Flags, CC_FLAG_TOOLITEM_NEEDDESTROY);

	hScrollBar = new HScrollBar();
	hScrollBar->SetLocation(1, 2);
	hScrollBar->SetSize(200, 17);
	hScrollBar->SetMinMaxValue(0, 255);
	hScrollBar->SetValue(0);
	hScrollBar->ValueChanged += EventHandler((Object*)this, (EventFun)&PickPercentCtl::Pop_ValueChanged);
	hScrollBar->Anchor = (AnchorStyles::Type)(AnchorStyles::Top | AnchorStyles::Left | AnchorStyles::Right);
	_pFloater->AddControl(hScrollBar);
	m_pResultCtl = NULL;
	nValueChangedCount = 0;
}

PickPercentCtl::~PickPercentCtl()
{
	Dispose();
}

void PickPercentCtl::Dispose()
{
	if(!GetIsDispose())
	{
		if(CC_BIT_HAS(_Flags, CC_FLAG_PICKPERCENTCTL_NEEDCLBK) && m_pResultCtl != NULL)
		{
			m_pResultCtl->LostFocus -= EventHandler((Object*)this, (EventFun)&PickPercentCtl::ResultCtl_LostFocus);
		}
		ToolItem::Dispose();
	}
}
// constructors/destructor
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// private/protected/internal used methods
//cannot use OuterStyle::AutoSize,here do nothing
RECT PickPercentCtl::CalcuteRect(bool updateRect, int nExpectX, int nExpectY, int nExpectCX, int nExpectCY, bool isFourceCalcute)
{
	return _Rect;
}

//keyArr is param address,but sHead is by value
void PickPercentCtl::DoConfig(bool isFromIni, KeyArr& keyArr, cc::Str sThisHead)
{
	if(!HasStyle(StyleType::Outer, OuterStyle::AutoConfig))
	{
		return;
	}
	if(isFromIni)
	{
		//if not exist,as usually
		if(keyArr.Exist(sThisHead))
		{
			this->SetValue((int)keyArr.Get(sThisHead, (DWORD)0));
		}
	}
	else
	{
		keyArr.Set(sThisHead, (DWORD)this->GetValue());
	}
}

void PickPercentCtl::SetResultControl(Control* pResultCtl, const cc::Str sResultFormat, bool textChangedCallback)
{
	if(CC_BIT_HAS(_Flags, CC_FLAG_PICKPERCENTCTL_NEEDCLBK) && m_pResultCtl != NULL)
	{
		m_pResultCtl->LostFocus -= EventHandler((Object*)this, (EventFun)&PickPercentCtl::ResultCtl_LostFocus);
	}
	CC_BIT_OFF(_Flags, CC_FLAG_PICKPERCENTCTL_NEEDCLBK);
	m_pResultCtl = NULL;
	m_sResultFormat = sResultFormat;
	if(pResultCtl != NULL)
	{
		//avoid dead loop,should first set m_pResultCtl to NULL
		if(m_sResultFormat.GetLength() <= 0 || m_sResultFormat.Equals(_T("%d")))
		{
			this->SetValue(cc::Str(pResultCtl->GetText()).ToInt());
			if(textChangedCallback)
			{
				CC_BIT_ON(_Flags, CC_FLAG_PICKPERCENTCTL_NEEDCLBK);
				pResultCtl->LostFocus += EventHandler((Object*)this, (EventFun)&PickPercentCtl::ResultCtl_LostFocus);
			}
		}
	}
	m_pResultCtl = pResultCtl;
}

void PickPercentCtl::SetMinMaxValue(int nMin, int nMax, bool bRedraw)
{
	hScrollBar->SetMinMaxValue(nMin, nMax, bRedraw);
}

int PickPercentCtl::GetMinValue()
{
	return hScrollBar->GetMinValue();
}

int PickPercentCtl::GetMaxValue()
{
	return hScrollBar->GetMaxValue();
}

int PickPercentCtl::SetValue(int nValue, bool bRedraw)
{
	int n = hScrollBar->SetValue(nValue, bRedraw);
	EventArgs ee(m_hWnd, 0, 0, (LPARAM)nValue);
	OnValueChanged(&ee);
	//cc::Str str;
	//str.Format(_T("%d"), nValue);
	//this->SetText((const TCHAR*)str.GetBuf());
	//for ScrollBar::OnScroll's OnValueChanged has filter:"if(nos != _nValue)",so need OnValueChanged and hScrollBar->SetValue
	return n;
}

int PickPercentCtl::GetValue()
{
	if(m_pResultCtl != NULL && m_pResultCtl->Focused())
	{
		ResultCtl_LostFocus(m_pResultCtl, NULL);
	}
	return hScrollBar->GetValue();
}

HScrollBar* PickPercentCtl::GetScrollBar()
{
	return hScrollBar;
}

//int value = (int)e->LParam;
void PickPercentCtl::OnValueChanged(EventArgs* e)
{
	int nValue = (int)e->LParam;
	cc::Str str;
	if(m_sResultFormat.GetLength() > 0)
	{
		str.Format((const TCHAR*)m_sResultFormat.GetBuf(), nValue);
	}
	else
	{
		str.Format(_T("%d"), nValue);
	}
	this->SetText((const TCHAR*)str.GetBuf());
	if(m_pResultCtl != NULL)
	{
		m_pResultCtl->SetText((const TCHAR*)str.GetBuf());
	}
	if(nValueChangedCount <= 0)
	{
		//for dead loop
		nValueChangedCount = 1;
		ValueChanged.Invoke((Object*)this, e);
		nValueChangedCount = 0;
	}
}

void PickPercentCtl::Pop_Deactivate(Object* sender, EventArgs* e)
{
	_pFloater->SetVisible(false);
	this->SetPushed(false);
}

void PickPercentCtl::Pop_ValueChanged(Object* sender, EventArgs* e)
{
	OnValueChanged(e);
}

void PickPercentCtl::ResultCtl_LostFocus(Object* sender, EventArgs* e)
{
	if(m_pResultCtl == sender)
	{
		int n = cc::Str(m_pResultCtl->GetText()).ToInt();
		if(n !=  hScrollBar->GetValue())
		{
			this->SetValue(n);
		}
	}
}
// private/protected/internal used methods
///////////////////////////////////////////////////////////////////////////////

//===========================================================cc::win::ComboMenu
///////////////////////////////////////////////////////////////////////////////
// constructors/destructor
ComboMenu::ComboMenu()
{
	//not need OuterStyle::AutoSize,OuterStyle::TransparentParentErase,also not need PushButton&ContextMenuButton
	//_StyleArray[(int)StyleType::Style] |= WS_TABSTOP;
	//cannot use OuterStyle::AutoSize
	_StyleArray[(int)StyleType::Outer] |= OuterStyle::OuterEdge|OuterStyle::ContextMenuButton|OuterStyle::PushButton;
	_StyleArray[(int)StyleType::Outer] &= ~(OuterStyle::AutoSize);
	_StyleArray[(int)StyleType::OwnerDrawText] = DT_VCENTER|DT_SINGLELINE;
	::SetRect(&_Rect, 0, 0, 100, 19);
	::SetRect(&_Margin, 3, 2, 3, 2);
#ifdef _DEBUG_
	SetName(_T("ComboMenu"));
#endif
	SetText(_T(" "));

	_pContextMenu = new ContextMenu();
	_pContextMenu->SetAllOwnerDraw(true);
	CC_BIT_ON(_Flags, CC_FLAG_TOOLITEM_NEEDDESTROY);
	_nSelectedIndex = -1;
}

ComboMenu::~ComboMenu()
{
}
// constructors/destructor
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// private/protected/internal used methods
void ComboMenu::SetConfigData(const cc::Str& sData, const cc::Str& sTips, const cc::Str& sTipsExt, bool isAppend)
{
	//CC_ASSERT(::IsWindow(m_hWnd));

	if(sData.IndexOf(_T(CONTROL_LINE_SPLIT)) >= 0)
	{
		int nIndex = GetSelectedIndex();
		if(!isAppend)
		{
			ItemClear();
		}
		cc::Str* strArr = NULL;
		cc::Str* strArr2 = NULL;
		cc::Str* strArr3 = NULL;
		int nInd = sData.Split(_T(CONTROL_LINE_SPLIT), &strArr);
		int nInd2 = sTips.Split(_T(CONTROL_LINE_SPLIT), &strArr2);
		if(nInd2 <= 0)
		{
			nInd2 = -1;
		}
		else
		{
			SetTips(NULL);
		}
		int nInd3 = sTipsExt.Split(_T(CONTROL_LINE_SPLIT), &strArr3);
		if(nInd3 <= 0)
		{
			nInd3 = -1;
		}
		else
		{
			SetTipsExt(NULL);
		}
		SetText(_T(" "));
		for(int i = 0; i < nInd; i++)
		{
			MenuItem* menu = new MenuItem();
			menu->SetText((TCHAR*)strArr[i].GetBuf());
			if(i <= nInd2)
			{
				menu->SetTips((TCHAR*)strArr2[i].GetBuf());
			}
			if(i <= nInd3)
			{
				menu->SetTipsExt((TCHAR*)strArr3[i].GetBuf());
			}
			//menu->SetIcon(_pImageList, IDI_ICON_EXIT);
			menu->Click += EventHandler((Object*)this, (EventFun)&ComboMenu::Pop_SelectedIndexChanged);
			_pContextMenu->ItemAdd(menu);
		}
		if(nInd2 > nInd)
		{
			SetTips((TCHAR*)strArr2[nInd].GetBuf());
		}
		if(nInd3 > nInd)
		{
			SetTipsExt((TCHAR*)strArr3[nInd].GetBuf());
		}
		if(strArr != NULL)
		{
			delete []strArr;
		}
		if(strArr2 != NULL)
		{
			delete []strArr2;
		}
		if(strArr3 != NULL)
		{
			delete []strArr3;
		}
		SetSelectedIndex(nIndex);
	}
}

void ComboMenu::CreateHandle()
{
	cc::Str sData = cc::Str(_Text);
	cc::Str sTips = cc::Str(_Tips);
	cc::Str sTipsExt = cc::Str(_TipsExt);
	SetConfigData(sData, sTips, sTipsExt, false);
	ToolItem::CreateHandle();
}

//keyArr is param address,but sHead is by value
void ComboMenu::DoConfig(bool isFromIni, KeyArr& keyArr, cc::Str sThisHead)
{
	if(!HasStyle(StyleType::Outer, OuterStyle::AutoConfig))
	{
		return;
	}
	if(isFromIni)
	{
		//if not exist,as usually
		if(keyArr.Exist(sThisHead))
		{
			this->SetSelectedIndex((int)keyArr.Get(sThisHead, (DWORD)0));
		}
	}
	else
	{
		keyArr.Set(sThisHead, (DWORD)this->GetSelectedIndex());
	}
}

void ComboMenu::DoSetLang(KeyArr& keyArr, cc::Str sThisHead)
{
	ToolItem::DoSetLang(keyArr, sThisHead);
	sThisHead.Append(_T(".Data"));
	if(keyArr.Exist(sThisHead))
	{
		cc::Str sData = keyArr.Get(sThisHead);
		cc::Str sTips = keyArr.Get(sThisHead.Append(_T("Tips")));
		cc::Str sTipsExt = keyArr.Get(sThisHead.Append(_T("Ext")));
		if(m_hWnd != NULL)
		{
			SetConfigData(sData, sTips, sTipsExt, false);
		}
		else
		{
			SetText((const TCHAR*)sData.GetBuf());
		}
	}
}

ContextMenu* ComboMenu::GetContextMenu()
{
	return _pContextMenu;
}

void ComboMenu::SetContextMenu(ContextMenu* pContMenu, bool needDestroy)
{
	if(_pContextMenu != NULL && CC_BIT_HAS(_Flags, CC_FLAG_TOOLITEM_NEEDDESTROY))
	{
		_pContextMenu->Dispose();
		delete _pContextMenu;
		_pContextMenu = NULL;
	}
	_pContextMenu = pContMenu;
	CC_BIT_SET(_Flags, CC_FLAG_TOOLITEM_NEEDDESTROY, needDestroy);
}

void ComboMenu::SetSelectedIndex(int selectedIndex)
{
	if(selectedIndex != _nSelectedIndex && selectedIndex >= 0 && selectedIndex < _pContextMenu->ItemCount())
	{
		if(_nSelectedIndex >= 0 && _nSelectedIndex < _pContextMenu->ItemCount())
		{
			((MenuItem*)_pContextMenu->ItemGet(_nSelectedIndex))->SetChecked(false);
		}
		_nSelectedIndex = selectedIndex;
		MenuItem* menu = (MenuItem*)_pContextMenu->ItemGet(_nSelectedIndex);
		menu->SetChecked(true);
		this->SetText(menu->GetText());
	}
}

int ComboMenu::GetSelectedIndex()
{
	return _nSelectedIndex;
}

int ComboMenu::ItemAdd(const TCHAR* txt, const TCHAR* txtTips, const TCHAR* txtTipsExt, int index, ImageList* imageList, int nIconID, int nIconIDTips, int nIconIDTipsExt)
{
	MenuItem* menu = new MenuItem();
	menu->SetText(txt);
	menu->SetIcon(imageList, nIconID);
	menu->SetTips(txtTips, imageList, nIconIDTips);
	menu->SetTipsExt(txtTipsExt, imageList, nIconIDTipsExt);
	menu->Click += EventHandler((Object*)this, (EventFun)&ComboMenu::Pop_SelectedIndexChanged);
	return _pContextMenu->ItemAdd(menu, index);
}

void ComboMenu::ItemDel(const TCHAR* txt)
{
	for(int n = 0; n < _pContextMenu->ItemCount(); n++)
	{
		MenuItem* menu = (MenuItem*)_pContextMenu->ItemGet(n);
		if(cc::Str::Cmp(txt, menu->GetText()) == 0)
		{
			_pContextMenu->ItemRemove(n);
			break;
		}
	}
}

void ComboMenu::ItemDel(int index)
{
	if(index >= 0 && index < _pContextMenu->ItemCount())
	{
		_pContextMenu->ItemRemove(index);
	}
}

void ComboMenu::ItemSet(int index, const TCHAR* txt)
{
	if(index >= 0 && index < _pContextMenu->ItemCount())
	{
		((MenuItem*)_pContextMenu->ItemGet(index))->SetText(txt);
	}
}

const TCHAR* ComboMenu::ItemGet(int index)
{
	if(index >= 0 && index < _pContextMenu->ItemCount())
	{
		return ((MenuItem*)_pContextMenu->ItemGet(index))->GetText();
	}
	return NULL;
}

int ComboMenu::ItemCount() const
{
	return _pContextMenu->ItemCount();
}

void ComboMenu::ItemClear()
{
	_pContextMenu->ItemClear();
}

//int value = (int)e->LParam;
void ComboMenu::Pop_SelectedIndexChanged(Object* sender, EventArgs* e)
{
	MenuItem* menu = (MenuItem*)sender;
	this->SetText(menu->GetText());

	if(_nSelectedIndex >= 0 && _nSelectedIndex < _pContextMenu->ItemCount())
	{
		((MenuItem*)_pContextMenu->ItemGet(_nSelectedIndex))->SetChecked(false);
	}
	_nSelectedIndex = _pContextMenu->ItemGetIndex(menu);
	((MenuItem*)_pContextMenu->ItemGet(_nSelectedIndex))->SetChecked(true);
	e->LParam = (LPARAM)_nSelectedIndex;
	SelectedIndexChanged.Invoke((Object*)this, e);
}

int ComboMenu::FindString(int nIndexStart, LPCTSTR lpszFind, bool IgnoreCase) const
{
	for(; nIndexStart < _pContextMenu->ItemCount(); nIndexStart++)
	{
		MenuItem* menu = (MenuItem*)_pContextMenu->ItemGet(nIndexStart);
		if(cc::Str::Search(menu->GetText(), lpszFind, -1, IgnoreCase) >= 0)
		{
			return nIndexStart;
		}
	}
	return -1;
}

int ComboMenu::FindStringExact(int nIndexStart, LPCTSTR lpszFind, bool IgnoreCase) const
{
	for(; nIndexStart < _pContextMenu->ItemCount(); nIndexStart++)
	{
		MenuItem* menu = (MenuItem*)_pContextMenu->ItemGet(nIndexStart);
		if(cc::Str::Cmp(lpszFind, menu->GetText(), IgnoreCase) == 0)
		{
			return nIndexStart;
		}
	}
	return -1;
}

int ComboMenu::FindStringOption(int nIndexStart, LPCTSTR lpszFind, int searchType, bool IgnoreCase) const
{
	int len = cc::Str::Len(lpszFind);
	if(len <= 0)
	{
		return -1;
	}
	for(; nIndexStart < _pContextMenu->ItemCount(); nIndexStart++)
	{
		MenuItem* menu = (MenuItem*)_pContextMenu->ItemGet(nIndexStart);
		cc::Str sText = menu->GetText();
		if(sText.GetLength() >= len)
		{
			//if(cc::Str::NCmp(lpszFind, menu->GetText(), len, IgnoreCase) == 0)
			//{
			//	return nIndexStart;
			//}
			if(searchType == 1 && sText.StartsWith(lpszFind, IgnoreCase))
			{
				return nIndexStart;
			}
			else if(searchType == 2 && sText.EndsWith(lpszFind, IgnoreCase))
			{
				return nIndexStart;
			}
			else
			{
				if(sText.IndexOf(lpszFind, IgnoreCase) >= 0)
				{
					return nIndexStart;
				}
			}
		}
	}
	return -1;
}
// private/protected/internal used methods
///////////////////////////////////////////////////////////////////////////////

//===========================================================cc::win::ComboList
///////////////////////////////////////////////////////////////////////////////
// constructors/destructor
ComboList::ComboList()
{
	//not need OuterStyle::AutoSize,OuterStyle::TransparentParentErase,also not need PushButton&ContextMenuButton
	//_StyleArray[(int)StyleType::Style] |= WS_TABSTOP;
	//cannot use OuterStyle::AutoSize
	_StyleArray[(int)StyleType::Outer] |= OuterStyle::OuterEdge|OuterStyle::ContextMenuButton|OuterStyle::PushButton;
	_StyleArray[(int)StyleType::Outer] &= ~(OuterStyle::AutoSize);
	_StyleArray[(int)StyleType::OwnerDrawText] = DT_VCENTER|DT_SINGLELINE;
	::SetRect(&_Rect, 0, 0, 100, 19);
	::SetRect(&_Margin, 3, 2, 3, 2);
#ifdef _DEBUG_
	SetName(_T("ComboList"));
#endif
	_szPop.cx = -1;
	_szPop.cy = -1;
	SetText(_T(" "));
	_pFloater = new FloatContainer();
	_pFloater->SetStyle(StyleType::Style, WS_CAPTION, 0);
	_pFloater->SetStyle(StyleType::ExStyle, 0, WS_EX_TOPMOST);
	_pFloater->SetSizeBaseOnClient(204, 145);
	_pFloater->Deactivate += EventHandler((Object*)this, (EventFun)&ComboList::Pop_Deactivate);
	CC_BIT_ON(_Flags, CC_FLAG_TOOLITEM_NEEDDESTROY);

	_pListBox = new ListBox();
	_pListBox->SetLocation(1, 0);
	_pListBox->SetSize(200, 145 - 2);
	_pListBox->MouseMove += MouseEventHandler((Object*)this, (MouseEventFun)&ComboList::Pop_MouseMove);
	_pListBox->MouseDown += MouseEventHandler((Object*)this, (MouseEventFun)&ComboList::Pop_MouseDown);
	_pListBox->KeyDown += KeyEventHandler((Object*)this, (KeyEventFun)&ComboList::Pop_KeyDown);
	//_pListBox->SelectedIndexChanged += EventHandler((Object*)this, (EventFun)&ComboList::Pop_SelectedIndexChanged);
	//_pListBox->Anchor = (AnchorStyles::Type)(AnchorStyles::Top | AnchorStyles::Left | AnchorStyles::Right | AnchorStyles::Bottom);
	_pListBox->Dock = DockStyles::Fill;
	_pListBox->SetStyle(StyleType::ExStyle, WS_EX_CLIENTEDGE, 0);
	_pFloater->AddControl(_pListBox);
	_nSelectedIndex = -1;
	//_pFloater->CreateControl();
}

ComboList::~ComboList()
{
	Dispose();
}

void ComboList::Dispose()
{
	if(!GetIsDispose())
	{
		ToolItem::Dispose();
	}
}
// constructors/destructor
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// private/protected/internal used methods
void ComboList::SetPopSize(int width, int height, bool isResize, int minHeight, int MaxWidth, int maxHeight)
{
	_szPop.cx = width;
	_szPop.cy = height;
	_pFloater->SetSize(width, height);

	_pFloater->SetStyleBool(StyleType::Style, WS_THICKFRAME, isResize);
	_pFloater->SetMinSize(_Rect.right - _Rect.left, minHeight);
	_pFloater->SetMaxSize(MaxWidth, maxHeight);
}

void ComboList::OnResize(EventArgs* e)
{
	ToolItem::OnResize(e);
	int nH = _pFloater->GetMinSize().cy;
	//i think _Rect is refreshed at Control::OnResize
	_pFloater->SetMinSize(_Rect.right - _Rect.left, nH);
	RECT rect = _pFloater->GetBounds();
	if(_szPop.cx <= 0)
	{
		_pFloater->SetSize(_Rect.right - _Rect.left, rect.bottom - rect.top);
	}
	else
	{
		_pFloater->SetSize(CC_MAX(_szPop.cx, _Rect.right - _Rect.left), rect.bottom - rect.top);
	}
}

void ComboList::SetConfigData(const cc::Str& sData, bool isAppend)
{
	if(_pListBox->m_hWnd != NULL)
	{
		_pListBox->SetConfigData(sData, isAppend);
	}
	else
	{
		_pListBox->SetText(sData);
	}
}

void ComboList::CreateHandle()
{
	ToolItem::CreateHandle();
	_pFloater->SetParent(this, true);
	_pFloater->SetVisible(false);
	_pFloater->CreateControl();
	SetConfigData(cc::Str(_Text), false);
	if(_pListBox->m_hWnd != NULL)
	{
		_pListBox->SetSelectedIndex(_nSelectedIndex);
		this->SetText(_pListBox->ItemGet(_nSelectedIndex));
		return;
	}
}

//keyArr is param address,but sHead is by value
void ComboList::DoConfig(bool isFromIni, KeyArr& keyArr, cc::Str sThisHead)
{
	if(!HasStyle(StyleType::Outer, OuterStyle::AutoConfig))
	{
		return;
	}
	if(isFromIni)
	{
		//if not exist,as usually
		if(keyArr.Exist(sThisHead))
		{
			this->SetSelectedIndex((int)keyArr.Get(sThisHead, (DWORD)0));
		}
	}
	else
	{
		keyArr.Set(sThisHead, (DWORD)this->GetSelectedIndex());
	}
}

void ComboList::DoSetLang(KeyArr& keyArr, cc::Str sThisHead)
{
	ToolItem::DoSetLang(keyArr, sThisHead);
	sThisHead.Append(_T(".Data"));
	if(keyArr.Exist(sThisHead))
	{
		cc::Str sData = keyArr.Get(sThisHead);
		if(m_hWnd != NULL)
		{
			SetConfigData(sData, false);
		}
		else
		{
			SetText((const TCHAR*)sData.GetBuf());
		}
	}
}

ListBox* ComboList::GetListBox()
{
	return _pListBox;
}

//int value = (int)e->LParam;
void ComboList::SetSelectedIndex(int selectedIndex)
{
	if(_nSelectedIndex != selectedIndex)
	{
		_nSelectedIndex = selectedIndex;
		if(_pListBox->m_hWnd != NULL)
		{
			_pListBox->SetSelectedIndex(selectedIndex);
			_nSelectedIndex = _pListBox->GetSelectedIndex();
			this->SetText(_pListBox->ItemGet(_nSelectedIndex));
			EventArgs ee(m_hWnd, 0, 0, (LPARAM)_nSelectedIndex);
			SelectedIndexChanged.Invoke((Object*)this, &ee);
			return;
		}
	}
}

int ComboList::GetSelectedIndex()
{
	if(_pListBox->m_hWnd != NULL)
	{
		_nSelectedIndex = _pListBox->GetSelectedIndex();
	}
	return _nSelectedIndex;
}

int ComboList::ItemAdd(const TCHAR* txt, int index)
{
	return _pListBox->ItemAdd(txt, index);
}

void ComboList::ItemDel(const TCHAR* txt)
{
	_pListBox->ItemDel(txt);
}

void ComboList::ItemDel(int index)
{
	_pListBox->ItemDel(index);
}

void ComboList::ItemSet(int index, const TCHAR* txt)
{
	_pListBox->ItemSet(index, txt);
}

cc::Str ComboList::ItemGet(int index)
{
	return _pListBox->ItemGet(index);
}

int ComboList::ItemCount() const
{
	return _pListBox->ItemCount();
}

void ComboList::ItemClear()
{
	_pListBox->ItemClear();
}

int ComboList::FindString(int nIndexStart, LPCTSTR lpszFind) const
{
	return _pListBox->FindString(nIndexStart, lpszFind);
}

int ComboList::FindStringExact(int nIndexStart, LPCTSTR lpszFind) const
{
	return _pListBox->FindStringExact(nIndexStart, lpszFind);
}

int ComboList::FindStringOption(int nIndexStart, LPCTSTR lpszFind, bool IgnoreCase) const
{
	return _pListBox->FindStringOption(nIndexStart, lpszFind, IgnoreCase);
}

void ComboList::Pop_Deactivate(Object* sender, EventArgs* e)
{
	_pFloater->SetVisible(false);
	this->SetPushed(false);
	_pListBox->SetSelectedIndex(_nSelectedIndex);
}

void ComboList::Pop_MouseMove(Object* sender, MouseEventArgs* e)
{
	Pop_MouseDown(sender, NULL);
}

void ComboList::Pop_MouseDown(Object* sender, MouseEventArgs* e)
{
	POINT point;
	GetCursorPos(&point);
	::ScreenToClient(_pFloater->m_hWnd, &point);

	int cyItem = _pListBox->GetItemHeight(0);
	if(point.y < cyItem * _pListBox->ItemCount())
	{
		int curPos = ::GetScrollPos(_pListBox->m_hWnd, SB_HORZ);
		point.x += curPos;
		int nIndex = _pListBox->GetTopIndex() + point.y / cyItem;
		if(nIndex >= 0)
		{
			if(e != NULL && e->Button == MouseButtons::Left)
			{
				this->SetSelectedIndex(nIndex);
				Pop_Deactivate(NULL, NULL);
			}
			else
			{
				_pListBox->SetSelectedIndex(nIndex);
			}
		}
	}
}

void ComboList::Pop_KeyDown(Object* sender, KeyEventArgs* e)
{
	if((int)e->baseArgs->WParam == ' ' || (int)e->baseArgs->WParam == 13)
	{
		this->SetSelectedIndex(_pListBox->GetSelectedIndex());
		Pop_Deactivate(NULL, NULL);
	}
}
// private/protected/internal used methods
///////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CAMSEdit

// Constructs the object with the default attributes
CAMSEdit::CAMSEdit()
{
}

// Destroys the object (virtual).
CAMSEdit::~CAMSEdit()
{
}

// Returns the control's text without leading or trailing blanks.
cc::Str CAMSEdit::GetTrimText()
{
	cc::Str strText = cc::Str(GetText());
    strText.Trim();
    return strText;
}

// Returns the control's value in a valid format.
cc::Str CAMSEdit::GetValidText()
{
	return cc::Str(GetText());
}

// Redraws the window's text.
void CAMSEdit::Redraw()
{
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
	{
		cc::Str strText = GetValidText();
		if(!strText.Equals(GetText()))
		{
			SetText(strText);
		}
	}
}

// Returns true if the given character should be entered into the control.
bool CAMSEdit::ShouldEnter(TCHAR) const
{
	return true;
}

void CAMSEdit::Clear()
{
	TextBox::Clear();
	Redraw();
}

void CAMSEdit::Paste()
{
	TextBox::Paste();
	Redraw();
}

void CAMSEdit::Cut()
{
	TextBox::Cut();
	Redraw();
}

// Handles the WM_SETTEXT message to ensure that text (set via SetWindowText) is valid.
void CAMSEdit::OnTextChanged(EventArgs *e)
{
	TextBox::OnTextChanged(e);
	Redraw();
}

/////////////////////////////////////////////////////////////////////////////
// CAMSEdit::SelectionSaver

// Constructs the selection saver object for the given edit control.
// It then saves the edit control's current selection.
CAMSEdit::SelectionSaver::SelectionSaver(TextBox* pEdit) : m_pEdit(pEdit)
{
	//ASSERT(pEdit);
	pEdit->GetSel(m_nStart, m_nEnd);
}

// Constructs the selection saver object for the given edit control.
// It then saves the given nStart and nEnd values.
CAMSEdit::SelectionSaver::SelectionSaver(TextBox* pEdit, int nStart, int nEnd) :
	m_pEdit(pEdit),
	m_nStart(nStart),
	m_nEnd(nEnd)
{
	//ASSERT(pEdit);
	//ASSERT(nStart <= nEnd);
}

// Destroys the object and restores the selection to the saved start and end values.
CAMSEdit::SelectionSaver::~SelectionSaver()
{
	if(m_pEdit)
	{
		m_pEdit->SetSel(m_nStart, m_nEnd, true);
	}
}

// Changes the start and end values to nStart and nEnd respectively.
void CAMSEdit::SelectionSaver::MoveTo(int nStart, int nEnd)
{
	//ASSERT(nStart <= nEnd);

	m_nStart = nStart;
	m_nEnd = nEnd;
}

// Changes the start and end values by nStart and nEnd respectively.
void CAMSEdit::SelectionSaver::MoveBy(int nStart, int nEnd)
{
	m_nStart += nStart;
	m_nEnd += nEnd;

	//ASSERT(m_nStart <= m_nEnd);
}

// Changes both the start and end values by nPos.
void CAMSEdit::SelectionSaver::MoveBy(int nPos)
{
	m_nStart += nPos;
	m_nEnd += nPos;
}

// Changes both the start and end values by nPos.
void CAMSEdit::SelectionSaver::operator+=(int nPos)
{
	MoveBy(nPos);
}

// Returns the value for the selection's start.
int CAMSEdit::SelectionSaver::GetStart() const
{
	return m_nStart;
}

// Returns the value for the selection's end.
int CAMSEdit::SelectionSaver::GetEnd() const
{
	return m_nEnd;
}

// Updates the selection's start and end with the current selection.
void CAMSEdit::SelectionSaver::Update()
{
	if(m_pEdit)
	{
		m_pEdit->GetSel(m_nStart, m_nEnd);
	}
}

// Disables resetting the selection in the destructor
void CAMSEdit::SelectionSaver::Disable()
{
	m_pEdit = NULL;
}


//===========================================================cc::win::InputDateTimeCtl
///////////////////////////////////////////////////////////////////////////////
// constructors/destructor
InputDateTimeCtl::InputDateTimeCtl(int nY, int nM, int nD, int nHH, int nMM, int nSS, int nSSS, bool isY, bool isM, bool isD, bool isHH, bool isMM, bool isSS, bool isSSS)
{
	cc::Str strbuf(10, 0);
	int nLeft = 0;
	if(isY)
	{
		edtY = new TextBox();
		edtY->SetStyleNumber(true);
		edtY->SetLocation(nLeft, 0);
		edtY->SetSize(30, 17);
		strbuf.ValueOf(nY);
		edtY->SetText(strbuf);
		nLeft += 30;
		spinY = new SpinButton();
		spinY->SetRange(1000, 9999);
		spinY->SetLocation(nLeft, 0);
		spinY->SetSize(40, 27);
		nLeft += 40;
		labY = new Label();
		labY->SetLocation(nLeft, 0);
		labY->SetSize(10, 17);
		labY->SetText(_T("/"));
		nLeft += 10;
		this->AddControl(edtY);
		this->AddControl(spinY);
		this->AddControl(labY);
	}
	else
	{
		labY = NULL;
		edtY = NULL;
		spinY = NULL;
	}
	if(isM)
	{
		edtM = new TextBox();
		edtM->SetStyleNumber(true);
		edtM->SetLocation(nLeft, 0);
		edtM->SetSize(20, 17);
		strbuf.ValueOf(nM);
		edtY->SetText(strbuf);
		nLeft += 20;
		spinM = new SpinButton();
		spinM->SetRange(1, 12);
		labM = new Label();
		labM->SetLocation(nLeft, 0);
		labM->SetSize(10, 17);
		labM->SetText(_T("/"));
		nLeft += 10;
		this->AddControl(edtM);
		this->AddControl(spinM);
		this->AddControl(labM);
	}
	else
	{
		labM = NULL;
		edtM = NULL;
		spinM = NULL;
	}
	labD = NULL;
	if(isD)
	{
		edtD = new TextBox();
		edtD->SetStyleNumber(true);
		edtD->SetLocation(nLeft, 0);
		edtD->SetSize(20, 17);
		strbuf.ValueOf(nD);
		edtD->SetText(strbuf);
		nLeft += 20;
		spinD = new SpinButton();
		spinD->SetRange(1, 31);
		this->AddControl(edtD);
		this->AddControl(spinD);
	}
	else
	{
		edtD = NULL;
		spinD = NULL;
	}

	if(nLeft != 0)
	{
		nLeft += 10;
	}
	if(isHH)
	{
		edtHH = new TextBox();
		edtHH->SetStyleNumber(true);
		edtHH->SetLocation(nLeft, 0);
		edtHH->SetSize(20, 17);
		strbuf.ValueOf(nHH);
		edtHH->SetText(strbuf);
		nLeft += 20;
		labHH = new Label();
		labHH->SetLocation(nLeft, 0);
		labHH->SetSize(10, 17);
		labHH->SetText(_T(":"));
		nLeft += 10;
		spinHH = new SpinButton();
		spinHH->SetRange(0, 23);
		this->AddControl(edtHH);
		this->AddControl(spinHH);
		this->AddControl(labHH);
	}
	else
	{
		labHH = NULL;
		edtHH = NULL;
		spinHH = NULL;
	}

	if(isMM)
	{
		edtMM = new TextBox();
		edtMM->SetStyleNumber(true);
		edtMM->SetLocation(nLeft, 0);
		edtMM->SetSize(20, 17);
		strbuf.ValueOf(nMM);
		edtMM->SetText(strbuf);
		nLeft += 20;
		spinMM = new SpinButton();
		spinMM->SetRange(0, 59);
		this->AddControl(edtMM);
		this->AddControl(spinMM);
		if(isSS)
		{
			labMM = new Label();
			labMM->SetLocation(nLeft, 0);
			labMM->SetSize(10, 17);
			labMM->SetText(_T(":"));
			nLeft += 10;
			this->AddControl(labMM);
		}
		else
		{
			labMM = NULL;
		}
	}
	else
	{
		labMM = NULL;
		edtMM = NULL;
		spinMM = NULL;
	}

	if(isSS)
	{
		edtSS = new TextBox();
		edtSS->SetStyleNumber(true);
		edtSS->SetLocation(nLeft, 0);
		edtSS->SetSize(20, 17);
		strbuf.ValueOf(nSS);
		edtSS->SetText(strbuf);
		nLeft += 20;
		spinSS = new SpinButton();
		spinSS->SetRange(0, 59);
		this->AddControl(edtSS);
		this->AddControl(spinSS);
		if(isSSS)
		{
			labSS = new Label();
			labSS->SetLocation(nLeft, 0);
			labSS->SetSize(10, 17);
			labSS->SetText(_T("."));
			nLeft += 10;
			this->AddControl(labSS);
		}
		else
		{
			labSS = NULL;
		}
	}
	else
	{
		labSS = NULL;
		edtSS = NULL;
		spinSS = NULL;
	}

	labSSS = NULL;
	if(isSSS)
	{
		edtSSS = new TextBox();
		edtSSS->SetStyleNumber(true);
		edtSSS->SetLocation(nLeft, 0);
		edtSSS->SetSize(30, 17);
		strbuf.ValueOf(nSSS);
		edtSSS->SetText(strbuf);
		nLeft += 30;
		spinSSS = new SpinButton();
		spinSSS->SetRange(0, 999);
		this->AddControl(edtSSS);
		this->AddControl(spinSSS);
	}
	else
	{
		edtSSS = NULL;
		spinSSS = NULL;
	}
	this->SetSize(232, 115);
#ifdef _DEBUG_
	SetName(_T("InputDateTimeCtl"));
#endif
}

InputDateTimeCtl::~InputDateTimeCtl()
{
}
// constructors/destructor
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// private/protected/internal used methods
// private/protected/internal used methods
///////////////////////////////////////////////////////////////////////////////

//===========================================================cc::win::CancelDlg
///////////////////////////////////////////////////////////////////////////////
// constructors/destructor
//the Width&Height is for client area
CancelDlg::CancelDlg(int nDialogWidth, int nDialogHeight, int nIconWidth, int nIconHeight)
{
	m_hWndParent = NULL;
	bEnableParent = false;
	eStatus = CancelStatus::Ready;
	labMsg = new Label();
	labMsg->SetName(_T("labMsg"));
	labMsg->SetText(NULL);
	labImg = NULL;
	if(nIconWidth > 0 && nIconHeight > 0)
	{
		labImg = new PictureBox();
		labImg->SetName(_T("labImg"));
		labImg->SetText(_T(""));
		labImg->SetLocation(10, 10);
		labImg->SetSize(nIconWidth, nIconHeight);
		this->AddControl(labImg);
		labMsg->SetLocation(16 + nIconWidth, 10);
		labMsg->SetSize(nDialogWidth - nIconWidth - 32, nDialogHeight - 64);
	}
	else
	{
		labMsg->SetLocation(16, 10);
		labMsg->SetSize(nDialogWidth - 32, nDialogHeight - 64);
	}
	this->AddControl(labMsg);

	btnCancel = new Button();
	btnCancel->SetName(_T("btnCancel"));
	btnCancel->SetSize(140, 30);
	btnCancel->SetLocation((int)(nDialogWidth - 140)/2, nDialogHeight - 52);
	btnCancel->Click += EventHandler((Object*)this, (EventFun)&CancelDlg::cancel_Click);
	btnCancel->SetText((TCHAR*)strCancel.GetBuf());
	btnCancel->TabIndex = 1;
	this->AddControl(btnCancel);
	this->SetCancelButton(btnCancel);

	labStatus = new Label();
	labStatus->SetName(_T("labStatus"));
	labStatus->SetLocation(10, nDialogHeight - 18);
	labStatus->SetSize(nDialogWidth - 20, 17);
	labStatus->SetText(NULL);
	this->AddControl(labStatus);

	SetCloseButtonEnable(false);
	this->SetSizeBaseOnClient(nDialogWidth, nDialogHeight);
	this->SetName(_T("CancelDlg"));

	SetLang(*CC_APP_GetLang(), cc::Str(CC_CONFIG_LANGSECTION));
	//not show first time!
	SetVisible(false);
}

CancelDlg::~CancelDlg()
{
}
// constructors/destructor
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// private/protected/internal used methods
void CancelDlg::DoSetLang(KeyArr& keyArr, cc::Str sThisHead)
{
	this->SetText(TCHAR_LANG("Lang.S_Cancel_Title"));
	strCancel = TCHAR_LANG("Lang.S_Cancel_CancelText");
	strCanceling = TCHAR_LANG("Lang.S_Cancel_CancelingText");
	strStartMsg = TCHAR_LANG("Lang.S_Cancel_StartMsg");
	strAlreadyRunMsg = TCHAR_LANG("Lang.S_Cancel_AlreadyRunMsg");
	strCancelMsg = TCHAR_LANG("Lang.S_Cancel_CancelMsg");

	Dialog::DoSetLang(keyArr, sThisHead);
}

void CancelDlg::SetCancelText(const TCHAR* txtCancel, const TCHAR* txtCanceling)
{
	if(txtCancel != NULL)
	{
		strCancel = cc::Str(txtCancel);
		if(eStatus != CancelStatus::Canceling)
		{
			btnCancel->SetText((TCHAR*)strCancel.GetBuf());
		}
	}
	if(txtCanceling != NULL)
	{
		strCanceling = cc::Str(txtCanceling);
		if(eStatus == CancelStatus::Canceling)
		{
			btnCancel->SetText((TCHAR*)strCanceling.GetBuf());
		}
	}
}

void CancelDlg::SetShowText(const TCHAR* txtStartMsg, const TCHAR* txtAlreadyRunMsg, const TCHAR* txtCancelMsg)
{
	if(txtStartMsg != NULL)
	{
		strStartMsg = cc::Str(txtStartMsg);
	}
	if(txtAlreadyRunMsg != NULL)
	{
		strAlreadyRunMsg = cc::Str(txtAlreadyRunMsg);
	}
	if(txtCancelMsg != NULL)
	{
		strCancelMsg = cc::Str(txtCancelMsg);
	}
}

bool CancelDlg::Start(bool isShowStartMsg, bool isShowAlreadyRunMsg)
{
	//if is isRuning or isCanceling, cannot restart
	if(eStatus == CancelStatus::Runing || eStatus == CancelStatus::Canceling)
	{
		if(isShowAlreadyRunMsg)
		{
			CC_ASSERT(::IsWindow(m_hWnd));
			::MessageBox(m_hWnd, (const TCHAR*)strAlreadyRunMsg.GetBuf(), TCHAR_LANG("Lang.S_MsgBox_Title_Critical"), MB_OK | MB_ICONSTOP);
		}
		return false;
	}
	//if isCanceled,can restart
	CancelStatus::Type oldStatus = eStatus;
	eStatus = CancelStatus::Runing;
	m_hWndParent = NULL;
	if(_pParent != NULL)
	{
		m_hWndParent = _pParent->m_hWnd;
	}
	else
	{
		Control* actForm = App::GetActiveForm();
		if(actForm != NULL)
		{
			m_hWndParent = actForm->m_hWnd;
		}
	}
	if(isShowStartMsg)
	{
		//this time,m_hWnd is not show
		CC_ASSERT(!::IsWindow(m_hWnd) || !GetVisible());
		if(IDYES != (int)::MessageBox(m_hWndParent == NULL ? m_hWnd : m_hWndParent, (const TCHAR*)strStartMsg.GetBuf(), TCHAR_LANG("Lang.S_MsgBox_Title_Question"), MB_YESNOCANCEL | MB_ICONQUESTION))
		{
			eStatus = oldStatus;
			return false;
		}
	}
	btnCancel->SetText((TCHAR*)strCancel.GetBuf());
	btnCancel->SetEnabled(true);
	btnCancel->Focus();
	//if not show(should be this)
	if(!GetVisible())
	{
		this->SetCenter();
		bEnableParent = false;
		if(m_hWndParent && m_hWndParent != ::GetDesktopWindow() && ::IsWindowEnabled(m_hWndParent))
		{
			::EnableWindow(m_hWndParent, false);
			bEnableParent = true;
		}
		Show();
	}
	return true;
}

bool CancelDlg::IsCancel(bool isShowCancelMsg)
{
	PostMessage(this->m_hWnd, WM_NULL, 0, 0);
	App::DoEvents(m_hWnd);
	//if is canceling or canceled, cannot restart
	if(eStatus == CancelStatus::Canceled)
	{
		return true;
	}
	if(eStatus == CancelStatus::Canceling)
	{
		if(isShowCancelMsg)
		{
			//this time m_hWnd should be valid
			if(IDYES != (int)::MessageBox(m_hWnd, (const TCHAR*)strCancelMsg.GetBuf(), TCHAR_LANG("Lang.S_MsgBox_Title_Question"), MB_YESNOCANCEL | MB_ICONQUESTION | MB_DEFBUTTON3))
			{
				eStatus = CancelStatus::Runing;
				btnCancel->SetText((TCHAR*)strCancel.GetBuf());
				btnCancel->SetEnabled(true);
				btnCancel->Focus();
				return false;
			}
		}
		//end show
		End();
		eStatus = CancelStatus::Canceled;
		return true;
	}
	return false;
}

bool CancelDlg::End()
{
	//if shown
	if(GetVisible())
	{
		//::SetWindowPos(m_hWnd, NULL, 0, 0, 0, 0, SWP_HIDEWINDOW|SWP_NOSIZE|SWP_NOMOVE|SWP_NOACTIVATE|SWP_NOZORDER);
		if(m_hWndParent != NULL)
		{
			//show parent
			if(bEnableParent)
			{
				::EnableWindow(m_hWndParent, true);
			}
			::SetActiveWindow(m_hWndParent);
			m_hWndParent = NULL;
		}
	}
	//hide this
	SetVisible(false);
	eStatus = CancelStatus::Ready;
	return true;
}

CancelStatus::Type CancelDlg::GetStatus()
{
	return eStatus;
}

void CancelDlg::SetMessage(const TCHAR* txt, ImageList* imageList, int nIconID)
{
	if(imageList != NULL && labImg != NULL)
	{
		labImg->SetImage((HBITMAP)NULL, false);
		labImg->SetImage((HICON)NULL, false);
		labImg->SetIcon(imageList, nIconID);
	}
	labMsg->SetText(txt);
	if(GetVisible())
	{
		PostMessage(this->m_hWnd, WM_NULL, 0, 0);
		App::DoEvents(m_hWnd);
	}
}

void CancelDlg::SetFooter(const TCHAR* txt)
{
	labStatus->SetText(txt);
	if(GetVisible())
	{
		PostMessage(this->m_hWnd, WM_NULL, 0, 0);
		App::DoEvents(m_hWnd);
	}
}

void CancelDlg::SetTitle(const TCHAR* txt)
{
	this->SetText(txt);
	if(GetVisible())
	{
		PostMessage(this->m_hWnd, WM_NULL, 0, 0);
		App::DoEvents(m_hWnd);
	}
}

void CancelDlg::cancel_Click(Object* sender, EventArgs* e)
{
	eStatus = CancelStatus::Canceling;
	btnCancel->SetEnabled(false);
	btnCancel->SetText((TCHAR*)strCanceling.GetBuf());
	CancelClick.Invoke((Object*)this, e);
}

#ifdef _DEBUG_
void CancelDlg::test(Control* parent)
{
	CancelDlg* input2 = new CancelDlg();
	input2->SetParent(parent, true);
	input2->SetLocation(2, 100);
	input2->Show();

	CancelDlg* input3 = new CancelDlg();
	input3->SetParent(parent, true);
	input3->SetLocation(202, 200);
	input3->Show();

	CancelDlg* input4 = new CancelDlg();
	input4->SetParent(parent, true);
	input4->SetLocation(102, 300);
	input4->Show();

	CancelDlg* input = new CancelDlg();
	input->SetParent(parent, true);
	input->SetLocation(2, 300);
	DialogResult::Type type = input->ShowDialog();

	input->Dispose();
	input2->Dispose();
	input3->Dispose();
	input4->Dispose();
	delete input;
	delete input2;
	delete input3;
	delete input4;
}
#endif
// private/protected/internal used methods
///////////////////////////////////////////////////////////////////////////////

//===========================================================cc::win::MsgDlg
///////////////////////////////////////////////////////////////////////////////
// constructors/destructor
MsgDlg::MsgDlg(const TCHAR* sMsg, ImageList* imageList, int nIconID, const TCHAR* sTitle,
			   int nDialogWidth, int nIconWidth, int nIconHeight, const TCHAR* sMsgBeLow, DWORD nBtnFlag)
{
	CC_ASSERT(sMsg != NULL);
	_btnOK = NULL;
	_btnCancel = NULL;
	_btnYes = NULL;
	_btnNo = NULL;
	_btnRetry = NULL;
	_btnAbort = NULL;
	_btnIgnore = NULL;

	nLeft = 16;
	nTop = 10;
	LPTSTR pIcon = NULL;
	if(imageList == NULL || nIconID <= 0)
	{
		int nIconType = (nBtnFlag & 0x000000F0L);
		if(nIconType == MB_ICONHAND)
		{
			pIcon = IDI_HAND;
		}
		else if(nIconType == MB_ICONQUESTION)
		{
			pIcon = IDI_QUESTION;
		}
		else if(nIconType == MB_ICONEXCLAMATION)
		{
			pIcon = IDI_EXCLAMATION;
		}
		else if(nIconType == MB_ICONASTERISK)
		{
			pIcon = IDI_ASTERISK;
		}
	}
	if((imageList != NULL && nIconID > 0) || pIcon != NULL)
	{
		nLeft = 10 + nIconWidth + 6;
		PictureBox* labImg = new PictureBox();
		labImg->SetName(_T("labImg"));
		labImg->SetLocation(10, 10);
		labImg->SetSize(nIconWidth, nIconHeight);
		if(imageList != NULL && nIconID > 0)
		{
			labImg->SetIcon(imageList, nIconID);
		}
		else
		{
			HICON hIcon = ::LoadIcon(NULL, MAKEINTRESOURCE(pIcon));
			labImg->SetImage(hIcon, true);
		}
		this->AddControl(labImg);
	}

	if(CC_BIT_HAS(nBtnFlag, CC_RICHTEXTBOX))
	{
		RichTextBox* labBeLow2 = new RichTextBox();
		labBeLow2->SetReadOnly(true);
		labBeLow2->SetMultiline(true);
		labBeLow2->SetStyle(StyleType::ExStyle, WS_EX_CLIENTEDGE, WS_EX_TRANSPARENT);
		labMsg = labBeLow2;
	}
	else
	{
		labMsg = new Label();
	}
	labMsg->SetName(_T("labMsg"));
	labMsg->SetText(sMsg);
	labMsg->SetLocation(nLeft, nTop);

	//here labMsg->m_hWnd is NULL
	HDC hDC = ::GetDC(labMsg->m_hWnd);
	HFONT hOldFont = (HFONT)::SelectObject(hDC, labMsg->GetFont());
	RECT rc = {0, 0, nDialogWidth - nLeft - 20, 0};
	::DrawText(hDC, sMsg, -1, &rc, DT_CALCRECT|DT_WORDBREAK);
	if(CC_BIT_HAS(nBtnFlag, CC_RICHTEXTBOX) && rc.bottom > 600) {
		rc.bottom = 600;
	}
	labMsg->SetSize(nDialogWidth - nLeft - 20, rc.bottom);
	this->AddControl(labMsg);
	if(rc.bottom < nIconHeight)
	{
		labMsg->SetLocation(nLeft, nTop + (nIconHeight - rc.bottom)/2);
	}
	nTop += CC_MAX(rc.bottom, nIconHeight) + 2;

	labBeLow = NULL;
	if(sMsgBeLow != NULL)
	{
		labBeLow = new Label();
		labBeLow->SetName(_T("labBeLow"));
		labBeLow->SetText(sMsgBeLow);
		nTop += 2;
		labBeLow->SetLocation(16, nTop);
		::SetRect(&rc, 0, 0, nDialogWidth - 16 - 20, 0);
		::DrawText(hDC, sMsgBeLow, -1, &rc, DT_CALCRECT|DT_WORDBREAK);
		labBeLow->SetSize(nDialogWidth - 16 - 20, rc.bottom);
		this->AddControl(labBeLow);
		nTop += rc.bottom;
	}
	::SelectObject(hDC, hOldFont);
	::ReleaseDC(labMsg->m_hWnd, hDC);

	nTop += 4;
	if(CC_BIT_HAS(nBtnFlag, CC_BOTTOM_LINE))
	{
		nTop += 15;
		Label* labLine = new Label();
		labLine->SetName(_T("labLine"));
		labLine->SetText(_T(""));
		labLine->SetLocation(5, nTop);
		labLine->SetSize(nDialogWidth - 10, 2);
		//labLine->SetStyle(StyleType::Style, 0xFFFFFFFF, WS_CHILD | WS_VISIBLE | SS_ETCHEDHORZ);
		labLine->SetStyle(StyleType::Outer, 0, OuterStyle::Break);
		labLine->SetBackground(RGB(255, 255, 255));
		labLine->Anchor = (AnchorStyles::Type)(AnchorStyles::Top | AnchorStyles::Left | AnchorStyles::Right);
		this->AddControl(labLine);
	}

	nTop += 7;
	int nBtnType = (nBtnFlag & 0x0000000FL);
	if(nBtnType == MB_OKCANCEL)
	{
		_btnOK = AddButton(_T("btnOK"), _T("Lang.S_Dialog_OK"), _T("OK"));
		_btnCancel = AddButton(_T("btnCancel"), _T("Lang.S_Dialog_Cancel"), _T("Cancel"));
		this->SetCancelButton(_btnCancel);
		this->SetOKButton(_btnOK);
		if(CC_BIT_HAS(nBtnFlag, CC_RIGHT_BUTTON))
		{
			_btnCancel->SetLocation(nDialogWidth - 70 - 10, nTop);
			_btnOK->SetLocation(nDialogWidth - 70 - 10 - 70 - 5, nTop);
		}
		else
		{
			_btnOK->SetLocation((int)(nDialogWidth/2) - 70 - 5, nTop);
			_btnCancel->SetLocation((int)(nDialogWidth/2) + 5, nTop);
		}
	}
	else if(nBtnType == MB_ABORTRETRYIGNORE)
	{
		_btnAbort = AddButton(_T("btnAbort"), _T("Lang.S_Dialog_Abort"), _T("Abort"));
		_btnRetry = AddButton(_T("btnRetry"), _T("Lang.S_Dialog_Retry"), _T("Retry"));
		_btnIgnore = AddButton(_T("btnIgnore"), _T("Lang.S_Dialog_Ignore"), _T("Ignore"));
		if(CC_BIT_HAS(nBtnFlag, CC_RIGHT_BUTTON))
		{
			_btnIgnore->SetLocation(nDialogWidth - 70 - 10, nTop);
			_btnRetry->SetLocation(nDialogWidth - 70 - 10 - 70 - 5, nTop);
			_btnAbort->SetLocation(nDialogWidth - 70 - 10 - 70 - 5 - 70 - 5, nTop);
		}
		else
		{
			_btnAbort->SetLocation((int)(nDialogWidth/2) - 70/2 -70 - 10, nTop);
			_btnRetry->SetLocation((int)(nDialogWidth/2) - 70/2, nTop);
			_btnIgnore->SetLocation((int)(nDialogWidth/2) - 70/2 + 70 + 10, nTop);
		}
	}
	else if(nBtnType == MB_YESNOCANCEL)
	{
		_btnYes = AddButton(_T("btnYes"), _T("Lang.S_Dialog_Yes"), _T("Yes"));
		_btnNo = AddButton(_T("btnNo"), _T("Lang.S_Dialog_No"), _T("No"));
		_btnCancel = AddButton(_T("btnCancel"), _T("Lang.S_Dialog_Cancel"), _T("Cancel"));
		this->SetCancelButton(_btnCancel);
		if(CC_BIT_HAS(nBtnFlag, CC_RIGHT_BUTTON))
		{
			_btnCancel->SetLocation(nDialogWidth - 70 - 10, nTop);
			_btnNo->SetLocation(nDialogWidth - 70 - 10 - 70 - 5, nTop);
			_btnYes->SetLocation(nDialogWidth - 70 - 10 - 70 - 5 - 70 - 5, nTop);
		}
		else
		{
			_btnYes->SetLocation((int)(nDialogWidth/2) - 70/2 -70 - 10, nTop);
			_btnNo->SetLocation((int)(nDialogWidth/2) - 70/2, nTop);
			_btnCancel->SetLocation((int)(nDialogWidth/2) - 70/2 + 70 + 10, nTop);
		}
	}
	else if(nBtnType == MB_YESNO)
	{
		_btnYes = AddButton(_T("btnYes"), _T("Lang.S_Dialog_Yes"), _T("Yes"));
		_btnNo = AddButton(_T("btnNo"), _T("Lang.S_Dialog_No"), _T("No"));
		_btnYes->SetLocation((int)(nDialogWidth/2) - 70 - 5, nTop);
		_btnNo->SetLocation((int)(nDialogWidth/2) + 5, nTop);
		if(CC_BIT_HAS(nBtnFlag, CC_RIGHT_BUTTON))
		{
			_btnNo->SetLocation(nDialogWidth - 70 - 10, nTop);
			_btnYes->SetLocation(nDialogWidth - 70 - 10 - 70 - 5, nTop);
		}
		else
		{
			_btnYes->SetLocation((int)(nDialogWidth/2) - 70 - 5, nTop);
			_btnNo->SetLocation((int)(nDialogWidth/2) + 5, nTop);
		}
	}
	else if(nBtnType == MB_RETRYCANCEL)
	{
		_btnRetry = AddButton(_T("btnRetry"), _T("Lang.S_Dialog_Retry"), _T("Retry"));
		_btnCancel = AddButton(_T("btnCancel"), _T("Lang.S_Dialog_Cancel"), _T("Cancel"));
		this->SetCancelButton(_btnCancel);
		if(CC_BIT_HAS(nBtnFlag, CC_RIGHT_BUTTON))
		{
			_btnCancel->SetLocation(nDialogWidth - 70 - 10, nTop);
			_btnRetry->SetLocation(nDialogWidth - 70 - 10 - 70 - 5, nTop);
		}
		else
		{
			_btnRetry->SetLocation((int)(nDialogWidth/2) - 70 - 5, nTop);
			_btnCancel->SetLocation((int)(nDialogWidth/2) + 5, nTop);
		}
	}
	else if(CC_BIT_HAS(nBtnFlag, CC_CUSTOMBTN))
	{
	}
	else
	{
		//MB_OK
		_btnOK = AddButton(_T("btnOK"), _T("Lang.S_Dialog_OK"), _T("OK"));
		this->SetOKButton(_btnOK);
		if(CC_BIT_HAS(nBtnFlag, CC_RIGHT_BUTTON))
		{
			_btnOK->SetLocation(nDialogWidth - 70 - 10, nTop);
		}
		else
		{
			_btnOK->SetLocation((int)(nDialogWidth - 70)/2, nTop);
		}
	}

	if(sTitle == NULL)
	{
		this->SetText((TCHAR*)CC_APP_GetLang()->Get(_T("Lang.S_Dialog_OK"), cc::Str(_T("Message"))).GetBuf());
	}
	else
	{
		this->SetText(sTitle);
	}
	//cannot nTop+=17 here,for TipsDlg use of nTop!
	this->SetSizeBaseOnClient(nDialogWidth, nTop + 17 + 10);
	this->SetName(_T("MsgDlg"));

	SetLang(*CC_APP_GetLang(), cc::Str(CC_CONFIG_LANGSECTION));
}

MsgDlg::~MsgDlg()
{
}
// constructors/destructor
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// private/protected/internal used methods
Button* MsgDlg::AddButton(const TCHAR* name, const TCHAR* txtkey, const TCHAR* txtdef, int nWidth)
{
	if(nWidth <= 0)
	{
		nWidth = 70;
	}
	Button* btn = new Button();
	btn->SetName(name);
	btn->SetSize(nWidth, 19);
	//btn->SetLocation((int)(nDialogWidth - 70)/2, nTop);
	btn->Click += EventHandler((Object*)this, (EventFun)&MsgDlg::Control_Click);
	btn->SetText(CC_APP_GetLang()->Get(txtkey, cc::Str(txtdef)));
	this->AddControl(btn);
	return btn;
}

void MsgDlg::Control_Click(Object* sender, EventArgs* e)
{
	DialogResult::Type type;
	if(sender == _btnOK)
	{
		type = DialogResult::OK;
	}
	else if(sender == _btnCancel)
	{
		type = DialogResult::Cancel;
	}
	else if(sender == _btnYes)
	{
		type = DialogResult::Yes;
	}
	else if(sender == _btnNo)
	{
		type = DialogResult::No;
	}
	else if(sender == _btnRetry)
	{
		type = DialogResult::Retry;
	}
	else if(sender == _btnAbort)
	{
		type = DialogResult::Abort;
	}
	else if(sender == _btnIgnore)
	{
		type = DialogResult::Ignore;
	}
	EndDialog(type);
}

#ifdef _DEBUG_
void MsgDlg::test(Control* parent)
{
	MsgDlg* input2 = new MsgDlg(_T("meg4:"), NULL, 0, _T("Message..."), 160);
	input2->SetParent(parent, true);
	input2->SetLocation(2, 100);
	input2->Show();

	MsgDlg* input3 = new MsgDlg(_T("megasdfasdf\r\nasdfasdf4:"));
	input3->SetParent(parent, true);
	input3->SetLocation(202, 200);
	input3->Show();

	MsgDlg* input4 = new MsgDlg(_T("megasdf f\r\nasdfas\r\ndfasdf4:"));
	input4->SetParent(parent, true);
	input4->SetLocation(102, 300);
	input4->Show();

	MsgDlg* input = new MsgDlg(_T("about this form\r\nsadfasd asdfas dfasdf fadsfasdfasdf msg2\r\nm asdf asdf fa dsfdsfsg3\r\nm asdfa sdfdf  fdasfasdfasdfasdfsg4:"), NULL, 0, _T("About"), 350);
	input->SetParent(parent, true);
	input->SetLocation(2, 300);
	DialogResult::Type type = input->ShowDialog();

	input->Dispose();
	input2->Dispose();
	input3->Dispose();
	input4->Dispose();
	delete input;
	delete input2;
	delete input3;
	delete input4;
}
#endif
// private/protected/internal used methods
///////////////////////////////////////////////////////////////////////////////

//===========================================================cc::win::AboutDlg
///////////////////////////////////////////////////////////////////////////////
// constructors/destructor
AboutDlg::AboutDlg(const TCHAR* sMsg, ImageList* imageList, int nIconID, const TCHAR* sTitle,
				   int nDialogWidth, int nIconWidth, int nIconHeight, const TCHAR* sMsgBeLow,
				   bool isLink1, bool isLink2)
	: MsgDlg(sMsg, imageList, nIconID, sTitle, nDialogWidth, nIconWidth, nIconHeight, sMsgBeLow, MB_OK)
{
	if(sTitle == NULL)
	{
		sTitle = (const TCHAR*)CC_APP_GetLang()->Get(_T("Lang.S_AboutDlg_Title"), _T("About"));
	}
	this->SetText(sTitle);
	this->SetName(_T("AboutDlg"));

	if(isLink1)
	{
		nTop -= 5;
		LabelLink* link = new LabelLink();
		link->SetName(_T("link"));
		link->SetText((const TCHAR*)CC_APP_GetLang()->Get(_T("Lang.U_Link")));
		link->SetLocation(16, nTop);
		link->SetSize(100, 17);
		link->SetOuterStyle(OuterStyle::AutoSize, true);
		link->SetURL((const TCHAR*)CC_APP_GetLang()->Get(_T("Lang.U_LinkURL")));
		this->AddControl(link);
		if(isLink2)
		{
			nTop += 15;
			LabelLink* link2 = new LabelLink();
			link2->SetName(_T("link2"));
			link2->SetText((const TCHAR*)CC_APP_GetLang()->Get(_T("Lang.U_Link2")));
			link2->SetLocation(16, nTop);
			link2->SetSize(100, 17);
			link2->SetOuterStyle(OuterStyle::AutoSize, true);
			link2->SetURL((const TCHAR*)CC_APP_GetLang()->Get(_T("Lang.U_Link2URL")));
			this->AddControl(link2);
		}
		nTop += 22;
		if(_btnOK != NULL)
		{
			_btnOK->SetLocation((int)(nDialogWidth - 70)/2, nTop);
		}
	}
	this->SetSizeBaseOnClient(nDialogWidth, nTop + 22 + 10);
	this->SetName(_T("AboutDlg"));

	SetLang(*CC_APP_GetLang(), cc::Str(CC_CONFIG_LANGSECTION));
}

AboutDlg::~AboutDlg()
{
}

#ifdef _DEBUG_
void AboutDlg::test(Control* parent)
{
	AboutDlg* input2 = new AboutDlg(_T("meg4:"), NULL, 0, _T("Message..."), 160);
	input2->SetParent(parent, true);
	input2->SetLocation(2, 100);
	input2->Show();

	AboutDlg* input3 = new AboutDlg(_T("megasdfasdf\r\nasdfasdf4:"));
	input3->SetParent(parent, true);
	input3->SetLocation(202, 200);
	input3->Show();

	AboutDlg* input4 = new AboutDlg(_T("megasdf f\r\nasdfas\r\ndfasdf4:"));
	input4->SetParent(parent, true);
	input4->SetLocation(102, 300);
	input4->Show();

	AboutDlg* input = new AboutDlg(_T("about this form\r\nsadfasd asdfas dfasdf fadsfasdfasdf msg2\r\nm asdf asdf fa dsfdsfsg3\r\nm asdfa sdfdf  fdasfasdfasdfasdfsg4:"), NULL, 0, _T("About"), 350);
	input->SetParent(parent, true);
	input->SetLocation(2, 300);
	DialogResult::Type type = input->ShowDialog();

	input->Dispose();
	input2->Dispose();
	input3->Dispose();
	input4->Dispose();
	delete input;
	delete input2;
	delete input3;
	delete input4;
}
#endif
// constructors/destructor
///////////////////////////////////////////////////////////////////////////////

//===========================================================cc::win::TipsDlg
///////////////////////////////////////////////////////////////////////////////
// constructors/destructor
TipsDlg::TipsDlg(int nTipsDlgId, const TCHAR* sMsg, ImageList* imageList, int nIconID, const TCHAR* sTitle, int nDialogWidth,
				 int nIconWidth, int nIconHeight, const TCHAR* sMsgBeLow, const TCHAR* sMsgCheckBox, DWORD nBtnFlag)
	: MsgDlg(sMsg, imageList, nIconID, sTitle, nDialogWidth, nIconWidth, nIconHeight, sMsgBeLow, nBtnFlag|CC_BOTTOM_LINE|CC_RIGHT_BUTTON)
{
	this->_nTipsDlgId = nTipsDlgId;
	if(sTitle == NULL)
	{
		sTitle = (const TCHAR*)CC_APP_GetLang()->Get(_T("Lang.S_MsgBox_Title_Info"), _T("Message"));
	}
	this->SetText(sTitle);
	this->SetName(_T("TipsDlg"));

	//if(sMsgCheckBox != NULL)
	//{
		//nTop += 20;
		chkNotShowAgain = new CheckBox();
		chkNotShowAgain->SetName(_T("chkNotShowAgain"));
		if(sMsgCheckBox == NULL)
		{
			chkNotShowAgain->SetText(TCHAR_LANGDEF("Lang.S_NotShowAgain", _T("Do not show this message aga&in")));
		}
		else
		{
			chkNotShowAgain->SetText(sMsgCheckBox);
		}
		chkNotShowAgain->SetLocation(16, nTop);
		chkNotShowAgain->SetSize(100, 17);
		//chkNotShowAgain->SetChecked(CC_APP_GetIni()->GetBool(_T("Main.VRemove"), true));
		chkNotShowAgain->SetStyle(StyleType::Outer, 0, OuterStyle::AutoSize);
		chkNotShowAgain->Click += EventHandler((Object*)this, (EventFun)&TipsDlg::chkNotShowAgain_Click);
		this->AddControl(chkNotShowAgain);
	//}
	//nTop += 17;
	//this->SetSizeBaseOnClient(nDialogWidth, nTop + 10);

	SetLang(*CC_APP_GetLang(), cc::Str(CC_CONFIG_LANGSECTION));
}

TipsDlg::~TipsDlg()
{
}

void TipsDlg::ShowAllAgain()
{
	return CC_APP_GetIni()->Clear(_T("Main.TipsDlg."));
}

bool TipsDlg::GetIsNotShowAgain(int nTipsDlgId)
{
	cc::Str sKey;
	sKey.Format(_T("Main.TipsDlg.%d"), nTipsDlgId);
	return CC_APP_GetIni()->GetBool(sKey, false);
}

void TipsDlg::chkNotShowAgain_Click(Object* sender, EventArgs* e)
{
	bool chk = chkNotShowAgain->GetChecked();
	cc::Str sKey;
	sKey.Format(_T("Main.TipsDlg.%d"), _nTipsDlgId);
	CC_APP_GetIni()->SetBool(sKey, chk);
}

#ifdef _DEBUG_
void TipsDlg::test(Control* parent)
{
	TipsDlg* input2 = new TipsDlg(1, _T("about this form\r\nsadfasd asdfas dfasdf fadsfasdfasdfg4:"), NULL, 0, _T("About"), 350, 0, 0, _T("dasd asdf ad\r\nasdfasdfasdsd f"), _T("dsfasdfasdf"), MB_OKCANCEL);
	input2->SetParent(parent, true);
	input2->SetLocation(2, 500);
	input2->Show();

	TipsDlg* input3 = new TipsDlg(2, _T("about this form\r\nsadfasd asdfas dfasdf fadsfasdfasdfg4:"), NULL, 0, _T("About"), 350, 0, 0, _T("dasd asdf ad\r\nasdfasdfasdsd f"), NULL, MB_ABORTRETRYIGNORE);
	input3->SetParent(parent, true);
	input3->SetLocation(400, 100);
	input3->Show();

	TipsDlg* input4 = new TipsDlg(3, _T("megasdf f\r\nasdfas\r\ndfasdf4:"));
	input4->SetParent(parent, true);
	input4->SetLocation(400, 500);
	input4->Show();

	TipsDlg* input = new TipsDlg(4, _T("about this form\r\nsadfasd asdfas dfasdf fadsfasdfasdf msg2\r\nm asdf asdf fa dsfdsfsg3\r\nm asdfa sdfdf  fdasfasdfasdfasdfsg4:"), NULL, 0, _T("About"), 350, 0, 0, NULL, _T("dsfasdfasdf"), MB_YESNOCANCEL);
	input->SetParent(parent, true);
	input->SetLocation(2, 100);
	DialogResult::Type type = input->ShowDialog();

	input->Dispose();
	input2->Dispose();
	input3->Dispose();
	input4->Dispose();
	delete input;
	delete input2;
	delete input3;
	delete input4;
}
#endif
// constructors/destructor
///////////////////////////////////////////////////////////////////////////////

//===========================================================cc::win::InputDlg
///////////////////////////////////////////////////////////////////////////////
// constructors/destructor
InputDlg::InputDlg(const TCHAR* sTxt, bool isMultiline, const TCHAR* sMsg, const TCHAR* sTitle,
	bool isNotBlank, int nWinWidth, int nMultilineHeight, const TCHAR* sConfirmMsg)
{
	//_StyleArray[(int)StyleType::Style] = WS_CAPTION|WS_VISIBLE|DS_MODALFRAME|WS_POPUP;
	//SetStyle(StyleType::Inner, InnerStyle::CloseButtonDisable, true);
	this->sConfirmMsg = sConfirmMsg;
	int nTop = 3;
	this->nIsNotBlank = isNotBlank ? 1 : 0;
	labMsg = new Label();
	labMsg->SetName(_T("labMsg"));
	labMsg->SetText(sMsg);
	labMsg->SetLocation(3, nTop);
	labMsg->SetSize(600 - 6, 26);
	SIZE sz = labMsg->BestSize(true);
	labMsg->Anchor = (AnchorStyles::Type)(AnchorStyles::Top | AnchorStyles::Left | AnchorStyles::Right);
	this->AddControl(labMsg);

	nTop += sz.cy + 5;
	editText = new TextBox();
	editText->SetName(_T("editText"));
	editText->SetText(sTxt);
	editText->SetLocation(3, nTop);
	if(isMultiline)
	{
		if(nMultilineHeight <= 0)
		{
			nMultilineHeight = 91;
		}
		editText->SetMultiline(true);
		editText->SetSize(600 - 6, nMultilineHeight);
		editText->SetMinSize(0, 17);
		editText->Anchor = (AnchorStyles::Type)(AnchorStyles::Top | AnchorStyles::Left | AnchorStyles::Right | AnchorStyles::Bottom);
		nTop = 45 - 17 + nMultilineHeight;
	}
	else
	{
		editText->SetSize(600 - 6, 17);
		editText->Anchor = (AnchorStyles::Type)(AnchorStyles::Top | AnchorStyles::Left | AnchorStyles::Right);
		nTop = 45;
	}
	this->AddControl(editText);

	nTop += 15;
	btnOK = new Button();
	btnOK->SetName(_T("btnOK"));
	btnOK->SetLocation(3, nTop);
	btnOK->SetSize(70, 19);
	btnOK->Click += EventHandler((Object*)this, (EventFun)&InputDlg::Control_Click);
	btnOK->SetText((TCHAR*)CC_APP_GetLang()->Get(_T("Lang.S_Dialog_OK")).GetBuf());
	btnOK->Anchor = (AnchorStyles::Type)(AnchorStyles::Left | AnchorStyles::Bottom);
	btnOK->MinPosition.y = 45;
	this->AddControl(btnOK);
	this->SetOKButton(btnOK);

	btnCancel = new Button();
	btnCancel->SetName(_T("btnCancel"));
	btnCancel->SetLocation(75, nTop);
	btnCancel->SetSize(70, 19);
	btnCancel->Click += EventHandler((Object*)this, (EventFun)&InputDlg::Control_Click);
	btnCancel->SetText((TCHAR*)CC_APP_GetLang()->Get(_T("Lang.S_Dialog_Cancel")).GetBuf());
	btnCancel->Anchor = (AnchorStyles::Type)(AnchorStyles::Left | AnchorStyles::Bottom);
	btnCancel->MinPosition.y = 45;
	this->AddControl(btnCancel);
	this->SetCancelButton(btnCancel);

	nTop += 25;
	if(sTitle == NULL)
	{
		this->SetText((TCHAR*)CC_APP_GetLang()->Get(_T("Lang.S_InputDlg_Title"), _T("Input Dialog")).GetBuf());
	}
	else
	{
		this->SetText(sTitle);
	}
	this->SetName(_T("InputDlg"));
	this->SetStyle(StyleType::Style, 0, WS_THICKFRAME);
	this->SetClientSize(600, nTop);
	this->SetSize(nWinWidth, nTop + 19 + 10);
	this->SetMinSize(0, 80 + 19 + 10);
	if(!isMultiline)
	{
		this->SetMinSize(0, nTop + 19 + 10);
		this->SetMaxSize(0, nTop + 19 + 10);
	}

	SetLang(*CC_APP_GetLang(), cc::Str(CC_CONFIG_LANGSECTION));
}
// constructors/destructor
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// private/protected/internal used methods
void InputDlg::SetInputText(const TCHAR* sTxt, const TCHAR* sMsg)
{
	editText->SetText(sTxt);
	if(sMsg != NULL)
	{
		labMsg->SetText(sMsg);
	}
	sInput = cc::Str(sTxt);
}

cc::Str InputDlg::GetInputText()
{
	return sInput;
}

void InputDlg::Control_Click(Object* sender, EventArgs* e)
{
	if(sender == btnOK)
	{
		cc::Str sVal = cc::Str(editText->GetText()).Trim();
		if(nIsNotBlank == 1)
		{
			if(editText->GetTextLen() != sVal.GetLength())
			{
				editText->SetText((TCHAR*)sVal.GetBuf());
			}
			if(sVal.GetLength() < 1)
			{
				::MessageBox(m_hWnd, TCHAR_LANG("Lang.S_Dialog_Blank"), TCHAR_LANG("Lang.S_MsgBox_Title_Critical"), MB_OK | MB_ICONSTOP);
				editText->Focus();
				return;
			}
			if(this->sConfirmMsg.GetLength() > 0)
			{
				if(IDYES != (int)::MessageBox(m_hWnd, (const TCHAR*)this->sConfirmMsg.GetBuf(), TCHAR_LANG("Lang.S_MsgBox_Title_Question"), MB_YESNOCANCEL | MB_ICONQUESTION))
				{
					editText->Focus();
					return;
				}
			}
		}
		sInput = sVal;
		EndDialog(DialogResult::OK);
	}
	else if(sender == btnCancel)
	{
		EndDialog(DialogResult::Cancel);
	}
}
// private/protected/internal used methods
#ifdef _DEBUG_
void InputDlg::test(Control* parent)
{
	InputDlg* input2 = new InputDlg(_T("text1t3"), false, _T("meg4:"));
	input2->SetParent(parent, true);
	input2->SetLocation(2, 100);
	input2->Show();

	InputDlg* input3 = new InputDlg(_T("text1t3"), false, _T("megasdfasdf\r\nasdfasdf4:"));
	input3->SetParent(parent, true);
	input3->SetLocation(202, 200);
	input3->Show();

	InputDlg* input4 = new InputDlg(_T("text1t3"), false, _T("megasdf f\r\nasdfas\r\ndfasdf4:"));
	input4->SetParent(parent, true);
	input4->SetLocation(102, 300);
	input4->Show();

	InputDlg* input = new InputDlg(_T("text1\r\ntext2\r\ntext3"), true, _T("meg1\r\nmsg2\r\nmsg3\r\nmsg4:"));
	input->SetParent(parent, true);
	input->SetLocation(2, 300);
	DialogResult::Type type = input->ShowDialog();

	input->Dispose();
	delete input;
	input2->Dispose();
	input3->Dispose();
	input4->Dispose();
	delete input2;
	delete input3;
	delete input4;
}
#endif
///////////////////////////////////////////////////////////////////////////////

//===========================================================cc::win::ChangePWDDlg
///////////////////////////////////////////////////////////////////////////////
// constructors/destructor
ChangePWDDlg::ChangePWDDlg(const TCHAR* sPWD, bool isPWDMayEmpty, const TCHAR* sTitle)
{
	_tcPWD = NULL;
	CC_StrCopy(&_tcPWD, sPWD);
	_isPWDMayEmpty = isPWDMayEmpty;
	label1 = new Label();
	label1->SetName(_T("label1"));
	label1->SetText(CC_APP_GetLang()->Get("Lang.S_ChangePWDDlg_OldPWD"));
	label1->SetLocation(6, 5);
	label1->SetSize(90, 17);

	label2 = new Label();
	label2->SetName(_T("label2"));
	label2->SetText(CC_APP_GetLang()->Get("Lang.S_ChangePWDDlg_NewPWD1"));
	label2->SetLocation(6, 25);
	label2->SetSize(90, 17);

	label3 = new Label();
	label3->SetName(_T("label3"));
	label3->SetText(CC_APP_GetLang()->Get("Lang.S_ChangePWDDlg_NewPWD2"));
	label3->SetLocation(6, 45);
	label3->SetSize(90, 17);

	_editText1 = new TextBox();
	_editText1->SetName(_T("editText1"));
	_editText1->SetStyle(StyleType::Style, 0, ES_PASSWORD);
	_editText1->SetText(_T(""));
	_editText1->SetLocation(100, 4);
	_editText1->SetSize(120, 18);

	_editText2 = new TextBox();
	_editText2->SetName(_T("editText2"));
	_editText2->SetStyle(StyleType::Style, 0, ES_PASSWORD);
	_editText2->SetText(_T(""));
	_editText2->SetLocation(100, 24);
	_editText2->SetSize(120, 18);

	_editText3 = new TextBox();
	_editText3->SetName(_T("editText3"));
	_editText3->SetStyle(StyleType::Style, 0, ES_PASSWORD);
	_editText3->SetText(_T(""));
	_editText3->SetLocation(100, 44);
	_editText3->SetSize(120, 18);

	_btnOK = new Button();
	_btnOK->SetName(_T("btnOK"));
	_btnOK->SetLocation(37, 65);
	_btnOK->SetSize(70, 19);
	_btnOK->Click += EventHandler((Object*)this, (EventFun)&ChangePWDDlg::ok_Click);
	_btnOK->SetText((TCHAR*)CC_APP_GetLang()->Get(_T("Lang.S_Dialog_OK")).GetBuf());
	this->SetOKButton(_btnOK);

	_btnCancel = new Button();
	_btnCancel->SetName(_T("btnCancel"));
	_btnCancel->SetLocation(109, 65);
	_btnCancel->SetSize(70, 19);
	_btnCancel->Click += EventHandler((Object*)this, (EventFun)&ChangePWDDlg::cancel_Click);
	_btnCancel->SetText((TCHAR*)CC_APP_GetLang()->Get(_T("Lang.S_Dialog_Cancel")).GetBuf());
	this->SetCancelButton(_btnCancel);

	this->AddControl(label1);
	this->AddControl(label2);
	this->AddControl(label3);
	this->AddControl(_editText1);
	this->AddControl(_editText2);
	this->AddControl(_editText3);
	this->AddControl(_btnOK);
	this->AddControl(_btnCancel);
	if(sTitle == NULL)
	{
		this->SetText((TCHAR*)CC_APP_GetLang()->Get(_T("Lang.S_ChangePWDDlg_Title")).GetBuf());
	}
	else
	{
		this->SetText(sTitle);
	}
	this->SetSizeBaseOnClient(232, 65 + 19 + 5);
	this->SetName(_T("ChangePWDDlg"));

	SetLang(*CC_APP_GetLang(), cc::Str(CC_CONFIG_LANGSECTION));
}

ChangePWDDlg::~ChangePWDDlg()
{
}
// constructors/destructor
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// private/protected/internal used methods
void ChangePWDDlg::ok_Click(Object* sender, EventArgs* e)
{
	if(OKClick.IsEmpty())
	{
		cc::Str p0 = cc::Str(_tcPWD).Trim();
		cc::Str p1 = cc::Str(_editText1->GetText()).Trim();
		cc::Str p2 = cc::Str(_editText2->GetText()).Trim();
		cc::Str p3 = cc::Str(_editText3->GetText()).Trim();
		if(p2 != p3)
		{
			::MessageBox(m_hWnd, CC_APP_GetLang()->GetFormat(_T("Lang.S_ChangePWDDlg_NewPWD_NotSame")), CC_APP_GetLang()->Get(_T("Lang.S_MsgBox_Title_Critical")), MB_OK | MB_ICONSTOP);
			return;
		}
		if(!_isPWDMayEmpty && p2.GetLength() < 1)
		{
			::MessageBox(m_hWnd, CC_APP_GetLang()->GetFormat(_T("Lang.S_ChangePWDDlg_NewPWD_IsEmpty")), CC_APP_GetLang()->Get(_T("Lang.S_MsgBox_Title_Critical")), MB_OK | MB_ICONSTOP);
			return;
		}
		if(p0 == p1 || (p0.GetLength() == 0 && p1.GetLength() == 0))
		{
		}
		else
		{
			::MessageBox(m_hWnd, CC_APP_GetLang()->GetFormat(_T("Lang.S_ChangePWDDlg_OldPWD_NotSame")), CC_APP_GetLang()->Get(_T("Lang.S_MsgBox_Title_Critical")), MB_OK | MB_ICONSTOP);
			return;
		}
		EndDialog(DialogResult::OK);
	}
	else
	{
		e->Handled = true;
		OKClick.Invoke((Object*)this, e);
		if(e->Handled)
		{
			EndDialog(DialogResult::OK);
		}
	}
}

void ChangePWDDlg::cancel_Click(Object* sender, EventArgs* e)
{
	e->Handled = true;
	CancelClick.Invoke((Object*)this, e);
	if(e->Handled)
	{
		EndDialog(DialogResult::Cancel);
	}
}
#ifdef _DEBUG_
void ChangePWDDlg::test(Control* parent)
{
}
#endif
// private/protected/internal used methods
///////////////////////////////////////////////////////////////////////////////

//===========================================================cc::win::OptionPage
///////////////////////////////////////////////////////////////////////////////
// constructors/destructor
OptionPage::OptionPage() : TabPage()
{
	_StyleArray[(int)StyleType::Style] = WS_CHILD | WS_VISIBLE | WS_TABSTOP;
	::SetRect(&_Rect, 0, 0, 200, 200);
	::CopyRect(&_OrgRect, &_Rect);
	//not used anyaway
	//::SetRect(&_Margin, 4, 2, 4, 1);
	SetText(_T("OptionPage"));

	_OptionStyle = OptionStyles::ShowOK | OptionStyles::ShowCancel | OptionStyles::ShowApply | OptionStyles::ShowDefault;
	_bIsDirty = 0;
	SetName(_T("OptionPage"));

	SetLang(*CC_APP_GetLang(), cc::Str(CC_CONFIG_LANGSECTION));
}

OptionPage::~OptionPage()
{
}

bool OptionPage::IsOptionStyle(OptionStyles::Type optionStyle) const
{
	return ((_OptionStyle & optionStyle) != 0);
}

//if update,then use UpdateFlag::UpdateOuterStyle to notice update
void OptionPage::SetOptionStyle(OptionStyles::Type optionStyle, bool value, bool needUpdate)
{
	if(value)
	{
		_OptionStyle |= optionStyle;
	}
	else
	{
		_OptionStyle &= ~optionStyle;
	}
	if(needUpdate)
	{
		_nUpdateFlag |= UpdateFlag::UpdateOuterStyle;
		Update();
	}
}

bool OptionPage::GetIsDirty() const
{
	return (_bIsDirty == 1);
}

void OptionPage::SetDirty(bool value)
{
	_bIsDirty = (value ? 1 : 0);
}
// constructors/destructor
///////////////////////////////////////////////////////////////////////////////

//===========================================================cc::win::OptionCtl
///////////////////////////////////////////////////////////////////////////////
// constructors/destructor
OptionCtl::OptionCtl() : Container()
{
	_StyleArray[(int)StyleType::Style] = WS_CHILD | WS_VISIBLE | WS_TABSTOP;
	::SetRect(&_Rect, 0, 0, 400, 400);
	::CopyRect(&_OrgRect, &_Rect);
	//not used anyaway
	//::SetRect(&_Margin, 4, 2, 4, 1);
	SetText(_T("OptionCtl"));

#ifdef _DEBUG_
	SetName(_T("OptionCtl"));
#endif

	activePage = NULL;
	_nSelectedIndex = 0;
	_nLastSelectedIndex = 0;

	tree = new TreeView();
	tree->SetName(_T("tree"));
	tree->SetLocation(1, 0);
	tree->SetSize(196, 340);
	tree->TabIndex = 1;
	tree->Anchor = (AnchorStyles::Type)(AnchorStyles::Top | AnchorStyles::Left | AnchorStyles::Bottom);
	//tree->SetStyle(-1, WS_CHILD|WS_VISIBLE|TVS_HASBUTTONS|TVS_EDITLABELS);
	this->AddControl(tree);

	labTitle = new Label();
	labTitle->SetName(_T("labTitle"));
	labTitle->SetLocation(200, 2);
	labTitle->SetSize(200, 30);
	labTitle->SetFont(20);
	labTitle->Anchor = (AnchorStyles::Type)(AnchorStyles::Top | AnchorStyles::Left | AnchorStyles::Right);
	this->AddControl(labTitle);

	labLine1 = new Label();
	labLine1->SetName(_T("labLine1"));
	labLine1->SetLocation(200, 32);
	labLine1->SetSize(200, 2);
	labLine1->SetStyle(StyleType::Style, 0xFFFFFFFF, WS_OVERLAPPED | WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | SS_LEFT | SS_CENTER | SS_BLACKRECT | SS_GRAYFRAME | SS_NOTIFY);
	labLine1->SetStyle(StyleType::ExStyle, 0xFFFFFFFF, WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR | WS_EX_NOINHERITLAYOUT);
	labLine1->Anchor = (AnchorStyles::Type)(AnchorStyles::Top | AnchorStyles::Left | AnchorStyles::Right);
	this->AddControl(labLine1);

	labLine2 = new Label();
	labLine2->SetName(_T("labLine2"));
	labLine2->SetLocation(200, 32);
	labLine2->SetSize(200, 2);
	labLine2->SetStyle(StyleType::Style, 0xFFFFFFFF, WS_OVERLAPPED | WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | SS_LEFT | SS_CENTER | SS_BLACKRECT | SS_GRAYFRAME | SS_NOTIFY);
	labLine2->SetStyle(StyleType::ExStyle, 0xFFFFFFFF, WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR | WS_EX_NOINHERITLAYOUT);
	labLine2->Anchor = (AnchorStyles::Type)(AnchorStyles::Top | AnchorStyles::Left | AnchorStyles::Right);
	this->AddControl(labLine2);

	btnHelp = new Button();
	btnHelp->SetName(_T("btnHelp"));
	btnHelp->SetLocation(400 - 240 - 8, 400 - 50);
	btnHelp->SetSize(80, 19);
	btnHelp->Anchor = (AnchorStyles::Type)(AnchorStyles::Top | AnchorStyles::Left | AnchorStyles::Bottom);
	btnHelp->Click += EventHandler((Object*)this, (EventFun)&OptionCtl::btnHelp_Click);
	this->AddControl(btnHelp);

	btnApply = new Button();
	btnApply->SetName(_T("btnApply"));
	btnApply->SetLocation(400 - 160 - 8, 400 - 50);
	btnApply->SetSize(80, 19);
	btnApply->Anchor = (AnchorStyles::Type)(AnchorStyles::Top | AnchorStyles::Left | AnchorStyles::Bottom);
	btnApply->Click += EventHandler((Object*)this, (EventFun)&OptionCtl::btnApply_Click);
	this->AddControl(btnApply);

	btnDefault = new Button();
	btnDefault->SetName(_T("btnDefault"));
	btnDefault->SetLocation(400 - 80 - 6, 400 - 50);
	btnDefault->SetSize(80, 19);
	btnDefault->Anchor = (AnchorStyles::Type)(AnchorStyles::Top | AnchorStyles::Left | AnchorStyles::Bottom);
	btnDefault->Click += EventHandler((Object*)this, (EventFun)&OptionCtl::btnDefault_Click);
	this->AddControl(btnDefault);

	btnOK = new Button();
	btnOK->SetName(_T("btnOK"));
	btnOK->SetLocation(400 - 160 - 8, 400 - 19 - 3);
	btnOK->SetSize(80, 19);
	btnOK->Anchor = (AnchorStyles::Type)(AnchorStyles::Top | AnchorStyles::Left | AnchorStyles::Bottom);
	btnOK->Click += EventHandler((Object*)this, (EventFun)&OptionCtl::btnOK_Click);
	this->AddControl(btnOK);

	btnCancel = new Button();
	btnCancel->SetName(_T("btnCancel"));
	btnCancel->SetLocation(400 - 80 - 6, 400 - 19 - 3);
	btnCancel->SetSize(80, 19);
	btnCancel->Anchor = (AnchorStyles::Type)(AnchorStyles::Top | AnchorStyles::Left | AnchorStyles::Bottom);
	btnCancel->Click += EventHandler((Object*)this, (EventFun)&OptionCtl::btnCancel_Click);
	this->AddControl(btnCancel);

}

OptionCtl::~OptionCtl()
{
}
// constructors/destructor
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// private/protected/internal used methods

void OptionCtl::btnOK_Click(Object* sender, EventArgs* e)
{
	int nCnt = (_lstControls == NULL) ? 0 : _lstControls->GetSize();
	if(nCnt > 0)
	{
		for(int n = 0; n < nCnt; n++)
		{
			OptionPage* ctl = (OptionPage*)_lstControls->GetKey(n);
			ctl->OKClick.Invoke((Object*)this, e);
		}
	}
	OKClick.Invoke((Object*)this, e);
}

void OptionCtl::btnCancel_Click(Object* sender, EventArgs* e)
{
	int nCnt = (_lstControls == NULL) ? 0 : _lstControls->GetSize();
	if(nCnt > 0)
	{
		for(int n = 0; n < nCnt; n++)
		{
			OptionPage* ctl = (OptionPage*)_lstControls->GetKey(n);
			ctl->CancelClick.Invoke((Object*)this, e);
		}
	}
	CancelClick.Invoke((Object*)this, e);
}

void OptionCtl::btnHelp_Click(Object* sender, EventArgs* e)
{
	if(activePage != NULL)
	{
		activePage->HelpClick.Invoke((Object*)this, e);
	}
}

void OptionCtl::btnApply_Click(Object* sender, EventArgs* e)
{
	if(activePage != NULL)
	{
		activePage->ApplyClick.Invoke((Object*)this, e);
	}
}

void OptionCtl::btnDefault_Click(Object* sender, EventArgs* e)
{
	if(activePage != NULL)
	{
		activePage->DefaultClick.Invoke((Object*)this, e);
	}
}

void OptionCtl::CreateHandle()
{
	int nCnt = (_lstControls == NULL) ? 0 : _lstControls->GetSize();
	if(nCnt > 0)
	{
		for(int n = 0; n < nCnt; n++)
		{
			Control* ctl = _lstControls->GetKey(n);
			ctl->SetVisible(false);
		}
	}
	Container::CreateHandle();
	SetSelectedIndex(_nSelectedIndex);
}

void OptionCtl::ResizeOptionPage(OptionPage* page)
{
}

void OptionCtl::tree_SelectedIndexChanged(Object* sender, EventArgs* e)
{
	int nIndex = GetSelectedIndex();
	if(nIndex >= 0 && _lstControls != NULL)
	{
		Control* page = NULL;
		if(_nLastSelectedIndex != nIndex && _nLastSelectedIndex >= 0 && _nLastSelectedIndex < _lstControls->GetSize())
		{
			page = _lstControls->GetKey(_nLastSelectedIndex);
			((TabPage*)page)->SetActivateFocus();
		}
		activePage = (OptionPage*)_lstControls->GetKey(nIndex);
		((Control*)activePage)->SetVisible(true);
		if(page != NULL)
		{
			page->SetVisible(false);
		}
		_nLastSelectedIndex = nIndex;
	}
	SelectedIndexChanged.Invoke((Object*)this, e);
}

int OptionCtl::GetSelectedIndex()
{
	return _nSelectedIndex;
}

void OptionCtl::SetSelectedIndex(int selectedIndex)
{
	if(_lstControls == NULL || _lstControls->GetSize() <= 0)
	{
		_nSelectedIndex = -1;
		activePage = NULL;
		return;
	}
	_nSelectedIndex = selectedIndex;
	if(_nSelectedIndex < 0)
	{
		_nSelectedIndex = 0;
	}
	if(_nSelectedIndex >= _lstControls->GetSize())
	{
		_nSelectedIndex = _lstControls->GetSize() - 1;
	}

	//curent is not enabled,and no next enabled page,set current disabled page
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
	{
		tree_SelectedIndexChanged(NULL, NULL);
	}
}

void OptionCtl::OnResize(EventArgs* e)
{
	Update(UpdateFlag::UpdateBackground);
	//btnApply->SetVisible(IsOptionStyle(OptionStyles::ShowApply));
	//btnApply->SetEnabled(IsOptionStyle(OptionStyles::DisabledApply));
	//btnDefault->SetVisible(IsOptionStyle(OptionStyles::ShowDefault));
	//btnDefault->SetEnabled(IsOptionStyle(OptionStyles::DisabledDefault));
	int nCnt = (_lstControls == NULL) ? 0 : _lstControls->GetSize();
	if(nCnt > 0)
	{
		SetOuterStyle(OuterStyle::Sizing, true);
		RECT body;
		::GetClientRect(m_hWnd, &body);
		body.right = body.right - body.left - 200;
		body.bottom = body.bottom - body.top - 40;
		if(btnApply->GetVisible() || btnDefault->GetVisible())
		{
			body.bottom -= 25;
		}
		for(int n = 0; n < nCnt; n++)
		{
			Control* ctl = _lstControls->GetKey(n);
			ctl->SetVisible(n == _nSelectedIndex);
			ctl->SetBoundsNotOriginal(200, 40, body.right, body.bottom, BoundsSpecified::All);
		}
		activePage = (OptionPage*)_lstControls->GetKey(_nSelectedIndex);
		((Control*)activePage)->SetVisible(true);
		Container::OnResize(e);
		SetOuterStyle(OuterStyle::Sizing, false);
	}
}
//
//void OptionCtl::OnHandleCreated(EventArgs* e)
//{
//	UserControl::OnHandleCreated(e);
//
//	OnLoad(e);
//}
//
//void OptionCtl::OnLoad(EventArgs* e)
//{
//	Load.Invoke((Object*)this, e);
//}
// private/protected/internal used methods
///////////////////////////////////////////////////////////////////////////////

#define CC_CONFIGBACKDLG_NOFILE "Background Image File not exist:%s"
#define CC_CONFIGBACKDLG_WWIDTH 375
//===========================================================cc::win::ConfigBackDlg
///////////////////////////////////////////////////////////////////////////////
// constructors/destructor
//the Width&Height is for client area
ConfigBackDlg::ConfigBackDlg()
{
	m_pResultCtl = NULL;
	m_dwResultStyle = 0;
	m_kaSave = NULL;
	int nTop = 5;
	radBack1 = new RadioButton();
	radBack1->SetName(_T("rad1"));
	radBack1->SetText(_T("Single Color:"));
	radBack1->SetLocation(5, nTop);
	//radBack1->SetStyle(StyleType::Outer, 0, OuterStyle::AutoSize|OuterStyle::AutoConfig);
	radBack1->SetStyle(StyleType::Outer, 0, OuterStyle::AutoSize);
	radBack1->Click += EventHandler((Object*)this, (EventFun)&ConfigBackDlg::Control_Click);
	this->AddControl(radBack1);
	pickColor1 = new PickColorCtl();
	pickColor1->SetName(_T("Color1"));
	pickColor1->SetText(_T("Color"));
	pickColor1->SetLocation(120, nTop);
	pickColor1->SetSize(55, 20);
	//pickColor1->SetStyle(StyleType::Outer, OuterStyle::OuterEdge|OuterStyle::PushButton, OuterStyle::AutoSize|OuterStyle::AutoConfig);
	pickColor1->SetStyle(StyleType::Outer, 0, OuterStyle::AutoSize|OuterStyle::AutoConfig);
	pickColor1->ValueChanged += EventHandler((Object*)this, (EventFun)&ConfigBackDlg::Control_Click);
	this->AddControl(pickColor1);

	//Hatch Styles
	//#define   HS_HORIZONTAL  0  /*   -----   */  
	//#define   HS_VERTICAL    1  /*   |||||   */  
	//#define   HS_FDIAGONAL   2  /*   \\\\\   */  
	//#define   HS_BDIAGONAL   3  /*   /////   */  
	//#define   HS_CROSS       4  /*   +++++   */  
	//#define   HS_DIAGCROSS   5  /*   xxxxx   */
	//nTop += 22;
	combCross = new ComboBox();
	combCross->SetName(_T("Cross"));
	combCross->SetComboBoxStyle(ComboBoxStyle::DropDownList);
	combCross->SetLocationOffset(pickColor1, false, 3, 0);
	combCross->SetSize(100, 145);
	combCross->SetStyle(StyleType::Outer, 0, OuterStyle::AutoConfig);
	combCross->SelectedIndexChanged += EventHandler((Object*)this, (EventFun)&ConfigBackDlg::Control_Click);
	this->AddControl(combCross);

	nTop += 22;
	radBack2 = new RadioButton();
	radBack2->SetName(_T("rad2"));
	radBack2->SetText(_T("Gradient Color:"));
	radBack2->SetLocation(5, nTop);
	//radBack2->SetStyle(StyleType::Outer, 0, OuterStyle::AutoSize|OuterStyle::AutoConfig);
	radBack2->SetStyle(StyleType::Outer, 0, OuterStyle::AutoSize);
	radBack2->Click += EventHandler((Object*)this, (EventFun)&ConfigBackDlg::Control_Click);
	this->AddControl(radBack2);

	//nTop += 20;
	pickColor2 = new PickColorCtl();
	pickColor2->SetName(_T("Color2"));
	pickColor2->SetText(_T("Color From"));
	pickColor2->SetLocation(120, nTop);
	pickColor2->SetSize(85, 20);
	//pickColor2->SetStyle(StyleType::Outer, OuterStyle::OuterEdge|OuterStyle::PushButton, OuterStyle::AutoSize|OuterStyle::AutoConfig);
	pickColor2->SetStyle(StyleType::Outer, 0, OuterStyle::AutoSize|OuterStyle::AutoConfig);
	pickColor2->ValueChanged += EventHandler((Object*)this, (EventFun)&ConfigBackDlg::Control_Click);
	this->AddControl(pickColor2);
	pickColor3 = new PickColorCtl();
	pickColor3->SetName(_T("Color3"));
	pickColor3->SetText(_T("Color To"));
	pickColor3->SetLocationOffset(pickColor2, false, 3, 0);
	pickColor3->SetSize(75, 20);
	//pickColor3->SetStyle(StyleType::Outer, OuterStyle::OuterEdge|OuterStyle::PushButton, OuterStyle::AutoSize|OuterStyle::AutoConfig);
	pickColor3->SetStyle(StyleType::Outer, 0, OuterStyle::AutoSize|OuterStyle::AutoConfig);
	pickColor3->ValueChanged += EventHandler((Object*)this, (EventFun)&ConfigBackDlg::Control_Click);
	this->AddControl(pickColor3);
	combVertical = new ComboBox();
	combVertical->SetName(_T("Vertical"));
	combVertical->SetComboBoxStyle(ComboBoxStyle::DropDownList);
	combVertical->SetLocationOffset(pickColor3, false, 3, 0);
	combVertical->SetSize(60, 145);
	combVertical->SetStyle(StyleType::Outer, 0, OuterStyle::AutoConfig);
	combVertical->SelectedIndexChanged += EventHandler((Object*)this, (EventFun)&ConfigBackDlg::Control_Click);
	this->AddControl(combVertical);

	nTop += 22;
	radBack3 = new RadioButton();
	radBack3->SetName(_T("rad3"));
	radBack3->SetText(_T("Outer Image:"));
	radBack3->SetLocation(5, nTop);
	//radBack3->SetStyle(StyleType::Outer, 0, OuterStyle::AutoSize|OuterStyle::AutoConfig);
	radBack3->SetStyle(StyleType::Outer, 0, OuterStyle::AutoSize);
	radBack3->Click += EventHandler((Object*)this, (EventFun)&ConfigBackDlg::Control_Click);
	this->AddControl(radBack3);
	label = new Label();
	label->SetName(_T("labImg"));
	label->SetText(_T("File:"));
	label->SetLocation(120, nTop + 2);
	label->SetSize(45, 17);
	this->AddControl(label);
	editImg = new TextBox();
	editImg->SetName(_T("ImgPath"));
	editImg->SetText(_T(""));
	editImg->SetLocationOffset(label, false, 0, -2);
	editImg->SetSize(95, 17);
	editImg->SetOuterStyle(OuterStyle::AutoConfig, true);
	this->AddControl(editImg);
	btnSelFile = new PathButton();
	btnSelFile->SetLocationOffset(editImg, false, 0, 1);
	btnSelFile->SetSize(15, 17);
	btnSelFile->IsFile = true;
	btnSelFile->PathControl = editImg;
	btnSelFile->tcCaption = _T("select Image file:");
	btnSelFile->tcFilter = _T("Image Files (*.jpg;*.gif;*.tif;*.bmp;*.ico)\0*.jpg;*.gif;*.tif;*.bmp;*.ico\0\0");
	btnSelFile->tcDefaultExt = _T("bmp");
	btnSelFile->Click += EventHandler((Object*)this, (EventFun)&ConfigBackDlg::Control_Click);
	this->AddControl(btnSelFile);
	combStretch = new ComboBox();
	combStretch->SetName(_T("Stretch"));
	combStretch->SetComboBoxStyle(ComboBoxStyle::DropDownList);
	combStretch->SetLocationOffset(btnSelFile, false, 3, 0);
	combStretch->SetSize(90, 145);
	combStretch->SetStyle(StyleType::Outer, 0, OuterStyle::AutoConfig);
	combStretch->SelectedIndexChanged += EventHandler((Object*)this, (EventFun)&ConfigBackDlg::Control_Click);
	this->AddControl(combStretch);

	nTop += 22;
	radBack4 = new RadioButton();
	radBack4->SetName(_T("rad4"));
	radBack4->SetText(_T("Theme(Window XP or above):"));
	radBack4->SetLocation(5, nTop);
	//radBack4->SetStyle(StyleType::Outer, 0, OuterStyle::AutoSize|OuterStyle::AutoConfig);
	radBack4->SetStyle(StyleType::Outer, 0, OuterStyle::AutoSize);
	radBack4->Click += EventHandler((Object*)this, (EventFun)&ConfigBackDlg::Control_Click);
	this->AddControl(radBack4);

	nTop += 22;
	radBack5 = new RadioButton();
	radBack5->SetName(_T("rad5"));
	radBack5->SetText(_T("Inner Image:"));
	radBack5->SetLocation(5, nTop);
	//radBack5->SetStyle(StyleType::Outer, 0, OuterStyle::AutoSize|OuterStyle::AutoConfig);
	radBack5->SetStyle(StyleType::Outer, 0, OuterStyle::AutoSize);
	radBack5->Click += EventHandler((Object*)this, (EventFun)&ConfigBackDlg::Control_Click);
	this->AddControl(radBack5);
	combInnerBmp = new ComboBox();
	combInnerBmp->SetName(_T("InnerImg"));
	combInnerBmp->SetText(_T("NONE"));
	combInnerBmp->SetComboBoxStyle(ComboBoxStyle::DropDownList);
	combInnerBmp->SetLocation(120, nTop);
	combInnerBmp->SetSize(150, 145);
	combInnerBmp->SetStyle(StyleType::Outer, 0, OuterStyle::AutoConfig);
	combInnerBmp->SelectedIndexChanged += EventHandler((Object*)this, (EventFun)&ConfigBackDlg::Control_Click);
	this->AddControl(combInnerBmp);

	nTop += 25;
	btnOK = new Button();
	btnOK->SetName(_T("btnOK"));
	btnOK->SetLocation(3, nTop);
	btnOK->SetSize(70, 19);
	btnOK->Click += EventHandler((Object*)this, (EventFun)&ConfigBackDlg::Control_Click);
	btnOK->SetText((TCHAR*)CC_APP_GetLang()->Get(_T("Lang.S_Dialog_OK")).GetBuf());
	this->AddControl(btnOK);
	this->SetOKButton(btnOK);

	btnCancel = new Button();
	btnCancel->SetName(_T("btnCancel"));
	btnCancel->SetLocation(75, nTop);
	btnCancel->SetSize(70, 19);
	btnCancel->Click += EventHandler((Object*)this, (EventFun)&ConfigBackDlg::Control_Click);
	btnCancel->SetText((TCHAR*)CC_APP_GetLang()->Get(_T("Lang.S_Dialog_Cancel")).GetBuf());
	this->AddControl(btnCancel);
	this->SetCancelButton(btnCancel);

	//this->SetCloseButtonEnable(false);
	this->SetSizeBaseOnClient(CC_CONFIGBACKDLG_WWIDTH, nTop + 19 + 10);
	this->SetName(_T("CfgBack"));
	this->Closed += EventHandler((Object*)this, (EventFun)&ConfigBackDlg::Form_Closed);
	//this module need next,but with next then cannot use design,so set next while "this->Config"
	//this->SetConfigName(_T(""));
	//this->SetOuterStyle(OuterStyle::AutoConfig, true);

	m_sThisHead.ValueOf(_T("CfgBack"));
	SetLang(*CC_APP_GetLang(), cc::Str(CC_CONFIG_LANGSECTION));
}

ConfigBackDlg::~ConfigBackDlg()
{
}
// constructors/destructor
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// private/protected/internal used methods
//void ConfigBackDlg::DoSetLang(KeyArr& keyArr, cc::Str sThisHead)
//{
//	Dialog::DoSetLang(keyArr, sThisHead);
//}

void ConfigBackDlg::SetResultControl(Control* pResultCtl, DWORD dwResultStyle, cc::Str sThisHead, cc::KeyArr* kaSave)
{
	m_pResultCtl = pResultCtl;
	m_dwResultStyle = dwResultStyle;
	m_kaSave = kaSave;
	m_sThisHead = sThisHead;
	if(m_sThisHead.GetLength() <= 0)
	{
		m_sThisHead.ValueOf(_T("CfgBack"));
	}
}

void ConfigBackDlg::CreateHandle()
{
	cc::KeyArr* l_kaSave = m_kaSave;
	if(l_kaSave == NULL)
	{
		l_kaSave = CC_APP_GetIni();
	}
	m_dwResultStyle |= CC_CONFIGBACKDLG_DONOTHING;
	combInnerBmp->SetText(sInnerImgs);
	sInnerImgs.Clear();
	Dialog::CreateHandle();
	//load config
	this->SetConfigName(_T(""));
	this->Config(true, *l_kaSave, m_sThisHead);
	this->SetConfigName(NULL);

	combCross->SetConfigData(_T("Hatch:NONE;	-----;	|||||;	\\\\\\\\\\;	/////;	+++++;	xxxxx;	"));
	combVertical->SetConfigData(_T("Horizontal;	Vertical;	"));
	combStretch->SetConfigData(_T("Repeat;	Stretch;	"));

	int nIndex = (int)l_kaSave->Get(m_sThisHead + _T(".Cross"), (DWORD)0);
	combCross->SetSelectedIndex(nIndex);
	nIndex = (int)l_kaSave->Get(m_sThisHead + _T(".Vertical"), (DWORD)0);
	combVertical->SetSelectedIndex(nIndex);
	nIndex = (int)l_kaSave->Get(m_sThisHead + _T(".Stretch"), (DWORD)0);
	combStretch->SetSelectedIndex(nIndex);

	nIndex = (int)l_kaSave->Get(m_sThisHead + _T(".Index"), (DWORD)-1);
	//if(!l_kaSave->GetBool(m_sThisHead + _T(".Saved"), false))
	if(nIndex < 0 || nIndex >= 5)
	{
		//l_kaSave->SetBool(m_sThisHead + _T(".Saved"), true);
		if(m_pResultCtl != NULL)
		{
			pickColor1->SetBackground(m_pResultCtl->GetBackground());
			pickColor2->SetBackground(m_pResultCtl->GetHotBackColor());
			pickColor3->SetBackground(m_pResultCtl->GetBackground());
			if(m_pResultCtl->HasStyle(StyleType::Inner, InnerStyle::BackgroundTheme))
			{
				radBack4->SetChecked(true);
				nIndex = 3;
			}
			else if(m_pResultCtl->HasStyle(StyleType::Inner, InnerStyle::BackgroundGradient))
			{
				radBack2->SetChecked(true);
				nIndex = 1;
			}
			else
			{
				//BackgroundColor
				radBack1->SetChecked(true);
				nIndex = 0;
			}
		}
		else
		{
			radBack1->SetChecked(true);
			nIndex = 0;
		}
		l_kaSave->Set(m_sThisHead + _T(".Index"), nIndex);
		combInnerBmp->SetSelectedIndex(0);
		//save config
		this->SetConfigName(_T(""));
		this->Config(false, *l_kaSave, m_sThisHead);
		this->SetConfigName(NULL);
	}
	else
	{
		if(nIndex == 0 || (nIndex == 4 && combInnerBmp->ItemCount() < 1))
		{
			radBack1->SetChecked(true);
			if(nIndex == 2)
			{
				l_kaSave->Set(m_sThisHead + _T(".Index"), (DWORD)0);
			}
		}
		else if(nIndex == 1)
		{
			radBack2->SetChecked(true);
		}
		else if(nIndex == 2)
		{
			radBack3->SetChecked(true);
		}
		else if(nIndex == 3)
		{
			radBack4->SetChecked(true);
		}
		else if(nIndex == 4)
		{
			radBack5->SetChecked(true);
		}
	}
	if(combInnerBmp->ItemCount() < 1)
	{
		radBack5->SetEnabled(false);
		combInnerBmp->SetEnabled(false);
	}
	btnCancel->Focus();
	m_dwResultStyle &= ~CC_CONFIGBACKDLG_DONOTHING;
}

void ConfigBackDlg::Form_Closed(Object* sender, EventArgs* e)
{
	cc::KeyArr* l_kaSave = m_kaSave;
	if(l_kaSave == NULL)
	{
		l_kaSave = CC_APP_GetIni();
	}
	CancelClick.Invoke((Object*)this, e);
	ConfigResult(m_pResultCtl, m_dwResultStyle, m_sThisHead, l_kaSave);
}

void ConfigBackDlg::Control_Click(Object* sender, EventArgs* e)
{
	cc::KeyArr* l_kaSave = m_kaSave;
	if(l_kaSave == NULL)
	{
		l_kaSave = CC_APP_GetIni();
	}
	if(sender == btnOK)
	{
		e->Handled = false;
		OKClick.Invoke((Object*)this, e);
		if(e->Handled)
		{
			return;
		}
		int nIndex = 0;
		if(radBack1->GetChecked())
		{
		}
		else if(radBack2->GetChecked())
		{
			nIndex = 1;
		}
		else if(radBack3->GetChecked())
		{
			const TCHAR* tcPath = editImg->GetText();
			if(!cc::Util::fExist(tcPath))
			{
				cc::Str retPath = cc::Util::getStartupRelatePath(tcPath);
				if(!cc::Util::fExist(retPath))
				{
					::MessageBox(m_hWnd, CC_APP_GetLang()->GetFormatDefaultValue(cc::Str(CC_CONFIGBACKDLG_NOFILE), _T("Lang.CfgBack_NoFile"), tcPath), CC_APP_GetLang()->Get(_T("Lang.S_MsgBox_Title_Critical")), MB_OK | MB_ICONSTOP);
					return;
				}
			}
			nIndex = 2;
		}
		else if(radBack4->GetChecked())
		{
			nIndex = 3;
		}
		else if(radBack5->GetChecked())
		{
			nIndex = 4;
		}
		//save config
		l_kaSave->Set(m_sThisHead + _T(".Index"), nIndex);
		this->SetConfigName(_T(""));
		this->Config(false, *l_kaSave, m_sThisHead);
		this->SetConfigName(NULL);
		ConfigResult(m_pResultCtl, m_dwResultStyle, m_sThisHead, l_kaSave);
		EndDialog(DialogResult::OK);
	}
	else if(sender == btnCancel)
	{
		e->Handled = false;
		CancelClick.Invoke((Object*)this, e);
		if(e->Handled)
		{
			return;
		}
		ConfigResult(m_pResultCtl, m_dwResultStyle, m_sThisHead, l_kaSave);
		EndDialog(DialogResult::Cancel);
	}

	if(m_pResultCtl == NULL || (m_dwResultStyle & CC_CONFIGBACKDLG_DONOTHING) != 0)
	{
		return;
	}
	if(sender == radBack1)
	{
		if(m_pResultCtl->HasStyle(StyleType::Inner, InnerStyle::BackgroundGradient))
		{
			m_pResultCtl->SetStyle(StyleType::Inner, InnerStyle::BackgroundGradient, 0);
		}

		//#define   HS_HORIZONTAL  0  /*   -----   */
		//#define   HS_VERTICAL    1  /*   |||||   */
		//#define   HS_FDIAGONAL   2  /*   \\\\\   */
		//#define   HS_BDIAGONAL   3  /*   /////   */
		//#define   HS_CROSS       4  /*   +++++   */
		//#define   HS_DIAGCROSS   5  /*   xxxxx   */
		int nInd = combCross->GetSelectedIndex();
		//if(nInd > 0)
		//{
		//	HBRUSH hbr = ::CreateHatchBrush(nInd - 1, pickColor1->GetBackground());
		//	m_pResultCtl->SetBackgroundBrush(hbr);
		//}
		m_pResultCtl->SetBackground(pickColor1->GetBackground(), nInd);
	}
	else if(sender == radBack2)
	{
		m_pResultCtl->SetHotBackColor(pickColor2->GetBackground());
		m_pResultCtl->SetBackground(pickColor3->GetBackground());
		int nInd = combVertical->GetSelectedIndex();
		m_pResultCtl->SetStyleBool(StyleType::Inner2, Inner2Style::GradientVertical, nInd == 1);
		m_pResultCtl->SetStyle(StyleType::Inner,
			InnerStyle::BackgroundBrush|InnerStyle::BackgroundColor|InnerStyle::BackgroundBmp|InnerStyle::BackgroundTheme,
			InnerStyle::BackgroundGradient);
	}
	else if(sender == radBack3)
	{
		cc::Str sPath = editImg->GetText();
		if(sPath.GetLength() > 0 && !cc::Util::fExist(sPath))
		{
			sPath = cc::Util::getStartupRelatePath(sPath);
			if(sPath.GetLength() > 0 && !cc::Util::fExist(sPath))
			{
				::MessageBox(m_hWnd, CC_APP_GetLang()->GetFormatDefaultValue(cc::Str(CC_CONFIGBACKDLG_NOFILE), _T("Lang.CfgBack_NoFile"), (TCHAR*)sPath), CC_APP_GetLang()->Get(_T("Lang.S_MsgBox_Title_Critical")), MB_OK | MB_ICONSTOP);
				return;
			}
		}
		if(m_pResultCtl->HasStyle(StyleType::Inner, InnerStyle::BackgroundGradient))
		{
			m_pResultCtl->SetStyle(StyleType::Inner, InnerStyle::BackgroundGradient, 0);
		}
		int nInd = combStretch->GetSelectedIndex();
		m_pResultCtl->SetBackgroundImage(sPath, nInd == 1);
	}
	else if(sender == radBack4)
	{
		m_pResultCtl->SetStyle(StyleType::Inner,
			InnerStyle::BackgroundBrush|InnerStyle::BackgroundColor|InnerStyle::BackgroundBmp|InnerStyle::BackgroundGradient,
			InnerStyle::BackgroundTheme);
	}
	else if(sender == radBack5)
	{
		if(m_pResultCtl->HasStyle(StyleType::Inner, InnerStyle::BackgroundGradient))
		{
			m_pResultCtl->SetStyle(StyleType::Inner, InnerStyle::BackgroundGradient, 0);
		}
		int nInd = combInnerBmp->GetSelectedIndex();
		if(nInd >= 0)
		{
			cc::Str sbuf = cc::Str(combInnerBmp->ItemGet(nInd));
			int nPos = sbuf.LastIndexOf(_T("["));
			//sbuf.Format(_T("%s[%d,%d,%d]"), info, id, isBmp, isRepeat);
			if(nPos > 0)
			{
				sbuf.Remove(0, nPos + 1);
				cc::Str sbuf2 = sbuf.SplitBy(_T(","));
				int nResID = sbuf2.ToInt();
				cc::Str sResType = sbuf.SplitBy(_T(","));
				sbuf2 = sbuf.SplitBy(_T(","));
				int nStretch = sbuf2.ToInt();
				l_kaSave->Set(m_sThisHead + _T(".ResID"), nResID);
				l_kaSave->Set(m_sThisHead + _T(".ResType"), sResType);
				l_kaSave->Set(m_sThisHead + _T(".Stretch3"), nStretch);

				//const TCHAR* resType = NULL;
				//if(sResType.StartsWith(_T("#")))
				//{
				//	sResType.Remove(0, 1);
				//	resType = MAKEINTRESOURCE(sResType.ToInt());
				//}
				//else
				//{
				//	resType = (const TCHAR*)sResType.GetBuf();
				//}
				//const TCHAR* resType = sResType.StartsWith(_T("#")) ? (const TCHAR*)sResType.GetBuf() : MAKEINTRESOURCE(sResType.ToInt());
				m_pResultCtl->SetBackgroundImage(nResID, (const TCHAR*)sResType.GetBuf(), nStretch == 1);
			}
		}
	}
	else if(sender == pickColor1 || sender == combCross)
	{
		if(radBack1->GetChecked())
		{
			Control_Click(radBack1, NULL);
		}
	}
	else if(sender == combVertical || sender == pickColor2 || sender == pickColor3)
	{
		if(radBack2->GetChecked())
		{
			Control_Click(radBack2, NULL);
		}
	}
	else if(sender == combInnerBmp)
	{
		if(radBack5->GetChecked())
		{
			Control_Click(radBack5, NULL);
		}
	}
	else if(sender == combStretch)
	{
		if(radBack3->GetChecked())
		{
			Control_Click(radBack3, NULL);
		}
	}
	else if(sender == btnSelFile)
	{
		if(radBack3->GetChecked())
		{
			if(btnSelFile->SelectedPath.GetLength() > 0)
			{
				Control_Click(radBack3, NULL);
			}
		}
	}
}

void ConfigBackDlg::AddInnerImage(UINT nResID, const TCHAR *resType, bool isStretch, const TCHAR* info)
{
	cc::Str sbuf;
	if(IS_INTRESOURCE(resType))
	{
		sbuf.Format(_T("%s[%d,#%d,%d,]"), info, nResID, (WORD)((ULONG_PTR)resType), isStretch);
	}
	else
	{
		sbuf.Format(_T("%s[%d,%s,%d,]"), info, nResID, resType, isStretch);
	}
	sInnerImgs.Append(sbuf);
	sInnerImgs.Append(_T(CONTROL_LINE_SPLIT));
}

void ConfigBackDlg::ConfigResult(Control* pResultCtl, DWORD dwResultStyle, cc::Str sThisHead, cc::KeyArr* kaSave)
{
	if(pResultCtl == NULL)
	{
		return;
	}

	if(kaSave == NULL)
	{
		kaSave = CC_APP_GetIni();
	}
	if(sThisHead.GetLength() < 1)
	{
		sThisHead.ValueOf(_T("CfgBack"));
	}
	int nIndex = (int)kaSave->Get(sThisHead + _T(".Index"), (DWORD)-1);
	if(nIndex == 1)
	{
		COLORREF cor = (COLORREF)kaSave->Get(sThisHead + _T(".Color2"), (DWORD)0);
		pResultCtl->SetHotBackColor(cor);
		cor = (COLORREF)kaSave->Get(sThisHead + _T(".Color3"), (DWORD)0);
		pResultCtl->SetBackground(cor);
		pResultCtl->SetStyleBool(StyleType::Inner2, Inner2Style::GradientVertical, kaSave->Get(sThisHead + _T(".Vertical"), (DWORD)0) == 1);
		pResultCtl->SetStyle(StyleType::Inner,
			InnerStyle::BackgroundBrush|InnerStyle::BackgroundColor|InnerStyle::BackgroundBmp|InnerStyle::BackgroundTheme,
			InnerStyle::BackgroundGradient);
	}
	else if(nIndex == 2)
	{
		if(pResultCtl->HasStyle(StyleType::Inner, InnerStyle::BackgroundGradient))
		{
			pResultCtl->SetStyle(StyleType::Inner, InnerStyle::BackgroundGradient, 0);
		}
		cc::Str sPath = kaSave->Get(sThisHead + _T(".ImgPath"));

		if(!cc::Util::fExist((const TCHAR*)sPath.GetBuf()))
		{
			sPath = cc::Util::getStartupRelatePath(sPath);
		}
		if(!cc::Util::fExist((const TCHAR*)sPath.GetBuf()))
		{
			//save config
			kaSave->Set(sThisHead + _T(".Index"), (DWORD)0);
			//next do as nIndex = 0
			nIndex = 99;
		}
		else
		{
			pResultCtl->SetBackgroundImage((const TCHAR*)sPath.GetBuf(), kaSave->Get(sThisHead + _T(".Stretch"), (DWORD)0) == 1);
		}
	}
	else if(nIndex == 3)
	{
		pResultCtl->SetStyle(StyleType::Inner,
			InnerStyle::BackgroundBrush|InnerStyle::BackgroundColor|InnerStyle::BackgroundBmp|InnerStyle::BackgroundGradient,
			InnerStyle::BackgroundTheme);
	}
	else if(nIndex == 4)
	{
		if(pResultCtl->HasStyle(StyleType::Inner, InnerStyle::BackgroundGradient))
		{
			pResultCtl->SetStyle(StyleType::Inner, InnerStyle::BackgroundGradient, 0);
		}
		int nResID = kaSave->Get(sThisHead + _T(".ResID"), 0);
		cc::Str sResType = kaSave->Get(sThisHead + _T(".ResType"));
		int nStretch = kaSave->Get(sThisHead + _T(".Stretch3"), 0);
		pResultCtl->SetBackgroundImage(nResID, (const TCHAR*)sResType.GetBuf(), nStretch == 1);
	}
	if(nIndex == 0 || nIndex == 99)
	{
		if(pResultCtl->HasStyle(StyleType::Inner, InnerStyle::BackgroundGradient))
		{
			pResultCtl->SetStyle(StyleType::Inner, InnerStyle::BackgroundGradient, 0);
		}
		COLORREF cor = (COLORREF)kaSave->Get(sThisHead + _T(".Color1"), (DWORD)0);
		//first is NONE
		int nInd = kaSave->Get(sThisHead + _T(".Cross"), (DWORD)0);
		//if(nInd > 0)
		//{
		//	HBRUSH hbr = ::CreateHatchBrush(nInd - 1, cor);
		//	pResultCtl->SetBackgroundBrush(hbr);
		//}
		pResultCtl->SetBackground(cor, nInd);
		if(nIndex == 99)
		{
			//ImgPath not exist
			cc::Str sPath = kaSave->Get(sThisHead + _T(".ImgPath"));
			::MessageBox(pResultCtl->m_hWnd, CC_APP_GetLang()->GetFormatDefaultValue(cc::Str(CC_CONFIGBACKDLG_NOFILE),
				_T("Lang.CfgBack_NoFile"), (const TCHAR*)sPath.GetBuf()),
				CC_APP_GetLang()->Get(_T("Lang.S_MsgBox_Title_Critical")), MB_OK | MB_ICONSTOP);
		}
	}
}
///////////////////////////////////////////////////////////////////////////////

//===========================================================cc::win::LEDControl
///////////////////////////////////////////////////////////////////////////////
//http://www.codeproject.com/KB/static/cstatictime.aspx
DWORD g_LEDControlFlag[] = {
	LEDCONTROL_RECT0 | LEDCONTROL_RECT1 | LEDCONTROL_RECT2 | LEDCONTROL_RECT3 | LEDCONTROL_RECT4 | LEDCONTROL_RECT5,
	LEDCONTROL_RECT1 | LEDCONTROL_RECT2,
	LEDCONTROL_RECT0 | LEDCONTROL_RECT1 | LEDCONTROL_RECT3 | LEDCONTROL_RECT4 | LEDCONTROL_RECT6,
	LEDCONTROL_RECT0 | LEDCONTROL_RECT1 | LEDCONTROL_RECT2 | LEDCONTROL_RECT3 | LEDCONTROL_RECT6,
	LEDCONTROL_RECT1 | LEDCONTROL_RECT2 | LEDCONTROL_RECT5 | LEDCONTROL_RECT6,
	LEDCONTROL_RECT0 | LEDCONTROL_RECT2 | LEDCONTROL_RECT3 | LEDCONTROL_RECT5 | LEDCONTROL_RECT6,
	LEDCONTROL_RECT0 | LEDCONTROL_RECT2 | LEDCONTROL_RECT3 | LEDCONTROL_RECT4 | LEDCONTROL_RECT5 | LEDCONTROL_RECT6,
	LEDCONTROL_RECT0 | LEDCONTROL_RECT1 | LEDCONTROL_RECT2,
	LEDCONTROL_RECT0 | LEDCONTROL_RECT1 | LEDCONTROL_RECT2 | LEDCONTROL_RECT3 | LEDCONTROL_RECT4 | LEDCONTROL_RECT5 | LEDCONTROL_RECT6,
	LEDCONTROL_RECT0 | LEDCONTROL_RECT1 | LEDCONTROL_RECT2 | LEDCONTROL_RECT3 | LEDCONTROL_RECT5 | LEDCONTROL_RECT6,
	//colon(:)
	LEDCONTROL_RECT7 | LEDCONTROL_RECT8,
	//bottom(.)
	LEDCONTROL_RECT9,
	0
};

DWORD LEDControlConfig::nStyle = WS_CHILDWINDOW | WS_VISIBLE | WS_TABSTOP;
DWORD LEDControlConfig::nExStyle = 0;
//DWORD LEDControlConfig::nClassStyle = CS_DBLCLKS | CS_GLOBALCLASS;
DWORD LEDControlConfig::nInnerStyle = InnerStyle::TranceContextMenuUp;
//Blank for not draw text
DWORD LEDControlConfig::nOuterStyle = OuterStyle::Blank;
DWORD LEDControlConfig::nOwnerDrawTextStyle = DT_LEFT|DT_SINGLELINE;
COLORREF LEDControlConfig::corBackground = ::GetSysColor(COLOR_BTNFACE);
COLORREF LEDControlConfig::corTextground = ::GetSysColor(COLOR_BTNTEXT);
COLORREF LEDControlConfig::corHotBackColor = ::GetSysColor(COLOR_BTNFACE);
COLORREF LEDControlConfig::corHotTextColor = ::GetSysColor(COLOR_BTNTEXT);
int LEDControlConfig::nMarginLeft = 0;
int LEDControlConfig::nMarginTop = 0;
int LEDControlConfig::nMarginRight = 0;
int LEDControlConfig::nMarginBottom = 0;
// constructors/destructor
LEDControl::LEDControl()
{
	BASE_CONTROL_CONFIG_INIT(LEDControl);
	::SetRect(&_Margin, PanelConfig::nMarginLeft, PanelConfig::nMarginTop, PanelConfig::nMarginRight, PanelConfig::nMarginBottom);
	//m_nCharWidth = 10;
	m_nCharWidth = -1;
	m_nMarginPercentOfChar = 0.25;
	_corTextBackground = _corBackground;
	SetText(_T("99:99:99"));
	CC_BIT_SET(m_nFlag, CC_LED_START, true);
	CC_BIT_SET(m_nFlag, CC_LED_HHMM, false);
	m_nTimerSec = -1;
}

///////////////////// destructor //////////////////////////
LEDControl::~LEDControl()
{
}
// constructors/destructor

void LEDControl::SetStart(bool isStart)
{
	CC_BIT_SET(m_nFlag, CC_LED_START, isStart);
}

bool LEDControl::GetStart()
{
	return (CC_BIT_IS(m_nFlag, CC_LED_START));
}

void LEDControl::SetTimerSec(int nTimerSec)
{
	m_nTimerSec = (double)nTimerSec;
	CC_BIT_SET(m_nFlag, CC_LED_REDRAW, true);
}

int LEDControl::GetTimerSec()
{
	return (int)m_nTimerSec;
}

void LEDControl::SetHhmm(bool isHhmm)
{
	CC_BIT_SET(m_nFlag, CC_LED_HHMM, isHhmm);
}

bool LEDControl::GetHhmm()
{
	return (CC_BIT_IS(m_nFlag, CC_LED_HHMM));
}

COLORREF LEDControl::GetTextBackground() const
{
	return _corTextBackground;
}

void LEDControl::SetTextBackground(COLORREF corTextBackground)
{
	_corTextBackground = corTextBackground;
	if((_StyleArray[(int)StyleType::Inner2] & Inner2Style::HandleCreated) != 0)
	{
		::InvalidateRect(m_hWnd, NULL, TRUE);
	}
}

int LEDControl::GetCharWidth()
{
	return m_nCharWidth;
}

void LEDControl::SetCharWidth(int nCharWidth)
{
	m_nCharWidth = nCharWidth;
}

double LEDControl::GetMarginPercentOfChar()
{
	return m_nMarginPercentOfChar;
}

void LEDControl::SetMarginPercentOfChar(double nMarginPercentOfChar)
{
	m_nMarginPercentOfChar = nMarginPercentOfChar;
}

void LEDControl::OnCreateControl()
{
	::SetTimer(m_hWnd, LED_TIMERID_UPDATE, 100, NULL);
}

//cannot use OuterStyle::AutoSize,here do nothing
RECT LEDControl::CalcuteRect(bool updateRect, int nExpectX, int nExpectY, int nExpectCX, int nExpectCY, bool isFourceCalcute)
{
	return _Rect;
}

void LEDControl::OnTimerEnd(EventArgs* e)
{
	TimerEnd.Invoke((Object*)this, e);
}

void LEDControl::OnInnerTimer(EventArgs* e)
{
	//e->WParam is ID
	if(e->WParam != LED_TIMERID_UPDATE)
	{
		Control::OnInnerTimer(e);
		return;
	}

	if(CC_BIT_IS(m_nFlag, CC_LED_START) || CC_BIT_IS(m_nFlag, CC_LED_REDRAW))
	{
		//cc::Str str = time.Format(_T("%Y.%m.%d %H:%M:%S"));
		cc::Str str;
		if(m_nTimerSec >= 0) {
			if(m_nTimerSec > 0) {
				int h = (int)(m_nTimerSec/3600);
				int m = (int)((m_nTimerSec - h*3600)/60);
				int s = (int)(m_nTimerSec - h*3600 - m*60);
				int sss = (int)((m_nTimerSec - h*3600 - m*60 - s)*10);
				str.Format(_T("%02d:%02d:%02d.%01d"), h, m, s, sss);
				m_nTimerSec -= 0.1;
				if(m_nTimerSec <= 0) {
					OnTimerEnd(NULL);
					m_nTimerSec = 0;
				}
			}
			else {
				str.ValueOf(_T("00:00:00.0"));
			}
		}
		else {
			Time time = Time::GetCurrentTime();
			if(CC_BIT_IS(m_nFlag, CC_LED_HHMM)) {
				if(time.GetSecond() % 2 == 0)
				{
					str = time.Format(_T("%H %M"));
				}
				else
				{
					str = time.Format(_T("%H:%M"));
				}
			}
			else {
				str = time.Format(_T("%H:%M:%S"));
			}
		}
		if(!str.Equals(_Text)) {
			SetText(str);
			if(m_hWnd != NULL)
			{
				::InvalidateRect(m_hWnd, NULL, FALSE);
			}
		}
		CC_BIT_SET(m_nFlag, CC_LED_REDRAW, false);
	}
}

#define LEDCONTROL_PENWIDTH 4
void LEDControl::OnPaint(PaintEventArgs *e)
{
	int nLen = cc::Str::Len(_Text);
	if(nLen < 1)
	{
		return;
	}
	HDC hDC = BeginPaint(m_hWnd, &(e->data));
	if(e->data.rcPaint.bottom != 0 && e->data.rcPaint.right != 0)
	{
		// set bkcolor
		RECT rect;
		::GetClientRect(m_hWnd, &rect);
		if(!this->HasStyle(StyleType::Inner, InnerStyle::BackgroundGradient)
			&& (!this->HasStyle(StyleType::Outer, OuterStyle::TransparentParentErase)
			|| (this->GetIsMouseOn() && this->HasStyle(StyleType::Outer, OuterStyle::TransparentOffMouseOn))))
		{
			if(this->GetIsMouseOn())
			{
				//COLORREF back = isMouseOn ? this->GetHotBackColor() : this->GetBackground();
				::SetBkColor(hDC, this->GetHotBackColor());
				::ExtTextOut(hDC, 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL);
			}
			else
			{
				HBRUSH hbr = this->GetBackgroundBrush();
				::FillRect(hDC, &rect, hbr);
			}
		}
		else
		{
			::SendMessage(this->m_hWnd, WM_ERASEBKGND, (WPARAM)hDC, (LPARAM)this);
		}

		double nCharWidth = m_nCharWidth;
		double nLen2 = 0;
		if(nCharWidth <= 0)
		{
			for(int n = 0; n < nLen; n++)
			{
				int nInd = 11;
				//if char is not in "0123456789:",then draw as ".",include ".".
				if(_Text[n] >= '0' && _Text[n] <= ':')
				{
					nInd = (_Text[n] - '0');
				}
				else if(_Text[n] == ' ')
				{
					nInd = 12;
				}
				if(nInd <= 9)
				{
					nLen2 += 1;
				}
				else
				{
					nLen2 += 0.5;
				}
			}
			nCharWidth = (rect.right - _Margin.left - _Margin.right - 2) / nLen2;
		}
		double m_nCharMargin = m_nMarginPercentOfChar * nCharWidth;
		if(m_nCharMargin < 1.0) {
			m_nCharMargin = 1.0;
		}
		nCharWidth -= m_nCharMargin;
		double nLeft = _Margin.left + 1;
		int nH = (rect.bottom - _Margin.top - _Margin.bottom) / 2 - 1;
		int nPW = (int)(nCharWidth * 0.32) < 1.0 ? 1 : (int)(nCharWidth * 0.32);
		nCharWidth = CC_MAX(5, nCharWidth);
		int nPH = (int)(nH * 0.32) < 1.0 ? 1 : (int)(nH * 0.32);
		int nPWH = CC_MIN(nPW, nPH);
		if(nLen2 != 0) {
			nCharWidth -= (nPWH/nLen2);
		}
		HPEN hPenH = CreatePen(PS_SOLID|PS_ENDCAP_ROUND, nPWH, _corTextground);
		HPEN hPenV = CreatePen(PS_SOLID|PS_ENDCAP_ROUND, nPWH, _corTextground);
		//COLORREF backText = RGB(GetRValue(_corTextground)/3, GetGValue(_corTextground)/3, GetBValue(_corTextground)/3);
		HPEN hPenBackH = CreatePen(PS_SOLID|PS_ENDCAP_ROUND, nPWH, _corTextBackground);
		HPEN hPenBackV = CreatePen(PS_SOLID|PS_ENDCAP_ROUND, nPWH, _corTextBackground);
		HPEN hPenOld = (HPEN)SelectObject(hDC, hPenH);
		for(int n = 0; nLeft <= rect.right && n < nLen; n++)
		{
			int nInd = 11;
			//if char is not in "0123456789:",then draw as ".",include ".".
			if(_Text[n] >= '0' && _Text[n] <= ':')
			{
				nInd = (_Text[n] - '0');
			}
			else if(_Text[n] == ' ')
			{
				nInd = 12;
			}
			if(n == nLen - 1 && nLeft + nCharWidth > rect.right) {
				nLeft = rect.right - nCharWidth;
			}
			if(nInd <= 9 && _corTextBackground != _corBackground)
			{
				//draw back rect
				OnDrawNumber(hDC, nPWH, hPenBackH, hPenBackV, LEDCONTROL_RECTALL, (int)nLeft, _Margin.top, nH, (int)nCharWidth);
			}
			OnDrawNumber(hDC, nPWH, hPenH, hPenV, g_LEDControlFlag[nInd], (int)nLeft, _Margin.top, nH, (int)nCharWidth);
			if(nInd <= 9)
			{
				nLeft += nCharWidth + m_nCharMargin;
			}
			else
			{
				nLeft += nCharWidth/2 + m_nCharMargin;
			}
		}
		DeleteObject(hPenH);
		DeleteObject(hPenV);
		DeleteObject(hPenBackH);
		DeleteObject(hPenBackV);
		SelectObject(hDC, hPenOld);
	}
	EndPaint(m_hWnd, &(e->data));
}

void LEDControl::OnDrawNumber(HDC hDC, int nPWH, HPEN hPenH, HPEN hPenV, DWORD dwPartFlag, int nLeft, int nTop, int nH, int nCharWidth)
{
	int half = (int)(nPWH/2);
	if((dwPartFlag & LEDCONTROL_RECT0) != 0)
	{
		SelectObject(hDC, hPenH);
		MoveToEx(hDC, nLeft + half, nTop + half, 0);
		LineTo(hDC, nLeft - half + nCharWidth, nTop + half);
	}
	if((dwPartFlag & LEDCONTROL_RECT1) != 0)
	{
		SelectObject(hDC, hPenV);
		MoveToEx(hDC, nLeft - half + nCharWidth, nTop + half, 0);
		LineTo(hDC, nLeft - half + nCharWidth, nTop + nH);
	}
	if((dwPartFlag & LEDCONTROL_RECT2) != 0)
	{
		SelectObject(hDC, hPenV);
		MoveToEx(hDC, nLeft - half + nCharWidth, nTop + nH, 0);
		LineTo(hDC, nLeft - half + nCharWidth, nTop + nH + nH - half);
	}
	if((dwPartFlag & LEDCONTROL_RECT3) != 0)
	{
		SelectObject(hDC, hPenH);
		MoveToEx(hDC, nLeft + half, nTop + nH + nH - half, 0);
		LineTo(hDC, nLeft - half + nCharWidth, nTop + nH + nH - half);
	}
	if((dwPartFlag & LEDCONTROL_RECT4) != 0)
	{
		SelectObject(hDC, hPenV);
		MoveToEx(hDC, nLeft + half, nTop + nH, 0);
		LineTo(hDC, nLeft + half, nTop + nH + nH - half);
	}
	if((dwPartFlag & LEDCONTROL_RECT5) != 0)
	{
		SelectObject(hDC, hPenV);
		MoveToEx(hDC, nLeft + half, nTop + half, 0);
		LineTo(hDC, nLeft + half, nTop + nH);
	}
	if((dwPartFlag & LEDCONTROL_RECT6) != 0)
	{
		SelectObject(hDC, hPenH);
		MoveToEx(hDC, nLeft + half, nTop + nH, 0);
		LineTo(hDC, nLeft - half + nCharWidth, nTop + nH);
	}
	if((dwPartFlag & LEDCONTROL_RECT7) != 0)
	{
		SelectObject(hDC, hPenH);
		MoveToEx(hDC, nLeft, nTop + nH/2, 0);
		LineTo(hDC, nLeft + nCharWidth/2, nTop + nH/2);
	}
	if((dwPartFlag & LEDCONTROL_RECT8) != 0)
	{
		SelectObject(hDC, hPenH);
		MoveToEx(hDC, nLeft, nTop + nH + nH/2, 0);
		LineTo(hDC, nLeft + nCharWidth/2, nTop + nH + nH/2);
	}
	if((dwPartFlag & LEDCONTROL_RECT9) != 0)
	{
		SelectObject(hDC, hPenH);
		MoveToEx(hDC, nLeft + nCharWidth/4, nTop + nH + nH/2, 0);
		LineTo(hDC, nLeft + nCharWidth/2 - nCharWidth/4, nTop + nH + nH/2);
	}
}
//===========================================================cc::win::LEDControl
