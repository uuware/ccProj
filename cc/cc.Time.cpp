// cc.Time.h
// cc.Time class
// Author: Shu.KK
// 2006-01-20
#include "cc.Time.h"
using namespace cc;

/////////////////////////////////////////////////////////////////////////////
// Time
/////////////////////////////////////////////////////////////////////////////
Time Time::GetCurrentTime()
{
	return Time(::time(NULL));
}

///////////////////// constructors //////////////////////////
Time::Time() : m_time(0)
{
}

Time::Time(__time64_t time) : m_time((time_t)time)
{
}

Time::Time(int nYear, int nMonth, int nDay, int nHour, int nMin, int nSec, int nDST)
{
	struct tm atm;
	atm.tm_sec = nSec;
	atm.tm_min = nMin;
	atm.tm_hour = nHour;
	atm.tm_mday = nDay;
	atm.tm_mon = nMonth - 1;        // tm_mon is 0 based
	atm.tm_year = nYear - 1900;     // tm_year is 1900 based
	atm.tm_isdst = nDST;
	m_time = mktime(&atm);
}

Time::Time(WORD wDosDate, WORD wDosTime, int nDST)
{
	struct tm atm;
	atm.tm_sec = (wDosTime & ~0xFFE0) << 1;
	atm.tm_min = (wDosTime & ~0xF800) >> 5;
	atm.tm_hour = wDosTime >> 11;

	atm.tm_mday = wDosDate & ~0xFFE0;
	atm.tm_mon = ((wDosDate & ~0xFE00) >> 5) - 1;
	atm.tm_year = (wDosDate >> 9) + 80;
	atm.tm_isdst = nDST;
	m_time = mktime(&atm);
}

Time::Time(const SYSTEMTIME& sysTime, int nDST)
{
	if(sysTime.wYear < 1900)
	{
		__time64_t time0 = 0L;
		Time timeT(time0);
		*this = timeT;
	}
	else
	{
		Time timeT(
			(int)sysTime.wYear, (int)sysTime.wMonth, (int)sysTime.wDay,
			(int)sysTime.wHour, (int)sysTime.wMinute, (int)sysTime.wSecond,
			nDST);
		*this = timeT;
	}
}

Time::Time(const FILETIME& fileTime, int nDST)
{
	// first convert file time (UTC time) to local time
	FILETIME localTime;
	if(!FileTimeToLocalFileTime(&fileTime, &localTime))
	{
		m_time = 0;
		return;
	}

	// then convert that time to system time
	SYSTEMTIME sysTime;
	if(!FileTimeToSystemTime(&localTime, &sysTime))
	{
		m_time = 0;
		return;
	}

	// then convert the system time to a time_t (C-runtime local time)
	Time timeT(sysTime, nDST);
	*this = timeT;
}

///////////////////// operator //////////////////////////
Time& Time::operator=(Time time)
{
	m_time = time.m_time;
	return (*this);
}

Time& Time::operator=(__time64_t time)
{
	m_time = (time_t)time;
	return (*this);
}

bool Time::operator==(Time time) const
{
	return (m_time == time.m_time);
}

bool Time::operator!=(Time time) const
{
	return (m_time != time.m_time);
}

bool Time::operator<(Time time) const
{
	return (m_time < time.m_time);
}

bool Time::operator>(Time time) const
{
	return (m_time > time.m_time);
}

bool Time::operator<=(Time time) const
{
	return (m_time <= time.m_time);
}

bool Time::operator>=(Time time) const
{
	return (m_time >= time.m_time);
}

struct tm* Time::GetGmtTm(struct tm* ptm) const
{
	struct tm * ptmTemp;
	if(ptm != NULL)
	{
#ifdef __STDC_SECURE_LIB__
		struct tm ptmTemp1;
		gmtime_s(&ptmTemp1, &m_time);
		ptmTemp = &ptmTemp1;
#else
		ptmTemp = gmtime(&m_time);
#endif
		if(ptmTemp == NULL)
		{
			return NULL;
		}
		*ptm = *ptmTemp;
		return ptm;
	}
	else
	{
#ifdef __STDC_SECURE_LIB__
		struct tm ptmTemp1;
		gmtime_s(&ptmTemp1, &m_time);
		ptmTemp = &ptmTemp1;
		return ptmTemp;
#else
		return gmtime(&m_time);
#endif
	}
}

struct tm* Time::GetLocalTm(struct tm* ptm) const
{
	struct tm* ptmTemp;
	if (ptm != NULL)
	{
#ifdef __STDC_SECURE_LIB__
		struct tm ptmTemp1;
		localtime_s(&ptmTemp1, &m_time);
		ptmTemp = &ptmTemp1;
#else
		ptmTemp = localtime(&m_time);
#endif
		if (ptmTemp == NULL)
		{
			return NULL;    // indicates the m_time was not initialized!
		}
		*ptm = *ptmTemp;
		return ptm;
	}
	else
	{
#ifdef __STDC_SECURE_LIB__
		struct tm ptmTemp1;
		localtime_s(&ptmTemp1, &m_time);
		ptmTemp = &ptmTemp1;
		return ptmTemp;
#else
		return localtime(&m_time);
#endif
	}
}

bool Time::GetAsSystemTime(SYSTEMTIME& timeDest) const
{
	struct tm* ptm = GetLocalTm(NULL);
	if (!ptm)
	{
		return false;
	}

	timeDest.wYear = (WORD) (1900 + ptm->tm_year);
	timeDest.wMonth = (WORD) (1 + ptm->tm_mon);
	timeDest.wDayOfWeek = (WORD) ptm->tm_wday;
	timeDest.wDay = (WORD) ptm->tm_mday;
	timeDest.wHour = (WORD) ptm->tm_hour;
	timeDest.wMinute = (WORD) ptm->tm_min;
	timeDest.wSecond = (WORD) ptm->tm_sec;
	timeDest.wMilliseconds = 0;

	return true;
}

double Time::GetPassedSeconds() const
{
	return difftime(::time(NULL), m_time);
}

double Time::GetPassedSeconds(Time time) const
{
	return difftime(time.m_time, m_time);
}

LONGLONG Time::GetTotalSeconds() const
{
	return (m_time);
}

__time64_t Time::GetTime() const
{
	return( m_time );
}

int Time::GetYear() const
{
	struct tm * ptm;
	ptm = GetLocalTm();
	return ptm ? (ptm->tm_year) + 1900 : 0 ; 
}

int Time::GetMonth() const
{
	struct tm * ptm;
	ptm = GetLocalTm();
	return ptm ? ptm->tm_mon + 1 : 0;
}

int Time::GetDay() const
{
	struct tm * ptm;
	ptm = GetLocalTm();
	return ptm ? ptm->tm_mday : 0 ; 
}

int Time::GetHour() const
{
	struct tm * ptm;
	ptm = GetLocalTm();
	return ptm ? ptm->tm_hour : -1 ; 
}

int Time::GetMinute() const
{
	struct tm * ptm;
	ptm = GetLocalTm();
	return ptm ? ptm->tm_min : -1 ; 
}

int Time::GetSecond() const
{
	struct tm * ptm;
	ptm = GetLocalTm();
	return ptm ? ptm->tm_sec : -1 ;
}

int Time::GetDayOfWeek() const
{
	struct tm * ptm;
	ptm = GetLocalTm();
	return ptm ? ptm->tm_wday + 1 : 0 ;
}

Time& Time::AddYear(int nYear)
{
#ifdef __STDC_SECURE_LIB__
	struct tm ptmTemp1;
	localtime_s(&ptmTemp1, &m_time);
	struct tm* ptmTemp = &ptmTemp1;
#else
	struct tm *ptmTemp = localtime(&m_time);
#endif
	ptmTemp->tm_year += nYear;
	m_time = mktime(ptmTemp);
	return (*this);
}

Time& Time::AddMonth(int nMonth)
{
#ifdef __STDC_SECURE_LIB__
	struct tm ptmTemp1;
	localtime_s(&ptmTemp1, &m_time);
	struct tm* ptmTemp = &ptmTemp1;
#else
	struct tm *ptmTemp = localtime(&m_time);
#endif
	ptmTemp->tm_mon += nMonth;
	m_time = mktime(ptmTemp);
	return (*this);
}

Time& Time::AddDay(int nDay)
{
	m_time += nDay * 24 * 60 * 60;
	return (*this);
}

Time& Time::AddHour(int nHour)
{
	m_time += nHour * 60 * 60;
	return (*this);
}

Time& Time::AddMinute(int nMinute)
{
	m_time += nMinute * 60;
	return (*this);
}

Time& Time::AddSecond(int nSecond)
{
	m_time += nSecond;
	return (*this);
}

Time& Time::Add(int nYear, int nMonth, int nDay, int nHour, int nMin, int nSec)
{
#ifdef __STDC_SECURE_LIB__
	struct tm ptmTemp1;
	localtime_s(&ptmTemp1, &m_time);
	struct tm* ptmTemp = &ptmTemp1;
#else
	struct tm *ptmTemp = localtime(&m_time);
#endif
	ptmTemp->tm_year += nYear;
	ptmTemp->tm_mon += nMonth;
	m_time = mktime(ptmTemp);
	m_time += (((nDay * 24 + nHour) * 60 + nMin) * 60 + nSec);
	return (*this);
}

bool Time::GetIsLeapYear()
{
	int nYear = GetYear();
	return ((nYear & 3) == 0 && (nYear % 100 != 0 || nYear % 400 == 0));
}

#if defined(_ccStr)
cc::Str Time::Format(const TCHAR *pFormat) const
{
	if(pFormat == NULL)
		return pFormat;

	TCHAR szBuffer[maxTimeBufferSize];

#ifdef __STDC_SECURE_LIB__
	struct tm ptmTemp1;
	localtime_s(&ptmTemp1, &m_time);
	struct tm* ptmTemp = &ptmTemp1;
#else
	struct tm* ptmTemp = localtime(&m_time);
#endif
	if (ptmTemp == NULL || !_tcsftime(szBuffer, maxTimeBufferSize, pFormat, ptmTemp))
	{
		szBuffer[0] = '\0';
	}
	return szBuffer;
}

cc::Str Time::FormatGmt(const TCHAR *pFormat) const
{
	if(pFormat == NULL)
		return pFormat;

	TCHAR szBuffer[maxTimeBufferSize];

#ifdef __STDC_SECURE_LIB__
	struct tm ptmTemp1;
	gmtime_s(&ptmTemp1, &m_time);
	struct tm* ptmTemp = &ptmTemp1;
#else
	struct tm* ptmTemp = gmtime(&m_time);
#endif
	if (ptmTemp == NULL || !_tcsftime(szBuffer, maxTimeBufferSize, pFormat, ptmTemp))
	{
		szBuffer[0] = '\0';
	}
	return szBuffer;
}
#endif

///////////////////// destructor //////////////////////////
Time::~Time()
{
	;
}

