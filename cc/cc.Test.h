// cc.Test.h
// Test to screen
// Author: Shu.KK
// 2005-09-01
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef CC_Test_H_
#define CC_Test_H_

#include "cc.h"
#include <windows.h>
#include <wchar.h>
#include <tchar.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>

#include "cc.Log.h"

//define comment of class
#if defined(_UNICODE) && defined(__MINGW32__)
//for MINGW32 can not know _T(__FUNCTION__)
#define TESTCLASS_COMMENT(class_comment)	\
	FileName = _T(__FILE__);				\
	MultiByteToWideChar(CP_ACP, 0, __FUNCTION__, -1, ClassName, 200);	\
	ClassComment = _T( class_comment );
#else
#define TESTCLASS_COMMENT(class_comment)	\
	FileName = _T(__FILE__);				\
	ClassName = _T(__FUNCTION__);		\
	ClassComment = _T( class_comment );
#endif

//begin of testcase
#define TESTCASE_BEGIN(test_case_comment)	\
	do										\
	{										\
		if(!runTestCaseBegin(_T( test_case_comment ), _T(__FILE__), __LINE__))	\
		{									\
			break;							\
		}									\
		try									\
		{									\

//end of testcase
#define TESTCASE_END						\
		}									\
		catch(cc::TestException)			\
		{									\
		}									\
		catch(...)							\
		{									\
			error();						\
		}									\
		runTestCaseEnd();					\
	}while(false);

//is cancel testcase
#define TESTCASE_ISCANCEL()					\
	if(isCancel())							\
	{										\
		return;								\
	}

//assert equal
#define TESTASSERT_EQUAL(expected, actual) assertEquals(_T("assertion failed for not equals"), (expected), (actual), _T(__FILE__), __LINE__);
#define TESTASSERT_EQUAL_MSG(message, expected, actual) assertEquals(_T(message), (expected), (actual), _T(__FILE__), __LINE__);
#define TESTASSERT_NULL(condition) assertNull(_T("assertion failed for not null"), (condition), _T(__FILE__), __LINE__);
#define TESTASSERT_NOTNULL(condition) assertNotNull(_T("assertion failed for is null"), (condition), _T(__FILE__), __LINE__);
#define TESTASSERT_TRUE(condition) assertTrue(_T("assertion failed for not true"), (condition), _T(__FILE__), __LINE__);
#define TESTASSERT_FALSE(condition) assertFalse(_T("assertion failed for not false"), (condition), _T(__FILE__), __LINE__);
#define TESTASSERT_FATAL(message) fatal(_T(message), _T(__FILE__), __LINE__);

namespace cc
{

class TestException {
};

class Test
{
public:
	enum RunResult
	{
		RUN_OK = 0,
		RUN_SKIP = 1,
		RUN_FATAL = 2,
		RUN_ERROR = 3,
		RUN_NEVER = 4,
	};
	const TCHAR *FileName;
#if defined(_UNICODE) && defined(__MINGW32__)
	//only for _UNICODE&__MINGW32__
	TCHAR ClassName[200 + 1];
#else
	const TCHAR *ClassName;
#endif
	const TCHAR *ClassComment;
	bool isCanceled;
	bool isLogTrace;
protected:
	struct mEntry {
		const TCHAR *testName;
		const TCHAR *fileName;
		int nLine;
		bool isSkip;
		double nRunTime;
		TCHAR *testMsg;
		RunResult nResult;
	};
	mEntry **m_TestCaseArr;
	bool m_isRunTestNow;
	int m_nTestFatCount;
	int m_nTestErrCount;
	int m_nCaseCount;
	int m_nCaseIndex; //count m_nCaseIndex when run
	int m_nRealCount;
	int m_nRunCurLine;
	static LONGLONG m_llFreq;
	LARGE_INTEGER m_liClockStart;
	//clock_t m_nClockStart;
	//function to judge iscancel
	bool (*m_FunJudgeCancel)();
	bool isCancel();

	//pointer to a class method
	typedef void (Test::* CLASSFUN)(void *runFunParam);
	HANDLE _threadHandle;
	struct mExpect {
		bool isExpectting;
		const TCHAR *winClass;
		const TCHAR *winTitle;
		CLASSFUN runFun;
		void *runFunParam;
	};
	mExpect **mExpectEntity;
	int nExpectCount;
private:
	void Init_();
	void fatal_(TCHAR *msg);

public:
	// constructors
	Test();
	// destructor
	virtual ~Test();

	//========================================
	//MAIN FUNCTION
	void treateTestCase();
	void runTest();
	virtual void Setup();
	virtual void TestCase();
	virtual void TearDown();
	int getCaseCount();
	int getFatalCount();
	int getErrorCount();
	const TCHAR *getCaseName(int nIndex);
	const TCHAR *getCaseFile(int nIndex);
	TCHAR *getCaseMsg(int nIndex);
	int getCaseLine(int nIndex);
	bool getCaseSkip(int nIndex);
	void setCaseSkip(int nIndex, bool isSkip);
	RunResult getRunResult(int nIndex);
	double getRunTime(int nIndex);
	void setFunJudgeCancel(bool (*m_FunJudgeCancel)());

protected:
	//if true for run,or others for define testcase or skip
	bool runTestCaseBegin(TCHAR *testName, const TCHAR *fileName, int nLine);
	void runTestCaseEnd();
	TCHAR *Format(TCHAR *m_vBuf, const TCHAR *strFormat, ...);
	void setCaseMsg(int nIndex, TCHAR *msg);
	void setRunResult(int nIndex, RunResult nResult);
	void setRunTime(int nIndex, double nRunTime);

	//assert function
	void assertEquals(TCHAR *msg, int expected, int actual, const TCHAR *fileName, int nLine);
	void assertEquals(TCHAR *msg, long expected, long actual, const TCHAR *fileName, int nLine);
	void assertEquals(TCHAR *msg, float expected, float actual, const TCHAR *fileName, int nLine);
	void assertEquals(TCHAR *msg, double expected, double actual, const TCHAR *fileName, int nLine);
	void assertEquals(TCHAR *msg, const char *expected, const char *actual, const TCHAR *fileName, int nLine);
	void assertEquals(TCHAR *msg, const wchar_t *expected, const wchar_t *actual, const TCHAR *fileName, int nLine);

#if defined(_ccStr)
	void assertEquals(TCHAR *msg, const cc::Str &expected, const cc::Str &actual, const TCHAR *fileName, int nLine);
#endif
#if _INTEGRAL_MAX_BITS >= 64
	void assertEquals(TCHAR *msg, const __int64 expected, const __int64 actual, const TCHAR *fileName, int nLine);
#endif

	void assertNull(TCHAR *msg, const void *condition, const TCHAR *fileName, int nLine);
	void assertNotNull(TCHAR *msg, const void *condition, const TCHAR *fileName, int nLine);
	void assertTrue(TCHAR *msg, bool condition, const TCHAR *fileName, int nLine);
	void assertFalse(TCHAR *msg, bool condition, const TCHAR *fileName, int nLine);
	void fatal(TCHAR *msg, const TCHAR *fileName, int nLine);
	void error(); //msg is fixed

	//treate ExpectModal
	void ExpectModal(TCHAR *className, TCHAR *title, CLASSFUN runFun, void *runFunParam);
	static unsigned long _stdcall ExpectModalThread(void *thisClass);
};

} //namespace cc

#endif //#ifndef CC_Test_H_
