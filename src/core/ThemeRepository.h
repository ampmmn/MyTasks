#pragma once

#include <memory>
#include "core/Theme.h"

class Project;

class ThemeRepository
{
public:
	ThemeRepository();
	~ThemeRepository();

public:
	static ThemeRepository* Get();

	void Save();
	void Load();

	Theme* IssueTheme();
	void EnumThemes(Project* prj, std::vector<Theme*>& themes);
	void EnumThemes(uint32_t prjID, std::vector<Theme*>& themes);
	void EnumActiveThemes(uint32_t prjID, std::vector<Theme*>& themes);

private:
	struct PImpl;
	std::unique_ptr<PImpl> in;
};
