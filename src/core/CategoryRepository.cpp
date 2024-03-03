#include "pch.h"
#include "CategoryRepository.h"
#include "Category.h"
#include <stdint.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

struct CategoryRepository::PImpl
{
	std::vector<Category*> mCategoryList;

	uint32_t mNextCategoryID;
};

CategoryRepository::CategoryRepository() : in(new PImpl)
{
	in->mNextCategoryID = 1;
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
	// ToDo: 実装
}

void CategoryRepository::Load()
{
	// ToDo: 実装
	// ToDo: mNextCategoryIDの復元
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


