//#include "ActivScp.h"
// cc.win.OleBase.h
// The C# like Controls class
// Author: Shu.KK
// 2006-07-01
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef CC_WIN_OLEBASE_H_
#define CC_WIN_OLEBASE_H_

#include <tchar.h>
#include <windows.h>

//#include "cc.h"
#include "cc.Log.h"
//#include "cc.Str.h"


static UINT cc_win_nOleInitCount = 0;
static UINT cc_win_hOleInitResult = S_FALSE;
HRESULT __stdcall CC_OleInitialize();
void __stdcall CC_OleUnInitialize();



namespace cc
{
namespace win
{

EXTERN_C const IID IID_IPersist;
MIDL_INTERFACE("0000010c-0000-0000-C000-000000000046")
ie_IPersistx : public IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE GetClassID( 
        /* [out] */ CLSID *pClassID) = 0;
    
};

EXTERN_C const IID IID_IPersistStreamInit;
MIDL_INTERFACE("7FD52380-4E07-101B-AE2D-08002B2EC713")
ie_IPersistStreamInitx : public ie_IPersistx
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



} //namespace win
} //namespace cc

#endif //#ifndef CC_WIN_OLEBASE_H_
