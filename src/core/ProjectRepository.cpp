#include "pch.h"
#include "ProjectRepository.h"
#include "Project.h"
#include "utility/IniFile.h"
#include <map>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


static CString GetProjectSettingFilePath()
{
	TCHAR path[MAX_PATH_NTFS];
	GetModuleFileName(nullptr, path, MAX_PATH_NTFS);
	PathRemoveFileSpec(path);
	PathAppend(path, _T("projects.dat"));
	return path;
}

struct ProjectRepository::PImpl
{
	std::vector<Project*> mProjectList;

	uint32_t mNextProjectID;
};

ProjectRepository::ProjectRepository() : in(new PImpl)
{
	in->mNextProjectID = 1;

	Load();
}

ProjectRepository::~ProjectRepository()
{
	for (auto prj : in->mProjectList) {
		delete prj;
	}
}

ProjectRepository* ProjectRepository::Get()
{
	static ProjectRepository inst;
	return &inst;
}

void ProjectRepository::Save()
{
	CIniFile ini;
	ini.Open(GetProjectSettingFilePath());

	ini.Write(_T("Repository"), _T("NextID"), (int)in->mNextProjectID);

	CString sectionName;

	int count = (int)in->mProjectList.size();
	ini.Write(_T("Repository"), _T("ProjectCount"), count);

	for (int i = 0; i < count; ++i) {

		sectionName.Format(_T("Project%d"), i+1);

		auto& prj = in->mProjectList[i];

		// プロジェクトID(内部管理用)
		ini.Write(sectionName, _T("ID"), (int)prj->GetID());

		const auto& data = prj->mData;

		// プロジェクト識別子
		ini.Write(sectionName, _T("Code"), data.mCode);
		// プロジェクト名
		ini.Write(sectionName, _T("DisplayName"), data.mName);

		// 開始日
		ini.Write(sectionName, _T("StartDate"), data.mStartDate);
		// 終了日
		ini.Write(sectionName, _T("EndDate"), data.mEndDate);
		// 説明
		ini.Write(sectionName, _T("Description"), data.mDescription);

		ini.Write(sectionName, _T("IsArchived"), prj->IsArchived() ? 1 : 0);
	}
}

void ProjectRepository::Load()
{
	CIniFile ini;
	ini.Open(GetProjectSettingFilePath());

	in->mNextProjectID = ini.GetInt(_T("Repository"), _T("NextID"), 1);

	CString sectionName;
	CString sectionKey;

	int count = ini.GetInt(_T("Repository"), _T("ProjectCount"), 0);
	for (int i = 0; i < count; ++i) {

		sectionName.Format(_T("Project%d"), i+1);

		// プロジェクトID(内部管理用)
		int id = ini.GetInt(sectionName, _T("ID"), 0);
		if (id == 0) {
			continue;
		}

		ProjectData data;

		// プロジェクト識別子
		data.mCode = ini.GetString(sectionName, _T("Code"), _T(""));
		if (data.mCode.IsEmpty()) {
			continue;
		}

		// プロジェクト名
		data.mName = ini.GetString(sectionName, _T("DisplayName"), _T(""));
		if (data.mName.IsEmpty()) {
			continue;
		}

		// 開始日
		data.mStartDate = ini.GetString(sectionName, _T("StartDate"), _T(""));
		// 終了日
		data.mEndDate = ini.GetString(sectionName, _T("EndDate"), _T(""));
		// 説明
		data.mDescription = ini.GetString(sectionName, _T("Description"), _T(""));

		// アーカイブフラグ
		bool isArchived = ini.GetInt(sectionName, _T("IsArchived"), 0) != 0;

		std::unique_ptr<Project> prj(new Project);
		prj->mID = id;
		prj->mData = data;
		prj->mIsArchived = isArchived;

		in->mProjectList.push_back(prj.release());
	}
}

Project* ProjectRepository::NewProject()
{
	auto prj = new Project;

	prj->mID = in->mNextProjectID++;

	in->mProjectList.push_back(prj);

	return prj;
}

void ProjectRepository::EnumProjects(std::vector<Project*>& projects)
{
	projects.clear();
	projects.insert(projects.end(), in->mProjectList.begin(), in->mProjectList.end());
}

void ProjectRepository::EnumActiveProjects(std::vector<Project*>& projects)
{
	projects.clear();
	for (auto& prj : in->mProjectList) {
		if (prj->IsArchived()) {
			continue;
		}
		projects.push_back(prj);
	}
}

void ProjectRepository::EnumArchivedProjects(std::vector<Project*>& projects)
{
	projects.clear();
	for (auto& prj : in->mProjectList) {
		if (prj->IsArchived() == false) {
			continue;
		}
		projects.push_back(prj);
	}
}

bool ProjectRepository::FindProjectFromCode(const CString& code, Project** prj)
{
	for (auto p : in->mProjectList) {
		if (code != p->GetCode()) {
			continue;
		}
		*prj = p;
		return true;
	}
	return false;
}

bool ProjectRepository::FindProjectFromName(const CString& name, Project** prj)
{
	for (auto p : in->mProjectList) {
		if (name != p->GetName()) {
			continue;
		}
		*prj = p;
		return true;
	}
	return false;
}

