#pragma once

// 時間区分
class Section
{
public:
	Section();
	~Section();
	
public:
	// 区分の表示名
	CString mDisplayName;
	// 開始時刻
	int mStartHour;
	int mStartMinute;
	// 終了時刻
	int mEndHour;
	int mEndMinute;
};

