// cc.win.OleBase.h
// The C# like Controls class
// Author: Shu.KK
// 2006-07-01
// add function at 2007-05-01
#include "cc.win.OleBase.h"
using namespace cc;
using namespace cc::win;

//http://www.codeproject.com/KB/clipboard/dragdropinterface.aspx
//http://catch22.net/tuts/dragdrop6.asp
//http://catch22.net/tuts/dragdrop6.asp

//===========================================================cc::win::OleBase
HRESULT __stdcall CC_OleInitialize()
{
	if(cc_win_nOleInitCount == 0)
	{
		cc_win_hOleInitResult = OleInitialize(NULL);
	}
	cc_win_nOleInitCount++;
	return cc_win_hOleInitResult;
}
void __stdcall CC_OleUnInitialize()
{
	if(cc_win_nOleInitCount > 0)
	{
		cc_win_nOleInitCount--;
	}
	if(cc_win_nOleInitCount == 0)
	{
		if(cc_win_hOleInitResult == S_OK)
		{
#ifndef _DEBUG_
			try
			{
#endif
				//if OleInitialize at this cope,then relese it,cannot use SUCCEEDED!
				OleUninitialize();
#ifndef _DEBUG_
			}
			catch(...)
			{
			}
#endif
			cc_win_hOleInitResult = S_FALSE;
		}
	}
}
