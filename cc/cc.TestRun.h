// cc.TestRun.h
// TestRun
// Author: Shu.KK
// 2005-12-01
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef CC_TestRun_H_
#define CC_TestRun_H_

#include "cc.h"
#include <windows.h>
#include <wchar.h>
#include <tchar.h>

#include "cc.Log.h"
#include "cc.Test.h"

namespace cc
{

class TestRun
{
public:
protected:
	struct mEntry {
		Test *testClass;
		//only use for:relation of class<-->list,index of case of testclass
		//if class:-1 for check on all testcase, -2 not so
		int nCaseIndex;
	};
	static mEntry **m_testClassArr;
	static int m_nRealCount;
	static int m_nClassCount;
	static mEntry *m_testClassRelaList;
	static int m_nListCount;
	static bool m_isRunning;
	static bool m_isCancel;
	static HINSTANCE m_hInstance;
	static TCHAR sBuf[512 + 1];
	static HWND m_hList;
	static HWND m_hEdit;
	static HWND m_hSplitter;
	static bool m_isMouseDown;
	static long m_SplitMoveY;
	static long m_SplitPosY;

	static int nRunOK;
	static int nRunSkip;
	static int nRunFatal;
	static int nRunErr;
	static int nRunNever;
	static int nRunOther;

private:

public:
	// constructors
	TestRun();
	// destructor
	virtual ~TestRun();

	//========================================
	//MAIN FUNCTION
	static bool isCancel();
	void addTestClass(Test *test);
	bool ShowDialog(HINSTANCE hInstance = NULL);
protected:
	void Init_();
	static void setMsg_(TCHAR *msg, bool isAppend);
	static void refreshList_(int nIndex, TCHAR *item);
	static void WndProc_Create_(HWND hwnd);
	static void WndProc_Run_(HWND hwnd);
	static void WndProc_Run_Ret(int nIndex);
	static LRESULT CALLBACK WndProc_(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK WndProc_Splitter(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
};

} //namespace cc

#endif //#ifndef CC_TestRun_H_
