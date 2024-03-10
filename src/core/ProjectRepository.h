#pragma once

#include <memory>
#include <vector>
#include "core/Project.h"

class ProjectRepository
{
private:
	ProjectRepository();
	~ProjectRepository();

public:
	static ProjectRepository* Get();

	void Save();
	void Load();

	Project* NewProject();

	void EnumProjects(std::vector<Project*>& projects);
	void EnumActiveProjects(std::vector<Project*>& projects);
	void EnumArchivedProjects(std::vector<Project*>& projects);

	bool FindProjectFromCode(const CString& code, Project** prj);
	bool FindProjectFromName(const CString& name, Project** prj);

private:
	struct PImpl;
	std::unique_ptr<PImpl> in;
};
