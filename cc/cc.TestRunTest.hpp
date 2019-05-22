// cc.TestRunTest.h
// TestRunTest
// Author: Shu.KK
// 2005-12-01
#ifndef cc_TestRunTest_H_
#define cc_TestRunTest_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "cc.TestTest.hpp"
#include "cc.TestRun.h"

namespace cc
{

class TestRunTest
{
public:
	static void start()
	{
		//define testrun
		cc::TestRun testRun = cc::TestRun();

		//add testcalss
		cc::TestTest *samp = new cc::TestTest();
		testRun.addTestClass(samp);
		cc::TestTest *samp2 = new cc::TestTest();
		testRun.addTestClass(samp2);

		//show testrunner in modle dialog
		testRun.ShowDialog(NULL);
	};
};

} //namespace cc

#endif //#ifndef cc_TestRunTest_H_
