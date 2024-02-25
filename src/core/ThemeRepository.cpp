#include "pch.h"
#include "ThemeRepository.h"
#include "Project.h"
#include "Theme.h"
#include <vector>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


struct ThemeRepository::PImpl
{
	uint32_t mNextThemeID;

	std::vector<Theme*> mThemeList;
};

ThemeRepository::ThemeRepository() : in(new PImpl)
{
	in->mNextThemeID = 1;
}

ThemeRepository::~ThemeRepository()
{
	for (auto theme : in->mThemeList) {
		delete theme;
	}
}

ThemeRepository* ThemeRepository::Get()
{
	static ThemeRepository inst;
	return &inst;
}

void ThemeRepository::Save()
{
	// ToDo: 実装
}

void ThemeRepository::Load()
{
	// ToDo: 実装
	// ToDo: mNextThemeIDの復元
}

Theme* ThemeRepository::IssueTheme()
{
	auto theme = new Theme;

	theme->mID = in->mNextThemeID++;

	in->mThemeList.push_back(theme);

	return theme;
}


void ThemeRepository::EnumThemes(Project* prj, std::vector<Theme*>& themes)
{
	uint32_t prjID = prj->GetID();

	themes.clear();
	themes.reserve(in->mThemeList.size());
	for (auto& item : in->mThemeList) {
		if (prjID != item->mProjectID) {
			continue;
		}
		themes.push_back(item);
	}
}

