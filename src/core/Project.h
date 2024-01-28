#pragma once

#include "core/Thema.h"

class Project
{
public:
	Project();
	Project(const Project&) = default;
	~Project();


	// プロジェクトID(内部管理用の連番)
	uint32_t mID;
	// プロジェクト識別子(UIで表示するためのもの)
	CString mCode;
	// プロジェクト名
	CString mName;

	// プロジェクトの作業テーマ
	std::vector<Thema> mThemaList;
};
