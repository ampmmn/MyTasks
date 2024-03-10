#pragma once

#include <memory>
#include <vector>

#include "core/Category.h"

class CategoryRepository
{
private:
	CategoryRepository();
	~CategoryRepository();

public:
	static CategoryRepository* Get();

	void Save();
	void Load();

	Category* NewCategory();

	void EnumCategories(std::vector<Category*>& categories);
	void EnumActiveCategories(std::vector<Category*>& categories);
	void EnumArchivedCategories(std::vector<Category*>& categories);

	bool FindCategory(const CString& name, Category** cat);

private:
	struct PImpl;
	std::unique_ptr<PImpl> in;

};

