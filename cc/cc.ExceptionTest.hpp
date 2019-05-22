/* cc.ExceptionTest.hpp
   Test for cc.Exception
   Author: Shu.KK
   created:2006-07-28
*/
#ifndef cc_ExceptionTest_H_
#define cc_ExceptionTest_H_

#include <windows.h>
#include <stdio.h>
#include "cc.Test.h"
#include "cc.Exception.h"

namespace cc
{

//////////////////////// class fot test of Exception START ///////////////////
class ExceptionTest : public cc::Test
{
public:
	ExceptionTest()
	{
		TESTCLASS_COMMENT("test of cc::Exception");
	}

	void Setup()
	{
	}

	void TearDown()
	{
	}

	void TestCase()
	{
		TESTCASE_BEGIN("test:throw UserException(msg)/catch(UserException exp)")
		{
			int flg = 0;
			TCHAR* msg = _T("msg");
			try
			{
				throw UserException(msg);
			}
			catch(UserException exp)
			{
				flg = 1;
				//confirm the exception msg
				TESTASSERT_EQUAL(msg, exp.GetMessage());
			}
			//confirm that entered catch
			TESTASSERT_EQUAL(1, flg);
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test:throw UserException(msg)/catch(Exception exp)")
		{
			int flg = 0;
			TCHAR* msg = _T("msg");
			try
			{
				throw UserException(msg);
			}
			catch(Exception exp)
			{
				flg = 1;
				//confirm the exception msg
				TESTASSERT_EQUAL(msg, exp.GetMessage());
			}
			//confirm that entered catch
			TESTASSERT_EQUAL(1, flg);
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test:throw UserException(exp)/catch(Exception exp)")
		{
			int flg = 0;
			TCHAR* msg1 = EXCEPTION_MSG("msg1");
			TCHAR* msg2 = EXCEPTION_MSG("has error at base");
			int n = (int)_tcslen(msg1) + (int)_tcslen(msg2) + 2;
			try
			{
				try
				{
					throw UserException(msg1);
				}
				catch(Exception exp)
				{
					throw UserException(exp, msg2);
				}
				TESTASSERT_FATAL("not catch expect exception");
			}
			catch(Exception exp)
			{
				flg = 1;
				//confirm the exception msg
				TESTASSERT_EQUAL(msg2, exp.GetMessage());
				//TESTASSERT_EQUAL(msgall, exp.GetAllMessage());
				cc::Log::debug(_T("exp.GetAllMessage:"), exp.GetAllMessage());
			}
			//confirm that entered catch
			TESTASSERT_EQUAL(1, flg);
		}
		TESTCASE_END;
	};
};

} //namespace cc

#endif //#ifndef cc_ExceptionTest_H_
