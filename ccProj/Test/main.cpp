#include "../../cc/cc.All.h"
#include "../../cc/cc.TestRun.h"
#include "../../cc/cc.ExceptionTest.hpp"
//#include "../../cc/cc.DelegateTest.hpp"
#include "../../cc/cc.StrTest.hpp"
#include "../../cc/cc.ListTest.hpp"
#include "../../cc/cc.UtilTest.hpp"
#include "../../cc/cc.FileTest.hpp"
//#include "../../cc/cc.TimeTest.hpp"
//#include "../../cc/cc.win.FormTest.hpp"
using namespace cc;
using namespace cc::win;


namespace cc
{

//////////////////////// class fot test of Arr START ///////////////////
class TempTest : public cc::Test
{
public:
	TempTest()
	{
		TESTCLASS_COMMENT("test for Temp");
	}

	void Setup()
	{
	}

	void TearDown()
	{
	}

	void TestCase()
	{
		TESTCASE_BEGIN("test:readIni")
		{
		}
		TESTCASE_END;
	};
};

} //namespace cc

class ProjComponentTest
{
public:
	static void start()
	{
		//define testrun
		cc::TestRun testRun = cc::TestRun();

		//add testcalss
		cc::TempTest *tempTest = new cc::TempTest();
		testRun.addTestClass(tempTest);

		cc::ExceptionTest *exceptionTest = new cc::ExceptionTest();
		testRun.addTestClass(exceptionTest);
		//cc::DelegateTest *delegateTest = new cc::DelegateTest();
		//testRun.addTestClass(delegateTest);
		cc::StrTest *strTest = new cc::StrTest();
		testRun.addTestClass(strTest);
		cc::UtilTest *utilTest = new cc::UtilTest();
		testRun.addTestClass(utilTest);
		cc::FileTest *fileTest = new cc::FileTest();
		testRun.addTestClass(fileTest);
		cc::ListTest *listTest = new cc::ListTest();
		testRun.addTestClass(listTest);
		cc::MapTest *mapTest = new cc::MapTest();
		testRun.addTestClass(mapTest);
		cc::MapTableTest *mapTableTest = new cc::MapTableTest();
		testRun.addTestClass(mapTableTest);
		//cc::TimeTest *timeTest = new cc::TimeTest();
		//testRun.addTestClass(timeTest);
		//cc::win::FormTest *formTest = new cc::win::FormTest();
		//testRun.addTestClass(formTest);

		//show testrunner in modle dialog,and test will delete in testrunner
		testRun.ShowDialog();
	};
};

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
	cc::win::App::SetInstance(hInstance);
	ProjComponentTest::start();
	return 0;
}
