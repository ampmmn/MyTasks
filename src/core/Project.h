#pragma once

#include "core/Theme.h"

class ProjectData
{
public:
	// プロジェクト識別子(UIで表示するためのもの)
	CString mCode;
	// プロジェクト名
	CString mName;
	// 開始日(YYYY/MM/DD)
	CString mStartDate;
	// 終了日(YYYY/MM/DD)
	CString mEndDate;
	// 説明・コメントなど
	CString mDescription;
};

class Project
{
public:
	Project();
	Project(const Project&) = default;
	~Project();

	uint32_t GetID() const;
	const CString& GetCode() const;
	const CString& GetName() const;
	bool IsArchived() const;

	void Archive();
	void Unarchive();

	// プロジェクトID(内部管理用の連番)
	uint32_t mID;
	//
	ProjectData mData;
	//
	bool mIsArchived;
};
