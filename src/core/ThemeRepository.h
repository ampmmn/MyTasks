#pragma once

#include <memory>

class Project;
class Theme;

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

private:
	struct PImpl;
	std::unique_ptr<PImpl> in;
};
