#pragma once

#include <vector>
#include <memory>

class DataFile
{
public:
	class Entry;

	struct Exception {};

	enum {
		TYPE_INT,
		TYPE_DOUBLE,
		TYPE_STRING,
		TYPE_BOOLEAN,
		TYPE_STREAM,
		TYPE_UNKNOWN,
	};

public:
	DataFile();
	~DataFile();

	void SetFilePath(const CString& filePath);

public:
	int GetEntryCount() const;


	Entry* NewEntry(const CString& name);
	Entry* GetEntry(int index) const;
	Entry* FindEntry(const CString& name) const;

	CString GetName(Entry* entry) const;

	void MarkAsUsed(Entry* entry);
	bool IsUsedEntry(Entry* entry) const;

	bool HasValue(Entry* entry, LPCTSTR key) const;

	int GetValueType(Entry* entry, LPCTSTR key) const;

	int Get(Entry* entry, LPCTSTR key, int defValue) const;
	void Set(Entry* entry, LPCTSTR key, int value);

	double Get(Entry* entry, LPCTSTR key, double defValue) const;
	void Set(Entry* entry, LPCTSTR key, double value);

	CString Get(Entry* entry, LPCTSTR key, LPCTSTR defValue) const;
	void Set(Entry* entry, LPCTSTR key, const CString& value);

	bool Get(Entry* entry, LPCTSTR key, bool defValue) const;
	void Set(Entry* entry, LPCTSTR key, bool value);

	bool Get(Entry* entry, LPCTSTR key, std::vector<uint8_t>& value) const;
	void Set(Entry* entry, LPCTSTR key, const std::vector<uint8_t>& value);

	void ClearEntries();
	bool Load();
	bool Save();

	static void TrimComment(CString& s);

	static CString EscapeString(const CString& s);
	static CString UnescapeString(const CString& s);

protected:
	struct PImpl;
	std::unique_ptr<PImpl> in;
};

