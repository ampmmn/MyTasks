#pragma once

#include <stdint.h>

// 時間区分
class Section
{
public:
	Section();
	Section(const CString& dispName, int startHour, int startMin, int endHour, int endMin);
	Section(const Section&) = default;
	~Section();

	int GetStartHour() const;
	int GetStartMinute() const;
	int GetEndHour() const;
	int GetEndMinute() const;

	CString ToStringStartTime();
	CString ToStringEndTime();

	bool SetStartTime(const CString& strExpr);
	bool SetEndTime(const CString& strExpr);

	static bool ParseStringExpression(const CString& str, int& time);

	static bool IsValidStringExpression(const CString& str);
	
public:
	// 区分の表示名
	CString mDisplayName;
	// 開始時刻
	uint8_t mStartHour;
	uint8_t mStartMinute;
	// 終了時刻
	uint8_t mEndHour;
	uint8_t mEndMinute;
};

