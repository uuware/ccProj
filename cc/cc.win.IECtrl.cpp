// cc.win.IECtrl.h
// The C# like Controls class
// Author: Shu.KK
// 2006-07-01
// add function at 2007-05-01
#include "cc.win.IECtrl.h"
using namespace cc;
using namespace cc::win;

//===========================================================cc::win::ie_ClientSite
ie_ClientSite::ie_ClientSite(HWND hWndParent, ie_InPlaceSite *pInPlaceSite, ie_DocHandler *pDocHandler)
{
	if(pInPlaceSite != NULL)
	{
		m_pInPlaceSite = pInPlaceSite;
	}
	else
	{
		m_pInPlaceSite = new ie_InPlaceSite(hWndParent);
	}
	m_pDocHandler = pDocHandler;
}

ie_ClientSite::~ie_ClientSite()
{
	if(m_pInPlaceSite != NULL)
	{
		delete m_pInPlaceSite;
		m_pInPlaceSite = NULL;
	}
	if(m_pDocHandler != NULL)
	{
		delete m_pDocHandler;
		m_pDocHandler = NULL;
	}
}

STDMETHODIMP ie_ClientSite::QueryInterface(REFIID iid, void ** ppvObject)
{
	if(iid == IID_IUnknown || iid == IID_IOleClientSite)
	{
		*ppvObject = this;
	}
	else if(iid == IID_IOleInPlaceSite)
	{
		*ppvObject = m_pInPlaceSite;
	}
	else if(iid == ie_IDocHostUIHandler && m_pDocHandler != NULL)
	{
		*ppvObject = m_pDocHandler;
	}
	else
	{
		return E_NOINTERFACE;
	}
	return S_OK;
}

ULONG STDMETHODCALLTYPE ie_ClientSite::AddRef()
{
	return 1;
}

ULONG STDMETHODCALLTYPE ie_ClientSite::Release()
{
	return 1;
}

STDMETHODIMP ie_ClientSite::SaveObject()
{
	return E_NOTIMPL;
}

STDMETHODIMP ie_ClientSite::GetMoniker(DWORD dwAssign, DWORD dwWhichMoniker, IMoniker ** ppmk)
{
	return E_NOTIMPL;
}

STDMETHODIMP ie_ClientSite::OnShowWindow(BOOL fShow)
{
	return E_NOTIMPL;
}

STDMETHODIMP ie_ClientSite::RequestNewObjectLayout()
{
	return E_NOTIMPL;
}

STDMETHODIMP ie_ClientSite::GetContainer(LPOLECONTAINER FAR* ppContainer)
{
	return E_NOTIMPL;
}

STDMETHODIMP ie_ClientSite::ShowObject()
{
	return S_OK;
}

ie_DocHandler *ie_ClientSite::getDocHandler()
{
	return m_pDocHandler;
}

//===========================================================cc::win::ie_InPlaceSite
ie_InPlaceSite::ie_InPlaceSite(HWND hWndParent)
{
	m_hWndParent = hWndParent;
}

ie_InPlaceSite::~ie_InPlaceSite()
{
}

//IUnknown
STDMETHODIMP ie_InPlaceSite::QueryInterface(REFIID iid, void ** ppvObject)
{
	return E_NOINTERFACE;
}

ULONG STDMETHODCALLTYPE ie_InPlaceSite::AddRef()
{
	return 1;
}

ULONG STDMETHODCALLTYPE ie_InPlaceSite::Release()
{
	return 1;
}

//IOleWindow
HRESULT STDMETHODCALLTYPE ie_InPlaceSite::GetWindow(HWND * phwnd)
{
	*phwnd = m_hWndParent;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE ie_InPlaceSite::ContextSensitiveHelp(BOOL fEnterMode)
{
	return E_NOTIMPL;
}

//IOleInPlaceSite
HRESULT STDMETHODCALLTYPE ie_InPlaceSite::CanInPlaceActivate(void)
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE ie_InPlaceSite::OnInPlaceActivate(void)
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE ie_InPlaceSite::OnUIActivate(void)
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE ie_InPlaceSite::GetWindowContext(
	IOleInPlaceFrame **ppFrame,
	IOleInPlaceUIWindow **ppDoc,
	LPRECT lprcPosRect, LPRECT lprcClipRect,
	LPOLEINPLACEFRAMEINFO lpFrameInfo)
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE ie_InPlaceSite::Scroll(SIZE scrollExtant)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE ie_InPlaceSite::OnUIDeactivate(BOOL fUndoable)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE ie_InPlaceSite::OnInPlaceDeactivate(void)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE ie_InPlaceSite::DiscardUndoState(void)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE ie_InPlaceSite::DeactivateAndUndo(void)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE ie_InPlaceSite::OnPosRectChange(LPCRECT lprcPosRect)
{
	return E_NOTIMPL;
}

//===========================================================cc::win::ie_EventHandler
ie_EventHandler::ie_EventHandler()
{
	pWebBrowser = NULL;
}

ie_EventHandler::~ie_EventHandler()
{
}

HRESULT STDMETHODCALLTYPE ie_EventHandler::QueryInterface(REFIID classid, void** ppvObject)
{
	if(classid == IID_IUnknown || classid == IID_IDispatch || classid == ie_DIID_DWebBrowserEvents2) 
	{
		*ppvObject = this;
		return S_OK;
	}
	return E_NOINTERFACE;
}

ULONG STDMETHODCALLTYPE ie_EventHandler::AddRef()
{
	return 1;
}

ULONG STDMETHODCALLTYPE ie_EventHandler::Release()
{
	return 1;
}

HRESULT STDMETHODCALLTYPE ie_EventHandler::GetTypeInfoCount(/* [out] */ UINT *pctinfo)
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE ie_EventHandler::GetTypeInfo(
	/* [in] */ UINT iTInfo,
	/* [in] */ LCID lcid,
	/* [out] */ ITypeInfo **ppTInfo)
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE ie_EventHandler::GetIDsOfNames(
	/* [in] */ REFIID riid,
	/* [size_is][in] */ LPOLESTR *rgszNames,
	/* [in] */ UINT cNames,
	/* [in] */ LCID lcid,
	/* [size_is][out] */ DISPID *rgDispId)
{
	return S_OK;
}

/* [local] */ HRESULT STDMETHODCALLTYPE ie_EventHandler::Invoke(
	/* [in] */ DISPID dispIdMember,
	/* [in] */ REFIID riid,
	/* [in] */ LCID lcid,
	/* [in] */ WORD wFlags,
	/* [out][in] */ DISPPARAMS *pDispParams,
	/* [out] */ VARIANT *pVarResult,
	/* [out] */ EXCEPINFO *pExcepInfo,
	/* [out] */ UINT *puArgErr)
{
	return S_OK;
}

//===========================================================cc::win::ie_DocHandler
ie_DocHandler::ie_DocHandler(ie_EventHandler *commandHandler)
{
	m_bIsShowContextMenu = true;
	m_CommandHandler = commandHandler;
}

ie_DocHandler::~ie_DocHandler()
{
	if(m_CommandHandler != NULL)
	{
		delete m_CommandHandler;
		m_CommandHandler = NULL;
	}
}

HRESULT STDMETHODCALLTYPE ie_DocHandler::QueryInterface(REFIID classid, void** intf)
{
	if(classid == IID_IUnknown || classid == ie_IID_IDocHostUIHandler || classid == IID_IDispatch)
	{
		*intf = (void*)this;
	}
	else
	{
		return E_NOINTERFACE;
	}
	return S_OK;
}

ULONG STDMETHODCALLTYPE ie_DocHandler::AddRef()
{
	return 1;
}

ULONG STDMETHODCALLTYPE ie_DocHandler::Release()
{
	return 1;
}

//return S_OK, no right click menu,or E_NOINTERFACE disable menu
HRESULT STDMETHODCALLTYPE ie_DocHandler::ShowContextMenu(
	/* [in] */ DWORD dwID, 
	/* [in] */ POINT __RPC_FAR *ppt, 
	/* [in] */ IUnknown __RPC_FAR *pcmdtReserved, 
	/* [in] */ IDispatch __RPC_FAR *pdispReserved)
{
	return (m_bIsShowContextMenu ? E_NOINTERFACE : S_OK);
}

HRESULT STDMETHODCALLTYPE ie_DocHandler::GetHostInfo(/* [out][in] */ void *pInfo)
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE ie_DocHandler::ShowUI(
	/* [in] */ DWORD dwID,
	/* [in] */ IOleInPlaceActiveObject __RPC_FAR *pActiveObject,
	/* [in] */ IUnknown __RPC_FAR *pCommandTarget,
	/* [in] */ IOleInPlaceFrame __RPC_FAR *pFrame,
	/* [in] */ IOleInPlaceUIWindow __RPC_FAR *pDoc)
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE ie_DocHandler::HideUI(void)
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE ie_DocHandler::UpdateUI(void)
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE ie_DocHandler::EnableModeless(/* [in] */ BOOL fEnable)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE ie_DocHandler::OnDocWindowActivate(/* [in] */ BOOL fActivate)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE ie_DocHandler::OnFrameWindowActivate(/* [in] */ BOOL fActivate)
{
	return E_NOTIMPL; 
}

HRESULT STDMETHODCALLTYPE ie_DocHandler::ResizeBorder(
	/* [in] */ LPCRECT prcBorder,
	/* [in] */ IOleInPlaceUIWindow __RPC_FAR *pUIWindow,
	/* [in] */ BOOL fRameWindow)
{
	return E_NOTIMPL; 
}

HRESULT STDMETHODCALLTYPE ie_DocHandler::TranslateAccelerator(
	/* [in] */ LPMSG lpMsg,
	/* [in] */ const GUID __RPC_FAR *pguidCmdGroup,
	/* [in] */ DWORD nCmdID)
{
	return S_FALSE; 
}

HRESULT STDMETHODCALLTYPE ie_DocHandler::GetOptionKeyPath(
	/* [out] */ LPOLESTR __RPC_FAR *pchKey,
	/* [in] */ DWORD dw)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE ie_DocHandler::GetDropTarget(
/* [in] */ IDropTarget __RPC_FAR *pDropTarget,
/* [out] */ IDropTarget __RPC_FAR *__RPC_FAR *ppDropTarget)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE ie_DocHandler::GetExternal(/* [out] */ IDispatch __RPC_FAR *__RPC_FAR *ppDispatch)
{
	*ppDispatch = m_CommandHandler;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE ie_DocHandler::TranslateUrl(
	/* [in] */ DWORD dwTranslate,
	/* [in] */ OLECHAR __RPC_FAR *pchURLIn,
	/* [out] */ OLECHAR __RPC_FAR *__RPC_FAR *ppchURLOut)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE ie_DocHandler::FilterDataObject(
	/* [in] */ IDataObject __RPC_FAR *pDO,
	/* [out] */ IDataObject __RPC_FAR *__RPC_FAR *ppDORet)
{
	return E_NOTIMPL;
}

ie_EventHandler *ie_DocHandler::getEventHandler()
{
	return m_CommandHandler;
}

//===========================================================cc::win::IEBase
///////////////////// constructors //////////////////////////
IEBase::IEBase()
{
	//m_hOleInit = S_FALSE;
	pUnknown = NULL;
	pActiveObject = NULL;
	pObject = NULL;
	pWebBrowser = NULL;
	pInPlaceObject = NULL;

	m_pClientSite = NULL;
	m_pCPoint = NULL;
	m_dwCPointCookie = 0;
	m_pIEEventHandler = NULL;
	m_pUIHandler = NULL;
}

///////////////////// destructor //////////////////////////
IEBase::~IEBase()
{
#ifndef _DEBUG_
	try
	{
#endif
		if(m_pCPoint != NULL)
		{
			m_pCPoint->Unadvise(m_dwCPointCookie);
			m_pCPoint->Release();
			m_pCPoint = NULL;
		}
#ifndef _DEBUG_
	}
	catch(...)
	{
	}
#endif

#ifndef _DEBUG_
	try
	{
#endif
		if(pUnknown != NULL)
		{
			pUnknown->Release();
			pUnknown = NULL;
		}
#ifndef _DEBUG_
	}
	catch(...)
	{
	}
#endif

#ifndef _DEBUG_
	try
	{
#endif
		if(pActiveObject != NULL)
		{
			pActiveObject->Release();
			pActiveObject = NULL;
		}
#ifndef _DEBUG_
	}
	catch(...)
	{
	}
#endif

#ifndef _DEBUG_
	try
	{
#endif
		if(pWebBrowser != NULL)
		{
			pWebBrowser->Release();
			pWebBrowser = NULL;
		}
#ifndef _DEBUG_
	}
	catch(...)
	{
	}
#endif

#ifndef _DEBUG_
	try
	{
#endif
		if(pObject != NULL)
		{
			pObject->Close(OLECLOSE_NOSAVE);
			pObject->Release();
			pObject = NULL;
		}
#ifndef _DEBUG_
	}
	catch(...)
	{
	}
#endif

#ifndef _DEBUG_
	try
	{
#endif
		//Clean up hosted classes
		if(m_pClientSite != NULL)
		{
			m_pClientSite->Release();
			delete m_pClientSite; //also:m_pInPlaceSite,m_pDocHandler,my_DocHandler
			m_pClientSite = NULL;
		}
#ifndef _DEBUG_
	}
	catch(...)
	{
	}
#endif

#ifndef _DEBUG_
	try
	{
#endif
		if(m_pIEEventHandler != NULL)
		{
			m_pIEEventHandler->Release();
			delete m_pIEEventHandler;
			m_pIEEventHandler = NULL;
		}
#ifndef _DEBUG_
	}
	catch(...)
	{
	}
#endif

	CC_OleUnInitialize();
//#ifndef _DEBUG_
//	try
//	{
//#endif
//		//if OleInitialize at this cope,then relese it,cannot use SUCCEEDED!
//		if(m_hOleInit == S_OK)
//		{
//			OleUninitialize();
//		}
//#ifndef _DEBUG_
//	}
//	catch(...)
//	{
//	}
//#endif
}

///////////////////// private/internal used methods //////////////////////////
bool IEBase::_SetEventHandler()
{
	if(pUnknown != NULL && m_pIEEventHandler != NULL)
	{
		HRESULT hr = 0;
		if(m_pCPoint == NULL)
		{
			ie_IConnectionPointContainer *conter = NULL;
			//ie_IConnectionPoint *m_pCPoint = NULL;
			hr = pUnknown->QueryInterface(ie_IID_IConnectionPointContainer, (void**)&conter);
			hr = conter->FindConnectionPoint(ie_DIID_DWebBrowserEvents2, &m_pCPoint);
			if(FAILED(hr))
			{
				return false;
			}
			conter->Release();
		}
		else
		{
			m_pCPoint->Unadvise(m_dwCPointCookie);
		}
		//DWORD m_dwCPointCookie = 0;
		hr = m_pCPoint->Advise(m_pIEEventHandler, &m_dwCPointCookie);
		//m_pCPoint->Release();
		return (hr >= 0);
	}
	return false;
}

bool IEBase::SetEventHandler(ie_EventHandler *pIEEventHandler)
{
	m_pIEEventHandler = pIEEventHandler;
	return _SetEventHandler();
}

bool IEBase::OnCreate(HWND hWndParent, HINSTANCE hInst)
{
	return OnCreate(hWndParent, hInst, NULL, NULL, NULL);
}

bool IEBase::OnCreate(HWND hWndParent, HINSTANCE hInst, ie_DocHandler *pUIHandler,
					  ie_InPlaceSite *pInPlaceSite, ie_ClientSite *pClientSite, BSTR url)
{
	//CC_ASSERT(pUnknown == NULL);
	//CC_ASSERT(pWebBrowser == NULL);
	try
	{
		m_pUIHandler = pUIHandler;
		HRESULT hr;
		////Initialize COM library
		//m_hOleInit = OleInitialize(NULL);
		CC_OleInitialize();

		//LPCLASSFACTORY pClassFactory = NULL;
		//DWORD dwClsCtx = CLSCTX_INPROC_SERVER | CLSCTX_INPROC_HANDLER;
		//if(SUCCEEDED(hr = CoGetClassObject(ie_CLSID_WebBrowser, dwClsCtx, NULL, IID_IClassFactory, (void**)&pClassFactory)))
		//{
		//	CC_ASSERT(pClassFactory != NULL);
		//	hr = pClassFactory->CreateInstance(pUnknown, IID_IOleObject, (void**)&pObject);
		//	pClassFactory->Release();
		//}
		//if(FAILED(hr) || pObject == NULL)
		//{
		//	return false;
		//}

		//Instantiate Web Browser control
		/* 8856F961-340A-11D0-A96B-00C04FD705A2 */
		hr = CoCreateInstance(ie_CLSID_WebBrowser, NULL, CLSCTX_INPROC_SERVER | CLSCTX_INPROC_HANDLER, IID_IUnknown, (void**)(&pUnknown));
		if(FAILED(hr) || pUnknown == NULL)
		{
			return false;
		}
		//Instantiate all needed interfaces
		pUnknown->QueryInterface(IID_IOleObject, (void**)(&pObject));
		if(FAILED(hr) || pObject == NULL)
		{
			return false;
		}

		_SetEventHandler();

		m_pClientSite = pClientSite;
		if(m_pClientSite == NULL)
		{
			//m_pUIHandler is deleted at ie_ClientSite
			m_pClientSite = new ie_ClientSite(hWndParent, pInPlaceSite, m_pUIHandler);
		}
		//Set the client site
		hr = pObject->SetClientSite(m_pClientSite);

		//Set the site default limits
		RECT rWindowPos;
		//::SetRect(&rWindowPos, 0, 0, 100, 100);
		::GetClientRect(hWndParent, &rWindowPos);
		hr = pObject->QueryInterface(IID_IOleInPlaceObject, (LPVOID*)&pInPlaceObject);
		if(FAILED(hr) || pInPlaceObject == NULL)
		{
			return false;
		}
		//hr = pUnknown->QueryInterface(IID_IOleInPlaceObject, (void**)(&pInPlaceObject));
		hr = pInPlaceObject->SetObjectRects(&rWindowPos, &rWindowPos);
		hr = pObject->DoVerb(OLEIVERB_INPLACEACTIVATE, NULL, m_pClientSite, 0, hWndParent, &rWindowPos);

		//Get Web Browser interface
		hr = pUnknown->QueryInterface(ie_IID_IWebBrowser2, (void**)&pWebBrowser);
		if(FAILED(hr) || pWebBrowser == NULL)
		{
			return false;
		}
		if(m_pUIHandler != NULL)
		{
			m_pUIHandler->getEventHandler()->pWebBrowser = pWebBrowser;
		}

		hr = pWebBrowser->QueryInterface(IID_IOleInPlaceActiveObject, (void**)&pActiveObject);
		if(FAILED(hr) || pActiveObject == NULL)
		{
			return false;
		}

		//Go to URL
		return Navigate(url == NULL ? L"about:blank" : url);
	}
	catch(...)
	{
		return false;
	}
	return true;
}

void IEBase::Update()
{
	if(pInPlaceObject != NULL)
	{
		::UpdateWindow(getHWND());
	}
	//ExecHelperNN(IDM_REFRESH, OLECMDF_SUPPORTED);
	//ExecCommand(IDM_REFRESH, OLECMDF_SUPPORTED);
	//if(pWebBrowser != NULL)
	//{
	//	pActiveObject->GetWindow(&hWnd);
	//	pWebBrowser->QueryInterface(IID_IOleInPlaceActiveObject, (void**)&pActiveObject);
	//}
	//MSG msg;
	//while(PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
	//{
	//	if(msg.message == WM_KEYDOWN && GetKeyState(VK_CONTROL) < 0 && msg.wParam != 0x4e)
	//	{
	//		TranslateAccelerator(&msg);
	//		continue;
	//	}
	//	TranslateMessage(&msg);
	//	DispatchMessage(&msg);
	//}
}

void IEBase::Location(int Left, int Top)
{
	if(pWebBrowser != NULL && pInPlaceObject != NULL)
	{
		RECT m_Rect;
		long l = 0;
		m_Rect.left = Left;
		m_Rect.top = Top;
		pWebBrowser->get_Height(&l);
		m_Rect.bottom = m_Rect.top + l;
		pWebBrowser->get_Width(&l);
		m_Rect.right = m_Rect.left + l;
		pInPlaceObject->SetObjectRects(&m_Rect, &m_Rect);
	}
}

void IEBase::Size(int Width, int Height)
{
	if(pWebBrowser != NULL && pInPlaceObject != NULL)
	{
		RECT m_Rect;
		pWebBrowser->get_Top(&m_Rect.top);
		pWebBrowser->get_Left(&m_Rect.left);
		m_Rect.right = m_Rect.left + Width;
		m_Rect.bottom = m_Rect.top + Height;
		pInPlaceObject->SetObjectRects(&m_Rect, &m_Rect);
	}
}

HWND IEBase::getHWND()
{
	if(pInPlaceObject != NULL)
	{
		HWND hWnd = NULL;
		pInPlaceObject->GetWindow(&hWnd);
		return hWnd;
	}
	return NULL;
}

RECT IEBase::getPosition()
{
	//Set the site limits
	RECT m_Rect;
	if(pWebBrowser != NULL)
	{
		long l = 0;
		pWebBrowser->get_Top(&m_Rect.top);
		pWebBrowser->get_Left(&m_Rect.left);
		pWebBrowser->get_Height(&l);
		m_Rect.bottom = m_Rect.top + l;
		pWebBrowser->get_Width(&l);
		m_Rect.right = m_Rect.left + l;
	}
	return m_Rect;
}

void IEBase::setPosition(RECT rect)
{
	if(pInPlaceObject != NULL)
	{
		pInPlaceObject->SetObjectRects(&rect, &rect);
	}
}

bool IEBase::isUnloadOk()
{
	if(pWebBrowser != NULL)
	{
		VARIANT vOut;
		VariantInit(&vOut);
		HRESULT hr = pWebBrowser->ExecWB(OLECMDID_ONUNLOAD, OLECMDEXECOPT_DODEFAULT, NULL, &vOut);
		if(SUCCEEDED(hr))
		{
			// Check VT_BOOL return value of vOut to make sure we're really shutting down - IE or the user
			// might decide not to honor this request.
			//not know how to cancel it
			if(vOut.bVal == 0)
			{
				return false;
			}
		}
	}
	return true;
}

bool IEBase::TranslateAccelerator(MSG *pMsg)
{
	//if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_F12)
	//{
	//	return false;
	//}
	//return (pActiveObject->TranslateAccelerator(pMsg) == S_OK) ? true : false;
	if(pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST)
	{
		if(pMsg->wParam != VK_F12)
		{
			return (pActiveObject->TranslateAccelerator(pMsg) == S_OK) ? true : false;
		}
	}
	return false;
}

bool IEBase::Navigate(BSTR url)
{
	if(pWebBrowser != NULL)
	{
		HRESULT hr = pWebBrowser->Navigate(url, NULL, NULL, NULL, NULL);
		return (hr >= 0);
	}
	return false;
}

bool IEBase::execScript(BSTR code, BSTR language)
{
	ie_IHTMLDocument2* pDocument2 = getDocument();
	if(pDocument2 == NULL)
	{
		return false;
	}
	HRESULT hr = S_OK;

	ie_IHTMLWindow2* pWindow2 = NULL;
	hr = pDocument2->get_parentWindow(&pWindow2);
	if(!SUCCEEDED(hr) || pWindow2 == NULL)
	{
		pDocument2->Release();
		return false;
	}

	ie_IHTMLWindow2* pWindowTop = NULL;
	hr = pWindow2->get_top(&pWindowTop);
	if(!SUCCEEDED(hr) || pWindowTop == NULL)
	{
		pWindow2->Release();
		pDocument2->Release();
		return false;
	}

	VARIANT varResult;
	VariantInit(&varResult);
	hr = pWindowTop->execScript(code, language, &varResult);
	pWindowTop->Release();
	pWindow2->Release();
	pDocument2->Release();
	return SUCCEEDED(hr);

	//DISPID dispidScriptObject, dispidMethodToInvoke;
	//IDispatch *pDispScriptObject = NULL;
	//DISPPARAMS dispparamsNoArgs = { NULL, NULL, 0, 0};
	//VARIANT varResult;
	//
	//// Get the DISPID of the document's Script property.
	//hr = pDocDisp->GetIDsOfNames(IID_NULL, &language, 1, LOCALE_USER_DEFAULT, &dispidScriptObject);
	//if(!SUCCEEDED(hr))
	//{
	//	pDocDisp->Release();
	//	return false;
	//}

	//// Get the document's Script property.
	//hr = pDocDisp->Invoke(dispidScriptObject, IID_NULL, LOCALE_USER_DEFAULT,
	//	DISPATCH_PROPERTYGET, &dispparamsNoArgs, &varResult, NULL, NULL);
	//if(!SUCCEEDED(hr) || varResult.pdispVal == NULL)
	//{
	//	pDocDisp->Release();
	//	return false;
	//}

	//pDispScriptObject = varResult.pdispVal;
	//VariantInit(&varResult);
	//// Get the DISPID of the script method to invoke.
	//hr = pDispScriptObject->GetIDsOfNames(IID_NULL, &code, 1, 
	//	LOCALE_USER_DEFAULT, &dispidMethodToInvoke);
	//if(!SUCCEEDED(hr))
	//{
	//	pDispScriptObject->Release();
	//	pDocDisp->Release();
	//	return false;
	//}

	//// Invoke the script method.
	//hr = pDispScriptObject->Invoke(dispidMethodToInvoke, IID_NULL,
	//	LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dispparamsNoArgs, 
	//	&varResult, NULL, NULL);
	//pDispScriptObject->Release();
	//pDocDisp->Release();
	//return SUCCEEDED(hr);
}

bool IEBase::write(BSTR txt, bool isAppend)
{
	ie_IHTMLDocument2* pDocument2 = getDocument();
	if(pDocument2 == NULL)
	{
		return false;
	}
	if(!isAppend)
	{
		pDocument2->clear();
		pDocument2->close();
	}

	HRESULT hr = S_OK;
	SAFEARRAY *sfArray = SafeArrayCreateVector(VT_VARIANT, 0, 1);
	if(sfArray == NULL)
	{
		pDocument2->Release();
		return false;
	}

	VARIANT *param;
	hr = SafeArrayAccessData(sfArray, (LPVOID*)&param);
	param->vt = VT_BSTR;
	param->bstrVal = txt;
	hr = SafeArrayUnaccessData(sfArray);
	hr = pDocument2->write(sfArray);

	SafeArrayDestroy(sfArray);
	pDocument2->Release();
	return SUCCEEDED(hr);
}

//need Release() out of this
ie_IHTMLDocument2* IEBase::getDocument()
{
	if(pWebBrowser == NULL)
	{
		return NULL;
	}
	HRESULT hr = S_OK;

	IDispatch* pDocDisp = NULL;
	hr = pWebBrowser->get_Document(&pDocDisp);
	if(!SUCCEEDED(hr) || pDocDisp == NULL)
	{
		return NULL;
	}

	ie_IHTMLDocument2* pDocument2 = NULL;
	hr = pDocDisp->QueryInterface(IID_IHTMLDocument2, (void**)&pDocument2);
	pDocDisp->Release();
	return pDocument2;
}

bool IEBase::ValidFocus()
{
	ie_IHTMLDocument2* pDocument2 = getDocument();
	if(pDocument2 == NULL)
	{
		return false;
	}

	HRESULT hr = S_OK;
	HWND hWnd = getHWND();
	if(pObject != NULL && m_pClientSite != NULL)
	{
		pObject->DoVerb(OLEIVERB_UIACTIVATE, NULL, m_pClientSite, 0, hWnd, NULL);
	}

	ie_IHTMLElement* pElement = NULL;
	hr = pDocument2->get_activeElement(&pElement);
	if(!SUCCEEDED(hr))
	{
		pDocument2->Release();
		return false;
	}
	bool isOK = false;
	if(pElement != NULL)
	{
		isOK = true;
		BSTR bstr = ::SysAllocString(L"                    ");
		hr = pElement->get_tagName(&bstr);
		if(SUCCEEDED(hr) && bstr != NULL && _wcsicmp(bstr, L"BODY") == 0)
		{
			isOK = false;
		}
		::SysFreeString(bstr);
		pElement->Release();
		pElement = NULL;
	}
	hr = S_OK;
	if(!isOK)
	{
		MSG msg = {hWnd, WM_KEYDOWN, VK_TAB, 0, 0, 0};
		hr = pActiveObject->TranslateAccelerator(&msg);
	}
	pDocument2->Release();
	if(pObject != NULL && m_pClientSite != NULL)
	{
		//fonfirm set focus again
		//RECT rc;
		//::GetClientRect(this->m_hWnd, &rc);
		//pObject->DoVerb(OLEIVERB_UIACTIVATE, NULL, m_pClientSite, 0, hWnd, &rc);
		pObject->DoVerb(OLEIVERB_UIACTIVATE, NULL, m_pClientSite, 0, hWnd, NULL);
	}
	return !SUCCEEDED(hr);
	//also can use javascript to doit!
	//BSTR bstrCode = ::SysAllocString(L"if(document){if(!document.activeElement || document.activeElement.tagName=='BODY'){  for(var i=0;i<document.all.length;i++){if(  document.all[i].tagName=='A' || document.all[i].tagName=='INPUT'   ){document.all[i].focus();break;}}  }};");
	//execScript(bstrCode, L"javascript");
	//::SysFreeString(bstrCode);
}

bool IEBase::SetFocus()
{
	if(pWebBrowser != NULL)
	{
		HWND hWnd = getHWND();
		::SetFocus(hWnd);
		ValidFocus();
		return true;
	}
	return false;
}

bool IEBase::waitComplete(int millisecondsTimeout)
{
	MSG msg;
	DWORD nFirstTick = ::GetTickCount();
	while(!isComplete())
	{
		while(PeekMessage(&msg, (HWND)0,  0, 0, PM_REMOVE) != 0)
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		Sleep(200);
		if(millisecondsTimeout > 0)
		{
			if((int)(::GetTickCount () - nFirstTick) > millisecondsTimeout)
			{
				break;
			}
		}
	}
	return isComplete();
}

bool IEBase::isComplete()
{
	if(pWebBrowser != NULL)
	{
		ie_READYSTATE plReadyState;
		pWebBrowser->get_ReadyState(&plReadyState);
		if(plReadyState != READYSTATE_LOADED && plReadyState != READYSTATE_COMPLETE)
		{
			return false;
		}
	}
	return true;
}

bool IEBase::loadText(const void* txt, DWORD nLength)
{
	if(pWebBrowser == NULL || txt == NULL)
	{
		return false;
	}

	bool isOK = false;
	HRESULT hr;
	IStream* pStream = NULL;
	HGLOBAL hHTMLText = ::GlobalAlloc(GHND, nLength + 10);
	if(hHTMLText != NULL)
	{
		void *pLock = (void *)GlobalLock(hHTMLText);
		::ZeroMemory(pLock, nLength + 10);
		//_tcscpy(pLock, tcText);
		memcpy(pLock, txt, nLength);
		GlobalUnlock(hHTMLText);
		hr = CreateStreamOnHGlobal(hHTMLText, TRUE, &pStream);
		if(SUCCEEDED(hr))
		{
			// Retrieve the document object.
			IDispatch* pHtmlDoc = NULL;
			hr = pWebBrowser->get_Document(&pHtmlDoc);
			if(SUCCEEDED(hr))
			{
				// Call the helper function to load the browser from the stream.
				isOK = loadStream(pStream, pHtmlDoc);
				pHtmlDoc->Release();
			}
			pStream->Release();
		}
		GlobalFree(hHTMLText);
	}
	return isOK;
}

//need free point out this fun
bool IEBase::saveText(void** txt, DWORD& nLength)
{
	if(pWebBrowser == NULL)
	{
		return false;
	}

	bool bRet = false;
	HRESULT hr;
	IDispatch* pHtmlDoc = NULL;

	// Retrieve the document object.
	hr = pWebBrowser->get_Document(&pHtmlDoc);
	if(SUCCEEDED(hr))
	{
		bRet = saveText(pHtmlDoc, txt, nLength);
		pHtmlDoc->Release();
	}
	return bRet;
}

//static
//need free point out this fun
bool IEBase::saveText(IDispatch* pHtmlDoc, void** txt, DWORD& nLength)
{
	bool bRet = false;
	HRESULT hr;
	IStream* pStream = NULL;

	if(pHtmlDoc != NULL)
	{
		// Initialize the document.
		hr = CreateStreamOnHGlobal(NULL, TRUE, &pStream);
		if(SUCCEEDED(hr))
		{
			// save the contents to stream.
			bRet = saveStream(pStream, pHtmlDoc);

			LARGE_INTEGER li;
			li.QuadPart = 0L;
			pStream->Seek(li, STREAM_SEEK_SET, NULL);
			STATSTG stat;
			pStream->Stat(&stat, 0);

			*txt = malloc(stat.cbSize.LowPart + 10);
			::ZeroMemory(*txt, stat.cbSize.LowPart + 10);
			pStream->Read(*txt, stat.cbSize.LowPart, &nLength);
			pStream->Release();
		}
	}

	return bRet;
}

//static
bool IEBase::saveStream(IStream* pStream, IDispatch* pHtmlDoc)
{
	if(pHtmlDoc == NULL)
	{
		return false;
	}

	bool isOK = false;
	HRESULT hr;
	// Query for IPersistStreamInit.
	ie_IPersistStreamInit* pPersistStreamInit = NULL;
	hr = pHtmlDoc->QueryInterface(IID_IPersistStreamInit, (void**)&pPersistStreamInit);
	if(SUCCEEDED(hr))
	{
		// save the contents to stream.
		hr = pPersistStreamInit->Save(pStream, FALSE);
		if(SUCCEEDED(hr))
		{
			isOK = true;
		}
		pPersistStreamInit->Release();
	}
	return isOK;
}

//static
bool IEBase::loadStream(IStream* pStream, IDispatch* pHtmlDoc)
{
	if(pHtmlDoc == NULL)
	{
		return false;
	}

	bool isOK = false;
	HRESULT hr;
	// Query for IPersistStreamInit.
	ie_IPersistStreamInit* pPersistStreamInit = NULL;
	hr = pHtmlDoc->QueryInterface(IID_IPersistStreamInit, (void**)&pPersistStreamInit);
	if(SUCCEEDED(hr))
	{
		// Initialize the document.
		hr = pPersistStreamInit->InitNew();
		if(SUCCEEDED(hr))
		{
			// Load the contents of the stream.
			hr = pPersistStreamInit->Load(pStream);
			if(SUCCEEDED(hr))
			{
				isOK = true;
			}
		}
		pPersistStreamInit->Release();
	}
	return isOK;
}

//http://cboard.cprogramming.com/archive/index.php/t-52437.html
//http://www.codeguru.com/cpp/i-n/internet/browsercontrol/article.php/c6175/
#ifndef LPFNOBJECTFROMLRESULT
typedef HRESULT (STDAPICALLTYPE *LPFNOBJECTFROMLRESULT)(LRESULT lResult, REFIID riid, WPARAM wParam, void** ppvObject);
#endif //LPFNOBJECTFROMLRESULT
//even success for attach,cannot use event
//also this function is only get pWebBrowser from hWnd,so some other functions of IEBase cannot be used
/*
	IEBase iebase;
	if(!iebase.AttachHWND(hWnd))
	{
		return;
	}
	void* txt = NULL;
	DWORD nLen = 0;
	if(iebase.saveText(&txt, nLen))
	{
		//see point as char*
		char* ctxt = (char*)txt;
		cc::Str str = cc::Str(ctxt);
		editInfo->AppendText((TCHAR*)str.GetBuf());
	}
*/
//need Release out this scope
ie_IHTMLDocument2* IEBase::GetDocumentFromHWND(HWND hWnd)
{
	TCHAR buf[31];
	::GetClassName(hWnd, (LPTSTR)&buf, 30);
	ie_IHTMLDocument2 *spDoc = NULL;
	if(_tcscmp(buf, _T("Internet Explorer_Server")) == 0)
	{
		HINSTANCE hOleaccInst = ::LoadLibrary(_T("OLEACC.DLL"));
		if(hOleaccInst != NULL)
		{
			LRESULT lRes;
			UINT nMsg = ::RegisterWindowMessage(_T("WM_HTML_GETOBJECT"));
			::SendMessageTimeout(hWnd, nMsg, 0L, 0L, SMTO_ABORTIFHUNG, 1000, (PDWORD_PTR)(DWORD*)&lRes);
			LPFNOBJECTFROMLRESULT pfObjectFromLresult = (LPFNOBJECTFROMLRESULT)::GetProcAddress(hOleaccInst, "ObjectFromLresult");
			if(pfObjectFromLresult != NULL)
			{
				(*pfObjectFromLresult)(lRes, IID_IHTMLDocument2, 0, (void**)&spDoc);
				//if(SUCCEEDED(hr) && spDoc != NULL)
			}
			::FreeLibrary(hOleaccInst);
		}
	}
	return spDoc;
}

bool IEBase::AttachHWND(HWND hWnd)
{
	//CC_ASSERT(pUnknown == NULL);
	//CC_ASSERT(pActiveObject == NULL);
	//CC_ASSERT(pObject == NULL);
	//CC_ASSERT(pWebBrowser == NULL);
	ie_IHTMLDocument2 *spDoc = GetDocumentFromHWND(hWnd);
	if(spDoc == NULL)
	{
		return false;
	}

	IServiceProvider* isp = NULL;
	IServiceProvider* isp2 = NULL;
	HRESULT hr = spDoc->QueryInterface(IID_IServiceProvider, reinterpret_cast<void **>(&isp));
	spDoc->Release();
	if(SUCCEEDED(hr) && isp != NULL)
	{
		hr = isp->QueryService(ie_SID_STopLevelBrowser, IID_IServiceProvider, reinterpret_cast<void **>(&isp2));
		isp->Release();
		if(SUCCEEDED(hr) && isp2 != NULL)
		{
			hr = isp2->QueryService(IID_IWebBrowserApp, IID_IWebBrowser2, reinterpret_cast<void **>(&pWebBrowser));
			isp2->Release();
			if(SUCCEEDED(hr) && pWebBrowser != NULL)
			{
				//pUnknown = isp2;
				//hr = pUnknown->QueryInterface(IID_IOleObject, (void**)(&pObject));
				//hr = pObject->QueryInterface(IID_IOleInPlaceObject, (LPVOID*)&pInPlaceObject);
				//hr = pWebBrowser->QueryInterface(IID_IOleInPlaceActiveObject, (void**)&pActiveObject);
				return true;
			}
		}
	}
	return false;
}


//IETranceData data;
//ScriptHost sh;
//sh.InitializeHost(L"VBScript");
//sh.RunScript(L"MsgBox \"myObj.Buffer\"\r\n", L"obj", &data);
//Constructor
ScriptHost::ScriptHost()
{
	//CC_TRACE((((((_T("ScriptHost::ScriptHost()"));
	m_refCount = 0;
	m_Engine = NULL;
	m_Object = NULL;
	m_bRunError = false;
}

//Destructor
ScriptHost::~ScriptHost()
{
	//CC_TRACE((((((_T("ScriptHost::~ScriptHost() S, m_refCount:%d"), m_refCount);

	if(m_Engine != NULL)
	{
		m_Engine->Release();
		m_Engine = NULL;
	}
	if(m_Parser != NULL)
	{
		m_Parser->Release();
		m_Parser = NULL;
	}
	if(m_Object != NULL)
	{
		m_Object->Release();
		m_Object = NULL;
	}
	//CC_TRACE((((((_T("ScriptHost::~ScriptHost() E, m_refCount:%d"), m_refCount);
}

/******************************************************************************
*   InitializeScriptFrame -- Creates the ActiveX Scripting Engine and 
*   initializes it.  Returns true if successful, false otherwise.
******************************************************************************/
BOOL ScriptHost::InitializeHost(LPOLESTR pwszLanguage)
{
	if(pwszLanguage == NULL)
	{
		pwszLanguage = L"VBScript";
	}
	//CC_TRACE((((((_T("ScriptHost::InitializeHost, pwszLanguage:%s"), pwszLanguage);

	CLSID id;
	CLSIDFromProgID(pwszLanguage, &id);
	//First, create the scripting engine with a call to CoCreateInstance, placing the created engine in m_Engine.
	HRESULT hr = CoCreateInstance(id, NULL, CLSCTX_INPROC_SERVER, IID_IActiveScript, (void **)&m_Engine);
	if(FAILED(hr))
	{
		//CC_TRACE((((((_T("Failed to create scripting engine."));
		return false;
	}

	//Now query for the IActiveScriptParse interface of the engine
	hr = m_Engine->QueryInterface(IID_IActiveScriptParse, (void**)&m_Parser);
	if(FAILED(hr))
	{
		//CC_TRACE((((((_T("Engine doesn't support IActiveScriptParse."));
		return false;
	}

	//The engine needs to know the host it runs on.
	hr = m_Engine->SetScriptSite(this);
	if(FAILED(hr))
	{
		//CC_TRACE((((((_T("Error calling SetScriptSite"));
		return false;
	}

	//Initialize the script engine so it's ready to run.
	hr = m_Parser->InitNew();
	if(FAILED(hr))
	{
		//CC_TRACE((((((_T("Error calling InitNew"));
		return false;
	}

	return true;

}

/******************************************************************************
*   RunScript -- starts the script engine and executes it's instructions.
*   here only save addres of point of pstrName and pObject,so it's need be alive while call this
******************************************************************************/
bool ScriptHost::RunScript(LPOLESTR pwszScript, LPOLESTR pstrName, LPUNKNOWN pObject)
{
	//tracing purposes only
	//CC_TRACE((((((_T("ScriptHost::RunScript, pstrName:%s"), pstrName);
	m_bRunError = false;
	m_pNamedItem = pstrName;
	m_Object = pObject;
	//if(m_Object != NULL)
	//{
	//	m_Object->AddRef();
	//}

	//Add the name of the object that will respond to the script
	m_Engine->AddNamedItem(pstrName, SCRIPTITEM_ISSOURCE | SCRIPTITEM_ISVISIBLE);

 	//Pass the script to be run to the script engine with a call to ParseScriptText
	HRESULT hr = m_Parser->ParseScriptText(pwszScript, pstrName, NULL, NULL, 0, 0, 0L, NULL, NULL);
	if(FAILED(hr))
	{
		//CC_TRACE((((((_T("Error calling ParseScriptText"));
		return false;
	}

	//Tell the engine to start processing the script with a call to 
	//SetScriptState().
	hr = m_Engine->SetScriptState(SCRIPTSTATE_CONNECTED);
	if(FAILED(hr))
	{
		//CC_TRACE((((((_T("Error calling SetScriptState"));
		return false;
	}

	return true;
}

bool ScriptHost::QuitScript()
{
	HRESULT hr = m_Engine->Close();
	return SUCCEEDED(hr);
}

/******************************************************************************
*   IUnknown Interfaces -- All COM objects must implement, either directly or 
*   indirectly, the IUnknown interface.
******************************************************************************/

/******************************************************************************
*   QueryInterface -- Determines if this component supports the requested 
*   interface, places a pointer to that interface in ppvObj if it's available,
*   and returns S_OK.  If not, sets ppvObj to NULL and returns E_NOINTERFACE.
******************************************************************************/
STDMETHODIMP ScriptHost::QueryInterface(REFIID riid, void ** ppvObj)
{
	//tracing purposes only
	//CC_TRACE((((((_T("ScriptHost::QueryInterface->"));

	if(riid == IID_IUnknown)
	{
		//CC_TRACE((((((_T("IUnknown"));
		*ppvObj = static_cast<IActiveScriptSite*>(this);
	}
	else if(riid == IID_IActiveScriptSite)
	{
		//CC_TRACE((((((_T("IActiveScriptSite"));
		*ppvObj = static_cast<IActiveScriptSite*>(this);
	}
	else if(riid == IID_IActiveScriptSiteWindow)
	{
		//CC_TRACE((((((_T("IActiveScriptSiteWindow"));
		*ppvObj = static_cast<IActiveScriptSiteWindow*>(this);
	}
	else
	{
		//CC_TRACE((((((_T("Unsupported Interface"));
		*ppvObj = NULL;
		return E_NOINTERFACE;
	}

	static_cast<IUnknown*>(*ppvObj)->AddRef();
	return S_OK;
}

/******************************************************************************
*   AddRef() -- In order to allow an object to delete itself when it is no 
*   longer needed, it is necessary to maintain a count of all references to 
*   this object.  When a new reference is created, this function increments
*   the count.
******************************************************************************/
STDMETHODIMP_(ULONG) ScriptHost::AddRef()
{
	//tracing purposes only
	++m_refCount;
	//CC_TRACE((((((_T("ScriptHost::AddRef"));

	//TODO:
	return 1;
	//return ++m_refCount;
}

/******************************************************************************
*   Release() -- When a reference to this object is removed, this function 
*   decrements the reference count.  If the reference count is 0, then this 
*   function deletes this object and returns 0;
******************************************************************************/
STDMETHODIMP_(ULONG) ScriptHost::Release()
{
	//tracing purposes only
	--m_refCount;
	//CC_TRACE((((((_T("ScriptHost::Release:%d"), m_refCount);

	//TODO:
	return 1;
	//if(--m_refCount == 0)
	//{
	//	delete this;
	//	return 0;
	//}
	//return m_refCount;
}

/******************************************************************************
*   IActiveScriptSite Interfaces -- These interfaces define the exposed methods
*   of ActiveX Script Hosts.
******************************************************************************/

/******************************************************************************
*   GetLCID() -- Gets the identifier of the host's user interface.  This method 
*   returns S_OK if the identifier was placed in plcid, E_NOTIMPL if this 
*   function is not implemented, in which case the system-defined identifier
*   should be used, and E_POINTER if the specified pointer was invalid.
******************************************************************************/
STDMETHODIMP ScriptHost::GetLCID(LCID *plcid)
{
	//tracing purposes only
	//CC_TRACE((((((_T("ScriptHost::GetLCID"));

	*plcid = LOCALE_USER_DEFAULT;
	return(S_OK);
}

/******************************************************************************
*   GetItemInfo() -- Retrieves information about an item that was added to the 
*   script engine through a call to AddNamedItem.
*   Parameters:   pstrName -- the name of the item, specified in AddNamedItem.
*            dwReturnMask -- Mask indicating what kind of pointer to return
*               SCRIPTINFO_IUNKNOWN or SCRIPTINFO_ITYPEINFO
*            ppunkItem -- return spot for an IUnknown pointer
*            ppTypeInfo -- return spot for an ITypeInfo pointer
*   Returns:   S_OK if the call was successful
*            E_INVALIDARG if one of the arguments was invalid
*            E_POINTER if one of the pointers was invalid
*            TYPE_E_ELEMENTNOTFOUND if there wasn't an item of the 
*               specified type.
******************************************************************************/
STDMETHODIMP ScriptHost::GetItemInfo(LPCOLESTR pstrName, DWORD dwReturnMask, IUnknown **ppunkItem, ITypeInfo **ppTypeInfo)
{
	//tracing purposes only
	//CC_TRACE((((((_T("ScriptHost::GetItemInfo, pstrName:%s"), pstrName);

	//Use logical ANDs to determine which type(s) of pointer the caller wants, 
	//and make sure that that placeholder is currently valid.
	if(dwReturnMask & SCRIPTINFO_IUNKNOWN)
	{
		//CC_TRACE((((((_T("ScriptHost::GetItemInfo, SCRIPTINFO_IUNKNOWN1"));
		if(!ppunkItem)
		{
			return E_INVALIDARG;
		}
		*ppunkItem = NULL;
	}
	if(dwReturnMask & SCRIPTINFO_ITYPEINFO)
	{
		//CC_TRACE((((((_T("ScriptHost::GetItemInfo, SCRIPTINFO_ITYPEINFO1"));
		if(!ppTypeInfo)
		{
			return E_INVALIDARG;
		}
		*ppTypeInfo = NULL;
	}

	// get named items here
	if(!_wcsicmp(m_pNamedItem, pstrName) && m_Object != NULL)
	{
		if(dwReturnMask & SCRIPTINFO_IUNKNOWN)
		{
			//CC_TRACE((((((_T("ScriptHost::GetItemInfo, SCRIPTINFO_IUNKNOWN2"));
			m_Object->QueryInterface(IID_IUnknown, (void**)ppunkItem);
			return S_OK;
		}
		//else if(dwReturnMask & SCRIPTINFO_ITYPEINFO)
		//{
		//	//CC_TRACE((((((_T("ScriptHost::GetItemInfo, SCRIPTINFO_ITYPEINFO2"));
			//return m_Object->LoadTypeInfo(ppTypeInfo, CLSID_ashObject, 0);

		//	return S_OK;
		//}
	}

	return TYPE_E_ELEMENTNOTFOUND;
}

/******************************************************************************
*   GetDocVersionString() -- It is possible, even likely that a script document
*   can be changed between runs.  The host can define a unique version number 
*   for the script, which can be saved along with the script.  If the version 
*   changes, the engine will know to recompile the script on the next run.
******************************************************************************/
STDMETHODIMP ScriptHost::GetDocVersionString(BSTR *pbstrVersionString)
{
	//tracing purposes only
	//CC_TRACE((((((_T("ScriptHost::GetDocVersionString"));

	//For the generic case, this function isn't implemented.
	return E_NOTIMPL;
}

/******************************************************************************
*   OnScriptTerminate() -- This method may give the host a chance to react when
*   the script terminates.  pvarResult give the result of the script or NULL
*   if the script doesn't give a result, and pexcepinfo gives the location of
*   any exceptions raised by the script.  Returns S_OK if the calls succeeds.
******************************************************************************/
STDMETHODIMP ScriptHost::OnScriptTerminate(const VARIANT *pvarResult, const EXCEPINFO *pexcepinfo)
{
	//tracing purposes only
	//CC_TRACE((((((_T("ScriptHost::OnScriptTerminate"));

	//If something needs to happen when the script terminates, put it here.
	return S_OK;
}

/******************************************************************************
*   OnStateChange() -- This function gives the host a chance to react when the
*   state of the script engine changes.  ssScriptState lets the host know the
*   new state of the machine.  Returns S_OK if successful.
******************************************************************************/
STDMETHODIMP ScriptHost::OnStateChange( SCRIPTSTATE ssScriptState)
{
	//tracing purposes only
	//CC_TRACE((((((_T("ScriptHost::OnStateChange"));

	//If something needs to happen when the script enters a certain state, put it here.
	switch (ssScriptState)
	{
	case SCRIPTSTATE_UNINITIALIZED:
		//CC_TRACE((((((_T("State: Uninitialized."));
		break;
	case SCRIPTSTATE_INITIALIZED:
		//CC_TRACE((((((_T("State: Initialized."));
		break;
	case SCRIPTSTATE_STARTED:
		//CC_TRACE((((((_T("State: Started."));
		break;
	case SCRIPTSTATE_CONNECTED:
		//CC_TRACE((((((_T("State: Connected."));
		break;
	case SCRIPTSTATE_DISCONNECTED:
		//CC_TRACE((((((_T("State: Disconnected."));
		break;
	case SCRIPTSTATE_CLOSED:
		//CC_TRACE((((((_T("State: Closed."));
		break;
	default:
		break;
	}

	return S_OK;
}

/******************************************************************************
*   OnScriptError() -- This function gives the host a chance to respond when 
*   an error occurs while running a script.  pase holds a reference to the 
*   IActiveScriptError object, which the host can use to get information about
*   the error.  Returns S_OK if the error was handled successfully, and an OLE
*   error code if not.
******************************************************************************/
STDMETHODIMP ScriptHost::OnScriptError(IActiveScriptError *pase)
{
	//tracing purposes only
	//CC_TRACE((((((_T("ScriptHost::OnScriptError"));
	m_bRunError = true;

	//Display a message box with information about the error.
	EXCEPINFO theException;
	HRESULT hr = pase->GetExceptionInfo(&theException);
	if(SUCCEEDED(hr))
	{
		//CC_TRACE((((((_T("ScriptHost::OnScriptError MSG:%s"), theException.bstrDescription);
	}
	return S_OK;
}

bool ScriptHost::GetIsRunError()
{
	return m_bRunError;
}

/******************************************************************************
*   OnEnterScript() -- This function gives the host a chance to respond when
*   the script begins running.  Returns S_OK if the call was successful.
******************************************************************************/
STDMETHODIMP ScriptHost::OnEnterScript(void)
{
	//tracing purposes only
	//CC_TRACE((((((_T("ScriptHost::OnEnterScript"));

	return S_OK;
}

/******************************************************************************
*   OnExitScript() -- This function gives the host a chance to respond when
*   the script finishes running.  Returns S_OK if the call was successful.
******************************************************************************/
STDMETHODIMP ScriptHost::OnLeaveScript(void)
{
	//tracing purposes only
	//CC_TRACE((((((_T("ScriptHost::OnLeaveScript"));

	return S_OK;
}

/******************************************************************************
*   IActiveScriptSiteWindow -- This interface allows the script engine to 
*   manipulate the user interface, if it's located in the same object as the 
*   IActiveScriptSite.
******************************************************************************/

/******************************************************************************
*   GetWindow() -- This function returns a handle to a window that the script
*   engine can use to display information to the user.  Returns S_OK if the 
*   call was successful, and E_FAIL if there was an error.
******************************************************************************/
STDMETHODIMP ScriptHost::GetWindow(HWND *phwnd)
{
	//CC_TRACE((((((_T("ScriptHost::GetWindow, phwnd:%d"), phwnd);

	if(phwnd == NULL)
	{
		return E_POINTER;
	}
	*phwnd = ::GetDesktopWindow();
	return S_OK;
}

/******************************************************************************
*   EnableModeless() -- This function instructs the host to enable or disable
*   it's main window and any modeless dialog boxes it may have.  Returns S_OK
*   if successful, and E_FAIL if not.
******************************************************************************/
STDMETHODIMP ScriptHost::EnableModeless(BOOL fEnable)
{
	//tracing purposes only
	//CC_TRACE((((((_T("ScriptHost::EnableModeless, fEnable:%d"), fEnable);

	//EnableWindow(MainWindow, enable);
	return S_OK;
}
