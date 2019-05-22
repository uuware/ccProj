#if _MSC_VER > 1000
#pragma once
#endif //#if _MSC_VER > 1000

#include <windows.h>
#include <string.h>
#include <wchar.h>
#include <tchar.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <comdef.h>
#include <conio.h>

#include "../../cc/cc.h"
#include "../../cc/cc.StrTest.hpp"
#include "../../cc/cc.TestRun.h"
using namespace cc;

class ProjComponentTest
{
public:
	static void start()
	{
		//define testrun
		cc::TestRun testRun = cc::TestRun();

		//add testcalss
		cc::StrTest *strTest = new cc::StrTest();
		testRun.addTestClass(strTest);

		//show testrunner in modle dialog,and test will delete in testrunner
		testRun.ShowDialog();
	};
};

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
	ProjComponentTest::start();
	return 0;
}
