// cc.TestRun.cpp
// Test to screen
// Author: Shu.KK
// 2005/12/08
#include "cc.TestRun.h"
using namespace cc;

HINSTANCE TestRun::m_hInstance = NULL;
HWND TestRun::m_hList = NULL;
HWND TestRun::m_hEdit = NULL;
TestRun::mEntry **TestRun::m_testClassArr = NULL;
TestRun::mEntry *TestRun::m_testClassRelaList = NULL;
int TestRun::m_nRealCount = 0;
int TestRun::m_nClassCount = 0;
int TestRun::m_nListCount = 0;
bool TestRun::m_isRunning = false;
bool TestRun::m_isCancel = false;
TCHAR TestRun::sBuf[512 + 1] = _T("\0");
HWND TestRun::m_hSplitter = NULL;
bool TestRun::m_isMouseDown = false;
long TestRun::m_SplitMoveY = 0;
long TestRun::m_SplitPosY = 70;

int TestRun::nRunOK = 0;
int TestRun::nRunSkip = 0;
int TestRun::nRunFatal = 0;
int TestRun::nRunErr = 0;
int TestRun::nRunNever = 0;
int TestRun::nRunOther = 0;

#define CC_LOG_CONFIG "Log"

///////////////////// constructors //////////////////////////
TestRun::TestRun()
{
	Init_();
}

///////////////////// destructor //////////////////////////
TestRun::~TestRun()
{
	if(m_testClassArr != NULL)
	{
		for(int i = 0; i < m_nClassCount; i++)
		{
			mEntry *mBufTmp = *(m_testClassArr + i);
			//Test *test will be deleted in this class,so need define this class with point and not del it out this class
			delete mBufTmp->testClass;
			delete []mBufTmp;
		}
		free(m_testClassArr);
		m_testClassArr = NULL;
	}
	if(m_testClassRelaList != NULL)
	{
		delete []m_testClassRelaList;
		m_testClassArr = NULL;
	}
}

///////////////////// private/internal used methods //////////////////////////
void TestRun::Init_()
{
	m_nRealCount = 0;
	m_nClassCount = 0;
	m_testClassArr = NULL;
	m_testClassRelaList = NULL;
}

void TestRun::setMsg_(TCHAR *msg, bool isAppend)
{
	long Pos = (long)SendMessage(m_hEdit, WM_GETTEXTLENGTH, 0, 0);
	if(isAppend)
	{
		SendMessage(m_hEdit, EM_SETSEL, Pos, Pos);
		SendMessage(m_hEdit, EM_REPLACESEL, false, (LPARAM)msg);
	}
	else
	{
		SendMessage(m_hEdit, EM_SETSEL, 0, Pos);
		SendMessage(m_hEdit, EM_REPLACESEL, false, (LPARAM)msg);
	}
}

void TestRun::refreshList_(int nIndex, TCHAR *item)
{
	//if item is selected,save it's pos
	int nInd = (int)SendMessage(m_hList, LB_GETCURSEL, 0, 0);
	//Insert new string at List(i)
	SendMessage(m_hList, LB_INSERTSTRING, (WPARAM)(nIndex), (LPARAM)item);
	//Delete string at List(i+1)
	SendMessage(m_hList, LB_DELETESTRING, (WPARAM)(nIndex + 1), 0L);
	if(nInd == nIndex)
	{
		//Restore the current selection
		SendMessage(m_hList, LB_SETCURSEL, (WPARAM)nIndex, 0L);
	}
}

LRESULT CALLBACK TestRun::WndProc_Splitter(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch (Message)
	{
	case WM_LBUTTONDOWN:
		{
			SetCapture(hwnd);
			m_isMouseDown = true;
			return 0;
		}
	case WM_MOUSEMOVE:
		{
			if(!m_isMouseDown)
			{
				return 0;
			}
			if(m_SplitMoveY == HIWORD(lParam)) return 0;
			m_SplitMoveY = HIWORD(lParam);
			RECT rect;
			HWND hParent = GetParent(hwnd);
			GetClientRect(hParent, &rect);
			if(lParam > 0)
			{
				m_SplitPosY = m_SplitPosY - m_SplitMoveY;
			}
			else
			{
				m_SplitPosY = m_SplitPosY + (0xffff - m_SplitMoveY);
			}
			if(m_SplitPosY > rect.bottom - 40) m_SplitPosY = rect.bottom - 40;
			if(m_SplitPosY < 0) m_SplitPosY = 0;
			SendMessage(GetParent(hwnd), WM_SIZE, 0, 0);
			return 0;
		}
	case WM_LBUTTONUP:
		{
			m_isMouseDown = false;
			ReleaseCapture();
			return 0;
		}
	default:
			break;
	}
	return DefWindowProc(hwnd,Message,wParam,lParam);  //let system deal with msg
}

#define IDC_BTN_RUN 1201
#define IDC_BTN_EXIT 1202
#define IDC_BTN_SELALL 1203
#define IDC_BTN_SELRES 1204
#define IDC_CHK_LOGTRACE 1208
#define IDC_LST_CLS 1205
#define IDC_TXT_COMM 1206
#define IDC_SPLITTER 1207
#define IDC_VSPLITBAR 30981       // splitter
void TestRun::WndProc_Create_(HWND hwnd)
{
	//create a single line edit control
	CreateWindowEx(BS_PUSHBUTTON,						  //more or 'extended' styles
					TEXT("BUTTON"),						  //'class' of control to create
					TEXT("runTest"), 					  //the control caption
					WS_CHILD|WS_VISIBLE|WS_BORDER,		  //control style: how it looks
					2, 2, 80, 26,						  //control position: left,top,width,height
					hwnd,								  //parent window handle
					(HMENU)IDC_BTN_RUN,				  //control's ID
					m_hInstance, 							  //application instance
					NULL);
	//create a single line edit control
	CreateWindowEx(BS_PUSHBUTTON,						  //more or 'extended' styles
					TEXT("BUTTON"),						  //'class' of control to create
					TEXT("Exit"),					   //the control caption
					WS_CHILD|WS_VISIBLE|WS_BORDER,		  //control style: how it looks
					84, 2, 80, 26,						 //control position: left,top,width,height
					hwnd,								  //parent window handle
					(HMENU)IDC_BTN_EXIT, 					  //control's ID
					m_hInstance, 							  //application instance
					NULL);
	//create a single line edit control
	CreateWindowEx(BS_PUSHBUTTON,						  //more or 'extended' styles
					TEXT("BUTTON"),						  //'class' of control to create
					TEXT("select all"),						 //the control caption
					WS_CHILD|WS_VISIBLE|WS_BORDER,		  //control style: how it looks
					168, 8, 100, 20,						  //control position: left,top,width,height
					hwnd,								  //parent window handle
					(HMENU)IDC_BTN_SELALL,						//control's ID
					m_hInstance, 							  //application instance
					NULL);
	//create a single line edit control
	CreateWindowEx(BS_PUSHBUTTON,						  //more or 'extended' styles
					TEXT("BUTTON"),						  //'class' of control to create
					TEXT("Adverse selection"),						 //the control caption
					WS_CHILD|WS_VISIBLE|WS_BORDER,		  //control style: how it looks
					270, 8, 140, 20,						  //control position: left,top,width,height
					hwnd,								  //parent window handle
					(HMENU)IDC_BTN_SELRES,						//control's ID
					m_hInstance, 							  //application instance
					NULL);
	//create a single line edit control
	CreateWindowEx(BS_PUSHBUTTON,						  //more or 'extended' styles
					TEXT("BUTTON"),						  //'class' of control to create
					TEXT("Log Trace"),						 //the control caption
					WS_CHILD|WS_VISIBLE|WS_TABSTOP|BS_AUTOCHECKBOX,		  //control style: how it looks
					412, 8, 100, 20,						  //control position: left,top,width,height
					hwnd,								  //parent window handle
					(HMENU)IDC_CHK_LOGTRACE,						//control's ID
					m_hInstance, 							  //application instance
					NULL);

    //create a list box control
    CreateWindowEx(WS_EX_CLIENTEDGE,              //more or 'extended' styles
                    TEXT("LISTBOX"),               //'class' of control to create
                    NULL,                          //the control caption
                    WS_CHILD|WS_VISIBLE|WS_BORDER|LBS_NOTIFY
					|WS_HSCROLL|WS_VSCROLL, //control style: how it looks
					24, 2, 200, 200,			  //control position: left,top,width,height
                    hwnd,                          //parent window handle
					(HMENU)IDC_LST_CLS,						//control's ID
                    m_hInstance,                       //application instance
                    NULL);
	m_hList = GetDlgItem(hwnd, IDC_LST_CLS);

	//create a multi-line edit control
	CreateWindowEx(WS_EX_CLIENTEDGE,					  //more or 'extended' styles
					TEXT("EDIT"),						  //'class' of control to create
					TEXT(""),	//the control caption
					WS_CHILD|WS_VISIBLE|WS_BORDER|		  //control style: how it looks
					ES_MULTILINE|WS_VSCROLL|ES_NOHIDESEL|ES_READONLY,
					2, 200, 400, 80,			  //control position: left,top,width,height
					hwnd,								  //parent window handle
					(HMENU)IDC_TXT_COMM, 					  //control's ID
					m_hInstance, 							  //application instance
					NULL);
	m_hEdit = GetDlgItem(hwnd, IDC_TXT_COMM);

	//create a Splitter control
	TCHAR chClassName[] = TEXT("TESTSPLITTERWND");
	WNDCLASS wndcls;
	wndcls.style = CS_DBLCLKS;
	wndcls.lpfnWndProc = (WNDPROC)WndProc_Splitter;
	wndcls.cbClsExtra = 0;
	wndcls.cbWndExtra = 0;
	wndcls.hInstance = m_hInstance;
	wndcls.hIcon = NULL;
	wndcls.hCursor = (HCURSOR)LoadImage(0, IDC_SIZENS, IMAGE_CURSOR, 0, 0, LR_SHARED);
	wndcls.hbrBackground = NULL;
	wndcls.lpszMenuName = NULL;
	wndcls.lpszClassName = chClassName;
	//Register the wnd class with the Windows system
	if(RegisterClass(&wndcls))
	{
		m_hSplitter = ::CreateWindowEx(0, chClassName, NULL, WS_CHILD|WS_VISIBLE,
				0, 0, 0, 0, //control position: left,top,width,height
				hwnd, NULL, m_hInstance, NULL);
	}

	//get listCount,so create array that relation to list's item
	int nListCount = 0;
	int nIndex;
	for(nIndex = 0; nIndex < m_nClassCount; nIndex++)
	{
		nListCount++;
		mEntry *mBufTmp = (mEntry *)*(m_testClassArr + nIndex);
		Test *test = mBufTmp->testClass;
		nListCount += test->getCaseCount();
	}
	//save list count
	m_nListCount = nListCount;
	//show to list,and save relation test to m_testClassRelaList
	m_testClassRelaList = new mEntry[m_nListCount];
	nListCount = -1;
	for(nIndex = 0; nIndex < m_nClassCount; nIndex++)
	{
		nListCount++;
		mEntry *mBufTmp = (mEntry *)*(m_testClassArr + nIndex);
		Test *test = mBufTmp->testClass;
		//_sntprintf(sBuf, 512, _T("[TestClass]%s(%s)"), test->ClassName, test->ClassComment);
		cc::Buf::Format(sBuf, 513, _T("[TestClass]%s(%s)"), test->ClassName, test->ClassComment);
        SendMessage(m_hList, LB_ADDSTRING, 0, (LPARAM)sBuf);

		m_testClassRelaList[nListCount].testClass = mBufTmp->testClass;
		m_testClassRelaList[nListCount].nCaseIndex = -1; //check on all testcase

		for(int nCase = 0; nCase < test->getCaseCount(); nCase++)
		{
			nListCount++;
			//_sntprintf(sBuf, 512, _T("  [o][norun] %s(line:%d)"), test->getCaseName(nCase), test->getCaseLine(nCase));
			cc::Buf::Format(sBuf, 513, _T("  [o][norun] %s(line:%d)"), test->getCaseName(nCase), test->getCaseLine(nCase));
			SendMessage(m_hList, LB_ADDSTRING, 0, (LPARAM)sBuf);

			m_testClassRelaList[nListCount].testClass = mBufTmp->testClass;
			m_testClassRelaList[nListCount].nCaseIndex = nCase;
		}
	}
}

void TestRun::WndProc_Run_Ret(int nIndex)
{
	int nCase = m_testClassRelaList[nIndex].nCaseIndex;
	if(nCase >= 0)
	{
		SendMessage(m_hList, LB_GETTEXT, nIndex, (LPARAM)&sBuf);
		Test *test = m_testClassRelaList[nIndex].testClass;
		int nCase = m_testClassRelaList[nIndex].nCaseIndex;
		cc::Test::RunResult nResult = test->getRunResult(nCase);
		if(nResult == cc::Test::RUN_OK)
		{
			//_sntprintf(sBuf + 6, 5, _T("ok   "));
			memmove(sBuf + 6, _T("ok   "), 5 * sizeof(TCHAR));
			nRunOK++;
		}
		else if(nResult == cc::Test::RUN_SKIP)
		{
			//_sntprintf(sBuf + 6, 5, _T("skip "));
			memmove(sBuf + 6, _T("skip "), 5 * sizeof(TCHAR));
			nRunSkip++;
		}
		else if(nResult == cc::Test::RUN_FATAL)
		{
			//_sntprintf(sBuf + 6, 5, _T("fatal"));
			memmove(sBuf + 6, _T("fatal"), 5 * sizeof(TCHAR));
			nRunFatal++;
		}
		else if(nResult == cc::Test::RUN_ERROR)
		{
			//_sntprintf(sBuf + 6, 5, _T("error"));
			memmove(sBuf + 6, _T("error"), 5 * sizeof(TCHAR));
			nRunErr++;
		}
		else if(nResult == cc::Test::RUN_NEVER)
		{
			//_sntprintf(sBuf + 6, 5, _T("norun"));
			memmove(sBuf + 6, _T("norun"), 5 * sizeof(TCHAR));
			nRunNever++;
		}
		else
		{
			//_sntprintf(sBuf + 6, 5, _T("     "));
			memmove(sBuf + 6, _T("     "), 5 * sizeof(TCHAR));
			nRunOther++;
		}
		refreshList_(nIndex, sBuf); //refresh item
	}
}
void TestRun::WndProc_Run_(HWND hwnd)
{
	if(m_isRunning)
	{
		return;
	}
	m_isRunning = true;
	m_isCancel = false;
	//set not need run(skip:[x]) class to testClass
	int nIndex;
	for(nIndex = 0; nIndex < m_nListCount; nIndex++)
	{
		int nCase = m_testClassRelaList[nIndex].nCaseIndex;
		if(nCase >= 0)
		{
			SendMessage(m_hList, LB_GETTEXT, nIndex, (LPARAM)&sBuf);
			Test *test = m_testClassRelaList[nIndex].testClass;
			if(sBuf[3] == 'o')
			{
				//need run
				test->setCaseSkip(nCase, false);
			}
			else
			{
				//not need run
				test->setCaseSkip(nCase, true);
			}
			//_sntprintf(sBuf + 6, 5, _T("norun"));
			memmove(sBuf + 6, _T("norun"), 5 * sizeof(TCHAR));
			refreshList_(nIndex, sBuf); //refresh item
		}
	}

	cc::Log::debug(_T("==========  ccTest Running  =========="));
	setMsg_(_T("ccTest Run Begin\r\n"), false);
	//here add testcase
	for(nIndex = 0; nIndex < m_nClassCount; nIndex++)
	{
		mEntry *mBufTmp = (mEntry *)*(m_testClassArr + nIndex);
		Test *test = mBufTmp->testClass;
		test->setFunJudgeCancel(isCancel);
		test->isCanceled = false;
		int nCheck = (int)::SendMessage(GetDlgItem(hwnd, IDC_CHK_LOGTRACE), BM_GETCHECK, 0, 0);
		test->isLogTrace = (nCheck != 0);
		//_sntprintf(sBuf, 512, _T("  %s(%s)..."), test->ClassName, test->ClassComment);
		cc::Buf::Format(sBuf, 513, _T("  %s(%s)..."), test->ClassName, test->ClassComment);
		setMsg_(sBuf, true);
		test->runTest();
		if(test->isCanceled)
		{
			setMsg_(_T("  \r\ncanceled by user\r\n"), true);
			break;
		}
		int nCount = test->getCaseCount();
		int nFatal = test->getFatalCount();
		int nError = test->getErrorCount();
		//_sntprintf(sBuf, 512, _T("TestCase:%d, OK:%d, Fatal:%d, Error:%d\r\n"), nCount, nCount - nFatal - nError, nFatal, nError);
		cc::Buf::Format(sBuf, 513, _T("TestCase:%d, OK:%d, Fatal:%d, Error:%d\r\n"), nCount, nCount - nFatal - nError, nFatal, nError);
		setMsg_(sBuf, true);
		WndProc_Run_Ret(nIndex);
		isCancel();
	}

	//set run result to list
	nRunOK = 0;
	nRunSkip = 0;
	nRunFatal = 0;
	nRunErr = 0;
	nRunNever = 0;
	nRunOther = 0;
	for(nIndex = 0; nIndex < m_nListCount; nIndex++)
	{
		WndProc_Run_Ret(nIndex);
	}
	cc::Buf::Format(sBuf, 513, _T("Run Over(OK:%d, Skip:%d, Fatal:%d, Error:%d, Never:%d, Other:%d)\r\n"),
		nRunOK, nRunSkip, nRunFatal, nRunErr, nRunNever, nRunOther);
	setMsg_(sBuf, true);
	m_isRunning = false;
}

LRESULT CALLBACK TestRun::WndProc_(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	//cc::Log::debug("IDC_LST_CLS,%x,%x", Message, HIWORD(wParam));
	switch (Message)
	{
	case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
			case IDC_BTN_EXIT:
				{
					if(m_isRunning)
					{
						m_isCancel = true;
					}
					else
					{
						DestroyWindow(hwnd);
					}
					break;
				}
			case IDC_BTN_RUN:
				{
					EnableWindow(GetDlgItem(hwnd, IDC_BTN_RUN), false);
					WndProc_Run_(hwnd);
					EnableWindow(GetDlgItem(hwnd, IDC_BTN_RUN), true);
					break;
				}
			case IDC_BTN_SELALL:
				{
					for(int i = 0; i < m_nListCount; i++)
					{
						int nCase = m_testClassRelaList[i].nCaseIndex;
						if(nCase >= 0)
						{
							SendMessage(m_hList, LB_GETTEXT, i, (LPARAM)&sBuf);
							sBuf[3] = _T('o');
							refreshList_(i, sBuf); //refresh item
						}
						else
						{
							m_testClassRelaList[i].nCaseIndex = -1;
						}
					}
					break;
				}
			case IDC_BTN_SELRES:
				{
					for(int i = 0; i < m_nListCount; i++)
					{
						int nCase = m_testClassRelaList[i].nCaseIndex;
						if(nCase >= 0)
						{
							SendMessage(m_hList, LB_GETTEXT, i, (LPARAM)&sBuf);
							if(sBuf[3] == _T('o'))
							{
								sBuf[3] = _T('x');
							}
							else
							{
								sBuf[3] = _T('o');
							}
							refreshList_(i, sBuf); //refresh item
						}
					}
					break;
				}
			case IDC_LST_CLS:
				{
					switch(HIWORD(wParam))
					{
					case LBN_SELCHANGE:
						{
							int nInd = (int)SendMessage(m_hList, LB_GETCURSEL, 0, 0);
							if(nInd < 0) break;

							Test *test = m_testClassRelaList[nInd].testClass;
							int nCase = m_testClassRelaList[nInd].nCaseIndex;
							if(nCase >= 0)
							{
								//is test case
								//_sntprintf(sBuf, 512, _T("%s(line:%d):\r\n  file:%s\r\n"), test->getCaseName(nCase), test->getCaseLine(nCase), test->getCaseFile(nCase));
								cc::Buf::Format(sBuf, 513, _T("%s(line:%d):\r\n  file:%s\r\n"), test->getCaseName(nCase), test->getCaseLine(nCase), test->getCaseFile(nCase));
								setMsg_(sBuf, false);
								cc::Test::RunResult nResult = test->getRunResult(nCase);
								//ok:0;skip:-1;fatal:-2;error:-3;notrun:-9;
								if(nResult == cc::Test::RUN_OK)
								{
									setMsg_(_T("  run ok\r\n"), true);
									//_sntprintf(sBuf, 512, _T("Elapsed MilliSecond:[%.3f]\r\n"), test->getRunTime(nCase));
									cc::Buf::Format(sBuf, 513, _T("Elapsed MilliSecond:[%.3f]\r\n"), test->getRunTime(nCase));
									setMsg_(sBuf, true);
								}
								else if(nResult == cc::Test::RUN_SKIP)
								{
									setMsg_(_T("  run skipped"), true);
								}
								else if(nResult == cc::Test::RUN_NEVER)
								{
									setMsg_(_T("  not run"), true);
								}
								else if(test->getCaseMsg(nCase) != NULL)
								{
									setMsg_(_T("  Test Result:"), true);
									setMsg_(test->getCaseMsg(nCase), true);
								}
							}
							else
							{
								//is test class
								//_sntprintf(sBuf, 512, _T("%s(%s):\r\n  file:%s\r\n"), test->ClassName, test->ClassComment, test->FileName);
								cc::Buf::Format(sBuf, 513, _T("%s(%s):\r\n  file:%s\r\n"), test->ClassName, test->ClassComment, test->FileName);
								setMsg_(sBuf, false);
								int nCount = test->getCaseCount();
								int nFatal = test->getFatalCount();
								int nError = test->getErrorCount();
								//_sntprintf(sBuf, 512, _T("  TestCase:%d, OK(or not run):%d, Fatal:%d, Error:%d\r\n"), nCount, nCount - nFatal - nError, nFatal, nError);
								cc::Buf::Format(sBuf, 513, _T("  TestCase:%d, OK(or not run):%d, Fatal:%d, Error:%d\r\n"), nCount, nCount - nFatal - nError, nFatal, nError);
								setMsg_(sBuf, true);
							}
							break;
						}
					case LBN_DBLCLK:
						{
							int nInd = (int)SendMessage(m_hList, LB_GETCURSEL, 0, 0);
							if(nInd < 0) break;

							int nCase = m_testClassRelaList[nInd].nCaseIndex;
							if(nCase >= 0)
							{
								//is test case
								SendMessage(m_hList, LB_GETTEXT, nInd, (LPARAM)&sBuf);
								if(sBuf[3] == 'o')
								{
									sBuf[3] = 'x';
								}
								else
								{
									sBuf[3] = 'o';
								}
								refreshList_(nInd, sBuf); //refresh item
							}
							else
							{
								//is test class,so set all sub(testcase)
								int nChar;
								if(nCase == -1)
								{
									m_testClassRelaList[nInd].nCaseIndex = -2; //all check off
									nChar = _T('x');
								}
								else
								{
									m_testClassRelaList[nInd].nCaseIndex = -1; //all check on
									nChar = _T('o');
								}
								for(int i = nInd + 1; i < m_nListCount; i++)
								{
									int nCaseSub = m_testClassRelaList[i].nCaseIndex;
									if(nCaseSub < 0) break;
									SendMessage(m_hList, LB_GETTEXT, i, (LPARAM)&sBuf);
									sBuf[3] = nChar;
									refreshList_(i, sBuf); //refresh item
								}
							}
							break;
						}
					} //switch (LOWORD(wParam))
					return 0;
				}
			} //switch (LOWORD(wParam))
			return 0;
		}
	case WM_CREATE:
		{
			WndProc_Create_(hwnd);
			return 0;
		}
	case WM_DESTROY:
		{
			RECT rect;
			GetWindowRect(hwnd, &rect);
			RECT rect_lst;
			GetClientRect(m_hList, &rect_lst);

			TCHAR buf[100];
			TCHAR m_tcIniFile[_MAX_FNAME * 2 + 1] = {'\0'};
			GetModuleFileName(NULL, m_tcIniFile, _MAX_FNAME * 2);
			cc::Buf::Cat(m_tcIniFile, _MAX_FNAME * 2, _T(".ini\0"));
			//save position
			cc::Buf::Format(buf, 100, _T("%d"), rect.left);
			CC_LOG_SETCONFIG(_T(CC_LOG_CONFIG), _T("TextX"), buf, m_tcIniFile);
			cc::Buf::Format(buf, 100, _T("%d"), rect.top);
			CC_LOG_SETCONFIG(_T(CC_LOG_CONFIG), _T("TextY"), buf, m_tcIniFile);
			cc::Buf::Format(buf, 100, _T("%d"), rect.right - rect.left);
			CC_LOG_SETCONFIG(_T(CC_LOG_CONFIG), _T("TextW"), buf, m_tcIniFile);
			cc::Buf::Format(buf, 100, _T("%d"), rect.bottom - rect.top);
			CC_LOG_SETCONFIG(_T(CC_LOG_CONFIG), _T("TextH"), buf, m_tcIniFile);

			cc::Buf::Format(buf, 100, _T("%d"), m_SplitPosY);
			CC_LOG_SETCONFIG(_T(CC_LOG_CONFIG), _T("TextSplitH"), buf, m_tcIniFile);

			PostQuitMessage(0);    //signal end of application
			break;
		}
	case WM_SIZE:
		{
			RECT rect_cl;
			GetClientRect(hwnd, &rect_cl);
			if(m_SplitPosY <= 0)
			{
				m_SplitPosY = 70;
			}
			MoveWindow(m_hList, 2 , 30, rect_cl.right - 3, rect_cl.bottom - m_SplitPosY - 37, TRUE);
			RECT rect_lst;
			GetClientRect(m_hList, &rect_lst);
			MoveWindow(m_hSplitter, 2 , rect_lst.bottom + 35, rect_cl.right - 2, 5, TRUE);
			MoveWindow(m_hEdit, 2 , rect_lst.bottom + 39, rect_cl.right - 3, rect_cl.bottom - rect_lst.bottom - 40, TRUE);
			return 0;
		}
	default:
			break;
	}
	return DefWindowProc(hwnd,Message,wParam,lParam);  //let system deal with msg
}

bool TestRun::ShowDialog(HINSTANCE hInstance)
{
	m_hInstance = hInstance;
	if(m_hInstance == NULL)
	{
		m_hInstance = GetModuleHandle(NULL);
	}
	HWND	hwnd;	 //the wnd handle
	HICON	hIcon;	 //window icon
	HCURSOR hCursor; //window cursor
	//declare and initialise wnd registration information variables
	TCHAR chClassName[] = TEXT("TESTRUNWND");
	WNDCLASSEX wcx;    //this structure is used for storing information about the wnd 'class'
	hIcon = (HICON)LoadImage(0, IDI_APPLICATION, IMAGE_ICON, 0, 0, LR_SHARED);
	hCursor = (HCURSOR)LoadImage(0, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_SHARED);
	wcx.cbSize			 = sizeof(WNDCLASSEX);				//byte size of WNDCLASSEX struct
	wcx.style			 = CS_HREDRAW|CS_VREDRAW;			//ensure wnd is always redrawn
	wcx.lpfnWndProc 	 = (WNDPROC)WndProc_;				//pointer to the Window Procedure
	wcx.cbClsExtra		 = 0;								//Extra bytes for 'class' wnds
	wcx.cbWndExtra		 = 0;								//Extra bytes for this wnd
	wcx.hInstance		 = m_hInstance;						//Application instance
	wcx.hIcon			 = hIcon;							//Application icon
	wcx.hCursor 		 = hCursor; 						//Cursor for wnd
	wcx.hbrBackground	 = (HBRUSH)(COLOR_BTNFACE+1);		//Background wnd colour
	wcx.lpszMenuName	 = NULL;							//Name of wnd menu
	wcx.lpszClassName	 = chClassName; 					//Name of this wnd 'class'
	wcx.hIconSm 		 = NULL;							//Icon in top-left corner of wnd
	//Register the wnd class with the Windows system
	if(!RegisterClassEx(&wcx))
	{
		return false;
	}

	TCHAR m_tcIniFile[_MAX_FNAME * 2 + 1] = {'\0'};
	int nConsoleW;
	int nConsoleH;
	int nConsoleX;
	int nConsoleY;
	GetModuleFileName(NULL, m_tcIniFile, _MAX_FNAME * 2);
	cc::Buf::Cat(m_tcIniFile, _MAX_FNAME * 2, _T(".ini\0"));

	//get last pos from ini
	int nScreenX = GetSystemMetrics(SM_CXSCREEN);
	int nScreenY = GetSystemMetrics(SM_CYSCREEN);
	m_SplitPosY = CC_LOG_GETCONFIG(_T(CC_LOG_CONFIG), _T("TextSplitH"), 70, m_tcIniFile);
	nConsoleW = CC_LOG_GETCONFIG(_T(CC_LOG_CONFIG), _T("TextW"), 800, m_tcIniFile);
	nConsoleH = CC_LOG_GETCONFIG(_T(CC_LOG_CONFIG), _T("TextH"), 600, m_tcIniFile);
	if(nConsoleW > nScreenX)
	{
		nConsoleW = nScreenX;
	}
	if(nConsoleH > nScreenY)
	{
		nConsoleH = nScreenY;
	}
	nConsoleX = CC_LOG_GETCONFIG(_T(CC_LOG_CONFIG), _T("TextX"), (nScreenX - nConsoleW - 20)/2, m_tcIniFile);
	nConsoleY = CC_LOG_GETCONFIG(_T(CC_LOG_CONFIG), _T("TextY"), (nScreenY - nConsoleH - 30)/2, m_tcIniFile);
	if(nConsoleX < 0)
	{
		nConsoleX = 0;
	}
	else if(nConsoleX > nScreenX)
	{
		nConsoleX = nScreenX - 20;
	}
	if(nConsoleY < 0)
	{
		nConsoleY = 0;
	}
	else if(nConsoleY > nScreenY)
	{
		nConsoleY = nScreenY - 30;
	}

	//create wnd of the 'class' just registered
	hwnd = CreateWindowEx(0,									//more or 'extended' styles
						chClassName,						//the 'class' of window to create
						TEXT("ccTest - By Shu.KK(Version:20051216)"),  //the window title
						WS_OVERLAPPEDWINDOW,				//window style: how it looks
						nConsoleX, //(int)(GetSystemMetrics(SM_CXSCREEN) * 0.25),	//window position: left
						nConsoleY, //(int)(GetSystemMetrics(SM_CYSCREEN) * 0.25),	//window position: top
						nConsoleW, //(int)(GetSystemMetrics(SM_CXSCREEN) * 0.5),	//window width
						nConsoleH, //(int)(GetSystemMetrics(SM_CYSCREEN) * 0.5),	//window height
						NULL,								//parent window handle
						NULL,								//handle to this windows's menu
						m_hInstance,						//application instance
						NULL);								//user defined information
	if(!hwnd)
	{
		return false;
	}
	//Display the wnd
	ShowWindow(hwnd, SW_NORMAL);
	UpdateWindow(hwnd);

	//start message loop
	MSG Msg;	 //a simple structure for storing message information
	while(GetMessage(&Msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	::ExitProcess(0);
	return true;
}

bool TestRun::isCancel()
{
	MSG msg;
	while(::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
	{
		if(::GetMessage(&msg, NULL, 0, 0))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		else
		{
			break;
		}
	}
	return m_isCancel;
}

//Test *test will be deleted in this class,so need define this class with point and not del it out this class
void TestRun::addTestClass(Test *test)
{
	m_nClassCount++;
	if(m_nClassCount > m_nRealCount)
	{
		m_nRealCount += 10;
		m_testClassArr = (mEntry **)realloc(m_testClassArr, (m_nRealCount + 1) * sizeof(mEntry*));
	}
	//not delete (TCHAR *) at all
	mEntry *mBufTmp = new mEntry[1];
	memset(mBufTmp, '\0', sizeof(mEntry));
	//get testcase(must run once)
	test->treateTestCase();
	mBufTmp->testClass = test;
	*(m_testClassArr + m_nClassCount - 1) = mBufTmp;
}

