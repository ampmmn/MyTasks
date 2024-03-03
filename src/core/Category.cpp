#include "pch.h"
#include "Category.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


Category::Category() : mIsArchived(false)
{
}

Category::~Category()
{
}

uint32_t Category::GetID() const
{
	return mID;
}

const CString& Category::GetDisplayName() const
{
	return mData.mDisplayName;
}

const CString& Category::GetDescription() const
{
	return mData.mDescription;
}

void Category::Archive()
{
	mIsArchived = true;
}

bool Category::IsArchived() const
{
	return mIsArchived;
}


