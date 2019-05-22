// cc.Time.h
// cc.Time class
// Author: Shu.KK
// 2006-01-20
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef CC_Time_H_
#define CC_Time_H_

#ifdef __BORLANDC__
#include <windows.h>
#endif /* __BORLANDC__ */

#include "cc.h"
#include <windows.h>
#include <string.h>
#include <wchar.h>
#include <tchar.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>

#include "cc.Str.h"

namespace cc
{

class Time : public Object
{
public:
	// constructors
	Time();
	Time(__time64_t time);
	Time(int nYear, int nMonth, int nDay, int nHour, int nMin, int nSec, int nDST = -1);
	Time(WORD wDosDate, WORD wDosTime, int nDST = -1);
	Time(const SYSTEMTIME& sysTime, int nDST = -1);
	Time(const FILETIME& fileTime, int nDST = -1);
	// destructor
	virtual ~Time();

	Time& operator=(Time time);
	Time& operator=(__time64_t time);

	bool operator==(Time time) const;
	bool operator!=(Time time) const;
	bool operator<(Time time) const;
	bool operator>(Time time) const;
	bool operator<=(Time time) const;
	bool operator>=(Time time) const;

	static Time GetCurrentTime();
	struct tm* GetGmtTm(struct tm* ptm = NULL) const;
	struct tm* GetLocalTm(struct tm* ptm = NULL) const;
	bool GetAsSystemTime(SYSTEMTIME& st) const;
	bool GetAsFileTime(FILETIME& ft) const;
	double GetPassedSeconds() const;
	double GetPassedSeconds(Time time) const;
	LONGLONG GetTotalSeconds() const;
	__time64_t GetTime() const;

	int GetYear() const;
	int GetMonth() const;
	int GetDay() const;
	int GetHour() const;
	int GetMinute() const;
	int GetSecond() const;
	int GetDayOfWeek() const;

	Time& AddYear(int nYear);
	Time& AddMonth(int nMonth);
	Time& AddDay(int nDay);
	Time& AddHour(int nHour);
	Time& AddMinute(int nMinute);
	Time& AddSecond(int nSecond);
	//first add year&month,then add others as sec
	Time& Add(int nYear, int nMonth, int nDay, int nHour = 0, int nMin = 0, int nSec = 0);

	bool GetIsLeapYear();
/*
The format argument consists of one or more codes; as in printf,
the formatting codes are preceded by a percent sign (%). Characters
that do not begin with % are copied unchanged to strDest.
The LC_TIME category of the current locale affects the output
formatting of strftime.(For more information on LC_TIME, see setlocale.)
The formatting codes for strftime are listed below:

%a    Abbreviated weekday name
%A    Full weekday name
%b    Abbreviated month name
%B    Full month name
%c    Date and time representation appropriate for locale
%d    Day of month as decimal number (01   31)
%H    Hour in 24-hour format (00   23)
%I    Hour in 12-hour format (01   12)
%j    Day of year as decimal number (001   366)
%m    Month as decimal number (01   12)
%M    Minute as decimal number (00   59)
%p    Current locale's A.M./P.M. indicator for 12-hour clock
%S    Second as decimal number (00   59)
%U    Week of year as decimal number, with Sunday as first day of week (00   53)
%w    Weekday as decimal number (0   6; Sunday is 0)
%W    Week of year as decimal number, with Monday as first day of week (00   53)
%x    Date representation for current locale
%X    Time representation for current locale
%y    Year without century, as decimal number (00   99)
%Y    Year with century, as decimal number
%z, %Z    Either the time-zone name or time zone abbreviation, depending on registry settings; no characters if time zone is unknown
%%    Percent sign

Sample:
	%Y/%m/%d %H:%M:%S => 2010/08/10 15:26:30
*/
#if defined(_ccStr)
	cc::Str Format(const TCHAR *pFormat) const;
	cc::Str FormatGmt(const TCHAR *pFormat) const;
#endif

private:
	time_t m_time;
};

const int maxTimeBufferSize = 128;
} //namespace cc

#endif //#ifndef CC_Time_H_

