#include "pch.h"
#include "Section.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


Section::Section() :
	mStartHour(0), mStartMinute(0),
	mEndHour(0), mEndMinute(0)
{
}

Section::Section(
		const CString& dispName,
	 	int startHour,
	 	int startMin,
	 	int endHour,
	 	int endMin
) : 
	mDisplayName(dispName),
	mStartHour(startHour), mStartMinute(startMin),
	mEndHour(endHour), mEndMinute(endMin)

{
}

Section::~Section()
{
}

int Section::GetStartHour() const
{
	return mStartHour;
}

int Section::GetStartMinute() const
{
	return mStartMinute;
}

int Section::GetEndHour() const
{
	return mEndHour;
}

int Section::GetEndMinute() const
{
	return mEndMinute;
}

// 指定された時間tmと開始時刻の差を分単位で求める
void Section::GetStartTimespan(CTime tm, int& minutes)
{
	CTime tmStart(tm.GetYear(), tm.GetMonth(), tm.GetDay(), mStartHour, mStartMinute, 0);

	CTimeSpan span = tmStart - tm;
	minutes = (int)span.GetTotalMinutes();
}

void Section::GetEndTimespan(CTime tm, int& minutes)
{
	CTime tmEnd(tm.GetYear(), tm.GetMonth(), tm.GetDay(), mEndHour, mEndMinute, 0);

	CTimeSpan span = tmEnd - tm;
	minutes = (int)span.GetTotalMinutes();
}
	
CString Section::ToStringStartTime()
{
	CString str;
	str.Format(_T("%02d%02d"), mStartHour, mStartMinute);
	return str;
}

CString Section::ToStringEndTime()
{
	CString str;
	str.Format(_T("%02d%02d"), mEndHour, mEndMinute);
	return str;
}

bool Section::SetStartTime(const CString& strExpr)
{
	int timeVal;
	if (ParseStringExpression(strExpr, timeVal) == false) {
		return false;
	}
	mStartHour = timeVal / 100;
	volatile int m = ((timeVal / 100) * 100);
	mStartMinute = timeVal - m;

	return true;
}

bool Section::SetEndTime(const CString& strExpr)
{
	int timeVal;
	if (ParseStringExpression(strExpr, timeVal) == false) {
		return false;
	}
	mEndHour = timeVal / 100;
	volatile int m = ((timeVal / 100) * 100);
	mEndMinute = timeVal - m;

	return true;
}

void Section::SetDisplayName(const CString& name)
{
	mDisplayName = name;
}


const CString& Section::GetDisplayName() const
{
	return mDisplayName;
}

bool Section::ParseStringExpression(const CString& str, int& time)
{
	if (IsValidStringExpression(str) == false) {
		return false;
	}
	static tregex pat(_T("^(\\d\\d)(\\d\\d)"));

	tstring s(str);
	auto hourStr = std::regex_replace(s, pat, _T("$1"));
	auto minuteStr = std::regex_replace(s, pat, _T("$2"));

	int hour;
	_stscanf_s(hourStr.c_str(), _T("%02d"), &hour);
	int minute;
	_stscanf_s(minuteStr.c_str(), _T("%02d"), &minute);

	time = hour * 100 + minute;
	return true;
}

bool Section::IsValidStringExpression(const CString& str)
{
	static tregex pat(_T("^(([01][0-9])|(2[0-4]))[0-5][0-9]"));
	return std::regex_match(tstring(str), pat);
}

