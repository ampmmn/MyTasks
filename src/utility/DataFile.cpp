#include "pch.h"
#include "framework.h"
#include "DataFile.h"
#include <wincrypt.h>
#include <map>
#include <set>
#include <regex>

#pragma comment(lib, "crypt32.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static CString EncodeBase64(const std::vector<uint8_t>& stm)
{
	// 長さを調べる
	DWORD dstLen = 0;
	if (stm.size() == 0) {
		return CString();
	}
	CryptBinaryToString( &stm.front(), (int)stm.size(), CRYPT_STRING_BASE64 | CRYPT_STRING_NOCRLF, nullptr, &dstLen);

	// 変換する
	CString dstStr;
	CryptBinaryToString( &stm.front(), (int)stm.size(), CRYPT_STRING_BASE64 | CRYPT_STRING_NOCRLF, dstStr.GetBuffer(dstLen+1), &dstLen );
	dstStr.ReleaseBuffer();

	return dstStr;
}

static std::vector<uint8_t> DecodeBase64(const CString& src)
{
	DWORD dstLen = 0;
	if (CryptStringToBinary( (LPCTSTR)src, src.GetLength(), CRYPT_STRING_BASE64, nullptr, &dstLen, nullptr, nullptr ) == FALSE) {
		return std::vector<uint8_t>();
	}

	std::vector<uint8_t> dst(dstLen);
	CryptStringToBinary( (LPCTSTR)src, src.GetLength(), CRYPT_STRING_BASE64, &dst.front(), &dstLen, nullptr, nullptr);

	return dst;
}

class DataFile::Entry
{
public:
	Entry() : mIsUsed(false)
	{
	}


	CString mName;
	bool mIsUsed;

	std::map<CString, int> mTypeMap;

	std::map<CString, int> mIntMap;
	std::map<CString, CString> mStrMap;
	std::map<CString, bool> mBoolMap;
	std::map<CString, double> mDoubleMap;
	std::map<CString, std::vector<uint8_t> > mStreamMap;
};


struct DataFile::PImpl
{
	// ファイルのパス
	CString mFilePath;

	// エントリのリスト
	std::vector<std::unique_ptr<DataFile::Entry> > mEntries;
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////



DataFile::DataFile() : in(std::make_unique<PImpl>())
{
}

DataFile::~DataFile()
{
	ClearEntries();
}

void DataFile::SetFilePath(const CString& filePath)
{
	in->mFilePath = filePath;
}

void DataFile::TrimComment(CString& s)
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

static void WriteLine(CStdioFile& file, LPTSTR name, LPCTSTR value)
{
	file.WriteString(name);
	file.WriteString(_T("="));
	file.WriteString(value);
	file.WriteString(_T("\n"));
}

static void WriteLine(CStdioFile& file, LPTSTR name, int value)
{
	file.WriteString(name);
	file.WriteString(_T("="));

	CString val;
	val.Format(_T("%d"), value);
	file.WriteString(val);
	file.WriteString(_T("\n"));
}

static void WriteLine(CStdioFile& file, LPTSTR name, double value)
{
	file.WriteString(name);
	file.WriteString(_T("="));

	CString val;
	val.Format(_T("%lg"), value);
	file.WriteString(val);
	file.WriteString(_T("\n"));
}


int DataFile::GetEntryCount() const
{
	return (int)in->mEntries.size();
}

DataFile::Entry* DataFile::NewEntry(
	const CString& name
)
{
	auto entry = std::make_unique<Entry>();
	entry->mName = name;

	auto retPtr = entry.get();
	in->mEntries.push_back(std::move(entry));
	return retPtr;
}


DataFile::Entry*
DataFile::GetEntry(int index) const
{
	ASSERT(0 <= index && index < (int)in->mEntries.size());
	return in->mEntries[index].get();
}

DataFile::Entry* DataFile::FindEntry(const CString& name) const
{
	for (auto& entry :  in->mEntries) {
		if (name != entry->mName) {
			continue;
		}
		return entry.get();
	}
	return nullptr;
}

CString DataFile::GetName(Entry* entry) const
{
	ASSERT(entry);
	return entry->mName;
}

void DataFile::MarkAsUsed(Entry* entry)
{
	ASSERT(entry);
	entry->mIsUsed = true;
}

bool DataFile::IsUsedEntry(Entry* entry) const
{
	ASSERT(entry);
	return entry->mIsUsed;
}

bool DataFile::HasValue(Entry* entry, LPCTSTR key) const
{
	ASSERT(entry);
	return GetValueType(entry, key) != TYPE_UNKNOWN;
}


int DataFile::GetValueType(Entry* entry, LPCTSTR key) const
{
	ASSERT(entry);
	auto itFind = entry->mTypeMap.find(key);
	return itFind == entry->mTypeMap.end() ? TYPE_UNKNOWN : itFind->second;
}


int DataFile::Get(Entry* entry, LPCTSTR key, int defValue) const
{
	ASSERT(entry);
	auto itFind = entry->mIntMap.find(key);
	if (itFind == entry->mIntMap.end()) {
		return defValue;
	}
	return itFind->second;
}

void DataFile::Set(Entry* entry, LPCTSTR key, int value)
{
	ASSERT(entry);
	entry->mIntMap[key] = value;
	entry->mTypeMap[key] = TYPE_INT;
}


double DataFile::Get(Entry* entry, LPCTSTR key, double defValue) const
{
	ASSERT(entry);
	ASSERT(entry);
	auto itFind = entry->mDoubleMap.find(key);
	if (itFind == entry->mDoubleMap.end()) {
		return defValue;
	}
	return itFind->second;
}

void DataFile::Set(Entry* entry, LPCTSTR key, double value)
{
	ASSERT(entry);
	entry->mDoubleMap[key] = value;
	entry->mTypeMap[key] = TYPE_DOUBLE;
}


CString DataFile::Get(Entry* entry, LPCTSTR key, LPCTSTR defValue) const
{
	ASSERT(entry);
	auto itFind = entry->mStrMap.find(key);
	if (itFind == entry->mStrMap.end()) {
		return defValue;
	}
	return itFind->second;
}

void DataFile::Set(Entry* entry, LPCTSTR key, const CString& value)
{
	ASSERT(entry);
	entry->mStrMap[key] = value;
	entry->mTypeMap[key] = TYPE_STRING;
}


bool DataFile::Get(Entry* entry, LPCTSTR key, bool defValue) const
{
	ASSERT(entry);
	auto itFind = entry->mBoolMap.find(key);
	if (itFind == entry->mBoolMap.end()) {
		return defValue;
	}
	return itFind->second;
}

void DataFile::Set(Entry* entry, LPCTSTR key, bool value)
{
	ASSERT(entry);
	entry->mBoolMap[key] = value;
	entry->mTypeMap[key] = TYPE_BOOLEAN;
}

bool DataFile::Get(Entry* entry, LPCTSTR key, std::vector<uint8_t>& value) const
{
	ASSERT(entry);
	auto itFind = entry->mStreamMap.find(key);
	if (itFind == entry->mStreamMap.end()) {
		return false;
	}

	value = itFind->second;
	return true;
}

void DataFile::Set(Entry* entry, LPCTSTR key, const std::vector<uint8_t>& value)
{
	ASSERT(entry);
	entry->mStreamMap[key] = value;
	entry->mTypeMap[key] = TYPE_STREAM;
}

void DataFile::ClearEntries()
{
	in->mEntries.clear();
}

bool DataFile::Load()
{
	FILE* fpIn = nullptr;
	if (_tfopen_s(&fpIn, in->mFilePath, _T("r,ccs=UTF-8")) != 0) {
		return false;
	}
	// ファイルを読む
	CStdioFile file(fpIn);

	std::unique_ptr<Entry> curEntry;

	std::vector<std::unique_ptr<Entry> > entries;

	tregex regInt(_T("^ *-?[0-9]+ *$"));
	tregex regDouble(_T("^ *-?[0-9]+\\.[0-9]+ *$"));

	CString strLine;
	while(file.ReadString(strLine)) {

		TrimComment(strLine);
		strLine.Trim();

		if (strLine.IsEmpty()) {
			continue;
		}

		if (strLine[0] == _T('[')) {

			if (curEntry != nullptr) {
				entries.push_back(std::move(curEntry));
			}

			CString strCurSectionName = strLine.Mid(1, strLine.GetLength()-2);

			curEntry = std::make_unique<Entry>();
			curEntry->mName =strCurSectionName;
			continue;
		}

		int n = strLine.Find(_T('='));
		if (n == -1) {
			continue;
		}

		if (curEntry == nullptr) {
			continue;
		}

		CString strKey = strLine.Left(n);
		strKey.Trim();

		CString strValue = strLine.Mid(n+1);
		strValue.Trim();
		tstring pat(strValue);

		if (strValue== _T("true")) {
			curEntry->mBoolMap[strKey] = true;
			curEntry->mTypeMap[strKey] = TYPE_BOOLEAN;
		}
		else if (strValue== _T("false") || strValue== _T("second")) {  // second:初期実装時のバグのリカバーのための処理
			curEntry->mBoolMap[strKey] = false;
			curEntry->mTypeMap[strKey] = TYPE_BOOLEAN;
		}
		else if (strValue.Left(7) == _T("stream:")) {
			curEntry->mStreamMap[strKey] = DecodeBase64(strValue.Mid(7));
			curEntry->mTypeMap[strKey] = TYPE_STREAM;
		}
		else if (std::regex_match(pat, regDouble)) {
			double value;
			_stscanf_s(strValue, _T("%lg"), &value);
			curEntry->mDoubleMap[strKey] = value;
			curEntry->mTypeMap[strKey] = TYPE_DOUBLE;
		}
		else if (std::regex_match(pat, regInt)) {
			int value;
			_stscanf_s(strValue, _T("%d"), &value);
			curEntry->mIntMap[strKey] = value;
			curEntry->mTypeMap[strKey] = TYPE_INT;
		}
		else {
			if (strValue.Left(1) == _T('"') && strValue.Right(1) == _T('"')) {
				strValue = strValue.Mid(1, strValue.GetLength()-2);
			}
			else if (strValue.Left(1) == _T('\'') && strValue.Right(1) == _T('\'')) {
				strValue = strValue.Mid(1, strValue.GetLength()-2);
			}
			curEntry->mStrMap[strKey] = UnescapeString(strValue);
			curEntry->mTypeMap[strKey] = TYPE_STRING;
		}
	}

	if (curEntry) {
		entries.push_back(std::move(curEntry));
	}

	file.Close();
	fclose(fpIn);

	in->mEntries.swap(entries);
	return true;
}


bool DataFile::Save()
{
	FILE* fpOut = nullptr;
	try {
		CString filePathTmp = in->mFilePath + _T(".tmp");

		if (_tfopen_s(&fpOut, filePathTmp, _T("w,ccs=UTF-8")) != 0) {
			return false;
		}
		CStdioFile file(fpOut);

		for (auto& entry :  in->mEntries) {

			file.WriteString(_T("["));
			file.WriteString(entry->mName);
			file.WriteString(_T("]\n"));

			for (auto& kv : entry->mIntMap) {
				file.WriteString(kv.first);
				file.WriteString(_T("="));

				TCHAR val[256];
				_stprintf_s(val, _T("%d"), kv.second);
				file.WriteString(val);
				file.WriteString(_T("\n"));
			}
			for (auto& kv : entry->mDoubleMap) {
				file.WriteString(kv.first);
				file.WriteString(_T("="));

				TCHAR val[256];
				_stprintf_s(val, _T("%lg"), kv.second);
				file.WriteString(val);
				file.WriteString(_T("\n"));
			}
			for (auto& kv : entry->mStrMap) {
				file.WriteString(kv.first);
				file.WriteString(_T("=\""));
				file.WriteString(EscapeString(kv.second));
				file.WriteString(_T("\"\n"));
			}
			for (auto& kv : entry->mBoolMap) {
				file.WriteString(kv.first);
				file.WriteString(_T("="));
				file.WriteString(kv.second ? _T("true") : _T("false"));
				file.WriteString(_T("\n"));
			}
			for (auto& kv : entry->mStreamMap) {
				file.WriteString(kv.first);
				file.WriteString(_T("=stream:"));
				file.WriteString(EncodeBase64(kv.second));
				file.WriteString(_T("\n"));
			}

			file.WriteString(_T("\n"));
		}

		file.Close();
		fclose(fpOut);

		// 最後に一時ファイルを書き戻す
		if (CopyFile(filePathTmp, in->mFilePath, FALSE) == FALSE) {
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

CString DataFile::EscapeString(const CString& s)
{
	CString ret(s);
	ret.Replace(_T("\r"), _T("%0D"));
	ret.Replace(_T("\n"), _T("%0A"));
	return ret;
}


CString DataFile::UnescapeString(const CString& s)
{
	CString ret(s);
	ret.Replace(_T("%0D"), _T("\r"));
	ret.Replace(_T("%0A"), _T("\n"));
	return ret;
}

