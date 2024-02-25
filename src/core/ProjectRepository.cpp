#include "pch.h"
#include "ProjectRepository.h"
#include "Project.h"
#include <map>
#include <vector>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


struct ProjectRepository::PImpl
{
	std::vector<Project*> mProjectList;
	std::map<CString, Project*> mProjectMap;

	uint32_t mNextProjectID;
};

ProjectRepository::ProjectRepository() : in(new PImpl)
{
	in->mNextProjectID = 1;
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
	// ToDo: 実装
}

void ProjectRepository::Load()
{
	// ToDo: 実装
	// ToDo: mNextProjectIDの復元
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
	projects.reserve(in->mProjectMap.size());
	projects.insert(projects.end(), in->mProjectList.begin(), in->mProjectList.end());
}

void ProjectRepository::EnumActiveProjects(std::vector<Project*>& projects)
{
	projects.clear();
	projects.reserve(in->mProjectMap.size());
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
	projects.reserve(in->mProjectMap.size());
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

