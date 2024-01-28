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

	// 指定された時間tmと開始時刻の差を分単位で求める
	void GetStartTimespan(CTime tm, int& minutes);
	void GetEndTimespan(CTime tm, int& minutes);

	CString ToStringStartTime();
	CString ToStringEndTime();

	bool SetStartTime(const CString& strExpr);
	bool SetEndTime(const CString& strExpr);

	void SetDisplayName(const CString& name);
	const CString& GetDisplayName() const;

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

