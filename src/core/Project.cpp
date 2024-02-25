#include "pch.h"
#include "Project.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

Project::Project() : mIsArchived(false)
{
}

Project::~Project()
{
}

uint32_t Project::GetID() const
{
	return mID;
}

const CString& Project::GetCode() const
{
	return mData.mCode;
}

const CString& Project::GetName() const
{
	return mData.mName;
}

bool Project::IsArchived() const
{
	return mIsArchived;
}

void Project::Archive()
{
	mIsArchived = true;
}

void Project::Unarchive()
{
	mIsArchived = false;
}
