/* cc.TimeTest.hpp
   Test for cc.Time
   Author: Shu.KK
   created:2006-01-19
*/
#ifndef cc_TimeTest_H_
#define cc_TimeTest_H_

#include <windows.h>
#include <process.h>
#include <stdio.h>
#include <time.h>

#include "cc.h"
#include "cc.Time.h"
#include "cc.Test.h"

namespace cc
{

//////////////////////// class fot test of File START ///////////////////
class TimeTest : public cc::Test
{
public:
	TimeTest()
	{
		TESTCLASS_COMMENT("test of cc::Time");
	}

	void Setup()
	{
	}

	void TearDown()
	{
	}

	void TestCase()
	{
		TESTCASE_BEGIN("test:constructors of time")
		{
			//default is 0
			Time time;
			TESTASSERT_EQUAL((__int64)0, time.GetTotalSeconds());

			//load from __time64_t
			struct tm testTM;
			testTM.tm_sec = 59;
			testTM.tm_min = 50;
			testTM.tm_hour = 17;
			testTM.tm_mday = 20;
			testTM.tm_mon = 1 - 1;        // tm_mon is 0 based
			testTM.tm_year = 2006 - 1900;     // tm_year is 1900 based
			__time64_t time64 = mktime(&testTM);
			Time time2(time64);
			TESTASSERT_EQUAL(2006, time2.Year());
			TESTASSERT_EQUAL(1, time2.Month());
			TESTASSERT_EQUAL(20, time2.Day());
			TESTASSERT_EQUAL(17, time2.Hour());
			TESTASSERT_EQUAL(50, time2.Minute());
			TESTASSERT_EQUAL(59, time2.Second());

			//specify for ymd
			Time time3(2006, 1, 20, 17, 50, 0);
			TESTASSERT_EQUAL(2006, time3.Year());
			TESTASSERT_EQUAL(1, time3.Month());
			TESTASSERT_EQUAL(20, time3.Day());
			TESTASSERT_EQUAL(17, time3.Hour());
			TESTASSERT_EQUAL(50, time3.Minute());
			TESTASSERT_EQUAL(0, time3.Second());
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test:GetCurrentTime")
		{
			//default is 0
			Time time = Time::GetCurrentTime();
			cc::Str str = time.Format(_T("%Y/%m/%d %H:%M:%S"));
			cc::Log::debug(_T("cc::Time::GetCurrentTime:%s"), (TCHAR*)str.GetBuf());
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test:add(int nYear, int nMonth, int nDay, int nHour = 0, int nMin = 0, int nSec = 0)")
		{
			//specify for ymd
			Time time(2004, 2, 28, 0, 0, 0);
			time.add(0, 0, 2, 1, 1, 1);
			TESTASSERT_EQUAL(2004, time.Year());
			TESTASSERT_EQUAL(3, time.Month());
			TESTASSERT_EQUAL(1, time.Day());
			TESTASSERT_EQUAL(1, time.Hour());
			TESTASSERT_EQUAL(1, time.Minute());
			TESTASSERT_EQUAL(1, time.Second());

			time = Time(2004, 2, 28, 0, 0, 0);
			time.add(0, 0, 0, 48, 1, 1);
			TESTASSERT_EQUAL(2004, time.Year());
			TESTASSERT_EQUAL(3, time.Month());
			TESTASSERT_EQUAL(1, time.Day());
			TESTASSERT_EQUAL(0, time.Hour());
			TESTASSERT_EQUAL(1, time.Minute());
			TESTASSERT_EQUAL(1, time.Second());

			//pay attation to this
			time = Time(2004, 1, 31, 0, 0, 0);
			time.add(4, 1, 0, 0, 0, 0);
			TESTASSERT_EQUAL(2008, time.Year());
			TESTASSERT_EQUAL(3, time.Month());
			TESTASSERT_EQUAL(2, time.Day());
			TESTASSERT_EQUAL(0, time.Hour());
			TESTASSERT_EQUAL(0, time.Minute());
			TESTASSERT_EQUAL(0, time.Second());

			time = Time(2004, 1, 20, 0, 0, 0);
			time.add(4, 1, 10, 0, 0, 0);
			TESTASSERT_EQUAL(2008, time.Year());
			TESTASSERT_EQUAL(3, time.Month());
			TESTASSERT_EQUAL(1, time.Day());
			TESTASSERT_EQUAL(0, time.Hour());
			TESTASSERT_EQUAL(0, time.Minute());
			TESTASSERT_EQUAL(0, time.Second());
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test:addYear/addMonth/addDay/addHour/addMinute/addSecond")
		{
			//default is 0
			Time timeY(2006, 1, 20, 17, 50, 30);
			TESTASSERT_EQUAL(2006, timeY.Year());
			timeY.addYear(1);
			TESTASSERT_EQUAL(2007, timeY.Year());
			timeY.addYear(-2);
			TESTASSERT_EQUAL(2005, timeY.Year());

			Time timeMon(2006, 1, 20, 17, 50, 30);
			TESTASSERT_EQUAL(1, timeMon.Month());
			timeMon.addMonth(11);
			TESTASSERT_EQUAL(12, timeMon.Month());
			timeMon.addMonth(2);
			TESTASSERT_EQUAL(2, timeMon.Month());
			TESTASSERT_EQUAL(2007, timeMon.Year());
			timeMon.addMonth(24);
			TESTASSERT_EQUAL(2, timeMon.Month());
			TESTASSERT_EQUAL(2009, timeMon.Year());
			timeMon.addMonth(-2);
			TESTASSERT_EQUAL(12, timeMon.Month());
			TESTASSERT_EQUAL(2008, timeMon.Year());

			Time timeD(2004, 2, 28, 17, 50, 30);
			TESTASSERT_EQUAL(28, timeD.Day());
			timeD.addDay(1);
			TESTASSERT_EQUAL(29, timeD.Day());
			timeD.addDay(1);
			TESTASSERT_EQUAL(1, timeD.Day());
			TESTASSERT_EQUAL(3, timeD.Month());

			Time timeD2(2004, 12, 30, 17, 50, 30);
			TESTASSERT_EQUAL(30, timeD2.Day());
			timeD2.addDay(1);
			TESTASSERT_EQUAL(31, timeD2.Day());
			timeD2.addDay(1);
			TESTASSERT_EQUAL(1, timeD2.Day());
			TESTASSERT_EQUAL(1, timeD2.Month());
			TESTASSERT_EQUAL(2005, timeD2.Year());

			Time timeD3(2005, 1, 2, 17, 50, 30);
			TESTASSERT_EQUAL(2, timeD3.Day());
			timeD3.addDay(-1);
			TESTASSERT_EQUAL(1, timeD3.Day());
			timeD3.addDay(-1);
			TESTASSERT_EQUAL(31, timeD3.Day());
			TESTASSERT_EQUAL(12, timeD3.Month());
			TESTASSERT_EQUAL(2004, timeD3.Year());

			Time timeH(2006, 12, 31, 22, 50, 30);
			TESTASSERT_EQUAL(22, timeH.Hour());
			timeH.addHour(1);
			TESTASSERT_EQUAL(23, timeH.Hour());
			timeH.addHour(1);
			TESTASSERT_EQUAL(0, timeH.Hour());
			TESTASSERT_EQUAL(1, timeH.Day());
			TESTASSERT_EQUAL(1, timeH.Month());
			TESTASSERT_EQUAL(2007, timeH.Year());

			Time timeH2(2006, 1, 1, 1, 50, 30);
			TESTASSERT_EQUAL(1, timeH2.Hour());
			timeH2.addHour(-1);
			TESTASSERT_EQUAL(0, timeH2.Hour());
			timeH2.addHour(-1);
			TESTASSERT_EQUAL(23, timeH2.Hour());
			TESTASSERT_EQUAL(31, timeH2.Day());
			TESTASSERT_EQUAL(12, timeH2.Month());
			TESTASSERT_EQUAL(2005, timeH2.Year());

			Time timeM(2006, 12, 31, 23, 50, 30);
			TESTASSERT_EQUAL(50, timeM.Minute());
			timeM.addMinute(9);
			TESTASSERT_EQUAL(59, timeM.Minute());
			timeM.addMinute(1);
			TESTASSERT_EQUAL(0, timeM.Minute());
			TESTASSERT_EQUAL(0, timeM.Hour());
			TESTASSERT_EQUAL(1, timeM.Day());
			TESTASSERT_EQUAL(1, timeM.Month());
			TESTASSERT_EQUAL(2007, timeM.Year());

			Time timeM2(2006, 1, 1, 0, 50, 30);
			TESTASSERT_EQUAL(50, timeM2.Minute());
			timeM2.addMinute(-50);
			TESTASSERT_EQUAL(0, timeM2.Minute());
			timeM2.addMinute(-1);
			TESTASSERT_EQUAL(59, timeM2.Minute());
			TESTASSERT_EQUAL(23, timeM2.Hour());
			TESTASSERT_EQUAL(31, timeM2.Day());
			TESTASSERT_EQUAL(12, timeM2.Month());
			TESTASSERT_EQUAL(2005, timeM2.Year());

			Time timeS(2006, 12, 31, 23, 59, 30);
			TESTASSERT_EQUAL(30, timeS.Second());
			timeS.addSecond(29);
			TESTASSERT_EQUAL(59, timeS.Second());
			timeS.addSecond(1);
			TESTASSERT_EQUAL(0, timeS.Second());
			TESTASSERT_EQUAL(0, timeS.Minute());
			TESTASSERT_EQUAL(0, timeS.Hour());
			TESTASSERT_EQUAL(1, timeS.Day());
			TESTASSERT_EQUAL(1, timeS.Month());
			TESTASSERT_EQUAL(2007, timeS.Year());

			Time timeS2(2006, 1, 1, 0, 0, 30);
			TESTASSERT_EQUAL(30, timeS2.Second());
			timeS2.addSecond(-30);
			TESTASSERT_EQUAL(0, timeS2.Second());
			timeS2.addSecond(-1);
			TESTASSERT_EQUAL(59, timeS2.Second());
			TESTASSERT_EQUAL(59, timeS2.Minute());
			TESTASSERT_EQUAL(23, timeS2.Hour());
			TESTASSERT_EQUAL(31, timeS2.Day());
			TESTASSERT_EQUAL(12, timeS2.Month());
			TESTASSERT_EQUAL(2005, timeS2.Year());

		}
		TESTCASE_END;
	};
};

} //namespace cc

#endif //#ifndef cc_TimeTest_H_
