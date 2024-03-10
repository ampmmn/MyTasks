#include "pch.h"
#include "ThemeRepository.h"
#include "core/Profile.h"
#include "core/Project.h"
#include "core/Theme.h"
#include "utility/DataFile.h"
#include <vector>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using Profile = mytasks::core::Profile;

static CString GetSettingFilePath()
{
	return Profile::Get()->GetFilePath(_T("themes.dat"));
}


struct ThemeRepository::PImpl
{
	uint32_t mNextThemeID;

	std::vector<Theme*> mThemeList;
};

ThemeRepository::ThemeRepository() : in(new PImpl)
{
	in->mNextThemeID = 1;
	Load();
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
	DataFile file;
	file.SetFilePath(GetSettingFilePath());

	auto entry = file.NewEntry(_T("Repository"));
	file.Set(entry, _T("NextID"), (int)in->mNextThemeID);

	CString sectionName;

	int count = (int)in->mThemeList.size();
	file.Set(entry, _T("ThemeCount"), count);

	for (int i = 0; i < count; ++i) {

		sectionName.Format(_T("Theme%d"), i+1);

		entry = file.NewEntry(sectionName);

		auto& theme = in->mThemeList[i];

		// テーマID(内部管理用)
		file.Set(entry, _T("ID"), (int)theme->GetID());
		// プロジェクトID
		file.Set(entry, _T("ProjectID"), (int)theme->GetProjectID());

		const auto& data = theme->mData;

		// テーマ識別子
		file.Set(entry, _T("Code"), data.mCode);
		// テーマ名
		file.Set(entry, _T("Subject"), data.mSubject);
		// 説明
		file.Set(entry, _T("Description"), data.mDescription);

		file.Set(entry, _T("IsArchived"), theme->IsArchived() ? 1 : 0);
	}

	file.Save();
}

void ThemeRepository::Load()
{
	DataFile file;
	file.SetFilePath(GetSettingFilePath());
	if (file.Load() == false) {
		return;
	}

	auto entry = file.FindEntry(_T("Repository"));
	if (entry == nullptr) {
		return;
	}

	in->mNextThemeID = file.Get(entry, _T("NextID"), 1);

	CString sectionName;
	CString sectionKey;

	int count = file.Get(entry, _T("ThemeCount"), 0);
	for (int i = 0; i < count; ++i) {

		sectionName.Format(_T("Theme%d"), i+1);

		entry = file.FindEntry(sectionName);
		if (entry == nullptr) {
			continue;
		}

		// テーマID(内部管理用)
		int id = file.Get(entry, _T("ID"), 0);
		if (id == 0) {
			continue;
		}
		int projectId = file.Get(entry, _T("ProjectID"), 0);
		// 0は未分類

		ThemeData data;

		// テーマコード
		data.mCode = file.Get(entry, _T("Code"), _T(""));
		if (data.mCode.IsEmpty()) {
			continue;
		}

		// プロジェクト名
		data.mSubject = file.Get(entry, _T("Subject"), _T(""));
		if (data.mSubject.IsEmpty()) {
			continue;
		}
		// 説明
		data.mDescription = file.Get(entry, _T("Description"), _T(""));

		// アーカイブフラグ
		bool isArchived = file.Get(entry, _T("IsArchived"), 0) != 0;

		std::unique_ptr<Theme> theme(new Theme);
		theme->mID = id;
		theme->mProjectID = projectId;
		theme->mData = data;
		theme->mIsArchived = isArchived;

		in->mThemeList.push_back(theme.release());
	}
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
	EnumThemes(prj->GetID(), themes);
}

void ThemeRepository::EnumThemes(uint32_t prjID, std::vector<Theme*>& themes)
{
	themes.clear();
	themes.reserve(in->mThemeList.size());
	for (auto& item : in->mThemeList) {
		if (prjID != item->mProjectID) {
			continue;
		}
		themes.push_back(item);
	}
}

void ThemeRepository::EnumActiveThemes(uint32_t prjID, std::vector<Theme*>& themes)
{
	themes.clear();
	themes.reserve(in->mThemeList.size());
	for (auto& item : in->mThemeList) {
		if (prjID != item->mProjectID) {
			continue;
		}
		if (item->IsArchived()) {
			continue;
		}
		themes.push_back(item);
	}
}

