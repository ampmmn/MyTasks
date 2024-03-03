#pragma once

class CategoryData
{
public:
	// 表示名称
	CString mDisplayName;
	// 説明
	CString mDescription;
	// 順序
	int mOrder;
};

class Category
{
public:
	Category();
	~Category();

	uint32_t GetID() const;
	const CString& GetDisplayName() const;
	const CString& GetDescription() const;

	void Archive();
	bool IsArchived() const;

public:
	// 内部管理用の連番(画面には表示しない)
	uint32_t mID;
	// データ
	CategoryData mData;
	//
	bool mIsArchived;
};

