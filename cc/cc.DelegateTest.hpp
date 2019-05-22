/* cc.DelegateTest.hpp
   Test for cc.Delegate
   Author: Shu.KK
   created:2006-08-01
*/
#ifndef cc_DelegateTest_H_
#define cc_DelegateTest_H_

#include <windows.h>
#include <stdio.h>
#include "cc.Test.h"
#include "cc.Delegate.h"

namespace cc
{

//////////////////////// class fot test of Delegate START ///////////////////
class DelegateTest : public cc::Test
{
public:
	DelegateTest()
	{
		TESTCLASS_COMMENT("test of cc::Delegate");
	}

	void Setup()
	{
	}

	void TearDown()
	{
	}

	int flgForDelegateTest;
	void Delegate_NoParam()
	{
		flgForDelegateTest = 1;
	}

	void Delegate_WithParam1(int n)
	{
		flgForDelegateTest = n;
	}

	int Delegate_WithParam2(int n, DelegateTest* test)
	{
		flgForDelegateTest = n;
		return test->flgForDelegateTest;
	}

	void TestCase()
	{
		TESTCASE_BEGIN("test:Delegate<DelegateTest, void> delegate")
		{
			flgForDelegateTest = 0;
			Delegate<DelegateTest, void (DelegateTest::*)()> delegate(this, Delegate_NoParam);
			if(delegate.obj !=  NULL)
			{
				(delegate.obj->*delegate.method)();
			}
			//Delegate0<DelegateTest, void> delegate0(this, Delegate_NoParam);
			//delegate0();
			TESTASSERT_EQUAL(1, flgForDelegateTest);
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test:Delegate<DelegateTest, void, int> delegate")
		{
			flgForDelegateTest = 0;
			Delegate<DelegateTest, void (DelegateTest::*)(int)> delegate(this, Delegate_WithParam1);
			if(delegate.obj !=  NULL)
			{
				(delegate.obj->*delegate.method)(2);
			}
			//Delegate1<DelegateTest, void, int> delegate1(this, Delegate_WithParam1);
			//delegate1(2);
			TESTASSERT_EQUAL(2, flgForDelegateTest);
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test:Delegate<DelegateTest, int, int, DelegateTest*> delegate")
		{
			flgForDelegateTest = 0;
			int nRetu = 0;
			Delegate<DelegateTest, int (DelegateTest::*)(int, DelegateTest*)> delegate(this, Delegate_WithParam2);
			if(delegate.obj !=  NULL)
			{
				nRetu = (delegate.obj->*delegate.method)(2, this);
			}
			//Delegate2<DelegateTest, int, int, DelegateTest*> delegate2(this, Delegate_WithParam2);
			//nRetu = delegate2(3, this);
			TESTASSERT_EQUAL(3, nRetu);
			TESTASSERT_EQUAL(3, flgForDelegateTest);
		}
		TESTCASE_END;
	};
};

} //namespace cc

#endif //#ifndef cc_DelegateTest_H_
