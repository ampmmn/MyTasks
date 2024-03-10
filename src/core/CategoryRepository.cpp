#include "pch.h"
#include "CategoryRepository.h"
#include "core/Profile.h"
#include "Category.h"
#include "utility/DataFile.h"
#include <stdint.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using Profile = mytasks::core::Profile;

static CString GetCategorySettingFilePath()
{
	return Profile::Get()->GetFilePath(_T("category.dat"));
}

struct CategoryRepository::PImpl
{
	std::vector<Category*> mCategoryList;

	uint32_t mNextCategoryID;
};

CategoryRepository::CategoryRepository() : in(new PImpl)
{
	in->mNextCategoryID = 1;

	Load();
}

CategoryRepository::~CategoryRepository()
{
	for (auto cat : in->mCategoryList) {
		delete cat;
	}
}

CategoryRepository* CategoryRepository::Get()
{
	static CategoryRepository inst;
	return &inst;
}

void CategoryRepository::Save()
{
	DataFile file;
	file.SetFilePath(GetCategorySettingFilePath());

	auto entry = file.NewEntry(_T("Repository"));
	file.Set(entry, _T("NextCategoryID"), (int)in->mNextCategoryID);

	CString sectionName;

	int count = (int)in->mCategoryList.size();
	file.Set(entry, _T("CategoryCount"), count);

	for (int i = 0; i < count; ++i) {

		sectionName.Format(_T("Category%d"), i+1);

		entry = file.NewEntry(sectionName);

		auto& cat = in->mCategoryList[i];

		// プロジェクトID(内部管理用)
		file.Set(entry, _T("ID"), (int)cat->GetID());

		const auto& data = cat->mData;

		// 表示名称
		file.Set(entry, _T("DisplayName"), data.mDisplayName);
		// 説明
		file.Set(entry, _T("Description"), data.mDescription);
		// 順序
		file.Set(entry, _T("Order"), data.mOrder);

		file.Set(entry, _T("IsArchived"), cat->IsArchived() ? 1 : 0);
	}

	file.Save();
}

void CategoryRepository::Load()
{
	DataFile file;
	file.SetFilePath(GetCategorySettingFilePath());
	file.Load();

	auto entry = file.FindEntry(_T("Repository"));
	if (entry == nullptr) {
		return;
	}

	in->mNextCategoryID = file.Get(entry, _T("NextCategoryID"), 1);

	CString sectionName;

	int count = file.Get(entry, _T("CategoryCount"), 0);

	for (int i = 0; i < count; ++i) {

		sectionName.Format(_T("Category%d"), i+1);

		entry = file.FindEntry(sectionName);

		// プロジェクトID(内部管理用)
		std::unique_ptr<Category> cat(new Category);

		cat->mID = file.Get(entry, _T("ID"), 0);
		if (cat->mID == 0) {
			continue;
		}

		auto& data = cat->mData;
		// 表示名称
		data.mDisplayName = file.Get(entry, _T("DisplayName"), _T(""));
		// 説明
		data.mDescription = file.Get(entry, _T("Description"), _T(""));
		// 順序
		data.mOrder = file.Get(entry, _T("Order"), 0);

		cat->mIsArchived = file.Get(entry, _T("IsArchived"), false);

		in->mCategoryList.push_back(cat.release());
	}
}

Category* CategoryRepository::NewCategory()
{
	auto cat = new Category;

	cat->mID = in->mNextCategoryID++;

	in->mCategoryList.push_back(cat);

	return cat;
}

void CategoryRepository::EnumCategories(std::vector<Category*>& categories)
{
	categories.clear();
	categories.reserve(in->mCategoryList.size());
	categories.insert(categories.end(), in->mCategoryList.begin(), in->mCategoryList.end());
}

void CategoryRepository::EnumActiveCategories(std::vector<Category*>& categories)
{
	categories.clear();
	categories.reserve(in->mCategoryList.size());
	for (auto& cat : in->mCategoryList) {
		if (cat->IsArchived()) {
			continue;
		}
		categories.push_back(cat);
	}
}

void CategoryRepository::EnumArchivedCategories(std::vector<Category*>& categories)
{
	categories.clear();
	categories.reserve(in->mCategoryList.size());
	for (auto& cat : in->mCategoryList) {
		if (cat->IsArchived() == false) {
			continue;
		}
		categories.push_back(cat);
	}
}

bool CategoryRepository::FindCategory(const CString& name, Category** cat)
{
	for (auto& c : in->mCategoryList) {
		if (name != c->GetDisplayName()) {
			continue;
		}

		*cat = c;
		return true;
	}
	return false;
}


