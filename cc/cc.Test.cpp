// cc.Test.cpp
// Test to screen
// Author: Shu.KK
// 2005/12/08
#include "cc.Test.h"
using namespace cc;

#ifdef _UNICODE
//need free memory
int codeASC2UNI(wchar_t **desNeedFree, const char *src)
{
	int nRet = -1;
	if(*desNeedFree != NULL)
	{
		free(*desNeedFree);
		*desNeedFree = NULL;
	}
	if(src != NULL)
	{
		nRet = MultiByteToWideChar(CP_ACP, 0, src, -1, NULL, 0);
		if(nRet >= 0)
		{
			*desNeedFree = (wchar_t*)malloc((nRet + 1) * sizeof(wchar_t));
			memset(*desNeedFree, '\0', (nRet + 1) * sizeof(wchar_t));
			nRet = MultiByteToWideChar(CP_ACP, 0, src, -1, *desNeedFree, nRet);
		}
	}
	return nRet;
}
#endif

#ifndef _UNICODE
//need free memory
int codeUNI2ASC(char **desNeedFree, const wchar_t *src)
{
	int nRet = -1;
	if(*desNeedFree != NULL)
	{
		free(*desNeedFree);
		*desNeedFree = NULL;
	}
	if(src != NULL)
	{
		nRet = WideCharToMultiByte(CP_ACP, 0, src, -1, NULL, 0, NULL, NULL);
		if(nRet >= 0)
		{
			*desNeedFree = (char*)malloc((nRet + 1) * sizeof(char));
			memset(*desNeedFree, '\0', (nRet + 1) * sizeof(char));
			nRet = WideCharToMultiByte(CP_ACP, 0, src, -1, *desNeedFree, nRet, NULL, NULL);
		}
	}
	return nRet;
}
#endif

///////////////////// virtual //////////////////////////
LONGLONG Test::m_llFreq = (LONGLONG)0.0;
//virtual Setup
void Test::Setup()
{
}

//virtual TestCase
void Test::TestCase()
{
	if(m_isRunTestNow)
	{
		cc::Log::debug(_T("need defined TestCase in your Class\r\n"));
	}
}

//virtual TearDown
void Test::TearDown()
{
}

///////////////////// constructors //////////////////////////
Test::Test()
{
	if(m_llFreq == 0.0)
	{
		::QueryPerformanceFrequency(&m_liClockStart);
		m_llFreq = m_liClockStart.QuadPart/1000;
	}
	Init_();
	FileName = _T(__FILE__);
#if defined(_UNICODE) && defined(__MINGW32__)
	//only for _UNICODE&__MINGW32__
	MultiByteToWideChar(CP_ACP, 0, __FUNCTION__, -1, ClassName, 200);
#else
	ClassName = _T(__FUNCTION__);
#endif
	ClassComment = _T("Test Base Class");
	m_nRunCurLine = -1;
}

///////////////////// destructor //////////////////////////
Test::~Test()
{
	if(m_TestCaseArr != NULL)
	{
		for(int i = 0; i < m_nCaseCount; i++)
		{
			mEntry *mBufTmp = *(m_TestCaseArr + i);
			if(mBufTmp->testMsg != NULL)
			{
				free(mBufTmp->testMsg);
			}
			delete []mBufTmp;
		}
		free(m_TestCaseArr);
		m_TestCaseArr = NULL;
	}
}

///////////////////// private/internal used methods //////////////////////////
void Test::Init_()
{
	//init inner various
	m_TestCaseArr = NULL;
	m_isRunTestNow = true;
	isLogTrace = true;
	m_nTestFatCount = 0;
	m_nTestErrCount= 0;
	m_nCaseCount= 0;
	m_nRealCount = 0;
	m_nCaseIndex = -1;
	m_FunJudgeCancel = NULL;
	isCanceled = false;

	_threadHandle = NULL;
	nExpectCount = 0;
	mExpectEntity = NULL;
}

TCHAR *Test::Format(TCHAR *m_vBuf, const TCHAR *strFormat, ...)
{
	CC_ASSERT(strFormat != NULL);
	va_list argList;
	va_start(argList, strFormat);
	Buf::FormatV(&m_vBuf, strFormat, argList);
	va_end(argList);
	return m_vBuf;
}

int Test::getCaseCount()
{
	return m_nCaseCount;
}

int Test::getFatalCount()
{
	return m_nTestFatCount;
}

int Test::getErrorCount()
{
	return m_nTestErrCount;
}

const TCHAR *Test::getCaseName(int nIndex)
{
	if(nIndex < 0 || nIndex >= m_nCaseCount) return NULL;
	return (*(m_TestCaseArr + nIndex))->testName;
}

const TCHAR *Test::getCaseFile(int nIndex)
{
	if(nIndex < 0 || nIndex >= m_nCaseCount) return NULL;
	return (*(m_TestCaseArr + nIndex))->fileName;
}

TCHAR *Test::getCaseMsg(int nIndex)
{
	if(nIndex < 0 || nIndex >= m_nCaseCount) return NULL;
	return (*(m_TestCaseArr + nIndex))->testMsg;
}

void Test::setCaseMsg(int nIndex, TCHAR *msg)
{
	if(nIndex < 0 || nIndex >= m_nCaseCount) return;
	if((*(m_TestCaseArr + nIndex))->testMsg != NULL)
	{
		free((*(m_TestCaseArr + nIndex))->testMsg);
	}
	(*(m_TestCaseArr + nIndex))->testMsg = msg;
}

int Test::getCaseLine(int nIndex)
{
	if(nIndex < 0 || nIndex >= m_nCaseCount) return -1;
	return (*(m_TestCaseArr + nIndex))->nLine;
}

bool Test::getCaseSkip(int nIndex)
{
	if(nIndex < 0 || nIndex >= m_nCaseCount) return false;
	return (*(m_TestCaseArr + nIndex))->isSkip;
}

void Test::setCaseSkip(int nIndex, bool isSkip)
{
	if(nIndex < 0 || nIndex >= m_nCaseCount) return;
	(*(m_TestCaseArr + nIndex))->isSkip = isSkip;
}

double Test::getRunTime(int nIndex)
{
	if(nIndex < 0 || nIndex >= m_nCaseCount) return -1;
	return (*(m_TestCaseArr + nIndex))->nRunTime;
}

void Test::setRunTime(int nIndex, double nRunTime)
{
	if(nIndex < 0 || nIndex >= m_nCaseCount) return;
	(*(m_TestCaseArr + nIndex))->nRunTime = nRunTime;
}

Test::RunResult Test::getRunResult(int nIndex)
{
	if(nIndex < 0 || nIndex >= m_nCaseCount) return RUN_NEVER;
	return (*(m_TestCaseArr + nIndex))->nResult;
}

void Test::setRunResult(int nIndex, RunResult nResult)
{
	if(nIndex < 0 || nIndex >= m_nCaseCount) return;
	(*(m_TestCaseArr + nIndex))->nResult = nResult;
}

void Test::setFunJudgeCancel(bool (*funJudgeCancel)())
{
	m_FunJudgeCancel = funJudgeCancel;
}

bool Test::runTestCaseBegin(TCHAR *testName, const TCHAR *fileName, int nLine)
{
	m_nRunCurLine = nLine;
	if(!m_isRunTestNow)
	{
		//add case
		m_nCaseCount++;
		if(m_nCaseCount > m_nRealCount)
		{
			m_nRealCount += 10;
			m_TestCaseArr = (mEntry **)realloc(m_TestCaseArr, (m_nRealCount + 1) * sizeof(mEntry*));
		}
		//not delete (TCHAR *) at all
		mEntry* mBufTmp = new mEntry[1];
		memset(mBufTmp, '\0', sizeof(mEntry));
		mBufTmp->testName = testName;
		mBufTmp->fileName = fileName;
		mBufTmp->testMsg = NULL;
		mBufTmp->nLine = nLine;
		mBufTmp->nResult = RUN_NEVER;
		mBufTmp->isSkip = false;
		mBufTmp->nRunTime = 0;
		*(m_TestCaseArr + m_nCaseCount - 1) = mBufTmp;
		return false;
	}
	else
	{
		//run test
		m_nCaseIndex++;
		if(isCancel())
		{
			return false;
		}
		if(getCaseSkip(m_nCaseIndex))
		{
			return false;
		}
		try
		{
			Setup();
		}
		catch(...)
		{
			//here count error,and set msg
			m_nTestErrCount++;
			setRunResult(m_nCaseIndex, RUN_ERROR);
			TCHAR *resultmsg = Format(NULL, _T("run Setup() error at[casename:%s, file:%s, line:%d]"),
				getCaseName(m_nCaseIndex), getCaseFile(m_nCaseIndex), getCaseLine(m_nCaseIndex));
			setCaseMsg(m_nCaseIndex, resultmsg);
			cc::Log::debug(resultmsg);
			return false;
		}
		setRunResult(m_nCaseIndex, cc::Test::RUN_OK);
		//start time
		//m_nClockStart = ::clock();
		::QueryPerformanceFrequency(&m_liClockStart);
		return true;
	}
}

void Test::runTestCaseEnd()
{
	if(m_isRunTestNow)
	{
		//setRunTime(m_nCaseIndex, (double)(clock() - m_nClockStart));
		LARGE_INTEGER m_liClockEnd;
		::QueryPerformanceCounter(&m_liClockEnd);
		//notice:MilliSecond
		setRunTime(m_nCaseIndex, (float)(m_liClockStart.QuadPart - m_liClockEnd.QuadPart)/m_llFreq);
		try
		{
			TearDown();
		}
		catch(...)
		{
			if(getRunResult(m_nCaseIndex) != RUN_ERROR)
			{
				//here count error,and set msg
				m_nTestErrCount++;
				setRunResult(m_nCaseIndex, RUN_ERROR);
				TCHAR *resultmsg = Format(NULL, _T("run TearDown() error at[casename:%s, file:%s, line:%d]"),
					getCaseName(m_nCaseIndex), getCaseFile(m_nCaseIndex), getCaseLine(m_nCaseIndex));
				setCaseMsg(m_nCaseIndex, resultmsg);
				cc::Log::debug(resultmsg);
			}
		}

		if(_threadHandle != NULL)
		{
			//delete Buf that for expectModal
			int nexpCnt = nExpectCount;
			int nNotExpect = 0;
			//set nExpectCount to 0,to exit of loop in ExpectModalThread
			nExpectCount = 0;
			Sleep(100);
			if(mExpectEntity != NULL)
			{
				for(int i = 0; i < nexpCnt; i++)
				{
					mExpect *entity = *(mExpectEntity + i);
					if(entity->isExpectting)
					{
						nNotExpect++;
					}
					delete []entity;
				}
				free(mExpectEntity);
				//must set to NULL,
				mExpectEntity = NULL;
			}
			if(nNotExpect > 0)
			{
				if(getRunResult(m_nCaseIndex) != RUN_ERROR)
				{
					//here count error,and set msg
					m_nTestErrCount++;
					setRunResult(m_nCaseIndex, RUN_ERROR);
					TCHAR *resultmsg = Format(NULL, _T("exist without ExpectModal(Count:%d) at[casename:%s, file:%s, line:%d]"),
						nNotExpect, getCaseName(m_nCaseIndex), getCaseFile(m_nCaseIndex), getCaseLine(m_nCaseIndex));
					setCaseMsg(m_nCaseIndex, resultmsg);
					cc::Log::debug(resultmsg);
				}
			}
			//here wait until end of ExpectModalThread
			::WaitForSingleObject(_threadHandle, INFINITE);
			_threadHandle = NULL;
		}
	}
}

//run
void Test::treateTestCase()
{
	//treate only need once
	if(m_TestCaseArr != NULL)
	{
		//get TestCase with TestCase() & m_isRunTestNow = false
		return;
	}
	else
	{
		//get TestCase with TestCase() & m_isRunTestNow = false
		m_isRunTestNow = false;
		m_nCaseIndex = -1;
		TestCase();
	}
}

//run
void Test::runTest()
{
	//now run Test with m_isRunTestNow = true
	m_isRunTestNow = true;
	m_nTestFatCount = 0;
	m_nTestErrCount= 0;
	m_nCaseIndex = -1;

	//restore status
	for(int i = 0; i < m_nCaseCount; i++)
	{
		mEntry *mBufTmp = *(m_TestCaseArr + i);
		if(mBufTmp->testMsg != NULL)
		{
			free(mBufTmp->testMsg);
			mBufTmp->testMsg = NULL;
		}
		if(mBufTmp->isSkip)
		{
			mBufTmp->nResult = RUN_SKIP;
		}
		else
		{
			mBufTmp->nResult = RUN_NEVER;
		}
	}

	//run test
	TestCase();
}

bool Test::isCancel()
{
	if(m_FunJudgeCancel == NULL) return false;
	//here save if is canceled
	isCanceled = m_FunJudgeCancel();
	if(isCanceled)
	{
		cc::Log::debug(_T("canceled by user."));
	}
	return isCanceled;
}

void Test::error()
{
	//here count error,and set msg
	m_nTestErrCount++;
	setRunResult(m_nCaseIndex, RUN_ERROR);
	TCHAR *resultmsg = Format(NULL, _T("error at[casename:%s, file:%s, line:%d],after line:%d"),
		getCaseName(m_nCaseIndex), getCaseFile(m_nCaseIndex), getCaseLine(m_nCaseIndex), m_nRunCurLine);
	setCaseMsg(m_nCaseIndex, resultmsg);
	cc::Log::debug(resultmsg);
}

void Test::fatal_(TCHAR *resultmsg)
{
	//here count fatal,and set msg
	m_nTestFatCount++;
	setRunResult(m_nCaseIndex, RUN_FATAL);
	setCaseMsg(m_nCaseIndex, resultmsg);
	cc::Log::debug(resultmsg);
	throw TestException();
}

void Test::fatal(TCHAR *msg, const TCHAR *fileName, int nLine)
{
	m_nRunCurLine = nLine;
	TCHAR *resultmsg = Format(NULL, _T("fatal at[casename:%s, file:%s, line:%d]"),
		getCaseName(m_nCaseIndex), fileName, nLine);
	fatal_(resultmsg);
}

void Test::assertEquals(TCHAR *msg, int expected, int actual, const TCHAR *fileName, int nLine)
{
	if(isLogTrace)
	{
		cc::Log::trace(_T("Casename:%s, Msg:%s, File:%s, Line:%d"), getCaseName(m_nCaseIndex), msg, fileName, nLine);
	}
	m_nRunCurLine = nLine;
	if(expected != actual)
	{
		TCHAR *resultmsg = Format(NULL, _T("%s[int,expected:<%d>,but was:<%d>], fatal at[casename:%s, file:%s, line:%d]"),
			msg, expected, actual, getCaseName(m_nCaseIndex), fileName, nLine);
		fatal_(resultmsg);
	}
}

void Test::assertEquals(TCHAR *msg, long expected, long actual, const TCHAR *fileName, int nLine)
{
	if(isLogTrace)
	{
		cc::Log::trace(_T("Casename:%s, Msg:%s, File:%s, Line:%d"), getCaseName(m_nCaseIndex), msg, fileName, nLine);
	}
	m_nRunCurLine = nLine;
	if(expected != actual)
	{
		TCHAR *resultmsg = Format(NULL, _T("%s[long,expected:<%d>,but was:<%d>], fatal at[casename:%s, file:%s, line:%d]"),
			msg, expected, actual, getCaseName(m_nCaseIndex), fileName, nLine);
		fatal_(resultmsg);
	}
}

void Test::assertEquals(TCHAR *msg, float expected, float actual, const TCHAR *fileName, int nLine)
{
	if(isLogTrace)
	{
		cc::Log::trace(_T("Casename:%s, Msg:%s, File:%s, Line:%d"), getCaseName(m_nCaseIndex), msg, fileName, nLine);
	}
	m_nRunCurLine = nLine;
	if(expected != actual)
	{
		TCHAR *resultmsg = Format(NULL, _T("%s[float,expected:<%f>,but was:<%f>], fatal at[casename:%s, file:%s, line:%d]"),
			msg, expected, actual, getCaseName(m_nCaseIndex), fileName, nLine);
		fatal_(resultmsg);
	}
}

void Test::assertEquals(TCHAR *msg, double expected, double actual, const TCHAR *fileName, int nLine)
{
	if(isLogTrace)
	{
		cc::Log::trace(_T("Casename:%s, Msg:%s, File:%s, Line:%d"), getCaseName(m_nCaseIndex), msg, fileName, nLine);
	}
	m_nRunCurLine = nLine;
	if(expected != actual)
	{
		TCHAR *resultmsg = Format(NULL, _T("%s[double,expected:<%f>,but was:<%f>], fatal at[casename:%s, file:%s, line:%d]"),
			msg, expected, actual, getCaseName(m_nCaseIndex), fileName, nLine);
		fatal_(resultmsg);
	}
}

void Test::assertEquals(TCHAR *msg, const char *expected, const char *actual, const TCHAR *fileName, int nLine)
{
	if(isLogTrace)
	{
		cc::Log::trace(_T("Casename:%s, Msg:%s, File:%s, Line:%d"), getCaseName(m_nCaseIndex), msg, fileName, nLine);
	}
	m_nRunCurLine = nLine;
	if((expected == NULL && actual == NULL))
	{
		return;
	}
	if((expected == NULL && actual != NULL) || (expected != NULL && actual == NULL) || strcmp(expected, actual) != 0)
	{
#ifdef _UNICODE
		wchar_t *expected2 = NULL;
		wchar_t *actual2 = NULL;
		codeASC2UNI(&expected2, expected);
		codeASC2UNI(&actual2, actual);
		TCHAR *resultmsg = Format(NULL, _T("%s[cc::Str&,expected:<%s>,but was:<%s>], fatal at[casename:%s, file:%s, line:%d]"),
			msg, expected2, actual2, getCaseName(m_nCaseIndex), fileName, nLine);
		if(expected2 != NULL)
		{
			free(expected2);
		}
		if(actual2 != NULL)
		{
			free(actual2);
		}
#else
		TCHAR *resultmsg = Format(NULL, _T("%s[char*,expected:<%s>,but was:<%s>], fatal at[casename:%s, file:%s, line:%d]"),
			msg, expected, actual, getCaseName(m_nCaseIndex), fileName, nLine);
#endif
		fatal_(resultmsg);
	}
}

void Test::assertEquals(TCHAR *msg, const wchar_t *expected, const wchar_t *actual, const TCHAR *fileName, int nLine)
{
	if(isLogTrace)
	{
		cc::Log::trace(_T("Casename:%s, Msg:%s, File:%s, Line:%d"), getCaseName(m_nCaseIndex), msg, fileName, nLine);
	}
	m_nRunCurLine = nLine;
	if((expected == NULL && actual == NULL))
	{
		return;
	}
	if((expected == NULL && actual != NULL) || (expected != NULL && actual == NULL) || wcscmp(expected, actual) != 0)
	{
#ifdef _UNICODE
		TCHAR *resultmsg = Format(NULL, _T("%s[wchar_t*,expected:<%s>,but was:<%s>], fatal at[casename:%s, file:%s, line:%d]"),
			msg, expected, actual, getCaseName(m_nCaseIndex), fileName, nLine);
#else
		char *expected2 = NULL;
		char *actual2 = NULL;
		codeUNI2ASC(&expected2, expected);
		codeUNI2ASC(&actual2, actual);
		TCHAR *resultmsg = Format(NULL, _T("%s[cc::Str&,expected:<%s>,but was:<%s>], fatal at[casename:%s, file:%s, line:%d]"),
			msg, expected2, actual2, getCaseName(m_nCaseIndex), fileName, nLine);
		if(expected2 != NULL)
		{
			free(expected2);
		}
		if(actual2 != NULL)
		{
			free(actual2);
		}
#endif
		fatal_(resultmsg);
	}
}

#if defined(_ccStr)
void Test::assertEquals(TCHAR *msg, const cc::Str &expected, const cc::Str &actual, const TCHAR *fileName, int nLine)
{
	if(isLogTrace)
	{
		cc::Log::trace(_T("Casename:%s, Msg:%s, File:%s, Line:%d"), getCaseName(m_nCaseIndex), msg, fileName, nLine);
	}
	m_nRunCurLine = nLine;
	if((expected.IsNULL() && actual.IsNULL()))
	{
		return;
	}
	if((expected.IsNULL() && !actual.IsNULL()) || (!expected.IsNULL() && actual.IsNULL()) || expected != actual)
	{
		TCHAR *resultmsg;
		if(expected.GetLangCode() != LANGC_TCHAR || actual.GetLangCode() != LANGC_TCHAR)
		{
			cc::Str expected2 = cc::Str(LANGC_TCHAR, expected);
			cc::Str actual2 = cc::Str(LANGC_TCHAR, actual);
			resultmsg = Format(NULL, _T("%s[cc::Str&,expected:<%s>,but was:<%s>], fatal at[casename:%s, file:%s, line:%d]"),
				msg, (TCHAR*)expected2.GetBuf(), (TCHAR*)actual2.GetBuf(), getCaseName(m_nCaseIndex), fileName, nLine);
		}
		else
		{
			resultmsg = Format(NULL, _T("%s[cc::Str&,expected:<%s>,but was:<%s>], fatal at[casename:%s, file:%s, line:%d]"),
				msg, (TCHAR*)expected.GetBuf(), (TCHAR*)actual.GetBuf(), getCaseName(m_nCaseIndex), fileName, nLine);
		}
		fatal_(resultmsg);
	}
}
#endif

#if _INTEGRAL_MAX_BITS >= 64
void Test::assertEquals(TCHAR *msg, const __int64 expected, const __int64 actual, const TCHAR *fileName, int nLine)
{
	if(isLogTrace)
	{
		cc::Log::trace(_T("Casename:%s, Msg:%s, File:%s, Line:%d"), getCaseName(m_nCaseIndex), msg, fileName, nLine);
	}
	m_nRunCurLine = nLine;
	if(expected != actual)
	{
		TCHAR *resultmsg = Format(NULL, _T("%s[__int64,expected:<%f>,but was:<%f>], fatal at[casename:%s, file:%s, line:%d]"),
				msg, expected, actual, getCaseName(m_nCaseIndex), fileName, nLine);
		fatal_(resultmsg);
	}
}
#endif

void Test::assertNull(TCHAR *msg, const void *condition, const TCHAR *fileName, int nLine)
{
	if(isLogTrace)
	{
		cc::Log::trace(_T("Casename:%s, Msg:%s, File:%s, Line:%d"), getCaseName(m_nCaseIndex), msg, fileName, nLine);
	}
	m_nRunCurLine = nLine;
	if(condition != NULL)
	{
		TCHAR *resultmsg = Format(NULL, _T("%s[expected:<null>,but was:<not null>], fatal at[casename:%s, file:%s, line:%d]"),
			msg, getCaseName(m_nCaseIndex), fileName, nLine);
		fatal_(resultmsg);
	}
}

void Test::assertNotNull(TCHAR *msg, const void *condition, const TCHAR *fileName, int nLine)
{
	if(isLogTrace)
	{
		cc::Log::trace(_T("Casename:%s, Msg:%s, File:%s, Line:%d"), getCaseName(m_nCaseIndex), msg, fileName, nLine);
	}
	m_nRunCurLine = nLine;
	if(condition == NULL)
	{
		TCHAR *resultmsg = Format(NULL, _T("%s[expected:<not null>,but was:<null>], fatal at[casename:%s, file:%s, line:%d]"),
			msg, getCaseName(m_nCaseIndex), fileName, nLine);
		fatal_(resultmsg);
	}
}

void Test::assertTrue(TCHAR *msg, bool condition, const TCHAR *fileName, int nLine)
{
	if(isLogTrace)
	{
		cc::Log::trace(_T("Casename:%s, Msg:%s, File:%s, Line:%d"), getCaseName(m_nCaseIndex), msg, fileName, nLine);
	}
	m_nRunCurLine = nLine;
	if(!condition)
	{
		TCHAR *resultmsg = Format(NULL, _T("%s[expected:<true>,but was:<false>], fatal at[casename:%s, file:%s, line:%d]"),
			msg, getCaseName(m_nCaseIndex), fileName, nLine);
		fatal_(resultmsg);
	}
}

void Test::assertFalse(TCHAR *msg, bool condition, const TCHAR *fileName, int nLine)
{
	if(isLogTrace)
	{
		cc::Log::trace(_T("Casename:%s, Msg:%s, File:%s, Line:%d"), getCaseName(m_nCaseIndex), msg, fileName, nLine);
	}
	m_nRunCurLine = nLine;
	if(condition)
	{
		TCHAR *resultmsg = Format(NULL, _T("%s[expected:<false>,but was:<true>], fatal at[casename:%s, file:%s, line:%d]"),
			msg, getCaseName(m_nCaseIndex), fileName, nLine);
		fatal_(resultmsg);
	}
}

//treate ExpectModal
unsigned long _stdcall Test::ExpectModalThread(void *thisClass)
{
	Test *test = (Test*)thisClass;
	if(test->isLogTrace)
	{
		cc::Log::trace(_T("Casename:%s, ClassName:%s."), test->getCaseName(test->m_nCaseIndex), test->ClassName);
	}
	while(test->mExpectEntity != NULL)
	{
		for(int i = 0; i < test->nExpectCount; i++)
		{
			mExpect *entity = *(test->mExpectEntity + i);
			if(entity->isExpectting)
			{
				HWND winHandle = FindWindow(entity->winClass, entity->winTitle);
				if(winHandle > 0)
				{
					entity->isExpectting = false;
					//run Fun of ExpectModal
					(test->*(entity->runFun))(entity->runFunParam);
				}
			}
		}
		Sleep(200);
	}
	return 0;
}

void Test::ExpectModal(TCHAR *className, TCHAR *title, CLASSFUN runFun, void *runFunParam)
{
	if(isLogTrace)
	{
		cc::Log::trace(_T("Casename:%s, ClassName:%s, Title:%s.."), getCaseName(m_nCaseIndex), className, title);
	}
	bool isFirst = (mExpectEntity == NULL);
	//memory is released at runTestCaseEnd
	mExpect *entity = new mExpect[1];
	entity->winClass = className;
	entity->winTitle = title;
	entity->isExpectting = true;
	entity->runFun = (CLASSFUN)runFun;
	entity->runFunParam = runFunParam;
	mExpectEntity = (mExpect **)realloc(mExpectEntity, (nExpectCount + 1) * sizeof(mExpect*));
	*(mExpectEntity + nExpectCount) = entity;
	//when set mExpectEntity over then add nExpectCount!!used at ExpectModalThread
	nExpectCount++;
	if(isFirst)
	{
		//only first time need to start thread
		//why not use _beginthreadex?see head of "cc.File.h"
		_threadHandle = CreateThread(NULL, 0, ExpectModalThread, (void*)&*this, 0, NULL);
		Sleep(100);
	}
}
