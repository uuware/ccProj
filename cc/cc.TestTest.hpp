// cc.TestTest.h
// Test of cc.Test
// Author: Shu.KK
// 2005-02-20
#ifndef cc_TestTest_H_
#define cc_TestTest_H_

#include "cc.Log.h"
#include "cc.Test.h"

namespace cc
{

class TestTestSamp
{
	public:
		int add(int n1,int n2)
		{
			return (n1 + n2);
		};
		TCHAR *add(TCHAR *tc1, const TCHAR *tc2)
		{
			return _tcscat(tc1, tc2);
		};
};

class TestTest : public cc::Test
{
public:
	TestTest()
	{
		TESTCLASS_COMMENT("sample test");
	}

	void Setup()
	{
	}

	void TearDown()
	{
	}

	void TestCase()
	{
		TESTCASE_BEGIN("test null")
		{
			TestTestSamp* s1 = NULL;
			TESTASSERT_NULL(s1);
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test not null")
		{
			TestTestSamp* s1 = new TestTestSamp();
			TESTASSERT_NOTNULL(s1);
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test fatal")
		{
			TESTASSERT_FATAL("must fatal here");
		}
		TESTCASE_END;

		TESTCASE_BEGIN("equals of int")
		{
			TestTestSamp* s1 = new TestTestSamp();
			int nRes = s1->add(1, 2);
			TESTASSERT_EQUAL("err for add of int", 3, nRes);
		}
		TESTCASE_END;

		TESTCASE_BEGIN("equals of TCHAR")
		{
			TestTestSamp* s1 = new TestTestSamp();
			TCHAR test[10];
			_tcscpy(test, _T("AABBCC"));
			TCHAR *result = s1->add(test, _T("ZZ"));
			TCHAR expect[10];
			_tcscpy(expect, _T("AABBCCZZ"));
			TESTASSERT_EQUAL("err for add of TCHAR", expect, result);
		}
		TESTCASE_END;
	};
};

} //namespace cc

#endif //#ifndef cc_TestTest_H_
