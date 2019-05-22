// cc.win.Form.h
// The C# like Form class
// Author: Shu.KK
// 2006-01-01
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef CC_WIN_MODULE_H_
#define CC_WIN_MODULE_H_

#include "cc.win.Socket.h"
#include "cc.win.Form.h"
#include "..\ccExt\AES.h"

//DeviceIoControl
#include <winioctl.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32")//★：[ws2_32.lib]ライブラリをリンクする

//get mac address
#include <Iphlpapi.h>

namespace cc
{
namespace win
{

class VersionInfo;
class PathButton;
class PickColorCtl;
class PickPercentCtl;
class ComboMenu;
class ComboList;

//Can draw text, others all is same to Control
class ControlContainer : public Control
{
public:
	ControlContainer();
	virtual ~ControlContainer();

protected:
	virtual void OnPaint(PaintEventArgs *e);
};

typedef struct tagPrintEventArgs : tagCancelEventArgs
{
	tagPrintEventArgs(EventArgs* baseArgs) : tagCancelEventArgs(baseArgs)
	{
	};
} PrintEventArgs;
CC_DEF_EVENTHANDLER(PrintEvent)
typedef void (Object::* PrintEventFun)(Object* sender, PrintEventArgs* e);

typedef struct tagQueryPageSettingsEventArgs : tagPrintEventArgs
{
	tagQueryPageSettingsEventArgs(EventArgs* baseArgs) : tagPrintEventArgs(baseArgs)
	{
	};
} QueryPageSettingsEventArgs;
CC_DEF_EVENTHANDLER(QueryPageSettingsEvent)
typedef void (Object::* QueryPageSettingsEventFun)(Object* sender, QueryPageSettingsEventArgs* e);

typedef struct tagPrintPageEventArgs
{
	EventArgs* baseArgs;
	//LPMEASUREITEMSTRUCT data;
	HDC hDC;
	bool Cancel;
	bool HasMorePages;
	RECT MarginBounds;
	RECT PageBounds;
	tagPrintPageEventArgs(EventArgs* baseArgs)
	{
		this->baseArgs = baseArgs;
		//data = NULL;
		hDC = NULL;
		Cancel = false;
		HasMorePages = false;
	};
} PrintPageEventArgs;
CC_DEF_EVENTHANDLER(PrintPageEvent)
typedef void (Object::* PrintPageEventFun)(Object* sender, PrintPageEventArgs* e);

class PrintDocument;
class PrintController;
class PrintController : public Object
{
public:
	PrintController(){};
	//virtual ~PrintController(){};

	// Event that is emitted at the end of a page.
	virtual bool IsPreview()
	{
		return false;
	};

	// Event that is emitted at the end of a page.
	virtual void OnEndPage(PrintDocument* document, PrintPageEventArgs* e)
	{
		// Nothing to do here.
	};

	// Event that is emitted at the end of the print process.
	virtual void OnEndPrint(PrintDocument* document, PrintEventArgs* e)
	{
		// Nothing to do here.
	};

	// Event that is emitted at the start of a page.
	virtual HDC OnStartPage(PrintDocument* document, PrintPageEventArgs* e)
	{
		// Nothing to do here.
		return NULL;
	};

	// Event that is emitted at the start of the print process.
	virtual void OnStartPrint(PrintDocument* document, PrintEventArgs* e)
	{
		// Nothing to do here.
	};
};

class PrintDocument : public Object
{
public:
	PrintEventHandler BeginPrint;
	PrintEventHandler EndPrint;
	PrintPageEventHandler PrintPage;
	// Event that is emitted to query page settings prior to printing a page.
	QueryPageSettingsEventHandler QueryPageSettings;

public:
	PrintDocument();
	virtual ~PrintDocument();

	//void SetDefaultPageSettings(PageSettings defaultPageSettings);
	//PageSettings GetDefaultPageSettings();
	//void SetDocumentName(const TCHAR* documentName);
	//const TCHAR* GetDocumentName();
	//void SetOriginAtMargins(bool originAtMargins);
	//bool GetOriginAtMargins();
	void SetPrintController(PrintController* printController);
	PrintController* GetPrintController();
	//void SetPrinterSettings(PrinterSettings* printerSettings);
	//PrinterSettings GetPrinterSettings();

	// Print the document.
	void Print();

protected:
	//PageSettings _eDefaultPageSettings;
	//TCHAR* _pDocumentName;
	//bool _bOriginAtMargins;
	PrintController* _pPrintController;
	//PrinterSettings printerSettings;

	virtual void OnBeginPrint(PrintEventArgs* e);
	virtual void OnEndPrint(PrintEventArgs* e);
	virtual void OnPrintPage(PrintPageEventArgs* e);
	virtual void OnQueryPageSettings(QueryPageSettingsEventArgs* e);
};

class StandardPrintController : public PrintController
{
public:
	StandardPrintController();
	//virtual ~StandardPrintController();
};

class PreviewPrintController : public PrintController
{
public:
	PreviewPrintController();
	//virtual ~PrintPreviewControl();
};

class PrintPreviewControl : public Control
{
public:
	PrintPreviewControl();
	//virtual ~PrintPreviewControl();
};

class PrintPreviewDialog : public Dialog
{
public:
	PrintPreviewDialog();
	//virtual ~PrintPreviewDialog();
};

//class info_DocEvent : public ie_EventHandler
//{
//public:
// 	virtual HRESULT STDMETHODCALLTYPE GetIDsOfNames(REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgDispId)
//	{
//		if(wcscmp(*rgszNames, L"name") == 0)
//		{
//			*rgDispId = 1;
//		}
//		else if(wcscmp(*rgszNames, L"version") == 0)
//		{
//			*rgDispId = 2;
//		}
//		else if(wcscmp(*rgszNames, L"domain") == 0)
//		{
//			*rgDispId = 3;
//		}
//		else if(wcscmp(*rgszNames, L"quit") == 0)
//		{
//			*rgDispId = 10;
//		}
//		else
//		{
//			return S_FALSE;
//		}
//		return S_OK;
//	}
//
//	virtual HRESULT STDMETHODCALLTYPE Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pDispParams, VARIANT *pVarResult, EXCEPINFO *pExcepInfo, UINT *puArgErr)
//	{
//		switch(dispIdMember)
//		{
//		case 1: //name
//			{
//				pVarResult->vt = 8;
//#if defined (_UNICODE)
//				pVarResult->bstrVal = ::SysAllocString(App::GetName());
//#else
//				{
//					cc::Str str = cc::Str(LangCode::UNICODE16, App::GetName());
//					pVarResult->bstrVal = ::SysAllocString((wchar_t*)str.GetBuf());
//				}
//#endif
//			}
//			break;
//		case 2: //version
//			{
//				pVarResult->vt = 8;
//#if defined (_UNICODE)
//				pVarResult->bstrVal = ::SysAllocString(App::GetVersion());
//#else
//				{
//					cc::Str str = cc::Str(LangCode::UNICODE16, App::GetVersion());
//					pVarResult->bstrVal = ::SysAllocString((wchar_t*)str.GetBuf());
//				}
//#endif
//			}
//			break;
//		case 3: //domain
//			{
//				pVarResult->vt = 8;
//				pVarResult->bstrVal = ::SysAllocString(L"uuware.com");
//			}
//			break;
//		case 10: //quit
//			{
//				App::Exit();
//			}
//			break;
//		default:
//			break;
//		}
//		return S_OK;
//	}
//};
//
//#define CC_TIMERID_VERSION CC_TIMERID_CCPROJ_INNER + 1
//#define CC_VERINFO_URL L"http://www.uuware.com/"
//#define CC_VERINFO_URL2 L"http://uuware.googlepages.com/"
//#define CC_VERINFO_URL3 L"http://uuware.spaces.live.com/"
//#define CC_VERINFO_URLS "[#CLIENT_S#]"
//#define CC_VERINFO_URLE "[#CLIENT_E#]"
//#define CC_VERINFO_SELF "[#CLIENT_SELF#]"
////if more than 4 hours from last run,then show verinfo
//#define CC_VERINFO_SKIPHOUR 4
//class VersionInfoXX : public GroupPanel
//{
//public:
//	//Constructors
//	VersionInfoXX(bool isGroup = true);
//	//destructor
//	virtual ~VersionInfoXX();
//
//protected:
//	CheckBox* chkAutoRemove;
//	Button* btnReShow;
//	IEControl* ie;
//	int nStep;
//	int nURLIndex;
//	cc::Str strURL;
//
//	virtual void OnInnerTimer(EventArgs *e);
//	virtual void OnCreateControl();
//	//keyArr is param address,but sThisHead is by value
//	virtual void DoSetLang(KeyArr& keyArr, cc::Str sThisHead);
//
//	virtual void chkAutoRemove_Click(Object* sender, EventArgs* e);
//	virtual void btnReShow_Click(Object* sender, EventArgs* e);
//};

#define HTML_TAG_BEGIN 0
#define HTML_TAG_DIV 1
#define HTML_TAG_FONT 2
#define HTML_TAG_SPAN 3
#define HTML_TAG_A 4
#define HTML_TAG_CENTER 5
typedef struct tagHtmlInfo
{
	int nTag;
	int nSize;
	int nColor;
	int nBColor;
	cc::Str sURL;
	tagHtmlInfo()
	{
		nTag = -1;
		nSize = -1;
		nColor = CLR_INVALID;
		nBColor = CLR_INVALID;
		sURL = cc::Str(LangCode::SYSTEM);
	};
}HtmlInfo;

class RTFCreaterConfig : public ObjectConfig
{
public:
	static char* sLinkMarkStart;
	static char* sLinkMarkUrl;
	static char* sLinkMarkEnd;
};
//next is special comment for this system:
//<!-- [#~RTF_COMM:S#] --> any thing <!-- [#~RTF_COMM:E#] -->
class RTFCreater : public Object
{
public:
	cc::Str m_sText;

	//Constructors
	RTFCreater();
	//destructor
	virtual ~RTFCreater();

	UINT GetDefaultSize();
	void SetDefaultSize(UINT nSize);
	void ClearList();
	//const List<void*>* GetLinkList();
	void CreaterStart();
	void CreaterEnd(bool isHtml);
	int AddText(const char* html, int nStart, int nEnd, int nColor, int nBColor, int nSize);
	void AddLinkStart(const char* link);
	void AddLinkEnd();

	void LoadHtml(const char* html, int nLen = -1, LangCode::Type lang = LangCode::UTF8);

protected:
	List<void*>* hListInfo;
	//List<void*>* hListLink;
	cc::Str m_sColorTable;
	HtmlInfo* pInfoTmp;
	HtmlInfo* pInfoCur;
	//while LoadHtml,the len of html
	int nHtmlLen;
	int nDefaultSize;
	int nCurSize;
	int nCurColor;
	int nCurBColor;
	cc::Str sCurAlign;
	cc::Str sCurHref;
	int nLastSize;
	int nLastColor;
	int nLastBColor;
	//cc::Str sLastAlign;
	bool lastIsRTFKey;
	int nLinkCount;

	HtmlInfo* PushInfo(int nTag);
	bool PopInfo(int nTag);
	int AddColor(int nColor);
	int HtmlNextChar(const char* html, int nStart, char ch);
	int HtmlNextWord(const char* html, int nStart, cc::Str& sRet);
	int HtmlNextStyle(const char* html, int nStart, cc::Str& sRet);
	int HtmlStyle(const char* html, int nStart, int nEnd);
	int HtmlProperty(const char* html, int nStart, bool isFont, bool isHref);
	bool HtmlTag(const char* html, int& nStart, const char* tcKey, int nKeyLen, bool isTagEnd, int& nContent, const char* tcAdd, int nTag = -1, bool isNewLine = false);
	bool HtmlSkip(const char* html, int& nStart, int& nContent, const char* tcKey, int nKeyLen, const char* tcKey2, int nKeyLen2);
};

#define CC_FLAG_HTMLTEXT_END				CC_FLAG_RICHTEXTBOX_END
class HtmlTextBoxConfig : public ObjectConfig
{
public:
	//url startswith this, then donothing while click but ClickLink.Invoke
	static char* sSkipURL;
};
class HtmlTextBox : public RichTextBox
{
public:
	EventHandler ClickLink;

public:
	//Constructors
	HtmlTextBox();
	//destructor
	virtual ~HtmlTextBox();

	void LoadHtml(const char* html, int nLen = -1, LangCode::Type lang = LangCode::UTF8);
	//here will treate links in rtf
	void LoadRtf(const char* rtf, int nLen = -1);

protected:
	List<void*>* hListLink;

	HtmlInfo* AddLink(const char* url);
	void ClearList();
	void MarkLink();
	virtual void OnMouseLeave(EventArgs* e);
	virtual void OnMouseHover(EventArgs* e);
	virtual void OnMouseUp(MouseEventArgs *e);
	//virtual void OnChildNotify(EventArgs *e);
};

#define CC_TIMERID_VERSION CC_TIMERID_CCPROJ_INNER + 1
#define CC_VERINFO_MAXREDIRECT 6
#define CC_VERINFO_STEP_COUNT 300
#define CC_VERINFO_STEP_END 400
class VersionInfoConfig : public ObjectConfig
{
public:
	static char* sURL1;
	static char* sURL2;
	static char* sURL3;
	//if cannot download any page, then load this.
	static char* sDefaultHtml;
	//default:[#VERSION_URL:%s%s#],search url is start and end with this mark.
	// 1:search target page's url
	//   first %s is appName, second %s is langname like en-US,ja-JP.
	//   sample:[#VERSION_URL:en-US#]http://www.uuware.com/client_en2.htm[#VERSION_URL:en-US#]
	// 2:search "count url". "count url" is put in target page.
	//   [#VERSION_URL:ACCESSCOUNT#],if more than one url,then use [#BREAK#] to split.
	//   sample:<!-- [#VERSION_URL:ACCESSCOUNT#][#BREAK#]http://www.colossal.jp/getstats.php?w=[#C:W#]&h=[#C:H#]&c=32&k=0&f=&NS_url=[#C:URL#]&t=&sid=1123423[#BREAK#]http://s4.histats.com/stats/0.php?65520&11&1&1&4&1371&72007003&2&0&&1000&0&0&0&0&ja&[#C:W#]&[#C:URL#]&[#BREAK#][#VERSION_URL:ACCESSCOUNT#] -->
	static char* sURLFlag;
	//if page include this mark then is last(target) page. default:[#VERSION_SELF#]
	static char* sSelfFlag;
	//if more than nSkipHour hours from last run,then show verinfo
	static int nSkipHour;
};
class VersionInfo : public GroupPanel
{
public:
	//Constructors
	VersionInfo();
	//destructor
	virtual ~VersionInfo();

protected:
	CheckBox* chkAutoRemove;
	Button* btnReShow;
	HtmlTextBox* html;
	WinInet* winInet;
	int nStep;
	int nReDirectTry;
	cc::Str strURLNext;
	cc::Str strURLEnd;
	cc::Str strURLCountAll;
	cc::Str strURLCountOne;

	virtual void OnCreateControl();
	//keyArr is param address,but sThisHead is by value
	virtual void DoSetLang(KeyArr& keyArr, cc::Str sThisHead);

	virtual void OnInnerTimer(EventArgs *e);
	int GetBestUrl(cc::Str& sLocation, const cc::Str& str, const cc::Str& id, bool isAddExt);
	void Control_Click(Object* sender, EventArgs* e);
};

class PathButton : public Button
{
public:
	//Constructors
	PathButton();
	//destructor
	virtual ~PathButton();

	bool IsFile;
	//only valid while IsFile
	bool IsSaveAs;
	bool IsOpenMuitl;
	Control* PathControl;
	const TCHAR* tcCaption;
	const TCHAR* tcFilter;
	const TCHAR* tcDefaultExt;
	cc::Str SelectedPath;

protected:
	virtual void OnClick(EventArgs *e);
};

class WinUtil : public Object
{
public:
	//add lang item(res and files) to ComboBox
	static int AddLangItems(ComboBox* ctl, bool searchRes = true, bool searchFile = true);
	//get lang item from ComboBox and set to CC_APP_GetIni()
	static bool SetLangToConfig(ComboBox* ctl);
	static bool PrintHtml(cc::Str& sHtml, Form* parent = NULL);

	static bool RegistSave(const TCHAR* sPrgRegDir, const char* sREG_KEY1, const char* sREG_KEY2);
	static bool RegistLoad(const TCHAR* sPrgRegDir, const char* sREG_KEY1, cc::Str& sRegCryptData, const char* sKey, cc::Str& sKeyValue, const char* sKey2 = NULL, cc::Str& sKeyValue2 = cc::Str());
	static bool RegistGetValue(const char* sREG_KEY1, const cc::Str& sRegCryptData, const char* sKey, cc::Str& sKeyValue, const char* sKey2 = NULL, cc::Str& sKeyValue2 = cc::Str());
	static bool RegistInput(cc::Str& sInputData, const char* sREG_KEY1);
	//return 20080908, newst(biggst) of bios or system regist.
	static bool RegistHasData(cc::Str& sTime, cc::Str& sRegNow1, cc::Str& sRegNow2, const char* sREG_KEY1, int nBios, int nFile, cc::Str& sNowCrypt);
	static int RegistExpired(const TCHAR* sPrgRegDir, const char* sREG_KEY1);

	static cc::Str GetIp(cc::Str sSkipIp_Pre);
	static cc::Str GetMAC(cc::Str sSkipIp_Pre, bool isWithIpAddress = false);
};

//while _nVHeadWidth <= 0,the height of _pToolPriv or _pToolNext...
#define TABEX_TOOLITEM_VHEIGHT 13
//the default height of head
#define TABEX_DEFAULT_HEADHEIGHT 20
//if page has icon,when draw head the space between icon and text
#define TABEX_SPLITE_ICON_TEXT 2
//space between head and icon,if have head right icon
#define TABEX_SPLITE_HEAD_RICON 10
//space between right buttons,or space between toolitem of Prev,Next...
#define TABEX_SPLITE_HEADBUTTON 0
//space between head and rightbuttons,if have rightbutton.
#define TABEX_SPLITE_HEAD_RBUTTON 0
//if total of all page head width is long than tab head width,the longger head will has this width
#define TABEX_DEFAULT_HEAD_BESTWIDTH 60
//the min widht of pagehead
#define TABEX_PAGEHEAD_MINWIDTH 10

//margin of head while draw
#define TABEX_PAGEHEAD_MARGINL 6
#define TABEX_PAGEHEAD_MARGINT 3
#define TABEX_PAGEHEAD_MARGINR 6
#define TABEX_PAGEHEAD_MARGINB 1


//for control
typedef struct tagTabStyle
{
	enum Type
	{
		//Update?? is set befor call DoUpdate()
		//or UpdateFlag is set for update interface for ownerdraw,ex:tabcontrol
		PrevShowLeft				= 0x0001,
		PrevShowRight				= 0x0002,
		PrevShowMenu				= 0x0004,
		NextShowLeft				= 0x0008,
		NextShowRight				= 0x0010,
		NextShowMenu				= 0x0020,
		//if TopEndShow,then TopToolItem is left of PrevToolItem and EndToolItem is right of NextToolItem
		//and is show while show prev and next
		TopEndShow					= 0x0040,
		//not draw rect of body
		NoBodyRect					= 0x0080,
		FlatTitle					= 0x0100,
		DUMY9						= 0x0200,
		DUMY8						= 0x0400,
		//if true,then swap the selected page to _nHeadFirstIndex~_nHeadLastIndex
		IsSwapSelectedIndex			= 0x0800,
		DUMY6						= 0x1000,
		DUMY3						= 0x2000,
		DUMY2						= 0x4000,
		CreateItemListMenu			= 0x8000,
	}; // enum UpdateFlag,used 0xFF,FF
}TabStyle;

class TabControlExConfig : public ObjectConfig
{
public:
	BASE_CONTROL_CONFIG_DEF;
	static COLORREF corFlatButtonsHeadBackColor;
	static COLORREF corNoBodyRectColor;
	static int nMarginLeft;
	static int nMarginTop;
	static int nMarginRight;
	static int nMarginBottom;
};
#define CC_FLAG_TABPAGE_END				CC_FLAG_CONTAINERCONTROL_END

//here TabAppearance,while Normal as Normal,
//but while Buttons    :excel sheet's head
//    while FlatButtons:Microsoft Visual Studio's head
class TabControlEx : public Container
{
public:
	EventHandler HeadDoubleClick;
	EventHandler SelectedIndexChanged;
	EventHandler SelectedIndexChanging;
	//e->WParam is the index of clicked icon
	EventHandler HeadIconClick;
	//e->WParam is the index of hotpage,-1 for mouse out of head
	EventHandler HeadHotHit;

protected:
	struct HeadInfo
	{
		//text and icon draw width
		int width;
		//draw rect
		RECT rect;
		POINT ptIcon;
		POINT ptIconAct;
		RECT rectText;
		RECT rectTextAct;
		RECT rectEdge;
		RECT rectEdgeAct;
		RECT rectErase;
		RECT rectEraseAct;
		RECT rectRIcon;
		RECT rectRIconAct;
		//adjust for next right icon
		int nRIconAdjustX;
		int nRIconAdjustY;
		DWORD flagEdge;
		int activeIndex;
		//bool isNeedReCalcute;
		HeadInfo()
		{
			width = 0; 
			activeIndex = 0; 
			//isNeedReCalcute = false; 
			::SetRectEmpty(&rect); 
			flagEdge = 0;
		}
	};

public:
	//Constructors
	TabControlEx();
	//destructor
	virtual ~TabControlEx();
	virtual void Dispose();

	bool HasTabStyle(DWORD style);
	void SetTabStyle(DWORD dwRemove, DWORD dwAdd);
	void AddPage(TabPage *control, int nIndex = -1);
	//show delete TabPage out this scope
	TabPage* RemovePage(int nIndex);
	TabPage *GetPage(TCHAR* const ctlName) const;
	TabPage *GetPage(int nIndex) const;
	int GetTabCount() const;
	void SetSelectedIndex(int selectedIndex);
	int GetSelectedIndex();
	TabPage* GetSelectedTab() const;
	void SetSelectedTab(TabPage* selectedTab);
	void SetHeadContextMenu(ContextMenu* menu);
	ContextMenu* GetHeadContextMenu() const;

	TabAlignment::Type GetTabAlignment() const;
	void SetTabAlignment(TabAlignment::Type alignment);
	TabAppearance::Type GetTabAppearance() const;
	void SetTabAppearance(TabAppearance::Type appearance);

	void AddRightButton(Control* control);
	void ClearRightButton();
	//if isIconShare,then not destroy icon in this class
	//void AddHeadIcon(HICON hIcon, bool needDestroy = false);
	//the imageList is same with "SetIcon(ImageList* imageList, UINT nIconID)" 's imageList.and is only share with one point!
	void AddHeadIcon(ImageList* imageList, int nIconID);
	void ClearHeadIcon();

	//if -1 for Vlist text
	void SetVHeadWidth(int nVHeadWidth);
	int GetVHeadWidth() const;
	void SetBestHeadWidth(int nBestHeadWidth);
	int GetBestHeadWidth() const;
	//this size is include headicon and headrighticon.default is 16x16
	void SetHeadIconSize(int cx, int cy);

protected:
	//height of head for _eAlignment=top or bottom or (left or right with _nVHeadWidth != -1)
	int _nHeadHeight;
	//int _nHeadHeightCur; //copy from _nHeadHeight,changed while style change
	int _nVHeadWidth;
	int _nBestHeadWidth;
	//int _nVHeadWidthCur; //copy from _nHeadHeight,changed while style change
	int _nSelectedIndex;
	int _nHeadFirstIndex;
	int _nHeadLastIndex;
	int _nLastSelectedIndex;
	int _nLastActiveIndex;
	RECT _rectBody;
	RECT _rectPage;
	RECT _rectHead;
	SIZE _sizeHeadIcon;
	//only for mouseon index
	int m_nHotIndex;
	//only valid while FlatButtons(Visual Studio's head)
	COLORREF _corHeadBackColor;
	TabAlignment::Type _eAlignment;
	TabAppearance::Type _eAppearance;
	ContextMenu* _pHeadContextMenu;
	ContextMenu* _pItemListMenu;

	DWORD _TabStyle;
	List<Control*>* _lstRightCtls;
	//List<void*>* _lstHeadIcons;
	List<int>* _lstHeadIconsID;
	ToolItem* _pToolPriv;
	ToolItem* _pToolNext;
	ToolItem* _pToolTop;
	ToolItem* _pToolEnd;

	//keyArr is param addres,but sHead is by value
	virtual void DoConfig(bool isFromIni, KeyArr& keyArr, cc::Str sThisHead);
	virtual void OnCreateControl();
	void AddItemListMenu(TabPage *control, int nIndex);
	virtual void DoUpdate();
	virtual void OnPaint(PaintEventArgs *e);
	virtual void OnResize(EventArgs *e);
	virtual void OnSelectedIndexChanged(EventArgs *e);
// Overridables (for owner draw only)
	//virtual void DrawItem(DrawItemEventArgs* e);
	virtual void DrawHead(TabPage *page, HDC hDC, bool isActive, bool isMouseOn, bool isEdge);
	virtual void DrawHeadIcon(TabPage *page, HDC hDC, POINT ptMouse);
	virtual void SetHit(int index, HDC hDC, bool isMouseOn, bool isEdge);
	virtual void SetHotHit(int index, bool isMouseOn, bool isEdge);

	virtual void OnMouseLeave(EventArgs* e);
	virtual void OnMouseMove(MouseEventArgs* e);
	virtual void OnMouseDown(MouseEventArgs *e);
	virtual void OnContextMenu(MouseEventArgs *e);
	virtual void OnDoubleClick(EventArgs *e);
	virtual void OnHeadIconClick(EventArgs *e);

	virtual void OnStyleChanged(bool isAddItemListMenu);
	//nMustShowIndex is should show while calcute head rect
	virtual void CalcuteHeadInfo(int nMustShowIndex = -1);
	//return pos of right
	virtual int CalcuteHeadInfoRButton(bool isShow, int nPosLeft, int nPosTop, int nPosRight, int nHeight);
	virtual void CalcuteHeadWidth(TabPage *control, int nIndex);
	virtual void CalcuteHeadActiveIndex(TabPage *control);
	//AdjustRect _rectBody&_rectHead while resize or Alignment changed
	virtual void AdjustRect();
	//virtual void GetItemRect(int nIndex, RECT &rect);
	int PointToIndex(POINT point) const;
	virtual int DoInsPage(TabPage *control, int nIndex = -1);
	virtual int DoModPage(TabPage *control, int nIndex = -1);

	virtual void HeadDropMenu_OnClick(Object* sender, EventArgs *e);
	virtual void PrivTool_OnClick(Object* sender, EventArgs *e);
	virtual void NextTool_OnClick(Object* sender, EventArgs *e);
	virtual void TopTool_OnClick(Object* sender, EventArgs *e);
	virtual void EndTool_OnClick(Object* sender, EventArgs *e);

private:
	//hide this
	void AddControl(Control* control, int nIndex = -1){};
	Control* GetControl(TCHAR* const ctlName) const{return Container::GetControl(ctlName);};
	Control* GetControl(int nIndex) const{return Container::GetControl(nIndex);};
};

class TreeViewExConfig : public TreeViewConfig
{
public:
	BASE_CONTROL_CONFIG_DEF;
	static COLORREF corTextgroundDisabled;
};

struct CC_TREEVIEWEX_DATA
{
public:
	//m_nChecks(multi checkbox,max is 4*8/2=16)
	//0000 0000 0000 0000
	//                  |-----------> index 0 for Item Enabled/Disabled(CC_ACCESS_ITEM_DISIABLED)
	//                 |-----------> index 1 for Checkbox Enabled/Disabled(CC_ACCESS_CHKBOX_DISIABLED)
	UINT m_nFlags;
	COLORREF corTextground;
	COLORREF corHotTextColor;

	CC_TREEVIEWEX_DATA()
	{
		m_nFlags = 0;
		//will init while NewItemData
		corTextground = TreeViewExConfig::corTextground;
		corHotTextColor = TreeViewExConfig::corHotTextColor;
	};
};

//#define CC_FLAG_TREEVIEWEX_NOCUSTOMDRAW		CC_FLAG_TREEVIEW_END
#define CC_FLAG_TREEVIEWEX_END				CC_FLAG_TREEVIEW_END
class TreeViewEx : public TreeView
{
public:
	TreeViewEx();
	virtual ~TreeViewEx();

	//virtual void* GetItemDataPtr(HTREEITEM hItem);
	//virtual BOOL SetItemDataPtr(HTREEITEM hItem, void* pData);
	void SetItemEnabled(HTREEITEM hItem, bool bEnabled = true);
	bool GetItemEnabled(HTREEITEM hItem);
	void SetItemTextColor(HTREEITEM hItem, COLORREF clrNew);
	COLORREF GetItemTextColor(HTREEITEM hItem);
	void SetItemHotTextColor(HTREEITEM hItem, COLORREF clrNew);
	COLORREF GetItemHotTextColor(HTREEITEM hItem);
	void SetCheckStyle(CheckStyle::Type eStyle);
	CheckStyle::Type GetCheckStyle();
	//can show checobox for single ons and is different to others
	void SetItemCheckBox(HTREEITEM hItem, bool bShowBox);
	bool GetItemCheckBox(HTREEITEM hItem);
	void SetCheckboxEnabled(HTREEITEM hItem, bool bEnabled = true);
	bool GetCheckboxEnabled(HTREEITEM hItem);

protected:
	//used in OnCustomDraw
	HIMAGELIST m_hImgList;
	CheckStyle::Type m_nStyle;
	Map<int, void*>* _mapData;

	void OnItemCheckChangedLoopChild(HTREEITEM hItem, int nChecked);
	void OnItemCheckChangedLoopParent(HTREEITEM hItem, int nChecked);
	virtual int GetCustomDataIndex(HTREEITEM hItem, bool isCreate);
	virtual void OnDeleteItem(EventArgs *e);
	virtual void OnCustomDraw(EventArgs *e);
	virtual void OnItemCheckChanged(EventArgs *e);
	virtual void OnSelectedIndexChanging(EventArgs *e);
	//virtual void OnMouseDown(MouseEventArgs *e);
	//for child class to overwrite to control item disabled or checkbox disabled
	virtual DWORD GetAccessFlag(HTREEITEM hItem);

public:
};
/*
class MoveFormCtl : public ToolItem
{
public:
	MoveFormCtl();
	virtual ~MoveFormCtl();

	void SetMoveForm(Form* form);

protected:
	Form* moveForm;
	virtual void OnMouseDown(MouseEventArgs *e);
};*/

#define CC_PICKCOLOR_BACKGROUND 0x0002
#define CC_PICKCOLOR_TEXTCOLOR 0x0004
#define CC_PICKCOLOR_HOTBACKCOLOR 0x0008
#define CC_PICKCOLOR_HOTTEXTCOLOR 0x0010
#define CC_FLAG_PICKCOLOR_MSTART				CC_FLAG_TOOLITEM_END
#define CC_FLAG_PICKCOLOR_END					CC_FLAG_TOOLITEM_END<<1
class PickColorCtl : public ToolItem
{
public:
	EventHandler ValueChanged;

public:
	PickColorCtl();
	virtual ~PickColorCtl();

	void SetBackground(COLORREF corBackColor, bool isInvokeValueChanged = true);
	void SetHotBackColor(COLORREF corHotBackColor, bool isInvokeValueChanged = true);
	void SetResultControl(Control* pResultCtl, DWORD dwResultStyle);

protected:
	Control* m_pResultCtl;
	DWORD m_dwResultStyle;
	int nIndexSave;
	cc::Str sColorOthers;
	int nValueChangedCount;

	//keyArr is param addres,but sHead is by value
	virtual void DoConfig(bool isFromIni, KeyArr& keyArr, cc::Str sThisHead);
	//keyArr is param address,but sThisHead is by value
	virtual void DoSetLang(KeyArr& keyArr, cc::Str sThisHead);
	void OnValueChanged(EventArgs* e, bool isInvokeValueChanged = true);
	virtual void OnClick(EventArgs *e);
	//virtual void OnMouseDown(MouseEventArgs *e);
	virtual void OnMouseMove(MouseEventArgs *e);
	virtual void OnMouseUp(MouseEventArgs *e);
	//virtual void OnMouseUp(MouseEventArgs *e);

	void Pop_PaintEnd(Object* sender, PaintEventArgs* e);
	void Pop_MouseMove(Object* sender, MouseEventArgs* e);
	void Pop_MouseDown(Object* sender, MouseEventArgs* e);
	void Pop_MouseLeave(Object* sender, EventArgs* e);
	void Pop_Deactivate(Object* sender, EventArgs* e);
};

#define CC_FLAG_PICKPERCENTCTL_NEEDCLBK				CC_FLAG_TOOLITEM_END
#define CC_FLAG_PICKPERCENTCTL_END					CC_FLAG_TOOLITEM_END<<1
class PickPercentCtl : public ToolItem
{
public:
	EventHandler ValueChanged;

public:
	PickPercentCtl();
	virtual ~PickPercentCtl();
	virtual void Dispose();

	int GetMinValue();
	int GetMaxValue();
	void SetMinMaxValue(int nMin, int nMax, bool bRedraw = true);
	int SetValue(int nValue, bool bRedraw = true);
	int GetValue();
	void SetResultControl(Control* pResultCtl, const cc::Str sResultFormat = cc::Str(), bool textChangedCallback = false);
	//do what u know
	HScrollBar* GetScrollBar();

protected:
	Control* m_pResultCtl;
	cc::Str m_sResultFormat;
	HScrollBar* hScrollBar;
	int nValueChangedCount;

	//keyArr is param addres,but sHead is by value
	virtual void DoConfig(bool isFromIni, KeyArr& keyArr, cc::Str sThisHead);
	virtual RECT CalcuteRect(bool updateRect = false, int nExpectX = -1, int nExpectY = -1, int nExpectCX = -1, int nExpectCY = -1, bool isFourceCalcute = false);
	virtual void OnValueChanged(EventArgs *e);
	void Pop_Deactivate(Object* sender, EventArgs* e);
	void Pop_ValueChanged(Object* sender, EventArgs* e);
	void ResultCtl_LostFocus(Object* sender, EventArgs* e);
};

class ComboMenu : public ToolItem
{
public:
	EventHandler SelectedIndexChanged;

public:
	ComboMenu();
	virtual ~ComboMenu();

	ContextMenu* GetContextMenu();
	void SetContextMenu(ContextMenu* pContMenu, bool needDestroy);
	void SetSelectedIndex(int selectedIndex);
	int GetSelectedIndex();
	int ItemAdd(const TCHAR* txt, const TCHAR* txtTips = NULL, const TCHAR* txtTipsExt = NULL, int index = -1, ImageList* imageList = NULL, int nIconID = -1, int nIconIDTips = -1, int nIconIDTipsExt = -1);
	void ItemDel(const TCHAR* txt);
	void ItemDel(int index);
	void ItemSet(int index, const TCHAR* txt);
	const TCHAR* ItemGet(int index);
	int ItemCount() const;
	void ItemClear();
	int FindString(int nIndexStart, LPCTSTR lpszFind, bool IgnoreCase = false) const;
	int FindStringExact(int nIndexStart, LPCTSTR lpszFind, bool IgnoreCase = false) const;
	int FindStringOption(int nIndexStart, LPCTSTR lpszFind, int searchType, bool IgnoreCase = false) const;
	//if has more than one item,then if not isAppend do ItemClear
	void SetConfigData(const cc::Str& sData, const cc::Str& sTips, const cc::Str& sTipsExt, bool isAppend = false);

protected:
	int _nSelectedIndex;

	virtual void CreateHandle();
	//keyArr is param addres,but sHead is by value
	virtual void DoConfig(bool isFromIni, KeyArr& keyArr, cc::Str sThisHead);
	//keyArr is param address,but sThisHead is by value
	virtual void DoSetLang(KeyArr& keyArr, cc::Str sThisHead);
	void Pop_SelectedIndexChanged(Object* sender, EventArgs* e);
};

#define CC_FLAG_COMBOLIST_END				CC_FLAG_TOOLITEM_END
class ComboList : public ToolItem
{
public:
	EventHandler SelectedIndexChanged;

public:
	ComboList();
	virtual ~ComboList();
	virtual void Dispose();

	ListBox* GetListBox();
	void SetSelectedIndex(int selectedIndex);
	int GetSelectedIndex();
	int ItemAdd(const TCHAR* txt, int index = -1);
	void ItemDel(const TCHAR* txt);
	void ItemDel(int index);
	void ItemSet(int index, const TCHAR* txt);
	cc::Str ItemGet(int index);
	int ItemCount() const;
	void ItemClear();
	int FindString(int nIndexStart, LPCTSTR lpszFind) const;
	int FindStringExact(int nIndexStart, LPCTSTR lpszFind) const;
	int FindStringOption(int nIndexStart, LPCTSTR lpszFind, bool IgnoreCase = false) const;
	//isAppend is valid while (_pListBox->m_hWnd != NULL)
	void SetConfigData(const cc::Str& sData, bool isAppend = false);
	//minWidth is as same to this's width,and even set width for pop,the real width is max of width and this(self)'s width
	void SetPopSize(int width, int height, bool isResize, int minHeight = -1, int MaxWidth = -1, int maxHeight = -1);

protected:
	SIZE _szPop;
	int _nSelectedIndex;
	ListBox* _pListBox;

	virtual void CreateHandle();
	//keyArr is param addres,but sHead is by value
	virtual void DoConfig(bool isFromIni, KeyArr& keyArr, cc::Str sThisHead);
	//keyArr is param address,but sThisHead is by value
	virtual void DoSetLang(KeyArr& keyArr, cc::Str sThisHead);
	virtual void OnResize(EventArgs *e);
	void Pop_Deactivate(Object* sender, EventArgs* e);
	void Pop_MouseMove(Object* sender, MouseEventArgs* e);
	void Pop_MouseDown(Object* sender, MouseEventArgs* e);
	void Pop_KeyDown(Object* sender, KeyEventArgs* e);
};

/////////////////////////////////////////////////////////////////////////////
// CAMSEdit window

// Class CAMSEdit is the base class for all the other AMS CEdit classes.  
// It provides some base functionality to set and get the text and change
// its text and background color.
//
class CAMSEdit : public TextBox
{
public:
	// Construction/destruction
	CAMSEdit();
	virtual ~CAMSEdit();

	// Operations
	cc::Str GetTrimText();

protected:
	virtual void Redraw();
	virtual cc::Str GetValidText();
	virtual bool ShouldEnter(TCHAR c) const;

protected:
	enum InternalFlags
	{
		None				= 0x0000,
		TextColorHasBeenSet = 0x0001
	};
	UINT m_uInternalFlags;
	// Generated message map functions (for CAMSEdit)
protected:
	virtual void Clear();
	virtual void Paste();
	virtual void Cut();
	virtual void OnTextChanged(EventArgs *e);

public:
	// Class SelectionSaver is used to save an edit box's current
	// selection and then restore it on destruction.
	class SelectionSaver
	{
	public:
		SelectionSaver(TextBox* pEdit);
		SelectionSaver(TextBox* pEdit, int nStart, int nEnd);
		~SelectionSaver();

		void MoveTo(int nStart, int nEnd);
		void MoveBy(int nStart, int nEnd);
		void MoveBy(int nPos);
		void operator+=(int nPos);

		int GetStart() const;
		int GetEnd() const;

		void Update();
		void Disable();

	protected:
		TextBox* m_pEdit;
		int m_nStart;
		int m_nEnd;
	};


};

class InputDateTimeCtl : public Control
{
public:
	InputDateTimeCtl(int nY, int nM, int nD, int nHH, int nMM, int nSS = -1, int nSSS = -1, bool isY = true, bool isM = true, bool isD = true, bool isHH = true, bool isMM = true, bool isSS = true, bool isSSS = false);
	virtual ~InputDateTimeCtl();

protected:
	Label* labY;
	Label* labM;
	Label* labD;
	Label* labHH;
	Label* labMM;
	Label* labSS;
	Label* labSSS;
	TextBox* edtY;
	TextBox* edtM;
	TextBox* edtD;
	TextBox* edtHH;
	TextBox* edtMM;
	TextBox* edtSS;
	TextBox* edtSSS;
	SpinButton* spinY;
	SpinButton* spinM;
	SpinButton* spinD;
	SpinButton* spinHH;
	SpinButton* spinMM;
	SpinButton* spinSS;
	SpinButton* spinSSS;
};

//for control
typedef struct tagCancelStatus
{
	enum Type
	{
		Ready				= 0x0001,
		Runing				= 0x0002,
		Canceling			= 0x0004,
		Canceled			= 0x0008,
	};
}CancelStatus;

class CancelDlg : public Dialog
{
public:
	EventHandler CancelClick;

public:
	CancelDlg(int nDialogWidth = 360, int nDialogHeight = 150, int nIconWidth = 32, int nIconHeight = 32);
	virtual ~CancelDlg();

	bool Start(bool isShowStartMsg = true, bool isShowAlreadyRunMsg = true);
	bool End();
	bool IsCancel(bool isShowCancelMsg = true);
	CancelStatus::Type GetStatus();

	void SetCancelText(const TCHAR* txtCancel, const TCHAR* txtCanceling);
	//if NULL,then not set
	void SetShowText(const TCHAR* txtStartMsg = NULL, const TCHAR* txtAlreadyRunMsg = NULL, const TCHAR* txtCancelMsg = NULL);
	void SetMessage(const TCHAR* txt, ImageList* imageList = NULL, int nIconID = -1);
	void SetTitle(const TCHAR* txt);
	void SetFooter(const TCHAR* txt);
#ifdef _DEBUG_
	static void test(Control* parent);
#endif

protected:
	CancelStatus::Type eStatus;
	bool bEnableParent;
	HWND m_hWndParent;
	cc::Str strCancel;
	cc::Str strCanceling;
	cc::Str strStartMsg;
	cc::Str strAlreadyRunMsg;
	cc::Str strCancelMsg;
	Label* labMsg;
	Button* btnCancel;
	PictureBox* labImg;
	Label* labStatus;

	//keyArr is param address,but sThisHead is by value
	virtual void DoSetLang(KeyArr& keyArr, cc::Str sThisHead);
	void cancel_Click(Object* sender, EventArgs* e);
};

//nBtnFlag:
//#define MB_OK                       0x00000000L
//#define MB_OKCANCEL                 0x00000001L
//#define MB_ABORTRETRYIGNORE         0x00000002L
//#define MB_YESNOCANCEL              0x00000003L
//#define MB_YESNO                    0x00000004L
//#define MB_RETRYCANCEL              0x00000005L

//#define MB_ICONHAND                 0x00000010L //MB_ICONERROR,MB_ICONSTOP
//#define MB_ICONQUESTION             0x00000020L
//#define MB_ICONEXCLAMATION          0x00000030L //MB_ICONWARNING
//#define MB_ICONASTERISK             0x00000040L //MB_ICONINFORMATION

#define CC_BOTTOM_LINE                0x10000000L
#define CC_RIGHT_BUTTON               0x20000000L
#define CC_RICHTEXTBOX                0x40000000L
#define CC_CUSTOMBTN                  0x80000000L
class MsgDlg : public Dialog
{
public:
	MsgDlg(const TCHAR* sMsg, ImageList* imageList = NULL, int nIconID = -1, const TCHAR* sTitle = NULL, int nDialogWidth = 300,
		int nIconWidth = 32, int nIconHeight = 32, const TCHAR* sMsgBeLow = NULL, DWORD nBtnFlag = 0);
	virtual ~MsgDlg();

#ifdef _DEBUG_
	static void test(Control* parent);
#endif

protected:
	//label or RichTextBox
	Control* labMsg;
	Label* labBeLow;
	Button* _btnOK;
	Button* _btnCancel;
	Button* _btnYes;
	Button* _btnNo;
	Button* _btnRetry;
	Button* _btnAbort;
	Button* _btnIgnore;
	int nLeft;
	int nTop;

	Button* AddButton(const TCHAR* name, const TCHAR* txtkey, const TCHAR* txtdef, int nWidth = -1);
	void Control_Click(Object* sender, EventArgs* e);
};

class AboutDlg : public MsgDlg
{
public:
	AboutDlg(const TCHAR* sMsg, ImageList* imageList = NULL, int nIconID = -1, const TCHAR* sTitle = NULL,
		int nDialogWidth = 300, int nIconWidth = 32, int nIconHeight = 32, const TCHAR* sMsgBeLow = NULL,
		bool isLink1 = false, bool isLink2 = false);
	virtual ~AboutDlg();

#ifdef _DEBUG_
	static void test(Control* parent);
#endif
};

class TipsDlg : public MsgDlg
{
public:
	TipsDlg(int nTipsDlgId, const TCHAR* sMsg, ImageList* imageList = NULL, int nIconID = -1, const TCHAR* sTitle = NULL,
		int nDialogWidth = 300, int nIconWidth = 32, int nIconHeight = 32, const TCHAR* sMsgBeLow = NULL,
		const TCHAR* sMsgCheckBox = NULL, DWORD nBtnFlag = MB_OKCANCEL);
	virtual ~TipsDlg();

	static void ShowAllAgain();
	static bool GetIsNotShowAgain(int nTipsDlgId);
#ifdef _DEBUG_
	static void test(Control* parent);
#endif

protected:
	int _nTipsDlgId;
	CheckBox* chkNotShowAgain;
	void chkNotShowAgain_Click(Object* sender, EventArgs* e);
};

class InputDlg : public Dialog
{
public:
	InputDlg(const TCHAR* sTxt = NULL, bool isMultiline = false, const TCHAR* sMsg = NULL,
		const TCHAR* sTitle = NULL, bool isNotBlank = false, int nWinWidth = 240, int nMultilineHeight = 0,
		const TCHAR* sConfirmMsg = NULL);

	void SetInputText(const TCHAR* sTxt, const TCHAR* sMsg);
	cc::Str GetInputText();
#ifdef _DEBUG_
	static void test(Control* parent);
#endif

protected:
	int nIsNotBlank;
	cc::Str sInput;
	cc::Str sConfirmMsg;
	Label* labMsg;
	TextBox* editText;
	Button* btnOK;
	Button* btnCancel;

	void Control_Click(Object* sender, EventArgs* e);
};

class ChangePWDDlg : public Dialog
{
public:
	EventHandler OKClick;
	EventHandler CancelClick;

public:
	ChangePWDDlg(const TCHAR* sPWD = NULL, bool isPWDMayEmpty = false, const TCHAR* sTitle = NULL);
	virtual ~ChangePWDDlg();

#ifdef _DEBUG_
	static void test(Control* parent);
#endif

protected:
	bool _isPWDMayEmpty;
	TCHAR* _tcPWD;
	Label* label1;
	Label* label2;
	Label* label3;
	TextBox* _editText1;
	TextBox* _editText2;
	TextBox* _editText3;
	Button* _btnOK;
	Button* _btnCancel;

	void ok_Click(Object* sender, EventArgs* e);
	void cancel_Click(Object* sender, EventArgs* e);
};

//for control
typedef struct tagOptionStyles
{
	enum Type
	{
		//while StatusItem, equals to Contents
		ShowOK						= 0x00000001,
		ShowCancel					= 0x00000002,
		ShowHelp					= 0x00000004,
		DisabledOK					= 0x00000008,
		DisabledCance				= 0x00000010,
		DisabledHelp				= 0x00000040,
		ShowApply					= 0x00000080,
		ShowDefault					= 0x00000100,
		DisabledApply				= 0x00000200,
		DisabledDefault				= 0x00000400,
		DUMY1						= 0x00000800,
		DUMY2						= 0x00001000,
		DUMY3						= 0x00002000,
		DUMY4						= 0x00004000,
		DUMY5						= 0x00008000,
		DUMY6						= 0x00010000,
		DUMY7						= 0x00020000,
		DUMY8						= 0x00040000,
		DUMY9						= 0x00080000,
	}; // enum OptionStyles
}OptionStyles;

class OptionPage : public TabPage
{
public:
	//Event
	EventHandler OKClick;
	EventHandler CancelClick;
	EventHandler HelpClick;
	EventHandler ApplyClick;
	EventHandler DefaultClick;

public:
	OptionPage();
	virtual ~OptionPage();

	bool IsOptionStyle(OptionStyles::Type optionStyle) const;
	void SetOptionStyle(OptionStyles::Type optionStyle, bool value, bool needUpdate = false);
	bool GetIsDirty() const;
	void SetDirty(bool value);

protected:
	DWORD _OptionStyle;
	int _bIsDirty;
};

class OptionCtl : public Container
{
public:
	//Event
	EventHandler OKClick;
	EventHandler CancelClick;
	EventHandler SelectedIndexChanged;

public:
	OptionCtl();
	virtual ~OptionCtl();

	int GetSelectedIndex();
	void SetSelectedIndex(int selectedIndex);

protected:
	int _nSelectedIndex;
	int _nLastSelectedIndex;

	TreeView* tree;
	Button* btnHelp;
	Button* btnDefault;
	Button* btnApply;
	Button* btnOK;
	Button* btnCancel;
	Label* labTitle;
	Label* labLine1;
	Label* labLine2;
	OptionPage* activePage;

	virtual void btnOK_Click(Object* sender, EventArgs *e);
	virtual void btnCancel_Click(Object* sender, EventArgs *e);
	virtual void btnHelp_Click(Object* sender, EventArgs *e);
	virtual void btnDefault_Click(Object* sender, EventArgs* e);
	virtual void btnApply_Click(Object* sender, EventArgs* e);

	virtual void tree_SelectedIndexChanged(Object* sender, EventArgs *e);

	virtual void CreateHandle();
	virtual void OnResize(EventArgs* e);

	virtual void ResizeOptionPage(OptionPage* page);
	
	//virtual void OnHandleCreated(EventArgs *e);
	//virtual void OnLoad(EventArgs *e);
};

#define CC_CONFIGBACKDLG_DONOTHING 0x0001
//instead of SetConfigName,you should set key of load/save ini at SetResultControl
class ConfigBackDlg : public Dialog
{
public:
	EventHandler OKClick;
	EventHandler CancelClick;

public:
	ConfigBackDlg();
	virtual ~ConfigBackDlg();
	//sThisHead is load/save ini key
	void SetResultControl(Control* pResultCtl, DWORD dwResultStyle, cc::Str sThisHead = cc::Str(), cc::KeyArr* kaSave = NULL);
	//IMAGE_ICON;IMAGE_BITMAP
	void AddInnerImage(UINT nResID, const TCHAR *resType, bool isStretch, const TCHAR* info);
	static void ConfigResult(Control* pResultCtl, DWORD dwResultStyle, cc::Str sThisHead = cc::Str(), cc::KeyArr* kaSave = NULL);

protected:
	Control* m_pResultCtl;
	DWORD m_dwResultStyle;
	cc::KeyArr* m_kaSave;
	cc::Str m_sThisHead;
	cc::Str sInnerImgs;
	Label* label;
	RadioButton* radBack1;
	PickColorCtl* pickColor1;
	ComboBox* combCross;
	RadioButton* radBack2;
	PickColorCtl* pickColor2;
	PickColorCtl* pickColor3;
	ComboBox* combVertical;
	RadioButton* radBack3;
	ComboBox* combInnerBmp;
	RadioButton* radBack4;
	RadioButton* radBack5;
	TextBox* editImg;
	PathButton* btnSelFile;
	ComboBox* combStretch;
	Button* btnOK;
	Button* btnCancel;

	//keyArr is param address,but sThisHead is by value
	//virtual void DoSetLang(KeyArr& keyArr, cc::Str sThisHead);
	virtual void CreateHandle();
	void Control_Click(Object* sender, EventArgs* e);
	void Form_Closed(Object* sender, EventArgs* e);
	//not need call this from outer
	void SetConfigName(const TCHAR* sName){_ConfigName = CC_StrCopy(&_ConfigName, sName);}
};

//     0     
//   *****   
//  *     *  
//5 *     * 1  ** 7
//  *  6  *  
//   *****   
//  *     *  
//4 *     * 2  ** 8  * 9
//  *     *  
//   *****   
//     3     
#define LEDCONTROL_RECT0 0x0001
#define LEDCONTROL_RECT1 0x0002
#define LEDCONTROL_RECT2 0x0004
#define LEDCONTROL_RECT3 0x0008
#define LEDCONTROL_RECT4 0x0010
#define LEDCONTROL_RECT5 0x0020
#define LEDCONTROL_RECT6 0x0040
#define LEDCONTROL_RECT7 0x0100
#define LEDCONTROL_RECT8 0x0200
#define LEDCONTROL_RECT9 0x0400
#define LEDCONTROL_RECTALL (LEDCONTROL_RECT0 | LEDCONTROL_RECT1 | LEDCONTROL_RECT2 | LEDCONTROL_RECT3 | LEDCONTROL_RECT4 | LEDCONTROL_RECT5 | LEDCONTROL_RECT6)

#define CC_LED_START                0x00000001L
#define CC_LED_HHMM                 0x00000002L
#define CC_LED_REDRAW               0x00000004L

#define LED_TIMERID_UPDATE   CC_TIMERID_CCPROJ_INNER + 1
class LEDControlConfig : public ObjectConfig
{
public:
	BASE_CONTROL_CONFIG_DEF;
	static int nMarginLeft;
	static int nMarginTop;
	static int nMarginRight;
	static int nMarginBottom;
};
class LEDControl : public Control
{
public:
	EventHandler TimerEnd;

public:
	//Constructors
	LEDControl();
	//destructor
	virtual ~LEDControl();

	COLORREF GetTextBackground() const;
	virtual void SetTextBackground(COLORREF corTextBackground);
	void SetCharWidth(int nCharWidth);
	int GetCharWidth();
	void SetMarginPercentOfChar(double nMarginPercentOfChar);
	double GetMarginPercentOfChar();
	void SetStart(bool isStart);
	bool GetStart();
	void SetHhmm(bool isHhmm);
	bool GetHhmm();
	void SetTimerSec(int nTimerSec);
	int GetTimerSec();

protected:
	int m_nCharWidth;
	double m_nMarginPercentOfChar;
	int m_nFlag;
	double m_nTimerSec;
	COLORREF _corTextBackground;

	void OnTimerEnd(EventArgs* e);
	virtual void OnCreateControl();
	virtual void OnInnerTimer(EventArgs *e);
	virtual void OnPaint(PaintEventArgs *e);
	virtual RECT CalcuteRect(bool updateRect = false, int nExpectX = -1, int nExpectY = -1, int nExpectCX = -1, int nExpectCY = -1, bool isFourceCalcute = false);
	virtual void OnDrawNumber(HDC hDC, int nPWH, HPEN hPenH, HPEN hPenV, DWORD dwPartFlag, int nLeft, int nTop, int nH, int nCharWidth);
};

} //namespace win
} //namespace cc

#endif //#ifndef CC_WIN_MODULE_H_
