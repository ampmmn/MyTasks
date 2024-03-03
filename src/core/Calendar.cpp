#include "pch.h"
#include "Calendar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


Calendar::Calendar()
{
}

Calendar::Calendar(const Calendar& rhs) : mNoOpDays(rhs.mNoOpDays)
{
}

Calendar::~Calendar()
{
}

static CString& GetDefaultCalendarFilePath(CString& path)
{
	LPTSTR p = path.GetBuffer(MAX_PATH_NTFS);
	GetModuleFileName(nullptr, p, MAX_PATH_NTFS); 
	PathRemoveFileSpec(p);
	PathAppend(p, _T("calendar.dat"));
	path.ReleaseBuffer();
	return path;
}

bool Calendar::Load()
{
	CString path;
	return Load(GetDefaultCalendarFilePath(path));
}

static void TrimComment(CString& s)
{
	bool inDQ = false;

	int n = s.GetLength();
	for (int i = 0; i < n; ++i) {
		if (inDQ == false && s[i] == _T('#')) {
			s = s.Left(i);
			return;
		}

		if (inDQ == false && s[i] == _T('"')) {
			inDQ = true;
		}
		if (inDQ != true && s[i] == _T('"')) {
			inDQ = false;
		}
	}
}

bool Calendar::Load(const CString& path)
{
	FILE* fpIn = nullptr;
	if (_tfopen_s(&fpIn, path, _T("r,ccs=UTF-8")) != 0) {
		return false;
	}

	std::vector<CString> tmpDays;

	// ファイルを読む
	CStdioFile file(fpIn);

	CString strLine;
	while(file.ReadString(strLine)) {

		TrimComment(strLine);
		strLine.Trim();

		if (strLine.IsEmpty()) {
			continue;
		}

		if (IsValidDateFormat(strLine) == false) {
			continue;
		}
		tmpDays.push_back(strLine);
	}

	mNoOpDays.swap(tmpDays);

	file.Close();
	fclose(fpIn);

	return true;
}

bool Calendar::Save()
{
	CString path;
	return Save(GetDefaultCalendarFilePath(path));
}

bool Calendar::Save(const CString& path)
{
	FILE* fpOut = nullptr;
	try {
		CString filePathTmp = path + _T(".tmp");

		if (_tfopen_s(&fpOut, filePathTmp, _T("w,ccs=UTF-8")) != 0) {
			return false;
		}
		CStdioFile file(fpOut);

		for (auto& day : mNoOpDays) {

			file.WriteString(day);
			file.WriteString(_T("\n"));
		}

		file.Close();
		fclose(fpOut);

		// 最後に一時ファイルを書き戻す
		if (CopyFile(filePathTmp, path, FALSE) == FALSE) {
			return false;
		}

		// 一時ファイルを消す
		DeleteFile(filePathTmp);

		return true;
	}
	catch(CFileException* e) {
		e->Delete();
		fclose(fpOut);
		return false;
	}
}

bool Calendar::IsValidDateFormat(const CString& str)
{
	static tregex pat(_T("^(\\d\\d\\d\\d)/(\\d\\d)/(\\d\\d)$"));
	return std::regex_match(tstring(str), pat) != false;
}

