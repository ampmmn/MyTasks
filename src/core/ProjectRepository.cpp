#include "pch.h"
#include "ProjectRepository.h"
#include "core/Profile.h"
#include "core/Project.h"
#include "utility/DataFile.h"
#include <map>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using Profile = mytasks::core::Profile;

static CString GetProjectSettingFilePath()
{
	return Profile::Get()->GetFilePath(_T("projects.dat"));
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
	DataFile file;
	file.SetFilePath(GetProjectSettingFilePath());

	auto entry = file.NewEntry(_T("Repository"));
	file.Set(entry, _T("NextID"), (int)in->mNextProjectID);

	CString sectionName;

	int count = (int)in->mProjectList.size();
	file.Set(entry, _T("ProjectCount"), count);

	for (int i = 0; i < count; ++i) {

		sectionName.Format(_T("Project%d"), i+1);

		entry = file.NewEntry(sectionName);

		auto& prj = in->mProjectList[i];

		// プロジェクトID(内部管理用)
		file.Set(entry, _T("ID"), (int)prj->GetID());

		const auto& data = prj->mData;

		// プロジェクト識別子
		file.Set(entry, _T("Code"), data.mCode);
		// プロジェクト名
		file.Set(entry, _T("DisplayName"), data.mName);

		// 開始日
		file.Set(entry, _T("StartDate"), data.mStartDate);
		// 終了日
		file.Set(entry, _T("EndDate"), data.mEndDate);
		// 説明
		file.Set(entry, _T("Description"), data.mDescription);

		file.Set(entry, _T("IsArchived"), prj->IsArchived() ? 1 : 0);
	}

	file.Save();
}

void ProjectRepository::Load()
{
	DataFile file;
	file.SetFilePath(GetProjectSettingFilePath());
	if (file.Load() == false) {
		return;
	}

	auto entry = file.FindEntry(_T("Repository"));
	if (entry == nullptr) {
		return;
	}


	in->mNextProjectID = file.Get(entry, _T("NextID"), 1);

	CString sectionName;
	CString sectionKey;

	int count = file.Get(entry, _T("ProjectCount"), 0);
	for (int i = 0; i < count; ++i) {

		sectionName.Format(_T("Project%d"), i+1);

		entry = file.FindEntry(sectionName);
		if (entry == nullptr) {
			continue;
		}

		// プロジェクトID(内部管理用)
		int id = file.Get(entry, _T("ID"), 0);
		if (id == 0) {
			continue;
		}

		ProjectData data;

		// プロジェクト識別子
		data.mCode = file.Get(entry, _T("Code"), _T(""));
		if (data.mCode.IsEmpty()) {
			continue;
		}

		// プロジェクト名
		data.mName = file.Get(entry, _T("DisplayName"), _T(""));
		if (data.mName.IsEmpty()) {
			continue;
		}

		// 開始日
		data.mStartDate = file.Get(entry, _T("StartDate"), _T(""));
		// 終了日
		data.mEndDate = file.Get(entry, _T("EndDate"), _T(""));
		// 説明
		data.mDescription = file.Get(entry, _T("Description"), _T(""));

		// アーカイブフラグ
		bool isArchived = file.Get(entry, _T("IsArchived"), 0) != 0;

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

