#include "ActivScp.h"
// cc.win.IECtrl.h
// The C# like Controls class
// Author: Shu.KK
// 2006-07-01
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef CC_WIN_IECTRL_H_
#define CC_WIN_IECTRL_H_

#include <tchar.h>
#include <windows.h>
//i donot want to include "mshtmlc.h"
//#include "mshtmlc.h"

//#include "cc.h"
#include "cc.Log.h"
#include "cc.win.OleBase.h"
//#include "cc.Str.h"

namespace cc
{
namespace win
{

//here treated for invoid of #include <exdisp.h> or <mshtml.h>
EXTERN_C const IID IID_IWebBrowser;
EXTERN_C const IID IID_IWebBrowser2;
EXTERN_C const IID IID_IWebBrowserApp;
EXTERN_C const IID IID_ICustomDoc;
EXTERN_C const IID IID_IDocHostUIHandler;
EXTERN_C const IID IID_IConnectionPointContainer;
EXTERN_C const IID DIID_DWebBrowserEvents2;
EXTERN_C const IID IID_IConnectionPointContainer;
EXTERN_C const IID IID_IHTMLDocument;
EXTERN_C const IID IID_IHTMLDocument2;
EXTERN_C const IID IID_IHTMLScriptElement;
EXTERN_C const IID IID_IHTMLElementCollection;
EXTERN_C const IID IID_IHTMLElement;
EXTERN_C const IID IID_IHTMLWindow2;

//from _SHLGUID_H_
const IID ie_SID_STopLevelBrowser = {0x4C96BE40L, 0x915C, 0x11CF, 0x99, 0xD3, 0x00, 0xAA, 0x00, 0x4A, 0xE8, 0x37};
	
const IID ie_IDocHostUIHandler = {0xBD3F23C0, 0xD43E, 0x11CF, 0x89, 0x3B, 0x00, 0xAA, 0x00, 0xBD, 0xCE, 0x1A};
const IID ie_IID_IWebBrowser = IID_IWebBrowser;
const IID ie_IID_IWebBrowser2 = IID_IWebBrowser2;
const IID ie_IID_IWebBrowserApp = IID_IWebBrowserApp; //{0x05DF0200, 0x0000, 0x0000, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46};
const IID ie_IID_ICustomDoc = IID_ICustomDoc; //{0xF0F35030, 0xB598, 0xCF11, 0xBB, 0x82, 0x00, 0xAA, 0x00, 0xBD, 0xCE, 0x0B};
const IID ie_IID_IDocHostUIHandler = IID_IDocHostUIHandler; //{0xC0233FBD, 0x3ED4, 0xCF11, 0x89, 0x3B, 0x00, 0xAA, 0x00, 0xBD, 0xCE, 0x1A};
const IID ie_DIID_DWebBrowserEvents2 = DIID_DWebBrowserEvents2; //{0xA015A734, 0x8765, 0xD011, 0x92, 0x4A, 0x00, 0x20, 0xAF, 0xC7, 0xAC, 0x4D};
const IID ie_IID_IConnectionPointContainer = IID_IConnectionPointContainer; //{0x84B296B1, 0xB4BA, 0x1A10, 0xB6, 0x9C, 0x00, 0xAA, 0x00, 0x34, 0x1D, 0x07};
const CLSID ie_CLSID_WebBrowser = {0x8856F961, 0x340A, 0x11D0, 0xA9, 0x6B, 0x00, 0xC0, 0x4F, 0xD7, 0x05, 0xA2};
const IID ie_IID_IHTMLDocument2 = {0x25442C33, 0xCB26, 0xD011, 0xB4, 0x83, 0x00, 0xC0, 0x4F, 0xD9, 0x01, 0x19};
const IID ie_IID_IHTMLElement = IID_IHTMLElement;
const IID ie_IID_IHTMLWindow2 = IID_IHTMLWindow2;

typedef interface ie_IHTMLDocument ie_IHTMLDocument;
typedef interface ie_IHTMLDocument2 ie_IHTMLDocument2;
typedef interface ie_IHTMLElementCollection ie_IHTMLElementCollection;
typedef interface ie_IHTMLElement ie_IHTMLElement;

EXTERN_C const IID IID_IPersist;
MIDL_INTERFACE("0000010c-0000-0000-C000-000000000046")
ie_IPersist : public IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE GetClassID( 
        /* [out] */ CLSID *pClassID) = 0;
    
};

EXTERN_C const IID IID_IPersistStreamInit;
MIDL_INTERFACE("7FD52380-4E07-101B-AE2D-08002B2EC713")
ie_IPersistStreamInit : public ie_IPersist
{
public:
	virtual HRESULT STDMETHODCALLTYPE IsDirty( void) = 0;

	virtual HRESULT STDMETHODCALLTYPE Load( 
		/* [in] */ LPSTREAM pStm) = 0;

	virtual HRESULT STDMETHODCALLTYPE Save( 
		/* [in] */ LPSTREAM pStm,
		/* [in] */ BOOL fClearDirty) = 0;

	virtual HRESULT STDMETHODCALLTYPE GetSizeMax( 
		/* [out] */ ULARGE_INTEGER *pCbSize) = 0;

	virtual HRESULT STDMETHODCALLTYPE InitNew( void) = 0;

};


MIDL_INTERFACE("3050f21f-98b5-11cf-bb82-00aa00bdce0b")
ie_IHTMLElementCollection : public IDispatch
{
public:
	virtual /* [id] */ HRESULT STDMETHODCALLTYPE toString( 
		/* [out][retval] */ BSTR *String) = 0;

	virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_length( 
		/* [in] */ long v) = 0;

	virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_length( 
		/* [out][retval] */ long *p) = 0;

	virtual /* [restricted][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE get__newEnum( 
		/* [out][retval] */ IUnknown **p) = 0;

	virtual /* [id] */ HRESULT STDMETHODCALLTYPE item( 
		/* [in][optional] */ VARIANT name,
		/* [in][optional] */ VARIANT index,
		/* [out][retval] */ IDispatch **pdisp) = 0;

	virtual /* [id] */ HRESULT STDMETHODCALLTYPE tags( 
		/* [in] */ VARIANT tagName,
		/* [out][retval] */ IDispatch **pdisp) = 0;
    
};

EXTERN_C const IID IID_IHTMLFramesCollection2;
MIDL_INTERFACE("332c4426-26cb-11d0-b483-00c04fd90119")
ie_IHTMLFramesCollection2 : public IDispatch
{
public:
	virtual /* [id] */ HRESULT STDMETHODCALLTYPE item( 
		/* [in] */ VARIANT *pvarIndex,
		/* [out][retval] */ VARIANT *pvarResult) = 0;

	virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_length( 
		/* [out][retval] */ long *p) = 0;
};

MIDL_INTERFACE("3050f1ff-98b5-11cf-bb82-00aa00bdce0b")
ie_IHTMLElement : public IDispatch
{
public:
	virtual /* [id] */ HRESULT STDMETHODCALLTYPE setAttribute( 
		/* [in] */ BSTR strAttributeName,
		/* [in] */ VARIANT AttributeValue,
		/* [in][defaultvalue] */ LONG lFlags = 1) = 0;

	virtual /* [id] */ HRESULT STDMETHODCALLTYPE getAttribute( 
		/* [in] */ BSTR strAttributeName,
		/* [in][defaultvalue] */ LONG lFlags,
		/* [out][retval] */ VARIANT *AttributeValue) = 0;

	virtual /* [id] */ HRESULT STDMETHODCALLTYPE removeAttribute( 
		/* [in] */ BSTR strAttributeName,
		/* [in][defaultvalue] */ LONG lFlags,
		/* [out][retval] */ VARIANT_BOOL *pfSuccess) = 0;

	virtual /* [bindable][id][propput] */ HRESULT STDMETHODCALLTYPE put_className( 
		/* [in] */ BSTR v) = 0;

	virtual /* [bindable][id][propget] */ HRESULT STDMETHODCALLTYPE get_className( 
		/* [out][retval] */ BSTR *p) = 0;

	virtual /* [bindable][id][propput] */ HRESULT STDMETHODCALLTYPE put_id( 
		/* [in] */ BSTR v) = 0;

	virtual /* [bindable][id][propget] */ HRESULT STDMETHODCALLTYPE get_id( 
		/* [out][retval] */ BSTR *p) = 0;

	virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_tagName( 
		/* [out][retval] */ BSTR *p) = 0;

	virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_parentElement( 
		/* [out][retval] */ ie_IHTMLElement **p) = 0;

	virtual /* [nonbrowsable][id][propget] */ HRESULT STDMETHODCALLTYPE get_style( 
		/* [out][retval] IHTMLStyle */ IDispatch **p) = 0;

	virtual /* [bindable][displaybind][id][propput] */ HRESULT STDMETHODCALLTYPE put_onhelp( 
		/* [in] */ VARIANT v) = 0;

	virtual /* [bindable][displaybind][id][propget] */ HRESULT STDMETHODCALLTYPE get_onhelp( 
		/* [out][retval] */ VARIANT *p) = 0;

	virtual /* [bindable][displaybind][id][propput] */ HRESULT STDMETHODCALLTYPE put_onclick( 
		/* [in] */ VARIANT v) = 0;

	virtual /* [bindable][displaybind][id][propget] */ HRESULT STDMETHODCALLTYPE get_onclick( 
		/* [out][retval] */ VARIANT *p) = 0;

	virtual /* [bindable][displaybind][id][propput] */ HRESULT STDMETHODCALLTYPE put_ondblclick( 
		/* [in] */ VARIANT v) = 0;

	virtual /* [bindable][displaybind][id][propget] */ HRESULT STDMETHODCALLTYPE get_ondblclick( 
		/* [out][retval] */ VARIANT *p) = 0;

	virtual /* [bindable][displaybind][id][propput] */ HRESULT STDMETHODCALLTYPE put_onkeydown( 
		/* [in] */ VARIANT v) = 0;

	virtual /* [bindable][displaybind][id][propget] */ HRESULT STDMETHODCALLTYPE get_onkeydown( 
		/* [out][retval] */ VARIANT *p) = 0;

	virtual /* [bindable][displaybind][id][propput] */ HRESULT STDMETHODCALLTYPE put_onkeyup( 
		/* [in] */ VARIANT v) = 0;

	virtual /* [bindable][displaybind][id][propget] */ HRESULT STDMETHODCALLTYPE get_onkeyup( 
		/* [out][retval] */ VARIANT *p) = 0;

	virtual /* [bindable][displaybind][id][propput] */ HRESULT STDMETHODCALLTYPE put_onkeypress( 
		/* [in] */ VARIANT v) = 0;

	virtual /* [bindable][displaybind][id][propget] */ HRESULT STDMETHODCALLTYPE get_onkeypress( 
		/* [out][retval] */ VARIANT *p) = 0;

	virtual /* [bindable][displaybind][id][propput] */ HRESULT STDMETHODCALLTYPE put_onmouseout( 
		/* [in] */ VARIANT v) = 0;

	virtual /* [bindable][displaybind][id][propget] */ HRESULT STDMETHODCALLTYPE get_onmouseout( 
		/* [out][retval] */ VARIANT *p) = 0;

	virtual /* [bindable][displaybind][id][propput] */ HRESULT STDMETHODCALLTYPE put_onmouseover( 
		/* [in] */ VARIANT v) = 0;

	virtual /* [bindable][displaybind][id][propget] */ HRESULT STDMETHODCALLTYPE get_onmouseover( 
		/* [out][retval] */ VARIANT *p) = 0;

	virtual /* [bindable][displaybind][id][propput] */ HRESULT STDMETHODCALLTYPE put_onmousemove( 
		/* [in] */ VARIANT v) = 0;

	virtual /* [bindable][displaybind][id][propget] */ HRESULT STDMETHODCALLTYPE get_onmousemove( 
		/* [out][retval] */ VARIANT *p) = 0;

	virtual /* [bindable][displaybind][id][propput] */ HRESULT STDMETHODCALLTYPE put_onmousedown( 
		/* [in] */ VARIANT v) = 0;

	virtual /* [bindable][displaybind][id][propget] */ HRESULT STDMETHODCALLTYPE get_onmousedown( 
		/* [out][retval] */ VARIANT *p) = 0;

	virtual /* [bindable][displaybind][id][propput] */ HRESULT STDMETHODCALLTYPE put_onmouseup( 
		/* [in] */ VARIANT v) = 0;

	virtual /* [bindable][displaybind][id][propget] */ HRESULT STDMETHODCALLTYPE get_onmouseup( 
		/* [out][retval] */ VARIANT *p) = 0;

	virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_document( 
		/* [out][retval] */ IDispatch **p) = 0;

	virtual /* [bindable][displaybind][id][propput] */ HRESULT STDMETHODCALLTYPE put_title( 
		/* [in] */ BSTR v) = 0;

	virtual /* [bindable][displaybind][id][propget] */ HRESULT STDMETHODCALLTYPE get_title( 
		/* [out][retval] */ BSTR *p) = 0;

	virtual /* [bindable][displaybind][id][propput] */ HRESULT STDMETHODCALLTYPE put_language( 
		/* [in] */ BSTR v) = 0;

	virtual /* [bindable][displaybind][id][propget] */ HRESULT STDMETHODCALLTYPE get_language( 
		/* [out][retval] */ BSTR *p) = 0;

	virtual /* [bindable][displaybind][id][propput] */ HRESULT STDMETHODCALLTYPE put_onselectstart( 
		/* [in] */ VARIANT v) = 0;

	virtual /* [bindable][displaybind][id][propget] */ HRESULT STDMETHODCALLTYPE get_onselectstart( 
		/* [out][retval] */ VARIANT *p) = 0;

	virtual /* [id] */ HRESULT STDMETHODCALLTYPE scrollIntoView( 
		/* [in][optional] */ VARIANT varargStart) = 0;

	virtual /* [id] */ HRESULT STDMETHODCALLTYPE contains( 
		/* [in] */ ie_IHTMLElement *pChild,
		/* [out][retval] */ VARIANT_BOOL *pfResult) = 0;

	virtual /* [bindable][id][propget] */ HRESULT STDMETHODCALLTYPE get_sourceIndex( 
		/* [out][retval] */ long *p) = 0;

	virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_recordNumber( 
		/* [out][retval] */ VARIANT *p) = 0;

	virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_lang( 
		/* [in] */ BSTR v) = 0;

	virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_lang( 
		/* [out][retval] */ BSTR *p) = 0;

	virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_offsetLeft( 
		/* [out][retval] */ long *p) = 0;

	virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_offsetTop( 
		/* [out][retval] */ long *p) = 0;

	virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_offsetWidth( 
		/* [out][retval] */ long *p) = 0;

	virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_offsetHeight( 
		/* [out][retval] */ long *p) = 0;

	virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_offsetParent( 
		/* [out][retval] */ ie_IHTMLElement **p) = 0;

	virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_innerHTML( 
		/* [in] */ BSTR v) = 0;

	virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_innerHTML( 
		/* [out][retval] */ BSTR *p) = 0;

	virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_innerText( 
		/* [in] */ BSTR v) = 0;

	virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_innerText( 
		/* [out][retval] */ BSTR *p) = 0;

	virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_outerHTML( 
		/* [in] */ BSTR v) = 0;

	virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_outerHTML( 
		/* [out][retval] */ BSTR *p) = 0;

	virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_outerText( 
		/* [in] */ BSTR v) = 0;

	virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_outerText( 
		/* [out][retval] */ BSTR *p) = 0;

	virtual /* [id] */ HRESULT STDMETHODCALLTYPE insertAdjacentHTML( 
		/* [in] */ BSTR where,
		/* [in] */ BSTR html) = 0;

	virtual /* [id] */ HRESULT STDMETHODCALLTYPE insertAdjacentText( 
		/* [in] */ BSTR where,
		/* [in] */ BSTR text) = 0;

	virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_parentTextEdit( 
		/* [out][retval] */ ie_IHTMLElement **p) = 0;

	virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_isTextEdit( 
		/* [out][retval] */ VARIANT_BOOL *p) = 0;

	virtual /* [id] */ HRESULT STDMETHODCALLTYPE click( void) = 0;

	virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_filters( 
		/* [out][retval] IHTMLFiltersCollection */ IDispatch **p) = 0;

	virtual /* [bindable][displaybind][id][propput] */ HRESULT STDMETHODCALLTYPE put_ondragstart( 
		/* [in] */ VARIANT v) = 0;

	virtual /* [bindable][displaybind][id][propget] */ HRESULT STDMETHODCALLTYPE get_ondragstart( 
		/* [out][retval] */ VARIANT *p) = 0;

	virtual /* [id] */ HRESULT STDMETHODCALLTYPE toString( 
		/* [out][retval] */ BSTR *String) = 0;

	virtual /* [bindable][displaybind][id][propput] */ HRESULT STDMETHODCALLTYPE put_onbeforeupdate( 
		/* [in] */ VARIANT v) = 0;

	virtual /* [bindable][displaybind][id][propget] */ HRESULT STDMETHODCALLTYPE get_onbeforeupdate( 
		/* [out][retval] */ VARIANT *p) = 0;

	virtual /* [bindable][displaybind][id][propput] */ HRESULT STDMETHODCALLTYPE put_onafterupdate( 
		/* [in] */ VARIANT v) = 0;

	virtual /* [bindable][displaybind][id][propget] */ HRESULT STDMETHODCALLTYPE get_onafterupdate( 
		/* [out][retval] */ VARIANT *p) = 0;

	virtual /* [bindable][displaybind][id][propput] */ HRESULT STDMETHODCALLTYPE put_onerrorupdate( 
		/* [in] */ VARIANT v) = 0;

	virtual /* [bindable][displaybind][id][propget] */ HRESULT STDMETHODCALLTYPE get_onerrorupdate( 
		/* [out][retval] */ VARIANT *p) = 0;

	virtual /* [bindable][displaybind][id][propput] */ HRESULT STDMETHODCALLTYPE put_onrowexit( 
		/* [in] */ VARIANT v) = 0;

	virtual /* [bindable][displaybind][id][propget] */ HRESULT STDMETHODCALLTYPE get_onrowexit( 
		/* [out][retval] */ VARIANT *p) = 0;

	virtual /* [bindable][displaybind][id][propput] */ HRESULT STDMETHODCALLTYPE put_onrowenter( 
		/* [in] */ VARIANT v) = 0;

	virtual /* [bindable][displaybind][id][propget] */ HRESULT STDMETHODCALLTYPE get_onrowenter( 
		/* [out][retval] */ VARIANT *p) = 0;

	virtual /* [bindable][displaybind][id][propput] */ HRESULT STDMETHODCALLTYPE put_ondatasetchanged( 
		/* [in] */ VARIANT v) = 0;

	virtual /* [bindable][displaybind][id][propget] */ HRESULT STDMETHODCALLTYPE get_ondatasetchanged( 
		/* [out][retval] */ VARIANT *p) = 0;

	virtual /* [bindable][displaybind][id][propput] */ HRESULT STDMETHODCALLTYPE put_ondataavailable( 
		/* [in] */ VARIANT v) = 0;

	virtual /* [bindable][displaybind][id][propget] */ HRESULT STDMETHODCALLTYPE get_ondataavailable( 
		/* [out][retval] */ VARIANT *p) = 0;

	virtual /* [bindable][displaybind][id][propput] */ HRESULT STDMETHODCALLTYPE put_ondatasetcomplete( 
		/* [in] */ VARIANT v) = 0;

	virtual /* [bindable][displaybind][id][propget] */ HRESULT STDMETHODCALLTYPE get_ondatasetcomplete( 
		/* [out][retval] */ VARIANT *p) = 0;

	virtual /* [bindable][displaybind][id][propput] */ HRESULT STDMETHODCALLTYPE put_onfilterchange( 
		/* [in] */ VARIANT v) = 0;

	virtual /* [bindable][displaybind][id][propget] */ HRESULT STDMETHODCALLTYPE get_onfilterchange( 
		/* [out][retval] */ VARIANT *p) = 0;

	virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_children( 
		/* [out][retval] */ IDispatch **p) = 0;

	virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_all( 
		/* [out][retval] */ IDispatch **p) = 0;

};

MIDL_INTERFACE("332c4427-26cb-11d0-b483-00c04fd90119")
ie_IHTMLWindow2 : public ie_IHTMLFramesCollection2
{
public:
	virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_frames( 
	    /* [out][retval] */ ie_IHTMLFramesCollection2 **p) = 0;

	virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_defaultStatus( 
	    /* [in] */ BSTR v) = 0;

	virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_defaultStatus( 
	    /* [out][retval] */ BSTR *p) = 0;

	virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_status( 
	    /* [in] */ BSTR v) = 0;

	virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_status( 
	    /* [out][retval] */ BSTR *p) = 0;

	virtual /* [id] */ HRESULT STDMETHODCALLTYPE setTimeout( 
	    /* [in] */ BSTR expression,
	    /* [in] */ long msec,
	    /* [in][optional] */ VARIANT *language,
	    /* [out][retval] */ long *timerID) = 0;

	virtual /* [id] */ HRESULT STDMETHODCALLTYPE clearTimeout( 
	    /* [in] */ long timerID) = 0;

	virtual /* [id] */ HRESULT STDMETHODCALLTYPE alert( 
	    /* [in][defaultvalue] */ BSTR message = L"") = 0;

	virtual /* [id] */ HRESULT STDMETHODCALLTYPE confirm( 
	    /* [in][defaultvalue] */ BSTR message,
	    /* [out][retval] */ VARIANT_BOOL *confirmed) = 0;

	virtual /* [id] */ HRESULT STDMETHODCALLTYPE prompt( 
	    /* [in][defaultvalue] */ BSTR message,
	    /* [in][defaultvalue] */ BSTR defstr,
	    /* [out][retval] */ VARIANT *textdata) = 0;

	virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Image( 
	    /* [out][retval] IHTMLImageElementFactory */ IDispatch **p) = 0;

	virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_location( 
	    /* [out][retval] IHTMLLocation */ IDispatch **p) = 0;

	virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_history( 
	    /* [out][retval] IOmHistory */ IDispatch **p) = 0;

	virtual /* [id] */ HRESULT STDMETHODCALLTYPE close( void) = 0;

	virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_opener( 
	    /* [in] */ VARIANT v) = 0;

	virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_opener( 
	    /* [out][retval] */ VARIANT *p) = 0;

	virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_navigator( 
	    /* [out][retval] IOmNavigator */ IDispatch **p) = 0;

	virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_name( 
	    /* [in] */ BSTR v) = 0;

	virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_name( 
	    /* [out][retval] */ BSTR *p) = 0;

	virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_parent( 
	    /* [out][retval] */ ie_IHTMLWindow2 **p) = 0;

	virtual /* [id] */ HRESULT STDMETHODCALLTYPE open( 
	    /* [in][defaultvalue] */ BSTR url,
	    /* [in][defaultvalue] */ BSTR name,
	    /* [in][defaultvalue] */ BSTR features,
	    /* [in][defaultvalue] */ VARIANT_BOOL replace,
	    /* [out][retval] */ ie_IHTMLWindow2 **pomWindowResult) = 0;

	virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_self( 
	    /* [out][retval] */ ie_IHTMLWindow2 **p) = 0;

	virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_top( 
	    /* [out][retval] */ ie_IHTMLWindow2 **p) = 0;

	virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_window( 
	    /* [out][retval] */ ie_IHTMLWindow2 **p) = 0;

	virtual /* [id] */ HRESULT STDMETHODCALLTYPE navigate( 
	    /* [in] */ BSTR url) = 0;

	virtual /* [bindable][displaybind][id][propput] */ HRESULT STDMETHODCALLTYPE put_onfocus( 
	    /* [in] */ VARIANT v) = 0;

	virtual /* [bindable][displaybind][id][propget] */ HRESULT STDMETHODCALLTYPE get_onfocus( 
	    /* [out][retval] */ VARIANT *p) = 0;

	virtual /* [bindable][displaybind][id][propput] */ HRESULT STDMETHODCALLTYPE put_onblur( 
	    /* [in] */ VARIANT v) = 0;

	virtual /* [bindable][displaybind][id][propget] */ HRESULT STDMETHODCALLTYPE get_onblur( 
	    /* [out][retval] */ VARIANT *p) = 0;

	virtual /* [bindable][displaybind][id][propput] */ HRESULT STDMETHODCALLTYPE put_onload( 
	    /* [in] */ VARIANT v) = 0;

	virtual /* [bindable][displaybind][id][propget] */ HRESULT STDMETHODCALLTYPE get_onload( 
	    /* [out][retval] */ VARIANT *p) = 0;

	virtual /* [bindable][displaybind][id][propput] */ HRESULT STDMETHODCALLTYPE put_onbeforeunload( 
	    /* [in] */ VARIANT v) = 0;

	virtual /* [bindable][displaybind][id][propget] */ HRESULT STDMETHODCALLTYPE get_onbeforeunload( 
	    /* [out][retval] */ VARIANT *p) = 0;

	virtual /* [bindable][displaybind][id][propput] */ HRESULT STDMETHODCALLTYPE put_onunload( 
	    /* [in] */ VARIANT v) = 0;

	virtual /* [bindable][displaybind][id][propget] */ HRESULT STDMETHODCALLTYPE get_onunload( 
	    /* [out][retval] */ VARIANT *p) = 0;

	virtual /* [bindable][displaybind][id][propput] */ HRESULT STDMETHODCALLTYPE put_onhelp( 
	    /* [in] */ VARIANT v) = 0;

	virtual /* [bindable][displaybind][id][propget] */ HRESULT STDMETHODCALLTYPE get_onhelp( 
	    /* [out][retval] */ VARIANT *p) = 0;

	virtual /* [bindable][displaybind][id][propput] */ HRESULT STDMETHODCALLTYPE put_onerror( 
	    /* [in] */ VARIANT v) = 0;

	virtual /* [bindable][displaybind][id][propget] */ HRESULT STDMETHODCALLTYPE get_onerror( 
	    /* [out][retval] */ VARIANT *p) = 0;

	virtual /* [bindable][displaybind][id][propput] */ HRESULT STDMETHODCALLTYPE put_onresize( 
	    /* [in] */ VARIANT v) = 0;

	virtual /* [bindable][displaybind][id][propget] */ HRESULT STDMETHODCALLTYPE get_onresize( 
	    /* [out][retval] */ VARIANT *p) = 0;

	virtual /* [bindable][displaybind][id][propput] */ HRESULT STDMETHODCALLTYPE put_onscroll( 
	    /* [in] */ VARIANT v) = 0;

	virtual /* [bindable][displaybind][id][propget] */ HRESULT STDMETHODCALLTYPE get_onscroll( 
	    /* [out][retval] */ VARIANT *p) = 0;

	virtual /* [source][id][propget] */ HRESULT STDMETHODCALLTYPE get_document( 
	    /* [out][retval] */ ie_IHTMLDocument2 **p) = 0;

	virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_event( 
	    /* [out][retval] IHTMLEventObj */ IDispatch **p) = 0;

	virtual /* [restricted][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE get__newEnum( 
	    /* [out][retval] */ IUnknown **p) = 0;

	virtual /* [id] */ HRESULT STDMETHODCALLTYPE showModalDialog( 
	    /* [in] */ BSTR dialog,
	    /* [in][optional] */ VARIANT *varArgIn,
	    /* [in][optional] */ VARIANT *varOptions,
	    /* [out][retval] */ VARIANT *varArgOut) = 0;

	virtual /* [id] */ HRESULT STDMETHODCALLTYPE showHelp( 
	    /* [in] */ BSTR helpURL,
	    /* [in][optional] */ VARIANT helpArg,
	    /* [in][defaultvalue] */ BSTR features = L"") = 0;

	virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_screen( 
	    /* [out][retval] IHTMLScreen */ IDispatch **p) = 0;

	virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Option( 
	    /* [out][retval] IHTMLOptionElementFactory */ IDispatch **p) = 0;

	virtual /* [id] */ HRESULT STDMETHODCALLTYPE focus( void) = 0;

	virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_closed( 
	    /* [out][retval] */ VARIANT_BOOL *p) = 0;

	virtual /* [id] */ HRESULT STDMETHODCALLTYPE blur( void) = 0;

	virtual /* [id] */ HRESULT STDMETHODCALLTYPE scroll( 
	    /* [in] */ long x,
	    /* [in] */ long y) = 0;

	virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_clientInformation( 
	    /* [out][retval] IOmNavigator  */ IDispatch **p) = 0;

	virtual /* [id] */ HRESULT STDMETHODCALLTYPE setInterval( 
	    /* [in] */ BSTR expression,
	    /* [in] */ long msec,
	    /* [in][optional] */ VARIANT *language,
	    /* [out][retval] */ long *timerID) = 0;

	virtual /* [id] */ HRESULT STDMETHODCALLTYPE clearInterval( 
	    /* [in] */ long timerID) = 0;

	virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_offscreenBuffering( 
	    /* [in] */ VARIANT v) = 0;

	virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_offscreenBuffering( 
	    /* [out][retval] */ VARIANT *p) = 0;

	virtual /* [id] */ HRESULT STDMETHODCALLTYPE execScript( 
	    /* [in] */ BSTR code,
	    /* [in][defaultvalue] */ BSTR language,
	    /* [out][retval] */ VARIANT *pvarRet) = 0;

	virtual /* [id] */ HRESULT STDMETHODCALLTYPE toString( 
	    /* [out][retval] */ BSTR *String) = 0;

	virtual /* [id] */ HRESULT STDMETHODCALLTYPE scrollBy( 
	    /* [in] */ long x,
	    /* [in] */ long y) = 0;

	virtual /* [id] */ HRESULT STDMETHODCALLTYPE scrollTo( 
	    /* [in] */ long x,
	    /* [in] */ long y) = 0;

	virtual /* [id] */ HRESULT STDMETHODCALLTYPE moveTo( 
	    /* [in] */ long x,
	    /* [in] */ long y) = 0;

	virtual /* [id] */ HRESULT STDMETHODCALLTYPE moveBy( 
	    /* [in] */ long x,
	    /* [in] */ long y) = 0;

	virtual /* [id] */ HRESULT STDMETHODCALLTYPE resizeTo( 
	    /* [in] */ long x,
	    /* [in] */ long y) = 0;

	virtual /* [id] */ HRESULT STDMETHODCALLTYPE resizeBy( 
	    /* [in] */ long x,
	    /* [in] */ long y) = 0;

	virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_external( 
	    /* [out][retval] */ IDispatch **p) = 0;
};

MIDL_INTERFACE("3050f3f0-98b5-11cf-bb82-00aa00bdce0b")
ie_ICustomDoc : public IUnknown
{
public:
	virtual HRESULT STDMETHODCALLTYPE SetUIHandler(/* [in] */ IUnknown *pUIHandler) = 0;
};

MIDL_INTERFACE("EAB22AC1-30C1-11CF-A7EB-0000C05BAE0B")
ie_IWebBrowser : public IDispatch
{
public:
	virtual /* [helpcontext][helpstring][id] */ HRESULT STDMETHODCALLTYPE GoBack( void) = 0;
	virtual /* [helpcontext][helpstring][id] */ HRESULT STDMETHODCALLTYPE GoForward( void) = 0;
	virtual /* [helpcontext][helpstring][id] */ HRESULT STDMETHODCALLTYPE GoHome( void) = 0;
	virtual /* [helpcontext][helpstring][id] */ HRESULT STDMETHODCALLTYPE GoSearch( void) = 0;
	virtual /* [helpcontext][helpstring][id] */ HRESULT STDMETHODCALLTYPE Navigate( 
		/* [in] */ BSTR URL,
		/* [optional][in] */ VARIANT *Flags,
		/* [optional][in] */ VARIANT *TargetFrameName,
		/* [optional][in] */ VARIANT *PostData,
		/* [optional][in] */ VARIANT *Headers) = 0;
	virtual /* [helpcontext][helpstring][id] */ HRESULT STDMETHODCALLTYPE Refresh( void) = 0;
	virtual /* [helpcontext][helpstring][id] */ HRESULT STDMETHODCALLTYPE Refresh2( 
		/* [optional][in] */ VARIANT *Level) = 0;
	virtual /* [helpcontext][helpstring][id] */ HRESULT STDMETHODCALLTYPE Stop( void) = 0;
	virtual /* [helpcontext][helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_Application( 
		/* [retval][out] */ IDispatch **ppDisp) = 0;
	virtual /* [helpcontext][helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_Parent( 
		/* [retval][out] */ IDispatch **ppDisp) = 0;
	virtual /* [helpcontext][helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_Container( 
		/* [retval][out] */ IDispatch **ppDisp) = 0;
	virtual /* [helpcontext][helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_Document( 
		/* [retval][out] */ IDispatch **ppDisp) = 0;
	virtual /* [helpcontext][helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_TopLevelContainer( 
		/* [retval][out] */ VARIANT_BOOL *pBool) = 0;
	virtual /* [helpcontext][helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_Type( 
		/* [retval][out] */ BSTR *Type) = 0;
	virtual /* [helpcontext][helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_Left( 
		/* [retval][out] */ long *pl) = 0;
	virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_Left( 
		/* [in] */ long Left) = 0;
	virtual /* [helpcontext][helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_Top( 
		/* [retval][out] */ long *pl) = 0;
	virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_Top( 
		/* [in] */ long Top) = 0;
	virtual /* [helpcontext][helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_Width( 
		/* [retval][out] */ long *pl) = 0;
	virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_Width( 
		/* [in] */ long Width) = 0;
	virtual /* [helpcontext][helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_Height( 
		/* [retval][out] */ long *pl) = 0;
	virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_Height( 
		/* [in] */ long Height) = 0;
	virtual /* [helpcontext][helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_LocationName( 
		/* [retval][out] */ BSTR *LocationName) = 0;
	virtual /* [helpcontext][helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_LocationURL( 
		/* [retval][out] */ BSTR *LocationURL) = 0;
	virtual /* [helpcontext][helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_Busy( 
		/* [retval][out] */ VARIANT_BOOL *pBool) = 0;
};

MIDL_INTERFACE("0002DF05-0000-0000-C000-000000000046")
ie_IWebBrowserApp : public ie_IWebBrowser
{
public:
	virtual /* [helpcontext][helpstring][id] */ HRESULT STDMETHODCALLTYPE Quit( void) = 0;
	virtual /* [helpcontext][helpstring][id] */ HRESULT STDMETHODCALLTYPE ClientToWindow( 
		/* [out][in] */ int *pcx,
		/* [out][in] */ int *pcy) = 0;
	virtual /* [helpcontext][helpstring][id] */ HRESULT STDMETHODCALLTYPE PutProperty( 
		/* [in] */ BSTR Property,
		/* [in] */ VARIANT vtValue) = 0;
	virtual /* [helpcontext][helpstring][id] */ HRESULT STDMETHODCALLTYPE GetProperty( 
		/* [in] */ BSTR Property,
		/* [retval][out] */ VARIANT *pvtValue) = 0;
	virtual /* [helpcontext][helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_Name( 
		/* [retval][out] */ BSTR *Name) = 0;
	virtual /* [helpcontext][helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_HWND( 
		/* [retval][out] */ SHANDLE_PTR *pHWND) = 0;
	virtual /* [helpcontext][helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_FullName( 
		/* [retval][out] */ BSTR *FullName) = 0;
	virtual /* [helpcontext][helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_Path( 
		/* [retval][out] */ BSTR *Path) = 0;
	virtual /* [helpcontext][helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_Visible( 
		/* [retval][out] */ VARIANT_BOOL *pBool) = 0;
	virtual /* [helpcontext][helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_Visible( 
		/* [in] */ VARIANT_BOOL Value) = 0;
	virtual /* [helpcontext][helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_StatusBar( 
		/* [retval][out] */ VARIANT_BOOL *pBool) = 0;
	virtual /* [helpcontext][helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_StatusBar( 
		/* [in] */ VARIANT_BOOL Value) = 0;
	virtual /* [helpcontext][helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_StatusText( 
		/* [retval][out] */ BSTR *StatusText) = 0;
	virtual /* [helpcontext][helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_StatusText( 
		/* [in] */ BSTR StatusText) = 0;
	virtual /* [helpcontext][helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_ToolBar( 
		/* [retval][out] */ int *Value) = 0;
	virtual /* [helpcontext][helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_ToolBar( 
		/* [in] */ int Value) = 0;
	virtual /* [helpcontext][helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_MenuBar( 
		/* [retval][out] */ VARIANT_BOOL *Value) = 0;
	virtual /* [helpcontext][helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_MenuBar( 
		/* [in] */ VARIANT_BOOL Value) = 0;
	virtual /* [helpcontext][helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_FullScreen( 
		/* [retval][out] */ VARIANT_BOOL *pbFullScreen) = 0;
	virtual /* [helpcontext][helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_FullScreen( 
		/* [in] */ VARIANT_BOOL bFullScreen) = 0;
};

typedef 
enum ie_tagOLECMDEXECOPT
    {	OLECMDEXECOPT_DODEFAULT	= 0,
	OLECMDEXECOPT_PROMPTUSER	= 1,
	OLECMDEXECOPT_DONTPROMPTUSER	= 2,
	OLECMDEXECOPT_SHOWHELP	= 3
    } 	ie_OLECMDEXECOPT;

typedef 
enum ie_tagOLECMDF
    {	OLECMDF_SUPPORTED	= 0x1,
	OLECMDF_ENABLED	= 0x2,
	OLECMDF_LATCHED	= 0x4,
	OLECMDF_NINCHED	= 0x8,
	OLECMDF_INVISIBLE	= 0x10,
	OLECMDF_DEFHIDEONCTXTMENU	= 0x20
    } 	ie_OLECMDF;

/* OLECMDID_STOPDOWNLOAD and OLECMDID_ALLOWUILESSSAVEAS are supported for QueryStatus Only */
typedef 
enum ie_tagOLECMDID
    {	OLECMDID_OPEN	= 1,
	OLECMDID_NEW	= 2,
	OLECMDID_SAVE	= 3,
	OLECMDID_SAVEAS	= 4,
	OLECMDID_SAVECOPYAS	= 5,
	OLECMDID_PRINT	= 6,
	OLECMDID_PRINTPREVIEW	= 7,
	OLECMDID_PAGESETUP	= 8,
	OLECMDID_SPELL	= 9,
	OLECMDID_PROPERTIES	= 10,
	OLECMDID_CUT	= 11,
	OLECMDID_COPY	= 12,
	OLECMDID_PASTE	= 13,
	OLECMDID_PASTESPECIAL	= 14,
	OLECMDID_UNDO	= 15,
	OLECMDID_REDO	= 16,
	OLECMDID_SELECTALL	= 17,
	OLECMDID_CLEARSELECTION	= 18,
	OLECMDID_ZOOM	= 19,
	OLECMDID_GETZOOMRANGE	= 20,
	OLECMDID_UPDATECOMMANDS	= 21,
	OLECMDID_REFRESH	= 22,
	OLECMDID_STOP	= 23,
	OLECMDID_HIDETOOLBARS	= 24,
	OLECMDID_SETPROGRESSMAX	= 25,
	OLECMDID_SETPROGRESSPOS	= 26,
	OLECMDID_SETPROGRESSTEXT	= 27,
	OLECMDID_SETTITLE	= 28,
	OLECMDID_SETDOWNLOADSTATE	= 29,
	OLECMDID_STOPDOWNLOAD	= 30,
	OLECMDID_ONTOOLBARACTIVATED	= 31,
	OLECMDID_FIND	= 32,
	OLECMDID_DELETE	= 33,
	OLECMDID_HTTPEQUIV	= 34,
	OLECMDID_HTTPEQUIV_DONE	= 35,
	OLECMDID_ENABLE_INTERACTION	= 36,
	OLECMDID_ONUNLOAD	= 37,
	OLECMDID_PROPERTYBAG2	= 38,
	OLECMDID_PREREFRESH	= 39,
	OLECMDID_SHOWSCRIPTERROR	= 40,
	OLECMDID_SHOWMESSAGE	= 41,
	OLECMDID_SHOWFIND	= 42,
	OLECMDID_SHOWPAGESETUP	= 43,
	OLECMDID_SHOWPRINT	= 44,
	OLECMDID_CLOSE	= 45,
	OLECMDID_ALLOWUILESSSAVEAS	= 46,
	OLECMDID_DONTDOWNLOADCSS	= 47,
	OLECMDID_UPDATEPAGESTATUS	= 48,
	OLECMDID_PRINT2	= 49,
	OLECMDID_PRINTPREVIEW2	= 50,
	OLECMDID_SETPRINTTEMPLATE	= 51,
	OLECMDID_GETPRINTTEMPLATE	= 52
    } 	ie_OLECMDID;

/* State values for the DISPID_READYSTATE property */
typedef /* [v1_enum] */ 
enum ie_tagREADYSTATE
    {	READYSTATE_UNINITIALIZED	= 0,
	READYSTATE_LOADING	= 1,
	READYSTATE_LOADED	= 2,
	READYSTATE_INTERACTIVE	= 3,
	READYSTATE_COMPLETE	= 4
    } 	ie_READYSTATE;

MIDL_INTERFACE("D30C1661-CDAF-11d0-8A3E-00C04FC9E26E")
ie_IWebBrowser2 : public ie_IWebBrowserApp
{
public:
	virtual /* [helpcontext][helpstring][id] */ HRESULT STDMETHODCALLTYPE Navigate2( 
		/* [in] */ VARIANT *URL,
		/* [optional][in] */ VARIANT *Flags,
		/* [optional][in] */ VARIANT *TargetFrameName,
		/* [optional][in] */ VARIANT *PostData,
		/* [optional][in] */ VARIANT *Headers) = 0;
	virtual /* [helpcontext][helpstring][id] */ HRESULT STDMETHODCALLTYPE QueryStatusWB( 
		/* [in] */ ie_OLECMDID cmdID,
		/* [retval][out] */ ie_OLECMDF *pcmdf) = 0;
	virtual /* [helpcontext][helpstring][id] */ HRESULT STDMETHODCALLTYPE ExecWB( 
		/* [in] */ ie_OLECMDID cmdID,
		/* [in] */ ie_OLECMDEXECOPT cmdexecopt,
		/* [optional][in] */ VARIANT *pvaIn,
		/* [optional][in][out] */ VARIANT *pvaOut) = 0;
	virtual /* [helpcontext][helpstring][id] */ HRESULT STDMETHODCALLTYPE ShowBrowserBar( 
		/* [in] */ VARIANT *pvaClsid,
		/* [optional][in] */ VARIANT *pvarShow,
		/* [optional][in] */ VARIANT *pvarSize) = 0;
	virtual /* [bindable][propget][id] */ HRESULT STDMETHODCALLTYPE get_ReadyState( 
		/* [out][retval] */ ie_READYSTATE *plReadyState) = 0;
	virtual /* [helpcontext][helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_Offline( 
		/* [retval][out] */ VARIANT_BOOL *pbOffline) = 0;
	virtual /* [helpcontext][helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_Offline( 
		/* [in] */ VARIANT_BOOL bOffline) = 0;
	virtual /* [helpcontext][helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_Silent( 
		/* [retval][out] */ VARIANT_BOOL *pbSilent) = 0;
	virtual /* [helpcontext][helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_Silent( 
		/* [in] */ VARIANT_BOOL bSilent) = 0;
	virtual /* [helpcontext][helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_RegisterAsBrowser( 
		/* [retval][out] */ VARIANT_BOOL *pbRegister) = 0;
	virtual /* [helpcontext][helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_RegisterAsBrowser( 
		/* [in] */ VARIANT_BOOL bRegister) = 0;
	virtual /* [helpcontext][helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_RegisterAsDropTarget( 
		/* [retval][out] */ VARIANT_BOOL *pbRegister) = 0;
	virtual /* [helpcontext][helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_RegisterAsDropTarget( 
		/* [in] */ VARIANT_BOOL bRegister) = 0;
	virtual /* [helpcontext][helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_TheaterMode( 
		/* [retval][out] */ VARIANT_BOOL *pbRegister) = 0;
	virtual /* [helpcontext][helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_TheaterMode( 
		/* [in] */ VARIANT_BOOL bRegister) = 0;
	virtual /* [helpcontext][helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_AddressBar( 
		/* [retval][out] */ VARIANT_BOOL *Value) = 0;
	virtual /* [helpcontext][helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_AddressBar( 
		/* [in] */ VARIANT_BOOL Value) = 0;
	virtual /* [helpcontext][helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_Resizable( 
		/* [retval][out] */ VARIANT_BOOL *Value) = 0;
	virtual /* [helpcontext][helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_Resizable( 
		/* [in] */ VARIANT_BOOL Value) = 0;
};

//===========================================================cc::win::for ie_IHTMLDocument
MIDL_INTERFACE("626FC520-A41E-11cf-A731-00A0C9082637")
ie_IHTMLDocument : public IDispatch
{
public:
	virtual /* [nonbrowsable][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE get_Script( 
		/* [out][retval] */ IDispatch **p) = 0;
    
};

//===========================================================cc::win::for ie_IHTMLDocument2
MIDL_INTERFACE("332c4425-26cb-11d0-b483-00c04fd90119")
ie_IHTMLDocument2 : public ie_IHTMLDocument
{
public:
	virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_all( 
		/* [out][retval] */ ie_IHTMLElementCollection **p) = 0;

	virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_body( 
		/* [out][retval] */ ie_IHTMLElement **p) = 0;

	virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_activeElement( 
		/* [out][retval] */ ie_IHTMLElement **p) = 0;

	virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_images( 
		/* [out][retval] */ ie_IHTMLElementCollection **p) = 0;

	virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_applets( 
		/* [out][retval] */ ie_IHTMLElementCollection **p) = 0;

	virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_links( 
		/* [out][retval] */ ie_IHTMLElementCollection **p) = 0;

	virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_forms( 
		/* [out][retval] */ ie_IHTMLElementCollection **p) = 0;

	virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_anchors( 
		/* [out][retval] */ ie_IHTMLElementCollection **p) = 0;

	virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_title( 
		/* [in] */ BSTR v) = 0;

	virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_title( 
		/* [out][retval] */ BSTR *p) = 0;

	virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_scripts( 
		/* [out][retval] */ ie_IHTMLElementCollection **p) = 0;

	virtual /* [hidden][id][propput] */ HRESULT STDMETHODCALLTYPE put_designMode( 
		/* [in] */ BSTR v) = 0;

	virtual /* [hidden][id][propget] */ HRESULT STDMETHODCALLTYPE get_designMode( 
		/* [out][retval] */ BSTR *p) = 0;

	virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_selection( 
		/* [out][retval] IHTMLSelectionObject */ IDispatch **p) = 0;

	virtual /* [bindable][id][propget] */ HRESULT STDMETHODCALLTYPE get_readyState( 
		/* [out][retval] */ BSTR *p) = 0;

	virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_frames( 
		/* [out][retval] */ ie_IHTMLFramesCollection2 **p) = 0;

	virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_embeds( 
		/* [out][retval] */ ie_IHTMLElementCollection **p) = 0;

	virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_plugins( 
		/* [out][retval] */ ie_IHTMLElementCollection **p) = 0;

	virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_alinkColor( 
		/* [in] */ VARIANT v) = 0;

	virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_alinkColor( 
		/* [out][retval] */ VARIANT *p) = 0;

	virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_bgColor( 
		/* [in] */ VARIANT v) = 0;

	virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_bgColor( 
		/* [out][retval] */ VARIANT *p) = 0;

	virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_fgColor( 
		/* [in] */ VARIANT v) = 0;

	virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_fgColor( 
		/* [out][retval] */ VARIANT *p) = 0;

	virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_linkColor( 
		/* [in] */ VARIANT v) = 0;

	virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_linkColor( 
		/* [out][retval] */ VARIANT *p) = 0;

	virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_vlinkColor( 
		/* [in] */ VARIANT v) = 0;

	virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_vlinkColor( 
		/* [out][retval] */ VARIANT *p) = 0;

	virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_referrer( 
		/* [out][retval] */ BSTR *p) = 0;

	virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_location( 
		/* [out][retval] IHTMLLocation */ IDispatch **p) = 0;

	virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_lastModified( 
		/* [out][retval] */ BSTR *p) = 0;

	virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_URL( 
		/* [in] */ BSTR v) = 0;

	virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_URL( 
		/* [out][retval] */ BSTR *p) = 0;

	virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_domain( 
		/* [in] */ BSTR v) = 0;

	virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_domain( 
		/* [out][retval] */ BSTR *p) = 0;

	virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_cookie( 
		/* [in] */ BSTR v) = 0;

	virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_cookie( 
		/* [out][retval] */ BSTR *p) = 0;

	virtual /* [hidden][bindable][id][propput] */ HRESULT STDMETHODCALLTYPE put_expando( 
		/* [in] */ VARIANT_BOOL v) = 0;

	virtual /* [hidden][bindable][id][propget] */ HRESULT STDMETHODCALLTYPE get_expando( 
		/* [out][retval] */ VARIANT_BOOL *p) = 0;

	virtual /* [hidden][id][propput] */ HRESULT STDMETHODCALLTYPE put_charset( 
		/* [in] */ BSTR v) = 0;

	virtual /* [hidden][id][propget] */ HRESULT STDMETHODCALLTYPE get_charset( 
		/* [out][retval] */ BSTR *p) = 0;

	virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_defaultCharset( 
		/* [in] */ BSTR v) = 0;

	virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_defaultCharset( 
		/* [out][retval] */ BSTR *p) = 0;

	virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_mimeType( 
		/* [out][retval] */ BSTR *p) = 0;

	virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_fileSize( 
		/* [out][retval] */ BSTR *p) = 0;

	virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_fileCreatedDate( 
		/* [out][retval] */ BSTR *p) = 0;

	virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_fileModifiedDate( 
		/* [out][retval] */ BSTR *p) = 0;

	virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_fileUpdatedDate( 
		/* [out][retval] */ BSTR *p) = 0;

	virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_security( 
		/* [out][retval] */ BSTR *p) = 0;

	virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_protocol( 
		/* [out][retval] */ BSTR *p) = 0;

	virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_nameProp( 
		/* [out][retval] */ BSTR *p) = 0;

	virtual /* [id][vararg] */ HRESULT STDMETHODCALLTYPE write( 
		/* [in] */ SAFEARRAY * psarray) = 0;

	virtual /* [id][vararg] */ HRESULT STDMETHODCALLTYPE writeln( 
		/* [in] */ SAFEARRAY * psarray) = 0;

	virtual /* [id] */ HRESULT STDMETHODCALLTYPE open( 
		/* [in][defaultvalue] */ BSTR url,
		/* [in][optional] */ VARIANT name,
		/* [in][optional] */ VARIANT features,
		/* [in][optional] */ VARIANT replace,
		/* [out][retval] */ IDispatch **pomWindowResult) = 0;

	virtual /* [id] */ HRESULT STDMETHODCALLTYPE close( void) = 0;

	virtual /* [id] */ HRESULT STDMETHODCALLTYPE clear( void) = 0;

	virtual /* [id] */ HRESULT STDMETHODCALLTYPE queryCommandSupported( 
		/* [in] */ BSTR cmdID,
		/* [out][retval] */ VARIANT_BOOL *pfRet) = 0;

	virtual /* [id] */ HRESULT STDMETHODCALLTYPE queryCommandEnabled( 
		/* [in] */ BSTR cmdID,
		/* [out][retval] */ VARIANT_BOOL *pfRet) = 0;

	virtual /* [id] */ HRESULT STDMETHODCALLTYPE queryCommandState( 
		/* [in] */ BSTR cmdID,
		/* [out][retval] */ VARIANT_BOOL *pfRet) = 0;

	virtual /* [id] */ HRESULT STDMETHODCALLTYPE queryCommandIndeterm( 
		/* [in] */ BSTR cmdID,
		/* [out][retval] */ VARIANT_BOOL *pfRet) = 0;

	virtual /* [id] */ HRESULT STDMETHODCALLTYPE queryCommandText( 
		/* [in] */ BSTR cmdID,
		/* [out][retval] */ BSTR *pcmdText) = 0;

	virtual /* [id] */ HRESULT STDMETHODCALLTYPE queryCommandValue( 
		/* [in] */ BSTR cmdID,
		/* [out][retval] */ VARIANT *pcmdValue) = 0;

	virtual /* [id] */ HRESULT STDMETHODCALLTYPE execCommand( 
		/* [in] */ BSTR cmdID,
		/* [in][defaultvalue] */ VARIANT_BOOL showUI,
		/* [in][optional] */ VARIANT value,
		/* [out][retval] */ VARIANT_BOOL *pfRet) = 0;

	virtual /* [id] */ HRESULT STDMETHODCALLTYPE execCommandShowHelp( 
		/* [in] */ BSTR cmdID,
		/* [out][retval] */ VARIANT_BOOL *pfRet) = 0;

	virtual /* [id] */ HRESULT STDMETHODCALLTYPE createElement( 
		/* [in] */ BSTR eTag,
		/* [out][retval] */ ie_IHTMLElement **newElem) = 0;

	virtual /* [bindable][displaybind][id][propput] */ HRESULT STDMETHODCALLTYPE put_onhelp( 
		/* [in] */ VARIANT v) = 0;

	virtual /* [bindable][displaybind][id][propget] */ HRESULT STDMETHODCALLTYPE get_onhelp( 
		/* [out][retval] */ VARIANT *p) = 0;

	virtual /* [bindable][displaybind][id][propput] */ HRESULT STDMETHODCALLTYPE put_onclick( 
		/* [in] */ VARIANT v) = 0;

	virtual /* [bindable][displaybind][id][propget] */ HRESULT STDMETHODCALLTYPE get_onclick( 
		/* [out][retval] */ VARIANT *p) = 0;

	virtual /* [bindable][displaybind][id][propput] */ HRESULT STDMETHODCALLTYPE put_ondblclick( 
		/* [in] */ VARIANT v) = 0;

	virtual /* [bindable][displaybind][id][propget] */ HRESULT STDMETHODCALLTYPE get_ondblclick( 
		/* [out][retval] */ VARIANT *p) = 0;

	virtual /* [bindable][displaybind][id][propput] */ HRESULT STDMETHODCALLTYPE put_onkeyup( 
		/* [in] */ VARIANT v) = 0;

	virtual /* [bindable][displaybind][id][propget] */ HRESULT STDMETHODCALLTYPE get_onkeyup( 
		/* [out][retval] */ VARIANT *p) = 0;

	virtual /* [bindable][displaybind][id][propput] */ HRESULT STDMETHODCALLTYPE put_onkeydown( 
		/* [in] */ VARIANT v) = 0;

	virtual /* [bindable][displaybind][id][propget] */ HRESULT STDMETHODCALLTYPE get_onkeydown( 
		/* [out][retval] */ VARIANT *p) = 0;

	virtual /* [bindable][displaybind][id][propput] */ HRESULT STDMETHODCALLTYPE put_onkeypress( 
		/* [in] */ VARIANT v) = 0;

	virtual /* [bindable][displaybind][id][propget] */ HRESULT STDMETHODCALLTYPE get_onkeypress( 
		/* [out][retval] */ VARIANT *p) = 0;

	virtual /* [bindable][displaybind][id][propput] */ HRESULT STDMETHODCALLTYPE put_onmouseup( 
		/* [in] */ VARIANT v) = 0;

	virtual /* [bindable][displaybind][id][propget] */ HRESULT STDMETHODCALLTYPE get_onmouseup( 
		/* [out][retval] */ VARIANT *p) = 0;

	virtual /* [bindable][displaybind][id][propput] */ HRESULT STDMETHODCALLTYPE put_onmousedown( 
		/* [in] */ VARIANT v) = 0;

	virtual /* [bindable][displaybind][id][propget] */ HRESULT STDMETHODCALLTYPE get_onmousedown( 
		/* [out][retval] */ VARIANT *p) = 0;

	virtual /* [bindable][displaybind][id][propput] */ HRESULT STDMETHODCALLTYPE put_onmousemove( 
		/* [in] */ VARIANT v) = 0;

	virtual /* [bindable][displaybind][id][propget] */ HRESULT STDMETHODCALLTYPE get_onmousemove( 
		/* [out][retval] */ VARIANT *p) = 0;

	virtual /* [bindable][displaybind][id][propput] */ HRESULT STDMETHODCALLTYPE put_onmouseout( 
		/* [in] */ VARIANT v) = 0;

	virtual /* [bindable][displaybind][id][propget] */ HRESULT STDMETHODCALLTYPE get_onmouseout( 
		/* [out][retval] */ VARIANT *p) = 0;

	virtual /* [bindable][displaybind][id][propput] */ HRESULT STDMETHODCALLTYPE put_onmouseover( 
		/* [in] */ VARIANT v) = 0;

	virtual /* [bindable][displaybind][id][propget] */ HRESULT STDMETHODCALLTYPE get_onmouseover( 
		/* [out][retval] */ VARIANT *p) = 0;

	virtual /* [bindable][displaybind][id][propput] */ HRESULT STDMETHODCALLTYPE put_onreadystatechange( 
		/* [in] */ VARIANT v) = 0;

	virtual /* [bindable][displaybind][id][propget] */ HRESULT STDMETHODCALLTYPE get_onreadystatechange( 
		/* [out][retval] */ VARIANT *p) = 0;

	virtual /* [bindable][displaybind][id][propput] */ HRESULT STDMETHODCALLTYPE put_onafterupdate( 
		/* [in] */ VARIANT v) = 0;

	virtual /* [bindable][displaybind][id][propget] */ HRESULT STDMETHODCALLTYPE get_onafterupdate( 
		/* [out][retval] */ VARIANT *p) = 0;

	virtual /* [bindable][displaybind][id][propput] */ HRESULT STDMETHODCALLTYPE put_onrowexit( 
		/* [in] */ VARIANT v) = 0;

	virtual /* [bindable][displaybind][id][propget] */ HRESULT STDMETHODCALLTYPE get_onrowexit( 
		/* [out][retval] */ VARIANT *p) = 0;

	virtual /* [bindable][displaybind][id][propput] */ HRESULT STDMETHODCALLTYPE put_onrowenter( 
		/* [in] */ VARIANT v) = 0;

	virtual /* [bindable][displaybind][id][propget] */ HRESULT STDMETHODCALLTYPE get_onrowenter( 
		/* [out][retval] */ VARIANT *p) = 0;

	virtual /* [bindable][displaybind][id][propput] */ HRESULT STDMETHODCALLTYPE put_ondragstart( 
		/* [in] */ VARIANT v) = 0;

	virtual /* [bindable][displaybind][id][propget] */ HRESULT STDMETHODCALLTYPE get_ondragstart( 
		/* [out][retval] */ VARIANT *p) = 0;

	virtual /* [bindable][displaybind][id][propput] */ HRESULT STDMETHODCALLTYPE put_onselectstart( 
		/* [in] */ VARIANT v) = 0;

	virtual /* [bindable][displaybind][id][propget] */ HRESULT STDMETHODCALLTYPE get_onselectstart( 
		/* [out][retval] */ VARIANT *p) = 0;

	virtual /* [id] */ HRESULT STDMETHODCALLTYPE elementFromPoint( 
		/* [in] */ long x,
		/* [in] */ long y,
		/* [out][retval] */ ie_IHTMLElement **elementHit) = 0;

	virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_parentWindow( 
		/* [out][retval] */ ie_IHTMLWindow2 **p) = 0;

	virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_styleSheets( 
		/* [out][retval] IHTMLStyleSheetsCollection */ IDispatch **p) = 0;

	virtual /* [bindable][displaybind][id][propput] */ HRESULT STDMETHODCALLTYPE put_onbeforeupdate( 
		/* [in] */ VARIANT v) = 0;

	virtual /* [bindable][displaybind][id][propget] */ HRESULT STDMETHODCALLTYPE get_onbeforeupdate( 
		/* [out][retval] */ VARIANT *p) = 0;

	virtual /* [bindable][displaybind][id][propput] */ HRESULT STDMETHODCALLTYPE put_onerrorupdate( 
		/* [in] */ VARIANT v) = 0;

	virtual /* [bindable][displaybind][id][propget] */ HRESULT STDMETHODCALLTYPE get_onerrorupdate( 
		/* [out][retval] */ VARIANT *p) = 0;

	virtual /* [id] */ HRESULT STDMETHODCALLTYPE toString( 
		/* [out][retval] */ BSTR *String) = 0;

	virtual /* [id] */ HRESULT STDMETHODCALLTYPE createStyleSheet( 
		/* [in][defaultvalue] */ BSTR bstrHref,
		/* [in][defaultvalue] */ long lIndex,
		/* [out][retval] IHTMLStyleSheet */ IDispatch **ppnewStyleSheet) = 0;
};

//===========================================================cc::win::for ie_EventHandler
MIDL_INTERFACE("B196B284-BAB4-101A-B69C-00AA00341D07") ie_IConnectionPointContainer;
MIDL_INTERFACE("B196B287-BAB4-101A-B69C-00AA00341D07") ie_IEnumConnections;
typedef struct ie_tagCONNECTDATA
{
	IUnknown *pUnk;
	DWORD dwCookie;
} 	CONNECTDATA;

MIDL_INTERFACE("B196B286-BAB4-101A-B69C-00AA00341D07")
ie_IConnectionPoint : public IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE GetConnectionInterface( 
        /* [out] */ IID *pIID) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE GetConnectionPointContainer( 
        /* [out] */ ie_IConnectionPointContainer **ppCPC) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE Advise( 
        /* [in] */ IUnknown *pUnkSink,
        /* [out] */ DWORD *pdwCookie) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE Unadvise( 
        /* [in] */ DWORD dwCookie) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE EnumConnections( 
        /* [out] */ ie_IEnumConnections **ppEnum) = 0;
    
};

MIDL_INTERFACE("B196B287-BAB4-101A-B69C-00AA00341D07")
ie_IEnumConnections : public IUnknown
{
public:
    virtual /* [local] */ HRESULT STDMETHODCALLTYPE Next( 
        /* [in] */ ULONG cConnections,
        /* [length_is][size_is][out] */ ie_tagCONNECTDATA *rgcd,
        /* [out] */ ULONG *pcFetched) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE Skip( 
        /* [in] */ ULONG cConnections) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE Reset( void) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE Clone( 
        /* [out] */ ie_IEnumConnections **ppEnum) = 0;
    
};

MIDL_INTERFACE("B196B285-BAB4-101A-B69C-00AA00341D07")
ie_IEnumConnectionPoints : public IUnknown
{
public:
    virtual /* [local] */ HRESULT STDMETHODCALLTYPE Next( 
        /* [in] */ ULONG cConnections,
        /* [length_is][size_is][out] */ ie_IConnectionPoint **ppCP,
        /* [out] */ ULONG *pcFetched) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE Skip( 
        /* [in] */ ULONG cConnections) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE Reset( void) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE Clone( 
        /* [out] */ ie_IEnumConnectionPoints **ppEnum) = 0;
    
};

MIDL_INTERFACE("B196B284-BAB4-101A-B69C-00AA00341D07")
ie_IConnectionPointContainer : public IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE EnumConnectionPoints( 
        /* [out] */ ie_IEnumConnectionPoints **ppEnum) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE FindConnectionPoint( 
        /* [in] */ REFIID riid,
        /* [out] */ ie_IConnectionPoint **ppCP) = 0;
    
};

//===========================================================cc::win::ie_ClientSite
class ie_InPlaceSite;
class ie_DocHandler;
class ie_ClientSite : public IOleClientSite
{
protected:
	ie_InPlaceSite *m_pInPlaceSite;
	ie_DocHandler *m_pDocHandler;

public:
	ie_ClientSite(HWND hWndParent, ie_InPlaceSite *pInPlaceSite = NULL, ie_DocHandler *pDocHandler = NULL);
	~ie_ClientSite();

	//IUnknown
	STDMETHODIMP QueryInterface(REFIID iid, void ** ppvObject);
	ULONG STDMETHODCALLTYPE AddRef();
	ULONG STDMETHODCALLTYPE Release();

	//IOleClientSite
	STDMETHODIMP GetContainer(LPOLECONTAINER FAR* ppContainer);
	STDMETHODIMP SaveObject();
	STDMETHODIMP GetMoniker(DWORD dwAssign, DWORD dwWhichMoniker, IMoniker ** ppmk);
	STDMETHODIMP ShowObject();
	STDMETHODIMP OnShowWindow(BOOL fShow);
	STDMETHODIMP RequestNewObjectLayout();

	//myself fun
	ie_DocHandler *getDocHandler();
};

//===========================================================cc::win::ie_InPlaceSite
class ie_InPlaceSite : public IOleInPlaceSite
{
protected:
	HWND m_hWndParent;

public:
	ie_InPlaceSite(HWND hWndParent);
	~ie_InPlaceSite();

	// IUnknown
	STDMETHODIMP QueryInterface(REFIID iid, void ** ppvObject);
	ULONG STDMETHODCALLTYPE AddRef();
	ULONG STDMETHODCALLTYPE Release();

	//IOleWindow
	HRESULT STDMETHODCALLTYPE GetWindow(HWND * phwnd);
	HRESULT STDMETHODCALLTYPE ContextSensitiveHelp(BOOL fEnterMode);

	//IOleInPlaceSite
	HRESULT STDMETHODCALLTYPE CanInPlaceActivate(void);
	HRESULT STDMETHODCALLTYPE OnInPlaceActivate(void);
	HRESULT STDMETHODCALLTYPE OnUIActivate(void);
	HRESULT STDMETHODCALLTYPE GetWindowContext(IOleInPlaceFrame **ppFrame,
		IOleInPlaceUIWindow **ppDoc, LPRECT lprcPosRect,
		LPRECT lprcClipRect,
		LPOLEINPLACEFRAMEINFO lpFrameInfo);
	HRESULT STDMETHODCALLTYPE Scroll(SIZE scrollExtant);
	HRESULT STDMETHODCALLTYPE OnUIDeactivate(BOOL fUndoable);
	HRESULT STDMETHODCALLTYPE OnInPlaceDeactivate(void);
	HRESULT STDMETHODCALLTYPE DiscardUndoState(void);
	HRESULT STDMETHODCALLTYPE DeactivateAndUndo(void);
	HRESULT STDMETHODCALLTYPE OnPosRectChange(LPCRECT lprcPosRect);
};

//===========================================================cc::win::ie_IUnknown
class ie_IUnknown : public IUnknown
{
private:
	unsigned long m_RefCount;

public:
	ie_IUnknown() : m_RefCount(1)
	{
	}

	virtual HRESULT STDMETHODCALLTYPE QueryInterface( 
        /* [in] */ REFIID riid,
        /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject)
	{
		if (riid == IID_IUnknown)
		{
			*ppvObject = static_cast<IUnknown*>(this);
		}
		else
		{
			*ppvObject = NULL;
			return E_NOINTERFACE;
		}

		static_cast<IUnknown*>(*ppvObject)->AddRef();
		return S_OK;
	}

	virtual ULONG STDMETHODCALLTYPE AddRef( void)
	{
		InterlockedIncrement(reinterpret_cast<long*>(&m_RefCount));
		return m_RefCount;
	}

	virtual ULONG STDMETHODCALLTYPE Release( void)
	{
		if (!InterlockedDecrement(reinterpret_cast<long*>(&m_RefCount)))
		{
			delete this;
			return 0;
		}
		return m_RefCount;
	}

	template<class Q>
	HRESULT STDMETHODCALLTYPE QueryInterface(Q** pp)
	{
		return QueryInterface(__uuidof(Q), (void **)pp);
	}
};

//===========================================================cc::win::ie_IDispatch
class ie_IDispatch : public ie_IUnknown
{
public:
	ie_IDispatch() : ie_IUnknown()
	{
	}

	virtual HRESULT STDMETHODCALLTYPE QueryInterface( 
        /* [in] */ REFIID riid,
        /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject)
	{
		if (riid == IID_IUnknown)
		{
			*ppvObject = static_cast<IUnknown*>(this);
		}
		else if (riid == IID_IDispatch)
		{
			*ppvObject = (IDispatch*)(this);
		}
		else
		{
			*ppvObject = NULL;
			return E_NOINTERFACE;
		}

		static_cast<IUnknown*>(*ppvObject)->AddRef();
		return S_OK;
	};

	virtual HRESULT STDMETHODCALLTYPE GetTypeInfoCount( 
        /* [out] */ UINT *pctinfo)
	{
		*pctinfo = 0;
		return S_OK;
	};

	virtual HRESULT STDMETHODCALLTYPE GetTypeInfo( 
        /* [in] */ UINT iTInfo,
        /* [in] */ LCID lcid,
        /* [out] */ ITypeInfo **ppTInfo)
	{
		return E_NOTIMPL;
	};

	virtual HRESULT STDMETHODCALLTYPE GetIDsOfNames( 
        /* [in] */ REFIID riid,
        /* [size_is][in] */ LPOLESTR *rgszNames,
        /* [in] */ UINT cNames,
        /* [in] */ LCID lcid,
        /* [size_is][out] */ DISPID *rgDispId)
	{
		return E_NOTIMPL;
	};

    virtual /* [local] */ HRESULT STDMETHODCALLTYPE Invoke( 
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
	};
};

//===========================================================cc::win::ie_EventHandler
class ie_EventHandler : public IDispatch
{
public:
	//will set by IECtrl when create
	ie_IWebBrowser2 *pWebBrowser;

	ie_EventHandler();
	~ie_EventHandler();
	//IUnknown
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID classid, void** intf);
	virtual ULONG STDMETHODCALLTYPE AddRef();
	virtual ULONG STDMETHODCALLTYPE Release();
	//IDispatch
	virtual HRESULT STDMETHODCALLTYPE GetTypeInfoCount(/* [out] */ UINT *pctinfo);
	virtual HRESULT STDMETHODCALLTYPE GetTypeInfo(
		/* [in] */ UINT iTInfo,
		/* [in] */ LCID lcid,
		/* [out] */ ITypeInfo **ppTInfo);
	virtual HRESULT STDMETHODCALLTYPE GetIDsOfNames(
		/* [in] */ REFIID riid,
		/* [size_is][in] */ LPOLESTR *rgszNames,
		/* [in] */ UINT cNames,
		/* [in] */ LCID lcid,
		/* [size_is][out] */ DISPID *rgDispId);
	virtual /* [local] */ HRESULT STDMETHODCALLTYPE Invoke(
		/* [in] */ DISPID dispIdMember,
		/* [in] */ REFIID riid,
		/* [in] */ LCID lcid,
		/* [in] */ WORD wFlags,
		/* [out][in] */ DISPPARAMS *pDispParams,
		/* [out] */ VARIANT *pVarResult,
		/* [out] */ EXCEPINFO *pExcepInfo,
		/* [out] */ UINT *puArgErr);
};

class ie_DocHandler :public IUnknown
{
public:
	bool m_bIsShowContextMenu;
protected:
	ie_EventHandler *m_CommandHandler; //delete while end this class

public:
	ie_DocHandler(ie_EventHandler *commandHandler);
	~ie_DocHandler();
	//IUnknown
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID classid, void** intf);
	virtual ULONG STDMETHODCALLTYPE AddRef();
	virtual ULONG STDMETHODCALLTYPE Release();
	//IDispatch
	//return S_OK, no right click menu
	virtual HRESULT STDMETHODCALLTYPE ShowContextMenu(
		/* [in] */ DWORD dwID,
		/* [in] */ POINT __RPC_FAR *ppt,
		/* [in] */ IUnknown __RPC_FAR *pcmdtReserved,
		/* [in] */ IDispatch __RPC_FAR *pdispReserved);
	virtual HRESULT STDMETHODCALLTYPE GetHostInfo(/* [out][in] */ void *pInfo);
	virtual HRESULT STDMETHODCALLTYPE ShowUI(
		/* [in] */ DWORD dwID,
		/* [in] */ IOleInPlaceActiveObject __RPC_FAR *pActiveObject,
		/* [in] */ IUnknown __RPC_FAR *pCommandTarget,
		/* [in] */ IOleInPlaceFrame __RPC_FAR *pFrame,
		/* [in] */ IOleInPlaceUIWindow __RPC_FAR *pDoc);
	virtual HRESULT STDMETHODCALLTYPE HideUI(void);
	virtual HRESULT STDMETHODCALLTYPE UpdateUI(void);
	virtual HRESULT STDMETHODCALLTYPE EnableModeless(/* [in] */ BOOL fEnable);
	virtual HRESULT STDMETHODCALLTYPE OnDocWindowActivate(/* [in] */ BOOL fActivate);
	virtual HRESULT STDMETHODCALLTYPE OnFrameWindowActivate(/* [in] */ BOOL fActivate);
	virtual HRESULT STDMETHODCALLTYPE ResizeBorder(
		/* [in] */ LPCRECT prcBorder,
		/* [in] */ IOleInPlaceUIWindow __RPC_FAR *pUIWindow,
		/* [in] */ BOOL fRameWindow);
	virtual HRESULT STDMETHODCALLTYPE TranslateAccelerator(
		/* [in] */ LPMSG lpMsg,
		/* [in] */ const GUID __RPC_FAR *pguidCmdGroup,
		/* [in] */ DWORD nCmdID);
	virtual HRESULT STDMETHODCALLTYPE GetOptionKeyPath(
		/* [out] */ LPOLESTR __RPC_FAR *pchKey,
		/* [in] */ DWORD dw);
	virtual HRESULT STDMETHODCALLTYPE GetDropTarget(
		/* [in] */ IDropTarget __RPC_FAR *pDropTarget,
		/* [out] */ IDropTarget __RPC_FAR *__RPC_FAR *ppDropTarget);
	virtual HRESULT STDMETHODCALLTYPE GetExternal(
		/* [out] */ IDispatch __RPC_FAR *__RPC_FAR *ppDispatch);
	virtual HRESULT STDMETHODCALLTYPE TranslateUrl(
		/* [in] */ DWORD dwTranslate,
		/* [in] */ OLECHAR __RPC_FAR *pchURLIn,
		/* [out] */ OLECHAR __RPC_FAR *__RPC_FAR *ppchURLOut);
	virtual HRESULT STDMETHODCALLTYPE ie_DocHandler::FilterDataObject(
		/* [in] */ IDataObject __RPC_FAR *pDO,
		/* [out] */ IDataObject __RPC_FAR *__RPC_FAR *ppDORet);

	//myself fun
	ie_EventHandler *getEventHandler();
};

#ifdef _DEBUG_
class my_IEEvent : public ie_EventHandler
{
public:
	virtual HRESULT STDMETHODCALLTYPE GetIDsOfNames(
		/* [in] */ REFIID riid,
		/* [size_is][in] */ LPOLESTR *rgszNames,
		/* [in] */ UINT cNames,
		/* [in] */ LCID lcid,
		/* [size_is][out] */ DISPID *rgDispId)
	{
		return S_OK;
	}

	virtual /* [local] */ HRESULT STDMETHODCALLTYPE Invoke(
		/* [in] */ DISPID dispIdMember,
		/* [in] */ REFIID riid,
		/* [in] */ LCID lcid,
		/* [in] */ WORD wFlags,
		/* [out][in] */ DISPPARAMS *pDispParams,
		/* [out] */ VARIANT *pVarResult,
		/* [out] */ EXCEPINFO *pExcepInfo,
		/* [out] */ UINT *puArgErr)
	{
		//cc::Log::debug(_T("Invoke:%d"), dispIdMember);
		switch (dispIdMember)
		{
			case 250: //DISPID_BEFORENAVIGATE2
				BeforeNavigate2(pDispParams->rgvarg[5].pvarVal->bstrVal, pDispParams->rgvarg[0].pboolVal);
				break;
			case 252: //DISPID_NAVIGATECOMPLETE2
				NavigateComplete2(pDispParams->rgvarg[0].pvarVal->bstrVal);
				break;
			case 259: //DISPID_DOCUMENTCOMPLETE
				DocumentComplete(pDispParams->rgvarg[0].pvarVal->bstrVal);
				break;
			case 102:
				StatusTextChange(pDispParams->rgvarg[0].bstrVal);
				break;
			case 113:
				TitleChange(pDispParams->rgvarg[0].bstrVal);
				break;
			//Read the DWebBrowserEvents2 documentation to see detail.(exdispid.h)
			//108:ProgressChange, 105:CommandStateChange, 106:DownloadBegin, 104:DownloadComplete, 112:PropertyChange
			//251:NewWindow2, 253:Quit, 254:Visible, 255:ToolBar, 256:MenuBar, 257:StatusBar, 258:FullScreen, 260:TheaterMode
		}
		return S_OK;
	}
	//DWebBrowserEvents2
	void BeforeNavigate2(const BSTR url, short *cancel)
	{
		//cc::Log::debug(_T("BeforeNavigate2:"));
	}

	void NavigateComplete2(const BSTR url) 
	{
		//cc::Log::debug(_T("NavigateComplete2:"));
	}

	void DocumentComplete(const BSTR url)
	{
		//cc::Log::debug(_T("DocumentComplete:"));
	}
	//void myObject::DocumentComplete(LPDISPATCH pDisp, VARIANT* URL)
	//{
	//	HRESULT hr;
	//	IUnknown* pUnkBrowser = NULL;
	//	IUnknown* pUnkDisp = NULL;

	//	// Is this the DocumentComplete event for the top frame window?
	//	// Check COM identity: compare IUnknown interface pointers.
	//	hr = m_pBrowser->QueryInterface( IID_IUnknown,  (void**)&pUnkBrowser );
	//	if ( SUCCEEDED(hr) )
	//	{
	//		hr = pDisp->QueryInterface( IID_IUnknown,  (void**)&pUnkDisp );
	//		if ( SUCCEEDED(hr) )
	//		{
	//			if ( pUnkBrowser == pUnkDisp )
	//			{
	//				// This is the DocumentComplete event for the top 
	//				//   frame - page is loaded!
	//				//...
	//			}
	//			pUnkDisp->Release();
	//		}
	//		pUnkBrowser->Release();
	//	}
	//}

	void StatusTextChange(const BSTR text)
	{
	}

	void TitleChange(const BSTR title)
	{
	}
};

class my_DocEvent : public ie_EventHandler
{
public:
	virtual HRESULT STDMETHODCALLTYPE GetIDsOfNames(
		/* [in] */ REFIID riid,
		/* [size_is][in] */ LPOLESTR *rgszNames,
		/* [in] */ UINT cNames,
		/* [in] */ LCID lcid,
		/* [size_is][out] */ DISPID *rgDispId)
	{
		*rgDispId = 1;
		return S_OK;
	}

	virtual /* [local] */ HRESULT STDMETHODCALLTYPE Invoke(
		/* [in] */ DISPID dispIdMember,
		/* [in] */ REFIID riid,
		/* [in] */ LCID lcid,
		/* [in] */ WORD wFlags,
		/* [out][in] */ DISPPARAMS *pDispParams,
		/* [out] */ VARIANT *pVarResult,
		/* [out] */ EXCEPINFO *pExcepInfo,
		/* [out] */ UINT *puArgErr)
	{
		//cc::Log::debug(_T("Invoke:%d"), dispIdMember);
		if(dispIdMember == 1)
		{
			//MessageBox(0, _T("Hello Worldyyyyyyyyy"), _T("Hello"), 0); //place your code here
		}
		//(rr,"def",window,456,document,789);
		VARIANT p0;
		VARIANT p1;
		VARIANT p2;
		VARIANT p3;
		VARIANT p4;
		VARIANT p5;
        VariantInit(&p0);
        VariantInit(&p1);
        VariantInit(&p2);
        VariantInit(&p3);
        VariantInit(&p4);
        VariantInit(&p5);
        VariantCopy(&p0,&pDispParams->rgvarg[0]);
        VariantCopy(&p1,&pDispParams->rgvarg[1]);
        VariantCopy(&p2,&pDispParams->rgvarg[2]);
        VariantCopy(&p3,&pDispParams->rgvarg[3]);
        VariantCopy(&p4,&pDispParams->rgvarg[4]);
        VariantCopy(&p5,&pDispParams->rgvarg[5]);


		IDispatch* pHtmlDocDispatch = NULL;
		pWebBrowser->get_Document(&pHtmlDocDispatch);
		pVarResult->vt = 9;
		pVarResult->pdispVal = pHtmlDocDispatch;
		if(pWebBrowser != NULL && FALSE)
		{

			VARIANT vVal;
			VariantInit(&vVal);
			VariantCopy(&vVal,&p3);
			vVal.vt = 9;
			vVal.pdispVal = pHtmlDocDispatch;
			VariantCopy(pVarResult,&p3);
			vVal.vt = 9;
			pVarResult->pdispVal = pHtmlDocDispatch;
			//VariantCopy(pVarResult,&p3);
			//VariantCopy(pVarResult,&vVal);
		}
		//SAFEARRAY* results;
		return S_OK;
	}
};

class my_DocHandler : public ie_DocHandler
{
public:
	my_DocHandler(ie_EventHandler *m_CommandHandler) : ie_DocHandler(m_CommandHandler)
	{
	}
	//return S_OK, no right click menu,or E_NOINTERFACE disable menu
	virtual HRESULT STDMETHODCALLTYPE ShowContextMenu(
		/* [in] */ DWORD dwID, 
		/* [in] */ POINT __RPC_FAR *ppt, 
		/* [in] */ IUnknown __RPC_FAR *pcmdtReserved, 
		/* [in] */ IDispatch __RPC_FAR *pdispReserved)
	{
		return S_OK;
	}
};
#endif

class IEBase
{
protected:
	//object of ie
	//HRESULT m_hOleInit; //save result of OleInitialize
	IUnknown *pUnknown;
    IOleInPlaceObject *pInPlaceObject;
	IOleObject *pObject;
	IOleInPlaceActiveObject *pActiveObject;

	ie_ClientSite *m_pClientSite;
	ie_IConnectionPoint *m_pCPoint;
	DWORD m_dwCPointCookie;
	ie_EventHandler *m_pIEEventHandler; //delete while end this class

	virtual bool _SetEventHandler();

public:
	//Constructors
	IEBase();
	//destructor
	virtual ~IEBase();

	//call by form while oncreate msg
	virtual bool OnCreate(HWND hWndParent, HINSTANCE hInst = NULL);
	virtual bool OnCreate(HWND hWndParent, HINSTANCE hInst, ie_DocHandler *pUIHandler,
		ie_InPlaceSite *pInPlaceSite = NULL, ie_ClientSite *pClientSite = NULL, BSTR url = NULL);
	bool SetEventHandler(ie_EventHandler *pIEEventHandler);

	RECT getPosition();
	void setPosition(RECT rect);
	void Location(int Left, int Top);
	void Size(int Width, int Height);
	void Update();

	ie_DocHandler *m_pUIHandler; //m_pUIHandler is deleted at ie_ClientSite
	ie_IWebBrowser2 *pWebBrowser; //delete while end this class

	//WebBrowser
	HWND getHWND();
	ie_IHTMLDocument2* getDocument();
	virtual bool isUnloadOk();

	bool execScript(BSTR code, BSTR language);
	virtual bool TranslateAccelerator(MSG *msg);
	bool Navigate(BSTR url);
	bool SetFocus();
	bool ValidFocus();
	bool waitComplete(int millisecondsTimeout);
	bool isComplete();
	virtual bool write(BSTR txt, bool isAppend);
	bool loadText(const void* txt, DWORD nLength);
	//need free point out this fun
	bool saveText(void** txt, DWORD& nLength);
	static bool saveText(IDispatch* pHtmlDoc, void** txt, DWORD& nLength);
	static bool saveStream(IStream* pStream, IDispatch* pHtmlDoc);
	static bool loadStream(IStream* pStream, IDispatch* pHtmlDoc);

	//even success for attach,cannot use event
	//also this function is only get pWebBrowser from hWnd,so some other functions of IEBase cannot be used
	bool AttachHWND(HWND hWnd);
	static ie_IHTMLDocument2* GetDocumentFromHWND(HWND hWnd);
};

class ScriptHost : public IActiveScriptSite, public IActiveScriptSiteWindow
{
protected:
	int m_refCount;               //variable to maintain the reference count
	IActiveScript* m_Engine;      //reference to the scripting engine
	IActiveScriptParse* m_Parser;   //reference to scripting engine in parsing mode.
	LPUNKNOWN m_Object;         //The object that will interact with the script
	LPOLESTR m_pNamedItem;
	bool m_bRunError;

public:
	//Constructor
	ScriptHost();
	//Destructor
	~ScriptHost();

	BOOL InitializeHost(LPOLESTR pwszLanguage = NULL);
	bool RunScript(LPOLESTR pwszScript, LPOLESTR pstrName = NULL, LPUNKNOWN pObject = NULL);
	bool QuitScript();
	bool GetIsRunError();

	/******* IUnknown *******/
	STDMETHODIMP QueryInterface(REFIID riid, void** ppvObj);
	STDMETHODIMP_(ULONG) AddRef();
	STDMETHODIMP_(ULONG) Release();

	/******* IActiveScriptSite *******/
	STDMETHODIMP GetLCID(LCID *plcid);   // address of variable for 
	//language identifier
	STDMETHODIMP GetItemInfo(
		LPCOLESTR pstrName,          // address of item name
		DWORD dwReturnMask,         // bit mask for information retrieval
		IUnknown **ppunkItem,      // address of pointer to item's IUnknown
		ITypeInfo **ppTypeInfo);   // address of pointer to item's ITypeInfo
	STDMETHODIMP GetDocVersionString(
		BSTR *pbstrVersionString);  // address of document version string
		STDMETHODIMP OnScriptTerminate(
		const VARIANT *pvarResult,   // address of script results
		const EXCEPINFO *pexcepinfo);   // address of structure with exception 
	//information
	STDMETHODIMP OnStateChange(SCRIPTSTATE ssScriptState);   // new state of engine
	STDMETHODIMP OnScriptError(IActiveScriptError *pase);   // address of error interface
	STDMETHODIMP OnEnterScript(void);
	STDMETHODIMP OnLeaveScript(void);

	/******* IActiveScriptSiteWindow *******/
	STDMETHODIMP GetWindow(HWND *phwnd);
	STDMETHODIMP EnableModeless(BOOL fEnable);  // enable flag
};

class IETranceData : public ie_EventHandler
{
public:
	VARIANT var1;
	VARIANT var2;
	VARIANT var3;

public:
	IETranceData() : ie_EventHandler()
	{
		VariantInit(&var1);
		VariantInit(&var2);
		VariantInit(&var3);
	}

	virtual HRESULT STDMETHODCALLTYPE GetIDsOfNames(REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgDispId)
	{
		if(wcscmp(*rgszNames, L"var1") == 0)
		{
			*rgDispId = 1;
		}
		else if(wcscmp(*rgszNames, L"var2") == 0)
		{
			*rgDispId = 2;
		}
		else if(wcscmp(*rgszNames, L"var3") == 0)
		{
			*rgDispId = 3;
		}
		else
		{
			return S_FALSE;
		}
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pDispParams, VARIANT *pVarResult, EXCEPINFO *pExcepInfo, UINT *puArgErr)
	{
		switch(dispIdMember)
		{
		case 1: //var1
			CC_TRACE((_T("IETranceData::Invoke, var1")));
			VariantCopy(pVarResult, &var1);
			break;
		case 2: //var2
			CC_TRACE((_T("IETranceData::Invoke, var2")));
			VariantCopy(pVarResult, &var2);
			break;
		case 3: //var3
			CC_TRACE((_T("IETranceData::Invoke, var3")));
			VariantCopy(pVarResult, &var3);
			break;
		default:
			break;
		}
		return S_OK;
	}
};


} //namespace win
} //namespace cc

#endif //#ifndef CC_WIN_IECTRL_H_
