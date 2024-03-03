#pragma once

#include <vector>

// $B5Y6HF|$r07$&$?$a$N%+%l%s%@!<%G!<%?(B
class Calendar
{
public:
	Calendar();
	Calendar(const Calendar& rhs);
	~Calendar();

	Calendar& operator = (const Calendar& rhs) = default;

	bool Load();
	bool Load(const CString& path);
	bool Save();
	bool Save(const CString& path);

	static bool IsValidDateFormat(const CString& str);

	std::vector<CString> mNoOpDays;
};
